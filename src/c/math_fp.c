#include "gba.h"
#include "match_hacks.h"
#include "math.h"
#include "variables.h"

/**
 * Returns sin(angle) as fp16_16.
 *
 * @romaddress 0x08032788
 */
fp16_16 math_sinFp16(Angle8 angle)
{
    Angle8 index = angle;
    u8 quadrant = index / ANGLE8_QUARTER_TURN;

    if (quadrant & 1)
    {
        // mirror by using wraparound
        index = ANGLE8_HALF_TURN - index;
    }
    if (quadrant & 2)
    {
        // negate
        return -SineQuarterTurn[index - ANGLE8_HALF_TURN];
    }
    return SineQuarterTurn[index];
}

/**
 * Returns cos(angle) as fp16_16.
 *
 * @romaddress 0x080327cc
 */
fp16_16 math_cosFp16(Angle8 angle)
{
    Angle8 index = angle + ANGLE8_QUARTER_TURN;
    u8 quadrant = index / ANGLE8_QUARTER_TURN;

    if (quadrant & 1)
    {
        // mirror by using wraparound
        index = ANGLE8_HALF_TURN - index;
    }

    // negate?
    return (quadrant & 2) ? -SineQuarterTurn[index - ANGLE8_HALF_TURN] : SineQuarterTurn[index];
}

/**
 * Returns sin(angle) as fp8_8.
 *
 * @romaddress 0x08032818
 */
fp8_8 math_sinFp8(Angle8 angle)
{
    return math_sinFp16(angle) >> 8;
}

/**
 * Returns cos(angle) as fp8_8.
 *
 * @romaddress 0x0803282c
 */
fp8_8 math_cosFp8(Angle8 angle)
{
    return math_cosFp16(angle) >> 8;
}

/**
 * Returns a * b, truncated toward zero.
 *
 * @romaddress 0x08032840
 */
fp8_8 math_multiplyFp8(fp8_8 a, fp8_8 b)
{
    s32 product = a * b;
    s32 biased;

    // The bias makes the shift truncate toward zero like a signed divide; MATCH_FRESH keeps
    // it in its own register, as the ROM sign-tests the unbiased product.
    MATCH_FRESH(biased, product);
    if (product < 0)
    {
        biased += FP8_8_ONE - 1;
    }
    return biased >> FP8_8_SHIFT;
}

/**
 * Returns numerator / denominator, truncated toward zero.
 *
 * @romaddress 0x0803285c
 */
fp8_8 math_divideFp8(fp8_8 numerator, fp8_8 denominator)
{
    return numerator * FP8_8_ONE / denominator;
}

/**
 * Returns 1 / value, truncated toward zero.
 *
 * @romaddress 0x08032874
 */
fp8_8 math_reciprocalFp8(fp8_8 value)
{
    s32 numerator = FP8_8_ONE * FP8_8_ONE;

    return numerator / value;
}

/**
 * Returns a * b. Both operands drop their low 6 bits first, so the product is approximate and
 * overflows once |a * b| reaches 2048.
 *
 * @romaddress 0x0803288c
 */
fp16_16 math_multiplyFp16(fp16_16 a, fp16_16 b)
{
    return ((a >> 6) * (b >> 6)) >> 4;
}

/**
 * Returns 1 / value truncated to a whole number, so any |value| above 1.0 gives 0.
 *
 * @romaddress 0x08032898
 */
fp16_16 math_reciprocalFp16(fp16_16 value)
{
    return (FP16_16_ONE / value) << FP16_16_SHIFT;
}

/**
 * Returns numerator / denominator. The denominator drops its low 6 bits first, so the quotient
 * is approximate and a denominator below 2^-10 divides by zero. The numerator overflows once
 * |numerator| reaches 512.
 *
 * @romaddress 0x080328ac
 */
fp16_16 math_divideFp16(fp16_16 numerator, fp16_16 denominator)
{
    return bios_div(numerator << 6, denominator >> 6) << 4;
}
