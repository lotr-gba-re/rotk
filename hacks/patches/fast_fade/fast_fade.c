// Speed up the shared blocking screen transitions so flipping between scenes stops feeling
// slow. Every stock fade walks BLDY (or MOSAIC) one level per two vblanks; the replacements
// below interpolate the same ramp over a chosen number of steps and drop the second vblank.
//
// Every ramp also drops its first stock step, which reproduces the state the screen is
// already in: no blend at all for a fade-out, full black for a fade-in. Only the far end of
// a ramp has to be reached, and rampAt lands on it exactly for any step count.
//
// Shortening a fade is safe because the game is effectively paused while it runs. The
// while-running pair is the exception: it ticks the game once per frame, so its length is
// simulation time and FAST_FADE_RUNNING_FRAMES trades speed and smoothness together.
//
// TODO: transitions still at stock speed:
//      - The cutscene page crossfade, ~33 frames : states 5/6 of scene_cutscene_tick (0x08045a34)
//      - The mission-script window wipes, ~80 frames: indices 2-11 of the transition tables
//        (in 0x0806e118, out 0x0806e154), dispatched through 0x0804e094. Unnamed.
//      - Index 14 of those tables, the co-op pair: it syncs over the link, so both consoles
//        would have to run the same frame count.

#include "actor.h"
#include "game.h"
#include "gba_io.h"
#include "gfx.h"
#include "sprite.h"
#include "system.h"
#include "variables.h"

/**
 * Steps every blocking BLDY ramp is interpolated over. A step is one vblank, so this is the
 * transition's duration in frames; more is smoother and slower. Stock walks all 17 BLDY
 * levels and holds each for two vblanks, the coarser way to spend the same time: 34 frames,
 * or 32 for gfx_fadeOutToBlack and gfx_windowFadeOutToBlack, which stop one level short.
 *
 * Make this quick as it's the common case for entering the backpack, skills, shops, ...
 */
#define FAST_FADE_STEPS 6

/**
 * Steps the mosaic ramps are interpolated over. The pixelation is a coarser effect than a
 * plain BLDY fade and reads as rushed at the same length, so it gets its own count. Stock
 * walks all 16 MOSAIC stretches and holds each for two vblanks: 32 frames.
 *
 * Use a more "cinematic" duration as this effect is used much less frequently.
 */
#define FAST_FADE_MOSAIC_STEPS 15

/**
 * Frames the while-running ramps take. They tick the game once per frame to hold the
 * simulation at 1x, so their duration cannot be traded against smoothness the way
 * FAST_FADE_STEPS can: fewer frames here is a faster fade AND a coarser one. Stock takes 34
 * frames, or 32 for the fade-out, which stops one level short of black.
 *
 * Use a more "cinematic" duration as this effect is used much less frequently.
 */
#define FAST_FADE_RUNNING_FRAMES 15

/**
 * Divisor on the alpha ramps' per-step hold; 1 keeps the stock hold. The caller owns both the
 * step count and the hold there, and the one call in the ROM asks for 6 steps of 4 frames, so
 * stock is 28 frames.
 */
#define FAST_FADE_HOLD_DIVISOR 4

_Static_assert(FAST_FADE_STEPS >= 1, "FAST_FADE_STEPS must be at least 1");
_Static_assert(FAST_FADE_MOSAIC_STEPS >= 1, "FAST_FADE_MOSAIC_STEPS must be at least 1");
_Static_assert(FAST_FADE_RUNNING_FRAMES >= 1, "FAST_FADE_RUNNING_FRAMES must be at least 1");
_Static_assert(FAST_FADE_HOLD_DIVISOR >= 1, "FAST_FADE_HOLD_DIVISOR must be at least 1");

/**
 * Value of a ramp at `step` of `steps`. Step 0 gives 0 and step `steps` gives max, whatever
 * the step count, so a ramp always lands exactly on both ends.
 */
static u16 rampAt(s32 step, s32 steps, s32 max)
{
    return (u32)step * max / steps;
}

/**
 * REPLACES (whole function): gfx_fadeInFromBlack @0x0803e148.
 */
void fastFade_fadeInFromBlack(u16 targets)
{
    s32 step; // CHANGE (ramp): stock's counter is the BLDY level itself

    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    // CHANGE (ramp): stock walks BLDY down one level per step, starting from full black.
    for (step = FAST_FADE_STEPS - 1; step >= 0; step--)
    {
        REG_BLDY = rampAt(step, FAST_FADE_STEPS, GFX_FADE_LEVELS);
        system_waitVBlank();

        // CHANGE (vblanks): stock waits a second time here.
        // system_waitVBlank();
    }
}

