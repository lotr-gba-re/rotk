#include "gba_io.h"
#include "gfx.h"
#include "sprite.h"
#include "system.h"

/**
 * Ramp the darkening blend from none to full black, blocking for two vblanks per step.
 *
 * @romaddress 0x0803e374
 */
void gfx_fadeOutToBlack(u16 targets)
{
    u16 level;

    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    for (level = 0; level < GFX_FADE_LEVELS; level++)
    {
        REG_BLDY = level;
        system_waitVBlank();
        system_waitVBlank();
    }
}

/**
 * gfx_fadeOutToBlack that keeps the simulation running: every vblank wait is preceded by a
 * full actor/palette/BG tick, so the ramp costs two game frames per step instead of stalling.
 *
 * @romaddress 0x0803e3a8
 */
void gfx_fadeOutToBlackWhileRunning(u16 targets)
{
    u16 level;

    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    for (level = 0; level < GFX_FADE_LEVELS; level++)
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
 * Ramp the brightening blend from none to full white, ticking the palette animations along.
 *
 * @romaddress 0x0803e3fc
 */
void gfx_fadeOutToWhite(u16 targets)
{
    u16 level;

    REG_BLDCNT = targets | GFX_BLEND_BRIGHTEN;
    for (level = 0; level <= GFX_FADE_LEVELS; level++)
    {
        REG_BLDY = level;
        gfx_tickPaletteAnims();
        system_waitVBlank();
        system_waitVBlank();
    }
}
