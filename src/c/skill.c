#include "skill.h"
#include "game.h"
#include "player.h"
#include "stats.h"
#include "variables.h"

// HACK: The inline boundary keeps count-up loops from becoming down-counters.
static inline void addTemporaryActiveLevel(u8 playerIndex, s32 row)
{
    PLAYER(playerIndex).activeSkillLevels[row]++;
}

// Search the player's nine passive-skill slots for an id.
static inline u8 findSlot(u8 playerIndex, u8 passiveSkillId)
{
    s32 slot;

    for (slot = 0; slot < HERO_PASSIVE_SKILL_COUNT; slot++)
    {
        if (PLAYER(playerIndex).passiveSkillIds[slot] == passiveSkillId)
        {
            return slot;
        }
    }

    return PASSIVE_SKILL_SLOT_NONE;
}

/**
 * Buy count levels of passiveSkillId for the player: raises Player.passiveSkillLevels and applies
 * the stat records of each new level.
 *
 * BUG: count can raise a one-level skill past its maxLevel, up to PASSIVE_SKILL_MAX_LEVEL.
 *
 * @return FALSE when the hero's tree lacks the skill or it is already maxed
 *
 * @romaddress 0x08042bc4
 */
bool skill_passive_addLevels(u8 playerIndex, u8 passiveSkillId, u8 count)
{
    u8 slot = findSlot(playerIndex, passiveSkillId);

    if (slot == PASSIVE_SKILL_SLOT_NONE ||
        PLAYER(playerIndex).passiveSkillLevels[slot] >= PassiveSkills[passiveSkillId].maxLevel)
    {
        return FALSE;
    }

    PLAYER(playerIndex).passiveSkillLevels[slot] += count;
    if (PLAYER(playerIndex).passiveSkillLevels[slot] > PASSIVE_SKILL_MAX_LEVEL)
    {
        PLAYER(playerIndex).passiveSkillLevels[slot] = PASSIVE_SKILL_MAX_LEVEL;
    }

    skill_passive_applyLevels(playerIndex, passiveSkillId, count);

    if (passiveSkillId == PASSIVE_SKILL_FLEET_OF_FOOT)
    {
        PLAYER(playerIndex).combatFlags.p |= PLAYER_COMBAT_FLAG_MOVE_SPEED_DIRTY;
    }
    return TRUE;
}

/**
 * Apply count new levels of passiveSkillId: raise Player.passiveSkillLevelsApplied (capped at
 * PASSIVE_SKILL_MAX_LEVEL) and apply the stat records of each level taken.
 *
 * @return FALSE when the hero's tree lacks the skill
 *
 * @romaddress 0x08042c6c
 */
bool skill_passive_applyLevels(u8 playerIndex, u8 passiveSkillId, u8 count)
{
    s32 levels;
    u8 slot = findSlot(playerIndex, passiveSkillId);

    if (slot == PASSIVE_SKILL_SLOT_NONE)
    {
        return FALSE;
    }

    for (levels = 0; levels < count; levels++)
    {
        if (PLAYER(playerIndex).passiveSkillLevelsApplied[slot] < PASSIVE_SKILL_MAX_LEVEL)
        {
            PLAYER(playerIndex).passiveSkillLevelsApplied[slot]++;
            skill_passive_applyLevelStats(playerIndex, slot, 1);
        }
    }

    return TRUE;
}

/**
 * Take count levels back off passiveSkillId: subtract count from Player.passiveSkillLevels,
 * clamping at 0, and drop count applied levels too unless the subtraction underflowed.
 *
 * @return FALSE when the hero's tree lacks the skill
 *
 * @romaddress 0x08042ce8
 */
bool skill_passive_removeLevels(u8 playerIndex, u8 passiveSkillId, u8 count)
{
    u8 slot = findSlot(playerIndex, passiveSkillId);

    if (slot == PASSIVE_SKILL_SLOT_NONE)
    {
        return FALSE;
    }

    PLAYER(playerIndex).passiveSkillLevels[slot] -= count;
    if (PLAYER(playerIndex).passiveSkillLevels[slot] < 0)
    {
        // BUG: purchased levels become zero, but applied levels and their stat bonuses remain.
        PLAYER(playerIndex).passiveSkillLevels[slot] = 0;
    }
    else
    {
        skill_passive_removeAppliedLevels(playerIndex, passiveSkillId, count);
    }

    return TRUE;
}

