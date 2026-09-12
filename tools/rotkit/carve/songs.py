"""Carves Songs (SongEntry[] @0x082898f0): the song table sfx_requestSong plays from.

Each entry pairs a pointer to the driver song data with the sfx_driverPlaySong
flags/variant args. The song data blobs are unmapped, so the pointers are emitted as
absolute casts; a re-carve picks up names once they land in config/data.cfg.
"""

import os
from struct import Struct

from rotkit.carve import data_symbol, doc_comment, upsert_map, write_table
from rotkit.cheaders import extract_enum, extract_struct_fields, invert_enum
from rotkit.rom import ROMBASE, load_rom

# SongEntry (0x8): data pointer, flags byte, variant byte, 2 bytes tail pad.
_ENTRY = Struct("<IBBH")


def emit_songs(rom: bytes) -> tuple[int, str]:
    table = data_symbol("SongEntry[")
    f_data, f_flags, f_variant = extract_struct_fields("include/sfx.h", "SongEntry")
    song_ids = invert_enum(extract_enum("include/sfx.h", "SongId"))
    base_off = table.addr - ROMBASE
    rows = [
        _ENTRY.unpack_from(rom, base_off + i * _ENTRY.size) for i in range(table.count)
    ]

    lines = [
        '#include "sfx.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(table.addr),
        f"const SongEntry {table.name}[{table.count}] = {{",
    ]
    for i, (data, flags, variant, pad) in enumerate(rows):
        if pad != 0:
            raise SystemExit(
                f"Songs[{i}]: tail pad is 0x{pad:04x}, not zero; the SongEntry "
                "layout is wrong"
            )
        lines += [
            f"    // [{i}] {song_ids[i]}",
            f"    {{ .{f_data} = (const void *)0x{data:08x}, "
            f".{f_flags} = 0x{flags:02x}, .{f_variant} = 0x{variant:02x} }},",
        ]
    lines.append("};")
    return table.addr, write_table(os.path.join("sfx_tables", f"{table.name}.c"), lines)


def run() -> None:
    rom = load_rom()
    addr, src = emit_songs(rom)
    print(f"  carved {src}")
    upsert_map([(addr, src)], owned_dirs=["sfx_tables"])
    print("  updated config/split.cfg. Now run: make verify")
