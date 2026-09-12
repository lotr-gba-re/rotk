#pragma once

#include "types.h"

/**
 * 8-way facing direction.
 * Value order is clockwise from North
 */
typedef enum Direction8
{
    /** Towards negative Y */
    DIRECTION8_NORTH = 0,
    /** Towards positive X and negative Y */
    DIRECTION8_NORTHEAST = 1,
    /** Towards positive X */
    DIRECTION8_EAST = 2,
    /** Towards positive X and positive Y */
    DIRECTION8_SOUTHEAST = 3,
    /** Towards positive Y */
    DIRECTION8_SOUTH = 4,
    /** Towards negative X and positive Y */
    DIRECTION8_SOUTHWEST = 5,
    /** Towards negative X */
    DIRECTION8_WEST = 6,
    /** Towards negative X and negative Y */
    DIRECTION8_NORTHWEST = 7,
    /** No direction (e.g. dpad idle); not a facing */
    DIRECTION8_NONE = 0xff
} __attribute__((packed)) Direction8;

/**
 * 32-way facing sector, clockwise from North on the same grid as Direction8 (sector
 * == direction8 * 4). Sectors between the named ones are unnamed.
 */
typedef enum Direction32
{
    DIRECTION32_NORTH = 0,
    DIRECTION32_NORTHEAST = 4,
    DIRECTION32_EAST = 8,
    DIRECTION32_SOUTHEAST = 12,
    DIRECTION32_SOUTH = 16,
    DIRECTION32_SOUTHWEST = 20,
    DIRECTION32_WEST = 24,
    DIRECTION32_NORTHWEST = 28,
    /** Sector count, one past the last sector */
    DIRECTION32_COUNT = 32
} __attribute__((packed)) Direction32;

/**
 * A 2D integer vector with no inherent unit. By-value arg of the scale-invariant ops:
 * the unit (integer, fp16_16, ...) is a call-site convention.
 */
typedef struct Vector2Int
{
    s32 x;
    s32 y;
} Vector2Int;
/** A 2D world vector in 16.16 fixed point (Actor.coords, Actor.velocity, FxEmitter.position). */
typedef struct Vector2Fp16
{
    fp16_16 x;
    fp16_16 y;
} Vector2Fp16;
Direction8 vector2_direction8(Vector2Int v);
Direction32 vector2_direction32(Vector2Fp16 v);
Direction32 vector2_stepDirection32(Direction32 current, Direction32 target, s8 *outStep);

void vector2_subtract(const Vector2Fp16 *a, const Vector2Fp16 *b, Vector2Fp16 *out);
void vector2_copy(const Vector2Fp16 *src, Vector2Fp16 *dst);
void vector2_fromFp16(const Vector2Fp16 *src, Vector2Int *dst);
void vector2_toFp16(const Vector2Int *src, Vector2Fp16 *dst);
bool vector2_equals(const Vector2Fp16 *a, const Vector2Fp16 *b);
u32 vector2_distance(Vector2Int a, Vector2Int b);

/** Componentwise a - b. */
static inline Vector2Int vector2_diff(Vector2Int a, Vector2Int b)
{
    Vector2Int d = a;

    d.x -= b.x;
    d.y -= b.y;
    return d;
}

/** Componentwise absolute value. */
static inline Vector2Int vector2_abs(Vector2Int v)
{
    v.x = ABS(v.x);
    v.y = ABS(v.y);
    return v;
}
