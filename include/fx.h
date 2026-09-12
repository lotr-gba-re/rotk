#pragma once

#include "actor.h"
#include "types.h"
#include "vector2.h"

/** fx_spawnOnActor type ids - rows of the FX descriptor table @0x08287b7c (stride 0x14). */
#define FX_TYPE_HIT_BURST 0x16

/** the on-kill AoE flash spawned on the killer by combat_sunburstOnKill/combat_nightburstOnKill */
#define FX_TYPE_AOE_BURST 0

// AoE-burst variant / fx_allocEmitter kind by proc
#define FX_AOE_BURST_SUN 8
#define FX_AOE_BURST_NIGHT 7

/** spawned by Gimli's shield bubble when it absorbs part of a hit */
#define FX_TYPE_SHIELD_ABSORB 9

/** written to that shield-absorb FX actor's recordType */
#define FX_SHIELD_ABSORB_UNK_PARAM 0x4d

/** variant values (sprite/palette select nibble) for FX_TYPE_HIT_BURST */
enum FxBurstVariant
{
    FX_BURST_TRIPLE = 6,
    FX_BURST_FIRE = 7,
    FX_BURST_VENOM = 8,
};

/**
 * written to the spawned burst FX actor's recordType by the Fireburst / Venomous melee hit effects
 */
#define FX_BURST_UNK_PARAM 0x68

/**
 * Spawn an FX actor attached to parent (copies its position, actionState =
 * ACTOR_STATE_FX_ATTACHED). variant selects the sprite/palette; alt flips a
 * display flag. Returns the FX actor (0 if the pool is full).
 */
Actor *fx_spawnOnActor(Actor *parent, u8 fxType, u8 variant, u8 alt);

/**
 * Tick callback for attached FX: follows the parent, self-destructs when the
 * parent actor is gone.
 */
u32 fx_tickAttachment(Actor *fx);

/**
 * Spawn the boxed interaction FX (recordType 0xe: hand-built trigger box) at a
 * position; variant selects the behavior. Called for a chest's on-open spawn.
 * Provisional.
 */
void fx_spawnTriggerFx(s32 x, s32 y, u8 variant);

/**
 * Transient particle-burst emitter, pooled (max 14 live). Its bulk is an un-decoded particle
 * descriptor filled by fx_allocEmitter from the descriptor tables; only .target and the handful of
 * fields the AoE on-kill procs (combat_sunburstOnKill/combat_nightburstOnKill) set afterward are
 * broken out.
 */
typedef struct FxEmitter
{
    u8 field_0x0[8];

    /** actor the emitter is anchored to */
    Actor *target;

    u32 field_0xc;
    u8 field_0x10[8];

    /** world position the burst plays at (fp16_16); anchored on .target's animation frame */
    Vector2Fp16 position;

    u32 field_0x20;
    void *field_0x24;
    u16 field_0x28;
    u16 field_0x2a;

    /** emitter kind (fx_allocEmitter's first arg) */
    u8 kind;

    u8 field_0x2d;
    u16 field_0x2e; // flag bits; the burst procs OR in 0x4082
    u16 field_0x30;
    u8 field_0x32[2];
    u16 field_0x34;
    u8 field_0x36;
    u8 field_0x37;
    u8 field_0x38;
    u8 field_0x39;
    u16 field_0x3a;
    u8 field_0x3c[3];
    u8 field_0x3f;
    u8 field_0x40[4];
} FxEmitter;
/** Anchor the emitter's world position on the actor's current animation frame. */
static inline void fx_setEmitterPosition(FxEmitter *emitter, Actor *actor, Vector2Fp16 *out)
{
    // the callee fills raw frame offsets; they become fp16_16 after the scale below
    actor_getAnimationFrame(actor, (u32 *)out);
    out->x = actor->coords.x;
    out->y = actor->coords.y - (out->y << 15);
    emitter->position = *out;
}

/**
 * Allocate and preload a pooled FxEmitter for the given kind (0 when the 14-slot pool is full).
 * The two extra args index the particle descriptor tables.
 */
FxEmitter *fx_allocEmitter(u8 kind, u32 group, u32 variant);

/** Tick every live emitter in the pool, freeing the ones that expired. */
void fx_tickEmitters(void);

/**
 * Attach a timed status-effect FX to target (e.g. the Sunburst stun visual), tagging
 * target->field_0x100 with flagBit so it is applied at most once. parent is stored on the
 * new FX actor as its parentActor (the attacker at the combat call sites); NULL for none.
 * Meaning of the remaining args unverified.
 */
Actor *fx_attachStatusEffect(Actor *parent, Actor *target, u16 duration, u8 kind, u8 arg4, u8 arg5,
                             u8 flagBit);
