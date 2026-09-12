#include "font.h"
#include "gba_io.h"
#include "text.h"
#include "variables.h"

/**
 * Select the current font style: g_FontRenderCtx.fontDesc = &g_FontDescriptors[styleIndex].
 *
 * @param styleIndex font/size, a FontStyle (FONT_STYLE_8PX_A .. FONT_STYLE_21PX)
 * @param color    color/palette
 * @param style    style word
 *
 * @romaddress 0x0801d5c0
 */
void font_setStyle(FontStyle styleIndex, s32 color, s32 style)
{
    s32 paletteBits;
    g_FontRenderCtx.fontDesc = &g_FontDescriptors[styleIndex];
    g_FontRenderCtx.lineHeight = g_FontDescriptors[styleIndex].lineHeight;
    if (g_FontRenderCtx.colors256 == 1)
    {
        g_FontRenderCtx.colorIndex = color;
        paletteBits = 0;
    }
    else
    {
        g_FontRenderCtx.colorIndex = color & 0xf;
        paletteBits = (color >> 4) << 12;
    }
    g_FontRenderCtx.paletteBits = paletteBits;
    g_FontRenderCtx.style = style;
}

/**
 * Override the line advance used by the multi-line draw helpers.
 *
 * @romaddress 0x0801d5fc
 */
void font_setLineHeight(s32 lineHeight)
{
    g_FontRenderCtx.lineHeight = lineHeight;
}

/**
 * Measure str's rendered width in pixels with the current font (g_FontRenderCtx.fontDesc).
 * '@<digit>' inserts the matching g_FontScratchSlots entry's width ('@1' = slot 0).
 *
 * @return width in pixels
 *
 * @romaddress 0x0801d608
 */
u32 font_getStringWidth(char *str)
{
    FontDesc *fontDesc = g_FontRenderCtx.fontDesc;
    s32 width = 0;
    s32 glyphWidth;

    for (; *str != '\0'; str++)
    {
        if (*str == '@')
        {
            str++;
            glyphWidth = font_getStringWidth(g_FontScratchSlots[FONT_SCRATCH_SLOT_INDEX(*str)]);
        }
        else
        {
            glyphWidth = font_lookupGlyph(fontDesc, *str);
        }
        width += glyphWidth;
    }
    return width;
}

/**
 * Height in pixels of a single line of str in the current font (0 for an empty string).
 *
 * @romaddress 0x0801d64c
 */
u32 font_getStringHeight(char *str)
{
    u32 height = 0;

    if (*str != '\0')
    {
        height = g_FontRenderCtx.fontDesc->height;
    }
    return height;
}

/**
 * Draw *pstr as word-wrapped lines inside a maxWidth x maxHeight box, one line advance
 * (g_FontRenderCtx.lineHeight) apart, stopping when the text or the height runs out.
 * *pstr is left at the first undrawn text.
 *
 * @return charbase tile index one past the last written glyph
 *
 * @romaddress 0x0801d668
 */
s32 font_drawLinesInBox(s32 charbaseTileIndex, s32 x, s32 y, s32 maxWidth, s32 maxHeight,
                        char **pstr, s32 mode)
{
    u16 lineWidth;

    while (**pstr != '\0' && maxHeight >= g_FontRenderCtx.lineHeight)
    {
        charbaseTileIndex =
            font_drawLine(charbaseTileIndex, x, y, maxWidth, pstr, mode, &lineWidth);
        y += g_FontRenderCtx.lineHeight;
        maxHeight -= g_FontRenderCtx.lineHeight;
    }
    return charbaseTileIndex;
}

/**
 * Draw all of str as word-wrapped lines of at most maxWidth px.
 *
 * @return charbase tile index one past the last written glyph
 *
 * @romaddress 0x0801d6d0
 */
s32 font_drawLines(s32 charbaseTileIndex, s32 x, s32 y, s32 maxWidth, char *str, s32 mode)
{
    u16 lineWidth;

    while (*str != '\0')
    {
        charbaseTileIndex =
            font_drawLine(charbaseTileIndex, x, y, maxWidth, &str, mode, &lineWidth);
        y += g_FontRenderCtx.lineHeight;
    }
    return charbaseTileIndex;
}

