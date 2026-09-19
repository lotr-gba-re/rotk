"""Carves the player tables: XpThresholds (u32[50] @0x080628d0) and the passive skill tables,
PassiveSkills (PassiveSkill[35] @0x0806e3d4) and PassiveSkillRequiredLevels (s8[35][6]
@0x082874bc).
"""

import os
from struct import unpack_from

from rotkit.cheaders import extract_enum, extract_struct_fields, invert_enum
from rotkit.carve import (
    TableSymbol,
    doc_comment,
    table_symbols,
    upsert_map,
    write_table,
)
from rotkit.rom import ROMBASE, load_rom

# PassiveSkill is three PassiveSkillStatRecord (statIndex u8, a padding byte, valuePerLevel
# u16[5]) followed by tier, initialRequiredLevel and maxLevel. PassiveSkillRequiredLevels has one column
# per skill level. The two size constants are sizeof() of those structs: `make verify` checks the
# carved table against the ROM, so a layout drift shows up as a diff.
_STAT_COUNT = 3
_RECORD_SIZE = 0xC
_SKILL_SIZE = 0x28
_SKILL_LEVELS = 6


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


def _skill_label(skill_ids: dict[int, str], index: int) -> str:
    return skill_ids.get(index, f"PASSIVE_SKILL_{index}").removeprefix("PASSIVE_SKILL_")


def emit_passive_skills(name: str, addr: int, count: int, rom: bytes) -> str:
    f_records, f_tier, f_req, f_max = extract_struct_fields(
        "include/skill.h", "PassiveSkill"
    )
    r_stat, r_values = extract_struct_fields(
        "include/skill.h", "PassiveSkillStatRecord"
    )
    stats_enum = extract_enum("include/stats.h", "StatIndex")
    stats = invert_enum(stats_enum)
    stat_none = stats_enum["STAT_NONE"]
    skill_ids = invert_enum(extract_enum("include/skill.h", "PassiveSkillId"))
    tiers = invert_enum(extract_enum("include/skill.h", "PassiveSkillTier"))
    base = addr - ROMBASE
    lines = [
        '#include "types.h"',
        '#include "stats.h"',
        '#include "skill.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(
            addr,
            [
                "One row per PassiveSkillId. STAT_NONE marks an unused stat record.",
            ],
        ),
        f"const PassiveSkill {name}[{count}] = {{",
    ]
    for i in range(count):
        row = rom[base + i * _SKILL_SIZE : base + (i + 1) * _SKILL_SIZE]
        lines.append(f"    // [{i}] {_skill_label(skill_ids, i)}")
        lines.append(f"    {{ .{f_records} = {{")
        for record in range(_STAT_COUNT):
            offset = record * _RECORD_SIZE
            stat_index = row[offset]
            if stat_index == stat_none:
                lines.append(f"             {{ .{r_stat} = STAT_NONE }},")
                continue
            values = unpack_from("<5H", row, offset + 2)
            stat = stats.get(stat_index, f"STAT_UNKNOWN_{stat_index}")
            joined = ", ".join(str(v) for v in values)
            lines.append(
                f"             {{ .{r_stat} = {stat}, .{r_values} = {{ {joined} }} }},"
            )
        lines += [
            "         },",
            f"      .{f_tier} = {tiers.get(row[0x24], row[0x24])},",
            f"      .{f_req} = {row[0x25]},",
            f"      .{f_max} = {row[0x26]} }},",
            "",
        ]
    lines[-1] = "};"
    return write_table(os.path.join("player_tables", f"{name}.c"), lines)


def _owned_table(name: str, type_prefix: str) -> TableSymbol:
    """Return one table owned by this carver, failing if its config symbol is not unique."""
    matches = [table for table in table_symbols(type_prefix) if table.name == name]
    if len(matches) != 1:
        raise SystemExit(f"expected exactly one {name} symbol in config/data.cfg")
    return matches[0]


def emit_required_levels(name: str, addr: int, count: int, rom: bytes) -> str:
    skill_ids = invert_enum(extract_enum("include/skill.h", "PassiveSkillId"))
    base = addr - ROMBASE
    lines = [
        '#include "types.h"',
        '#include "skill.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(
            addr,
            [
                "Hero level a player needs before buying each level of a skill: row = skill id,",
                "column = level bought so far, -1 once maxLevel is reached.",
            ],
        ),
        f"const s8 {name}[{count}][{_SKILL_LEVELS}] = {{",
    ]
    width = len(str(count - 1))
    for i in range(count):
        values = unpack_from(f"<{_SKILL_LEVELS}b", rom, base + i * _SKILL_LEVELS)
        joined = ", ".join(f"{v:2}" for v in values)
        lines.append(
            f"    {{ {joined} }}, // [{i:>{width}}] {_skill_label(skill_ids, i)}"
        )
    lines.append("};")
    return write_table(os.path.join("player_tables", f"{name}.c"), lines)


def run() -> None:
    rom = load_rom()
    entries = []

    table = _owned_table("XpThresholds", "u32[")
    src = emit_xp_thresholds(table.name, table.addr, table.count, rom)
    print(f"  carved {src}  ({table.count} entries)")
    entries.append((table.addr, src))

    table = _owned_table("PassiveSkills", "PassiveSkill[")
    src = emit_passive_skills(table.name, table.addr, table.count, rom)
    print(f"  carved {src}  ({table.count} skills)")
    entries.append((table.addr, src))

    table = _owned_table("PassiveSkillRequiredLevels", "s8[")
    src = emit_required_levels(table.name, table.addr, table.count, rom)
    print(f"  carved {src}")
    entries.append((table.addr, src))

    upsert_map(entries, owned_dirs=["player_tables"])
    print("  updated config/split.cfg. Now run: make verify")
