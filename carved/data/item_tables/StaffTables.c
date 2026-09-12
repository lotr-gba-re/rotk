#include "item.h"
#include "libc.h"
#include "loot.h"
#include "stats.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off


/** @romaddress 0x0806398c */
const ItemBase StaffItems[4] = {
    // [0] "Short Staff": Melee Damage +6, % Critical +5
    { .nameId = TEXT_ID_ITEM_SHORT_STAFF,
      .tier = 1,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 =  6,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_NONE,                .val2 =  0,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [1] "Staff": Melee Damage +8, % Critical +5, Wisdom +1
    { .nameId = TEXT_ID_ITEM_STAFF,
      .tier = 3,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 =  8,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_WISDOM,              .val2 =  1,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [2] "Quarterstaff": Melee Damage +10, % Critical +5, Wisdom +2
    { .nameId = TEXT_ID_ITEM_QUARTERSTAFF,
      .tier = 6,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 = 10,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_WISDOM,              .val2 =  2,
      .stat3 = STAT_NONE,                .val3 =  0 },

    // [3] "Magestaff": Melee Damage +6, % Critical +5, Wisdom +4
    { .nameId = TEXT_ID_ITEM_MAGESTAFF,
      .tier = 8,
      .stat0 = STAT_DAMAGE_SLASH,        .val0 =  6,
      .stat1 = STAT_CRIT_CHANCE_PERCENT, .val1 =  5,
      .stat2 = STAT_WISDOM,              .val2 =  4,
      .stat3 = STAT_NONE,                .val3 =  0 },
};

/**
 * Per-base-item gfx/loot rows; indexed by baseIndex. For unique-item drops,
 * loot_rollBaseItem reads .lootType to pick the loot.
 *
 * @romaddress 0x080639bc
 */
const ItemGfx StaffGfx[4] = {
    // [0] "Short Staff"
    ITEM_GFX(StaffShortStaff, LOOT_TYPE_STAFF_TIER_1),
    // [1] "Staff"
    ITEM_GFX(StaffStaff, LOOT_TYPE_STAFF_TIER_1),
    // [2] "Quarterstaff"
    ITEM_GFX(StaffQuarterstaff, LOOT_TYPE_STAFF_TIER_2),
    // [3] "Magestaff"
    ITEM_GFX(StaffMagestaff, LOOT_TYPE_STAFF_TIER_2),
};

/** most common row (4 of 4); deviations are spelled out per entry */
#define FLAGS_STAFF { .familyStaff = 1, .slotOffhand = 1, .classGandalf = 1 }

/** @romaddress 0x08063a2c */
const ItemMetadata StaffItemFlags[4] = {
    // [0] "Short Staff"
    { .flags.d = FLAGS_STAFF, .equipSfx = 0x2b },

    // [1] "Staff"
    { .flags.d = FLAGS_STAFF, .equipSfx = 0x2b },

    // [2] "Quarterstaff"
    { .flags.d = FLAGS_STAFF, .equipSfx = 0x2b },

    // [3] "Magestaff"
    { .flags.d = FLAGS_STAFF, .equipSfx = 0x2b },
};
#undef FLAGS_STAFF
// clang-format on
