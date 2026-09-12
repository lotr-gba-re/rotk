#include "scene/gameplay.h"
#include "actor.h"
#include "game.h"
#include "gba.h"
#include "gba_io.h"
#include "gfx.h"
#include "mission.h"
#include "multiplayer.h"
#include "player.h"
#include "scene.h"
#include "sfx.h"
#include "sprite.h"
#include "system.h"
#include "variables.h"

/**
 * Run a pending scene change's transition: capture the screen, fade out (unless
 * GAME_FLAG_SKIP_TRANSITION_FADE), tear down the outgoing scene per its args (sfx/sprite
 * flush, music, player heal or checkpoint var clear), and reset the vblank callback and OAM.
 *
 * @romaddress 0x08032098
 */
void scene_gameplay_exit(void)
{
    MpMessageArgs msgArgs;
    u32 nextId;
    u32 sceneArg0;

    msgArgs.words[0] = 0;
    nextId = g_SceneNext.id & ~SCENE_NEXT_PENDING;
    if (nextId != SCENE_ID_MAIN_MENU && nextId != SCENE_ID_CUTSCENE &&
        nextId != SCENE_ID_GRIPTONITE_LOGO && nextId != SCENE_ID_UNUSED_15 &&
        nextId != SCENE_ID_CREDITS && nextId != SCENE_ID_SAVE_LOAD_MENU &&
        nextId != SCENE_ID_GAMEPLAY)
    {
        scene_gameplay_parkWorld();
    }
    if (!(g_GameFlags.p & GAME_FLAG_SKIP_TRANSITION_FADE))
    {
        switch (g_TransitionState.effect)
        {
        case 1:
        case 8:
            gfx_disableBg(1);
            break;
        case 7:
            gfx_setBlendControl(4, 0x10);
            gfx_setBlendAlpha(8, 8);
            break;
        }
        if ((g_GameFlags.p & (GAME_FLAG_REQUEST_OVERLAY | GAME_FLAG_COOP_ACTIVE)) ==
            (GAME_FLAG_REQUEST_OVERLAY | GAME_FLAG_COOP_ACTIVE))
        {
            multiplayer_clearSessionFlag(0);
            gfx_fadeOutToBlackCoop(0x3f);
        }
        else
        {
            gfx_fadeOutToBlack(0x3f);
        }
    }
    if (g_GameFlags.p & GAME_FLAG_REQUEST_OVERLAY)
    {
        g_GameFlags.p |= GAME_FLAG_OVERLAY;
    }
    if (g_GameFlags.p & GAME_FLAG_31)
    {
        g_MpMessageMask &= ~(MP_MSG_FLAG_MENU_OPEN_PLAYER0 | MP_MSG_FLAG_MENU_OPEN_PLAYER1);
    }
    bios_cpuFastSet((const void *)VRAM, g_VramCapture, 0x3000);
    sceneArg0 = g_SceneCurrent.args[0];
    if (sceneArg0 == GAMEPLAY_MODE_START)
    {
        sfx_stopAllOnExit();
        sprite_flushList(&g_ActorListMain);
        if (g_GameFlags.p & GAME_FLAG_COOP_ACTIVE)
        {
            // words[1]/[2] uninitialized: queued as-is (original-game bug)
            msgArgs.words[0] = sceneArg0;
            multiplayer_clearSessionFlag(0);
            multiplayer_queueMessage(MP_MSG_SYNC, msgArgs);
        }
    }
    if (g_SceneCurrent.args[0] - GAMEPLAY_MODE_RESUME < 2)
    {
        sprite_resetTileAllocator();
        if (g_SceneCurrent.args[0] == GAMEPLAY_MODE_3)
        {
            mission_unknown0803915c();
            mission_unknown08039198();
        }
    }
    else
    {
        mission_unknown0803915c();
        mission_unknown08039198();
    }
    if (g_SceneCurrent.args[3] != 0)
    {
        PLAYER(g_SceneCurrent.args[2]).currentHp = PLAYER_STAT(g_SceneCurrent.args[2], STAT_MAX_HP);
        PLAYER(g_SceneCurrent.args[2]).currentSpirit =
            PLAYER_STAT(g_SceneCurrent.args[2], STAT_MAX_SPIRIT);
        PLAYER(g_SceneCurrent.args[2]).corruption = 0;
        if (g_TransitionState.effect == 8)
        {
            scene_unknown0804a48c();
        }
        player_resetHuds();
    }
    else
    {
        if ((g_SceneNext.id & ~SCENE_NEXT_PENDING) == SCENE_ID_GAMEPLAY)
        {
            g_MissionCheckpointVars[0] = 0;
            g_MissionCheckpointVars[1] = 0;
            g_MissionCheckpointVars[2] = 0;
            player_resetHuds();
        }
    }
    gfx_clearPaletteAnims();
    gfx_resetBgTransform(1);
    system_setVblankCallback(system_defaultVblankCallback);
    sprite_endFrame();
    if (!(g_GameFlags.p & GAME_FLAG_COOP_ACTIVE))
    {
        if (sfx_isPlaying(g_UnknownSfxHandle_030035c4))
        {
            sfx_stop(g_UnknownSfxHandle_030035c4);
        }
        if (sfx_isPlaying(g_UnknownSfxHandle_0300416c))
        {
            sfx_stop(g_UnknownSfxHandle_0300416c);
        }
    }
    sprite_resetOam();
    g_GameFlags.p &= ~GAME_FLAG_30;
}

/**
 * Tear down the gameplay world for a transition into a non-gameplay scene: free the
 * transition timer, stop the transition sfx, park the actor list (skipped for overlay
 * scenes), snapshot the OBJ palette, and preselect the light teardown mode
 * (GAMEPLAY_MODE_RESUME) for scene_gameplay_exit. scene_gameplay_enter restores world and palette.
 *
 * @romaddress 0x080322dc
 */
void scene_gameplay_parkWorld(void)
{
    if (g_TransitionTimer != NULL && g_TransitionTimer->behaviorState == 3 &&
        !(g_GameFlags.p & GAME_FLAG_OVERLAY))
    {
        sprite_free(g_TransitionTimer);
    }
    transition_stopSfx();
    if (!(g_GameFlags.p & GAME_FLAG_REQUEST_OVERLAY))
    {
        actor_parkMainList();
        g_GameFlags.p |= GAME_FLAG_WORLD_PARKED;
        if (!(g_GameFlags.p & GAME_FLAG_COOP_ACTIVE))
        {
            u8 i;
            // BUG: dead loop; actor_parkMainList just emptied the list, so the
            // standingFlags clear never runs and loot piles keep their per-player
            // standing flags across a park/resume
            for (i = 0; i < 2; i++)
            {
                Actor *actor;
                for (actor = g_ActorListMain.head; actor != NULL; actor = actor->next)
                {
                    if (actor->recordType == ACTOR_TYPE_LOOT_PILE)
                    {
                        actor->as.lootPile.standingFlags[i] = 0;
                    }
                }
            }
        }
    }
    g_SceneCurrent.args[0] = GAMEPLAY_MODE_RESUME;
    bios_cpuSet((const void *)OBJ_PALETTE_RAM, g_ObjPaletteSnapshot,
                0x100); // halfwords = 512 bytes
}
