#include "item.h"
#include "libc.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off

// TODO: [32] "Crystal Crook of Jas Mynn" has STAT_FRAGILE in .stat2 - never procs on a base item,
//       see combat_fragileOnKill (reads only the prefix's stat1)

/** @romaddress 0x08064b84 */
const ItemBase UniqueItems[53] = {
    // [0] "Sting": Impaling Damage +14, Armor Piercing +1, dmg to Orcs +6, Courage +4
    { .nameId = TEXT_ID_UNIQUE_STING,
      .tier = 9,
      .stat0 = STAT_DAMAGE_IMPALE,             .val0 =  14,
      .stat1 = STAT_ARMOR_PIERCE,              .val1 =   1,
      .stat2 = STAT_DAMAGE_TO_ORCS,            .val2 =   6,
      .stat3 = STAT_COURAGE,                   .val3 =   4 },

    // [1] "Symbolblade": Melee Damage +12, All Primary Stats +3, Health +3, dmg to Orcs +2
    { .nameId = TEXT_ID_UNIQUE_SYMBOLBLADE,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =  12,
      .stat1 = STAT_ALL_PRIMARY_STATS,         .val1 =   3,
      .stat2 = STAT_HEALTH,                    .val2 =   3,
      .stat3 = STAT_DAMAGE_TO_ORCS,            .val3 =   2 },

    // [2] "Glamdring": Melee Damage +17, Courage +6, Wisdom +5, Spirit Points +15
    { .nameId = TEXT_ID_UNIQUE_GLAMDRING,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =  17,
      .stat1 = STAT_COURAGE,                   .val1 =   6,
      .stat2 = STAT_WISDOM,                    .val2 =   5,
      .stat3 = STAT_MAX_SPIRIT,                .val3 =  15 },

    // [3] "Orcrist": Melee Damage +17, Knockback +10, dmg to Orcs +8
    { .nameId = TEXT_ID_UNIQUE_ORCRIST,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =  17,
      .stat1 = STAT_KNOCKBACK,                 .val1 =  10,
      .stat2 = STAT_DAMAGE_TO_ORCS,            .val2 =   8,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [4] "Narsil": Melee Damage +30, Accuracy +10, Strength +10, Courage +5
    { .nameId = TEXT_ID_UNIQUE_NARSIL,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =  30,
      .stat1 = STAT_ACCURACY,                  .val1 =  10,
      .stat2 = STAT_STRENGTH,                  .val2 =  10,
      .stat3 = STAT_COURAGE,                   .val3 =   5 },

    // [5] "Hadhafang": Melee Damage +18, Health +8, Strength +4
    { .nameId = TEXT_ID_UNIQUE_HADHAFANG,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =  18,
      .stat1 = STAT_HEALTH,                    .val1 =   8,
      .stat2 = STAT_STRENGTH,                  .val2 =   4,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [6] "Silveraxe": Melee Damage +23, Gemfinder +1, Strength +7, Valuable +1
    { .nameId = TEXT_ID_UNIQUE_SILVERAXE,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =  23,
      .stat1 = STAT_GEMFINDER,                 .val1 =   1,
      .stat2 = STAT_STRENGTH,                  .val2 =   7,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [7] "Staff of Five Mages": Melee Damage +8, Courage +5, Wisdom +4, Spirit Points +25
    { .nameId = TEXT_ID_UNIQUE_STAFF_OF_FIVE_MAGES,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =   8,
      .stat1 = STAT_COURAGE,                   .val1 =   5,
      .stat2 = STAT_WISDOM,                    .val2 =   4,
      .stat3 = STAT_MAX_SPIRIT,                .val3 =  25 },

    // [8] "Starfall Bow": Impaling Damage +20, Sunburst +1, Health +3, Strength +5
    { .nameId = TEXT_ID_UNIQUE_STARFALL_BOW,
      .tier = 9,
      .stat0 = STAT_DAMAGE_IMPALE,             .val0 =  20,
      .stat1 = STAT_SUNBURST,                  .val1 =   1,
      .stat2 = STAT_HEALTH,                    .val2 =   3,
      .stat3 = STAT_STRENGTH,                  .val3 =   5 },

    // [9] "Nightfell Bow": Impaling Damage +20, Nightburst +1, Accuracy +5, Defense +3
    { .nameId = TEXT_ID_UNIQUE_NIGHTFELL_BOW,
      .tier = 9,
      .stat0 = STAT_DAMAGE_IMPALE,             .val0 =  20,
      .stat1 = STAT_NIGHTBURST,                .val1 =   1,
      .stat2 = STAT_ACCURACY,                  .val2 =   5,
      .stat3 = STAT_DEFENSE,                   .val3 =   3 },

    // [10] "Celeborn's Phial": % Poison Resistance +75
    { .nameId = TEXT_ID_UNIQUE_CELEBORNS_PHIAL,
      .tier = 9,
      .stat0 = STAT_POISON_RESIST_PERCENT,     .val0 =  75,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [11] "Boromir's Horn": % Experience Bonus +10, Courage +3
    { .nameId = TEXT_ID_UNIQUE_BOROMIRS_HORN,
      .tier = 9,
      .stat0 = STAT_EXTRA_EXP_PERCENT,         .val0 =  10,
      .stat1 = STAT_COURAGE,                   .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [12] "Cyclopedia of Herbs": Hit Points from herbs +25
    { .nameId = TEXT_ID_UNIQUE_CYCLOPEDIA_OF_HERBS,
      .tier = 9,
      .stat0 = STAT_HP_FROM_HERBS,             .val0 =  25,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [13] "Sam's Cookpot": Hit Points Regen +4
    { .nameId = TEXT_ID_UNIQUE_SAMS_COOKPOT,
      .tier = 9,
      .stat0 = STAT_HP_REGEN,                  .val0 =   4,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [14] "Fletching Kit": Damage +3
    { .nameId = TEXT_ID_UNIQUE_FLETCHING_KIT,
      .tier = 9,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   3,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [15] "Scrolls of Minas Tirith": Wisdom +4
    { .nameId = TEXT_ID_UNIQUE_SCROLLS_OF_MINAS_TIRITH,
      .tier = 9,
      .stat0 = STAT_WISDOM,                    .val0 =   4,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [16] "Evenstar of Arwen": Courage +20
    { .nameId = TEXT_ID_UNIQUE_EVENSTAR_OF_ARWEN,
      .tier = 9,
      .stat0 = STAT_COURAGE,                   .val0 =  20,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [17] "Galadriel's Phial": % Fear Resistance +50, % Poison Resistance +50
    { .nameId = TEXT_ID_UNIQUE_GALADRIELS_PHIAL,
      .tier = 9,
      .stat0 = STAT_FEAR_RESIST_PERCENT,       .val0 =  50,
      .stat1 = STAT_POISON_RESIST_PERCENT,     .val1 =  50,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [18] "Forgotten Palantir": % Corruption Resistance -50, All Primary Stats +5
    { .nameId = TEXT_ID_UNIQUE_FORGOTTEN_PALANTIR,
      .tier = 9,
      .stat0 = STAT_CORRUPTION_RESIST_PERCENT, .val0 = -50,
      .stat1 = STAT_ALL_PRIMARY_STATS,         .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [19] "Cracked Palantir": % Corruption Resistance -30, All Primary Stats +3
    { .nameId = TEXT_ID_UNIQUE_CRACKED_PALANTIR,
      .tier = 9,
      .stat0 = STAT_CORRUPTION_RESIST_PERCENT, .val0 = -30,
      .stat1 = STAT_ALL_PRIMARY_STATS,         .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [20] "Orc Drum": Accuracy -5, Valuable +1
    { .nameId = TEXT_ID_UNIQUE_ORC_DRUM,
      .tier = 9,
      .stat0 = STAT_ACCURACY,                  .val0 =  -5,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [21] "Woven Goblin Doll": Strength +1, Defense +1, Health +1
    { .nameId = TEXT_ID_UNIQUE_WOVEN_GOBLIN_DOLL,
      .tier = 9,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_DEFENSE,                   .val1 =   1,
      .stat2 = STAT_HEALTH,                    .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [22] "Oliphaunt Ivory Statuette": Health +3, Melee Armor +2
    { .nameId = TEXT_ID_UNIQUE_OLIPHAUNT_IVORY_STATUETTE,
      .tier = 9,
      .stat0 = STAT_HEALTH,                    .val0 =   3,
      .stat1 = STAT_MELEE_ARMOR,               .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [23] "Orc Head": Courage +1
    { .nameId = TEXT_ID_UNIQUE_ORC_HEAD,
      .tier = 9,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [24] "Swollen Spider Glands": Damage +3
    { .nameId = TEXT_ID_UNIQUE_SWOLLEN_SPIDER_GLANDS,
      .tier = 9,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   3,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [25] "Featherwood Flute": % Shield Block +10, % Dodge +5
    { .nameId = TEXT_ID_UNIQUE_FEATHERWOOD_FLUTE,
      .tier = 9,
      .stat0 = STAT_BLOCK_PERCENT,             .val0 =  10,
      .stat1 = STAT_DODGE_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [26] "King Aldazar's Longsword": Melee Damage +14, All Primary Stats +3, Courage +2, dmg to Orcs +2
    { .nameId = TEXT_ID_UNIQUE_KING_ALDAZARS_LONGSWORD,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =  14,
      .stat1 = STAT_ALL_PRIMARY_STATS,         .val1 =   3,
      .stat2 = STAT_COURAGE,                   .val2 =   2,
      .stat3 = STAT_DAMAGE_TO_ORCS,            .val3 =   2 },

    // [27] "Hawly's Sword of Leaves": Melee Damage +13, Hit Points Regen +3, dmg to Animals +4, % Experience Bonus +5
    { .nameId = TEXT_ID_UNIQUE_HAWLYS_SWORD_OF_LEAVES,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =  13,
      .stat1 = STAT_HP_REGEN,                  .val1 =   3,
      .stat2 = STAT_DAMAGE_TO_ANIMALS,         .val2 =   4,
      .stat3 = STAT_EXTRA_EXP_PERCENT,         .val3 =   5 },

    // [28] "The Witch King's Daughter": Melee Damage +29, Melee Armor +3, Venomous +1, Cursed +1
    { .nameId = TEXT_ID_UNIQUE_THE_WITCH_KINGS_DAUGHTER,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =  29,
      .stat1 = STAT_MELEE_ARMOR,               .val1 =   3,
      .stat2 = STAT_VENOMOUS,                  .val2 =   1,
      .stat3 = STAT_CURSED,                    .val3 =   1 },

    // [29] "Brakash's Dwarf Axe of Hacking": Melee Damage +25, Strength +5, % Critical +15
    { .nameId = TEXT_ID_UNIQUE_BRAKASHS_DWARF_AXE_OF_HACKING,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =  25,
      .stat1 = STAT_STRENGTH,                  .val1 =   5,
      .stat2 = STAT_CRIT_CHANCE_PERCENT,       .val2 =  15,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [30] "Meat Cleaver of Maeck": Melee Damage +24, Accuracy -10, dmg to Animals +5
    { .nameId = TEXT_ID_UNIQUE_MEAT_CLEAVER_OF_MAECK,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =  24,
      .stat1 = STAT_ACCURACY,                  .val1 = -10,
      .stat2 = STAT_DAMAGE_TO_ANIMALS,         .val2 =   5,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [31] "Green Mace of the Outlander": Melee Damage +10, Knockback +10, Hit Points Regen +5
    { .nameId = TEXT_ID_UNIQUE_GREEN_MACE_OF_THE_OUTLANDER,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =  10,
      .stat1 = STAT_KNOCKBACK,                 .val1 =  10,
      .stat2 = STAT_HP_REGEN,                  .val2 =   5,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [32] "Crystal Crook of Jas Mynn": Melee Damage +6, Wisdom +5, Fragile +1, Courage +5
    { .nameId = TEXT_ID_UNIQUE_CRYSTAL_CROOK_OF_JAS_MYNN,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =   6,
      .stat1 = STAT_WISDOM,                    .val1 =   5,
      .stat2 = STAT_FRAGILE,                   .val2 =   1,
      .stat3 = STAT_COURAGE,                   .val3 =   5 },

    // [33] "Staff of Brilee the Scarlet": Melee Damage +14, Wisdom +1, Health +7, Knockback +7
    { .nameId = TEXT_ID_UNIQUE_STAFF_OF_BRILEE_THE_SCARLET,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =  14,
      .stat1 = STAT_WISDOM,                    .val1 =   1,
      .stat2 = STAT_HEALTH,                    .val2 =   7,
      .stat3 = STAT_KNOCKBACK,                 .val3 =   7 },

    // [34] "Oakstaff of Old Thalcos": Melee Damage +4, Wisdom +5, Spirit Points +40
    { .nameId = TEXT_ID_UNIQUE_OAKSTAFF_OF_OLD_THALCOS,
      .tier = 9,
      .stat0 = STAT_DAMAGE_SLASH,              .val0 =   4,
      .stat1 = STAT_WISDOM,                    .val1 =   5,
      .stat2 = STAT_MAX_SPIRIT,                .val2 =  40,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [35] "Vella of Lorien's Goldbow": Impaling Damage +19, Accuracy +3, Sunburst +1, Armor Piercing +1
    { .nameId = TEXT_ID_UNIQUE_VELLA_OF_LORIENS_GOLDBOW,
      .tier = 9,
      .stat0 = STAT_DAMAGE_IMPALE,             .val0 =  19,
      .stat1 = STAT_ACCURACY,                  .val1 =   3,
      .stat2 = STAT_SUNBURST,                  .val2 =   1,
      .stat3 = STAT_ARMOR_PIERCE,              .val3 =   1 },

    // [36] "Maegborion Elfbow": Impaling Damage +21, Accuracy +6, Courage +3
    { .nameId = TEXT_ID_UNIQUE_MAEGBORION_ELFBOW,
      .tier = 9,
      .stat0 = STAT_DAMAGE_IMPALE,             .val0 =  21,
      .stat1 = STAT_ACCURACY,                  .val1 =   6,
      .stat2 = STAT_COURAGE,                   .val2 =   3,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [37] "Orc Lump's Big Bow": Impaling Damage +20, Strength +5, Accuracy -10, Gemfinder +1
    { .nameId = TEXT_ID_UNIQUE_ORC_LUMPS_BIG_BOW,
      .tier = 9,
      .stat0 = STAT_DAMAGE_IMPALE,             .val0 =  20,
      .stat1 = STAT_STRENGTH,                  .val1 =   5,
      .stat2 = STAT_ACCURACY,                  .val2 = -10,
      .stat3 = STAT_GEMFINDER,                 .val3 =   1 },

    // [38] "Blue Chainmail of Zephyrmaster": Melee Armor +11, Missile Armor +6, % Speed +5, % Fear Resistance +50
    { .nameId = TEXT_ID_UNIQUE_BLUE_CHAINMAIL_OF_ZEPHYRMASTER,
      .tier = 9,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =  11,
      .stat1 = STAT_MISSILE_ARMOR,             .val1 =   6,
      .stat2 = STAT_SPEED_PERCENT,             .val2 =   5,
      .stat3 = STAT_FEAR_RESIST_PERCENT,       .val3 =  50 },

    // [39] "Fomac's Mail": Melee Armor +10, Missile Armor +8, Wisdom +2, Courage +3
    { .nameId = TEXT_ID_UNIQUE_FOMACS_MAIL,
      .tier = 9,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =  10,
      .stat1 = STAT_MISSILE_ARMOR,             .val1 =   8,
      .stat2 = STAT_WISDOM,                    .val2 =   2,
      .stat3 = STAT_COURAGE,                   .val3 =   3 },

    // [40] "Scalemail of Marr Kuz the Tall": Melee Armor +12, Missile Armor +8, Hit Points +20, Strength +5
    { .nameId = TEXT_ID_UNIQUE_SCALEMAIL_OF_MARR_KUZ_THE_TALL,
      .tier = 9,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =  12,
      .stat1 = STAT_MISSILE_ARMOR,             .val1 =   8,
      .stat2 = STAT_MAX_HP,                    .val2 =  20,
      .stat3 = STAT_STRENGTH,                  .val3 =   5 },

    // [41] "Mage Crown of Ulrick": Melee Armor +1, Critical Armor +1, Wisdom +3, Courage +3
    { .nameId = TEXT_ID_UNIQUE_MAGE_CROWN_OF_ULRICK,
      .tier = 9,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =   1,
      .stat1 = STAT_CRITICAL_ARMOR,            .val1 =   1,
      .stat2 = STAT_WISDOM,                    .val2 =   3,
      .stat3 = STAT_COURAGE,                   .val3 =   3 },

    // [42] "Shining Helm of Jae Sunbae": Melee Armor +2, Critical Armor +12, % Fear Resistance +50, % Poison Resistance +50
    { .nameId = TEXT_ID_UNIQUE_SHINING_HELM_OF_JAE_SUNBAE,
      .tier = 9,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =   2,
      .stat1 = STAT_CRITICAL_ARMOR,            .val1 =  12,
      .stat2 = STAT_FEAR_RESIST_PERCENT,       .val2 =  50,
      .stat3 = STAT_POISON_RESIST_PERCENT,     .val3 =  50 },

    // [43] "Erlina's Guamanian Boar Helm": Melee Armor +1, Critical Armor +8, Health +7
    { .nameId = TEXT_ID_UNIQUE_ERLINAS_GUAMANIAN_BOAR_HELM,
      .tier = 9,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =   1,
      .stat1 = STAT_CRITICAL_ARMOR,            .val1 =   8,
      .stat2 = STAT_HEALTH,                    .val2 =   7,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [44] "Wizard Robes of Bilroy the Brown": Melee Armor +1, Missile Armor +2, Spirit Regen +3, Wisdom +2
    { .nameId = TEXT_ID_UNIQUE_WIZARD_ROBES_OF_BILROY_THE_BROWN,
      .tier = 9,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =   1,
      .stat1 = STAT_MISSILE_ARMOR,             .val1 =   2,
      .stat2 = STAT_SPIRIT_REGEN,              .val2 =   3,
      .stat3 = STAT_WISDOM,                    .val3 =   2 },

    // [45] "Love-Blessed Cloak of Adin-Ari": Melee Armor +1, Missile Armor +2, Hit Points Regen +5, % Fear Resistance +15
    { .nameId = TEXT_ID_UNIQUE_LOVE_BLESSED_CLOAK_OF_ADIN_ARI,
      .tier = 9,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =   1,
      .stat1 = STAT_MISSILE_ARMOR,             .val1 =   2,
      .stat2 = STAT_HP_REGEN,                  .val2 =   5,
      .stat3 = STAT_FEAR_RESIST_PERCENT,       .val3 =  15 },

    // [46] "Aramat's Garnet Cowl": Melee Armor +3, Missile Armor +5, Defense +4, Backstab +1
    { .nameId = TEXT_ID_UNIQUE_ARAMATS_GARNET_COWL,
      .tier = 9,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =   3,
      .stat1 = STAT_MISSILE_ARMOR,             .val1 =   5,
      .stat2 = STAT_DEFENSE,                   .val2 =   4,
      .stat3 = STAT_BACKSTAB,                  .val3 =   1 },

    // [47] "Sindalvin Gloves of Elf Bowyers": Accuracy +10
    { .nameId = TEXT_ID_UNIQUE_SINDALVIN_GLOVES_OF_ELF_BOWYERS,
      .tier = 9,
      .stat0 = STAT_ACCURACY,                  .val0 =  10,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [48] "Dusten Bardic Gloves": Wisdom +1, Accuracy +3, Strength +3
    { .nameId = TEXT_ID_UNIQUE_DUSTEN_BARDIC_GLOVES,
      .tier = 9,
      .stat0 = STAT_WISDOM,                    .val0 =   1,
      .stat1 = STAT_ACCURACY,                  .val1 =   3,
      .stat2 = STAT_STRENGTH,                  .val2 =   3,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [49] "Gauntlets of Numbers": Health +4, Damage +4
    { .nameId = TEXT_ID_UNIQUE_GAUNTLETS_OF_NUMBERS,
      .tier = 9,
      .stat0 = STAT_HEALTH,                    .val0 =   4,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   4,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [50] "The Unknown Amulet of Kempor": Strength +5, Damage +5, Cursed +1
    { .nameId = TEXT_ID_UNIQUE_THE_UNKNOWN_AMULET_OF_KEMPOR,
      .tier = 9,
      .stat0 = STAT_STRENGTH,                  .val0 =   5,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   5,
      .stat2 = STAT_CURSED,                    .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [51] "Devitt Crystal Amulet": % Poison Resistance +15, Courage +3, Accuracy +1, All Primary Stats +1
    { .nameId = TEXT_ID_UNIQUE_DEVITT_CRYSTAL_AMULET,
      .tier = 9,
      .stat0 = STAT_POISON_RESIST_PERCENT,     .val0 =  15,
      .stat1 = STAT_COURAGE,                   .val1 =   3,
      .stat2 = STAT_ACCURACY,                  .val2 =   1,
      .stat3 = STAT_ALL_PRIMARY_STATS,         .val3 =   1 },

    // [52] "Nickohare's Foot": Defense +13
    { .nameId = TEXT_ID_UNIQUE_NICKOHARES_FOOT,
      .tier = 9,
      .stat0 = STAT_DEFENSE,                   .val0 =  13,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x08064e00
 */
const ItemGfx UniqueGfx[53] = {
    // [0] "Sting"
    ITEM_GFX(UniqueSting, 0x3d),
    // [1] "Symbolblade"
    ITEM_GFX(UniqueSymbolblade, 0x3e),
    // [2] "Glamdring"
    ITEM_GFX(UniqueGlamdring, 0x3f),
    // [3] "Orcrist"
    ITEM_GFX(UniqueOrcrist, 0x40),
    // [4] "Narsil"
    ITEM_GFX(UniqueNarsil, 0x41),
    // [5] "Hadhafang"
    ITEM_GFX(UniqueHadhafang, 0x42),
    // [6] "Silveraxe"
    ITEM_GFX(UniqueSilveraxe, 0x43),
    // [7] "Staff of Five Mages"
    ITEM_GFX(UniqueStaffOfFiveMages, 0x44),
    // [8] "Starfall Bow"
    ITEM_GFX(UniqueStarfallBow, 0x45),
    // [9] "Nightfell Bow"
    ITEM_GFX(UniqueNightfellBow, 0x46),
    // [10] "Celeborn's Phial"
    ITEM_GFX_CARRIED(UniqueCelebornSPhial, 0x47),
    // [11] "Boromir's Horn"
    ITEM_GFX_CARRIED(UniqueBoromirSHorn, 0x48),
    // [12] "Cyclopedia of Herbs"
    ITEM_GFX_CARRIED(UniqueCyclopediaOfHerbs, 0x49),
    // [13] "Sam's Cookpot"
    ITEM_GFX_CARRIED(UniqueSamSCookpot, 0x4a),
    // [14] "Fletching Kit"
    ITEM_GFX_CARRIED(UniqueFletchingKit, 0x4b),
    // [15] "Scrolls of Minas Tirith"
    ITEM_GFX_CARRIED(UniqueScrollsOfMinasTirith, 0x4c),
    // [16] "Evenstar of Arwen"
    ITEM_GFX_CARRIED(UniqueEvenstarOfArwen, 0x4d),
    // [17] "Galadriel's Phial"
    ITEM_GFX_CARRIED(UniqueGaladrielSPhial, 0x4e),
    // [18] "Forgotten Palantir"
    ITEM_GFX_CARRIED(UniqueForgottenPalantir, 0x4f),
    // [19] "Cracked Palantir"
    ITEM_GFX_CARRIED(UniqueCrackedPalantir, 0x50),
    // [20] "Orc Drum"
    ITEM_GFX_CARRIED(UniqueOrcDrum, 0x51),
    // [21] "Woven Goblin Doll"
    ITEM_GFX_CARRIED(UniqueWovenGoblinDoll, 0x52),
    // [22] "Oliphaunt Ivory Statuette"
    ITEM_GFX_CARRIED(UniqueOliphauntIvoryStatuette, 0x53),
    // [23] "Orc Head"
    ITEM_GFX_CARRIED(UniqueOrcHead, 0x54),
    // [24] "Swollen Spider Glands"
    ITEM_GFX_CARRIED(UniqueSwollenSpiderGlands, 0x55),
    // [25] "Featherwood Flute"
    ITEM_GFX_CARRIED(UniqueFeatherwoodFlute, 0x56),
    // [26] "King Aldazar's Longsword"
    ITEM_GFX(UniqueKingAldazarSLongsword, LOOT_TYPE_UNIQUE_SWORD),
    // [27] "Hawly's Sword of Leaves"
    ITEM_GFX(UniqueHawlySSwordOfLeaves, LOOT_TYPE_UNIQUE_SWORD),
    // [28] "The Witch King's Daughter"
    ITEM_GFX(UniqueTheWitchKingSDaughter, LOOT_TYPE_UNIQUE_SWORD),
    // [29] "Brakash's Dwarf Axe of Hacking"
    ITEM_GFX(UniqueBrakashSDwarfAxeOfHacking, LOOT_TYPE_UNIQUE_MELEE),
    // [30] "Meat Cleaver of Maeck"
    ITEM_GFX(UniqueMeatCleaverOfMaeck, LOOT_TYPE_UNIQUE_MELEE),
    // [31] "Green Mace of the Outlander"
    ITEM_GFX(UniqueGreenMaceOfTheOutlander, LOOT_TYPE_UNIQUE_MELEE),
    // [32] "Crystal Crook of Jas Mynn"
    ITEM_GFX(UniqueCrystalCrookOfJasMynn, LOOT_TYPE_UNIQUE_STAFF),
    // [33] "Staff of Brilee the Scarlet"
    ITEM_GFX(UniqueStaffOfBrileeTheScarlet, LOOT_TYPE_UNIQUE_STAFF),
    // [34] "Oakstaff of Old Thalcos"
    ITEM_GFX(UniqueOakstaffOfOldThalcos, LOOT_TYPE_UNIQUE_STAFF),
    // [35] "Vella of Lorien's Goldbow"
    ITEM_GFX(UniqueVellaOfLorienSGoldbow, LOOT_TYPE_UNIQUE_BOW),
    // [36] "Maegborion Elfbow"
    ITEM_GFX(UniqueMaegborionElfbow, LOOT_TYPE_UNIQUE_BOW),
    // [37] "Orc Lump's Big Bow"
    ITEM_GFX(UniqueOrcLumpSBigBow, LOOT_TYPE_UNIQUE_BOW),
    // [38] "Blue Chainmail of Zephyrmaster"
    ITEM_GFX(UniqueBlueChainmailOfZephyrmaster, LOOT_TYPE_ARMOR_TIER_2),
    // [39] "Fomac's Mail"
    ITEM_GFX(UniqueFomacSMail, LOOT_TYPE_ARMOR_TIER_3),
    // [40] "Scalemail of Marr Kuz the Tall"
    ITEM_GFX(UniqueScalemailOfMarrKuzTheTall, LOOT_TYPE_ARMOR_TIER_2),
    // [41] "Mage Crown of Ulrick"
    ITEM_GFX(UniqueMageCrownOfUlrick, LOOT_TYPE_HELM_CROWN),
    // [42] "Shining Helm of Jae Sunbae"
    ITEM_GFX(UniqueShiningHelmOfJaeSunbae, LOOT_TYPE_HELM_METAL),
    // [43] "Erlina's Guamanian Boar Helm"
    ITEM_GFX(UniqueErlinaSGuamanianBoarHelm, LOOT_TYPE_HELM_METAL),
    // [44] "Wizard Robes of Bilroy the Brown"
    ITEM_GFX(UniqueWizardRobesOfBilroyTheBrown, LOOT_TYPE_CLOAK_TIER_2),
    // [45] "Love-Blessed Cloak of Adin-Ari"
    ITEM_GFX(UniqueLoveBlessedCloakOfAdinAri, LOOT_TYPE_CLOAK_TIER_2),
    // [46] "Aramat's Garnet Cowl"
    ITEM_GFX(UniqueAramatSGarnetCowl, LOOT_TYPE_CLOAK_TIER_2),
    // [47] "Sindalvin Gloves of Elf Bowyers"
    ITEM_GFX(UniqueSindalvinGlovesOfElfBowyers, LOOT_TYPE_GLOVE),
    // [48] "Dusten Bardic Gloves"
    ITEM_GFX(UniqueDustenBardicGloves, LOOT_TYPE_GLOVE),
    // [49] "Gauntlets of Numbers"
    ITEM_GFX(UniqueGauntletsOfNumbers, LOOT_TYPE_GLOVE),
    // [50] "The Unknown Amulet of Kempor"
    ITEM_GFX(UniqueTheUnknownAmuletOfKempor, LOOT_TYPE_ACCESSORY_AMULET),
    // [51] "Devitt Crystal Amulet"
    ITEM_GFX(UniqueDevittCrystalAmulet, LOOT_TYPE_ACCESSORY_AMULET),
    // [52] "Nickohare's Foot"
    ITEM_GFX(UniqueNickohareSFoot, LOOT_TYPE_ACCESSORY_AMULET),
};

/** carried-passive trinket row, most common (11 of 53); deviations are spelled out per entry */
#define FLAGS_UNIQUE_PASSIVE { .noPrefix = 1, .noSuffix = 1, .carriedPassive = 1, .classFrodo = 1, .classLegolas = 1, .classAragorn = 1, .classGandalf = 1, .classEowyn = 1, .classGimli = 1 }

/** @romaddress 0x080653cc */
const ItemMetadata UniqueItemFlags[53] = {
    // [0] "Sting"
    { .flags.d = { .familySword = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .uniqueWeapon = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classFrodo = 1 },
      .equipSfx = 0x27 },

    // [1] "Symbolblade"
    { .flags.d = { .familySword = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .uniqueWeapon = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classFrodo = 1 },
      .equipSfx = 0x27 },

    // [2] "Glamdring"
    { .flags.d = { .familySword = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .uniqueWeapon = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classGandalf = 1 },
      .equipSfx = 0x27 },

    // [3] "Orcrist"
    { .flags.d = { .familySword = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .uniqueWeapon = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1 },
      .equipSfx = 0x27 },

    // [4] "Narsil"
    { .flags.d = { .familySword = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .uniqueWeapon = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classAragorn = 1 },
      .equipSfx = 0x27 },

    // [5] "Hadhafang"
    { .flags.d = { .familySword = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .uniqueWeapon = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1 },
      .equipSfx = 0x27 },

    // [6] "Silveraxe"
    { .flags.d = { .familyAxe = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .uniqueWeapon = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classGimli = 1 },
      .equipSfx = 0x27 },

    // [7] "Staff of Five Mages"
    { .flags.d = { .familyStaff = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .uniqueWeapon = 1,
                   .slotOffhand = 1,
                   .classGandalf = 1 },
      .equipSfx = 0x2b },

    // [8] "Starfall Bow"
    { .flags.d = { .familyBow = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .uniqueWeapon = 1,
                   .slotWeapon = 1,
                   .classLegolas = 1 },
      .equipSfx = 0x2c },

    // [9] "Nightfell Bow"
    { .flags.d = { .familyBow = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .uniqueWeapon = 1,
                   .slotWeapon = 1,
                   .classLegolas = 1 },
      .equipSfx = 0x2c },

    // [10] "Celeborn's Phial"
    { .flags.d = FLAGS_UNIQUE_PASSIVE, .equipSfx = 0x21 },

    // [11] "Boromir's Horn"
    { .flags.d = FLAGS_UNIQUE_PASSIVE, .equipSfx = 0x21 },

    // [12] "Cyclopedia of Herbs"
    { .flags.d = FLAGS_UNIQUE_PASSIVE, .equipSfx = 0x21 },

    // [13] "Sam's Cookpot"
    { .flags.d = { .minLevelPlus5A = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .carriedPassive = 1,
                   .classFrodo = 1 },
      .equipSfx = 0x21 },

    // [14] "Fletching Kit"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .carriedPassive = 1,
                   .classLegolas = 1 },
      .equipSfx = 0x21 },

    // [15] "Scrolls of Minas Tirith"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .carriedPassive = 1,
                   .classGandalf = 1 },
      .equipSfx = 0x21 },

    // [16] "Evenstar of Arwen"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .carriedPassive = 1,
                   .classAragorn = 1 },
      .equipSfx = 0x21 },

    // [17] "Galadriel's Phial"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .carriedPassive = 1,
                   .classFrodo = 1 },
      .equipSfx = 0x21 },

    // [18] "Forgotten Palantir"
    { .flags.d = FLAGS_UNIQUE_PASSIVE, .equipSfx = 0x21 },

    // [19] "Cracked Palantir"
    { .flags.d = FLAGS_UNIQUE_PASSIVE, .equipSfx = 0x21 },

    // [20] "Orc Drum"
    { .flags.d = FLAGS_UNIQUE_PASSIVE, .equipSfx = 0x21 },

    // [21] "Woven Goblin Doll"
    { .flags.d = FLAGS_UNIQUE_PASSIVE, .equipSfx = 0x21 },

    // [22] "Oliphaunt Ivory Statuette"
    { .flags.d = FLAGS_UNIQUE_PASSIVE, .equipSfx = 0x21 },

    // [23] "Orc Head"
    { .flags.d = FLAGS_UNIQUE_PASSIVE, .equipSfx = 0x21 },

    // [24] "Swollen Spider Glands"
    { .flags.d = FLAGS_UNIQUE_PASSIVE, .equipSfx = 0x21 },

    // [25] "Featherwood Flute"
    { .flags.d = FLAGS_UNIQUE_PASSIVE, .equipSfx = 0x21 },

    // [26] "King Aldazar's Longsword"
    { .flags.d = { .familySword = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x27 },

    // [27] "Hawly's Sword of Leaves"
    { .flags.d = { .familySword = 1,
                   .minLevelPlus5A = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1 },
      .equipSfx = 0x27 },

    // [28] "The Witch King's Daughter"
    { .flags.d = { .familySword = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .twoHanded = 1,
                   .slotWeapon = 1,
                   .classAragorn = 1,
                   .classEowyn = 1 },
      .equipSfx = 0x27 },

    // [29] "Brakash's Dwarf Axe of Hacking"
    { .flags.d = { .familyAxe = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .twoHanded = 1,
                   .slotWeapon = 1,
                   .classGimli = 1 },
      .equipSfx = 0x27 },

    // [30] "Meat Cleaver of Maeck"
    { .flags.d = { .familyAxe = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classFrodo = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x27 },

    // [31] "Green Mace of the Outlander"
    { .flags.d = { .familyAxe = 1,
                   .minLevelPlus5A = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .slotWeapon = 1,
                   .slotOffhand = 1,
                   .classFrodo = 1,
                   .classAragorn = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x27 },

    // [32] "Crystal Crook of Jas Mynn"
    { .flags.d = { .familyStaff = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .slotOffhand = 1,
                   .classGandalf = 1 },
      .equipSfx = 0x2b },

    // [33] "Staff of Brilee the Scarlet"
    { .flags.d = { .familyStaff = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .slotOffhand = 1,
                   .classGandalf = 1 },
      .equipSfx = 0x2b },

    // [34] "Oakstaff of Old Thalcos"
    { .flags.d = { .familyStaff = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .slotOffhand = 1,
                   .classGandalf = 1 },
      .equipSfx = 0x2b },

    // [35] "Vella of Lorien's Goldbow"
    { .flags.d = { .familyBow = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .slotWeapon = 1,
                   .classLegolas = 1 },
      .equipSfx = 0x2c },

    // [36] "Maegborion Elfbow"
    { .flags.d = { .familyBow = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .slotWeapon = 1,
                   .classLegolas = 1 },
      .equipSfx = 0x2c },

    // [37] "Orc Lump's Big Bow"
    { .flags.d = { .familyBow = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .slotWeapon = 1,
                   .classLegolas = 1 },
      .equipSfx = 0x2c },

    // [38] "Blue Chainmail of Zephyrmaster"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .slotArmor = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [39] "Fomac's Mail"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .slotArmor = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [40] "Scalemail of Marr Kuz the Tall"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .slotArmor = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [41] "Mage Crown of Ulrick"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .slotHelmet = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [42] "Shining Helm of Jae Sunbae"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .slotHelmet = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [43] "Erlina's Guamanian Boar Helm"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .slotHelmet = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x20 },

    // [44] "Wizard Robes of Bilroy the Brown"
    { .flags.d = { .minLevelPlus5B = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .slotCloak = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x22 },

    // [45] "Love-Blessed Cloak of Adin-Ari"
    { .flags.d = { .minLevelPlus5A = 1,
                   .noPrefix = 1,
                   .noSuffix = 1,
                   .slotCloak = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x22 },

    // [46] "Aramat's Garnet Cowl"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .slotCloak = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x22 },

    // [47] "Sindalvin Gloves of Elf Bowyers"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .slotGloves = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x24 },

    // [48] "Dusten Bardic Gloves"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .slotGloves = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x24 },

    // [49] "Gauntlets of Numbers"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .slotGloves = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x24 },

    // [50] "The Unknown Amulet of Kempor"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .slotJewelry = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x25 },

    // [51] "Devitt Crystal Amulet"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .slotJewelry = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x25 },

    // [52] "Nickohare's Foot"
    { .flags.d = { .noPrefix = 1,
                   .noSuffix = 1,
                   .slotJewelry = 1,
                   .classFrodo = 1,
                   .classLegolas = 1,
                   .classAragorn = 1,
                   .classGandalf = 1,
                   .classEowyn = 1,
                   .classGimli = 1 },
      .equipSfx = 0x25 },
};
#undef FLAGS_UNIQUE_PASSIVE
// clang-format on
