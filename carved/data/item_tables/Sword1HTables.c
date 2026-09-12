#include "item.h"
#include "libc.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off


/** @romaddress 0x080631dc */
const ItemBase Sword1HItems[11] = {
    // [0] "Longsword": Melee Damage +11, % Critical +10
    { .nameId = TEXT_ID_ITEM_LONGSWORD,
      .tier = 1,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 11,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [1] "Backsword": Melee Damage +12, % Critical +5
    { .nameId = TEXT_ID_ITEM_BACKSWORD,
      .tier = 1,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 12,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [2] "Scimitar": Melee Damage +13, % Critical +5
    { .nameId = TEXT_ID_ITEM_SCIMITAR,
      .tier = 2,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 13,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [3] "Estoc": Impaling Damage +10, % Critical +15
    { .nameId = TEXT_ID_ITEM_ESTOC,
      .tier = 2,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 = 10,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [4] "Falchion": Melee Damage +14, % Critical +5, Accuracy -2
    { .nameId = TEXT_ID_ITEM_FALCHION,
      .tier = 3,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 14,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_ACCURACY,            .val2 = -2,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [5] "Broadsword": Melee Damage +14, % Critical +5
    { .nameId = TEXT_ID_ITEM_BROADSWORD,
      .tier = 4,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 14,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [6] "Steelsword": Melee Damage +15, % Critical +5
    { .nameId = TEXT_ID_ITEM_STEELSWORD,
      .tier = 5,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 15,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [7] "Leaf Sword": Melee Damage +14, % Critical +10
    { .nameId = TEXT_ID_ITEM_LEAF_SWORD,
      .tier = 6,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 14,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [8] "Prestige Sword": Melee Damage +15, % Critical +10, Courage +3
    { .nameId = TEXT_ID_ITEM_PRESTIGE_SWORD,
      .tier = 6,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 15,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_COURAGE,             .val2 =  3,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [9] "Sword Rapier": Melee Damage +15, % Critical +15
    { .nameId = TEXT_ID_ITEM_SWORD_RAPIER,
      .tier = 7,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 15,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [10] "Serrated Sword": Melee Damage +16, % Critical +5
    { .nameId = TEXT_ID_ITEM_SERRATED_SWORD,
      .tier = 8,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 16,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x08063260
 */
const ItemGfx Sword1HGfx[11] = {
    // [0] "Longsword"
    ITEM_GFX(Sword1HLongsword, LOOT_TYPE_SWORD_ONE_HAND),
    // [1] "Backsword"
    ITEM_GFX(Sword1HBacksword, LOOT_TYPE_SWORD_ONE_HAND),
    // [2] "Scimitar"
    ITEM_GFX(Sword1HScimitar, LOOT_TYPE_SWORD_ONE_HAND),
    // [3] "Estoc"
    ITEM_GFX(Sword1HEstoc, LOOT_TYPE_SWORD_ONE_HAND),
    // [4] "Falchion"
    ITEM_GFX(Sword1HFalchion, LOOT_TYPE_SWORD_ONE_HAND),
    // [5] "Broadsword"
    ITEM_GFX(Sword1HBroadsword, LOOT_TYPE_SWORD_ONE_HAND),
    // [6] "Steelsword"
    ITEM_GFX(Sword1HSteelsword, LOOT_TYPE_SWORD_ONE_HAND),
    // [7] "Leaf Sword"
    ITEM_GFX(Sword1HLeafSword, LOOT_TYPE_SWORD_ONE_HAND),
    // [8] "Prestige Sword"
    ITEM_GFX(Sword1HPrestigeSword, LOOT_TYPE_SWORD_ONE_HAND),
    // [9] "Sword Rapier"
    ITEM_GFX(Sword1HSwordRapier, LOOT_TYPE_SWORD_ONE_HAND),
    // [10] "Serrated Sword"
    ITEM_GFX(Sword1HSerratedSword, LOOT_TYPE_SWORD_ONE_HAND),
};

/** most common row (10 of 11); deviations are spelled out per entry */
#define FLAGS_SWORD1H { .familySword = 1, .slotWeapon = 1, .slotOffhand = 1, .classAragorn = 1, .classGandalf = 1, .classEowyn = 1 }

/** @romaddress 0x08063394 */
const ItemMetadata Sword1HItemFlags[11] = {
    // [0] "Longsword"
    { .flags.d = FLAGS_SWORD1H, .equipSfx = 0x27 },

    // [1] "Backsword"
    { .flags.d = FLAGS_SWORD1H, .equipSfx = 0x27 },

    // [2] "Scimitar"
    { .flags.d = FLAGS_SWORD1H, .equipSfx = 0x27 },

    // [3] "Estoc"
    { .flags.d = FLAGS_SWORD1H, .equipSfx = 0x27 },

    // [4] "Falchion"
    { .flags.d = FLAGS_SWORD1H, .equipSfx = 0x27 },

    // [5] "Broadsword"
    { .flags.d = FLAGS_SWORD1H, .equipSfx = 0x27 },

    // [6] "Steelsword"
    { .flags.d = FLAGS_SWORD1H, .equipSfx = 0x27 },

    // [7] "Leaf Sword"
    { .flags.d = FLAGS_SWORD1H, .equipSfx = 0x27 },

    // [8] "Prestige Sword"
    { .flags.d = { .familySword = 1,
                   .ancient = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1 },
      .equipSfx = 0x27 },

    // [9] "Sword Rapier"
    { .flags.d = FLAGS_SWORD1H, .equipSfx = 0x27 },

    // [10] "Serrated Sword"
    { .flags.d = FLAGS_SWORD1H, .equipSfx = 0x27 },
};
#undef FLAGS_SWORD1H
// clang-format on
