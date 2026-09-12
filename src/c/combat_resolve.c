#include "activeSkill.h"
#include "combat.h"
#include "enemy.h"
#include "fx.h"
#include "item.h"
#include "match_hacks.h"
#include "math.h"
#include "rng.h"
#include "sfx.h"
#include "variables.h"

/**
 * Apply the stun active skill to an already-stunned victim: flag the stun bit on field_0x64, run
 * the stun active skill for its leveled duration, and attach the status-effect FX. The callers
 * store ACTOR_STATE_STUNNED first; the compiler cross-jump-merges the two inlined call sites
 * into the ROM's shared stun tail (0x0800f554).
 */
static inline void applyStun(Actor *attacker, Actor *victim, u32 playerIndex, u8 fxArg5)
{
    victim->field_0x64[0] |= 2;
    {
        s32 duration = activeSkill_getLeveledValue(playerIndex, ACTIVE_SKILL_UNKNOWN_3, 1,
                                                   PLAYER(playerIndex).activeSkillLevels[3]);
        s32 stunDuration = (s16)duration;
        victim->actionStateTimer = stunDuration;
        fx_attachStatusEffect(attacker, victim, (u16)stunDuration, 0xc, 8, fxArg5, 8);
    }
}

/**
 * Apply a normal flinch to the victim: seed the hit-stun timer from the enemy type, enter the
 * hurt state, and set the hurt-state timer.
 */
static inline void applyFlinch(Actor *victim, u32 playerIndex, u32 unitType)
{
    victim->hitStunTimer = EnemyTypes[unitType].hitStun;
    victim->actionState = ACTOR_STATE_HURT;
    victim->field_0x64[0] |= 2;
    victim->actionStateTimer = PLAYER_STAT(playerIndex, STAT_FOE_STUN_TIME) + 5;
}

/**
 * Resolve a player's melee hit on an enemy: backstab, accuracy, victim evade, shield-bubble
 * absorb, crit roll (forced while PLAYER_COMBAT_FLAG_WARRIORS_POOL is set), damage application, the
 * instakill/stun/knockback state transitions, and on-kill/lifesteal/spirit/burst procs.
 *
 * @return TRUE on a connected hit, FALSE on miss, evade, or an invulnerable victim
 *
 * @romaddress 0x0800f168
 */
