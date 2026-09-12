#pragma once

#include "types.h"

/**
 * SceneState.args scratch-slot roles for the main menu (scene 0x05).
 * args[1] (cursor) and args[3] (dialog state) are shared with the other menu scenes.
 */
enum MainMenuArg
{
    SCENE_ARG_MAIN_MENU_CURSOR_INDEX = 1,
    SCENE_ARG_MAIN_MENU_STATE_INDEX = 3,
    SCENE_ARG_MAIN_MENU_REDRAW_DELAY_INDEX = 4,
};

// g_SceneCurrent.args accessors for the slots above.
#define SCENE_ARG_MAIN_MENU_CURSOR g_SceneCurrent.args[SCENE_ARG_MAIN_MENU_CURSOR_INDEX]
#define SCENE_ARG_MAIN_MENU_STATE g_SceneCurrent.args[SCENE_ARG_MAIN_MENU_STATE_INDEX]
#define SCENE_ARG_MAIN_MENU_REDRAW_DELAY g_SceneCurrent.args[SCENE_ARG_MAIN_MENU_REDRAW_DELAY_INDEX]

/** SCENE_ARG_MAIN_MENU_STATE values. */
enum MainMenuState
{
    MAIN_MENU_STATE_PRESS_START = 0,
    MAIN_MENU_STATE_REDRAW_DELAY = 1, // countdown frames before drawing the entries
    MAIN_MENU_STATE_IDLE = 2,
    MAIN_MENU_STATE_LINK_WAIT = 3, // waiting for the link partner
    MAIN_MENU_STATE_LINK_HANDSHAKE = 4,
    MAIN_MENU_STATE_LINK_LIVE = 5, // session up; waiting for MP_MSG_COOP_SAVE_MENU
};

/** SCENE_ARG_MAIN_MENU_CURSOR rows (entry texts at TEXT_ID_MAIN_MENU_ENTRY_BASE + row). */
enum MainMenuEntry
{
    MAIN_MENU_ENTRY_NEW_GAME = 0,
    MAIN_MENU_ENTRY_LOAD_GAME = 1,
    MAIN_MENU_ENTRY_MULTIPLAYER = 2,
    MAIN_MENU_ENTRY_OPTIONS = 3,
    MAIN_MENU_ENTRY_GCN_LINK = 4,
    MAIN_MENU_ENTRY_BONUS_MAPS = 5,
};
#define MAIN_MENU_ENTRY_COUNT 6

// Scene 0x05: main menu.
void scene_mainMenu_enter(void);
void scene_mainMenu_tick(void);
void scene_mainMenu_exit(void);
void scene_mainMenu_onConfirm(void);
void scene_mainMenu_moveCursor(void);
void scene_mainMenu_drawEntries(void);
void scene_mainMenu_drawEntry(s32 entryIndex, s32 selectedIndex);
void scene_mainMenu_clearPrompt(void);
void scene_mainMenu_showLinkPrompt(void);