/**
 * Drop up to count applied levels of passiveSkillId that sit above the bought level, removing the
 * stat records of each level taken back.
 *
 * @return FALSE when the hero's tree lacks the skill
 *
 * @romaddress 0x08042d60
 */
bool skill_passive_removeAppliedLevels(u8 playerIndex, u8 passiveSkillId, u8 count)
{
    u8 slot = findSlot(playerIndex, passiveSkillId);
    s32 i;

    if (slot == PASSIVE_SKILL_SLOT_NONE)
    {
        return FALSE;
    }

    for (i = 0; i < count; i++)
    {
        if (PLAYER(playerIndex).passiveSkillLevelsApplied[slot] >
            PLAYER(playerIndex).passiveSkillLevels[slot])
        {
            skill_passive_applyLevelStats(playerIndex, slot, -1);
            PLAYER(playerIndex).passiveSkillLevelsApplied[slot] -= 1;
        }
    }

    return TRUE;
}

/**
 * Drop up to count temporary levels (applied above bought) from every passive skill slot.
 *
 * @romaddress 0x08042df4
 */
void skill_passive_removeTemporaryLevels(u8 playerIndex, u8 count)
{
    s32 n;
    s32 slot;

    for (n = 0; n < count; n++)
    {
        for (slot = 0; slot < HERO_PASSIVE_SKILL_COUNT; slot++)
        {
            if (PLAYER(playerIndex).passiveSkillLevelsApplied[slot] >
                PLAYER(playerIndex).passiveSkillLevels[slot])
            {
                // BUG: passes the skill id where skill_passive_applyLevelStats expects a tree
                // slot, so it reads the skill id and applied level of passiveSkillIds[skillId]
                // (past the 9-entry array for most ids) and removes the wrong skill's records.
                skill_passive_applyLevelStats(playerIndex,
                                              PLAYER(playerIndex).passiveSkillIds[slot], -1);
                PLAYER(playerIndex).passiveSkillLevelsApplied[slot] -= 1;
            }
        }
    }
}

/**
 * Add count temporary levels to learned passive skills (capped at five) and active skill rows
 * with callbacks, including unlearned active skills.
 *
 * BUG: the active loop walks 7 rows instead of HERO_ACTIVE_SKILL_COUNT (see
 * skill_active_addTemporaryLevelsToAll).
 *
 * @romaddress 0x08042e84
 */
void skill_addTemporaryLevels(u8 playerIndex, u8 count)
{
    s32 activeLevel;
    s32 activeRow;
    s32 passiveSlot;
    s32 passiveLevel;

    for (passiveLevel = 0; passiveLevel < count; passiveLevel++)
    {
        u8 level;

        for (passiveSlot = 0; passiveSlot < HERO_PASSIVE_SKILL_COUNT; passiveSlot++)
        {
            level = PLAYER(playerIndex).passiveSkillLevelsApplied[passiveSlot];
            if (level > 0 && level < PASSIVE_SKILL_MAX_LEVEL)
            {
                PLAYER(playerIndex).passiveSkillLevelsApplied[passiveSlot] = level + 1;
                // BUG: passes the skill id where skill_passive_applyLevelStats expects a tree slot
                // (see skill_passive_removeTemporaryLevels), so the wrong skill's records apply.
                skill_passive_applyLevelStats(playerIndex,
                                              PLAYER(playerIndex).passiveSkillIds[passiveSlot], 1);
            }
        }
    }

    for (activeLevel = 0; activeLevel < count; activeLevel++)
    {
        for (activeRow = 0; activeRow < HERO_ACTIVE_SKILL_COUNT + 1; activeRow++)
        {
            if (PLAYER_ACTIVE_SKILL(playerIndex, activeRow).callback != NULL)
            {
                addTemporaryActiveLevel(playerIndex, activeRow);
            }
        }
    }
}

