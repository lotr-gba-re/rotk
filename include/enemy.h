#pragma once

#include "actor.h"
#include "types.h"

/**
 * Creature family (EnemyTypeInfo.category); selects the attacker's STAT_DAMAGE_TO_* bonus in
 * combat_getEnemyTypeDamageBonus. 0 = no family (no per-family damage bonus applies).
 * 1-byte enum. agbcc honors __attribute__((packed)); Ghidra's C parser hardcodes enum size to 4
 * (issue #4518) but apply_symbols.py resizes enums to their minimal width after parsing, keeping
 * the packed EnemyTypeInfo layout intact.
 */
typedef enum CreatureCategory
{
    CREATURE_NONE = 0,
    CREATURE_ANIMALS = 1,
    CREATURE_CREBAIN = 2,
    CREATURE_WARGS = 3,
    CREATURE_ORCS = 4,
    CREATURE_URUKHAI = 5,
    CREATURE_NAZGUL = 6,
} __attribute__((packed)) CreatureCategory;
/**
 * EnemyTypeInfo.attackFlags - describes how this enemy's own attack hits a player, read by the
 * enemy->player armor path. Bits 0/1 are the two
 * melee kinds (a table entry sets one or the other, never both); either one means melee (the
 * player's Melee Armor applies, slash vs impale picks hit SFX 7/8).
 */
enum EnemyAttackFlag
{
    ENEMY_ATTACK_MELEE_IMPALE = 1 << 0,
    ENEMY_ATTACK_MELEE_SLASH = 1 << 1,
    // any-melee test mask, not a table value
    ENEMY_ATTACK_MELEE_MASK = ENEMY_ATTACK_MELEE_IMPALE | ENEMY_ATTACK_MELEE_SLASH,
    // ranged: the player's Missile Armor applies instead
    ENEMY_ATTACK_RANGED = 1 << 2,
    // inflicts poison, magnitude poisonPower
    ENEMY_ATTACK_POISON = 1 << 3,
    // fire: adds the player's Fire Protection to armor
    ENEMY_ATTACK_FIRE = 1 << 4,
    // inflicts fear, magnitude fearPower
    ENEMY_ATTACK_FEAR = 1 << 5,
};

/**
 * EnemyTypeInfo.attackFlags; matched code tests the ENEMY_ATTACK_* masks on the word view
 * instead. Packed (union and bitfield struct): agbcc would otherwise pad the union to 4-byte
 * size/alignment and shift the EnemyTypeInfo layout.
 */
typedef union EnemyAttackFlags {
    u16 p;

    struct
    {
        u16 meleeImpale : 1;  // 1 << 0
        u16 meleeSlash : 1;   // 1 << 1
        u16 ranged : 1;       // 1 << 2: the player's Missile Armor applies instead
        u16 poison : 1;       // 1 << 3: inflicts poison, magnitude poisonPower
        u16 fire : 1;         // 1 << 4: adds the player's Fire Protection to armor
        u16 fear : 1;         // 1 << 5: inflicts fear, magnitude fearPower
        u16 field_bit_6 : 1;  // 1 << 6
        u16 field_bit_7 : 1;  // 1 << 7
        u16 field_bit_8 : 1;  // 1 << 8
        u16 field_bit_9 : 1;  // 1 << 9
        u16 field_bit_10 : 1; // 1 << 10
        u16 field_bit_11 : 1; // 1 << 11
        u16 field_bit_12 : 1; // 1 << 12
        u16 field_bit_13 : 1; // 1 << 13
        u16 field_bit_14 : 1; // 1 << 14
        u16 field_bit_15 : 1; // 1 << 15
    } __attribute__((packed)) d;
} __attribute__((packed)) EnemyAttackFlags;
// Buffs every on-screen enemy gets while g_FlagBearerCount (on-screen, living flag bearers:
// ACTOR_TYPE_ENEMY kind 0xd, unitType 0x1c) is nonzero, along with a status aura FX.
#define FLAG_BEARER_ACCURACY_BONUS 20
#define FLAG_BEARER_DEFENSE_BONUS 20
#define FLAG_BEARER_DAMAGE_BONUS_PERCENT 50

/**
 * Frames of poison/fear status duration inflicted per point of the attacking enemy's
 * poisonPower / fearPower, before the player's
 * resist cut.
 */
#define STATUS_FRAMES_PER_POWER 30

