#include "item.h"
#include "libc.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off


/** @romaddress 0x08063a4c */
const ItemBase BowItems[13] = {
    // [0] "Shortbow": Impaling Damage +8, % Critical +5
    { .nameId = TEXT_ID_ITEM_SHORTBOW,
      .tier = 1,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 =  8,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [1] "Half Bow": Impaling Damage +9, % Critical +5
    { .nameId = TEXT_ID_ITEM_HALF_BOW,
      .tier = 2,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 =  9,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [2] "Bow": Impaling Damage +10, % Critical +5
    { .nameId = TEXT_ID_ITEM_BOW,
      .tier = 3,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 = 10,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [3] "Yew Bow": Impaling Damage +9, % Critical +10
    { .nameId = TEXT_ID_ITEM_YEW_BOW,
      .tier = 3,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 =  9,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [4] "Backed Bow": Impaling Damage +10, % Critical +10
    { .nameId = TEXT_ID_ITEM_BACKED_BOW,
      .tier = 4,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 = 10,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [5] "Hunter Bow": Impaling Damage +12, % Critical +5
    { .nameId = TEXT_ID_ITEM_HUNTER_BOW,
      .tier = 5,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 = 12,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [6] "Steelbow": Impaling Damage +10, % Critical +15
    { .nameId = TEXT_ID_ITEM_STEELBOW,
      .tier = 5,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 = 10,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [7] "Longbow": Impaling Damage +13, % Critical +10
    { .nameId = TEXT_ID_ITEM_LONGBOW,
      .tier = 6,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 = 13,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [8] "Horn Bow": Impaling Damage +11, % Critical +10, Accuracy +4
    { .nameId = TEXT_ID_ITEM_HORN_BOW,
      .tier = 6,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 = 11,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_ACCURACY,            .val2 =  4,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [9] "Composite Bow": Impaling Damage +15, % Critical +10
    { .nameId = TEXT_ID_ITEM_COMPOSITE_BOW,
      .tier = 7,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 = 15,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [10] "Recurved Bow": Impaling Damage +17, % Critical +10
    { .nameId = TEXT_ID_ITEM_RECURVED_BOW,
      .tier = 8,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 = 17,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [11] "Dynasty Bow": Impaling Damage +21, % Critical +15
    { .nameId = TEXT_ID_ITEM_DYNASTY_BOW,
      .tier = 9,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 = 21,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [12] "Great Bow": Impaling Damage +20, % Critical +10
    { .nameId = TEXT_ID_ITEM_GREAT_BOW,
      .tier = 9,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 = 20,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x08063ae8
 */
const ItemGfx BowGfx[13] = {
    // [0] "Shortbow"
    ITEM_GFX(BowShortbow, LOOT_TYPE_BOW_TIER_1),
    // [1] "Half Bow"
    ITEM_GFX(BowHalfBow, LOOT_TYPE_BOW_TIER_1),
    // [2] "Bow"
    ITEM_GFX(BowBow, LOOT_TYPE_BOW_TIER_1),
    // [3] "Yew Bow"
    ITEM_GFX(BowYewBow, LOOT_TYPE_BOW_TIER_1),
    // [4] "Backed Bow"
    ITEM_GFX(BowBackedBow, LOOT_TYPE_BOW_TIER_1),
    // [5] "Hunter Bow"
    ITEM_GFX_ICONS(BowHunterBow, BowBowEquipIcon, BowBowHollowIcon, LOOT_TYPE_BOW_TIER_1),
    // [6] "Steelbow"
    ITEM_GFX(BowSteelbow, LOOT_TYPE_BOW_TIER_1),
    // [7] "Longbow"
    ITEM_GFX(BowLongbow, LOOT_TYPE_BOW_TIER_2),
    // [8] "Horn Bow"
    ITEM_GFX(BowHornBow, LOOT_TYPE_BOW_TIER_2),
    // [9] "Composite Bow"
    ITEM_GFX(BowCompositeBow, LOOT_TYPE_BOW_TIER_2),
    // [10] "Recurved Bow"
    ITEM_GFX(BowRecurvedBow, LOOT_TYPE_BOW_TIER_2),
    // [11] "Dynasty Bow"
    ITEM_GFX(BowDynastyBow, LOOT_TYPE_BOW_TIER_2),
    // [12] "Great Bow"
    ITEM_GFX(BowGreatBow, LOOT_TYPE_BOW_TIER_2),
};

/** most common row (12 of 13); deviations are spelled out per entry */
#define FLAGS_BOW { .familyBow = 1, .slotWeapon = 1, .classLegolas = 1 }

/** @romaddress 0x08063c54 */
const ItemMetadata BowItemFlags[13] = {
    // [0] "Shortbow"
    { .flags.d = FLAGS_BOW, .equipSfx = 0x2c },

    // [1] "Half Bow"
    { .flags.d = FLAGS_BOW, .equipSfx = 0x2c },

    // [2] "Bow"
    { .flags.d = FLAGS_BOW, .equipSfx = 0x2c },

    // [3] "Yew Bow"
    { .flags.d = FLAGS_BOW, .equipSfx = 0x2c },

    // [4] "Backed Bow"
    { .flags.d = FLAGS_BOW, .equipSfx = 0x2c },

    // [5] "Hunter Bow"
    { .flags.d = FLAGS_BOW, .equipSfx = 0x2c },

    // [6] "Steelbow"
    { .flags.d = FLAGS_BOW, .equipSfx = 0x2c },

    // [7] "Longbow"
    { .flags.d = FLAGS_BOW, .equipSfx = 0x2c },

    // [8] "Horn Bow"
    { .flags.d = { .familyBow = 1,
                   .ancient = 1,
                   .slotWeapon = 1,
                   .classLegolas = 1 },
      .equipSfx = 0x2c },

    // [9] "Composite Bow"
    { .flags.d = FLAGS_BOW, .equipSfx = 0x2c },

    // [10] "Recurved Bow"
    { .flags.d = FLAGS_BOW, .equipSfx = 0x2c },

    // [11] "Dynasty Bow"
    { .flags.d = FLAGS_BOW, .equipSfx = 0x2c },

    // [12] "Great Bow"
    { .flags.d = FLAGS_BOW, .equipSfx = 0x2c },
};
#undef FLAGS_BOW
// clang-format on
