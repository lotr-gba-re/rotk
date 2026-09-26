"""Carves the three per-enemy-type tables, which are ROM-adjacent and share one output file:
EnemyTypeInfos (combat stats, EnemyTypeInfo[83] @0x08057518), EnemySfxSets (EnemySfxSet[83]
@0x08057b94) and EnemyLootInfos (loot roll params, EnemyLootInfo[83] @0x08057f78).
"""

from dataclasses import dataclass
from struct import Struct

from rotkit.cheaders import extract_enum, extract_struct_fields, invert_enum
from rotkit.carve import (
    GroupWriter,
    Section,
    bit_indices,
    d_init,
    doc_comment,
    enum_entry_comment,
    flags_d_members,
    table_symbols,
    todo_lines,
    upsert_map,
)
from rotkit.rom import ROMBASE, load_rom

# EnemyTypeInfo (0x14 B): moveSpeed u32, attackFlags u16, maxHp/xpReward/attackDamage u16,
# then 8 trailing u8 (armor0,armor1,hitStun,accuracy,defense,poisonPower,fearPower,family).
_INFO = Struct("<I H H H H 8s")
# EnemySfxSet (0xC B): attack/aggro/hit/crit/death/miss, six u16.
_SFX = Struct("<6H")
# EnemyLootInfo (0x8 B): dropChanceBonus/qualityLo/qualityHi s8, prefixTierChance u8, 4 u8 tail.
_LOOT = Struct("<b b b B 4s")


@dataclass(frozen=True)
class EnemyTypeInfo:
    move_speed: int
    attack_flags: int
    max_hp: int
    xp_reward: int
    attack_damage: int
    armor: tuple[int, int]
    hit_stun: int
    accuracy: int
    defense: int
    poison_power: int
    fear_power: int
    family: int


@dataclass(frozen=True)
class EnemySfxSet:
    attack_sfx: int
    aggro_sfx: int
    hit_sfx: int
    crit_sfx: int
    death_sfx: int
    miss_sfx: int


def emit_type_table(
    name: str,
    addr: int,
    count: int,
    rom: bytes,
    attack_flags: dict[int, str],
    enemy_type_names: dict[int, str],
) -> Section:
    d_bits = extract_struct_fields("include/enemy.h", "EnemyAttackFlags", "d")
    known = bit_indices(attack_flags)
    (
        f_move_speed,
        f_attack_flags,
        f_max_hp,
        f_xp_reward,
        f_attack_damage,
        f_slash_armor,
        f_impale_armor,
        f_hit_stun,
        f_accuracy,
        f_defense,
        f_poison_power,
        f_fear_power,
        f_family,
    ) = extract_struct_fields("include/enemy.h", "EnemyTypeInfo")
    families = invert_enum(extract_enum("include/enemy.h", "EnemyFamily"))
    base_off = addr - ROMBASE
    rows = []
    for i in range(count):
        speed, flags, hp, xp, dmg, tail = _INFO.unpack_from(
            rom, base_off + i * _INFO.size
        )
        (
            armor0,
            armor1,
            hit_stun,
            accuracy,
            defense,
            poison_power,
            fear_power,
            family,
        ) = tail
        rows.append(
            EnemyTypeInfo(
                move_speed=speed,
                attack_flags=flags,
                max_hp=hp,
                xp_reward=xp,
                attack_damage=dmg,
                armor=(armor0, armor1),
                hit_stun=hit_stun,
                accuracy=accuracy,
                defense=defense,
                poison_power=poison_power,
                fear_power=fear_power,
                family=family,
            )
        )

    lines = []
    unknown_families = sorted({e.family for e in rows if e.family not in families})
    if unknown_families:
        lines += todo_lines(
            "family values with no EnemyFamily name: "
            + ", ".join(str(family) for family in unknown_families)
        )
    if lines:
        lines.append("")
    lines += [
        *doc_comment(addr),
        f"const EnemyTypeInfo {name}[{count}] = {{",
    ]
    for i, e in enumerate(rows):
        lines += [
            f"    {enum_entry_comment(i, enemy_type_names)}",
            f"    {{ .{f_move_speed} = 0x{e.move_speed:x},  // {e.move_speed / 65536:g} px/f",
            f"      .{f_attack_flags}.d = {d_init(flags_d_members(e.attack_flags, d_bits, known, 'EnemyTypeInfos.attackFlags'))},",
            f"      .{f_max_hp} = {e.max_hp},",
            f"      .{f_xp_reward} = {e.xp_reward},",
            f"      .{f_attack_damage} = {e.attack_damage},",
            f"      .{f_slash_armor} = {e.armor[0]},",
            f"      .{f_impale_armor} = {e.armor[1]},",
            f"      .{f_hit_stun} = {e.hit_stun},",
            f"      .{f_accuracy} = {e.accuracy},",
            f"      .{f_defense} = {e.defense},",
            f"      .{f_poison_power} = {e.poison_power},",
            f"      .{f_fear_power} = {e.fear_power},",
            f"      .{f_family} = {families.get(e.family, e.family)} }},",
            "",
        ]
    lines[-1] = "};"
    return Section(addr, ["enemy.h"], lines)


