#pragma once

#include "text.h"
#include "types.h"

/** Font descriptor (g_FontDescriptors[5], one per FontStyle). Glyphs are 2bpp bitmaps. */
typedef struct FontDesc
{
    u8 height;
    u8 firstChar;
    u8 lastChar;
    u8 lineHeight;
    u16 *glyphOffsets; // per glyph, byte offset into glyphBitmaps
    u8 *glyphWidths;   // per glyph, in pixels; [1] is the fallback for unknown chars
    u8 *glyphBitmaps;
} FontDesc;
/**
 * Text render state (g_FontRenderCtx). screenbaseAddr/charbaseAddr are the active
 * text-BG VRAM bases the rasterizer writes to; several other fields still unnamed.
 */
typedef struct FontRenderCtx
{
    FontDesc *fontDesc;
    s32 colorIndex;  // added to each 2bpp glyph pixel value (4bpp: low nibble of the color)
    s32 style;       // -1 draws over the existing tiles and keeps their screen-entry palette
    s32 paletteBits; // screen-entry palette bits (color >> 4 << 12) for 4bpp BGs
    s32 lineHeight;
    s32 screenbaseAddr;
    s32 field_18;
    s32 charbaseAddr;
    s32 tileStride; // bytes per tile: 0x20 (4bpp) or 0x40 (8bpp)
    s32 colors256;  // 1 when the target BG is 8bpp
    s32 field_28;
    void *tileScratch; // 0x800-byte heap buffer
    s32 dirtyFlag;
} FontRenderCtx;
/**
 * Font/glyph size selected by font_setStyle (which descriptor font_drawString rasterizes).
 * 5 fonts by pixel height; names provisional (by size) - refine as each style's role is known.
 */
typedef enum FontStyle
{
    FONT_STYLE_8PX_A = 0,
    FONT_STYLE_8PX_B = 1,
    FONT_STYLE_11PX_A = 2,
    FONT_STYLE_11PX_B = 3,
    FONT_STYLE_21PX = 4,
} FontStyle;

void font_setStyle(FontStyle styleIndex, s32 color, s32 style);
void font_setLineHeight(s32 lineHeight);

/** Load every FontDesc, clear g_FontRenderCtx, and allocate its tile scratch buffer. */
void font_init(void);
/** Point the rasterizer at raw VRAM bases; font_setRenderCtxBg derives them from a BGCNT. */
void font_setRenderTarget(s32 screenbaseAddr, s32 charbaseAddr, s32 colors256);
/** Mirror one tile (4bpp or 8bpp per g_FontRenderCtx.colors256) left to right in place. */
void font_flipTileHorizontal(u8 *tile);

/** One '@<digit>' scratch string (NUL-terminated), filled by font_formatIntSlot0/1. */
typedef char FontScratchSlot[0x40];

/** '@<digit>' escape char -> g_FontScratchSlots index ('@1' is slot 0). */
#define FONT_SCRATCH_SLOT_INDEX(chr) (CHAR_TO_DIGIT(chr) - 1)

u32 font_getStringWidth(char *str);
u32 font_getStringHeight(char *str);
void font_loadStyleDescriptor(s32 styleIndex);
s32 font_lookupGlyph(FontDesc *fontDesc, s32 ch);

/**
 * Draw one word-wrapped line of *pstr (at most maxWidth px, breaking at spaces or
 * after '-', or at a '\n') with font_drawString and advance *pstr past it and any
 * following spaces.
 *
 * @param widthOut receives the drawn line's pixel width
 *
 * @return charbase tile index one past the last written glyph
 */
s32 font_drawLine(s32 charbaseTileIndex, s32 x, s32 y, s32 maxWidth, char **pstr, s32 mode,
                  u16 *widthOut);
s32 font_drawLinesInBox(s32 charbaseTileIndex, s32 x, s32 y, s32 maxWidth, s32 maxHeight,
                        char **pstr, s32 mode);
s32 font_drawLines(s32 charbaseTileIndex, s32 x, s32 y, s32 maxWidth, char *str, s32 mode);
void font_fillScreenRow(s32 colStart, s32 row, s32 colEnd, u16 entry);
void font_clearScreenRow(s32 colStart, s32 row, s32 colEnd);
void font_fillTiles(u32 *tiles, s32 tileCount, u32 color);
void font_readCellColumn(u32 *dst, s32 rows, u16 *screenEntry, s32 charbaseAddr);
void font_writeCellColumn(u32 *src, s32 rows, u16 *screenEntry, s32 charbaseAddr, s32 tileIndex);
u16 *font_screenEntryAt(s32 screenbaseAddr, s32 x, s32 y);

/**
 * Draw the given string at screen pixel (x, y) with alignment per mode. Each glyph is written as a
 * 4bpp tile into charbase at charbaseTileIndex AND the matching screenbase entry routing the
 * on-screen cell to that tile is written (the text-BG base addresses come from g_FontRenderCtx).
 * Wraps font_rasterizeString after computing alignment offsets.
 *
 * @param charbaseTileIndex charbase tile index to write the first glyph at (also stored
 *                          as the tile number in each screenbase entry)
 * @param mode              alignment: 0 = x is the left edge and y the top, 1 = centered on x,
 *                          2 = right-aligned to x, 4 = centered on y, 8 = bottom-aligned to y.
 *                          The x and y codes are compared, not masked, so they do not combine.
 *
 * @return charbase tile index one past the last written glyph
 */
s32 font_drawString(s32 charbaseTileIndex, s32 x, s32 y, char *str, s32 mode);

/**
 * Rasterize str as glyphs starting at screen pixel (x, y) and charbase tile
 * charbaseTileIndex. For each glyph tile: writes the 4bpp pattern into charbase
 * (at charbaseAddr + charbaseTileIndex*0x20) and writes the screenbase entry
 * (palette | charbaseTileIndex) at cell (x/8, y/8). No alignment; font_drawString
 * wraps this after computing offsets.
 *
 * @param charbaseTileIndex charbase tile index to write the first glyph at (also stored
 *                          as the tile number in each screenbase entry)
 *
 * @return charbase tile index one past the last written glyph
 */
s32 font_rasterizeString(s32 charbaseTileIndex, s32 x, s32 y, char *str);

void font_formatIntSlot(s32 value, s32 slotIndex);
void font_formatIntSlot0(s32 value);
void font_formatIntSlot1(s32 value);
void font_formatIntSlot2(s32 value);

/** Set the font render context's screen/charbase from a BGCNT value. */
void font_setRenderCtxBg(u32 bgcnt);

void font_copyStringToSlot(char *src, s32 slotIndex);
void font_copyStringToSlot0(char *src);
void font_copyStringToSlot1(char *src);
void font_copyStringToSlot2(char *src);
void font_copyStringToSlot3(char *src);
