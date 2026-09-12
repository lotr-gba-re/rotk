"""Carves XpThresholds (u32[50] @0x080628d0): the per-level XP requirement scanned by
player_awardXp's level-up loop. Plain u32s, so the table is keyed by symbol name rather
than by element type.
"""

import os
from struct import unpack_from

from rotkit.carve import doc_comment, table_symbols, upsert_map, write_table
from rotkit.rom import ROMBASE, load_rom


def emit_xp_thresholds(name: str, addr: int, count: int, rom: bytes) -> str:
    values = unpack_from(f"<{count}I", rom, addr - ROMBASE)
    lines = [
        '#include "types.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(
            addr,
            [
                "Each level costs 100 * n XP more than the previous one (n = index + 1):",
                "100, +200, +300, ... Closed form: 100 * n * (n + 1) / 2.",
                "",
                "Exception: [13] is 10050, not the expected 10500. This is probably a",
                "typo by the devs.",
            ],
        ),
        f"const u32 {name}[{count}] = {{",
    ]
    width = max(len(f"{v},") for v in values)
    for index, value in enumerate(values):
        lines.append(f"    {f'{value},':<{width}} // [{index:2}]")
    lines.append("};")
    return write_table(os.path.join("player_tables", f"{name}.c"), lines)


def run() -> None:
    rom = load_rom()
    tables = [t for t in table_symbols("u32[") if t.name == "XpThresholds"]
    if len(tables) != 1:
        raise SystemExit("expected one XpThresholds u32[N] symbol in config/data.cfg")
    table = tables[0]
    src = emit_xp_thresholds(table.name, table.addr, table.count, rom)
    print(f"  carved {src}  ({table.count} entries)")
    upsert_map([(table.addr, src)], owned_dirs=["player_tables"])
    print("  updated config/split.cfg. Now run: make verify")
