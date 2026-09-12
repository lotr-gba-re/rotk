"""Carves RegionMissionRanges (include/mission.h): the mission-id ranges per region that
mission_getRegion scans, emitted as enum MissionId members with the decoded region names.
"""

import functools
import os
from struct import Struct

from rotkit.cheaders import extract_enum, extract_struct_fields, invert_enum
from rotkit.carve import doc_comment, table_symbols, upsert_map, write_table
from rotkit.rom import ROMBASE, load_rom
from rotkit.textdb import decode_strings

# Layout the carver owns (struct body lives in include/mission.h).
_REGION_MISSION_RANGE = Struct("<H H")  # RegionMissionRange: loMission, hiMission

# Decoded string ids: region name = region id + 0x6b (TEXT_ID_ANDUIN_RIVER, see
# TEXT_ID_REGION_NAME_BASE in include/text.h).
_REGION_NAME_TEXT_BASE = 0x6B


@functools.lru_cache(maxsize=1)
def region_annotations(rom: bytes) -> list[str]:
    """Per region id: 'REGION_DWARF_HALLS "Dwarf Halls"' (enum RegionId name + decoded string).

    Cached: one full text-package decode serves every table in a carve run."""
    names = invert_enum(extract_enum("include/mission.h", "RegionId"))
    strings = decode_strings(rom)
    return [
        f'{names[i]} "{strings.get(_REGION_NAME_TEXT_BASE + i, f"0x{_REGION_NAME_TEXT_BASE + i:04x}")}"'
        for i in sorted(names)
    ]


def emit_region_mission_ranges(name: str, addr: int, count: int, rom: bytes) -> str:
    """Range bounds are enum MissionId members, middle missions one comment line each."""
    f_lo, f_hi = extract_struct_fields("include/mission.h", "RegionMissionRange")
    missions = invert_enum(extract_enum("include/mission.h", "MissionId"))
    regions = region_annotations(rom)
    base_off = addr - ROMBASE
    rows = [
        _REGION_MISSION_RANGE.unpack_from(rom, base_off + i * 4) for i in range(count)
    ]
    for lo, hi in rows:
        if lo > hi:
            raise SystemExit(f"{name}: {f_lo} 0x{lo:04x} > {f_hi} 0x{hi:04x}")
        if any(m not in missions for m in range(lo, hi + 1)):
            raise SystemExit(
                f"{name}: 0x{lo:02x}..0x{hi:02x} not covered by enum MissionId"
            )
    if len(regions) != count:
        raise SystemExit(f"enum RegionId does not cover 0..{count - 1}")

    lines = ['#include "mission.h"', "", "// clang-format off", ""]
    lines += [
        *doc_comment(addr),
        f"const RegionMissionRange {name}[{count}] = {{",
    ]
    for i, (lo, hi) in enumerate(rows):
        lines.append(f"    // [0x{i:02x}] {regions[i]}")
        if lo == hi:
            lines.append(
                f"    {{ .{f_lo} = {missions[lo]}, .{f_hi} = {missions[hi]} }},"
            )
        else:
            lines.append(f"    {{ .{f_lo} = {missions[lo]},")
            for m in range(lo + 1, hi):
                lines.append(f"      // {missions[m]}")
            lines.append(f"      .{f_hi} = {missions[hi]} }},")
        lines.append("")
    lines[-1] = "};"
    return write_table(os.path.join("mission_tables", f"{name}.c"), lines)


def run() -> None:
    rom = load_rom()
    carved = []
    for table in table_symbols("RegionMissionRange["):
        carved.append(
            (
                table.addr,
                emit_region_mission_ranges(table.name, table.addr, table.count, rom),
            )
        )
        print(
            f"  carved carved/data/mission_tables/{table.name}.c  ({table.count} entries)"
        )
    upsert_map(carved, owned_dirs=["mission_tables"])
    print(
        f"  carved {len(carved)} tables; updated config/split.cfg. Now run: make verify"
    )
