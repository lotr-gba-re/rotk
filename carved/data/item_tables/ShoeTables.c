#include "item.h"
#include "libc.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off


/** @romaddress 0x08064a34 */
const ItemBase ShoeItems[7] = {
    // [0] "Wrappings": % Speed +10
    { .nameId = TEXT_ID_ITEM_WRAPPINGS,
      .tier = 1,
      .stat0 = STAT_SPEED_PERCENT, .val0 = 10,
      .stat1 = STAT_NONE,          .val1 =  0,
      .stat2 = STAT_NONE,          .val2 =  0,
      .stat3 = STAT_NONE,          .val3 =  0 },

    // [1] "Sandals": Melee Armor +1, % Speed +5
    { .nameId = TEXT_ID_ITEM_SANDALS,
      .tier = 2,
      .stat0 = STAT_MELEE_ARMOR,   .val0 =  1,
      .stat1 = STAT_SPEED_PERCENT, .val1 =  5,
      .stat2 = STAT_NONE,          .val2 =  0,
      .stat3 = STAT_NONE,          .val3 =  0 },

    // [2] "Clogs": Melee Armor +2, Cheap +1
    { .nameId = TEXT_ID_ITEM_CLOGS,
      .tier = 3,
      .stat0 = STAT_MELEE_ARMOR,   .val0 =  2,
      .stat1 = STAT_NONE,          .val1 =  0,
      .stat2 = STAT_NONE,          .val2 =  0,
      .stat3 = STAT_CHEAP,         .val3 =  1 },

    // [3] "Shoes": Melee Armor +3
    { .nameId = TEXT_ID_ITEM_SHOES,
      .tier = 4,
      .stat0 = STAT_MELEE_ARMOR,   .val0 =  3,
      .stat1 = STAT_NONE,          .val1 =  0,
      .stat2 = STAT_NONE,          .val2 =  0,
      .stat3 = STAT_NONE,          .val3 =  0 },

    // [4] "Boots": Melee Armor +4
    { .nameId = TEXT_ID_ITEM_BOOTS,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,   .val0 =  4,
      .stat1 = STAT_NONE,          .val1 =  0,
      .stat2 = STAT_NONE,          .val2 =  0,
      .stat3 = STAT_NONE,          .val3 =  0 },

    // [5] "Riding Boots": Melee Armor +3, % Speed +5
    { .nameId = TEXT_ID_ITEM_RIDING_BOOTS,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,   .val0 =  3,
      .stat1 = STAT_SPEED_PERCENT, .val1 =  5,
      .stat2 = STAT_NONE,          .val2 =  0,
      .stat3 = STAT_NONE,          .val3 =  0 },

    // [6] "Sollerets": Melee Armor +5
    { .nameId = TEXT_ID_ITEM_SOLLERETS,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR,   .val0 =  5,
      .stat1 = STAT_NONE,          .val1 =  0,
      .stat2 = STAT_NONE,          .val2 =  0,
      .stat3 = STAT_NONE,          .val3 =  0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x08064a88
 */
const ItemGfx ShoeGfx[7] = {
    // [0] "Wrappings"
    ITEM_GFX(ShoeWrappings, LOOT_TYPE_SHOE_TIER_1),
    // [1] "Sandals"
    ITEM_GFX(ShoeSandals, LOOT_TYPE_SHOE_TIER_1),
    // [2] "Clogs"
    ITEM_GFX(ShoeClogs, LOOT_TYPE_SHOE_TIER_1),
    // [3] "Shoes"
    ITEM_GFX(ShoeShoes, LOOT_TYPE_SHOE_TIER_1),
    // [4] "Boots"
    ITEM_GFX(ShoeBoots, LOOT_TYPE_SHOE_TIER_2),
    // [5] "Riding Boots"
    ITEM_GFX(ShoeRidingBoots, LOOT_TYPE_SHOE_TIER_2),
    // [6] "Sollerets"
    ITEM_GFX(ShoeSollerets, LOOT_TYPE_SHOE_TIER_2),
};

/** most common row (7 of 7); deviations are spelled out per entry */
#define FLAGS_SHOE { .slotBoots = 1, .classLegolas = 1, .classAragorn = 1, .classGandalf = 1, .classEowyn = 1, .classGimli = 1 }

/** @romaddress 0x08064b4c */
const ItemMetadata ShoeItemFlags[7] = {
    // [0] "Wrappings"
    { .flags.d = FLAGS_SHOE, .equipSfx = 0x21 },

    // [1] "Sandals"
    { .flags.d = FLAGS_SHOE, .equipSfx = 0x21 },

    // [2] "Clogs"
    { .flags.d = FLAGS_SHOE, .equipSfx = 0x21 },

    // [3] "Shoes"
    { .flags.d = FLAGS_SHOE, .equipSfx = 0x21 },

    // [4] "Boots"
    { .flags.d = FLAGS_SHOE, .equipSfx = 0x21 },

    // [5] "Riding Boots"
    { .flags.d = FLAGS_SHOE, .equipSfx = 0x21 },

    // [6] "Sollerets"
    { .flags.d = FLAGS_SHOE, .equipSfx = 0x21 },
};
#undef FLAGS_SHOE
// clang-format on
