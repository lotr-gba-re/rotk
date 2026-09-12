#include "vector2.h"
#include "gba.h"
#include "variables.h"

/**
 * Componentwise subtraction: out = a - b.
 *
 * @romaddress 0x08032908
 */
void vector2_subtract(const Vector2Fp16 *a, const Vector2Fp16 *b, Vector2Fp16 *out)
{
    out->x = a->x - b->x;
    out->y = a->y - b->y;
}

/**
 * Componentwise copy: dst = src.
 *
 * @romaddress 0x08032920
 */
void vector2_copy(const Vector2Fp16 *src, Vector2Fp16 *dst)
{
    dst->x = src->x;
    dst->y = src->y;
}

/**
 * fp16_16 -> integer conversion, componentwise (>> 16). The integer part is truncated
 * to s16: values beyond +-32767 wrap.
 *
 * @param src the fp16_16 coordinates (e.g. &Actor.coords)
 * @param dst receives the integer coordinates
 *
 * @romaddress 0x0803292c
 */
void vector2_fromFp16(const Vector2Fp16 *src, Vector2Int *dst)
{
    dst->x = (s16)(src->x >> 16);
    dst->y = (s16)(src->y >> 16);
}

/**
 * Integer -> fp16_16 conversion, componentwise (<< 16).
 *
 * @romaddress 0x0803293c
 */
void vector2_toFp16(const Vector2Int *src, Vector2Fp16 *dst)
{
    dst->x = src->x << 16;
    dst->y = src->y << 16;
}

/**
 * Componentwise equality.
 *
 * @romaddress 0x0803294c
 */
bool vector2_equals(const Vector2Fp16 *a, const Vector2Fp16 *b)
{
    bool equal = FALSE;

    if (a->x == b->x && a->y == b->y)
    {
        equal = TRUE;
    }
    return equal;
}

/**
 * Classify a vector into an 8-way Direction (e.g. attacker->victim for knockback/impact
 * facing). Uses sign and dominant axis of (v.x, v.y); +y is south. Scale-invariant:
 * callers pass both fp16_16 and integer vectors.
 *
 * @romaddress 0x08032970
 */
Direction vector2_direction8(Vector2Int v)
{
    u8 flags = 0;

    if (v.x < 0)
    {
        v.x = -v.x;
        flags |= 1;
    }
    if (v.y < 0)
    {
        v.y = -v.y;
        flags |= 2;
    }
    if (v.x > v.y * 2)
    {
        flags |= 4;
    }
    if (v.x * 2 < v.y)
    {
        flags |= 8;
    }
    return g_DirectionsFromVectors[flags];
}

/**
 * Classify an fp16_16 vector into a 32-way direction index via BIOS ArcTan2.
 *
 * @romaddress 0x080329b4
 */
u8 vector2_direction32(Vector2Fp16 v)
{
    s16 x16;
    s16 y16;
    s32 angle;
    u32 angle16;
    u32 dir;

    v.x >>= 16;
    v.y >>= 16;
    x16 = v.x;
    y16 = v.y;
    angle = bios_arcTan2(x16, y16);

    angle16 = (u16)angle;
    // 32 sectors over the 0..0xffff angle; 0x800 rounds the sector, +8 and & 0x1f
    // shift and wrap the 8-offset direction grid.
    dir = (angle16 * 0x20 + 0x800) >> 16;
    dir += 8;
    return dir & 0x1f;
}

/**
 * Octagonal distance approximation (max + 3/8*min) between two positions. Scale-invariant:
 * the result is in the same unit as the inputs (callers use both integer and fp16_16).
 *
 * @romaddress 0x080329e0
 */
u32 vector2_distance(Vector2Int a, Vector2Int b)
{
    Vector2Int absDiff = vector2_abs(vector2_diff(a, b));

    // min is the shorter component: max + 3/8 * min
    if (absDiff.x <= absDiff.y)
    {
        return ((absDiff.x * 3) >> 3) + absDiff.y;
    }
    return ((absDiff.y * 3) >> 3) + absDiff.x;
}
