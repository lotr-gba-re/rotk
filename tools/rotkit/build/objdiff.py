"""Build the objdiff object pair for one matched src/c/**/*.c file.

objdiff diffs two ELF objects symbol-by-symbol; this splice-style repo has neither a target
object tree nor per-function build objects, so both are synthesized from what exists:

  base   (your build)    <- rotkit.compile builds src/c/<Name>.c to a linked .text ELF (keep_elf).
                            Carries ARM mapping symbols ($t code / $d literal pools) and
                            per-function FUNC symbols. Laid out sequentially from the first
                            function's VMA, so a size regression in one function slips every later
                            function's symbols + $t/$d by that many bytes.
  target (original ROM)  <- a fresh .text ELF built from the true symbol table (per-function VMAs
                            from config/functions.cfg, sizes = next true addr - addr) with the ROM
                            bytes spliced in, plus $t/$d RELOCATED from base.elf by per-function
                            slip (true_addr(sym) = cand_addr(sym) + true_func - cand_func).

The target's symbol and mapping layout is the true one regardless of candidate slip, so a
byte-identical function reads 100% even when an upstream function has slipped; only the
defective function shows a diff.

Writes build/objdiff/<stem>/{base.elf,target.elf} (stem = path relative to src/c, no .c).
objdiff's build command regenerates them on save; validated headlessly by objdiff-cli.
"""

import bisect
import functools
import os
from dataclasses import dataclass

from rotkit.paths import BUILD, ROOT, SRC_C
from rotkit.stores import data_symbols, func_symbols

from rotkit.compile import compile_file, run_tool
from rotkit.rom import ROMBASE, load_rom


OUT = BUILD / "objdiff"

_run = functools.partial(run_tool, prefix="objdiff")


def text_section(elf: str) -> tuple[int, int]:
    """(vma, size) of the .text PROGBITS section in a linked ELF."""
    for line in _run("arm-none-eabi-readelf", "-SW", elf).stdout.splitlines():
        cols = line.split()
        if "PROGBITS" in cols and ".text" in cols:
            idx = cols.index("PROGBITS")
            return int(cols[idx + 1], 16), int(cols[idx + 3], 16)
    raise SystemExit(f"objdiff: no .text section in {elf}")


@dataclass(frozen=True)
class ElfSym:
    """One base.elf symbol: FUNC, $t/$d mapping symbol, or global external."""

    addr: int  # FUNC addrs carry the THUMB low bit (|1) as emitted by agbcc; mask for math
    name: str
    is_func: bool
    size: int
    bind: str


def _elf_symbols(elf: str) -> list[ElfSym]:
    """readelf -sW symbols: FUNC, $t/$d and the global externals (.set absolutes at true
    store addresses, which let objdiff normalize bl/ldr targets that leave the TU)."""
    out = _run("arm-none-eabi-readelf", "-sW", elf)
    syms: list[ElfSym] = []
    for line in out.stdout.splitlines():
        cols = line.split()
        if len(cols) < 8:
            continue
        try:
            addr = int(cols[1], 16)
        except ValueError:
            continue
        name, typ, bind = cols[-1], cols[3], cols[4]
        size = int(cols[2]) if cols[2].isdigit() else 0
        if typ == "FUNC":
            syms.append(ElfSym(addr, name, True, size, bind))
        elif name in ("$t", "$d"):
            syms.append(ElfSym(addr, name, False, 0, bind))
        elif bind == "GLOBAL":
            syms.append(ElfSym(addr, name, False, size, bind))  # externals + data
    return syms


