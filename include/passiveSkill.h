#pragma once

#include "types.h"

/**
 * Passive skill ids: rows of PassiveSkills[]. A hero's skill tree is its 9-entry
 * HeroPassiveSkillIds* list (Player.passiveSkillIds); the name text is TEXT_ID_SKILL_FEARLESS +
 * passiveSkillId.
 */
enum PassiveSkillId
{
    PASSIVE_SKILL_FEARLESS = 0,
    PASSIVE_SKILL_ACCURACY = 1,
    PASSIVE_SKILL_NIMBLE = 2,
    PASSIVE_SKILL_DEATH_STRIKE = 3,
    PASSIVE_SKILL_BLADEMASTER = 4,
    PASSIVE_SKILL_AXEMASTER = 5,
    PASSIVE_SKILL_HERB_LORE = 6,
    PASSIVE_SKILL_KEEN_EYES = 7,
    PASSIVE_SKILL_RANGEMASTER = 8,
    PASSIVE_SKILL_LUCK = 9,
    PASSIVE_SKILL_DIRTY_CLAWS = 10,
    PASSIVE_SKILL_WHY_DOES_IT_HURT_SMEAGOL = 11,
    PASSIVE_SKILL_ORCSLAYER = 12,
    PASSIVE_SKILL_WOODSMAN = 13,
    PASSIVE_SKILL_DWARF_SENSE = 14,
    PASSIVE_SKILL_IRON_WILL = 15,
    PASSIVE_SKILL_SPIRIT_OF_MIDDLE_EARTH = 16,
    PASSIVE_SKILL_FIGHTERS_RESOLVE = 17,
    PASSIVE_SKILL_SHIELD_OFFENSE = 18,
    PASSIVE_SKILL_FLEET_OF_FOOT = 19,
    PASSIVE_SKILL_HARDY = 20,
    PASSIVE_SKILL_WISE = 21,
    PASSIVE_SKILL_WISDOM_OF_THE_AGES = 22,
    PASSIVE_SKILL_BATTLE_SCARRED = 23,
    PASSIVE_SKILL_LAST_STAND = 24,
    PASSIVE_SKILL_ARROW_PARRY = 25,
    PASSIVE_SKILL_WRAITHSLAYER = 26,
    PASSIVE_SKILL_GALADRIELS_BLESSING = 27,
    PASSIVE_SKILL_BERSERKER = 28,
    PASSIVE_SKILL_SERVANT_OF_THE_SECRET_FIRE = 29,
    PASSIVE_SKILL_RAGE_OF_THE_NORTH = 30,
    PASSIVE_SKILL_ARCHER_OF_MIRKWOOD = 31,
    PASSIVE_SKILL_GLOINS_DOUBLE_AXES = 32,
    PASSIVE_SKILL_DEFENDERS_FURY = 33,
    PASSIVE_SKILL_THE_PRECIOUS = 34,
    PASSIVE_SKILL_COUNT = 35,
};

/** Passive skills per hero skill tree (Player.passiveSkillIds, Player.passiveSkillLevels). */
#define HERO_PASSIVE_SKILL_COUNT 9

/** passiveSkill_findSlot result when the hero's tree lacks the skill. */
#define PASSIVE_SKILL_SLOT_NONE 9

#define PASSIVE_SKILL_MAX_LEVEL 5

/** One stat bonus of a skill. */
typedef struct PassiveSkillStatRecord
{
    u8 statIndex; // StatIndex; STAT_NONE = unused record
    u8 field_0x1;

    /** added to stats[statIndex] on reaching level 1..5; only the low byte is read */
    u16 valuePerLevel[PASSIVE_SKILL_MAX_LEVEL];
} PassiveSkillStatRecord;

typedef struct PassiveSkill
{
    PassiveSkillStatRecord records[3];

    /** 1/2/4/8 across the four skill tiers; reader not yet traced */
    u8 field_0x24;

    /** 2..20, grows with field_0x24 (hero-unique skills: 20); reader not yet traced */
    u8 field_0x25;

    u8 maxLevel; // 5, or 1 for the hero-unique skills
    u8 field_0x27;
} PassiveSkill;

/** Slot of passiveSkillId in the player's skill tree, or PASSIVE_SKILL_SLOT_NONE. */
u32 passiveSkill_findSlot(u8 playerIndex, u8 passiveSkillId);

/**
 * Buy count levels of passiveSkillId for the player (capped at PASSIVE_SKILL_MAX_LEVEL and the
 * skill's maxLevel): raises Player.passiveSkillLevels and applies the stat records of each new
 * level.
 *
 * @return FALSE when the hero's tree lacks the skill or it is already maxed
 */
bool passiveSkill_addLevels(u8 playerIndex, u8 passiveSkillId, u8 count);

/** Re-apply every skill slot's Player.passiveSkillLevelsApplied to stats[] (character load). */
void passiveSkill_applyAllLevels(u8 playerIndex);
