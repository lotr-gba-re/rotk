#pragma once

#include "types.h"

fp16_16 math_sinFp16(Angle8 angle);
fp16_16 math_cosFp16(Angle8 angle);
fp8_8 math_sinFp8(Angle8 angle);
fp8_8 math_cosFp8(Angle8 angle);
fp8_8 math_multiplyFp8(fp8_8 a, fp8_8 b);
fp8_8 math_divideFp8(fp8_8 numerator, fp8_8 denominator);
fp8_8 math_reciprocalFp8(fp8_8 value);
fp16_16 math_multiplyFp16(fp16_16 a, fp16_16 b);
fp16_16 math_reciprocalFp16(fp16_16 value);
fp16_16 math_divideFp16(fp16_16 numerator, fp16_16 denominator);

s32 math_percentOf(s32 base, u16 percent);
s32 math_percentage(s32 value, s32 total);
fp16_16 math_signMagnitudeToFp16(s16 value);
fp16_16 math_divideIntToFp16(s32 numerator, s32 denominator);