def build_target(rom: bytes, base_elf: str, outdir: str) -> tuple[str, int, int]:
    """Fresh target ELF: ROM bytes plus the true symbol and mapping table. FUNC labels sit at
    the store VMAs with the candidate's per-function sizes (slip-invariant); $t/$d are
    base.elf's mapping symbols relocated by per-function slip. Built as a padded stub,
    spliced with ROM bytes via --update-section, then $t/$d and externals injected via
    --add-symbol.
    """
    store = {s.name: s.addr for s in func_symbols()}
    known = store.keys() | {s.name for s in data_symbols()}
    all_symbols = _elf_symbols(base_elf)
    tu = [s for s in all_symbols if s.is_func]
    true_addr = {s.name: store[s.name] for s in tu}
    cand_size = {s.name: s.size for s in tu}
    funcs = sorted(tu, key=lambda s: true_addr[s.name])
    vma = true_addr[funcs[0].name]
    # span end = the first store symbol past the last TU function, else base.elf's section end
    max_true = max(true_addr.values())
    span_end = next((s.addr for s in func_symbols() if s.addr > max_true), None)
    if span_end is None:
        _, bsize = text_section(base_elf)
        span_end = vma + bsize
    span = span_end - vma

    romtext = os.path.join(outdir, "rom_text.bin")
    open(romtext, "wb").write(rom[vma - ROMBASE : vma - ROMBASE + span])

    stub = os.path.join(outdir, "target_stub.s")
    lines = [".thumb", '.section .text, "ax"']
    loc = 0
    for f in funcs:
        off = true_addr[f.name] - vma
        if off > loc:
            lines.append(f".org {off}")
        lines += [
            f".global {f.name}",
            f".type {f.name}, %function",
            f"{f.name}:",
            f".size {f.name}, {cand_size[f.name]}",
        ]
        loc = off
    if span > loc:
        lines.append(f".org {span}")
    open(stub, "w").write("\n".join(lines) + "\n")

    stub_o = os.path.join(outdir, "target_stub.o")
    target = os.path.join(outdir, "target.elf")
    tmp = target + ".tmp"
    _run("arm-none-eabi-as", "-mcpu=arm7tdmi", stub, "-o", stub_o)
    _run("arm-none-eabi-ld", "-Ttext", hex(vma), "-e", hex(vma), stub_o, "-o", target)
    _run(
        "arm-none-eabi-objcopy",
        "--update-section",
        f".text={romtext}",
        target,
        tmp,
    )
    os.replace(tmp, target)

    # a mapping symbol's slip is its host function's: the largest candidate start <= its addr
    cand_starts = sorted((s.addr & ~1, s.name) for s in tu)
    starts = [c[0] for c in cand_starts]
    add_args: list[str] = []
    for s in all_symbols:
        if s.name in ("$t", "$d"):
            idx = bisect.bisect_right(starts, s.addr) - 1  # largest func start <= addr
            if idx < 0:
                continue
            cand_func_addr, host_name = cand_starts[idx]
            off = s.addr + (true_addr[host_name] - cand_func_addr) - vma
            if 0 <= off <= span:
                add_args += ["--add-symbol", f"{s.name}=.text:{off},local"]
        elif not s.is_func and s.bind == "GLOBAL" and s.name in known:
            add_args += ["--add-symbol", f"{s.name}=0x{s.addr:08x},global"]
    if add_args:
        _run("arm-none-eabi-objcopy", target, *add_args, tmp)
        os.replace(tmp, target)

    return target, vma, span


def run(cfile: str) -> None:
    stem = os.path.splitext(os.path.relpath(os.path.abspath(cfile), SRC_C))[0]
    outdir = os.path.join(OUT, stem)
    os.makedirs(outdir, exist_ok=True)

    base_elf = os.path.join(outdir, "base.elf")
    _base, _data = compile_file(cfile, keep_elf=base_elf, werror=False)
    target_elf, vma, size = build_target(load_rom(), base_elf, outdir)
    print(f"  objdiff objects for {stem}: .text 0x{size:x} @ 0x{vma:08x}")
    print(
        f"    target {os.path.relpath(target_elf, ROOT)}   base {os.path.relpath(base_elf, ROOT)}"
    )
