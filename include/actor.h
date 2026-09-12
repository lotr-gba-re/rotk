#pragma once

#include "actor/backpackIcon.h"
#include "actor/chest.h"
#include "actor/enemySpawner.h"
#include "actor/gatePanel.h"
#include "actor/gemRain.h"
#include "actor/lootPile.h"
#include "actor/missionWarp.h"
#include "actor/npc.h"
#include "actor/player.h"
#include "actor/shieldBubble.h"
#include "actor/switch.h"
#include "gba.h"
#include "mission.h"
#include "spriteAnimation.h"
#include "types.h"
#include "vector2.h"

// tag fwds for mutually-referential pointer types (combat.h Player, sprite.h
// SpriteAnimation); struct Actor itself is forward-typedef'd in mission.h
struct Player;
struct SpriteAnimation;

/** union ActorCombatFlags bits (the mask view the code tests). */
enum ActorCombatFlag
{
    // evade a melee hit EVASION_CHANCE_PERCENT of the time
    ACTOR_COMBAT_FLAG_EVASIVE = 1 << 3,
};

/**
 * Actor.combatFlags; matched code tests the ACTOR_COMBAT_FLAG_* masks on the word view
 * instead.
 * Packed (union and bitfield struct): agbcc would otherwise pad the union to 4-byte
 * size/alignment and shift the Actor layout.
 */
typedef union ActorCombatFlags {
    u8 p;

    struct
    {
        u8 field_bit_0 : 1; // 1 << 0
        u8 field_bit_1 : 1; // 1 << 1
        u8 field_bit_2 : 1; // 1 << 2
        u8 evasive : 1;     // 1 << 3: evades a melee hit EVASION_CHANCE_PERCENT of the time
        u8 field_bit_4 : 1; // 1 << 4
        u8 field_bit_5 : 1; // 1 << 5
        u8 field_bit_6 : 1; // 1 << 6
        u8 field_bit_7 : 1; // 1 << 7
    } __attribute__((packed)) d;
} __attribute__((packed)) ActorCombatFlags;
/** union ActorFlags bits (the mask view the code tests). */
enum ActorFlag
{
    // submit the actor to the render queues; without it the actor still ticks, animates,
    // and collides but is never drawn. Cleared to hide (enemy AI) or to retire (with
    // PENDING_REMOVE)
    ACTOR_FLAG_RENDER = 1 << 0,

    // free the actor this frame
    ACTOR_FLAG_PENDING_REMOVE = 1 << 1,

    // participate in the collision pass: submitted to g_CollisionCheckQueue each frame;
    // pair tests require it on both actors
    ACTOR_FLAG_COLLISION_CHECK = 1 << 2,

    // tick even when culled off-screen; without it only ON_SCREEN actors tick
    ACTOR_FLAG_TICK_ALWAYS = 1 << 3,

    // play the animation: the animation VM advances frames while set (0x080022e4)
    ACTOR_FLAG_ANIMATION_PLAYING = 1 << 4,

    // NPC already reacting to an enemy attack; further enemy attacks whiff
    ACTOR_FLAG_5 = 1 << 5,

    // ignore attacks: skipped by the Sunburst/Nightburst AoE; attacks on it whiff
    // outright
    ACTOR_FLAG_INVULNERABLE = 1 << 6,

    // animation frame changed: set by the animation VM on a frame advance
    // (0x080022e4); dirty marker for sprite_commitGfx
    ACTOR_FLAG_ANIMATION_FRAME_CHANGED = 1 << 7,

    // freeze the committed graphics: skips sprite_commitGfx in both render-queue drains
    // and suppresses the gfx-slot release, so the actor keeps rendering
    // the last committed frame
    ACTOR_FLAG_FREEZE_GFX = 1 << 8,

    // collide with tiles: the wall-slide resolver runs each frame (0x080341a8)
    ACTOR_FLAG_TILE_COLLISION = 1 << 9,

    ACTOR_FLAG_10 = 1 << 10,

    // ignore soft tiles (tile type 1): only hard tiles block movement (0x080341a8)
    ACTOR_FLAG_IGNORE_SOFT_TILES = 1 << 11,

    // per-frame cull output: in a player's camera rect. Cleared at the top of every
    // actor_tick, re-set by the cull (0x08000c38); gates the tick and the world
    // render queue
    ACTOR_FLAG_ON_SCREEN = 1 << 12,

