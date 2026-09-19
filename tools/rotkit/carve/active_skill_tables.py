"""Carves the active skill tables: the per-hero skill data stream (ActiveSkillHeroSkills,
0x0806e94c), ActiveSkillRequiredLevels (0x0828758e) and
ActiveSkillDescriptionBaseTextIds (0x082874a4).

Per hero the stream holds two adjacent objects in ROM order: its 6 active skills
(ActiveSkills<Hero>, the array Player.activeSkills points at) and its 9 passiveSkillIds
(HeroSkillIds<Hero>, what Player.passiveSkillIds points at). The row arrays need 4-byte alignment,
which is what spaces the heroes 0xfc apart while each hero's data is only 0xf9 bytes. The ROM has
no such gap after the last hero, so the stream ends at 0x0806f129.
"""

import os
from functools import cache
from struct import unpack_from

from rotkit.carve import (
    bit_indices,
    d_init,
    doc_comment,
    flags_d_members,
    upsert_map,
    write_table,
)
from rotkit.cheaders import extract_enum, extract_struct_fields, invert_enum
from rotkit.rom import ROMBASE, load_rom
from rotkit.stores import func_symbols
from rotkit.textdb import decode_strings

# --- layout ------------------------------------------------------------------------------------

HERO_COUNT = 8
HERO_ROWS_ADDR = 0x0806E94C
ROW_SIZE = 0x28
ROW_COUNT = 6
ROWS_SIZE = ROW_COUNT * ROW_SIZE  # 0xf0
VALUE_COUNT = 2
VALUE_SIZE = 0xC
PASSIVE_SKILL_COUNT = 9
OBJECT_SIZE = (
    ROWS_SIZE + PASSIVE_SKILL_COUNT
)  # 0xf9, the bytes each hero actually emits
OBJECT_STRIDE = 0xFC  # 0xf9 rounded up to the row array's 4-byte alignment
HERO_DATA_END = HERO_ROWS_ADDR + 7 * OBJECT_STRIDE + OBJECT_SIZE  # 0x0806f129
REQUIRED_LEVELS_ADDR = 0x0828758E
REQUIRED_LEVELS_ROWS = 6
REQUIRED_LEVELS_COLUMNS = 6
DESCRIPTION_BASE_TEXT_IDS_ADDR = 0x082874A4

# ROM order of the hero blocks. This is not HeroId order.
HEROES_MEMORY_ORDER = [
    "Frodo",
    "Sam",
    "Smeagol",
    "Legolas",
    "Aragorn",
    "Gandalf",
    "Eowyn",
    "Gimli",
]


@cache
def _hero_ids() -> dict[str, int]:
    """HeroId values from include/player.h, loaded when carving."""
    return {
        name.removeprefix("HERO_ID_").title(): value
        for name, value in extract_enum("include/player.h", "HeroId").items()
    }


def _callback_init(value: int, names_by_addr: dict[int, str]) -> str:
    """NULL or the functions.cfg name at the callback address."""
    if value == 0:
        return "NULL"
    name = names_by_addr.get(value & ~1)
    if name is None:
        raise SystemExit(
            f"active skill callback 0x{value:08x} has no config/functions.cfg symbol"
        )
    return name


def _callback_value(rom: bytes, hero: str, row: int) -> int:
    """Active-skill callback pointer stored in one hero row."""
    return unpack_from("<I", rom, hero_rows_addr(hero) - ROMBASE + row * ROW_SIZE)[0]


def _skill_labels(rom: bytes) -> dict[str, list[str]]:
    """English skill names extracted from the complete active skill descriptions."""
    strings = decode_strings(rom)
    hero_ids = _hero_ids()
    labels = {}
    for hero in HEROES_MEMORY_ORDER:
        text_id = unpack_from(
            "<H", rom, DESCRIPTION_BASE_TEXT_IDS_ADDR - ROMBASE + hero_ids[hero] * 2
        )[0]
        labels[hero] = [
            strings[text_id + row].split(" (Level", 1)[0] for row in range(ROW_COUNT)
        ]
    return labels


