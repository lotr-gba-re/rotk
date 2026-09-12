#include "combat.h"
#include "actor.h"
#include "enemy.h"
#include "game.h"
#include "rng.h"
#include "scene.h"
#include "sfx.h"
#include "variables.h"

/**
 * BUG: An item is considered to be fragile iff stat1 is fragile.
 * stat0, stat2, and stat3 are ignored!
 */
#define ITEM_IS_FRAGILE(item) ((ITEM_PREFIX(item)).stat1 == STAT_FRAGILE)

/**
 * Get the difficulty-scaled max HP of an enemy type.
 *
 * @romaddress 0x08010818
 */
u16 combat_getEnemyMaxHp(u16 enemyIndex)
{
    u16 maxHp = 0;

    // Get bonus HP values for higher difficulties first
    // +200% for hard, +400% for grueling.
    if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_GRUELING)
    {
        maxHp = math_percentOf(EnemyTypes[enemyIndex].maxHp, 400);
    }
    else if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_HARD)
    {
        maxHp = math_percentOf(EnemyTypes[enemyIndex].maxHp, 200);
    }
    // Co-op stacks another +50% on top of the difficulty bonus.
    if (g_GameFlags.p & GAME_FLAG_COOP_ACTIVE)
    {
        maxHp += math_percentOf(EnemyTypes[enemyIndex].maxHp, 50);
    }
    // Add the base HP on top of the accumulated bonuses.
    maxHp += EnemyTypes[enemyIndex].maxHp;
    if (g_GameFlags.p & GAME_FLAG_CORRUPTION_TIER_1)
    {
        maxHp += math_percentOf(maxHp, 50);
    }
    return maxHp;
}

/**
 * This function handles 3 aspects of an enemy attack hitting a player:
 *   * Calculating and returning the armor value to subtract from the hit damage
 *   * Apply status effects such as venom and fear
 *   * Choose a sound effect based on the type of attack
 *
 * @param outHitSfx receives the sfx_play id for the landing hit
 * @param rawDamage incoming pre-armor damage, compared against the shield bubble immunity threshold
 * @return the armor value to subtract from the hit
 *
 * @romaddress 0x080108c4
 */
u16 combat_handleEnemyAttackOnPlayerStatusArmorSfx(Actor *attacker, Actor *victim, u32 *outHitSfx,
                                                   s16 rawDamage)
{
    u32 playerIndex = victim->as.combat.playerIndex;
    u8 attackerType = attacker->unitType;
    s32 armor = 0;
    bool statusApplies = TRUE;
    s32 cut;

    if (EnemyTypes[attackerType].attackFlags.p & ENEMY_ATTACK_RANGED)
    {
        armor = PLAYER_STAT(playerIndex, STAT_MISSILE_ARMOR);
        *outHitSfx = SFX_COMBAT_HIT_MISSILE;
    }
    else if (EnemyTypes[attackerType].attackFlags.p & ENEMY_ATTACK_MELEE_MASK)
    {
        armor = PLAYER_STAT(playerIndex, STAT_MELEE_ARMOR);

        if (EnemyTypes[attackerType].attackFlags.p & ENEMY_ATTACK_MELEE_SLASH)
        {
            *outHitSfx = SFX_COMBAT_HIT_SLASH;
        }
        else
        {
            *outHitSfx = SFX_COMBAT_HIT_IMPALE;
        }
    }

    if (PLAYER(playerIndex).shieldBubble != NULL && PLAYER(playerIndex).heroId == HERO_ID_GANDALF &&
        PLAYER(playerIndex).shieldBubble->as.shieldGandalf.shieldHealth >= rawDamage)
    {
        statusApplies = FALSE;
    }

    if ((EnemyTypes[attackerType].attackFlags.p & ENEMY_ATTACK_POISON) && statusApplies == TRUE)
    {
        victim->as.combat.venomDuration =
            EnemyTypes[attackerType].poisonPower * STATUS_FRAMES_PER_POWER;

        if (PLAYER_STAT(playerIndex, STAT_POISON_RESIST_PERCENT) > 0)
        {
            cut = math_percentOf(victim->as.combat.venomDuration,
                                 (u16)PLAYER_STAT(playerIndex, STAT_POISON_RESIST_PERCENT));
            victim->as.combat.venomDuration -= cut;
        }

        if (!(g_PlayerHuds[playerIndex].flags.p & PLAYERHUD_FLAG_POISON))
        {
            g_PlayerHuds[playerIndex].flags.p |= PLAYERHUD_FLAG_POISON | PLAYERHUD_FLAG_12;
        }

        player_subtractHp(playerIndex, 2, FALSE);
    }

    if ((EnemyTypes[attackerType].attackFlags.p & ENEMY_ATTACK_FEAR) && statusApplies == TRUE)
    {
        victim->as.combat.fearDuration =
            EnemyTypes[attackerType].fearPower * STATUS_FRAMES_PER_POWER;
        if (PLAYER_STAT(playerIndex, STAT_FEAR_RESIST_PERCENT) > 0)
        {
            cut = math_percentOf(victim->as.combat.fearDuration,
                                 (u16)PLAYER_STAT(playerIndex, STAT_FEAR_RESIST_PERCENT));
            victim->as.combat.fearDuration -= cut;
        }
        if (!(g_PlayerHuds[playerIndex].flags.p & PLAYERHUD_FLAG_FEAR))
        {
            g_PlayerHuds[playerIndex].flags.p |= PLAYERHUD_FLAG_FEAR | PLAYERHUD_FLAG_12;
        }
        player_addSpirit(playerIndex, -3);
    }

    if (EnemyTypes[attackerType].attackFlags.p & ENEMY_ATTACK_FIRE)
    {
        armor += PLAYER_STAT(playerIndex, STAT_FIRE_PROTECTION);
        *outHitSfx = SFX_COMBAT_FIREBURST_PROC;
    }

    return armor;
}