    // run only the tick callback: skips the animation VM, tile collision, and the
    // collision/render queue submissions
    ACTOR_FLAG_TICK_ONLY = 1 << 13,

    // never flinch into ACTOR_STATE_HURT on a landed hit (super armor)
    ACTOR_FLAG_NO_FLINCH = 1 << 14,

    // use the interactionRadius distance test instead of trigger boxes when both actors
    // in a collision pair have it
    ACTOR_FLAG_RADIUS_PROXIMITY = 1 << 15,

    // never initiate collision tests: sorted to the queue tail, always the tested party;
    // always set with ACTOR_FLAG_COLLISION_CHECK
    ACTOR_FLAG_COLLISION_PASSIVE = 1 << 17,

    ACTOR_FLAG_18 = 1 << 18,
    ACTOR_FLAG_19 = 1 << 19,

    // skip the camera cull and render via the fixed-order commit queue
    ACTOR_FLAG_ALWAYS_VISIBLE = 1 << 21,

    // animation reached its end: one-shot animations freeze on the last frame; cleared
    // by sprite_setAnimation
    ACTOR_FLAG_ANIMATION_END = 1 << 22,

    // END | PLAYING both set: a one-shot animation played out and froze on its last
    // frame; the animation-pulse restart test
    ACTOR_FLAG_ANIMATION_END_PLAYING = ACTOR_FLAG_ANIMATION_END | ACTOR_FLAG_ANIMATION_PLAYING,

    // set with ACTOR_FLAG_5 when an enemy attack knocks down a combat-active NPC
    ACTOR_FLAG_23 = 1 << 23,

    // skip the tickCallback and the post-tick update phase (animation, collision,
    // queues); the actor still renders
    ACTOR_FLAG_TICK_SUPPRESSED = 1 << 25,

    // render a white-out hit flash while the screen flash is active (g_GameFlags bits
    // 16/17)
    ACTOR_FLAG_HIT_FLASH = 1 << 26,

    // set by actor_applyTypeDefaults at the end of the constructors
    ACTOR_FLAG_DEFAULTS_APPLIED = 1 << 27,

    ACTOR_FLAG_28 = 1 << 28,

    // per-frame cull output: in the active player's camera rect (subset of ON_SCREEN;
    // forced for ALWAYS_VISIBLE and overlay-list actors). Keeps the main-sprite gfx
    // slot alive (actor_tick release gate) and feeds the alloc-priority call in
    // sprite_composeOam (0x08002ca8)
    ACTOR_FLAG_ON_ACTIVE_PLAYER_SCREEN = 1 << 30,

    // spawned from a mission record group: the record backrefs (recordGroup/recordIndex)
    // are valid
    // and the record's link stream is gated on it (set by mission_script_spawnGroup after
    // the constructor)
    ACTOR_FLAG_MISSION_SPAWNED = 1u << 31,
};

/** Actor.flags; matched code tests the ACTOR_FLAG_* masks on the word view instead. */
typedef union ActorFlags {
    u32 p;

    struct
    {
        u32 render : 1;                // 1 << 0: render-queue enable
        u32 pendingRemove : 1;         // 1 << 1: free this frame
        u32 collisionCheck : 1;        // 1 << 2: collision-pass participant
        u32 tickAlways : 1;            // 1 << 3: tick even off-screen
        u32 animationPlaying : 1;      // 1 << 4: play the animation (VM advances frames)
        u32 field_bit_5 : 1;           // 1 << 5
        u32 invulnerable : 1;          // 1 << 6: untargetable; attacks on it whiff
        u32 animationFrameChanged : 1; // 1 << 7: frame-changed dirty marker
        u32 freezeGfx : 1;             // 1 << 8: keep the last committed frame
        u32 tileCollision : 1;         // 1 << 9: wall-slide resolver runs
        u32 field_bit_10 : 1;          // 1 << 10
        u32 ignoreSoftTiles : 1;       // 1 << 11: soft tiles passable
        u32 onScreen : 1;              // 1 << 12: per-frame: in a player's camera rect
        u32 tickOnly : 1;              // 1 << 13: run only the tick callback
        u32 noFlinch : 1;              // 1 << 14: super armor
        u32 radiusProximity : 1;       // 1 << 15: interactionRadius tests
        u32 field_bit_16 : 1;          // 1 << 16
        u32 collisionPassive : 1;      // 1 << 17: tested, never tests (with collisionCheck)
        u32 field_bit_18 : 1;          // 1 << 18
        u32 field_bit_19 : 1;          // 1 << 19
        u32 field_bit_20 : 1;          // 1 << 20
        u32 alwaysVisible : 1;         // 1 << 21: skip cull, fixed-order render
        u32 animationEnd : 1;          // 1 << 22: animation script ended
        u32 field_bit_23 : 1;          // 1 << 23
        u32 field_bit_24 : 1;          // 1 << 24
        u32 tickSuppressed : 1;        // 1 << 25: skip tick + post-tick phase
        u32 hitFlash : 1;              // 1 << 26: white-out flash
        u32 defaultsApplied : 1;       // 1 << 27
        u32 field_bit_28 : 1;          // 1 << 28
        u32 field_bit_29 : 1;          // 1 << 29
        u32 onActivePlayerScreen : 1;  // 1 << 30: per-frame: in active player's rect
        u32 missionSpawned : 1;        // 1 << 31: from a mission record group
    } d;
} ActorFlags;

