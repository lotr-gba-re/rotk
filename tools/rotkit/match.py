"""Compile a candidate C reimplementation with rotkit.compile (the build's recipe) and diff it
against the ROM:

    uv run rotkit match <FunctionName> path/to/candidate.c [--full]

On a diff, the display is sliced to the named function and normalises away address slip: a
size change in an earlier function shifts every later symbol. The candidate and original
slices are disassembled at their own VMAs, branch and `ldr [pc]` targets are replaced by
symbol names or function-relative offsets, other external addresses are opaque, and difflib
Myers alignment localises inserts and deletes. If the named function's own instructions
match while the file still differs, the tool names the functions that diverge.
"""

import difflib
import functools
import os
import re
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from typing import Annotated

import typer

from rotkit.compile import compile_file, run_tool
from rotkit.paths import ROOT, TOOLS
from rotkit.rom import ROMBASE, load_rom
from rotkit.stores import data_symbols, func_symbols

CONTEXT_ROWS = 8  # default diff window, starting 3 rows before the first divergence

_run = functools.partial(run_tool, prefix="match")


def _readelf_syms(elf: str) -> subprocess.CompletedProcess[str]:
    return _run("arm-none-eabi-readelf", "-sW", elf)


def disasm(path: str, base: int) -> list[str]:
    result = _run(
        "arm-none-eabi-objdump",
        "-D",
        "-b",
        "binary",
        "-m",
        "arm",
        "-M",
        "force-thumb",
        "--no-show-raw-insn",
        "--adjust-vma",
        hex(base),
        path,
    )
    return [
        line for line in result.stdout.splitlines() if re.match(r"\s+[0-9a-f]+:", line)
    ]


def _fmt(line: str) -> str:
    """Collapse objdump whitespace so the two columns line up."""
    return re.sub(r"\s+", " ", line.strip())


def _mnemonic(fmt_line: str) -> str:
    """Instruction text with the leading `addr:` stripped."""
    parts = fmt_line.split(None, 1)
    return parts[1] if len(parts) > 1 else ""


@dataclass(frozen=True)
class ElfFunc:
    """One FUNC symbol of the linked candidate ELF."""

    addr: int  # thumb bit already masked
    size: int
    name: str


def _elf_func_symbols(elf: str) -> list[ElfFunc]:
    """readelf FUNC symbols of the linked candidate ELF."""
    result = _readelf_syms(elf)
    out = []
    for line in result.stdout.splitlines():
        cols = line.split()
        if len(cols) < 8 or cols[3] != "FUNC":
            continue
        try:
            out.append(ElfFunc(int(cols[1], 16) & ~1, int(cols[2]), cols[-1]))
        except ValueError:
            continue
    return out


def _elf_symmap(elf: str) -> dict[int, str]:
    """Candidate ELF addr -> name (TU funcs at their candidate VMAs, externals/data at true
    store VMAs via the .set stubs). $t/$d mapping symbols are excluded - they mark code/data, not
    branch targets, and would mis-resolve literal-pool `ldr` addresses."""
    result = _readelf_syms(elf)
    out: dict[int, str] = {}
    for line in result.stdout.splitlines():
        cols = line.split()
        if len(cols) < 8 or cols[-1] in ("$t", "$d"):
            continue
        try:
            out[int(cols[1], 16) & ~1] = cols[-1]
        except ValueError:
            continue
    return out


def _normalize(mnemonic: str, fstart: int, fend: int, smap: dict[int, str]) -> str:
    """Normalise a mnemonic's addresses so equivalent instructions compare equal across slip.

    Each `0x..` token is: a named symbol (function/data) -> `<name>`; an address inside this
    function (loops, own literal pool) -> `+0x..` (relative, slip-invariant); a small immediate
    (`#0x80`) -> left as-is; any other external address -> `<?>` (opaque: a literal misdecoded as
    a branch, or pad bytes - slip-dependent under raw-binary disasm with no $d mapping).
    """
    THRESHOLD = 0x1000

    def repl(match: re.Match[str]) -> str:
        addr = int(match.group(0), 16)
        if addr <= THRESHOLD:
            return match.group(0)
        name = smap.get(addr & ~1)
        if name:
            return f"<{name}>"
        if fstart <= addr < fend:
            return f"+0x{addr - fstart:x}"
        return "<?>"

    return re.sub(r"0x[0-9a-fA-F]+", repl, mnemonic)


