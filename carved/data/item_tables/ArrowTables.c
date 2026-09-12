#include "item.h"
#include "libc.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off


/** @romaddress 0x08063cbc */
const ItemBase ArrowItems[10] = {
    // [0] "Orcish Arrows": Damage +2, Accuracy -2, Cheap +1
    { .nameId = TEXT_ID_ITEM_ORCISH_ARROWS,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,       .val0 =  2,
      .stat1 = STAT_ACCURACY,          .val1 = -2,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_CHEAP,             .val3 =  1 },

    // [1] "Barbed Arrows": Damage +1, Cheap +1
    { .nameId = TEXT_ID_ITEM_BARBED_ARROWS,
      .tier = 1,
      .stat0 = STAT_DAMAGE_FLAT,       .val0 =  1,
      .stat1 = STAT_NONE,              .val1 =  0,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_CHEAP,             .val3 =  1 },

    // [2] "Uruk-Hai Arrows": Damage +4, Accuracy -4, Cheap +1
    { .nameId = TEXT_ID_ITEM_URUK_HAI_ARROWS,
      .tier = 2,
      .stat0 = STAT_DAMAGE_FLAT,       .val0 =  4,
      .stat1 = STAT_ACCURACY,          .val1 = -4,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_CHEAP,             .val3 =  1 },

    // [3] "Bodkin Arrows": Armor Piercing +1, Cheap +1
    { .nameId = TEXT_ID_ITEM_BODKIN_ARROWS,
      .tier = 3,
      .stat0 = STAT_ARMOR_PIERCE,      .val0 =  1,
      .stat1 = STAT_NONE,              .val1 =  0,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_CHEAP,             .val3 =  1 },

    // [4] "Gondorian Arrows": Damage +2, Cheap +1
    { .nameId = TEXT_ID_ITEM_GONDORIAN_ARROWS,
      .tier = 4,
      .stat0 = STAT_DAMAGE_FLAT,       .val0 =  2,
      .stat1 = STAT_NONE,              .val1 =  0,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_CHEAP,             .val3 =  1 },

    // [5] "Elven Arrows": Extra Projectiles +1, Damage +1, Cheap +1
    { .nameId = TEXT_ID_ITEM_ELVEN_ARROWS,
      .tier = 6,
      .stat0 = STAT_EXTRA_PROJECTILES, .val0 =  1,
      .stat1 = STAT_DAMAGE_FLAT,       .val1 =  1,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_CHEAP,             .val3 =  1 },

    // [6] "Fire Arrows": Fire Damage +3, Fireburst +1
    { .nameId = TEXT_ID_ITEM_FIRE_ARROWS,
      .tier = 7,
      .stat0 = STAT_DAMAGE_FIRE,       .val0 =  3,
      .stat1 = STAT_FIREBURST,         .val1 =  1,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_NONE,              .val3 =  0 },

    // [7] "Wood Elven Arrows": Damage +4, Armor Piercing +1, Cheap +1
    { .nameId = TEXT_ID_ITEM_WOOD_ELVEN_ARROWS,
      .tier = 8,
      .stat0 = STAT_DAMAGE_FLAT,       .val0 =  4,
      .stat1 = STAT_ARMOR_PIERCE,      .val1 =  1,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_CHEAP,             .val3 =  1 },

    // [8] "Moon Arrows": Spirit per Slain Foe +10
    { .nameId = TEXT_ID_ITEM_MOON_ARROWS,
      .tier = 9,
      .stat0 = STAT_SPIRIT_PER_KILL,   .val0 = 10,
      .stat1 = STAT_NONE,              .val1 =  0,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_NONE,              .val3 =  0 },

    // [9] "Sun Arrows": Hit Points per Slain Foe +10
    { .nameId = TEXT_ID_ITEM_SUN_ARROWS,
      .tier = 9,
      .stat0 = STAT_HP_PER_KILL,       .val0 = 10,
      .stat1 = STAT_NONE,              .val1 =  0,
      .stat2 = STAT_NONE,              .val2 =  0,
      .stat3 = STAT_NONE,              .val3 =  0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x08063d34
 */
const ItemGfx ArrowGfx[10] = {
    // [0] "Orcish Arrows"
    ITEM_GFX(ArrowOrcishArrows, LOOT_TYPE_ARROW),
    // [1] "Barbed Arrows"
    ITEM_GFX(ArrowBarbedArrows, LOOT_TYPE_ARROW),
    // [2] "Uruk-Hai Arrows"
    ITEM_GFX(ArrowUrukHaiArrows, LOOT_TYPE_ARROW),
    // [3] "Bodkin Arrows"
    ITEM_GFX(ArrowBodkinArrows, LOOT_TYPE_ARROW),
    // [4] "Gondorian Arrows"
    ITEM_GFX(ArrowGondorianArrows, LOOT_TYPE_ARROW),
    // [5] "Elven Arrows"
    ITEM_GFX(ArrowElvenArrows, LOOT_TYPE_ARROW),
    // [6] "Fire Arrows"
    ITEM_GFX(ArrowFireArrows, LOOT_TYPE_ARROW),
    // [7] "Wood Elven Arrows"
    ITEM_GFX(ArrowWoodElvenArrows, LOOT_TYPE_ARROW),
    // [8] "Moon Arrows"
    ITEM_GFX(ArrowMoonArrows, LOOT_TYPE_ARROW),
    // [9] "Sun Arrows"
    ITEM_GFX(ArrowSunArrows, LOOT_TYPE_ARROW),
};

/** most common row (10 of 10); deviations are spelled out per entry */
#define FLAGS_ARROW { .noPrefix = 1, .noSuffix = 1, .slotOffhand = 1, .classLegolas = 1 }

/** @romaddress 0x08063e4c */
const ItemMetadata ArrowItemFlags[10] = {
    // [0] "Orcish Arrows"
    { .flags.d = FLAGS_ARROW, .equipSfx = 0x30 },

    // [1] "Barbed Arrows"
    { .flags.d = FLAGS_ARROW, .equipSfx = 0x30 },

    // [2] "Uruk-Hai Arrows"
    { .flags.d = FLAGS_ARROW, .equipSfx = 0x30 },

    // [3] "Bodkin Arrows"
    { .flags.d = FLAGS_ARROW, .equipSfx = 0x30 },

    // [4] "Gondorian Arrows"
    { .flags.d = FLAGS_ARROW, .equipSfx = 0x30 },

    // [5] "Elven Arrows"
    { .flags.d = FLAGS_ARROW, .equipSfx = 0x30 },

    // [6] "Fire Arrows"
    { .flags.d = FLAGS_ARROW, .equipSfx = 0x30 },

    // [7] "Wood Elven Arrows"
    { .flags.d = FLAGS_ARROW, .equipSfx = 0x30 },

    // [8] "Moon Arrows"
    { .flags.d = FLAGS_ARROW, .equipSfx = 0x30 },

    // [9] "Sun Arrows"
    { .flags.d = FLAGS_ARROW, .equipSfx = 0x30 },
};
#undef FLAGS_ARROW

/** no decoded bits set (2 of 10 entries) */
#define AFFIX_FLAGS_NONE { 0 }

/**
 * Per-item flag table; indexed by ArrowItems baseIndex (intrinsic elemental FX flags, read
 * by the projectile FX path at 0x08026e88). Decoded bits: AFFIX_FLAG_* (item.h).
 *
 * @romaddress 0x08063e9c
 */
const AffixFlags ArrowAffixFlags[10] = {
    // [0] "Orcish Arrows"
    { .d = { .field_bit_29 = 1 } },

    // [1] "Barbed Arrows"
    { .d = AFFIX_FLAGS_NONE },

    // [2] "Uruk-Hai Arrows"
    { .d = { .field_bit_30 = 1 } },

    // [3] "Bodkin Arrows"
    { .d = AFFIX_FLAGS_NONE },

    // [4] "Gondorian Arrows"
    { .d = { .field_bit_28 = 1 } },

    // [5] "Elven Arrows"
    { .d = { .field_bit_15 = 1,
             .field_bit_18 = 1,
             .field_bit_31 = 1 } },

    // [6] "Fire Arrows"
    { .d = { .field_bit_17 = 1,
             .field_bit_23 = 1 } },

    // [7] "Wood Elven Arrows"
    { .d = { .field_bit_16 = 1,
             .field_bit_20 = 1,
             .field_bit_29 = 1 } },

    // [8] "Moon Arrows"
    { .d = { .field_bit_17 = 1,
             .field_bit_18 = 1,
             .field_bit_30 = 1 } },

    // [9] "Sun Arrows"
    { .d = { .field_bit_15 = 1,
             .field_bit_19 = 1,
             .field_bit_31 = 1 } },
};
#undef AFFIX_FLAGS_NONE
// clang-format on
