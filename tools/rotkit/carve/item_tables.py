"""Carves the item-table family: ItemTypeInfos (master table @0x08068f50) and everything
hanging off it.

Per-type base items, gfx/loot rows and item flags; the prefix/suffix affix tables with
their per-affix flag tables; the master table itself; Runes (@0x080691a8, indexed by an
item's runeIndex nibble) and ArrowAffixFlags (@0x08063e9c, the arrows' intrinsic elemental
FX flags). Also emits carved/include/unique_item_ids.h (enum UniqueItemId plus the
loot_rollBaseItem band boundaries and loot-kind bases) from the UniqueItems nameIds,
UniqueItemFlags runs and UniqueGfx categories, and carved/include/affix_ids.h (one index
enum per affix table; the AffixIndexRange tables in the loot-tables family use them).

Base-item counts come from the master row, affix counts from config/data.cfg.
ROM-adjacent tables share one file per group (GroupWriter) with a *Tables suffix: a
category's Items/Gfx/ItemFlags trio, an affix table plus its flag table.
"""

import os
import re
import textwrap
from collections import Counter
from dataclasses import astuple, dataclass
from struct import Struct
from typing import Callable

from rotkit.cheaders import (
    extract_defines,
    extract_enum,
    extract_struct_fields,
    invert_enum,
)
from rotkit.carve import (
    GroupWriter,
    Section,
    TableSymbol,
    bit_indices,
    check_d_init,
    d_init,
    d_init_lines,
    data_symbol,
    doc_comment,
    entry_comment,
    flags_d_members,
    table_symbols,
    todo_lines,
    upsert_map,
    write_table,
)
from rotkit.paths import CARVED_INCLUDE, ROOT
from rotkit.rom import ROMBASE, load_rom
from rotkit.stores import data_symbols
from rotkit.textdb import decode_strings

# --- Item table data ------------------------------------------------------------

# nameId u16, tier u8, 4x(stat u8, val s8), 1 align pad. 'Bb' signs the value.
ITEM_MOD = Struct("<H B Bb Bb Bb Bb x")
NUM_MOD_SLOTS = 4
EMPTY_STAT = 0x5A  # empty modifier slot sentinel
STAT_NAME_BASE = 330  # decoded stat name string id = statIdx + 330


@dataclass(frozen=True)
class ItemMod:
    """One parsed ItemBase/ItemAffix, before name resolution."""

    name_id: int
    tier: int
    slots: tuple[tuple[int, int], ...]  # NUM_MOD_SLOTS x (stat_idx, signed val)


def parse_item_mod(rom: bytes, offset: int) -> ItemMod:
    name_id, tier, s0, v0, s1, v1, s2, v2, s3, v3 = ITEM_MOD.unpack_from(rom, offset)
    return ItemMod(
        name_id=name_id, tier=tier, slots=((s0, v0), (s1, v1), (s2, v2), (s3, v3))
    )


def used_stat_indices(rom: bytes, tables: list[TableSymbol]) -> set[int]:
    """Every statIdx the given ItemMod tables carry (empty slots excluded)."""
    used: set[int] = set()
    for table in tables:
        for i in range(table.count):
            entry = parse_item_mod(rom, table.addr - ROMBASE + i * ITEM_MOD.size)
            used.update(stat for stat, _ in entry.slots)
    used.discard(EMPTY_STAT)
    return used


def item_label(name_id: int, strings: dict[int, str]) -> str:
    """Decoded item/affix name for a raw text id."""
    return strings.get(name_id, f"?id0x{name_id:04x}")


def format_mods(slots: tuple[tuple[int, int], ...], strings: dict[int, str]) -> str:
    """The "Strength +9, Fragile +1" summary for an entry's non-empty slots."""
    parts = []
    for stat, val in slots:
        if stat == EMPTY_STAT:
            continue
        name = strings.get(stat + STAT_NAME_BASE, f"stat{stat}")
        parts.append(f"{name} {val:+d}")
    return ", ".join(parts)


def mod_column_widths(
    entries: list[ItemMod], name_for_stat: Callable[[int], str]
) -> tuple[int, int]:
    """Max rendered width of a stat name and of a signed value, for column alignment."""
    stat_strs = [name_for_stat(stat) for entry in entries for stat, _ in entry.slots]
    val_strs = [str(val) for entry in entries for _, val in entry.slots]
    return max(len(s) for s in stat_strs), max(len(s) for s in val_strs)


def loot_type_names() -> dict[int, str]:
    """LootType value -> member name, minus the ITEM_MIN/ITEM_MAX band markers (the
    named-unique band 0x3d..0x56 is not enumerable and emits as hex)."""
    return {
        v: k
        for k, v in extract_enum("include/loot.h", "LootType").items()
        if k not in ("LOOT_TYPE_ITEM_MIN", "LOOT_TYPE_ITEM_MAX")
    }


def stat_index_names() -> dict[int, str]:
    return invert_enum(extract_enum("include/stats.h", "StatIndex"))


def check_stat_indices(used: set[int], stat_names: dict[int, str]) -> None:
    """Fail the carve if a ROM statIdx has no enum StatIndex member."""
    missing = used - stat_names.keys()
    if missing:
        raise SystemExit(
            "ROM item tables use statIdx with no member in enum StatIndex "
            "(include/stats.h); add a member for each and re-run:\n  "
            + ", ".join(f"0x{idx:02x}" for idx in sorted(missing))
        )


def stat_name(stat: int, stat_names: dict[int, str]) -> str:
    """STAT_* member for a ROM statIdx (STAT_NONE for the empty-slot sentinel)."""
    return "STAT_NONE" if stat == EMPTY_STAT else stat_names[stat]


_ITEM_TYPE_INFO = Struct("<10I")  # sizeof ItemTypeInfo = 0x28


@dataclass
class ItemTypeInfo:
    base_items: int
    prefix_map_ranges: int
    prefix_table: int
    prefix_flags: int
    suffix_tier_ranges: int
    suffix_table: int
    suffix_flags: int
    base_gfx: int
    base_metadata: int
    base_item_count: int