def _sliced_lines(
    data: bytes,
    base: int,
    fstart: int,
    fend: int,
    smap: dict[int, str],
    tmp: str,
    tag: str,
) -> list[tuple[str, str]]:
    """Disassemble `data` at `base` -> [(normalised_key, display_line)] for one function's slice."""
    path = os.path.join(tmp, f"{tag}_{base:08x}.bin")
    with open(path, "wb") as fh:
        fh.write(data)
    out = []
    for line in disasm(path, base):
        display = _fmt(line)
        out.append((_normalize(_mnemonic(display), fstart, fend, smap), display))
    return out


def _myers_rows(
    orig: list[tuple[str, str]], cand: list[tuple[str, str]]
) -> list[tuple[str, str, str]]:
    """Myers-align original vs candidate on the normalised key -> (mark, orig_disp, cand_disp)."""
    sm = difflib.SequenceMatcher(
        None, [k for k, _ in orig], [k for k, _ in cand], autojunk=False
    )
    rows: list[tuple[str, str, str]] = []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == "equal":
            for k in range(i2 - i1):
                rows.append((" ", orig[i1 + k][1], cand[j1 + k][1]))
        elif tag == "replace":
            for k in range(max(i2 - i1, j2 - j1)):
                rows.append(
                    (
                        "*",
                        orig[i1 + k][1] if i1 + k < i2 else "",
                        cand[j1 + k][1] if j1 + k < j2 else "",
                    )
                )
        elif tag == "delete":  # in original, not candidate
            for k in range(i2 - i1):
                rows.append(("*", orig[i1 + k][1], ""))
        elif tag == "insert":  # in candidate, not original
            for k in range(j2 - j1):
                rows.append(("*", "", cand[j1 + k][1]))
    return rows


def _divergent(rows: list[tuple[str, str, str]]) -> int:
    return sum(1 for mark, *_ in rows if mark == "*")


def _emit_diff(
    name: str, addr: int, rows: list[tuple[str, str, str]], full: bool
) -> None:
    """Print the diverging-rows side-by-side (windowed by default, full with --full)."""
    first = next((i for i, r in enumerate(rows) if r[0] == "*"), 0)
    print(f"DIFF   {name}  @ 0x{addr:08x}: {_divergent(rows)} instruction(s) differ")
    print("  --- original ---                       --- agbcc candidate ---")
    if full:
        selected = rows
    else:
        start = max(0, first - 3)
        selected = rows[start : start + CONTEXT_ROWS]
    for mark, orig_disp, cand_disp in selected:
        print(f"{mark} {orig_disp:<40}  {cand_disp}")
    if not full:
        print(
            f"(window shows {CONTEXT_ROWS} rows around the first diff; "
            "re-run with --full for the complete side-by-side, diverging rows marked with *)"
        )
    raise SystemExit(1)


