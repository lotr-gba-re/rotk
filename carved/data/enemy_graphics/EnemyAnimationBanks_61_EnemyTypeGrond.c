#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08055740 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGrond_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeGrond_Death_Facing0, &EnemyAnimationFrames_EnemyTypeGrond_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGrond_Death_Facing0, &EnemyAnimationFrames_EnemyTypeGrond_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGrond_Death_Facing0, &EnemyAnimationFrames_EnemyTypeGrond_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGrond_Death_Facing0, &EnemyAnimationFrames_EnemyTypeGrond_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGrond_Death_Facing0, &EnemyAnimationFrames_EnemyTypeGrond_Death_Facing0.header, NULL, 1 },
};

/** @romaddress 0x08055790 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGrond_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeGrond_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeGrond_Walk_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGrond_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeGrond_Walk_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGrond_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeGrond_Walk_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGrond_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeGrond_Walk_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGrond_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeGrond_Walk_Facing0.header, NULL, 3 },
};

// clang-format on
