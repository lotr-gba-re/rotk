#include "game.h"
#include "fx.h"
#include "gfx.h"
#include "input.h"
#include "mission.h"
#include "multiplayer.h"
#include "player.h"
#include "save.h"
#include "scene.h"
#include "scene/gameplay.h"
#include "system.h"
#include "variables.h"

/**
 * Advance the whole game state by one frame: multiplayer tick, then the pending scene
 * change if one was requested (exit handler, transition for a co-op full change, state
 * swap to the staged next scene, enter handler), then input, the current scene's tick
 * handler, and the gfx ticks.
 *
 * @romaddress 0x08033000
 */
void game_tickFrame(void)
{
    multiplayer_tick();
    g_FrameCounter += 1;
    if (game_isSceneChangePending())
    {
        g_GameFlags.p |= GAME_FLAG_SCENE_CHANGE;
        if (!(g_GameFlags.p & GAME_FLAG_COOP_ACTIVE))
        {
            game_dispatchSceneExit();
        }
        else if ((g_GameFlags.p & GAME_FLAG_31) && g_SceneCurrent.id != SCENE_ID_GAMEPLAY)
        {
            game_dispatchSceneExit();
            g_GameFlags.p = (g_GameFlags.p & ~(GAME_FLAG_20 | GAME_FLAG_SCENE_CHANGE)) |
                            GAME_FLAG_SKIP_TRANSITION_FADE;
            scene_gameplay_exit();
            g_GameFlags.p &= ~GAME_FLAG_SKIP_TRANSITION_FADE;
        }
        else
        {
            game_dispatchSceneExit();
        }
        g_SceneNext.id &= ~SCENE_NEXT_PENDING;
        g_GameFlags.p &= ~GAME_FLAG_31;
        g_ScenePrevious = g_SceneCurrent;
        g_SceneCurrent = g_SceneNext;
        game_dispatchSceneEnter();
        g_GameFlags.p &= ~GAME_FLAG_SCENE_CHANGE;
    }
    g_EngineState->fpsFrameCounter += 1;
    input_update();
    game_dispatchSceneTick();
    gfx_tickBgTransforms();
    gfx_tickPaletteAnims();
}

/**
 * Initialize the game state at boot: clear the current scene, snapshot it into the next
 * and previous slots, reset the mission state, then request the first scene - Language
 * Select on a fresh save, the Copyright logo chain otherwise.
 *
 * @romaddress 0x080330e8
 */
void game_initBoot(void)
{
    g_SceneCurrent.id = SCENE_ID_NO_OP_0;
    g_SceneNext = g_SceneCurrent;
    g_ScenePrevious = g_SceneCurrent;
    scene_gameplay_resetMissionState();
    if (g_SaveGlobals.d.languageSet == 0)
    {
        game_requestSceneChange(SCENE_ID_LANGUAGE_SELECT);
    }
    else
    {
        game_requestSceneChange(SCENE_ID_COPYRIGHT);
    }
}

/**
 * The scene currently running.
 *
 * @romaddress 0x08033138
 */
SceneId game_getCurrentSceneId(void)
{
    return g_SceneCurrent.id;
}

/**
 * The staged next scene's id, with the pending bit stripped so it compares against a
 * plain SceneId whether or not a change is currently pending. The overlay-menu exit
 * handlers read it to tell an overlay close from a full teardown.
 *
 * @romaddress 0x08033144
 */
SceneId game_getNextSceneId(void)
{
    return g_SceneNext.id & ~SCENE_NEXT_PENDING;
}

/**
 * Request a scene change with no handoff: stage the next scene's id, clear its args, and
 * mark the change pending; game_tickFrame applies it at the next frame.
 *
 * @romaddress 0x08033154
 */
void game_requestSceneChange(SceneId sceneId)
{
    g_SceneNext.id = sceneId | SCENE_NEXT_PENDING;
    g_SceneNext.args[0] = 0;
    g_SceneNext.args[1] = 0;
    g_SceneNext.args[2] = 0;
    g_SceneNext.args[3] = 0;
    g_SceneNext.args[4] = 0;
}

/**
 * Request a scene change with an arg handoff: stage the next scene's id and args[0]/[1]
 * and mark the change pending; game_tickFrame applies it at the next frame. The
 * remaining next-scene args are cleared.
 *
 * @romaddress 0x08033170
 */
void game_requestSceneChangeWithArgs(SceneId sceneId, u32 arg0, u32 arg1)
{
    g_SceneNext.id = sceneId | SCENE_NEXT_PENDING;
    g_SceneNext.args[0] = arg0;
    g_SceneNext.args[1] = arg1;
    g_SceneNext.args[2] = 0;
    g_SceneNext.args[3] = 0;
    g_SceneNext.args[4] = 0;
}

/**
 * Advance the FX emitters and every active player's HUD by one frame: a partial
 * game_tickFrame for when gameplay is suspended (the overlay-menu scene updates, the co-op
 * fade-out, the EEPROM read wait).
 *
 * @romaddress 0x08033190
 */
void game_tickFxAndHuds(void)
{
    u8 playerIndex;

    fx_tickEmitters();
    for (playerIndex = 0; playerIndex < g_PlayerGlobals.activePlayerCount; playerIndex++)
    {
        player_tickHud(playerIndex);
    }
}

/**
 * Whether a scene change is pending: the staged next scene's id (SCENE_NEXT_PENDING set
 * by game_requestSceneChange/game_requestSceneChangeWithArgs) differs from the current one.
 *
 * @romaddress 0x080331c4
 */
bool game_isSceneChangePending(void)
{
    if (g_SceneCurrent.id != g_SceneNext.id)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
 * Run the current scene's enter handler (NULL-checked). When the previous scene was the
 * in-game dialog (0x11), the dialog interrupted a mission script, so the paused spawn
 * group/stream is restarted instead of running the scene init.
 *
 * @romaddress 0x080331e8
 */
void game_dispatchSceneEnter(void)
{
    if (SceneHandlers[g_SceneCurrent.id].enter != NULL)
    {
        if (g_ScenePrevious.id != SCENE_ID_IN_GAME_DIALOG)
        {
            SceneHandlers[g_SceneCurrent.id].enter();
        }
        else
        {
            mission_script_spawnGroupAndStartStream(g_InGameDialogState.resumeGroup,
                                                    g_InGameDialogState.resumeStream);
        }
    }
}

/**
 * Run the current scene's tick handler (NULL-checked).
 *
 * @romaddress 0x0803322c
 */
void game_dispatchSceneTick(void)
{
    if (SceneHandlers[g_SceneCurrent.id].tick != NULL)
    {
        SceneHandlers[g_SceneCurrent.id].tick();
    }
}

/**
 * Run the current scene's exit handler (NULL-checked), unless the in-game dialog is
 * active (GAME_FLAG_IN_GAME_DIALOG): the dialog overlays the scene, which stays alive.
 *
 * @romaddress 0x08033254
 */
void game_dispatchSceneExit(void)
{
    if (SceneHandlers[g_SceneCurrent.id].exit != NULL &&
        (g_GameFlags.p & GAME_FLAG_IN_GAME_DIALOG) == 0)
    {
        SceneHandlers[g_SceneCurrent.id].exit();
    }
}

/**
 * Do-nothing scene handler: the shared enter/tick/exit stub for scene 0
 * (all three SceneHandlers[0] slots point here).
 *
 * @romaddress 0x0803328c
 */
void scene_noOp(void)
{
}