def parse_item_type_infos(rom: bytes, symbol: TableSymbol) -> list[ItemTypeInfo]:
    base = symbol.addr - ROMBASE
    return [
        ItemTypeInfo(*_ITEM_TYPE_INFO.unpack_from(rom, base + i * _ITEM_TYPE_INFO.size))
        for i in range(symbol.count)
    ]


def _base_item_count(row: ItemTypeInfo) -> int:
    """The row's base item count; the game reads only the low byte, so the rest must be 0."""
    if row.base_item_count & 0xFFFFFF00:
        raise SystemExit(
            f"baseItemCount 0x{row.base_item_count:08x} has nonzero upper bytes; "
            "the u32 field model is wrong"
        )
    return row.base_item_count


@dataclass
class CarveContext:
    """Everything the emitters share: ROM views, decoded strings, enum name maps."""

    rom: bytes
    master: list[ItemTypeInfo]
    master_symbol: TableSymbol  # the ItemTypeInfos symbol
    strings: dict[int, str]  # Huffman-decoded text id -> string
    text_id_names: dict[int, str]  # enum TextId, id -> TEXT_ID_* name
    stat_names: dict[int, str]  # enum StatIndex, value -> name
    affix_bit_names: dict[int, str]  # enum AffixFlag, bit value -> name
    flag_bits: list[tuple[int, str]]  # ITEM_FLAG_* (value, name) in emission order
    item_flag_d_bits: list[str]  # union ItemFlags.d member names, bit order
    class_mask: int  # ITEM_FLAG_CLASS_ALL
    names_by_addr: dict[int, str]  # every config/data.cfg symbol


# --- Mod emission (shared by the base-item and affix tables) --------------------


def _name_id_value(name_id: int, ctx: CarveContext) -> str:
    """The .nameId field value: the TEXT_ID_* constant, raw hex when the id has none."""
    return ctx.text_id_names.get(name_id, f"0x{name_id:04x}")


def _mod_todos(
    entries: list[ItemMod],
    strings: dict[int, str],
    mod_fields: list[str],
    fragile_stat: int | None = None,
) -> list[str]:
    """Known-unknowns in a mod table -> `// TODO:` header lines.

    fragile_stat is passed for base-item tables only; affixes flag FRAGILE per slot via
    AFFIX_SLOT_MESSAGES instead.
    """
    todos = []
    unnamed = sorted(
        {
            stat
            for entry in entries
            for stat, _ in entry.slots
            if stat != EMPTY_STAT and (stat + STAT_NAME_BASE) not in strings
        }
    )
    if unnamed:
        todos.append(
            "stat indices with no decoded name (in-entry `stat<n>` placeholder): "
            + ", ".join(str(stat) for stat in unnamed)
        )
    for i, entry in enumerate(entries):
        label = item_label(entry.name_id, strings)
        if entry.name_id not in strings:
            todos.append(
                f"[{i}] nameId 0x{entry.name_id:04x} decodes to no string - "
                "id out of range or wrong field read"
            )
        for k, (stat, val) in enumerate(entry.slots):
            if stat == EMPTY_STAT and val != 0:
                todos.append(
                    f'[{i}] "{label}" carries .{mod_fields[3 + k * 2]}={val} on an empty '
                    "(STAT_NONE) slot - meaning unknown"
                )
            if fragile_stat is not None and stat == fragile_stat:
                todos.append(
                    f'[{i}] "{label}" has STAT_FRAGILE in .{mod_fields[2 + k * 2]} - never procs on a '
                    "base item, see combat_fragileOnKill (reads only the prefix's "
                    f"{mod_fields[4]})"
                )
    return todos


def _emit_mod_table(
    table: TableSymbol,
    ctx: CarveContext,
    c_type: str,
    todos_fn: Callable[[list[ItemMod], list[str]], list[str]],
    render_entry: Callable[
        [ItemMod, int, CarveContext, Callable[[int], str], tuple[int, int], list[str]],
        list[str],
    ],
) -> tuple[Section, dict[int, str]]:
    """Parse + emit one 12-byte mod table -> (its group-file section, {index: name})."""
    mod_fields = extract_struct_fields("include/item.h", c_type)
    if len(mod_fields) != 2 + 2 * NUM_MOD_SLOTS:
        raise SystemExit(
            f"{c_type} has {len(mod_fields)} fields, expected {2 + 2 * NUM_MOD_SLOTS}"
        )
    entries = [
        parse_item_mod(ctx.rom, table.addr - ROMBASE + i * ITEM_MOD.size)
        for i in range(table.count)
    ]
    names = {i: item_label(e.name_id, ctx.strings) for i, e in enumerate(entries)}

    def name_for_stat(stat: int) -> str:
        return stat_name(stat, ctx.stat_names)

    widths = mod_column_widths(entries, name_for_stat)

    lines = []
    for todo in todos_fn(entries, mod_fields):
        lines += todo_lines(todo)
    lines += [
        "",
        *doc_comment(table.addr),
        f"const {c_type} {table.name}[{table.count}] = {{",
    ]
    for i, entry in enumerate(entries):
        lines += render_entry(entry, i, ctx, name_for_stat, widths, mod_fields)
        lines.append("")
    lines[-1] = "};"
    section = Section(table.addr, ["stats.h", "text_ids.h"], lines)
    return section, names


# --- Base item tables (ItemBase) ------------------------------------------------