/** per-frame tick callback (Actor.tickCallback); actor_tick discards the return value */
typedef void (*ActorCallback)(struct Actor *);

/**
 * trigger callback in an Actor's triggerCallbacks slot: the per-frame
 * collision pass calls it as (self, other) when one of self's
 * enabled trigger boxes overlaps `other`
 */
typedef void (*ActorTriggerCallback)(struct Actor *self, struct Actor *other);

/** combat actors (enemies/players) and FX */
typedef struct ActorCombatState
{
    // subkind-polymorphic head
    union {
        /**
         * FX attach/follow target for attach FX, seeded
         * with the parent at spawn (fx_spawnOnActor / 0x08043838)
         */
        struct Actor *attachTarget;

        // player avatar: animation-bank change detection; actor_player_tickTail applies
        // animationBank via sprite_selectAnimation when it differs from appliedAnimationBank
        struct
        {
            void *appliedAnimationBank;
            void *animationBank;
        } player;

        // enemy: per-kind AI parameter bytes seeded at spawn; meanings vary by
        // enemy kind. The melee-chaser AI (0x0800499c) reads [2] (s16) as the
        // aggro/deaggro distance, [4] as the close-range attack threshold and
        // [5]/[6] as actionStateTimer reloads; patrol kinds read [0]/[2]
        // as a target coordinate
        u8 enemyAiParams[8];
    } head;

    u8 field_0x8c[2];

    /**
     * stored damage read by the attack-state damage calc
     * (0x33/0x34/0x35/0x3a/0x3b/0x59)
     */
    u16 storedDamage;

    /**
     * frames left on the current poison/venom status (venom:
     * VENOM_DURATION_FRAMES; an enemy poison attack: poisonPower *
     * STATUS_FRAMES_PER_POWER, cut by STAT_POISON_RESIST_PERCENT)
     */
    s16 venomDuration;

    /**
     * frames left on the current fear status (an enemy fear attack: fearPower *
     * STATUS_FRAMES_PER_POWER, cut by STAT_FEAR_RESIST_PERCENT)
     */
    s16 fearDuration;

    /**
     * player avatar pose selection: the pose actor_player_tickTail last applied vs the one
     * the player tick requested
     */
    u8 appliedPose;
    u8 requestedPose;

    /**
     * index into g_PlayerGlobals.players (0/1); only meaningful for a player's
     * avatar
     */
    u8 playerIndex;

    u8 field_0x97[2];

    /**
     * per-actionState flags; bits cleared on state exit by
     * actor_resetActionState (bit0/1/3 mark the attack states
     * 0x33/0x34/0x35/0x3a/0x3b/0x58/0x59, bit2 states 0x4a..0x5c); individual
     * meanings unknown
     */
    u8 actionStateFlags;

    /**
     * player avatar combo/attack sub-state bytes (0x080236b8:
     * [2] combo stage flags, [3] bit 1 lock, [4] combo SFX pending, [5] attack
     * latch, [6] state-3 entry latch); [5] is also read by some enemy AI
     * (0x08014e08)
     */
    u8 comboState[7];

    u8 field_0xa1[11];
} ActorCombatState;

