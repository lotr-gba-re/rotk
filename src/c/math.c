#include "math.h"
#include "gba.h"

/**
 * Returns base * percent / 100, truncated toward zero.
 *
 * @romaddress 0x08032a5c
 */
s32 math_percentOf(s32 base, u16 percent)
{
    return bios_div(base * percent, 100);
}

/**
 * Returns value * 100 / total, truncated toward zero. The inverse of math_percentOf.
 *
 * @romaddress 0x08032a70
 */
s32 math_percentage(s32 value, s32 total)
{
    return bios_div(value * 100, total);
}

/**
 * Returns the given sign-magnitude value as fp16_16: bit 15 is the sign, the low 15 bits
 * are the magnitude in 7.8 format.
 *
 * @romaddress 0x08032a80
 */
fp16_16 math_signMagnitudeToFp16(s16 value)
{
    s32 scaled = (value << 8) & 0x7fffff;

    if (value < 0)
    {
        scaled = -scaled;
    }
    return scaled;
}

/**
 * Returns numerator / denominator as fp16_16, truncated toward zero. Overflows once
 * |numerator| reaches 32768.
 *
 * @romaddress 0x08032a9c
 */
fp16_16 math_divideIntToFp16(s32 numerator, s32 denominator)
{
    return (numerator << 16) / denominator;
}
