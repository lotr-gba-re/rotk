// Extra scene slots past the stock SceneHandlers table (mod_scenes). Shared by the patch
// that provides the table and by every patch that adds a row to it; no single patch owns
// the contract, so it lives at the hacks/ root (see hacks/README.md).
#pragma once

#if !HACK_mod_scenes
#error "requires the mod_scenes patch for the ModSceneHandlers table"
#endif

#include "scene.h" // struct SceneEntry

/** First scene id routed to ModSceneHandlers (== length of the stock SceneHandlers table). */
#define MOD_SCENE_FIRST_ID 0x1au

/**
 * scene rows
 *
 * Enum order is table order. Guard a patch's entry with its #if HACK_<name>. What a scene
 * must implement: hacks/docs/mod-scenes.md.
 */
enum ModSceneRow
{
#if HACK_mod_save
    MOD_SCENE_ROW_SAVE_MIGRATE,
#endif
    MOD_SCENE_ROW_COUNT,
};

/** The scene id to game_requestSceneChange / game_requestSceneChangeWithArgs for a row. */
#define MOD_SCENE_ID(name) (MOD_SCENE_FIRST_ID + (u32)MOD_SCENE_ROW_##name)

/** Handler rows for ids MOD_SCENE_FIRST_ID and up, one per enabled row. */
extern const SceneEntry ModSceneHandlers[MOD_SCENE_ROW_COUNT];

/**
 * The ModSceneHandlers initializer for a row, from the patch's <prefix>Enter /
 * <prefix>Tick / <prefix>Exit handler triple:
 *   MOD_SCENE_ROW(SAVE_MIGRATE, modSave_migrateScene)
 */
#define MOD_SCENE_ROW(name, prefix)                                                                \
    [MOD_SCENE_ROW_##name] = {prefix##Enter, prefix##Tick, prefix##Exit}
