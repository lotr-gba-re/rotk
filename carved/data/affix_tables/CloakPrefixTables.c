#include "item.h"
#include "stats.h"
#include "text_ids.h"

// clang-format off


/** @romaddress 0x08067c78 */
const ItemAffix CloakPrefixes[36] = {
    // [0] "Tattered": Missile Armor -1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_TATTERED,
      .tier = 1,
      .stat0 = STAT_MISSILE_ARMOR,       .val0 = -1,
      .stat1 = STAT_NONE,                .val1 =  0,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_CHEAP,               .val3 =  1 },

    // [1] "Moldy": Courage -1, Spirit Points +5, Cheap +1
    { .nameId = TEXT_ID_PREFIX_MOLDY,
      .tier = 1,
      .stat0 = STAT_COURAGE,             .val0 = -1,
      .stat1 = STAT_MAX_SPIRIT,          .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_CHEAP,               .val3 =  1 },

    // [2] "Foul": Defense +2, Cursed +1
    { .nameId = TEXT_ID_PREFIX_FOUL,
      .tier = 1,
      .stat0 = STAT_DEFENSE,             .val0 =  2,
      .stat1 = STAT_CURSED,              .val1 =  1,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [3] "Aldor's": Defense +1, Hit Points +5
    { .nameId = TEXT_ID_PREFIX_ALDORS,
      .tier = 1,
      .stat0 = STAT_DEFENSE,             .val0 =  1,
      .stat1 = STAT_MAX_HP,              .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [4] "Shoddy": Melee Armor -1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_SHODDY,
      .tier = 1,
      .stat0 = STAT_MELEE_ARMOR,         .val0 = -1,
      .stat1 = STAT_NONE,                .val1 =  0,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_CHEAP,               .val3 =  1 },

    // [5] "Torn": % Dodge +5, Defense -1, Cheap +1
    { .nameId = TEXT_ID_PREFIX_TORN,
      .tier = 1,
      .stat0 = STAT_DODGE_PERCENT,       .val0 =  5,
      .stat1 = STAT_DEFENSE,             .val1 = -1,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_CHEAP,               .val3 =  1 },

    // [6] "Hide": Hit Points +5
    { .nameId = TEXT_ID_PREFIX_HIDE,
      .tier = 1,
      .stat0 = STAT_MAX_HP,              .val0 =  5,
      .stat1 = STAT_NONE,                .val1 =  0,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [7] "Skin": Defense +1, Spirit Points +5
    { .nameId = TEXT_ID_PREFIX_SKIN,
      .tier = 1,
      .stat0 = STAT_DEFENSE,             .val0 =  1,
      .stat1 = STAT_MAX_SPIRIT,          .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [8] "Orc-hair": Missile Armor +2, Courage -1
    { .nameId = TEXT_ID_PREFIX_ORC_HAIR,
      .tier = 3,
      .stat0 = STAT_MISSILE_ARMOR,       .val0 =  2,
      .stat1 = STAT_COURAGE,             .val1 = -1,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [9] "Horsehair": Spirit Points +5, Courage +1
    { .nameId = TEXT_ID_PREFIX_HORSEHAIR,
      .tier = 3,
      .stat0 = STAT_MAX_SPIRIT,          .val0 =  5,
      .stat1 = STAT_COURAGE,             .val1 =  1,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [10] "Plain": Missile Armor +1, Hit Points +5
    { .nameId = TEXT_ID_PREFIX_PLAIN,
      .tier = 5,
      .stat0 = STAT_MISSILE_ARMOR,       .val0 =  1,
      .stat1 = STAT_MAX_HP,              .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [11] "Patched": Courage +3, Hit Points +10
    { .nameId = TEXT_ID_PREFIX_PATCHED,
      .tier = 5,
      .stat0 = STAT_COURAGE,             .val0 =  3,
      .stat1 = STAT_MAX_HP,              .val1 = 10,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [12] "Wool": Critical Armor +2
    { .nameId = TEXT_ID_PREFIX_WOOL,
      .tier = 3,
      .stat0 = STAT_CRITICAL_ARMOR,      .val0 =  2,
      .stat1 = STAT_NONE,                .val1 =  0,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [13] "Moqueshal": Fire Protection +3, Courage +1
    { .nameId = TEXT_ID_PREFIX_MOQUESHAL,
      .tier = 3,
      .stat0 = STAT_FIRE_PROTECTION,     .val0 =  3,
      .stat1 = STAT_COURAGE,             .val1 =  1,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [14] "Organza": Defense +2, Hit Points +12
    { .nameId = TEXT_ID_PREFIX_ORGANZA,
      .tier = 3,
      .stat0 = STAT_DEFENSE,             .val0 =  2,
      .stat1 = STAT_MAX_HP,              .val1 = 12,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [15] "Cotton": Fire Protection +4, Spirit Points +5
    { .nameId = TEXT_ID_PREFIX_COTTON,
      .tier = 3,
      .stat0 = STAT_FIRE_PROTECTION,     .val0 =  4,
      .stat1 = STAT_MAX_SPIRIT,          .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [16] "Fur": Critical Armor +3, Strength +1
    { .nameId = TEXT_ID_PREFIX_FUR,
      .tier = 3,
      .stat0 = STAT_CRITICAL_ARMOR,      .val0 =  3,
      .stat1 = STAT_STRENGTH,            .val1 =  1,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [17] "Flax": Defense +1, Spirit Points +12
    { .nameId = TEXT_ID_PREFIX_FLAX,
      .tier = 3,
      .stat0 = STAT_DEFENSE,             .val0 =  1,
      .stat1 = STAT_MAX_SPIRIT,          .val1 = 12,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [18] "Hooded": Melee Armor +1, Critical Armor +2
    { .nameId = TEXT_ID_PREFIX_HOODED,
      .tier = 3,
      .stat0 = STAT_MELEE_ARMOR,         .val0 =  1,
      .stat1 = STAT_CRITICAL_ARMOR,      .val1 =  2,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [19] "Silken": Courage +1, Hit Points +5
    { .nameId = TEXT_ID_PREFIX_SILKEN,
      .tier = 3,
      .stat0 = STAT_COURAGE,             .val0 =  1,
      .stat1 = STAT_MAX_HP,              .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [20] "Traveler's": Defense +3, Missile Armor +1
    { .nameId = TEXT_ID_PREFIX_TRAVELERS,
      .tier = 5,
      .stat0 = STAT_DEFENSE,             .val0 =  3,
      .stat1 = STAT_MISSILE_ARMOR,       .val1 =  1,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [21] "Took's": Courage +3, Wisdom +1
    { .nameId = TEXT_ID_PREFIX_TOOKS,
      .tier = 5,
      .stat0 = STAT_COURAGE,             .val0 =  3,
      .stat1 = STAT_WISDOM,              .val1 =  1,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [22] "Spiderweave": Immobilize +1, Spirit Points +20
    { .nameId = TEXT_ID_PREFIX_SPIDERWEAVE,
      .tier = 7,
      .stat0 = STAT_IMMOBILIZE,          .val0 =  1,
      .stat1 = STAT_MAX_SPIRIT,          .val1 = 20,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [23] "Webfiber": Courage +3, Hit Points Regen +3, Melee Armor +1
    { .nameId = TEXT_ID_PREFIX_WEBFIBER,
      .tier = 7,
      .stat0 = STAT_COURAGE,             .val0 =  3,
      .stat1 = STAT_HP_REGEN,            .val1 =  3,
      .stat2 = STAT_MELEE_ARMOR,         .val2 =  1,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [24] "Goldlace": Courage +2, Missile Armor +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_GOLDLACE,
      .tier = 5,
      .stat0 = STAT_COURAGE,             .val0 =  2,
      .stat1 = STAT_MISSILE_ARMOR,       .val1 =  1,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_VALUABLE,            .val3 =  1 },

    // [25] "Brocade": % Dodge +5, Hit Points +8
    { .nameId = TEXT_ID_PREFIX_BROCADE,
      .tier = 5,
      .stat0 = STAT_DODGE_PERCENT,       .val0 =  5,
      .stat1 = STAT_MAX_HP,              .val1 =  8,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [26] "Herald's": Fire Protection +3, Melee Armor +1
    { .nameId = TEXT_ID_PREFIX_HERALDS,
      .tier = 5,
      .stat0 = STAT_FIRE_PROTECTION,     .val0 =  3,
      .stat1 = STAT_MELEE_ARMOR,         .val1 =  1,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [27] "Elven": Defense +3, Spirit Points +20
    { .nameId = TEXT_ID_PREFIX_ELVEN,
      .tier = 5,
      .stat0 = STAT_DEFENSE,             .val0 =  3,
      .stat1 = STAT_MAX_SPIRIT,          .val1 = 20,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [28] "Willow": Courage +3, Health +1
    { .nameId = TEXT_ID_PREFIX_WILLOW,
      .tier = 5,
      .stat0 = STAT_COURAGE,             .val0 =  3,
      .stat1 = STAT_HEALTH,              .val1 =  1,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [29] "Velvet": Courage +1, Hit Points Regen +2
    { .nameId = TEXT_ID_PREFIX_VELVET,
      .tier = 5,
      .stat0 = STAT_COURAGE,             .val0 =  1,
      .stat1 = STAT_HP_REGEN,            .val1 =  2,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [30] "Majestic": Critical Armor +5, Spirit Points +20
    { .nameId = TEXT_ID_PREFIX_MAJESTIC,
      .tier = 5,
      .stat0 = STAT_CRITICAL_ARMOR,      .val0 =  5,
      .stat1 = STAT_MAX_SPIRIT,          .val1 = 20,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [31] "Silver-thread": Hit Points +15, Valuable +1
    { .nameId = TEXT_ID_PREFIX_SILVER_THREAD,
      .tier = 5,
      .stat0 = STAT_MAX_HP,              .val0 = 15,
      .stat1 = STAT_NONE,                .val1 =  0,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_VALUABLE,            .val3 =  1 },

    // [32] "Legendary": Defense +4, Courage +2
    { .nameId = TEXT_ID_PREFIX_LEGENDARY,
      .tier = 7,
      .stat0 = STAT_DEFENSE,             .val0 =  4,
      .stat1 = STAT_COURAGE,             .val1 =  2,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [33] "Maidenhair": Wisdom +3, Spirit Points +20
    { .nameId = TEXT_ID_PREFIX_MAIDENHAIR,
      .tier = 7,
      .stat0 = STAT_WISDOM,              .val0 =  3,
      .stat1 = STAT_MAX_SPIRIT,          .val1 = 20,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [34] "Elf-thread": Courage +3, % Critical +5, Missile Armor +1
    { .nameId = TEXT_ID_PREFIX_ELF_THREAD,
      .tier = 9,
      .stat0 = STAT_COURAGE,             .val0 =  3,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_MISSILE_ARMOR,       .val2 =  1,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [35] "Gem-crusted": Defense +3, Spirit Points +20, Gemfinder +1, Valuable +1
    { .nameId = TEXT_ID_PREFIX_GEM_CRUSTED,
      .tier = 9,
      .stat0 = STAT_DEFENSE,             .val0 =  3,
      .stat1 = STAT_MAX_SPIRIT,          .val1 = 20,
      .stat2 = STAT_GEMFINDER,           .val2 =  1,
      .stat3 = STAT_VALUABLE,            .val3 =  1 },
};

/** no decoded bits set (34 of 36 entries) */
#define AFFIX_FLAGS_NONE { 0 }

/**
 * Per-affix flag table; indexed by the affix index. Decoded bits: AFFIX_FLAG_* (item.h).
 *
 * @romaddress 0x08067e28
 */
const AffixFlags CloakPrefixFlags[36] = {
    // [0] "Tattered"
    { .d = AFFIX_FLAGS_NONE },

    // [1] "Moldy"
    { .d = AFFIX_FLAGS_NONE },

    // [2] "Foul"
    { .d = AFFIX_FLAGS_NONE },

    // [3] "Aldor's"
    { .d = AFFIX_FLAGS_NONE },

    // [4] "Shoddy"
    { .d = AFFIX_FLAGS_NONE },

    // [5] "Torn"
    { .d = AFFIX_FLAGS_NONE },

    // [6] "Hide"
    { .d = AFFIX_FLAGS_NONE },

    // [7] "Skin"
    { .d = AFFIX_FLAGS_NONE },

    // [8] "Orc-hair"
    { .d = AFFIX_FLAGS_NONE },

    // [9] "Horsehair"
    { .d = AFFIX_FLAGS_NONE },

    // [10] "Plain"
    { .d = AFFIX_FLAGS_NONE },

    // [11] "Patched"
    { .d = AFFIX_FLAGS_NONE },

    // [12] "Wool"
    { .d = AFFIX_FLAGS_NONE },

    // [13] "Moqueshal"
    { .d = AFFIX_FLAGS_NONE },

    // [14] "Organza"
    { .d = AFFIX_FLAGS_NONE },

    // [15] "Cotton"
    { .d = AFFIX_FLAGS_NONE },

    // [16] "Fur"
    { .d = AFFIX_FLAGS_NONE },

    // [17] "Flax"
    { .d = AFFIX_FLAGS_NONE },

    // [18] "Hooded"
    { .d = AFFIX_FLAGS_NONE },

    // [19] "Silken"
    { .d = AFFIX_FLAGS_NONE },

    // [20] "Traveler's"
    { .d = AFFIX_FLAGS_NONE },

    // [21] "Took's"
    { .d = AFFIX_FLAGS_NONE },

    // [22] "Spiderweave"
    { .d = AFFIX_FLAGS_NONE },

    // [23] "Webfiber"
    { .d = { .minLevelPlus5A = 1 } },

    // [24] "Goldlace"
    { .d = AFFIX_FLAGS_NONE },

    // [25] "Brocade"
    { .d = AFFIX_FLAGS_NONE },

    // [26] "Herald's"
    { .d = AFFIX_FLAGS_NONE },

    // [27] "Elven"
    { .d = AFFIX_FLAGS_NONE },

    // [28] "Willow"
    { .d = AFFIX_FLAGS_NONE },

    // [29] "Velvet"
    { .d = { .minLevelPlus5A = 1 } },

    // [30] "Majestic"
    { .d = AFFIX_FLAGS_NONE },

    // [31] "Silver-thread"
    { .d = AFFIX_FLAGS_NONE },

    // [32] "Legendary"
    { .d = AFFIX_FLAGS_NONE },

    // [33] "Maidenhair"
    { .d = AFFIX_FLAGS_NONE },

    // [34] "Elf-thread"
    { .d = AFFIX_FLAGS_NONE },

    // [35] "Gem-crusted"
    { .d = AFFIX_FLAGS_NONE },
};
#undef AFFIX_FLAGS_NONE
// clang-format on