def _render_base_item(
    entry: ItemMod,
    index: int,
    ctx: CarveContext,
    name_for_stat: Callable[[int], str],
    widths: tuple[int, int],
    mod_fields: list[str],
) -> list[str]:
    stat_width, val_width = widths
    header = f'    // [{index}] "{item_label(entry.name_id, ctx.strings)}"'
    mods = format_mods(entry.slots, ctx.strings)
    lines = [header + (f": {mods}" if mods else "")]

    lines.append(f"    {{ .{mod_fields[0]} = {_name_id_value(entry.name_id, ctx)},")
    lines.append(f"      .{mod_fields[1]} = {entry.tier},")
    for slot_idx, (stat, val) in enumerate(entry.slots):
        end = " }," if slot_idx == NUM_MOD_SLOTS - 1 else ","
        lines.append(
            f"      .{mod_fields[2 + slot_idx * 2]} = {name_for_stat(stat) + ',':<{stat_width + 1}} "
            f".{mod_fields[3 + slot_idx * 2]} = {val:>{val_width}}{end}"
        )
    return lines


def emit_base_items(
    table: TableSymbol, ctx: CarveContext
) -> tuple[Section, dict[int, str]]:
    fragile_stat = next(
        (v for v, n in ctx.stat_names.items() if n == "STAT_FRAGILE"), None
    )
    if fragile_stat is None:
        raise SystemExit("STAT_FRAGILE missing from enum StatIndex (include/stats.h)")
    return _emit_mod_table(
        table,
        ctx,
        "ItemBase",
        todos_fn=lambda entries, mf: _mod_todos(entries, ctx.strings, mf, fragile_stat),
        render_entry=_render_base_item,
    )


# --- Unique item ids (enum UniqueItemId) ------------------------------------------

_UNIQUE_ITEM_IDS_OUT = CARVED_INCLUDE / "unique_item_ids.h"
_TEXT_ID_UNIQUE_PREFIX = "TEXT_ID_UNIQUE_"


def _flag_run_end(flags: list[int], bit: int, start: int, what: str) -> int:
    """End of the contiguous `bit` run from `start`; a missing or stray bit is a carve error."""
    end = start
    while end < len(flags) and flags[end] & bit:
        end += 1
    if end == start or any(f & bit for f in flags[end:]):
        raise SystemExit(
            f"UniqueItemFlags: {what} flags are not one contiguous run from index {start}"
        )
    return end


def emit_unique_item_ids(
    table: TableSymbol, ctx: CarveContext, flags_addr: int, gfx_addr: int
) -> None:
    """UniqueItems -> carved/include/unique_item_ids.h: enum UniqueItemId, the
    loot_rollBaseItem band boundaries (derived from the UniqueItemFlags runs: the
    ITEM_FLAG_UNIQUE_WEAPON run ends the weapon band, ITEM_FLAG_CARRIED_PASSIVE the
    carried-passive band), and the bands' lootType bases (UniqueGfx rows; the
    lootType == base + index contiguity the uniqueWeaponsCollected /
    uniquePassivesCollected bit math relies on is asserted here)."""
    members = []
    for i in range(table.count):
        entry = parse_item_mod(ctx.rom, table.addr - ROMBASE + i * ITEM_MOD.size)
        text_id = ctx.text_id_names.get(entry.name_id, "")
        if not text_id.startswith(_TEXT_ID_UNIQUE_PREFIX):
            raise SystemExit(
                f"UniqueItems[{i}] nameId 0x{entry.name_id:04x} has no "
                f"{_TEXT_ID_UNIQUE_PREFIX}* member in enum TextId"
            )
        members.append("UNIQUE_ID_" + text_id.removeprefix(_TEXT_ID_UNIQUE_PREFIX))

    bit_by_name = {name: value for value, name in ctx.flag_bits}
    flags = [
        _ITEM_FLAGS_ROW.unpack_from(
            ctx.rom, flags_addr - ROMBASE + i * _ITEM_FLAGS_ROW.size
        )[0]
        for i in range(table.count)
    ]
    weapon_end = _flag_run_end(
        flags, bit_by_name["ITEM_FLAG_UNIQUE_WEAPON"], 0, "ITEM_FLAG_UNIQUE_WEAPON"
    )
    passive_end = _flag_run_end(
        flags,
        bit_by_name["ITEM_FLAG_CARRIED_PASSIVE"],
        weapon_end,
        "ITEM_FLAG_CARRIED_PASSIVE",
    )

    categories = [
        _ITEM_GFX.unpack_from(ctx.rom, gfx_addr - ROMBASE + i * _ITEM_GFX.size)[6]
        for i in range(table.count)
    ]
    weapon_base = categories[0]
    for i in range(passive_end):
        if categories[i] != weapon_base + i:
            raise SystemExit(
                f"UniqueGfx[{i}].lootType=0x{categories[i]:02x} breaks the weapon/"
                f"carried-passive band's base + index run (base 0x{weapon_base:02x})"
            )

    with open(_UNIQUE_ITEM_IDS_OUT, "w") as fh:
        fh.write(
            "// Written by rotkit carve item-tables: enum UniqueItemId, one member per\n"
            "// UniqueItems row (carved/data/item_tables/UniqueTables.c) named from its\n"
            "// TEXT_ID_UNIQUE_* name, plus:\n"
            "// - the loot_rollBaseItem band boundaries, derived from the UniqueItemFlags\n"
            "//   ITEM_FLAG_UNIQUE_WEAPON / ITEM_FLAG_CARRIED_PASSIVE runs (weapons, then\n"
            "//   carried passives, then repeatables);\n"
            "// - the bands' UniqueGfx.lootType bases (lootType == base + index across\n"
            "//   both bands, asserted at carve time); each base doubles as the\n"
            "//   Player.uniqueWeaponsCollected / uniquePassivesCollected bit base.\n"
        )
        fh.write("#pragma once\n\n")
        fh.write("enum UniqueItemId\n{\n")
        fh.write(",\n".join(f"    {name} = {i}" for i, name in enumerate(members)))
        fh.write("\n};\n\n")
        fh.write(f"#define UNIQUE_WEAPON_BAND_END {members[weapon_end]}\n")
        fh.write(f"#define UNIQUE_CARRIED_PASSIVE_BAND_END {members[passive_end]}\n")
        fh.write(f"#define UNIQUE_ITEM_COUNT ({members[-1]} + 1)\n\n")
        fh.write(f"#define LOOT_TYPE_UNIQUE_WEAPON_MIN 0x{weapon_base:02x}\n")
        fh.write(
            f"#define LOOT_TYPE_UNIQUE_PASSIVE_MIN 0x{categories[weapon_end]:02x}\n"
        )

    print(f"  wrote {os.path.relpath(_UNIQUE_ITEM_IDS_OUT, ROOT)}: {len(members)} ids")


