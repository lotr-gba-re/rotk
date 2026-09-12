#pragma once

#include "types.h"

// The player-controlled hero actor (recordType 0). Its Player slot is as.combat.playerIndex; the
// trigger callbacks are actor_resolveTouch and combat_resolvePlayerAttackOnActor.

/** Spawn the player's actor at the mission spawn point and rebuild the player from its slot. */
Actor *actor_player_create(u8 playerIndex);

/**
 * Set the actor's tick callback, animation bank, playerIndex, and combat defaults right after
 * allocation.
 */
void actor_player_initState(Actor *actor, u8 playerIndex);

/**
 * Per-frame player tick: d-pad movement, attack combos, active skill casts, death and revive,
 * and the hero-specific action states. Falls through into actor_player_tickTail.
 */
void actor_player_tick(Actor *actor);

/**
 * Shared tail of actor_player_tick: apply the new action state and facing to the sprite, then the
 * timed spirit regen, corruption decay, HP regen (STAT_HP_REGEN, Iron Will), and STAT_MEDITATIVE.
 */
void actor_player_tickTail(void);

/** Tick the poison (-2 HP / 32 frames) and fear (-3 spirit / 16 frames) status timers. */
void actor_player_tickPoisonFear(Actor *actor);

/**
 * Re-attach the weapon (slot 0) and offhand (slot 1) hand sprites for the equipped item types of
 * the hero, and refresh their affix glow FX; clears PLAYER_COMBAT_FLAG_HAND_SPRITES_DIRTY.
 */
void actor_player_updateHandSprites(Actor *actor);

/**
 * Point the actor at the hero's bank in HeroMoveAnimationBanks (0), HeroMeleeAnimationBanks (1),
 * or HeroCastAnimationBanks (2).
 */
void actor_player_selectAnimationBank(Actor *actor, u32 heroId, u8 bankSet);
