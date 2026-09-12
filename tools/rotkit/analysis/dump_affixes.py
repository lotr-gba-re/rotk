"""
Dump item prefixes and suffixes (the affix tables) from the ROM.

Items get a prefix ("Bronze") and/or suffix ("of the Fox") that each apply up to
four stat modifiers; item_affix_accumulateStats sums them into the stat vector and
item_affix_formatName builds "<prefix> <base> <suffix>".

Master item-type table @ 0x08068f50, row stride 0x28 (10 u32), indexed by item type:
  col2 (row+0x08) = prefix entry table for that type
  col3 (row+0x0c) = end of the prefix table (start of the next table)
  col6 (row+0x18) = end of the shared suffix table
Suffix entries are a single shared table @ 0x08068550.

Affix entry (12 bytes):
  +0  u16 nameId          Huffman string id of the affix name
  +2  u8  goldValue        gem-value weight (scene_shop_computeGemValue weights it x3/x7/x11/x13
                           by slot; prefix vs suffix vs in-area)
  +3  u8  statIdx0, +4 s8 value0     } up to four modifiers;
  +5  u8  statIdx1, +6 s8 value1     } statIdx 0x5a ('Z'=90) = empty slot.
  +7  u8  statIdx2, +8 s8 value2     } stat name = string id (statIdx + 330);
  +9  u8  statIdx3, +10 s8 value3    } so 5=Damage, 17=Strength, 18=Accuracy, 20=Defense...
  +11 u8  (unconfirmed; 0 in all observed entries)

Writes tools/out/item_affixes.tsv plus a summary; the `show` argument pretty-prints every
prefix and suffix instead.
"""

import os
import struct
from dataclasses import dataclass

from rotkit.paths import TOOLS_OUT
from rotkit.rom import ROMBASE, u32, load_rom
from rotkit.textdb import load_en_text

OUT = TOOLS_OUT / "item_affixes.tsv"


@dataclass(frozen=True)
class Mod:
    """One decoded affix modifier: stat index, signed value, resolved name."""

    stat_idx: int
    val: int
    name: str


@dataclass(frozen=True)
class ItemAffix:
    """One decoded 12-byte affix entry: name id + resolved name, gold-value tier, and its
    non-empty modifiers."""

    name_id: int
    name: str
    tier: int
    mods: tuple[Mod, ...]


@dataclass(frozen=True)
class PrefixTable:
    """A prefix entry table shared by one or more item types."""

    addr: int
    types: tuple[int, ...]
    entries: tuple[ItemAffix, ...]


MASTER = 0x08068F50  # ItemTypeInfos
ROW = 0x28  # bytes per item type
SUFFIX_TABLE = 0x08068550  # shared suffix entry table
ENTRY = 12
EMPTY_STAT = 0x5A  # 'Z' - empty modifier slot
STAT_NAME_BASE = 330  # stat name string id = statIdx + 330

# One affix entry: nameId u16, goldValue u8, four (statIdx u8, value s8) pairs. Byte 11 is
# struct padding (ItemAffix leads with a u16, so it aligns to 2) and reads 0 in every entry.
_AFFIX = struct.Struct("<H B Bb Bb Bb Bb x")

ITEM_TYPES = [
    "Knife",
    "1H Sword",
    "2H Sword",
    "Axe",
    "Mace",
    "Staff",
    "Bow",
    "Arrow",
    "Head/Body Armor",
    "Shield",
    "Accessory",
    "Cloak",
    "Gloves",
    "Shoes",
    "Unique",
]


def stat_name(en_text: dict[int, str], idx: int) -> str:
    return en_text.get(idx + STAT_NAME_BASE, f"stat{idx}")


@dataclass
class _Group:
    """Accumulator for item types sharing one prefix-table pointer."""

    end: int
    types: list[int]


def decode_affix(data: bytes, en_text: dict[int, str], addr: int) -> ItemAffix:
    name_id, tier, s0, v0, s1, v1, s2, v2, s3, v3 = _AFFIX.unpack_from(
        data, addr - ROMBASE
    )
    mods = []
    for stat_idx, val in ((s0, v0), (s1, v1), (s2, v2), (s3, v3)):
        if stat_idx != EMPTY_STAT:
            mods.append(Mod(stat_idx, val, stat_name(en_text, stat_idx)))
    return ItemAffix(
        name_id=name_id,
        name=en_text.get(name_id, f"?id{name_id}"),
        tier=tier,
        mods=tuple(mods),
    )