def _cast_flag_fields() -> tuple[list[str], set[int]]:
    """ActiveSkillCastFlags decoded-view fields and the bits named by its mask enum."""
    d_bits = extract_struct_fields("include/skill.h", "ActiveSkillCastFlags", "d")
    known_bits = bit_indices(
        invert_enum(extract_enum("include/skill.h", "ActiveSkillCastFlag"))
    )
    return d_bits, known_bits


def hero_rows_addr(hero: str) -> int:
    """ROM address of a hero's row array (ROM order, not HeroId order)."""
    return HERO_ROWS_ADDR + HEROES_MEMORY_ORDER.index(hero) * OBJECT_STRIDE


def required_levels_addr(hero: str) -> int:
    """ROM address of a hero's ActiveSkillRequiredLevels block (ROM order)."""
    return REQUIRED_LEVELS_ADDR + HEROES_MEMORY_ORDER.index(hero) * (
        REQUIRED_LEVELS_ROWS * REQUIRED_LEVELS_COLUMNS
    )


def check_arithmetic() -> None:
    """Print the layout arithmetic and fail on any inconsistency."""
    print("  layout arithmetic")
    print(
        f"    row       0x{ROW_SIZE:02x} = 0x04 callback + 0x02 spiritCost"
        " + 0x02 field_0x06"
    )
    print(
        f"               + {VALUE_COUNT} x 0x{VALUE_SIZE:02x} value record"
        " + 0x05 row fields + 0x03 tail padding"
    )
    print(f"    rows      6 x 0x{ROW_SIZE:02x} = 0x{ROWS_SIZE:03x} (0x28 x 6 = 0xf0)")
    print(
        f"    object    0x{ROWS_SIZE:03x} rows + {PASSIVE_SKILL_COUNT} passiveSkillIds"
        f" = 0x{OBJECT_SIZE:03x} per hero"
    )
    print(
        f"    stride    0x{OBJECT_SIZE:03x} rounds up to the row array's 4-byte alignment"
        f" = 0x{OBJECT_STRIDE:03x} (the {OBJECT_STRIDE - OBJECT_SIZE} gap bytes are zero)"
    )
    print(
        f"    heroes    {HERO_COUNT} objects, 0x{HERO_ROWS_ADDR:08x}..0x{HERO_DATA_END - 1:08x}"
        f" ({7 * OBJECT_STRIDE + OBJECT_SIZE} bytes; the last hero has no trailing gap)"
    )
    print(
        f"    required  {HERO_COUNT} x {REQUIRED_LEVELS_ROWS} x {REQUIRED_LEVELS_COLUMNS}"
        f" = {HERO_COUNT * REQUIRED_LEVELS_ROWS * REQUIRED_LEVELS_COLUMNS} bytes,"
        f" 0x{REQUIRED_LEVELS_ADDR:08x}.."
        f"0x{REQUIRED_LEVELS_ADDR + HERO_COUNT * REQUIRED_LEVELS_ROWS * REQUIRED_LEVELS_COLUMNS - 1:08x}"
    )
    assert ROWS_SIZE == 0xF0, "6 rows must end exactly where passiveSkillIds starts"
    assert OBJECT_SIZE == 0xF9 and OBJECT_STRIDE == 0xFC
    assert REQUIRED_LEVELS_ROWS == REQUIRED_LEVELS_COLUMNS == 6, "one column per level"
    assert REQUIRED_LEVELS_ADDR == 0x082874BC + 35 * 6, (
        "ActiveSkillRequiredLevels follows PassiveSkillRequiredLevels"
    )
    assert HERO_DATA_END == 0x0806F129, (
        "the hero stream ends right after HeroSkillIdsGimli, with no trailing padding"
    )
    assert HERO_COUNT == len(_hero_ids()) == len(HEROES_MEMORY_ORDER)


