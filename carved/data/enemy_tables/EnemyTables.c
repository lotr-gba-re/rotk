#include "enemy.h"

// clang-format off

// TODO: enemy type names are unreversed - map unitType (the [index] here) to creature names
//       (spawn/scene data?) and add them to the entry comments

/** @romaddress 0x08057518 */
const EnemyTypeInfo EnemyTypes[83] = {
    // [0x00]
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
      .category = CREATURE_ORCS },

    // [0x01]
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
      .category = CREATURE_ORCS },

    // [0x02]
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
      .category = CREATURE_ORCS },

    // [0x03]
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
      .category = CREATURE_ORCS },

    // [0x04]
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
      .category = CREATURE_ORCS },

    // [0x05]
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
      .category = CREATURE_ORCS },

    // [0x06]
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
      .category = CREATURE_ORCS },

    // [0x07]
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
      .category = CREATURE_ORCS },

    // [0x08]
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
      .category = CREATURE_ORCS },

    // [0x09]
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
      .category = CREATURE_ORCS },

    // [0x0a]
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
      .category = CREATURE_ORCS },

    // [0x0b]
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
      .category = CREATURE_ORCS },

    // [0x0c]
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
      .category = CREATURE_ORCS },

    // [0x0d]
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
      .category = CREATURE_ORCS },

    // [0x0e]
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
      .category = CREATURE_ORCS },

    // [0x0f]
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
      .category = CREATURE_ORCS },

    // [0x10]
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
      .category = CREATURE_ORCS },

    // [0x11]
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
      .category = CREATURE_ORCS },

    // [0x12]
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
      .category = CREATURE_ORCS },

    // [0x13]
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
      .category = CREATURE_ORCS },

    // [0x14]
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
      .category = CREATURE_ORCS },

    // [0x15]
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
      .category = CREATURE_ORCS },

    // [0x16]
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
      .category = CREATURE_ORCS },

    // [0x17]
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
      .category = CREATURE_ORCS },

    // [0x18]
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
      .category = CREATURE_ORCS },

    // [0x19]
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
      .category = CREATURE_ORCS },

    // [0x1a]
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
      .category = CREATURE_ORCS },

    // [0x1b]
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
      .category = CREATURE_ORCS },

    // [0x1c]
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
      .category = CREATURE_ORCS },

    // [0x1d]
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
      .category = CREATURE_ORCS },

    // [0x1e]
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
      .category = CREATURE_ORCS },

    // [0x1f]
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
      .category = CREATURE_URUKHAI },

    // [0x20]
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
      .category = CREATURE_URUKHAI },

    // [0x21]
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
      .category = CREATURE_URUKHAI },

    // [0x22]
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
      .category = CREATURE_URUKHAI },

    // [0x23]
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
      .category = CREATURE_URUKHAI },

    // [0x24]
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
      .category = CREATURE_URUKHAI },

    // [0x25]
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
      .category = CREATURE_NONE },

    // [0x26]
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
      .category = CREATURE_NONE },

    // [0x27]
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
      .category = CREATURE_NONE },

    // [0x28]
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
      .category = CREATURE_NONE },

    // [0x29]
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
      .category = CREATURE_WARGS },

    // [0x2a]
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
      .category = CREATURE_WARGS },

    // [0x2b]
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
      .category = CREATURE_WARGS },

    // [0x2c]
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
      .category = CREATURE_WARGS },

    // [0x2d]
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
      .category = CREATURE_NONE },

    // [0x2e]
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
      .category = CREATURE_NONE },

    // [0x2f]
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
      .category = CREATURE_NONE },

    // [0x30]
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
      .category = CREATURE_WARGS },

    // [0x31]
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
      .category = CREATURE_WARGS },

    // [0x32]
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
      .category = CREATURE_ORCS },

    // [0x33]
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
      .category = CREATURE_ORCS },

    // [0x34]
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
      .category = CREATURE_NONE },

    // [0x35]
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
      .category = CREATURE_NONE },

    // [0x36]
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
      .category = CREATURE_NONE },

    // [0x37]
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
      .category = CREATURE_CREBAIN },

    // [0x38]
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
      .category = CREATURE_CREBAIN },

    // [0x39]
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
      .category = CREATURE_NONE },

    // [0x3a]
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
      .category = CREATURE_NAZGUL },

    // [0x3b]
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
      .category = CREATURE_WARGS },

    // [0x3c]
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
      .category = CREATURE_WARGS },

    // [0x3d]
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
      .category = CREATURE_NONE },

    // [0x3e]
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
      .category = CREATURE_NONE },

    // [0x3f]
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
      .category = CREATURE_NONE },

    // [0x40]
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
      .category = CREATURE_NAZGUL },

    // [0x41]
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
      .category = CREATURE_NONE },

    // [0x42]
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
      .category = CREATURE_WARGS },

    // [0x43]
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
      .category = CREATURE_NONE },

    // [0x44]
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
      .category = CREATURE_NAZGUL },

    // [0x45]
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
      .category = CREATURE_NAZGUL },

    // [0x46]
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
      .category = CREATURE_NONE },

    // [0x47]
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
      .category = CREATURE_NONE },

    // [0x48]
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
      .category = CREATURE_NONE },

    // [0x49]
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
      .category = CREATURE_NONE },

    // [0x4a]
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
      .category = CREATURE_URUKHAI },

    // [0x4b]
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
      .category = CREATURE_URUKHAI },

    // [0x4c]
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
      .category = CREATURE_URUKHAI },

    // [0x4d]
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
      .category = CREATURE_URUKHAI },

    // [0x4e]
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
      .category = CREATURE_URUKHAI },

    // [0x4f]
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
      .category = CREATURE_WARGS },

    // [0x50]
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
      .category = CREATURE_WARGS },

    // [0x51]
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
      .category = CREATURE_NONE },

    // [0x52]
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
      .category = CREATURE_NONE },
};