# --- Affix tables (ItemAffix) ---------------------------------------------------

# fragileOnKillPickSlot in combat.c reads only the prefix's stat1, so FRAGILE fires only
# at (prefix, stat1).
_FRAGILE_BUG_TEXT = "Fragile only procs from a prefix's"


def _render_affix(
    entry: ItemMod,
    index: int,
    is_prefix: bool,
    ctx: CarveContext,
    name_for_stat: Callable[[int], str],
    widths: tuple[int, int],
    mod_fields: list[str],
) -> list[str]:
    stat_width, val_width = widths
    slots = entry.slots

    # FRAGILE slots that can never proc (anything but a prefix's stat1) get a BUG note.
    bug_slots = [
        name_for_stat(stat) == "STAT_FRAGILE" and not (is_prefix and slot_idx == 1)
        for slot_idx, (stat, _) in enumerate(slots)
    ]

    header = f'    // [{index}] "{item_label(entry.name_id, ctx.strings)}"'
    mods = format_mods(slots, ctx.strings)
    if mods:
        header += f": {mods}"
    lines = [header]
    lines += [
        f"    //   BUG: {mod_fields[2 + slot_idx * 2]} - {_FRAGILE_BUG_TEXT} {mod_fields[4]},"
        " see combat_fragileOnKill"
        for slot_idx, bug in enumerate(bug_slots)
        if bug
    ]

    lines.append(f"    {{ .{mod_fields[0]} = {_name_id_value(entry.name_id, ctx)},")
    lines.append(f"      .{mod_fields[1]} = {entry.tier},")
    for slot_idx, (stat, val) in enumerate(slots):
        end = " }," if slot_idx == NUM_MOD_SLOTS - 1 else ","
        line = (
            f"      .{mod_fields[2 + slot_idx * 2]} = {name_for_stat(stat) + ',':<{stat_width + 1}} "
            f".{mod_fields[3 + slot_idx * 2]} = {val:>{val_width}}{end}"
        )
        if bug_slots[slot_idx]:
            line += (
                f" // BUG: never procs if not in {mod_fields[4]},"
                " see combat_fragileOnKill"
            )
        lines.append(line)
    return lines


def emit_affix_table(
    table: TableSymbol, ctx: CarveContext
) -> tuple[Section, dict[int, str]]:
    is_prefix = "Prefix" in table.name  # ItemSuffixes is the only non-prefix table
    return _emit_mod_table(
        table,
        ctx,
        "ItemAffix",
        todos_fn=lambda entries, mf: _mod_todos(entries, ctx.strings, mf),
        render_entry=lambda entry, i, c, n, w, mf: _render_affix(
            entry, i, is_prefix, c, n, w, mf
        ),
    )


# --- Per-base-item flag tables (ItemFlags) --------------------------------------

_ITEM_FLAGS_ROW = Struct("<II")  # flags, equipSfx

_CLASS_ALL = "ITEM_FLAG_CLASS_ALL"

# ITEM_FLAG_* names in emission order within an entry (class bits come first).
_SLOT_BIT_NAMES = [
    "ITEM_FLAG_SLOT_ARMOR",
    "ITEM_FLAG_SLOT_WEAPON",
    "ITEM_FLAG_SLOT_OFFHAND",
    "ITEM_FLAG_SLOT_HELMET",
    "ITEM_FLAG_SLOT_CLOAK",
    "ITEM_FLAG_SLOT_GLOVES",
    "ITEM_FLAG_SLOT_BOOTS",
    "ITEM_FLAG_SLOT_JEWELRY",
]
_OTHER_BIT_NAMES = [
    "ITEM_FLAG_CARRIED_PASSIVE",
    "ITEM_FLAG_TWO_HANDED",
    "ITEM_FLAG_MIN_LEVEL_PLUS_5_A",
    "ITEM_FLAG_MIN_LEVEL_PLUS_5_B",
    "ITEM_FAMILY_SWORD",
    "ITEM_FAMILY_AXE",
    "ITEM_FAMILY_MACE",
    "ITEM_FAMILY_STAFF",
    "ITEM_FAMILY_BOW",
    "ITEM_FAMILY_SHIELD",
    "ITEM_FLAG_ANCIENT",
    "ITEM_FLAG_NO_PREFIX",
    "ITEM_FLAG_NO_SUFFIX",
    "ITEM_FLAG_UNIQUE_WEAPON",
]
_ALL_BIT_NAMES = _SLOT_BIT_NAMES + _OTHER_BIT_NAMES


def _item_flag_values() -> dict[str, int]:
    """ITEM_FLAG_* name -> value; ITEM_FLAG_CLASS_ALL is a #define, the rest enum
    ItemFlag."""
    values = {
        **extract_enum("include/item.h", "ItemFlag"),
        **extract_defines("include/item.h", {_CLASS_ALL}),
    }
    missing = [name for name in [*_ALL_BIT_NAMES, _CLASS_ALL] if name not in values]
    if missing:
        raise KeyError(
            "ITEM_FLAG_* name(s) in neither enum ItemFlag nor a #define: "
            + ", ".join(missing)
        )
    return values


def _item_flag_d_members(flags: int, ctx: CarveContext, what: str) -> list[str]:
    """One flags word -> the set bits' ItemFlags.d member names (bit order; class bits are
    the classFrodo..classGimli members)."""
    known = {i for i in range(32) if ctx.class_mask & (1 << i)} | bit_indices(
        dict(ctx.flag_bits)
    )
    members = flags_d_members(flags, ctx.item_flag_d_bits, known, what)
    check_d_init(members, flags, ctx.item_flag_d_bits)
    return members


