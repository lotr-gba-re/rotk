#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08053710 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGoblinScout_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinScout_Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeGoblinScout_Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeGoblinScout_Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeGoblinScout_Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeGoblinScout_Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08053760 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGoblinScout_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stand_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stand_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stand_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stand_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080537b0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGoblinScout_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Death_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinScout_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Death_Facing1, &EnemyAnimationFrames_EnemyTypeGoblinScout_Death_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Death_Facing2, &EnemyAnimationFrames_EnemyTypeGoblinScout_Death_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Death_Facing3, &EnemyAnimationFrames_EnemyTypeGoblinScout_Death_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Death_Facing4, &EnemyAnimationFrames_EnemyTypeGoblinScout_Death_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08053800 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGoblinScout_Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08053850 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGoblinScout_Stunned[5] = {
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stunned_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stunned_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stunned_Facing1, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stunned_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stunned_Facing2, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stunned_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stunned_Facing3, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stunned_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Stunned_Facing4, &EnemyAnimationFrames_EnemyTypeGoblinScout_Stunned_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080538a0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGoblinScout_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinScout_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeGoblinScout_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeGoblinScout_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeGoblinScout_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeGoblinScout_Attack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080538f0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGoblinScout_ExtraPose00[5] = {
    { EnemyAnimationTiles_EnemyTypeGoblinScout_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinScout_ExtraPose00_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinScout_ExtraPose00_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinScout_ExtraPose00_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinScout_ExtraPose00_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGoblinScout_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeGoblinScout_ExtraPose00_Facing0.header, NULL, 2 },
};

// clang-format on