/**
 * Kind-polymorphic span: every record kind lays out this window its own
 * way; sharing resumes at coords. combat is the view for combat actors (enemies,
 * players) and FX; within it the head union is again subkind-polymorphic (FX
 * attach target / player animation banks / enemy AI parameters). The other views are per
 * record type (enum ActorType), decoded from the ActorMissionConstructors. Embedded mid-struct, so
 * a view must fit 0x28 bytes and declare its pads; what every kind shares (coords,
 * velocity, flags, recordType, the record backrefs) stays a base member outside the
 * union.
 */
union ActorKindState {
    ActorCombatState combat;

    ActorShieldGandalfState shieldGandalf;
    ActorShieldGimliState shieldGimli;
    ActorGemRainState gemRain;
    ActorLootPileState lootPile;
    ActorMissionWarpState missionWarp;
    ActorSwitchState switch_;
    ActorChestState chest;
    ActorBackpackIconState backpackIcon;
    ActorGatePanelState gatePanel;
    ActorEnemySpawnerState enemySpawner;
};

/**
 * Sprite-engine render sub-state of an Actor: the multi-sprite slot
 * bookkeeping a sprite uses to display up to 4 sub-sprites (per-slot part id, tile number,
 * gfx size) plus its animation selection, and a linear x/y scale-over-time run.
 * Zeroed by pool_alloc's memset on every
 * allocation.
 */
struct SpriteRender
{
    /**
     * animation bank: SpriteAnimation *[part][7 poses], each pointing at one SpriteAnimation
     * per facing variant; a slot's clip is animationBank[slotPartIds[slot] * 7 + pose]
     * [facingVariant]
     */
    void *animationBank;

    /**
     * per-slot OBJ VRAM tile number of the committed frame gfx (0xffff = none), written into
     * the slot's OAM attr2; allocated by sprite_commitGfx, freed with slotGfxSizes
     */
    u16 slotTileNumbers[4];

    /** per-slot gfx byte size of the committed frame, the tile allocator's free argument */
    u16 slotGfxSizes[4];
    u8 slotPartIds[4]; // per-slot animation-bank part (0xff = slot free); sprite_detachSlot
                       // frees to 0xff

    /**
     * per-slot OAM palette number (attr2 bits 12-15); seeded from the actor's own
     * oam.paletteNumber by sprite_initSlots, set per slot by sprite_attachSlot
     */
    u8 slotPaletteNumbers[4];

    /**
     * per-slot draw order (0..7), an engine ordering distinct from the OAM BG priority;
     * sprite_composeOam emits slots at 7..4 before the main sprite and 3..0 after it
     */
    u8 slotDrawOrders[4];

    u8 slotFlags[4];  // per-slot flags; bit 2 set by sprite_setSlotFlag2, bits 2/3 gate the
                      // frame advance
    u8 pose;          // pose column (0..6) within the animation bank
    u8 facingVariant; // FacingVariants[facing]: which of the 5 per-pose animations
    u8 field_0x12a[2];

    // linear scale-over-time run (16.16 fixed-point texture-step divisors, 0x10000 = 1:1;
    // on-screen size is the reciprocal); armed by sprite_startScale, ticked once per
    // frame by sprite_tickScale while
    // scaleFramesLeft is non-zero. The affine group index aliases the OAM attr1 h/v
    // flip bits (attr1 bits 9-13), so a flipped (left-facing) sprite loses its flip
    // when a scale is armed and must be mirrored via a negative X scale instead
    u32 invScaleX; // 0x40000 / scaleX's 8.8 form = the visual magnification in 10.10
                   // fixed (0x400 = 1:1)
    u32 invScaleY; // 0x40000 / scaleY's 8.8 form = the visual magnification in 10.10
                   // fixed (0x400 = 1:1)
    s32 scaleX;    // current scales; advanced by the deltas each tick
    s32 scaleY;
    s32 scaleDeltaX; // per-frame deltas: (target - start) / frames
    s32 scaleDeltaY;
    u16 scaleFramesLeft;

    u8 field_0x146[2];
};

/** Which actor list/pool an Actor belongs to (Actor.listId). */
typedef enum ActorListId
{
    ACTOR_LIST_MAIN = 0,
    ACTOR_LIST_OVERLAY = 1,
} __attribute__((packed)) ActorListId;

