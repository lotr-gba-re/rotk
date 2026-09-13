// Owner of the boot entry point: replaces game_initBoot so exactly one patch decides where
// boot goes, and where a patch's once-per-boot work runs (runTasks).

#include "mod_startup.h"

#include "game.h"
#include "mission.h"
#include "save.h"
#include "scene.h"
#include "scene/gameplay.h"
#include "scene/languageSelect.h"
#include "sfx.h"
#include "text.h"
#include "variables.h"

#include "mod_scenes.h"
#include "patches/mod_save/save_state.h"
#if HACK_loot_qol
#include "patches/loot_qol/loot_qol.h"
#endif

/**
 * 1 = boot to the main menu, skipping the copyright/intro logo chain; 0 = the stock chain.
 * A plain build-time switch.
 */
#define MOD_STARTUP_SKIP_INTRO 1

/**
 * Once-per-boot work, run on a save image that is known good. Order between entries
 * carries no meaning.
 */
static void runTasks(void)
{
#if HACK_loot_qol
    lootQol_loadState();
#endif
}

/**
 * The scene after boot is settled: straight to the main menu, or the stock copyright/intro
 * chain (MOD_STARTUP_SKIP_INTRO).
 */
static void requestTitleScene(void)
{
#if MOD_STARTUP_SKIP_INTRO
    game_requestSceneChange(SCENE_ID_MAIN_MENU);
#else
    game_requestSceneChange(SCENE_ID_COPYRIGHT);
#endif
}

void modStartup_resumeBoot(void)
{
    runTasks();

    if (g_SaveGlobals.d.languageSet == 0)
    {
        game_requestSceneChange(SCENE_ID_LANGUAGE_SELECT);
        return;
    }
    requestTitleScene();
}

static void requestFirstScene(void)
{
    if (modSave_isUpToDate())
    {
        modSave_loadState();
        modStartup_resumeBoot();
    }
    else
    {
        // the migration scene loads the migrated save and resumes boot once done
        game_requestSceneChange(MOD_SCENE_ID(SAVE_MIGRATE));
    }
}

/**
 * Boot scene entry: the stock body with its first-scene decision delegated to
 * requestFirstScene.
 *
 * REPLACES (whole function): game_initBoot @0x080330e8.
 */
void modStartup_initBoot(void)
{
    g_SceneCurrent.id = SCENE_ID_NO_OP_0;
    g_SceneNext = g_SceneCurrent;
    g_ScenePrevious = g_SceneCurrent;
    scene_gameplay_resetMissionState();
    requestFirstScene();
}

/**
 * Language chosen: persist and continue booting, or return to the options menu.
 *
 * Stock tells the two apart by previous == 0 (the boot stub); entered from the migration
 * scene it would take the options-menu branch and transition BACK into that scene, skipping
 * the header write. The migration scene is a boot entry, so its id joins the boot test.
 *
 * REPLACES (whole function): scene_languageSelect_onConfirm @0x0802fc30.
 */
void modStartup_languageSelectOnConfirm(void)
{
    text_setLanguage((s32)SCENE_ARG_LANGUAGE_SELECT_CURSOR);
    save_stampLanguage();
    if (g_ScenePrevious.id == SCENE_ID_NO_OP_0 || g_ScenePrevious.id == MOD_SCENE_ID(SAVE_MIGRATE))
    {
        sfx_disableAudio();
        save_writeGlobalHeader();
        sfx_enableAudio();
        requestTitleScene();
    }
    else
    {
        // options-menu language change: back to the caller (stock behavior)
        game_requestSceneChangeWithArgs(g_ScenePrevious.id, 1, g_ScenePrevious.args[1]);
    }
    sfx_play(SFX_MENU_CONFIRM);
}
