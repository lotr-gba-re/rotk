// Extra scene slots: the stock SceneHandlers table is full (26 entries, ids 0x00..0x19)
// and cannot grow in place, so the three dispatchers (game_dispatchSceneEnter/Tick/Exit) are
// replaced with range-checking versions that route ids 0x1a+ to the modcode's
// ModSceneHandlers table. Stock dispatch behavior is reproduced exactly.
//
// A mod scene entered directly from the in-game dialog (previous id 0x11) has its enter
// skipped by the stock special case, like any scene.

#include "mod_scenes.h"
#include "game.h"
#include "mission.h"
#include "scene.h"
#include "variables.h"

#if HACK_mod_save
#include "patches/mod_save/save_migrate.h"
#endif

/**
 * One row per enabled mod scene (enum ModSceneRow in hacks/mod_scenes.h). .rodata: the
 * modcode has no .data/.bss, so rows are bound at compile time, not registered at run
 * time.
 */
const SceneEntry ModSceneHandlers[MOD_SCENE_ROW_COUNT] = {
#if HACK_mod_save
    MOD_SCENE_ROW(SAVE_MIGRATE, modSave_migrateScene),
#endif
};

/**
 * The handler triple for a scene id: stock table below MOD_SCENE_FIRST_ID,
 * ModSceneHandlers at/above, NULL past the end (out-of-range ids no-op instead of
 * running modcode garbage).
 */
static const SceneEntry *lookupEntry(u32 id)
{
    if (id < MOD_SCENE_FIRST_ID)
    {
        return &SceneHandlers[id];
    }
    id -= MOD_SCENE_FIRST_ID;
    if (id >= MOD_SCENE_ROW_COUNT)
    {
        return NULL;
    }
    return &ModSceneHandlers[id];
}

/**
 * Range-checked scene-enter dispatch (stock body + mod table).
 *
 * REPLACES (whole function): game_dispatchSceneEnter @0x080331e8.
 */
void modScenes_dispatchEnter(void)
{
    const SceneEntry *entry = lookupEntry(g_SceneCurrent.id);
    if (entry == NULL || entry->enter == NULL)
    {
        return;
    }
    if (g_ScenePrevious.id != SCENE_ID_IN_GAME_DIALOG)
    {
        entry->enter();
    }
    else
    {
        mission_script_spawnGroupAndStartStream(g_InGameDialogState.resumeGroup,
                                                g_InGameDialogState.resumeStream);
    }
}

/**
 * Range-checked scene-tick dispatch (stock body + mod table).
 *
 * REPLACES (whole function): game_dispatchSceneTick @0x0803322c.
 */
void modScenes_dispatchTick(void)
{
    const SceneEntry *entry = lookupEntry(g_SceneCurrent.id);
    if (entry != NULL && entry->tick != NULL)
    {
        entry->tick();
    }
}

/**
 * Range-checked scene-exit dispatch (stock body + mod table).
 *
 * REPLACES (whole function): game_dispatchSceneExit @0x08033254.
 */
void modScenes_dispatchExit(void)
{
    const SceneEntry *entry = lookupEntry(g_SceneCurrent.id);
    if (entry != NULL && entry->exit != NULL && (g_GameFlags.p & GAME_FLAG_IN_GAME_DIALOG) == 0)
    {
        entry->exit();
    }
}