/**
 * Configure a freshly spawned burst fx actor for an on-hit proc
 * (fireburst/venomous/triple/sunburst/nightburst) and play its sfx.
 * Inlined in the ROM, hence an inline here.
 */
static inline void configBurstFx(Actor *fx, s32 sfx)
{
    fx->recordType = FX_BURST_UNK_PARAM;
    // fx_tickAttachment returns a value that the tick dispatcher discards
    fx->tickCallback = (ActorCallback)fx_tickAttachment;
    sfx_play(sfx);
}

/**
 * Compute the damage a player's attack inflicts on an enemy.
 * Also plays applicable on-hit sfx such as fireburst.
 *
 * @return inflicted damage as u16.
 *
 * @romaddress 0x08010ad4
 */
s32 combat_computePveDamage(Actor *attacker, Actor *victim, bool crit)
{
    u32 playerIndex = attacker->as.combat.playerIndex;
    u32 damage;
    s32 enemyArmor;
    Actor *fx;

    if (crit)
    {
        damage = combat_getPlayerMaxDamage(playerIndex);
        damage += PLAYER_STAT(playerIndex, STAT_CRIT_DAMAGE);
    }
    else
    {
        damage = rng_rollRange(combat_getPlayerMinDamage(playerIndex),
                               combat_getPlayerMaxDamage(playerIndex));
    }

    damage += combat_getEnemyTypeDamageBonus(playerIndex, victim->unitType);

    if (victim->actionState == ACTOR_STATE_STUNNED)
    {
        damage *= 2;
    }

    switch (attacker->actionState)
    {
    case ACTOR_STATE_ATK_UNKNOWN_33:
    case ACTOR_STATE_ATK_UNKNOWN_34:
    case ACTOR_STATE_ATK_UNKNOWN_35:
    case ACTOR_STATE_ATK_UNKNOWN_3A:
    case ACTOR_STATE_ATK_UNKNOWN_59:
        damage += attacker->as.combat.storedDamage;
        break;
    case ACTOR_STATE_ATK_CHARGE_ONLY:
        damage = attacker->as.combat.storedDamage;
        break;
    case ACTOR_STATE_ATK_ACTIVE_SKILL3: {
        s32 activeSkillLevel = PLAYER(playerIndex).activeSkillLevels[ACTIVE_SKILL_UNKNOWN_3];
        damage =
            activeSkill_getLeveledValue(playerIndex, ACTIVE_SKILL_UNKNOWN_3, 0, activeSkillLevel);
        break;
    }
    }

    if (!PLAYER_STAT(playerIndex, STAT_BACKSTAB) || !combat_isBackstab(attacker, victim))
    {
        if (PLAYER(playerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_DAMAGE_TYPE)
        {
            enemyArmor = EnemyTypes[victim->unitType].slashArmor;
        }
        else
        {
            enemyArmor = EnemyTypes[victim->unitType].impaleArmor;
        }
        if (PLAYER_STAT(playerIndex, STAT_ARMOR_PIERCE))
        {
            enemyArmor /= 3;
        }
    }
    else
    {
        enemyArmor = 0;
    }

    damage -= enemyArmor;
    if ((s32)damage < 1)
    {
        damage = 1;
    }

    if (PLAYER_STAT(playerIndex, STAT_FIREBURST))
    {
        damage += FIREBURST_BONUS_DAMAGE;
        fx = fx_spawnOnActor(victim, FX_TYPE_HIT_BURST, FX_BURST_FIRE, 1);
        configBurstFx(fx, SFX_COMBAT_FIREBURST_PROC);
    }
    if (PLAYER_STAT(playerIndex, STAT_VENOMOUS))
    {
        damage += VENOMOUS_BONUS_DAMAGE;
        fx = fx_spawnOnActor(victim, FX_TYPE_HIT_BURST, FX_BURST_VENOM, 0);
        configBurstFx(fx, SFX_COMBAT_VENOM_PROC);
    }
    if (PLAYER(playerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_TRIPLE_DAMAGE)
    {
        damage *= 3;
        fx = fx_spawnOnActor(victim, FX_TYPE_HIT_BURST, FX_BURST_TRIPLE, 0);
        configBurstFx(fx, SFX_COMBAT_TRIPLE_DAMAGE);
    }
    return (u16)damage;
}

/**
 * This function handles 3 aspects of a PvP attack hitting the victim player:
 *   * Calculating and returning the armor value to subtract from the hit damage
 *   * Applying the attacker's venom status effect
 *   * Choosing and playing a hit sfx based on the type of attack
 *
 * @param rawDamage pre-armor damage
 * @return the armor value to subtract from the hit
 *
 * @romaddress 0x08010d90
 */
s32 combat_handlePvpStatusArmorSfx(Actor *attacker, Actor *victim, s16 rawDamage)
{
    u32 victimIndex = victim->as.combat.playerIndex;
    u32 attackerIndex = attacker->as.combat.playerIndex;
    u32 armor;
    bool applyVenom = TRUE;

    if (PLAYER(attackerIndex).heroId == HERO_ID_LEGOLAS)
    {
        armor = PLAYER_STAT(victimIndex, STAT_MISSILE_ARMOR);
        sfx_play(SFX_COMBAT_HIT_MISSILE);
    }
    else
    {
        armor = PLAYER_STAT(victimIndex, STAT_MELEE_ARMOR);
        if (PLAYER(attackerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_DAMAGE_TYPE)
        {
            sfx_play(SFX_COMBAT_HIT_SLASH);
        }
        else
        {
            sfx_play(SFX_COMBAT_HIT_IMPALE);
        }
    }

    if (PLAYER(victimIndex).shieldBubble != NULL && PLAYER(victimIndex).heroId == HERO_ID_GANDALF &&
        PLAYER(victimIndex).shieldBubble->as.shieldGandalf.shieldHealth >= rawDamage)
    {
        applyVenom = FALSE;
    }

    if (PLAYER_STAT(attackerIndex, STAT_VENOMOUS) && applyVenom == TRUE)
    {
        victim->as.combat.venomDuration = VENOM_DURATION_FRAMES;
        if (PLAYER_STAT(victimIndex, STAT_POISON_RESIST_PERCENT) > 0)
        {
            victim->as.combat.venomDuration -= math_percentOf(
                VENOM_DURATION_FRAMES, (u16)PLAYER_STAT(victimIndex, STAT_POISON_RESIST_PERCENT));
        }
        if (!(g_PlayerHuds[victimIndex].flags.p & PLAYERHUD_FLAG_POISON))
        {
            g_PlayerHuds[victimIndex].flags.p |= PLAYERHUD_FLAG_POISON | PLAYERHUD_FLAG_12;
        }
        player_subtractHp(victimIndex, 2, FALSE);
    }
    return (u16)armor;
}

/**
 * Compute the damage a player's attack inflicts on another player.
 * Also plays applicable on-hit sfx such as fireburst.
 *
 * @return inflicted damage as u16.
 *
 * @romaddress 0x08010edc
 */
s32 combat_computePvpDamage(Actor *attacker, Actor *victim, bool crit)
{
    u32 playerIndex = attacker->as.combat.playerIndex;
    u32 damage;
    s32 victimArmor;
    Actor *fx;

    if (crit)
    {
        damage = combat_getPlayerMaxDamage(playerIndex);
        damage += PLAYER_STAT(playerIndex, STAT_CRIT_DAMAGE);
    }
    else
    {
        damage = rng_rollRange(combat_getPlayerMinDamage(playerIndex),
                               combat_getPlayerMaxDamage(playerIndex));
    }

    if (victim->actionState == ACTOR_STATE_STUNNED)
    {
        damage *= 2;
    }

    switch (attacker->actionState)
    {
    case ACTOR_STATE_ATK_UNKNOWN_33:
    case ACTOR_STATE_ATK_UNKNOWN_34:
    case ACTOR_STATE_ATK_UNKNOWN_35:
    case ACTOR_STATE_ATK_UNKNOWN_3A:
    case ACTOR_STATE_ATK_UNKNOWN_59:
        damage += attacker->as.combat.storedDamage;
        break;
    case ACTOR_STATE_ATK_CHARGE_ONLY:
        damage = attacker->as.combat.storedDamage;
        break;
    case ACTOR_STATE_ATK_ACTIVE_SKILL3: {
        s32 activeSkillLevel = PLAYER(playerIndex).activeSkillLevels[ACTIVE_SKILL_UNKNOWN_3];
        damage =
            activeSkill_getLeveledValue(playerIndex, ACTIVE_SKILL_UNKNOWN_3, 0, activeSkillLevel);
        break;
    }
    }

    if (!PLAYER_STAT(playerIndex, STAT_BACKSTAB) || !combat_isBackstab(attacker, victim))
    {
        victimArmor = (u16)combat_handlePvpStatusArmorSfx(attacker, victim, damage);
        if (PLAYER_STAT(playerIndex, STAT_ARMOR_PIERCE))
        {
            victimArmor /= 3;
        }
    }
    else
    {
        victimArmor = 0;
    }

    damage -= victimArmor;
    if ((s32)damage < 1)
    {
        damage = 1;
    }

    if (PLAYER_STAT(playerIndex, STAT_FIREBURST))
    {
        damage += FIREBURST_BONUS_DAMAGE;
        fx = fx_spawnOnActor(victim, FX_TYPE_HIT_BURST, FX_BURST_FIRE, 1);
        configBurstFx(fx, SFX_COMBAT_FIREBURST_PROC);
    }
    if (PLAYER_STAT(playerIndex, STAT_VENOMOUS))
    {
        damage += VENOMOUS_BONUS_DAMAGE;
        fx = fx_spawnOnActor(victim, FX_TYPE_HIT_BURST, FX_BURST_VENOM, 1);
        configBurstFx(fx, SFX_COMBAT_VENOM_PROC);
    }
    if (PLAYER(playerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_TRIPLE_DAMAGE)
    {
        damage *= 3;
        fx = fx_spawnOnActor(victim, FX_TYPE_HIT_BURST, FX_BURST_TRIPLE, 1);
        configBurstFx(fx, SFX_COMBAT_TRIPLE_DAMAGE);
    }
    return (u16)damage;
}

/**
 * Get difficulty-scaled defense for the given enemy index.
 * Also applies the corruption tier-1 buff (GAME_FLAG_CORRUPTION_TIER_1).
 *
 * @romaddress 0x0801113c
 */
u16 combat_getEnemyDefense(u16 enemyIndex)
{
    u16 defense = 0;
    u16 baseDefense;

    // Get bonus defense values for higher difficulties first
    // +50% for hard, +100% for grueling.
    if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_GRUELING)
    {
        defense = math_percentOf(EnemyTypes[enemyIndex].defense, 100);
    }
    else if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_HARD)
    {
        defense = math_percentOf(EnemyTypes[enemyIndex].defense, 50);
    }

    // Add the base defense on top of the difficulty bonus.
    baseDefense = EnemyTypes[enemyIndex].defense;
    defense = baseDefense + defense;

    if (g_GameFlags.p & GAME_FLAG_CORRUPTION_TIER_1)
    {
        s16 signedDefense = defense;
        s32 boosted = signedDefense + math_percentOf(signedDefense, 25);
        defense = boosted;
    }
    if (g_FlagBearerCount)
    {
        s16 signedDefense = defense;
        defense = signedDefense + FLAG_BEARER_DEFENSE_BONUS;
    }

    return defense;
}

/**
 * Get the difficulty-scaled accuracy of an enemy type.
 * Also applies the corruption tier-1 buff (GAME_FLAG_CORRUPTION_TIER_1).
 *
 * @romaddress 0x080111e0
 */
u16 combat_getEnemyAccuracy(u16 enemyIndex)
{
    u16 accuracy = 0;
    u16 baseAccuracy;

    // Get bonus accuracy values for higher difficulties first
    // +50% for hard, +100% for grueling.
    if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_GRUELING)
    {
        accuracy = math_percentOf(EnemyTypes[enemyIndex].accuracy, 100);
    }
    else if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_HARD)
    {
        accuracy = math_percentOf(EnemyTypes[enemyIndex].accuracy, 50);
    }

    // Add the base accuracy on top of the difficulty bonus.
    baseAccuracy = EnemyTypes[enemyIndex].accuracy;
    accuracy = baseAccuracy + accuracy;

    if (g_GameFlags.p & GAME_FLAG_CORRUPTION_TIER_1)
    {
        s16 signedAccuracy = accuracy;
        s32 boosted = signedAccuracy + math_percentOf(signedAccuracy, 25);
        accuracy = boosted;
    }
    if (g_FlagBearerCount)
    {
        s16 signedAccuracy = accuracy;
        accuracy = signedAccuracy + FLAG_BEARER_ACCURACY_BONUS;
    }

    return accuracy;
}

