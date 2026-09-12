#pragma once

#include "types.h"

/**
 * SceneState.args scratch-slot roles for the options menu (scene 0x06).
 * args[1] is the menu cursor word, shared with the other menu scenes.
 */
enum OptionsArg
{
    SCENE_ARG_OPTIONS_CURSOR_INDEX = 1,
};

/** g_SceneCurrent.args accessor for the slot above. */
#define SCENE_ARG_OPTIONS_CURSOR g_SceneCurrent.args[SCENE_ARG_OPTIONS_CURSOR_INDEX]

// Scene 0x06: options (volumes, toggles, difficulty, language select, credits).
void scene_optionsMenu_enter(void);
void scene_optionsMenu_tick(void);
void scene_optionsMenu_exit(void);
void scene_optionsMenu_moveCursor(void);
void scene_optionsMenu_adjustValue(void);
void scene_optionsMenu_onConfirm(void);
void scene_optionsMenu_drawRow(s32 row, s32 cursor);
