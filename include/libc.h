#pragma once

// C runtime library functions shipped in the game ROM.
// The game's own allocator and block moves live in memory.h.

#include <stddef.h>

/** Fill the first count bytes of dest with value. Returns dest. */
void *memset(void *dest, int value, size_t count);

/** Copy n bytes from src to dest. Returns dest. */
void *memcpy(void *dest, const void *src, size_t n);

/** Zero the first n bytes of dest. */
void bzero(void *dest, size_t n);
