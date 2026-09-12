"""Carves the loot roll tables (include/loot.h): LootHealthDropChances (headed weighted
health-drop table), LootHeroItemTypeRolls (per-hero itemType thresholds), the three
distinct per-hero region-progression tables, and LootHeroRegionProgressions, the pointer
table referencing them by name. Also carves the AffixIndexRange tables (per-region prefix
ranges, per-tier suffix ranges) that parameterize loot_rollPrefix / loot_rollSuffix,
indexed with the carved/include/affix_ids.h enums.

The five ROM-adjacent roll tables share one file; the pointer table sits far away and
stays its own.
"""

from struct import Struct

from rotkit.cheaders import extract_enum, extract_struct_fields, invert_enum
from rotkit.carve import (
    GroupWriter,
    Section,
    TableSymbol,
    data_symbol,
    doc_comment,
    table_symbols,
    upsert_map,
    write_table,
)
from rotkit.rom import ROMBASE, U32, load_rom
from rotkit.carve.item_tables import loot_type_names
from rotkit.carve.mission_tables import region_annotations
from rotkit.stores import data_symbols

# Layouts the carver owns (struct bodies live in include/loot.h).
_HEALTH_DROP_HEAD = Struct("<H H")  # LootHealthDropTable head: count, field_0x2
_HEALTH_DROP_CHANCE = Struct("<H H")  # LootHealthDropChance: maxRoll, lootType
_REGION_PROGRESSION = Struct("<4B")  # LootRegionProgression: previous, next, 2 zero pad
_AFFIX_INDEX_RANGE = Struct("<4B")  # AffixIndexRange: loIndex, hiIndex, 2 zero pad

HEALTH_DROP_ENTRY_COUNT = 5  # LootHealthDropTable.entries[N] in include/loot.h
HERO_ROLL_ROW = 0x10  # bytes per heroId in LootHeroItemTypeRolls (last byte never read)


def emit_health_drop_table(
    name: str, addr: int, rom: bytes, kind_names: dict[int, str]
) -> Section:
    f_count, f_pad, f_entries = extract_struct_fields(
        "include/loot.h", "LootHealthDropTable"
    )
    f_max_roll, f_drop_kind = extract_struct_fields(
        "include/loot.h", "LootHealthDropChance"
    )
    base_off = addr - ROMBASE
    count, field_0x2 = _HEALTH_DROP_HEAD.unpack_from(rom, base_off)
    if count != HEALTH_DROP_ENTRY_COUNT:
        raise SystemExit(
            f"{name}: count is {count}, expected {HEALTH_DROP_ENTRY_COUNT}"
        )
    if field_0x2 != 0:
        raise SystemExit(f"{name}: {f_pad} is {field_0x2}, expected 0")
    rows = [
        _HEALTH_DROP_CHANCE.unpack_from(rom, base_off + 4 + i * 4) for i in range(count)
    ]

    lines = [
        *doc_comment(addr),
        f"const LootHealthDropTable {name} = {{",
        f"    .{f_count} = {count},",
        f"    .{f_pad} = {field_0x2},",
        f"    .{f_entries} = {{",
    ]
    for i, (max_roll, drop_kind) in enumerate(rows):
        kind = kind_names.get(drop_kind, f"0x{drop_kind:02x}")
        lines += [
            f"        // [0x{i:02x}]",
            f"        {{ .{f_max_roll} = {max_roll}, .{f_drop_kind} = {kind} }},",
            "",
        ]
    lines[-1] = "    },"
    lines += ["};"]
    return Section(addr, ["loot.h"], lines)


def emit_hero_item_type_rolls(
    name: str,
    addr: int,
    count: int,
    rom: bytes,
    hero_names: dict[int, str],
    field_names: list[str],
) -> Section:
    if len(field_names) != HERO_ROLL_ROW:
        raise SystemExit(
            f"LootItemTypeRolls.d has {len(field_names)} fields, "
            f"expected {HERO_ROLL_ROW}"
        )
    base_off = addr - ROMBASE
    rows: list[bytes] = [
        rom[base_off + hero * HERO_ROLL_ROW :][:HERO_ROLL_ROW] for hero in range(count)
    ]
    for hero, row in enumerate(rows):
        if row[-1] != 0:
            raise SystemExit(f"{name}[{hero}]: unread byte 15 is {row[-1]}, expected 0")
    # Absolute per-itemType drop share of each cumulative threshold (threshold[-1] = 0).
    shares: list[list[int]] = [
        [b - (row[i - 1] if i else 0) for i, b in enumerate(row[:-1])] for row in rows
    ]
    pct_width = max(len(str(share)) for row_shares in shares for share in row_shares)

    lines = [
        *doc_comment(
            addr,
            [
                "Per-hero 1..100 roll thresholds picking a dropped item's itemType",
                "(loot_generateItem): the first threshold >= the roll wins; column i is the itemType",
                "picked when threshold[i-1] < roll <= threshold[i]. Columns are the ItemType",
                "values 0..0xf; byte 0xf (.empty) is never read. The end-of-line comments give",
                "the absolute per-itemType drop share (threshold[i] - threshold[i-1]).",
            ],
        ),
        f"const LootItemTypeRolls {name}[{count}] = {{",
    ]
    # One field per line, continuation lines aligned under the first field, with the
    # share comments in a column past the widest field line.
    indent = " " * len("{ .d = { ")
    hero_rows: list[tuple[int, list[str]]] = []
    for hero, row in enumerate(rows):
        pairs = [f".{field} = {b}" for field, b in zip(field_names, row)]
        row_lines = [f"{{ .d = {{ {pairs[0]},"]
        row_lines += [f"{indent}{pair}," for pair in pairs[1:-1]]
        row_lines.append(f"{indent}{pairs[-1]} }} }},")
        hero_rows.append((hero, row_lines))
    col = max(len(line) for _, row_lines in hero_rows for line in row_lines)
    for (hero, row_lines), row_shares in zip(hero_rows, shares):
        comments = [f"// {share:>{pct_width}}%" for share in row_shares]
        comments.append("// never read")
        lines += [f"    // heroId {hero} ({hero_names[hero]})"]
        lines += [
            f"    {line:<{col}}  {comment}"
            for line, comment in zip(row_lines, comments)
        ]
        lines += [""]
    lines[-1] = "};"
    return Section(addr, ["loot.h"], lines)