/**
 * Get the difficulty-scaled max attack damage of an enemy type: base EnemyTypeInfo.attackDamage
 * Also applies the corruption tier-1 buff (GAME_FLAG_CORRUPTION_TIER_1).
 *
 * @romaddress 0x08011284
 */
u16 combat_getEnemyMaxDamage(u16 enemyIndex)
{
    u16 damage = 0;
    u16 baseDamage;

    // Get bonus damage values for higher difficulties first
    // +200% for hard, +400% for grueling.
    if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_GRUELING)
    {
        damage = math_percentOf(EnemyTypes[enemyIndex].attackDamage, 400);
    }
    else if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_HARD)
    {
        damage = math_percentOf(EnemyTypes[enemyIndex].attackDamage, 200);
    }

    // Add the base damage on top of the difficulty bonus.
    baseDamage = EnemyTypes[enemyIndex].attackDamage;
    damage = baseDamage + damage;

    if (g_GameFlags.p & GAME_FLAG_CORRUPTION_TIER_1)
    {
        s16 signedDamage = damage;
        s32 boosted = signedDamage + math_percentOf(signedDamage, FLAG_BEARER_DAMAGE_BONUS_PERCENT);
        damage = boosted;
    }
    if (g_FlagBearerCount)
    {
        s16 signedDamage = damage;
        s32 boosted = signedDamage + math_percentOf(signedDamage, FLAG_BEARER_DAMAGE_BONUS_PERCENT);
        damage = boosted;
    }

    return damage;
}