/**
 * REPLACES (whole function): gfx_fadeOutToBlack @0x0803e374.
 */
void fastFade_fadeOutToBlack(u16 targets)
{
    s32 step; // CHANGE (ramp): stock's counter is the BLDY level itself

    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    // CHANGE (ramp): stock walks BLDY up one level per step, starting from no blend, and
    // stops one step short of full black; the interpolated ramp ends on GFX_FADE_LEVELS.
    for (step = 1; step <= FAST_FADE_STEPS; step++)
    {
        REG_BLDY = rampAt(step, FAST_FADE_STEPS, GFX_FADE_LEVELS);
        system_waitVBlank();

        // CHANGE (vblanks): stock waits a second time here.
        // system_waitVBlank();
    }
}

/**
 * REPLACES (whole function): gfx_fadeInFromWhite @0x0803e1c8.
 */
void fastFade_fadeInFromWhite(u16 targets)
{
    s32 step; // CHANGE (ramp): stock's counter is the BLDY level itself

    REG_BLDCNT = targets | GFX_BLEND_BRIGHTEN;
    // CHANGE (ramp): stock walks BLDY down one level per step, starting from full black.
    for (step = FAST_FADE_STEPS - 1; step >= 0; step--)
    {
        REG_BLDY = rampAt(step, FAST_FADE_STEPS, GFX_FADE_LEVELS);
        gfx_tickPaletteAnims();
        system_waitVBlank();

        // CHANGE (vblanks): stock waits a second time here.
        // system_waitVBlank();
    }
}

/**
 * REPLACES (whole function): gfx_fadeOutToWhite @0x0803e3fc.
 */
void fastFade_fadeOutToWhite(u16 targets)
{
    s32 step; // CHANGE (ramp): stock's counter is the BLDY level itself

    REG_BLDCNT = targets | GFX_BLEND_BRIGHTEN;
    // CHANGE (ramp): stock walks BLDY up one level per step, starting from no blend.
    for (step = 1; step <= FAST_FADE_STEPS; step++)
    {
        REG_BLDY = rampAt(step, FAST_FADE_STEPS, GFX_FADE_LEVELS);
        gfx_tickPaletteAnims();
        system_waitVBlank();

        // CHANGE (vblanks): stock waits a second time here.
        // system_waitVBlank();
    }
}

/**
 * REPLACES (whole function): gfx_windowFadeInFromBlack @0x0803e2fc.
 */
void fastFade_windowFadeInFromBlack(u16 targets)
{
    s32 step; // CHANGE (ramp): stock's counter is the BLDY level itself

    REG_WININ = 0;
    REG_WINOUT = 0x1f;
    REG_WIN0H = 0x00f0;
    REG_WIN0V = 0x0020;
    REG_WIN1H = 0x00f0;
    REG_WIN1V = 0x80a0;
    REG_DISPCNT |= GFX_DISPCNT_WIN0 | GFX_DISPCNT_WIN1;
    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    // CHANGE (ramp): stock walks BLDY down one level per step, starting from full black.
    for (step = FAST_FADE_STEPS - 1; step >= 0; step--)
    {
        REG_BLDY = rampAt(step, FAST_FADE_STEPS, GFX_FADE_LEVELS);
        gfx_tickPaletteAnims();
        system_waitVBlank();

        // CHANGE (vblanks): stock waits a second time here.
        // system_waitVBlank();
    }
}

/**
 * REPLACES (whole function): gfx_windowFadeOutToBlack @0x0803e640.
 */
void fastFade_windowFadeOutToBlack(u16 targets)
{
    s32 step; // CHANGE (ramp): stock's counter is the BLDY level itself

    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    // CHANGE (ramp): stock walks BLDY up one level per step, starting from no blend, and
    // stops one step short of full black; the interpolated ramp ends on GFX_FADE_LEVELS.
    for (step = 1; step <= FAST_FADE_STEPS; step++)
    {
        REG_BLDY = rampAt(step, FAST_FADE_STEPS, GFX_FADE_LEVELS);
        gfx_tickPaletteAnims();
        system_waitVBlank();

        // CHANGE (vblanks): stock waits a second time here.
        // system_waitVBlank();
    }
    REG_DISPCNT &= ~(GFX_DISPCNT_WIN0 | GFX_DISPCNT_WIN1);
}