// TODO: the u16 SFX ids are magic numbers - grow enum SfxId (include/sfx.h) as they get identified
//       and emit them by name (0x0a = SFX_COMBAT_EVADE is already known)
// TODO: enemy type names are unreversed - same [index] = unitType as EnemyTypes

/** @romaddress 0x08057b94 */
const EnemySfx EnemySfxSets[83] = {
    // [0x00]
    { .attackSfx = 0xa2,
      .aggroSfx = 0xa3,
      .hitSfx = 0xa4,
      .critSfx = 0xa4,
      .deathSfx = 0xa5,
      .missSfx = 0x0a },

    // [0x01]
    { .attackSfx = 0xa2,
      .aggroSfx = 0xa3,
      .hitSfx = 0xa4,
      .critSfx = 0xa4,
      .deathSfx = 0xa5,
      .missSfx = 0x0a },

    // [0x02]
    { .attackSfx = 0xa2,
      .aggroSfx = 0xa3,
      .hitSfx = 0xa4,
      .critSfx = 0xa4,
      .deathSfx = 0xa5,
      .missSfx = 0x0a },

    // [0x03]
    { .attackSfx = 0xd2,
      .aggroSfx = 0xa3,
      .hitSfx = 0xa4,
      .critSfx = 0xa4,
      .deathSfx = 0xa5,
      .missSfx = 0x0a },

    // [0x04]
    { .attackSfx = 0xd2,
      .aggroSfx = 0xa3,
      .hitSfx = 0xa4,
      .critSfx = 0xa4,
      .deathSfx = 0xa5,
      .missSfx = 0x0a },

    // [0x05]
    { .attackSfx = 0xa2,
      .aggroSfx = 0x8d,
      .hitSfx = 0xa4,
      .critSfx = 0xa4,
      .deathSfx = 0xa5,
      .missSfx = 0x0a },

    // [0x06]
    { .attackSfx = 0x87,
      .aggroSfx = 0x89,
      .hitSfx = 0x8a,
      .critSfx = 0x8a,
      .deathSfx = 0x8b,
      .missSfx = 0x0a },

    // [0x07]
    { .attackSfx = 0x87,
      .aggroSfx = 0x89,
      .hitSfx = 0x8a,
      .critSfx = 0x8a,
      .deathSfx = 0x8b,
      .missSfx = 0x0a },

    // [0x08]
    { .attackSfx = 0x87,
      .aggroSfx = 0x89,
      .hitSfx = 0x8a,
      .critSfx = 0x8a,
      .deathSfx = 0x8b,
      .missSfx = 0x0a },

    // [0x09]
    { .attackSfx = 0x87,
      .aggroSfx = 0x89,
      .hitSfx = 0x8a,
      .critSfx = 0x8a,
      .deathSfx = 0x8b,
      .missSfx = 0x0a },

    // [0x0a]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x0b]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x0c]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x0d]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x0e]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x0f]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x10]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x11]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x12]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x13]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x14]
    { .attackSfx = 0xd2,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x15]
    { .attackSfx = 0xd2,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x16]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x17]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x18]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x19]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x1a]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x1b]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x1c]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x1d]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x1e]
    { .attackSfx = 0x93,
      .aggroSfx = 0x94,
      .hitSfx = 0x95,
      .critSfx = 0x95,
      .deathSfx = 0x96,
      .missSfx = 0x0a },

    // [0x1f]
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [0x20]
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [0x21]
    { .attackSfx = 0xd2,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [0x22]
    { .attackSfx = 0xd2,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [0x23]
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [0x24]
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [0x25]
    { .attackSfx = 0x9e,
      .aggroSfx = 0x9f,
      .hitSfx = 0xa0,
      .critSfx = 0xa0,
      .deathSfx = 0xa1,
      .missSfx = 0x0a },

    // [0x26]
    { .attackSfx = 0x9e,
      .aggroSfx = 0x9f,
      .hitSfx = 0xa0,
      .critSfx = 0xa0,
      .deathSfx = 0xa1,
      .missSfx = 0x0a },

    // [0x27]
    { .attackSfx = 0xaa,
      .aggroSfx = 0xab,
      .hitSfx = 0xac,
      .critSfx = 0xac,
      .deathSfx = 0xad,
      .missSfx = 0x1d },

    // [0x28]
    { .attackSfx = 0xa6,
      .aggroSfx = 0xa7,
      .hitSfx = 0xa8,
      .critSfx = 0xa8,
      .deathSfx = 0xa9,
      .missSfx = 0x1d },

    // [0x29]
    { .attackSfx = 0xb8,
      .aggroSfx = 0xb9,
      .hitSfx = 0xb9,
      .critSfx = 0xb9,
      .deathSfx = 0xba,
      .missSfx = 0x0a },

    // [0x2a]
    { .attackSfx = 0xb8,
      .aggroSfx = 0xb9,
      .hitSfx = 0xb9,
      .critSfx = 0xb9,
      .deathSfx = 0xba,
      .missSfx = 0x0a },

    // [0x2b]
    { .attackSfx = 0xb8,
      .aggroSfx = 0xb9,
      .hitSfx = 0xb9,
      .critSfx = 0xb9,
      .deathSfx = 0xba,
      .missSfx = 0x0a },

    // [0x2c]
    { .attackSfx = 0xb8,
      .aggroSfx = 0xb9,
      .hitSfx = 0xb9,
      .critSfx = 0xb9,
      .deathSfx = 0xba,
      .missSfx = 0x0a },

    // [0x2d]
    { .attackSfx = 0xc7,
      .aggroSfx = 0xc8,
      .hitSfx = 0xc9,
      .critSfx = 0xc9,
      .deathSfx = 0xca,
      .missSfx = 0x0a },

    // [0x2e]
    { .attackSfx = 0xc7,
      .aggroSfx = 0xc8,
      .hitSfx = 0xc9,
      .critSfx = 0xc9,
      .deathSfx = 0xca,
      .missSfx = 0x0a },

    // [0x2f]
    { .attackSfx = 0xc7,
      .aggroSfx = 0xc8,
      .hitSfx = 0xc9,
      .critSfx = 0xc9,
      .deathSfx = 0xca,
      .missSfx = 0x0a },

    // [0x30]
    { .attackSfx = 0xbf,
      .aggroSfx = 0xc0,
      .hitSfx = 0xc1,
      .critSfx = 0xc1,
      .deathSfx = 0xc2,
      .missSfx = 0x1d },

    // [0x31]
    { .attackSfx = 0xbb,
      .aggroSfx = 0xbc,
      .hitSfx = 0xbd,
      .critSfx = 0xbd,
      .deathSfx = 0xbe,
      .missSfx = 0x1d },

    // [0x32]
    { .attackSfx = 0xce,
      .aggroSfx = 0xcf,
      .hitSfx = 0xd0,
      .critSfx = 0xd0,
      .deathSfx = 0xd1,
      .missSfx = 0x0a },

    // [0x33]
    { .attackSfx = 0xce,
      .aggroSfx = 0xcf,
      .hitSfx = 0xd0,
      .critSfx = 0xd0,
      .deathSfx = 0xd1,
      .missSfx = 0x0a },

    // [0x34]
    { .attackSfx = 0xc7,
      .aggroSfx = 0xc8,
      .hitSfx = 0xc9,
      .critSfx = 0xc9,
      .deathSfx = 0xca,
      .missSfx = 0x0a },

    // [0x35]
    { .attackSfx = 0xc7,
      .aggroSfx = 0xc8,
      .hitSfx = 0xc9,
      .critSfx = 0xc9,
      .deathSfx = 0xca,
      .missSfx = 0x0a },

    // [0x36]
    { .attackSfx = 0xc7,
      .aggroSfx = 0xc8,
      .hitSfx = 0xc9,
      .critSfx = 0xc9,
      .deathSfx = 0xca,
      .missSfx = 0x0a },

    // [0x37]
    { .attackSfx = 0xae,
      .aggroSfx = 0xaf,
      .hitSfx = 0xb0,
      .critSfx = 0xb0,
      .deathSfx = 0xb1,
      .missSfx = 0x1d },

    // [0x38]
    { .attackSfx = 0xb2,
      .aggroSfx = 0xb3,
      .hitSfx = 0xb4,
      .critSfx = 0xb4,
      .deathSfx = 0xb5,
      .missSfx = 0x1d },

    // [0x39]
    { .attackSfx = 0xcb,
      .aggroSfx = 0xcc,
      .hitSfx = 0xcc,
      .critSfx = 0xcc,
      .deathSfx = 0xcd,
      .missSfx = 0x1d },

    // [0x3a]
    { .attackSfx = 0xc3,
      .aggroSfx = 0xc4,
      .hitSfx = 0xc5,
      .critSfx = 0xc5,
      .deathSfx = 0xc6,
      .missSfx = 0x0a },

    // [0x3b]
    { .attackSfx = 0xb8,
      .aggroSfx = 0xb6,
      .hitSfx = 0xb6,
      .critSfx = 0xb6,
      .deathSfx = 0xb7,
      .missSfx = 0x1d },

    // [0x3c]
    { .attackSfx = 0xd4,
      .aggroSfx = 0xd3,
      .hitSfx = 0xd7,
      .critSfx = 0xd6,
      .deathSfx = 0xd8,
      .missSfx = 0x0a },

    // [0x3d]
    { .attackSfx = 0x106,
      .aggroSfx = 0x10e,
      .hitSfx = 0x116,
      .critSfx = 0x116,
      .deathSfx = 0x117,
      .missSfx = 0x0a },

    // [0x3e]
    { .attackSfx = 0xdc,
      .aggroSfx = 0xdb,
      .hitSfx = 0xdd,
      .critSfx = 0xdd,
      .deathSfx = 0xde,
      .missSfx = 0x0a },

    // [0x3f]
    { .attackSfx = 0xe1,
      .aggroSfx = 0xdf,
      .hitSfx = 0xe3,
      .critSfx = 0xe3,
      .deathSfx = 0xe4,
      .missSfx = 0x1d },

    // [0x40]
    { .attackSfx = 0xeb,
      .aggroSfx = 0xea,
      .hitSfx = 0xec,
      .critSfx = 0xec,
      .deathSfx = 0xee,
      .missSfx = 0x0a },

    // [0x41]
    { .attackSfx = 0xf0,
      .aggroSfx = 0xef,
      .hitSfx = 0xf1,
      .critSfx = 0xf1,
      .deathSfx = 0xf5,
      .missSfx = 0x1d },

    // [0x42]
    { .attackSfx = 0xf6,
      .aggroSfx = 0xf6,
      .hitSfx = 0xf7,
      .critSfx = 0xf7,
      .deathSfx = 0xf8,
      .missSfx = 0x0a },

    // [0x43]
    { .attackSfx = 0xfa,
      .aggroSfx = 0xf9,
      .hitSfx = 0xf9,
      .critSfx = 0xf9,
      .deathSfx = 0xfb,
      .missSfx = 0x0a },

    // [0x44]
    { .attackSfx = 0x101,
      .aggroSfx = 0xfc,
      .hitSfx = 0x102,
      .critSfx = 0x102,
      .deathSfx = 0x103,
      .missSfx = 0x0a },

    // [0x45]
    { .attackSfx = 0xe7,
      .aggroSfx = 0xe5,
      .hitSfx = 0xe8,
      .critSfx = 0xe8,
      .deathSfx = 0xe9,
      .missSfx = 0x0a },

    // [0x46]
    { .attackSfx = 0x08,
      .aggroSfx = 0xcc,
      .hitSfx = 0x08,
      .critSfx = 0x08,
      .deathSfx = 0x08,
      .missSfx = 0x0a },

    // [0x47]
    { .attackSfx = 0x08,
      .aggroSfx = 0xcc,
      .hitSfx = 0x08,
      .critSfx = 0x08,
      .deathSfx = 0x08,
      .missSfx = 0x0a },

    // [0x48]
    { .attackSfx = 0x104,
      .aggroSfx = 0x105,
      .hitSfx = 0x105,
      .critSfx = 0x105,
      .deathSfx = 0x105,
      .missSfx = 0x0a },

    // [0x49]
    { .attackSfx = 0xc7,
      .aggroSfx = 0xc8,
      .hitSfx = 0xc9,
      .critSfx = 0xc9,
      .deathSfx = 0xca,
      .missSfx = 0x0a },

    // [0x4a]
    { .attackSfx = 0x104,
      .aggroSfx = 0x105,
      .hitSfx = 0x105,
      .critSfx = 0x105,
      .deathSfx = 0x105,
      .missSfx = 0x0a },

    // [0x4b]
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [0x4c]
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [0x4d]
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [0x4e]
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [0x4f]
    { .attackSfx = 0x97,
      .aggroSfx = 0x98,
      .hitSfx = 0x99,
      .critSfx = 0x99,
      .deathSfx = 0x9a,
      .missSfx = 0x0a },

    // [0x50]
    { .attackSfx = 0xb8,
      .aggroSfx = 0xb9,
      .hitSfx = 0xb9,
      .critSfx = 0xb9,
      .deathSfx = 0xba,
      .missSfx = 0x0a },

    // [0x51]
    { .attackSfx = 0xdc,
      .aggroSfx = 0xdb,
      .hitSfx = 0xdd,
      .critSfx = 0xdd,
      .deathSfx = 0xde,
      .missSfx = 0x0a },

    // [0x52]
    { .attackSfx = 0xdc,
      .aggroSfx = 0xdb,
      .hitSfx = 0xdd,
      .critSfx = 0xdd,
      .deathSfx = 0xde,
      .missSfx = 0x0a },
};

// TODO: enemy type names are unreversed - same [index] = unitType as EnemyTypes

/** @romaddress 0x08057f78 */
const EnemyLootInfo EnemyLootInfos[83] = {
    // [0x00]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x01]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x02]
    { .dropChanceBonus = 45,
      .qualityLo = 65,
      .qualityHi = 0,
      .prefixTierChance = 70,
      .field_0x4 = {100, 0, 0, 0} },

    // [0x03]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x04]
    { .dropChanceBonus = 45,
      .qualityLo = 65,
      .qualityHi = 0,
      .prefixTierChance = 70,
      .field_0x4 = {100, 0, 0, 0} },

    // [0x05]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 35,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x06]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x07]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x08]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x09]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x0a]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x0b]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x0c]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x0d]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x0e]
    { .dropChanceBonus = 45,
      .qualityLo = 65,
      .qualityHi = 0,
      .prefixTierChance = 80,
      .field_0x4 = {100, 0, 0, 0} },

    // [0x0f]
    { .dropChanceBonus = 45,
      .qualityLo = 65,
      .qualityHi = 0,
      .prefixTierChance = 80,
      .field_0x4 = {100, 0, 0, 0} },

    // [0x10]
    { .dropChanceBonus = 45,
      .qualityLo = 65,
      .qualityHi = 0,
      .prefixTierChance = 80,
      .field_0x4 = {100, 0, 0, 0} },

    // [0x11]
    { .dropChanceBonus = 45,
      .qualityLo = 65,
      .qualityHi = 0,
      .prefixTierChance = 80,
      .field_0x4 = {100, 0, 0, 0} },

    // [0x12]
    { .dropChanceBonus = 45,
      .qualityLo = 32,
      .qualityHi = 0,
      .prefixTierChance = 15,
      .field_0x4 = {105, 0, 0, 0} },

    // [0x13]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 95,
      .field_0x4 = {110, 0, 0, 0} },

    // [0x14]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x15]
    { .dropChanceBonus = 45,
      .qualityLo = 65,
      .qualityHi = 0,
      .prefixTierChance = 90,
      .field_0x4 = {100, 0, 0, 0} },

    // [0x16]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 15,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x17]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 15,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x18]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 15,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x19]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 25,
      .field_0x4 = {105, 0, 0, 0} },

    // [0x1a]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 15,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x1b]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 95,
      .field_0x4 = {105, 0, 0, 0} },

    // [0x1c]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 30,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x1d]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 15,
      .field_0x4 = {100, 0, 0, 0} },

    // [0x1e]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 20,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x1f]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x20]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 50,
      .field_0x4 = {110, 0, 0, 0} },

    // [0x21]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x22]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 50,
      .field_0x4 = {105, 0, 0, 0} },

    // [0x23]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x24]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 20,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x25]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x26]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 35,
      .field_0x4 = {105, 0, 0, 0} },

    // [0x27]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x28]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 90,
      .field_0x4 = {105, 0, 0, 0} },

    // [0x29]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x2a]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 75,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x2b]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 25,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x2c]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x2d]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x2e]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 60,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x2f]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x30]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x31]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 20,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x32]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x33]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 70,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x34]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 7,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x35]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x36]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 90,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x37]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 1,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x38]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 1,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x39]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 20,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x3a]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 55,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x3b]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x3c]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x3d]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x3e]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x3f]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x40]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x41]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x42]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x43]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 25,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x44]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x45]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x46]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 7,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x47]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 10,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x48]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 7,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x49]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = 0,
      .prefixTierChance = 7,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x4a]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x4b]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x4c]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x4d]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x4e]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x4f]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x50]
    { .dropChanceBonus = 45,
      .qualityLo = 70,
      .qualityHi = 0,
      .prefixTierChance = 100,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x51]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },

    // [0x52]
    { .dropChanceBonus = 0,
      .qualityLo = 0,
      .qualityHi = -31,
      .prefixTierChance = 5,
      .field_0x4 = {95, 0, 0, 0} },
};
// clang-format on
