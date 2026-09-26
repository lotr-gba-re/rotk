#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08055ec0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeDenethor_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeDenethor_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeDenethor_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeDenethor_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeDenethor_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeDenethor_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeDenethor_Attack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08055f10 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeDenethor_ExtraPose00[5] = {
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose00_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose00_Facing1, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose00_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose00_Facing2, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose00_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose00_Facing3, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose00_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose00_Facing4, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose00_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08055f60 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeDenethor_Stunned[5] = {
    { EnemyAnimationTiles_EnemyTypeDenethor_Stunned_Facing0, &EnemyAnimationFrames_EnemyTypeDenethor_Stunned_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Stunned_Facing1, &EnemyAnimationFrames_EnemyTypeDenethor_Stunned_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Stunned_Facing2, &EnemyAnimationFrames_EnemyTypeDenethor_Stunned_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Stunned_Facing3, &EnemyAnimationFrames_EnemyTypeDenethor_Stunned_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Stunned_Facing4, &EnemyAnimationFrames_EnemyTypeDenethor_Stunned_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08055fb0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeDenethor_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeDenethor_Death_Facing0, &EnemyAnimationFrames_EnemyTypeDenethor_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Death_Facing1, &EnemyAnimationFrames_EnemyTypeDenethor_Death_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Death_Facing2, &EnemyAnimationFrames_EnemyTypeDenethor_Death_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Death_Facing3, &EnemyAnimationFrames_EnemyTypeDenethor_Death_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Death_Facing4, &EnemyAnimationFrames_EnemyTypeDenethor_Death_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08056000 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeDenethor_ExtraPose01[5] = {
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose01_Facing0, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose01_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose01_Facing1, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose01_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose01_Facing2, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose01_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose01_Facing3, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose01_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose01_Facing4, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose01_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08056050 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeDenethor_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeDenethor_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeDenethor_Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeDenethor_Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeDenethor_Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeDenethor_Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeDenethor_Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080560a0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeDenethor_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeDenethor_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeDenethor_Walk_Facing0.header, NULL, 0 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeDenethor_Walk_Facing1.header, NULL, 0 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeDenethor_Walk_Facing2.header, NULL, 0 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeDenethor_Walk_Facing3.header, NULL, 0 },
    { EnemyAnimationTiles_EnemyTypeDenethor_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeDenethor_Walk_Facing4.header, NULL, 0 },
};

/** @romaddress 0x080560f0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeDenethor_ExtraPose02[5] = {
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose02_Facing0, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose02_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose02_Facing0, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose02_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose02_Facing0, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose02_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose02_Facing0, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose02_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose02_Facing0, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose02_Facing0.header, NULL, 3 },
};

/** @romaddress 0x08056140 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeDenethor_ExtraPose03[5] = {
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose03_Facing0, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose03_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose03_Facing1, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose03_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose03_Facing2, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose03_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose03_Facing3, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose03_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose03_Facing4, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose03_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08056190 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeDenethor_ExtraPose04[5] = {
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose04_Facing0, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose04_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose04_Facing1, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose04_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose04_Facing2, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose04_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose04_Facing3, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose04_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeDenethor_ExtraPose04_Facing4, &EnemyAnimationFrames_EnemyTypeDenethor_ExtraPose04_Facing4.header, NULL, 1 },
};

// clang-format on
