#pragma once

#include "types.h"

// Hero special-move / spell system. Each Player carries a pointer to its hero's active skill
// data table (Player.activeSkillTable): rows of 0x28 bytes per active skill, each holding value
// records of 0xc bytes ({..., s16 base @+8, s16 perLevelIncrement[5] @+10} - the increments
// overrun into the following record, so the rows are not clean sub-structs). Current active skill
// levels live in Player.activeSkillLevels[].

/**
 * Active skill index with a known consumer: combat_computePveDamage uses active skill 3 value 0 as
 * the melee damage of attack state ACTOR_STATE_ATK_ACTIVE_SKILL3. Which move this is is
 * unconfirmed.
 */
#define ACTIVE_SKILL_UNKNOWN_3 3

/**
 * An active skill's leveled value: base + the first `level` per-level increments (one extra level,
 * capped at 5, while STAT_ACTIVE_SKILL_LEVEL_BONUS is positive). Returns the summed s16 value.
 */
s16 activeSkill_getLeveledValue(u8 playerIndex, u8 activeSkillIndex, u8 valueIndex, u8 level);

/** Every hero's row 5 is Herbal Healing; it is cast directly instead of through the cast state. */
#define ACTIVE_SKILL_HERBAL_HEALING 5

/** Player.selectedActiveSkill sentinel: nothing on the cast button. */
#define ACTIVE_SKILL_NONE 6

/**
 * Whether the player may cast an active skill now: learned, enough spirit, the actor not already in
 * a cast state, plus per-hero gear checks (bow for arrow active skills, herbs for Herbal Healing).
 */
bool activeSkill_canCast(u8 playerIndex, u8 activeSkillIndex);

/**
 * Pay the active skill's spirit cost and start casting it: Herbal Healing runs its callback at
 * once, every other row enters the cast action state with Player.castingActiveSkill set.
 */
void activeSkill_cast(u8 playerIndex, u8 activeSkillIndex);
