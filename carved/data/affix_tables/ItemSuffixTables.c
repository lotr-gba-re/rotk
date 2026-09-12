#include "item.h"
#include "stats.h"
#include "text_ids.h"

// clang-format off


/** @romaddress 0x08068550 */
const ItemAffix ItemSuffixes[160] = {
    // [0] "of the Fox": Courage +1, % Speed +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_FOX,
      .tier = 1,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_SPEED_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [1] "of the Orc King": Strength +1, Damage +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_ORC_KING,
      .tier = 1,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [2] "of Goblin Men": % Fear Resistance +10, Hit Points +5
    { .nameId = TEXT_ID_SUFFIX_OF_GOBLIN_MEN,
      .tier = 1,
      .stat0 = STAT_FEAR_RESIST_PERCENT,       .val0 =  10,
      .stat1 = STAT_MAX_HP,                    .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [3] "of the South": % Critical +5, Accuracy +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_SOUTH,
      .tier = 1,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =   5,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [4] "of the Hound": % Speed +5, % Dodge +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_HOUND,
      .tier = 1,
      .stat0 = STAT_SPEED_PERCENT,             .val0 =   5,
      .stat1 = STAT_DODGE_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [5] "of Bree": Defense +1, Health +1
    { .nameId = TEXT_ID_SUFFIX_OF_BREE,
      .tier = 1,
      .stat0 = STAT_DEFENSE,                   .val0 =   1,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [6] "of Men": Spirit Points +2, Health +4
    { .nameId = TEXT_ID_SUFFIX_OF_MEN,
      .tier = 1,
      .stat0 = STAT_MAX_SPIRIT,                .val0 =   2,
      .stat1 = STAT_HEALTH,                    .val1 =   4,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [7] "of Light": Gemfinder +1, Spirit Points +5
    { .nameId = TEXT_ID_SUFFIX_OF_LIGHT,
      .tier = 1,
      .stat0 = STAT_GEMFINDER,                 .val0 =   1,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [8] "of the Wise": Wisdom +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_WISE,
      .tier = 1,
      .stat0 = STAT_WISDOM,                    .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [9] "of the Old Forest": Melee Armor +1, Damage +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_OLD_FOREST,
      .tier = 1,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [10] "of the Strong": Damage +1, Strength +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_STRONG,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   1,
      .stat1 = STAT_STRENGTH,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [11] "of Entwash": Health +1, Hit Points +5
    { .nameId = TEXT_ID_SUFFIX_OF_ENTWASH,
      .tier = 1,
      .stat0 = STAT_HEALTH,                    .val0 =   1,
      .stat1 = STAT_MAX_HP,                    .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [12] "of Hobbiton": Courage +1, Health +1
    { .nameId = TEXT_ID_SUFFIX_OF_HOBBITON,
      .tier = 1,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [13] "of Middle Earth": Strength +1, % Extra Treasure +10
    { .nameId = TEXT_ID_SUFFIX_OF_MIDDLE_EARTH,
      .tier = 1,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [14] "of Faramir": % Critical +5, dmg to Orcs +2
    { .nameId = TEXT_ID_SUFFIX_OF_FARAMIR,
      .tier = 1,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =   5,
      .stat1 = STAT_DAMAGE_TO_ORCS,            .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [15] "of the Deep": Spirit Points +15, Gemfinder +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_DEEP,
      .tier = 1,
      .stat0 = STAT_MAX_SPIRIT,                .val0 =  15,
      .stat1 = STAT_GEMFINDER,                 .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [16] "of Midgewater": Defense +2, Melee Armor +1
    { .nameId = TEXT_ID_SUFFIX_OF_MIDGEWATER,
      .tier = 2,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_MELEE_ARMOR,               .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [17] "of Helm's Deep": Hit Points +5, Spirit Points +10
    { .nameId = TEXT_ID_SUFFIX_OF_HELMS_DEEP,
      .tier = 2,
      .stat0 = STAT_MAX_HP,                    .val0 =   5,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [18] "of Deeping": % Poison Resistance +10, % Extra Treasure +15
    { .nameId = TEXT_ID_SUFFIX_OF_DEEPING,
      .tier = 2,
      .stat0 = STAT_POISON_RESIST_PERCENT,     .val0 =  10,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  15,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [19] "of Rangers": Accuracy +2, % Speed +5
    { .nameId = TEXT_ID_SUFFIX_OF_RANGERS,
      .tier = 2,
      .stat0 = STAT_ACCURACY,                  .val0 =   2,
      .stat1 = STAT_SPEED_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [20] "of Sorrow": Spirit Points -20, Strength +3
    { .nameId = TEXT_ID_SUFFIX_OF_SORROW,
      .tier = 2,
      .stat0 = STAT_MAX_SPIRIT,                .val0 = -20,
      .stat1 = STAT_STRENGTH,                  .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [21] "of the White Hand": % Fear Resistance +10, Wisdom +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_WHITE_HAND,
      .tier = 2,
      .stat0 = STAT_FEAR_RESIST_PERCENT,       .val0 =  10,
      .stat1 = STAT_WISDOM,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [22] "of the Orcs": Strength +1, Melee Armor +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_ORCS,
      .tier = 2,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_MELEE_ARMOR,               .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [23] "of Lies": Courage -5, Defense +5
    { .nameId = TEXT_ID_SUFFIX_OF_LIES,
      .tier = 2,
      .stat0 = STAT_COURAGE,                   .val0 =  -5,
      .stat1 = STAT_DEFENSE,                   .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [24] "of Barad-Dur": Accuracy +1, Health +2
    { .nameId = TEXT_ID_SUFFIX_OF_BARAD_DUR,
      .tier = 2,
      .stat0 = STAT_ACCURACY,                  .val0 =   1,
      .stat1 = STAT_HEALTH,                    .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [25] "of the Hill Folk": % Critical +5, Hit Points +10
    { .nameId = TEXT_ID_SUFFIX_OF_THE_HILL_FOLK,
      .tier = 2,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =   5,
      .stat1 = STAT_MAX_HP,                    .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [26] "of the Dwarves": Health +2, Melee Armor +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_DWARVES,
      .tier = 2,
      .stat0 = STAT_HEALTH,                    .val0 =   2,
      .stat1 = STAT_MELEE_ARMOR,               .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [27] "of the Valley": dmg to Orcs +2, Accuracy +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_VALLEY,
      .tier = 2,
      .stat0 = STAT_DAMAGE_TO_ORCS,            .val0 =   2,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [28] "of Morgul": Strength +2, Accuracy +1
    { .nameId = TEXT_ID_SUFFIX_OF_MORGUL,
      .tier = 2,
      .stat0 = STAT_STRENGTH,                  .val0 =   2,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [29] "of Wormtongue": % Poison Resistance +25
    { .nameId = TEXT_ID_SUFFIX_OF_WORMTONGUE,
      .tier = 2,
      .stat0 = STAT_POISON_RESIST_PERCENT,     .val0 =  25,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [30] "of Saruman": Wisdom +3, Cursed +1
    { .nameId = TEXT_ID_SUFFIX_OF_SARUMAN,
      .tier = 2,
      .stat0 = STAT_WISDOM,                    .val0 =   3,
      .stat1 = STAT_CURSED,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [31] "of Mithrandir": % Extra Treasure +20, Wisdom +1
    { .nameId = TEXT_ID_SUFFIX_OF_MITHRANDIR,
      .tier = 2,
      .stat0 = STAT_EXTRA_TREASURE_PERCENT,    .val0 =  20,
      .stat1 = STAT_WISDOM,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [32] "of Merry": Accuracy +2, % Poison Resistance +10
    { .nameId = TEXT_ID_SUFFIX_OF_MERRY,
      .tier = 3,
      .stat0 = STAT_ACCURACY,                  .val0 =   2,
      .stat1 = STAT_POISON_RESIST_PERCENT,     .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [33] "of Edoras": Defense +2, Hit Points +10
    { .nameId = TEXT_ID_SUFFIX_OF_EDORAS,
      .tier = 3,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_MAX_HP,                    .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [34] "of the Mountains": Strength +2, dmg to Orcs +2
    { .nameId = TEXT_ID_SUFFIX_OF_THE_MOUNTAINS,
      .tier = 3,
      .stat0 = STAT_STRENGTH,                  .val0 =   2,
      .stat1 = STAT_DAMAGE_TO_ORCS,            .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [35] "of Woodsmen": Health +2, Missile Armor +2
    { .nameId = TEXT_ID_SUFFIX_OF_WOODSMEN,
      .tier = 3,
      .stat0 = STAT_HEALTH,                    .val0 =   2,
      .stat1 = STAT_MISSILE_ARMOR,             .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [36] "of Rohan": Courage +2, % Extra Treasure +10
    { .nameId = TEXT_ID_SUFFIX_OF_ROHAN,
      .tier = 3,
      .stat0 = STAT_COURAGE,                   .val0 =   2,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [37] "of Nomads": Accuracy -3, Hit Points +15
    { .nameId = TEXT_ID_SUFFIX_OF_NOMADS,
      .tier = 3,
      .stat0 = STAT_ACCURACY,                  .val0 =  -3,
      .stat1 = STAT_MAX_HP,                    .val1 =  15,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [38] "of the Elves": Defense +3
    { .nameId = TEXT_ID_SUFFIX_OF_THE_ELVES,
      .tier = 3,
      .stat0 = STAT_DEFENSE,                   .val0 =   3,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [39] "of the Plains": % Fear Resistance +10, Spirit Points +10
    { .nameId = TEXT_ID_SUFFIX_OF_THE_PLAINS,
      .tier = 3,
      .stat0 = STAT_FEAR_RESIST_PERCENT,       .val0 =  10,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [40] "of Southfarthing": % Fear Resistance +10, % Dodge +5
    { .nameId = TEXT_ID_SUFFIX_OF_SOUTHFARTHING,
      .tier = 3,
      .stat0 = STAT_FEAR_RESIST_PERCENT,       .val0 =  10,
      .stat1 = STAT_DODGE_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [41] "of the Sun": Accuracy +1, % Critical +10
    { .nameId = TEXT_ID_SUFFIX_OF_THE_SUN,
      .tier = 3,
      .stat0 = STAT_ACCURACY,                  .val0 =   1,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,       .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [42] "of the Moon": Accuracy +2, % Critical +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_MOON,
      .tier = 3,
      .stat0 = STAT_ACCURACY,                  .val0 =   2,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,       .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [43] "of the Brave": Health +1, Courage +2
    { .nameId = TEXT_ID_SUFFIX_OF_THE_BRAVE,
      .tier = 3,
      .stat0 = STAT_HEALTH,                    .val0 =   1,
      .stat1 = STAT_COURAGE,                   .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [44] "of the Riddermark": Strength +2, % Fear Resistance +20
    { .nameId = TEXT_ID_SUFFIX_OF_THE_RIDDERMARK,
      .tier = 3,
      .stat0 = STAT_STRENGTH,                  .val0 =   2,
      .stat1 = STAT_FEAR_RESIST_PERCENT,       .val1 =  20,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [45] "of Theoden": Hit Points +15, Damage +1
    { .nameId = TEXT_ID_SUFFIX_OF_THEODEN,
      .tier = 3,
      .stat0 = STAT_MAX_HP,                    .val0 =  15,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [46] "of Eodig": Courage +3, Hit Points Regen +3
    { .nameId = TEXT_ID_SUFFIX_OF_EODIG,
      .tier = 3,
      .stat0 = STAT_COURAGE,                   .val0 =   3,
      .stat1 = STAT_HP_REGEN,                  .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [47] "of the Harad": % Poison Resistance +10, % Speed +5, Accuracy +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_HARAD,
      .tier = 3,
      .stat0 = STAT_POISON_RESIST_PERCENT,     .val0 =  10,
      .stat1 = STAT_SPEED_PERCENT,             .val1 =   5,
      .stat2 = STAT_ACCURACY,                  .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [48] "of the Riders": Courage +3, % Critical +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_RIDERS,
      .tier = 4,
      .stat0 = STAT_COURAGE,                   .val0 =   3,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,       .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [49] "of Haldir": % Fear Resistance +25, Critical Armor +2
    { .nameId = TEXT_ID_SUFFIX_OF_HALDIR,
      .tier = 4,
      .stat0 = STAT_FEAR_RESIST_PERCENT,       .val0 =  25,
      .stat1 = STAT_CRITICAL_ARMOR,            .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [50] "of Gamling": Courage +3, % Poison Resistance +10
    { .nameId = TEXT_ID_SUFFIX_OF_GAMLING,
      .tier = 4,
      .stat0 = STAT_COURAGE,                   .val0 =   3,
      .stat1 = STAT_POISON_RESIST_PERCENT,     .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [51] "of Fangorn": Strength +1, Damage +3
    { .nameId = TEXT_ID_SUFFIX_OF_FANGORN,
      .tier = 4,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [52] "of the Sky": Defense +3, % Speed +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_SKY,
      .tier = 4,
      .stat0 = STAT_DEFENSE,                   .val0 =   3,
      .stat1 = STAT_SPEED_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [53] "of Eastemnet": Accuracy +1, Critical Armor +3
    { .nameId = TEXT_ID_SUFFIX_OF_EASTEMNET,
      .tier = 4,
      .stat0 = STAT_ACCURACY,                  .val0 =   1,
      .stat1 = STAT_CRITICAL_ARMOR,            .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [54] "of Shadows": Health -5, Missile Armor +6
    { .nameId = TEXT_ID_SUFFIX_OF_SHADOWS,
      .tier = 4,
      .stat0 = STAT_HEALTH,                    .val0 =  -5,
      .stat1 = STAT_MISSILE_ARMOR,             .val1 =   6,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [55] "of the Mines": % Fear Resistance +25, % Critical +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_MINES,
      .tier = 4,
      .stat0 = STAT_FEAR_RESIST_PERCENT,       .val0 =  25,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,       .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [56] "of the Halflings": Accuracy +2, Spirit Points +15
    { .nameId = TEXT_ID_SUFFIX_OF_THE_HALFLINGS,
      .tier = 4,
      .stat0 = STAT_ACCURACY,                  .val0 =   2,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  15,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [57] "of the Anarion House": Defense +3, Hit Points +6
    { .nameId = TEXT_ID_SUFFIX_OF_THE_ANARION_HOUSE,
      .tier = 4,
      .stat0 = STAT_DEFENSE,                   .val0 =   3,
      .stat1 = STAT_MAX_HP,                    .val1 =   6,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [58] "of the Citadel": Health +8, % Extra Treasure +15
    { .nameId = TEXT_ID_SUFFIX_OF_THE_CITADEL,
      .tier = 4,
      .stat0 = STAT_HEALTH,                    .val0 =   8,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  15,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [59] "of Osgiliath": Accuracy +3, % Speed +5
    { .nameId = TEXT_ID_SUFFIX_OF_OSGILIATH,
      .tier = 4,
      .stat0 = STAT_ACCURACY,                  .val0 =   3,
      .stat1 = STAT_SPEED_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [60] "of Anarion": Damage +3, Defense +1
    { .nameId = TEXT_ID_SUFFIX_OF_ANARION,
      .tier = 4,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   3,
      .stat1 = STAT_DEFENSE,                   .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [61] "of Grey Havens": Health +3, Accuracy +1
    { .nameId = TEXT_ID_SUFFIX_OF_GREY_HAVENS,
      .tier = 4,
      .stat0 = STAT_HEALTH,                    .val0 =   3,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [62] "of Nurn": Strength +4, % Fear Resistance +10
    { .nameId = TEXT_ID_SUFFIX_OF_NURN,
      .tier = 4,
      .stat0 = STAT_STRENGTH,                  .val0 =   4,
      .stat1 = STAT_FEAR_RESIST_PERCENT,       .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [63] "of Gondor": % Critical +5, Damage +1
    { .nameId = TEXT_ID_SUFFIX_OF_GONDOR,
      .tier = 4,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =   5,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [64] "of the North": Accuracy +3, Spirit Points +20
    { .nameId = TEXT_ID_SUFFIX_OF_THE_NORTH,
      .tier = 5,
      .stat0 = STAT_ACCURACY,                  .val0 =   3,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  20,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [65] "of the High Elves": Courage +2, Melee Armor +2
    { .nameId = TEXT_ID_SUFFIX_OF_THE_HIGH_ELVES,
      .tier = 5,
      .stat0 = STAT_COURAGE,                   .val0 =   2,
      .stat1 = STAT_MELEE_ARMOR,               .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [66] "of the Trollkin": Strength +3, Hit Points Regen +3
    { .nameId = TEXT_ID_SUFFIX_OF_THE_TROLLKIN,
      .tier = 5,
      .stat0 = STAT_STRENGTH,                  .val0 =   3,
      .stat1 = STAT_HP_REGEN,                  .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [67] "of the Crow": Accuracy +1, % Dodge +10
    { .nameId = TEXT_ID_SUFFIX_OF_THE_CROW,
      .tier = 5,
      .stat0 = STAT_ACCURACY,                  .val0 =   1,
      .stat1 = STAT_DODGE_PERCENT,             .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [68] "of Rivendell": Courage +3, % Critical +5
    { .nameId = TEXT_ID_SUFFIX_OF_RIVENDELL,
      .tier = 5,
      .stat0 = STAT_COURAGE,                   .val0 =   3,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,       .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [69] "of Dunland": Accuracy +3, Hit Points +5
    { .nameId = TEXT_ID_SUFFIX_OF_DUNLAND,
      .tier = 5,
      .stat0 = STAT_ACCURACY,                  .val0 =   3,
      .stat1 = STAT_MAX_HP,                    .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [70] "of Trolls": Health +10, Hit Points +20
    { .nameId = TEXT_ID_SUFFIX_OF_TROLLS,
      .tier = 5,
      .stat0 = STAT_HEALTH,                    .val0 =  10,
      .stat1 = STAT_MAX_HP,                    .val1 =  20,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [71] "of the Glen": Accuracy +1, Spirit Regen +4
    { .nameId = TEXT_ID_SUFFIX_OF_THE_GLEN,
      .tier = 5,
      .stat0 = STAT_ACCURACY,                  .val0 =   1,
      .stat1 = STAT_SPIRIT_REGEN,              .val1 =   4,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [72] "of the Woods": Defense +4, % Poison Resistance +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_WOODS,
      .tier = 5,
      .stat0 = STAT_DEFENSE,                   .val0 =   4,
      .stat1 = STAT_POISON_RESIST_PERCENT,     .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [73] "of the Great": Health +3, dmg to Orcs +2
    { .nameId = TEXT_ID_SUFFIX_OF_THE_GREAT,
      .tier = 5,
      .stat0 = STAT_HEALTH,                    .val0 =   3,
      .stat1 = STAT_DAMAGE_TO_ORCS,            .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [74] "of the Caverns": Defense +2, % Poison Resistance +20
    { .nameId = TEXT_ID_SUFFIX_OF_THE_CAVERNS,
      .tier = 5,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_POISON_RESIST_PERCENT,     .val1 =  20,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [75] "of Isengard": Strength +5, Wisdom +3, Cursed +1
    { .nameId = TEXT_ID_SUFFIX_OF_ISENGARD,
      .tier = 5,
      .stat0 = STAT_STRENGTH,                  .val0 =   5,
      .stat1 = STAT_WISDOM,                    .val1 =   3,
      .stat2 = STAT_CURSED,                    .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [76] "of Gorgoroth": Health +4, % Poison Resistance +10
    { .nameId = TEXT_ID_SUFFIX_OF_GORGOROTH,
      .tier = 5,
      .stat0 = STAT_HEALTH,                    .val0 =   4,
      .stat1 = STAT_POISON_RESIST_PERCENT,     .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [77] "of Lothlorien": Spirit Points +25, Wisdom +1
    { .nameId = TEXT_ID_SUFFIX_OF_LOTHLORIEN,
      .tier = 5,
      .stat0 = STAT_MAX_SPIRIT,                .val0 =  25,
      .stat1 = STAT_WISDOM,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [78] "of the Third Age": Courage +4, Hit Points +10
    { .nameId = TEXT_ID_SUFFIX_OF_THE_THIRD_AGE,
      .tier = 5,
      .stat0 = STAT_COURAGE,                   .val0 =   4,
      .stat1 = STAT_MAX_HP,                    .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [79] "of Theodred": Courage +6, Hit Points -20, Damage +1
    { .nameId = TEXT_ID_SUFFIX_OF_THEODRED,
      .tier = 5,
      .stat0 = STAT_COURAGE,                   .val0 =   6,
      .stat1 = STAT_MAX_HP,                    .val1 = -20,
      .stat2 = STAT_DAMAGE_FLAT,               .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [80] "of Wood Elves": % Critical +10, Spirit Points +10
    { .nameId = TEXT_ID_SUFFIX_OF_WOOD_ELVES,
      .tier = 6,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =  10,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [81] "of the Wolf": Strength +3, Spirit Points +20
    { .nameId = TEXT_ID_SUFFIX_OF_THE_WOLF,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =   3,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  20,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [82] "of the Bear": Strength +10, Defense -6
    { .nameId = TEXT_ID_SUFFIX_OF_THE_BEAR,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =  10,
      .stat1 = STAT_DEFENSE,                   .val1 =  -6,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [83] "of Chaos": Strength -2, Wisdom +3, Accuracy +4, Defense -13
    { .nameId = TEXT_ID_SUFFIX_OF_CHAOS,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =  -2,
      .stat1 = STAT_WISDOM,                    .val1 =   3,
      .stat2 = STAT_ACCURACY,                  .val2 =   4,
      .stat3 = STAT_DEFENSE,                   .val3 = -13 },

    // [84] "of the Eyes": Accuracy +5, Gemfinder +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_EYES,
      .tier = 6,
      .stat0 = STAT_ACCURACY,                  .val0 =   5,
      .stat1 = STAT_GEMFINDER,                 .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [85] "of the Sea": % Poison Resistance +90
    { .nameId = TEXT_ID_SUFFIX_OF_THE_SEA,
      .tier = 6,
      .stat0 = STAT_POISON_RESIST_PERCENT,     .val0 =  90,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [86] "of Truth": Courage +3, % Extra Treasure +20
    { .nameId = TEXT_ID_SUFFIX_OF_TRUTH,
      .tier = 6,
      .stat0 = STAT_COURAGE,                   .val0 =   3,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  20,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [87] "of the River": Hit Points Regen +3, Health +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_RIVER,
      .tier = 6,
      .stat0 = STAT_HP_REGEN,                  .val0 =   3,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [88] "of the Eorlings": Health +4, Strength +1, % Speed +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_EORLINGS,
      .tier = 6,
      .stat0 = STAT_HEALTH,                    .val0 =   4,
      .stat1 = STAT_STRENGTH,                  .val1 =   1,
      .stat2 = STAT_SPEED_PERCENT,             .val2 =   5,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [89] "of the Virtuous": % Fear Resistance +50, % Poison Resistance +50
    { .nameId = TEXT_ID_SUFFIX_OF_THE_VIRTUOUS,
      .tier = 6,
      .stat0 = STAT_FEAR_RESIST_PERCENT,       .val0 =  50,
      .stat1 = STAT_POISON_RESIST_PERCENT,     .val1 =  50,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [90] "of the Mighty": Strength +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_MIGHTY,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =   5,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [91] "of Luck": % Critical +15, % Extra Treasure +30, Defense +1
    { .nameId = TEXT_ID_SUFFIX_OF_LUCK,
      .tier = 6,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =  15,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  30,
      .stat2 = STAT_DEFENSE,                   .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [92] "of Sammath Naur": Defense +3, Damage +3
    { .nameId = TEXT_ID_SUFFIX_OF_SAMMATH_NAUR,
      .tier = 6,
      .stat0 = STAT_DEFENSE,                   .val0 =   3,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [93] "of the Anduin": % Extra Treasure +5, % Fear Resistance +25
    { .nameId = TEXT_ID_SUFFIX_OF_THE_ANDUIN,
      .tier = 6,
      .stat0 = STAT_EXTRA_TREASURE_PERCENT,    .val0 =   5,
      .stat1 = STAT_FEAR_RESIST_PERCENT,       .val1 =  25,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [94] "of the Second Age": Health +3, Courage +2
    { .nameId = TEXT_ID_SUFFIX_OF_THE_SECOND_AGE,
      .tier = 6,
      .stat0 = STAT_HEALTH,                    .val0 =   3,
      .stat1 = STAT_COURAGE,                   .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [95] "of Amon Sul": Spirit Points +45, % Fear Resistance +25, Gemfinder +1, Valuable +1
    { .nameId = TEXT_ID_SUFFIX_OF_AMON_SUL,
      .tier = 6,
      .stat0 = STAT_MAX_SPIRIT,                .val0 =  45,
      .stat1 = STAT_FEAR_RESIST_PERCENT,       .val1 =  25,
      .stat2 = STAT_GEMFINDER,                 .val2 =   1,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [96] "of Damrod": Accuracy +4
    { .nameId = TEXT_ID_SUFFIX_OF_DAMROD,
      .tier = 7,
      .stat0 = STAT_ACCURACY,                  .val0 =   4,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [97] "of the Queen": Courage +3, % Extra Treasure +30
    { .nameId = TEXT_ID_SUFFIX_OF_THE_QUEEN,
      .tier = 7,
      .stat0 = STAT_COURAGE,                   .val0 =   3,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  30,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [98] "of the Owl": Missile Armor +3, % Extra Treasure +20
    { .nameId = TEXT_ID_SUFFIX_OF_THE_OWL,
      .tier = 7,
      .stat0 = STAT_MISSILE_ARMOR,             .val0 =   3,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  20,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [99] "of the East": % Poison Resistance +35, Accuracy +3
    { .nameId = TEXT_ID_SUFFIX_OF_THE_EAST,
      .tier = 7,
      .stat0 = STAT_POISON_RESIST_PERCENT,     .val0 =  35,
      .stat1 = STAT_ACCURACY,                  .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [100] "of the Dwarf King": Health +5, Defense +1, Melee Armor +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_DWARF_KING,
      .tier = 7,
      .stat0 = STAT_HEALTH,                    .val0 =   5,
      .stat1 = STAT_DEFENSE,                   .val1 =   1,
      .stat2 = STAT_MELEE_ARMOR,               .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [101] "of the Stars": Defense +3, Spirit Points +30, Wisdom +2
    { .nameId = TEXT_ID_SUFFIX_OF_THE_STARS,
      .tier = 7,
      .stat0 = STAT_DEFENSE,                   .val0 =   3,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  30,
      .stat2 = STAT_WISDOM,                    .val2 =   2,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [102] "of the White Mountains": Hit Points +25, Spirit Points +10
    { .nameId = TEXT_ID_SUFFIX_OF_THE_WHITE_MOUNTAINS,
      .tier = 7,
      .stat0 = STAT_MAX_HP,                    .val0 =  25,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [103] "of Hope": Accuracy +5, % Extra Treasure +25
    { .nameId = TEXT_ID_SUFFIX_OF_HOPE,
      .tier = 7,
      .stat0 = STAT_ACCURACY,                  .val0 =   5,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  25,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [104] "of the Eldar": Damage +4, % Speed +7
    { .nameId = TEXT_ID_SUFFIX_OF_THE_ELDAR,
      .tier = 7,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   4,
      .stat1 = STAT_SPEED_PERCENT,             .val1 =   7,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [105] "of the Beyond": Defense +2, % Dodge +5, Wisdom +3
    { .nameId = TEXT_ID_SUFFIX_OF_THE_BEYOND,
      .tier = 7,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_DODGE_PERCENT,             .val1 =   5,
      .stat2 = STAT_WISDOM,                    .val2 =   3,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [106] "of the Evenstar": % Critical +15, % Dodge +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_EVENSTAR,
      .tier = 7,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =  15,
      .stat1 = STAT_DODGE_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [107] "of Galadriel": Courage +7
    { .nameId = TEXT_ID_SUFFIX_OF_GALADRIEL,
      .tier = 7,
      .stat0 = STAT_COURAGE,                   .val0 =   7,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [108] "of Mt. Mindolluin": Defense +3, Hit Points +15
    { .nameId = TEXT_ID_SUFFIX_OF_MT_MINDOLLUIN,
      .tier = 7,
      .stat0 = STAT_DEFENSE,                   .val0 =   3,
      .stat1 = STAT_MAX_HP,                    .val1 =  15,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [109] "of Minas Tirith": Strength +4, Defense +3
    { .nameId = TEXT_ID_SUFFIX_OF_MINAS_TIRITH,
      .tier = 7,
      .stat0 = STAT_STRENGTH,                  .val0 =   4,
      .stat1 = STAT_DEFENSE,                   .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [110] "of the First Age": Courage +3, % Speed +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_FIRST_AGE,
      .tier = 7,
      .stat0 = STAT_COURAGE,                   .val0 =   3,
      .stat1 = STAT_SPEED_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [111] "of Numenor": All Primary Stats +2, Hit Points +10
    { .nameId = TEXT_ID_SUFFIX_OF_NUMENOR,
      .tier = 7,
      .stat0 = STAT_ALL_PRIMARY_STATS,         .val0 =   2,
      .stat1 = STAT_MAX_HP,                    .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [112] "of Legend": Health +1, Critical Armor +9
    { .nameId = TEXT_ID_SUFFIX_OF_LEGEND,
      .tier = 8,
      .stat0 = STAT_HEALTH,                    .val0 =   1,
      .stat1 = STAT_CRITICAL_ARMOR,            .val1 =   9,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [113] "of Beregond": Strength +3, % Poison Resistance +30
    { .nameId = TEXT_ID_SUFFIX_OF_BEREGOND,
      .tier = 8,
      .stat0 = STAT_STRENGTH,                  .val0 =   3,
      .stat1 = STAT_POISON_RESIST_PERCENT,     .val1 =  30,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [114] "of the West": Defense +4, Damage +2
    { .nameId = TEXT_ID_SUFFIX_OF_THE_WEST,
      .tier = 8,
      .stat0 = STAT_DEFENSE,                   .val0 =   4,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [115] "of Arnor": Health +4, % Extra Treasure +20
    { .nameId = TEXT_ID_SUFFIX_OF_ARNOR,
      .tier = 8,
      .stat0 = STAT_HEALTH,                    .val0 =   4,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  20,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [116] "of Ithilien": Accuracy +4, % Dodge +5
    { .nameId = TEXT_ID_SUFFIX_OF_ITHILIEN,
      .tier = 8,
      .stat0 = STAT_ACCURACY,                  .val0 =   4,
      .stat1 = STAT_DODGE_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [117] "of Amon Hen": % Speed -5, Strength +7
    { .nameId = TEXT_ID_SUFFIX_OF_AMON_HEN,
      .tier = 8,
      .stat0 = STAT_SPEED_PERCENT,             .val0 =  -5,
      .stat1 = STAT_STRENGTH,                  .val1 =   7,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [118] "of the Archer King": Accuracy +12, Missile Armor +5, Strength -5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_ARCHER_KING,
      .tier = 8,
      .stat0 = STAT_ACCURACY,                  .val0 =  12,
      .stat1 = STAT_MISSILE_ARMOR,             .val1 =   5,
      .stat2 = STAT_STRENGTH,                  .val2 =  -5,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [119] "of Dunharrow": % Poison Resistance +20, Melee Armor +3
    { .nameId = TEXT_ID_SUFFIX_OF_DUNHARROW,
      .tier = 8,
      .stat0 = STAT_POISON_RESIST_PERCENT,     .val0 =  20,
      .stat1 = STAT_MELEE_ARMOR,               .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [120] "of Umbar": Spirit Points +15, % Speed +10
    { .nameId = TEXT_ID_SUFFIX_OF_UMBAR,
      .tier = 8,
      .stat0 = STAT_MAX_SPIRIT,                .val0 =  15,
      .stat1 = STAT_SPEED_PERCENT,             .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [121] "of the Wizards": % Fear Resistance +30, Wisdom +3
    { .nameId = TEXT_ID_SUFFIX_OF_THE_WIZARDS,
      .tier = 8,
      .stat0 = STAT_FEAR_RESIST_PERCENT,       .val0 =  30,
      .stat1 = STAT_WISDOM,                    .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [122] "of Darkness": % Critical +10, % Dodge +5
    { .nameId = TEXT_ID_SUFFIX_OF_DARKNESS,
      .tier = 8,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =  10,
      .stat1 = STAT_DODGE_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [123] "of the Undead": % Poison Resistance +95, Spirit Points -25, Health +15
    { .nameId = TEXT_ID_SUFFIX_OF_THE_UNDEAD,
      .tier = 8,
      .stat0 = STAT_POISON_RESIST_PERCENT,     .val0 =  95,
      .stat1 = STAT_MAX_SPIRIT,                .val1 = -25,
      .stat2 = STAT_HEALTH,                    .val2 =  15,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [124] "of the Immortal": Strength +5, Damage +2, Hit Points Regen +5, Spirit Regen +2
    { .nameId = TEXT_ID_SUFFIX_OF_THE_IMMORTAL,
      .tier = 8,
      .stat0 = STAT_STRENGTH,                  .val0 =   5,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   2,
      .stat2 = STAT_HP_REGEN,                  .val2 =   5,
      .stat3 = STAT_SPIRIT_REGEN,              .val3 =   2 },

    // [125] "of Minas Morgul": % Extra Treasure +30, Health +10
    { .nameId = TEXT_ID_SUFFIX_OF_MINAS_MORGUL,
      .tier = 8,
      .stat0 = STAT_EXTRA_TREASURE_PERCENT,    .val0 =  30,
      .stat1 = STAT_HEALTH,                    .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [126] "of the Witch King": Damage +10, Cursed +1, % Corruption Resistance -60
    { .nameId = TEXT_ID_SUFFIX_OF_THE_WITCH_KING,
      .tier = 8,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =  10,
      .stat1 = STAT_CURSED,                    .val1 =   1,
      .stat2 = STAT_CORRUPTION_RESIST_PERCENT, .val2 = -60,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [127] "of Cirith Ungol": Defense +5, Spirit Points +20, % Fear Resistance +10, Cursed +1
    { .nameId = TEXT_ID_SUFFIX_OF_CIRITH_UNGOL,
      .tier = 8,
      .stat0 = STAT_DEFENSE,                   .val0 =   5,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  20,
      .stat2 = STAT_FEAR_RESIST_PERCENT,       .val2 =  10,
      .stat3 = STAT_CURSED,                    .val3 =   1 },

    // [128] "of Anorien": % Poison Resistance +25, Damage +4, % Fear Resistance +10
    { .nameId = TEXT_ID_SUFFIX_OF_ANORIEN,
      .tier = 9,
      .stat0 = STAT_POISON_RESIST_PERCENT,     .val0 =  25,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   4,
      .stat2 = STAT_FEAR_RESIST_PERCENT,       .val2 =  10,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [129] "of the Eagles": Courage +5, % Critical +10
    { .nameId = TEXT_ID_SUFFIX_OF_THE_EAGLES,
      .tier = 9,
      .stat0 = STAT_COURAGE,                   .val0 =   5,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,       .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [130] "of Ash": Damage +1, Melee Armor +3
    { .nameId = TEXT_ID_SUFFIX_OF_ASH,
      .tier = 9,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   1,
      .stat1 = STAT_MELEE_ARMOR,               .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [131] "of the Dead": % Fear Resistance +75, Courage +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_DEAD,
      .tier = 9,
      .stat0 = STAT_FEAR_RESIST_PERCENT,       .val0 =  75,
      .stat1 = STAT_COURAGE,                   .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [132] "of Law": % Critical +25, Health +1
    { .nameId = TEXT_ID_SUFFIX_OF_LAW,
      .tier = 9,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =  25,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [133] "of Fathers": Strength +5, Hit Points +20
    { .nameId = TEXT_ID_SUFFIX_OF_FATHERS,
      .tier = 9,
      .stat0 = STAT_STRENGTH,                  .val0 =   5,
      .stat1 = STAT_MAX_HP,                    .val1 =  20,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [134] "of the Corsairs": Accuracy +2, Damage +3
    { .nameId = TEXT_ID_SUFFIX_OF_THE_CORSAIRS,
      .tier = 9,
      .stat0 = STAT_ACCURACY,                  .val0 =   2,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [135] "of the Black Sails": Defense +3, % Critical +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_BLACK_SAILS,
      .tier = 9,
      .stat0 = STAT_DEFENSE,                   .val0 =   3,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,       .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [136] "of Dimholt": Health +1, % Speed +5
    { .nameId = TEXT_ID_SUFFIX_OF_DIMHOLT,
      .tier = 9,
      .stat0 = STAT_HEALTH,                    .val0 =   1,
      .stat1 = STAT_SPEED_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [137] "of Underharrow": % Poison Resistance +75, % Dodge +5, % Critical +10
    { .nameId = TEXT_ID_SUFFIX_OF_UNDERHARROW,
      .tier = 9,
      .stat0 = STAT_POISON_RESIST_PERCENT,     .val0 =  75,
      .stat1 = STAT_DODGE_PERCENT,             .val1 =   5,
      .stat2 = STAT_CRIT_CHANCE_PERCENT,       .val2 =  10,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [138] "of Weathertop": dmg to Nazgul +10, Defense +1
    { .nameId = TEXT_ID_SUFFIX_OF_WEATHERTOP,
      .tier = 9,
      .stat0 = STAT_DAMAGE_TO_NAZGUL,          .val0 =  10,
      .stat1 = STAT_DEFENSE,                   .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [139] "of Henneth Annun": Accuracy +5, Wisdom +3
    { .nameId = TEXT_ID_SUFFIX_OF_HENNETH_ANNUN,
      .tier = 9,
      .stat0 = STAT_ACCURACY,                  .val0 =   5,
      .stat1 = STAT_WISDOM,                    .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [140] "of the Undying Lands": Critical Armor +6, % Dodge +10
    { .nameId = TEXT_ID_SUFFIX_OF_THE_UNDYING_LANDS,
      .tier = 9,
      .stat0 = STAT_CRITICAL_ARMOR,            .val0 =   6,
      .stat1 = STAT_DODGE_PERCENT,             .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [141] "of the Kings of Men": Accuracy +5, Spirit Points +20, % Fear Resistance +20, Courage +3
    { .nameId = TEXT_ID_SUFFIX_OF_THE_KINGS_OF_MEN,
      .tier = 9,
      .stat0 = STAT_ACCURACY,                  .val0 =   5,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  20,
      .stat2 = STAT_FEAR_RESIST_PERCENT,       .val2 =  20,
      .stat3 = STAT_COURAGE,                   .val3 =   3 },

    // [142] "of the Valar": Health +10, % Poison Resistance +5, Courage -3
    { .nameId = TEXT_ID_SUFFIX_OF_THE_VALAR,
      .tier = 9,
      .stat0 = STAT_HEALTH,                    .val0 =  10,
      .stat1 = STAT_POISON_RESIST_PERCENT,     .val1 =   5,
      .stat2 = STAT_COURAGE,                   .val2 =  -3,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [143] "of Ecthelion Tower": Missile Armor +3, Melee Armor +3, Missile Armor +3
    { .nameId = TEXT_ID_SUFFIX_OF_ECTHELION_TOWER,
      .tier = 9,
      .stat0 = STAT_MISSILE_ARMOR,             .val0 =   3,
      .stat1 = STAT_MELEE_ARMOR,               .val1 =   3,
      .stat2 = STAT_MISSILE_ARMOR,             .val2 =   3,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [144] "of the Ghost King": Accuracy +5, Damage +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_GHOST_KING,
      .tier = 10,
      .stat0 = STAT_ACCURACY,                  .val0 =   5,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [145] "of the Stag": Strength +5, Spirit Points +10, Health +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_STAG,
      .tier = 10,
      .stat0 = STAT_STRENGTH,                  .val0 =   5,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  10,
      .stat2 = STAT_HEALTH,                    .val2 =   5,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [146] "of Ages": Accuracy +5, Melee Armor +5
    { .nameId = TEXT_ID_SUFFIX_OF_AGES,
      .tier = 10,
      .stat0 = STAT_ACCURACY,                  .val0 =   5,
      .stat1 = STAT_MELEE_ARMOR,               .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [147] "of Doom": % Fear Resistance +50, Strength +5, Hit Points Regen +4, Cursed +1
    { .nameId = TEXT_ID_SUFFIX_OF_DOOM,
      .tier = 10,
      .stat0 = STAT_FEAR_RESIST_PERCENT,       .val0 =  50,
      .stat1 = STAT_STRENGTH,                  .val1 =   5,
      .stat2 = STAT_HP_REGEN,                  .val2 =   4,
      .stat3 = STAT_CURSED,                    .val3 =   1 },

    // [148] "of Udun": Defense +7, % Dodge +5
    { .nameId = TEXT_ID_SUFFIX_OF_UDUN,
      .tier = 10,
      .stat0 = STAT_DEFENSE,                   .val0 =   7,
      .stat1 = STAT_DODGE_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [149] "of Elfsong": Courage +10, Missile Armor +1, Wisdom +1
    { .nameId = TEXT_ID_SUFFIX_OF_ELFSONG,
      .tier = 10,
      .stat0 = STAT_COURAGE,                   .val0 =  10,
      .stat1 = STAT_MISSILE_ARMOR,             .val1 =   1,
      .stat2 = STAT_WISDOM,                    .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [150] "of Pelennor": dmg to Orcs +5, Courage +1, Health +1
    { .nameId = TEXT_ID_SUFFIX_OF_PELENNOR,
      .tier = 10,
      .stat0 = STAT_DAMAGE_TO_ORCS,            .val0 =   5,
      .stat1 = STAT_COURAGE,                   .val1 =   1,
      .stat2 = STAT_HEALTH,                    .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [151] "of the Abyss": % Fear Resistance +40, Melee Armor +5, Cursed +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_ABYSS,
      .tier = 10,
      .stat0 = STAT_FEAR_RESIST_PERCENT,       .val0 =  40,
      .stat1 = STAT_MELEE_ARMOR,               .val1 =   5,
      .stat2 = STAT_CURSED,                    .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [152] "of the Wraiths": Accuracy +3, % Critical +20, Cursed +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_WRAITHS,
      .tier = 10,
      .stat0 = STAT_ACCURACY,                  .val0 =   3,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,       .val1 =  20,
      .stat2 = STAT_CURSED,                    .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [153] "of the Towers": Defense +9, Wisdom +1
    { .nameId = TEXT_ID_SUFFIX_OF_THE_TOWERS,
      .tier = 10,
      .stat0 = STAT_DEFENSE,                   .val0 =   9,
      .stat1 = STAT_WISDOM,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [154] "of the Kings": Health +5, Hit Points +20, Courage +3
    { .nameId = TEXT_ID_SUFFIX_OF_THE_KINGS,
      .tier = 10,
      .stat0 = STAT_HEALTH,                    .val0 =   5,
      .stat1 = STAT_MAX_HP,                    .val1 =  20,
      .stat2 = STAT_COURAGE,                   .val2 =   3,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [155] "of the Fates": % Critical +20, % Extra Treasure +60
    { .nameId = TEXT_ID_SUFFIX_OF_THE_FATES,
      .tier = 10,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =  20,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  60,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [156] "of the Argonath": Hit Points +50, % Critical +5, % Speed +5
    { .nameId = TEXT_ID_SUFFIX_OF_THE_ARGONATH,
      .tier = 10,
      .stat0 = STAT_MAX_HP,                    .val0 =  50,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,       .val1 =   5,
      .stat2 = STAT_SPEED_PERCENT,             .val2 =   5,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [157] "of Mt. Doom": All Primary Stats +4, Cursed +1, Strength +3
    { .nameId = TEXT_ID_SUFFIX_OF_MT_DOOM,
      .tier = 10,
      .stat0 = STAT_ALL_PRIMARY_STATS,         .val0 =   4,
      .stat1 = STAT_CURSED,                    .val1 =   1,
      .stat2 = STAT_STRENGTH,                  .val2 =   3,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [158] "of the Dark Lord": All Primary Stats +10, Cursed +1, Damage +5, % Corruption Resistance -90
    { .nameId = TEXT_ID_SUFFIX_OF_THE_DARK_LORD,
      .tier = 10,
      .stat0 = STAT_ALL_PRIMARY_STATS,         .val0 =  10,
      .stat1 = STAT_CURSED,                    .val1 =   1,
      .stat2 = STAT_DAMAGE_FLAT,               .val2 =   5,
      .stat3 = STAT_CORRUPTION_RESIST_PERCENT, .val3 = -90 },

    // [159] "of Mordor": All Primary Stats +5, Cursed +1, Health +1, % Corruption Resistance -90
    { .nameId = TEXT_ID_SUFFIX_OF_MORDOR,
      .tier = 10,
      .stat0 = STAT_ALL_PRIMARY_STATS,         .val0 =   5,
      .stat1 = STAT_CURSED,                    .val1 =   1,
      .stat2 = STAT_HEALTH,                    .val2 =   1,
      .stat3 = STAT_CORRUPTION_RESIST_PERCENT, .val3 = -90 },
};

/** no decoded bits set (154 of 160 entries) */
#define AFFIX_FLAGS_NONE { 0 }

/**
 * Per-affix flag table; indexed by the affix index. Decoded bits: AFFIX_FLAG_* (item.h).
 *
 * @romaddress 0x08068cd0
 */
const AffixFlags ItemSuffixFlags[160] = {
    // [0] "of the Fox"
    { .d = AFFIX_FLAGS_NONE },

    // [1] "of the Orc King"
    { .d = AFFIX_FLAGS_NONE },

    // [2] "of Goblin Men"
    { .d = AFFIX_FLAGS_NONE },

    // [3] "of the South"
    { .d = AFFIX_FLAGS_NONE },

    // [4] "of the Hound"
    { .d = AFFIX_FLAGS_NONE },

    // [5] "of Bree"
    { .d = AFFIX_FLAGS_NONE },

    // [6] "of Men"
    { .d = AFFIX_FLAGS_NONE },

    // [7] "of Light"
    { .d = AFFIX_FLAGS_NONE },

    // [8] "of the Wise"
    { .d = AFFIX_FLAGS_NONE },

    // [9] "of the Old Forest"
    { .d = AFFIX_FLAGS_NONE },

    // [10] "of the Strong"
    { .d = AFFIX_FLAGS_NONE },

    // [11] "of Entwash"
    { .d = AFFIX_FLAGS_NONE },

    // [12] "of Hobbiton"
    { .d = AFFIX_FLAGS_NONE },

    // [13] "of Middle Earth"
    { .d = AFFIX_FLAGS_NONE },

    // [14] "of Faramir"
    { .d = AFFIX_FLAGS_NONE },

    // [15] "of the Deep"
    { .d = AFFIX_FLAGS_NONE },

    // [16] "of Midgewater"
    { .d = AFFIX_FLAGS_NONE },

    // [17] "of Helm's Deep"
    { .d = AFFIX_FLAGS_NONE },

    // [18] "of Deeping"
    { .d = AFFIX_FLAGS_NONE },

    // [19] "of Rangers"
    { .d = AFFIX_FLAGS_NONE },

    // [20] "of Sorrow"
    { .d = AFFIX_FLAGS_NONE },

    // [21] "of the White Hand"
    { .d = AFFIX_FLAGS_NONE },

    // [22] "of the Orcs"
    { .d = AFFIX_FLAGS_NONE },

    // [23] "of Lies"
    { .d = AFFIX_FLAGS_NONE },

    // [24] "of Barad-Dur"
    { .d = AFFIX_FLAGS_NONE },

    // [25] "of the Hill Folk"
    { .d = AFFIX_FLAGS_NONE },

    // [26] "of the Dwarves"
    { .d = AFFIX_FLAGS_NONE },

    // [27] "of the Valley"
    { .d = AFFIX_FLAGS_NONE },

    // [28] "of Morgul"
    { .d = AFFIX_FLAGS_NONE },

    // [29] "of Wormtongue"
    { .d = AFFIX_FLAGS_NONE },

    // [30] "of Saruman"
    { .d = AFFIX_FLAGS_NONE },

    // [31] "of Mithrandir"
    { .d = AFFIX_FLAGS_NONE },

    // [32] "of Merry"
    { .d = AFFIX_FLAGS_NONE },

    // [33] "of Edoras"
    { .d = AFFIX_FLAGS_NONE },

    // [34] "of the Mountains"
    { .d = AFFIX_FLAGS_NONE },

    // [35] "of Woodsmen"
    { .d = AFFIX_FLAGS_NONE },

    // [36] "of Rohan"
    { .d = AFFIX_FLAGS_NONE },

    // [37] "of Nomads"
    { .d = AFFIX_FLAGS_NONE },

    // [38] "of the Elves"
    { .d = AFFIX_FLAGS_NONE },

    // [39] "of the Plains"
    { .d = AFFIX_FLAGS_NONE },

    // [40] "of Southfarthing"
    { .d = AFFIX_FLAGS_NONE },

    // [41] "of the Sun"
    { .d = AFFIX_FLAGS_NONE },

    // [42] "of the Moon"
    { .d = AFFIX_FLAGS_NONE },

    // [43] "of the Brave"
    { .d = AFFIX_FLAGS_NONE },

    // [44] "of the Riddermark"
    { .d = AFFIX_FLAGS_NONE },

    // [45] "of Theoden"
    { .d = AFFIX_FLAGS_NONE },

    // [46] "of Eodig"
    { .d = { .minLevelPlus5A = 1 } },

    // [47] "of the Harad"
    { .d = AFFIX_FLAGS_NONE },

    // [48] "of the Riders"
    { .d = AFFIX_FLAGS_NONE },

    // [49] "of Haldir"
    { .d = AFFIX_FLAGS_NONE },

    // [50] "of Gamling"
    { .d = AFFIX_FLAGS_NONE },

    // [51] "of Fangorn"
    { .d = AFFIX_FLAGS_NONE },

    // [52] "of the Sky"
    { .d = AFFIX_FLAGS_NONE },

    // [53] "of Eastemnet"
    { .d = AFFIX_FLAGS_NONE },

    // [54] "of Shadows"
    { .d = AFFIX_FLAGS_NONE },

    // [55] "of the Mines"
    { .d = AFFIX_FLAGS_NONE },

    // [56] "of the Halflings"
    { .d = AFFIX_FLAGS_NONE },

    // [57] "of the Anarion House"
    { .d = AFFIX_FLAGS_NONE },

    // [58] "of the Citadel"
    { .d = AFFIX_FLAGS_NONE },

    // [59] "of Osgiliath"
    { .d = AFFIX_FLAGS_NONE },

    // [60] "of Anarion"
    { .d = AFFIX_FLAGS_NONE },

    // [61] "of Grey Havens"
    { .d = AFFIX_FLAGS_NONE },

    // [62] "of Nurn"
    { .d = AFFIX_FLAGS_NONE },

    // [63] "of Gondor"
    { .d = AFFIX_FLAGS_NONE },

    // [64] "of the North"
    { .d = AFFIX_FLAGS_NONE },

    // [65] "of the High Elves"
    { .d = AFFIX_FLAGS_NONE },

    // [66] "of the Trollkin"
    { .d = { .minLevelPlus5A = 1 } },

    // [67] "of the Crow"
    { .d = AFFIX_FLAGS_NONE },

    // [68] "of Rivendell"
    { .d = AFFIX_FLAGS_NONE },

    // [69] "of Dunland"
    { .d = AFFIX_FLAGS_NONE },

    // [70] "of Trolls"
    { .d = AFFIX_FLAGS_NONE },

    // [71] "of the Glen"
    { .d = { .minLevelPlus5B = 1 } },

    // [72] "of the Woods"
    { .d = AFFIX_FLAGS_NONE },

    // [73] "of the Great"
    { .d = AFFIX_FLAGS_NONE },

    // [74] "of the Caverns"
    { .d = AFFIX_FLAGS_NONE },

    // [75] "of Isengard"
    { .d = AFFIX_FLAGS_NONE },

    // [76] "of Gorgoroth"
    { .d = AFFIX_FLAGS_NONE },

    // [77] "of Lothlorien"
    { .d = AFFIX_FLAGS_NONE },

    // [78] "of the Third Age"
    { .d = AFFIX_FLAGS_NONE },

    // [79] "of Theodred"
    { .d = AFFIX_FLAGS_NONE },

    // [80] "of Wood Elves"
    { .d = AFFIX_FLAGS_NONE },

    // [81] "of the Wolf"
    { .d = AFFIX_FLAGS_NONE },

    // [82] "of the Bear"
    { .d = AFFIX_FLAGS_NONE },

    // [83] "of Chaos"
    { .d = AFFIX_FLAGS_NONE },

    // [84] "of the Eyes"
    { .d = AFFIX_FLAGS_NONE },

    // [85] "of the Sea"
    { .d = AFFIX_FLAGS_NONE },

    // [86] "of Truth"
    { .d = AFFIX_FLAGS_NONE },

    // [87] "of the River"
    { .d = { .minLevelPlus5A = 1 } },

    // [88] "of the Eorlings"
    { .d = AFFIX_FLAGS_NONE },

    // [89] "of the Virtuous"
    { .d = AFFIX_FLAGS_NONE },

    // [90] "of the Mighty"
    { .d = AFFIX_FLAGS_NONE },

    // [91] "of Luck"
    { .d = AFFIX_FLAGS_NONE },

    // [92] "of Sammath Naur"
    { .d = AFFIX_FLAGS_NONE },

    // [93] "of the Anduin"
    { .d = AFFIX_FLAGS_NONE },

    // [94] "of the Second Age"
    { .d = AFFIX_FLAGS_NONE },

    // [95] "of Amon Sul"
    { .d = AFFIX_FLAGS_NONE },

    // [96] "of Damrod"
    { .d = AFFIX_FLAGS_NONE },

    // [97] "of the Queen"
    { .d = AFFIX_FLAGS_NONE },

    // [98] "of the Owl"
    { .d = AFFIX_FLAGS_NONE },

    // [99] "of the East"
    { .d = AFFIX_FLAGS_NONE },

    // [100] "of the Dwarf King"
    { .d = AFFIX_FLAGS_NONE },

    // [101] "of the Stars"
    { .d = AFFIX_FLAGS_NONE },

    // [102] "of the White Mountains"
    { .d = AFFIX_FLAGS_NONE },

    // [103] "of Hope"
    { .d = AFFIX_FLAGS_NONE },

    // [104] "of the Eldar"
    { .d = AFFIX_FLAGS_NONE },

    // [105] "of the Beyond"
    { .d = AFFIX_FLAGS_NONE },

    // [106] "of the Evenstar"
    { .d = AFFIX_FLAGS_NONE },

    // [107] "of Galadriel"
    { .d = AFFIX_FLAGS_NONE },

    // [108] "of Mt. Mindolluin"
    { .d = AFFIX_FLAGS_NONE },

    // [109] "of Minas Tirith"
    { .d = AFFIX_FLAGS_NONE },

    // [110] "of the First Age"
    { .d = AFFIX_FLAGS_NONE },

    // [111] "of Numenor"
    { .d = AFFIX_FLAGS_NONE },

    // [112] "of Legend"
    { .d = AFFIX_FLAGS_NONE },

    // [113] "of Beregond"
    { .d = AFFIX_FLAGS_NONE },

    // [114] "of the West"
    { .d = AFFIX_FLAGS_NONE },

    // [115] "of Arnor"
    { .d = AFFIX_FLAGS_NONE },

    // [116] "of Ithilien"
    { .d = AFFIX_FLAGS_NONE },

    // [117] "of Amon Hen"
    { .d = AFFIX_FLAGS_NONE },

    // [118] "of the Archer King"
    { .d = AFFIX_FLAGS_NONE },

    // [119] "of Dunharrow"
    { .d = AFFIX_FLAGS_NONE },

    // [120] "of Umbar"
    { .d = AFFIX_FLAGS_NONE },

    // [121] "of the Wizards"
    { .d = AFFIX_FLAGS_NONE },

    // [122] "of Darkness"
    { .d = AFFIX_FLAGS_NONE },

    // [123] "of the Undead"
    { .d = AFFIX_FLAGS_NONE },

    // [124] "of the Immortal"
    { .d = { .minLevelPlus5A = 1 } },

    // [125] "of Minas Morgul"
    { .d = AFFIX_FLAGS_NONE },

    // [126] "of the Witch King"
    { .d = AFFIX_FLAGS_NONE },

    // [127] "of Cirith Ungol"
    { .d = AFFIX_FLAGS_NONE },

    // [128] "of Anorien"
    { .d = AFFIX_FLAGS_NONE },

    // [129] "of the Eagles"
    { .d = AFFIX_FLAGS_NONE },

    // [130] "of Ash"
    { .d = AFFIX_FLAGS_NONE },

    // [131] "of the Dead"
    { .d = AFFIX_FLAGS_NONE },

    // [132] "of Law"
    { .d = AFFIX_FLAGS_NONE },

    // [133] "of Fathers"
    { .d = AFFIX_FLAGS_NONE },

    // [134] "of the Corsairs"
    { .d = AFFIX_FLAGS_NONE },

    // [135] "of the Black Sails"
    { .d = AFFIX_FLAGS_NONE },

    // [136] "of Dimholt"
    { .d = AFFIX_FLAGS_NONE },

    // [137] "of Underharrow"
    { .d = AFFIX_FLAGS_NONE },

    // [138] "of Weathertop"
    { .d = AFFIX_FLAGS_NONE },

    // [139] "of Henneth Annun"
    { .d = AFFIX_FLAGS_NONE },

    // [140] "of the Undying Lands"
    { .d = AFFIX_FLAGS_NONE },

    // [141] "of the Kings of Men"
    { .d = AFFIX_FLAGS_NONE },

    // [142] "of the Valar"
    { .d = AFFIX_FLAGS_NONE },

    // [143] "of Ecthelion Tower"
    { .d = AFFIX_FLAGS_NONE },

    // [144] "of the Ghost King"
    { .d = AFFIX_FLAGS_NONE },

    // [145] "of the Stag"
    { .d = AFFIX_FLAGS_NONE },

    // [146] "of Ages"
    { .d = AFFIX_FLAGS_NONE },

    // [147] "of Doom"
    { .d = { .minLevelPlus5A = 1 } },

    // [148] "of Udun"
    { .d = AFFIX_FLAGS_NONE },

    // [149] "of Elfsong"
    { .d = AFFIX_FLAGS_NONE },

    // [150] "of Pelennor"
    { .d = AFFIX_FLAGS_NONE },

    // [151] "of the Abyss"
    { .d = AFFIX_FLAGS_NONE },

    // [152] "of the Wraiths"
    { .d = AFFIX_FLAGS_NONE },

    // [153] "of the Towers"
    { .d = AFFIX_FLAGS_NONE },

    // [154] "of the Kings"
    { .d = AFFIX_FLAGS_NONE },

    // [155] "of the Fates"
    { .d = AFFIX_FLAGS_NONE },

    // [156] "of the Argonath"
    { .d = AFFIX_FLAGS_NONE },

    // [157] "of Mt. Doom"
    { .d = AFFIX_FLAGS_NONE },

    // [158] "of the Dark Lord"
    { .d = AFFIX_FLAGS_NONE },

    // [159] "of Mordor"
    { .d = AFFIX_FLAGS_NONE },
};
#undef AFFIX_FLAGS_NONE
// clang-format on
