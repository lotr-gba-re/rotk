#pragma once

#include "types.h"

s32 rng_rollRange(s32 lo, s32 hi);
/** As rng_rollRange, but drawn from the MT19937 stream instead of the gameplay stream. */
s32 rng_range(s32 lo, s32 hi);
bool rng_rollPercent(s32 percent);
s32 rng_mod(u32 n);
void rng_reseed(void);
