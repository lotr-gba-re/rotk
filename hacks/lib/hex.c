#include "hex.h"

char *hackLib_formatHexByte(char *dst, u8 byte)
{
    static const char digits[] = "0123456789ABCDEF";

    *dst++ = digits[byte >> 4];
    *dst++ = digits[byte & 0xF];
    return dst;
}

char hackLib_byteToFontChar(u8 byte)
{
    // '@' is font_rasterizeString's format placeholder.
    // If byte is this placeholder or out of range, return '.'
    if (byte < 0x20 || byte > 0x7A || byte == '@')
    {
        return '.';
    }

    return (char)byte;
}
