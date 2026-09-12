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

Compiler helpers the modcode gets from the host `-lgcc` (`__aeabi_uidiv`, its
`__udivsi3` alias, `_call_via_rN`, ...) are internal to it and not exported; some
share a name with the ROM's own libgcc rows in functions.cfg, and armips rejects a
second equate for the same name. Any other modcode symbol named like a store symbol
is an error: it shadows the ROM function for C callers while asm would still reach
the ROM, so it must be renamed.
"""

import os
import subprocess

from rotkit.stores import data_symbols, func_symbols


def _is_libgcc(name: str, in_stores: set[str]) -> bool:
    """EABI helpers and veneers, plus the host copies of the ROM's own libgcc rows."""
    return name.startswith(("__aeabi_", "_call_via_")) or (
        name.startswith("__") and name in in_stores
    )


def _store_names() -> set[str]:
    """Names game_symbols.inc already equates."""
    return {s.name for s in func_symbols()} | {s.name for s in data_symbols()}


def run(elf: str, out: str) -> None:
    nm = os.environ.get("NM", "arm-none-eabi-nm")
    result = subprocess.run([nm, elf], capture_output=True, text=True, check=True)
    in_stores = _store_names()

    syms: list[tuple[str, int]] = []
    skipped: list[str] = []
    clashes: list[str] = []
    for line in result.stdout.splitlines():
        parts = line.split()
        if len(parts) != 3:  # undefined ('U', no address) or malformed -> skip
            continue
        addr, typ, name = parts
        if typ not in "TDBR":  # global + defined in a section (exclude A/U/locals/weak)
            continue
        if _is_libgcc(name, in_stores):
            skipped.append(name)
        elif name in in_stores:
            clashes.append(name)
        else:
            syms.append((name, int(addr, 16)))

    if clashes:
        raise SystemExit(
            "modcode-syms: modcode symbol(s) named like a game symbol, rename them: "
            + ", ".join(sorted(clashes))
        )

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
    note = (
        f" (libgcc internals not exported: {', '.join(sorted(skipped))})"
        if skipped
        else ""
    )
    print(f"wrote {os.path.relpath(out)}: {len(syms)} modcode symbol(s){note}")
