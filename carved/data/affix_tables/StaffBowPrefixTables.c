#include "item.h"
#include "stats.h"
#include "text_ids.h"

// clang-format off


/** @romaddress 0x080660e8 */
const ItemAffix StaffBowPrefixes[108] = {
    // [0] "Servant's": Hit Points +5
    { .nameId = TEXT_ID_PREFIX_SERVANTS,
      .tier = 1,
      .stat0 = STAT_MAX_HP,                .val0 =   5,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [1] "Retainer's": Health +1
    { .nameId = TEXT_ID_PREFIX_RETAINERS,
      .tier = 1,
      .stat0 = STAT_HEALTH,                .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [2] "Mirkwood": Defense +1, Wisdom +1
    { .nameId = TEXT_ID_PREFIX_MIRKWOOD,
      .tier = 1,
      .stat0 = STAT_DEFENSE,               .val0 =   1,
      .stat1 = STAT_WISDOM,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [3] "Captive's": Courage +2, Defense -1
    { .nameId = TEXT_ID_PREFIX_CAPTIVES,
      .tier = 1,
      .stat0 = STAT_COURAGE,               .val0 =   2,
      .stat1 = STAT_DEFENSE,               .val1 =  -1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [4] "Cumbersome": Accuracy -3, Damage +3
    { .nameId = TEXT_ID_PREFIX_CUMBERSOME,
      .tier = 1,
      .stat0 = STAT_ACCURACY,              .val0 =  -3,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [5] "Uruk Hai": Accuracy -3, Strength +2
    { .nameId = TEXT_ID_PREFIX_URUK_HAI,
      .tier = 1,
      .stat0 = STAT_ACCURACY,              .val0 =  -3,
      .stat1 = STAT_STRENGTH,              .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [6] "Rotting": Strength +9, Fragile +1
    { .nameId = TEXT_ID_PREFIX_ROTTING,
      .tier = 1,
      .stat0 = STAT_STRENGTH,              .val0 =   9,
      .stat1 = STAT_FRAGILE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [7] "Dried": All Primary Stats +4, Fragile +1
    { .nameId = TEXT_ID_PREFIX_DRIED,
      .tier = 1,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   4,
      .stat1 = STAT_FRAGILE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [8] "Overseer's": Hit Points +15, Wisdom +2
    { .nameId = TEXT_ID_PREFIX_OVERSEERS,
      .tier = 3,
      .stat0 = STAT_MAX_HP,                .val0 =  15,
      .stat1 = STAT_WISDOM,                .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [9] "Dark Archer's": % Critical +15, Damage +2, Cursed +1
    { .nameId = TEXT_ID_PREFIX_DARK_ARCHERS,
      .tier = 3,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,   .val0 =  15,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   2,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [10] "Bloodwood": % Critical +10, Strength +3
    { .nameId = TEXT_ID_PREFIX_BLOODWOOD,
      .tier = 5,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,   .val0 =  10,
      .stat1 = STAT_STRENGTH,              .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [11] "Ungol": Strength +7, Cursed +1
    { .nameId = TEXT_ID_PREFIX_UNGOL,
      .tier = 5,
      .stat0 = STAT_STRENGTH,              .val0 =   7,
      .stat1 = STAT_CURSED,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [12] "Oiled": Defense +2
    { .nameId = TEXT_ID_PREFIX_OILED,
      .tier = 3,
      .stat0 = STAT_DEFENSE,               .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [13] "Strength": Strength +2, Damage +1
    { .nameId = TEXT_ID_PREFIX_STRENGTH,
      .tier = 3,
      .stat0 = STAT_STRENGTH,              .val0 =   2,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [14] "Fell": Damage +4, Cursed +1
    { .nameId = TEXT_ID_PREFIX_FELL,
      .tier = 3,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   4,
      .stat1 = STAT_CURSED,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [15] "Thorny": Damage +2, Hit Points per Slain Foe -1
    { .nameId = TEXT_ID_PREFIX_THORNY,
      .tier = 3,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_HP_PER_KILL,           .val1 =  -1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [16] "Blackwood": Damage +1, Wisdom +2
    { .nameId = TEXT_ID_PREFIX_BLACKWOOD,
      .tier = 3,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_WISDOM,                .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [17] "Rotwood": Damage +4, Fragile +1, Venomous +1
    { .nameId = TEXT_ID_PREFIX_ROTWOOD,
      .tier = 3,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   4,
      .stat1 = STAT_FRAGILE,               .val1 =   1,
      .stat2 = STAT_VENOMOUS,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [18] "Putrid": Venomous +1, Cursed +1
    { .nameId = TEXT_ID_PREFIX_PUTRID,
      .tier = 3,
      .stat0 = STAT_VENOMOUS,              .val0 =   1,
      .stat1 = STAT_CURSED,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [19] "Russet": Health +1, Strength +1
    { .nameId = TEXT_ID_PREFIX_RUSSET,
      .tier = 3,
      .stat0 = STAT_HEALTH,                .val0 =   1,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [20] "Webwood": Strength +2, Immobilize +1
    { .nameId = TEXT_ID_PREFIX_WEBWOOD,
      .tier = 5,
      .stat0 = STAT_STRENGTH,              .val0 =   2,
      .stat1 = STAT_IMMOBILIZE,            .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [21] "Oozing": % Poison Resistance +30, Defense +2, Venomous +1
    { .nameId = TEXT_ID_PREFIX_OOZING,
      .tier = 5,
      .stat0 = STAT_POISON_RESIST_PERCENT, .val0 =  30,
      .stat1 = STAT_DEFENSE,               .val1 =   2,
      .stat2 = STAT_VENOMOUS,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [22] "Spiderleg": % Poison Resistance +30, Accuracy -10, Venomous +1, Hit Points per Slain Foe +5
    { .nameId = TEXT_ID_PREFIX_SPIDERLEG,
      .tier = 7,
      .stat0 = STAT_POISON_RESIST_PERCENT, .val0 =  30,
      .stat1 = STAT_ACCURACY,              .val1 = -10,
      .stat2 = STAT_VENOMOUS,              .val2 =   1,
      .stat3 = STAT_HP_PER_KILL,           .val3 =   5 },

    // [23] "Jade": Knockback +10, Wisdom +4
    { .nameId = TEXT_ID_PREFIX_JADE,
      .tier = 7,
      .stat0 = STAT_KNOCKBACK,             .val0 =  10,
      .stat1 = STAT_WISDOM,                .val1 =   4,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [24] "Unnatural": All Primary Stats +3, Hit Points -50, Wisdom +2
    { .nameId = TEXT_ID_PREFIX_UNNATURAL,
      .tier = 6,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   3,
      .stat1 = STAT_MAX_HP,                .val1 = -50,
      .stat2 = STAT_WISDOM,                .val2 =   2,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [25] "Coal Black": Strength +2, Fire Damage +1
    { .nameId = TEXT_ID_PREFIX_COAL_BLACK,
      .tier = 6,
      .stat0 = STAT_STRENGTH,              .val0 =   2,
      .stat1 = STAT_DAMAGE_FIRE,           .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [26] "Burnt": Accuracy +1, Damage +1, Fire Damage +1
    { .nameId = TEXT_ID_PREFIX_BURNT,
      .tier = 6,
      .stat0 = STAT_ACCURACY,              .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   1,
      .stat2 = STAT_DAMAGE_FIRE,           .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [27] "Ash": Defense +3
    { .nameId = TEXT_ID_PREFIX_ASH,
      .tier = 6,
      .stat0 = STAT_DEFENSE,               .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [28] "Sacrificial": Damage +3, Armor Piercing +1, Backstab +1
    { .nameId = TEXT_ID_PREFIX_SACRIFICIAL,
      .tier = 6,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   3,
      .stat1 = STAT_ARMOR_PIERCE,          .val1 =   1,
      .stat2 = STAT_BACKSTAB,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [29] "Onyx": Defense +1, Accuracy +1, Damage +1, Wisdom +2
    { .nameId = TEXT_ID_PREFIX_ONYX,
      .tier = 6,
      .stat0 = STAT_DEFENSE,               .val0 =   1,
      .stat1 = STAT_ACCURACY,              .val1 =   1,
      .stat2 = STAT_DAMAGE_FLAT,           .val2 =   1,
      .stat3 = STAT_WISDOM,                .val3 =   2 },

    // [30] "Fossil": Damage +2, Wisdom +3
    { .nameId = TEXT_ID_PREFIX_FOSSIL,
      .tier = 6,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_WISDOM,                .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [31] "Black Bone": Damage +2, Wisdom +6, Cursed +1
    { .nameId = TEXT_ID_PREFIX_BLACK_BONE,
      .tier = 6,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_WISDOM,                .val1 =   6,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [32] "White Ash": Courage +3, Wisdom +3
    { .nameId = TEXT_ID_PREFIX_WHITE_ASH,
      .tier = 8,
      .stat0 = STAT_COURAGE,               .val0 =   3,
      .stat1 = STAT_WISDOM,                .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [33] "Gollum's": Strength +3, Courage +1, % Speed +15
    { .nameId = TEXT_ID_PREFIX_GOLLUMS,
      .tier = 8,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_SPEED_PERCENT,         .val2 =  15,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [34] "Tainted": All Primary Stats +4, Cursed +1
    { .nameId = TEXT_ID_PREFIX_TAINTED,
      .tier = 10,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   4,
      .stat1 = STAT_CURSED,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [35] "Black Lord's": All Primary Stats +2, Damage +5, Cursed +1, Hit Points per Slain Foe +5
    { .nameId = TEXT_ID_PREFIX_BLACK_LORDS,
      .tier = 10,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   2,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   5,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_HP_PER_KILL,           .val3 =   5 },

    // [36] "Bony": Damage +1, Defense +1
    { .nameId = TEXT_ID_PREFIX_BONY,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_DEFENSE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [37] "Knobbed": Damage +1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_KNOBBED,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_CHEAP,                 .val3 =   1 },

    // [38] "Warped": Defense +2, Accuracy -1
    { .nameId = TEXT_ID_PREFIX_WARPED,
      .tier = 1,
      .stat0 = STAT_DEFENSE,               .val0 =   2,
      .stat1 = STAT_ACCURACY,              .val1 =  -1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [39] "Hobbit": Health +1, Courage +1
    { .nameId = TEXT_ID_PREFIX_HOBBIT,
      .tier = 1,
      .stat0 = STAT_HEALTH,                .val0 =   1,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [40] "Puissant": Accuracy +1
    { .nameId = TEXT_ID_PREFIX_PUISSANT,
      .tier = 1,
      .stat0 = STAT_ACCURACY,              .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [41] "Fine": Damage +1
    { .nameId = TEXT_ID_PREFIX_FINE,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [42] "Sturdy": Hit Points +5, Strength +1
    { .nameId = TEXT_ID_PREFIX_STURDY,
      .tier = 1,
      .stat0 = STAT_MAX_HP,                .val0 =   5,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [43] "Ivory": Courage +1
    { .nameId = TEXT_ID_PREFIX_IVORY,
      .tier = 1,
      .stat0 = STAT_COURAGE,               .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [44] "Sorcerer's": Wisdom +2
    { .nameId = TEXT_ID_PREFIX_SORCERERS,
      .tier = 3,
      .stat0 = STAT_WISDOM,                .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [45] "Dark Etched": Spirit Points +35, Cursed +1
    { .nameId = TEXT_ID_PREFIX_DARK_ETCHED,
      .tier = 3,
      .stat0 = STAT_MAX_SPIRIT,            .val0 =  35,
      .stat1 = STAT_CURSED,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [46] "Silverleaf": All Primary Stats +1, dmg to Animals +3
    { .nameId = TEXT_ID_PREFIX_SILVERLEAF,
      .tier = 5,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_DAMAGE_TO_ANIMALS,     .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [47] "White Wizard": Courage +4, Wisdom +3
    { .nameId = TEXT_ID_PREFIX_WHITE_WIZARD,
      .tier = 5,
      .stat0 = STAT_COURAGE,               .val0 =   4,
      .stat1 = STAT_WISDOM,                .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [48] "Pine": Accuracy +1, Hit Points +5
    { .nameId = TEXT_ID_PREFIX_PINE,
      .tier = 2,
      .stat0 = STAT_ACCURACY,              .val0 =   1,
      .stat1 = STAT_MAX_HP,                .val1 =   5,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [49] "Mahogany": Damage +2
    { .nameId = TEXT_ID_PREFIX_MAHOGANY,
      .tier = 2,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [50] "Elven": dmg to Orcs +3
    { .nameId = TEXT_ID_PREFIX_ELVEN,
      .tier = 2,
      .stat0 = STAT_DAMAGE_TO_ORCS,        .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [51] "Prince's": Courage +1
    { .nameId = TEXT_ID_PREFIX_PRINCES,
      .tier = 2,
      .stat0 = STAT_COURAGE,               .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [52] "Gamekeeper's": dmg to Animals +3
    { .nameId = TEXT_ID_PREFIX_GAMEKEEPERS,
      .tier = 2,
      .stat0 = STAT_DAMAGE_TO_ANIMALS,     .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [53] "Shepherd's": dmg to Animals +1, Courage +1
    { .nameId = TEXT_ID_PREFIX_SHEPHERDS,
      .tier = 2,
      .stat0 = STAT_DAMAGE_TO_ANIMALS,     .val0 =   1,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [54] "Prairie": Hit Points +10, Spirit Points +10
    { .nameId = TEXT_ID_PREFIX_PRAIRIE,
      .tier = 2,
      .stat0 = STAT_MAX_HP,                .val0 =  10,
      .stat1 = STAT_MAX_SPIRIT,            .val1 =  10,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [55] "Branded": Damage +1, Courage +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_BRANDED,
      .tier = 2,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [56] "Staghorn": Strength +3, Armor Piercing +1
    { .nameId = TEXT_ID_PREFIX_STAGHORN,
      .tier = 4,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_ARMOR_PIERCE,          .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [57] "Eomer's": Strength +2, Courage +1
    { .nameId = TEXT_ID_PREFIX_EOMERS,
      .tier = 4,
      .stat0 = STAT_STRENGTH,              .val0 =   2,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [58] "Vulture's": % Hit Points per Slain Foe +10, Damage +1
    { .nameId = TEXT_ID_PREFIX_VULTURES,
      .tier = 6,
      .stat0 = STAT_HP_PERCENT_PER_KILL,   .val0 =  10,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [59] "Olive": Defense +3
    { .nameId = TEXT_ID_PREFIX_OLIVE,
      .tier = 6,
      .stat0 = STAT_DEFENSE,               .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [60] "Elm": Defense +2
    { .nameId = TEXT_ID_PREFIX_ELM,
      .tier = 3,
      .stat0 = STAT_DEFENSE,               .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [61] "Walnut": All Primary Stats +1, Wisdom +1, Strength -1
    { .nameId = TEXT_ID_PREFIX_WALNUT,
      .tier = 3,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_WISDOM,                .val1 =   1,
      .stat2 = STAT_STRENGTH,              .val2 =  -1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [62] "Cherry": Hit Points +25
    { .nameId = TEXT_ID_PREFIX_CHERRY,
      .tier = 3,
      .stat0 = STAT_MAX_HP,                .val0 =  25,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [63] "Etched": All Primary Stats +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_ETCHED,
      .tier = 3,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [64] "Masterwork": Damage +1, Accuracy +2
    { .nameId = TEXT_ID_PREFIX_MASTERWORK,
      .tier = 3,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_ACCURACY,              .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [65] "Woodworker's": Accuracy +2, Damage -1
    { .nameId = TEXT_ID_PREFIX_WOODWORKERS,
      .tier = 3,
      .stat0 = STAT_ACCURACY,              .val0 =   2,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =  -1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [66] "Heavy": Damage +2, Knockback +5
    { .nameId = TEXT_ID_PREFIX_HEAVY,
      .tier = 3,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_KNOCKBACK,             .val1 =   5,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [67] "Wildman's": Accuracy -2, Damage +3
    { .nameId = TEXT_ID_PREFIX_WILDMANS,
      .tier = 3,
      .stat0 = STAT_ACCURACY,              .val0 =  -2,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [68] "Unbreakable": Hit Points +35
    { .nameId = TEXT_ID_PREFIX_UNBREAKABLE,
      .tier = 5,
      .stat0 = STAT_MAX_HP,                .val0 =  35,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [69] "Celeborn's": Courage +3, Strength +1
    { .nameId = TEXT_ID_PREFIX_CELEBORNS,
      .tier = 5,
      .stat0 = STAT_COURAGE,               .val0 =   3,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [70] "Elrond's": Courage +4, Strength +2, Accuracy +5
    { .nameId = TEXT_ID_PREFIX_ELRONDS,
      .tier = 7,
      .stat0 = STAT_COURAGE,               .val0 =   4,
      .stat1 = STAT_STRENGTH,              .val1 =   2,
      .stat2 = STAT_ACCURACY,              .val2 =   5,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [71] "Courageous": Courage +5
    { .nameId = TEXT_ID_PREFIX_COURAGEOUS,
      .tier = 7,
      .stat0 = STAT_COURAGE,               .val0 =   5,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [72] "Ebony": Damage +12, dmg to Nazgul +10
    { .nameId = TEXT_ID_PREFIX_EBONY,
      .tier = 4,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =  12,
      .stat1 = STAT_DAMAGE_TO_NAZGUL,      .val1 =  10,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [73] "Old": Damage +8
    { .nameId = TEXT_ID_PREFIX_OLD,
      .tier = 4,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   8,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [74] "Age Old": All Primary Stats +1, Armor Piercing +1
    { .nameId = TEXT_ID_PREFIX_AGE_OLD,
      .tier = 4,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_ARMOR_PIERCE,          .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [75] "Blemished": Defense +5, Courage +5
    { .nameId = TEXT_ID_PREFIX_BLEMISHED,
      .tier = 4,
      .stat0 = STAT_DEFENSE,               .val0 =   5,
      .stat1 = STAT_COURAGE,               .val1 =   5,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [76] "Rattling": Courage +7, Venomous +1
    { .nameId = TEXT_ID_PREFIX_RATTLING,
      .tier = 4,
      .stat0 = STAT_COURAGE,               .val0 =   7,
      .stat1 = STAT_VENOMOUS,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [77] "Cavefish": % Poison Resistance +50
    { .nameId = TEXT_ID_PREFIX_CAVEFISH,
      .tier = 4,
      .stat0 = STAT_POISON_RESIST_PERCENT, .val0 =  50,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [78] "Carved": All Primary Stats +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_CARVED,
      .tier = 4,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [79] "Terrible": Strength +3, Hit Points per Slain Foe +10
    { .nameId = TEXT_ID_PREFIX_TERRIBLE,
      .tier = 4,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_HP_PER_KILL,           .val1 =  10,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [80] "Ghostwood": Melee Armor +6, Damage +2, Hit Points per Slain Foe +2
    { .nameId = TEXT_ID_PREFIX_GHOSTWOOD,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =   6,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   2,
      .stat2 = STAT_HP_PER_KILL,           .val2 =   2,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [81] "Black Rune": Nightburst +1, Damage +7, Cursed +1
    { .nameId = TEXT_ID_PREFIX_BLACK_RUNE,
      .tier = 6,
      .stat0 = STAT_NIGHTBURST,            .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   7,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [82] "Ethereal": Melee Armor +12, % Fear Resistance +20, % Poison Resistance +20
    { .nameId = TEXT_ID_PREFIX_ETHEREAL,
      .tier = 8,
      .stat0 = STAT_MELEE_ARMOR,           .val0 =  12,
      .stat1 = STAT_FEAR_RESIST_PERCENT,   .val1 =  20,
      .stat2 = STAT_POISON_RESIST_PERCENT, .val2 =  20,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [83] "Gemstudded": All Primary Stats +4, Valuable +1
    { .nameId = TEXT_ID_PREFIX_GEMSTUDDED,
      .tier = 8,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   4,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [84] "Oak": Strength +3, Knockback +5
    { .nameId = TEXT_ID_PREFIX_OAK,
      .tier = 5,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_KNOCKBACK,             .val1 =   5,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [85] "Dogwood": Courage +1, Wisdom +2
    { .nameId = TEXT_ID_PREFIX_DOGWOOD,
      .tier = 5,
      .stat0 = STAT_COURAGE,               .val0 =   1,
      .stat1 = STAT_WISDOM,                .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [86] "Icy": Immobilize +1, Armor Piercing +1
    { .nameId = TEXT_ID_PREFIX_ICY,
      .tier = 5,
      .stat0 = STAT_IMMOBILIZE,            .val0 =   1,
      .stat1 = STAT_ARMOR_PIERCE,          .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [87] "Navigator's": Courage +2, Accuracy +2, Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_NAVIGATORS,
      .tier = 5,
      .stat0 = STAT_COURAGE,               .val0 =   2,
      .stat1 = STAT_ACCURACY,              .val1 =   2,
      .stat2 = STAT_GEMFINDER,             .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [88] "Ivy": Hit Points Regen +3
    { .nameId = TEXT_ID_PREFIX_IVY,
      .tier = 5,
      .stat0 = STAT_HP_REGEN,              .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [89] "Wilderness": dmg to Animals +4, dmg to Orcs +2
    { .nameId = TEXT_ID_PREFIX_WILDERNESS,
      .tier = 5,
      .stat0 = STAT_DAMAGE_TO_ANIMALS,     .val0 =   4,
      .stat1 = STAT_DAMAGE_TO_ORCS,        .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [90] "Preserved": Hit Points +25, Spirit Points +20
    { .nameId = TEXT_ID_PREFIX_PRESERVED,
      .tier = 5,
      .stat0 = STAT_MAX_HP,                .val0 =  25,
      .stat1 = STAT_MAX_SPIRIT,            .val1 =  20,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [91] "Strengthened": Strength +3, Damage +2
    { .nameId = TEXT_ID_PREFIX_STRENGTHENED,
      .tier = 5,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [92] "Glass": Damage +11, Fragile +1
    { .nameId = TEXT_ID_PREFIX_GLASS,
      .tier = 7,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =  11,
      .stat1 = STAT_FRAGILE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [93] "Dungeoneer's": Strength +3, Courage +3, Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_DUNGEONEERS,
      .tier = 7,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_COURAGE,               .val1 =   3,
      .stat2 = STAT_GEMFINDER,             .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [94] "Crystal Rune": Damage +4, All Primary Stats +1
    { .nameId = TEXT_ID_PREFIX_CRYSTAL_RUNE,
      .tier = 9,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   4,
      .stat1 = STAT_ALL_PRIMARY_STATS,     .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [95] "Stone-etched": Damage +5
    { .nameId = TEXT_ID_PREFIX_STONE_ETCHED,
      .tier = 9,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   5,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [96] "Applewood": Hit Points Regen +4
    { .nameId = TEXT_ID_PREFIX_APPLEWOOD,
      .tier = 6,
      .stat0 = STAT_HP_REGEN,              .val0 =   4,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [97] "Purpleheart": Spirit Regen +3
    { .nameId = TEXT_ID_PREFIX_PURPLEHEART,
      .tier = 6,
      .stat0 = STAT_SPIRIT_REGEN,          .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [98] "Holly": Strength +3, Courage +1
    { .nameId = TEXT_ID_PREFIX_HOLLY,
      .tier = 6,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [99] "Teak": Hit Points +30
    { .nameId = TEXT_ID_PREFIX_TEAK,
      .tier = 6,
      .stat0 = STAT_MAX_HP,                .val0 =  30,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [100] "Mithrilstone": Courage +3, Damage +2, Strength +1
    { .nameId = TEXT_ID_PREFIX_MITHRILSTONE,
      .tier = 6,
      .stat0 = STAT_COURAGE,               .val0 =   3,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   2,
      .stat2 = STAT_STRENGTH,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [101] "Sage's": Courage +3, Spirit Points +10, Wisdom +1
    { .nameId = TEXT_ID_PREFIX_SAGES,
      .tier = 6,
      .stat0 = STAT_COURAGE,               .val0 =   3,
      .stat1 = STAT_MAX_SPIRIT,            .val1 =  10,
      .stat2 = STAT_WISDOM,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [102] "Storm": Damage +2, Nightburst +1
    { .nameId = TEXT_ID_PREFIX_STORM,
      .tier = 6,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_NIGHTBURST,            .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [103] "Orc Slaying": dmg to Orcs +4
    { .nameId = TEXT_ID_PREFIX_ORC_SLAYING,
      .tier = 6,
      .stat0 = STAT_DAMAGE_TO_ORCS,        .val0 =   4,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [104] "Heroic": Strength +4, Courage +3, Accuracy +1
    { .nameId = TEXT_ID_PREFIX_HEROIC,
      .tier = 8,
      .stat0 = STAT_STRENGTH,              .val0 =   4,
      .stat1 = STAT_COURAGE,               .val1 =   3,
      .stat2 = STAT_ACCURACY,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [105] "Denethor's": Strength +5, Hit Points +25, Cursed +1
    { .nameId = TEXT_ID_PREFIX_DENETHORS,
      .tier = 8,
      .stat0 = STAT_STRENGTH,              .val0 =   5,
      .stat1 = STAT_MAX_HP,                .val1 =  25,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [106] "High Wizard's": Courage +5, Wisdom +5
    { .nameId = TEXT_ID_PREFIX_HIGH_WIZARDS,
      .tier = 10,
      .stat0 = STAT_COURAGE,               .val0 =   5,
      .stat1 = STAT_WISDOM,                .val1 =   5,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [107] "High Elven": dmg to Orcs +6
    { .nameId = TEXT_ID_PREFIX_HIGH_ELVEN,
      .tier = 10,
      .stat0 = STAT_DAMAGE_TO_ORCS,        .val0 =   6,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },
};

/** no decoded bits set (72 of 108 entries) */
#define AFFIX_FLAGS_NONE { 0 }

/**
 * Per-affix flag table; indexed by the affix index. Decoded bits: AFFIX_FLAG_* (item.h).
 *
 * @romaddress 0x080665f8
 */
const AffixFlags StaffBowPrefixFlags[108] = {
    // [0] "Servant's"
    { .d = AFFIX_FLAGS_NONE },

    // [1] "Retainer's"
    { .d = AFFIX_FLAGS_NONE },

    // [2] "Mirkwood"
    { .d = AFFIX_FLAGS_NONE },

    // [3] "Captive's"
    { .d = AFFIX_FLAGS_NONE },

    // [4] "Cumbersome"
    { .d = AFFIX_FLAGS_NONE },

    // [5] "Uruk Hai"
    { .d = AFFIX_FLAGS_NONE },

    // [6] "Rotting"
    { .d = AFFIX_FLAGS_NONE },

    // [7] "Dried"
    { .d = AFFIX_FLAGS_NONE },

    // [8] "Overseer's"
    { .d = AFFIX_FLAGS_NONE },

    // [9] "Dark Archer's"
    { .d = AFFIX_FLAGS_NONE },

    // [10] "Bloodwood"
    { .d = { .field_bit_26 = 1 } },

    // [11] "Ungol"
    { .d = AFFIX_FLAGS_NONE },

    // [12] "Oiled"
    { .d = AFFIX_FLAGS_NONE },

    // [13] "Strength"
    { .d = AFFIX_FLAGS_NONE },

    // [14] "Fell"
    { .d = AFFIX_FLAGS_NONE },

    // [15] "Thorny"
    { .d = { .field_bit_29 = 1 } },

    // [16] "Blackwood"
    { .d = { .field_bit_30 = 1 } },

    // [17] "Rotwood"
    { .d = AFFIX_FLAGS_NONE },

    // [18] "Putrid"
    { .d = { .field_bit_17 = 1,
             .field_bit_22 = 1 } },

    // [19] "Russet"
    { .d = AFFIX_FLAGS_NONE },

    // [20] "Webwood"
    { .d = AFFIX_FLAGS_NONE },

    // [21] "Oozing"
    { .d = AFFIX_FLAGS_NONE },

    // [22] "Spiderleg"
    { .d = { .field_bit_29 = 1 } },

    // [23] "Jade"
    { .d = { .field_bit_27 = 1 } },

    // [24] "Unnatural"
    { .d = AFFIX_FLAGS_NONE },

    // [25] "Coal Black"
    { .d = { .field_bit_30 = 1 } },

    // [26] "Burnt"
    { .d = { .field_bit_30 = 1 } },

    // [27] "Ash"
    { .d = AFFIX_FLAGS_NONE },

    // [28] "Sacrificial"
    { .d = AFFIX_FLAGS_NONE },

    // [29] "Onyx"
    { .d = { .field_bit_25 = 1 } },

    // [30] "Fossil"
    { .d = { .field_bit_25 = 1 } },

    // [31] "Black Bone"
    { .d = { .field_bit_30 = 1 } },

    // [32] "White Ash"
    { .d = { .field_bit_25 = 1 } },

    // [33] "Gollum's"
    { .d = AFFIX_FLAGS_NONE },

    // [34] "Tainted"
    { .d = AFFIX_FLAGS_NONE },

    // [35] "Black Lord's"
    { .d = AFFIX_FLAGS_NONE },

    // [36] "Bony"
    { .d = { .field_bit_25 = 1 } },

    // [37] "Knobbed"
    { .d = AFFIX_FLAGS_NONE },

    // [38] "Warped"
    { .d = AFFIX_FLAGS_NONE },

    // [39] "Hobbit"
    { .d = AFFIX_FLAGS_NONE },

    // [40] "Puissant"
    { .d = AFFIX_FLAGS_NONE },

    // [41] "Fine"
    { .d = AFFIX_FLAGS_NONE },

    // [42] "Sturdy"
    { .d = AFFIX_FLAGS_NONE },

    // [43] "Ivory"
    { .d = { .field_bit_25 = 1 } },

    // [44] "Sorcerer's"
    { .d = { .field_bit_15 = 1,
             .field_bit_24 = 1 } },

    // [45] "Dark Etched"
    { .d = AFFIX_FLAGS_NONE },

    // [46] "Silverleaf"
    { .d = AFFIX_FLAGS_NONE },

    // [47] "White Wizard"
    { .d = { .field_bit_17 = 1,
             .field_bit_18 = 1,
             .field_bit_25 = 1 } },

    // [48] "Pine"
    { .d = AFFIX_FLAGS_NONE },

    // [49] "Mahogany"
    { .d = AFFIX_FLAGS_NONE },

    // [50] "Elven"
    { .d = AFFIX_FLAGS_NONE },

    // [51] "Prince's"
    { .d = AFFIX_FLAGS_NONE },

    // [52] "Gamekeeper's"
    { .d = AFFIX_FLAGS_NONE },

    // [53] "Shepherd's"
    { .d = AFFIX_FLAGS_NONE },

    // [54] "Prairie"
    { .d = AFFIX_FLAGS_NONE },

    // [55] "Branded"
    { .d = AFFIX_FLAGS_NONE },

    // [56] "Staghorn"
    { .d = { .field_bit_29 = 1 } },

    // [57] "Eomer's"
    { .d = AFFIX_FLAGS_NONE },

    // [58] "Vulture's"
    { .d = AFFIX_FLAGS_NONE },

    // [59] "Olive"
    { .d = AFFIX_FLAGS_NONE },

    // [60] "Elm"
    { .d = AFFIX_FLAGS_NONE },

    // [61] "Walnut"
    { .d = { .field_bit_29 = 1 } },

    // [62] "Cherry"
    { .d = { .field_bit_26 = 1 } },

    // [63] "Etched"
    { .d = AFFIX_FLAGS_NONE },

    // [64] "Masterwork"
    { .d = AFFIX_FLAGS_NONE },

    // [65] "Woodworker's"
    { .d = { .field_bit_29 = 1 } },

    // [66] "Heavy"
    { .d = AFFIX_FLAGS_NONE },

    // [67] "Wildman's"
    { .d = AFFIX_FLAGS_NONE },

    // [68] "Unbreakable"
    { .d = AFFIX_FLAGS_NONE },

    // [69] "Celeborn's"
    { .d = AFFIX_FLAGS_NONE },

    // [70] "Elrond's"
    { .d = AFFIX_FLAGS_NONE },

    // [71] "Courageous"
    { .d = AFFIX_FLAGS_NONE },

    // [72] "Ebony"
    { .d = { .field_bit_30 = 1 } },

    // [73] "Old"
    { .d = AFFIX_FLAGS_NONE },

    // [74] "Age Old"
    { .d = AFFIX_FLAGS_NONE },

    // [75] "Blemished"
    { .d = AFFIX_FLAGS_NONE },

    // [76] "Rattling"
    { .d = AFFIX_FLAGS_NONE },

    // [77] "Cavefish"
    { .d = AFFIX_FLAGS_NONE },

    // [78] "Carved"
    { .d = AFFIX_FLAGS_NONE },

    // [79] "Terrible"
    { .d = AFFIX_FLAGS_NONE },

    // [80] "Ghostwood"
    { .d = { .field_bit_14 = 1,
             .field_bit_25 = 1 } },

    // [81] "Black Rune"
    { .d = { .field_bit_17 = 1,
             .field_bit_23 = 1 } },

    // [82] "Ethereal"
    { .d = { .field_bit_14 = 1,
             .field_bit_28 = 1 } },

    // [83] "Gemstudded"
    { .d = AFFIX_FLAGS_NONE },

    // [84] "Oak"
    { .d = AFFIX_FLAGS_NONE },

    // [85] "Dogwood"
    { .d = AFFIX_FLAGS_NONE },

    // [86] "Icy"
    { .d = { .field_bit_17 = 1,
             .field_bit_18 = 1,
             .field_bit_28 = 1 } },

    // [87] "Navigator's"
    { .d = AFFIX_FLAGS_NONE },

    // [88] "Ivy"
    { .d = { .minLevelPlus5A = 1,
             .field_bit_15 = 1,
             .field_bit_20 = 1,
             .field_bit_27 = 1 } },

    // [89] "Wilderness"
    { .d = { .field_bit_15 = 1,
             .field_bit_20 = 1,
             .field_bit_29 = 1 } },

    // [90] "Preserved"
    { .d = AFFIX_FLAGS_NONE },

    // [91] "Strengthened"
    { .d = AFFIX_FLAGS_NONE },

    // [92] "Glass"
    { .d = { .field_bit_14 = 1,
             .field_bit_28 = 1 } },

    // [93] "Dungeoneer's"
    { .d = AFFIX_FLAGS_NONE },

    // [94] "Crystal Rune"
    { .d = AFFIX_FLAGS_NONE },

    // [95] "Stone-etched"
    { .d = { .field_bit_28 = 1 } },

    // [96] "Applewood"
    { .d = { .minLevelPlus5A = 1,
             .field_bit_29 = 1 } },

    // [97] "Purpleheart"
    { .d = { .minLevelPlus5B = 1,
             .field_bit_17 = 1,
             .field_bit_24 = 1,
             .field_bit_30 = 1 } },

    // [98] "Holly"
    { .d = { .field_bit_17 = 1,
             .field_bit_19 = 1,
             .field_bit_27 = 1 } },

    // [99] "Teak"
    { .d = AFFIX_FLAGS_NONE },

    // [100] "Mithrilstone"
    { .d = AFFIX_FLAGS_NONE },

    // [101] "Sage's"
    { .d = { .field_bit_17 = 1,
             .field_bit_21 = 1,
             .field_bit_25 = 1 } },

    // [102] "Storm"
    { .d = { .field_bit_16 = 1,
             .field_bit_18 = 1,
             .field_bit_28 = 1 } },

    // [103] "Orc Slaying"
    { .d = AFFIX_FLAGS_NONE },

    // [104] "Heroic"
    { .d = AFFIX_FLAGS_NONE },

    // [105] "Denethor's"
    { .d = AFFIX_FLAGS_NONE },

    // [106] "High Wizard's"
    { .d = { .field_bit_15 = 1,
             .field_bit_24 = 1,
             .field_bit_26 = 1 } },

    // [107] "High Elven"
    { .d = { .field_bit_17 = 1,
             .field_bit_18 = 1,
             .field_bit_27 = 1 } },
};
#undef AFFIX_FLAGS_NONE
// clang-format on
