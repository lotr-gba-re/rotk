#include "item.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off


/** @romaddress 0x08063ec4 */
const ItemBase ArmorItems[24] = {
    // [0] "Cloth Shirt": Melee Armor +2, Missile Armor +2
    { .nameId = TEXT_ID_ITEM_CLOTH_SHIRT,
      .tier = 1,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  2,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 =  2,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [1] "Woven Shirt": Melee Armor +3, Missile Armor +1
    { .nameId = TEXT_ID_ITEM_WOVEN_SHIRT,
      .tier = 1,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  3,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 =  1,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [2] "Hood": Melee Armor +0, Critical Armor +1
    { .nameId = TEXT_ID_ITEM_HOOD,
      .tier = 1,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  0,
      .stat1 = STAT_CRITICAL_ARMOR, .val1 =  1,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [3] "Padded Shirt": Melee Armor +4, Missile Armor +4
    { .nameId = TEXT_ID_ITEM_PADDED_SHIRT,
      .tier = 2,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  4,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 =  4,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [4] "Leather Jacket": Melee Armor +5, Missile Armor +5
    { .nameId = TEXT_ID_ITEM_LEATHER_JACKET,
      .tier = 2,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  5,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 =  5,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [5] "Cap": Melee Armor +0, Critical Armor +5
    { .nameId = TEXT_ID_ITEM_CAP,
      .tier = 2,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  0,
      .stat1 = STAT_CRITICAL_ARMOR, .val1 =  5,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [6] "Boiled Leather": Melee Armor +6, Missile Armor +6
    { .nameId = TEXT_ID_ITEM_BOILED_LEATHER,
      .tier = 3,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  6,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 =  6,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [7] "Coif": Melee Armor +1, Critical Armor +8
    { .nameId = TEXT_ID_ITEM_COIF,
      .tier = 3,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  1,
      .stat1 = STAT_CRITICAL_ARMOR, .val1 =  8,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [8] "Crown": Melee Armor +0, Courage +5, Critical Armor +1
    { .nameId = TEXT_ID_ITEM_CROWN,
      .tier = 3,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  0,
      .stat1 = STAT_COURAGE,        .val1 =  5,
      .stat2 = STAT_CRITICAL_ARMOR, .val2 =  1,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [9] "Brigandine": Melee Armor +7, Missile Armor +7
    { .nameId = TEXT_ID_ITEM_BRIGANDINE,
      .tier = 4,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  7,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 =  7,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [10] "Ringmail": Melee Armor +8, Missile Armor +6
    { .nameId = TEXT_ID_ITEM_RINGMAIL,
      .tier = 4,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  8,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 =  6,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [11] "Pothelm": Melee Armor +0, Critical Armor +10
    { .nameId = TEXT_ID_ITEM_POTHELM,
      .tier = 4,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  0,
      .stat1 = STAT_CRITICAL_ARMOR, .val1 = 10,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [12] "Chainmail": Melee Armor +9, Missile Armor +6
    { .nameId = TEXT_ID_ITEM_CHAINMAIL,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  9,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 =  6,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [13] "Scale Mail": Melee Armor +10, Missile Armor +8
    { .nameId = TEXT_ID_ITEM_SCALE_MAIL,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,    .val0 = 10,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 =  8,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [14] "Helm": Melee Armor +0, Critical Armor +12
    { .nameId = TEXT_ID_ITEM_HELM,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  0,
      .stat1 = STAT_CRITICAL_ARMOR, .val1 = 12,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [15] "Lorica": Melee Armor +11, Missile Armor +8
    { .nameId = TEXT_ID_ITEM_LORICA,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,    .val0 = 11,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 =  8,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [16] "Double Mail": Melee Armor +12, Missile Armor +7
    { .nameId = TEXT_ID_ITEM_DOUBLE_MAIL,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,    .val0 = 12,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 =  7,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [17] "Imperial Helm": Melee Armor +1, Critical Armor +14, Courage +1
    { .nameId = TEXT_ID_ITEM_IMPERIAL_HELM,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  1,
      .stat1 = STAT_CRITICAL_ARMOR, .val1 = 14,
      .stat2 = STAT_COURAGE,        .val2 =  1,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [18] "Lamellar": Melee Armor +13, Missile Armor +10
    { .nameId = TEXT_ID_ITEM_LAMELLAR,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR,    .val0 = 13,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 = 10,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [19] "Breastplate": Melee Armor +14, Missile Armor +14
    { .nameId = TEXT_ID_ITEM_BREASTPLATE,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR,    .val0 = 14,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 = 14,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [20] "Greathelm": Melee Armor +1, Critical Armor +20, Accuracy -3
    { .nameId = TEXT_ID_ITEM_GREATHELM,
      .tier = 8,
      .stat0 = STAT_MELEE_ARMOR,    .val0 =  1,
      .stat1 = STAT_CRITICAL_ARMOR, .val1 = 20,
      .stat2 = STAT_ACCURACY,       .val2 = -3,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [21] "Corselet": Melee Armor +15, Missile Armor +15
    { .nameId = TEXT_ID_ITEM_CORSELET,
      .tier = 8,
      .stat0 = STAT_MELEE_ARMOR,    .val0 = 15,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 = 15,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [22] "Full Corselet": Melee Armor +16, Missile Armor +16
    { .nameId = TEXT_ID_ITEM_FULL_CORSELET,
      .tier = 9,
      .stat0 = STAT_MELEE_ARMOR,    .val0 = 16,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 = 16,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },

    // [23] "Footman Plate": Melee Armor +17, Missile Armor +17
    { .nameId = TEXT_ID_ITEM_FOOTMAN_PLATE,
      .tier = 9,
      .stat0 = STAT_MELEE_ARMOR,    .val0 = 17,
      .stat1 = STAT_MISSILE_ARMOR,  .val1 = 17,
      .stat2 = STAT_NONE,           .val2 =  0,
      .stat3 = STAT_NONE,           .val3 =  0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x08063fe4
 */
const ItemGfx ArmorGfx[24] = {
    // [0] "Cloth Shirt"
    ITEM_GFX(ArmorClothShirt, LOOT_TYPE_ARMOR_TIER_1),
    // [1] "Woven Shirt"
    ITEM_GFX(ArmorWovenShirt, LOOT_TYPE_ARMOR_TIER_1),
    // [2] "Hood"
    ITEM_GFX(ArmorHood, LOOT_TYPE_HELM_HOOD),
    // [3] "Padded Shirt"
    ITEM_GFX(ArmorPaddedShirt, LOOT_TYPE_ARMOR_TIER_1),
    // [4] "Leather Jacket"
    ITEM_GFX(ArmorLeatherJacket, LOOT_TYPE_ARMOR_TIER_1),
    // [5] "Cap"
    ITEM_GFX(ArmorCap, LOOT_TYPE_HELM_HOOD),
    // [6] "Boiled Leather"
    ITEM_GFX(ArmorBoiledLeather, LOOT_TYPE_ARMOR_TIER_1),
    // [7] "Coif"
    ITEM_GFX(ArmorCoif, LOOT_TYPE_HELM_HOOD),
    // [8] "Crown"
    ITEM_GFX(ArmorCrown, LOOT_TYPE_HELM_CROWN),
    // [9] "Brigandine"
    ITEM_GFX(ArmorBrigandine, LOOT_TYPE_ARMOR_TIER_2),
    // [10] "Ringmail"
    ITEM_GFX(ArmorRingmail, LOOT_TYPE_ARMOR_TIER_2),
    // [11] "Pothelm"
    ITEM_GFX(ArmorPothelm, LOOT_TYPE_HELM_METAL),
    // [12] "Chainmail"
    ITEM_GFX(ArmorChainmail, LOOT_TYPE_ARMOR_TIER_2),
    // [13] "Scale Mail"
    ITEM_GFX(ArmorScaleMail, LOOT_TYPE_ARMOR_TIER_2),
    // [14] "Helm"
    ITEM_GFX(ArmorHelm, LOOT_TYPE_HELM_METAL),
    // [15] "Lorica"
    ITEM_GFX(ArmorLorica, LOOT_TYPE_ARMOR_TIER_3),
    // [16] "Double Mail"
    ITEM_GFX(ArmorDoubleMail, LOOT_TYPE_ARMOR_TIER_3),
    // [17] "Imperial Helm"
    ITEM_GFX(ArmorImperialHelm, LOOT_TYPE_HELM_METAL),
    // [18] "Lamellar"
    ITEM_GFX(ArmorLamellar, LOOT_TYPE_ARMOR_TIER_3),
    // [19] "Breastplate"
    ITEM_GFX(ArmorBreastplate, LOOT_TYPE_ARMOR_TIER_3),
    // [20] "Greathelm"
    ITEM_GFX(ArmorGreathelm, LOOT_TYPE_HELM_METAL),
    // [21] "Corselet"
    ITEM_GFX(ArmorCorselet, LOOT_TYPE_ARMOR_TIER_3),
    // [22] "Full Corselet"
    ITEM_GFX(ArmorFullCorselet, LOOT_TYPE_ARMOR_TIER_3),
    // [23] "Footman Plate"
    ITEM_GFX(ArmorFootmanPlate, LOOT_TYPE_ARMOR_TIER_3),
};

/** most common row (11 of 24); deviations are spelled out per entry */
#define FLAGS_ARMOR { .slotArmor = 1, .classFrodo = 1, .classLegolas = 1, .classAragorn = 1, .classGandalf = 1, .classEowyn = 1, .classGimli = 1 }

/** @romaddress 0x08064284 */
const ItemMetadata ArmorItemFlags[24] = {
    // [0] "Cloth Shirt"
    { .flags.d = FLAGS_ARMOR, .equipSfx = 0x20 },

    // [1] "Woven Shirt"
    { .flags.d = FLAGS_ARMOR, .equipSfx = 0x20 },

    // [2] "Hood"
    { .flags.d = { .slotHelmet = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [3] "Padded Shirt"
    { .flags.d = FLAGS_ARMOR, .equipSfx = 0x20 },

    // [4] "Leather Jacket"
    { .flags.d = FLAGS_ARMOR, .equipSfx = 0x20 },

    // [5] "Cap"
    { .flags.d = { .slotHelmet = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [6] "Boiled Leather"
    { .flags.d = FLAGS_ARMOR, .equipSfx = 0x20 },

    // [7] "Coif"
    { .flags.d = { .slotHelmet = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [8] "Crown"
    { .flags.d = { .slotHelmet = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [9] "Brigandine"
    { .flags.d = FLAGS_ARMOR, .equipSfx = 0x20 },

    // [10] "Ringmail"
    { .flags.d = FLAGS_ARMOR, .equipSfx = 0x20 },

    // [11] "Pothelm"
    { .flags.d = { .slotHelmet = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [12] "Chainmail"
    { .flags.d = FLAGS_ARMOR, .equipSfx = 0x20 },

    // [13] "Scale Mail"
    { .flags.d = FLAGS_ARMOR, .equipSfx = 0x20 },

    // [14] "Helm"
    { .flags.d = { .slotHelmet = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [15] "Lorica"
    { .flags.d = FLAGS_ARMOR, .equipSfx = 0x20 },

    // [16] "Double Mail"
    { .flags.d = FLAGS_ARMOR, .equipSfx = 0x20 },

    // [17] "Imperial Helm"
    { .flags.d = { .slotHelmet = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [18] "Lamellar"
    { .flags.d = { .slotArmor = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [19] "Breastplate"
    { .flags.d = { .slotArmor = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [20] "Greathelm"
    { .flags.d = { .slotHelmet = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [21] "Corselet"
    { .flags.d = { .slotArmor = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [22] "Full Corselet"
    { .flags.d = { .slotArmor = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [23] "Footman Plate"
    { .flags.d = { .ancient = 1,
                   .slotArmor = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },
};
#undef FLAGS_ARMOR
// clang-format on
