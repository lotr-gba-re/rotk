#include "item.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off

// TODO: [1] "Flint Knife" carries .val3=1 on an empty (STAT_NONE) slot - meaning unknown

/** @romaddress 0x08062e7c */
const ItemBase KnifeItems[18] = {
    // [0] "Spoon": Impaling Damage +3, % Critical +30
    { .nameId = TEXT_ID_ITEM_SPOON,
      .tier = 1,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 =  3,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 30,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [1] "Flint Knife": Impaling Damage +5, % Critical +20, Accuracy +1
    { .nameId = TEXT_ID_ITEM_FLINT_KNIFE,
      .tier = 1,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 =  5,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 20,
      .stat2 = STAT_ACCURACY,            .val2 =  1,
      .stat3 = STAT_NONE,                .val3 =  1 },

    // [2] "Dirk": Impaling Damage +4, % Critical +25, Accuracy +2, Armor Piercing +1
    { .nameId = TEXT_ID_ITEM_DIRK,
      .tier = 1,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 =  4,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 25,
      .stat2 = STAT_ACCURACY,            .val2 =  2,
      .stat3 = STAT_ARMOR_PIERCE,        .val3 =  1 },

    // [3] "Dagger": Impaling Damage +4, % Critical +25, Accuracy +3, Armor Piercing +1
    { .nameId = TEXT_ID_ITEM_DAGGER,
      .tier = 1,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 =  4,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 25,
      .stat2 = STAT_ACCURACY,            .val2 =  3,
      .stat3 = STAT_ARMOR_PIERCE,        .val3 =  1 },

    // [4] "Quillon": Impaling Damage +5, % Critical +20, Accuracy +2, Armor Piercing +1
    { .nameId = TEXT_ID_ITEM_QUILLON,
      .tier = 2,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 =  5,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 20,
      .stat2 = STAT_ACCURACY,            .val2 =  2,
      .stat3 = STAT_ARMOR_PIERCE,        .val3 =  1 },

    // [5] "Knife": Impaling Damage +6, % Critical +20, Accuracy +2
    { .nameId = TEXT_ID_ITEM_KNIFE,
      .tier = 2,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 =  6,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 20,
      .stat2 = STAT_ACCURACY,            .val2 =  2,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [6] "Long Knife": Melee Damage +6, % Critical +15, Accuracy +1, Armor Piercing +1
    { .nameId = TEXT_ID_ITEM_LONG_KNIFE,
      .tier = 3,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 =  6,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_ACCURACY,            .val2 =  1,
      .stat3 = STAT_ARMOR_PIERCE,        .val3 =  1 },

    // [7] "Rudius": Impaling Damage +6, % Critical +15, Strength +2
    { .nameId = TEXT_ID_ITEM_RUDIUS,
      .tier = 3,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 =  6,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_STRENGTH,            .val2 =  2,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [8] "Shortsword": Melee Damage +7, % Critical +15
    { .nameId = TEXT_ID_ITEM_SHORTSWORD,
      .tier = 4,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 =  7,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [9] "Courtsword": Impaling Damage +6, % Critical +20, Defense +2
    { .nameId = TEXT_ID_ITEM_COURTSWORD,
      .tier = 4,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 =  6,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 20,
      .stat2 = STAT_DEFENSE,             .val2 =  2,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [10] "Orcsword": Melee Damage +8, % Critical +10, Strength +3
    { .nameId = TEXT_ID_ITEM_ORCSWORD,
      .tier = 4,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 =  8,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_STRENGTH,            .val2 =  3,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [11] "5-finger Knife": Melee Damage +5, % Critical +35, Armor Piercing +1, Accuracy +3
    { .nameId = TEXT_ID_ITEM_5_FINGER_KNIFE,
      .tier = 4,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 =  5,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 35,
      .stat2 = STAT_ARMOR_PIERCE,        .val2 =  1,
      .stat3 = STAT_ACCURACY,            .val3 =  3 },

    // [12] "Gladius": Impaling Damage +8, % Critical +15, Strength +3
    { .nameId = TEXT_ID_ITEM_GLADIUS,
      .tier = 5,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 =  8,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_STRENGTH,            .val2 =  3,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [13] "Cultellus": Impaling Damage +8, % Critical +15, Armor Piercing +1, Strength +3
    { .nameId = TEXT_ID_ITEM_CULTELLUS,
      .tier = 5,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 =  8,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_ARMOR_PIERCE,        .val2 =  1,
      .stat3 = STAT_STRENGTH,            .val3 =  3 },

    // [14] "Smallsword": Impaling Damage +9, % Critical +15, Defense +2
    { .nameId = TEXT_ID_ITEM_SMALLSWORD,
      .tier = 6,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 =  9,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_DEFENSE,             .val2 =  2,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [15] "Cutlass": Melee Damage +10, % Critical +15, Defense -2
    { .nameId = TEXT_ID_ITEM_CUTLASS,
      .tier = 6,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 10,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_DEFENSE,             .val2 = -2,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [16] "Sabre": Melee Damage +9, % Critical +15, Defense +3
    { .nameId = TEXT_ID_ITEM_SABRE,
      .tier = 7,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 =  9,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_DEFENSE,             .val2 =  3,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [17] "Cavalry Sabre": Melee Damage +10, % Critical +15
    { .nameId = TEXT_ID_ITEM_CAVALRY_SABRE,
      .tier = 8,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 10,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x08062f54
 */
const ItemGfx KnifeGfx[18] = {
    // [0] "Spoon"
    ITEM_GFX(KnifeSpoon, LOOT_TYPE_SPOON),
    // [1] "Flint Knife"
    ITEM_GFX(KnifeFlintKnife, LOOT_TYPE_KNIFE_TIER_1),
    // [2] "Dirk"
    ITEM_GFX(KnifeDirk, LOOT_TYPE_KNIFE_TIER_1),
    // [3] "Dagger"
    ITEM_GFX(KnifeDagger, LOOT_TYPE_KNIFE_TIER_1),
    // [4] "Quillon"
    ITEM_GFX(KnifeQuillon, LOOT_TYPE_KNIFE_TIER_1),
    // [5] "Knife"
    ITEM_GFX(KnifeKnife, LOOT_TYPE_KNIFE_TIER_1),
    // [6] "Long Knife"
    ITEM_GFX(KnifeLongKnife, LOOT_TYPE_KNIFE_TIER_1),
    // [7] "Rudius"
    ITEM_GFX(KnifeRudius, LOOT_TYPE_KNIFE_TIER_1),
    // [8] "Shortsword"
    ITEM_GFX(KnifeShortsword, LOOT_TYPE_KNIFE_TIER_2),
    // [9] "Courtsword"
    ITEM_GFX(KnifeCourtsword, LOOT_TYPE_KNIFE_TIER_2),
    // [10] "Orcsword"
    ITEM_GFX(KnifeOrcsword, LOOT_TYPE_KNIFE_TIER_2),
    // [11] "5-finger Knife"
    ITEM_GFX(Knife5FingerKnife, LOOT_TYPE_KNIFE_TIER_2),
    // [12] "Gladius"
    ITEM_GFX(KnifeGladius, LOOT_TYPE_KNIFE_TIER_2),
    // [13] "Cultellus"
    ITEM_GFX(KnifeCultellus, LOOT_TYPE_KNIFE_TIER_2),
    // [14] "Smallsword"
    ITEM_GFX(KnifeSmallsword, LOOT_TYPE_KNIFE_TIER_2),
    // [15] "Cutlass"
    ITEM_GFX(KnifeCutlass, LOOT_TYPE_KNIFE_TIER_2),
    // [16] "Sabre"
    ITEM_GFX(KnifeSabre, LOOT_TYPE_KNIFE_TIER_2),
    // [17] "Cavalry Sabre"
    ITEM_GFX(KnifeCavalrySabre, LOOT_TYPE_KNIFE_TIER_2),
};

/** most common row (16 of 18); deviations are spelled out per entry */
#define FLAGS_KNIFE { .familySword = 1, .slotWeapon = 1, .slotOffhand = 1, .classFrodo = 1, .classAragorn = 1, .classGandalf = 1, .classEowyn = 1 }

/** @romaddress 0x0806314c */
const ItemMetadata KnifeItemFlags[18] = {
    // [0] "Spoon"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [1] "Flint Knife"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [2] "Dirk"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [3] "Dagger"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [4] "Quillon"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [5] "Knife"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [6] "Long Knife"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [7] "Rudius"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [8] "Shortsword"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [9] "Courtsword"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [10] "Orcsword"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [11] "5-finger Knife"
    { .flags.d = { .familySword = 1,
                   .ancient = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classFrodo = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1 },
      .equipSfx = 0x27 },

    // [12] "Gladius"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [13] "Cultellus"
    { .flags.d = { .familySword = 1,
                   .ancient = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classFrodo = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1 },
      .equipSfx = 0x27 },

    // [14] "Smallsword"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [15] "Cutlass"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [16] "Sabre"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },

    // [17] "Cavalry Sabre"
    { .flags.d = FLAGS_KNIFE, .equipSfx = 0x27 },
};
#undef FLAGS_KNIFE
// clang-format on
