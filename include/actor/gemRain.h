#pragma once

#include "types.h"

/**
 * ACTOR_TYPE_GEM_RAIN: dormant (actionState 1) until an external pulse
 * (behaviorState 4) starts the rain; then spawns one falling-gem child every
 * 5 frames (actionStateTimer cadence) over its x window while it has credits
 * (base currentHp, 10 at spawn, refunded when a child despawns: the live-gem
 * cap). After spawnInterval frames (base field_0x54) it fires its params link
 * and retires
 */
typedef struct ActorGemRainState
{
    /** x-spawn bounds (params x -/+ half-spread) */
    s16 spreadMin;
    s16 spreadMax;

    /**
     * total rain lifetime in frames (params interval * 30, default 150);
     * seeds base field_0x54
     */
    u16 spawnInterval;
} ActorGemRainState;