def emit_hero_skills(
    rom: bytes,
    names_by_addr: dict[int, str],
    labels: dict[str, list[str]],
) -> str:
    """The 8 row arrays and the 8 passive-id arrays, in ROM order."""
    passive_ids = invert_enum(extract_enum("include/skill.h", "PassiveSkillId"))
    cast_flag_d_bits, cast_flag_known_bits = _cast_flag_fields()
    lines = [
        '#include "skill.h"',
        '#include "types.h"',
        "",
        "// clang-format off",
    ]
    for hero in HEROES_MEMORY_ORDER:
        base = hero_rows_addr(hero)
        lines += [
            "",
            *doc_comment(base),
            f"const ActiveSkill ActiveSkills{hero}[HERO_ACTIVE_SKILL_COUNT] = {{",
        ]
        for row in range(ROW_COUNT):
            lines += emit_row(
                rom,
                base + row * ROW_SIZE,
                hero,
                row,
                cast_flag_d_bits,
                cast_flag_known_bits,
                names_by_addr,
                labels[hero][row],
            )
        lines.append("};")
        lines.append("")
        ids_addr = base + ROWS_SIZE
        ids = rom[ids_addr - ROMBASE : ids_addr - ROMBASE + PASSIVE_SKILL_COUNT]
        names = [passive_ids.get(value, f"PASSIVE_SKILL_{value}") for value in ids]
        lines += [
            *doc_comment(ids_addr),
            f"const u8 HeroSkillIds{hero}[HERO_PASSIVE_SKILL_COUNT] = {{",
        ]
        for names_chunk in [names[:5], names[5:]]:
            lines.append("    " + ", ".join(names_chunk) + ",")
        lines.append("};")
    return "\n".join(lines) + "\n"


def emit_row(
    rom: bytes,
    addr: int,
    hero: str,
    row: int,
    cast_flag_d_bits: list[str],
    cast_flag_known_bits: set[int],
    names_by_addr: dict[int, str],
    label: str,
) -> list[str]:
    """One ActiveSkill initializer."""
    off = addr - ROMBASE
    callback = unpack_from("<I", rom, off)[0]
    spirit_cost = unpack_from("<H", rom, off + 0x04)[0]
    field_0x06 = unpack_from("<H", rom, off + 0x06)[0]
    values = [
        unpack_from("<6h", rom, off + 0x08 + i * VALUE_SIZE) for i in range(VALUE_COUNT)
    ]
    field_0x20 = rom[off + 0x20]
    cast_flags = rom[off + 0x21]
    field_0x22 = rom[off + 0x22]
    cast_frame = rom[off + 0x23]
    field_0x24 = rom[off + 0x24]

    callback_init = _callback_init(callback, names_by_addr)
    # The thumb bit comes from the function symbol itself (SceneHandlers idiom).
    return [
        f"    // [{row}] {label} ({callback_init})",
        "    {",
        f"        .callback = {callback_init},",
        f"        .spiritCost = {spirit_cost},",
        f"        .field_0x06 = {field_0x06},",
        "        .values =",
        "        {",
        *[
            (
                f"            {{ .base = {value[0]}, .perLevel = {{ "
                + ", ".join(str(x) for x in value[1:])
                + " } },"
            )
            for value in values
        ],
        "        },",
        f"        .field_0x20 = {field_0x20},",
        f"        .castFlags = {flags_init(cast_flags, cast_flag_d_bits, cast_flag_known_bits, f'{hero} row {row} castFlags')},",
        f"        .field_0x22 = {field_0x22},",
        f"        .castTriggerFrame = {cast_frame},",
        f"        .field_0x24 = {field_0x24:#04x},",
        "    },",
    ]


def flags_init(value: int, d_bits: list[str], known_bits: set[int], what: str) -> str:
    """Flag byte -> decoded-view initializer (the carved-data idiom)."""
    members = flags_d_members(value, d_bits, known_bits, what)
    if not members:
        return "{ 0 }"
    return "{ .d = " + d_init(members) + " }"


