#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08052e00 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcAxeThrower_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Walk_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Walk_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Walk_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Walk_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Walk_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052e50 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcAxeThrower_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stand_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stand_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stand_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stand_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stand_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052ea0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcAxeThrower_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Death_Facing0.header, NULL, 1 },
};

/** @romaddress 0x08052ef0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcAxeThrower_Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052f40 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcAxeThrower_Stunned[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stand_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stand_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stand_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stand_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Stand_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052f90 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcAxeThrower_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_Attack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08052fe0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcAxeThrower_SpecialAttack[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_SpecialAttack_Facing0, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_SpecialAttack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_SpecialAttack_Facing1, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_SpecialAttack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_SpecialAttack_Facing2, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_SpecialAttack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_SpecialAttack_Facing3, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_SpecialAttack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcAxeThrower_SpecialAttack_Facing4, &EnemyAnimationFrames_EnemyTypeOrcAxeThrower_SpecialAttack_Facing4.header, NULL, 2 },
};

// clang-format on
