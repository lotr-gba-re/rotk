#include "item.h"
#include "libc.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off


/** @romaddress 0x080648b4 */
const ItemBase GloveItems[8] = {
    // [0] "Half Gloves": Accuracy +2
    { .nameId = TEXT_ID_ITEM_HALF_GLOVES,
      .tier = 1,
      .stat0 = STAT_ACCURACY,    .val0 = 2,
      .stat1 = STAT_NONE,        .val1 = 0,
      .stat2 = STAT_NONE,        .val2 = 0,
      .stat3 = STAT_NONE,        .val3 = 0 },

    // [1] "Cloth Gloves": Melee Armor +1
    { .nameId = TEXT_ID_ITEM_CLOTH_GLOVES,
      .tier = 2,
      .stat0 = STAT_MELEE_ARMOR, .val0 = 1,
      .stat1 = STAT_NONE,        .val1 = 0,
      .stat2 = STAT_NONE,        .val2 = 0,
      .stat3 = STAT_NONE,        .val3 = 0 },

    // [2] "Leather Gloves": Melee Armor +2
    { .nameId = TEXT_ID_ITEM_LEATHER_GLOVES,
      .tier = 3,
      .stat0 = STAT_MELEE_ARMOR, .val0 = 2,
      .stat1 = STAT_NONE,        .val1 = 0,
      .stat2 = STAT_NONE,        .val2 = 0,
      .stat3 = STAT_NONE,        .val3 = 0 },

    // [3] "Studded Gloves": Melee Armor +2, Defense +1
    { .nameId = TEXT_ID_ITEM_STUDDED_GLOVES,
      .tier = 4,
      .stat0 = STAT_MELEE_ARMOR, .val0 = 2,
      .stat1 = STAT_DEFENSE,     .val1 = 1,
      .stat2 = STAT_NONE,        .val2 = 0,
      .stat3 = STAT_NONE,        .val3 = 0 },

    // [4] "Chain Gloves": Melee Armor +3, Defense +1
    { .nameId = TEXT_ID_ITEM_CHAIN_GLOVES,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR, .val0 = 3,
      .stat1 = STAT_DEFENSE,     .val1 = 1,
      .stat2 = STAT_NONE,        .val2 = 0,
      .stat3 = STAT_NONE,        .val3 = 0 },

    // [5] "Scale Gloves": Melee Armor +3, Defense +2
    { .nameId = TEXT_ID_ITEM_SCALE_GLOVES,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR, .val0 = 3,
      .stat1 = STAT_DEFENSE,     .val1 = 2,
      .stat2 = STAT_NONE,        .val2 = 0,
      .stat3 = STAT_NONE,        .val3 = 0 },

    // [6] "Plated Gloves": Melee Armor +4, Defense +2
    { .nameId = TEXT_ID_ITEM_PLATED_GLOVES,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR, .val0 = 4,
      .stat1 = STAT_DEFENSE,     .val1 = 2,
      .stat2 = STAT_NONE,        .val2 = 0,
      .stat3 = STAT_NONE,        .val3 = 0 },

    // [7] "Gauntlets": Melee Armor +4, Defense +3
    { .nameId = TEXT_ID_ITEM_GAUNTLETS,
      .tier = 8,
      .stat0 = STAT_MELEE_ARMOR, .val0 = 4,
      .stat1 = STAT_DEFENSE,     .val1 = 3,
      .stat2 = STAT_NONE,        .val2 = 0,
      .stat3 = STAT_NONE,        .val3 = 0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x08064914
 */
const ItemGfx GloveGfx[8] = {
    // [0] "Half Gloves"
    ITEM_GFX(GloveHalfGloves, LOOT_TYPE_GLOVE),
    // [1] "Cloth Gloves"
    ITEM_GFX(GloveClothGloves, LOOT_TYPE_GLOVE),
    // [2] "Leather Gloves"
    ITEM_GFX(GloveLeatherGloves, LOOT_TYPE_GLOVE),
    // [3] "Studded Gloves"
    ITEM_GFX(GloveStuddedGloves, LOOT_TYPE_GLOVE),
    // [4] "Chain Gloves"
    ITEM_GFX(GloveChainGloves, LOOT_TYPE_GLOVE),
    // [5] "Scale Gloves"
    ITEM_GFX(GloveScaleGloves, LOOT_TYPE_GLOVE),
    // [6] "Plated Gloves"
    ITEM_GFX(GlovePlatedGloves, LOOT_TYPE_GLOVE),
    // [7] "Gauntlets"
    ITEM_GFX(GloveGauntlets, LOOT_TYPE_GLOVE),
};

/** most common row (8 of 8); deviations are spelled out per entry */
#define FLAGS_GLOVE { .slotGloves = 1, .classFrodo = 1, .classLegolas = 1, .classAragorn = 1, .classGandalf = 1, .classEowyn = 1, .classGimli = 1 }

/** @romaddress 0x080649f4 */
const ItemMetadata GloveItemFlags[8] = {
    // [0] "Half Gloves"
    { .flags.d = FLAGS_GLOVE, .equipSfx = 0x24 },

    // [1] "Cloth Gloves"
    { .flags.d = FLAGS_GLOVE, .equipSfx = 0x24 },

    // [2] "Leather Gloves"
    { .flags.d = FLAGS_GLOVE, .equipSfx = 0x24 },

    // [3] "Studded Gloves"
    { .flags.d = FLAGS_GLOVE, .equipSfx = 0x24 },

    // [4] "Chain Gloves"
    { .flags.d = FLAGS_GLOVE, .equipSfx = 0x24 },

    // [5] "Scale Gloves"
    { .flags.d = FLAGS_GLOVE, .equipSfx = 0x24 },

    // [6] "Plated Gloves"
    { .flags.d = FLAGS_GLOVE, .equipSfx = 0x24 },

    // [7] "Gauntlets"
    { .flags.d = FLAGS_GLOVE, .equipSfx = 0x24 },
};
#undef FLAGS_GLOVE
// clang-format on
