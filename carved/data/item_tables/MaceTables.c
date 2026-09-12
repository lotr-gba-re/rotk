#include "item.h"
#include "libc.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off


/** @romaddress 0x080637ac */
const ItemBase MaceItems[10] = {
    // [0] "Club": Melee Damage +6, % Critical +5
    { .nameId = TEXT_ID_ITEM_CLUB,
      .tier = 1,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 =  6,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [1] "Heavy Club": Melee Damage +7, % Critical +5, 2H Damage +4
    { .nameId = TEXT_ID_ITEM_HEAVY_CLUB,
      .tier = 1,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 =  7,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_DAMAGE_TWOHAND,      .val2 =  4,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [2] "Mace": Melee Damage +8, % Critical +10
    { .nameId = TEXT_ID_ITEM_MACE,
      .tier = 2,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 =  8,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 10,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [3] "Long Mace": Melee Damage +12, % Critical +15
    { .nameId = TEXT_ID_ITEM_LONG_MACE,
      .tier = 3,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 12,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [4] "Flanged Mace": Melee Damage +13, % Critical +15
    { .nameId = TEXT_ID_ITEM_FLANGED_MACE,
      .tier = 4,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 13,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [5] "Spiked Mace": Melee Damage +14, % Critical +15
    { .nameId = TEXT_ID_ITEM_SPIKED_MACE,
      .tier = 5,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 14,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 15,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [6] "Pick": Impaling Damage +13, % Critical +5, Armor Piercing +1
    { .nameId = TEXT_ID_ITEM_PICK,
      .tier = 6,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 = 13,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_ARMOR_PIERCE,        .val2 =  1,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [7] "Mattock": Melee Damage +24, % Critical +20, Knockback +15
    { .nameId = TEXT_ID_ITEM_MATTOCK,
      .tier = 7,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 24,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 20,
      .stat2 = STAT_KNOCKBACK,           .val2 = 15,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [8] "Warhammer": Impaling Damage +22, % Critical +5, Armor Piercing +1
    { .nameId = TEXT_ID_ITEM_WARHAMMER,
      .tier = 8,
      .stat0 = STAT_DAMAGE_IMPALE,       .val0 = 22,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_ARMOR_PIERCE,        .val2 =  1,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [9] "Maul": Melee Damage +27, % Critical +20, Knockback +20
    { .nameId = TEXT_ID_ITEM_MAUL,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 27,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 = 20,
      .stat2 = STAT_KNOCKBACK,           .val2 = 20,
      .stat3 = STAT_NONE,                .val3 =  0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x08063824
 */
const ItemGfx MaceGfx[10] = {
    // [0] "Club"
    ITEM_GFX(MaceClub, LOOT_TYPE_CLUB),
    // [1] "Heavy Club"
    ITEM_GFX(MaceHeavyClub, LOOT_TYPE_CLUB),
    // [2] "Mace"
    ITEM_GFX(MaceMace, LOOT_TYPE_MACE),
    // [3] "Long Mace"
    ITEM_GFX(MaceLongMace, LOOT_TYPE_MACE),
    // [4] "Flanged Mace"
    ITEM_GFX(MaceFlangedMace, LOOT_TYPE_MACE),
    // [5] "Spiked Mace"
    ITEM_GFX(MaceSpikedMace, LOOT_TYPE_MACE),
    // [6] "Pick"
    ITEM_GFX(MacePick, LOOT_TYPE_MACE),
    // [7] "Mattock"
    ITEM_GFX(MaceMattock, LOOT_TYPE_MACE),
    // [8] "Warhammer"
    ITEM_GFX(MaceWarhammer, LOOT_TYPE_MACE),
    // [9] "Maul"
    ITEM_GFX(MaceMaul, LOOT_TYPE_CLUB),
};

/** most common row (3 of 10); deviations are spelled out per entry */
#define FLAGS_MACE { .familyMace = 1, .slotWeapon = 1, .slotOffhand = 1, .classFrodo = 1, .classAragorn = 1, .classEowyn = 1, .classGimli = 1 }

/** @romaddress 0x0806393c */
const ItemMetadata MaceItemFlags[10] = {
    // [0] "Club"
    { .flags.d = FLAGS_MACE, .equipSfx = 0x23 },

    // [1] "Heavy Club"
    { .flags.d = FLAGS_MACE, .equipSfx = 0x23 },

    // [2] "Mace"
    { .flags.d = FLAGS_MACE, .equipSfx = 0x23 },

    // [3] "Long Mace"
    { .flags.d = { .familyMace = 1,
                   .ancient = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classFrodo = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x23 },

    // [4] "Flanged Mace"
    { .flags.d = { .familyMace = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x23 },

    // [5] "Spiked Mace"
    { .flags.d = { .familyMace = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x23 },

    // [6] "Pick"
    { .flags.d = { .familyMace = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classGimli = 1 },
      .equipSfx = 0x23 },

    // [7] "Mattock"
    { .flags.d = { .familyMace = 1,
                   .twoHanded = 1,
                   .slotWeapon = 1,
                   .classGimli = 1 },
      .equipSfx = 0x23 },

    // [8] "Warhammer"
    { .flags.d = { .familyMace = 1,
                   .twoHanded = 1,
                   .slotWeapon = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x23 },

    // [9] "Maul"
    { .flags.d = { .familyMace = 1,
                   .twoHanded = 1,
                   .slotWeapon = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x23 },
};
#undef FLAGS_MACE
// clang-format on