def emit_sfx_table(
    name: str,
    addr: int,
    count: int,
    rom: bytes,
    enemy_type_names: dict[int, str],
) -> Section:
    f_attack, f_aggro, f_hit, f_crit, f_death, f_miss = extract_struct_fields(
        "include/enemy.h", "EnemySfxSet"
    )
    base_off = addr - ROMBASE
    lines = todo_lines(
        "the u16 SFX ids are magic numbers - grow enum SfxId (include/sfx.h) as "
        "they get identified and emit them by name (0x0a = SFX_COMBAT_EVADE is "
        "already known)"
    )
    lines += [
        "",
        *doc_comment(addr),
        f"const EnemySfxSet {name}[{count}] = {{",
    ]
    for i in range(count):
        s = EnemySfxSet(*_SFX.unpack_from(rom, base_off + i * _SFX.size))
        lines += [
            f"    {enum_entry_comment(i, enemy_type_names)}",
            f"    {{ .{f_attack} = 0x{s.attack_sfx:02x},",
            f"      .{f_aggro} = 0x{s.aggro_sfx:02x},",
            f"      .{f_hit} = 0x{s.hit_sfx:02x},",
            f"      .{f_crit} = 0x{s.crit_sfx:02x},",
            f"      .{f_death} = 0x{s.death_sfx:02x},",
            f"      .{f_miss} = 0x{s.miss_sfx:02x} }},",
            "",
        ]
    lines[-1] = "};"
    return Section(addr, ["enemy.h"], lines)


def emit_loot_table(
    name: str,
    addr: int,
    count: int,
    rom: bytes,
    enemy_type_names: dict[int, str],
) -> Section:
    f_bonus, f_lo, f_hi, f_prefix_chance, f_tail = extract_struct_fields(
        "include/enemy.h", "EnemyLootInfo"
    )
    base_off = addr - ROMBASE
    lines = [
        *doc_comment(addr),
        f"const EnemyLootInfo {name}[{count}] = {{",
    ]
    for i in range(count):
        bonus, lo, hi, prefix_chance, tail = _LOOT.unpack_from(
            rom, base_off + i * _LOOT.size
        )
        lines += [
            f"    {enum_entry_comment(i, enemy_type_names)}",
            f"    {{ .{f_bonus} = {bonus},",
            f"      .{f_lo} = {lo},",
            f"      .{f_hi} = {hi},",
            f"      .{f_prefix_chance} = {prefix_chance},",
            f"      .{f_tail} = {{{tail[0]}, {tail[1]}, {tail[2]}, {tail[3]}}} }},",
            "",
        ]
    lines[-1] = "};"
    return Section(addr, ["enemy.h"], lines)


def run() -> None:
    rom = load_rom()
    # ENEMY_ATTACK_MELEE_MASK (0x3) is a test mask, not a single bit, so the per-bit
    # known_bits skip it - table entries spell the melee kind explicitly.
    attack_flags = invert_enum(extract_enum("include/enemy.h", "EnemyAttackFlag"))
    enemy_type_names = invert_enum(extract_enum("include/enemy.h", "EnemyType"))

    group = GroupWriter("enemy_tables/EnemyTables.c")
    for table in table_symbols("EnemyTypeInfo["):
        group.add(
            emit_type_table(
                table.name,
                table.addr,
                table.count,
                rom,
                attack_flags,
                enemy_type_names,
            )
        )
    for table in table_symbols("EnemySfxSet["):
        group.add(
            emit_sfx_table(table.name, table.addr, table.count, rom, enemy_type_names)
        )
    for table in table_symbols("EnemyLootInfo["):
        group.add(
            emit_loot_table(table.name, table.addr, table.count, rom, enemy_type_names)
        )
    if not group.sections:
        raise SystemExit(
            "no EnemyTypeInfo[N]/EnemySfxSet[N]/EnemyLootInfo[N] symbols in config/data.cfg"
        )
    addr, src = group.write()
    print(f"  carved {src}  ({len(group.sections)} tables)")
    upsert_map([(addr, src)], owned_dirs=["enemy_tables"])