struct Actor
{
    /** next actor in the actor list */
    struct Actor *next;

    /** previous actor in the actor list */
    struct Actor *previous;

    /**
     * owning Player of a player-side companion/summon/hollow actor (NULL for enemies
     * and objects); the companion AI reads it to face/follow the owner
     */
    struct Player *ownerChar;

    /**
     * logical parent / attack origin: the attach parent of an FX actor, the spawner of
     * a spawned child, the attacker of a projectile (combat reads it as the attack's source)
     */
    struct Actor *parentActor;

    /**
     * render-follow target: while set, the per-frame render glue in
     * actor_tickAllInList (0x08003540) copies the target's render coords and layer
     * bits into this actor, offset by spriteFlags bits 3/4 (attach FX set both this
     * and parentActor)
     */
    struct Actor *renderFollowTarget;

    ActorCallback tickCallback;

    /**
     * per-trigger-box callbacks, fired by the collision pass
     * (actor_checkCollisions, over the per-frame g_CollisionCheckQueue submission) as
     * (self, other) when box slot i overlaps another actor;
     * installed per kind (mission warp / chest / switch trigger handlers,
     * actor_lootPile_trigger for ground drops)
     */
    ActorTriggerCallback triggerCallbacks[2];

    /**
     * frames left in the current animation frame; reset to frameDuration when the frame
     * advances
     */
    u8 animationFramesLeft;

    /** per-frame duration; copied from SpriteAnimation.frameDuration by sprite_setAnimation */
    u8 animationFrameDuration;

    /** previous animation frame index (the frame advance copies currentFrame here) */
    u8 previousAnimationFrame;

    /**
     * current animation frame index into spriteAnimation->frames; read by actor_getAnimationFrame
     * and the per-slot frame lookups
     */
    u8 currentAnimationFrame;

    /** active animation descriptor (sprite_setAnimation / 0x08001bf0) */
    struct SpriteAnimation *spriteAnimation;

    /** animation bytecode cursor, advanced by the animation VM (0x080022e4) */
    const u8 *animationScriptCursor;

    /** jump base the animation VM computes opcode targets from (0x080022e4) */
    const u8 *animationJumpBase;

    /**
     * bytes 2..5 of the active animation descriptor, copied on animation set; read as
     * signed offsets (0x08008558)
     */
    s8 animationHeader[4];

    u8 field_0x34[2];

    /**
     * trigger-box slots (see struct ActorTriggerBox), refreshed per animation
     * frame from frame data (sprite_setAnimationFrame / 0x08000f0c); the active count is
     * triggerBoxCount. Kinds without an animation (mission warp, attach FX) write
     * slot 0 by hand. The tail slots double as movement/AI scratch for kinds whose
     * animations declare few boxes (0x08003650, 0x0800b968)
     */
    struct ActorTriggerBox triggerBoxes[4];

    u8 field_0x4e[2];

    /**
     * frames left in the current actionState (stun: SUNBURST_STUN_FRAMES / STAT_FOE_STUN_TIME);
     * a ground drop's despawn countdown while LOOT_DROP_DESPAWNING
     */
    s32 actionStateTimer;

    /**
     * per-kind scratch timers. Known reuses: a ground drop's
     * "backpack full" message timer (LOOT_DROP_FULL_MESSAGE) / animation pulse
     * period, a gem-rain actor's rain lifetime countdown (field_0x54, seeded from
     * as.gemRain.spawnInterval; the 5-frame per-gem cadence runs in
     * actionStateTimer), enemy AI / combat hurt-guard counters, the player
     * avatar's hurt/death animation timers (0x080236b8)
     */
    s16 field_0x54;
    s16 field_0x56;
    u8 field_0x58[2];

    /**
     * hit points of a combat actor (zeroed by pool_alloc's memset, seeded per kind).
     * Non-combat reuses: a chest's remaining drops, a gem-rain
     * actor's live-gem credits (10 at spawn, spent per spawned gem, refunded when
     * a gem child despawns)
     */
    s16 currentHp;

    /**
     * flinch/stagger frames left (combat actors only); reseeded from
     * EnemyTypeInfo.hitStun on a flinching hit
     */
    s16 hitStunTimer;

