#include "item.h"
#include "stats.h"
#include "text_ids.h"

// clang-format off

// TODO: [11] "Blessed" carries .val1=5 on an empty (STAT_NONE) slot - meaning unknown
// TODO: [21] "Grishnakh's" carries .val2=1 on an empty (STAT_NONE) slot - meaning unknown
// TODO: [37] "Orchide" carries .val1=1 on an empty (STAT_NONE) slot - meaning unknown
// TODO: [55] "Slaver's" carries .val1=1 on an empty (STAT_NONE) slot - meaning unknown
// TODO: [65] "Bleached" carries .val1=1 on an empty (STAT_NONE) slot - meaning unknown
// TODO: [75] "Turquoise" carries .val2=1 on an empty (STAT_NONE) slot - meaning unknown

/** @romaddress 0x08067f28 */
const ItemAffix GloveShoePrefixes[96] = {
    // [0] "Easterling": Backstab +1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_EASTERLING,
      .tier = 1,
      .stat0 = STAT_BACKSTAB,                  .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_CHEAP,                     .val3 =   1 },

    // [1] "Black": Damage +1
    { .nameId = TEXT_ID_PREFIX_BLACK,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [2] "Oiled": Defense +1
    { .nameId = TEXT_ID_PREFIX_OILED,
      .tier = 1,
      .stat0 = STAT_DEFENSE,                   .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [3] "Used": Cheap +1
    { .nameId = TEXT_ID_PREFIX_USED,
      .tier = 1,
      .stat0 = STAT_NONE,                      .val0 =   0,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_CHEAP,                     .val3 =   1 },

    // [4] "Worn": % Speed +5
    { .nameId = TEXT_ID_PREFIX_WORN,
      .tier = 1,
      .stat0 = STAT_SPEED_PERCENT,             .val0 =   5,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [5] "Fisher's": Knockback +5
    { .nameId = TEXT_ID_PREFIX_FISHERS,
      .tier = 1,
      .stat0 = STAT_KNOCKBACK,                 .val0 =   5,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [6] "Red": Damage +1, Strength +1, Health -2
    { .nameId = TEXT_ID_PREFIX_RED,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   1,
      .stat1 = STAT_STRENGTH,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_HEALTH,                    .val3 =  -2 },

    // [7] "Hunter's": Accuracy +1
    { .nameId = TEXT_ID_PREFIX_HUNTERS,
      .tier = 1,
      .stat0 = STAT_ACCURACY,                  .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [8] "Fine": Courage +1, Accuracy +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_FINE,
      .tier = 3,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [9] "White": Courage +1, Spirit Points +10
    { .nameId = TEXT_ID_PREFIX_WHITE,
      .tier = 3,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_MAX_SPIRIT,                .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [10] "Gemstudded": Gemfinder +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_GEMSTUDDED,
      .tier = 5,
      .stat0 = STAT_GEMFINDER,                 .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [11] "Blessed": All Primary Stats +1
    { .nameId = TEXT_ID_PREFIX_BLESSED,
      .tier = 5,
      .stat0 = STAT_ALL_PRIMARY_STATS,         .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [12] "Noble": Courage +2, Valuable +1
    { .nameId = TEXT_ID_PREFIX_NOBLE,
      .tier = 3,
      .stat0 = STAT_COURAGE,                   .val0 =   2,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [13] "Woodsman's": Strength +1, Damage +1, dmg to Animals +1
    { .nameId = TEXT_ID_PREFIX_WOODSMANS,
      .tier = 3,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_DAMAGE_TO_ANIMALS,         .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [14] "Fishscale": Missile Armor +2
    { .nameId = TEXT_ID_PREFIX_FISHSCALE,
      .tier = 3,
      .stat0 = STAT_MISSILE_ARMOR,             .val0 =   2,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [15] "Cursed": Strength +7, Courage -5, Cursed +1
    { .nameId = TEXT_ID_PREFIX_CURSED,
      .tier = 3,
      .stat0 = STAT_STRENGTH,                  .val0 =   7,
      .stat1 = STAT_COURAGE,                   .val1 =  -5,
      .stat2 = STAT_CURSED,                    .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [16] "Uruk": Strength +3, Defense -2
    { .nameId = TEXT_ID_PREFIX_URUK,
      .tier = 3,
      .stat0 = STAT_STRENGTH,                  .val0 =   3,
      .stat1 = STAT_DEFENSE,                   .val1 =  -2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [17] "Bladesman's": Accuracy +2, Damage +1
    { .nameId = TEXT_ID_PREFIX_BLADESMANS,
      .tier = 3,
      .stat0 = STAT_ACCURACY,                  .val0 =   2,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [18] "Sorcerer's": Spirit Points +10, Wisdom +1
    { .nameId = TEXT_ID_PREFIX_SORCERERS,
      .tier = 3,
      .stat0 = STAT_MAX_SPIRIT,                .val0 =  10,
      .stat1 = STAT_WISDOM,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [19] "Umber": dmg to Animals +3, Health +1
    { .nameId = TEXT_ID_PREFIX_UMBER,
      .tier = 3,
      .stat0 = STAT_DAMAGE_TO_ANIMALS,         .val0 =   3,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [20] "Spiderbelly": Hit Points Regen +2, Backstab +1
    { .nameId = TEXT_ID_PREFIX_SPIDERBELLY,
      .tier = 5,
      .stat0 = STAT_HP_REGEN,                  .val0 =   2,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_BACKSTAB,                  .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [21] "Grishnakh's": Strength +2, Defense +2
    { .nameId = TEXT_ID_PREFIX_GRISHNAKHS,
      .tier = 5,
      .stat0 = STAT_STRENGTH,                  .val0 =   2,
      .stat1 = STAT_DEFENSE,                   .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [22] "Isildur's": Courage +2, Strength +2
    { .nameId = TEXT_ID_PREFIX_ISILDURS,
      .tier = 7,
      .stat0 = STAT_COURAGE,                   .val0 =   2,
      .stat1 = STAT_STRENGTH,                  .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [23] "Gil-Galad's": Defense +2, Courage +1, Strength +1
    { .nameId = TEXT_ID_PREFIX_GIL_GALADS,
      .tier = 7,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_COURAGE,                   .val1 =   1,
      .stat2 = STAT_STRENGTH,                  .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [24] "Rune Sewn": Accuracy +1, Courage +1, Spirit Regen +2
    { .nameId = TEXT_ID_PREFIX_RUNE_SEWN,
      .tier = 5,
      .stat0 = STAT_ACCURACY,                  .val0 =   1,
      .stat1 = STAT_COURAGE,                   .val1 =   1,
      .stat2 = STAT_SPIRIT_REGEN,              .val2 =   2,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [25] "Goblin King's": Strength +1, Accuracy +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_GOBLIN_KINGS,
      .tier = 5,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_ACCURACY,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [26] "Black Onyx": Defense +2, Strength +1, % Extra Treasure +25
    { .nameId = TEXT_ID_PREFIX_BLACK_ONYX,
      .tier = 5,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_STRENGTH,                  .val1 =   1,
      .stat2 = STAT_EXTRA_TREASURE_PERCENT,    .val2 =  25,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [27] "Skullcrested": Strength +1, Damage +5, Health -10
    { .nameId = TEXT_ID_PREFIX_SKULLCRESTED,
      .tier = 5,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   5,
      .stat2 = STAT_HEALTH,                    .val2 = -10,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [28] "Fell-hide": Defense +5, Cursed +1
    { .nameId = TEXT_ID_PREFIX_FELL_HIDE,
      .tier = 5,
      .stat0 = STAT_DEFENSE,                   .val0 =   5,
      .stat1 = STAT_CURSED,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [29] "Dwarf Prince": Courage +1, Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_DWARF_PRINCE,
      .tier = 5,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_GEMFINDER,                 .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [30] "Trollskin": Hit Points Regen +2, % Poison Resistance +25
    { .nameId = TEXT_ID_PREFIX_TROLLSKIN,
      .tier = 5,
      .stat0 = STAT_HP_REGEN,                  .val0 =   2,
      .stat1 = STAT_POISON_RESIST_PERCENT,     .val1 =  25,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [31] "Warrior's": Melee Armor +1, Strength +1
    { .nameId = TEXT_ID_PREFIX_WARRIORS,
      .tier = 5,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =   1,
      .stat1 = STAT_STRENGTH,                  .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [32] "Lost Age": Accuracy +1, Courage +1
    { .nameId = TEXT_ID_PREFIX_LOST_AGE,
      .tier = 7,
      .stat0 = STAT_ACCURACY,                  .val0 =   1,
      .stat1 = STAT_COURAGE,                   .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [33] "Dragon's": Strength +4, Defense +4, % Extra Treasure +40
    { .nameId = TEXT_ID_PREFIX_DRAGONS,
      .tier = 7,
      .stat0 = STAT_STRENGTH,                  .val0 =   4,
      .stat1 = STAT_DEFENSE,                   .val1 =   4,
      .stat2 = STAT_EXTRA_TREASURE_PERCENT,    .val2 =  40,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [34] "Tainted": Strength +10, Cursed +1
    { .nameId = TEXT_ID_PREFIX_TAINTED,
      .tier = 9,
      .stat0 = STAT_STRENGTH,                  .val0 =  10,
      .stat1 = STAT_CURSED,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [35] "Elrond's": Accuracy +3, Courage +3, % Corruption Resistance +50
    { .nameId = TEXT_ID_PREFIX_ELRONDS,
      .tier = 9,
      .stat0 = STAT_ACCURACY,                  .val0 =   3,
      .stat1 = STAT_COURAGE,                   .val1 =   3,
      .stat2 = STAT_CORRUPTION_RESIST_PERCENT, .val2 =  50,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [36] "Manflesh": Courage -1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_MANFLESH,
      .tier = 1,
      .stat0 = STAT_COURAGE,                   .val0 =  -1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_CHEAP,                     .val3 =   1 },

    // [37] "Orchide": Health +1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_ORCHIDE,
      .tier = 1,
      .stat0 = STAT_HEALTH,                    .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_CHEAP,                     .val3 =   1 },

    // [38] "Flayed": Strength +1, Accuracy -1
    { .nameId = TEXT_ID_PREFIX_FLAYED,
      .tier = 1,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_ACCURACY,                  .val1 =  -1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [39] "Stained": Accuracy +1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_STAINED,
      .tier = 1,
      .stat0 = STAT_ACCURACY,                  .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_CHEAP,                     .val3 =   1 },

    // [40] "Goblin": Accuracy +1
    { .nameId = TEXT_ID_PREFIX_GOBLIN,
      .tier = 1,
      .stat0 = STAT_ACCURACY,                  .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [41] "Burned": Fire Protection +1
    { .nameId = TEXT_ID_PREFIX_BURNED,
      .tier = 1,
      .stat0 = STAT_FIRE_PROTECTION,           .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [42] "Foul": Strength +3, Damage +1, Cursed +1
    { .nameId = TEXT_ID_PREFIX_FOUL,
      .tier = 1,
      .stat0 = STAT_STRENGTH,                  .val0 =   3,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_CURSED,                    .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [43] "Lurtz's": Hit Points +5, Missile Armor +1
    { .nameId = TEXT_ID_PREFIX_LURTZS,
      .tier = 1,
      .stat0 = STAT_MAX_HP,                    .val0 =   5,
      .stat1 = STAT_MISSILE_ARMOR,             .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [44] "Entbark": Defense +1, % Poison Resistance +10
    { .nameId = TEXT_ID_PREFIX_ENTBARK,
      .tier = 3,
      .stat0 = STAT_DEFENSE,                   .val0 =   1,
      .stat1 = STAT_POISON_RESIST_PERCENT,     .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [45] "Uruk Hai": dmg to Orcs +5
    { .nameId = TEXT_ID_PREFIX_URUK_HAI,
      .tier = 3,
      .stat0 = STAT_DAMAGE_TO_ORCS,            .val0 =   5,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [46] "Arcane": Spirit Regen +4, Wisdom +1
    { .nameId = TEXT_ID_PREFIX_ARCANE,
      .tier = 5,
      .stat0 = STAT_SPIRIT_REGEN,              .val0 =   4,
      .stat1 = STAT_WISDOM,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [47] "Gandalf's": Spirit Regen +3, % Fear Resistance +30, Wisdom +1
    { .nameId = TEXT_ID_PREFIX_GANDALFS,
      .tier = 5,
      .stat0 = STAT_SPIRIT_REGEN,              .val0 =   3,
      .stat1 = STAT_FEAR_RESIST_PERCENT,       .val1 =  30,
      .stat2 = STAT_WISDOM,                    .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [48] "Field Green": Courage +1, Health +1
    { .nameId = TEXT_ID_PREFIX_FIELD_GREEN,
      .tier = 2,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [49] "Rider's": Health +2
    { .nameId = TEXT_ID_PREFIX_RIDERS,
      .tier = 2,
      .stat0 = STAT_HEALTH,                    .val0 =   2,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [50] "Horseman's": Hit Points +5, % Speed +5
    { .nameId = TEXT_ID_PREFIX_HORSEMANS,
      .tier = 2,
      .stat0 = STAT_MAX_HP,                    .val0 =   5,
      .stat1 = STAT_SPEED_PERCENT,             .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [51] "Wildman's": Strength +3, Accuracy -5
    { .nameId = TEXT_ID_PREFIX_WILDMANS,
      .tier = 2,
      .stat0 = STAT_STRENGTH,                  .val0 =   3,
      .stat1 = STAT_ACCURACY,                  .val1 =  -5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [52] "Traveler's": Defense +1, % Extra Treasure +10
    { .nameId = TEXT_ID_PREFIX_TRAVELERS,
      .tier = 2,
      .stat0 = STAT_DEFENSE,                   .val0 =   1,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [53] "Ugluk's": Strength +1, Health +1
    { .nameId = TEXT_ID_PREFIX_UGLUKS,
      .tier = 2,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [54] "Great Orc's": Strength +2, Courage -1
    { .nameId = TEXT_ID_PREFIX_GREAT_ORCS,
      .tier = 2,
      .stat0 = STAT_STRENGTH,                  .val0 =   2,
      .stat1 = STAT_COURAGE,                   .val1 =  -1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [55] "Slaver's": Backstab +1
    { .nameId = TEXT_ID_PREFIX_SLAVERS,
      .tier = 2,
      .stat0 = STAT_BACKSTAB,                  .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [56] "Eomer's": dmg to Uruk-Hai +3, dmg to Orcs +1
    { .nameId = TEXT_ID_PREFIX_EOMERS,
      .tier = 4,
      .stat0 = STAT_DAMAGE_TO_URUKHAI,         .val0 =   3,
      .stat1 = STAT_DAMAGE_TO_ORCS,            .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [57] "Rohirrim": Courage +1, Defense +2
    { .nameId = TEXT_ID_PREFIX_ROHIRRIM,
      .tier = 4,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_DEFENSE,                   .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [58] "Eowyn's": Defense +3
    { .nameId = TEXT_ID_PREFIX_EOWYNS,
      .tier = 6,
      .stat0 = STAT_DEFENSE,                   .val0 =   3,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [59] "Plain King's": Spirit Regen +2, Damage +1
    { .nameId = TEXT_ID_PREFIX_PLAIN_KINGS,
      .tier = 6,
      .stat0 = STAT_SPIRIT_REGEN,              .val0 =   2,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [60] "Spectral": % Poison Resistance +45
    { .nameId = TEXT_ID_PREFIX_SPECTRAL,
      .tier = 4,
      .stat0 = STAT_POISON_RESIST_PERCENT,     .val0 =  45,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [61] "Old King's": % Critical +5, % Fear Resistance +10
    { .nameId = TEXT_ID_PREFIX_OLD_KINGS,
      .tier = 4,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,       .val0 =   5,
      .stat1 = STAT_FEAR_RESIST_PERCENT,       .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [62] "Bone Runed": Damage +2, % Fear Resistance +10
    { .nameId = TEXT_ID_PREFIX_BONE_RUNED,
      .tier = 4,
      .stat0 = STAT_DAMAGE_FLAT,               .val0 =   2,
      .stat1 = STAT_FEAR_RESIST_PERCENT,       .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [63] "Hearty": Courage +1, Hit Points +12, % Poison Resistance +15
    { .nameId = TEXT_ID_PREFIX_HEARTY,
      .tier = 4,
      .stat0 = STAT_COURAGE,                   .val0 =   1,
      .stat1 = STAT_MAX_HP,                    .val1 =  12,
      .stat2 = STAT_POISON_RESIST_PERCENT,     .val2 =  15,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [64] "Cavern": Hit Points +15
    { .nameId = TEXT_ID_PREFIX_CAVERN,
      .tier = 4,
      .stat0 = STAT_MAX_HP,                    .val0 =  15,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [65] "Bleached": Defense +2, % Extra Treasure +20, Valuable +1
    { .nameId = TEXT_ID_PREFIX_BLEACHED,
      .tier = 4,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_NONE,                      .val1 =   1,
      .stat2 = STAT_EXTRA_TREASURE_PERCENT,    .val2 =  20,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [66] "Haunt Skin": Strength +1, Damage +1, Health +1, % Fear Resistance +10
    { .nameId = TEXT_ID_PREFIX_HAUNT_SKIN,
      .tier = 4,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_HEALTH,                    .val2 =   1,
      .stat3 = STAT_FEAR_RESIST_PERCENT,       .val3 =  10 },

    // [67] "Lieutenant's": Strength +2, Courage +1
    { .nameId = TEXT_ID_PREFIX_LIEUTENANTS,
      .tier = 4,
      .stat0 = STAT_STRENGTH,                  .val0 =   2,
      .stat1 = STAT_COURAGE,                   .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [68] "Commander's": Strength +1, Courage +2
    { .nameId = TEXT_ID_PREFIX_COMMANDERS,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_COURAGE,                   .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [69] "Banshee's": Spirit Points +5, % Fear Resistance +80
    { .nameId = TEXT_ID_PREFIX_BANSHEES,
      .tier = 6,
      .stat0 = STAT_MAX_SPIRIT,                .val0 =   5,
      .stat1 = STAT_FEAR_RESIST_PERCENT,       .val1 =  80,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [70] "Fossil Decor": dmg to Nazgul +7
    { .nameId = TEXT_ID_PREFIX_FOSSIL_DECOR,
      .tier = 8,
      .stat0 = STAT_DAMAGE_TO_NAZGUL,          .val0 =   7,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [71] "Ethereal": Spirit Points +35, Armor Piercing +1
    { .nameId = TEXT_ID_PREFIX_ETHEREAL,
      .tier = 8,
      .stat0 = STAT_MAX_SPIRIT,                .val0 =  35,
      .stat1 = STAT_ARMOR_PIERCE,              .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [72] "Mountaineer's": Health +1, Courage +2, Strength +1
    { .nameId = TEXT_ID_PREFIX_MOUNTAINEERS,
      .tier = 5,
      .stat0 = STAT_HEALTH,                    .val0 =   1,
      .stat1 = STAT_COURAGE,                   .val1 =   2,
      .stat2 = STAT_STRENGTH,                  .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [73] "Heir's": Defense +2, Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_HEIRS,
      .tier = 5,
      .stat0 = STAT_DEFENSE,                   .val0 =   2,
      .stat1 = STAT_GEMFINDER,                 .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [74] "Boatman's": Strength +1, Courage +1, % Speed +5
    { .nameId = TEXT_ID_PREFIX_BOATMANS,
      .tier = 5,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_COURAGE,                   .val1 =   1,
      .stat2 = STAT_SPEED_PERCENT,             .val2 =   5,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [75] "Turquoise": Strength +1, Damage +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_TURQUOISE,
      .tier = 5,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   1,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [76] "Pirate's": Accuracy +1, % Extra Treasure +35
    { .nameId = TEXT_ID_PREFIX_PIRATES,
      .tier = 5,
      .stat0 = STAT_ACCURACY,                  .val0 =   1,
      .stat1 = STAT_EXTRA_TREASURE_PERCENT,    .val1 =  35,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [77] "Merchant's": Gemfinder +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_MERCHANTS,
      .tier = 5,
      .stat0 = STAT_GEMFINDER,                 .val0 =   1,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [78] "Fisherman's": Hit Points Regen +3, Spirit Regen +1, Health +3
    { .nameId = TEXT_ID_PREFIX_FISHERMANS,
      .tier = 5,
      .stat0 = STAT_HP_REGEN,                  .val0 =   3,
      .stat1 = STAT_SPIRIT_REGEN,              .val1 =   1,
      .stat2 = STAT_HEALTH,                    .val2 =   3,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [79] "Rugged": Defense +3, Hit Points +15
    { .nameId = TEXT_ID_PREFIX_RUGGED,
      .tier = 5,
      .stat0 = STAT_DEFENSE,                   .val0 =   3,
      .stat1 = STAT_MAX_HP,                    .val1 =  15,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [80] "Snow King's": Fire Protection +4, Health +1
    { .nameId = TEXT_ID_PREFIX_SNOW_KINGS,
      .tier = 7,
      .stat0 = STAT_FIRE_PROTECTION,           .val0 =   4,
      .stat1 = STAT_HEALTH,                    .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [81] "Blood Pirate": Accuracy +1, Strength +1, Damage +2, % Extra Treasure +45
    { .nameId = TEXT_ID_PREFIX_BLOOD_PIRATE,
      .tier = 7,
      .stat0 = STAT_ACCURACY,                  .val0 =   1,
      .stat1 = STAT_STRENGTH,                  .val1 =   1,
      .stat2 = STAT_DAMAGE_FLAT,               .val2 =   2,
      .stat3 = STAT_EXTRA_TREASURE_PERCENT,    .val3 =  45 },

    // [82] "Royal": Hit Points Regen +3, Courage +3, Valuable +1
    { .nameId = TEXT_ID_PREFIX_ROYAL,
      .tier = 9,
      .stat0 = STAT_HP_REGEN,                  .val0 =   3,
      .stat1 = STAT_COURAGE,                   .val1 =   3,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [83] "Illustrious": Hit Points +25, % Hit Points per Slain Foe +10
    { .nameId = TEXT_ID_PREFIX_ILLUSTRIOUS,
      .tier = 9,
      .stat0 = STAT_MAX_HP,                    .val0 =  25,
      .stat1 = STAT_HP_PERCENT_PER_KILL,       .val1 =  10,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [84] "Soldier's": Melee Armor +3, Critical Armor +2
    { .nameId = TEXT_ID_PREFIX_SOLDIERS,
      .tier = 6,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =   3,
      .stat1 = STAT_CRITICAL_ARMOR,            .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [85] "Artillerist's": Missile Armor +3, Damage +1, Defense +1, % Fear Resistance +10
    { .nameId = TEXT_ID_PREFIX_ARTILLERISTS,
      .tier = 6,
      .stat0 = STAT_MISSILE_ARMOR,             .val0 =   3,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   1,
      .stat2 = STAT_DEFENSE,                   .val2 =   1,
      .stat3 = STAT_FEAR_RESIST_PERCENT,       .val3 =  10 },

    // [86] "Pikeman's": Strength +2, Courage +1
    { .nameId = TEXT_ID_PREFIX_PIKEMANS,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =   2,
      .stat1 = STAT_COURAGE,                   .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [87] "Footman's": Strength +1, Courage +2, Defense +1
    { .nameId = TEXT_ID_PREFIX_FOOTMANS,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_COURAGE,                   .val1 =   2,
      .stat2 = STAT_DEFENSE,                   .val2 =   1,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [88] "Burgundy": Health +3, Courage +1
    { .nameId = TEXT_ID_PREFIX_BURGUNDY,
      .tier = 6,
      .stat0 = STAT_HEALTH,                    .val0 =   3,
      .stat1 = STAT_COURAGE,                   .val1 =   1,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [89] "Haradrim": Accuracy +2, Defense +1, % Poison Resistance +10
    { .nameId = TEXT_ID_PREFIX_HARADRIM,
      .tier = 6,
      .stat0 = STAT_ACCURACY,                  .val0 =   2,
      .stat1 = STAT_DEFENSE,                   .val1 =   1,
      .stat2 = STAT_POISON_RESIST_PERCENT,     .val2 =  10,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [90] "Superior": Accuracy +4
    { .nameId = TEXT_ID_PREFIX_SUPERIOR,
      .tier = 6,
      .stat0 = STAT_ACCURACY,                  .val0 =   4,
      .stat1 = STAT_NONE,                      .val1 =   0,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [91] "Cavalier's": Strength +1, Damage +2, Valuable +1
    { .nameId = TEXT_ID_PREFIX_CAVALIERS,
      .tier = 6,
      .stat0 = STAT_STRENGTH,                  .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_VALUABLE,                  .val3 =   1 },

    // [92] "Elite": Strength +2, Defense +2
    { .nameId = TEXT_ID_PREFIX_ELITE,
      .tier = 8,
      .stat0 = STAT_STRENGTH,                  .val0 =   2,
      .stat1 = STAT_DEFENSE,                   .val1 =   2,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [93] "Denethor's": Defense +3, Damage +5, Cursed +1, % Fear Resistance -30
    { .nameId = TEXT_ID_PREFIX_DENETHORS,
      .tier = 8,
      .stat0 = STAT_DEFENSE,                   .val0 =   3,
      .stat1 = STAT_DAMAGE_FLAT,               .val1 =   5,
      .stat2 = STAT_CURSED,                    .val2 =   1,
      .stat3 = STAT_FEAR_RESIST_PERCENT,       .val3 = -30 },

    // [94] "Heroic": Melee Armor +5, Fire Protection +5
    { .nameId = TEXT_ID_PREFIX_HEROIC,
      .tier = 10,
      .stat0 = STAT_MELEE_ARMOR,               .val0 =   5,
      .stat1 = STAT_FIRE_PROTECTION,           .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },

    // [95] "Boromir's": Missile Armor +5, Critical Armor +5
    { .nameId = TEXT_ID_PREFIX_BOROMIRS,
      .tier = 10,
      .stat0 = STAT_MISSILE_ARMOR,             .val0 =   5,
      .stat1 = STAT_CRITICAL_ARMOR,            .val1 =   5,
      .stat2 = STAT_NONE,                      .val2 =   0,
      .stat3 = STAT_NONE,                      .val3 =   0 },
};

/** no decoded bits set (88 of 96 entries) */
#define AFFIX_FLAGS_NONE { 0 }

/**
 * Per-affix flag table; indexed by the affix index. Decoded bits: AFFIX_FLAG_* (item.h).
 *
 * @romaddress 0x080683a8
 */
const AffixFlags GloveShoePrefixFlags[96] = {
    // [0] "Easterling"
    { .d = AFFIX_FLAGS_NONE },

    // [1] "Black"
    { .d = AFFIX_FLAGS_NONE },

    // [2] "Oiled"
    { .d = AFFIX_FLAGS_NONE },

    // [3] "Used"
    { .d = AFFIX_FLAGS_NONE },

    // [4] "Worn"
    { .d = AFFIX_FLAGS_NONE },

    // [5] "Fisher's"
    { .d = AFFIX_FLAGS_NONE },

    // [6] "Red"
    { .d = AFFIX_FLAGS_NONE },

    // [7] "Hunter's"
    { .d = AFFIX_FLAGS_NONE },

    // [8] "Fine"
    { .d = AFFIX_FLAGS_NONE },

    // [9] "White"
    { .d = AFFIX_FLAGS_NONE },

    // [10] "Gemstudded"
    { .d = AFFIX_FLAGS_NONE },

    // [11] "Blessed"
    { .d = AFFIX_FLAGS_NONE },

    // [12] "Noble"
    { .d = AFFIX_FLAGS_NONE },

    // [13] "Woodsman's"
    { .d = AFFIX_FLAGS_NONE },

    // [14] "Fishscale"
    { .d = AFFIX_FLAGS_NONE },

    // [15] "Cursed"
    { .d = AFFIX_FLAGS_NONE },

    // [16] "Uruk"
    { .d = AFFIX_FLAGS_NONE },

    // [17] "Bladesman's"
    { .d = AFFIX_FLAGS_NONE },

    // [18] "Sorcerer's"
    { .d = AFFIX_FLAGS_NONE },

    // [19] "Umber"
    { .d = AFFIX_FLAGS_NONE },

    // [20] "Spiderbelly"
    { .d = { .minLevelPlus5A = 1 } },

    // [21] "Grishnakh's"
    { .d = AFFIX_FLAGS_NONE },

    // [22] "Isildur's"
    { .d = AFFIX_FLAGS_NONE },

    // [23] "Gil-Galad's"
    { .d = AFFIX_FLAGS_NONE },

    // [24] "Rune Sewn"
    { .d = { .minLevelPlus5B = 1 } },

    // [25] "Goblin King's"
    { .d = AFFIX_FLAGS_NONE },

    // [26] "Black Onyx"
    { .d = AFFIX_FLAGS_NONE },

    // [27] "Skullcrested"
    { .d = AFFIX_FLAGS_NONE },

    // [28] "Fell-hide"
    { .d = AFFIX_FLAGS_NONE },

    // [29] "Dwarf Prince"
    { .d = AFFIX_FLAGS_NONE },

    // [30] "Trollskin"
    { .d = { .minLevelPlus5A = 1 } },

    // [31] "Warrior's"
    { .d = AFFIX_FLAGS_NONE },

    // [32] "Lost Age"
    { .d = AFFIX_FLAGS_NONE },

    // [33] "Dragon's"
    { .d = AFFIX_FLAGS_NONE },

    // [34] "Tainted"
    { .d = AFFIX_FLAGS_NONE },

    // [35] "Elrond's"
    { .d = AFFIX_FLAGS_NONE },

    // [36] "Manflesh"
    { .d = AFFIX_FLAGS_NONE },

    // [37] "Orchide"
    { .d = AFFIX_FLAGS_NONE },

    // [38] "Flayed"
    { .d = AFFIX_FLAGS_NONE },

    // [39] "Stained"
    { .d = AFFIX_FLAGS_NONE },

    // [40] "Goblin"
    { .d = AFFIX_FLAGS_NONE },

    // [41] "Burned"
    { .d = AFFIX_FLAGS_NONE },

    // [42] "Foul"
    { .d = AFFIX_FLAGS_NONE },

    // [43] "Lurtz's"
    { .d = AFFIX_FLAGS_NONE },

    // [44] "Entbark"
    { .d = AFFIX_FLAGS_NONE },

    // [45] "Uruk Hai"
    { .d = AFFIX_FLAGS_NONE },

    // [46] "Arcane"
    { .d = { .minLevelPlus5B = 1 } },

    // [47] "Gandalf's"
    { .d = { .minLevelPlus5B = 1 } },

    // [48] "Field Green"
    { .d = AFFIX_FLAGS_NONE },

    // [49] "Rider's"
    { .d = AFFIX_FLAGS_NONE },

    // [50] "Horseman's"
    { .d = AFFIX_FLAGS_NONE },

    // [51] "Wildman's"
    { .d = AFFIX_FLAGS_NONE },

    // [52] "Traveler's"
    { .d = AFFIX_FLAGS_NONE },

    // [53] "Ugluk's"
    { .d = AFFIX_FLAGS_NONE },

    // [54] "Great Orc's"
    { .d = AFFIX_FLAGS_NONE },

    // [55] "Slaver's"
    { .d = AFFIX_FLAGS_NONE },

    // [56] "Eomer's"
    { .d = AFFIX_FLAGS_NONE },

    // [57] "Rohirrim"
    { .d = AFFIX_FLAGS_NONE },

    // [58] "Eowyn's"
    { .d = AFFIX_FLAGS_NONE },

    // [59] "Plain King's"
    { .d = { .minLevelPlus5B = 1 } },

    // [60] "Spectral"
    { .d = AFFIX_FLAGS_NONE },

    // [61] "Old King's"
    { .d = AFFIX_FLAGS_NONE },

    // [62] "Bone Runed"
    { .d = AFFIX_FLAGS_NONE },

    // [63] "Hearty"
    { .d = AFFIX_FLAGS_NONE },

    // [64] "Cavern"
    { .d = AFFIX_FLAGS_NONE },

    // [65] "Bleached"
    { .d = AFFIX_FLAGS_NONE },

    // [66] "Haunt Skin"
    { .d = AFFIX_FLAGS_NONE },

    // [67] "Lieutenant's"
    { .d = AFFIX_FLAGS_NONE },

    // [68] "Commander's"
    { .d = AFFIX_FLAGS_NONE },

    // [69] "Banshee's"
    { .d = AFFIX_FLAGS_NONE },

    // [70] "Fossil Decor"
    { .d = AFFIX_FLAGS_NONE },

    // [71] "Ethereal"
    { .d = AFFIX_FLAGS_NONE },

    // [72] "Mountaineer's"
    { .d = AFFIX_FLAGS_NONE },

    // [73] "Heir's"
    { .d = AFFIX_FLAGS_NONE },

    // [74] "Boatman's"
    { .d = AFFIX_FLAGS_NONE },

    // [75] "Turquoise"
    { .d = AFFIX_FLAGS_NONE },

    // [76] "Pirate's"
    { .d = AFFIX_FLAGS_NONE },

    // [77] "Merchant's"
    { .d = AFFIX_FLAGS_NONE },

    // [78] "Fisherman's"
    { .d = { .minLevelPlus5A = 1,
             .minLevelPlus5B = 1 } },

    // [79] "Rugged"
    { .d = AFFIX_FLAGS_NONE },

    // [80] "Snow King's"
    { .d = AFFIX_FLAGS_NONE },

    // [81] "Blood Pirate"
    { .d = AFFIX_FLAGS_NONE },

    // [82] "Royal"
    { .d = { .minLevelPlus5A = 1 } },

    // [83] "Illustrious"
    { .d = AFFIX_FLAGS_NONE },

    // [84] "Soldier's"
    { .d = AFFIX_FLAGS_NONE },

    // [85] "Artillerist's"
    { .d = AFFIX_FLAGS_NONE },

    // [86] "Pikeman's"
    { .d = AFFIX_FLAGS_NONE },

    // [87] "Footman's"
    { .d = AFFIX_FLAGS_NONE },

    // [88] "Burgundy"
    { .d = AFFIX_FLAGS_NONE },

    // [89] "Haradrim"
    { .d = AFFIX_FLAGS_NONE },

    // [90] "Superior"
    { .d = AFFIX_FLAGS_NONE },

    // [91] "Cavalier's"
    { .d = AFFIX_FLAGS_NONE },

    // [92] "Elite"
    { .d = AFFIX_FLAGS_NONE },

    // [93] "Denethor's"
    { .d = AFFIX_FLAGS_NONE },

    // [94] "Heroic"
    { .d = AFFIX_FLAGS_NONE },

    // [95] "Boromir's"
    { .d = AFFIX_FLAGS_NONE },
};
#undef AFFIX_FLAGS_NONE
// clang-format on
