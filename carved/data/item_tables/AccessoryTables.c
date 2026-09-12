#include "item.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off


/** @romaddress 0x080644c4 */
const ItemBase AccessoryItems[12] = {
    // [0] "Collar": Defense +1
    { .nameId = TEXT_ID_ITEM_COLLAR,
      .tier = 1,
      .stat0 = STAT_DEFENSE,     .val0 =  1,
      .stat1 = STAT_NONE,        .val1 =  0,
      .stat2 = STAT_NONE,        .val2 =  0,
      .stat3 = STAT_NONE,        .val3 =  0 },

    // [1] "Keepsake": Accuracy +1
    { .nameId = TEXT_ID_ITEM_KEEPSAKE,
      .tier = 1,
      .stat0 = STAT_ACCURACY,    .val0 =  1,
      .stat1 = STAT_NONE,        .val1 =  0,
      .stat2 = STAT_NONE,        .val2 =  0,
      .stat3 = STAT_NONE,        .val3 =  0 },

    // [2] "Choker": Health +2
    { .nameId = TEXT_ID_ITEM_CHOKER,
      .tier = 2,
      .stat0 = STAT_HEALTH,      .val0 =  2,
      .stat1 = STAT_NONE,        .val1 =  0,
      .stat2 = STAT_NONE,        .val2 =  0,
      .stat3 = STAT_NONE,        .val3 =  0 },

    // [3] "Clasp": Courage +2
    { .nameId = TEXT_ID_ITEM_CLASP,
      .tier = 3,
      .stat0 = STAT_COURAGE,     .val0 =  2,
      .stat1 = STAT_NONE,        .val1 =  0,
      .stat2 = STAT_NONE,        .val2 =  0,
      .stat3 = STAT_NONE,        .val3 =  0 },

    // [4] "Pendant": Strength +2
    { .nameId = TEXT_ID_ITEM_PENDANT,
      .tier = 3,
      .stat0 = STAT_STRENGTH,    .val0 =  2,
      .stat1 = STAT_NONE,        .val1 =  0,
      .stat2 = STAT_NONE,        .val2 =  0,
      .stat3 = STAT_NONE,        .val3 =  0 },

    // [5] "Necklace": Damage +2
    { .nameId = TEXT_ID_ITEM_NECKLACE,
      .tier = 4,
      .stat0 = STAT_DAMAGE_FLAT, .val0 =  2,
      .stat1 = STAT_NONE,        .val1 =  0,
      .stat2 = STAT_NONE,        .val2 =  0,
      .stat3 = STAT_NONE,        .val3 =  0 },

    // [6] "Charm": Courage +3
    { .nameId = TEXT_ID_ITEM_CHARM,
      .tier = 5,
      .stat0 = STAT_COURAGE,     .val0 =  3,
      .stat1 = STAT_NONE,        .val1 =  0,
      .stat2 = STAT_NONE,        .val2 =  0,
      .stat3 = STAT_NONE,        .val3 =  0 },

    // [7] "Medallion": Hit Points +20
    { .nameId = TEXT_ID_ITEM_MEDALLION,
      .tier = 6,
      .stat0 = STAT_MAX_HP,      .val0 = 20,
      .stat1 = STAT_NONE,        .val1 =  0,
      .stat2 = STAT_NONE,        .val2 =  0,
      .stat3 = STAT_NONE,        .val3 =  0 },

    // [8] "Talisman": Spirit Points +25
    { .nameId = TEXT_ID_ITEM_TALISMAN,
      .tier = 7,
      .stat0 = STAT_MAX_SPIRIT,  .val0 = 25,
      .stat1 = STAT_NONE,        .val1 =  0,
      .stat2 = STAT_NONE,        .val2 =  0,
      .stat3 = STAT_NONE,        .val3 =  0 },

    // [9] "Amulet": Melee Armor +2
    { .nameId = TEXT_ID_ITEM_AMULET,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR, .val0 =  2,
      .stat1 = STAT_NONE,        .val1 =  0,
      .stat2 = STAT_NONE,        .val2 =  0,
      .stat3 = STAT_NONE,        .val3 =  0 },

    // [10] "Brooch": Damage +3
    { .nameId = TEXT_ID_ITEM_BROOCH,
      .tier = 8,
      .stat0 = STAT_DAMAGE_FLAT, .val0 =  3,
      .stat1 = STAT_NONE,        .val1 =  0,
      .stat2 = STAT_NONE,        .val2 =  0,
      .stat3 = STAT_NONE,        .val3 =  0 },

    // [11] "Firinga": Strength +5, Accuracy +3
    { .nameId = TEXT_ID_ITEM_FIRINGA,
      .tier = 10,
      .stat0 = STAT_STRENGTH,    .val0 =  5,
      .stat1 = STAT_ACCURACY,    .val1 =  3,
      .stat2 = STAT_NONE,        .val2 =  0,
      .stat3 = STAT_NONE,        .val3 =  0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x08064554
 */
const ItemGfx AccessoryGfx[12] = {
    // [0] "Collar"
    ITEM_GFX(AccessoryCollar, LOOT_TYPE_ACCESSORY_COLLAR),
    // [1] "Keepsake"
    ITEM_GFX(AccessoryKeepsake, LOOT_TYPE_ACCESSORY_TRINKET),
    // [2] "Choker"
    ITEM_GFX(AccessoryChoker, LOOT_TYPE_ACCESSORY_COLLAR),
    // [3] "Clasp"
    ITEM_GFX(AccessoryClasp, LOOT_TYPE_ACCESSORY_TRINKET),
    // [4] "Pendant"
    ITEM_GFX(AccessoryPendant, LOOT_TYPE_ACCESSORY_AMULET),
    // [5] "Necklace"
    ITEM_GFX(AccessoryNecklace, LOOT_TYPE_ACCESSORY_AMULET),
    // [6] "Charm"
    ITEM_GFX(AccessoryCharm, LOOT_TYPE_ACCESSORY_TRINKET),
    // [7] "Medallion"
    ITEM_GFX(AccessoryMedallion, LOOT_TYPE_ACCESSORY_AMULET),
    // [8] "Talisman"
    ITEM_GFX(AccessoryTalisman, LOOT_TYPE_ACCESSORY_AMULET),
    // [9] "Amulet"
    ITEM_GFX(AccessoryAmulet, LOOT_TYPE_ACCESSORY_AMULET),
    // [10] "Brooch"
    ITEM_GFX(AccessoryBrooch, LOOT_TYPE_ACCESSORY_TRINKET),
    // [11] "Firinga"
    ITEM_GFX(AccessoryFiringa, LOOT_TYPE_ACCESSORY_AMULET),
};

/** most common row (12 of 12); deviations are spelled out per entry */
#define FLAGS_ACCESSORY { .slotJewelry = 1, .classFrodo = 1, .classLegolas = 1, .classAragorn = 1, .classGandalf = 1, .classEowyn = 1, .classGimli = 1 }

/** @romaddress 0x080646a4 */
const ItemMetadata AccessoryItemFlags[12] = {
    // [0] "Collar"
    { .flags.d = FLAGS_ACCESSORY, .equipSfx = 0x25 },

    // [1] "Keepsake"
    { .flags.d = FLAGS_ACCESSORY, .equipSfx = 0x25 },

    // [2] "Choker"
    { .flags.d = FLAGS_ACCESSORY, .equipSfx = 0x25 },

    // [3] "Clasp"
    { .flags.d = FLAGS_ACCESSORY, .equipSfx = 0x25 },

    // [4] "Pendant"
    { .flags.d = FLAGS_ACCESSORY, .equipSfx = 0x25 },

    // [5] "Necklace"
    { .flags.d = FLAGS_ACCESSORY, .equipSfx = 0x25 },

    // [6] "Charm"
    { .flags.d = FLAGS_ACCESSORY, .equipSfx = 0x25 },

    // [7] "Medallion"
    { .flags.d = FLAGS_ACCESSORY, .equipSfx = 0x25 },

    // [8] "Talisman"
    { .flags.d = FLAGS_ACCESSORY, .equipSfx = 0x25 },

    // [9] "Amulet"
    { .flags.d = FLAGS_ACCESSORY, .equipSfx = 0x25 },

    // [10] "Brooch"
    { .flags.d = FLAGS_ACCESSORY, .equipSfx = 0x25 },

    // [11] "Firinga"
    { .flags.d = FLAGS_ACCESSORY, .equipSfx = 0x25 },
};
#undef FLAGS_ACCESSORY
// clang-format on
