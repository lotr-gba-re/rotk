#pragma once

// Auto loot: pick up gem stacks without an R press (auto_loot.c).

#include "types.h"

// The auto loot row's mod_options accessors (compiled when HACK_mod_options).
u8 autoLoot_getGems(void);
void autoLoot_setGems(u8 value);