def emit_region_progressions(
    name: str,
    addr: int,
    count: int,
    rom: bytes,
    heroes: list[int],
    hero_names: dict[int, str],
) -> Section:
    """heroes lists the heroIds sharing the table."""
    f_prev, f_next, f_pad = extract_struct_fields(
        "include/loot.h", "LootRegionProgression"
    )
    base_off = addr - ROMBASE
    regions = region_annotations(rom)
    if count != len(regions):
        raise SystemExit(f"{name}: {count} rows, expected {len(regions)} regions")

    sharers = ", ".join(f"{h} ({hero_names[h]})" for h in heroes)
    region_names = invert_enum(extract_enum("include/mission.h", "RegionId"))
    lines = [
        *doc_comment(
            addr,
            [
                f"Indexed by the region (enum RegionId); the campaign previous/next links of heroId {sharers}",
                "(via LootHeroRegionProgressions). field_0x2 is always zero.",
            ],
        ),
        f"const LootRegionProgression {name}[{count}] = {{",
    ]
    for i in range(count):
        prev, nxt, pad0, pad1 = _REGION_PROGRESSION.unpack_from(rom, base_off + i * 4)
        if (pad0, pad1) != (0, 0):
            raise SystemExit(f"{name}[{i}]: {f_pad} is {pad0}, {pad1}, expected 0, 0")
        if prev >= len(regions) or nxt >= len(regions):
            raise SystemExit(f"{name}[{i}]: region id out of range")
        lines += [
            f"    // [0x{i:02x}] {regions[i]}",
            f"    {{ .{f_prev} = {region_names[prev]}, .{f_next} = {region_names[nxt]} }},",
            "",
        ]
    lines[-1] = "};"
    return Section(addr, ["loot.h"], lines)


def emit_progression_pointer_table(
    table: TableSymbol,
    rom: bytes,
    names_by_addr: dict[int, str],
    hero_names: dict[int, str],
) -> str:
    base_off = table.addr - ROMBASE
    ptrs = [U32.unpack_from(rom, base_off + i * 4)[0] for i in range(table.count)]
    refs = []
    for ptr in ptrs:
        ref = names_by_addr.get(ptr)
        if ref is None:
            raise SystemExit(
                f"{table.name}: pointer 0x{ptr:08x} matches no carved data symbol; "
                "add the target to config/data.cfg"
            )
        refs.append(ref)

    lines = [
        '#include "variables.h"',
        '#include "loot.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(
            table.addr,
            [
                "One region-progression table per heroId; heroes share the underlying tables."
            ],
        ),
        f"const LootRegionProgression *const {table.name}[{table.count}] = {{",
    ]
    ref_width = max(len(ref) for ref in refs) + 1
    for i, ref in enumerate(refs):
        lines.append(f"    {ref + ',':<{ref_width}} // heroId {i} ({hero_names[i]})")
    lines += ["};"]
    return write_table(f"loot_tables/{table.name}.c", lines)


