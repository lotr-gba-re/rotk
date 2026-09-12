#include "item.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off


/** @romaddress 0x0806359c */
const ItemBase AxeItems[11] = {
    // [0] "Hatchet": Melee Damage +9, % Critical +5, Accuracy -2
    { .nameId = TEXT_ID_ITEM_HATCHET,
      .tier = 1,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 =  9,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_ACCURACY,            .val2 = -2,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [1] "Axe": Melee Damage +15, % Critical +5, Accuracy -2
    { .nameId = TEXT_ID_ITEM_AXE,
      .tier = 2,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 15,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_ACCURACY,            .val2 = -2,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [2] "Flat Axe": Melee Damage +16, % Critical +5, Accuracy -2
    { .nameId = TEXT_ID_ITEM_FLAT_AXE,
      .tier = 3,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 16,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_ACCURACY,            .val2 = -2,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [3] "Breidox Axe": Melee Damage +14, % Critical +5, 2H Damage +9
    { .nameId = TEXT_ID_ITEM_BREIDOX_AXE,
      .tier = 4,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 14,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_DAMAGE_TWOHAND,      .val2 =  9,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [4] "War Axe": Melee Damage +16, % Critical +10, Accuracy -2
    { .nameId = TEXT_ID_ITEM_WAR_AXE,
      .tier = 5,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 16,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_ACCURACY,            .val2 = -2,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [5] "Battle Axe": Melee Damage +17, % Critical +5, 2H Damage +11
    { .nameId = TEXT_ID_ITEM_BATTLE_AXE,
      .tier = 6,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 17,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_DAMAGE_TWOHAND,      .val2 = 11,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [6] "Skeggox Axe": Melee Damage +18, % Critical +5, 2H Damage +12
    { .nameId = TEXT_ID_ITEM_SKEGGOX_AXE,
      .tier = 7,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 18,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_DAMAGE_TWOHAND,      .val2 = 12,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [7] "Dwarf Axe": Melee Damage +19, % Critical +10, 2H Damage +12
    { .nameId = TEXT_ID_ITEM_DWARF_AXE,
      .tier = 8,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 19,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_DAMAGE_TWOHAND,      .val2 = 12,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [8] "Bardiche": Melee Damage +29, % Critical +5, Accuracy -4
    { .nameId = TEXT_ID_ITEM_BARDICHE,
      .tier = 8,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 29,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_ACCURACY,            .val2 = -4,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [9] "Pole Axe": Melee Damage +31, % Critical +5, Accuracy -4
    { .nameId = TEXT_ID_ITEM_POLE_AXE,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 31,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_ACCURACY,            .val2 = -4,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [10] "Great Axe": Melee Damage +33, % Critical +5, Accuracy -5
    { .nameId = TEXT_ID_ITEM_GREAT_AXE,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 33,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_ACCURACY,            .val2 = -5,
      .stat3 = STAT_NONE,                .val3 =  0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x08063620
 */
const ItemGfx AxeGfx[11] = {
    // [0] "Hatchet"
    ITEM_GFX(AxeHatchet, LOOT_TYPE_AXE_TIER_1),
    // [1] "Axe"
    ITEM_GFX(AxeAxe, LOOT_TYPE_AXE_TIER_1),
    // [2] "Flat Axe"
    ITEM_GFX(AxeFlatAxe, LOOT_TYPE_AXE_TIER_1),
    // [3] "Breidox Axe"
    ITEM_GFX(AxeBreidoxAxe, LOOT_TYPE_AXE_TIER_1),
    // [4] "War Axe"
    ITEM_GFX(AxeWarAxe, LOOT_TYPE_AXE_TIER_1),
    // [5] "Battle Axe"
    ITEM_GFX(AxeBattleAxe, LOOT_TYPE_AXE_TIER_2),
    // [6] "Skeggox Axe"
    ITEM_GFX(AxeSkeggoxAxe, LOOT_TYPE_AXE_TIER_2),
    // [7] "Dwarf Axe"
    ITEM_GFX(AxeDwarfAxe, LOOT_TYPE_AXE_TIER_2),
    // [8] "Bardiche"
    ITEM_GFX(AxeBardiche, LOOT_TYPE_AXE_TIER_2),
    // [9] "Pole Axe"
    ITEM_GFX(AxePoleAxe, LOOT_TYPE_AXE_TIER_2),
    // [10] "Great Axe"
    ITEM_GFX(AxeGreatAxe, LOOT_TYPE_AXE_TIER_2),
};

/** most common row (4 of 11); deviations are spelled out per entry */
#define FLAGS_AXE { .familyAxe = 1, .slotWeapon = 1, .slotOffhand = 1, .classAragorn = 1, .classEowyn = 1, .classGimli = 1 }

/** @romaddress 0x08063754 */
const ItemMetadata AxeItemFlags[11] = {
    // [0] "Hatchet"
    { .flags.d = { .familyAxe = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classFrodo = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x27 },

    // [1] "Axe"
    { .flags.d = { .familyAxe = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classFrodo = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x27 },

    // [2] "Flat Axe"
    { .flags.d = { .familyAxe = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classFrodo = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x27 },

    // [3] "Breidox Axe"
    { .flags.d = FLAGS_AXE, .equipSfx = 0x27 },

    // [4] "War Axe"
    { .flags.d = FLAGS_AXE, .equipSfx = 0x27 },

    // [5] "Battle Axe"
    { .flags.d = FLAGS_AXE, .equipSfx = 0x27 },

    // [6] "Skeggox Axe"
    { .flags.d = FLAGS_AXE, .equipSfx = 0x27 },

    // [7] "Dwarf Axe"
    { .flags.d = { .familyAxe = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classGimli = 1 },
      .equipSfx = 0x27 },

    // [8] "Bardiche"
    { .flags.d = { .familyAxe = 1,
                   .ancient = 1,
                   .twoHanded = 1,
                   .slotWeapon = 1,
                   .classGimli = 1 },
      .equipSfx = 0x27 },

    // [9] "Pole Axe"
    { .flags.d = { .familyAxe = 1,
                   .twoHanded = 1,
                   .slotWeapon = 1,
                   .classGimli = 1 },
      .equipSfx = 0x27 },

    // [10] "Great Axe"
    { .flags.d = { .familyAxe = 1,
                   .twoHanded = 1,
                   .slotWeapon = 1,
                   .classGimli = 1 },
      .equipSfx = 0x27 },
};
#undef FLAGS_AXE
// clang-format on