/**
 * Fill screen entries colStart..colEnd of one text-BG row with entry.
 *
 * @romaddress 0x0801d714
 */
void font_fillScreenRow(s32 colStart, s32 row, s32 colEnd, u16 entry)
{
    volatile u16 fill;

    if (colStart <= colEnd)
    {
        u32 dst = g_FontRenderCtx.screenbaseAddr + (row * 32 + colStart) * 2;
        s32 bytes = (colEnd - colStart + 1) * 2;

        fill = entry;
        DMA3_FILL16(&fill, dst, bytes >> 1);
    }
}

/**
 * Clear screen entries colStart..colEnd of one text-BG row to tile 0.
 *
 * @romaddress 0x0801d75c
 */
void font_clearScreenRow(s32 colStart, s32 row, s32 colEnd)
{
    volatile u16 zero;

    if (colStart <= colEnd)
    {
        u32 dst = g_FontRenderCtx.screenbaseAddr + (row * 32 + colStart) * 2;
        s32 bytes = (colEnd - colStart + 1) * 2;

        zero = 0;
        DMA3_FILL16(&zero, dst, bytes >> 1);
    }
}

static inline void copyStringToSlotInline(char *src, s32 slotIndex)
{
    char *dst = g_FontScratchSlots[slotIndex];

    while (*src != '\0')
    {
        *dst++ = *src++;
    }
    *dst = '\0';
}

/**
 * Copy src (NUL-terminated) into g_FontScratchSlots slot slotIndex.
 *
 * @param slotIndex zero-based slot ('@1' = slot 0)
 *
 * @romaddress 0x0801d7a4
 */
void font_copyStringToSlot(char *src, s32 slotIndex)
{
    copyStringToSlotInline(src, slotIndex);
}

/**
 * Copy src (NUL-terminated) into g_FontScratchSlots slot 0 ('@1').
 *
 * @romaddress 0x0801d7c8
 */
void font_copyStringToSlot0(char *src)
{
    copyStringToSlotInline(src, 0);
}

/**
 * Copy src (NUL-terminated) into g_FontScratchSlots slot 1 ('@2').
 *
 * @romaddress 0x0801d7e8
 */
void font_copyStringToSlot1(char *src)
{
    copyStringToSlotInline(src, 1);
}

/**
 * Copy src (NUL-terminated) into g_FontScratchSlots slot 2 ('@3').
 *
 * @romaddress 0x0801d80c
 */
void font_copyStringToSlot2(char *src)
{
    copyStringToSlotInline(src, 2);
}

static inline void formatIntSlotInline(s32 value, s32 slotIndex)
{
    text_formatInt(value, g_FontScratchSlots[slotIndex]);
}

/**
 * Format value as decimal with separators into g_FontScratchSlots slot slotIndex.
 *
 * @param slotIndex zero-based slot ('@1' = slot 0)
 *
 * @romaddress 0x0801d830
 */
void font_formatIntSlot(s32 value, s32 slotIndex)
{
    formatIntSlotInline(value, slotIndex);
}

/**
 * Format value as decimal with separators into g_FontScratchSlots slot 0 ('@1').
 *
 * @romaddress 0x0801d844
 */
void font_formatIntSlot0(s32 value)
{
    formatIntSlotInline(value, 0);
}

/**
 * Format value as decimal with separators into g_FontScratchSlots slot 1 ('@2').
 *
 * @romaddress 0x0801d854
 */
void font_formatIntSlot1(s32 value)
{
    formatIntSlotInline(value, 1);
}

/**
 * Format value as decimal with separators into g_FontScratchSlots slot 2 ('@3').
 *
 * @romaddress 0x0801d868
 */
void font_formatIntSlot2(s32 value)
{
    formatIntSlotInline(value, 2);
}