def emit_affix_index_ranges(
    table: TableSymbol, rom: bytes, sibling: TableSymbol
) -> str:
    """Carve one AffixIndexRange table (sibling = the ItemAffix table it indexes),
    lo/hi as the sibling's affix_ids.h enum members."""
    f_lo, f_hi, f_pad = extract_struct_fields("include/item.h", "AffixIndexRange")
    members = invert_enum(
        extract_enum(
            "carved/include/affix_ids.h", sibling.name.removesuffix("es") + "Id"
        )
    )
    if len(members) != sibling.count:
        raise SystemExit(
            f"{table.name}: enum covers {len(members)} of {sibling.name}'s "
            f"{sibling.count} affixes; re-run rotkit carve item-tables"
        )
    base_off = table.addr - ROMBASE
    rows = [
        _AFFIX_INDEX_RANGE.unpack_from(rom, base_off + i * _AFFIX_INDEX_RANGE.size)
        for i in range(table.count)
    ]
    for i, (lo, hi, pad0, pad1) in enumerate(rows):
        if pad0 or pad1:
            raise SystemExit(
                f"{table.name}[{i}]: {f_pad} is {pad0}, {pad1}, expected 0, 0"
            )
        if lo > hi:
            raise SystemExit(f"{table.name}[{i}]: {f_lo} {lo} > {f_hi} {hi}")
        if hi >= sibling.count:
            raise SystemExit(
                f"{table.name}[{i}]: {f_hi} {hi} is past {sibling.name} "
                f"({sibling.count} entries)"
            )

    suffix = "Suffix" in table.name
    roll = "loot_rollSuffix" if suffix else "loot_rollPrefix"
    tier = "rolled suffix tier" if suffix else "region"
    regions = None if suffix else region_annotations(rom)
    lines = [
        '#include "affix_ids.h"',
        '#include "item.h"',
        "",
        "// clang-format off",
        "",
    ]
    lines += [
        *doc_comment(
            table.addr,
            [
                f"Index ranges into {sibling.name} per {tier} ({roll}); the roll picks a",
                f"uniform affix index in [{f_lo}, {f_hi}]. {f_pad} is always zero.",
            ],
        ),
        f"const AffixIndexRange {table.name}[{table.count}] = {{",
    ]
    for i, (lo, hi, _pad0, _pad1) in enumerate(rows):
        comment = f"[{i}]" if regions is None else f"[0x{i:02x}] {regions[i]}"
        lines += [
            f"    // {comment}",
            f"    {{ .{f_lo} = {members[lo]},",
            f"      .{f_hi} = {members[hi]} }},",
            "",
        ]
    lines[-1] = "};"
    return write_table(f"loot_tables/{table.name}.c", lines)


def run() -> None:
    rom = load_rom()
    names_by_addr = {s.addr: s.name for s in data_symbols()}
    hero_names = invert_enum(extract_enum("include/player.h", "HeroId"))
    roll_fields = extract_struct_fields("include/loot.h", "LootItemTypeRolls", "d")

    # Group heroIds by their progression table so each carved table can name its sharers.
    ptr_symbol = data_symbol("LootRegionProgression *")
    ptrs = [
        U32.unpack_from(rom, ptr_symbol.addr - ROMBASE + i * 4)[0]
        for i in range(ptr_symbol.count)
    ]
    groups: dict[int, list[int]] = {}
    for hero, ptr in enumerate(ptrs):
        groups.setdefault(ptr, []).append(hero)

    merged = GroupWriter("loot_tables/LootTables.c")

    health_symbols = data_symbols("LootHealthDropTable")
    if len(health_symbols) != 1:
        raise SystemExit(
            "expected exactly one LootHealthDropTable symbol in config/data.cfg, "
            f"found {len(health_symbols)}"
        )
    health_name, health_addr = health_symbols[0].name, health_symbols[0].addr
    merged.add(emit_health_drop_table(health_name, health_addr, rom, loot_type_names()))

    hero_rolls = data_symbol("LootItemTypeRolls[")
    merged.add(
        emit_hero_item_type_rolls(
            hero_rolls.name,
            hero_rolls.addr,
            hero_rolls.count,
            rom,
            hero_names,
            roll_fields,
        )
    )

    for table in table_symbols("LootRegionProgression["):
        heroes = groups.get(table.addr)
        if heroes is None:
            raise SystemExit(
                f"{table.name} is not referenced by {ptr_symbol.name}; "
                "stray LootRegionProgression[N] symbol in config/data.cfg"
            )
        merged.add(
            emit_region_progressions(
                table.name, table.addr, table.count, rom, heroes, hero_names
            )
        )

    carved = [merged.write()]
    print(f"  carved {carved[0][1]}  ({len(merged.sections)} tables)")

    carved.append(
        (
            ptr_symbol.addr,
            emit_progression_pointer_table(ptr_symbol, rom, names_by_addr, hero_names),
        )
    )
    print(
        f"  carved carved/data/loot_tables/{ptr_symbol.name}.c  ({ptr_symbol.count} pointers)"
    )

    # Affix index-range tables (deduped: the weapon types share WeaponPrefixRegionRanges,
    # every suffixed type shares SuffixTierRanges); the sibling affix table follows from
    # the range table's name.
    affix_syms = {t.name: t for t in table_symbols("ItemAffix[")}
    for table in table_symbols("AffixIndexRange["):
        sib_name = (
            "ItemSuffixes"
            if "Suffix" in table.name
            else table.name.replace("RegionRanges", "es")
        )
        sibling = affix_syms.get(sib_name)
        if sibling is None:
            raise SystemExit(
                f"{table.name}: no ItemAffix[N] symbol {sib_name} in config/data.cfg"
            )
        carved.append((table.addr, emit_affix_index_ranges(table, rom, sibling)))
        print(f"  carved carved/data/loot_tables/{table.name}.c  ({table.count} rows)")

    upsert_map(carved, owned_dirs=["loot_tables"])
    print(
        f"  carved {len(carved)} files; updated config/split.cfg. Now run: make verify"
    )
