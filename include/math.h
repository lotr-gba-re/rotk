#pragma once

#include "types.h"

// Integer math helpers.
u32 math_percentOf(s32 base, s32 percent);
/** value*100/total - what percentage of total is value. */
u32 math_percentage(s32 value, s32 total);
