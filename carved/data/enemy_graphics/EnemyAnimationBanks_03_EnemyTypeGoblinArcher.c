#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08053530 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGoblinArcher_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08053580 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGoblinArcher_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stand_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stand_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stand_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stand_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stand_Facing4.header, NULL, 3 },
};

/** @romaddress 0x080535d0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGoblinArcher_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Death_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Death_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Death_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Death_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Death_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Death_Facing0.header, NULL, 2 },
};

/** @romaddress 0x08053620 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGoblinArcher_Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08053670 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGoblinArcher_Stunned[5] = {
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stunned_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stunned_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stunned_Facing1, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stunned_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stunned_Facing2, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stunned_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stunned_Facing3, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stunned_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Stunned_Facing4, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Stunned_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080536c0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGoblinArcher_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Attack_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Attack_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Attack_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Attack_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGoblinArcher_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeGoblinArcher_Attack_Facing4.header, NULL, 3 },
};

// clang-format on
