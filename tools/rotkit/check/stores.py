"""Store consistency lint.

Lints the canonical text stores for internal consistency. The stores are the source of
truth, so a self-contradiction is silent corruption - this catches that class plus
malformed lines, and asserts each decompiled function's module-header prototype matches
its src/c definition:

  config/functions.cfg   address<TAB>mode(arm|thumb)<TAB>name
  config/data.cfg        address<TAB>name<TAB>type
  carved/config/*.cfg    data.cfg rows a carver generates (same columns)

Errors (exit 1):
  - same address, two different names         (in either file)
  - same name, two different addresses        (duplicate symbol)
  - address collision between functions.cfg and data.cfg
  - malformed line (wrong column count, bad address, bad mode)
  - a leaked Ghidra auto-name (FUN_/DAT_/LAB_/...) - means a default crept in
  - a decompiled src/c function with no include/ prototype, or one that disagrees with the .c
  - a decompiled src/c function whose ROM binding drifted: missing @romaddress tag, no
    functions.cfg row (rename drift), or @romaddress disagreeing with functions.cfg
  - a config/split.cfg row naming a source file that is not on disk

Warnings (printed, exit 0):
  - a committed src/c or carved/data .c with no config/split.cfg row
"""

import glob
import os
import re
import sys
from collections.abc import Iterator

from rotkit.paths import CARVED_DATA, INCLUDE, ROOT, SRC_C

from rotkit.decomp import (
    signatures_in_c,
    defined_in_c,
    romaddrs_in_c,
    prototypes_in_header,
)

from rotkit.stores import FUNCS_CFG as FUNCS, SPLIT_CFG as SPLIT
from rotkit.stores import data_store_paths, read_split

ADDR_RE = re.compile(r"^0x[0-9a-fA-F]{8}$")
AUTO_PREFIXES = (
    "FUN_",
    "DAT_",
    "LAB_",
    "SUB_",
    "UNK_",
    "thunk_FUN_",
    "switchD_",
    "caseD_",
)
errors = []
warnings = []


def rows(path: str, arity: int) -> Iterator[tuple[int, list[str]]]:
    """Yield (lineno, fields) for non-comment, non-blank lines; flag bad arity."""
    if not os.path.isfile(path):
        warnings.append(f"{os.path.relpath(path, ROOT)}: missing (not seeded yet?)")
        return
    with open(path) as fh:
        for lineno, line in enumerate(fh, 1):
            s = line.rstrip("\n")
            if not s.strip() or s.lstrip().startswith("#"):
                continue
            cols = s.split("\t")
            if len(cols) != arity:
                errors.append(
                    f"{os.path.relpath(path, ROOT)}:{lineno}: "
                    f"expected {arity} tab-separated columns, got {len(cols)}: {s!r}"
                )
                continue
            yield lineno, cols


def _norm_proto(proto: str) -> str:
    """Normalize a prototype for comparison: drop the trailing ;, collapse whitespace."""
    return " ".join(proto.replace(";", " ").split())


def _decomp_protos() -> dict[str, tuple[str, str]]:
    """name -> (prototype, header path) declared in a hand-written include/*.h."""
    protos = {}
    for path in glob.glob(os.path.join(INCLUDE, "**", "*.h"), recursive=True):
        for name, proto in prototypes_in_header(path).items():
            protos[name] = (proto, path)
    return protos


def _check_decomp(rel) -> int:
    """Every src/c function is declared in an include/*.h header with a prototype matching the
    .c definition. Returns the count checked."""
    sigs = signatures_in_c()  # name -> .c signature (the matched truth)
    headers = _decomp_protos()  # name -> (header proto, file)
    for name, sig in sorted(sigs.items()):
        if name not in headers:
            errors.append(
                f"decompiled {name!r} (src/c) has no hand-written include/ declaration - "
                f"declare it (hand-write the prototype in a subsystem header)"
            )
            continue
        header_proto, header_file = headers[name]
        if _norm_proto(header_proto) != _norm_proto(sig):
            errors.append(
                f"{rel(header_file)}: prototype for {name!r} disagrees with src/c - "
                f"header {_norm_proto(header_proto)!r} vs .c {_norm_proto(sig)!r}"
            )
    return len(sigs)