def emit_item_flags(
    table: TableSymbol, ctx: CarveContext, names: dict[int, str]
) -> Section:
    """Carve one ItemFlags table (entry comments from the sibling *Items table)."""
    f_flags, f_equip_sfx = extract_struct_fields("include/item.h", "ItemMetadata")
    rows = [
        _ITEM_FLAGS_ROW.unpack_from(
            ctx.rom, table.addr - ROMBASE + i * _ITEM_FLAGS_ROW.size
        )
        for i in range(table.count)
    ]

    modal_flags, modal_count = Counter(f for f, _ in rows).most_common(1)[0]
    macro = f"FLAGS_{table.name.removesuffix('ItemFlags').upper()}"
    macro_doc = f"most common row ({modal_count} of {table.count}); deviations are spelled out per entry"
    if table.name == "UniqueItemFlags":
        macro = "FLAGS_UNIQUE_PASSIVE"
        macro_doc = (
            f"carried-passive trinket row, most common ({modal_count} of {table.count});"
            " deviations are spelled out per entry"
        )

    lines = [
        f"/** {macro_doc} */",
        f"#define {macro} {d_init(_item_flag_d_members(modal_flags, ctx, table.name))}",
        "",
        *doc_comment(table.addr),
        f"const ItemMetadata {table.name}[{table.count}] = {{",
    ]
    for i, (flags, sfx) in enumerate(rows):
        comment = entry_comment(i, names.get(i, ""))
        if i > 0:
            lines.append("")
        lines.append(f"    {comment}")
        if flags == modal_flags:
            lines.append(
                f"    {{ .{f_flags}.d = {macro}, .{f_equip_sfx} = 0x{sfx:02x} }},"
            )
            continue
        members = _item_flag_d_members(flags, ctx, table.name)
        if len(members) <= 1:
            lines.append(
                f"    {{ .{f_flags}.d = {d_init(members)}, .{f_equip_sfx} = 0x{sfx:02x} }},"
            )
            continue
        lines += d_init_lines(f"    {{ .{f_flags}.d = {{ ", members, " },")
        lines.append(f"      .{f_equip_sfx} = 0x{sfx:02x} }},")
    lines += ["};", f"#undef {macro}"]
    return Section(table.addr, ["item.h"], lines)


# --- Per-affix flag tables (AffixFlags) -----------------------------------------


_AFFIX_FLAGS_WORD = Struct("<I")


@dataclass
class AffixFlagsTable:
    """One per-affix AffixFlags table to carve."""

    name: str  # carved symbol, e.g. WeaponPrefixFlags
    addr: int
    count: int  # shared with the sibling affix table
    sibling_addr: int  # affix table whose entry names supply the comments


def emit_affix_flags(
    table: AffixFlagsTable,
    ctx: CarveContext,
    names: dict[int, str],
    doc: str = "Per-affix flag table; indexed by the affix index.",
) -> Section:
    """Carve one AffixFlags[N] table (entry comments from the sibling table's names).

    doc is the table comment's first line (wrapped); the AFFIX_FLAG_* pointer and the
    zero-row macro note follow.
    """
    d_bits = extract_struct_fields("include/item.h", "AffixFlags", "d")
    known = bit_indices(ctx.affix_bit_names)
    words = [
        _AFFIX_FLAGS_WORD.unpack_from(ctx.rom, table.addr - ROMBASE + i * 4)[0]
        for i in range(table.count)
    ]

    # a macro for the zero row once it repeats
    none_macro = "AFFIX_FLAGS_NONE"
    none_count = words.count(0)
    use_none_macro = none_count >= 2

    doc_lines = textwrap.wrap(f"{doc} Decoded bits: AFFIX_FLAG_* (item.h).", width=89)
    lines = []
    if use_none_macro:
        lines += [
            f"/** no decoded bits set ({none_count} of {table.count} entries) */",
            f"#define {none_macro} {{ 0 }}",
            "",
        ]
    lines += [
        *doc_comment(table.addr, doc_lines),
        f"const AffixFlags {table.name}[{table.count}] = {{",
    ]
    for i, word in enumerate(words):
        members = flags_d_members(word, d_bits, known, table.name)
        check_d_init(members, word, d_bits)
        if i > 0:
            lines.append("")
        lines.append(f"    {entry_comment(i, names.get(i, ''))}")
        if use_none_macro and not members:
            lines.append(f"    {{ .d = {none_macro} }},")
        elif len(members) <= 1:
            lines.append(f"    {{ .d = {d_init(members)} }},")
        else:
            lines += d_init_lines("    { .d = { ", members, " } },")
    lines += ["};"]
    if use_none_macro:
        lines.append(f"#undef {none_macro}")
    return Section(table.addr, ["item.h"], lines)


# --- Per-base-item gfx/loot tables (ItemGfx) ------------------------------------

_ITEM_GFX = Struct("<7I")  # 7 u32 = 0x1c bytes; matches struct ItemGfx
ITEM_GFX_SIZE = _ITEM_GFX.size


@dataclass
class ItemGfx:
    tiles: int
    frames: int
    palette: int
    frame_duration: int  # frameDuration + pad word
    equip_icon: int
    hollow_icon: int
    loot_type: int


def parse_item_gfx(rom: bytes, offset: int) -> ItemGfx:
    return ItemGfx(*_ITEM_GFX.unpack_from(rom, offset))


_ITEM_GFX_FIELDS = ["animation", "equipIcon", "hollowIcon", "lootType"]


def _icon_symbol(ctx: CarveContext, addr: int, where: str) -> str:
    """The store's name for an icon the row points at; the ITEM_GFX macros need it."""
    name = ctx.names_by_addr.get(addr)
    if name is None:
        raise SystemExit(
            f"{where}: no data store symbol at 0x{addr:08x}; run `rotkit carve item-gfx`"
            " first, it names the icons this table points at"
        )
    return name


