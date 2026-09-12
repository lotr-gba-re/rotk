#pragma once

#include "mission.h"
#include "types.h"

/** ACTOR_TYPE_CHEST payload (16 bytes total). */
typedef struct ActorChestMissionParams
{
    ActorMissionParams head;
    u8 maxDrops;        // drop-count roll bound (seeds as.chest.maxDrops)
    u8 guaranteedDrops; // 0 = only a 1-in-10 roll yields drops
    u8 spawnOnOpen;     // when set, opening spawns a companion object
    u8 spawnVariant;    // variant of the on-open spawn
    u8 linkGroup;
    u8 linkStream;   // fired on open when flags bit 31 is set
    u8 field_0xe[2]; // 0xff 0xff
} ActorChestMissionParams;

typedef struct ActorChestState
{
    /** params.guaranteedDrops */
    u8 guaranteedDrops;

    /** already opened (blocks retrigger) */
    bool opened;

    /** params.spawnOnOpen */
    bool spawnOnOpen;

    /** drop count is rolled 1..maxDrops on open (not set by the constructor) */
    u8 maxDrops;

    /** frames between scattered drops (not set by the constructor) */
    u8 dropInterval;

    /** params.spawnVariant */
    u8 spawnVariant;
    u8 field_0x8a[2];
} ActorChestState;

// ACTOR_TYPE_CHEST (loot.h's loot container): its unitType is LOOT_UNIT_TYPE_CONTAINER.
// actor_chest_trigger opens it on the player's R press; the update scatters
// actor_lootPile_create drops
// at its own coords, then fires its params link.
Actor *actor_chest_createFromMission(u8 groupIndex, u8 actorIndex);
void actor_chest_tick(Actor *actor);

/** Player-contact callback of the chest (R press opens it). */
void actor_chest_trigger(Actor *chest, Actor *touching);
