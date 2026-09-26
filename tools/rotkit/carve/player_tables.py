"""Carves player progression tables: XpThresholds (u32[50] @0x080628d0)."""

import os
from struct import unpack_from

from rotkit.carve import (
    TableSymbol,
    doc_comment,
    table_symbols,
    upsert_map,
    write_table,
)
from rotkit.rom import ROMBASE, load_rom

DATA_DIR = "player_tables"
_XP_THRESHOLDS_PATH = f"carved/data/{DATA_DIR}/XpThresholds.c"


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
    width = max(len(f"{value},") for value in values)
    for index, value in enumerate(values):
        lines.append(f"    {f'{value},':<{width}} // [{index:2}]")
    lines.append("};")
    return write_table(os.path.join(DATA_DIR, f"{name}.c"), lines)


def _owned_table(name: str, type_prefix: str) -> TableSymbol:
    """Return one table owned by this carver, failing if its config symbol is not unique."""
    matches = [table for table in table_symbols(type_prefix) if table.name == name]
    if len(matches) != 1:
        raise SystemExit(f"expected exactly one {name} symbol in config/data.cfg")
    return matches[0]


def run() -> None:
    rom = load_rom()
    table = _owned_table("XpThresholds", "u32[")
    src = emit_xp_thresholds(table.name, table.addr, table.count, rom)
    print(f"  carved {src}  ({table.count} entries)")
    upsert_map([(table.addr, src)], owned_files=[_XP_THRESHOLDS_PATH])
