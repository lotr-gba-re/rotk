#pragma once

/**
 * Indices into Player.stats[90].
 * The same indices are also used for item affix modifiers and PassiveSkill stat records.
 */
enum StatIndex
{
    STAT_DAMAGE_SLASH = 0,  // "Melee Dmg" in the item UI
    STAT_DAMAGE_IMPALE = 1, // "Impaling Dmg" in the item UI
    STAT_DAMAGE_FIRE = 2,
    STAT_DAMAGE_POISON = 3,
    STAT_DAMAGE_TWOHAND = 4,
    STAT_DAMAGE_FLAT = 5, // Flat damage bonus, e.g. from equipment
    STAT_DAMAGE_ALL_ALLIES = 6,
    STAT_DAMAGE_TO_ANIMALS = 7,
    STAT_DAMAGE_TO_CREBAIN = 8,
    STAT_DAMAGE_TO_WARGS = 9,
    STAT_DAMAGE_TO_ORCS = 10,
    STAT_DAMAGE_TO_URUKHAI = 11,
    STAT_DAMAGE_TO_NAZGUL = 12,
    STAT_MELEE_ARMOR = 13,
    STAT_MISSILE_ARMOR = 14,
    STAT_FIRE_PROTECTION = 15,
    STAT_CRITICAL_ARMOR = 16,
    STAT_STRENGTH = 17, // primary stat strength
    STAT_ACCURACY = 18, // primary stat accuracy
    STAT_HEALTH = 19,   // primary stat health
    STAT_DEFENSE = 20,  // primary stat defense
    STAT_COURAGE = 21,  // primary stat courage
    STAT_MAX_HP = 22,
    STAT_MAX_SPIRIT = 23,
    STAT_SPEED_PERCENT = 24,
    STAT_HP_PER_KILL = 25,
    STAT_SPIRIT_PER_KILL = 26,
    STAT_HP_PERCENT_PER_KILL = 27,
    STAT_SPIRIT_PER_HIT = 28,
    STAT_HP_REGEN = 29,
    STAT_SPIRIT_REGEN = 30,
    STAT_EXTRA_GEMS_PERCENT = 31,
    STAT_EXTRA_TREASURE_PERCENT = 32,
    STAT_EXTRA_HP_FROM_FOOD_PERCENT = 33,
    STAT_EXTRA_EXP_PERCENT = 34,
    STAT_CRIT_CHANCE_PERCENT = 35,
    STAT_CRIT_DAMAGE = 36,
    STAT_PUSH_FOE_BACK = 37,
    STAT_FOE_STUN_TIME = 38,
    STAT_THORNS_DAMAGE = 39,
    STAT_CORRUPTION_RESIST_PERCENT = 40,
    STAT_FEAR_RESIST_PERCENT = 41,
    STAT_POISON_RESIST_PERCENT = 42,
    STAT_PARRY_PERCENT = 43,
    STAT_DODGE_PERCENT = 44,
    STAT_RIPOSTE_PERCENT = 45,
    STAT_BLOCK_PERCENT = 46,
    STAT_EXTRA_PROJECTILES = 47,
    STAT_KNOCKBACK = 48,
    STAT_IMMOBILIZE = 49,
    STAT_ARMOR_PIERCE = 50,
    STAT_FRAGILE = 51, // BUG: Only works in prefixes and only if the item has this in stat1
                       // position! See combat_fragileOnKill
    STAT_WISDOM = 52,
    STAT_SUNBURST = 53,
    STAT_NIGHTBURST = 54,
    STAT_CURSED = 55,
    STAT_FIREBURST = 56,
    STAT_BACKSTAB = 57,
    STAT_GEMFINDER = 58,
    STAT_VALUABLE = 59,
    STAT_CHEAP = 60,
    STAT_VENOMOUS = 61,
    STAT_MEDITATIVE = 62,
    STAT_ALL_PRIMARY_STATS =
        63, // adds value to all primary stats: strength/health/courage/accuracy/defense
    STAT_HP_FROM_HERBS = 64,
    STAT_DAMAGE_WITH_SHIELD = 65, // flat damage bonus when a shield is equipped
    STAT_DAMAGE_WITH_BLADE =
        66, // flat damage bonus when a blade (knife / 1H sword / 2H sword) is equipped
    STAT_DAMAGE_WITH_AXE = 67, // flat damage bonus when an axe is equipped
    STAT_DAMAGE_WITH_BOW = 68, // flat damage bonus when a bow is equipped
    // Woodsman skill; its text promises "+2 damage / level to spiders, wargs, and mumakil".
    // BUG: nothing reads it, so the skill has no effect
    STAT_WOODSMAN_DAMAGE = 69,
    STAT_LOW_HP_DAMAGE = 70, // extra flat damage while at <= 25% of max HP (Fighter's Resolve)
    // Iron Will: while HP% is below the threshold, each HP regen tick also adds STAT_LOW_HP_REGEN
    STAT_LOW_HP_REGEN_THRESHOLD_PERCENT = 71,
    STAT_LOW_HP_REGEN = 72,
    STAT_RANGED_DAMAGE = 73, // flat damage bonus on player projectile hits (Wisdom of the Ages)
    STAT_REVIVE_CHANCE_PERCENT = 74, // chance on death to revive at half max HP (Last Stand)
    STAT_ARROW_PARRY_PERCENT = 75,
    STAT_ARROW_SPEED_PERCENT = 76,      // player projectile speed bonus (Galadriel's Blessing)
    STAT_INSTAKILL_CHANCE_PERCENT = 77, // Rage of the North
    STAT_UNKNOWN_78 = 78,
    STAT_DOUBLE_AXE_THROW = 79, // Gloin's Double Axes: the axe throw launches two axes
    // chance when struck to spawn an invulnerability bubble (Defender's Fury)
    STAT_INVULNERABILITY_PROC_PERCENT = 80,
    STAT_UNKNOWN_81 = 81, // Defender's Fury adds 8 per level
    // chance when struck to spawn a triple-damage buff bubble (The Precious)
    STAT_TRIPLE_DAMAGE_PROC_PERCENT = 82,
    STAT_UNKNOWN_83 = 83,
    // activeSkill_getLeveledValue uses one extra level while > 0 (Servant of the Secret Fire)
    STAT_ACTIVE_SKILL_LEVEL_BONUS = 84,
    STAT_UNKNOWN_85 = 85,
    STAT_UNKNOWN_86 = 86,
    STAT_UNKNOWN_87 = 87,
    STAT_UNKNOWN_88 = 88,
    STAT_SPIRIT_REGEN_PERCENT = 89, // spirit regen tick bonus, +1 at 100 (Spirit of Middle-earth)
    STAT_NONE = 90,                 // empty affix modifier slot sentinel
};