    u8 field_0x5e[2]; // combat actors (a countdown reseeded to 5 near the
                      // enemy death check, 0x08009dec / 0x0800a480)

    /**
     * per-kind variant byte (pairs with unitType like fx_spawnOnActor's fxType/variant);
     * engine-read for palette slot selection. A ground drop's LootType, a
     * loot container/pickup object's kind (0..0xc), an ACTOR_TYPE_NPC actor's
     * enum NpcKind (indexes NpcSfxSets)
     */
    u8 variant;

    /**
     * engine unit class: indexes EnemyTypes/EnemyLootInfos for an enemy; at or above
     * LOOT_UNIT_TYPE_CONTAINER there is no EnemyLootInfo entry (chests)
     */
    u8 unitType;
    /**
     * per-kind state byte (enum ActorActionState for combat actors); a shop menu
     * cursor's grid cell (SHOP_CURSOR_*)
     */
    u8 actionState;

    /**
     * state-machine byte (per-kind); a ground drop's LootDropState. Script
     * op 51 (group_set_state1) writes 1 here: for a spawner that is the shutdown
     * request
     */
    u8 behaviorState;

    u8 field_0x64[1]; // per-kind state-flags byte; bit 1 (0x2) marks a fresh
                      // state entry for the engine's animation refresh, set by every
                      // enemy-AI state transition and actor_checkDeath; bits 3/4
                      // read by enemy AI (0x0801b798)

    /**
     * playerIndex of the player that last damaged this actor (kill credit / loot attribution);
     * doubles as the kill-credit code for XP: KILL_CREDIT_INDIRECT / KILL_CREDIT_NONE
     * when the killer was not a player; a ground drop stores the slot of the player who
     * picked it up, a chest the slot of its opener
     */
    u8 lastHitByPlayerIndex;

    u8 field_0x66[3]; // per-kind state-flags byte (bits 1/3 seen across enemy
                      // AI, combat, FX, loot)

    /**
     * ActorCombatFlags (mask view: .p & ACTOR_COMBAT_*). Combat actors; shop setup
     * writes bits 0/3 but nothing reads them there (dead)
     */
    ActorCombatFlags combatFlags;

    /**
     * (group, stream) record link cached at spawn by kinds that fire it on a lifecycle
     * event (enemy death, spawner retirement, shop); other kinds (chest, switch,
     * mission warp, gate panel) re-read their on-disk record through
     * recordGroup/recordIndex instead
     */
    u8 linkGroup;
    u8 linkStream;

    u8 field_0x6c[4];

    /**
     * second (group, stream) record-link cache, fired by the generic
     * behavior-state machine and the proximity trigger; also read by one FX path (0x0800a64c)
     */
    u8 behaviorLinkGroup;
    u8 behaviorLinkStream;

    /**
     * interaction radius in fp16 units, tested by the proximity check of the
     * collision pass; seeded per kind by the constructors (players: 8,
     * 0x28 while attacking)
     */
    u8 interactionRadius;

    u8 field_0x73; // mostly unused

    /**
     * combat attack/recovery cooldown pair (s16); also cleared by the
     * loot constructors and saved across a proximity-trigger align
     */
    u16 field_0x74;
    u16 field_0x76;
    u8 field_0x78[12];

    /**
     * kind-polymorphic span: "view as" - combat for enemies/players/FX,
     * the record types otherwise (actor->as.enemySpawner.children, ...)
     */
    union ActorKindState as;

    Vector2Fp16 coords;

    /**
     * 2-word displacement velocity (16.16 fixed px/frame); written by
     * actor_setVelocityFromDirection, zeroed on KNOCKED_DOWN entry (0x08027434)
     */
    Vector2Fp16 velocity;

    u8 field_0xbc[16]; // enemy-AI scratch, a remembered target/waypoint
                       // position ([8]/[12] fp16) for the pursuit code
                       // (0x0800b968 / 0x0800bd44); reused as spawn-position
                       // scratch
    /**
     * render-space fp16 position (logical coords plus the
     * render-follow glue offset); basis of the trigger-box overlap tests and edge
     * alignment
     */
    Vector2Fp16 renderCoords;
    u8 field_0xd4[8]; // engine scratch

