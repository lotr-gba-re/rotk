#pragma once

#include "mission.h"
#include "types.h"

/** ACTOR_TYPE_GATE_PANEL payload (20 bytes total). */
typedef struct ActorGatePanelMissionParams
{
    ActorMissionParams head;
    u8 gateType;  // GatePanelTypes index (block shape + tiles)
    u8 mode;      // count direction (1 = the door panels)
    u8 tileSet;   // tile base selector for the block write
    u8 threshold; // count-down threshold (mode 0)
    u8 count;     // count-up multiplier (target = step * count + threshold)
    u8 timerReload;
    bool oneShot;  // retire after one pass
    u8 useCounter; // external use count (script op 13 decrements)
    u8 linkGroup;
    u8 linkStream; // fired when a pass completes
    bool sfxEnable;
    u8 field_0x13; // 0xff
} ActorGatePanelMissionParams;

/**
 * ACTOR_TYPE_GATE_PANEL: the tile-block slide state machine (base behaviorState:
 * 0 dormant, 1 fire-link-and-retire, 4 animating)
 */
typedef struct ActorGatePanelState
{
    /** params.mode */
    u8 mode;

    /**
     * params.oneShot; retire (fire the params link) when the target is
     * reached
     */
    bool oneShot;

    /** params.useCounter != 0 */
    bool hasUseCounter;

    /** set by script op 16 on activation; gates the one-shot slide SFX */
    bool activated;

    /** armed by the first shut pulse while op-16 active */
    bool shutArmed;

    /** external shut request pulse (cleared each update) */
    bool shutPulse;

    /** tiles per tick (GatePanelTypes[params.gateType]) */
    s8 step;

    /** frameTimer reload */
    u8 timerReload;

    u8 cursor;

    /** count-down threshold (mode 0) */
    u8 threshold;

    /** step * params.count + params.threshold; count-up target (mode 1) */
    u8 target;
    u8 field_0x8f;

    u8 frameTimer;

    /** script op 13 decrements */
    u8 useCounter;

    /** tile base selector for the block write */
    u8 tileSet;

    /** block shape (GatePanelTypes[params.gateType]) */
    u8 cols;
    u8 rows;

    /** slide SFX enable */
    bool sfxEnable;
} ActorGatePanelState;

// ACTOR_TYPE_GATE_PANEL: draws or erases its tile block on the map (shape from
// GatePanelTypes[params.gateType]), then fires its params link.
Actor *actor_gatePanel_createFromMission(u8 groupIndex, u8 actorIndex);
void actor_gatePanel_tick(Actor *actor);

/**
 * Write the panel's cols x rows tile block at its map position, advancing the animation
 * cursor; also fixes up collision per tile.
 */
void actor_gatePanel_writeTileBlock(Actor *actor);
