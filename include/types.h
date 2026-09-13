#pragma once

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

/**
 * 16.16 fixed point (world coordinates, speeds). Plain s32 for codegen; the typedef
 * documents the format.
 */
typedef s32 fp16_16;

/** 8.8 fixed point. Plain s16 for codegen; the typedef documents the format. */
typedef s16 fp8_8;

#define FP8_8_SHIFT 8
#define FP16_16_SHIFT 16
#define FP8_8_ONE (1 << FP8_8_SHIFT)
#define FP16_16_ONE (1 << FP16_16_SHIFT)

/** Angle, 256 units per full turn. */
typedef u8 Angle8;

#define ANGLE8_QUARTER_TURN 64
#define ANGLE8_HALF_TURN 128

/** Angle, 65536 units per full turn. */
typedef u16 Angle16;

#ifndef __cplusplus
typedef unsigned char bool;
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/** element count of a static array (no pointer decay) */
#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

/** absolute value; evaluates v twice */
#define ABS(v) ((v) < 0 ? -(v) : (v))
