#include "gba_io.h"
#include "gfx.h"
#include "system.h"

/**
 * gfx_fadeOutToBlack counterpart of gfx_windowFadeInFromBlack; disables both windows once
 * the screen is black.
 *
 * @romaddress 0x0803e640
 */
void gfx_windowFadeOutToBlack(u16 targets)
{
    u16 level;

    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    for (level = 0; level < GFX_FADE_LEVELS; level++)
    {
        REG_BLDY = level;
        gfx_tickPaletteAnims();
        system_waitVBlank();
        system_waitVBlank();
    }
    REG_DISPCNT &= ~(GFX_DISPCNT_WIN0 | GFX_DISPCNT_WIN1);
}
