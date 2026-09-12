#pragma once

#include "scene.h"
#include "types.h"

/**
 * g_GameFlags bits (mask view: .p & GAME_FLAG_*). Global game/session state
 * word; mostly undecoded.
 */
enum GameFlag
{
    // in-game dialog overlay active; game_dispatchSceneExit skips the scene exit handler
    // while set (set by scene_requestInGameDialog, cleared by scene_inGameDialog_tick)
    GAME_FLAG_IN_GAME_DIALOG = 1 << 0,
    // gameplay world parked: the main actor list is stashed in g_ActorListParked while
    // a non-gameplay scene runs; set by scene_gameplay_parkWorld, cleared by scene_gameplay_enter
    // on resume and by scene_gameplay_discardParkedWorld on quit-to-menu
    GAME_FLAG_WORLD_PARKED = 1 << 3,
    // a scene change is being dispatched; set from the game_isSceneChangePending branch
    // until the new scene's init has run
    GAME_FLAG_SCENE_CHANGE = 1 << 4,
    // co-op session active: enemy stat getters add +50% HP and switch indirect kill credit
    // (KILL_CREDIT_INDIRECT) to full XP; multiplayer shop actions route through the
    // message queue instead of applying locally
    GAME_FLAG_COOP_ACTIVE = 1 << 5,
    // bonus map session: set when the save/load menu opens for a bonus-map load, cleared
    // by scene_mainMenu_enter; the save (de)serializer keeps the slot's story progress
    // in g_SaveParkedProgress instead of loading it, and a save does not stamp a fresh game
    GAME_FLAG_BONUS_MAP_ACTIVE = 1 << 7,
    // set around the scene_gameplay_exit call in the co-op pending-change path;
    // scene_gameplay_exit skips its fade-out while set
    GAME_FLAG_SKIP_TRANSITION_FADE = 1 << 10,
    // set for the duration of the blocking mosaic and co-op transitions; read on the
    // actor tick path
    GAME_FLAG_11 = 1 << 11,
    // set around EEPROM transfers to park the render interrupt
    GAME_FLAG_SAVE_OP_IN_PROGRESS = 1 << 13,
    // co-op session past the level select: set as scene_multiplayerLevelSelect_enter
    // finishes, cleared by scene_mainMenu_enter; the save serializer keeps the slot's
    // story progress in g_SaveParkedProgress like GAME_FLAG_BONUS_MAP_ACTIVE
    GAME_FLAG_COOP_LEVEL_SELECTED = 1 << 14,
    GAME_FLAG_18 = 1 << 18,
    // request that the scene being faded into runs as an overlay menu;
    // scene_gameplay_exit turns it into GAME_FLAG_OVERLAY, the overlay scene's init
    // clears it
    GAME_FLAG_REQUEST_OVERLAY = 1 << 19,
    GAME_FLAG_20 = 1 << 20, // cleared with GAME_FLAG_SCENE_CHANGE before scene_gameplay_exit
    // render the overlay actor list (g_ActorListOverlay) in place of the main list,
    // leaving the main-list actors intact underneath; the overlay scene's exit handler
    // clears it
    GAME_FLAG_OVERLAY = 1 << 22,
    // corruption meter tiers, set by the corruption update (0x0802623c) at meter 51 / 81 /
    // 91: tier 1 buffs all enemies (+25% accuracy/defense, +50% HP/damage), tier 2 has no
    // consumer, tier 3 makes the corruption HUD actor spawn a Nazgul at the player
    GAME_FLAG_CORRUPTION_TIER_1 = 1 << 26,
    GAME_FLAG_CORRUPTION_TIER_2 = 1 << 27,
    GAME_FLAG_CORRUPTION_TIER_3 = 1 << 28,
    GAME_FLAG_30 = 1 << 30, // cleared as scene_gameplay_exit finishes
    // gates the scene_gameplay_exit path of a co-op pending change;
    // cleared once the change is applied
    GAME_FLAG_31 = 1u << 31,
};

typedef union GameFlags {
    u32 p;

    struct
    {
        u32 inGameDialog : 1; // 1 << 0
        u32 : 3;
        u32 sceneChange : 1; // 1 << 4
        u32 coopActive : 1;  // 1 << 5
        u32 : 1;
        u32 bonusMapActive : 1; // 1 << 7
        u32 : 2;
        u32 skipTransitionFade : 1; // 1 << 10
        u32 field_bit_11 : 1;       // 1 << 11
        u32 : 1;
        u32 saveOpInProgress : 1;  // 1 << 13
        u32 coopLevelSelected : 1; // 1 << 14
        u32 : 4;
        u32 requestOverlay : 1; // 1 << 19
        u32 field_bit_20 : 1;   // 1 << 20
        u32 : 1;
        u32 overlay : 1; // 1 << 22
        u32 : 3;
        u32 corruptionTier1 : 1; // 1 << 26
        u32 corruptionTier2 : 1; // 1 << 27
        u32 corruptionTier3 : 1; // 1 << 28
        u32 : 1;
        u32 field_bit_30 : 1; // 1 << 30
        u32 field_bit_31 : 1; // 1u << 31
    } d;
} GameFlags;

void game_initBoot(void);
void game_tickFrame(void);
void game_tickFxAndHuds(void);

// Scene orchestration: the game core's mode switch. Requests stage g_SceneNext;
// game_tickFrame applies the change and runs the SceneHandlers slots.
SceneId game_getCurrentSceneId(void);
SceneId game_getNextSceneId(void);
void game_requestSceneChange(SceneId sceneId);
void game_requestSceneChangeWithArgs(SceneId sceneId, u32 arg0, u32 arg1);
bool game_isSceneChangePending(void);

void game_dispatchSceneEnter(void);
void game_dispatchSceneTick(void);
void game_dispatchSceneExit(void);
