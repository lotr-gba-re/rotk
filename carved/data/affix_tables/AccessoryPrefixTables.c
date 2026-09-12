#include "item.h"
#include "stats.h"
#include "text_ids.h"

// clang-format off


/** @romaddress 0x08067248 */
const ItemAffix AccessoryPrefixes[156] = {
    // [0] "Cheap": Spirit Points -5, Cheap +1
    { .nameId = TEXT_ID_PREFIX_CHEAP,
      .tier = 1,
      .stat0 = STAT_MAX_SPIRIT,                .val0 =  -5,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_CHEAP,                     .val3 =   1 },

    // [1] "Tarnished": Strength +3, Cursed +1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_TARNISHED,
      .tier = 1,
      .stat0 = STAT_STRENGTH,                  .val0 =   3,
      .stat1 = STAT_CURSED,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_CHEAP,                     .val3 =   1 },

    // [2] "Brass": Damage +1, Critical Armor +1
    { .nameId = TEXT_ID_PREFIX_BRASS,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   1,
      .stat1 = STAT_CRITICAL_ARMOR,            .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [3] "Ebony": Courage +1, Health +1
    { .nameId = TEXT_ID_PREFIX_EBONY,
      .tier = 1,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [4] "Rusty": Defense +1, Accuracy +1
    { .nameId = TEXT_ID_PREFIX_RUSTY,
      .tier = 1,
      .stat0 = STAT_DEFENSE,                   .val0 =   1,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [5] "Iron": Strength -1, Courage +3
    { .nameId = TEXT_ID_PREFIX_IRON,
      .tier = 1,
      .stat0 = STAT_STRENGTH,                  .val0 =  -1,
      .stat1 = STAT_COURAGE,                   .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [6] "Green Copper": % Speed +5
    { .nameId = TEXT_ID_PREFIX_GREEN_COPPER,
      .tier = 1,
      .stat0 = STAT_SPEED_PERCENT,             .val0 =   5,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [7] "Wrought": Health +1, Strength +1
    { .nameId = TEXT_ID_PREFIX_WROUGHT,
      .tier = 1,
      .stat0 = STAT_HEALTH,                    .val0 =   1,
      .stat1 = STAT_STRENGTH,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [8] "Stone": % Critical +5, % Speed -5, Strength +1
    { .nameId = TEXT_ID_PREFIX_STONE,
      .tier = 3,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =   5,
      .stat1 = STAT_SPEED_PERCENT,             .val1 =  -5,
      .stat2 = STAT_STRENGTH,                  .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [9] "Granite": Strength +2, Spirit Points +5
    { .nameId = TEXT_ID_PREFIX_GRANITE,
      .tier = 3,
      .stat0 = STAT_STRENGTH,                  .val0 =   2,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [10] "Raging": Hit Points +10, % Critical +5
    { .nameId = TEXT_ID_PREFIX_RAGING,
      .tier = 5,
      .stat0 = STAT_MAX_HP,                    .val0 =  10,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,       .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [11] "Black Glass": % Extra Treasure +15, Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_BLACK_GLASS,
      .tier = 5,
      .stat0 = STAT_EXTRA_TREASURE_PERCENT,    .val0 =  15,
      .stat1 = STAT_GEMFINDER,                 .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [12] "Copper": Defense +2, Strength -1
    { .nameId = TEXT_ID_PREFIX_COPPER,
      .tier = 2,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_STRENGTH,                  .val1 =  -1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [13] "Ivory": Defense +1
    { .nameId = TEXT_ID_PREFIX_IVORY,
      .tier = 2,
      .stat0 = STAT_DEFENSE,                   .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [14] "Hobbit": Health +1, Backstab +1
    { .nameId = TEXT_ID_PREFIX_HOBBIT,
      .tier = 2,
      .stat0 = STAT_HEALTH,                    .val0 =   1,
      .stat1 = STAT_BACKSTAB,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [15] "Uruk": Hit Points +5, Accuracy +1
    { .nameId = TEXT_ID_PREFIX_URUK,
      .tier = 2,
      .stat0 = STAT_MAX_HP,                    .val0 =   5,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [16] "Troll Glass": Fire Damage +3
    { .nameId = TEXT_ID_PREFIX_TROLL_GLASS,
      .tier = 2,
      .stat0 = STAT_DAMAGE_FIRE,               .val0 =   3,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [17] "Sharded": Defense +1, % Speed +5
    { .nameId = TEXT_ID_PREFIX_SHARDED,
      .tier = 2,
      .stat0 = STAT_DEFENSE,                   .val0 =   1,
      .stat1 = STAT_SPEED_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [18] "Bronze": % Dodge +5, Accuracy -1
    { .nameId = TEXT_ID_PREFIX_BRONZE,
      .tier = 2,
      .stat0 = STAT_DODGE_PERCENT,             .val0 =   5,
      .stat1 = STAT_ACCURACY,                  .val1 =  -1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [19] "Heavy": Strength +1, Courage +1
    { .nameId = TEXT_ID_PREFIX_HEAVY,
      .tier = 2,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_COURAGE,                   .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [20] "Silverstar": Missile Armor +2, Health +1
    { .nameId = TEXT_ID_PREFIX_SILVERSTAR,
      .tier = 4,
      .stat0 = STAT_MISSILE_ARMOR,             .val0 =   2,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [21] "Elven": % Critical +5, Spirit Points +5
    { .nameId = TEXT_ID_PREFIX_ELVEN,
      .tier = 4,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =   5,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [22] "Speckled": Damage +1, Knockback +10
    { .nameId = TEXT_ID_PREFIX_SPECKLED,
      .tier = 6,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   1,
      .stat1 = STAT_KNOCKBACK,                 .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [23] "Glittering": Courage +3, % Extra Treasure +25
    { .nameId = TEXT_ID_PREFIX_GLITTERING,
      .tier = 6,
      .stat0 = STAT_COURAGE,                   .val0 =   3,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  25,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [24] "Spidersilver": Health +2, Courage +1
    { .nameId = TEXT_ID_PREFIX_SPIDERSILVER,
      .tier = 3,
      .stat0 = STAT_HEALTH,                    .val0 =   2,
      .stat1 = STAT_COURAGE,                   .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [25] "Spider's": Strength +1, % Critical +5
    { .nameId = TEXT_ID_PREFIX_SPIDERS,
      .tier = 3,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,       .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [26] "Ancient": % Critical +5, Fire Protection +3
    { .nameId = TEXT_ID_PREFIX_ANCIENT,
      .tier = 3,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =   5,
      .stat1 = STAT_FIRE_PROTECTION,           .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [27] "Forgotten": % Extra Treasure +20, Accuracy +1
    { .nameId = TEXT_ID_PREFIX_FORGOTTEN,
      .tier = 3,
      .stat0 = STAT_EXTRA_TREASURE_PERCENT,    .val0 =  20,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [28] "Half-Eaten": Accuracy -1, Spirit Points +15
    { .nameId = TEXT_ID_PREFIX_HALF_EATEN,
      .tier = 3,
      .stat0 = STAT_ACCURACY,                  .val0 =  -1,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  15,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [29] "Bloody": Defense +1, Health +1
    { .nameId = TEXT_ID_PREFIX_BLOODY,
      .tier = 3,
      .stat0 = STAT_DEFENSE,                   .val0 =   1,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [30] "Webbed": Immobilize +1, Hit Points +10
    { .nameId = TEXT_ID_PREFIX_WEBBED,
      .tier = 3,
      .stat0 = STAT_IMMOBILIZE,                .val0 =   1,
      .stat1 = STAT_MAX_HP,                    .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [31] "Venomous": % Poison Resistance +20, Venomous +1
    { .nameId = TEXT_ID_PREFIX_VENOMOUS,
      .tier = 3,
      .stat0 = STAT_POISON_RESIST_PERCENT,     .val0 =  20,
      .stat1 = STAT_VENOMOUS,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [32] "Dwarf Etched": Melee Armor +1, Damage +2
    { .nameId = TEXT_ID_PREFIX_DWARF_ETCHED,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [33] "Old Dwarf's": Courage +2, Knockback +10
    { .nameId = TEXT_ID_PREFIX_OLD_DWARFS,
      .tier = 5,
      .stat0 = STAT_COURAGE,                   .val0 =   2,
      .stat1 = STAT_KNOCKBACK,                 .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [34] "Mummified": Courage +3, Immobilize +1
    { .nameId = TEXT_ID_PREFIX_MUMMIFIED,
      .tier = 7,
      .stat0 = STAT_COURAGE,                   .val0 =   3,
      .stat1 = STAT_IMMOBILIZE,                .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [35] "Spiderkin": % Dodge +10, Strength +1
    { .nameId = TEXT_ID_PREFIX_SPIDERKIN,
      .tier = 7,
      .stat0 = STAT_DODGE_PERCENT,             .val0 =  10,
      .stat1 = STAT_STRENGTH,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [36] "Trollbone": % Speed -5, Nightburst +1
    { .nameId = TEXT_ID_PREFIX_TROLLBONE,
      .tier = 4,
      .stat0 = STAT_SPEED_PERCENT,             .val0 =  -5,
      .stat1 = STAT_NIGHTBURST,                .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [37] "Olog's": Strength +2, % Critical +5
    { .nameId = TEXT_ID_PREFIX_OLOGS,
      .tier = 4,
      .stat0 = STAT_STRENGTH,                  .val0 =   2,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,       .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [38] "Bloodstone": Health -5, Spirit Points +25
    { .nameId = TEXT_ID_PREFIX_BLOODSTONE,
      .tier = 4,
      .stat0 = STAT_HEALTH,                    .val0 =  -5,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  25,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [39] "Golden": Spirit Points +10, Sunburst +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_GOLDEN,
      .tier = 4,
      .stat0 = STAT_MAX_SPIRIT,                .val0 =  10,
      .stat1 = STAT_SUNBURST,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [40] "Opal": % Dodge +5, % Speed +5
    { .nameId = TEXT_ID_PREFIX_OPAL,
      .tier = 4,
      .stat0 = STAT_DODGE_PERCENT,             .val0 =   5,
      .stat1 = STAT_SPEED_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [41] "Amethyst": Defense +2, Accuracy +1
    { .nameId = TEXT_ID_PREFIX_AMETHYST,
      .tier = 4,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [42] "Jade": Hit Points -5, Critical Armor +3
    { .nameId = TEXT_ID_PREFIX_JADE,
      .tier = 4,
      .stat0 = STAT_MAX_HP,                    .val0 =  -5,
      .stat1 = STAT_CRITICAL_ARMOR,            .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [43] "Electrum": % Speed +5, Courage +1
    { .nameId = TEXT_ID_PREFIX_ELECTRUM,
      .tier = 4,
      .stat0 = STAT_SPEED_PERCENT,             .val0 =   5,
      .stat1 = STAT_COURAGE,                   .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [44] "Eldarstone": Hit Points +15, Wisdom +2
    { .nameId = TEXT_ID_PREFIX_ELDARSTONE,
      .tier = 6,
      .stat0 = STAT_MAX_HP,                    .val0 =  15,
      .stat1 = STAT_WISDOM,                    .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [45] "Fellstone": Health +8, Cursed +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_FELLSTONE,
      .tier = 6,
      .stat0 = STAT_HEALTH,                    .val0 =   8,
      .stat1 = STAT_CURSED,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [46] "Nazgul's": Melee Armor +25, % Extra Treasure +3, % Corruption Resistance -50
    { .nameId = TEXT_ID_PREFIX_NAZGULS,
      .tier = 8,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =  25,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =   3,
      .stat2 = STAT_CORRUPTION_RESIST_PERCENT, .val2 = -50,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [47] "Haven Stone": % Critical +10, Armor Piercing +1
    { .nameId = TEXT_ID_PREFIX_HAVEN_STONE,
      .tier = 8,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =  10,
      .stat1 = STAT_ARMOR_PIERCE,              .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [48] "Platinum": Courage +3, % Critical +5, Valuable +1
    { .nameId = TEXT_ID_PREFIX_PLATINUM,
      .tier = 5,
      .stat0 = STAT_COURAGE,                   .val0 =   3,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,       .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [49] "Ruby": Strength +1, Hit Points +10
    { .nameId = TEXT_ID_PREFIX_RUBY,
      .tier = 5,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_MAX_HP,                    .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [50] "Emerald": Damage +2, Health +1
    { .nameId = TEXT_ID_PREFIX_EMERALD,
      .tier = 5,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   2,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [51] "Darkstone": Accuracy +2, Strength +1
    { .nameId = TEXT_ID_PREFIX_DARKSTONE,
      .tier = 5,
      .stat0 = STAT_ACCURACY,                  .val0 =   2,
      .stat1 = STAT_STRENGTH,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [52] "Fossil": Strength -1, % Extra Treasure +15
    { .nameId = TEXT_ID_PREFIX_FOSSIL,
      .tier = 5,
      .stat0 = STAT_STRENGTH,                  .val0 =  -1,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  15,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [53] "Onyx": Courage -2, Backstab +1, Damage +1
    { .nameId = TEXT_ID_PREFIX_ONYX,
      .tier = 5,
      .stat0 = STAT_COURAGE,                   .val0 =  -2,
      .stat1 = STAT_BACKSTAB,                  .val1 =   1,
      .stat2 = STAT_DAMAGE_FLAT,               .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [54] "Steel": Missile Armor +2, Damage +1
    { .nameId = TEXT_ID_PREFIX_STEEL,
      .tier = 5,
      .stat0 = STAT_MISSILE_ARMOR,             .val0 =   2,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [55] "Gleaming": Spirit Points +20
    { .nameId = TEXT_ID_PREFIX_GLEAMING,
      .tier = 5,
      .stat0 = STAT_MAX_SPIRIT,                .val0 =  20,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [56] "Glowing": % Dodge +5, Fire Protection +5
    { .nameId = TEXT_ID_PREFIX_GLOWING,
      .tier = 7,
      .stat0 = STAT_DODGE_PERCENT,             .val0 =   5,
      .stat1 = STAT_FIRE_PROTECTION,           .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [57] "Dragon's": Defense +2, Spirit Points +25
    { .nameId = TEXT_ID_PREFIX_DRAGONS,
      .tier = 7,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  25,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [58] "Tainted": Cursed +1, Nightburst +1, All Primary Stats +1
    { .nameId = TEXT_ID_PREFIX_TAINTED,
      .tier = 9,
      .stat0 = STAT_CURSED,                    .val0 =   1,
      .stat1 = STAT_NIGHTBURST,                .val1 =   1,
      .stat2 = STAT_ALL_PRIMARY_STATS,         .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [59] "Isildur's": Spirit Points +35, % Dodge +10
    { .nameId = TEXT_ID_PREFIX_ISILDURS,
      .tier = 9,
      .stat0 = STAT_MAX_SPIRIT,                .val0 =  35,
      .stat1 = STAT_DODGE_PERCENT,             .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [60] "Crystal": % Extra Treasure +35, Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_CRYSTAL,
      .tier = 6,
      .stat0 = STAT_EXTRA_TREASURE_PERCENT,    .val0 =  35,
      .stat1 = STAT_GEMFINDER,                 .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [61] "Miriel": Melee Armor +2, % Critical +5
    { .nameId = TEXT_ID_PREFIX_MIRIEL,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =   2,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,       .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [62] "Mithril": Defense +2, Melee Armor +2
    { .nameId = TEXT_ID_PREFIX_MITHRIL,
      .tier = 6,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_MELEE_ARMOR,               .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [63] "Diamond": Damage +2, % Extra Treasure +50
    { .nameId = TEXT_ID_PREFIX_DIAMOND,
      .tier = 6,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   2,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  50,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [64] "Radiant": Defense +3, Spirit Points +20
    { .nameId = TEXT_ID_PREFIX_RADIANT,
      .tier = 6,
      .stat0 = STAT_DEFENSE,                   .val0 =   3,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  20,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [65] "Fiery": Fire Damage +3, Accuracy +1
    { .nameId = TEXT_ID_PREFIX_FIERY,
      .tier = 6,
      .stat0 = STAT_DAMAGE_FIRE,               .val0 =   3,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [66] "Wraith Gem": % Speed +10, Nightburst +1
    { .nameId = TEXT_ID_PREFIX_WRAITH_GEM,
      .tier = 6,
      .stat0 = STAT_SPEED_PERCENT,             .val0 =  10,
      .stat1 = STAT_NIGHTBURST,                .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [67] "Blackrock": Hit Points +25, Damage +1
    { .nameId = TEXT_ID_PREFIX_BLACKROCK,
      .tier = 6,
      .stat0 = STAT_MAX_HP,                    .val0 =  25,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [68] "Elendil's": Health +4, Missile Armor +3
    { .nameId = TEXT_ID_PREFIX_ELENDILS,
      .tier = 8,
      .stat0 = STAT_HEALTH,                    .val0 =   4,
      .stat1 = STAT_MISSILE_ARMOR,             .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [69] "Gil-Galad's": Damage +4, Health +3
    { .nameId = TEXT_ID_PREFIX_GIL_GALADS,
      .tier = 8,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   4,
      .stat1 = STAT_HEALTH,                    .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [70] "Terrible": % Critical +15, Valuable +1
    { .nameId = TEXT_ID_PREFIX_TERRIBLE,
      .tier = 10,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =  15,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [71] "Doomrock": Damage +5, Strength +1
    { .nameId = TEXT_ID_PREFIX_DOOMROCK,
      .tier = 10,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   5,
      .stat1 = STAT_STRENGTH,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [72] "Ugluk's": Hit Points +3
    { .nameId = TEXT_ID_PREFIX_UGLUKS,
      .tier = 1,
      .stat0 = STAT_MAX_HP,                    .val0 =   3,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [73] "Towerstone": Strength +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_TOWERSTONE,
      .tier = 1,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [74] "White Opal": Wisdom +1
    { .nameId = TEXT_ID_PREFIX_WHITE_OPAL,
      .tier = 1,
      .stat0 = STAT_WISDOM,                    .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [75] "Eothain's": Courage +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_EOTHAINS,
      .tier = 1,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [76] "Corroded": Damage +2, Accuracy -2, Cheap +1
    { .nameId = TEXT_ID_PREFIX_CORRODED,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   2,
      .stat1 = STAT_ACCURACY,                  .val1 =  -2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_CHEAP,                     .val3 =   1 },

    // [77] "Waterstone": Health +1, Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_WATERSTONE,
      .tier = 1,
      .stat0 = STAT_HEALTH,                    .val0 =   1,
      .stat1 = STAT_GEMFINDER,                 .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [78] "Lurtz's": Strength +1
    { .nameId = TEXT_ID_PREFIX_LURTZS,
      .tier = 1,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [79] "Palestone": Defense +1, Hit Points +3
    { .nameId = TEXT_ID_PREFIX_PALESTONE,
      .tier = 1,
      .stat0 = STAT_DEFENSE,                   .val0 =   1,
      .stat1 = STAT_MAX_HP,                    .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [80] "Gandalf's": Defense +1, Accuracy +1, Wisdom +1, Courage +1
    { .nameId = TEXT_ID_PREFIX_GANDALFS,
      .tier = 3,
      .stat0 = STAT_DEFENSE,                   .val0 =   1,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_WISDOM,                    .val2 =   1,
      .stat3 = STAT_COURAGE,                   .val3 =   1 },

    // [81] "Ent Eye": Defense +1, % Fear Resistance +30, % Poison Resistance +30, Valuable +1
    { .nameId = TEXT_ID_PREFIX_ENT_EYE,
      .tier = 3,
      .stat0 = STAT_DEFENSE,                   .val0 =   1,
      .stat1 = STAT_FEAR_RESIST_PERCENT,       .val1 =  30,
      .stat2 = STAT_POISON_RESIST_PERCENT,     .val2 =  30,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [82] "Arcane": Strength +1, Spirit Regen +3
    { .nameId = TEXT_ID_PREFIX_ARCANE,
      .tier = 5,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_SPIRIT_REGEN,              .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [83] "Palantir-chip": Health +3, Damage +3, Cursed +1
    { .nameId = TEXT_ID_PREFIX_PALANTIR_CHIP,
      .tier = 5,
      .stat0 = STAT_HEALTH,                    .val0 =   3,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   3,
      .stat2 = STAT_CURSED,                    .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [84] "Morwen's": Melee Armor +1
    { .nameId = TEXT_ID_PREFIX_MORWENS,
      .tier = 2,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [85] "Refugee's": Courage +1
    { .nameId = TEXT_ID_PREFIX_REFUGEES,
      .tier = 2,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [86] "Beaded": Health +1, Spirit Points +5
    { .nameId = TEXT_ID_PREFIX_BEADED,
      .tier = 2,
      .stat0 = STAT_HEALTH,                    .val0 =   1,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [87] "Gemstone": Accuracy +3, Spirit Points +5, Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_GEMSTONE,
      .tier = 2,
      .stat0 = STAT_ACCURACY,                  .val0 =   3,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =   5,
      .stat2 = STAT_GEMFINDER,                 .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [88] "Blue Sapphire": Health +1, Spirit Points +5
    { .nameId = TEXT_ID_PREFIX_BLUE_SAPPHIRE,
      .tier = 2,
      .stat0 = STAT_HEALTH,                    .val0 =   1,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [89] "Topaz": Strength +1, Spirit Points +5, Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_TOPAZ,
      .tier = 2,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =   5,
      .stat2 = STAT_GEMFINDER,                 .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [90] "Rare": Health +1, Spirit Points +8, Valuable +1
    { .nameId = TEXT_ID_PREFIX_RARE,
      .tier = 2,
      .stat0 = STAT_HEALTH,                    .val0 =   1,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =   8,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [91] "Fieldstone": Strength +1, Health +1
    { .nameId = TEXT_ID_PREFIX_FIELDSTONE,
      .tier = 2,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [92] "Eowyn's": Defense +1, Courage +2, dmg to Orcs +1
    { .nameId = TEXT_ID_PREFIX_EOWYNS,
      .tier = 4,
      .stat0 = STAT_DEFENSE,                   .val0 =   1,
      .stat1 = STAT_COURAGE,                   .val1 =   2,
      .stat2 = STAT_DAMAGE_TO_ORCS,            .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [93] "Starlit": Defense +2, Melee Armor +1, % Extra Treasure +15
    { .nameId = TEXT_ID_PREFIX_STARLIT,
      .tier = 4,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_MELEE_ARMOR,               .val1 =   1,
      .stat2 = STAT_EXTRA_TREASURE_PERCENT,    .val2 =  15,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [94] "Tourmaline": Strength +1, Accuracy +1, Hit Points Regen +3
    { .nameId = TEXT_ID_PREFIX_TOURMALINE,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_HP_REGEN,                  .val2 =   3,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [95] "Yellow Sapphire": Strength +3, Accuracy +1, Melee Armor +2, Valuable +1
    { .nameId = TEXT_ID_PREFIX_YELLOW_SAPPHIRE,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =   3,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_MELEE_ARMOR,               .val2 =   2,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [96] "Spinel": Strength +2
    { .nameId = TEXT_ID_PREFIX_SPINEL,
      .tier = 3,
      .stat0 = STAT_STRENGTH,                  .val0 =   2,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [97] "Iolite": Hit Points Regen +2
    { .nameId = TEXT_ID_PREFIX_IOLITE,
      .tier = 3,
      .stat0 = STAT_HP_REGEN,                  .val0 =   2,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [98] "White Sapphire": Courage +2, Valuable +1
    { .nameId = TEXT_ID_PREFIX_WHITE_SAPPHIRE,
      .tier = 3,
      .stat0 = STAT_COURAGE,                   .val0 =   2,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [99] "Precious": % Experience Bonus +5
    { .nameId = TEXT_ID_PREFIX_PRECIOUS,
      .tier = 3,
      .stat0 = STAT_EXTRA_EXP_PERCENT,         .val0 =   5,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [100] "Silver": Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_SILVER,
      .tier = 3,
      .stat0 = STAT_GEMFINDER,                 .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [101] "Elfstone": Accuracy +2, Spirit Points +5, % Extra Treasure +15
    { .nameId = TEXT_ID_PREFIX_ELFSTONE,
      .tier = 3,
      .stat0 = STAT_ACCURACY,                  .val0 =   2,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =   5,
      .stat2 = STAT_EXTRA_TREASURE_PERCENT,    .val2 =  15,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [102] "Azure": Strength +1, Melee Armor +1
    { .nameId = TEXT_ID_PREFIX_AZURE,
      .tier = 3,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_MELEE_ARMOR,               .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [103] "Warrior's": Critical Armor +3, Missile Armor +2, Melee Armor +1
    { .nameId = TEXT_ID_PREFIX_WARRIORS,
      .tier = 3,
      .stat0 = STAT_CRITICAL_ARMOR,            .val0 =   3,
      .stat1 = STAT_MISSILE_ARMOR,             .val1 =   2,
      .stat2 = STAT_MELEE_ARMOR,               .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [104] "Cavalier's": All Primary Stats +1, % Poison Resistance +10
    { .nameId = TEXT_ID_PREFIX_CAVALIERS,
      .tier = 5,
      .stat0 = STAT_ALL_PRIMARY_STATS,         .val0 =   1,
      .stat1 = STAT_POISON_RESIST_PERCENT,     .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [105] "Princess": % Experience Bonus +5, % Poison Resistance +10, % Fear Resistance +10
    { .nameId = TEXT_ID_PREFIX_PRINCESS,
      .tier = 5,
      .stat0 = STAT_EXTRA_EXP_PERCENT,         .val0 =   5,
      .stat1 = STAT_POISON_RESIST_PERCENT,     .val1 =  10,
      .stat2 = STAT_FEAR_RESIST_PERCENT,       .val2 =  10,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [106] "Prismatic": All Primary Stats +2
    { .nameId = TEXT_ID_PREFIX_PRISMATIC,
      .tier = 7,
      .stat0 = STAT_ALL_PRIMARY_STATS,         .val0 =   2,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [107] "Elrond's": % Experience Bonus +5
    { .nameId = TEXT_ID_PREFIX_ELRONDS,
      .tier = 7,
      .stat0 = STAT_EXTRA_EXP_PERCENT,         .val0 =   5,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [108] "Moonstone": Spirit Points +20, % Fear Resistance +10, Valuable +1
    { .nameId = TEXT_ID_PREFIX_MOONSTONE,
      .tier = 4,
      .stat0 = STAT_MAX_SPIRIT,                .val0 =  20,
      .stat1 = STAT_FEAR_RESIST_PERCENT,       .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [109] "Smoky Topaz": Courage +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_SMOKY_TOPAZ,
      .tier = 4,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [110] "Amber": Immobilize +1
    { .nameId = TEXT_ID_PREFIX_AMBER,
      .tier = 4,
      .stat0 = STAT_IMMOBILIZE,                .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [111] "Citrine": Courage +2, Defense +1
    { .nameId = TEXT_ID_PREFIX_CITRINE,
      .tier = 4,
      .stat0 = STAT_COURAGE,                   .val0 =   2,
      .stat1 = STAT_DEFENSE,                   .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [112] "Bloodveined": Strength +1, Damage +1, Hit Points Regen +3
    { .nameId = TEXT_ID_PREFIX_BLOODVEINED,
      .tier = 4,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_HP_REGEN,                  .val2 =   3,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [113] "Peridot": Defense +2, Strength +1, Spirit Points +5
    { .nameId = TEXT_ID_PREFIX_PERIDOT,
      .tier = 4,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_STRENGTH,                  .val1 =   1,
      .stat2 = STAT_MAX_SPIRIT,                .val2 =   5,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [114] "Orbed": Health +1, Defense +1, Wisdom +2
    { .nameId = TEXT_ID_PREFIX_ORBED,
      .tier = 4,
      .stat0 = STAT_HEALTH,                    .val0 =   1,
      .stat1 = STAT_DEFENSE,                   .val1 =   1,
      .stat2 = STAT_WISDOM,                    .val2 =   2,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [115] "Brittle": Courage +1, Wisdom +2
    { .nameId = TEXT_ID_PREFIX_BRITTLE,
      .tier = 4,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_WISDOM,                    .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [116] "Glowstone": Courage +2, Damage +1, % Fear Resistance +20
    { .nameId = TEXT_ID_PREFIX_GLOWSTONE,
      .tier = 6,
      .stat0 = STAT_COURAGE,                   .val0 =   2,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_FEAR_RESIST_PERCENT,       .val2 =  20,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [117] "Spectral": Damage +1, Wisdom +1
    { .nameId = TEXT_ID_PREFIX_SPECTRAL,
      .tier = 6,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   1,
      .stat1 = STAT_WISDOM,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [118] "Ghoststone": Strength +3, Courage +2, % Fear Resistance +20
    { .nameId = TEXT_ID_PREFIX_GHOSTSTONE,
      .tier = 8,
      .stat0 = STAT_STRENGTH,                  .val0 =   3,
      .stat1 = STAT_COURAGE,                   .val1 =   2,
      .stat2 = STAT_FEAR_RESIST_PERCENT,       .val2 =  20,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [119] "Ethereal": Defense +7, Accuracy +5, % Experience Bonus -10
    { .nameId = TEXT_ID_PREFIX_ETHEREAL,
      .tier = 8,
      .stat0 = STAT_DEFENSE,                   .val0 =   7,
      .stat1 = STAT_ACCURACY,                  .val1 =   5,
      .stat2 = STAT_EXTRA_EXP_PERCENT,         .val2 = -10,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [120] "White Diamond": Strength +3, Defense +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_WHITE_DIAMOND,
      .tier = 5,
      .stat0 = STAT_STRENGTH,                  .val0 =   3,
      .stat1 = STAT_DEFENSE,                   .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [121] "Ice Gem": Defense +1, Critical Armor +2, Fire Protection +3
    { .nameId = TEXT_ID_PREFIX_ICE_GEM,
      .tier = 5,
      .stat0 = STAT_DEFENSE,                   .val0 =   1,
      .stat1 = STAT_CRITICAL_ARMOR,            .val1 =   2,
      .stat2 = STAT_FIRE_PROTECTION,           .val2 =   3,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [122] "Snow Crystal": Accuracy +1, Health +2, Spirit Regen +2
    { .nameId = TEXT_ID_PREFIX_SNOW_CRYSTAL,
      .tier = 5,
      .stat0 = STAT_ACCURACY,                  .val0 =   1,
      .stat1 = STAT_HEALTH,                    .val1 =   2,
      .stat2 = STAT_SPIRIT_REGEN,              .val2 =   2,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [123] "Dwarf Maiden": Melee Armor +2, Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_DWARF_MAIDEN,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =   2,
      .stat1 = STAT_GEMFINDER,                 .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [124] "Crystalline": Courage +1, Spirit Regen +3, Hit Points Regen +1
    { .nameId = TEXT_ID_PREFIX_CRYSTALLINE,
      .tier = 5,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_SPIRIT_REGEN,              .val1 =   3,
      .stat2 = STAT_HP_REGEN,                  .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [125] "Polished": Defense +2, Accuracy +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_POLISHED,
      .tier = 5,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [126] "Gold Dusted": Gemfinder +1, Accuracy +2, Damage +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_GOLD_DUSTED,
      .tier = 5,
      .stat0 = STAT_GEMFINDER,                 .val0 =   1,
      .stat1 = STAT_ACCURACY,                  .val1 =   2,
      .stat2 = STAT_DAMAGE_FLAT,               .val2 =   1,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [127] "Shining": dmg to Orcs +1, % Experience Bonus +5
    { .nameId = TEXT_ID_PREFIX_SHINING,
      .tier = 5,
      .stat0 = STAT_DAMAGE_TO_ORCS,            .val0 =   1,
      .stat1 = STAT_EXTRA_EXP_PERCENT,         .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [128] "Dwarf Diamond": dmg to Orcs +5, Valuable +1
    { .nameId = TEXT_ID_PREFIX_DWARF_DIAMOND,
      .tier = 7,
      .stat0 = STAT_DAMAGE_TO_ORCS,            .val0 =   5,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [129] "Snow Ruby": Strength +3, Defense +1, Accuracy +1
    { .nameId = TEXT_ID_PREFIX_SNOW_RUBY,
      .tier = 7,
      .stat0 = STAT_STRENGTH,                  .val0 =   3,
      .stat1 = STAT_DEFENSE,                   .val1 =   1,
      .stat2 = STAT_ACCURACY,                  .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [130] "Dazzling": Defense +3, Gemfinder +1, Accuracy +2
    { .nameId = TEXT_ID_PREFIX_DAZZLING,
      .tier = 9,
      .stat0 = STAT_DEFENSE,                   .val0 =   3,
      .stat1 = STAT_GEMFINDER,                 .val1 =   1,
      .stat2 = STAT_ACCURACY,                  .val2 =   2,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [131] "Dwarfruned": Strength +5, Gemfinder +1, Hit Points +5
    { .nameId = TEXT_ID_PREFIX_DWARFRUNED,
      .tier = 9,
      .stat0 = STAT_STRENGTH,                  .val0 =   5,
      .stat1 = STAT_GEMFINDER,                 .val1 =   1,
      .stat2 = STAT_MAX_HP,                    .val2 =   5,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [132] "Victory Stone": % Experience Bonus +10
    { .nameId = TEXT_ID_PREFIX_VICTORY_STONE,
      .tier = 6,
      .stat0 = STAT_EXTRA_EXP_PERCENT,         .val0 =  10,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [133] "Haradrim": Strength +3, Accuracy +3, Spirit Points +10, Cursed +1
    { .nameId = TEXT_ID_PREFIX_HARADRIM,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =   3,
      .stat1 = STAT_ACCURACY,                  .val1 =   3,
      .stat2 = STAT_MAX_SPIRIT,                .val2 =  10,
      .stat3 = STAT_CURSED,                    .val3 =   1 },

    // [134] "Flawless": Strength +1, Accuracy +1, Health +1, Damage +1
    { .nameId = TEXT_ID_PREFIX_FLAWLESS,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_HEALTH,                    .val2 =   1,
      .stat3 = STAT_DAMAGE_FLAT,               .val3 =   1 },

    // [135] "Southron's": Strength +2, Courage +1, Health +1
    { .nameId = TEXT_ID_PREFIX_SOUTHRONS,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =   2,
      .stat1 = STAT_COURAGE,                   .val1 =   1,
      .stat2 = STAT_HEALTH,                    .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [136] "Comrade's": Courage +3, Defense +1
    { .nameId = TEXT_ID_PREFIX_COMRADES,
      .tier = 6,
      .stat0 = STAT_COURAGE,                   .val0 =   3,
      .stat1 = STAT_DEFENSE,                   .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [137] "Dull Adamant": Defense +3, dmg to Orcs +2
    { .nameId = TEXT_ID_PREFIX_DULL_ADAMANT,
      .tier = 6,
      .stat0 = STAT_DEFENSE,                   .val0 =   3,
      .stat1 = STAT_DAMAGE_TO_ORCS,            .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [138] "Tirith Rock": Strength +1, Health +3, Hit Points +15
    { .nameId = TEXT_ID_PREFIX_TIRITH_ROCK,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_HEALTH,                    .val1 =   3,
      .stat2 = STAT_MAX_HP,                    .val2 =  15,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [139] "Gondor Gold": Strength +1, Defense +1, Accuracy +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_GONDOR_GOLD,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_DEFENSE,                   .val1 =   1,
      .stat2 = STAT_ACCURACY,                  .val2 =   1,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [140] "Lime Emerald": Hit Points Regen +2, Spirit Regen +2, % Experience Bonus +5
    { .nameId = TEXT_ID_PREFIX_LIME_EMERALD,
      .tier = 8,
      .stat0 = STAT_HP_REGEN,                  .val0 =   2,
      .stat1 = STAT_SPIRIT_REGEN,              .val1 =   2,
      .stat2 = STAT_EXTRA_EXP_PERCENT,         .val2 =   5,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [141] "Denethor's": Strength +10, Courage -5, Cursed +1
    { .nameId = TEXT_ID_PREFIX_DENETHORS,
      .tier = 8,
      .stat0 = STAT_STRENGTH,                  .val0 =  10,
      .stat1 = STAT_COURAGE,                   .val1 =  -5,
      .stat2 = STAT_CURSED,                    .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [142] "King's Craft": Strength +5, Courage +3
    { .nameId = TEXT_ID_PREFIX_KINGS_CRAFT,
      .tier = 10,
      .stat0 = STAT_STRENGTH,                  .val0 =   5,
      .stat1 = STAT_COURAGE,                   .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [143] "Aragorn's": Strength +6, Courage +4, Accuracy +3, Defense +2
    { .nameId = TEXT_ID_PREFIX_ARAGORNS,
      .tier = 10,
      .stat0 = STAT_STRENGTH,                  .val0 =   6,
      .stat1 = STAT_COURAGE,                   .val1 =   4,
      .stat2 = STAT_ACCURACY,                  .val2 =   3,
      .stat3 = STAT_DEFENSE,                   .val3 =   2 },

    // [144] "Black Diamond": Strength +5, % Fear Resistance -20, Valuable +1
    { .nameId = TEXT_ID_PREFIX_BLACK_DIAMOND,
      .tier = 7,
      .stat0 = STAT_STRENGTH,                  .val0 =   5,
      .stat1 = STAT_FEAR_RESIST_PERCENT,       .val1 = -20,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [145] "Twistwood": Defense +2, Health +3, Defense +1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_TWISTWOOD,
      .tier = 7,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_HEALTH,                    .val1 =   3,
      .stat2 = STAT_DEFENSE,                   .val2 =   1,
      .stat3 = STAT_CHEAP,                     .val3 =   1 },

    // [146] "Obsidian": Damage +1, Health +1, Defense +1, % Critical +5
    { .nameId = TEXT_ID_PREFIX_OBSIDIAN,
      .tier = 7,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   1,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_DEFENSE,                   .val2 =   1,
      .stat3 = STAT_CRIT_CHANCE_PERCENT,       .val3 =   5 },

    // [147] "Magnificent": Strength +5, Health +1, Damage +1, % Critical +5
    { .nameId = TEXT_ID_PREFIX_MAGNIFICENT,
      .tier = 7,
      .stat0 = STAT_STRENGTH,                  .val0 =   5,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_DAMAGE_FLAT,               .val2 =   1,
      .stat3 = STAT_CRIT_CHANCE_PERCENT,       .val3 =   5 },

    // [148] "Blackheart": Strength +9, Accuracy +5, Defense -8, Cursed +1
    { .nameId = TEXT_ID_PREFIX_BLACKHEART,
      .tier = 7,
      .stat0 = STAT_STRENGTH,                  .val0 =   9,
      .stat1 = STAT_ACCURACY,                  .val1 =   5,
      .stat2 = STAT_DEFENSE,                   .val2 =  -8,
      .stat3 = STAT_CURSED,                    .val3 =   1 },

    // [149] "Peculiar": Strength -27, Defense +31, % Fear Resistance -50, % Experience Bonus +10
    { .nameId = TEXT_ID_PREFIX_PECULIAR,
      .tier = 7,
      .stat0 = STAT_STRENGTH,                  .val0 = -27,
      .stat1 = STAT_DEFENSE,                   .val1 =  31,
      .stat2 = STAT_FEAR_RESIST_PERCENT,       .val2 = -50,
      .stat3 = STAT_EXTRA_EXP_PERCENT,         .val3 =  10 },

    // [150] "Corrupt": Strength +10, Cursed +1, dmg to Nazgul +5
    { .nameId = TEXT_ID_PREFIX_CORRUPT,
      .tier = 7,
      .stat0 = STAT_STRENGTH,                  .val0 =  10,
      .stat1 = STAT_CURSED,                    .val1 =   1,
      .stat2 = STAT_DAMAGE_TO_NAZGUL,          .val2 =   5,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [151] "Dark Topaz": dmg to Nazgul +7, Accuracy +2, Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_DARK_TOPAZ,
      .tier = 7,
      .stat0 = STAT_DAMAGE_TO_NAZGUL,          .val0 =   7,
      .stat1 = STAT_ACCURACY,                  .val1 =   2,
      .stat2 = STAT_GEMFINDER,                 .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [152] "Dark Sapphire": dmg to Nazgul +5, Defense +4, % Extra Treasure +90
    { .nameId = TEXT_ID_PREFIX_DARK_SAPPHIRE,
      .tier = 9,
      .stat0 = STAT_DAMAGE_TO_NAZGUL,          .val0 =   5,
      .stat1 = STAT_DEFENSE,                   .val1 =   4,
      .stat2 = STAT_EXTRA_TREASURE_PERCENT,    .val2 =  90,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [153] "Vein Crystal": Strength +5, Accuracy +9, Defense +2, % Critical +40
    { .nameId = TEXT_ID_PREFIX_VEIN_CRYSTAL,
      .tier = 9,
      .stat0 = STAT_STRENGTH,                  .val0 =   5,
      .stat1 = STAT_ACCURACY,                  .val1 =   9,
      .stat2 = STAT_DEFENSE,                   .val2 =   2,
      .stat3 = STAT_CRIT_CHANCE_PERCENT,       .val3 =  40 },

    // [154] "Necromancer's": Spirit Regen +8, dmg to Nazgul +17, Cursed +1, % Corruption Resistance -50
    { .nameId = TEXT_ID_PREFIX_NECROMANCERS,
      .tier = 11,
      .stat0 = STAT_SPIRIT_REGEN,              .val0 =   8,
      .stat1 = STAT_DAMAGE_TO_NAZGUL,          .val1 =  17,
      .stat2 = STAT_CURSED,                    .val2 =   1,
      .stat3 = STAT_CORRUPTION_RESIST_PERCENT, .val3 = -50 },

    // [155] "Lordstone": Wisdom +9, Courage +3
    { .nameId = TEXT_ID_PREFIX_LORDSTONE,
      .tier = 11,
      .stat0 = STAT_WISDOM,                    .val0 =   9,
      .stat1 = STAT_COURAGE,                   .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },
};

/** no decoded bits set (148 of 156 entries) */
#define AFFIX_FLAGS_NONE { 0 }

/**
 * Per-affix flag table; indexed by the affix index. Decoded bits: AFFIX_FLAG_* (item.h).
 *
 * @romaddress 0x08067998
 */
const AffixFlags AccessoryPrefixFlags[156] = {
    // [0] "Cheap"
    { .d = AFFIX_FLAGS_NONE },

    // [1] "Tarnished"
    { .d = AFFIX_FLAGS_NONE },

    // [2] "Brass"
    { .d = AFFIX_FLAGS_NONE },

    // [3] "Ebony"
    { .d = AFFIX_FLAGS_NONE },

    // [4] "Rusty"
    { .d = AFFIX_FLAGS_NONE },

    // [5] "Iron"
    { .d = AFFIX_FLAGS_NONE },

    // [6] "Green Copper"
    { .d = AFFIX_FLAGS_NONE },

    // [7] "Wrought"
    { .d = AFFIX_FLAGS_NONE },

    // [8] "Stone"
    { .d = AFFIX_FLAGS_NONE },

    // [9] "Granite"
    { .d = AFFIX_FLAGS_NONE },

    // [10] "Raging"
    { .d = AFFIX_FLAGS_NONE },

    // [11] "Black Glass"
    { .d = AFFIX_FLAGS_NONE },

    // [12] "Copper"
    { .d = AFFIX_FLAGS_NONE },

    // [13] "Ivory"
    { .d = AFFIX_FLAGS_NONE },

    // [14] "Hobbit"
    { .d = AFFIX_FLAGS_NONE },

    // [15] "Uruk"
    { .d = AFFIX_FLAGS_NONE },

    // [16] "Troll Glass"
    { .d = AFFIX_FLAGS_NONE },

    // [17] "Sharded"
    { .d = AFFIX_FLAGS_NONE },

    // [18] "Bronze"
    { .d = AFFIX_FLAGS_NONE },

    // [19] "Heavy"
    { .d = AFFIX_FLAGS_NONE },

    // [20] "Silverstar"
    { .d = AFFIX_FLAGS_NONE },

    // [21] "Elven"
    { .d = AFFIX_FLAGS_NONE },

    // [22] "Speckled"
    { .d = AFFIX_FLAGS_NONE },

    // [23] "Glittering"
    { .d = AFFIX_FLAGS_NONE },

    // [24] "Spidersilver"
    { .d = AFFIX_FLAGS_NONE },

    // [25] "Spider's"
    { .d = AFFIX_FLAGS_NONE },

    // [26] "Ancient"
    { .d = AFFIX_FLAGS_NONE },

    // [27] "Forgotten"
    { .d = AFFIX_FLAGS_NONE },

    // [28] "Half-Eaten"
    { .d = AFFIX_FLAGS_NONE },

    // [29] "Bloody"
    { .d = AFFIX_FLAGS_NONE },

    // [30] "Webbed"
    { .d = AFFIX_FLAGS_NONE },

    // [31] "Venomous"
    { .d = AFFIX_FLAGS_NONE },

    // [32] "Dwarf Etched"
    { .d = AFFIX_FLAGS_NONE },

    // [33] "Old Dwarf's"
    { .d = AFFIX_FLAGS_NONE },

    // [34] "Mummified"
    { .d = AFFIX_FLAGS_NONE },

    // [35] "Spiderkin"
    { .d = AFFIX_FLAGS_NONE },

    // [36] "Trollbone"
    { .d = AFFIX_FLAGS_NONE },

    // [37] "Olog's"
    { .d = AFFIX_FLAGS_NONE },

    // [38] "Bloodstone"
    { .d = AFFIX_FLAGS_NONE },

    // [39] "Golden"
    { .d = AFFIX_FLAGS_NONE },

    // [40] "Opal"
    { .d = AFFIX_FLAGS_NONE },

    // [41] "Amethyst"
    { .d = AFFIX_FLAGS_NONE },

    // [42] "Jade"
    { .d = AFFIX_FLAGS_NONE },

    // [43] "Electrum"
    { .d = AFFIX_FLAGS_NONE },

    // [44] "Eldarstone"
    { .d = AFFIX_FLAGS_NONE },

    // [45] "Fellstone"
    { .d = AFFIX_FLAGS_NONE },

    // [46] "Nazgul's"
    { .d = AFFIX_FLAGS_NONE },

    // [47] "Haven Stone"
    { .d = AFFIX_FLAGS_NONE },

    // [48] "Platinum"
    { .d = AFFIX_FLAGS_NONE },

    // [49] "Ruby"
    { .d = AFFIX_FLAGS_NONE },

    // [50] "Emerald"
    { .d = AFFIX_FLAGS_NONE },

    // [51] "Darkstone"
    { .d = AFFIX_FLAGS_NONE },

    // [52] "Fossil"
    { .d = AFFIX_FLAGS_NONE },

    // [53] "Onyx"
    { .d = AFFIX_FLAGS_NONE },

    // [54] "Steel"
    { .d = AFFIX_FLAGS_NONE },

    // [55] "Gleaming"
    { .d = AFFIX_FLAGS_NONE },

    // [56] "Glowing"
    { .d = AFFIX_FLAGS_NONE },

    // [57] "Dragon's"
    { .d = AFFIX_FLAGS_NONE },

    // [58] "Tainted"
    { .d = AFFIX_FLAGS_NONE },

    // [59] "Isildur's"
    { .d = AFFIX_FLAGS_NONE },

    // [60] "Crystal"
    { .d = AFFIX_FLAGS_NONE },

    // [61] "Miriel"
    { .d = AFFIX_FLAGS_NONE },

    // [62] "Mithril"
    { .d = AFFIX_FLAGS_NONE },

    // [63] "Diamond"
    { .d = AFFIX_FLAGS_NONE },

    // [64] "Radiant"
    { .d = AFFIX_FLAGS_NONE },

    // [65] "Fiery"
    { .d = AFFIX_FLAGS_NONE },

    // [66] "Wraith Gem"
    { .d = AFFIX_FLAGS_NONE },

    // [67] "Blackrock"
    { .d = AFFIX_FLAGS_NONE },

    // [68] "Elendil's"
    { .d = AFFIX_FLAGS_NONE },

    // [69] "Gil-Galad's"
    { .d = AFFIX_FLAGS_NONE },

    // [70] "Terrible"
    { .d = AFFIX_FLAGS_NONE },

    // [71] "Doomrock"
    { .d = AFFIX_FLAGS_NONE },

    // [72] "Ugluk's"
    { .d = AFFIX_FLAGS_NONE },

    // [73] "Towerstone"
    { .d = AFFIX_FLAGS_NONE },

    // [74] "White Opal"
    { .d = AFFIX_FLAGS_NONE },

    // [75] "Eothain's"
    { .d = AFFIX_FLAGS_NONE },

    // [76] "Corroded"
    { .d = AFFIX_FLAGS_NONE },

    // [77] "Waterstone"
    { .d = AFFIX_FLAGS_NONE },

    // [78] "Lurtz's"
    { .d = AFFIX_FLAGS_NONE },

    // [79] "Palestone"
    { .d = AFFIX_FLAGS_NONE },

    // [80] "Gandalf's"
    { .d = AFFIX_FLAGS_NONE },

    // [81] "Ent Eye"
    { .d = AFFIX_FLAGS_NONE },

    // [82] "Arcane"
    { .d = { .minLevelPlus5B = 1 } },

    // [83] "Palantir-chip"
    { .d = AFFIX_FLAGS_NONE },

    // [84] "Morwen's"
    { .d = AFFIX_FLAGS_NONE },

    // [85] "Refugee's"
    { .d = AFFIX_FLAGS_NONE },

    // [86] "Beaded"
    { .d = AFFIX_FLAGS_NONE },

    // [87] "Gemstone"
    { .d = AFFIX_FLAGS_NONE },

    // [88] "Blue Sapphire"
    { .d = AFFIX_FLAGS_NONE },

    // [89] "Topaz"
    { .d = AFFIX_FLAGS_NONE },

    // [90] "Rare"
    { .d = AFFIX_FLAGS_NONE },

    // [91] "Fieldstone"
    { .d = AFFIX_FLAGS_NONE },

    // [92] "Eowyn's"
    { .d = AFFIX_FLAGS_NONE },

    // [93] "Starlit"
    { .d = AFFIX_FLAGS_NONE },

    // [94] "Tourmaline"
    { .d = { .minLevelPlus5A = 1 } },

    // [95] "Yellow Sapphire"
    { .d = AFFIX_FLAGS_NONE },

    // [96] "Spinel"
    { .d = AFFIX_FLAGS_NONE },

    // [97] "Iolite"
    { .d = { .minLevelPlus5A = 1 } },

    // [98] "White Sapphire"
    { .d = AFFIX_FLAGS_NONE },

    // [99] "Precious"
    { .d = AFFIX_FLAGS_NONE },

    // [100] "Silver"
    { .d = AFFIX_FLAGS_NONE },

    // [101] "Elfstone"
    { .d = AFFIX_FLAGS_NONE },

    // [102] "Azure"
    { .d = AFFIX_FLAGS_NONE },

    // [103] "Warrior's"
    { .d = AFFIX_FLAGS_NONE },

    // [104] "Cavalier's"
    { .d = AFFIX_FLAGS_NONE },

    // [105] "Princess"
    { .d = AFFIX_FLAGS_NONE },

    // [106] "Prismatic"
    { .d = AFFIX_FLAGS_NONE },

    // [107] "Elrond's"
    { .d = AFFIX_FLAGS_NONE },

    // [108] "Moonstone"
    { .d = AFFIX_FLAGS_NONE },

    // [109] "Smoky Topaz"
    { .d = AFFIX_FLAGS_NONE },

    // [110] "Amber"
    { .d = AFFIX_FLAGS_NONE },

    // [111] "Citrine"
    { .d = AFFIX_FLAGS_NONE },

    // [112] "Bloodveined"
    { .d = { .minLevelPlus5A = 1 } },

    // [113] "Peridot"
    { .d = AFFIX_FLAGS_NONE },

    // [114] "Orbed"
    { .d = AFFIX_FLAGS_NONE },

    // [115] "Brittle"
    { .d = AFFIX_FLAGS_NONE },

    // [116] "Glowstone"
    { .d = AFFIX_FLAGS_NONE },

    // [117] "Spectral"
    { .d = AFFIX_FLAGS_NONE },

    // [118] "Ghoststone"
    { .d = AFFIX_FLAGS_NONE },

    // [119] "Ethereal"
    { .d = AFFIX_FLAGS_NONE },

    // [120] "White Diamond"
    { .d = AFFIX_FLAGS_NONE },

    // [121] "Ice Gem"
    { .d = AFFIX_FLAGS_NONE },

    // [122] "Snow Crystal"
    { .d = { .minLevelPlus5B = 1 } },

    // [123] "Dwarf Maiden"
    { .d = AFFIX_FLAGS_NONE },

    // [124] "Crystalline"
    { .d = { .minLevelPlus5A = 1,
             .minLevelPlus5B = 1 } },

    // [125] "Polished"
    { .d = AFFIX_FLAGS_NONE },

    // [126] "Gold Dusted"
    { .d = AFFIX_FLAGS_NONE },

    // [127] "Shining"
    { .d = AFFIX_FLAGS_NONE },

    // [128] "Dwarf Diamond"
    { .d = AFFIX_FLAGS_NONE },

    // [129] "Snow Ruby"
    { .d = AFFIX_FLAGS_NONE },

    // [130] "Dazzling"
    { .d = AFFIX_FLAGS_NONE },

    // [131] "Dwarfruned"
    { .d = AFFIX_FLAGS_NONE },

    // [132] "Victory Stone"
    { .d = AFFIX_FLAGS_NONE },

    // [133] "Haradrim"
    { .d = AFFIX_FLAGS_NONE },

    // [134] "Flawless"
    { .d = AFFIX_FLAGS_NONE },

    // [135] "Southron's"
    { .d = AFFIX_FLAGS_NONE },

    // [136] "Comrade's"
    { .d = AFFIX_FLAGS_NONE },

    // [137] "Dull Adamant"
    { .d = AFFIX_FLAGS_NONE },

    // [138] "Tirith Rock"
    { .d = AFFIX_FLAGS_NONE },

    // [139] "Gondor Gold"
    { .d = AFFIX_FLAGS_NONE },

    // [140] "Lime Emerald"
    { .d = { .minLevelPlus5A = 1,
             .minLevelPlus5B = 1 } },

    // [141] "Denethor's"
    { .d = AFFIX_FLAGS_NONE },

    // [142] "King's Craft"
    { .d = AFFIX_FLAGS_NONE },

    // [143] "Aragorn's"
    { .d = AFFIX_FLAGS_NONE },

    // [144] "Black Diamond"
    { .d = AFFIX_FLAGS_NONE },

    // [145] "Twistwood"
    { .d = AFFIX_FLAGS_NONE },

    // [146] "Obsidian"
    { .d = AFFIX_FLAGS_NONE },

    // [147] "Magnificent"
    { .d = AFFIX_FLAGS_NONE },

    // [148] "Blackheart"
    { .d = AFFIX_FLAGS_NONE },

    // [149] "Peculiar"
    { .d = AFFIX_FLAGS_NONE },

    // [150] "Corrupt"
    { .d = AFFIX_FLAGS_NONE },

    // [151] "Dark Topaz"
    { .d = AFFIX_FLAGS_NONE },

    // [152] "Dark Sapphire"
    { .d = AFFIX_FLAGS_NONE },

    // [153] "Vein Crystal"
    { .d = AFFIX_FLAGS_NONE },

    // [154] "Necromancer's"
    { .d = { .minLevelPlus5B = 1 } },

    // [155] "Lordstone"
    { .d = AFFIX_FLAGS_NONE },
};
#undef AFFIX_FLAGS_NONE
// clang-format on
