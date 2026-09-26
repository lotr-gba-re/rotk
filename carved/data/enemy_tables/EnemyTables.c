#include "enemy.h"

// clang-format off

/** @romaddress 0x08057518 */
const EnemyTypeInfo EnemyTypeInfos[83] = {
    // [0] ENEMY_TYPE_GOBLIN_SCOUT
    { .moveSpeed = 0x2c000,  // 2.75 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 15,
      .xpReward = 23,
      .attackDamage = 36,
      .slashArmor = 0,
      .impaleArmor = 2,
      .hitStun = 10,
      .accuracy = 25,
      .defense = 25,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [1] ENEMY_TYPE_GOBLIN
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 19,
      .xpReward = 28,
      .attackDamage = 32,
      .slashArmor = 1,
      .impaleArmor = 3,
      .hitStun = 10,
      .accuracy = 30,
      .defense = 35,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [2] ENEMY_TYPE_GOBLIN_ELITE
    { .moveSpeed = 0x30000,  // 3 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 72,
      .xpReward = 60,
      .attackDamage = 39,
      .slashArmor = 3,
      .impaleArmor = 5,
      .hitStun = 10,
      .accuracy = 40,
      .defense = 45,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [3] ENEMY_TYPE_GOBLIN_ARCHER
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .ranged = 1 },
      .maxHp = 14,
      .xpReward = 35,
      .attackDamage = 25,
      .slashArmor = 0,
      .impaleArmor = 2,
      .hitStun = 10,
      .accuracy = 30,
      .defense = 25,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [4] ENEMY_TYPE_GOBLIN_ARCHER_ELITE
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .ranged = 1 },
      .maxHp = 56,
      .xpReward = 70,
      .attackDamage = 36,
      .slashArmor = 3,
      .impaleArmor = 5,
      .hitStun = 10,
      .accuracy = 45,
      .defense = 35,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [5] ENEMY_TYPE_ORC_DRUMMER
    { .moveSpeed = 0x30000,  // 3 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 25,
      .xpReward = 40,
      .attackDamage = 47,
      .slashArmor = 2,
      .impaleArmor = 5,
      .hitStun = 0,
      .accuracy = 25,
      .defense = 38,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [6] ENEMY_TYPE_ORC_PITCHFORK_1
    { .moveSpeed = 0x20000,  // 2 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 19,
      .xpReward = 39,
      .attackDamage = 36,
      .slashArmor = 2,
      .impaleArmor = 2,
      .hitStun = 10,
      .accuracy = 40,
      .defense = 25,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [7] ENEMY_TYPE_ORC_PITCHFORK_2
    { .moveSpeed = 0x20000,  // 2 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 19,
      .xpReward = 39,
      .attackDamage = 39,
      .slashArmor = 2,
      .impaleArmor = 2,
      .hitStun = 10,
      .accuracy = 40,
      .defense = 20,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [8] ENEMY_TYPE_ORC_PITCHFORK_3
    { .moveSpeed = 0x20000,  // 2 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 19,
      .xpReward = 35,
      .attackDamage = 38,
      .slashArmor = 2,
      .impaleArmor = 2,
      .hitStun = 10,
      .accuracy = 40,
      .defense = 27,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [9] ENEMY_TYPE_ORC_PITCHFORK_4
    { .moveSpeed = 0x20000,  // 2 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 19,
      .xpReward = 39,
      .attackDamage = 38,
      .slashArmor = 2,
      .impaleArmor = 2,
      .hitStun = 10,
      .accuracy = 45,
      .defense = 35,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [10] ENEMY_TYPE_ORC_WARRIOR_AXE
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 45,
      .xpReward = 75,
      .attackDamage = 65,
      .slashArmor = 4,
      .impaleArmor = 4,
      .hitStun = 30,
      .accuracy = 70,
      .defense = 45,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [11] ENEMY_TYPE_ORC_WARRIOR_ORC_SWORD
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 45,
      .xpReward = 75,
      .attackDamage = 70,
      .slashArmor = 4,
      .impaleArmor = 4,
      .hitStun = 30,
      .accuracy = 70,
      .defense = 40,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [12] ENEMY_TYPE_ORC_WARRIOR_MACE
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 55,
      .xpReward = 65,
      .attackDamage = 55,
      .slashArmor = 4,
      .impaleArmor = 4,
      .hitStun = 30,
      .accuracy = 70,
      .defense = 47,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [13] ENEMY_TYPE_ORC_WARRIOR_LONGSWORD
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 45,
      .xpReward = 75,
      .attackDamage = 60,
      .slashArmor = 4,
      .impaleArmor = 4,
      .hitStun = 30,
      .accuracy = 75,
      .defense = 55,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [14] ENEMY_TYPE_ORC_WARRIOR_ELITE_AXE
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 135,
      .xpReward = 150,
      .attackDamage = 75,
      .slashArmor = 8,
      .impaleArmor = 8,
      .hitStun = 30,
      .accuracy = 80,
      .defense = 65,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [15] ENEMY_TYPE_ORC_WARRIOR_ELITE_ORC_SWORD
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 135,
      .xpReward = 150,
      .attackDamage = 80,
      .slashArmor = 8,
      .impaleArmor = 8,
      .hitStun = 30,
      .accuracy = 80,
      .defense = 60,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [16] ENEMY_TYPE_ORC_WARRIOR_ELITE_MACE
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 145,
      .xpReward = 140,
      .attackDamage = 70,
      .slashArmor = 8,
      .impaleArmor = 8,
      .hitStun = 30,
      .accuracy = 80,
      .defense = 67,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [17] ENEMY_TYPE_ORC_WARRIOR_ELITE_LONGSWORD
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 135,
      .xpReward = 150,
      .attackDamage = 70,
      .slashArmor = 8,
      .impaleArmor = 8,
      .hitStun = 30,
      .accuracy = 85,
      .defense = 75,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [18] ENEMY_TYPE_ORC_CAPTAIN_ARMORED
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 60,
      .xpReward = 95,
      .attackDamage = 52,
      .slashArmor = 5,
      .impaleArmor = 5,
      .hitStun = 30,
      .accuracy = 80,
      .defense = 75,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [19] ENEMY_TYPE_ORC_CAPTAIN_ARMORED_ELITE
    { .moveSpeed = 0x2c000,  // 2.75 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 200,
      .xpReward = 185,
      .attackDamage = 70,
      .slashArmor = 7,
      .impaleArmor = 7,
      .hitStun = 30,
      .accuracy = 110,
      .defense = 85,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [20] ENEMY_TYPE_ORC_ARCHER
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .ranged = 1 },
      .maxHp = 30,
      .xpReward = 90,
      .attackDamage = 32,
      .slashArmor = 1,
      .impaleArmor = 1,
      .hitStun = 30,
      .accuracy = 70,
      .defense = 50,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [21] ENEMY_TYPE_ORC_ARCHER_ELITE
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .ranged = 1 },
      .maxHp = 90,
      .xpReward = 155,
      .attackDamage = 37,
      .slashArmor = 15,
      .impaleArmor = 10,
      .hitStun = 30,
      .accuracy = 70,
      .defense = 60,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [22] ENEMY_TYPE_ORC_ARCHER_FIRE
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .ranged = 1, .fire = 1 },
      .maxHp = 35,
      .xpReward = 120,
      .attackDamage = 35,
      .slashArmor = 1,
      .impaleArmor = 1,
      .hitStun = 30,
      .accuracy = 60,
      .defense = 55,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [23] ENEMY_TYPE_ORC_ARCHER_POISON
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .ranged = 1, .poison = 1 },
      .maxHp = 35,
      .xpReward = 120,
      .attackDamage = 30,
      .slashArmor = 1,
      .impaleArmor = 1,
      .hitStun = 30,
      .accuracy = 60,
      .defense = 55,
      .poisonPower = 11,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [24] ENEMY_TYPE_ORC_CAPTAIN_UNARMORED
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 97,
      .xpReward = 46,
      .attackDamage = 60,
      .slashArmor = 1,
      .impaleArmor = 1,
      .hitStun = 30,
      .accuracy = 35,
      .defense = 40,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [25] ENEMY_TYPE_ORC_CAPTAIN_UNARMORED_ELITE
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 155,
      .xpReward = 75,
      .attackDamage = 73,
      .slashArmor = 1,
      .impaleArmor = 1,
      .hitStun = 30,
      .accuracy = 60,
      .defense = 35,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [26] ENEMY_TYPE_ORC_AXE_THROWER
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 45,
      .xpReward = 99,
      .attackDamage = 60,
      .slashArmor = 6,
      .impaleArmor = 6,
      .hitStun = 30,
      .accuracy = 60,
      .defense = 60,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [27] ENEMY_TYPE_ORC_AXE_THROWER_ELITE
    { .moveSpeed = 0x2c000,  // 2.75 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 150,
      .xpReward = 195,
      .attackDamage = 73,
      .slashArmor = 8,
      .impaleArmor = 8,
      .hitStun = 30,
      .accuracy = 100,
      .defense = 65,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [28] ENEMY_TYPE_ORC_FLAGBEARER
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .ranged = 1 },
      .maxHp = 35,
      .xpReward = 110,
      .attackDamage = 50,
      .slashArmor = 3,
      .impaleArmor = 3,
      .hitStun = 10,
      .accuracy = 70,
      .defense = 40,
      .poisonPower = 0,
      .fearPower = 5,
      .family = ENEMY_FAMILY_ORCS },

    // [29] ENEMY_TYPE_ORC_HOPLITE
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 50,
      .xpReward = 115,
      .attackDamage = 50,
      .slashArmor = 7,
      .impaleArmor = 3,
      .hitStun = 10,
      .accuracy = 70,
      .defense = 70,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [30] ENEMY_TYPE_ORC_HOPLITE_ELITE
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 220,
      .xpReward = 185,
      .attackDamage = 70,
      .slashArmor = 10,
      .impaleArmor = 4,
      .hitStun = 10,
      .accuracy = 115,
      .defense = 100,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [31] ENEMY_TYPE_URUK
    { .moveSpeed = 0x20000,  // 2 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 16,
      .xpReward = 17,
      .attackDamage = 27,
      .slashArmor = 0,
      .impaleArmor = 1,
      .hitStun = 10,
      .accuracy = 22,
      .defense = 15,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_URUKHAI },

    // [32] ENEMY_TYPE_URUK_ELITE
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 45,
      .xpReward = 45,
      .attackDamage = 37,
      .slashArmor = 3,
      .impaleArmor = 4,
      .hitStun = 10,
      .accuracy = 35,
      .defense = 25,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_URUKHAI },

    // [33] ENEMY_TYPE_URUK_CROSSBOWMAN
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .ranged = 1 },
      .maxHp = 13,
      .xpReward = 23,
      .attackDamage = 23,
      .slashArmor = 1,
      .impaleArmor = 3,
      .hitStun = 10,
      .accuracy = 20,
      .defense = 15,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_URUKHAI },

    // [34] ENEMY_TYPE_URUK_CROSSBOWMAN_ELITE
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .ranged = 1 },
      .maxHp = 40,
      .xpReward = 55,
      .attackDamage = 33,
      .slashArmor = 3,
      .impaleArmor = 4,
      .hitStun = 10,
      .accuracy = 30,
      .defense = 27,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_URUKHAI },

    // [35] ENEMY_TYPE_URUK_BERSERKER
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .meleeSlash = 1, .fire = 1 },
      .maxHp = 16,
      .xpReward = 35,
      .attackDamage = 27,
      .slashArmor = 1,
      .impaleArmor = 4,
      .hitStun = 10,
      .accuracy = 90,
      .defense = 26,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_URUKHAI },

    // [36] ENEMY_TYPE_URUK_BERSERKER_ELITE
    { .moveSpeed = 0x2c000,  // 2.75 px/f
      .attackFlags.d = { .meleeSlash = 1, .fire = 1 },
      .maxHp = 40,
      .xpReward = 55,
      .attackDamage = 37,
      .slashArmor = 3,
      .impaleArmor = 5,
      .hitStun = 10,
      .accuracy = 95,
      .defense = 38,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_URUKHAI },

    // [37] ENEMY_TYPE_WILDMAN
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 30,
      .xpReward = 41,
      .attackDamage = 44,
      .slashArmor = 4,
      .impaleArmor = 2,
      .hitStun = 30,
      .accuracy = 42,
      .defense = 32,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [38] ENEMY_TYPE_WILDMAN_ELITE
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 95,
      .xpReward = 72,
      .attackDamage = 58,
      .slashArmor = 7,
      .impaleArmor = 3,
      .hitStun = 30,
      .accuracy = 52,
      .defense = 42,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [39] ENEMY_TYPE_MOUNTAIN_TROLL
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeImpale = 1 },
      .maxHp = 350,
      .xpReward = 550,
      .attackDamage = 95,
      .slashArmor = 8,
      .impaleArmor = 8,
      .hitStun = 35,
      .accuracy = 90,
      .defense = 45,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [40] ENEMY_TYPE_HALF_TROLL
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeImpale = 1 },
      .maxHp = 250,
      .xpReward = 350,
      .attackDamage = 80,
      .slashArmor = 4,
      .impaleArmor = 4,
      .hitStun = 35,
      .accuracy = 60,
      .defense = 35,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [41] ENEMY_TYPE_SPIDER
    { .moveSpeed = 0x2c000,  // 2.75 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 16,
      .xpReward = 45,
      .attackDamage = 28,
      .slashArmor = 1,
      .impaleArmor = 4,
      .hitStun = 10,
      .accuracy = 40,
      .defense = 30,
      .poisonPower = 4,
      .fearPower = 0,
      .family = ENEMY_FAMILY_WARGS },

    // [42] ENEMY_TYPE_SPIDER_RED
    { .moveSpeed = 0x30000,  // 3 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 74,
      .xpReward = 90,
      .attackDamage = 20,
      .slashArmor = 1,
      .impaleArmor = 4,
      .hitStun = 10,
      .accuracy = 50,
      .defense = 30,
      .poisonPower = 8,
      .fearPower = 0,
      .family = ENEMY_FAMILY_WARGS },

    // [43] ENEMY_TYPE_SPIDER_GREEN
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1, .poison = 1 },
      .maxHp = 18,
      .xpReward = 75,
      .attackDamage = 28,
      .slashArmor = 1,
      .impaleArmor = 4,
      .hitStun = 10,
      .accuracy = 40,
      .defense = 30,
      .poisonPower = 6,
      .fearPower = 0,
      .family = ENEMY_FAMILY_WARGS },

    // [44] ENEMY_TYPE_SPIDER_SMALL
    { .moveSpeed = 0x30000,  // 3 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 5,
      .xpReward = 25,
      .attackDamage = 19,
      .slashArmor = 0,
      .impaleArmor = 5,
      .hitStun = 10,
      .accuracy = 50,
      .defense = 20,
      .poisonPower = 2,
      .fearPower = 0,
      .family = ENEMY_FAMILY_WARGS },

    // [45] ENEMY_TYPE_CORSAIR
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 45,
      .xpReward = 65,
      .attackDamage = 55,
      .slashArmor = 4,
      .impaleArmor = 6,
      .hitStun = 30,
      .accuracy = 55,
      .defense = 45,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [46] ENEMY_TYPE_CORSAIR_ELITE
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 170,
      .xpReward = 115,
      .attackDamage = 65,
      .slashArmor = 6,
      .impaleArmor = 8,
      .hitStun = 30,
      .accuracy = 75,
      .defense = 55,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [47] ENEMY_TYPE_CORSAIR_CAPTAIN
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 250,
      .xpReward = 345,
      .attackDamage = 88,
      .slashArmor = 7,
      .impaleArmor = 9,
      .hitStun = 30,
      .accuracy = 95,
      .defense = 70,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [48] ENEMY_TYPE_WARG
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 65,
      .xpReward = 90,
      .attackDamage = 75,
      .slashArmor = 1,
      .impaleArmor = 1,
      .hitStun = 10,
      .accuracy = 50,
      .defense = 30,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_WARGS },

    // [49] ENEMY_TYPE_WARG_ELITE
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 250,
      .xpReward = 180,
      .attackDamage = 95,
      .slashArmor = 4,
      .impaleArmor = 4,
      .hitStun = 10,
      .accuracy = 70,
      .defense = 40,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_WARGS },

    // [50] ENEMY_TYPE_HARADRIM_ARCHER
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .ranged = 1 },
      .maxHp = 44,
      .xpReward = 85,
      .attackDamage = 42,
      .slashArmor = 1,
      .impaleArmor = 1,
      .hitStun = 30,
      .accuracy = 90,
      .defense = 40,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [51] ENEMY_TYPE_HARADRIM_ARCHER_ELITE
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .ranged = 1 },
      .maxHp = 144,
      .xpReward = 145,
      .attackDamage = 52,
      .slashArmor = 2,
      .impaleArmor = 2,
      .hitStun = 30,
      .accuracy = 100,
      .defense = 40,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_ORCS },

    // [52] ENEMY_TYPE_EASTERLING_HEAVY
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 50,
      .xpReward = 75,
      .attackDamage = 68,
      .slashArmor = 6,
      .impaleArmor = 6,
      .hitStun = 30,
      .accuracy = 70,
      .defense = 50,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [53] ENEMY_TYPE_EASTERLING_LIGHT
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 45,
      .xpReward = 65,
      .attackDamage = 74,
      .slashArmor = 4,
      .impaleArmor = 4,
      .hitStun = 30,
      .accuracy = 90,
      .defense = 80,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [54] ENEMY_TYPE_EASTERLING_ELITE
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 190,
      .xpReward = 155,
      .attackDamage = 98,
      .slashArmor = 8,
      .impaleArmor = 8,
      .hitStun = 30,
      .accuracy = 120,
      .defense = 90,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [55] ENEMY_TYPE_CREBAIN
    { .moveSpeed = 0x30000,  // 3 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 3,
      .xpReward = 12,
      .attackDamage = 14,
      .slashArmor = 0,
      .impaleArmor = 0,
      .hitStun = 0,
      .accuracy = 40,
      .defense = 10,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_CREBAIN },

    // [56] ENEMY_TYPE_BAT
    { .moveSpeed = 0x30000,  // 3 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 6,
      .xpReward = 14,
      .attackDamage = 19,
      .slashArmor = 0,
      .impaleArmor = 0,
      .hitStun = 0,
      .accuracy = 40,
      .defense = 10,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_CREBAIN },

    // [57] ENEMY_TYPE_GHOST
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1, .fear = 1 },
      .maxHp = 32,
      .xpReward = 70,
      .attackDamage = 32,
      .slashArmor = 2,
      .impaleArmor = 5,
      .hitStun = 10,
      .accuracy = 60,
      .defense = 60,
      .poisonPower = 0,
      .fearPower = 5,
      .family = ENEMY_FAMILY_NONE },

    // [58] ENEMY_TYPE_NAZGUL
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1, .poison = 1, .fear = 1 },
      .maxHp = 75,
      .xpReward = 350,
      .attackDamage = 75,
      .slashArmor = 20,
      .impaleArmor = 20,
      .hitStun = 10,
      .accuracy = 100,
      .defense = 100,
      .poisonPower = 10,
      .fearPower = 15,
      .family = ENEMY_FAMILY_NAZGUL },

    // [59] ENEMY_TYPE_FLY_SWARM
    { .moveSpeed = 0x30000,  // 3 px/f
      .attackFlags.d = { .meleeSlash = 1, .poison = 1 },
      .maxHp = 18,
      .xpReward = 35,
      .attackDamage = 30,
      .slashArmor = 0,
      .impaleArmor = 11,
      .hitStun = 10,
      .accuracy = 40,
      .defense = 20,
      .poisonPower = 6,
      .fearPower = 0,
      .family = ENEMY_FAMILY_WARGS },

    // [60] ENEMY_TYPE_SHELOB
    { .moveSpeed = 0x2c000,  // 2.75 px/f
      .attackFlags.d = { .meleeSlash = 1, .poison = 1 },
      .maxHp = 425,
      .xpReward = 900,
      .attackDamage = 50,
      .slashArmor = 5,
      .impaleArmor = 8,
      .hitStun = 10,
      .accuracy = 55,
      .defense = 40,
      .poisonPower = 9,
      .fearPower = 0,
      .family = ENEMY_FAMILY_WARGS },

    // [61] ENEMY_TYPE_GROND
    { .moveSpeed = 0x20000,  // 2 px/f
      .attackFlags.d = { .meleeImpale = 1, .fear = 1 },
      .maxHp = 100,
      .xpReward = 600,
      .attackDamage = 30,
      .slashArmor = 5,
      .impaleArmor = 5,
      .hitStun = 10,
      .accuracy = 40,
      .defense = 40,
      .poisonPower = 0,
      .fearPower = 3,
      .family = ENEMY_FAMILY_NONE },

    // [62] ENEMY_TYPE_SARUMAN
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .meleeImpale = 1, .fear = 1 },
      .maxHp = 100,
      .xpReward = 450,
      .attackDamage = 32,
      .slashArmor = 2,
      .impaleArmor = 8,
      .hitStun = 0,
      .accuracy = 45,
      .defense = 35,
      .poisonPower = 0,
      .fearPower = 5,
      .family = ENEMY_FAMILY_NONE },

    // [63] ENEMY_TYPE_DENETHOR
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeImpale = 1 },
      .maxHp = 500,
      .xpReward = 1200,
      .attackDamage = 60,
      .slashArmor = 8,
      .impaleArmor = 8,
      .hitStun = 10,
      .accuracy = 80,
      .defense = 150,
      .poisonPower = 10,
      .fearPower = 10,
      .family = ENEMY_FAMILY_NONE },

    // [64] ENEMY_TYPE_WITCH_KING
    { .moveSpeed = 0x20000,  // 2 px/f
      .attackFlags.d = { .meleeSlash = 1, .poison = 1, .fear = 1 },
      .maxHp = 550,
      .xpReward = 1800,
      .attackDamage = 60,
      .slashArmor = 9,
      .impaleArmor = 13,
      .hitStun = 100,
      .accuracy = 80,
      .defense = 50,
      .poisonPower = 3,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NAZGUL },

    // [65] ENEMY_TYPE_GOLLUM
    { .moveSpeed = 0x2c000,  // 2.75 px/f
      .attackFlags.d = { .meleeSlash = 1, .poison = 1 },
      .maxHp = 120,
      .xpReward = 1400,
      .attackDamage = 65,
      .slashArmor = 3,
      .impaleArmor = 9,
      .hitStun = 10,
      .accuracy = 100,
      .defense = 90,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [66] ENEMY_TYPE_MUMAKIL
    { .moveSpeed = 0xcccc,  // 0.799988 px/f
      .attackFlags.d = { .meleeImpale = 1, .fear = 1 },
      .maxHp = 400,
      .xpReward = 850,
      .attackDamage = 70,
      .slashArmor = 10,
      .impaleArmor = 15,
      .hitStun = 0,
      .accuracy = 70,
      .defense = 30,
      .poisonPower = 0,
      .fearPower = 10,
      .family = ENEMY_FAMILY_WARGS },

    // [67] ENEMY_TYPE_KING_OF_THE_DEAD
    { .moveSpeed = 0x38000,  // 3.5 px/f
      .attackFlags.d = { .meleeImpale = 1, .fear = 1 },
      .maxHp = 999,
      .xpReward = 5,
      .attackDamage = 32,
      .slashArmor = 99,
      .impaleArmor = 99,
      .hitStun = 10,
      .accuracy = 45,
      .defense = 150,
      .poisonPower = 0,
      .fearPower = 3,
      .family = ENEMY_FAMILY_NONE },

    // [68] ENEMY_TYPE_MOUTH_OF_SAURON
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeImpale = 1, .poison = 1, .fire = 1, .fear = 1 },
      .maxHp = 575,
      .xpReward = 1900,
      .attackDamage = 90,
      .slashArmor = 12,
      .impaleArmor = 15,
      .hitStun = 10,
      .accuracy = 90,
      .defense = 70,
      .poisonPower = 5,
      .fearPower = 10,
      .family = ENEMY_FAMILY_NAZGUL },

    // [69] ENEMY_TYPE_FLYING_NAZGUL
    { .moveSpeed = 0x20000,  // 2 px/f
      .attackFlags.d = { .meleeImpale = 1, .fear = 1 },
      .maxHp = 50,
      .xpReward = 300,
      .attackDamage = 70,
      .slashArmor = 8,
      .impaleArmor = 8,
      .hitStun = 0,
      .accuracy = 75,
      .defense = 45,
      .poisonPower = 0,
      .fearPower = 10,
      .family = ENEMY_FAMILY_NAZGUL },

    // [70] ENEMY_TYPE_GROUND_SKELETON
    { .moveSpeed = 0x0,  // 0 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 20,
      .xpReward = 25,
      .attackDamage = 35,
      .slashArmor = 1,
      .impaleArmor = 3,
      .hitStun = 10,
      .accuracy = 62,
      .defense = 25,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [71] ENEMY_TYPE_SKELETON_WARRIOR
    { .moveSpeed = 0x2c000,  // 2.75 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 25,
      .xpReward = 55,
      .attackDamage = 42,
      .slashArmor = 4,
      .impaleArmor = 12,
      .hitStun = 10,
      .accuracy = 65,
      .defense = 75,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [72] ENEMY_TYPE_SKELETON_WARRIOR_ELITE
    { .moveSpeed = 0x0,  // 0 px/f
      .attackFlags.d = { .meleeImpale = 1 },
      .maxHp = 250,
      .xpReward = 225,
      .attackDamage = 100,
      .slashArmor = 3,
      .impaleArmor = 12,
      .hitStun = 0,
      .accuracy = 50,
      .defense = 10,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [73] ENEMY_TYPE_GONDOR_SOLDIER
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 45,
      .xpReward = 20,
      .attackDamage = 65,
      .slashArmor = 4,
      .impaleArmor = 4,
      .hitStun = 30,
      .accuracy = 70,
      .defense = 45,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [74] ENEMY_TYPE_UNUSED_74
    { .moveSpeed = 0x24000,  // 2.25 px/f
      .attackFlags.d = { .meleeImpale = 1 },
      .maxHp = 1,
      .xpReward = 0,
      .attackDamage = 35,
      .slashArmor = 10,
      .impaleArmor = 20,
      .hitStun = 30,
      .accuracy = 50,
      .defense = 10,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_URUKHAI },

    // [75] ENEMY_TYPE_UNUSED_75
    { .moveSpeed = 0x20000,  // 2 px/f
      .attackFlags.d = { .meleeImpale = 1, .fire = 1 },
      .maxHp = 20,
      .xpReward = 0,
      .attackDamage = 95,
      .slashArmor = 1,
      .impaleArmor = 3,
      .hitStun = 30,
      .accuracy = 40,
      .defense = 20,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_URUKHAI },

    // [76] ENEMY_TYPE_UNUSED_76
    { .moveSpeed = 0x20000,  // 2 px/f
      .attackFlags.d = { .meleeImpale = 1, .fire = 1 },
      .maxHp = 30,
      .xpReward = 0,
      .attackDamage = 135,
      .slashArmor = 3,
      .impaleArmor = 4,
      .hitStun = 30,
      .accuracy = 50,
      .defense = 20,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_URUKHAI },

    // [77] ENEMY_TYPE_UNUSED_77
    { .moveSpeed = 0x20000,  // 2 px/f
      .attackFlags.d = { .meleeSlash = 1 },
      .maxHp = 20,
      .xpReward = 0,
      .attackDamage = 58,
      .slashArmor = 1,
      .impaleArmor = 3,
      .hitStun = 30,
      .accuracy = 40,
      .defense = 20,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_URUKHAI },

    // [78] ENEMY_TYPE_UNUSED_78
    { .moveSpeed = 0x20000,  // 2 px/f
      .attackFlags.d = { .ranged = 1, .fire = 1 },
      .maxHp = 5,
      .xpReward = 0,
      .attackDamage = 5,
      .slashArmor = 1,
      .impaleArmor = 3,
      .hitStun = 30,
      .accuracy = 40,
      .defense = 20,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_URUKHAI },

    // [79] ENEMY_TYPE_UNUSED_79
    { .moveSpeed = 0x20000,  // 2 px/f
      .attackFlags.d = { .ranged = 1, .fear = 1 },
      .maxHp = 5,
      .xpReward = 0,
      .attackDamage = 30,
      .slashArmor = 1,
      .impaleArmor = 3,
      .hitStun = 30,
      .accuracy = 90,
      .defense = 20,
      .poisonPower = 0,
      .fearPower = 10,
      .family = ENEMY_FAMILY_WARGS },

    // [80] ENEMY_TYPE_UNUSED_SHELOB_CLONE
    { .moveSpeed = 0x2c000,  // 2.75 px/f
      .attackFlags.d = { .meleeImpale = 1, .poison = 1, .fire = 1, .fear = 1 },
      .maxHp = 425,
      .xpReward = 800,
      .attackDamage = 75,
      .slashArmor = 5,
      .impaleArmor = 8,
      .hitStun = 10,
      .accuracy = 55,
      .defense = 40,
      .poisonPower = 15,
      .fearPower = 15,
      .family = ENEMY_FAMILY_WARGS },

    // [81] ENEMY_TYPE_UNUSED_81
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .ranged = 1, .fire = 1 },
      .maxHp = 300,
      .xpReward = 0,
      .attackDamage = 26,
      .slashArmor = 2,
      .impaleArmor = 4,
      .hitStun = 30,
      .accuracy = 40,
      .defense = 30,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },

    // [82] ENEMY_TYPE_UNUSED_82
    { .moveSpeed = 0x28000,  // 2.5 px/f
      .attackFlags.d = { .ranged = 1 },
      .maxHp = 100,
      .xpReward = 0,
      .attackDamage = 35,
      .slashArmor = 2,
      .impaleArmor = 4,
      .hitStun = 30,
      .accuracy = 40,
      .defense = 30,
      .poisonPower = 0,
      .fearPower = 0,
      .family = ENEMY_FAMILY_NONE },
};