def collect(
    data: bytes, en_text: dict[int, str]
) -> tuple[list[PrefixTable], list[ItemAffix], int]:
    """Return (prefix_tables, suffix_entries, suffix_end).

    prefix_tables: one per distinct prefix-table pointer, each carrying its item types
    and decoded entries. suffix_entries: the shared suffix table's decoded entries.
    """
    # group item types by their (distinct) prefix table pointer
    groups: dict[int, _Group] = {}  # col2 (table addr) -> _Group
    for type_idx in range(15):
        row = MASTER + type_idx * ROW
        col2 = u32(row + 0x08)
        if col2 == 0:
            continue  # Arrow / Unique have no prefix
        col3 = u32(row + 0x0C)
        group = groups.setdefault(col2, _Group(end=col3, types=[]))
        group.types.append(type_idx)

    prefix_tables: list[PrefixTable] = []
    for addr in sorted(groups):
        group = groups[addr]
        count = (group.end - addr) // ENTRY
        entries = tuple(
            decode_affix(data, en_text, addr + i * ENTRY) for i in range(count)
        )
        prefix_tables.append(
            PrefixTable(
                addr=addr,
                types=tuple(group.types),
                entries=entries,
            )
        )

    # suffix: shared table, bounded by col6 (same for every row)
    suffix_end = u32(MASTER + 0x18)
    suffix_count = (suffix_end - SUFFIX_TABLE) // ENTRY
    suffix = [
        decode_affix(data, en_text, SUFFIX_TABLE + i * ENTRY)
        for i in range(suffix_count)
    ]
    return prefix_tables, suffix, suffix_end


def fmt_mods(mods: tuple[Mod, ...]) -> str:
    return "; ".join(f"{m.name}:{m.val:+d}" for m in mods)


def fmt_raw(mods: tuple[Mod, ...]) -> str:
    return ";".join(f"{m.stat_idx},{m.val}" for m in mods)


def write_tsv(prefix_tables: list[PrefixTable], suffix: list[ItemAffix]) -> None:
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as fh:
        fh.write(
            "# scope\titem_types\tindex\taddr\tname_id\tname\tgold_value\tmods\traw_mods\n"
        )
        fh.write(
            "# Generated by rotkit analysis dump-affixes. prefix scope lists the item types "
            "sharing that table; suffix is one table shared by all types.\n"
        )
        for table in prefix_tables:
            types = ",".join(ITEM_TYPES[t] for t in table.types)
            for i, entry in enumerate(table.entries):
                fh.write(
                    f"prefix\t{types}\t{i}\t0x{table.addr + i * ENTRY:08x}\t{entry.name_id}\t{entry.name}\t"
                    f"{entry.tier}\t{fmt_mods(entry.mods)}\t{fmt_raw(entry.mods)}\n"
                )
        for i, entry in enumerate(suffix):
            fh.write(
                f"suffix\t*\t{i}\t0x{SUFFIX_TABLE + i * ENTRY:08x}\t{entry.name_id}\t{entry.name}\t"
                f"{entry.tier}\t{fmt_mods(entry.mods)}\t{fmt_raw(entry.mods)}\n"
            )


def pretty(prefix_tables: list[PrefixTable], suffix: list[ItemAffix]) -> None:
    for table in prefix_tables:
        types = ", ".join(ITEM_TYPES[t] for t in table.types)
        print(
            f"\n=== PREFIXES for {types}  (0x{table.addr:08x}, {len(table.entries)}) ==="
        )
        for i, entry in enumerate(table.entries):
            print(f"  {i:3d}  {entry.name:<22} {fmt_mods(entry.mods)}")
    print(
        f"\n=== SUFFIXES (0x{SUFFIX_TABLE:08x}, {len(suffix)}; shared by all types) ==="
    )
    for i, entry in enumerate(suffix):
        print(f"  {i:3d}  {entry.name:<22} {fmt_mods(entry.mods)}")


def run(show: bool = False) -> None:
    data = load_rom()
    en_text = load_en_text()
    prefix_tables, suffix, suffix_end = collect(data, en_text)

    if show:
        pretty(prefix_tables, suffix)
        return

    write_tsv(prefix_tables, suffix)
    total_pref = sum(len(t.entries) for t in prefix_tables)
    print(f"Wrote {OUT}")
    print(
        f"  {len(prefix_tables)} prefix tables, {total_pref} prefixes; "
        f"{len(suffix)} suffixes (0x{SUFFIX_TABLE:08x}..0x{suffix_end:08x})"
    )
    for table in prefix_tables:
        print(
            f"    prefix 0x{table.addr:08x}  {len(table.entries):3d}  "
            f"{', '.join(ITEM_TYPES[i] for i in table.types)}"
        )