/** NONPLAYER_CRIT_CHANCE_PERCENT of non-player hits crit for full maxDamage. */
static inline bool rollNonPlayerCrit(void)
{
    if (rng_rollRange(1, 100) <= NONPLAYER_CRIT_CHANCE_PERCENT)
    {
        return TRUE;
    }
    return FALSE;
}

/**
 * Apply a non-player hit to an enemy actor. Non-player damage includes:
 *   - enemy-vs-enemy friendly fire, e.g. an AoE swing catching nearby allies or a confused / feared
 *     enemy striking another enemy
 *   - environmental / object damage, e.g. a trap, a fire tile, or a siege boulder
 *   - attacks by the NPC family: allied soldiers and companions, fired siege objects
 *
 * This includes a check for evasion / invulnerable state before applying
 * any damage.
 *
 * @param maxDamage crit damage, and the upper bound of a non-crit roll in [maxDamage/2, maxDamage]
 * @return true if the attack hit, false if the attack was evaded
 *
 * @romaddress 0x08011330
 */
bool combat_applyNonPlayerHitOnEnemy(Actor *victim, s16 maxDamage)
{
    if (combat_rollVictimEvade(victim) || (victim->flags.p & ACTOR_FLAG_INVULNERABLE))
    {
        return FALSE;
    }

    // A non-crit is reduced to a uniform roll in [maxDamage/2, maxDamage].
    if (!rollNonPlayerCrit())
    {
        maxDamage = rng_rollRange(maxDamage / 2, maxDamage);
    }

    maxDamage -= EnemyTypes[victim->unitType].slashArmor;
    if (maxDamage < 0)
    {
        maxDamage = 1;
    }

    victim->currentHp -= maxDamage;

    if (!(victim->flags.p & ACTOR_FLAG_NO_FLINCH) &&
        (victim->hitStunTimer < 1 || victim->currentHp < 1))
    {
        victim->hitStunTimer = EnemyTypes[victim->unitType].hitStun;
        victim->actionState = ACTOR_STATE_HURT;
        victim->field_0x64[0] |= 2;
        victim->actionStateTimer = HURT_STATE_FRAMES;
    }

    sfx_play(EnemySfxSets[victim->unitType].hitSfx);
    sfx_play(SFX_COMBAT_HIT_SLASH);

    return TRUE;
}

