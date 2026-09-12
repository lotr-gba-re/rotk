#pragma once

#include "mission.h"
#include "types.h"

/** ACTOR_TYPE_MISSION_WARP payload (16 bytes total). */
typedef struct ActorMissionWarpMissionParams
{
    ActorMissionParams head;
    u8 halfWidth; // touch rect half extents
    u8 halfHeight;
    u8 spawnPoint;
    u8 mission;      // 0x00/0x01 = unset placeholder; the link stream's transition
                     // ops supersede it
    u8 linkStream;   // run synchronously on trigger
    u8 field_0xd[3]; // 0xff 0xff 0xff
} ActorMissionWarpMissionParams;

typedef struct ActorMissionWarpState
{
    /** params.mission */
    u16 mission;

    /** both players' params.spawnPoint */
    u8 spawnPoint[2];

    u8 field_0x88;

    /**
     * players currently inside the trigger box; the warp fires when this
     * reaches g_PlayerGlobals.activePlayerCount
     */
    u8 playersInside;
} ActorMissionWarpState;

/**
 * ACTOR_TYPE_MISSION_WARP: on trigger it also runs its linked stream synchronously, and
 * that stream's transition ops supersede the params' raw target.
 */
Actor *actor_missionWarp_createFromMission(u8 groupIndex, u8 actorIndex);

/**
 * Per-frame update: tracks the nearest player distance and raises the HUD text flag
 * while a player is within range.
 */
void actor_missionWarp_tick(Actor *actor);

/** Player-contact callback of the mission warp. */
void actor_missionWarp_trigger(Actor *warp, Actor *touching);
