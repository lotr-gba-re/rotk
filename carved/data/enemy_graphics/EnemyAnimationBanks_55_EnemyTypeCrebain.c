#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08054b60 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeCrebain_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeCrebain_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeCrebain_Walk_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeCrebain_Walk_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeCrebain_Walk_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeCrebain_Walk_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeCrebain_Walk_Facing4.header, NULL, 3 },
};

/** @romaddress 0x08054bb0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeCrebain_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeCrebain_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeCrebain_Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeCrebain_Stand_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeCrebain_Stand_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeCrebain_Stand_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeCrebain_Stand_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08054c00 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeCrebain_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeCrebain_Death_Facing0, &EnemyAnimationFrames_EnemyTypeCrebain_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Death_Facing0, &EnemyAnimationFrames_EnemyTypeCrebain_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Death_Facing0, &EnemyAnimationFrames_EnemyTypeCrebain_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Death_Facing0, &EnemyAnimationFrames_EnemyTypeCrebain_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Death_Facing0, &EnemyAnimationFrames_EnemyTypeCrebain_Death_Facing0.header, NULL, 2 },
};

/** @romaddress 0x08054c50 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeCrebain_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeCrebain_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeCrebain_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeCrebain_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeCrebain_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeCrebain_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeCrebain_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeCrebain_Attack_Facing4.header, NULL, 2 },
};

// clang-format on