/**
 * Kill an enemy on the spot: zero its HP and put it into the hurt flinch state.
 *
 * TODO: Find out what this is used for.
 *
 * @romaddress 0x08011418
 */
void combat_killEnemyOutright(Actor *victim)
{
    victim->currentHp = 0;
    if (!(victim->flags.p & ACTOR_FLAG_NO_FLINCH))
    {
        victim->hitStunTimer = EnemyTypes[victim->unitType].hitStun;
        victim->actionState = ACTOR_STATE_HURT;
        victim->field_0x64[0] |= 2;
        victim->actionStateTimer = HURT_STATE_FRAMES;
    }
    sfx_play(EnemySfxSets[victim->unitType].hitSfx);
}

/**
 * Calculate player's bonus damage against the victim's creature family (animals, wargs, orcs, ...),
 *
 * @param enemyType index into EnemyTypes (its .category selects the stat)
 *
 * @romaddress 0x08011488
 */
u16 combat_getEnemyTypeDamageBonus(s32 playerIndex, u8 enemyType)
{
    switch (EnemyTypes[enemyType].category)
    {
    case CREATURE_ANIMALS:
        return PLAYER_STAT(playerIndex, STAT_DAMAGE_TO_ANIMALS);
    case CREATURE_CREBAIN:
        return PLAYER_STAT(playerIndex, STAT_DAMAGE_TO_CREBAIN);
    case CREATURE_WARGS:
        return PLAYER_STAT(playerIndex, STAT_DAMAGE_TO_WARGS);
    case CREATURE_ORCS:
        return PLAYER_STAT(playerIndex, STAT_DAMAGE_TO_ORCS);
    case CREATURE_URUKHAI:
        return PLAYER_STAT(playerIndex, STAT_DAMAGE_TO_URUKHAI);
    case CREATURE_NAZGUL:
        return PLAYER_STAT(playerIndex, STAT_DAMAGE_TO_NAZGUL);
    default:
        return 0;
    }
}

/**
 * Handles absorption of hits into a player's shield bubble. Used by Gandalf and Gimli for skills.
 *
 * @romaddress 0x0801154c
 */