    /**
     * OAM entry template for the sprite: sprite_composeOam copies it, overwrites the
     * position and tile number from renderCoords / tileNumber, and emits one entry per
     * OBJ of the frame. Gameplay code sets the configuration fields (objMode, colors256,
     * mosaic, bgPriority, paletteNumber) on it directly.
     */
    OamEntry oam;

    /**
     * current movement speed, 16.16 fixed px/frame; seeded from EnemyTypeInfo.moveSpeed when
     * the behavior state machine enters a walking state. Combat actors; also written
     * for shop and menu-cursor actors
     */
    u32 moveSpeed;

    /** ActorFlags (mask view: .p & ACTOR_FLAG_*) */
    ActorFlags flags;

    /**
     * gfx byte size of the main sprite's committed frame, the gfxSize the tile allocator
     * frees at tileNumber; an item drop stores its cell area here
     */
    u32 gfxSize;

    /**
     * record type (enum ActorType) of a record-spawned actor (0 = engine-spawned:
     * players, FX). FX actors reuse it as a burst param (combat.c); combat keys
     * per-enemy-kind behavior off it (0x1f/0x23/0x26 in
     * combat_resolvePveAttack)
     */
    u16 recordType;

    /**
     * OBJ VRAM tile number of the main sprite's committed frame gfx (0xffff = none), written
     * into OAM attr2 by sprite_commitGfx
     */
    u16 tileNumber;

    u8 field_0xf4[2];
    u8 field_0xf6[2]; // per-kind bytes (combat/FX)
    Direction facing;

    /** spawn-group record backreference (group/index), set by mission_script_spawnGroup */
    u8 recordGroup;
    u8 recordIndex;

    /**
     * active trigger-box count (slots at triggerBoxes); seeded from the animation
     * descriptor (byte 0xb) on animation set (0x08000de8), set to 1 by kinds that
     * write a hand-built box (mission warp, attach FX)
     */
    u8 triggerBoxCount;

    /**
     * sprite-engine flag byte; bits 3/4 offset the render-follow glue
     * (0x08003540), other bits gfx bookkeeping
     */
    u8 spriteFlags;

    /** sprite cell index (indexes the 0x2c-strided cell table) */
    u8 spriteCellIndex;

    u8 field_0xfe;

    /** Owning actor list identifier (main vs overlay) */
    ActorListId listId;

    u8 field_0x100; // combat/FX flags byte; bit 5 (0x20): cleared by
                    // actor_resetActionState on leaving actionState 0x35, read by the
                    // Legolas-knife melee path; bit 0 tagged as at-most-once by
                    // fx_attachStatusEffect
    u8 field_0x101[3];

    /** Sprite-engine render sub-state; see struct SpriteRender. */
    struct SpriteRender sprite;
};

/**
 * Actor-list descriptor: head is the most recently allocated actor, tail
 * the oldest active one, where actor_tickAllInList starts its oldest-first tick pass;
 * sprite_free advances tail when it frees the tail actor.
 */
typedef struct ActorList
{
    Actor *head;
    Actor *tail;
} ActorList;
/** Prepend actor to the list (new head); does not touch tail. */
void list_pushFront(ActorList *list, Actor *actor);

/**
 * Park the gameplay world for a non-gameplay scene: suppress every main-list actor's tick
 * (ACTOR_FLAG_TICK_SUPPRESSED) and stash the list into g_ActorListParked, leaving
 * g_ActorListMain empty. actor_restoreParkedMainList reverses it.
 */
void actor_parkMainList(void);
void actor_restoreParkedMainList(void);
// OR / clear a flag mask on every actor in g_ActorListMain.
void actor_setFlagsAll(u32 mask);
void actor_clearFlagsAll(u32 mask);

#define ACTOR_POOL_MAIN_CAPACITY 0x100
#define ACTOR_POOL_OVERLAY_CAPACITY 0x28

/** memory_zalloc'd pool storage backing g_ActorListMain / g_ActorListOverlay. */
typedef Actor ActorPoolMain[ACTOR_POOL_MAIN_CAPACITY];
typedef Actor ActorPoolOverlay[ACTOR_POOL_OVERLAY_CAPACITY];

