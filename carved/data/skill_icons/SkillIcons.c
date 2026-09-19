#include "gfx.h"
#include "gfx/skill_icons/active.inc"
#include "gfx/skill_icons/passive.inc"

// clang-format off

/**
 * Passive and active skill icons: raw 32x24 4bpp BG tiles without a
 * palette or tilemap, in ROM order.
 *
 * @romaddress 0x0878271c
 */

// [0] "Fearless"
BG_ASSET_TILES_ONLY(PassiveSkillFearlessIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [1] "Accuracy"
BG_ASSET_TILES_ONLY(PassiveSkillAccuracyIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [2] "Nimble"
BG_ASSET_TILES_ONLY(PassiveSkillNimbleIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [3] "Death Strike"
BG_ASSET_TILES_ONLY(PassiveSkillDeathStrikeIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [4] "Blademaster"
BG_ASSET_TILES_ONLY(PassiveSkillBlademasterIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [5] "Axemaster"
BG_ASSET_TILES_ONLY(PassiveSkillAxemasterIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [6] "Herb Lore"
BG_ASSET_TILES_ONLY(PassiveSkillHerbLoreIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [7] "Keen Eyes"
BG_ASSET_TILES_ONLY(PassiveSkillKeenEyesIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [8] "Rangemaster"
BG_ASSET_TILES_ONLY(PassiveSkillRangemasterIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [9] "Luck"
BG_ASSET_TILES_ONLY(PassiveSkillLuckIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [10] "Dirty Claws"
BG_ASSET_TILES_ONLY(PassiveSkillDirtyClawsIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [11] "Why Does it Hurt Smeagol?"
BG_ASSET_TILES_ONLY(PassiveSkillWhyDoesItHurtSmeagolIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [12] "Orcslayer"
BG_ASSET_TILES_ONLY(PassiveSkillOrcslayerIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [13] "Woodsman"
BG_ASSET_TILES_ONLY(PassiveSkillWoodsmanIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [14] "Dwarf Sense"
BG_ASSET_TILES_ONLY(PassiveSkillDwarfSenseIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [15] "Iron Will"
BG_ASSET_TILES_ONLY(PassiveSkillIronWillIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [16] "Spirit of Middle-earth"
BG_ASSET_TILES_ONLY(PassiveSkillSpiritOfMiddleEarthIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [17] "Fighter's Resolve"
BG_ASSET_TILES_ONLY(PassiveSkillFightersResolveIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [18] "Shield Offense"
BG_ASSET_TILES_ONLY(PassiveSkillShieldOffenseIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [19] "Fleet of Foot"
BG_ASSET_TILES_ONLY(PassiveSkillFleetOfFootIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [20] "Hardy"
BG_ASSET_TILES_ONLY(PassiveSkillHardyIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [21] "Wise"
BG_ASSET_TILES_ONLY(PassiveSkillWiseIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [22] "Wisdom of the Ages"
BG_ASSET_TILES_ONLY(PassiveSkillWisdomOfTheAgesIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [23] "Battle Scarred"
BG_ASSET_TILES_ONLY(PassiveSkillBattleScarredIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [24] "Last Stand"
BG_ASSET_TILES_ONLY(PassiveSkillLastStandIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [25] "Arrow Parry"
BG_ASSET_TILES_ONLY(PassiveSkillArrowParryIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [26] "Wraithslayer"
BG_ASSET_TILES_ONLY(PassiveSkillWraithslayerIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [27] "Galadriel's Blessing"
BG_ASSET_TILES_ONLY(PassiveSkillGaladrielsBlessingIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [28] "Berserker"
BG_ASSET_TILES_ONLY(PassiveSkillBerserkerIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [29] "Servant of the Secret Fire"
BG_ASSET_TILES_ONLY(PassiveSkillServantOfTheSecretFireIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [30] "Rage of the North"
BG_ASSET_TILES_ONLY(PassiveSkillRageOfTheNorthIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [31] "Archer of Mirkwood"
BG_ASSET_TILES_ONLY(PassiveSkillArcherOfMirkwoodIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [32] "Gloin's Double Axes"
BG_ASSET_TILES_ONLY(PassiveSkillGloinsDoubleAxesIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [33] "Defender's Fury"
BG_ASSET_TILES_ONLY(PassiveSkillDefendersFuryIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// [34] "The Precious"
BG_ASSET_TILES_ONLY(PassiveSkillThePreciousIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Frodo[0] "Knife Toss"
BG_ASSET_TILES_ONLY(ActiveSkillFrodoKnifeTossIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Frodo[1] "Snare"
BG_ASSET_TILES_ONLY(ActiveSkillFrodoSnareIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Frodo[2] "Galadriel's Cloak"
BG_ASSET_TILES_ONLY(ActiveSkillFrodoGaladrielsCloakIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Frodo[3] "The One Ring"
BG_ASSET_TILES_ONLY(ActiveSkillFrodoTheOneRingIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Frodo[4] "Ring's Persuasion"
BG_ASSET_TILES_ONLY(ActiveSkillFrodoRingsPersuasionIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Frodo[5] "Herbal Healing" (also Sam, Smeagol)
BG_ASSET_TILES_ONLY(ActiveSkillFrodoHerbalHealingIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Sam[0] "Knife Toss"
BG_ASSET_TILES_ONLY(ActiveSkillSamKnifeTossIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Sam[1] "Snare"
BG_ASSET_TILES_ONLY(ActiveSkillSamSnareIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Sam[2] "Galadriel's Cloak"
BG_ASSET_TILES_ONLY(ActiveSkillSamGaladrielsCloakIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Sam[3] "Cookpot Smash"
BG_ASSET_TILES_ONLY(ActiveSkillSamCookpotSmashIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Sam[4] "Samwise the Strong"
BG_ASSET_TILES_ONLY(ActiveSkillSamSamwiseTheStrongIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Smeagol[0] "Berserk Attack"
BG_ASSET_TILES_ONLY(ActiveSkillSmeagolBerserkAttackIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Smeagol[1] "Rock Throw"
BG_ASSET_TILES_ONLY(ActiveSkillSmeagolRockThrowIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Smeagol[2] "Cower"
BG_ASSET_TILES_ONLY(ActiveSkillSmeagolCowerIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Smeagol[3] "Gollum!"
BG_ASSET_TILES_ONLY(ActiveSkillSmeagolGollumIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Smeagol[4] "Pitiful Wail"
BG_ASSET_TILES_ONLY(ActiveSkillSmeagolPitifulWailIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Legolas[0] "White Knives"
BG_ASSET_TILES_ONLY(ActiveSkillLegolasWhiteKnivesIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Legolas[1] "Spread Fire"
BG_ASSET_TILES_ONLY(ActiveSkillLegolasSpreadFireIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Legolas[2] "Friend of Mirkwood"
BG_ASSET_TILES_ONLY(ActiveSkillLegolasFriendOfMirkwoodIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Legolas[3] "Foraging"
BG_ASSET_TILES_ONLY(ActiveSkillLegolasForagingIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Legolas[4] "Silent Stride"
BG_ASSET_TILES_ONLY(ActiveSkillLegolasSilentStrideIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Legolas[5] "Herbal Healing"
BG_ASSET_TILES_ONLY(ActiveSkillLegolasHerbalHealingIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Aragorn[0] "Sweep"
BG_ASSET_TILES_ONLY(ActiveSkillAragornSweepIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Aragorn[1] "King's Command"
BG_ASSET_TILES_ONLY(ActiveSkillAragornKingsCommandIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Aragorn[2] "Sword Throw"
BG_ASSET_TILES_ONLY(ActiveSkillAragornSwordThrowIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Aragorn[3] "Numenorean Will"
BG_ASSET_TILES_ONLY(ActiveSkillAragornNumenoreanWillIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Aragorn[4] "Call of the Dead"
BG_ASSET_TILES_ONLY(ActiveSkillAragornCallOfTheDeadIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Aragorn[5] "Herbal Healing"
BG_ASSET_TILES_ONLY(ActiveSkillAragornHerbalHealingIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Gandalf[0] "Sword of Power"
BG_ASSET_TILES_ONLY(ActiveSkillGandalfSwordOfPowerIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Gandalf[1] "Lightstrike"
BG_ASSET_TILES_ONLY(ActiveSkillGandalfLightstrikeIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Gandalf[2] "Shield"
BG_ASSET_TILES_ONLY(ActiveSkillGandalfShieldIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Gandalf[3] "Blinding Aura"
BG_ASSET_TILES_ONLY(ActiveSkillGandalfBlindingAuraIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Gandalf[4] "Summon Gwaihir"
BG_ASSET_TILES_ONLY(ActiveSkillGandalfSummonGwaihirIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Gandalf[5] "Herbal Healing"
BG_ASSET_TILES_ONLY(ActiveSkillGandalfHerbalHealingIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Eowyn[0] "Double Strike"
BG_ASSET_TILES_ONLY(ActiveSkillEowynDoubleStrikeIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Eowyn[1] "Shieldmaiden of Rohan"
BG_ASSET_TILES_ONLY(ActiveSkillEowynShieldmaidenOfRohanIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Eowyn[2] "Rohan Sprint"
BG_ASSET_TILES_ONLY(ActiveSkillEowynRohanSprintIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Eowyn[3] "Shield Bash"
BG_ASSET_TILES_ONLY(ActiveSkillEowynShieldBashIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Eowyn[4] "Forth Eorlingas!"
BG_ASSET_TILES_ONLY(ActiveSkillEowynForthEorlingasIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Eowyn[5] "Herbal Healing"
BG_ASSET_TILES_ONLY(ActiveSkillEowynHerbalHealingIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Gimli[0] "Axe Throw"
BG_ASSET_TILES_ONLY(ActiveSkillGimliAxeThrowIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Gimli[1] "Whirling Attack"
BG_ASSET_TILES_ONLY(ActiveSkillGimliWhirlingAttackIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Gimli[2] "Dwarven Rage"
BG_ASSET_TILES_ONLY(ActiveSkillGimliDwarvenRageIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Gimli[3] "Stoicism"
BG_ASSET_TILES_ONLY(ActiveSkillGimliStoicismIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Gimli[4] "Earth Shatter"
BG_ASSET_TILES_ONLY(ActiveSkillGimliEarthShatterIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// Gimli[5] "Herbal Healing"
BG_ASSET_TILES_ONLY(ActiveSkillGimliHerbalHealingIcon, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);
// clang-format on