/**
 * Fill tileCount tiles with one color, at the target BG's depth (color is a palette index
 * for 8bpp, a nibble for 4bpp).
 *
 * @romaddress 0x0801d87c
 */
void font_fillTiles(u32 *tiles, s32 tileCount, u32 color)
{
    s32 words;

    if (g_FontRenderCtx.colors256 == 1)
    {
        color += (color << 24) + (color << 16) + (color << 8);
        words = tileCount << 4;
    }
    else
    {
        color &= 0xf;
        color += color << 4;
        color += color << 8;
        color += color << 16;
        words = tileCount << 3;
    }
    while (--words != -1)
    {
        *tiles++ = color;
    }
}

/**
 * Gather the tiles of a column of rows screen cells (one screen entry per BG row, starting
 * at screenEntry) from charbase into dst, undoing a horizontal flip flag on the way.
 *
 * @romaddress 0x0801d8cc
 */
void font_readCellColumn(u32 *dst, s32 rows, u16 *screenEntry, s32 charbaseAddr)
{
    while (--rows != -1)
    {
        u32 *src = (u32 *)(charbaseAddr + (*screenEntry & 0x3ff) * g_FontRenderCtx.tileStride);
        s32 words = (u32)g_FontRenderCtx.tileStride >> 2;

        while (--words != -1)
        {
            *dst++ = *src++;
        }
        if (*screenEntry & 0x400)
        {
            font_flipTileHorizontal((u8 *)dst - g_FontRenderCtx.tileStride);
        }
        screenEntry += 32;
    }
}

/**
 * Write rows tiles from src into charbase at tileIndex.. and point a column of screen
 * cells (one per BG row, starting at screenEntry) at them. A 4bpp cell keeps its palette
 * bits when g_FontRenderCtx.style is -1, else takes g_FontRenderCtx.paletteBits.
 *
 * @romaddress 0x0801d958
 */
void font_writeCellColumn(u32 *src, s32 rows, u16 *screenEntry, s32 charbaseAddr, s32 tileIndex)
{
    while (--rows != -1)
    {
        u32 *dst = (u32 *)(charbaseAddr + tileIndex * g_FontRenderCtx.tileStride);
        s32 words = (u32)g_FontRenderCtx.tileStride >> 2;

        while (--words != -1)
        {
            *dst++ = *src++;
        }
        if (g_FontRenderCtx.colors256 == 1)
        {
            *screenEntry = tileIndex;
        }
        else if (g_FontRenderCtx.style == -1)
        {
            *screenEntry = (*screenEntry & 0xf000) | tileIndex;
        }
        else
        {
            *screenEntry = g_FontRenderCtx.paletteBits | tileIndex;
        }
        screenEntry += 32;
        tileIndex++;
    }
}

/**
 * Screen entry of the 32-cell-wide text BG covering pixel (x, y).
 *
 * @romaddress 0x0801d9f8
 */
u16 *font_screenEntryAt(s32 screenbaseAddr, s32 x, s32 y)
{
    y /= 8;
    x /= 8;
    return (u16 *)(screenbaseAddr + (y * 32 + x) * 2);
}

/**
 * Width of ch's glyph in fontDesc (chars outside firstChar..lastChar get the fallback
 * width, a NUL 0).
 *
 * @romaddress 0x0801da1c
 */
s32 font_lookupGlyph(FontDesc *fontDesc, s32 ch)
{
    u8 c = ch;
    s32 width = 0;

    if (c != '\0')
    {
        s32 index = c - fontDesc->firstChar;

        if (index >= 0 && index <= fontDesc->lastChar - fontDesc->firstChar)
        {
            width = fontDesc->glyphWidths[index];
        }
        else
        {
            width = fontDesc->glyphWidths[1];
        }
    }
    return width;
}

/**
 * Copy src (NUL-terminated) into g_FontScratchSlots slot 3 ('@4').
 *
 * @romaddress 0x0801da4c
 */
void font_copyStringToSlot3(char *src)
{
    copyStringToSlotInline(src, 3);
}
