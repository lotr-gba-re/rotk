"""The compile recipe: build a matched C file into exact ROM bytes with the period compiler.

A C file may hold one function or a contiguous run of functions (a translation unit, e.g.
combat.c) - calls between functions in the file resolve at link time; only calls leaving the
file need external symbols. The file maps to one split.cfg region at its first function's
address.

Recipe: preprocess (host cpp, include/ only) -> agbcc -O2 -> as -> link at the first
function's VMA with store-absolute stubs for externals -> objcopy .text -> trim to the last
function's end. Shared by `rotkit build cc` (the Makefile's split/<stem>.bin rule),
`rotkit match`, `rotkit build objdiff`, and `rotkit build debug`.

agbcc: tools/agbcc/bin/agbcc (build via tools/setup_agbcc.sh) or $AGBCC.
"""

import os
import shutil
import subprocess
import sys
import tempfile

from rotkit.paths import BUILD_INCLUDE, CARVED_INCLUDE, INCLUDE, TOOLS
from rotkit.stores import data_symbols, func_symbols


def run_tool(
    *args: str,
    prefix: str = "compile",
    cwd: str | None = None,
    env: dict[str, str] | None = None,
) -> subprocess.CompletedProcess[str]:
    """Run a toolchain command, failing loudly."""
    result = subprocess.run(args, capture_output=True, text=True, cwd=cwd, env=env)
    if result.returncode:
        raise SystemExit(f"{prefix}: {args[0]} failed:\n{result.stdout}{result.stderr}")
    # relay warnings (non-fatal without -Werror); quiet tools print nothing
    if result.stderr:
        sys.stderr.write(result.stderr)
    return result


def _stores() -> dict[str, tuple[int, bool]]:
    """name -> (addr, is_thumb_func)."""
    out: dict[str, tuple[int, bool]] = {}
    for f in func_symbols():
        out[f.name] = (f.addr, f.mode == "thumb")
    for d in data_symbols():
        out[d.name] = (d.addr, False)
    return out