def _gfx_row(
    row: ItemGfx, ctx: CarveContext, where: str, loot_type_names: dict[int, str]
) -> str:
    """One ITEM_GFX* macro line, the narrowest form the row's icons allow."""
    if row.frame_duration != 0:
        raise SystemExit(
            f"{where}: frameDuration word 0x{row.frame_duration:08x}, expected 0"
        )
    tiles = _icon_symbol(ctx, row.tiles, where)
    suffix = "BackpackIconTiles"
    if not tiles.endswith(suffix):
        raise SystemExit(f"{where}: .animation.tiles is {tiles}, not a *{suffix}")
    name = tiles[: -len(suffix)]
    for addr, expected in (
        (row.frames, f"{name}BackpackIconFrames"),
        (row.palette, f"{name}BackpackIconPalette"),
    ):
        got = _icon_symbol(ctx, addr, where)
        if got != expected:
            raise SystemExit(f"{where}: expected {expected}, found {got}")

    loot_type = loot_type_names.get(row.loot_type, f"0x{row.loot_type:02x}")
    equip = _icon_symbol(ctx, row.equip_icon, where) if row.equip_icon else "NULL"
    hollow = _icon_symbol(ctx, row.hollow_icon, where)
    if hollow == f"{name}HollowIcon":
        if equip == f"{name}EquipIcon":
            return f"ITEM_GFX({name}, {loot_type})"
        if equip == "NULL":
            return f"ITEM_GFX_CARRIED({name}, {loot_type})"
    return f"ITEM_GFX_ICONS({name}, {equip}, {hollow}, {loot_type})"


def emit_item_gfx(
    table: TableSymbol,
    ctx: CarveContext,
    loot_type_names: dict[int, str],
    names: dict[int, str],
) -> Section:
    """Carve one ItemGfx table (entry comments from the sibling *Items table)."""
    fields = extract_struct_fields("include/item.h", "ItemGfx")
    if fields != _ITEM_GFX_FIELDS:
        raise SystemExit(
            f"ItemGfx fields {fields} differ from the ITEM_GFX macros' {_ITEM_GFX_FIELDS}"
        )
    rows = [
        parse_item_gfx(ctx.rom, table.addr - ROMBASE + i * _ITEM_GFX.size)
        for i in range(table.count)
    ]
    lines = [
        *doc_comment(
            table.addr,
            [
                "Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,",
                "loot_rollBaseItem reads .lootType to pick the loot.",
            ],
        ),
        f"const ItemGfx {table.name}[{table.count}] = {{",
    ]
    for i, row in enumerate(rows):
        where = f"{table.name}[{i}]"
        if row.loot_type & 0xFFFFFF00:
            raise SystemExit(
                f"{where}: .lootType=0x{row.loot_type:08x} has nonzero upper bytes"
            )
        lines.append(f"    {entry_comment(i, names.get(i, ''))}")
        lines.append(f"    {_gfx_row(row, ctx, where, loot_type_names)},")
    lines += ["};"]
    return Section(table.addr, ["item.h", "libc.h", "loot.h", "variables.h"], lines)


# --- Affix index enums (affix_ids.h) -----------------------------------------

_AFFIX_IDS_OUT = CARVED_INCLUDE / "affix_ids.h"


def _snake_upper(name: str) -> str:
    return re.sub(r"(?<!^)(?=[A-Z])", "_", name).upper()


def emit_affix_ids(affix_tables: list[TableSymbol], ctx: CarveContext) -> None:
    """Affix tables -> carved/include/affix_ids.h: one index enum per ItemAffix table
    (WeaponPrefixId, ..., ItemSuffixId), members named from the affixes'
    TEXT_ID_PREFIX_* / TEXT_ID_SUFFIX_* names."""
    blocks = []
    for table in affix_tables:
        base = table.name.removesuffix("es")  # WeaponPrefixes -> WeaponPrefix
        member_prefix = _snake_upper(base) + "_"
        text_prefix = "TEXT_ID_SUFFIX_" if "Suffix" in table.name else "TEXT_ID_PREFIX_"
        members = []
        for i in range(table.count):
            entry = parse_item_mod(ctx.rom, table.addr - ROMBASE + i * ITEM_MOD.size)
            text_id = ctx.text_id_names.get(entry.name_id, "")
            if not text_id.startswith(text_prefix):
                raise SystemExit(
                    f"{table.name}[{i}] nameId 0x{entry.name_id:04x} has no "
                    f"{text_prefix}* member in enum TextId"
                )
            members.append(member_prefix + text_id.removeprefix(text_prefix))
        if len(set(members)) != len(members):
            raise SystemExit(f"{table.name}: duplicate affix names in enum {base}Id")
        body = ",\n".join(f"    {m} = {i}" for i, m in enumerate(members))
        blocks.append(f"enum {base}Id\n{{\n{body}\n}};\n")
    with open(_AFFIX_IDS_OUT, "w") as fh:
        fh.write(
            "// Written by rotkit carve item-tables: one index enum per ItemAffix table\n"
            "// (carved/data/affix_tables/), members named from the affixes' TEXT_ID_PREFIX_*\n"
            "// / TEXT_ID_SUFFIX_* names; the AffixIndexRange tables (carved/data/loot_tables/)\n"
            "// index with them.\n"
        )
        fh.write("#pragma once\n\n")
        fh.write("\n".join(blocks))
    print(f"  wrote {os.path.relpath(_AFFIX_IDS_OUT, ROOT)}: {len(blocks)} enums")


# --- Master dispatch table (ItemTypeInfo) ---------------------------------------

# (ItemTypeInfo attribute, render kind) in struct order; the C field names are read back from
# include/item.h at carve time. 'ptr' must resolve to a carved symbol or NULL (a miss means the
# struct model or data.cfg is wrong); 'flagsptr' may point at uncarved data -> AffixFlags cast
# literal; 'count' is a plain u32.
_TYPE_INFO_FIELDS = (
    ("base_items", "ptr"),
    ("prefix_map_ranges", "ptr"),
    ("prefix_table", "ptr"),
    ("prefix_flags", "flagsptr"),
    ("suffix_tier_ranges", "ptr"),
    ("suffix_table", "ptr"),
    ("suffix_flags", "flagsptr"),
    ("base_gfx", "ptr"),
    ("base_metadata", "ptr"),
    ("base_item_count", "count"),
)


