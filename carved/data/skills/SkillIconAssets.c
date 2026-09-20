#include "skill.h"
#include "variables.h"

// clang-format off

/**
 * Raw 32x24 icon asset per PassiveSkillId.
 *
 * @romaddress 0x082876b0
 */
const void *const PassiveSkillIconAssets[PASSIVE_SKILL_COUNT] = {
    [PASSIVE_SKILL_FEARLESS] = PassiveSkillFearlessIcon,
    [PASSIVE_SKILL_ACCURACY] = PassiveSkillAccuracyIcon,
    [PASSIVE_SKILL_NIMBLE] = PassiveSkillNimbleIcon,
    [PASSIVE_SKILL_DEATH_STRIKE] = PassiveSkillDeathStrikeIcon,
    [PASSIVE_SKILL_BLADEMASTER] = PassiveSkillBlademasterIcon,
    [PASSIVE_SKILL_AXEMASTER] = PassiveSkillAxemasterIcon,
    [PASSIVE_SKILL_HERB_LORE] = PassiveSkillHerbLoreIcon,
    [PASSIVE_SKILL_KEEN_EYES] = PassiveSkillKeenEyesIcon,
    [PASSIVE_SKILL_RANGEMASTER] = PassiveSkillRangemasterIcon,
    [PASSIVE_SKILL_LUCK] = PassiveSkillLuckIcon,
    [PASSIVE_SKILL_DIRTY_CLAWS] = PassiveSkillDirtyClawsIcon,
    [PASSIVE_SKILL_WHY_DOES_IT_HURT_SMEAGOL] = PassiveSkillWhyDoesItHurtSmeagolIcon,
    [PASSIVE_SKILL_ORCSLAYER] = PassiveSkillOrcslayerIcon,
    [PASSIVE_SKILL_WOODSMAN] = PassiveSkillWoodsmanIcon,
    [PASSIVE_SKILL_DWARF_SENSE] = PassiveSkillDwarfSenseIcon,
    [PASSIVE_SKILL_IRON_WILL] = PassiveSkillIronWillIcon,
    [PASSIVE_SKILL_SPIRIT_OF_MIDDLE_EARTH] = PassiveSkillSpiritOfMiddleEarthIcon,
    [PASSIVE_SKILL_FIGHTERS_RESOLVE] = PassiveSkillFightersResolveIcon,
    [PASSIVE_SKILL_SHIELD_OFFENSE] = PassiveSkillShieldOffenseIcon,
    [PASSIVE_SKILL_FLEET_OF_FOOT] = PassiveSkillFleetOfFootIcon,
    [PASSIVE_SKILL_HARDY] = PassiveSkillHardyIcon,
    [PASSIVE_SKILL_WISE] = PassiveSkillWiseIcon,
    [PASSIVE_SKILL_WISDOM_OF_THE_AGES] = PassiveSkillWisdomOfTheAgesIcon,
    [PASSIVE_SKILL_BATTLE_SCARRED] = PassiveSkillBattleScarredIcon,
    [PASSIVE_SKILL_LAST_STAND] = PassiveSkillLastStandIcon,
    [PASSIVE_SKILL_ARROW_PARRY] = PassiveSkillArrowParryIcon,
    [PASSIVE_SKILL_WRAITHSLAYER] = PassiveSkillWraithslayerIcon,
    [PASSIVE_SKILL_GALADRIELS_BLESSING] = PassiveSkillGaladrielsBlessingIcon,
    [PASSIVE_SKILL_BERSERKER] = PassiveSkillBerserkerIcon,
    [PASSIVE_SKILL_SERVANT_OF_THE_SECRET_FIRE] = PassiveSkillServantOfTheSecretFireIcon,
    [PASSIVE_SKILL_RAGE_OF_THE_NORTH] = PassiveSkillRageOfTheNorthIcon,
    [PASSIVE_SKILL_ARCHER_OF_MIRKWOOD] = PassiveSkillArcherOfMirkwoodIcon,
    [PASSIVE_SKILL_GLOINS_DOUBLE_AXES] = PassiveSkillGloinsDoubleAxesIcon,
    [PASSIVE_SKILL_DEFENDERS_FURY] = PassiveSkillDefendersFuryIcon,
    [PASSIVE_SKILL_THE_PRECIOUS] = PassiveSkillThePreciousIcon,
};

/**
 * Six raw 32x24 active skill icon assets per hero, in active skill table
 * ROM order rather than HeroId order.
 *
 * @romaddress 0x0828773c
 */
const void *const ActiveSkillIconAssets[8][HERO_ACTIVE_SKILL_COUNT] = {
    // Frodo
    {ActiveSkillFrodoKnifeTossIcon, ActiveSkillFrodoSnareIcon, ActiveSkillFrodoGaladrielsCloakIcon, ActiveSkillFrodoTheOneRingIcon, ActiveSkillFrodoRingsPersuasionIcon, ActiveSkillFrodoHerbalHealingIcon},
    // Sam
    {ActiveSkillSamKnifeTossIcon, ActiveSkillSamSnareIcon, ActiveSkillSamGaladrielsCloakIcon, ActiveSkillSamCookpotSmashIcon, ActiveSkillSamSamwiseTheStrongIcon, ActiveSkillFrodoHerbalHealingIcon},
    // Smeagol
    {ActiveSkillSmeagolBerserkAttackIcon, ActiveSkillSmeagolRockThrowIcon, ActiveSkillSmeagolCowerIcon, ActiveSkillSmeagolGollumIcon, ActiveSkillSmeagolPitifulWailIcon, ActiveSkillFrodoHerbalHealingIcon},
    // Legolas
    {ActiveSkillLegolasWhiteKnivesIcon, ActiveSkillLegolasSpreadFireIcon, ActiveSkillLegolasFriendOfMirkwoodIcon, ActiveSkillLegolasForagingIcon, ActiveSkillLegolasSilentStrideIcon, ActiveSkillLegolasHerbalHealingIcon},
    // Aragorn
    {ActiveSkillAragornSweepIcon, ActiveSkillAragornKingsCommandIcon, ActiveSkillAragornSwordThrowIcon, ActiveSkillAragornNumenoreanWillIcon, ActiveSkillAragornCallOfTheDeadIcon, ActiveSkillAragornHerbalHealingIcon},
    // Gandalf
    {ActiveSkillGandalfSwordOfPowerIcon, ActiveSkillGandalfLightstrikeIcon, ActiveSkillGandalfShieldIcon, ActiveSkillGandalfBlindingAuraIcon, ActiveSkillGandalfSummonGwaihirIcon, ActiveSkillGandalfHerbalHealingIcon},
    // Eowyn
    {ActiveSkillEowynDoubleStrikeIcon, ActiveSkillEowynShieldmaidenOfRohanIcon, ActiveSkillEowynRohanSprintIcon, ActiveSkillEowynShieldBashIcon, ActiveSkillEowynForthEorlingasIcon, ActiveSkillEowynHerbalHealingIcon},
    // Gimli
    {ActiveSkillGimliAxeThrowIcon, ActiveSkillGimliWhirlingAttackIcon, ActiveSkillGimliDwarvenRageIcon, ActiveSkillGimliStoicismIcon, ActiveSkillGimliEarthShatterIcon, ActiveSkillGimliHerbalHealingIcon},
};
// clang-format on
