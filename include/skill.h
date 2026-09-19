#pragma once

#include "actor.h"
#include "types.h"

// PassiveSkillId values index PassiveSkills[].
// Each hero's skill tree contains nine passive-skill IDs.
// Skill-name text IDs are TEXT_ID_SKILL_FEARLESS + passiveSkillId.
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

// Passive skills per hero skill tree.
#define HERO_PASSIVE_SKILL_COUNT 9

// Tree-slot sentinel when the hero lacks the passive skill.
#define PASSIVE_SKILL_SLOT_NONE 9

#define PASSIVE_SKILL_MAX_LEVEL 5

// One stat bonus of a skill.
typedef struct PassiveSkillStatRecord
{
    // StatIndex. STAT_NONE marks an unused record.
    u8 statIndex;

    // Stat increments for levels 1 through 5.
    u16 valuePerLevel[PASSIVE_SKILL_MAX_LEVEL];
} PassiveSkillStatRecord;

// Values stored in PassiveSkill.tier. This mechanism seems unused.
typedef enum PassiveSkillTier
{
    PASSIVE_SKILL_TIER_1 = 1 << 0,
    PASSIVE_SKILL_TIER_2 = 1 << 1,
    PASSIVE_SKILL_TIER_3 = 1 << 2,
    PASSIVE_SKILL_TIER_4 = 1 << 3,
} __attribute__((packed)) PassiveSkillTier;

// A passive skill row with trailing u16 alignment padding after maxLevel.
typedef struct PassiveSkill
{
    PassiveSkillStatRecord records[3];

    // Passive skill tier value. Probably unused.
    PassiveSkillTier tier;

    // Hero level required to buy the first skill level.
    u8 initialRequiredLevel;

    // PASSIVE_SKILL_MAX_LEVEL, or 1 for a hero-unique skill.
    u8 maxLevel;
} PassiveSkill;

u8 skill_passive_findSlot(u8 playerIndex, u8 passiveSkillId);

bool skill_passive_addLevels(u8 playerIndex, u8 passiveSkillId, u8 count);

bool skill_passive_applyLevels(u8 playerIndex, u8 passiveSkillId, u8 count);

void skill_passive_applyLevelStats(u8 playerIndex, u8 slot, s8 sign);

void skill_passive_applyStatsUpToLevel(u8 playerIndex, u8 slot, s8 level);

void skill_passive_applyAllLevels(u8 playerIndex);

bool skill_passive_removeLevels(u8 playerIndex, u8 passiveSkillId, u8 count);

bool skill_passive_removeAppliedLevels(u8 playerIndex, u8 passiveSkillId, u8 count);

void skill_passive_addStat(u8 playerIndex, u8 statIndex, s16 delta);

/** Remove a passive skill's stat contribution. */
static inline void skill_passive_subtractStat(u8 playerIndex, u8 statIndex, s32 value)
{
    s16 delta = -value;
    skill_passive_addStat(playerIndex, statIndex, delta);
}

// Active skill rows per hero.
#define HERO_ACTIVE_SKILL_COUNT 6

// Per-level increments in an active skill's value curve and the purchase cap.
#define ACTIVE_SKILL_MAX_LEVEL 5

// Two value records per row. Their meanings vary by skill.
#define ACTIVE_SKILL_VALUE_COUNT 2

// Active skill callback. The actor parameter is the caster.
typedef void (*ActiveSkillCallback)(Actor *actor);

typedef struct ActiveSkillValue
{
    s16 base;
    s16 perLevel[ACTIVE_SKILL_MAX_LEVEL];
} ActiveSkillValue;

enum ActiveSkillCastFlag
{
    // Set: the cast state requests pose 0. Clear: use castingActiveSkill + 1.
    ACTIVE_SKILL_CAST_FLAG_FORCE_POSE_0 = 1 << 0,

    // Set: keep the callback's state as the next state. Clear: enter cast recovery state 0xa.
    ACTIVE_SKILL_CAST_FLAG_KEEP_CALLBACK_STATE = 1 << 1,
};

typedef union ActiveSkillCastFlags {
    u8 p;

    struct
    {
        u8 forcePose0 : 1;        // 1 << 0
        u8 keepCallbackState : 1; // 1 << 1
    } __attribute__((packed)) d;
} __attribute__((packed)) ActiveSkillCastFlags;

// One row of a hero's skill block. Each row is 0x28 bytes.
// A null callback marks a row outside the hero's tree.
typedef struct ActiveSkill
{
    ActiveSkillCallback callback;

    // Spirit cost for casting this skill.
    u16 spiritCost;

    u16 field_0x06;

    // The row's two s16 level curves.
    ActiveSkillValue values[ACTIVE_SKILL_VALUE_COUNT];

    u8 field_0x20;

    // Flags controlling the cast pose and the state after the callback.
    ActiveSkillCastFlags castFlags;

    u8 field_0x22;

    // Cast animation frame at which the callback runs. Zero runs it immediately.
    u8 castTriggerFrame;

    u8 field_0x24;
} ActiveSkill;

// The player's active skill at activeSkillIndex.
#define PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex)                                         \
    (PLAYER(playerIndex).activeSkills[activeSkillIndex])

