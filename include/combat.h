#pragma once

#include "actor.h"
#include "player.h"
#include "types.h"

// Flat damage bonuses added on a landed melee hit.
#define FIREBURST_BONUS_DAMAGE 4
#define VENOMOUS_BONUS_DAMAGE 5

/** combat_getFlatDamageBonus: STAT_LOW_HP_DAMAGE applies at or below this share of max HP. */
#define LOW_HP_BONUS_THRESHOLD_PERCENT 25

// combat_getFlatDamageBonus: +1 damage per STRENGTH_DAMAGE_STEP strength above
// STRENGTH_DAMAGE_BASE.
#define STRENGTH_DAMAGE_BASE 20
#define STRENGTH_DAMAGE_STEP 4

// Chance in percent to proc Sunburst/Nightburst
#define BURST_PROC_PERCENT 10
// Sunburst/Nightburst radius
#define BURST_RADIUS 0x28
#define SUNBURST_STUN_FRAMES 120
#define NIGHTBURST_DAMAGE 7

/**
 * Base actionStateTimer for ACTOR_STATE_HURT (hit flinch); nightburst stacks STAT_FOE_STUN_TIME
 * on top.
 */
#define HURT_STATE_FRAMES 5

#define FRAGILE_PROC_PERCENT 10

// STAT_IMMOBILIZE proc chance, and the actionStateTimer / status-FX duration a proc stuns
// the victim for.
#define IMMOBILIZE_PROC_PERCENT 25
#define IMMOBILIZE_STUN_FRAMES 90

/** An evasive unit (ACTOR_COMBAT_FLAG_EVASIVE) evades a melee hit this percent of the time. */
#define EVASION_CHANCE_PERCENT 15

/** combat_rollAvoidance clamps each avoidance proc chance to this. */
#define AVOIDANCE_CAP_PERCENT 75

// combat_rollAccuracy clamps the hit chance to this window: a hit is never certain and never
// impossible.
#define MIN_HIT_CHANCE_PERCENT 10
#define MAX_HIT_CHANCE_PERCENT 95

/**
 * combat_applyNonPlayerHitOnEnemy crit chance: a d100 roll on this or lower crits and deals the
 * full maxDamage; otherwise the hit is reduced to a uniform roll in [maxDamage/2, maxDamage]. The
 * non-player analogue of a player crit - no bonus damage, just the max.
 */
#define NONPLAYER_CRIT_CHANCE_PERCENT 10

/**
 * Venom status duration applied to the victim on a Venomous hit, in frames, before the
 * victim's STAT_POISON_RESIST_PERCENT percent cut.
 */
#define VENOM_DURATION_FRAMES 90

/**
 * Kill-credit codes stored in Actor.lastHitByPlayerIndex when the last damage did NOT come from a
 * player's own attack (values 0/1 are otherwise the attacker's playerIndex). combat_awardKillXp
 * awards full XP for 0, half for INDIRECT (full when GAME_FLAG_COOP_ACTIVE - note a real
 * playerIndex 1 also reads as INDIRECT then, quirky), none for NONE.
 */
enum KillCredit
{
    KILL_CREDIT_INDIRECT = 1, // player-attributed indirect kill (ally / fired siege object)
    KILL_CREDIT_NONE = 2,     // enemy friendly fire / unattributed kill
};

/**
 * Actor.behaviorState of a combat-active ACTOR_TYPE_NPC actor (fighting companion or a
 * fired siege object): its kills credit the player (KILL_CREDIT_INDIRECT,
 * combat_resolveNonPlayerAttackOnEnemy) and enemy hits can knock it down. Partially inferred.
 */
#define NPC_BEHAVIOR_COMBAT_ACTIVE 0x2e

/**
 * combat_resolveNonPlayerAttackOnEnemy floors the caller-supplied max damage to this
 * before rolling.
 */
#define NONPLAYER_MIN_MAX_DAMAGE 12

bool combat_isBackstab(Actor *attacker, Actor *victim);
bool combat_rollAccuracy(u16 attackerAccuracy, u16 victimDefense);
bool combat_rollPercentage(u16 chance);
bool combat_rollAvoidance(u16 statIndex, s32 playerIndex);
bool combat_rollVictimEvade(Actor *victim);
u16 combat_getEnemyTypeDamageBonus(s32 playerIndex, u8 enemyType);
u16 combat_getPlayerMaxDamage(s32 playerIndex);
u16 combat_getPlayerMinDamage(s32 playerIndex);

bool combat_applyNonPlayerHitOnEnemy(Actor *victim, s16 maxDamage);
void combat_killEnemyOutright(Actor *victim);
bool combat_resolveEnemyAttackOnEnemy(Actor *attacker, Actor *victim);
u16 combat_handleEnemyAttackOnPlayerStatusArmorSfx(Actor *attacker, Actor *victim, u32 *outHitSfx,
                                                   s16 rawDamage);
u16 combat_absorbDamageIntoShield(Actor *attacker, u16 initialDamage, s32 playerIndex);

void combat_fragileOnKill(s32 playerIndex);

void combat_sunburstOnKill(Actor *procTarget);
void combat_nightburstOnKill(s32 playerIndex, Actor *procTarget);

bool combat_resolveNonPlayerAttackOnEnemy(Actor *attacker, Actor *victim, u16 accuracy,
                                          u16 maxDamage);
bool combat_resolveEnemyAttackOnPlayer(Actor *attacker, Actor *victim);
bool combat_resolveEnemyAttackOnNpc(Actor *attacker, Actor *victim);
bool combat_resolveObjectAttackOnPlayer(Actor *victim, u16 maxDamage);

bool combat_applyHazardHitOnPlayer(Actor *attacker, Actor *victim, u16 maxDamage);
bool combat_applyHazardHitOnEnemy(Actor *victim, u16 maxDamage);
bool combat_applyPveRangedHit(Actor *attacker, Actor *victim, u16 baseDamage);

s32 combat_resolvePveAttack(Actor *attacker, Actor *victim);
bool combat_resolvePvpAttack(Actor *attacker, Actor *victim);
void combat_resolvePlayerAttackOnActor(Actor *attacker, Actor *victim);
s32 combat_computePveDamage(Actor *attacker, Actor *victim, bool crit);
s32 combat_computePvpDamage(Actor *attacker, Actor *victim, bool crit);
s32 combat_handlePvpStatusArmorSfx(Actor *attacker, Actor *victim, s16 rawDamage);
s32 combat_getWeaponDamage(s32 playerIndex);
s32 combat_getFlatDamageBonus(s32 playerIndex);

/**
 * Try to absorb `damage` into the victim's active shield-bubble actor (DAT_03005500); returns
 * TRUE when the bubble absorbed it (HP remaining), FALSE with no bubble.
 */
bool combat_shieldAbsorbsHit(Actor *victim, s32 damage);