/**
 * REPLACES (whole function): gfx_fadeInFromBlackWhileRunning @0x0803e178.
 */
void fastFade_fadeInFromBlackWhileRunning(u16 targets)
{
    s32 frame; // CHANGE (ramp): stock's counter is the BLDY level itself

    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    // CHANGE (ramp): stock holds each BLDY level for two ticked frames. Ticking once per
    // iteration holds the simulation at 1x just the same and lets BLDY move every frame, so
    // the ramp is interpolated over FAST_FADE_RUNNING_FRAMES frames instead.
    for (frame = FAST_FADE_RUNNING_FRAMES - 1; frame >= 0; frame--)
    {
        REG_BLDY = rampAt(frame, FAST_FADE_RUNNING_FRAMES, GFX_FADE_LEVELS);
        actor_tickAll();
        gfx_tickPaletteAnims();
        gfx_tickBgTransforms();
        sprite_endFrame();
        system_waitVBlank();
    }
}

/**
 * REPLACES (whole function): gfx_fadeOutToBlackWhileRunning @0x0803e3a8.
 */
void fastFade_fadeOutToBlackWhileRunning(u16 targets)
{
    s32 frame; // CHANGE (ramp): stock's counter is the BLDY level itself

    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    // CHANGE (ramp): stock holds each BLDY level for two ticked frames and stops one step
    // short of full black. Ticking once per iteration holds the simulation at 1x just the
    // same and lets BLDY move every frame, so the ramp is interpolated over
    // FAST_FADE_RUNNING_FRAMES frames instead and ends on GFX_FADE_LEVELS.
    for (frame = 1; frame <= FAST_FADE_RUNNING_FRAMES; frame++)
    {
        REG_BLDY = rampAt(frame, FAST_FADE_RUNNING_FRAMES, GFX_FADE_LEVELS);
        actor_tickAll();
        gfx_tickPaletteAnims();
        gfx_tickBgTransforms();
        sprite_endFrame();
        system_waitVBlank();
    }
}

/**
 * REPLACES (whole function): gfx_mosaicInFromBlack @0x0803d264.
 * The volatile on the DISPCNT snapshot in the matched decompilation is a byte-matching
 * artifact and is dropped here.
 */
void fastFade_mosaicInFromBlack(u16 targets)
{
    u16 layers;
    s32 step; // CHANGE (ramp): stock's counter is h itself
    s16 h;
    s16 v;
    Actor *actor;

    g_GameFlags.p |= GAME_FLAG_11;
    layers = REG_DISPCNT & ~GFX_DISPCNT_MODE_MASK;
    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    if (layers & GFX_DISPCNT_BG0)
    {
        REG_BG0CNT |= GFX_BGCNT_MOSAIC;
    }
    if (layers & GFX_DISPCNT_BG1)
    {
        REG_BG1CNT |= GFX_BGCNT_MOSAIC;
    }
    if (layers & GFX_DISPCNT_BG2)
    {
        REG_BG2CNT |= GFX_BGCNT_MOSAIC;
    }
    if (layers & GFX_DISPCNT_BG3)
    {
        REG_BG3CNT |= GFX_BGCNT_MOSAIC;
    }
    if (layers & GFX_DISPCNT_OBJ)
    {
        for (actor = g_ActorListMain.head; actor != NULL; actor = actor->next)
        {
            actor->oam.mosaic = 1;
        }
    }
    // CHANGE START (ramp): stock walks h and v down from GFX_MOSAIC_MAX one level per
    // step and writes h to BLDY as well, so the screen starts at GFX_MOSAIC_MAX of
    // GFX_FADE_LEVELS rather than full black. Both are interpolated over
    // FAST_FADE_MOSAIC_STEPS steps instead, each against its own maximum, so BLDY starts
    // fully black and both land on 0; the in-body h/v decrements go with it.
    for (step = FAST_FADE_MOSAIC_STEPS - 1; step >= 0; step--)
    {
        h = rampAt(step, FAST_FADE_MOSAIC_STEPS, GFX_MOSAIC_MAX);
        v = h;
        // CHANGE END
        REG_MOSAIC = (v << 12) | (h << 8) | (v << 4) | h;
        // CHANGE (ramp): stock writes h here.
        REG_BLDY = rampAt(step, FAST_FADE_MOSAIC_STEPS, GFX_FADE_LEVELS);
        if (actor_tickAll())
        {
            sprite_endFrame();
        }
        system_waitVBlank();

        // CHANGE (vblanks): stock waits a second time here.
        // system_waitVBlank();
    }
    if (layers & GFX_DISPCNT_OBJ)
    {
        for (actor = g_ActorListMain.head; actor != NULL; actor = actor->next)
        {
            actor->oam.mosaic = 0;
        }
    }
    g_GameFlags.p &= ~GAME_FLAG_11;
}

