#pragma once

#include "types.h"

/**
 * SceneState.args roles for gameplay (scene 0x01): args[0] is the mode word below (the
 * game_requestSceneChangeWithArgs param), args[1] the mission id to load.
 */
enum GameplayArg
{
    SCENE_ARG_GAMEPLAY_MODE_INDEX = 0,
    SCENE_ARG_GAMEPLAY_MISSION_INDEX = 1,
};

// g_SceneCurrent.args accessors for the slots above.
#define SCENE_ARG_GAMEPLAY_MODE g_SceneCurrent.args[SCENE_ARG_GAMEPLAY_MODE_INDEX]
#define SCENE_ARG_GAMEPLAY_MISSION g_SceneCurrent.args[SCENE_ARG_GAMEPLAY_MISSION_INDEX]

/**
 * The gameplay mode word: how scene_gameplay_enter builds the world and how
 * scene_gameplay_exit tears it down. Cutscene-driven transitions pull it from a byte in
 * the cutscene table, so all values can arrive data-driven.
 */
enum GameplayMode
{
    // fresh mission load; on exit, full teardown (sfx stop, actor flush, peer notify)
    GAMEPLAY_MODE_START = 0,
    // resume the parked world (g_VramCapture + parked actor list); set by
    // scene_gameplay_parkWorld for the overlay-menu round trip
    GAMEPLAY_MODE_RESUME = 2,
    // as GAMEPLAY_MODE_RESUME, and the exit also drains the pooled object lists
    GAMEPLAY_MODE_3 = 3,
    GAMEPLAY_MODE_4 = 4, // handled like GAMEPLAY_MODE_START_HEALED
    // fresh load with every player healed to full (menu launches: save load, bonus maps)
    GAMEPLAY_MODE_START_HEALED = 5,
    // GAMEPLAY_MODE_START_HEALED plus the mission variant from args[3] (co-op level select)
    GAMEPLAY_MODE_START_VARIANT = 6,
    GAMEPLAY_MODE_7 = 7, // handled like GAMEPLAY_MODE_START_VARIANT
};

// Scene 0x01: gameplay. The exit slot is scene_gameplay_exit (shared with pending scene
// changes).
void scene_gameplay_enter(void);
void scene_gameplay_tick(void);
void scene_gameplay_exit(void);

void scene_gameplay_parkWorld(void);
/**
 * Quit-path counterpart of scene_gameplay_parkWorld (scene_pauseMenu_exit, next == MAIN_MENU):
 * free the parked actor list, drain the pooled object lists, reset music/transition
 * state and the corruption tiers.
 */
void scene_gameplay_discardParkedWorld(void);
/**
 * Reset the boot-time mission/session state: clear the scene param word, allocate the
 * 0x200-byte state buffer, and mark the current mission empty (SLOT_MISSION_EMPTY).
 * Called once from game_initBoot.
 */
void scene_gameplay_resetMissionState(void);
