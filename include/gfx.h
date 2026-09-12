#pragma once

#include "types.h"
#include "vector2.h"

/** DISPCNT bits 0-2: BG mode. */
#define GFX_DISPCNT_MODE_MASK 0x7

/** DISPCNT display-enable bits. */
#define GFX_DISPCNT_BG0 (1 << 8)
#define GFX_DISPCNT_BG1 (1 << 9)
#define GFX_DISPCNT_BG2 (1 << 10)
#define GFX_DISPCNT_BG3 (1 << 11)
#define GFX_DISPCNT_OBJ (1 << 12)
#define GFX_DISPCNT_WIN0 (1 << 13)
#define GFX_DISPCNT_WIN1 (1 << 14)

/** BGxCNT bit 6: BG mosaic enable. */
#define GFX_BGCNT_MOSAIC (1 << 6)

/** MOSAIC stretch steps: the register packs four 4-bit sizes, so a ramp tops out at 15. */
#define GFX_MOSAIC_MAX 15

/**
 * BLDCNT first-target bits 0-5 (BG0-3, OBJ, backdrop) all set: the fade/mosaic
 * affects every layer.
 */
#define GFX_BLEND_ALL_TARGETS 0x3f

// BLDCNT bits 6-7 select the effect applied to the first targets.
#define GFX_BLEND_ALPHA (1 << 6)
#define GFX_BLEND_BRIGHTEN (2 << 6)
#define GFX_BLEND_DARKEN (3 << 6)

/** BLDY steps: 0 = unblended, 16 = fully black (darken) or white (brighten). */
#define GFX_FADE_LEVELS 16

void gfx_fadeSetDarken(u16 targets, u16 level);
void gfx_fadeSetBrighten(u16 targets, u16 level);
void gfx_fadeInFromBlack(u16 targets);
void gfx_fadeInFromBlackWhileRunning(u16 targets);
void gfx_fadeInFromWhite(u16 targets);
void gfx_windowFadeInFromBlack(u16 targets);
void gfx_fadeOutToBlack(u16 targets);
void gfx_fadeOutToBlackWhileRunning(u16 targets);
void gfx_fadeOutToWhite(u16 targets);
void gfx_windowFadeOutToBlack(u16 targets);
void gfx_alphaBlendIn(u16 targets, u16 secondTargets, s32 steps, s32 framesPerStep);
void gfx_alphaBlendOut(u16 targets, u16 secondTargets, s32 steps, s32 framesPerStep);
void gfx_mosaicInFromBlack(u16 targets);
void gfx_mosaicOutToBlack(u16 targets);
void gfx_fillBgRect(s32 layer, s32 col, s32 row, s32 width, s32 height, s32 param6);

void gfx_initDisplay(u16 mode);
void gfx_enableDisplayFlags(u16 mask);
void gfx_setupBg(u32 layer, u32 bgcnt);
void gfx_disableBg(u32 layer);
void gfx_setBgOffset(u32 layer, u32 offset);
/*
 * BG asset layout: u8 flags, u8 flags2, then the parts the flags select, in this order:
 * a 256- or 16-color palette, a raw VRAM block (u16 size, bytes), a tilemap (u16 width,
 * u16 height, entries padded to an even count) and the tiles (u16 stream size, stream).
 * A map entry is tile bits 0-9, hflip bit 10, vflip bit 11, palette bank bits 12-15.
 * The carved assets (carved/data/hero_portraits/, dialog_portraits/, cutscenes/)
 * spell this out as structs.
 */
#define BG_ASSET_PALETTE_256 (1 << 0)
#define BG_ASSET_PALETTE_16 (1 << 1)
#define BG_ASSET_RAW_BLOCK (1 << 2)
#define BG_ASSET_MAP (1 << 3)
#define BG_ASSET_TILES (1 << 4)
#define BG_ASSET_CODEC_RAW (0 << 5)      // bits 5-6: tile codec
#define BG_ASSET_CODEC_PUCRUNCH (3 << 5) // decompress_pucrunch
#define BG_ASSET_8BPP (1 << 7)
#define BG_ASSET2_BYTE_MAP (1 << 0) // flags2: map entries are bytes

/**
 * Define a BG asset of colors_ palette entries and a tilesWide x tilesHigh tilemap, its parts
 * taken from the name##_PALETTE / _MAP / _TILES byte lists rotkit build gfx generates from its
 * PNG. The stored map is padded to an even entry count.
 */
