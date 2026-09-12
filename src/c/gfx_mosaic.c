#include "actor.h"
#include "game.h"
#include "gba_io.h"
#include "gfx.h"
#include "sprite.h"
#include "system.h"
#include "variables.h"

/**
 * Fade in from black through BLDY while ramping the MOSAIC stretch back down on every
 * DISPCNT-enabled BG and every main-list sprite; keeps the actor tick running.
 *
 * @romaddress 0x0803d264
 */
void gfx_mosaicInFromBlack(u16 targets)
{
    // volatile: the ROM reads the snapshot back from its frame slot at each use
    volatile u16 layers;
    s16 h;
    s16 v;
    Actor *actor;

    h = GFX_MOSAIC_MAX;
    v = GFX_MOSAIC_MAX;
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
    while (h >= 0)
    {
        REG_MOSAIC = (v << 12) | (h << 8) | (v << 4) | h;
        REG_BLDY = h;
        h--;
        v--;
        if (actor_tickAll())
        {
            sprite_endFrame();
        }
        system_waitVBlank();
        system_waitVBlank();
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
 * Fade to black through BLDY while stepping the MOSAIC register up on every DISPCNT-enabled
 * BG and every main-list sprite; keeps the actor tick running.
 *
 * @romaddress 0x0803d3cc
 */
void gfx_mosaicOutToBlack(u16 targets)
{
    // volatile: the ROM reads the snapshot back from its frame slot at each use
    volatile u16 layers;
    u8 h;
    u8 v;
    Actor *actor;

    h = 0;
    v = 0;
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
    while (h <= GFX_MOSAIC_MAX)
    {
        REG_MOSAIC = (v << 12) | (h << 8) | (v << 4) | h;
        REG_BLDY = h;
        h++;
        v++;
        if (actor_tickAll())
        {
            sprite_endFrame();
        }
        system_waitVBlank();
        system_waitVBlank();
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
