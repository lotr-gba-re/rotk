"""Carves NpcSfxSets (NpcSfx[27] @0x0806cddc): the voice/sound quad per NPC kind, indexed by
enum NpcKind and annotated with the kind names.
"""

import os
from struct import Struct

from rotkit.carve import doc_comment, table_symbols, todo_lines, upsert_map, write_table
from rotkit.cheaders import extract_enum, extract_struct_fields, invert_enum
from rotkit.rom import ROMBASE, load_rom

# NpcSfx (0x8 B): four u16 SFX ids.
_SFX = Struct("<4H")


def emit_npc_sfx_table(name: str, addr: int, count: int, rom: bytes) -> str:
    fields = extract_struct_fields("include/actor/npc.h", "NpcSfx")
    kinds = invert_enum(extract_enum("include/actor/npc.h", "NpcKind"))
    base_off = addr - ROMBASE
    lines = ['#include "actor.h"', "", "// clang-format off", ""]
    lines += todo_lines(
        "the unannotated NPC kinds are unmapped - name them in enum NpcKind "
        "(include/actor/npc.h) and re-carve"
    )
    lines += todo_lines(
        "only hitSfx has a known consumer - most rows are one base+offset id quad "
        "(+0, +2, +3, +1); name the other three fields in include/actor/npc.h as "
        "consumers turn up"
    )
    lines += [
        "",
        *doc_comment(addr),
        f"const NpcSfx {name}[{count}] = {{",
    ]
    for i in range(count):
        ids = _SFX.unpack_from(rom, base_off + i * 0x8)
        cells = ", ".join(f".{f} = 0x{v:03x}" for f, v in zip(fields, ids))
        label = kinds.get(i)
        kind = f" {label}" if label else ""
        lines.append(f"    // [0x{i:02x}]{kind}")
        lines.append(f"    {{ {cells} }},")
    lines.append("};")
    return write_table(os.path.join("actor_tables", f"{name}.c"), lines)


def run() -> None:
    rom = load_rom()
    carved = []
    for table in table_symbols("NpcSfx["):
        carved.append(
            (
                table.addr,
                emit_npc_sfx_table(table.name, table.addr, table.count, rom),
            )
        )
        print(
            f"  carved carved/data/actor_tables/{table.name}.c  ({table.count} entries)"
        )
    if not carved:
        raise SystemExit("no NpcSfx[N] symbols in config/data.cfg")
    upsert_map(carved, owned_dirs=["actor_tables"])
    print("  updated config/split.cfg. Now run: make verify")
