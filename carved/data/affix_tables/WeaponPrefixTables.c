#include "item.h"
#include "stats.h"
#include "text_ids.h"

// clang-format off

// TODO: [29] "Elven" carries .val1=5 on an empty (STAT_NONE) slot - meaning unknown

/** @romaddress 0x080656b8 */
const ItemAffix WeaponPrefixes[156] = {
    // [0] "Bronze": Damage -1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_BRONZE,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =  -1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_CHEAP,                 .val3 =   1 },

    // [1] "Cumbersome": Damage +1, Accuracy -2
    { .nameId = TEXT_ID_PREFIX_CUMBERSOME,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_ACCURACY,              .val1 =  -2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [2] "Goblin": Damage -1
    { .nameId = TEXT_ID_PREFIX_GOBLIN,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =  -1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [3] "Black": Damage +1
    { .nameId = TEXT_ID_PREFIX_BLACK,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [4] "Rusted": Damage +5, Fragile +1
    { .nameId = TEXT_ID_PREFIX_RUSTED,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   5,
      .stat1 = STAT_FRAGILE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [5] "Stained": Strength +1
    { .nameId = TEXT_ID_PREFIX_STAINED,
      .tier = 1,
      .stat0 = STAT_STRENGTH,              .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [6] "Cheap": Fragile +0, Cheap +1
    //   BUG: stat0 - Fragile only procs from a prefix's stat1, see combat_fragileOnKill
    { .nameId = TEXT_ID_PREFIX_CHEAP,
      .tier = 1,
      .stat0 = STAT_FRAGILE,               .val0 =   0, // BUG: never procs if not in stat1, see combat_fragileOnKill
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_CHEAP,                 .val3 =   1 },

    // [7] "Bloody": Health +1
    { .nameId = TEXT_ID_PREFIX_BLOODY,
      .tier = 1,
      .stat0 = STAT_HEALTH,                .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [8] "Ebony": Defense +3
    { .nameId = TEXT_ID_PREFIX_EBONY,
      .tier = 3,
      .stat0 = STAT_DEFENSE,               .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [9] "Obsidian": Damage +9, Defense +3, Fragile +1
    //   BUG: stat2 - Fragile only procs from a prefix's stat1, see combat_fragileOnKill
    { .nameId = TEXT_ID_PREFIX_OBSIDIAN,
      .tier = 3,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   9,
      .stat1 = STAT_DEFENSE,               .val1 =   3,
      .stat2 = STAT_FRAGILE,               .val2 =   1, // BUG: never procs if not in stat1, see combat_fragileOnKill
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [10] "Seeping": Damage +1, Venomous +1
    { .nameId = TEXT_ID_PREFIX_SEEPING,
      .tier = 5,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_VENOMOUS,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [11] "Darkmoon": Accuracy +3, Spirit Regen +3
    { .nameId = TEXT_ID_PREFIX_DARKMOON,
      .tier = 5,
      .stat0 = STAT_ACCURACY,              .val0 =   3,
      .stat1 = STAT_SPIRIT_REGEN,          .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [12] "Fired": Damage +2
    { .nameId = TEXT_ID_PREFIX_FIRED,
      .tier = 2,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [13] "Ancient": Courage +2
    { .nameId = TEXT_ID_PREFIX_ANCIENT,
      .tier = 2,
      .stat0 = STAT_COURAGE,               .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [14] "Iron": Damage +1, Accuracy -1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_IRON,
      .tier = 2,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_ACCURACY,              .val1 =  -1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_CHEAP,                 .val3 =   1 },

    // [15] "Old": Damage +7, Fragile +1, Accuracy +5
    { .nameId = TEXT_ID_PREFIX_OLD,
      .tier = 2,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   7,
      .stat1 = STAT_FRAGILE,               .val1 =   1,
      .stat2 = STAT_ACCURACY,              .val2 =   5,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [16] "Ragged": Damage +3, Accuracy -2
    { .nameId = TEXT_ID_PREFIX_RAGGED,
      .tier = 2,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   3,
      .stat1 = STAT_ACCURACY,              .val1 =  -2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [17] "Assassin's": % Critical +5, Backstab +1
    { .nameId = TEXT_ID_PREFIX_ASSASSINS,
      .tier = 2,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,   .val0 =   5,
      .stat1 = STAT_BACKSTAB,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [18] "Hunter's": Accuracy +1
    { .nameId = TEXT_ID_PREFIX_HUNTERS,
      .tier = 2,
      .stat0 = STAT_ACCURACY,              .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [19] "Uruk": Damage +1, Strength +1
    { .nameId = TEXT_ID_PREFIX_URUK,
      .tier = 2,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [20] "Udun": Strength +2, Defense +1, Courage +1
    { .nameId = TEXT_ID_PREFIX_UDUN,
      .tier = 4,
      .stat0 = STAT_STRENGTH,              .val0 =   2,
      .stat1 = STAT_DEFENSE,               .val1 =   1,
      .stat2 = STAT_COURAGE,               .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [21] "Master's": Accuracy +3
    { .nameId = TEXT_ID_PREFIX_MASTERS,
      .tier = 4,
      .stat0 = STAT_ACCURACY,              .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [22] "Scarred": Hit Points +25, Courage +4
    { .nameId = TEXT_ID_PREFIX_SCARRED,
      .tier = 6,
      .stat0 = STAT_MAX_HP,                .val0 =  25,
      .stat1 = STAT_COURAGE,               .val1 =   4,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [23] "Skull King's": Damage +5, Cursed +1
    { .nameId = TEXT_ID_PREFIX_SKULL_KINGS,
      .tier = 6,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   5,
      .stat1 = STAT_CURSED,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [24] "Spider-Iron": Defense +1, dmg to Animals +1, Immobilize +1
    { .nameId = TEXT_ID_PREFIX_SPIDER_IRON,
      .tier = 3,
      .stat0 = STAT_DEFENSE,               .val0 =   1,
      .stat1 = STAT_DAMAGE_TO_ANIMALS,     .val1 =   1,
      .stat2 = STAT_IMMOBILIZE,            .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [25] "Arachnid": Defense +1, Venomous +1
    { .nameId = TEXT_ID_PREFIX_ARACHNID,
      .tier = 3,
      .stat0 = STAT_DEFENSE,               .val0 =   1,
      .stat1 = STAT_VENOMOUS,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [26] "Venomous": Immobilize +1, Venomous +1
    { .nameId = TEXT_ID_PREFIX_VENOMOUS,
      .tier = 3,
      .stat0 = STAT_IMMOBILIZE,            .val0 =   1,
      .stat1 = STAT_VENOMOUS,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [27] "Poisoned": Damage +2, Venomous +1
    { .nameId = TEXT_ID_PREFIX_POISONED,
      .tier = 3,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_VENOMOUS,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [28] "Crimson": Damage +2
    { .nameId = TEXT_ID_PREFIX_CRIMSON,
      .tier = 3,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [29] "Elven": dmg to Orcs +3
    { .nameId = TEXT_ID_PREFIX_ELVEN,
      .tier = 3,
      .stat0 = STAT_DAMAGE_TO_ORCS,        .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   5,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [30] "Green": Courage +2, Damage +1, Strength -2
    { .nameId = TEXT_ID_PREFIX_GREEN,
      .tier = 3,
      .stat0 = STAT_COURAGE,               .val0 =   2,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   1,
      .stat2 = STAT_STRENGTH,              .val2 =  -2,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [31] "Strengthened": Strength +1, Knockback +10
    { .nameId = TEXT_ID_PREFIX_STRENGTHENED,
      .tier = 3,
      .stat0 = STAT_STRENGTH,              .val0 =   1,
      .stat1 = STAT_KNOCKBACK,             .val1 =  10,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [32] "Opal": Strength +1, Wisdom +3
    { .nameId = TEXT_ID_PREFIX_OPAL,
      .tier = 5,
      .stat0 = STAT_STRENGTH,              .val0 =   1,
      .stat1 = STAT_WISDOM,                .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [33] "Vile": Health -10, Damage +7, Venomous +1
    { .nameId = TEXT_ID_PREFIX_VILE,
      .tier = 5,
      .stat0 = STAT_HEALTH,                .val0 = -10,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   7,
      .stat2 = STAT_VENOMOUS,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [34] "Razor-edged": Damage +4, Melee Damage +2
    { .nameId = TEXT_ID_PREFIX_RAZOR_EDGED,
      .tier = 7,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   4,
      .stat1 = STAT_DAMAGE_SLASH,          .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [35] "Mithril": Damage +3, Armor Piercing +1, dmg to Orcs +3
    { .nameId = TEXT_ID_PREFIX_MITHRIL,
      .tier = 7,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   3,
      .stat1 = STAT_ARMOR_PIERCE,          .val1 =   1,
      .stat2 = STAT_DAMAGE_TO_ORCS,        .val2 =   3,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [36] "Cold": Damage +2, Immobilize +1
    { .nameId = TEXT_ID_PREFIX_COLD,
      .tier = 4,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_IMMOBILIZE,            .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [37] "Olog's": Strength +3
    { .nameId = TEXT_ID_PREFIX_OLOGS,
      .tier = 4,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [38] "Sooty": Courage +1, Defense +2
    { .nameId = TEXT_ID_PREFIX_SOOTY,
      .tier = 4,
      .stat0 = STAT_COURAGE,               .val0 =   1,
      .stat1 = STAT_DEFENSE,               .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [39] "Volcanic": Health +1, Fire Damage +1, Fireburst +1
    { .nameId = TEXT_ID_PREFIX_VOLCANIC,
      .tier = 4,
      .stat0 = STAT_HEALTH,                .val0 =   1,
      .stat1 = STAT_DAMAGE_FIRE,           .val1 =   1,
      .stat2 = STAT_FIREBURST,             .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [40] "Spiked": Damage +3
    { .nameId = TEXT_ID_PREFIX_SPIKED,
      .tier = 4,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [41] "Glass": Damage +11, Fragile +1
    { .nameId = TEXT_ID_PREFIX_GLASS,
      .tier = 4,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =  11,
      .stat1 = STAT_FRAGILE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [42] "Regenerating": Hit Points Regen +2
    { .nameId = TEXT_ID_PREFIX_REGENERATING,
      .tier = 4,
      .stat0 = STAT_HP_REGEN,              .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [43] "Easterling": Melee Damage +2, Strength +4, Courage -1
    { .nameId = TEXT_ID_PREFIX_EASTERLING,
      .tier = 4,
      .stat0 = STAT_DAMAGE_SLASH,          .val0 =   2,
      .stat1 = STAT_STRENGTH,              .val1 =   4,
      .stat2 = STAT_COURAGE,               .val2 =  -1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [44] "Orc Bane": dmg to Orcs +5
    { .nameId = TEXT_ID_PREFIX_ORC_BANE,
      .tier = 6,
      .stat0 = STAT_DAMAGE_TO_ORCS,        .val0 =   5,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [45] "Bonewhite": Damage +1, Hit Points Regen +2
    { .nameId = TEXT_ID_PREFIX_BONEWHITE,
      .tier = 6,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_HP_REGEN,              .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [46] "Nazgul's": Damage +6, Poison Damage +1, Cursed +1
    { .nameId = TEXT_ID_PREFIX_NAZGULS,
      .tier = 8,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   6,
      .stat1 = STAT_DAMAGE_POISON,         .val1 =   1,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [47] "Blackflame": All Primary Stats +3, Fire Damage +2, Fireburst +1
    { .nameId = TEXT_ID_PREFIX_BLACKFLAME,
      .tier = 8,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   3,
      .stat1 = STAT_DAMAGE_FIRE,           .val1 =   2,
      .stat2 = STAT_FIREBURST,             .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [48] "Red": Strength +3, Damage +1
    { .nameId = TEXT_ID_PREFIX_RED,
      .tier = 5,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [49] "Piercing": Armor Piercing +1, % Critical +5, Melee Damage +1
    { .nameId = TEXT_ID_PREFIX_PIERCING,
      .tier = 5,
      .stat0 = STAT_ARMOR_PIERCE,          .val0 =   1,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,   .val1 =   5,
      .stat2 = STAT_DAMAGE_SLASH,          .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [50] "Smith's": Damage +1, % Critical +10
    { .nameId = TEXT_ID_PREFIX_SMITHS,
      .tier = 5,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,   .val1 =  10,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [51] "Dark": Courage -10, Accuracy +5, Strength +3, Health +3
    { .nameId = TEXT_ID_PREFIX_DARK,
      .tier = 5,
      .stat0 = STAT_COURAGE,               .val0 = -10,
      .stat1 = STAT_ACCURACY,              .val1 =   5,
      .stat2 = STAT_STRENGTH,              .val2 =   3,
      .stat3 = STAT_HEALTH,                .val3 =   3 },

    // [52] "Mottled": % Critical +5, Knockback +10, Courage +1
    { .nameId = TEXT_ID_PREFIX_MOTTLED,
      .tier = 5,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,   .val0 =   5,
      .stat1 = STAT_KNOCKBACK,             .val1 =  10,
      .stat2 = STAT_COURAGE,               .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [53] "Amber": Defense +3, Immobilize +1
    { .nameId = TEXT_ID_PREFIX_AMBER,
      .tier = 5,
      .stat0 = STAT_DEFENSE,               .val0 =   3,
      .stat1 = STAT_IMMOBILIZE,            .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [54] "Ember": Damage +1, Fireburst +1
    { .nameId = TEXT_ID_PREFIX_EMBER,
      .tier = 5,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_FIREBURST,             .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [55] "Sorcerer's": Wisdom +4
    { .nameId = TEXT_ID_PREFIX_SORCERERS,
      .tier = 5,
      .stat0 = STAT_WISDOM,                .val0 =   4,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [56] "Gil-Galad's": All Primary Stats +1
    { .nameId = TEXT_ID_PREFIX_GIL_GALADS,
      .tier = 7,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [57] "Enormous": Damage +7, % Speed -10
    { .nameId = TEXT_ID_PREFIX_ENORMOUS,
      .tier = 7,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   7,
      .stat1 = STAT_SPEED_PERCENT,         .val1 = -10,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [58] "Corrupt": All Primary Stats +3, Nightburst +1, Cursed +1
    { .nameId = TEXT_ID_PREFIX_CORRUPT,
      .tier = 9,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   3,
      .stat1 = STAT_NIGHTBURST,            .val1 =   1,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [59] "Vorpal": Damage +4, % Critical +15
    { .nameId = TEXT_ID_PREFIX_VORPAL,
      .tier = 9,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   4,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,   .val1 =  15,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [60] "Ruby": Courage +2, Accuracy +1, Sunburst +1
    { .nameId = TEXT_ID_PREFIX_RUBY,
      .tier = 6,
      .stat0 = STAT_COURAGE,               .val0 =   2,
      .stat1 = STAT_ACCURACY,              .val1 =   1,
      .stat2 = STAT_SUNBURST,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [61] "Furnace": Strength +1, Damage +2, Fire Damage +1, Fireburst +1
    { .nameId = TEXT_ID_PREFIX_FURNACE,
      .tier = 6,
      .stat0 = STAT_STRENGTH,              .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   2,
      .stat2 = STAT_DAMAGE_FIRE,           .val2 =   1,
      .stat3 = STAT_FIREBURST,             .val3 =   1 },

    // [62] "Slayer's": Damage +1, % Critical +5, Nightburst +1
    { .nameId = TEXT_ID_PREFIX_SLAYERS,
      .tier = 6,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,   .val1 =   5,
      .stat2 = STAT_NIGHTBURST,            .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [63] "Perfect": All Primary Stats +1
    { .nameId = TEXT_ID_PREFIX_PERFECT,
      .tier = 6,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [64] "Shadowed": All Primary Stats +2, Nightburst +1, Cursed +1
    { .nameId = TEXT_ID_PREFIX_SHADOWED,
      .tier = 6,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   2,
      .stat1 = STAT_NIGHTBURST,            .val1 =   1,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [65] "Antique": Accuracy +3, Damage +1
    { .nameId = TEXT_ID_PREFIX_ANTIQUE,
      .tier = 6,
      .stat0 = STAT_ACCURACY,              .val0 =   3,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [66] "Molten": Hit Points per Slain Foe -3, Sunburst +1, Fireburst +1
    { .nameId = TEXT_ID_PREFIX_MOLTEN,
      .tier = 6,
      .stat0 = STAT_HP_PER_KILL,           .val0 =  -3,
      .stat1 = STAT_SUNBURST,              .val1 =   1,
      .stat2 = STAT_FIREBURST,             .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [67] "Fell": All Primary Stats +3, dmg to Animals +6, Cursed +1
    { .nameId = TEXT_ID_PREFIX_FELL,
      .tier = 6,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   3,
      .stat1 = STAT_DAMAGE_TO_ANIMALS,     .val1 =   6,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [68] "Lord's": Strength +4
    { .nameId = TEXT_ID_PREFIX_LORDS,
      .tier = 8,
      .stat0 = STAT_STRENGTH,              .val0 =   4,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [69] "Elrond's": Courage +5
    { .nameId = TEXT_ID_PREFIX_ELRONDS,
      .tier = 8,
      .stat0 = STAT_COURAGE,               .val0 =   5,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [70] "Isildur's": Courage -10, Strength +10, Melee Damage +2, % Critical +10
    { .nameId = TEXT_ID_PREFIX_ISILDURS,
      .tier = 10,
      .stat0 = STAT_COURAGE,               .val0 = -10,
      .stat1 = STAT_STRENGTH,              .val1 =  10,
      .stat2 = STAT_DAMAGE_SLASH,          .val2 =   2,
      .stat3 = STAT_CRIT_CHANCE_PERCENT,   .val3 =  10 },

    // [71] "Elendil's": Hit Points +50
    { .nameId = TEXT_ID_PREFIX_ELENDILS,
      .tier = 10,
      .stat0 = STAT_MAX_HP,                .val0 =  50,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [72] "Tarnished": Damage +3, Fragile +1
    { .nameId = TEXT_ID_PREFIX_TARNISHED,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   3,
      .stat1 = STAT_FRAGILE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [73] "Tainted": Damage +2, Cursed +1
    { .nameId = TEXT_ID_PREFIX_TAINTED,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_CURSED,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [74] "Grunt's": Damage +1
    { .nameId = TEXT_ID_PREFIX_GRUNTS,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [75] "Ruddy": Strength +1
    { .nameId = TEXT_ID_PREFIX_RUDDY,
      .tier = 1,
      .stat0 = STAT_STRENGTH,              .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [76] "Oversized": Damage +1, % Speed -5
    { .nameId = TEXT_ID_PREFIX_OVERSIZED,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_SPEED_PERCENT,         .val1 =  -5,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [77] "Dirtied": Cheap +1
    { .nameId = TEXT_ID_PREFIX_DIRTIED,
      .tier = 1,
      .stat0 = STAT_NONE,                  .val0 =   0,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_CHEAP,                 .val3 =   1 },

    // [78] "Warped": Accuracy -4, Strength +2, Damage +2
    { .nameId = TEXT_ID_PREFIX_WARPED,
      .tier = 1,
      .stat0 = STAT_ACCURACY,              .val0 =  -4,
      .stat1 = STAT_STRENGTH,              .val1 =   2,
      .stat2 = STAT_DAMAGE_FLAT,           .val2 =   2,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [79] "Ent Trodden": Hit Points +10
    { .nameId = TEXT_ID_PREFIX_ENT_TRODDEN,
      .tier = 1,
      .stat0 = STAT_MAX_HP,                .val0 =  10,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [80] "White": Courage +2, Hit Points +5
    { .nameId = TEXT_ID_PREFIX_WHITE,
      .tier = 3,
      .stat0 = STAT_COURAGE,               .val0 =   2,
      .stat1 = STAT_MAX_HP,                .val1 =   5,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [81] "Orthanc": Courage +3, Hit Points +20, Cursed +1
    { .nameId = TEXT_ID_PREFIX_ORTHANC,
      .tier = 3,
      .stat0 = STAT_COURAGE,               .val0 =   3,
      .stat1 = STAT_MAX_HP,                .val1 =  20,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [82] "Orc Slaying": dmg to Orcs +4
    { .nameId = TEXT_ID_PREFIX_ORC_SLAYING,
      .tier = 5,
      .stat0 = STAT_DAMAGE_TO_ORCS,        .val0 =   4,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [83] "Lurtz's": Defense -2, Strength +2, Hit Points +10
    { .nameId = TEXT_ID_PREFIX_LURTZS,
      .tier = 5,
      .stat0 = STAT_DEFENSE,               .val0 =  -2,
      .stat1 = STAT_STRENGTH,              .val1 =   2,
      .stat2 = STAT_MAX_HP,                .val2 =  10,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [84] "Wanderer's": Hit Points +10
    { .nameId = TEXT_ID_PREFIX_WANDERERS,
      .tier = 2,
      .stat0 = STAT_MAX_HP,                .val0 =  10,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [85] "Ally's": Damage +1, Courage +1
    { .nameId = TEXT_ID_PREFIX_ALLYS,
      .tier = 2,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [86] "Lancer's": dmg to Orcs +2
    { .nameId = TEXT_ID_PREFIX_LANCERS,
      .tier = 2,
      .stat0 = STAT_DAMAGE_TO_ORCS,        .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [87] "Blacksmith's": Health +1, Damage +1
    { .nameId = TEXT_ID_PREFIX_BLACKSMITHS,
      .tier = 2,
      .stat0 = STAT_HEALTH,                .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [88] "Maiden's": All Primary Stats +1
    { .nameId = TEXT_ID_PREFIX_MAIDENS,
      .tier = 2,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [89] "Sharpened": Damage +2
    { .nameId = TEXT_ID_PREFIX_SHARPENED,
      .tier = 2,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [90] "Polished": % Experience Bonus +35, Fragile +1
    { .nameId = TEXT_ID_PREFIX_POLISHED,
      .tier = 2,
      .stat0 = STAT_EXTRA_EXP_PERCENT,     .val0 =  35,
      .stat1 = STAT_FRAGILE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [91] "Rohirrim": Hit Points +10, Courage +1
    { .nameId = TEXT_ID_PREFIX_ROHIRRIM,
      .tier = 2,
      .stat0 = STAT_MAX_HP,                .val0 =  10,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [92] "Eomer's": Accuracy +1, Strength +2
    { .nameId = TEXT_ID_PREFIX_EOMERS,
      .tier = 4,
      .stat0 = STAT_ACCURACY,              .val0 =   1,
      .stat1 = STAT_STRENGTH,              .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [93] "Eowyn's": Defense +3
    { .nameId = TEXT_ID_PREFIX_EOWYNS,
      .tier = 4,
      .stat0 = STAT_DEFENSE,               .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [94] "Fearless": % Fear Resistance +50
    { .nameId = TEXT_ID_PREFIX_FEARLESS,
      .tier = 6,
      .stat0 = STAT_FEAR_RESIST_PERCENT,   .val0 =  50,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [95] "Exotic": All Primary Stats +2, Valuable +1
    { .nameId = TEXT_ID_PREFIX_EXOTIC,
      .tier = 6,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [96] "Miner's": dmg to Orcs +2, Health +1
    { .nameId = TEXT_ID_PREFIX_MINERS,
      .tier = 3,
      .stat0 = STAT_DAMAGE_TO_ORCS,        .val0 =   2,
      .stat1 = STAT_HEALTH,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [97] "Brown": dmg to Animals +3
    { .nameId = TEXT_ID_PREFIX_BROWN,
      .tier = 3,
      .stat0 = STAT_DAMAGE_TO_ANIMALS,     .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [98] "Guidesman's": dmg to Orcs +2, dmg to Animals +2
    { .nameId = TEXT_ID_PREFIX_GUIDESMANS,
      .tier = 3,
      .stat0 = STAT_DAMAGE_TO_ORCS,        .val0 =   2,
      .stat1 = STAT_DAMAGE_TO_ANIMALS,     .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [99] "Sigiled": All Primary Stats +1
    { .nameId = TEXT_ID_PREFIX_SIGILED,
      .tier = 3,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [100] "Runic": Strength +1, Courage +1
    { .nameId = TEXT_ID_PREFIX_RUNIC,
      .tier = 3,
      .stat0 = STAT_STRENGTH,              .val0 =   1,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [101] "Harrowed": Damage -2, dmg to Nazgul +5
    { .nameId = TEXT_ID_PREFIX_HARROWED,
      .tier = 3,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =  -2,
      .stat1 = STAT_DAMAGE_TO_NAZGUL,      .val1 =   5,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [102] "Heavy": Defense -2, Damage +3
    { .nameId = TEXT_ID_PREFIX_HEAVY,
      .tier = 3,
      .stat0 = STAT_DEFENSE,               .val0 =  -2,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [103] "Honed": Damage +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_HONED,
      .tier = 3,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [104] "Storm-forged": Sunburst +1
    { .nameId = TEXT_ID_PREFIX_STORM_FORGED,
      .tier = 5,
      .stat0 = STAT_SUNBURST,              .val0 =   1,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [105] "Nightmare": All Primary Stats +2, % Fear Resistance -20
    { .nameId = TEXT_ID_PREFIX_NIGHTMARE,
      .tier = 5,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   2,
      .stat1 = STAT_FEAR_RESIST_PERCENT,   .val1 = -20,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [106] "Elf Queen's": Spirit Points +40
    { .nameId = TEXT_ID_PREFIX_ELF_QUEENS,
      .tier = 7,
      .stat0 = STAT_MAX_SPIRIT,            .val0 =  40,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [107] "Warrior's": Strength +3, Accuracy +2
    { .nameId = TEXT_ID_PREFIX_WARRIORS,
      .tier = 7,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_ACCURACY,              .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [108] "Cursed": Damage +4, Melee Armor -3, Cursed +1, Strength +3
    { .nameId = TEXT_ID_PREFIX_CURSED,
      .tier = 4,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   4,
      .stat1 = STAT_MELEE_ARMOR,           .val1 =  -3,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_STRENGTH,              .val3 =   3 },

    // [109] "Jagged": Damage +2
    { .nameId = TEXT_ID_PREFIX_JAGGED,
      .tier = 4,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [110] "Glowing": dmg to Orcs +3, Courage +1
    { .nameId = TEXT_ID_PREFIX_GLOWING,
      .tier = 4,
      .stat0 = STAT_DAMAGE_TO_ORCS,        .val0 =   3,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [111] "Vengeance": Courage +1, Backstab +1
    { .nameId = TEXT_ID_PREFIX_VENGEANCE,
      .tier = 4,
      .stat0 = STAT_COURAGE,               .val0 =   1,
      .stat1 = STAT_BACKSTAB,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [112] "Frog's": Hit Points Regen +2, Hit Points +5
    { .nameId = TEXT_ID_PREFIX_FROGS,
      .tier = 4,
      .stat0 = STAT_HP_REGEN,              .val0 =   2,
      .stat1 = STAT_MAX_HP,                .val1 =   5,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [113] "Ruthless": Damage +2, Backstab +1
    { .nameId = TEXT_ID_PREFIX_RUTHLESS,
      .tier = 4,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_BACKSTAB,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [114] "Cobalt": Defense +3
    { .nameId = TEXT_ID_PREFIX_COBALT,
      .tier = 4,
      .stat0 = STAT_DEFENSE,               .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [115] "Serpent's": Defense +1, Hit Points +15
    { .nameId = TEXT_ID_PREFIX_SERPENTS,
      .tier = 4,
      .stat0 = STAT_DEFENSE,               .val0 =   1,
      .stat1 = STAT_MAX_HP,                .val1 =  15,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [116] "Azure": Damage +2, Hit Points +30
    { .nameId = TEXT_ID_PREFIX_AZURE,
      .tier = 6,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_MAX_HP,                .val1 =  30,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [117] "Ghostmetal": Damage +5, Armor Piercing +1, Hit Points per Slain Foe -5
    { .nameId = TEXT_ID_PREFIX_GHOSTMETAL,
      .tier = 6,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   5,
      .stat1 = STAT_ARMOR_PIERCE,          .val1 =   1,
      .stat2 = STAT_HP_PER_KILL,           .val2 =  -5,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [118] "Ethereal": Damage +4, Armor Piercing +1
    { .nameId = TEXT_ID_PREFIX_ETHEREAL,
      .tier = 8,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   4,
      .stat1 = STAT_ARMOR_PIERCE,          .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [119] "Platinum": Damage +3, Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_PLATINUM,
      .tier = 8,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   3,
      .stat1 = STAT_GEMFINDER,             .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [120] "Engraved": Valuable +1
    { .nameId = TEXT_ID_PREFIX_ENGRAVED,
      .tier = 5,
      .stat0 = STAT_NONE,                  .val0 =   0,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [121] "Dwarf Maiden": Accuracy +1, Courage +1, Strength +1
    { .nameId = TEXT_ID_PREFIX_DWARF_MAIDEN,
      .tier = 5,
      .stat0 = STAT_ACCURACY,              .val0 =   1,
      .stat1 = STAT_COURAGE,               .val1 =   1,
      .stat2 = STAT_STRENGTH,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [122] "Elder": dmg to Orcs +1, % Experience Bonus +5
    { .nameId = TEXT_ID_PREFIX_ELDER,
      .tier = 5,
      .stat0 = STAT_DAMAGE_TO_ORCS,        .val0 =   1,
      .stat1 = STAT_EXTRA_EXP_PERCENT,     .val1 =   5,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [123] "Grey": Courage +3
    { .nameId = TEXT_ID_PREFIX_GREY,
      .tier = 5,
      .stat0 = STAT_COURAGE,               .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [124] "Snowbird's": Strength +1, % Speed +10
    { .nameId = TEXT_ID_PREFIX_SNOWBIRDS,
      .tier = 5,
      .stat0 = STAT_STRENGTH,              .val0 =   1,
      .stat1 = STAT_SPEED_PERCENT,         .val1 =  10,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [125] "Frozen": Damage +1, Hit Points +25, Immobilize +1
    { .nameId = TEXT_ID_PREFIX_FROZEN,
      .tier = 5,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   1,
      .stat1 = STAT_MAX_HP,                .val1 =  25,
      .stat2 = STAT_IMMOBILIZE,            .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [126] "Crystalline": Damage +11, Fragile +1
    { .nameId = TEXT_ID_PREFIX_CRYSTALLINE,
      .tier = 5,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =  11,
      .stat1 = STAT_FRAGILE,               .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [127] "Hero's": Courage +1, % Experience Bonus +5
    { .nameId = TEXT_ID_PREFIX_HEROS,
      .tier = 5,
      .stat0 = STAT_COURAGE,               .val0 =   1,
      .stat1 = STAT_EXTRA_EXP_PERCENT,     .val1 =   5,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [128] "Old Dwarf's": % Experience Bonus +5, Gemfinder +1, Damage +1
    { .nameId = TEXT_ID_PREFIX_OLD_DWARFS,
      .tier = 7,
      .stat0 = STAT_EXTRA_EXP_PERCENT,     .val0 =   5,
      .stat1 = STAT_GEMFINDER,             .val1 =   1,
      .stat2 = STAT_DAMAGE_FLAT,           .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [129] "Goldsmith's": All Primary Stats +2, Gemfinder +1
    { .nameId = TEXT_ID_PREFIX_GOLDSMITHS,
      .tier = 7,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   2,
      .stat1 = STAT_GEMFINDER,             .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [130] "Legendary": Courage +6
    { .nameId = TEXT_ID_PREFIX_LEGENDARY,
      .tier = 9,
      .stat0 = STAT_COURAGE,               .val0 =   6,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [131] "Ancient Mithril": Damage +4, dmg to Orcs +5, Armor Piercing +1
    { .nameId = TEXT_ID_PREFIX_ANCIENT_MITHRIL,
      .tier = 9,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   4,
      .stat1 = STAT_DAMAGE_TO_ORCS,        .val1 =   5,
      .stat2 = STAT_ARMOR_PIERCE,          .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [132] "Warder's": Defense +3
    { .nameId = TEXT_ID_PREFIX_WARDERS,
      .tier = 6,
      .stat0 = STAT_DEFENSE,               .val0 =   3,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [133] "Keeper's": Defense +3, Strength +1
    { .nameId = TEXT_ID_PREFIX_KEEPERS,
      .tier = 6,
      .stat0 = STAT_DEFENSE,               .val0 =   3,
      .stat1 = STAT_STRENGTH,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [134] "General's": Courage +3, Wisdom +1
    { .nameId = TEXT_ID_PREFIX_GENERALS,
      .tier = 6,
      .stat0 = STAT_COURAGE,               .val0 =   3,
      .stat1 = STAT_WISDOM,                .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [135] "Scoundrel's": All Primary Stats -3, Damage +7, Backstab +1
    { .nameId = TEXT_ID_PREFIX_SCOUNDRELS,
      .tier = 6,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =  -3,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   7,
      .stat2 = STAT_BACKSTAB,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [136] "River's": % Speed +10
    { .nameId = TEXT_ID_PREFIX_RIVERS,
      .tier = 6,
      .stat0 = STAT_SPEED_PERCENT,         .val0 =  10,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [137] "Consecrated": Hit Points per Slain Foe +5, Sunburst +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_CONSECRATED,
      .tier = 6,
      .stat0 = STAT_HP_PER_KILL,           .val0 =   5,
      .stat1 = STAT_SUNBURST,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [138] "Cruel": All Primary Stats +1, Backstab +1
    { .nameId = TEXT_ID_PREFIX_CRUEL,
      .tier = 6,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   1,
      .stat1 = STAT_BACKSTAB,              .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [139] "Knight's": % Fear Resistance +50, % Poison Resistance +50
    { .nameId = TEXT_ID_PREFIX_KNIGHTS,
      .tier = 6,
      .stat0 = STAT_FEAR_RESIST_PERCENT,   .val0 =  50,
      .stat1 = STAT_POISON_RESIST_PERCENT, .val1 =  50,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [140] "Troll's": Strength +5
    { .nameId = TEXT_ID_PREFIX_TROLLS,
      .tier = 8,
      .stat0 = STAT_STRENGTH,              .val0 =   5,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [141] "Historical": Damage +3, All Primary Stats +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_HISTORICAL,
      .tier = 8,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   3,
      .stat1 = STAT_ALL_PRIMARY_STATS,     .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [142] "Magnificent": All Primary Stats +5, Valuable +1
    { .nameId = TEXT_ID_PREFIX_MAGNIFICENT,
      .tier = 10,
      .stat0 = STAT_ALL_PRIMARY_STATS,     .val0 =   5,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_VALUABLE,              .val3 =   1 },

    // [143] "Tremendous": Strength +9
    { .nameId = TEXT_ID_PREFIX_TREMENDOUS,
      .tier = 10,
      .stat0 = STAT_STRENGTH,              .val0 =   9,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [144] "Heavy Ithilien": Strength +3, Damage +2
    { .nameId = TEXT_ID_PREFIX_HEAVY_ITHILIEN,
      .tier = 7,
      .stat0 = STAT_STRENGTH,              .val0 =   3,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   2,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [145] "Flanged": Defense +1, Damage +3
    { .nameId = TEXT_ID_PREFIX_FLANGED,
      .tier = 7,
      .stat0 = STAT_DEFENSE,               .val0 =   1,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [146] "Black Metal": Damage +3, Armor Piercing +1
    { .nameId = TEXT_ID_PREFIX_BLACK_METAL,
      .tier = 7,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   3,
      .stat1 = STAT_ARMOR_PIERCE,          .val1 =   1,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [147] "Savage": Strength +4, Health +1, Knockback +15
    { .nameId = TEXT_ID_PREFIX_SAVAGE,
      .tier = 7,
      .stat0 = STAT_STRENGTH,              .val0 =   4,
      .stat1 = STAT_HEALTH,                .val1 =   1,
      .stat2 = STAT_KNOCKBACK,             .val2 =  15,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [148] "Fierce": Hit Points +20, Armor Piercing +1, Knockback +12
    { .nameId = TEXT_ID_PREFIX_FIERCE,
      .tier = 7,
      .stat0 = STAT_MAX_HP,                .val0 =  20,
      .stat1 = STAT_ARMOR_PIERCE,          .val1 =   1,
      .stat2 = STAT_KNOCKBACK,             .val2 =  12,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [149] "Wyrd": Wisdom +4
    { .nameId = TEXT_ID_PREFIX_WYRD,
      .tier = 7,
      .stat0 = STAT_WISDOM,                .val0 =   4,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [150] "Burning": Damage +2, Fire Damage +1, Fireburst +1, Hit Points per Slain Foe -5
    { .nameId = TEXT_ID_PREFIX_BURNING,
      .tier = 7,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   2,
      .stat1 = STAT_DAMAGE_FIRE,           .val1 =   1,
      .stat2 = STAT_FIREBURST,             .val2 =   1,
      .stat3 = STAT_HP_PER_KILL,           .val3 =  -5 },

    // [151] "Deadly": Damage +4
    { .nameId = TEXT_ID_PREFIX_DEADLY,
      .tier = 7,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   4,
      .stat1 = STAT_NONE,                  .val1 =   0,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [152] "Torturer's": % Fear Resistance +80, Damage +5, Cursed +1
    { .nameId = TEXT_ID_PREFIX_TORTURERS,
      .tier = 9,
      .stat0 = STAT_FEAR_RESIST_PERCENT,   .val0 =  80,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   5,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [153] "Executioner's": Damage +3, % Critical +10, Backstab +1
    { .nameId = TEXT_ID_PREFIX_EXECUTIONERS,
      .tier = 9,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   3,
      .stat1 = STAT_CRIT_CHANCE_PERCENT,   .val1 =  10,
      .stat2 = STAT_BACKSTAB,              .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [154] "Judgement's": Damage +5, All Primary Stats +3
    { .nameId = TEXT_ID_PREFIX_JUDGEMENTS,
      .tier = 11,
      .stat0 = STAT_DAMAGE_FLAT,           .val0 =   5,
      .stat1 = STAT_ALL_PRIMARY_STATS,     .val1 =   3,
      .stat2 = STAT_NONE,                  .val2 =   0,
      .stat3 = STAT_NONE,                  .val3 =   0 },

    // [155] "Old Morgul": % Critical +10, Damage +8, Cursed +1
    { .nameId = TEXT_ID_PREFIX_OLD_MORGUL,
      .tier = 11,
      .stat0 = STAT_CRIT_CHANCE_PERCENT,   .val0 =  10,
      .stat1 = STAT_DAMAGE_FLAT,           .val1 =   8,
      .stat2 = STAT_CURSED,                .val2 =   1,
      .stat3 = STAT_NONE,                  .val3 =   0 },
};

/** no decoded bits set (100 of 156 entries) */
#define AFFIX_FLAGS_NONE { 0 }

/**
 * Per-affix flag table; indexed by the affix index. Decoded bits: AFFIX_FLAG_* (item.h).
 *
 * @romaddress 0x08065e08
 */
const AffixFlags WeaponPrefixFlags[156] = {
    // [0] "Bronze"
    { .d = { .field_bit_31 = 1 } },

    // [1] "Cumbersome"
    { .d = AFFIX_FLAGS_NONE },

    // [2] "Goblin"
    { .d = AFFIX_FLAGS_NONE },

    // [3] "Black"
    { .d = { .field_bit_30 = 1 } },

    // [4] "Rusted"
    { .d = AFFIX_FLAGS_NONE },

    // [5] "Stained"
    { .d = AFFIX_FLAGS_NONE },

    // [6] "Cheap"
    { .d = AFFIX_FLAGS_NONE },

    // [7] "Bloody"
    { .d = { .field_bit_26 = 1 } },

    // [8] "Ebony"
    { .d = { .field_bit_30 = 1 } },

    // [9] "Obsidian"
    { .d = { .field_bit_30 = 1 } },

    // [10] "Seeping"
    { .d = AFFIX_FLAGS_NONE },

    // [11] "Darkmoon"
    { .d = { .minLevelPlus5B = 1,
             .field_bit_30 = 1 } },

    // [12] "Fired"
    { .d = AFFIX_FLAGS_NONE },

    // [13] "Ancient"
    { .d = AFFIX_FLAGS_NONE },

    // [14] "Iron"
    { .d = AFFIX_FLAGS_NONE },

    // [15] "Old"
    { .d = AFFIX_FLAGS_NONE },

    // [16] "Ragged"
    { .d = AFFIX_FLAGS_NONE },

    // [17] "Assassin's"
    { .d = { .field_bit_30 = 1 } },

    // [18] "Hunter's"
    { .d = { .field_bit_29 = 1 } },

    // [19] "Uruk"
    { .d = AFFIX_FLAGS_NONE },

    // [20] "Udun"
    { .d = AFFIX_FLAGS_NONE },

    // [21] "Master's"
    { .d = AFFIX_FLAGS_NONE },

    // [22] "Scarred"
    { .d = AFFIX_FLAGS_NONE },

    // [23] "Skull King's"
    { .d = AFFIX_FLAGS_NONE },

    // [24] "Spider-Iron"
    { .d = { .field_bit_30 = 1 } },

    // [25] "Arachnid"
    { .d = AFFIX_FLAGS_NONE },

    // [26] "Venomous"
    { .d = { .field_bit_27 = 1 } },

    // [27] "Poisoned"
    { .d = { .field_bit_17 = 1,
             .field_bit_20 = 1 } },

    // [28] "Crimson"
    { .d = { .field_bit_26 = 1 } },

    // [29] "Elven"
    { .d = AFFIX_FLAGS_NONE },

    // [30] "Green"
    { .d = { .field_bit_27 = 1 } },

    // [31] "Strengthened"
    { .d = AFFIX_FLAGS_NONE },

    // [32] "Opal"
    { .d = { .field_bit_30 = 1 } },

    // [33] "Vile"
    { .d = AFFIX_FLAGS_NONE },

    // [34] "Razor-edged"
    { .d = AFFIX_FLAGS_NONE },

    // [35] "Mithril"
    { .d = AFFIX_FLAGS_NONE },

    // [36] "Cold"
    { .d = { .field_bit_17 = 1,
             .field_bit_21 = 1 } },

    // [37] "Olog's"
    { .d = AFFIX_FLAGS_NONE },

    // [38] "Sooty"
    { .d = { .field_bit_30 = 1 } },

    // [39] "Volcanic"
    { .d = AFFIX_FLAGS_NONE },

    // [40] "Spiked"
    { .d = AFFIX_FLAGS_NONE },

    // [41] "Glass"
    { .d = { .field_bit_28 = 1 } },

    // [42] "Regenerating"
    { .d = { .minLevelPlus5A = 1 } },

    // [43] "Easterling"
    { .d = AFFIX_FLAGS_NONE },

    // [44] "Orc Bane"
    { .d = AFFIX_FLAGS_NONE },

    // [45] "Bonewhite"
    { .d = { .minLevelPlus5A = 1 } },

    // [46] "Nazgul's"
    { .d = { .field_bit_30 = 1 } },

    // [47] "Blackflame"
    { .d = { .field_bit_16 = 1,
             .field_bit_23 = 1,
             .field_bit_30 = 1 } },

    // [48] "Red"
    { .d = { .field_bit_26 = 1 } },

    // [49] "Piercing"
    { .d = AFFIX_FLAGS_NONE },

    // [50] "Smith's"
    { .d = AFFIX_FLAGS_NONE },

    // [51] "Dark"
    { .d = { .field_bit_30 = 1 } },

    // [52] "Mottled"
    { .d = { .field_bit_29 = 1 } },

    // [53] "Amber"
    { .d = { .field_bit_31 = 1 } },

    // [54] "Ember"
    { .d = AFFIX_FLAGS_NONE },

    // [55] "Sorcerer's"
    { .d = { .field_bit_15 = 1,
             .field_bit_24 = 1 } },

    // [56] "Gil-Galad's"
    { .d = AFFIX_FLAGS_NONE },

    // [57] "Enormous"
    { .d = AFFIX_FLAGS_NONE },

    // [58] "Corrupt"
    { .d = AFFIX_FLAGS_NONE },

    // [59] "Vorpal"
    { .d = AFFIX_FLAGS_NONE },

    // [60] "Ruby"
    { .d = { .field_bit_26 = 1 } },

    // [61] "Furnace"
    { .d = AFFIX_FLAGS_NONE },

    // [62] "Slayer's"
    { .d = AFFIX_FLAGS_NONE },

    // [63] "Perfect"
    { .d = AFFIX_FLAGS_NONE },

    // [64] "Shadowed"
    { .d = { .field_bit_14 = 1,
             .field_bit_30 = 1 } },

    // [65] "Antique"
    { .d = AFFIX_FLAGS_NONE },

    // [66] "Molten"
    { .d = { .field_bit_17 = 1,
             .field_bit_19 = 1,
             .field_bit_26 = 1 } },

    // [67] "Fell"
    { .d = AFFIX_FLAGS_NONE },

    // [68] "Lord's"
    { .d = AFFIX_FLAGS_NONE },

    // [69] "Elrond's"
    { .d = AFFIX_FLAGS_NONE },

    // [70] "Isildur's"
    { .d = AFFIX_FLAGS_NONE },

    // [71] "Elendil's"
    { .d = AFFIX_FLAGS_NONE },

    // [72] "Tarnished"
    { .d = { .field_bit_29 = 1 } },

    // [73] "Tainted"
    { .d = AFFIX_FLAGS_NONE },

    // [74] "Grunt's"
    { .d = AFFIX_FLAGS_NONE },

    // [75] "Ruddy"
    { .d = { .field_bit_26 = 1 } },

    // [76] "Oversized"
    { .d = AFFIX_FLAGS_NONE },

    // [77] "Dirtied"
    { .d = { .field_bit_29 = 1 } },

    // [78] "Warped"
    { .d = AFFIX_FLAGS_NONE },

    // [79] "Ent Trodden"
    { .d = AFFIX_FLAGS_NONE },

    // [80] "White"
    { .d = { .field_bit_25 = 1 } },

    // [81] "Orthanc"
    { .d = AFFIX_FLAGS_NONE },

    // [82] "Orc Slaying"
    { .d = AFFIX_FLAGS_NONE },

    // [83] "Lurtz's"
    { .d = AFFIX_FLAGS_NONE },

    // [84] "Wanderer's"
    { .d = AFFIX_FLAGS_NONE },

    // [85] "Ally's"
    { .d = AFFIX_FLAGS_NONE },

    // [86] "Lancer's"
    { .d = AFFIX_FLAGS_NONE },

    // [87] "Blacksmith's"
    { .d = AFFIX_FLAGS_NONE },

    // [88] "Maiden's"
    { .d = AFFIX_FLAGS_NONE },

    // [89] "Sharpened"
    { .d = AFFIX_FLAGS_NONE },

    // [90] "Polished"
    { .d = AFFIX_FLAGS_NONE },

    // [91] "Rohirrim"
    { .d = AFFIX_FLAGS_NONE },

    // [92] "Eomer's"
    { .d = AFFIX_FLAGS_NONE },

    // [93] "Eowyn's"
    { .d = AFFIX_FLAGS_NONE },

    // [94] "Fearless"
    { .d = AFFIX_FLAGS_NONE },

    // [95] "Exotic"
    { .d = { .field_bit_31 = 1 } },

    // [96] "Miner's"
    { .d = AFFIX_FLAGS_NONE },

    // [97] "Brown"
    { .d = { .field_bit_29 = 1 } },

    // [98] "Guidesman's"
    { .d = AFFIX_FLAGS_NONE },

    // [99] "Sigiled"
    { .d = AFFIX_FLAGS_NONE },

    // [100] "Runic"
    { .d = AFFIX_FLAGS_NONE },

    // [101] "Harrowed"
    { .d = AFFIX_FLAGS_NONE },

    // [102] "Heavy"
    { .d = AFFIX_FLAGS_NONE },

    // [103] "Honed"
    { .d = AFFIX_FLAGS_NONE },

    // [104] "Storm-forged"
    { .d = { .field_bit_15 = 1,
             .field_bit_18 = 1 } },

    // [105] "Nightmare"
    { .d = { .field_bit_17 = 1,
             .field_bit_19 = 1,
             .field_bit_30 = 1 } },

    // [106] "Elf Queen's"
    { .d = AFFIX_FLAGS_NONE },

    // [107] "Warrior's"
    { .d = AFFIX_FLAGS_NONE },

    // [108] "Cursed"
    { .d = AFFIX_FLAGS_NONE },

    // [109] "Jagged"
    { .d = AFFIX_FLAGS_NONE },

    // [110] "Glowing"
    { .d = { .field_bit_17 = 1,
             .field_bit_18 = 1,
             .field_bit_25 = 1 } },

    // [111] "Vengeance"
    { .d = AFFIX_FLAGS_NONE },

    // [112] "Frog's"
    { .d = { .minLevelPlus5A = 1,
             .field_bit_15 = 1,
             .field_bit_20 = 1,
             .field_bit_27 = 1 } },

    // [113] "Ruthless"
    { .d = AFFIX_FLAGS_NONE },

    // [114] "Cobalt"
    { .d = { .field_bit_28 = 1 } },

    // [115] "Serpent's"
    { .d = { .field_bit_16 = 1,
             .field_bit_20 = 1 } },

    // [116] "Azure"
    { .d = { .field_bit_28 = 1 } },

    // [117] "Ghostmetal"
    { .d = { .field_bit_14 = 1,
             .field_bit_31 = 1 } },

    // [118] "Ethereal"
    { .d = { .field_bit_14 = 1 } },

    // [119] "Platinum"
    { .d = AFFIX_FLAGS_NONE },

    // [120] "Engraved"
    { .d = AFFIX_FLAGS_NONE },

    // [121] "Dwarf Maiden"
    { .d = AFFIX_FLAGS_NONE },

    // [122] "Elder"
    { .d = AFFIX_FLAGS_NONE },

    // [123] "Grey"
    { .d = AFFIX_FLAGS_NONE },

    // [124] "Snowbird's"
    { .d = { .field_bit_17 = 1,
             .field_bit_18 = 1,
             .field_bit_28 = 1 } },

    // [125] "Frozen"
    { .d = { .field_bit_17 = 1,
             .field_bit_18 = 1 } },

    // [126] "Crystalline"
    { .d = { .field_bit_14 = 1,
             .field_bit_28 = 1 } },

    // [127] "Hero's"
    { .d = AFFIX_FLAGS_NONE },

    // [128] "Old Dwarf's"
    { .d = AFFIX_FLAGS_NONE },

    // [129] "Goldsmith's"
    { .d = { .field_bit_17 = 1,
             .field_bit_24 = 1,
             .field_bit_31 = 1 } },

    // [130] "Legendary"
    { .d = AFFIX_FLAGS_NONE },

    // [131] "Ancient Mithril"
    { .d = AFFIX_FLAGS_NONE },

    // [132] "Warder's"
    { .d = AFFIX_FLAGS_NONE },

    // [133] "Keeper's"
    { .d = AFFIX_FLAGS_NONE },

    // [134] "General's"
    { .d = AFFIX_FLAGS_NONE },

    // [135] "Scoundrel's"
    { .d = AFFIX_FLAGS_NONE },

    // [136] "River's"
    { .d = { .field_bit_28 = 1 } },

    // [137] "Consecrated"
    { .d = { .field_bit_16 = 1,
             .field_bit_18 = 1 } },

    // [138] "Cruel"
    { .d = AFFIX_FLAGS_NONE },

    // [139] "Knight's"
    { .d = AFFIX_FLAGS_NONE },

    // [140] "Troll's"
    { .d = AFFIX_FLAGS_NONE },

    // [141] "Historical"
    { .d = AFFIX_FLAGS_NONE },

    // [142] "Magnificent"
    { .d = { .field_bit_15 = 1,
             .field_bit_24 = 1,
             .field_bit_31 = 1 } },

    // [143] "Tremendous"
    { .d = AFFIX_FLAGS_NONE },

    // [144] "Heavy Ithilien"
    { .d = AFFIX_FLAGS_NONE },

    // [145] "Flanged"
    { .d = AFFIX_FLAGS_NONE },

    // [146] "Black Metal"
    { .d = { .field_bit_30 = 1 } },

    // [147] "Savage"
    { .d = AFFIX_FLAGS_NONE },

    // [148] "Fierce"
    { .d = AFFIX_FLAGS_NONE },

    // [149] "Wyrd"
    { .d = AFFIX_FLAGS_NONE },

    // [150] "Burning"
    { .d = { .field_bit_16 = 1,
             .field_bit_19 = 1,
             .field_bit_26 = 1 } },

    // [151] "Deadly"
    { .d = { .field_bit_15 = 1,
             .field_bit_23 = 1,
             .field_bit_26 = 1 } },

    // [152] "Torturer's"
    { .d = AFFIX_FLAGS_NONE },

    // [153] "Executioner's"
    { .d = { .field_bit_17 = 1,
             .field_bit_19 = 1,
             .field_bit_30 = 1 } },

    // [154] "Judgement's"
    { .d = { .field_bit_17 = 1,
             .field_bit_18 = 1,
             .field_bit_30 = 1 } },

    // [155] "Old Morgul"
    { .d = AFFIX_FLAGS_NONE },
};
#undef AFFIX_FLAGS_NONE
// clang-format on