// Recompute max HP and spirit while preserving bonuses independent of primary stats.
#define PLAYER_RECOMPUTE_BASE_STATS(playerIndex, baseStats, hpBonus, spiritBonus)                  \
    {                                                                                              \
        baseStats.strength = PLAYER_STAT(playerIndex, STAT_STRENGTH);                              \
        baseStats.health = PLAYER_STAT(playerIndex, STAT_HEALTH);                                  \
        baseStats.courage = PLAYER_STAT(playerIndex, STAT_COURAGE);                                \
        baseStats.maxHp = player_computeMaxHp(playerIndex, baseStats);                             \
        PLAYER_STAT(playerIndex, STAT_MAX_HP) = baseStats.maxHp + hpBonus;                         \
        PLAYER_STAT(playerIndex, STAT_MAX_SPIRIT) =                                                \
            player_computeMaxSpirit(playerIndex, baseStats) + spiritBonus;                         \
        player_clampCurrentHpSpirit(playerIndex);                                                  \
    }

/**
 * Add one PassiveSkill stat record value to stats[]: a primary stat or STAT_ALL_PRIMARY_STATS also
 * recomputes max HP / spirit, STAT_SPEED_PERCENT sets PLAYER_COMBAT_FLAG_MOVE_SPEED_DIRTY.
 *
 * @romaddress 0x08042f48
 */
void skill_passive_addStat(u8 playerIndex, u8 statIndex, s16 delta)
{
    HeroBaseStats baseStats;
    s16 hpBonus;
    s16 spiritBonus;

    switch (statIndex)
    {
    case STAT_STRENGTH:
    case STAT_HEALTH:
    case STAT_COURAGE:
        spiritBonus = player_getMaxSpiritBonus(playerIndex);
        hpBonus = player_getMaxHpBonus(playerIndex);
        PLAYER_STAT(playerIndex, statIndex) += delta;
        PLAYER_RECOMPUTE_BASE_STATS(playerIndex, baseStats, hpBonus, spiritBonus);
        return;

    case STAT_ALL_PRIMARY_STATS:
        spiritBonus = player_getMaxSpiritBonus(playerIndex);
        hpBonus = player_getMaxHpBonus(playerIndex);
        PLAYER_STAT(playerIndex, STAT_STRENGTH) += delta;
        PLAYER_STAT(playerIndex, STAT_ACCURACY) += delta;
        PLAYER_STAT(playerIndex, STAT_HEALTH) += delta;
        PLAYER_STAT(playerIndex, STAT_DEFENSE) += delta;
        PLAYER_STAT(playerIndex, STAT_COURAGE) += delta;
        PLAYER_RECOMPUTE_BASE_STATS(playerIndex, baseStats, hpBonus, spiritBonus);
        return;

    default:
        break;
    }

    PLAYER_STAT(playerIndex, statIndex) += delta;
    if (statIndex == STAT_SPEED_PERCENT)
    {
        PLAYER(playerIndex).combatFlags.p |= PLAYER_COMBAT_FLAG_MOVE_SPEED_DIRTY;
    }
}

/**
 * Pay the active skill's spirit cost and start casting it: Herbal Healing runs its callback at
 * once, every other row enters the cast action state with Player.castingActiveSkill set.
 *
 * @romaddress 0x080430e8
 */
void skill_active_cast(u8 playerIndex, u8 activeSkillIndex)
{
    Actor *actor;

    if (PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex).callback == NULL)
    {
        return;
    }

    if (PLAYER(playerIndex).currentSpirit <
            PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex).spiritCost &&
        !(g_GameFlags.p & GAME_FLAG_2))
    {
        return;
    }

    actor = PLAYER(playerIndex).ownerActor;

    if (!(g_GameFlags.p & GAME_FLAG_2))
    {
        player_subtractSpirit(playerIndex,
                              PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex).spiritCost);
    }

    if (activeSkillIndex == ACTIVE_SKILL_HERBAL_HEALING)
    {
        PLAYER_ACTIVE_SKILL(playerIndex, ACTIVE_SKILL_HERBAL_HEALING).callback(actor);
    }
    else
    {
        actor->actionState = ACTOR_STATE_ACTIVE_SKILL_CAST;
        PLAYER(playerIndex).castingActiveSkill = activeSkillIndex;
        PLAYER(playerIndex).activeSkillStateFlags |= ACTIVE_SKILL_STATE_CASTING;
    }
}