/**
 * REPLACES (whole function): gfx_mosaicOutToBlack @0x0803d3cc.
 * The volatile on the DISPCNT snapshot in the matched decompilation is a byte-matching
 * artifact and is dropped here.
 */
void fastFade_mosaicOutToBlack(u16 targets)
{
    u16 layers;
    s32 step; // CHANGE (ramp): stock's counter is h itself
    u8 h;
    u8 v;
    Actor *actor;

    g_GameFlags.p |= GAME_FLAG_11;
    layers = REG_DISPCNT & ~GFX_DISPCNT_MODE_MASK;
    REG_BLDCNT = targets | GFX_BLEND_DARKEN;
    if (layers & GFX_DISPCNT_BG0)
    {
        REG_BG0CNT |= GFX_BGCNT_MOSAIC;
    }
    if (layers & GFX_DISPCNT_BG1)
    {
        REG_BG1CNT |= GFX_BGCNT_MOSAIC;
    }
    if (layers & GFX_DISPCNT_BG2)
    {
        REG_BG2CNT |= GFX_BGCNT_MOSAIC;
    }
    if (layers & GFX_DISPCNT_BG3)
    {
        REG_BG3CNT |= GFX_BGCNT_MOSAIC;
    }
    if (layers & GFX_DISPCNT_OBJ)
    {
        for (actor = g_ActorListMain.head; actor != NULL; actor = actor->next)
        {
            actor->oam.mosaic = 1;
        }
    }
    // CHANGE START (ramp): stock walks h and v up to GFX_MOSAIC_MAX one level per step
    // and writes h to BLDY as well, so the screen never gets darker than GFX_MOSAIC_MAX of
    // GFX_FADE_LEVELS. Both are interpolated over FAST_FADE_MOSAIC_STEPS steps instead,
    // each against its own maximum, so the stretch lands on GFX_MOSAIC_MAX and BLDY on
    // full black; the in-body h/v increments go with it.
    for (step = 1; step <= FAST_FADE_MOSAIC_STEPS; step++)
    {
        h = rampAt(step, FAST_FADE_MOSAIC_STEPS, GFX_MOSAIC_MAX);
        v = h;
        // CHANGE END
        REG_MOSAIC = (v << 12) | (h << 8) | (v << 4) | h;
        // CHANGE (ramp): stock writes h here.
        REG_BLDY = rampAt(step, FAST_FADE_MOSAIC_STEPS, GFX_FADE_LEVELS);
        if (actor_tickAll())
        {
            sprite_endFrame();
        }
        system_waitVBlank();

        // CHANGE (vblanks): stock waits a second time here.
        // system_waitVBlank();
    }
    if (layers & GFX_DISPCNT_OBJ)
    {
        for (actor = g_ActorListMain.head; actor != NULL; actor = actor->next)
        {
            actor->oam.mosaic = 0;
        }
    }
    g_GameFlags.p &= ~GAME_FLAG_11;
}

/**
 * REPLACES (whole function): gfx_alphaBlendIn @0x0803df60.
 */
void fastFade_alphaBlendIn(u16 targets, u16 secondTargets, s32 steps, s32 framesPerStep)
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
        // CHANGE (hold): the caller picks the step count here, so that stays and the
        // per-step hold shrinks instead, rounding up so a one-frame hold survives.
        for (frame = (framesPerStep + FAST_FADE_HOLD_DIVISOR - 1) / FAST_FADE_HOLD_DIVISOR;
             frame > 0; frame--)
        {
            system_waitVBlank();
        }
    }
}

/**
 * REPLACES (whole function): gfx_alphaBlendOut @0x0803dfdc.
 */
void fastFade_alphaBlendOut(u16 targets, u16 secondTargets, s32 steps, s32 framesPerStep)
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
        // CHANGE (hold): see fastFade_alphaBlendIn.
        for (frame = (framesPerStep + FAST_FADE_HOLD_DIVISOR - 1) / FAST_FADE_HOLD_DIVISOR;
             frame > 0; frame--)
        {
            system_waitVBlank();
        }
    }
}
