#include "gba_io.h"
#include "gfx.h"
#include "system.h"

/**
 * gfx_fadeInFromBlack behind two windows that blank a top and a middle band of the screen
 * (window 0 rows 0-32, window 1 rows 128-160); the bands stay blanked afterwards.
 *
 * @romaddress 0x0803e2fc
 */
void gfx_windowFadeInFromBlack(u16 targets)
{
    s32 level;

    REG_WININ = 0;
    REG_WINOUT = 0x1f;
    REG_WIN0H = 0x00f0;
    REG_WIN0V = 0x0020;
    REG_WIN1H = 0x00f0;
    REG_WIN1V = 0x80a0;
    REG_DISPCNT |= GFX_DISPCNT_WIN0 | GFX_DISPCNT_WIN1;
    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    for (level = GFX_FADE_LEVELS; level >= 0; level--)
    {
        REG_BLDY = level;
        gfx_tickPaletteAnims();
        system_waitVBlank();
        system_waitVBlank();
    }
}
