#pragma once

#include "types.h"
#include "vector2.h"

void math_init(void);

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

u32 math_vector2_length(Vector2Int v);
void math_vector2_add(const Vector2Fp16 *a, const Vector2Fp16 *b, Vector2Fp16 *out);
void math_vector2_subtract(const Vector2Fp16 *a, const Vector2Fp16 *b, Vector2Fp16 *out);
void math_vector2_copy(const Vector2Fp16 *src, Vector2Fp16 *dst);
void math_vector2_fromFp16(const Vector2Fp16 *src, Vector2Int *dst);
void math_vector2_toFp16(const Vector2Int *src, Vector2Fp16 *dst);
bool math_vector2_equals(const Vector2Fp16 *a, const Vector2Fp16 *b);
Direction8 math_vector2_direction8(Vector2Int v);
Direction32 math_vector2_direction32(Vector2Fp16 v);
u32 math_vector2_distance(Vector2Int a, Vector2Int b);
Direction32 math_vector2_stepDirection32(Direction32 current, Direction32 target, s8 *outStep);

s32 math_percentOf(s32 base, u16 percent);
s32 math_percentage(s32 value, s32 total);
fp16_16 math_signMagnitudeToFp16(s16 value);
fp16_16 math_divideIntToFp16(s32 numerator, s32 denominator);

/** Componentwise a - b. */
static inline Vector2Int math_vector2_diff(Vector2Int a, Vector2Int b)
{
    Vector2Int d = a;

    d.x -= b.x;
    d.y -= b.y;
    return d;
}

/** Componentwise absolute value. */
static inline Vector2Int math_vector2_abs(Vector2Int v)
{
    v.x = ABS(v.x);
    v.y = ABS(v.y);
    return v;
}
