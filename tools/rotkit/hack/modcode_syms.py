"""Modcode ELF -> armips equates for its entry symbols.

Reads the GLOBAL defined symbols (the functions/data the modcode exposes) from a
linked modcode ELF via `arm-none-eabi-nm`, and writes armips equates so the
umbrella hack.asm can `bl`/reference each C entry by name at its real linked address:

    menu_text_run                     equ 0x08ffcd00

The linked addresses come straight from the ELF, so an asm hook can call any
exported C function by name.

Only GLOBAL, section-defined symbols are exported (nm type T/D/B/R) - so locals
(e.g. `static` globals) and the absolute game PROVIDEs from game_symbols.ld are
left out. Run by the Makefile modcode rule.
"""

import os
import subprocess


def run(elf: str, out: str) -> None:
    nm = os.environ.get("NM", "arm-none-eabi-nm")
    result = subprocess.run([nm, elf], capture_output=True, text=True, check=True)

    syms = []
    for line in result.stdout.splitlines():
        parts = line.split()
        if len(parts) != 3:  # undefined ('U', no address) or malformed -> skip
            continue
        addr, typ, name = parts
        if typ in "TDBR":  # global + defined in a section (exclude A/U/locals/weak)
            syms.append((name, int(addr, 16)))

    syms.sort(key=lambda item: item[1])
    with open(out, "w") as fh:
        fh.write(
            f"; GENERATED from {os.path.basename(elf)} by rotkit hack modcode-syms - do not edit.\n"
        )
        fh.write(
            "; armips equates for the modcode's exported (global) symbols at their linked\n"
        )
        fh.write(
            "; addresses. .include this in hacks/hack.asm to call C entries by name.\n\n"
        )
        for name, addr in syms:
            fh.write(f"{name:<32} equ 0x{addr:08x}\n")
    print(f"wrote {os.path.relpath(out)}: {len(syms)} modcode symbol(s)")