s32 combat_resolvePveAttack(Actor *attacker, Actor *victim)
{
    FxEmitter *emitter = NULL;
    u32 playerIndex = attacker->as.combat.playerIndex;
    u32 hitSfx;
    bool crit = FALSE;
    u32 unitType = victim->unitType;
    bool hitPossible = TRUE;
    u32 damage;
    u8 *unitTypePtr;
    Vector2Fp16 out;
    // ACTOR_FLAG_NO_FLINCH bits; the two stun arms reuse it as applyStun's fx argument,
    // where it is 0
    u32 noFlinchFlags;
    Vector2Int diff;

    if ((u8)(unitType - 0x1d) < 2 && victim->actionState != 0x0e && victim->actionState != 0x8)
    {
        // These enemies can only be hit on backstab. Probably shield carriers.
        hitPossible = combat_isBackstab(attacker, victim);
    }

    if (victim->flags.p & ACTOR_FLAG_INVULNERABLE)
    {
        return 0;
    }
    // assigned here: the address pseudo's birth position decides its spill slot
    unitTypePtr = &victim->unitType;

    if (hitPossible)
    {
        u16 attackerAccuracy = PLAYER_STAT(playerIndex, STAT_ACCURACY);
        u16 victimDefense = combat_getEnemyDefense(*unitTypePtr);
        if (combat_rollAccuracy(attackerAccuracy, victimDefense))
        {
            if (combat_rollVictimEvade(victim))
            {
                return 0;
            }

            // Gandalf shield-bubble absorbs the whole hit for one enemy record type (recordType ==
            // 0x26)
            if (victim->recordType == 0x26)
            {
                damage = (u16)combat_computePveDamage(attacker, victim, crit);
                if (combat_shieldAbsorbsHit(victim, damage))
                {
                    return 0;
                }
            }

            // hit: spawn the basic hit FX on the victim (unless it is a no-FX state)
            if (victim->recordType != 0x1f && victim->recordType != 0x23)
            {
                emitter = fx_allocEmitter(4, 0, 1);
                emitter->target = victim;
                fx_setEmitterPosition(emitter, victim, &out);
                emitter->field_0x36 = 0;
                emitter->field_0x37 = victim->facing;
                emitter->field_0x2e |= 0x6082;
            }

            // crit roll; PLAYER_COMBAT_FLAG_WARRIORS_POOL forces a crit
            if (PLAYER(playerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_WARRIORS_POOL)
            {
                crit = TRUE;
            }
            else
            {
                crit = combat_rollPercentage(PLAYER_STAT(playerIndex, STAT_CRIT_CHANCE_PERCENT));
            }
            if (crit || (PLAYER(playerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_FORCE_CRIT_UNKNOWN))
            {
                if (victim->recordType != 0x1f && victim->recordType != 0x23)
                {
                    emitter->field_0x3a = 0xc;
                    emitter->field_0x38 = 3;
                    emitter->field_0x30 = 0xf;
                }
                sfx_play(EnemySfxSets[unitType].critSfx);
                hitSfx = SFX_COMBAT_CRIT_HIT;
            }
            else
            {
                if (victim->recordType != 0x1f && victim->recordType != 0x23)
                {
                    emitter->field_0x3a = 4;
                    emitter->field_0x38 = 1;
                    emitter->field_0x30 = 5;
                }
                hitSfx = (PLAYER(playerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_DAMAGE_TYPE)
                             ? SFX_COMBAT_HIT_SLASH
                             : SFX_COMBAT_HIT_IMPALE;
                sfx_play(EnemySfxSets[*unitTypePtr].hitSfx);
            }

            damage = (u16)combat_computePveDamage(attacker, victim, crit);

            {
                Player *playersBase = g_PlayerGlobals.players;
                Player *player = &playersBase[playerIndex];
                {
                    u8 heroId = player->heroId;

                    // Aragorn instakill proc (STAT_INSTAKILL_CHANCE_PERCENT)
                    if (heroId == HERO_ID_ARAGORN && !(victim->flags.p & ACTOR_FLAG_NO_FLINCH))
                    {
                        if (player->stats[STAT_INSTAKILL_CHANCE_PERCENT] > 0 &&
                            rng_rollRange(0, 100) <= player->stats[STAT_INSTAKILL_CHANCE_PERCENT])
                        {
                            damage = victim->currentHp;
                            emitter->field_0x38 = 0xa;
                            emitter->field_0x3a = 0xc;
                            player->combatFlags.p |= PLAYER_COMBAT_FLAG_XP_DOUBLE;
                            {
                                Actor *fx = fx_spawnOnActor(victim, 0, 3, 0);
                                fx->recordType = FX_BURST_UNK_PARAM;
                                // fx_tickAttachment returns a value that the tick dispatcher
                                // discards
                                fx->tickCallback = (ActorCallback)fx_tickAttachment;
                            }
                            sfx_play(SFX_COMBAT_INSTAKILL);
                        }
                    }

                    victim->currentHp -= damage;
                    victim->lastHitByPlayerIndex = attacker->as.combat.playerIndex;

                    noFlinchFlags = victim->flags.p;
                    noFlinchFlags &= ACTOR_FLAG_NO_FLINCH;
                }

                if (noFlinchFlags == 0)
                {
                    if (attacker->actionState == 0x3b && victim->currentHp > 0 &&
                        actor_isStunnable(victim))
                    {
                        victim->actionState = ACTOR_STATE_STUNNED;
                        applyStun(attacker, victim, playerIndex, noFlinchFlags);
                    }
                    else if (PLAYER_STAT(playerIndex, STAT_KNOCKBACK) != 0 && victim->currentHp > 0)
                    {
                        // knockback proc
                        actor_applyKnockback(victim, attacker,
                                             (u8)PLAYER_STAT(playerIndex, STAT_KNOCKBACK));
                        victim->field_0x64[0] |= 2;
                    }
                    else if (PLAYER_STAT(playerIndex, STAT_IMMOBILIZE) != 0 &&
                             victim->currentHp > 0 && actor_isStunnable(victim))
                    {
                        // immobilize proc
                        if (rng_rollRange(1, 100) < IMMOBILIZE_PROC_PERCENT)
                        {
                            victim->actionState = ACTOR_STATE_STUNNED;
                            victim->field_0x64[0] |= 2;
                            victim->actionStateTimer = IMMOBILIZE_STUN_FRAMES;
                            fx_attachStatusEffect(attacker, victim, IMMOBILIZE_STUN_FRAMES, 0xc, 8,
                                                  0, 8);
                        }
                    }
                    else if (attacker->actionState == 0x58 && actor_isStunnable(victim))
                    {
                        {
                            // separate address statement: the zeroing must land inside the
                            // store expansion
                            u8 *actionAddr = &victim->actionState;
                            noFlinchFlags = 0; // applyStun's fx argument
                            *actionAddr = ACTOR_STATE_STUNNED;
                        }
                        applyStun(attacker, victim, playerIndex, noFlinchFlags);
                    }
                    // knockback-via-state for one creature family
                    else if (unitType - 10 <= 7 && victim->behaviorState == 0x14 &&
                             (crit || (PLAYER(playerIndex).combatFlags.p &
                                       PLAYER_COMBAT_FLAG_FORCE_CRIT_UNKNOWN)))
                    {
                        victim->actionState = ACTOR_STATE_KNOCKED_DOWN;
                        victim->field_0x64[0] |= 2;
                        {
                            u8 direction = (u8)actor_direction8Toward(attacker, victim);
                            actor_setVelocityFromDirection8(&victim->velocity, victim->flags.p,
                                                            direction, victim->moveSpeed);
                        }
                    }
                    else
                    {
                        // normal flinch, once the hitstun ran out or on the killing blow
                        if (victim->hitStunTimer <= 0 || victim->currentHp <= 0)
                        {
                            applyFlinch(victim, playerIndex, unitType);
                        }
                    }
                }
                else if (unitType != 0x3c && unitType != 0x50 && unitType != 0x42)
                {
                    if (victim->hitStunTimer <= 0 || victim->currentHp <= 0)
                    {
                        applyFlinch(victim, playerIndex, unitType);
                    }
                }
            }

            if (victim->currentHp < 1)
            {
                if (PLAYER_STAT(playerIndex, STAT_HP_PERCENT_PER_KILL) != 0)
                {
                    u32 heal = math_percentOf(
                        damage, (u16)PLAYER_STAT(playerIndex, STAT_HP_PERCENT_PER_KILL));
                    if (heal == 0)
                    {
                        heal = 1;
                    }
                    player_addHp(playerIndex, (s16)heal, FALSE);
                }
                if (PLAYER_STAT(playerIndex, STAT_HP_PER_KILL) != 0)
                {
                    player_addHp(playerIndex, PLAYER_STAT(playerIndex, STAT_HP_PER_KILL), FALSE);
                }
                if (PLAYER_STAT(playerIndex, STAT_SPIRIT_PER_KILL) != 0)
                {
                    player_addSpirit(playerIndex, PLAYER_STAT(playerIndex, STAT_SPIRIT_PER_KILL));
                }
                if (PLAYER_STAT(playerIndex, STAT_FRAGILE) != 0)
                {
                    combat_fragileOnKill(playerIndex);
                }
                if (PLAYER_STAT(playerIndex, STAT_SUNBURST) != 0)
                {
                    combat_sunburstOnKill(victim);
                }
                if (PLAYER_STAT(playerIndex, STAT_NIGHTBURST) != 0)
                {
                    combat_nightburstOnKill(playerIndex, victim);
                }
            }

            if (PLAYER_STAT(playerIndex, STAT_SPIRIT_PER_HIT) > 0)
            {
                player_addSpirit(playerIndex, PLAYER_STAT(playerIndex, STAT_SPIRIT_PER_HIT));
            }
            if (victim->recordType != 0x1f && victim->recordType != 0x23)
            {
                fx_setEmitterPosition(emitter, victim, &out);
                emitter->field_0x28 = 1;
                emitter->field_0x2a = 8;
                emitter->field_0x36 = 1;
                emitter->field_0x37 = attacker->facing;
                emitter->field_0x2e |= 0x6082;
            }
            sfx_play(hitSfx);
            return 1;
        }
    }

    sfx_play(EnemySfxSets[*unitTypePtr].missSfx);
    if (victim->recordType == 0x1f || victim->recordType == 0x23)
    {
        return 0;
    }
    emitter = fx_allocEmitter(2, 0, 0);
    emitter->target = victim;
    fx_setEmitterPosition(emitter, victim, &out);
    emitter->field_0x28 = 1;
    emitter->field_0x3a = 1;
    emitter->field_0x38 = 2;
    emitter->field_0x2a = 4;
    emitter->field_0x30 = 5;
    emitter->field_0x36 = 3;
    emitter->field_0x20 = 0x10000;
    {
        diff.x = victim->coords.x - attacker->coords.x;
        diff.y = victim->coords.y - attacker->coords.y;
        emitter->field_0x37 = vector2_direction8(diff);
    }
    emitter->field_0x2e |= 0x6086;
    return 0;
}

/**
 * Resolve a player-vs-player defended hit (Smeagol backstab / Grueling-difficulty friendly fire):
 * accuracy vs defense, victim avoidance rolls (parry/dodge/riposte, Legolas, shield block),
 * attacker Thornmail backlash, crit, damage, shield-bubble absorb, knockback and SFX.
 * Returns true on a connected hit, false on miss/parry/invulnerable.
 *
 * @romaddress 0x0800f8bc
 */
bool combat_resolvePvpAttack(Actor *attacker, Actor *victim)
{
    u32 attackerIndex = attacker->as.combat.playerIndex;
    u32 playerIndex = victim->as.combat.playerIndex;
    FxEmitter *emitter;
    Vector2Fp16 out;
    Vector2Int diff;

    if (victim->flags.p & ACTOR_FLAG_INVULNERABLE)
    {
        return FALSE;
    }

    if (combat_rollAccuracy((u16)PLAYER_STAT(attackerIndex, STAT_ACCURACY),
                            (u16)PLAYER_STAT(playerIndex, STAT_DEFENSE)))
    {
        if (combat_rollAvoidance(STAT_PARRY_PERCENT, playerIndex))
        {
            return FALSE;
        }
        if (combat_rollAvoidance(STAT_DODGE_PERCENT, playerIndex))
        {
            return FALSE;
        }

        if (PLAYER(attackerIndex).heroId == HERO_ID_LEGOLAS &&
            combat_rollAvoidance(STAT_ARROW_PARRY_PERCENT, playerIndex))
        {
            Vector2Fp16 out2;
            Vector2Int diff2;

            emitter = fx_allocEmitter(2, 0, 0);
            if (emitter == NULL)
            {
                return FALSE;
            }
            emitter->target = victim;
            fx_setEmitterPosition(emitter, victim, &out2);
            emitter->field_0x28 = 1;
            emitter->field_0x3a = 1;
            emitter->field_0x38 = 2;
            emitter->field_0x2a = 4;
            emitter->field_0x30 = 5;
            emitter->field_0x36 = 3;
            emitter->field_0x20 = 0x10000;
            diff2.x = victim->coords.x - attacker->coords.x;
            diff2.y = victim->coords.y - attacker->coords.y;
            emitter->field_0x37 = vector2_direction8(diff2);
            emitter->field_0x2e |= 0x6086;
            return FALSE;
        }

        {
            Player *player;
            u16 thorns;
            s32 critChance;
            bool crit;
            s32 damage;
            s32 hitSfx;

            if (PLAYER(playerIndex).inventory.slots.offhand.d.itemType == ITEM_TYPE_SHIELD &&
                combat_rollAvoidance(STAT_BLOCK_PERCENT, playerIndex))
            {
                return FALSE;
            }
            if (combat_rollAvoidance(STAT_RIPOSTE_PERCENT, playerIndex))
            {
                combat_resolvePvpAttack(victim, attacker);
            }

            player = &PLAYER(playerIndex);
            thorns = (u16)player->stats[STAT_THORNS_DAMAGE];
            if (player->stats[STAT_THORNS_DAMAGE] > 0)
            {
                s16 *attackerHp = &attacker->currentHp;
                *attackerHp -= thorns;
            }

            emitter = fx_allocEmitter(4, 0, 1);
            emitter->target = victim;
            fx_setEmitterPosition(emitter, victim, &out);
            emitter->field_0x36 = 0;
            emitter->field_0x37 = victim->facing;
            emitter->field_0x2e |= 0x6082;

            if (player->stats[STAT_CURSED] != 0)
            {
                critChance = PLAYER_STAT(attackerIndex, STAT_CRIT_CHANCE_PERCENT) << 1;
            }
            else
            {
                critChance = PLAYER_STAT(attackerIndex, STAT_CRIT_CHANCE_PERCENT);
            }
            if (PLAYER(attackerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_WARRIORS_POOL)
            {
                crit = TRUE;
            }
            else
            {
                crit = combat_rollPercentage((u16)critChance);
            }

            // return unused; called for its burst FX and sfx side effects
            combat_computePveDamage(attacker, victim, crit);

            if (crit ||
                (PLAYER(attackerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_FORCE_CRIT_UNKNOWN) != 0)
            {
                emitter->field_0x3a = 0xc;
                emitter->field_0x38 = 3;
                emitter->field_0x30 = 0xf;
                hitSfx = SFX_COMBAT_CRIT_HIT;
            }
            else
            {
                emitter->field_0x3a = 4;
                emitter->field_0x38 = 1;
                emitter->field_0x30 = 5;
                hitSfx = (PLAYER(attackerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_DAMAGE_TYPE)
                             ? SFX_COMBAT_HIT_SLASH
                             : SFX_COMBAT_HIT_IMPALE;
            }

            damage = combat_absorbDamageIntoShield(
                victim, (u16)combat_computePvpDamage(attacker, victim, crit), playerIndex);
            if (damage > 0)
            {
                MATCH_BARRIER(damage); // HACK: keep the dead `if (damage < 0) damage = 5;` clamp
                if (damage < 0)
                {
                    damage = 5;
                }
                if (crit || (PLAYER(attackerIndex).combatFlags.p &
                             PLAYER_COMBAT_FLAG_FORCE_CRIT_UNKNOWN) != 0)
                {
                    if (PLAYER(attackerIndex).heroId != HERO_ID_LEGOLAS)
                    {
                        actor_resetActionState(victim);
                        victim->actionState = ACTOR_STATE_KNOCKED_DOWN;
                        victim->field_0x64[0] |= 2;
                        {
                            u8 direction = (u8)actor_direction8Toward(attacker, victim);
                            Vector2Fp16 *velocity = &victim->velocity;
                            actor_setVelocityFromDirection8(velocity, victim->flags.p, direction,
                                                            victim->moveSpeed);
                        }
                    }
                    player_subtractHp(playerIndex, damage, FALSE);
                    hitSfx = SFX_COMBAT_KNOCKDOWN;
                }
                else
                {
                    player_subtractHp(playerIndex, damage, TRUE);
                }
                sfx_play(hitSfx);
            }
            return TRUE;
        }
    }
    else
    {
        sfx_play(SFX_COMBAT_EVADE);
        emitter = fx_allocEmitter(2, 0, 0);
        emitter->target = victim;
        fx_setEmitterPosition(emitter, victim, &out);
        emitter->field_0x28 = 1;
        emitter->field_0x3a = 1;
        emitter->field_0x38 = 2;
        emitter->field_0x2a = 4;
        emitter->field_0x30 = 5;
        emitter->field_0x36 = 3;
        emitter->field_0x20 = 0x10000;
        diff.x = victim->coords.x - attacker->coords.x;
        diff.y = victim->coords.y - attacker->coords.y;
        emitter->field_0x37 = vector2_direction8(diff);
        emitter->field_0x2e |= 0x6086;
        return FALSE;
    }
}

/**
 * Resolve an enemy's attack on a player: enemy accuracy vs the player's defense, the
 * avoidance rolls (parry, arrow-parry/dodge, shield block, riposte), Thornmail backlash,
 * the enemy crit roll, armor and status via combat_handleEnemyAttackOnPlayerStatusArmorSfx,
 * shield-bubble absorb, knockdown vs flinch, and the on-hit bubble procs.
 *
 * @return TRUE on a connected hit, FALSE on miss/avoid/invulnerable
 *
 * @romaddress 0x0800fce0
 */
bool combat_resolveEnemyAttackOnPlayer(Actor *attacker, Actor *victim)
{
    u32 playerIndex = victim->as.combat.playerIndex;
    u32 unitType = attacker->unitType;
    u16 accuracy;
    Vector2Fp16 out;
    u32 hitSfx;
    bool crit;
    s32 critArmor = 0;
    Vector2Int diff;
    s32 maxDamage;
    // crit-chance percent, then reused for the knockdown roll; splitting it into two
    // locals allocates different registers than the ROM
    s32 percent;
    s32 damage;
    FxEmitter *emitter;

    if (victim->flags.p & ACTOR_FLAG_INVULNERABLE)
    {
        return FALSE;
    }

    accuracy = combat_getEnemyAccuracy(unitType);
    if (combat_rollAccuracy(accuracy, (u16)PLAYER_STAT(playerIndex, STAT_DEFENSE)))
    {
        if (combat_rollAvoidance(STAT_PARRY_PERCENT, playerIndex))
        {
            return FALSE;
        }

        if (EnemyTypes[unitType].attackFlags.p & ENEMY_ATTACK_RANGED)
        {
            if (combat_rollAvoidance(STAT_ARROW_PARRY_PERCENT, playerIndex))
            {
                // arrow parried: spawn the whiff FX on the player
                emitter = fx_allocEmitter(3, 0, 6);
                if (emitter != NULL)
                {
                    emitter->target = victim;
                    fx_setEmitterPosition(emitter, victim, &out);
                    emitter->field_0x28 = 1;
                    emitter->field_0x3a = 2;
                    emitter->field_0x38 = 2;
                    emitter->field_0x2a = 4;
                    emitter->field_0x30 = 5;
                    emitter->field_0x36 = 3;
                    emitter->field_0x20 = 0x10000;
                    diff.x = victim->coords.x - attacker->coords.x;
                    diff.y = victim->coords.y - attacker->coords.y;
                    emitter->field_0x37 = vector2_direction8(diff);
                    emitter->field_0x2e |= 0x6086;
                }
                return FALSE;
            }
            if (combat_rollAvoidance(STAT_DODGE_PERCENT, playerIndex))
            {
                sfx_play(SFX_COMBAT_PLAYER_DODGE_RANGED);
                return FALSE;
            }
        }
        else
        {
            if (combat_rollAvoidance(STAT_DODGE_PERCENT, playerIndex))
            {
                sfx_play(SFX_COMBAT_PLAYER_DODGE);
                return FALSE;
            }
        }

        if (PLAYER(playerIndex).inventory.slots.offhand.d.itemType == ITEM_TYPE_SHIELD &&
            combat_rollAvoidance(STAT_BLOCK_PERCENT, playerIndex))
        {
            return FALSE;
        }

        if (combat_rollAvoidance(STAT_RIPOSTE_PERCENT, playerIndex))
        {
            combat_resolvePveAttack(victim, attacker);
        }

        if (PLAYER_STAT(playerIndex, STAT_THORNS_DAMAGE) > 0)
        {
            attacker->currentHp -= PLAYER_STAT(playerIndex, STAT_THORNS_DAMAGE);
        }

        // crit roll: base 10%, 20% vs a Cursed player
        if (PLAYER_STAT(playerIndex, STAT_CURSED) != 0)
        {
            percent = 20;
        }
        else
        {
            percent = 10;
        }
        crit = combat_rollPercentage((u16)percent);

        maxDamage = combat_getEnemyMaxDamage(unitType);
        if (crit)
        {
            critArmor = PLAYER_STAT(playerIndex, STAT_CRITICAL_ARMOR);
            maxDamage <<= 1;
        }
        else
        {
            maxDamage = rng_rollRange(maxDamage / 2, maxDamage);
        }

        if (PLAYER_STAT(playerIndex, STAT_CURSED) != 0 &&
            EnemyTypes[unitType].category == CREATURE_NAZGUL)
        {
            maxDamage += PLAYER_STAT(playerIndex, STAT_CURSED) * 15;
        }

        critArmor +=
            combat_handleEnemyAttackOnPlayerStatusArmorSfx(attacker, victim, &hitSfx, maxDamage);
        maxDamage = combat_absorbDamageIntoShield(victim, maxDamage, playerIndex);
        if (maxDamage > 0)
        {
            damage = maxDamage - critArmor;
            if (damage < 0)
            {
                damage = 5;
            }
            if (crit || (EnemyTypes[unitType].attackFlags.p & ENEMY_ATTACK_MELEE_IMPALE) != 0)
            {
                percent = rng_rollRange(1, 100);
                if ((EnemyTypes[unitType].attackFlags.p & ENEMY_ATTACK_RANGED) == 0 &&
                    (unitType != 0x44 || percent < 41))
                {
                    actor_resetActionState(victim);
                    victim->actionState = ACTOR_STATE_KNOCKED_DOWN;
                    victim->field_0x64[0] |= 2;
                    {
                        u8 direction = (u8)actor_direction8Toward(attacker, victim);
                        actor_setVelocityFromDirection8(&victim->velocity, victim->flags.p,
                                                        direction, victim->moveSpeed);
                    }
                }
                player_subtractHp(playerIndex, damage, FALSE);
                hitSfx = SFX_COMBAT_KNOCKDOWN;
            }
            else
            {
                player_subtractHp(playerIndex, damage, TRUE);
            }

            // on-hit bubble procs
            if (PLAYER_STAT(playerIndex, STAT_INVULNERABILITY_PROC_PERCENT) > 0 &&
                PLAYER(playerIndex).shieldBubble == NULL &&
                rng_rollRange(1, 100) < PLAYER_STAT(playerIndex, STAT_INVULNERABILITY_PROC_PERCENT))
            {
                player_spawnInvulnerabilityBubble(victim);
            }
            if (PLAYER_STAT(playerIndex, STAT_TRIPLE_DAMAGE_PROC_PERCENT) > 0 &&
                PLAYER(playerIndex).shieldBubble == NULL &&
                rng_rollRange(1, 100) < PLAYER_STAT(playerIndex, STAT_TRIPLE_DAMAGE_PROC_PERCENT))
            {
                player_spawnTripleDamageBubble(victim);
            }
            sfx_play(hitSfx);
        }
        return TRUE;
    }
    return FALSE;
}
/**
 * Resolve an enemy's attack on an ACTOR_TYPE_NPC actor (companion, civilian, ...):
 * fixed defense, a 10% knockdown on a combat-active NPC, hurt reaction and per-kind
 * voice SFX; only NPC_KIND_SOLDIER takes HP damage.
 *
 * @return TRUE on a connected hit, FALSE on miss or a latched (ACTOR_FLAG_5) or
 *         invulnerable NPC
 *
 * @romaddress 0x08010094
 */
bool combat_resolveEnemyAttackOnNpc(Actor *attacker, Actor *victim)
{
    u8 behaviorState = victim->behaviorState;
    u16 accuracy;
    bool hit;
    u32 maxDamage;

    if (behaviorState != 0x20 && behaviorState != 0x25 &&
        behaviorState != NPC_BEHAVIOR_COMBAT_ACTIVE)
    {
        return FALSE;
    }
    if (victim->flags.p & ACTOR_FLAG_INVULNERABLE)
    {
        return FALSE;
    }
    accuracy = combat_getEnemyAccuracy(attacker->unitType);
    hit = combat_rollAccuracy(accuracy, 70);
    if (hit && !(victim->flags.p & ACTOR_FLAG_5))
    {
        if (combat_rollPercentage(10))
        {
            if (victim->behaviorState == NPC_BEHAVIOR_COMBAT_ACTIVE)
            {
                u8 direction;

                victim->actionState = ACTOR_STATE_KNOCKED_DOWN;
                victim->field_0x64[0] |= 2;
                direction = (u8)actor_direction8Toward(attacker, victim);
                actor_setVelocityFromDirection8(&victim->velocity, victim->flags.p, direction,
                                                victim->moveSpeed);
                victim->flags.p |= ACTOR_FLAG_23 | ACTOR_FLAG_5;
                sfx_play(NpcSfxSets[victim->variant].hitSfx);
                return TRUE;
            }
        }
        else if (victim->behaviorState == NPC_BEHAVIOR_COMBAT_ACTIVE)
        {
            victim->flags.p |= ACTOR_FLAG_5;
        }
        if (victim->variant == NPC_KIND_SOLDIER)
        {
            maxDamage = combat_getEnemyMaxDamage(attacker->unitType);
            // the (u16) keeps the low bound's shift off the return's zero-extension
            maxDamage = rng_rollRange((u16)maxDamage / 2, maxDamage);
            victim->currentHp -= maxDamage;
        }
        victim->actionState = ACTOR_STATE_HURT;
        victim->field_0x64[0] |= 2;
        sfx_play(NpcSfxSets[victim->variant].hitSfx);
        return TRUE;
    }
    sfx_play(SFX_COMBAT_EVADE);
    return FALSE;
}

/**
 * Apply a hazard/object hit to a player: roll [maxDamage/2, maxDamage] and subtract
 * STAT_MELEE_ARMOR (STAT_CRITICAL_ARMOR for attacker recordType 0x6e) after the
 * shield-bubble absorb. No accuracy or avoidance rolls.
 *
 * @return TRUE on a connected hit, FALSE on an invulnerable victim
 *
 * @romaddress 0x080101bc
 */
bool combat_applyHazardHitOnPlayer(Actor *attacker, Actor *victim, u16 maxDamage)
{
    u8 playerIndex = victim->as.combat.playerIndex;
    s16 damage;
    s16 armor;
    s16 remaining;

    if (!(victim->flags.p & ACTOR_FLAG_INVULNERABLE))
    {
        damage = rng_rollRange(maxDamage / 2, maxDamage);
        if (attacker->recordType == 0x6e)
        {
            armor = PLAYER_STAT(playerIndex, STAT_CRITICAL_ARMOR);
        }
        else
        {
            armor = PLAYER_STAT(playerIndex, STAT_MELEE_ARMOR);
        }
        remaining = (s16)combat_absorbDamageIntoShield(victim, damage, playerIndex);
        if (remaining > 0)
        {
            damage = remaining - armor;
            if (damage < 0)
            {
                damage = 5;
            }
            player_subtractHp(playerIndex, damage, TRUE);
        }
        sfx_play(SFX_COMBAT_HIT_IMPALE);
        return TRUE;
    }
    return FALSE;
}

/**
 * Resolve an object/hazard hit on a player: fixed accuracy 65 vs the player's defense,
 * shield block roll, damage roll in [maxDamage/2, maxDamage], shield-bubble absorb, then
 * Missile Armor reduction (a hit the armor exceeds leaves 5 damage).
 *
 * @return TRUE on a connected hit, FALSE on miss/block/invulnerable
 *
 * @romaddress 0x08010254
 */
bool combat_resolveObjectAttackOnPlayer(Actor *victim, u16 maxDamage)
{
    u8 playerIndex = victim->as.combat.playerIndex;
    s32 roll;
    s16 missileArmor;
    s16 absorbed;
    s16 damage;

    if (victim->flags.p & ACTOR_FLAG_INVULNERABLE)
    {
        return FALSE;
    }

    if (combat_rollAccuracy(65, (u16)PLAYER_STAT(playerIndex, STAT_DEFENSE)))
    {
        if (PLAYER(playerIndex).inventory.slots.offhand.d.itemType == ITEM_TYPE_SHIELD &&
            combat_rollAvoidance(STAT_BLOCK_PERCENT, playerIndex))
        {
            return FALSE;
        }

        roll = rng_rollRange(maxDamage >> 1, maxDamage);
        missileArmor = PLAYER_STAT(playerIndex, STAT_MISSILE_ARMOR);
        absorbed = combat_absorbDamageIntoShield(victim, (u16)roll, playerIndex);
        if (absorbed > 0)
        {
            damage = absorbed - missileArmor;
            if (damage < 0)
            {
                damage = 5;
            }
            player_subtractHp(playerIndex, damage, TRUE);
        }
        sfx_play(SFX_COMBAT_HIT_MISSILE);
        return TRUE;
    }
    return FALSE;
}

/**
 * Apply a hazard/object hit to an enemy: roll [maxDamage/2, maxDamage] minus slashArmor,
 * with an unconditional flinch and no evade/crit roll, sfx, or kill credit.
 *
 * @return TRUE on a connected hit, FALSE on an invulnerable victim
 *
 * @romaddress 0x08010304
 */
bool combat_applyHazardHitOnEnemy(Actor *victim, u16 maxDamage)
{
    s16 damage;

    if (!(victim->flags.p & ACTOR_FLAG_INVULNERABLE))
    {
        damage = rng_rollRange(maxDamage / 2, maxDamage);
        damage -= EnemyTypes[victim->unitType].slashArmor;
        if (damage < 0)
        {
            damage = 1;
        }
        victim->lastHitByPlayerIndex = KILL_CREDIT_NONE;
        victim->currentHp -= damage;
        victim->actionState = ACTOR_STATE_HURT;
        victim->actionStateTimer = HURT_STATE_FRAMES;
        victim->field_0x64[0] |= 2;
        return TRUE;
    }
    return FALSE;
}

/**
 * Apply a player projectile hit to an enemy with no accuracy roll: baseDamage +
 * STAT_RANGED_DAMAGE (+ STAT_WISDOM for Gandalf) minus armor, with the flinch and
 * the on-kill lifesteal/spirit procs.
 *
 * @return TRUE on a connected hit, FALSE on an invulnerable victim
 *
 * @romaddress 0x08010380
 */
bool combat_applyPveRangedHit(Actor *attacker, Actor *victim, u16 baseDamage)
{
    u8 playerIndex = attacker->as.combat.playerIndex;
    u8 unitType = victim->unitType;
    s16 damage;

    if (!(victim->flags.p & ACTOR_FLAG_INVULNERABLE))
    {
        damage = baseDamage;
        damage += PLAYER_STAT(playerIndex, STAT_RANGED_DAMAGE);
        if (PLAYER(playerIndex).heroId == HERO_ID_GANDALF)
        {
            damage += PLAYER_STAT(playerIndex, STAT_WISDOM);
        }
        damage -= EnemyTypes[unitType].slashArmor;
        if (damage < 0)
        {
            damage = 1;
        }

        // Gandalf shield-bubble absorbs the whole hit for one enemy record type (recordType ==
        // 0x26)
        if (victim->recordType == 0x26 && combat_shieldAbsorbsHit(victim, damage))
        {
            return TRUE;
        }

        victim->currentHp -= damage;
        victim->lastHitByPlayerIndex = attacker->as.combat.playerIndex;
        sfx_play(EnemySfxSets[unitType].hitSfx);

        // re-flinch only once the previous hitstun ran out, or on the killing blow
        if (!(victim->flags.p & ACTOR_FLAG_NO_FLINCH) &&
            (victim->hitStunTimer <= 0 || victim->currentHp <= 0))
        {
            victim->hitStunTimer = EnemyTypes[unitType].hitStun;
            victim->actionState = ACTOR_STATE_HURT;
            victim->field_0x64[0] |= 2;
            victim->actionStateTimer = PLAYER_STAT(playerIndex, STAT_FOE_STUN_TIME) + 5;
        }

        if (victim->currentHp < 1)
        {
            if (PLAYER_STAT(playerIndex, STAT_HP_PERCENT_PER_KILL) != 0)
            {
                u32 heal =
                    math_percentOf(damage, (u16)PLAYER_STAT(playerIndex, STAT_HP_PERCENT_PER_KILL));
                if (heal == 0)
                {
                    heal = 1;
                }
                player_addHp(playerIndex, heal, FALSE);
            }
            if (PLAYER_STAT(playerIndex, STAT_HP_PER_KILL) != 0)
            {
                player_addHp(playerIndex, PLAYER_STAT(playerIndex, STAT_HP_PER_KILL), FALSE);
            }
            if (PLAYER_STAT(playerIndex, STAT_SPIRIT_PER_KILL) != 0)
            {
                player_addSpirit(playerIndex, PLAYER_STAT(playerIndex, STAT_SPIRIT_PER_KILL));
            }
        }
        return TRUE;
    }
    return FALSE;
}
