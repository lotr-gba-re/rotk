#include "gba_io.h"
#include "gfx.h"
#include "sprite.h"
#include "system.h"

/**
 * Set the darkening blend to a fixed level (0 = untouched, 16 = full black) without animating.
 *
 * @romaddress 0x0803e0f8
 */
void gfx_fadeSetDarken(u16 targets, u16 level)
{
    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    if (level > GFX_FADE_LEVELS)
    {
        level = GFX_FADE_LEVELS;
    }
    REG_BLDY = level;
}

/**
 * Set the brightening blend to a fixed level (0 = untouched, 16 = full white) without animating.
 *
 * @romaddress 0x0803e120
 */
void gfx_fadeSetBrighten(u16 targets, u16 level)
{
    REG_BLDCNT = targets | GFX_BLEND_BRIGHTEN;
    if (level > GFX_FADE_LEVELS)
    {
        level = GFX_FADE_LEVELS;
    }
    REG_BLDY = level;
}

/**
 * Ramp the darkening blend from full black to none, blocking for two vblanks per step.
 *
 * @romaddress 0x0803e148
 */
void gfx_fadeInFromBlack(u16 targets)
{
    s32 level;

    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    for (level = GFX_FADE_LEVELS; level >= 0; level--)
    {
        REG_BLDY = level;
        system_waitVBlank();
        system_waitVBlank();
    }
}

/**
 * gfx_fadeInFromBlack that keeps the simulation running: every vblank wait is preceded by a
 * full actor/palette/BG tick, so the ramp costs two game frames per step instead of stalling.
 *
 * @romaddress 0x0803e178
 */
void gfx_fadeInFromBlackWhileRunning(u16 targets)
{
    s32 level;

    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    for (level = GFX_FADE_LEVELS; level >= 0; level--)
    {
        REG_BLDY = level;
        actor_tickAll();
        gfx_tickPaletteAnims();
        gfx_tickBgTransforms();
        sprite_endFrame();
        system_waitVBlank();
        actor_tickAll();
        gfx_tickPaletteAnims();
        gfx_tickBgTransforms();
        sprite_endFrame();
        system_waitVBlank();
    }
}

/**
 * Ramp the brightening blend from full white to none, ticking the palette animations along.
 *
 * @romaddress 0x0803e1c8
 */
void gfx_fadeInFromWhite(u16 targets)
{
    s32 level;

    REG_BLDCNT = targets | GFX_BLEND_BRIGHTEN;
    for (level = GFX_FADE_LEVELS; level >= 0; level--)
    {
        REG_BLDY = level;
        gfx_tickPaletteAnims();
        system_waitVBlank();
        system_waitVBlank();
    }
}
