#pragma once

#include "item.h"
#include "mission.h"
#include "types.h"
#include "vector2.h"

/** ACTOR_TYPE_LOOT_PILE payload (12 bytes total). */
typedef struct ActorLootPileMissionParams
{
    ActorMissionParams head;
    u8 lootType; // 0..7 = the artifacts (collected bits in
                 // g_SaveGlobals.d.artifactMask), 8..0xc = plain scripted pickups,
                 // LOOT_TYPE_RUNE rolls a rune, anything else generates its
                 // drop on spawn
    u8 linkGroup;
    u8 linkStream;         // fired when the pickup is collected
    u8 extraTreasureBonus; // added to the picking player's
                           // STAT_EXTRA_TREASURE_PERCENT when generating
} ActorLootPileMissionParams;

typedef struct ActorLootPileState
{
    s32 despawnTimer;

    /** the dropped item (gem stacks pack their value into it) */
    Item item;

    /** per-player "standing on the drop" flags (gate the HUD text) */
    u8 standingFlags[2];

    bool pulseEnabled;
} ActorLootPileState;

// ACTOR_TYPE_LOOT_PILE (loot.h's ground drop / scripted pickup). actor_lootPile_createFromMission
// builds one from its spawn-group slot; actor_lootPile_create drops one dynamically (chest scatter,
// enemy death).
Actor *actor_lootPile_createFromMission(u8 groupIndex, u8 actorIndex);
Actor *actor_lootPile_create(Vector2Fp16 pos, u32 lootType, Item item, s32 despawnTimer);
void actor_lootPile_tick(Actor *drop);

// Player-contact callbacks of the loot pile (R press picks it up): actor_lootPile_trigger for
// drops and scripted pickups from loot type 8 up, actor_lootPile_triggerArtifact for the 8
// artifacts (loot types 0..7).
void actor_lootPile_trigger(Actor *drop, Actor *playerActor);
void actor_lootPile_triggerArtifact(Actor *drop, Actor *playerActor);

void actor_lootPile_pulseAnimation(Actor *drop);
void actor_lootPile_eatFood(u32 playerIndex, u16 healPercent);