/**
 * Per-enemy-type record; EnemyTypes @0x08057518 (83 entries, indexed by Actor.unitType).
 * All player-facing combat stats of an enemy come from here - enemies have no per-instance stat
 * vector like the players' Player.stats.
 */
typedef struct EnemyTypeInfo
{
    /**
     * walk speed, 16.16 fixed px/frame; seeded into Actor.moveSpeed
     * when the behavior state machine enters a walking state
     */
    u32 moveSpeed;

    /** EnemyAttackFlags (mask view: .p & ENEMY_ATTACK_*) */
    EnemyAttackFlags attackFlags;

    /**
     * base max HP; combat_getEnemyMaxHp scales it by difficulty (OPTION_FLAG_DIFFICULTY_HARD/2 ->
     * +200%/+400%) plus two +50% global modifiers
     */
    u16 maxHp;

    /**
     * XP awarded to the party when this enemy dies, full/halved/zero by the
     * kill-credit code in Actor.lastHitByPlayerIndex
     */
    u16 xpReward;

    /**
     * base attack damage vs players; combat_getEnemyMaxDamage scales it by difficulty
     * (OPTION_FLAG_DIFFICULTY_HARD/2 -> +200%/+400%) plus two +50% global modifiers
     */
    u16 attackDamage;

    /**
     * Melee armor vs a player's slash attack (attacker weapon's PLAYER_COMBAT_FLAG_DAMAGE_TYPE
     * set). Doubles as the generic armor: enemy-vs-enemy, non-player, hazard, and player
     * ranged hits always subtract this one.
     */
    u8 slashArmor;
    /**
     * Melee armor vs a player's impale attack; only the player->enemy melee path reads it.
     * The two armors genuinely differ across the table.
     */
    u8 impaleArmor;

    // Flinch/stagger timer seeded into victim->hitStunTimer on a surviving hit.
    u8 hitStun;

    /**
     * base to-hit accuracy vs players; combat_getEnemyAccuracy scales it by difficulty
     * (OPTION_FLAG_DIFFICULTY_HARD/2 -> +50%/+100%) plus smaller global modifiers
     */
    u8 accuracy;

    /**
     * base defense; the victim side of combat_rollAccuracy when a player (or another enemy)
     * attacks this enemy, difficulty-scaled by combat_getEnemyDefense like accuracy
     */
    u8 defense;

    // Base power of the poison / fear this enemy's attack inflicts (x30 frames of status duration,
    // then reduced by the player's Poison/Fear Resistance); gated by ENEMY_ATTACK_POISON / _FEAR.
    u8 poisonPower;

    u8 fearPower;
    CreatureCategory category;
} EnemyTypeInfo;
/**
 * Per-enemy-type sound set; EnemySfxSets @0x08057b94 (83 entries, parallel to EnemyTypes,
 * indexed by Actor.unitType).
 */
typedef struct EnemySfx
{
    /** starting an attack (behavior state machine) */
    u16 attackSfx;

    /** spotting / engaging a player */
    u16 aggroSfx;

    /** a hit lands on this enemy */
    u16 hitSfx;

    /** a critical hit lands on this enemy */
    u16 critSfx;

    /** this enemy dies */
    u16 deathSfx;

    /** an attack on this enemy whiffs (failed to-hit roll) */
    u16 missSfx;
} EnemySfx;
/**
 * Per-enemy-type loot record; EnemyLootInfos @0x08057f78 (83 entries, parallel to EnemyTypes,
 * indexed by Actor.unitType). Read by the loot rolls.
 */
typedef struct EnemyLootInfo
{
    /** signed bonus to the 1..100 drop-chance roll; a drop happens on roll > 45 */
    s8 dropChanceBonus;

    /**
     * quality-roll bounds: loot_generateDrop rolls rng_rollRange(qualityLo + 1,
     * qualityHi + 100); <= 45 a gem stack, 46..70 a health drop, >= 71 an equippable item
     */
    s8 qualityLo;
    s8 qualityHi;

    /** base chance (plus the global loot tier) that a generated item gets a prefix */
    u8 prefixTierChance;

    /** +4 carries 95/100/105/110 per entry, +5..+7 are always zero */
    u8 field_0x4[4];
} EnemyLootInfo;
u16 combat_getEnemyAccuracy(u16 enemyIndex);
u16 combat_getEnemyDefense(u16 enemyIndex);
u16 combat_getEnemyMaxHp(u16 enemyIndex);
u16 combat_getEnemyMaxDamage(u16 enemyIndex);
void combat_awardKillXp(Actor *victim);
