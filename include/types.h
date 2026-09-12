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
