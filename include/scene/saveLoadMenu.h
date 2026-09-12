#pragma once

#include "types.h"

/**
 * SceneState.args scratch-slot roles for the save/load slot menu (scene 0x09).
 * args[0] is the game_requestSceneChangeWithArgs param word (enum SaveLoadMode).
 * args[4] is read as a byte.
 */
enum SaveLoadArg
{
    SCENE_ARG_SAVE_LOAD_MODE_INDEX = 0,
    SCENE_ARG_SAVE_LOAD_CURSOR_INDEX = 1,
    SCENE_ARG_SAVE_LOAD_SELECTED_SLOT_INDEX = 2,
    SCENE_ARG_SAVE_LOAD_STATE_INDEX = 3,
    SCENE_ARG_SAVE_LOAD_FREE_SLOT_INDEX = 4,
};

/** SCENE_ARG_SAVE_LOAD_MODE values: what the menu was opened for. */
enum SaveLoadMode
{
    SAVE_LOAD_MODE_LOAD = 0,      // load a slot into single-player gameplay
    SAVE_LOAD_MODE_SAVE = 1,      // save (pause menu)
    SAVE_LOAD_MODE_COOP_LOAD = 2, // co-op: pick slots and exchange them over the link
    // save (victory screen); backing out continues to the Griptonite-logo cinematic
    SAVE_LOAD_MODE_SAVE_VICTORY = 3,
    SAVE_LOAD_MODE_BONUS_LOAD = 4, // load a slot for the bonus map menu
};

/** SCENE_ARG_SAVE_LOAD_STATE values. */
enum SaveLoadState
{
    SAVE_LOAD_STATE_SLOT_SELECT = 0,
    SAVE_LOAD_STATE_CONFIRM_OVERWRITE = 1, // Yes/No, cursor in args[1]
    SAVE_LOAD_STATE_CONFIRM_ERASE = 2,     // Yes/No, cursor in args[1]
    SAVE_LOAD_STATE_RESULT = 3,            // action done, a key press returns to slot select
    SAVE_LOAD_STATE_HERO_SELECT = 4,       // create a character in an empty slot
    // co-op handshake: exchange the picked save slots, host seeds the rng, apply the
    // option flags, then hand over to the multiplayer level select
    SAVE_LOAD_STATE_COOP_SLOT_EXCHANGE = 5,
    SAVE_LOAD_STATE_COOP_SEED = 6,
    SAVE_LOAD_STATE_COOP_APPLY_OPTIONS = 7,
    SAVE_LOAD_STATE_COOP_LAUNCH = 9,
};

// g_SceneCurrent.args accessors for the slots above.
#define SCENE_ARG_SAVE_LOAD_MODE g_SceneCurrent.args[SCENE_ARG_SAVE_LOAD_MODE_INDEX]
#define SCENE_ARG_SAVE_LOAD_CURSOR g_SceneCurrent.args[SCENE_ARG_SAVE_LOAD_CURSOR_INDEX]
#define SCENE_ARG_SAVE_LOAD_SELECTED_SLOT                                                          \
    g_SceneCurrent.args[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT_INDEX]
#define SCENE_ARG_SAVE_LOAD_STATE g_SceneCurrent.args[SCENE_ARG_SAVE_LOAD_STATE_INDEX]
#define SCENE_ARG_SAVE_LOAD_FREE_SLOT ((u8)g_SceneCurrent.args[SCENE_ARG_SAVE_LOAD_FREE_SLOT_INDEX])

// Scene 0x09: save/load slots.
void scene_saveLoadMenu_enter(void);
void scene_saveLoadMenu_tick(void);
void scene_saveLoadMenu_exit(void);
void scene_saveLoadMenu_moveCursor(void);
void scene_saveLoadMenu_onConfirm(void);
void scene_saveLoadMenu_onBack(void);
void scene_saveLoadMenu_onErase(void);
void scene_saveLoadMenu_drawActionPanel(void);
void scene_saveLoadMenu_drawYesNo(void);
void scene_saveLoadMenu_drawSlot(u8 slot, u8 selectedSlot);
void scene_saveLoadMenu_refreshSlot(s32 slotIndex);
void scene_saveLoadMenu_setupButtonSprites(void);
void scene_saveLoadMenu_setupSlotSprites(void);
void scene_saveLoadMenu_commitSlot(bool save);
void scene_saveLoadMenu_stampSlot(void);
void scene_saveLoadMenu_createCharacter(u8 playerIndex, u8 heroId);
void scene_saveLoadMenu_loadSlot(u32 playerIndex);
void scene_saveLoadMenu_loadPeerSlot(u32 playerIndex);
