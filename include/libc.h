#pragma once

// C runtime library helpers shipped in the game ROM (libc / libgcc). Hand-curated:
// prototypes for the handful of libc functions the decompiled code calls.

#include <stddef.h>

/** Fill the first count bytes of dest with value. Returns dest. */
void *memset(void *dest, int value, size_t count);

/** Copy n bytes from src to dest (4-byte aligned fast path). Returns dest. */
void *memcpy(void *dest, const void *src, size_t n);

/** Allocate `size` bytes (rounded up to 16) from the game heap, uninitialized. Returns NULL on
 * failure. */
void *malloc(size_t size);

/** malloc plus a zero fill. */
void *zalloc(size_t size);

/** Free a heap block previously returned by malloc or zalloc. */
void free(void *ptr);
