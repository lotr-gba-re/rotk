"""Carves SceneHandlers (SceneEntry[26] @0x0806c828): the per-scene enter/tick/exit handler
triple indexed by g_SceneCurrent.id.

Each slot is a thumb function pointer whose low bit marks thumb mode. A handler mapped in
config/functions.cfg is emitted as a named reference (the link step resolves it); an
unmapped slot stays an absolute (void *)0xADDR cast literal, and a zero slot is NULL.
"""

import os
import re
from struct import Struct

from rotkit.cheaders import extract_struct_fields
from rotkit.carve import data_symbol, doc_comment, upsert_map, write_table
from rotkit.rom import ROMBASE, load_rom
from rotkit.stores import func_symbols

# SceneEntry (0xc): enter, tick, exit - three thumb function pointers.
_ENTRY = Struct("<3I")


def _handler_init(value: int, names_by_addr: dict[int, str]) -> str:
    """NULL, the functions.cfg name at the address (thumb bit masked), or an absolute
    cast."""
    if value == 0:
        return "NULL"
    name = names_by_addr.get(value & ~1)
    return name if name is not None else f"(void *)0x{value:08x}"


def emit_scene_handlers(rom: bytes, names_by_addr: dict[int, str]) -> tuple[int, str]:
    table = data_symbol("SceneEntry[")
    f_enter, f_tick, f_exit = extract_struct_fields("include/scene.h", "SceneEntry")
    base_off = table.addr - ROMBASE
    rows = [
        _ENTRY.unpack_from(rom, base_off + i * _ENTRY.size) for i in range(table.count)
    ]

    inits = [tuple(_handler_init(v, names_by_addr) for v in row) for row in rows]
    # Each scene declares its own handlers in scene/<Name>.h, so the include list follows
    # whatever the table actually references.
    scene_headers = sorted(
        {
            f'#include "scene/{m.group(1)}.h"'
            for row in inits
            for init in row
            if (m := re.match(r"scene_([a-z][A-Za-z0-9]*)_", init))
        }
    )
    lines = [
        '#include "scene.h"',
        *scene_headers,
        "",
        "// clang-format off",
        "",
        *doc_comment(table.addr),
        f"const SceneEntry {table.name}[{table.count}] = {{",
    ]
    for i, (enter, tick, exit_) in enumerate(inits):
        lines += [
            f"    // [0x{i:02x}]",
            f"    {{ .{f_enter} = {enter},",
            f"      .{f_tick} = {tick},",
            f"      .{f_exit} = {exit_} }},",
            "",
        ]
    lines[-1] = "};"
    return table.addr, write_table(os.path.join("scene", f"{table.name}.c"), lines)


def run() -> None:
    rom = load_rom()
    names_by_addr = {s.addr & ~1: s.name for s in func_symbols()}
    addr, src = emit_scene_handlers(rom, names_by_addr)
    print(f"  carved {src}")
    upsert_map([(addr, src)], owned_dirs=["scene"])
    print("  updated config/split.cfg. Now run: make verify")
