#include "vector2.h"
#include "gba.h"
#include "variables.h"

// Octagonal length approximation of a component pair: the longer one plus 3/8 of the shorter.
#define OCTAGONAL_LENGTH(shorter, longer) ((((shorter) * 3) >> 3) + (longer))

/**
 * Returns the octagonal length approximation of the given vector.
 *
 * @romaddress 0x080328bc
 */
u32 vector2_length(Vector2Int v)
{
    Vector2Int absV = vector2_abs(v);
    u32 result;

    if (absV.x <= absV.y)
    {
        result = OCTAGONAL_LENGTH(absV.x, absV.y);
    }
    else
    {
        result = OCTAGONAL_LENGTH(absV.y, absV.x);
    }
    return result;
}

/**
 * Componentwise addition: out = a + b.
 *
 * @romaddress 0x080328f0
 */
void vector2_add(const Vector2Fp16 *a, const Vector2Fp16 *b, Vector2Fp16 *out)
{
    out->x = a->x + b->x;
    out->y = a->y + b->y;
}

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
 * fp16_16 -> integer conversion, componentwise (>> 16, so negatives floor). The result is
 * narrowed to s16: integer parts outside -32768..32767 wrap.
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
 * Classify a vector into a Direction8 (e.g. attacker->victim for knockback/impact
 * facing). Uses sign and dominant axis of (v.x, v.y); +y is south. Scale-invariant:
 * callers pass both fp16_16 and integer vectors.
 *
 * @romaddress 0x08032970
 */
Direction8 vector2_direction8(Vector2Int v)
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
    return g_Direction8sFromVectors[flags];
}

/**
 * Classify an fp16_16 vector into a 32-way direction index via BIOS ArcTan2.
 *
 * @romaddress 0x080329b4
 */
Direction32 vector2_direction32(Vector2Fp16 v)
{
    s16 x16;
    s16 y16;
    s32 angle;
    u32 angle16;
    u32 sector;

    v.x >>= 16;
    v.y >>= 16;
    x16 = v.x;
    y16 = v.y;
    angle = bios_arcTan2(x16, y16);

    angle16 = (u16)angle;
    // 32 sectors over the 0..0xffff angle; the +0x800 pulls each sector boundary down by 64
    // angle units, a 32nd of a sector. ArcTan2's origin is east, so adding DIRECTION32_EAST
    // puts sector 0 on north.
    sector = (angle16 * DIRECTION32_COUNT + 0x800) >> 16;
    sector += DIRECTION32_EAST;
    return sector & (DIRECTION32_COUNT - 1);
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

    if (absDiff.x <= absDiff.y)
    {
        return OCTAGONAL_LENGTH(absDiff.x, absDiff.y);
    }
    return OCTAGONAL_LENGTH(absDiff.y, absDiff.x);
}

/**
 * Rotate a 32-way direction one sector toward target, taking the shorter way around the circle.
 *
 * @param outStep receives the step that was taken (+1 or -1)
 *
 * @romaddress 0x08032a1c
 */
Direction32 vector2_stepDirection32(Direction32 current, Direction32 target, s8 *outStep)
{
    if (((current - target) & (DIRECTION32_COUNT - 1)) <= DIRECTION32_COUNT / 2)
    {
        *outStep = -1;
    }
    else
    {
        *outStep = 1;
    }
    current += *outStep;
    // the step wrapped the u8: DIRECTION32_COUNT stepped up past the last sector, anything
    // else stepped down below the first
    if (current < DIRECTION32_COUNT)
    {
        return current;
    }
    if (current == DIRECTION32_COUNT)
    {
        return DIRECTION32_NORTH;
    }
    return DIRECTION32_COUNT - 1;
}
