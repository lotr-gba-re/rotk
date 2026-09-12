#pragma once

#include "types.h"

/** Write `byte` as two uppercase hex chars to dst. Returns dst + 2; not terminated. */
char *hackLib_formatHexByte(char *dst, u8 byte);

/**
 * `byte` as the char the game font renders it as, '.' when the font would render
 * something else (the font is ASCII-compatible only for 0x20..0x7A, and '@' is the
 * formatter escape).
 */
char hackLib_byteToFontChar(u8 byte);
