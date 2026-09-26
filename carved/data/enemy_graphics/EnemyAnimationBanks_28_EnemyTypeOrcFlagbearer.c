#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08053170 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcFlagbearer_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Walk_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Walk_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Walk_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Walk_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Walk_Facing4.header, NULL, 1 },
};

/** @romaddress 0x080531c0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcFlagbearer_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Stand_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Stand_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Stand_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Stand_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08053210 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcFlagbearer_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Death_Facing0.header, NULL, 2 },
};

/** @romaddress 0x08053260 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcFlagbearer_Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Stagger_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Stagger_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Stagger_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Stagger_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Stagger_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080532b0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcFlagbearer_ExtraPose[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_ExtraPose_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_ExtraPose_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_ExtraPose_Facing1, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_ExtraPose_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_ExtraPose_Facing2, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_ExtraPose_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_ExtraPose_Facing3, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_ExtraPose_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_ExtraPose_Facing4, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_ExtraPose_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08053300 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcFlagbearer_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_Attack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08053350 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcFlagbearer_FlagPartWalk[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartWalk_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartWalk_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartWalk_Facing1, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartWalk_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartWalk_Facing2, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartWalk_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartWalk_Facing3, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartWalk_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartWalk_Facing4, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartWalk_Facing4.header, NULL, 1 },
};

/** @romaddress 0x080533a0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcFlagbearer_FlagPartStand[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartStand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartStand_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartStand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartStand_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartStand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartStand_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartStand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartStand_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartStand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartStand_Facing4.header, NULL, 1 },
};

/** @romaddress 0x080533f0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcFlagbearer_FlagPartDeath[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartDeath_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartDeath_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartDeath_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartDeath_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartDeath_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartDeath_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartDeath_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartDeath_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartDeath_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartDeath_Facing0.header, NULL, 1 },
};

/** @romaddress 0x08053440 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcFlagbearer_FlagPartStagger[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartStagger_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartStagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartStagger_Facing1, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartStagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartStagger_Facing2, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartStagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartStagger_Facing3, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartStagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartStagger_Facing4, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartStagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08053490 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcFlagbearer_FlagPartExtraPose[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartExtraPose_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartExtraPose_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartExtraPose_Facing1, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartExtraPose_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartExtraPose_Facing2, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartExtraPose_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartExtraPose_Facing3, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartExtraPose_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartExtraPose_Facing4, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartExtraPose_Facing4.header, NULL, 1 },
};

/** @romaddress 0x080534e0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcFlagbearer_FlagPartAttack[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartAttack_Facing0, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartAttack_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartAttack_Facing1, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartAttack_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartAttack_Facing2, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartAttack_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartAttack_Facing3, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartAttack_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcFlagbearer_FlagPartAttack_Facing4, &EnemyAnimationFrames_EnemyTypeOrcFlagbearer_FlagPartAttack_Facing4.header, NULL, 1 },
};

// clang-format on
