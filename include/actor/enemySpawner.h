#pragma once

#include "mission.h"
#include "types.h"

/**
 * ACTOR_TYPE_ENEMY_SPAWNER payload (16 bytes total; the unused RNG variant rearranges
 * the enemyId tail: three enemy ids and two probability bytes).
 */
typedef struct ActorEnemySpawnerMissionParams
{
    ActorMissionParams head;
    u8 variant;
    u8 enemyId; // index into the enemy-spawn table @0x080584a8 (not yet carved)
    u8 cap;     // concurrent children - 1
    u8 budget;  // total spawns; 0 = unbudgeted
    u8 linkGroup;
    u8 linkStream;   // fired on retirement
    u8 field_0xe[2]; // 0xff 0xff
} ActorEnemySpawnerMissionParams;

/**
 * ACTOR_TYPE_ENEMY_SPAWNER (the type-8 layout; the unused RNG variant rearranges the tail).
 * Spawn cadence runs in base actionStateTimer (init 15 frames); the sub-state
 * machine is base actionState (5 spawning, 0xc shutdown monitor, 4 retired)
 */
typedef struct ActorEnemySpawnerState
{
    /**
     * live children; a spawn tick only fills a NULL slot (respawn on
     * death)
     */
    struct Actor *children[4];

    /**
     * index into the enemy-spawn table @0x080584a8 ({recordType, variant, unitType}
     * triples; not yet carved)
     */
    u16 enemyId;
    u8 field_0x96[7];

    /** total spawns left, at 0 the spawner retires */
    s8 budget;

    /** params.cap + 1; concurrent spawn credits, restored when a child dies */
    u8 cap;

    /** params.budget == 0 (unbudgeted: never retires on its own) */
    bool unbudgeted;
} ActorEnemySpawnerState;

// ACTOR_TYPE_ENEMY_SPAWNER / ACTOR_TYPE_ENEMY_SPAWNER_RNG: keeps up to 4 child actors
// (as.enemySpawner.children), respawning dead ones up to a total spawn budget. Setting its state
// to 1 (script op 51) stops spawning and retires it once its children are dead, then fires its
// params link.
Actor *actor_enemySpawner_createFromMission(u8 groupIndex, u8 actorIndex);
void actor_enemySpawner_tick(Actor *actor);

/**
 * Spawn one child into a free slot, decrementing the budget.
 *
 * @return TRUE when a child was spawned
 */
bool actor_enemySpawner_trySpawn(Actor *actor);

/** @return first free child slot (0..3), 4 when all are occupied */
u32 actor_enemySpawner_findFreeChildSlot(Actor *actor);
Actor *actor_enemySpawner_spawnChild(u16 enemyIndex, Actor *spawner);