/** Actor.actionState values with known consumers (most states unmapped). */
enum ActorActionState
{
    ACTOR_STATE_HURT = 0x06,    // set by player_addHp flinch (from states 1/2/5)
    ACTOR_STATE_STUNNED = 0x1a, // victim takes double melee damage; set by Immobilize hits
    ACTOR_STATE_KNOCKBACK =
        0x25, // victim pushed away at 2.0 px/frame velocity; set by actor_applyKnockback
    ACTOR_STATE_FX_ATTACHED = 0x2b,
    ACTOR_STATE_SHIELD_SPENT = 0x40, // shield bubble leaves FX_ATTACHED for this on a block
    ACTOR_STATE_KNOCKED_DOWN = 0x31, // player knockdown from an enemy crit / heavy attack
    // attacker states with special melee damage: 0x33/0x34/0x35/0x3a/0x59 add chargeDamage,
    // 0x3b replaces the roll with chargeDamage, 0x58 replaces it with the active skill-3 value.
    ACTOR_STATE_ATK_UNKNOWN_33 = 0x33,
    ACTOR_STATE_ATK_UNKNOWN_34 = 0x34,
    ACTOR_STATE_ATK_UNKNOWN_35 = 0x35,
    ACTOR_STATE_ATK_UNKNOWN_3A = 0x3a,
    ACTOR_STATE_ATK_CHARGE_ONLY = 0x3b,
    ACTOR_STATE_ATK_ACTIVE_SKILL3 = 0x58,
    ACTOR_STATE_ATK_UNKNOWN_59 = 0x59,
};

/**
 * Iterate the actors within radius (16.16 fixed world units) of center. Pass 0 as cursor to start,
 * then the previous return each call; returns 0 when the sweep is done.
 */
Actor *actor_iterateInRadius(Actor *center, Actor *cursor, s32 radius);

/**
 * True when an actor is in a state that a burst/AoE may stun or interrupt - i.e. not already in a
 * protected actionState (0x04/0x11/0x13/0x15/0x1b), not behaviorState 0x2f, and not flagged 0x2.
 */
bool actor_isStunnable(Actor *actor);

// Actor-behavior helpers consumed by the melee-combat path.

/**
 * Write the current animation frame's x/y offsets (y = height above feet, 0.5-unit steps)
 * into `out` and return the frame's gfx id.
 */
u16 actor_getAnimationFrame(Actor *actor, u32 *out);
/** Direction (0..7) from actor `from` toward actor `to`. */
s32 actor_directionToward(Actor *from, Actor *to);

/** Get the distance between two actors. */
static inline s32 actor_distance(Actor *a, Actor *b)
{
    Vector2Int posA;
    Vector2Int posB;
    vector2_fromFp16(&a->coords, &posA);
    vector2_fromFp16(&b->coords, &posB);
    return vector2_distance(posA, posB);
}
/**
 * Set a 2-word velocity vector (Actor.velocity) from DirectionMoveVectors[direction] * speed;
 * flags bit 24 (1 << 24) selects DirectionMoveVectorsSteep instead.
 */
void actor_setVelocityFromDirection(Vector2Fp16 *velocityOut, u32 flags, s32 direction, s32 speed);
/** Apply knockback to `victim` away from `attacker` for `duration` frames. */
void actor_applyKnockback(Actor *victim, Actor *attacker, u32 duration);
/**
 * Clear the current actionState's leftovers (sprite slots, state flags, per-player busy bit)
 * before a new state is entered.
 */
void actor_resetActionState(Actor *actor);

/**
 * Generic behavior-state fallback for record-spawned actors (the behaviorState
 * machine; fires the behaviorLinkGroup/behaviorLinkStream record link for scripted
 * states).
 */
void actor_runBehaviorState(Actor *actor);

/**
 * Per-recordType defaults (flags bits, interactionRadius, ...) applied by the
 * ActorMissionConstructors after the base flags setup.
 */
void actor_applyTypeDefaults(Actor *actor);

// Touch reactions fired at the tail of the trigger callbacks.
void actor_resolveTouch(Actor *self, Actor *touching);
void actor_staggerOnTouch(Actor *self, Actor *touching);

/**
 * Compute a position at the edge of the touching actor's sprite cell (from both
 * actors' render coords and animation cell bounds).
 */
void actor_alignTouching(Actor *self, Actor *touching);

/**
 * Proximity trigger: when a player comes within interactionRadius, freeze, fire the
 * behavior link once (latching behaviorState 6), then align the player.
 */
void actor_proximityLinkTrigger(Actor *self, Actor *player);