def run(
    function_name: str,
    candidate: str,
    full: Annotated[
        bool,
        typer.Option(
            "--full",
            help="print the full per-function side-by-side with diverging rows marked "
            "(default: 8-row window around the first divergent instruction)",
        ),
    ] = False,
    keep: Annotated[
        str | None,
        typer.Option(
            "--keep",
            help="retain the compile artifacts under DIR/<function>/: the .s with -dp "
            "annotations (insn UIDs, pattern names, branch provenance), the -da per-pass "
            "RTL dumps, the linked ELF, and the f.i.trace-* compiler diagnostics "
            "(auto-enabled). Look up a divergent row's UID in the dumps to "
            "see which pass produced it. Bytes are unaffected.",
            metavar="DIR",
        ),
    ] = None,
    cc: Annotated[
        str,
        typer.Option(
            "--cc",
            help="compiler binary under tools/agbcc/bin: agbcc (default) or old_agbcc "
            "(the object-library second origin; matches ROMs of SDK library code)",
        ),
    ] = "agbcc",
) -> None:
    """Compile the candidate .c with agbcc and byte-diff it against the ROM; on DIFF, show the
    named function's diverging instructions side by side."""
    addr_by_name = {s.name: s.addr for s in func_symbols()}
    true_addr = addr_by_name.get(function_name)
    if true_addr is None:
        sys.exit(f"{function_name} not in config/functions.cfg")
    if not os.path.isfile(candidate):
        sys.exit(f"candidate not found: {candidate}")
    cc_path = str(TOOLS / "agbcc" / "bin" / cc)
    if not os.path.isfile(cc_path):
        sys.exit(f"compiler not found: {cc_path}")

    rom = load_rom()
    with tempfile.TemporaryDirectory() as tmp_dir:
        elf = os.path.join(tmp_dir, "cand.elf")
        keep_dir = os.path.join(keep, function_name) if keep else None
        base, cand_bytes = compile_file(
            candidate, keep_elf=elf, cc=cc_path, keep_dir=keep_dir, werror=False
        )
        if keep_dir:
            print(
                f"KEPT   compile artifacts (-dp .s, -da dumps, ELF, traces) in {keep_dir}"
            )

        # Whole-TU byte verdict - a MATCH means `make verify` is green for this file.
        orig_tu = rom[base - ROMBASE : base - ROMBASE + len(cand_bytes)]
        if cand_bytes == orig_tu:
            print(
                f"MATCH  {function_name}  @ 0x{base:08x}  ({len(cand_bytes)} bytes) "
                f"byte-identical - point split.cfg at {os.path.relpath(candidate, ROOT)}; "
                "`make verify` stays green."
            )
            return

        cand_funcs = _elf_func_symbols(elf)
        cand_by_name = {f.name: f for f in cand_funcs}
        cand_map = _elf_symmap(elf)
        true_map = {s.addr & ~1: s.name for s in func_symbols()}
        for s in data_symbols():
            true_map[s.addr & ~1] = s.name

        if function_name not in cand_by_name:
            # Function not defined in this TU (external ref or wrong file): best-effort whole-TU.
            orig = _sliced_lines(
                orig_tu, base, base, base + len(cand_bytes), true_map, tmp_dir, "o"
            )
            cand = _sliced_lines(
                cand_bytes, base, base, base + len(cand_bytes), cand_map, tmp_dir, "c"
            )
            print(
                f"DIFF   {function_name}  (not defined in {candidate}; showing whole TU)"
            )
            _emit_diff(function_name, base, _myers_rows(orig, cand), full)
            return

        # per-function slice, each side disassembled at its own VMA
        cand_func_sym = cand_by_name[function_name]
        cand_faddr, fsize = cand_func_sym.addr, cand_func_sym.size
        cand_off = cand_faddr - base
        cand_func = cand_bytes[cand_off : cand_off + fsize]
        orig_func = rom[true_addr - ROMBASE : true_addr - ROMBASE + fsize]
        orig = _sliced_lines(
            orig_func, true_addr, true_addr, true_addr + fsize, true_map, tmp_dir, "o"
        )
        cand = _sliced_lines(
            cand_func,
            cand_faddr,
            cand_faddr,
            cand_faddr + fsize,
            cand_map,
            tmp_dir,
            "c",
        )
        rows = _myers_rows(orig, cand)
        if _divergent(rows):
            _emit_diff(function_name, true_addr, rows, full)

        # The named function's instructions match; locate the function(s) that actually
        # diverge so the user knows where to look next.
        culprits = []
        for f in cand_funcs:
            if f.name not in addr_by_name:
                continue
            fn_true = addr_by_name[f.name]
            cf = cand_bytes[f.addr - base : f.addr - base + f.size]
            of = rom[fn_true - ROMBASE : fn_true - ROMBASE + f.size]
            frows = _myers_rows(
                _sliced_lines(
                    of,
                    fn_true,
                    fn_true,
                    fn_true + f.size,
                    true_map,
                    tmp_dir,
                    f"o{f.name}",
                ),
                _sliced_lines(
                    cf, f.addr, f.addr, f.addr + f.size, cand_map, tmp_dir, f"c{f.name}"
                ),
            )
            if _divergent(frows):
                culprits.append(f.name)
        print(
            f"OKSRC  {function_name}  @ 0x{true_addr:08x}  source matches ROM; "
            f"the byte diff is address slip from an upstream function."
        )
        if culprits:
            print(f"       divergent function(s): {', '.join(culprits)}")
            print(
                f"       try: uv run rotkit match {culprits[0]} {os.path.relpath(candidate, ROOT)} --full"
            )
        raise SystemExit(1)