// Six active skill rows followed by nine passive skill IDs.
// ROM blocks have a 0xfc stride, but Gimli's final block ends after its IDs at 0x0806f129,
// without this type's trailing padding.
typedef struct HeroSkillBlock
{
    ActiveSkill activeSkills[HERO_ACTIVE_SKILL_COUNT];

    // PassiveSkillId values in the hero's skill-tree order.
    u8 passiveSkillIds[HERO_PASSIVE_SKILL_COUNT];
} HeroSkillBlock;

// Placeholder for active skill row 3.
#define ACTIVE_SKILL_UNKNOWN_3 3

// Every hero's row 5 is Herbal Healing. It is cast directly instead of through the cast state.
#define ACTIVE_SKILL_HERBAL_HEALING 5

// Active skill selection sentinel: nothing on the cast button.
#define ACTIVE_SKILL_NONE 6

s16 skill_active_getLeveledValue(u8 playerIndex, u8 activeSkillIndex, u8 valueIndex, u8 levelCount);

/**
 * Whether the player may cast an active skill now: learned, enough spirit, the actor not already in
 * a cast state, plus per-hero gear checks (bow for arrow active skills, herbs for Herbal Healing).
 */
bool skill_active_canCast(u8 playerIndex, u8 activeSkillIndex);

void skill_active_cast(u8 playerIndex, u8 activeSkillIndex);

void skill_active_addLevels(u8 playerIndex, u8 activeSkillIndex, u8 count);

void skill_active_applyLevels(u8 playerIndex, u8 activeSkillIndex, u8 count);

void skill_active_addPurchasedLevels(u8 playerIndex, u8 activeSkillIndex, u8 count);

void skill_active_handleInput(u8 playerIndex);

void skill_active_handleCycleInput(u8 playerIndex, bool fromQuickSelect);

void skill_active_handleQuickSelectInput(u8 playerIndex);

// One callback per distinct active skill. The function name identifies the hero whose row defines
// the skill. Frodo's Knife Toss, Snare and Galadriel's Cloak callbacks are also used by Sam.
void skill_active_frodoKnifeToss(Actor *actor);
void skill_active_frodoSnare(Actor *actor);
void skill_active_frodoGaladrielsCloak(Actor *actor);
void skill_active_frodoTheOneRing(Actor *actor);
void skill_active_frodoRingsPersuasion(Actor *actor);
void skill_active_herbalHealing(Actor *actor);
void skill_active_samCookpotSmash(Actor *actor);
void skill_active_samSamwiseTheStrong(Actor *actor);
void skill_active_smeagolBerserkAttack(Actor *actor);
void skill_active_smeagolRockThrow(Actor *actor);
void skill_active_smeagolCower(Actor *actor);
void skill_active_smeagolGollum(Actor *actor);
void skill_active_smeagolPitifulWail(Actor *actor);
void skill_active_legolasWhiteKnives(Actor *actor);
void skill_active_legolasSpreadFire(Actor *actor);
void skill_active_legolasFriendOfMirkwood(Actor *actor);
void skill_active_legolasForaging(Actor *actor);
void skill_active_legolasSilentStride(Actor *actor);
void skill_active_aragornSweep(Actor *actor);
void skill_active_aragornKingsCommand(Actor *actor);
void skill_active_aragornSwordThrow(Actor *actor);
void skill_active_aragornNumenoreanWill(Actor *actor);
void skill_active_aragornCallOfTheDead(Actor *actor);
void skill_active_gandalfSwordOfPower(Actor *actor);
void skill_active_gandalfLightstrike(Actor *actor);
void skill_active_gandalfShield(Actor *actor);
void skill_active_gandalfBlindingAura(Actor *actor);
void skill_active_gandalfSummonGwaihir(Actor *actor);
void skill_active_eowynDoubleStrike(Actor *actor);
void skill_active_eowynShieldmaidenOfRohan(Actor *actor);
void skill_active_eowynRohanSprint(Actor *actor);
void skill_active_eowynShieldBash(Actor *actor);
void skill_active_eowynForthEorlingas(Actor *actor);
void skill_active_gimliAxeThrow(Actor *actor);
void skill_active_gimliWhirlingAttack(Actor *actor);
void skill_active_gimliDwarvenRage(Actor *actor);
void skill_active_gimliStoicism(Actor *actor);
void skill_active_gimliEarthShatter(Actor *actor);

// Temporary skill levels alter the applied passive levels or active skill levels without changing
// the purchased levels.

void skill_addTemporaryLevels(u8 playerIndex, u8 count);

void skill_active_addTemporaryLevels(u8 playerIndex, u8 activeSkillIndex, u8 count);

void skill_passive_addTemporaryLevels(u8 playerIndex, u8 count);

void skill_passive_removeTemporaryLevels(u8 playerIndex, u8 count);

void skill_active_addTemporaryLevelsToAll(u8 playerIndex, u8 count);

void skill_active_removeTemporaryLevelsFromAll(u8 playerIndex, u8 count);

void skill_removeTemporaryLevels(u8 playerIndex, u8 count);
