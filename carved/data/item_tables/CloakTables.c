#include "item.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off


/** @romaddress 0x08064704 */
const ItemBase CloakItems[9] = {
    // [0] "Cape": Melee Armor +1, Missile Armor +1
    { .nameId = TEXT_ID_ITEM_CAPE,
      .tier = 1,
      .stat0 = STAT_MELEE_ARMOR,       .val0 =  1,
      .stat1 = STAT_MISSILE_ARMOR,     .val1 =  1,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_NONE,              .val3 =  0 },

    // [1] "Mantle": Melee Armor +1, Missile Armor +2
    { .nameId = TEXT_ID_ITEM_MANTLE,
      .tier = 2,
      .stat0 = STAT_MELEE_ARMOR,       .val0 =  1,
      .stat1 = STAT_MISSILE_ARMOR,     .val1 =  2,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_NONE,              .val3 =  0 },

    // [2] "Coat": Melee Armor +2, Missile Armor +2
    { .nameId = TEXT_ID_ITEM_COAT,
      .tier = 3,
      .stat0 = STAT_MELEE_ARMOR,       .val0 =  2,
      .stat1 = STAT_MISSILE_ARMOR,     .val1 =  2,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_NONE,              .val3 =  0 },

    // [3] "Cloak": Melee Armor +2, Missile Armor +3
    { .nameId = TEXT_ID_ITEM_CLOAK,
      .tier = 4,
      .stat0 = STAT_MELEE_ARMOR,       .val0 =  2,
      .stat1 = STAT_MISSILE_ARMOR,     .val1 =  3,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_NONE,              .val3 =  0 },

    // [4] "Winter Cloak": Melee Armor +3, Missile Armor +3
    { .nameId = TEXT_ID_ITEM_WINTER_CLOAK,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,       .val0 =  3,
      .stat1 = STAT_MISSILE_ARMOR,     .val1 =  3,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_NONE,              .val3 =  0 },

    // [5] "Heavy Cloak": Melee Armor +3, Missile Armor +5
    { .nameId = TEXT_ID_ITEM_HEAVY_CLOAK,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR,       .val0 =  3,
      .stat1 = STAT_MISSILE_ARMOR,     .val1 =  5,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_NONE,              .val3 =  0 },

    // [6] "Robe": Melee Armor +1, Missile Armor +1, % Experience Bonus +5
    { .nameId = TEXT_ID_ITEM_ROBE,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR,       .val0 =  1,
      .stat1 = STAT_MISSILE_ARMOR,     .val1 =  1,
      .stat2 = STAT_EXTRA_EXP_PERCENT, .val2 =  5,
      .stat3 = STAT_NONE,              .val3 =  0 },

    // [7] "Tabard": Melee Armor +4, Missile Armor +4, % Speed +5
    { .nameId = TEXT_ID_ITEM_TABARD,
      .tier = 8,
      .stat0 = STAT_MELEE_ARMOR,       .val0 =  4,
      .stat1 = STAT_MISSILE_ARMOR,     .val1 =  4,
      .stat2 = STAT_SPEED_PERCENT,     .val2 =  5,
      .stat3 = STAT_NONE,              .val3 =  0 },

    // [8] "Vestments": Melee Armor +2, Missile Armor +1, % Experience Bonus +10
    { .nameId = TEXT_ID_ITEM_VESTMENTS,
      .tier = 10,
      .stat0 = STAT_MELEE_ARMOR,       .val0 =  2,
      .stat1 = STAT_MISSILE_ARMOR,     .val1 =  1,
      .stat2 = STAT_EXTRA_EXP_PERCENT, .val2 = 10,
      .stat3 = STAT_NONE,              .val3 =  0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x08064770
 */
const ItemGfx CloakGfx[9] = {
    // [0] "Cape"
    ITEM_GFX(CloakCape, LOOT_TYPE_CLOAK_TIER_1),
    // [1] "Mantle"
    ITEM_GFX(CloakMantle, LOOT_TYPE_CLOAK_TIER_1),
    // [2] "Coat"
    ITEM_GFX(CloakCoat, LOOT_TYPE_CLOAK_TIER_1),
    // [3] "Cloak"
    ITEM_GFX(CloakCloak, LOOT_TYPE_CLOAK_TIER_1),
    // [4] "Winter Cloak"
    ITEM_GFX(CloakWinterCloak, LOOT_TYPE_CLOAK_TIER_2),
    // [5] "Heavy Cloak"
    ITEM_GFX(CloakHeavyCloak, LOOT_TYPE_CLOAK_TIER_2),
    // [6] "Robe"
    ITEM_GFX(CloakRobe, LOOT_TYPE_CLOAK_TIER_3),
    // [7] "Tabard"
    ITEM_GFX(CloakTabard, LOOT_TYPE_CLOAK_TIER_3),
    // [8] "Vestments"
    ITEM_GFX(CloakVestments, LOOT_TYPE_CLOAK_TIER_3),
};

/** most common row (6 of 9); deviations are spelled out per entry */
#define FLAGS_CLOAK { .slotCloak = 1, .classFrodo = 1, .classLegolas = 1, .classAragorn = 1, .classGandalf = 1, .classEowyn = 1, .classGimli = 1 }

/** @romaddress 0x0806486c */
const ItemMetadata CloakItemFlags[9] = {
    // [0] "Cape"
    { .flags.d = FLAGS_CLOAK, .equipSfx = 0x22 },

    // [1] "Mantle"
    { .flags.d = FLAGS_CLOAK, .equipSfx = 0x22 },

    // [2] "Coat"
    { .flags.d = FLAGS_CLOAK, .equipSfx = 0x22 },

    // [3] "Cloak"
    { .flags.d = FLAGS_CLOAK, .equipSfx = 0x22 },

    // [4] "Winter Cloak"
    { .flags.d = FLAGS_CLOAK, .equipSfx = 0x22 },

    // [5] "Heavy Cloak"
    { .flags.d = FLAGS_CLOAK, .equipSfx = 0x22 },

    // [6] "Robe"
    { .flags.d = { .slotCloak = 1,
                   .classGandalf = 1 },
      .equipSfx = 0x22 },

    // [7] "Tabard"
    { .flags.d = { .ancient = 1,
                   .slotCloak = 1,
                   .classGandalf = 1 },
      .equipSfx = 0x22 },

    // [8] "Vestments"
    { .flags.d = { .slotCloak = 1,
                   .classGandalf = 1 },
      .equipSfx = 0x22 },
};
#undef FLAGS_CLOAK
// clang-format on