def _type_info_field_init(
    c_field: str, kind: str, value: int, names_by_addr: dict[int, str]
) -> tuple[str, str]:
    """-> (initializer, trailing_comment) for one field. Empty comment when none."""
    if kind == "count":
        return str(value), ""
    if value == 0:
        return "NULL", ""
    name = names_by_addr.get(value)
    if name is not None:
        return name, ""
    if kind == "ptr":
        raise SystemExit(
            f"{c_field} -> 0x{value:08x} matches no carved data symbol; add the target to "
            "config/data.cfg or fix the struct type"
        )
    return f"(const AffixFlags *)0x{value:08x}", "// uncarved"


def _render_type_info(
    row: ItemTypeInfo,
    type_name: str,
    names_by_addr: dict[int, str],
    fields: list[tuple[str, str, str]],
    field_width: int,
) -> list[str]:
    """One row -> its C lines: header comment + aligned field initializers."""
    lines = [f"    // {type_name}", "    {"]
    rendered = []
    for attr, c_field, kind in fields:
        init, comment = _type_info_field_init(
            c_field, kind, getattr(row, attr), names_by_addr
        )
        rendered.append((f"      .{c_field:<{field_width}} = {init},", comment))
    rendered[-1] = (rendered[-1][0] + " },", rendered[-1][1])
    lines += [code + (f"  {comment}" if comment else "") for code, comment in rendered]
    return lines


def emit_item_type_infos(ctx: CarveContext) -> str:
    symbol = ctx.master_symbol
    type_names = invert_enum(extract_enum("include/item.h", "ItemType"))
    c_fields = extract_struct_fields("include/item.h", "ItemTypeInfo")
    if len(c_fields) != len(_TYPE_INFO_FIELDS):
        raise SystemExit(
            f"ItemTypeInfo has {len(c_fields)} fields, "
            f"expected {len(_TYPE_INFO_FIELDS)}"
        )
    fields = [
        (attr, c_field, kind)
        for (attr, kind), c_field in zip(_TYPE_INFO_FIELDS, c_fields)
    ]
    field_width = max(len(c_field) for c_field in c_fields)

    lines = [
        '#include "variables.h"',
        '#include "item.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(symbol.addr),
        f"const ItemTypeInfo {symbol.name}[{symbol.count}] = {{",
    ]
    for i, row in enumerate(ctx.master):
        if i > 0:
            lines.append("")
        lines += _render_type_info(
            row,
            type_names.get(i, f"ITEM_TYPE_{i}"),
            ctx.names_by_addr,
            fields,
            field_width,
        )
    lines[-1] = lines[-1].rstrip(",")  # last entry drops its trailing comma
    lines += ["};"]
    return write_table("item_tables/ItemTypeInfos.c", lines)


# --- Rune table (RuneStats) -----------------------------------------------------

# 2x(stat u8, val s8). 'b' signs the value.
_RUNE_MOD = Struct("<BbBb")
# decoded rune name string id = runeIndex + 444 (TEXT_ID_DWARF_RUNE_I_DWARFMETAL)
_RUNE_NAME_BASE = 444


def emit_runes(ctx: CarveContext) -> tuple[int, str, int]:
    table = data_symbol("RuneStats[")
    fields = extract_struct_fields("include/item.h", "RuneStats")
    if len(fields) != 4:
        raise SystemExit(f"RuneStats has {len(fields)} fields, expected 4")
    entries = [
        _RUNE_MOD.unpack_from(ctx.rom, table.addr - ROMBASE + i * _RUNE_MOD.size)
        for i in range(table.count)
    ]
    check_stat_indices(
        {stat for entry in entries for stat in (entry[0], entry[2])} - {EMPTY_STAT},
        ctx.stat_names,
    )

    def name_for_stat(stat: int) -> str:
        return stat_name(stat, ctx.stat_names)

    stat_width = max(len(name_for_stat(entry[i])) for entry in entries for i in (0, 2))
    val_width = max(len(str(entry[i])) for entry in entries for i in (1, 3))

    lines = ['#include "stats.h"', '#include "item.h"', "", "// clang-format off"]
    lines += [
        "",
        *doc_comment(table.addr),
        f"const RuneStats {table.name}[{table.count}] = {{",
    ]
    for i, (s0, v0, s1, v1) in enumerate(entries):
        label = item_label(_RUNE_NAME_BASE + i, ctx.strings)
        mods = format_mods(((s0, v0), (s1, v1)), ctx.strings)
        lines.append(f'    // [{i}] "{label}"' + (f": {mods}" if mods else ""))
        lines.append(
            f"    {{ .{fields[0]} = {name_for_stat(s0) + ',':<{stat_width + 1}} "
            f".{fields[1]} = {v0:>{val_width}},"
        )
        lines.append(
            f"      .{fields[2]} = {name_for_stat(s1) + ',':<{stat_width + 1}} "
            f".{fields[3]} = {v1:>{val_width}} }},"
        )
        lines.append("")
    lines[-1] = "};"
    src = write_table(os.path.join("item_tables", f"{table.name}.c"), lines)
    return table.addr, src, table.count


# --- Entry point ----------------------------------------------------------------


