#pragma once

#include "types.h"

/**
 * Scene IDs: index into SceneHandlers and the value of g_SceneCurrent.id /
 * g_ScenePrevious.id. 0x15 has a NULL entry.
 */
typedef enum SceneId
{
    SCENE_ID_NO_OP_0 = 0x00, // do-nothing boot stub
    SCENE_ID_GAMEPLAY = 0x01,
    SCENE_ID_COPYRIGHT = 0x02,
    SCENE_ID_INTRO_LOGO = 0x03,
    SCENE_ID_GRIPTONITE_LOGO = 0x04,
    SCENE_ID_MAIN_MENU = 0x05,
    SCENE_ID_OPTIONS_MENU = 0x06,
    SCENE_ID_CUTSCENE = 0x07,
    SCENE_ID_NEW_GAME = 0x08,
    SCENE_ID_SAVE_LOAD_MENU = 0x09,
    SCENE_ID_BACKPACK = 0x0a,
    SCENE_ID_MULTIPLAYER_LEVEL_SELECT = 0x0b,
    SCENE_ID_UNUSED_0C = 0x0c, // bx lr stubs
    SCENE_ID_ARTIFACTS = 0x0d,
    SCENE_ID_CHARACTER_STATS = 0x0e,
    SCENE_ID_SKILL_TREE = 0x0f,
    SCENE_ID_PAUSE_MENU = 0x10,
    SCENE_ID_IN_GAME_DIALOG = 0x11,
    SCENE_ID_LANGUAGE_SELECT = 0x12,
    SCENE_ID_CREDITS = 0x13,
    SCENE_ID_SHOP = 0x14,
    SCENE_ID_UNUSED_15 = 0x15, // NULL entry
    SCENE_ID_UNKNOWN_16 = 0x16,
    SCENE_ID_GCN_LINK_MENU = 0x17,
    SCENE_ID_BONUS_MAP_MENU = 0x18,
    SCENE_ID_VICTORY = 0x19,
} SceneId;

typedef struct SceneState
{
    u32 id;
    // Per-scene scratch words; slot roles are defined per scene in the scene/
    // headers (e.g. scene/mainMenu.h). g_SceneNext's slots are the staged
    // handoff written by game_requestSceneChange/game_requestSceneChangeWithArgs.
    u32 args[5];
} SceneState;

/**
 * bit 7 of g_SceneNext.id: a scene change is pending;
 * game_tickFrame clears it as it applies the change
 */
#define SCENE_NEXT_PENDING (1 << 7)

/**
 * Scene/state handler triple (SceneHandlers @0x0806c828). The third slot is a
 * scene-EXIT handler (runs once on a pending scene change: fade/mosaic teardown),
 * not a per-frame render.
 */
typedef struct SceneEntry
{
    void (*enter)(void);
    void (*tick)(void);
    void (*exit)(void);
} SceneEntry;

/** Do-nothing scene handler: all three SceneHandlers[0] slots. */
void scene_noOp(void);

void scene_clearStateBytes(void);

/** In-game dialog (scene 0x11) state cluster; g_InGameDialogState @0x03003ba0. */
typedef struct InGameDialogState
{
    u8 field_0x0[4];

    /** the dialog being shown (scene_requestInGameDialog's arg; also g_InGameDialogId) */
    u8 dialogId;

    u8 field_0x5;
    u8 field_0x6;

    /** mission-script spawn group the dialog interrupted; restarted on return */
    u8 resumeGroup;

    /** mission-script stream the dialog interrupted; restarted on return */
    u8 resumeStream;
} InGameDialogState;

/** TransitionState.flags bits. */
enum TransitionFlag
{
    TRANSITION_FLAG_SFX_ACTIVE = 1 << 0, // the jingle plays; cleared by transition_stopSfx
};

/**
 * Transition teardown state; the outgoing scene picks the effect, scene_gameplay_exit
 * applies it.
 */
typedef struct TransitionState
{
    u32 field_0x00;
    u32 sfxHandle; // 0x04; the transition jingle
    u8 flags;      // 0x08; enum TransitionFlag; scene_unknown0804a48c clears bits 0/2/3
    u8 field_0x09[4];
    u8 effect;     // 0x0d: teardown effect selector (scene_gameplay_exit's switch)
    u8 field_0x0e; // 0x0e: read by scene_unknown0804a48c
    u8 field_0x0f;
} TransitionState;

/**
 * Request the in-game dialog overlay (SCENE_ID_IN_GAME_DIALOG) showing dialog
 * dialogId (stored to g_InGameDialogId); script op 5's single-player branch.
 */
void scene_requestInGameDialog(u8 dialogId);

/** Stop the transition jingle (g_TransitionState.sfxHandle) and clear its active bit. */
void transition_stopSfx(void);
/**
 * Advance the active transition effect one vblank (step sequencer over the effect
 * parameter table); called by system_vBlankIrqHandler.
 */
void transition_tick(void);
/**
 * Reset g_TransitionState, stop the jingle, and restore the saved music volume; called by
 * scene_gameplay_discardParkedWorld.
 */
void transition_reset(void);
/**
 * Scene backdrop (BG1) setup indexed by g_SceneCurrent.args[1]; the g_TransitionEffect == 8
 * path of scene_gameplay_exit.
 */
void scene_unknown0804a48c(void);

// Each scene's handlers and its SceneState.args slot roles live in scene/<Name>.h,
// named from the SceneId tail.
