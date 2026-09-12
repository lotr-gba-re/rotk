#pragma once

#include "types.h"

/**
 * SceneState.args scratch-slot roles for language select (scene 0x12).
 * args[1] is the menu cursor word, shared with the other menu scenes, and doubles as the
 * selected language.
 */
enum LanguageSelectArg
{
    SCENE_ARG_LANGUAGE_SELECT_CURSOR_INDEX = 1,
};

/** g_SceneCurrent.args accessor for the slot above. */
#define SCENE_ARG_LANGUAGE_SELECT_CURSOR g_SceneCurrent.args[SCENE_ARG_LANGUAGE_SELECT_CURSOR_INDEX]

// Scene 0x12: language select.
void scene_languageSelect_enter(void);
void scene_languageSelect_tick(void);
void scene_languageSelect_exit(void);
void scene_languageSelect_onConfirm(void);
void scene_languageSelect_onBack(void);
void scene_languageSelect_moveCursor(void);
void scene_languageSelect_drawEntry(s32 entryIndex);
void scene_languageSelect_redrawEntry(s32 entryIndex);