u16 combat_absorbDamageIntoShield(Actor *attacker, u16 initialDamage, s32 playerIndex)
{
    s32 damage = initialDamage;
    u16 remaining = damage;
    Actor *shieldBubble = PLAYER(playerIndex).shieldBubble;

    if (shieldBubble != NULL)
    {
        // reused for the spawned absorb FX in Gimli's arm
        Actor *fx = shieldBubble;

        switch (PLAYER(playerIndex).heroId)
        {
        case HERO_ID_GANDALF:
            if (fx->as.shieldGandalf.shieldHealth > 0)
            {
                fx->as.shieldGandalf.followTarget = attacker;
                if (fx->as.shieldGandalf.shieldHealth >= damage)
                {
                    fx->as.shieldGandalf.shieldHealth -= damage;
                    remaining = 0;
                }
                else
                {
                    fx->as.shieldGandalf.shieldHealth = 0;
                }

                if (fx->actionState == ACTOR_STATE_FX_ATTACHED)
                {
                    fx->actionState = ACTOR_STATE_SHIELD_SPENT;
                    sfx_play(SFX_COMBAT_GANDALF_SHIELD_ABSORB);
                }
            }
            break;
        case HERO_ID_GIMLI:
            remaining = damage - math_percentOf(damage, shieldBubble->as.shieldGimli.shieldPercent);
            fx = fx_spawnOnActor(attacker, FX_TYPE_SHIELD_ABSORB,
                                 FxChargeVariants[shieldBubble->as.shieldGimli.chargeLevel - 1], 1);
            // fx_tickAttachment returns a value that the tick dispatcher discards
            fx->tickCallback = (ActorCallback)fx_tickAttachment;
            fx->recordType = FX_SHIELD_ABSORB_UNK_PARAM;
            break;
        default:
            break;
        }
    }
    return remaining;
}

/**
 * Pick which inventory slot the fragile proc breaks.
 * Declared as inline because it's inlined in the ROM, too.
 */
