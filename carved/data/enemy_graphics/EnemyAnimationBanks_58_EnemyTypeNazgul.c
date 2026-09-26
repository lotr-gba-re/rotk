#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x080546b0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeNazgul_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeNazgul_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeNazgul_Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeNazgul_Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeNazgul_Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeNazgul_Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeNazgul_Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08054700 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeNazgul_StandCopy[5] = {
    { EnemyAnimationTiles_EnemyTypeNazgul_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeNazgul_Walk_Facing0.header, NULL, 0 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeNazgul_Walk_Facing1.header, NULL, 0 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeNazgul_Walk_Facing2.header, NULL, 0 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeNazgul_Walk_Facing3.header, NULL, 0 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeNazgul_Walk_Facing4.header, NULL, 0 },
};

/** @romaddress 0x08054750 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeNazgul_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeNazgul_Death_Facing0, &EnemyAnimationFrames_EnemyTypeNazgul_Death_Facing0.header, NULL, 4 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Death_Facing0, &EnemyAnimationFrames_EnemyTypeNazgul_Death_Facing0.header, NULL, 4 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Death_Facing0, &EnemyAnimationFrames_EnemyTypeNazgul_Death_Facing0.header, NULL, 4 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Death_Facing0, &EnemyAnimationFrames_EnemyTypeNazgul_Death_Facing0.header, NULL, 4 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Death_Facing0, &EnemyAnimationFrames_EnemyTypeNazgul_Death_Facing0.header, NULL, 4 },
};

/** @romaddress 0x080547a0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeNazgul_Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeNazgul_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeNazgul_Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeNazgul_Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeNazgul_Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeNazgul_Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeNazgul_Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x080547f0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeNazgul_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeNazgul_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeNazgul_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeNazgul_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeNazgul_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeNazgul_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeNazgul_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeNazgul_Attack_Facing4.header, NULL, 2 },
};

// clang-format on
