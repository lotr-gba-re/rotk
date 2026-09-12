#include "item.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off


/** @romaddress 0x080633ec */
const ItemBase Sword2HItems[9] = {
    // [0] "T-Sword": Melee Damage +19, % Critical +5, Accuracy -2
    { .nameId = TEXT_ID_ITEM_T_SWORD,
      .tier = 1,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 19,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_ACCURACY,            .val2 = -2,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [1] "Uruk Sword": Melee Damage +13, % Critical +5, 2H Damage +8
    { .nameId = TEXT_ID_ITEM_URUK_SWORD,
      .tier = 2,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 13,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_DAMAGE_TWOHAND,      .val2 =  8,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [2] "Dastard Sword": Melee Damage +14, % Critical +5, 2H Damage +9
    { .nameId = TEXT_ID_ITEM_DASTARD_SWORD,
      .tier = 3,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 14,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_DAMAGE_TWOHAND,      .val2 =  9,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [3] "Soldier Sword": Melee Damage +10, % Critical +5, 2H Damage +13
    { .nameId = TEXT_ID_ITEM_SOLDIER_SWORD,
      .tier = 4,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 10,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_DAMAGE_TWOHAND,      .val2 = 13,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [4] "War Sword": Melee Damage +22, % Critical +10
    { .nameId = TEXT_ID_ITEM_WAR_SWORD,
      .tier = 5,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 22,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [5] "Flambard": Melee Damage +20, % Critical +10, Armor Piercing +1
    { .nameId = TEXT_ID_ITEM_FLAMBARD,
      .tier = 6,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 20,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_ARMOR_PIERCE,        .val2 =  1,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [6] "Greatsword": Melee Damage +26, % Critical +5
    { .nameId = TEXT_ID_ITEM_GREATSWORD,
      .tier = 7,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 26,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [7] "Claymore": Melee Damage +27, % Critical +10
    { .nameId = TEXT_ID_ITEM_CLAYMORE,
      .tier = 8,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 27,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [8] "Imperial Sword": Melee Damage +28, % Critical +15
    { .nameId = TEXT_ID_ITEM_IMPERIAL_SWORD,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 28,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x08063458
 */
const ItemGfx Sword2HGfx[9] = {
    // [0] "T-Sword"
    ITEM_GFX(Sword2HTSword, LOOT_TYPE_SWORD_TWO_HAND_TIER_1),
    // [1] "Uruk Sword"
    ITEM_GFX(Sword2HUrukSword, LOOT_TYPE_SWORD_TWO_HAND_TIER_1),
    // [2] "Dastard Sword"
    ITEM_GFX(Sword2HDastardSword, LOOT_TYPE_SWORD_TWO_HAND_TIER_1),
    // [3] "Soldier Sword"
    ITEM_GFX(Sword2HSoldierSword, LOOT_TYPE_SWORD_TWO_HAND_TIER_1),
    // [4] "War Sword"
    ITEM_GFX(Sword2HWarSword, LOOT_TYPE_SWORD_TWO_HAND_TIER_1),
    // [5] "Flambard"
    ITEM_GFX(Sword2HFlambard, LOOT_TYPE_SWORD_TWO_HAND_TIER_2),
    // [6] "Greatsword"
    ITEM_GFX(Sword2HGreatsword, LOOT_TYPE_SWORD_TWO_HAND_TIER_2),
    // [7] "Claymore"
    ITEM_GFX(Sword2HClaymore, LOOT_TYPE_SWORD_TWO_HAND_TIER_2),
    // [8] "Imperial Sword"
    ITEM_GFX(Sword2HImperialSword, LOOT_TYPE_SWORD_TWO_HAND_TIER_2),
};

/** most common row (5 of 9); deviations are spelled out per entry */
#define FLAGS_SWORD2H { .familySword = 1, .twoHanded = 1, .slotWeapon = 1, .classAragorn = 1, .classEowyn = 1 }

/** @romaddress 0x08063554 */
const ItemMetadata Sword2HItemFlags[9] = {
    // [0] "T-Sword"
    { .flags.d = FLAGS_SWORD2H, .equipSfx = 0x27 },

    // [1] "Uruk Sword"
    { .flags.d = { .familySword = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classAragorn = 1,
                   .classEowyn = 1 },
      .equipSfx = 0x27 },

    // [2] "Dastard Sword"
    { .flags.d = { .familySword = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classAragorn = 1,
                   .classEowyn = 1 },
      .equipSfx = 0x27 },

    // [3] "Soldier Sword"
    { .flags.d = { .familySword = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classAragorn = 1,
                   .classEowyn = 1 },
      .equipSfx = 0x27 },

    // [4] "War Sword"
    { .flags.d = FLAGS_SWORD2H, .equipSfx = 0x27 },

    // [5] "Flambard"
    { .flags.d = FLAGS_SWORD2H, .equipSfx = 0x27 },

    // [6] "Greatsword"
    { .flags.d = FLAGS_SWORD2H, .equipSfx = 0x27 },

    // [7] "Claymore"
    { .flags.d = FLAGS_SWORD2H, .equipSfx = 0x27 },

    // [8] "Imperial Sword"
    { .flags.d = { .familySword = 1,
                   .ancient = 1,
                   .twoHanded = 1,
                   .slotWeapon = 1,
                   .classAragorn = 1,
                   .classEowyn = 1 },
      .equipSfx = 0x27 },
};
#undef FLAGS_SWORD2H
// clang-format on
