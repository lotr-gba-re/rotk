#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x080544d0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeWildman_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeWildman_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeWildman_Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeWildman_Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeWildman_Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeWildman_Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeWildman_Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08054520 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeWildman_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeWildman_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeWildman_Stand_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeWildman_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeWildman_Stand_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeWildman_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeWildman_Stand_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeWildman_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeWildman_Stand_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeWildman_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeWildman_Stand_Facing4.header, NULL, 3 },
};

/** @romaddress 0x08054570 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeWildman_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeWildman_Death_Facing0, &EnemyAnimationFrames_EnemyTypeWildman_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Death_Facing1, &EnemyAnimationFrames_EnemyTypeWildman_Death_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Death_Facing2, &EnemyAnimationFrames_EnemyTypeWildman_Death_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Death_Facing3, &EnemyAnimationFrames_EnemyTypeWildman_Death_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Death_Facing4, &EnemyAnimationFrames_EnemyTypeWildman_Death_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080545c0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeWildman_Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeWildman_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeWildman_Stagger_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeWildman_Stagger_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeWildman_Stagger_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeWildman_Stagger_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeWildman_Stagger_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08054610 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeWildman_Stunned[5] = {
    { EnemyAnimationTiles_EnemyTypeWildman_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeWildman_Stand_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeWildman_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeWildman_Stand_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeWildman_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeWildman_Stand_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeWildman_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeWildman_Stand_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeWildman_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeWildman_Stand_Facing4.header, NULL, 3 },
};

/** @romaddress 0x08054660 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeWildman_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeWildman_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeWildman_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeWildman_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeWildman_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeWildman_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWildman_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeWildman_Attack_Facing4.header, NULL, 2 },
};

// clang-format on