def emit_required_levels(rom: bytes, labels: dict[str, list[str]]) -> str:
    """ActiveSkillRequiredLevels, outer index = HEROES_MEMORY_ORDER."""
    lines = [
        '#include "types.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(
            REQUIRED_LEVELS_ADDR,
            [
                "Hero level required to buy each active skill level. Rows index",
                "Player.activeSkills and columns index activeSkillLevelsPurchased. A value of -1",
                "means no further level can be purchased at this count.",
                "Hero blocks follow ROM order, not HeroId order.",
            ],
        ),
        f"const s8 ActiveSkillRequiredLevels[{HERO_COUNT}][{REQUIRED_LEVELS_ROWS}]"
        f"[{REQUIRED_LEVELS_COLUMNS}] = {{",
    ]
    for memory_index, hero in enumerate(HEROES_MEMORY_ORDER):
        base = required_levels_addr(hero)
        lines.append(f"    // [{memory_index}] {hero} (HERO_ID_{hero.upper()})")
        lines.append("    {")
        for row in range(REQUIRED_LEVELS_ROWS):
            values = unpack_from(
                f"<{REQUIRED_LEVELS_COLUMNS}b",
                rom,
                base - ROMBASE + row * REQUIRED_LEVELS_COLUMNS,
            )
            joined = ", ".join(f"{value:2}" for value in values)
            lines.append(f"        {{ {joined} }}, // [{row}] {labels[hero][row]}")
        lines.append("    },")
    lines.append("};")
    return "\n".join(lines) + "\n"


def emit_description_base_text_ids(rom: bytes) -> str:
    """Per-hero base text IDs of complete active skill descriptions, indexed by HeroId."""
    hero_ids = _hero_ids()
    lines = [
        '#include "types.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(
            DESCRIPTION_BASE_TEXT_IDS_ADDR,
            [
                "Per-hero base text IDs of complete active skill descriptions, indexed by HeroId.",
                "Add the active skill row to get its description text ID.",
            ],
        ),
        f"const u16 ActiveSkillDescriptionBaseTextIds[{HERO_COUNT}] = {{",
    ]
    for hero, hero_id in sorted(hero_ids.items(), key=lambda kv: kv[1]):
        text_id = unpack_from(
            "<H", rom, DESCRIPTION_BASE_TEXT_IDS_ADDR - ROMBASE + hero_id * 2
        )[0]
        lines.append(f"    {text_id}, // [HERO_ID_{hero.upper()}]")
    lines.append("};")
    return "\n".join(lines) + "\n"


def print_callback_table(rom: bytes, names_by_addr: dict[int, str]) -> None:
    """Print the distinct callbacks read from the hero rows."""
    callbacks = {
        value: _callback_init(value, names_by_addr)
        for hero in HEROES_MEMORY_ORDER
        for row in range(ROW_COUNT)
        if (value := _callback_value(rom, hero, row)) != 0
    }
    slots = HERO_COUNT * ROW_COUNT
    print(f"    {slots} slots, {len(callbacks)} distinct callbacks")
    print("  callback address table")
    for addr, name in sorted(callbacks.items()):
        print(f"    0x{addr:08x}  {name}")


def run() -> None:
    rom = load_rom()
    names_by_addr = {symbol.addr & ~1: symbol.name for symbol in func_symbols()}
    labels = _skill_labels(rom)
    check_arithmetic()
    print_callback_table(rom, names_by_addr)
    entries = []
    for name, addr, text in (
        (
            "ActiveSkillHeroSkills.c",
            HERO_ROWS_ADDR,
            emit_hero_skills(rom, names_by_addr, labels),
        ),
        (
            "ActiveSkillRequiredLevels.c",
            REQUIRED_LEVELS_ADDR,
            emit_required_levels(rom, labels),
        ),
        (
            "ActiveSkillDescriptionBaseTextIds.c",
            DESCRIPTION_BASE_TEXT_IDS_ADDR,
            emit_description_base_text_ids(rom),
        ),
    ):
        src = write_table(os.path.join("active_skill_tables", name), text.splitlines())
        print(f"  carved {src}")
        entries.append((addr, src))
    upsert_map(entries, owned_dirs=["active_skill_tables"])
    print("  updated config/split.cfg. Now run: make verify")