#define BG_ASSET_MAPPED(name, flags_, colors_, tilesWide, tilesHigh)                               \
    const struct __attribute__((aligned(4)))                                                       \
    {                                                                                              \
        u8 flags, flags2;                                                                          \
        u16 palette[colors_];                                                                      \
        u16 mapWidth, mapHeight;                                                                   \
        u16 map[((tilesWide) * (tilesHigh) + 1) & ~1];                                             \
        u16 tilesSize;                                                                             \
        u8 tiles[name##_TILES_SIZE];                                                               \
    } name = {.flags = flags_,                                                                     \
              .palette = {name##_PALETTE},                                                         \
              .mapWidth = (tilesWide),                                                             \
              .mapHeight = (tilesHigh),                                                            \
              .map = {name##_MAP},                                                                 \
              .tilesSize = name##_TILES_SIZE,                                                      \
              .tiles = {name##_TILES}}
/** BG_ASSET_MAPPED for an asset that carries no tilemap, only a palette and tiles. */
#define BG_ASSET_UNMAPPED(name, flags_, colors_)                                                   \
    const struct __attribute__((aligned(4)))                                                       \
    {                                                                                              \
        u8 flags, flags2;                                                                          \
        u16 palette[colors_];                                                                      \
        u16 tilesSize;                                                                             \
        u8 tiles[name##_TILES_SIZE];                                                               \
    } name = {.flags = flags_,                                                                     \
              .palette = {name##_PALETTE},                                                         \
              .tilesSize = name##_TILES_SIZE,                                                      \
              .tiles = {name##_TILES}}

void gfx_loadBgAsset(u32 layer, const void *asset, u16 arg3, u16 arg4, u32 arg5, u32 arg6);
/**
 * Upload a palette asset: a 256-color asset fills palette RAM, a 16-color one lands at
 * (asset's own palette number + paletteNumber) * 16 colors.
 */
void gfx_loadPalette(const void *paletteAsset, u32 paletteNumber);
/**
 * Copy colorCount raw colors into a palette shadow buffer at firstColor and upload them
 * to palette RAM (the BG or OBJ half).
 */
void gfx_loadBgPaletteColors(const u16 *colors, u32 firstColor, u32 colorCount);
void gfx_loadObjPaletteColors(const u16 *colors, u32 firstColor, u32 colorCount);
/**
 * Queue colorCount raw colors for palette RAM at firstColor; gfx_flushPaletteWrites
 * commits the queue from the vblank handler.
 */
void gfx_queuePaletteWrite(const u16 *colors, u32 firstColor, u32 colorCount);
void gfx_commitBgs(void);
void gfx_setBlendControl(u32 arg1, u32 arg2);
void gfx_setBlendAlpha(u32 arg1, u32 arg2);

/**
 * One BLDALPHA coefficient (EVA or EVB) as an 8.8 value ping-ponging between min and max by
 * delta per tick; the integer part is what reaches the register.
 */
typedef struct BlendCoefficient
{
    s16 value;
    s16 delta;
    s16 min;
    s16 max;
} BlendCoefficient;

/*
 * Palette animations run from two slot tables: 12 palette cycles (rotate a range of palette
 * entries by one step every framesPerStep frames, flags bit 0x10 = ping-pong) and 4 palette
 * fades (lerp a range toward a target palette, optionally stepping through keyframe
 * palettes). Each stepped slot is committed to palette RAM from the vblank handler.
 */
/**
 * Register palette cycles from a counted table: u16 entry count at offset 0, then 4-byte
 * entries {firstIndex, count, framesPerStep, flags} from offset 4.
 */
void gfx_registerPaletteCycles(const void *table);
/** Stop every palette cycle and fade and drop the queued palette writes. */
void gfx_clearPaletteAnims(void);
// Vblank-side commits run by system_vBlankIrqHandler: push the shadow BG scroll and
// rotation/scaling state to BGxHOFS/VOFS and BG2/3 PA..Y, apply the stepped palette
// animations, and flush the queued palette writes.
void gfx_commitBgRegisters(void);
void gfx_commitPaletteAnims(void);
void gfx_flushPaletteWrites(void);
/** Stop and reset one BG's transform animation (slot state, HOFS/VOFS). */
void gfx_resetBgTransform(u32 layer);
/**
 * Fade-out that keeps the per-frame ticks running and syncs over the link (co-op
 * transition path of scene_gameplay_exit).
 */
void gfx_fadeOutToBlackCoop(s32 targets);

/** Fade-in counterpart of gfx_fadeOutToBlackCoop, used by the co-op overlay scenes. */
void gfx_fadeInFromBlackCoop(s32 targets);

/** Set a BG layer's scroll velocity, in px per frame. */
void gfx_setBgScrollSpeed(u32 layer, Vector2Fp16 speed);

/** Horizontal-only gfx_setBgScrollSpeed. */
void gfx_setBgScrollSpeedX(u32 layer, fp16_16 speedX);

/** Unnamed BG/tilemap helpers (roles not yet reversed). */
void gfx_unknown08012444(s32 a);

/** Per-player camera scroll update, clamped to the level bounds. */
void gfx_updatePlayerCameraScroll(u8 playerIndex);

/**
 * Per-frame BG transform tick: advances all 4 BG slots (position interpolation,
 * oscillation, rotation) and, for BG2/3 rotation/scaling slots, recomputes PA..PD and the
 * reference point.
 */
void gfx_tickBgTransforms(void);

/** Per-frame step of every active palette cycle and palette fade. */
void gfx_tickPaletteAnims(void);
