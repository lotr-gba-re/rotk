#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08056780 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeKingOfTheDead_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeKingOfTheDead_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeKingOfTheDead_Walk_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeKingOfTheDead_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeKingOfTheDead_Walk_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeKingOfTheDead_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeKingOfTheDead_Walk_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeKingOfTheDead_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeKingOfTheDead_Walk_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeKingOfTheDead_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeKingOfTheDead_Walk_Facing4.header, NULL, 3 },
};

/** @romaddress 0x080567d0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeKingOfTheDead_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeKingOfTheDead_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeKingOfTheDead_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeKingOfTheDead_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeKingOfTheDead_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeKingOfTheDead_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeKingOfTheDead_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeKingOfTheDead_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeKingOfTheDead_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeKingOfTheDead_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeKingOfTheDead_Attack_Facing0.header, NULL, 2 },
};

// clang-format on
