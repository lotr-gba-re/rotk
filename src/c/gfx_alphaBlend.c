#include "gba_io.h"
#include "gfx.h"
#include "system.h"
#include "variables.h"

/**
 * Ramp the alpha blend from all-second-target to a step/16 mix of the first targets, holding
 * each step for framesPerStep vblanks.
 *
 * @param steps last step of the ramp (0-16); the blend ends at steps/16 first target
 *
 * @romaddress 0x0803df60
 */
void gfx_alphaBlendIn(u16 targets, u16 secondTargets, s32 steps, s32 framesPerStep)
{
    s32 step;
    s32 frame;

    REG_BLDCNT = targets | GFX_BLEND_ALPHA | (secondTargets << 8);
    for (step = 0; step <= steps; step++)
    {
        u16 eva = step;
        u16 evb = GFX_FADE_LEVELS - step;

        g_BlendEva.value = eva << 8;
        g_BlendEvb.value = evb << 8;
        REG_BLDALPHA = (evb << 8) | eva;
        for (frame = framesPerStep; frame > 0; frame--)
        {
            system_waitVBlank();
        }
    }
}

/**
 * gfx_alphaBlendIn run backwards, from a steps/16 mix down to all-second-target.
 *
 * @romaddress 0x0803dfdc
 */
void gfx_alphaBlendOut(u16 targets, u16 secondTargets, s32 steps, s32 framesPerStep)
{
    s32 step;
    s32 frame;

    REG_BLDCNT = targets | GFX_BLEND_ALPHA | (secondTargets << 8);
    for (step = steps; step >= 0; step--)
    {
        u16 eva = step;
        u16 evb = GFX_FADE_LEVELS - step;

        g_BlendEva.value = eva << 8;
        g_BlendEvb.value = evb << 8;
        REG_BLDALPHA = (evb << 8) | eva;
        for (frame = framesPerStep; frame > 0; frame--)
        {
            system_waitVBlank();
        }
    }
}