def _check_romaddr_binding(name_to_addr: dict[str, tuple[str, str]]) -> None:
    """Every src/c definition carries an @romaddress tag, has a functions.cfg row, and the two
    addresses agree, so a rename that misses functions.cfg cannot silently unbind it."""
    romaddrs = romaddrs_in_c()
    for name in sorted(defined_in_c()):
        tagged = romaddrs.get(name)
        if tagged is None:
            errors.append(
                f"decompiled {name!r} (src/c) has no @romaddress tag in its Doxygen plate"
            )
        cfg = name_to_addr.get(name)
        if cfg is None:
            errors.append(
                f"decompiled {name!r} (src/c) has no config/functions.cfg row - "
                f"rename drift? (the name must also be updated there)"
            )
        elif tagged is not None and int(cfg[0], 16) != tagged:
            errors.append(
                f"{name!r}: @romaddress 0x{tagged:08x} disagrees with functions.cfg "
                f"{cfg[0]} ({cfg[1]})"
            )


def _check_split_coverage(rel) -> int:
    """A committed .c with no split.cfg row is neither compiled nor byte-compared: a warning,
    as it is usually work in progress. A row naming a missing file is an error. Returns the
    number of listed sources."""
    listed = set(read_split())
    on_disk = {
        path.relative_to(ROOT).as_posix()
        for root in (SRC_C, CARVED_DATA)
        for path in root.rglob("*.c")
    }
    for src in sorted(on_disk - listed):
        warnings.append(
            f"{src}: no config/split.cfg row - not compiled, not byte-compared by `make verify`"
        )
    for src in sorted(listed - on_disk):
        errors.append(f"{rel(SPLIT)}: row names {src!r}, which is not on disk")
    return len(listed)


def run() -> None:
    def rel(p):
        return os.path.relpath(p, ROOT)

    addr_to_name = {}  # addr -> (name, source file:line)
    name_to_addr = {}  # name -> (addr, source file:line)

    def register(addr, name, where):
        if addr in addr_to_name and addr_to_name[addr][0] != name:
            other_name, other_where = addr_to_name[addr]
            errors.append(
                f"address {addr}: two names - {name!r} ({where}) vs {other_name!r} ({other_where})"
            )
        else:
            addr_to_name.setdefault(addr, (name, where))
        if name in name_to_addr and name_to_addr[name][0] != addr:
            other_addr, other_where = name_to_addr[name]
            errors.append(
                f"name {name!r}: two addresses - {addr} ({where}) vs {other_addr} ({other_where})"
            )
        else:
            name_to_addr.setdefault(name, (addr, where))

    n_funcs = n_data = 0
    # functions.cfg: addr, mode, name
    for lineno, (addr, mode, name) in rows(FUNCS, 3):
        where = f"{rel(FUNCS)}:{lineno}"
        if not ADDR_RE.match(addr):
            errors.append(f"{where}: bad address {addr!r}")
            continue
        if mode not in ("arm", "thumb"):
            errors.append(f"{where}: bad mode {mode!r} (want arm|thumb)")
        if name.startswith(AUTO_PREFIXES):
            errors.append(f"{where}: leaked Ghidra auto-name {name!r}")
        register(addr.lower(), name, where)
        n_funcs += 1

    # data.cfg + carved/config/*.cfg: addr, name, type
    for store in data_store_paths():
        for lineno, (addr, name, _type) in rows(store, 3):
            where = f"{rel(store)}:{lineno}"
            if not ADDR_RE.match(addr):
                errors.append(f"{where}: bad address {addr!r}")
                continue
            if name.startswith(AUTO_PREFIXES):
                errors.append(f"{where}: leaked Ghidra auto-name {name!r}")
            register(addr.lower(), name, where)
            n_data += 1

    n_decomp = _check_decomp(rel)
    _check_romaddr_binding(name_to_addr)
    n_split = _check_split_coverage(rel)

    for w in warnings:
        print(f"  warn: {w}")
    if errors:
        for e in errors:
            print(f"  ERROR: {e}")
        print(
            f"\ncheck-stores: FAIL - {len(errors)} error(s) [{n_funcs} funcs, {n_data} data]"
        )
        sys.exit(1)
    print(
        f"check-stores: OK - {n_funcs} functions, {n_data} data symbols, "
        f"{n_decomp} decompiled (each declared in a hand-written include/ header), "
        f"{n_split} sources in the split manifest, no conflicts"
    )
