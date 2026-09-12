#pragma once

#include "mission.h"
#include "types.h"

/** ACTOR_TYPE_SWITCH payload (20 bytes total). */
typedef struct ActorSwitchMissionParams
{
    ActorMissionParams head;
    u8 param;
    u8 variant;    // 3 = lever, 6 = cycling door
    u16 holdTime;  // 30 = the hold-time preset
    u8 startAtEnd; // start at the last animation frame, position -1
    u8 useCounter; // variant-6 cycle count
    u8 forwardLinkGroup;
    u8 forwardLinkStream; // fired when the forward animation ends
    u8 backLinkGroup;
    u8 backLinkStream; // fired when the back animation ends
    u8 field_0x12[2];  // 0xff 0xff
} ActorSwitchMissionParams;

typedef struct ActorSwitchState
{
    /**
     * params.holdTime * 30; copied to actionStateTimer when an animation ends
     * in a hold variant
     */
    s32 timerPreset;
    u8 field_0x88; // variant 1's activation latch

    /** external trigger pulse (cleared each update) */
    bool triggerPulse;

    /** current end: 1 = forward, -1 = back; toggles per animation */
    s8 position;

    s8 currentFrame;

    u8 field_0x8c;

    /** params.param */
    u8 param;

    /** params.useCounter; variant 6's remaining cycles (gates each pass) */
    u8 useCounter;
} ActorSwitchState;

// ACTOR_TYPE_SWITCH: fires its params' forward/backward (group, stream) link at each
// animation end. Variant 3 = lever (externally triggered), variant 6 = cycling door gated by
// a use counter (script op 13 decrements).
Actor *actor_switch_createFromMission(u8 groupIndex, u8 actorIndex);
void actor_switch_tick(Actor *actor);
