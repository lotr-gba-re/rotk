#include "player.h"
#include "skill.h"
#include "stats.h"
#include "variables.h"

// HACK: The inline boundary keeps count-up loops from becoming down-counters.
static inline void removeTemporaryActiveLevel(u8 playerIndex, s32 row)
{
    PLAYER(playerIndex).activeSkillLevels[row]--;
}

/**
 * Slot of passiveSkillId in the player's skill tree, or PASSIVE_SKILL_SLOT_NONE.
 *
 * @romaddress 0x08045020
 */
u8 skill_passive_findSlot(u8 playerIndex, u8 passiveSkillId)
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
 * Apply the stat records of every slot's Player.passiveSkillLevelsApplied levels to stats[].
 *
 * BUG: walks 10 slots instead of HERO_PASSIVE_SKILL_COUNT. The extra ID is a zero padding byte,
 * or the first byte of the following direction table for Gimli. Both select Fearless.
 *
 * @romaddress 0x08045058
 */
void skill_passive_applyAllLevels(u8 playerIndex)
{
    s32 slot;

    for (slot = 0; slot < HERO_PASSIVE_SKILL_COUNT + 1; slot++)
    {
        // HACK: the double read is load-bearing: a single read puts the load into the argument
        // register and drops the ROM's register copy.
        if (PLAYER(playerIndex).passiveSkillLevelsApplied[slot] != 0)
        {
            skill_passive_applyStatsUpToLevel(playerIndex, slot,
                                              PLAYER(playerIndex).passiveSkillLevelsApplied[slot]);
        }
    }
}

/**
 * Apply the first `level` entries of each stat record of the skill in the player's tree slot.
 *
 * @romaddress 0x08045098
 */
void skill_passive_applyStatsUpToLevel(u8 playerIndex, u8 slot, s8 level)
{
    u8 skillId = PLAYER(playerIndex).passiveSkillIds[slot];
    s32 record;

    for (record = 0; record < 3; record++)
    {
        u8 statIndex = PassiveSkills[skillId].records[record].statIndex;
        s32 i;

        if (statIndex == STAT_NONE)
        {
            continue;
        }

        for (i = 0; i < level; i++)
        {
            u8 value = PassiveSkills[skillId].records[record].valuePerLevel[i];
            skill_passive_addStat(playerIndex, statIndex, value);
        }
    }
}

/**
 * Add count levels to one active skill row's purchased level, then mirror the same count into the
 * current level.
 *
 * @romaddress 0x08045138
 */
void skill_active_addLevels(u8 playerIndex, u8 activeSkillIndex, u8 count)
{
    PLAYER(playerIndex).activeSkillLevelsPurchased[activeSkillIndex] += count;
    skill_active_applyLevels(playerIndex, activeSkillIndex, count);
}

/**
 * Add count levels to one active skill row's current level (not the purchased mirror).
 *
 * @romaddress 0x0804516c
 */
void skill_active_applyLevels(u8 playerIndex, u8 activeSkillIndex, u8 count)
{
    PLAYER(playerIndex).activeSkillLevels[activeSkillIndex] += count;
}

/**
 * Add count to one active skill row's purchased level without changing its current level.
 *
 * @romaddress 0x0804519c
 */
void skill_active_addPurchasedLevels(u8 playerIndex, u8 activeSkillIndex, u8 count)
{
    PLAYER(playerIndex).activeSkillLevelsPurchased[activeSkillIndex] += count;
}

/**
 * Add count to one active skill row's level.
 *
 * @romaddress 0x080451cc
 */
void skill_active_addTemporaryLevels(u8 playerIndex, u8 activeSkillIndex, u8 count)
{
    PLAYER(playerIndex).activeSkillLevels[activeSkillIndex] += count;
}

/**
 * Add count temporary levels to learned passive skills, capped at PASSIVE_SKILL_MAX_LEVEL.
 *
 * @romaddress 0x080451fc
 */
void skill_passive_addTemporaryLevels(u8 playerIndex, u8 count)
{
    s32 slot;
    s32 levelIndex;

    for (levelIndex = 0; levelIndex < count; levelIndex++)
    {
        u8 level;

        for (slot = 0; slot < HERO_PASSIVE_SKILL_COUNT; slot++)
        {
            level = PLAYER(playerIndex).passiveSkillLevelsApplied[slot];
            if (level > 0 && level < PASSIVE_SKILL_MAX_LEVEL)
            {
                PLAYER(playerIndex).passiveSkillLevelsApplied[slot] = level + 1;
                // BUG: passes the skill id where skill_passive_applyLevelStats expects a tree slot
                // (see skill_passive_removeTemporaryLevels), so the wrong skill's records apply.
                skill_passive_applyLevelStats(playerIndex,
                                              PLAYER(playerIndex).passiveSkillIds[slot], 1);
            }
        }
    }
}

