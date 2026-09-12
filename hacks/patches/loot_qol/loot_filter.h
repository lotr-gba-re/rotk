#pragma once

// Loot filter: void ground drops that fail the active filter settings (loot_filter.c).

#include "types.h"

// The "Require Affix" modes (the state byte's affix field, loot_qol.h): the affixes a
// drop must have to spawn.
#define LOOT_FILTER_AFFIX_OFF 0    // no requirement, drop all items
#define LOOT_FILTER_AFFIX_ANY 1    // drop only items with a prefix or a suffix
#define LOOT_FILTER_AFFIX_PREFIX 2 // drop only items with a prefix
#define LOOT_FILTER_AFFIX_SUFFIX 3 // drop only items with a suffix
#define LOOT_FILTER_AFFIX_BOTH 4   // drop only items with a prefix and a suffix
#define LOOT_FILTER_AFFIX_COUNT 5

/** Menu value names per mode (mod_options). */
extern const char *const LootFilterAffixNames[LOOT_FILTER_AFFIX_COUNT];

// The filter rows' mod_options accessors (compiled when HACK_mod_options).
u8 lootFilter_getAffix(void);
void lootFilter_setAffix(u8 value);
u8 lootFilter_getSuppressArrows(void);
void lootFilter_setSuppressArrows(u8 value);
