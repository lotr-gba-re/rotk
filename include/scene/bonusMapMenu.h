#pragma once

#include "types.h"

/**
 * SceneState.args scratch-slot roles for the bonus map select (scene 0x18).
 * args[0] reuses the game_requestSceneChangeWithArgs param word as the difficulty cursor.
 */
enum BonusMapArg
{
    SCENE_ARG_BONUS_MAP_DIFFICULTY_INDEX = 0,
    SCENE_ARG_BONUS_MAP_CURSOR_INDEX = 1,
    SCENE_ARG_BONUS_MAP_SUB_ENTRY_INDEX = 2,
    SCENE_ARG_BONUS_MAP_STATE_INDEX = 3,
};

// g_SceneCurrent.args accessors for the slots above.
#define SCENE_ARG_BONUS_MAP_DIFFICULTY g_SceneCurrent.args[SCENE_ARG_BONUS_MAP_DIFFICULTY_INDEX]
#define SCENE_ARG_BONUS_MAP_CURSOR g_SceneCurrent.args[SCENE_ARG_BONUS_MAP_CURSOR_INDEX]
#define SCENE_ARG_BONUS_MAP_SUB_ENTRY g_SceneCurrent.args[SCENE_ARG_BONUS_MAP_SUB_ENTRY_INDEX]
#define SCENE_ARG_BONUS_MAP_STATE g_SceneCurrent.args[SCENE_ARG_BONUS_MAP_STATE_INDEX]

/** SCENE_ARG_BONUS_MAP_STATE values: which side-panel selector is active. */
enum BonusMapState
{
    BONUS_MAP_STATE_MISSION_SELECT = 0,
    BONUS_MAP_STATE_SUB_ENTRY_SELECT = 1,
    BONUS_MAP_STATE_DIFFICULTY_SELECT = 2,
    BONUS_MAP_STATE_LAUNCH_MISSION = 3, // difficulty chosen; Update launches the mission
};

/** BonusMapMenuEntries slots: the selectable maps, then the locked-display template. */
enum BonusMap
{
    BONUS_MAP_MORIA = 0,
    BONUS_MAP_WEATHERTOP = 1,
    BONUS_MAP_HELMS_DEEP = 2,
    BONUS_MAP_FANGORN = 3,
    BONUS_MAP_LOCKED = 4,
};
/** Selectable bonus maps. */
#define BONUS_MAP_COUNT 4

// Side-panel text: shared pixel x of the column, and the font_setStyle colors for a
// normal vs cursor-selected row.
#define BONUS_MAP_PANEL_TEXT_X 200
#define BONUS_MAP_TEXT_COLOR_NORMAL 0xc
#define BONUS_MAP_TEXT_COLOR_SELECTED 0x0

// Scene 0x18: bonus map select ("CHOOSE YOUR PATH"; entered from SaveLoad mode 4 only).
void scene_bonusMapMenu_enter(void);
void scene_bonusMapMenu_tick(void);
void scene_bonusMapMenu_exit(void);
void scene_bonusMapMenu_onConfirm(void);
void scene_bonusMapMenu_moveCursor(void);
void scene_bonusMapMenu_onBack(void);
void scene_bonusMapMenu_drawEntryLabel(void);
void scene_bonusMapMenu_drawSubEntry(u8 rowIndex, u8 selectedIndex);
void scene_bonusMapMenu_drawDifficultyRow(u8 rowIndex, u8 selectedIndex);
u32 scene_bonusMapMenu_subEntryCount(void);
void scene_bonusMapMenu_setupCursor(void);
void scene_bonusMapMenu_setupSprites(void);
