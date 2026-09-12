#include "item.h"
#include "stats.h"

// clang-format off

/** @romaddress 0x080691a8 */
const RuneStats Runes[15] = {
    // [0] "Dwarf Rune I - Dwarfmetal": Melee Armor +1
    { .stat0 = STAT_MELEE_ARMOR,                .val0 =   1,
      .stat1 = STAT_NONE,                       .val1 =   0 },

    // [1] "Dwarf Rune II - Sharpness": Damage +1
    { .stat0 = STAT_DAMAGE_FLAT,                .val0 =   1,
      .stat1 = STAT_NONE,                       .val1 =   0 },

    // [2] "Dwarf Rune III - The Forge": Critical Damage +5
    { .stat0 = STAT_CRIT_DAMAGE,                .val0 =   5,
      .stat1 = STAT_NONE,                       .val1 =   0 },

    // [3] "Dwarf Rune IV - Mountain Heart": Hit Points per Slain Foe +5
    { .stat0 = STAT_HP_PER_KILL,                .val0 =   5,
      .stat1 = STAT_NONE,                       .val1 =   0 },

    // [4] "Elf Rune I - Orc Foe": dmg to Orcs +2
    { .stat0 = STAT_DAMAGE_TO_ORCS,             .val0 =   2,
      .stat1 = STAT_NONE,                       .val1 =   0 },

    // [5] "Elf Rune II - Nimblefoot": % Dodge +10
    { .stat0 = STAT_DODGE_PERCENT,              .val0 =  10,
      .stat1 = STAT_NONE,                       .val1 =   0 },

    // [6] "Elf Rune III - Sacred Heart": Spirit Regen +2
    { .stat0 = STAT_SPIRIT_REGEN,               .val0 =   2,
      .stat1 = STAT_NONE,                       .val1 =   0 },

    // [7] "Elf Rune IV - Meditative": Meditative +1
    { .stat0 = STAT_MEDITATIVE,                 .val0 =   1,
      .stat1 = STAT_NONE,                       .val1 =   0 },

    // [8] "Man Rune I - White City": Missile Armor +2
    { .stat0 = STAT_MISSILE_ARMOR,              .val0 =   2,
      .stat1 = STAT_NONE,                       .val1 =   0 },

    // [9] "Man Rune II - Nature": % Extra Hit Points from Food +100, Hit Points from herbs +20
    { .stat0 = STAT_EXTRA_HP_FROM_FOOD_PERCENT, .val0 = 100,
      .stat1 = STAT_HP_FROM_HERBS,              .val1 =  20 },

    // [10] "Man Rune III - Might of Man": Damage +1, Hit Points +15
    { .stat0 = STAT_DAMAGE_FLAT,                .val0 =   1,
      .stat1 = STAT_MAX_HP,                     .val1 =  15 },

    // [11] "Man Rune IV - Rune of Fire": Fire Protection +3, Fireburst +1
    { .stat0 = STAT_FIRE_PROTECTION,            .val0 =   3,
      .stat1 = STAT_FIREBURST,                  .val1 =   1 },

    // [12] "Morgul Rune I - Orc Doom-King": dmg to Orcs +4, % Corruption Resistance -50
    { .stat0 = STAT_DAMAGE_TO_ORCS,             .val0 =   4,
      .stat1 = STAT_CORRUPTION_RESIST_PERCENT,  .val1 = -50 },

    // [13] "Morgul Rune II - Dire Fear": % to Push Foe Back +30, % Corruption Resistance -60
    { .stat0 = STAT_PUSH_FOE_BACK,              .val0 =  30,
      .stat1 = STAT_CORRUPTION_RESIST_PERCENT,  .val1 = -60 },

    // [14] "Morgul Rune III - Blackness": All Primary Stats +5, % Corruption Resistance -70
    { .stat0 = STAT_ALL_PRIMARY_STATS,          .val0 =   5,
      .stat1 = STAT_CORRUPTION_RESIST_PERCENT,  .val1 = -70 },
};
// clang-format on