def run() -> None:
    rom = load_rom()
    master_symbol = data_symbol("ItemTypeInfo[")
    master = parse_item_type_infos(rom, master_symbol)
    flag_values = _item_flag_values()
    ctx = CarveContext(
        rom=rom,
        master=master,
        master_symbol=master_symbol,
        strings=decode_strings(rom),
        text_id_names=invert_enum(extract_enum("carved/include/text_ids.h", "TextId")),
        stat_names=stat_index_names(),
        affix_bit_names=invert_enum(extract_enum("include/item.h", "AffixFlag")),
        flag_bits=[(flag_values[name], name) for name in _ALL_BIT_NAMES],
        item_flag_d_bits=extract_struct_fields("include/item.h", "ItemFlags", "d"),
        class_mask=flag_values[_CLASS_ALL],
        names_by_addr={s.addr: s.name for s in data_symbols()},
    )
    affix_counts = {t.addr: t.count for t in table_symbols("ItemAffix[")}

    # Every statIdx used by the base-item and affix tables must have an enum StatIndex
    # member before anything is emitted.
    mod_tables = [
        TableSymbol(
            ctx.names_by_addr[row.base_items],
            row.base_items,
            _base_item_count(row),
        )
        for row in master
    ]
    affix_tables: dict[int, TableSymbol] = {}
    affix_flags: dict[int, AffixFlagsTable] = {}
    for row in master:
        for table_addr, flags_addr in (
            (row.prefix_table, row.prefix_flags),
            (row.suffix_table, row.suffix_flags),
        ):
            if not table_addr:
                continue
            affix_tables[table_addr] = TableSymbol(
                ctx.names_by_addr[table_addr], table_addr, affix_counts[table_addr]
            )
            affix_flags[flags_addr] = AffixFlagsTable(
                name=ctx.names_by_addr[flags_addr],
                addr=flags_addr,
                count=affix_counts[table_addr],
                sibling_addr=table_addr,
            )
    check_stat_indices(
        used_stat_indices(rom, mod_tables + list(affix_tables.values())), ctx.stat_names
    )

    carved = []

    # Per-category group file (item_tables/KnifeTables.c): base items, gfx/loot rows,
    # item flags; the item names feed the entry comments of the other two.
    groups: dict[str, GroupWriter] = {}  # category ("Knife") -> its group file
    category_names: dict[str, dict[int, str]] = {}  # category -> {index: item name}
    loot_names = loot_type_names()
    for row, table in zip(master, mod_tables):
        category = table.name.removesuffix("Items")
        group = groups[category] = GroupWriter(f"item_tables/{category}Tables.c")

        section, names = emit_base_items(table, ctx)
        group.add(section)
        category_names[category] = names

        count = _base_item_count(row)
        gfx_table = TableSymbol(ctx.names_by_addr[row.base_gfx], row.base_gfx, count)
        group.add(emit_item_gfx(gfx_table, ctx, loot_names, names))

        flags_table = TableSymbol(
            ctx.names_by_addr[row.base_metadata], row.base_metadata, count
        )
        group.add(emit_item_flags(flags_table, ctx, names))

    # Standalone per-item AffixFlags tables: not hung off ItemTypeInfos (arrows roll no
    # affixes, so their intrinsic elemental FX flags live in a fixed per-arrow table read
    # by the projectile FX path at 0x08026e88). <X>AffixFlags joins category X's group.
    hung_off = {f for row in master for f in (row.prefix_flags, row.suffix_flags)}
    for table in table_symbols("AffixFlags["):
        if table.addr in hung_off:
            continue
        category = table.name.removesuffix("AffixFlags")
        if category not in groups:
            raise SystemExit(
                f"standalone AffixFlags table {table.name} has no {category}Items "
                "group to join"
            )
        groups[category].add(
            emit_affix_flags(
                AffixFlagsTable(table.name, table.addr, table.count, 0),
                ctx,
                category_names[category],
                doc=(
                    f"Per-item flag table; indexed by {category}Items baseIndex "
                    "(intrinsic elemental FX flags, read by the projectile FX path "
                    "at 0x08026e88)."
                ),
            )
        )

    for category, group in groups.items():
        addr, src = group.write()
        carved.append((addr, src))
        print(f"  carved {src}  ({len(group.sections)} tables)")

    # The unique base-item table also gets an id enum + band boundaries
    # (loot_rollBaseItem tests indices).
    unique_table = next(t for t in mod_tables if t.name == "UniqueItems")
    unique_row = next(r for r in master if r.base_items == unique_table.addr)
    emit_unique_item_ids(
        unique_table, ctx, unique_row.base_metadata, unique_row.base_gfx
    )

    # Affix group files (affix_tables/WeaponPrefixTables.c): the affix table + its
    # per-affix flag table (deduped: the weapon types share WeaponPrefixes, every type
    # shares ItemSuffixes/ItemSuffixFlags).
    affix_groups: dict[int, GroupWriter] = {}  # affix table addr -> its group file
    affix_names: dict[int, dict[int, str]] = {}
    for table in sorted(affix_tables.values(), key=lambda t: t.addr):
        section, names = emit_affix_table(table, ctx)
        group_name = table.name.removesuffix("es") + "Tables"
        group = affix_groups[table.addr] = GroupWriter(f"affix_tables/{group_name}.c")
        group.add(section)
        affix_names[table.addr] = names
    for table in sorted(affix_flags.values(), key=lambda t: t.addr):
        affix_groups[table.sibling_addr].add(
            emit_affix_flags(table, ctx, affix_names[table.sibling_addr])
        )
    for group in affix_groups.values():
        addr, src = group.write()
        carved.append((addr, src))
        print(f"  carved {src}  ({len(group.sections)} tables)")

    emit_affix_ids(sorted(affix_tables.values(), key=lambda t: t.addr), ctx)

    # The master table itself, last.
    src = emit_item_type_infos(ctx)
    carved.append((ctx.master_symbol.addr, src))
    print(f"  carved {src}  ({len(master)} rows)")

    # The rune table (indexed by an item's runeIndex nibble).
    addr, src, count = emit_runes(ctx)
    carved.append((addr, src))
    print(f"  carved {src}  ({count} entries)")

    upsert_map(carved, owned_dirs=["item_tables", "affix_tables"])
    print(
        f"  carved {len(carved)} files; updated config/split.cfg. Now run: make verify"
    )
