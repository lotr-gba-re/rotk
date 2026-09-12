#include "item.h"
#include "libc.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off


/** @romaddress 0x08064344 */
const ItemBase ShieldItems[8] = {
    // [0] "Buckler": % Shield Block +10
    { .nameId = TEXT_ID_ITEM_BUCKLER,
      .tier = 1,
      .stat0 = STAT_BLOCK_PERCENT, .val0 = 10,
      .stat1 = STAT_NONE,          .val1 =  0,
      .stat2 = STAT_NONE,          .val2 =  0,
      .stat3 = STAT_NONE,          .val3 =  0 },

    // [1] "Small Shield": % Shield Block +15
    { .nameId = TEXT_ID_ITEM_SMALL_SHIELD,
      .tier = 2,
      .stat0 = STAT_BLOCK_PERCENT, .val0 = 15,
      .stat1 = STAT_NONE,          .val1 =  0,
      .stat2 = STAT_NONE,          .val2 =  0,
      .stat3 = STAT_NONE,          .val3 =  0 },

    // [2] "Heater Shield": % Shield Block +20
    { .nameId = TEXT_ID_ITEM_HEATER_SHIELD,
      .tier = 3,
      .stat0 = STAT_BLOCK_PERCENT, .val0 = 20,
      .stat1 = STAT_NONE,          .val1 =  0,
      .stat2 = STAT_NONE,          .val2 =  0,
      .stat3 = STAT_NONE,          .val3 =  0 },

    // [3] "Splinted Shield": % Shield Block +20, Defense +2, Melee Armor +1
    { .nameId = TEXT_ID_ITEM_SPLINTED_SHIELD,
      .tier = 4,
      .stat0 = STAT_BLOCK_PERCENT, .val0 = 20,
      .stat1 = STAT_DEFENSE,       .val1 =  2,
      .stat2 = STAT_MELEE_ARMOR,   .val2 =  1,
      .stat3 = STAT_NONE,          .val3 =  0 },

    // [4] "Round Shield": % Shield Block +25
    { .nameId = TEXT_ID_ITEM_ROUND_SHIELD,
      .tier = 5,
      .stat0 = STAT_BLOCK_PERCENT, .val0 = 25,
      .stat1 = STAT_NONE,          .val1 =  0,
      .stat2 = STAT_NONE,          .val2 =  0,
      .stat3 = STAT_NONE,          .val3 =  0 },

    // [5] "Kite Shield": % Shield Block +25, Defense +3
    { .nameId = TEXT_ID_ITEM_KITE_SHIELD,
      .tier = 6,
      .stat0 = STAT_BLOCK_PERCENT, .val0 = 25,
      .stat1 = STAT_DEFENSE,       .val1 =  3,
      .stat2 = STAT_NONE,          .val2 =  0,
      .stat3 = STAT_NONE,          .val3 =  0 },

    // [6] "Large Shield": % Shield Block +30, Accuracy -3
    { .nameId = TEXT_ID_ITEM_LARGE_SHIELD,
      .tier = 7,
      .stat0 = STAT_BLOCK_PERCENT, .val0 = 30,
      .stat1 = STAT_ACCURACY,      .val1 = -3,
      .stat2 = STAT_NONE,          .val2 =  0,
      .stat3 = STAT_NONE,          .val3 =  0 },

    // [7] "Tower Shield": % Shield Block +35, Accuracy -5
    { .nameId = TEXT_ID_ITEM_TOWER_SHIELD,
      .tier = 8,
      .stat0 = STAT_BLOCK_PERCENT, .val0 = 35,
      .stat1 = STAT_ACCURACY,      .val1 = -5,
      .stat2 = STAT_NONE,          .val2 =  0,
      .stat3 = STAT_NONE,          .val3 =  0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x080643a4
 */
const ItemGfx ShieldGfx[8] = {
    // [0] "Buckler"
    ITEM_GFX(ShieldBuckler, LOOT_TYPE_SHIELD_TIER_1),
    // [1] "Small Shield"
    ITEM_GFX(ShieldSmallShield, LOOT_TYPE_SHIELD_TIER_1),
    // [2] "Heater Shield"
    ITEM_GFX(ShieldHeaterShield, LOOT_TYPE_SHIELD_TIER_1),
    // [3] "Splinted Shield"
    ITEM_GFX(ShieldSplintedShield, LOOT_TYPE_SHIELD_TIER_1),
    // [4] "Round Shield"
    ITEM_GFX(ShieldRoundShield, LOOT_TYPE_SHIELD_TIER_1),
    // [5] "Kite Shield"
    ITEM_GFX(ShieldKiteShield, LOOT_TYPE_SHIELD_TIER_2),
    // [6] "Large Shield"
    ITEM_GFX(ShieldLargeShield, LOOT_TYPE_SHIELD_TIER_2),
    // [7] "Tower Shield"
    ITEM_GFX(ShieldTowerShield, LOOT_TYPE_SHIELD_TIER_2),
};

/** most common row (4 of 8); deviations are spelled out per entry */
#define FLAGS_SHIELD { .familyShield = 1, .slotOffhand = 1, .classFrodo = 1, .classAragorn = 1, .classEowyn = 1, .classGimli = 1 }

/** @romaddress 0x08064484 */
const ItemMetadata ShieldItemFlags[8] = {
    // [0] "Buckler"
    { .flags.d = FLAGS_SHIELD, .equipSfx = 0x26 },

    // [1] "Small Shield"
    { .flags.d = FLAGS_SHIELD, .equipSfx = 0x26 },

    // [2] "Heater Shield"
    { .flags.d = FLAGS_SHIELD, .equipSfx = 0x26 },

    // [3] "Splinted Shield"
    { .flags.d = { .familyShield = 1,
                   .ancient = 1,
                   .slotOffhand = 1,
                   .classFrodo = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x26 },

    // [4] "Round Shield"
    { .flags.d = FLAGS_SHIELD, .equipSfx = 0x26 },

    // [5] "Kite Shield"
    { .flags.d = { .familyShield = 1,
                   .slotOffhand = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x26 },

    // [6] "Large Shield"
    { .flags.d = { .familyShield = 1,
                   .slotOffhand = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x26 },

    // [7] "Tower Shield"
    { .flags.d = { .familyShield = 1,
                   .slotOffhand = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x26 },
};
#undef FLAGS_SHIELD
// clang-format on
