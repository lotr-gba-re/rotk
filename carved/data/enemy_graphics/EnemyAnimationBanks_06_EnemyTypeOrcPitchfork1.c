#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08052860 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcPitchfork1_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080528b0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcPitchfork1_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stand_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stand_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stand_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stand_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stand_Facing4.header, NULL, 3 },
};

/** @romaddress 0x08052900 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcPitchfork1_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Death_Facing1, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Death_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Death_Facing2, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Death_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Death_Facing3, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Death_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Death_Facing4, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Death_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08052950 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcPitchfork1_Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x080529a0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcPitchfork1_StunnedCopy[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stand_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stand_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stand_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stand_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Stand_Facing4.header, NULL, 3 },
};

/** @romaddress 0x080529f0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcPitchfork1_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcPitchfork1_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeOrcPitchfork1_Attack_Facing4.header, NULL, 2 },
};

// clang-format on