def compile_file(
    cfile: str,
    keep_elf: str | None = None,
    debug: bool = False,
    cc: str | None = None,
    keep_dir: str | None = None,
    werror: bool = True,
) -> tuple[int, bytes]:
    """-> (base_addr, bytes) for the whole file (its first function's addr; bytes through the last).

    keep_elf: if set, copy the fully-linked .text ELF (symbols at their real VMAs, THUMB,
    externals resolved to store absolutes) to this path before the tempdir is cleaned. Used by
    the objdiff object-pair generator, which diffs it against an ELF of the original ROM bytes.
    debug: add -g (DWARF: args/locals/line info) - used by `rotkit build debug` for its gdb
    bundle; codegen is unaffected, so the returned bytes stay match-exact.
    cc: compiler binary to use instead of the default (e.g. tools/agbcc/bin/old_agbcc for the
    object-library second origin).
    keep_dir: if set, compile with -dp -da (asm comments + per-pass RTL dumps; comments and
    dumps only, bytes unchanged), enable all AGBCC_TRACE_* compiler diagnostics, and copy the
    .s, the linked ELF, the dump files, and the per-trace f.i.trace-* files here.
    werror: pass -Werror (the verify gate). Iterative tools (match, objdiff) pass False so
    in-progress code with warnings still compiles; warnings are relayed to stderr either way.
    """
    stores = _stores()
    agbcc = cc or os.environ.get("AGBCC", str(TOOLS / "agbcc" / "bin" / "agbcc"))
    if not os.path.isfile(agbcc):
        raise SystemExit(
            f"compile: agbcc not found at {agbcc} - run tools/setup_agbcc.sh"
        )
    with tempfile.TemporaryDirectory() as tmp_dir:

        def stage(ext):
            return os.path.join(tmp_dir, "f." + ext)

        run_tool(
            "arm-none-eabi-cpp",
            "-nostdinc",
            f"-I{INCLUDE}",
            f"-I{CARVED_INCLUDE}",
            f"-I{BUILD_INCLUDE}",
            # the era libc headers installed with agbcc (stddef.h's size_t matches cc1's
            # builtins; LAST so they never shadow the repo's own headers)
            f"-I{TOOLS / 'agbcc' / 'include'}",
            cfile,
            "-o",
            stage("i"),
        )
        # --keep: enable every compiler diagnostic; the f.i.trace-* files land in cwd
        # and are copied to keep_dir with the -da dumps below
        trace_env = (
            {
                k: "1"
                for k in (
                    "AGBCC_TRACE_ALLOC",
                    "AGBCC_TRACE_LOOP",
                    "AGBCC_TRACE_XJUMP",
                    "AGBCC_TRACE_RELOAD",
                    "AGBCC_TRACE_COMBINE",
                )
            }
            if keep_dir
            else {}
        )
        run_tool(
            agbcc,
            stage("i"),
            "-o",
            stage("s"),
            "-O2",
            "-mthumb-interwork",
            "-Wall",
            "-W",  # gcc 2.95 spelling of -Wextra
            "-Wmissing-prototypes",
            *(["-Werror"] if werror else []),
            *(["-g"] if debug else []),
            *(["-dp", "-da"] if keep_dir else []),
            # dumps (-da) and AGBCC_TRACE_* trace files land in the process cwd
            cwd=tmp_dir,
            env={**os.environ, **trace_env},
        )
        # manual AGBCC_TRACE_* without --keep: relay the trace files to stderr
        if not keep_dir and any(k.startswith("AGBCC_TRACE") for k in os.environ):
            for name in sorted(os.listdir(tmp_dir)):
                if ".trace-" in name:
                    with open(os.path.join(tmp_dir, name)) as fh:
                        sys.stderr.write(fh.read())
        run_tool(
            "arm-none-eabi-as", "-mcpu=arm7tdmi", stage("s"), "-o", stage("o")
        )  # NO -mthumb-interwork

        # functions DEFINED in this file (global text symbols) -> their store addresses
        defined = []
        nm_out = run_tool("arm-none-eabi-nm", stage("o")).stdout.splitlines()
        for line in nm_out:
            cols = line.split()
            if len(cols) == 3 and cols[1] in ("T", "t"):
                defined.append(cols[2])
        addrs = {name: stores[name][0] for name in defined if name in stores}
        if not addrs:
            raise SystemExit(
                f"compile: {os.path.basename(cfile)} defines no function in config/functions.cfg"
            )
        base = min(addrs.values())

        # externals leaving the file -> THUMB function (or data) absolutes (direct bl, no veneer)
        undef = [
            line.split()[-1]
            for line in run_tool(
                "arm-none-eabi-nm", "-u", stage("o")
            ).stdout.splitlines()
            if line.strip()
        ]
        stub = []
        for name in undef:
            if name not in stores:
                raise SystemExit(
                    f"compile: external '{name}' not in stores - name it in config/functions.cfg"
                )
            addr, thumb = stores[name]
            stub.append(
                f".thumb_func\n.global {name}\n.set {name}, 0x{addr | 1:08x}"
                if thumb
                else f".global {name}\n.set {name}, 0x{addr:08x}"
            )
        open(stage("ext.s"), "w").write(".thumb\n" + "\n".join(stub) + "\n")
        run_tool(
            "arm-none-eabi-as", "-mcpu=arm7tdmi", stage("ext.s"), "-o", stage("ext.o")
        )
        run_tool(
            "arm-none-eabi-ld",
            "-Ttext",
            hex(base),
            "-e",
            hex(base),
            stage("o"),
            stage("ext.o"),
            "-o",
            stage("elf"),
        )
        run_tool(
            "arm-none-eabi-objcopy",
            "-O",
            "binary",
            "-j",
            ".text",
            stage("elf"),
            stage("bin"),
        )

        # trim to the last function's end (linked addr + size); trailing pad stays base-ROM remainder
        end = base
        readelf = run_tool(
            "arm-none-eabi-readelf", "-sW", stage("elf")
        ).stdout.splitlines()
        for line in readelf:
            cols = line.split()
            if len(cols) >= 8 and cols[7] in addrs and cols[3] == "FUNC":
                end = max(
                    end, (int(cols[1], 16) & ~1) + int(cols[2])
                )  # mask the thumb |1 bit
        data = open(stage("bin"), "rb").read()[: end - base]
        if keep_elf:
            os.makedirs(os.path.dirname(keep_elf), exist_ok=True)
            shutil.copy(stage("elf"), keep_elf)
        if keep_dir:
            os.makedirs(keep_dir, exist_ok=True)
            shutil.copy(stage("s"), os.path.join(keep_dir, "cand.s"))
            shutil.copy(stage("elf"), os.path.join(keep_dir, "cand.elf"))
            stages = {stage(x) for x in ("i", "s", "o", "bin", "elf", "ext.s", "ext.o")}
            # everything else in the tempdir is an RTL dump (-da) or trace file
            for name in os.listdir(tmp_dir):
                path = os.path.join(tmp_dir, name)
                if path not in stages and os.path.isfile(path):
                    shutil.copy(path, keep_dir)
    return base, data
