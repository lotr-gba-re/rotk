#include "skill.h"
#include "stats.h"
#include "types.h"

// clang-format off

/**
 * One row per PassiveSkillId. STAT_NONE marks an unused stat record.
 *
 * @romaddress 0x0806e3d4
 */
const PassiveSkill PassiveSkills[35] = {
    // [0] FEARLESS
    { .records = {
             { .statIndex = STAT_MELEE_ARMOR, .valuePerLevel = { 1, 1, 1, 1, 1 } },
             { .statIndex = STAT_FEAR_RESIST_PERCENT, .valuePerLevel = { 6, 6, 6, 6, 6 } },
             { .statIndex = STAT_MISSILE_ARMOR, .valuePerLevel = { 1, 1, 1, 1, 1 } },
         },
      .tier = PASSIVE_SKILL_TIER_1,
      .initialRequiredLevel = 2,
      .maxLevel = 5 },

    // [1] ACCURACY
    { .records = {
             { .statIndex = STAT_ACCURACY, .valuePerLevel = { 1, 1, 1, 1, 1 } },
             { .statIndex = STAT_CRIT_DAMAGE, .valuePerLevel = { 3, 3, 3, 3, 3 } },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_1,
      .initialRequiredLevel = 2,
      .maxLevel = 5 },

    // [2] NIMBLE
    { .records = {
             { .statIndex = STAT_DODGE_PERCENT, .valuePerLevel = { 4, 4, 4, 4, 4 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_1,
      .initialRequiredLevel = 2,
      .maxLevel = 5 },

    // [3] DEATH_STRIKE
    { .records = {
             { .statIndex = STAT_CRIT_CHANCE_PERCENT, .valuePerLevel = { 3, 3, 3, 3, 3 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_1,
      .initialRequiredLevel = 2,
      .maxLevel = 5 },

    // [4] BLADEMASTER
    { .records = {
             { .statIndex = STAT_DAMAGE_WITH_BLADE, .valuePerLevel = { 1, 1, 1, 1, 1 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_1,
      .initialRequiredLevel = 2,
      .maxLevel = 5 },

    // [5] AXEMASTER
    { .records = {
             { .statIndex = STAT_DAMAGE_WITH_AXE, .valuePerLevel = { 1, 1, 1, 1, 1 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_1,
      .initialRequiredLevel = 2,
      .maxLevel = 5 },

    // [6] HERB_LORE
    { .records = {
             { .statIndex = STAT_HP_FROM_HERBS, .valuePerLevel = { 10, 10, 10, 10, 10 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_1,
      .initialRequiredLevel = 2,
      .maxLevel = 5 },

    // [7] KEEN_EYES
    { .records = {
             { .statIndex = STAT_EXTRA_GEMS_PERCENT, .valuePerLevel = { 5, 5, 5, 5, 5 } },
             { .statIndex = STAT_EXTRA_TREASURE_PERCENT, .valuePerLevel = { 5, 5, 5, 5, 5 } },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_1,
      .initialRequiredLevel = 2,
      .maxLevel = 5 },

    // [8] RANGEMASTER
    { .records = {
             { .statIndex = STAT_DAMAGE_WITH_BOW, .valuePerLevel = { 1, 1, 1, 1, 1 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_1,
      .initialRequiredLevel = 2,
      .maxLevel = 5 },

    // [9] LUCK
    { .records = {
             { .statIndex = STAT_ALL_PRIMARY_STATS, .valuePerLevel = { 1, 1, 1, 1, 1 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_1,
      .initialRequiredLevel = 2,
      .maxLevel = 5 },

    // [10] DIRTY_CLAWS
    { .records = {
             { .statIndex = STAT_DAMAGE_SLASH, .valuePerLevel = { 4, 4, 4, 4, 4 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_1,
      .initialRequiredLevel = 2,
      .maxLevel = 5 },

    // [11] WHY_DOES_IT_HURT_SMEAGOL
    { .records = {
             { .statIndex = STAT_MELEE_ARMOR, .valuePerLevel = { 4, 4, 4, 4, 4 } },
             { .statIndex = STAT_MISSILE_ARMOR, .valuePerLevel = { 2, 2, 2, 2, 2 } },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_1,
      .initialRequiredLevel = 2,
      .maxLevel = 5 },

    // [12] ORCSLAYER
    { .records = {
             { .statIndex = STAT_DAMAGE_TO_ORCS, .valuePerLevel = { 1, 1, 1, 1, 1 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_2,
      .initialRequiredLevel = 8,
      .maxLevel = 5 },

    // [13] WOODSMAN
    { .records = {
             { .statIndex = STAT_WOODSMAN_DAMAGE, .valuePerLevel = { 2, 3, 2, 3, 2 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_2,
      .initialRequiredLevel = 5,
      .maxLevel = 5 },

    // [14] DWARF_SENSE
    { .records = {
             { .statIndex = STAT_EXTRA_TREASURE_PERCENT, .valuePerLevel = { 10, 15, 10, 15, 10 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_2,
      .initialRequiredLevel = 8,
      .maxLevel = 5 },

    // [15] IRON_WILL
    { .records = {
             { .statIndex = STAT_LOW_HP_REGEN_THRESHOLD_PERCENT, .valuePerLevel = { 10, 10, 10, 10, 10 } },
             { .statIndex = STAT_LOW_HP_REGEN, .valuePerLevel = { 1, 0, 1, 0, 1 } },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_2,
      .initialRequiredLevel = 8,
      .maxLevel = 5 },

    // [16] SPIRIT_OF_MIDDLE_EARTH
    { .records = {
             { .statIndex = STAT_SPIRIT_REGEN_PERCENT, .valuePerLevel = { 20, 20, 20, 20, 20 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_2,
      .initialRequiredLevel = 6,
      .maxLevel = 5 },

    // [17] FIGHTERS_RESOLVE
    { .records = {
             { .statIndex = STAT_LOW_HP_DAMAGE, .valuePerLevel = { 3, 3, 3, 3, 3 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_2,
      .initialRequiredLevel = 8,
      .maxLevel = 5 },

    // [18] SHIELD_OFFENSE
    { .records = {
             { .statIndex = STAT_DAMAGE_WITH_SHIELD, .valuePerLevel = { 1, 1, 1, 1, 1 } },
             { .statIndex = STAT_BLOCK_PERCENT, .valuePerLevel = { 3, 3, 3, 3, 3 } },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_2,
      .initialRequiredLevel = 8,
      .maxLevel = 5 },

    // [19] FLEET_OF_FOOT
    { .records = {
             { .statIndex = STAT_SPEED_PERCENT, .valuePerLevel = { 5, 5, 5, 5, 5 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_2,
      .initialRequiredLevel = 8,
      .maxLevel = 5 },

    // [20] HARDY
    { .records = {
             { .statIndex = STAT_MAX_HP, .valuePerLevel = { 15, 15, 15, 15, 15 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_2,
      .initialRequiredLevel = 4,
      .maxLevel = 5 },

    // [21] WISE
    { .records = {
             { .statIndex = STAT_EXTRA_EXP_PERCENT, .valuePerLevel = { 3, 3, 3, 3, 3 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_3,
      .initialRequiredLevel = 15,
      .maxLevel = 5 },

    // [22] WISDOM_OF_THE_AGES
    { .records = {
             { .statIndex = STAT_RANGED_DAMAGE, .valuePerLevel = { 2, 2, 2, 2, 2 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_3,
      .initialRequiredLevel = 15,
      .maxLevel = 5 },

    // [23] BATTLE_SCARRED
    { .records = {
             { .statIndex = STAT_MELEE_ARMOR, .valuePerLevel = { 1, 2, 1, 2, 1 } },
             { .statIndex = STAT_MISSILE_ARMOR, .valuePerLevel = { 2, 1, 2, 1, 2 } },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_3,
      .initialRequiredLevel = 15,
      .maxLevel = 5 },

    // [24] LAST_STAND
    { .records = {
             { .statIndex = STAT_REVIVE_CHANCE_PERCENT, .valuePerLevel = { 8, 8, 8, 8, 8 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_3,
      .initialRequiredLevel = 15,
      .maxLevel = 5 },

    // [25] ARROW_PARRY
    { .records = {
             { .statIndex = STAT_ARROW_PARRY_PERCENT, .valuePerLevel = { 6, 6, 6, 6, 6 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_3,
      .initialRequiredLevel = 15,
      .maxLevel = 5 },

    // [26] WRAITHSLAYER
    { .records = {
             { .statIndex = STAT_DAMAGE_TO_NAZGUL, .valuePerLevel = { 4, 3, 4, 3, 4 } },
             { .statIndex = STAT_FEAR_RESIST_PERCENT, .valuePerLevel = { 6, 6, 6, 6, 6 } },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_3,
      .initialRequiredLevel = 10,
      .maxLevel = 5 },

    // [27] GALADRIELS_BLESSING
    { .records = {
             { .statIndex = STAT_ARROW_SPEED_PERCENT, .valuePerLevel = { 10, 10, 10, 10, 10 } },
             { .statIndex = STAT_CRIT_CHANCE_PERCENT, .valuePerLevel = { 3, 3, 3, 3, 3 } },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_3,
      .initialRequiredLevel = 15,
      .maxLevel = 5 },

    // [28] BERSERKER
    { .records = {
             { .statIndex = STAT_HP_PER_KILL, .valuePerLevel = { 4, 4, 4, 4, 4 } },
             { .statIndex = STAT_SPIRIT_PER_KILL, .valuePerLevel = { 4, 4, 4, 4, 4 } },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_3,
      .initialRequiredLevel = 15,
      .maxLevel = 5 },

    // [29] SERVANT_OF_THE_SECRET_FIRE
    { .records = {
             { .statIndex = STAT_ACTIVE_SKILL_LEVEL_BONUS, .valuePerLevel = { 1, 0, 0, 0, 0 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_4,
      .initialRequiredLevel = 20,
      .maxLevel = 1 },

    // [30] RAGE_OF_THE_NORTH
    { .records = {
             { .statIndex = STAT_INSTAKILL_CHANCE_PERCENT, .valuePerLevel = { 10, 0, 0, 0, 0 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_4,
      .initialRequiredLevel = 20,
      .maxLevel = 1 },

    // [31] ARCHER_OF_MIRKWOOD
    { .records = {
             { .statIndex = STAT_EXTRA_PROJECTILES, .valuePerLevel = { 1, 0, 0, 0, 0 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_4,
      .initialRequiredLevel = 20,
      .maxLevel = 1 },

    // [32] GLOINS_DOUBLE_AXES
    { .records = {
             { .statIndex = STAT_DOUBLE_AXE_THROW, .valuePerLevel = { 1, 0, 0, 0, 0 } },
             { .statIndex = STAT_NONE },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_4,
      .initialRequiredLevel = 20,
      .maxLevel = 1 },

    // [33] DEFENDERS_FURY
    { .records = {
             { .statIndex = STAT_INVULNERABILITY_PROC_PERCENT, .valuePerLevel = { 10, 0, 0, 0, 0 } },
             { .statIndex = STAT_UNKNOWN_81, .valuePerLevel = { 8, 0, 0, 0, 0 } },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_4,
      .initialRequiredLevel = 20,
      .maxLevel = 1 },

    // [34] THE_PRECIOUS
    { .records = {
             { .statIndex = STAT_TRIPLE_DAMAGE_PROC_PERCENT, .valuePerLevel = { 10, 0, 0, 0, 0 } },
             { .statIndex = STAT_TRIPLE_DAMAGE_PROC_PERCENT, .valuePerLevel = { 10, 0, 0, 0, 0 } },
             { .statIndex = STAT_NONE },
         },
      .tier = PASSIVE_SKILL_TIER_4,
      .initialRequiredLevel = 20,
      .maxLevel = 1 },
};
// clang-format on
