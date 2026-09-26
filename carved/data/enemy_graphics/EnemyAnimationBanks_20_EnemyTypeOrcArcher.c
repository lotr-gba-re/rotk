#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08052680 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcArcher_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeOrcArcher_Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeOrcArcher_Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeOrcArcher_Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeOrcArcher_Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeOrcArcher_Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080526d0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcArcher_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stand_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stand_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stand_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stand_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stand_Facing4.header, NULL, 3 },
};

/** @romaddress 0x08052720 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcArcher_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcArcher_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcArcher_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcArcher_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcArcher_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcArcher_Death_Facing0.header, NULL, 2 },
};

/** @romaddress 0x08052770 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcArcher_Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x080527c0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcArcher_Stunned[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stand_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stand_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stand_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stand_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcArcher_Stand_Facing4.header, NULL, 3 },
};

/** @romaddress 0x08052810 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcArcher_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeOrcArcher_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeOrcArcher_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeOrcArcher_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeOrcArcher_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcArcher_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeOrcArcher_Attack_Facing4.header, NULL, 2 },
};

// clang-format on
