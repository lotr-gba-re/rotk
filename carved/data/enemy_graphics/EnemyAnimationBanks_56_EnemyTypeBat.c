#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08054ca0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeBat_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeBat_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeBat_Walk_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeBat_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeBat_Walk_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeBat_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeBat_Walk_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeBat_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeBat_Walk_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeBat_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeBat_Walk_Facing4.header, NULL, 3 },
};

/** @romaddress 0x08054cf0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeBat_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeBat_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeBat_Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeBat_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeBat_Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeBat_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeBat_Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeBat_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeBat_Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeBat_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeBat_Stand_Facing0.header, NULL, 2 },
};

/** @romaddress 0x08054d40 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeBat_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeBat_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeBat_Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeBat_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeBat_Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeBat_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeBat_Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeBat_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeBat_Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeBat_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeBat_Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08054d90 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeBat_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeBat_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeBat_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeBat_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeBat_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeBat_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeBat_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeBat_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeBat_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeBat_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeBat_Attack_Facing4.header, NULL, 2 },
};

// clang-format on
