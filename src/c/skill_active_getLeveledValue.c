#include "skill.h"
#include "stats.h"
#include "variables.h"

/**
 * An active skill's leveled value: base + the first `levelCount` per-level increments, plus one
 * extra increment capped at ACTIVE_SKILL_MAX_LEVEL while STAT_ACTIVE_SKILL_LEVEL_BONUS is positive.
 * Returns the summed s16 value.
 *
 * @param playerIndex player whose active skill is being queried
 * @param activeSkillIndex active skill row to query
 * @param valueIndex level curve within the active skill row
 * @param levelCount number of purchased or temporary levels
 *
 * @romaddress 0x08043764
 */
s16 skill_active_getLeveledValue(u8 playerIndex, u8 activeSkillIndex, u8 valueIndex, u8 levelCount)
{
    u16 total = PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex).values[valueIndex].base;
    s32 level;

#ifdef BUGFIX
    if (levelCount > ACTIVE_SKILL_MAX_LEVEL)
    {
        levelCount = ACTIVE_SKILL_MAX_LEVEL;
    }
#else
    // BUG: a temporary level above the cap reads beyond ActiveSkillValue.perLevel.
#endif

    for (level = 0; level < levelCount; level++)
    {
        total +=
            PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex).values[valueIndex].perLevel[level];
    }

    if (PLAYER(playerIndex).stats[STAT_ACTIVE_SKILL_LEVEL_BONUS] > 0)
    {
        if (level < ACTIVE_SKILL_MAX_LEVEL)
        {
            total += PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex)
                         .values[valueIndex]
                         .perLevel[level];
        }
        else
        {
            total += PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex)
                         .values[valueIndex]
                         .perLevel[ACTIVE_SKILL_MAX_LEVEL - 1];
        }
    }
    return total;
}
