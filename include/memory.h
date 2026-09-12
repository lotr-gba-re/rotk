#pragma once

// The game's memory management functions. This TU likely spans the region 0x08032aa8..0x08032fff.

#include <stddef.h>

/** Allocate `size` bytes (rounded up to 16) from the game heap, uninitialized. Returns NULL on
 * failure. */
void *memory_malloc(size_t size);

/** memory_malloc plus a zero fill. */
void *memory_zalloc(size_t size);

/** Free a heap block previously returned by memory_malloc or memory_zalloc. */
void memory_free(void *ptr);

/** Fill the first count bytes of dest with value. Returns dest. */
void *memory_memset(void *dest, int value, size_t count);

/** Copy n bytes from src to dest (4-byte aligned fast path). Returns dest. */
void *memory_memcpy(void *dest, const void *src, size_t n);

/** Copy n bytes from src to dest, correct when the two overlap. Returns dest. */
void *memory_memmove(void *dest, const void *src, size_t n);
