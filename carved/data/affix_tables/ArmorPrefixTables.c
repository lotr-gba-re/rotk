#include "item.h"
#include "stats.h"
#include "text_ids.h"

// clang-format off


/** @romaddress 0x08066818 */
const ItemAffix ArmorPrefixes[156] = {
    // [0] "Shoddy": Defense -1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_SHODDY,
      .tier = 1,
      .stat0 = STAT_NONE,                  .val0 =   0,
      .stat1 = STAT_DEFENSE,               .val1 =  -1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_CHEAP,                 .val3 =   1 },

    // [1] "Cumbersome": Melee Armor +1, Accuracy -3
    { .nameId = TEXT_ID_PREFIX_CUMBERSOME,
      .tier = 1,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_ACCURACY,              .val1 =  -3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [2] "Goblin": Accuracy +1, Melee Armor -1
    { .nameId = TEXT_ID_PREFIX_GOBLIN,
      .tier = 1,
      .stat0 = STAT_ACCURACY,              .val0 =   1,
      .stat1 = STAT_MELEE_ARMOR,           .val1 =  -1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [3] "Black": Strength +1
    { .nameId = TEXT_ID_PREFIX_BLACK,
      .tier = 1,
      .stat0 = STAT_STRENGTH,              .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [4] "Thief's": Backstab +1, Courage -5
    { .nameId = TEXT_ID_PREFIX_THIEFS,
      .tier = 1,
      .stat0 = STAT_BACKSTAB,              .val0 =   1,
      .stat1 = STAT_COURAGE,               .val1 =  -5,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [5] "Hardened": Melee Armor +1
    { .nameId = TEXT_ID_PREFIX_HARDENED,
      .tier = 1,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [6] "Inferior": Defense -5, Cheap +1
    { .nameId = TEXT_ID_PREFIX_INFERIOR,
      .tier = 1,
      .stat0 = STAT_DEFENSE,               .val0 =  -5,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_CHEAP,                 .val3 =   1 },

    // [7] "Bloody": Hit Points +5
    { .nameId = TEXT_ID_PREFIX_BLOODY,
      .tier = 1,
      .stat0 = STAT_MAX_HP,                .val0 =   5,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [8] "Hero's": Melee Armor +1, Strength +1
    { .nameId = TEXT_ID_PREFIX_HEROS,
      .tier = 3,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [9] "Burglar's": Gemfinder +1, Backstab +1
    { .nameId = TEXT_ID_PREFIX_BURGLARS,
      .tier = 3,
      .stat0 = STAT_GEMFINDER,             .val0 =   1,
      .stat1 = STAT_BACKSTAB,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [10] "Smeagol's": Defense +2, Hit Points +15
    { .nameId = TEXT_ID_PREFIX_SMEAGOLS,
      .tier = 5,
      .stat0 = STAT_DEFENSE,               .val0 =   2,
      .stat1 = STAT_MAX_HP,                .val1 =  15,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [11] "Torturer's": % Critical +5, Accuracy +3
    { .nameId = TEXT_ID_PREFIX_TORTURERS,
      .tier = 5,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,   .val0 =   5,
      .stat1 = STAT_ACCURACY,              .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [12] "Thug's": Melee Armor +1, Hit Points +5
    { .nameId = TEXT_ID_PREFIX_THUGS,
      .tier = 2,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_MAX_HP,                .val1 =   5,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [13] "Ancient": Melee Armor +1
    { .nameId = TEXT_ID_PREFIX_ANCIENT,
      .tier = 2,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [14] "Studded": Melee Armor +1, Defense +1
    { .nameId = TEXT_ID_PREFIX_STUDDED,
      .tier = 2,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_DEFENSE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [15] "Brown": Defense +1
    { .nameId = TEXT_ID_PREFIX_BROWN,
      .tier = 2,
      .stat0 = STAT_DEFENSE,               .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [16] "Bladed": Damage +1
    { .nameId = TEXT_ID_PREFIX_BLADED,
      .tier = 2,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [17] "Miner's": Critical Armor +2
    { .nameId = TEXT_ID_PREFIX_MINERS,
      .tier = 2,
      .stat0 = STAT_CRITICAL_ARMOR,        .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [18] "Hunter's": Accuracy +1
    { .nameId = TEXT_ID_PREFIX_HUNTERS,
      .tier = 2,
      .stat0 = STAT_ACCURACY,              .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [19] "Raging": Accuracy -5, Strength +6, Melee Damage +1
    { .nameId = TEXT_ID_PREFIX_RAGING,
      .tier = 2,
      .stat0 = STAT_ACCURACY,              .val0 =  -5,
      .stat1 = STAT_STRENGTH,              .val1 =   6,
      .stat2 = STAT_DAMAGE_SLASH,          .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [20] "Mornie": All Primary Stats +1
    { .nameId = TEXT_ID_PREFIX_MORNIE,
      .tier = 4,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [21] "Crested": Courage +3, Valuable +1
    { .nameId = TEXT_ID_PREFIX_CRESTED,
      .tier = 4,
      .stat0 = STAT_COURAGE,               .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [22] "Warg Leader's": dmg to Animals +4, Strength +1
    { .nameId = TEXT_ID_PREFIX_WARG_LEADERS,
      .tier = 6,
      .stat0 = STAT_DAMAGE_TO_ANIMALS,     .val0 =   4,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [23] "Sergeant's": Strength +2, Accuracy +2, Defense +1
    { .nameId = TEXT_ID_PREFIX_SERGEANTS,
      .tier = 6,
      .stat0 = STAT_STRENGTH,              .val0 =   2,
      .stat1 = STAT_ACCURACY,              .val1 =   2,
      .stat2 = STAT_DEFENSE,               .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [24] "Fetid": Health -5, Strength +6, Defense +2
    { .nameId = TEXT_ID_PREFIX_FETID,
      .tier = 3,
      .stat0 = STAT_HEALTH,                .val0 =  -5,
      .stat1 = STAT_STRENGTH,              .val1 =   6,
      .stat2 = STAT_DEFENSE,               .val2 =   2,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [25] "Arachnid": Accuracy +1, Defense +1
    { .nameId = TEXT_ID_PREFIX_ARACHNID,
      .tier = 3,
      .stat0 = STAT_ACCURACY,              .val0 =   1,
      .stat1 = STAT_DEFENSE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [26] "Venomous": Hit Points -20, Damage +1, Venomous +1
    { .nameId = TEXT_ID_PREFIX_VENOMOUS,
      .tier = 3,
      .stat0 = STAT_MAX_HP,                .val0 = -20,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   1,
      .stat2 = STAT_VENOMOUS,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [27] "Putrid": Hit Points -20, All Primary Stats +1
    { .nameId = TEXT_ID_PREFIX_PUTRID,
      .tier = 3,
      .stat0 = STAT_MAX_HP,                .val0 = -20,
      .stat1 = STAT_ALL_PRIMARY_STATS,     .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [28] "Thorin's": Strength +2, Accuracy +1, Courage +1
    { .nameId = TEXT_ID_PREFIX_THORINS,
      .tier = 3,
      .stat0 = STAT_STRENGTH,              .val0 =   2,
      .stat1 = STAT_ACCURACY,              .val1 =   1,
      .stat2 = STAT_COURAGE,               .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [29] "Elven": Accuracy +2, Defense +1
    { .nameId = TEXT_ID_PREFIX_ELVEN,
      .tier = 3,
      .stat0 = STAT_ACCURACY,              .val0 =   2,
      .stat1 = STAT_DEFENSE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [30] "Green": % Poison Resistance +20, Melee Armor +1
    { .nameId = TEXT_ID_PREFIX_GREEN,
      .tier = 3,
      .stat0 = STAT_POISON_RESIST_PERCENT, .val0 =  20,
      .stat1 = STAT_MELEE_ARMOR,           .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [31] "Dwarf Etched": Melee Armor +1, Hit Points +1
    { .nameId = TEXT_ID_PREFIX_DWARF_ETCHED,
      .tier = 3,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_MAX_HP,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [32] "Opal": Wisdom +2
    { .nameId = TEXT_ID_PREFIX_OPAL,
      .tier = 5,
      .stat0 = STAT_WISDOM,                .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [33] "Vile": Melee Armor +1, Backstab +1, Nightburst +1, Cursed +1
    { .nameId = TEXT_ID_PREFIX_VILE,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_BACKSTAB,              .val1 =   1,
      .stat2 = STAT_NIGHTBURST,            .val2 =   1,
      .stat3 = STAT_CURSED,                .val3 =   1 },

    // [34] "Decomposing": Nightburst +1, Venomous +1, Health +1
    { .nameId = TEXT_ID_PREFIX_DECOMPOSING,
      .tier = 7,
      .stat0 = STAT_NIGHTBURST,            .val0 =   1,
      .stat1 = STAT_VENOMOUS,              .val1 =   1,
      .stat2 = STAT_HEALTH,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [35] "Mithril": Melee Armor +4
    { .nameId = TEXT_ID_PREFIX_MITHRIL,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   4,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [36] "Dark Fae": Nightburst +1
    { .nameId = TEXT_ID_PREFIX_DARK_FAE,
      .tier = 4,
      .stat0 = STAT_NIGHTBURST,            .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [37] "Olog's": Strength +2
    { .nameId = TEXT_ID_PREFIX_OLOGS,
      .tier = 4,
      .stat0 = STAT_STRENGTH,              .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [38] "Sooty": % Fear Resistance +10, Fire Protection +2
    { .nameId = TEXT_ID_PREFIX_SOOTY,
      .tier = 4,
      .stat0 = STAT_FEAR_RESIST_PERCENT,   .val0 =  10,
      .stat1 = STAT_FIRE_PROTECTION,       .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [39] "Maggot's": % Poison Resistance +50
    { .nameId = TEXT_ID_PREFIX_MAGGOTS,
      .tier = 4,
      .stat0 = STAT_POISON_RESIST_PERCENT, .val0 =  50,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [40] "Spiked": Damage +1
    { .nameId = TEXT_ID_PREFIX_SPIKED,
      .tier = 4,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [41] "Dunlending": Strength +1, Defense +1
    { .nameId = TEXT_ID_PREFIX_DUNLENDING,
      .tier = 4,
      .stat0 = STAT_STRENGTH,              .val0 =   1,
      .stat1 = STAT_DEFENSE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [42] "Half-Troll": Strength +3, Defense +1
    { .nameId = TEXT_ID_PREFIX_HALF_TROLL,
      .tier = 4,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_DEFENSE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [43] "Easterling": Defense +2, Health +1
    { .nameId = TEXT_ID_PREFIX_EASTERLING,
      .tier = 4,
      .stat0 = STAT_DEFENSE,               .val0 =   2,
      .stat1 = STAT_HEALTH,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [44] "Minion's": Hit Points +30
    { .nameId = TEXT_ID_PREFIX_MINIONS,
      .tier = 6,
      .stat0 = STAT_MAX_HP,                .val0 =  30,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [45] "Blood Eye": Spirit Points +10, Accuracy +3, Cursed +1
    { .nameId = TEXT_ID_PREFIX_BLOOD_EYE,
      .tier = 6,
      .stat0 = STAT_MAX_SPIRIT,            .val0 =  10,
      .stat1 = STAT_ACCURACY,              .val1 =   3,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [46] "Ringwraith's": Melee Armor +5, Cursed +1
    { .nameId = TEXT_ID_PREFIX_RINGWRAITHS,
      .tier = 8,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   5,
      .stat1 = STAT_CURSED,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [47] "Warrior's": Melee Armor +4, Strength +2, Health +1
    { .nameId = TEXT_ID_PREFIX_WARRIORS,
      .tier = 8,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   4,
      .stat1 = STAT_STRENGTH,              .val1 =   2,
      .stat2 = STAT_HEALTH,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [48] "Fire Red": Fire Protection +5, Strength +1
    { .nameId = TEXT_ID_PREFIX_FIRE_RED,
      .tier = 5,
      .stat0 = STAT_FIRE_PROTECTION,       .val0 =   5,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [49] "Jeweled": Melee Armor +2, Valuable +1
    { .nameId = TEXT_ID_PREFIX_JEWELED,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [50] "Prince's": Melee Armor +2, Courage +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_PRINCES,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   2,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [51] "Coal Black": Melee Armor +1, Fire Protection +4, Health +1
    { .nameId = TEXT_ID_PREFIX_COAL_BLACK,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_FIRE_PROTECTION,       .val1 =   4,
      .stat2 = STAT_HEALTH,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [52] "Spotted": Defense +3
    { .nameId = TEXT_ID_PREFIX_SPOTTED,
      .tier = 5,
      .stat0 = STAT_DEFENSE,               .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [53] "Amber": Hit Points +15, Wisdom +3
    { .nameId = TEXT_ID_PREFIX_AMBER,
      .tier = 5,
      .stat0 = STAT_MAX_HP,                .val0 =  15,
      .stat1 = STAT_WISDOM,                .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [54] "Forgotten": All Primary Stats +1
    { .nameId = TEXT_ID_PREFIX_FORGOTTEN,
      .tier = 5,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [55] "Smith's": Melee Armor +3
    { .nameId = TEXT_ID_PREFIX_SMITHS,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [56] "Gil-Galad's": All Primary Stats +1, Melee Armor +3
    { .nameId = TEXT_ID_PREFIX_GIL_GALADS,
      .tier = 7,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_MELEE_ARMOR,           .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [57] "Healthy": Health +3, Hit Points +35
    { .nameId = TEXT_ID_PREFIX_HEALTHY,
      .tier = 7,
      .stat0 = STAT_HEALTH,                .val0 =   3,
      .stat1 = STAT_MAX_HP,                .val1 =  35,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [58] "Corrupt": Hit Points +60, dmg to Orcs +4, Cursed +1
    { .nameId = TEXT_ID_PREFIX_CORRUPT,
      .tier = 9,
      .stat0 = STAT_MAX_HP,                .val0 =  60,
      .stat1 = STAT_DAMAGE_TO_ORCS,        .val1 =   4,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [59] "Hearty": Hit Points +90
    { .nameId = TEXT_ID_PREFIX_HEARTY,
      .tier = 9,
      .stat0 = STAT_MAX_HP,                .val0 =  90,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [60] "Crystalline": All Primary Stats +1, Defense +1
    { .nameId = TEXT_ID_PREFIX_CRYSTALLINE,
      .tier = 6,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_DEFENSE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [61] "Magma": Strength +2, Melee Armor +2
    { .nameId = TEXT_ID_PREFIX_MAGMA,
      .tier = 6,
      .stat0 = STAT_STRENGTH,              .val0 =   2,
      .stat1 = STAT_MELEE_ARMOR,           .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [62] "Slayer's": % Critical +5, Damage +1
    { .nameId = TEXT_ID_PREFIX_SLAYERS,
      .tier = 6,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,   .val0 =   5,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [63] "Flawless": Melee Armor +4, Valuable +1
    { .nameId = TEXT_ID_PREFIX_FLAWLESS,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   4,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [64] "Shadowed": Melee Armor +1, Defense +4
    { .nameId = TEXT_ID_PREFIX_SHADOWED,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_DEFENSE,               .val1 =   4,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [65] "Antique": Melee Armor +3, Defense +2, Valuable +1
    { .nameId = TEXT_ID_PREFIX_ANTIQUE,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   3,
      .stat1 = STAT_DEFENSE,               .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [66] "Molten": Fire Protection +6, Spirit Points +10
    { .nameId = TEXT_ID_PREFIX_MOLTEN,
      .tier = 6,
      .stat0 = STAT_FIRE_PROTECTION,       .val0 =   6,
      .stat1 = STAT_MAX_SPIRIT,            .val1 =  10,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [67] "Fell": All Primary Stats +2, Hit Points +15, Cursed +1
    { .nameId = TEXT_ID_PREFIX_FELL,
      .tier = 6,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   2,
      .stat1 = STAT_MAX_HP,                .val1 =  15,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [68] "Elf King's": Wisdom +4, Courage +3
    { .nameId = TEXT_ID_PREFIX_ELF_KINGS,
      .tier = 8,
      .stat0 = STAT_WISDOM,                .val0 =   4,
      .stat1 = STAT_COURAGE,               .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [69] "Elrond's": Strength +3, Courage +3, Defense +3
    { .nameId = TEXT_ID_PREFIX_ELRONDS,
      .tier = 8,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_COURAGE,               .val1 =   3,
      .stat2 = STAT_DEFENSE,               .val2 =   3,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [70] "Isildur's": Strength +4, Defense +3, Defense +9
    { .nameId = TEXT_ID_PREFIX_ISILDURS,
      .tier = 10,
      .stat0 = STAT_STRENGTH,              .val0 =   4,
      .stat1 = STAT_DEFENSE,               .val1 =   3,
      .stat2 = STAT_DEFENSE,               .val2 =   9,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [71] "Elendil's": Health +5, Defense +2, Accuracy +8
    { .nameId = TEXT_ID_PREFIX_ELENDILS,
      .tier = 10,
      .stat0 = STAT_HEALTH,                .val0 =   5,
      .stat1 = STAT_DEFENSE,               .val1 =   2,
      .stat2 = STAT_ACCURACY,              .val2 =   8,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [72] "Uruk": Strength +1
    { .nameId = TEXT_ID_PREFIX_URUK,
      .tier = 1,
      .stat0 = STAT_STRENGTH,              .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [73] "Oversized": Melee Armor +2, Strength -3
    { .nameId = TEXT_ID_PREFIX_OVERSIZED,
      .tier = 1,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   2,
      .stat1 = STAT_STRENGTH,              .val1 =  -3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [74] "Sloppy": Melee Armor +1, Defense -3, Cheap +1
    { .nameId = TEXT_ID_PREFIX_SLOPPY,
      .tier = 1,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_DEFENSE,               .val1 =  -3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_CHEAP,                 .val3 =   1 },

    // [75] "Smelly": Defense +1, Accuracy -1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_SMELLY,
      .tier = 1,
      .stat0 = STAT_DEFENSE,               .val0 =   1,
      .stat1 = STAT_ACCURACY,              .val1 =  -1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_CHEAP,                 .val3 =   1 },

    // [76] "Oiled": Defense +1
    { .nameId = TEXT_ID_PREFIX_OILED,
      .tier = 1,
      .stat0 = STAT_DEFENSE,               .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [77] "Gold Trimmed": Valuable +1
    { .nameId = TEXT_ID_PREFIX_GOLD_TRIMMED,
      .tier = 1,
      .stat0 = STAT_NONE,                  .val0 =   0,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [78] "Ruddy": Health +1
    { .nameId = TEXT_ID_PREFIX_RUDDY,
      .tier = 1,
      .stat0 = STAT_HEALTH,                .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [79] "Sharku's": Hit Points +5
    { .nameId = TEXT_ID_PREFIX_SHARKUS,
      .tier = 1,
      .stat0 = STAT_MAX_HP,                .val0 =   5,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [80] "Ugluk's": Health +2
    { .nameId = TEXT_ID_PREFIX_UGLUKS,
      .tier = 3,
      .stat0 = STAT_HEALTH,                .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [81] "Grishnakh's": Strength +1, Health +1
    { .nameId = TEXT_ID_PREFIX_GRISHNAKHS,
      .tier = 3,
      .stat0 = STAT_STRENGTH,              .val0 =   1,
      .stat1 = STAT_HEALTH,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [82] "Arcane": Wisdom +3, Courage +1
    { .nameId = TEXT_ID_PREFIX_ARCANE,
      .tier = 5,
      .stat0 = STAT_WISDOM,                .val0 =   3,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [83] "Lurtz's": Health +1, Strength +1, Defense +1
    { .nameId = TEXT_ID_PREFIX_LURTZS,
      .tier = 5,
      .stat0 = STAT_HEALTH,                .val0 =   1,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_DEFENSE,               .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [84] "Rohirrim": Defense +1
    { .nameId = TEXT_ID_PREFIX_ROHIRRIM,
      .tier = 2,
      .stat0 = STAT_DEFENSE,               .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [85] "Reinforced": Melee Armor +1, Defense +1
    { .nameId = TEXT_ID_PREFIX_REINFORCED,
      .tier = 2,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_DEFENSE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [86] "Armourer's": Melee Armor +2
    { .nameId = TEXT_ID_PREFIX_ARMOURERS,
      .tier = 2,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [87] "Cavalry": Melee Armor +1, Accuracy +1
    { .nameId = TEXT_ID_PREFIX_CAVALRY,
      .tier = 2,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_ACCURACY,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [88] "Maiden's": Defense +1, Courage +1
    { .nameId = TEXT_ID_PREFIX_MAIDENS,
      .tier = 2,
      .stat0 = STAT_DEFENSE,               .val0 =   1,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [89] "Madril's": Melee Armor +1, Strength +1
    { .nameId = TEXT_ID_PREFIX_MADRILS,
      .tier = 2,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [90] "Worn": Defense +1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_WORN,
      .tier = 2,
      .stat0 = STAT_DEFENSE,               .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_CHEAP,                 .val3 =   1 },

    // [91] "Bull's": Health +1, Strength +1
    { .nameId = TEXT_ID_PREFIX_BULLS,
      .tier = 2,
      .stat0 = STAT_HEALTH,                .val0 =   1,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [92] "Eomer's": Health +2, Strength +2, Melee Armor +1
    { .nameId = TEXT_ID_PREFIX_EOMERS,
      .tier = 4,
      .stat0 = STAT_HEALTH,                .val0 =   2,
      .stat1 = STAT_STRENGTH,              .val1 =   2,
      .stat2 = STAT_MELEE_ARMOR,           .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [93] "Eowyn's": Accuracy +2, Defense +2, Melee Armor +1
    { .nameId = TEXT_ID_PREFIX_EOWYNS,
      .tier = 4,
      .stat0 = STAT_ACCURACY,              .val0 =   2,
      .stat1 = STAT_DEFENSE,               .val1 =   2,
      .stat2 = STAT_MELEE_ARMOR,           .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [94] "Fearless": Strength +1, Defense +3, % Fear Resistance +30
    { .nameId = TEXT_ID_PREFIX_FEARLESS,
      .tier = 6,
      .stat0 = STAT_STRENGTH,              .val0 =   1,
      .stat1 = STAT_DEFENSE,               .val1 =   3,
      .stat2 = STAT_FEAR_RESIST_PERCENT,   .val2 =  30,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [95] "Brush Lord's": % Speed +10, Defense +1
    { .nameId = TEXT_ID_PREFIX_BRUSH_LORDS,
      .tier = 6,
      .stat0 = STAT_SPEED_PERCENT,         .val0 =  10,
      .stat1 = STAT_DEFENSE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [96] "Gatekeeper's": Defense +2
    { .nameId = TEXT_ID_PREFIX_GATEKEEPERS,
      .tier = 3,
      .stat0 = STAT_DEFENSE,               .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [97] "Slaver's": Critical Armor +4
    { .nameId = TEXT_ID_PREFIX_SLAVERS,
      .tier = 3,
      .stat0 = STAT_CRITICAL_ARMOR,        .val0 =   4,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [98] "Guardsman's": Melee Armor +2
    { .nameId = TEXT_ID_PREFIX_GUARDSMANS,
      .tier = 3,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [99] "Bladesman's": Damage +1, Melee Armor +1
    { .nameId = TEXT_ID_PREFIX_BLADESMANS,
      .tier = 3,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_MELEE_ARMOR,           .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [100] "Heavy": Melee Armor +2, Critical Armor +2
    { .nameId = TEXT_ID_PREFIX_HEAVY,
      .tier = 3,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   2,
      .stat1 = STAT_CRITICAL_ARMOR,        .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [101] "Stealthy": Defense +2, Backstab +1
    { .nameId = TEXT_ID_PREFIX_STEALTHY,
      .tier = 3,
      .stat0 = STAT_DEFENSE,               .val0 =   2,
      .stat1 = STAT_BACKSTAB,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [102] "Baron's": All Primary Stats +1, Melee Armor -1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_BARONS,
      .tier = 3,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_MELEE_ARMOR,           .val1 =  -1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [103] "Expert": Accuracy +3
    { .nameId = TEXT_ID_PREFIX_EXPERT,
      .tier = 3,
      .stat0 = STAT_ACCURACY,              .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [104] "Fortune's": Courage +3, Gemfinder +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_FORTUNES,
      .tier = 5,
      .stat0 = STAT_COURAGE,               .val0 =   3,
      .stat1 = STAT_GEMFINDER,             .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [105] "Glimmering": Defense +3, Melee Armor +1
    { .nameId = TEXT_ID_PREFIX_GLIMMERING,
      .tier = 5,
      .stat0 = STAT_DEFENSE,               .val0 =   3,
      .stat1 = STAT_MELEE_ARMOR,           .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [106] "Mithrilstone": Melee Armor +3, Defense +3, Valuable +1
    { .nameId = TEXT_ID_PREFIX_MITHRILSTONE,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   3,
      .stat1 = STAT_DEFENSE,               .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [107] "Decorated": Courage +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_DECORATED,
      .tier = 7,
      .stat0 = STAT_COURAGE,               .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [108] "Surveyor's": Accuracy +2, Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_SURVEYORS,
      .tier = 4,
      .stat0 = STAT_ACCURACY,              .val0 =   2,
      .stat1 = STAT_GEMFINDER,             .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [109] "Wolfskin": Melee Armor +1, Missile Armor +3, Health +1
    { .nameId = TEXT_ID_PREFIX_WOLFSKIN,
      .tier = 4,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_MISSILE_ARMOR,         .val1 =   3,
      .stat2 = STAT_HEALTH,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [110] "Spectral": Nightburst +1, Defense +2
    { .nameId = TEXT_ID_PREFIX_SPECTRAL,
      .tier = 4,
      .stat0 = STAT_NIGHTBURST,            .val0 =   1,
      .stat1 = STAT_DEFENSE,               .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [111] "Wisp's": Backstab +1, Sunburst +1, Defense +1
    { .nameId = TEXT_ID_PREFIX_WISPS,
      .tier = 4,
      .stat0 = STAT_BACKSTAB,              .val0 =   1,
      .stat1 = STAT_SUNBURST,              .val1 =   1,
      .stat2 = STAT_DEFENSE,               .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [112] "Dark": Health +3, Strength +2, Cursed +1
    { .nameId = TEXT_ID_PREFIX_DARK,
      .tier = 4,
      .stat0 = STAT_HEALTH,                .val0 =   3,
      .stat1 = STAT_STRENGTH,              .val1 =   2,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [113] "Dull Grey": Courage +1, Strength +1, Wisdom +1
    { .nameId = TEXT_ID_PREFIX_DULL_GREY,
      .tier = 4,
      .stat0 = STAT_COURAGE,               .val0 =   1,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_WISDOM,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [114] "Raven's": Spirit Regen +1, Wisdom +2
    { .nameId = TEXT_ID_PREFIX_RAVENS,
      .tier = 4,
      .stat0 = STAT_SPIRIT_REGEN,          .val0 =   1,
      .stat1 = STAT_WISDOM,                .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [115] "Ghoulish": Melee Armor +3, Cursed +1
    { .nameId = TEXT_ID_PREFIX_GHOULISH,
      .tier = 4,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   3,
      .stat1 = STAT_CURSED,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [116] "Cavemaster": Strength +3, Melee Armor +1
    { .nameId = TEXT_ID_PREFIX_CAVEMASTER,
      .tier = 6,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_MELEE_ARMOR,           .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [117] "Dead Hero's": Melee Armor -3, Health +7
    { .nameId = TEXT_ID_PREFIX_DEAD_HEROS,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =  -3,
      .stat1 = STAT_HEALTH,                .val1 =   7,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [118] "Radiant": % Fear Resistance +50, % Poison Resistance +50, All Primary Stats +1
    { .nameId = TEXT_ID_PREFIX_RADIANT,
      .tier = 8,
      .stat0 = STAT_FEAR_RESIST_PERCENT,   .val0 =  50,
      .stat1 = STAT_POISON_RESIST_PERCENT, .val1 =  50,
      .stat2 = STAT_ALL_PRIMARY_STATS,     .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [119] "Ethereal": Defense +4, Courage +1
    { .nameId = TEXT_ID_PREFIX_ETHEREAL,
      .tier = 8,
      .stat0 = STAT_DEFENSE,               .val0 =   4,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [120] "Coral": % Poison Resistance +40, Melee Armor +2
    { .nameId = TEXT_ID_PREFIX_CORAL,
      .tier = 5,
      .stat0 = STAT_POISON_RESIST_PERCENT, .val0 =  40,
      .stat1 = STAT_MELEE_ARMOR,           .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [121] "Sailor's": Accuracy +2, Defense +1
    { .nameId = TEXT_ID_PREFIX_SAILORS,
      .tier = 5,
      .stat0 = STAT_ACCURACY,              .val0 =   2,
      .stat1 = STAT_DEFENSE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [122] "Pirate's": Health +3, Strength +1, Accuracy +1
    { .nameId = TEXT_ID_PREFIX_PIRATES,
      .tier = 5,
      .stat0 = STAT_HEALTH,                .val0 =   3,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_ACCURACY,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [123] "Raftsman's": Strength +4
    { .nameId = TEXT_ID_PREFIX_RAFTSMANS,
      .tier = 5,
      .stat0 = STAT_STRENGTH,              .val0 =   4,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [124] "Aquamarine": Melee Armor +1, Wisdom +2
    { .nameId = TEXT_ID_PREFIX_AQUAMARINE,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_WISDOM,                .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [125] "Fishscale": Melee Armor +1, Missile Armor +2
    { .nameId = TEXT_ID_PREFIX_FISHSCALE,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_MISSILE_ARMOR,         .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [126] "Captain's": Melee Armor +2, Courage +1
    { .nameId = TEXT_ID_PREFIX_CAPTAINS,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   2,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [127] "Shipman's": Melee Armor +1, Hit Points +15
    { .nameId = TEXT_ID_PREFIX_SHIPMANS,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_MAX_HP,                .val1 =  15,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [128] "Scurvy": % Poison Resistance +30, Health +4
    { .nameId = TEXT_ID_PREFIX_SCURVY,
      .tier = 7,
      .stat0 = STAT_POISON_RESIST_PERCENT, .val0 =  30,
      .stat1 = STAT_HEALTH,                .val1 =   4,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [129] "Custom": Melee Armor +3, Defense +1, Accuracy +1
    { .nameId = TEXT_ID_PREFIX_CUSTOM,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   3,
      .stat1 = STAT_DEFENSE,               .val1 =   1,
      .stat2 = STAT_ACCURACY,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [130] "Adamant": Melee Armor +9, Strength +6
    { .nameId = TEXT_ID_PREFIX_ADAMANT,
      .tier = 9,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   9,
      .stat1 = STAT_STRENGTH,              .val1 =   6,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [131] "Sharkskin": Strength +3, Melee Armor +2, Missile Armor +1
    { .nameId = TEXT_ID_PREFIX_SHARKSKIN,
      .tier = 9,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_MELEE_ARMOR,           .val1 =   2,
      .stat2 = STAT_MISSILE_ARMOR,         .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [132] "Frozen": Melee Armor +1, Fire Protection +8
    { .nameId = TEXT_ID_PREFIX_FROZEN,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_FIRE_PROTECTION,       .val1 =   8,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [133] "Toughened": Melee Armor +3
    { .nameId = TEXT_ID_PREFIX_TOUGHENED,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [134] "Burly": Melee Armor +3, Strength +1
    { .nameId = TEXT_ID_PREFIX_BURLY,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   3,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [135] "Snow Grey": Defense +4
    { .nameId = TEXT_ID_PREFIX_SNOW_GREY,
      .tier = 6,
      .stat0 = STAT_DEFENSE,               .val0 =   4,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [136] "Camouflage": Melee Armor +2, Defense +1, Sunburst +1
    { .nameId = TEXT_ID_PREFIX_CAMOUFLAGE,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   2,
      .stat1 = STAT_DEFENSE,               .val1 =   1,
      .stat2 = STAT_SUNBURST,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [137] "Extra Heavy": Melee Armor +4, Strength +2, % Speed -5
    { .nameId = TEXT_ID_PREFIX_EXTRA_HEAVY,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   4,
      .stat1 = STAT_STRENGTH,              .val1 =   2,
      .stat2 = STAT_SPEED_PERCENT,         .val2 =  -5,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [138] "Glacial": Melee Armor +2, Fire Protection +6, Defense +1
    { .nameId = TEXT_ID_PREFIX_GLACIAL,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   2,
      .stat1 = STAT_FIRE_PROTECTION,       .val1 =   6,
      .stat2 = STAT_DEFENSE,               .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [139] "Balin's": Strength +4, Courage +1
    { .nameId = TEXT_ID_PREFIX_BALINS,
      .tier = 6,
      .stat0 = STAT_STRENGTH,              .val0 =   4,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [140] "Polar": Melee Armor +3, Fire Protection +5, Strength +1
    { .nameId = TEXT_ID_PREFIX_POLAR,
      .tier = 8,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   3,
      .stat1 = STAT_FIRE_PROTECTION,       .val1 =   5,
      .stat2 = STAT_STRENGTH,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [141] "White Runed": Accuracy +5
    { .nameId = TEXT_ID_PREFIX_WHITE_RUNED,
      .tier = 8,
      .stat0 = STAT_ACCURACY,              .val0 =   5,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [142] "Dwarf Butcher": Health +7, Strength +1
    { .nameId = TEXT_ID_PREFIX_DWARF_BUTCHER,
      .tier = 10,
      .stat0 = STAT_HEALTH,                .val0 =   7,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [143] "Massive": Melee Armor +5, Strength +4
    { .nameId = TEXT_ID_PREFIX_MASSIVE,
      .tier = 10,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   5,
      .stat1 = STAT_STRENGTH,              .val1 =   4,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [144] "Courageous": Strength +2, Courage +3, Melee Armor +5
    { .nameId = TEXT_ID_PREFIX_COURAGEOUS,
      .tier = 7,
      .stat0 = STAT_STRENGTH,              .val0 =   2,
      .stat1 = STAT_COURAGE,               .val1 =   3,
      .stat2 = STAT_MELEE_ARMOR,           .val2 =   5,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [145] "Elf Runed": Defense +5, Melee Armor +1
    { .nameId = TEXT_ID_PREFIX_ELF_RUNED,
      .tier = 7,
      .stat0 = STAT_DEFENSE,               .val0 =   5,
      .stat1 = STAT_MELEE_ARMOR,           .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [146] "Beryl": Defense +1, Melee Armor +4, Valuable +1
    { .nameId = TEXT_ID_PREFIX_BERYL,
      .tier = 7,
      .stat0 = STAT_DEFENSE,               .val0 =   1,
      .stat1 = STAT_MELEE_ARMOR,           .val1 =   4,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [147] "Artisan's": Melee Armor +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_ARTISANS,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [148] "Noble": Melee Armor +3, Courage +2, Valuable +1
    { .nameId = TEXT_ID_PREFIX_NOBLE,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   3,
      .stat1 = STAT_COURAGE,               .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [149] "Strengthened": Melee Armor +4, Strength +2
    { .nameId = TEXT_ID_PREFIX_STRENGTHENED,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   4,
      .stat1 = STAT_STRENGTH,              .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [150] "Fine Runed": Melee Armor +5
    { .nameId = TEXT_ID_PREFIX_FINE_RUNED,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   5,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [151] "Rearguard's": Melee Armor +4, Courage +3
    { .nameId = TEXT_ID_PREFIX_REARGUARDS,
      .tier = 7,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   4,
      .stat1 = STAT_COURAGE,               .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [152] "Fine Elven": Melee Armor +6, Defense +2
    { .nameId = TEXT_ID_PREFIX_FINE_ELVEN,
      .tier = 9,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   6,
      .stat1 = STAT_DEFENSE,               .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [153] "Denethor's": Courage -5, Strength +5, Melee Armor +3, Accuracy +4
    { .nameId = TEXT_ID_PREFIX_DENETHORS,
      .tier = 9,
      .stat0 = STAT_COURAGE,               .val0 =  -5,
      .stat1 = STAT_STRENGTH,              .val1 =   5,
      .stat2 = STAT_MELEE_ARMOR,           .val2 =   3,
      .stat3 = STAT_ACCURACY,              .val3 =   4 },

    // [154] "Heroic": Courage +5, Strength +5, Health +5
    { .nameId = TEXT_ID_PREFIX_HEROIC,
      .tier = 11,
      .stat0 = STAT_COURAGE,               .val0 =   5,
      .stat1 = STAT_STRENGTH,              .val1 =   5,
      .stat2 = STAT_HEALTH,                .val2 =   5,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [155] "Boromir's": Melee Armor +5, Accuracy +1, Hit Points +100
    { .nameId = TEXT_ID_PREFIX_BOROMIRS,
      .tier = 11,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   5,
      .stat1 = STAT_ACCURACY,              .val1 =   1,
      .stat2 = STAT_MAX_HP,                .val2 = 100,
      .stat3 = STAT_NONE,                  .val3 =   0 },
};

/** no decoded bits set (155 of 156 entries) */
#define AFFIX_FLAGS_NONE { 0 }

/**
 * Per-affix flag table; indexed by the affix index. Decoded bits: AFFIX_FLAG_* (item.h).
 *
 * @romaddress 0x08066f68
 */
const AffixFlags ArmorPrefixFlags[156] = {
    // [0] "Shoddy"
    { .d = AFFIX_FLAGS_NONE },

    // [1] "Cumbersome"
    { .d = AFFIX_FLAGS_NONE },

    // [2] "Goblin"
    { .d = AFFIX_FLAGS_NONE },

    // [3] "Black"
    { .d = AFFIX_FLAGS_NONE },

    // [4] "Thief's"
    { .d = AFFIX_FLAGS_NONE },

    // [5] "Hardened"
    { .d = AFFIX_FLAGS_NONE },

    // [6] "Inferior"
    { .d = AFFIX_FLAGS_NONE },

    // [7] "Bloody"
    { .d = AFFIX_FLAGS_NONE },

    // [8] "Hero's"
    { .d = AFFIX_FLAGS_NONE },

    // [9] "Burglar's"
    { .d = AFFIX_FLAGS_NONE },

    // [10] "Smeagol's"
    { .d = AFFIX_FLAGS_NONE },

    // [11] "Torturer's"
    { .d = AFFIX_FLAGS_NONE },

    // [12] "Thug's"
    { .d = AFFIX_FLAGS_NONE },

    // [13] "Ancient"
    { .d = AFFIX_FLAGS_NONE },

    // [14] "Studded"
    { .d = AFFIX_FLAGS_NONE },

    // [15] "Brown"
    { .d = AFFIX_FLAGS_NONE },

    // [16] "Bladed"
    { .d = AFFIX_FLAGS_NONE },

    // [17] "Miner's"
    { .d = AFFIX_FLAGS_NONE },

    // [18] "Hunter's"
    { .d = AFFIX_FLAGS_NONE },

    // [19] "Raging"
    { .d = AFFIX_FLAGS_NONE },

    // [20] "Mornie"
    { .d = AFFIX_FLAGS_NONE },

    // [21] "Crested"
    { .d = AFFIX_FLAGS_NONE },

    // [22] "Warg Leader's"
    { .d = AFFIX_FLAGS_NONE },

    // [23] "Sergeant's"
    { .d = AFFIX_FLAGS_NONE },

    // [24] "Fetid"
    { .d = AFFIX_FLAGS_NONE },

    // [25] "Arachnid"
    { .d = AFFIX_FLAGS_NONE },

    // [26] "Venomous"
    { .d = AFFIX_FLAGS_NONE },

    // [27] "Putrid"
    { .d = AFFIX_FLAGS_NONE },

    // [28] "Thorin's"
    { .d = AFFIX_FLAGS_NONE },

    // [29] "Elven"
    { .d = AFFIX_FLAGS_NONE },

    // [30] "Green"
    { .d = AFFIX_FLAGS_NONE },

    // [31] "Dwarf Etched"
    { .d = AFFIX_FLAGS_NONE },

    // [32] "Opal"
    { .d = AFFIX_FLAGS_NONE },

    // [33] "Vile"
    { .d = AFFIX_FLAGS_NONE },

    // [34] "Decomposing"
    { .d = AFFIX_FLAGS_NONE },

    // [35] "Mithril"
    { .d = AFFIX_FLAGS_NONE },

    // [36] "Dark Fae"
    { .d = AFFIX_FLAGS_NONE },

    // [37] "Olog's"
    { .d = AFFIX_FLAGS_NONE },

    // [38] "Sooty"
    { .d = AFFIX_FLAGS_NONE },

    // [39] "Maggot's"
    { .d = AFFIX_FLAGS_NONE },

    // [40] "Spiked"
    { .d = AFFIX_FLAGS_NONE },

    // [41] "Dunlending"
    { .d = AFFIX_FLAGS_NONE },

    // [42] "Half-Troll"
    { .d = AFFIX_FLAGS_NONE },

    // [43] "Easterling"
    { .d = AFFIX_FLAGS_NONE },

    // [44] "Minion's"
    { .d = AFFIX_FLAGS_NONE },

    // [45] "Blood Eye"
    { .d = AFFIX_FLAGS_NONE },

    // [46] "Ringwraith's"
    { .d = AFFIX_FLAGS_NONE },

    // [47] "Warrior's"
    { .d = AFFIX_FLAGS_NONE },

    // [48] "Fire Red"
    { .d = AFFIX_FLAGS_NONE },

    // [49] "Jeweled"
    { .d = AFFIX_FLAGS_NONE },

    // [50] "Prince's"
    { .d = AFFIX_FLAGS_NONE },

    // [51] "Coal Black"
    { .d = AFFIX_FLAGS_NONE },

    // [52] "Spotted"
    { .d = AFFIX_FLAGS_NONE },

    // [53] "Amber"
    { .d = AFFIX_FLAGS_NONE },

    // [54] "Forgotten"
    { .d = AFFIX_FLAGS_NONE },

    // [55] "Smith's"
    { .d = AFFIX_FLAGS_NONE },

    // [56] "Gil-Galad's"
    { .d = AFFIX_FLAGS_NONE },

    // [57] "Healthy"
    { .d = AFFIX_FLAGS_NONE },

    // [58] "Corrupt"
    { .d = AFFIX_FLAGS_NONE },

    // [59] "Hearty"
    { .d = AFFIX_FLAGS_NONE },

    // [60] "Crystalline"
    { .d = AFFIX_FLAGS_NONE },

    // [61] "Magma"
    { .d = AFFIX_FLAGS_NONE },

    // [62] "Slayer's"
    { .d = AFFIX_FLAGS_NONE },

    // [63] "Flawless"
    { .d = AFFIX_FLAGS_NONE },

    // [64] "Shadowed"
    { .d = AFFIX_FLAGS_NONE },

    // [65] "Antique"
    { .d = AFFIX_FLAGS_NONE },

    // [66] "Molten"
    { .d = AFFIX_FLAGS_NONE },

    // [67] "Fell"
    { .d = AFFIX_FLAGS_NONE },

    // [68] "Elf King's"
    { .d = AFFIX_FLAGS_NONE },

    // [69] "Elrond's"
    { .d = AFFIX_FLAGS_NONE },

    // [70] "Isildur's"
    { .d = AFFIX_FLAGS_NONE },

    // [71] "Elendil's"
    { .d = AFFIX_FLAGS_NONE },

    // [72] "Uruk"
    { .d = AFFIX_FLAGS_NONE },

    // [73] "Oversized"
    { .d = AFFIX_FLAGS_NONE },

    // [74] "Sloppy"
    { .d = AFFIX_FLAGS_NONE },

    // [75] "Smelly"
    { .d = AFFIX_FLAGS_NONE },

    // [76] "Oiled"
    { .d = AFFIX_FLAGS_NONE },

    // [77] "Gold Trimmed"
    { .d = AFFIX_FLAGS_NONE },

    // [78] "Ruddy"
    { .d = AFFIX_FLAGS_NONE },

    // [79] "Sharku's"
    { .d = AFFIX_FLAGS_NONE },

    // [80] "Ugluk's"
    { .d = AFFIX_FLAGS_NONE },

    // [81] "Grishnakh's"
    { .d = AFFIX_FLAGS_NONE },

    // [82] "Arcane"
    { .d = AFFIX_FLAGS_NONE },

    // [83] "Lurtz's"
    { .d = AFFIX_FLAGS_NONE },

    // [84] "Rohirrim"
    { .d = AFFIX_FLAGS_NONE },

    // [85] "Reinforced"
    { .d = AFFIX_FLAGS_NONE },

    // [86] "Armourer's"
    { .d = AFFIX_FLAGS_NONE },

    // [87] "Cavalry"
    { .d = AFFIX_FLAGS_NONE },

    // [88] "Maiden's"
    { .d = AFFIX_FLAGS_NONE },

    // [89] "Madril's"
    { .d = AFFIX_FLAGS_NONE },

    // [90] "Worn"
    { .d = AFFIX_FLAGS_NONE },

    // [91] "Bull's"
    { .d = AFFIX_FLAGS_NONE },

    // [92] "Eomer's"
    { .d = AFFIX_FLAGS_NONE },

    // [93] "Eowyn's"
    { .d = AFFIX_FLAGS_NONE },

    // [94] "Fearless"
    { .d = AFFIX_FLAGS_NONE },

    // [95] "Brush Lord's"
    { .d = AFFIX_FLAGS_NONE },

    // [96] "Gatekeeper's"
    { .d = AFFIX_FLAGS_NONE },

    // [97] "Slaver's"
    { .d = AFFIX_FLAGS_NONE },

    // [98] "Guardsman's"
    { .d = AFFIX_FLAGS_NONE },

    // [99] "Bladesman's"
    { .d = AFFIX_FLAGS_NONE },

    // [100] "Heavy"
    { .d = AFFIX_FLAGS_NONE },

    // [101] "Stealthy"
    { .d = AFFIX_FLAGS_NONE },

    // [102] "Baron's"
    { .d = AFFIX_FLAGS_NONE },

    // [103] "Expert"
    { .d = AFFIX_FLAGS_NONE },

    // [104] "Fortune's"
    { .d = AFFIX_FLAGS_NONE },

    // [105] "Glimmering"
    { .d = AFFIX_FLAGS_NONE },

    // [106] "Mithrilstone"
    { .d = AFFIX_FLAGS_NONE },

    // [107] "Decorated"
    { .d = AFFIX_FLAGS_NONE },

    // [108] "Surveyor's"
    { .d = AFFIX_FLAGS_NONE },

    // [109] "Wolfskin"
    { .d = AFFIX_FLAGS_NONE },

    // [110] "Spectral"
    { .d = AFFIX_FLAGS_NONE },

    // [111] "Wisp's"
    { .d = AFFIX_FLAGS_NONE },

    // [112] "Dark"
    { .d = AFFIX_FLAGS_NONE },

    // [113] "Dull Grey"
    { .d = AFFIX_FLAGS_NONE },

    // [114] "Raven's"
    { .d = AFFIX_FLAGS_NONE },

    // [115] "Ghoulish"
    { .d = { .minLevelPlus5B = 1 } },

    // [116] "Cavemaster"
    { .d = AFFIX_FLAGS_NONE },

    // [117] "Dead Hero's"
    { .d = AFFIX_FLAGS_NONE },

    // [118] "Radiant"
    { .d = AFFIX_FLAGS_NONE },

    // [119] "Ethereal"
    { .d = AFFIX_FLAGS_NONE },

    // [120] "Coral"
    { .d = AFFIX_FLAGS_NONE },

    // [121] "Sailor's"
    { .d = AFFIX_FLAGS_NONE },

    // [122] "Pirate's"
    { .d = AFFIX_FLAGS_NONE },

    // [123] "Raftsman's"
    { .d = AFFIX_FLAGS_NONE },

    // [124] "Aquamarine"
    { .d = AFFIX_FLAGS_NONE },

    // [125] "Fishscale"
    { .d = AFFIX_FLAGS_NONE },

    // [126] "Captain's"
    { .d = AFFIX_FLAGS_NONE },

    // [127] "Shipman's"
    { .d = AFFIX_FLAGS_NONE },

    // [128] "Scurvy"
    { .d = AFFIX_FLAGS_NONE },

    // [129] "Custom"
    { .d = AFFIX_FLAGS_NONE },

    // [130] "Adamant"
    { .d = AFFIX_FLAGS_NONE },

    // [131] "Sharkskin"
    { .d = AFFIX_FLAGS_NONE },

    // [132] "Frozen"
    { .d = AFFIX_FLAGS_NONE },

    // [133] "Toughened"
    { .d = AFFIX_FLAGS_NONE },

    // [134] "Burly"
    { .d = AFFIX_FLAGS_NONE },

    // [135] "Snow Grey"
    { .d = AFFIX_FLAGS_NONE },

    // [136] "Camouflage"
    { .d = AFFIX_FLAGS_NONE },

    // [137] "Extra Heavy"
    { .d = AFFIX_FLAGS_NONE },

    // [138] "Glacial"
    { .d = AFFIX_FLAGS_NONE },

    // [139] "Balin's"
    { .d = AFFIX_FLAGS_NONE },

    // [140] "Polar"
    { .d = AFFIX_FLAGS_NONE },

    // [141] "White Runed"
    { .d = AFFIX_FLAGS_NONE },

    // [142] "Dwarf Butcher"
    { .d = AFFIX_FLAGS_NONE },

    // [143] "Massive"
    { .d = AFFIX_FLAGS_NONE },

    // [144] "Courageous"
    { .d = AFFIX_FLAGS_NONE },

    // [145] "Elf Runed"
    { .d = AFFIX_FLAGS_NONE },

    // [146] "Beryl"
    { .d = AFFIX_FLAGS_NONE },

    // [147] "Artisan's"
    { .d = AFFIX_FLAGS_NONE },

    // [148] "Noble"
    { .d = AFFIX_FLAGS_NONE },

    // [149] "Strengthened"
    { .d = AFFIX_FLAGS_NONE },

    // [150] "Fine Runed"
    { .d = AFFIX_FLAGS_NONE },

    // [151] "Rearguard's"
    { .d = AFFIX_FLAGS_NONE },

    // [152] "Fine Elven"
    { .d = AFFIX_FLAGS_NONE },

    // [153] "Denethor's"
    { .d = AFFIX_FLAGS_NONE },

    // [154] "Heroic"
    { .d = AFFIX_FLAGS_NONE },

    // [155] "Boromir's"
    { .d = AFFIX_FLAGS_NONE },
};
#undef AFFIX_FLAGS_NONE
// clang-format on
