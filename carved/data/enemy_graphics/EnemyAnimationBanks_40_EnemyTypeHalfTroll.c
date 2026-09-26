#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08055600 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeHalfTroll_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeHalfTroll_Walk_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeHalfTroll_Walk_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeHalfTroll_Walk_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeHalfTroll_Walk_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeHalfTroll_Walk_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08055650 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeHalfTroll_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Death_Facing0, &EnemyAnimationFrames_EnemyTypeHalfTroll_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Death_Facing0, &EnemyAnimationFrames_EnemyTypeHalfTroll_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Death_Facing0, &EnemyAnimationFrames_EnemyTypeHalfTroll_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Death_Facing0, &EnemyAnimationFrames_EnemyTypeHalfTroll_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Death_Facing0, &EnemyAnimationFrames_EnemyTypeHalfTroll_Death_Facing0.header, NULL, 2 },
};

/** @romaddress 0x080556a0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeHalfTroll_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeHalfTroll_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeHalfTroll_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeHalfTroll_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeHalfTroll_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeHalfTroll_Attack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080556f0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeHalfTroll_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeHalfTroll_Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeHalfTroll_Stand_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeHalfTroll_Stand_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeHalfTroll_Stand_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeHalfTroll_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeHalfTroll_Stand_Facing4.header, NULL, 2 },
};

// clang-format on
