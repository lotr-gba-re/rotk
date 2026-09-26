#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08058754 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGollum_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeGollum_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeGollum_Stand_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGollum_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeGollum_Stand_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGollum_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeGollum_Stand_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGollum_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeGollum_Stand_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeGollum_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeGollum_Stand_Facing4.header, NULL, 3 },
};

/** @romaddress 0x080587a4 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGollum_ExtraPose[5] = {
    { EnemyAnimationTiles_EnemyTypeGollum_ExtraPose_Facing0, &EnemyAnimationFrames_EnemyTypeGollum_ExtraPose_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGollum_ExtraPose_Facing1, &EnemyAnimationFrames_EnemyTypeGollum_ExtraPose_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGollum_ExtraPose_Facing2, &EnemyAnimationFrames_EnemyTypeGollum_ExtraPose_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGollum_ExtraPose_Facing3, &EnemyAnimationFrames_EnemyTypeGollum_ExtraPose_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGollum_ExtraPose_Facing4, &EnemyAnimationFrames_EnemyTypeGollum_ExtraPose_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080587f4 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGollum_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeGollum_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeGollum_Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGollum_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeGollum_Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGollum_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeGollum_Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGollum_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeGollum_Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGollum_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeGollum_Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08058844 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGollum_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeGollum_Death_Facing0, &EnemyAnimationFrames_EnemyTypeGollum_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGollum_Death_Facing1, &EnemyAnimationFrames_EnemyTypeGollum_Death_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGollum_Death_Facing2, &EnemyAnimationFrames_EnemyTypeGollum_Death_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGollum_Death_Facing3, &EnemyAnimationFrames_EnemyTypeGollum_Death_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeGollum_Death_Facing4, &EnemyAnimationFrames_EnemyTypeGollum_Death_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08058894 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeGollum_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeGollum_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeGollum_Attack_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGollum_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeGollum_Attack_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGollum_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeGollum_Attack_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGollum_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeGollum_Attack_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeGollum_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeGollum_Attack_Facing4.header, NULL, 1 },
};

// clang-format on