static inline u8 fragileOnKillPickSlot(s32 playerIndex, Item weapon, Item offhand)
{
    u8 slot;

    if ((PLAYER(playerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_1) && ITEM_IS_FRAGILE(weapon))
    {
        slot = INVENTORY_SLOT_WEAPON;
    }
    else
    {
        s32 hasOffhand = PLAYER(playerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_2;

        slot = INVENTORY_SLOT_BACKPACK_0;
        if (hasOffhand && ITEM_IS_FRAGILE(offhand))
        {
            slot = INVENTORY_SLOT_OFFHAND;
        }
    }
    return slot;
}

/**
 * Roll to proc the 'Fragile' equipment effect and break a fragile item if it procs.
 * Also plays the breaking sfx if it procs.
 *
 * @romaddress 0x08011608
 */
void combat_fragileOnKill(s32 playerIndex)
{
    Item weapon = PLAYER(playerIndex).inventory.slots.weapon;
    Item offhand = PLAYER(playerIndex).inventory.slots.offhand;

    if (rng_rollRange(1, 100) < FRAGILE_PROC_PERCENT)
    {
        item_deleteFromInventory(fragileOnKillPickSlot(playerIndex, weapon, offhand), playerIndex);
        sfx_play(SFX_COMBAT_ITEM_BREAK);
    }
}

// TODO: Find out how this config works and if the name is accurate
static inline void configBurstEmitter(FxEmitter *e, Actor *target)
{
    e->target = target;
    e->field_0x28 = 2;
    e->field_0x3a = 30;
    e->field_0x38 = 1;
    e->field_0x2a = 8;
    e->field_0x30 = 15;
    e->field_0x36 = 3;
    e->field_0x37 = 0;
    e->field_0x2e |= 0x4082;
    e->field_0x3f = 0xff;
}

/**
 * Roll for a sunburst proc and apply the sunburst stun effect on success.
 *
 * @param procTarget The victim sunburst is being rolled for
 *
 * @romaddress 0x080116e8
 */
void combat_sunburstOnKill(Actor *procTarget)
{
    Actor *nextTarget = NULL;
    FxEmitter *e;
    Actor *fx;

    if (rng_rollRange(1, 100) <= BURST_PROC_PERCENT)
    {
        do
        {
            nextTarget = actor_iterateInRadius(procTarget, nextTarget, BURST_RADIUS << 16);
            if (nextTarget != NULL && nextTarget->currentHp > 0 &&
                !(nextTarget->flags.p & ACTOR_FLAG_INVULNERABLE) && actor_isStunnable(nextTarget))
            {
                nextTarget->actionState = ACTOR_STATE_STUNNED;
                nextTarget->actionStateTimer = SUNBURST_STUN_FRAMES;
                nextTarget->field_0x64[0] |= 2;
                fx_attachStatusEffect(NULL, nextTarget, SUNBURST_STUN_FRAMES, 0xc, 8, 0, 8);
                e = fx_allocEmitter(FX_AOE_BURST_SUN, 0, 4);
                if (e != NULL)
                {
                    configBurstEmitter(e, nextTarget);
                }
            }
        } while (nextTarget != NULL);
        fx = fx_spawnOnActor(procTarget, FX_TYPE_AOE_BURST, FX_AOE_BURST_SUN, 0);
        configBurstFx(fx, SFX_COMBAT_SUNBURST);
    }
}

/**
 * Roll for a nightburst proc and apply the nightburst damage and flinch effect on success.
 *
 * @param procTarget The victim nightburst is being rolled for
 *
 * @romaddress 0x080117e8
 */
void combat_nightburstOnKill(s32 playerIndex, Actor *procTarget)
{
    Actor *nextTarget = NULL;
    FxEmitter *e;
    Actor *fx;

    if (rng_rollRange(1, 100) <= BURST_PROC_PERCENT)
    {
        do
        {
            nextTarget = actor_iterateInRadius(procTarget, nextTarget, BURST_RADIUS << 16);
            if (nextTarget != NULL && nextTarget->currentHp > 0 &&
                !(nextTarget->flags.p & ACTOR_FLAG_INVULNERABLE))
            {
                nextTarget->currentHp -= NIGHTBURST_DAMAGE;

                if (nextTarget->hitStunTimer < 1 || nextTarget->currentHp < 1)
                {
                    nextTarget->hitStunTimer = EnemyTypes[nextTarget->unitType].hitStun;
                    nextTarget->actionState = ACTOR_STATE_HURT;
                    nextTarget->field_0x64[0] |= 2;
                    nextTarget->actionStateTimer =
                        PLAYER_STAT(playerIndex, STAT_FOE_STUN_TIME) + HURT_STATE_FRAMES;
                }

                e = fx_allocEmitter(FX_AOE_BURST_NIGHT, 0, 4);
                if (e != NULL)
                {
                    configBurstEmitter(e, nextTarget);
                }
            }
        } while (nextTarget != NULL);
        fx = fx_spawnOnActor(procTarget, FX_TYPE_AOE_BURST, FX_AOE_BURST_NIGHT, 0);
        configBurstFx(fx, SFX_COMBAT_TRIPLE_DAMAGE);
    }
}

/**
 * Resolve an enemy vs. enemy attack ("friendly fire").
 * Roll accuracy, apply the hit, and handle the sfx.
 *
 * @return true when damage was applied
 *
 * @romaddress 0x08011908
 */
bool combat_resolveEnemyAttackOnEnemy(Actor *attacker, Actor *victim)
{
    u16 accuracy;
    u16 defense;
    s16 damage;

    if (!(victim->flags.p & ACTOR_FLAG_INVULNERABLE))
    {
        accuracy = combat_getEnemyAccuracy(attacker->unitType);
        defense = combat_getEnemyDefense(victim->unitType);

        if (combat_rollAccuracy(accuracy, defense))
        {
            victim->lastHitByPlayerIndex = KILL_CREDIT_NONE;
            damage = combat_getEnemyMaxDamage(attacker->unitType);
            return combat_applyNonPlayerHitOnEnemy(victim, damage);
        }

        sfx_play(EnemySfxSets[victim->unitType].missSfx);
    }
    return FALSE;
}

/**
 * Resolve a non-player inflicted hit on an enemy (a trap, a fire tile, a siege boulder, or
 * an attacking NPC). Roll accuracy, apply the hit via
 * combat_applyNonPlayerHitOnEnemy, and handle the sfx. A hit from a combat-active ally or
 * fired siege object earns the player indirect kill credit.
 *
 * @param accuracy attacker to-hit accuracy
 * @param maxDamage upper bound of the non-crit damage roll
 * @return true when damage was applied
 *
 * @romaddress 0x08011988
 */
bool combat_resolveNonPlayerAttackOnEnemy(Actor *attacker, Actor *victim, u16 accuracy,
                                          u16 maxDamage)
{
    s32 enemyType = victim->unitType;
    u16 defense;

    if (maxDamage < NONPLAYER_MIN_MAX_DAMAGE)
    {
        maxDamage = NONPLAYER_MIN_MAX_DAMAGE;
    }

    if (!(victim->flags.p & ACTOR_FLAG_INVULNERABLE))
    {
        defense = combat_getEnemyDefense(enemyType);
        if (combat_rollAccuracy(accuracy, defense))
        {
            // hit
            if (attacker->behaviorState == NPC_BEHAVIOR_COMBAT_ACTIVE &&
                !(g_GameFlags.p & GAME_FLAG_COOP_ACTIVE))
            {
                victim->lastHitByPlayerIndex = KILL_CREDIT_INDIRECT;
            }
            else
            {
                victim->lastHitByPlayerIndex = KILL_CREDIT_NONE;
            }
            return combat_applyNonPlayerHitOnEnemy(victim, maxDamage);
        }

        // miss
        sfx_play(EnemySfxSets[enemyType].missSfx);
    }

    return FALSE;
}

/**
 * Award the dead enemy's EnemyTypeInfo.xpReward to the party, gated by the kill-credit code in
 * victim->lastHitByPlayerIndex: KILL_CREDIT_NONE awards nothing, KILL_CREDIT_INDIRECT (or
 * playerIndex 1!) awards half - unless GAME_FLAG_COOP_ACTIVE, which restores full XP.
 *
 * TODO: Confirm this, unchecked
 *
 * @romaddress 0x08011a28
 */
void combat_awardKillXp(Actor *victim)
{
    if (victim->lastHitByPlayerIndex != KILL_CREDIT_NONE)
    {
        if (!(g_GameFlags.p & GAME_FLAG_COOP_ACTIVE) && victim->lastHitByPlayerIndex != 0)
        {
            player_awardXp(EnemyTypes[victim->unitType].xpReward >> 1);
        }
        else
        {
            player_awardXp(EnemyTypes[victim->unitType].xpReward);
        }
    }
}

/**
 * Checks if the attacker is in a backstab position relative to the victim. This is solely
 * based on the direction each actor is facing. A hit is eligible for a backstab if the
 * attacker faces the same direction of 8 as the victim, allowing for a +-1 deviation.
 *
 * @romaddress 0x08011a88
 */
bool combat_isBackstab(Actor *attacker, Actor *victim)
{
    s32 attackerFacing = attacker->facing;
    s32 victimFacing = victim->facing;
    s32 prev;
    s32 next;

    switch (attackerFacing)
    {
    case DIRECTION8_NORTH:
        prev = DIRECTION8_NORTHWEST;
        next = DIRECTION8_NORTHEAST;
        break;
    case DIRECTION8_NORTHWEST:
        prev = DIRECTION8_WEST;
        next = DIRECTION8_NORTH;
        break;
    default:
        prev = attackerFacing - 1;
        next = attackerFacing + 1;
        break;
    }

    if (prev == victimFacing || attackerFacing == victimFacing || next == victimFacing)
    {
        return TRUE;
    }
    return FALSE;
}

/**
 * Performs an accuracy check of the attacker's accuracy against the victim's defense.
 * The chance is clamped to 10..95 (a hit is never certain and never impossible).
 *
 * @romaddress 0x08011ac0
 */
bool combat_rollAccuracy(u16 attackerAccuracy, u16 victimDefense)
{
    s16 chance = attackerAccuracy + 50 - victimDefense;
    if (chance < MIN_HIT_CHANCE_PERCENT)
    {
        chance = MIN_HIT_CHANCE_PERCENT;
    }
    else if (chance > MAX_HIT_CHANCE_PERCENT)
    {
        chance = MAX_HIT_CHANCE_PERCENT;
    }
    if (rng_rollRange(1, 100) <= chance)
    {
        return TRUE;
    }
    return FALSE;
}

/**
 * Performs a generic percentage-based proc roll.
 * Succeeds chance% of the time.
 *
 * @romaddress 0x08011b00
 */
bool combat_rollPercentage(u16 chance)
{
    if (rng_rollRange(1, 100) <= chance)
    {
        return TRUE;
    }
    return FALSE;
}

/**
 * Rolls for evasion if the victim has the evasive flag set.
 *
 * @romaddress 0x08011b20
 */
bool combat_rollVictimEvade(Actor *victim)
{
    if ((victim->combatFlags.p & ACTOR_COMBAT_FLAG_EVASIVE) &&
        rng_rollRange(1, 100) <= EVASION_CHANCE_PERCENT)
    {
        sfx_play(SFX_COMBAT_EVADE);
        return TRUE;
    }
    return FALSE;
}

/**
 * Rolls for an avoidance proc of the type given by the statIndex. Avoidance procs include:
 *   * dodge
 *   * parry
 *   * riposte
 *   * shield block
 * Plays the corresponding avoidance sfx if it triggers.
 *
 * @param statIndex avoidance stat (STAT_PARRY_PERCENT / STAT_DODGE_PERCENT / STAT_RIPOSTE_PERCENT /
 * STAT_BLOCK_PERCENT)
 *
 * @romaddress 0x08011b4c
 */
bool combat_rollAvoidance(u16 statIndex, s32 playerIndex)
{
    s16 chance = PLAYER_STAT(playerIndex, statIndex);
    if (chance > 0)
    {
        if (chance > AVOIDANCE_CAP_PERCENT)
        {
            chance = AVOIDANCE_CAP_PERCENT;
        }
        if (rng_rollRange(1, 100) <= chance)
        {
            switch (statIndex)
            {
            case STAT_DODGE_PERCENT:
                break;
            case STAT_RIPOSTE_PERCENT:
                sfx_play(SFX_COMBAT_RIPOSTE);
                break;
            case STAT_ARROW_PARRY_PERCENT:
                sfx_play(SFX_COMBAT_BLOCK);
                break;
            default: // STAT_PARRY_PERCENT, STAT_BLOCK_PERCENT
                sfx_play(SFX_COMBAT_BLOCK);
                break;
            }
            return TRUE;
        }
    }
    return FALSE;
}
