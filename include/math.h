#pragma once

#include "types.h"

u32 math_percentOf(s32 base, u16 percent);
u32 math_percentage(s32 value, s32 total);
fp16_16 math_signMagnitudeToFp16(s16 value);
fp16_16 math_divideFp16(s32 numerator, s32 denominator);