// TODO: the u16 SFX ids are magic numbers - grow enum SfxId (include/sfx.h) as they get identified
//       and emit them by name (0x0a = SFX_COMBAT_EVADE is already known)

/** @romaddress 0x08057b94 */
const EnemySfxSet EnemySfxSets[83] = {
    // [0] ENEMY_TYPE_GOBLIN_SCOUT
    { .attackSfx = 0xa2,
      .aggroSfx = 0xa3,
      .hitSfx = 0xa4,
      .critSfx = 0xa4,
      .deathSfx = 0xa5,
      .missSfx = 0x0a },

    // [1] ENEMY_TYPE_GOBLIN
    { .attackSfx = 0xa2,
      .aggroSfx = 0xa3,
      .hitSfx = 0xa4,
      .critSfx = 0xa4,
      .deathSfx = 0xa5,
      .missSfx = 0x0a },

    // [2] ENEMY_TYPE_GOBLIN_ELITE
    { .attackSfx = 0xa2,
      .aggroSfx = 0xa3,
      .hitSfx = 0xa4,
      .critSfx = 0xa4,
      .deathSfx = 0xa5,
      .missSfx = 0x0a },

    // [3] ENEMY_TYPE_GOBLIN_ARCHER
    { .attackSfx = 0xd2,
      .aggroSfx = 0xa3,
      .hitSfx = 0xa4,
      .critSfx = 0xa4,
      .deathSfx = 0xa5,
      .missSfx = 0x0a },

    // [4] ENEMY_TYPE_GOBLIN_ARCHER_ELITE
    { .attackSfx = 0xd2,
      .aggroSfx = 0xa3,
      .hitSfx = 0xa4,
      .critSfx = 0xa4,
      .deathSfx = 0xa5,
      .missSfx = 0x0a },

    // [5] ENEMY_TYPE_ORC_DRUMMER
    { .attackSfx = 0xa2,
      .aggroSfx = 0x8d,
      .hitSfx = 0xa4,
      .critSfx = 0xa4,
      .deathSfx = 0xa5,
      .missSfx = 0x0a },

    // [6] ENEMY_TYPE_ORC_PITCHFORK_1
    { .attackSfx = 0x87,
      .aggroSfx = 0x89,
      .hitSfx = 0x8a,
      .critSfx = 0x8a,
      .deathSfx = 0x8b,
      .missSfx = 0x0a },

    // [7] ENEMY_TYPE_ORC_PITCHFORK_2
    { .attackSfx = 0x87,
      .aggroSfx = 0x89,
      .hitSfx = 0x8a,
      .critSfx = 0x8a,
      .deathSfx = 0x8b,
      .missSfx = 0x0a },

    // [8] ENEMY_TYPE_ORC_PITCHFORK_3
    { .attackSfx = 0x87,
      .aggroSfx = 0x89,
      .hitSfx = 0x8a,
      .critSfx = 0x8a,
      .deathSfx = 0x8b,
      .missSfx = 0x0a },

    // [9] ENEMY_TYPE_ORC_PITCHFORK_4
    { .attackSfx = 0x87,
      .aggroSfx = 0x89,
      .hitSfx = 0x8a,
      .critSfx = 0x8a,
      .deathSfx = 0x8b,
      .missSfx = 0x0a },

    // [10] ENEMY_TYPE_ORC_WARRIOR_AXE
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [11] ENEMY_TYPE_ORC_WARRIOR_ORC_SWORD
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [12] ENEMY_TYPE_ORC_WARRIOR_MACE
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [13] ENEMY_TYPE_ORC_WARRIOR_LONGSWORD
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [14] ENEMY_TYPE_ORC_WARRIOR_ELITE_AXE
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [15] ENEMY_TYPE_ORC_WARRIOR_ELITE_ORC_SWORD
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [16] ENEMY_TYPE_ORC_WARRIOR_ELITE_MACE
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [17] ENEMY_TYPE_ORC_WARRIOR_ELITE_LONGSWORD
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [18] ENEMY_TYPE_ORC_CAPTAIN_ARMORED
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [19] ENEMY_TYPE_ORC_CAPTAIN_ARMORED_ELITE
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [20] ENEMY_TYPE_ORC_ARCHER
    { .attackSfx = 0xd2,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [21] ENEMY_TYPE_ORC_ARCHER_ELITE
    { .attackSfx = 0xd2,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [22] ENEMY_TYPE_ORC_ARCHER_FIRE
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [23] ENEMY_TYPE_ORC_ARCHER_POISON
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [24] ENEMY_TYPE_ORC_CAPTAIN_UNARMORED
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [25] ENEMY_TYPE_ORC_CAPTAIN_UNARMORED_ELITE
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [26] ENEMY_TYPE_ORC_AXE_THROWER
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [27] ENEMY_TYPE_ORC_AXE_THROWER_ELITE
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [28] ENEMY_TYPE_ORC_FLAGBEARER
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [29] ENEMY_TYPE_ORC_HOPLITE
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [30] ENEMY_TYPE_ORC_HOPLITE_ELITE
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [31] ENEMY_TYPE_URUK
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [32] ENEMY_TYPE_URUK_ELITE
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [33] ENEMY_TYPE_URUK_CROSSBOWMAN
    { .attackSfx = 0xd2,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [34] ENEMY_TYPE_URUK_CROSSBOWMAN_ELITE
    { .attackSfx = 0xd2,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [35] ENEMY_TYPE_URUK_BERSERKER
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [36] ENEMY_TYPE_URUK_BERSERKER_ELITE
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [37] ENEMY_TYPE_WILDMAN
    { .attackSfx = 0x9e,
      .aggroSfx = 0x9f,
      .hitSfx = 0xa0,
      .critSfx = 0xa0,
      .deathSfx = 0xa1,
      .missSfx = 0x0a },

    // [38] ENEMY_TYPE_WILDMAN_ELITE
    { .attackSfx = 0x9e,
      .aggroSfx = 0x9f,
      .hitSfx = 0xa0,
      .critSfx = 0xa0,
      .deathSfx = 0xa1,
      .missSfx = 0x0a },

    // [39] ENEMY_TYPE_MOUNTAIN_TROLL
    { .attackSfx = 0xaa,
      .aggroSfx = 0xab,
      .hitSfx = 0xac,
      .critSfx = 0xac,
      .deathSfx = 0xad,
      .missSfx = 0x1d },

    // [40] ENEMY_TYPE_HALF_TROLL
    { .attackSfx = 0xa6,
      .aggroSfx = 0xa7,
      .hitSfx = 0xa8,
      .critSfx = 0xa8,
      .deathSfx = 0xa9,
      .missSfx = 0x1d },

    // [41] ENEMY_TYPE_SPIDER
    { .attackSfx = 0xb8,
      .aggroSfx = 0xb9,
      .hitSfx = 0xb9,
      .critSfx = 0xb9,
      .deathSfx = 0xba,
      .missSfx = 0x0a },

    // [42] ENEMY_TYPE_SPIDER_RED
    { .attackSfx = 0xb8,
      .aggroSfx = 0xb9,
      .hitSfx = 0xb9,
      .critSfx = 0xb9,
      .deathSfx = 0xba,
      .missSfx = 0x0a },

    // [43] ENEMY_TYPE_SPIDER_GREEN
    { .attackSfx = 0xb8,
      .aggroSfx = 0xb9,
      .hitSfx = 0xb9,
      .critSfx = 0xb9,
      .deathSfx = 0xba,
      .missSfx = 0x0a },

    // [44] ENEMY_TYPE_SPIDER_SMALL
    { .attackSfx = 0xb8,
      .aggroSfx = 0xb9,
      .hitSfx = 0xb9,
      .critSfx = 0xb9,
      .deathSfx = 0xba,
      .missSfx = 0x0a },

    // [45] ENEMY_TYPE_CORSAIR
    { .attackSfx = 0xc7,
      .aggroSfx = 0xc8,
      .hitSfx = 0xc9,
      .critSfx = 0xc9,
      .deathSfx = 0xca,
      .missSfx = 0x0a },

    // [46] ENEMY_TYPE_CORSAIR_ELITE
    { .attackSfx = 0xc7,
      .aggroSfx = 0xc8,
      .hitSfx = 0xc9,
      .critSfx = 0xc9,
      .deathSfx = 0xca,
      .missSfx = 0x0a },

    // [47] ENEMY_TYPE_CORSAIR_CAPTAIN
    { .attackSfx = 0xc7,
      .aggroSfx = 0xc8,
      .hitSfx = 0xc9,
      .critSfx = 0xc9,
      .deathSfx = 0xca,
      .missSfx = 0x0a },

    // [48] ENEMY_TYPE_WARG
    { .attackSfx = 0xbf,
      .aggroSfx = 0xc0,
      .hitSfx = 0xc1,
      .critSfx = 0xc1,
      .deathSfx = 0xc2,
      .missSfx = 0x1d },

    // [49] ENEMY_TYPE_WARG_ELITE
    { .attackSfx = 0xbb,
      .aggroSfx = 0xbc,
      .hitSfx = 0xbd,
      .critSfx = 0xbd,
      .deathSfx = 0xbe,
      .missSfx = 0x1d },

    // [50] ENEMY_TYPE_HARADRIM_ARCHER
    { .attackSfx = 0xce,
      .aggroSfx = 0xcf,
      .hitSfx = 0xd0,
      .critSfx = 0xd0,
      .deathSfx = 0xd1,
      .missSfx = 0x0a },

    // [51] ENEMY_TYPE_HARADRIM_ARCHER_ELITE
    { .attackSfx = 0xce,
      .aggroSfx = 0xcf,
      .hitSfx = 0xd0,
      .critSfx = 0xd0,
      .deathSfx = 0xd1,
      .missSfx = 0x0a },

    // [52] ENEMY_TYPE_EASTERLING_HEAVY
    { .attackSfx = 0xc7,
      .aggroSfx = 0xc8,
      .hitSfx = 0xc9,
      .critSfx = 0xc9,
      .deathSfx = 0xca,
      .missSfx = 0x0a },

    // [53] ENEMY_TYPE_EASTERLING_LIGHT
    { .attackSfx = 0xc7,
      .aggroSfx = 0xc8,
      .hitSfx = 0xc9,
      .critSfx = 0xc9,
      .deathSfx = 0xca,
      .missSfx = 0x0a },

    // [54] ENEMY_TYPE_EASTERLING_ELITE
    { .attackSfx = 0xc7,
      .aggroSfx = 0xc8,
      .hitSfx = 0xc9,
      .critSfx = 0xc9,
      .deathSfx = 0xca,
      .missSfx = 0x0a },

    // [55] ENEMY_TYPE_CREBAIN
    { .attackSfx = 0xae,
      .aggroSfx = 0xaf,
      .hitSfx = 0xb0,
      .critSfx = 0xb0,
      .deathSfx = 0xb1,
      .missSfx = 0x1d },

    // [56] ENEMY_TYPE_BAT
    { .attackSfx = 0xb2,
      .aggroSfx = 0xb3,
      .hitSfx = 0xb4,
      .critSfx = 0xb4,
      .deathSfx = 0xb5,
      .missSfx = 0x1d },

    // [57] ENEMY_TYPE_GHOST
    { .attackSfx = 0xcb,
      .aggroSfx = 0xcc,
      .hitSfx = 0xcc,
      .critSfx = 0xcc,
      .deathSfx = 0xcd,
      .missSfx = 0x1d },

    // [58] ENEMY_TYPE_NAZGUL
    { .attackSfx = 0xc3,
      .aggroSfx = 0xc4,
      .hitSfx = 0xc5,
      .critSfx = 0xc5,
      .deathSfx = 0xc6,
      .missSfx = 0x0a },

    // [59] ENEMY_TYPE_FLY_SWARM
    { .attackSfx = 0xb8,
      .aggroSfx = 0xb6,
      .hitSfx = 0xb6,
      .critSfx = 0xb6,
      .deathSfx = 0xb7,
      .missSfx = 0x1d },

    // [60] ENEMY_TYPE_SHELOB
    { .attackSfx = 0xd4,
      .aggroSfx = 0xd3,
      .hitSfx = 0xd7,
      .critSfx = 0xd6,
      .deathSfx = 0xd8,
      .missSfx = 0x0a },

    // [61] ENEMY_TYPE_GROND
    { .attackSfx = 0x106,
      .aggroSfx = 0x10e,
      .hitSfx = 0x116,
      .critSfx = 0x116,
      .deathSfx = 0x117,
      .missSfx = 0x0a },

    // [62] ENEMY_TYPE_SARUMAN
    { .attackSfx = 0xdc,
      .aggroSfx = 0xdb,
      .hitSfx = 0xdd,
      .critSfx = 0xdd,
      .deathSfx = 0xde,
      .missSfx = 0x0a },

    // [63] ENEMY_TYPE_DENETHOR
    { .attackSfx = 0xe1,
      .aggroSfx = 0xdf,
      .hitSfx = 0xe3,
      .critSfx = 0xe3,
      .deathSfx = 0xe4,
      .missSfx = 0x1d },

    // [64] ENEMY_TYPE_WITCH_KING
    { .attackSfx = 0xeb,
      .aggroSfx = 0xea,
      .hitSfx = 0xec,
      .critSfx = 0xec,
      .deathSfx = 0xee,
      .missSfx = 0x0a },

    // [65] ENEMY_TYPE_GOLLUM
    { .attackSfx = 0xf0,
      .aggroSfx = 0xef,
      .hitSfx = 0xf1,
      .critSfx = 0xf1,
      .deathSfx = 0xf5,
      .missSfx = 0x1d },

    // [66] ENEMY_TYPE_MUMAKIL
    { .attackSfx = 0xf6,
      .aggroSfx = 0xf6,
      .hitSfx = 0xf7,
      .critSfx = 0xf7,
      .deathSfx = 0xf8,
      .missSfx = 0x0a },

    // [67] ENEMY_TYPE_KING_OF_THE_DEAD
    { .attackSfx = 0xfa,
      .aggroSfx = 0xf9,
      .hitSfx = 0xf9,
      .critSfx = 0xf9,
      .deathSfx = 0xfb,
      .missSfx = 0x0a },

    // [68] ENEMY_TYPE_MOUTH_OF_SAURON
    { .attackSfx = 0x101,
      .aggroSfx = 0xfc,
      .hitSfx = 0x102,
      .critSfx = 0x102,
      .deathSfx = 0x103,
      .missSfx = 0x0a },

    // [69] ENEMY_TYPE_FLYING_NAZGUL
    { .attackSfx = 0xe7,
      .aggroSfx = 0xe5,
      .hitSfx = 0xe8,
      .critSfx = 0xe8,
      .deathSfx = 0xe9,
      .missSfx = 0x0a },

    // [70] ENEMY_TYPE_GROUND_SKELETON
    { .attackSfx = 0x08,
      .aggroSfx = 0xcc,
      .hitSfx = 0x08,
      .critSfx = 0x08,
      .deathSfx = 0x08,
      .missSfx = 0x0a },

    // [71] ENEMY_TYPE_SKELETON_WARRIOR
    { .attackSfx = 0x08,
      .aggroSfx = 0xcc,
      .hitSfx = 0x08,
      .critSfx = 0x08,
      .deathSfx = 0x08,
      .missSfx = 0x0a },

    // [72] ENEMY_TYPE_SKELETON_WARRIOR_ELITE
    { .attackSfx = 0x104,
      .aggroSfx = 0x105,
      .hitSfx = 0x105,
      .critSfx = 0x105,
      .deathSfx = 0x105,
      .missSfx = 0x0a },

    // [73] ENEMY_TYPE_GONDOR_SOLDIER
    { .attackSfx = 0xc7,
      .aggroSfx = 0xc8,
      .hitSfx = 0xc9,
      .critSfx = 0xc9,
      .deathSfx = 0xca,
      .missSfx = 0x0a },

    // [74] ENEMY_TYPE_UNUSED_74
    { .attackSfx = 0x104,
      .aggroSfx = 0x105,
      .hitSfx = 0x105,
      .critSfx = 0x105,
      .deathSfx = 0x105,
      .missSfx = 0x0a },

    // [75] ENEMY_TYPE_UNUSED_75
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [76] ENEMY_TYPE_UNUSED_76
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [77] ENEMY_TYPE_UNUSED_77
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [78] ENEMY_TYPE_UNUSED_78
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [79] ENEMY_TYPE_UNUSED_79
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [80] ENEMY_TYPE_UNUSED_SHELOB_CLONE
    { .attackSfx = 0xb8,
      .aggroSfx = 0xb9,
      .hitSfx = 0xb9,
      .critSfx = 0xb9,
      .deathSfx = 0xba,
      .missSfx = 0x0a },

    // [81] ENEMY_TYPE_UNUSED_81
    { .attackSfx = 0xdc,
      .aggroSfx = 0xdb,
      .hitSfx = 0xdd,
      .critSfx = 0xdd,
      .deathSfx = 0xde,
      .missSfx = 0x0a },

    // [82] ENEMY_TYPE_UNUSED_82
    { .attackSfx = 0xdc,
      .aggroSfx = 0xdb,
      .hitSfx = 0xdd,
      .critSfx = 0xdd,
      .deathSfx = 0xde,
      .missSfx = 0x0a },
};

/** @romaddress 0x08057f78 */
const EnemyLootInfo EnemyLootInfos[83] = {
    // [0] ENEMY_TYPE_GOBLIN_SCOUT
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [1] ENEMY_TYPE_GOBLIN
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [2] ENEMY_TYPE_GOBLIN_ELITE
    { .dropChanceBonus = 45,
      .qualityLo = 65,
      .qualityHi = 0,
      .prefixTierChance = 70,
      .field_0x4 = {100, 0, 0, 0} },

    // [3] ENEMY_TYPE_GOBLIN_ARCHER
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [4] ENEMY_TYPE_GOBLIN_ARCHER_ELITE
    { .dropChanceBonus = 45,
      .qualityLo = 65,
      .qualityHi = 0,
      .prefixTierChance = 70,
      .field_0x4 = {100, 0, 0, 0} },

    // [5] ENEMY_TYPE_ORC_DRUMMER
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 35,
      .field_0x4 = {95, 0, 0, 0} },

    // [6] ENEMY_TYPE_ORC_PITCHFORK_1
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [7] ENEMY_TYPE_ORC_PITCHFORK_2
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [8] ENEMY_TYPE_ORC_PITCHFORK_3
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [9] ENEMY_TYPE_ORC_PITCHFORK_4
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [10] ENEMY_TYPE_ORC_WARRIOR_AXE
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [11] ENEMY_TYPE_ORC_WARRIOR_ORC_SWORD
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [12] ENEMY_TYPE_ORC_WARRIOR_MACE
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [13] ENEMY_TYPE_ORC_WARRIOR_LONGSWORD
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [14] ENEMY_TYPE_ORC_WARRIOR_ELITE_AXE
    { .dropChanceBonus = 45,
      .qualityLo = 65,
      .qualityHi = 0,
      .prefixTierChance = 80,
      .field_0x4 = {100, 0, 0, 0} },

    // [15] ENEMY_TYPE_ORC_WARRIOR_ELITE_ORC_SWORD
    { .dropChanceBonus = 45,
      .qualityLo = 65,
      .qualityHi = 0,
      .prefixTierChance = 80,
      .field_0x4 = {100, 0, 0, 0} },

    // [16] ENEMY_TYPE_ORC_WARRIOR_ELITE_MACE
    { .dropChanceBonus = 45,
      .qualityLo = 65,
      .qualityHi = 0,
      .prefixTierChance = 80,
      .field_0x4 = {100, 0, 0, 0} },

    // [17] ENEMY_TYPE_ORC_WARRIOR_ELITE_LONGSWORD
    { .dropChanceBonus = 45,
      .qualityLo = 65,
      .qualityHi = 0,
      .prefixTierChance = 80,
      .field_0x4 = {100, 0, 0, 0} },

    // [18] ENEMY_TYPE_ORC_CAPTAIN_ARMORED
    { .dropChanceBonus = 45,
      .qualityLo = 32,
      .qualityHi = 0,
      .prefixTierChance = 15,
      .field_0x4 = {105, 0, 0, 0} },

    // [19] ENEMY_TYPE_ORC_CAPTAIN_ARMORED_ELITE
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 95,
      .field_0x4 = {110, 0, 0, 0} },

    // [20] ENEMY_TYPE_ORC_ARCHER
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [21] ENEMY_TYPE_ORC_ARCHER_ELITE
    { .dropChanceBonus = 45,
      .qualityLo = 65,
      .qualityHi = 0,
      .prefixTierChance = 90,
      .field_0x4 = {100, 0, 0, 0} },

    // [22] ENEMY_TYPE_ORC_ARCHER_FIRE
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 15,
      .field_0x4 = {95, 0, 0, 0} },

    // [23] ENEMY_TYPE_ORC_ARCHER_POISON
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 15,
      .field_0x4 = {95, 0, 0, 0} },

    // [24] ENEMY_TYPE_ORC_CAPTAIN_UNARMORED
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 15,
      .field_0x4 = {95, 0, 0, 0} },

    // [25] ENEMY_TYPE_ORC_CAPTAIN_UNARMORED_ELITE
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 25,
      .field_0x4 = {105, 0, 0, 0} },

    // [26] ENEMY_TYPE_ORC_AXE_THROWER
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 15,
      .field_0x4 = {95, 0, 0, 0} },

    // [27] ENEMY_TYPE_ORC_AXE_THROWER_ELITE
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 95,
      .field_0x4 = {105, 0, 0, 0} },

    // [28] ENEMY_TYPE_ORC_FLAGBEARER
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 30,
      .field_0x4 = {95, 0, 0, 0} },

    // [29] ENEMY_TYPE_ORC_HOPLITE
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 15,
      .field_0x4 = {100, 0, 0, 0} },

    // [30] ENEMY_TYPE_ORC_HOPLITE_ELITE
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 20,
      .field_0x4 = {95, 0, 0, 0} },

    // [31] ENEMY_TYPE_URUK
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [32] ENEMY_TYPE_URUK_ELITE
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 50,
      .field_0x4 = {110, 0, 0, 0} },

    // [33] ENEMY_TYPE_URUK_CROSSBOWMAN
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [34] ENEMY_TYPE_URUK_CROSSBOWMAN_ELITE
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 50,
      .field_0x4 = {105, 0, 0, 0} },

    // [35] ENEMY_TYPE_URUK_BERSERKER
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [36] ENEMY_TYPE_URUK_BERSERKER_ELITE
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 20,
      .field_0x4 = {95, 0, 0, 0} },

    // [37] ENEMY_TYPE_WILDMAN
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [38] ENEMY_TYPE_WILDMAN_ELITE
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 35,
      .field_0x4 = {105, 0, 0, 0} },

    // [39] ENEMY_TYPE_MOUNTAIN_TROLL
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [40] ENEMY_TYPE_HALF_TROLL
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 90,
      .field_0x4 = {105, 0, 0, 0} },

    // [41] ENEMY_TYPE_SPIDER
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [42] ENEMY_TYPE_SPIDER_RED
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 75,
      .field_0x4 = {95, 0, 0, 0} },

    // [43] ENEMY_TYPE_SPIDER_GREEN
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 25,
      .field_0x4 = {95, 0, 0, 0} },

    // [44] ENEMY_TYPE_SPIDER_SMALL
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [45] ENEMY_TYPE_CORSAIR
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [46] ENEMY_TYPE_CORSAIR_ELITE
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 60,
      .field_0x4 = {95, 0, 0, 0} },

    // [47] ENEMY_TYPE_CORSAIR_CAPTAIN
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [48] ENEMY_TYPE_WARG
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [49] ENEMY_TYPE_WARG_ELITE
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 20,
      .field_0x4 = {95, 0, 0, 0} },

    // [50] ENEMY_TYPE_HARADRIM_ARCHER
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [51] ENEMY_TYPE_HARADRIM_ARCHER_ELITE
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 70,
      .field_0x4 = {95, 0, 0, 0} },

    // [52] ENEMY_TYPE_EASTERLING_HEAVY
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 7,
      .field_0x4 = {95, 0, 0, 0} },

    // [53] ENEMY_TYPE_EASTERLING_LIGHT
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [54] ENEMY_TYPE_EASTERLING_ELITE
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 90,
      .field_0x4 = {95, 0, 0, 0} },

    // [55] ENEMY_TYPE_CREBAIN
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 1,
      .field_0x4 = {95, 0, 0, 0} },

    // [56] ENEMY_TYPE_BAT
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 1,
      .field_0x4 = {95, 0, 0, 0} },

    // [57] ENEMY_TYPE_GHOST
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 20,
      .field_0x4 = {95, 0, 0, 0} },

    // [58] ENEMY_TYPE_NAZGUL
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 55,
      .field_0x4 = {95, 0, 0, 0} },

    // [59] ENEMY_TYPE_FLY_SWARM
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [60] ENEMY_TYPE_SHELOB
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [61] ENEMY_TYPE_GROND
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [62] ENEMY_TYPE_SARUMAN
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [63] ENEMY_TYPE_DENETHOR
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [64] ENEMY_TYPE_WITCH_KING
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [65] ENEMY_TYPE_GOLLUM
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [66] ENEMY_TYPE_MUMAKIL
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [67] ENEMY_TYPE_KING_OF_THE_DEAD
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 25,
      .field_0x4 = {95, 0, 0, 0} },

    // [68] ENEMY_TYPE_MOUTH_OF_SAURON
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [69] ENEMY_TYPE_FLYING_NAZGUL
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [70] ENEMY_TYPE_GROUND_SKELETON
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 7,
      .field_0x4 = {95, 0, 0, 0} },

    // [71] ENEMY_TYPE_SKELETON_WARRIOR
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [72] ENEMY_TYPE_SKELETON_WARRIOR_ELITE
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 7,
      .field_0x4 = {95, 0, 0, 0} },

    // [73] ENEMY_TYPE_GONDOR_SOLDIER
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 7,
      .field_0x4 = {95, 0, 0, 0} },

    // [74] ENEMY_TYPE_UNUSED_74
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [75] ENEMY_TYPE_UNUSED_75
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [76] ENEMY_TYPE_UNUSED_76
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [77] ENEMY_TYPE_UNUSED_77
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [78] ENEMY_TYPE_UNUSED_78
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [79] ENEMY_TYPE_UNUSED_79
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [80] ENEMY_TYPE_UNUSED_SHELOB_CLONE
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [81] ENEMY_TYPE_UNUSED_81
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [82] ENEMY_TYPE_UNUSED_82
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },
};
// clang-format on