/**
 * Add count levels to every active skill row that has a callback.
 *
 * BUG: walks 7 rows instead of HERO_ACTIVE_SKILL_COUNT, so row 6 reads the hero's
 * HeroSkillIds[0..3] as a callback pointer and writes Player.activeSkillLevels[6].
 *
 * @romaddress 0x0804527c
 */
void skill_active_addTemporaryLevelsToAll(u8 playerIndex, u8 count)
{
    s32 i;
    s32 n;

    for (n = 0; n < count; n++)
    {
        for (i = 0; i < HERO_ACTIVE_SKILL_COUNT + 1; i++)
        {
            if (PLAYER_ACTIVE_SKILL(playerIndex, i).callback != NULL)
            {
                PLAYER(playerIndex).activeSkillLevels[i] += 1;
            }
        }
    }
}

/**
 * Remove count levels from every active skill row that has a callback.
 * Callers must balance preceding temporary additions. No floor protects purchased levels or zero.
 *
 * BUG: walks 7 rows instead of HERO_ACTIVE_SKILL_COUNT (see
 * skill_active_addTemporaryLevelsToAll).
 *
 * @romaddress 0x080452dc
 */
void skill_active_removeTemporaryLevelsFromAll(u8 playerIndex, u8 count)
{
    s32 i;
    s32 n;

    for (n = 0; n < count; n++)
    {
        for (i = 0; i < HERO_ACTIVE_SKILL_COUNT + 1; i++)
        {
            if (PLAYER_ACTIVE_SKILL(playerIndex, i).callback != NULL)
            {
                PLAYER(playerIndex).activeSkillLevels[i] -= 1;
            }
        }
    }
}

/**
 * Remove count temporary levels from every passive slot and active skill row.
 * Active removals must balance preceding additions. Only passive removals preserve bought levels.
 *
 * BUG: walks 7 rows instead of HERO_ACTIVE_SKILL_COUNT (see
 * skill_active_addTemporaryLevelsToAll).
 *
 * @romaddress 0x0804533c
 */
void skill_removeTemporaryLevels(u8 playerIndex, u8 count)
{
    s32 i;
    s32 n;

    skill_passive_removeTemporaryLevels(playerIndex, count);

    for (n = 0; n < count; n++)
    {
        for (i = 0; i < HERO_ACTIVE_SKILL_COUNT + 1; i++)
        {
            if (PLAYER_ACTIVE_SKILL(playerIndex, i).callback != NULL)
            {
                removeTemporaryActiveLevel(playerIndex, i);
            }
        }
    }
}

/**
 * Apply (or, with sign == -1, remove) the stat record value of the player's applied level of the
 * skill in the given tree slot.
 *
 * @param playerIndex player whose passive skill stat is being changed
 * @param slot passive skill tree slot
 * @param sign 1 to apply the value, or -1 to remove it
 *
 * @romaddress 0x0804539c
 */
void skill_passive_applyLevelStats(u8 playerIndex, u8 slot, s8 sign)
{
    u8 skillId = PLAYER(playerIndex).passiveSkillIds[slot];
    s32 record;

    for (record = 0; record < 3; record++)
    {
        u8 statIndex = PassiveSkills[skillId].records[record].statIndex;
        u8 value;

        if (statIndex == STAT_NONE)
        {
            continue;
        }

        value = PassiveSkills[skillId]
                    .records[record]
                    .valuePerLevel[(u8)PLAYER(playerIndex).passiveSkillLevelsApplied[slot] - 1];
        if (sign == -1)
        {
            skill_passive_subtractStat(playerIndex, statIndex, value);
        }
        else
        {
            skill_passive_addStat(playerIndex, statIndex, value);
        }
    }
}

/**
 * Route the level-button active skill input: with PLAYER_OPTION_FLAG_QUICK_SKILL_SELECT clear,
 * cycle/cast through selectedActiveSkill, else run the L+button quick select.
 *
 * @romaddress 0x08045458
 */
void skill_active_handleInput(u8 playerIndex)
{
    if ((PLAYER(playerIndex).optionFlags.p & PLAYER_OPTION_FLAG_QUICK_SKILL_SELECT) != 0)
    {
        skill_active_handleQuickSelectInput(playerIndex);
    }
    else
    {
        skill_active_handleCycleInput(playerIndex, FALSE);
    }
}
