#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x080561e0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeWitchKing_ExtraPose00[5] = {
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose00_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose00_Facing1, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose00_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose00_Facing2, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose00_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose00_Facing3, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose00_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose00_Facing4, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose00_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08056230 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeWitchKing_ExtraPose01[5] = {
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose01_Facing0, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose01_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose01_Facing1, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose01_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose01_Facing2, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose01_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose01_Facing3, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose01_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose01_Facing4, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose01_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08056280 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeWitchKing_ExtraPose02[5] = {
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose01_Facing0, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose01_Facing0.header, NULL, 0 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose01_Facing1, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose01_Facing1.header, NULL, 0 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose01_Facing2, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose01_Facing2.header, NULL, 0 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose01_Facing3, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose01_Facing3.header, NULL, 0 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose01_Facing4, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose01_Facing4.header, NULL, 0 },
};

/** @romaddress 0x080562d0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeWitchKing_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeWitchKing_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeWitchKing_Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeWitchKing_Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeWitchKing_Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeWitchKing_Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeWitchKing_Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08056320 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeWitchKing_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeWitchKing_Death_Facing0, &EnemyAnimationFrames_EnemyTypeWitchKing_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Death_Facing0, &EnemyAnimationFrames_EnemyTypeWitchKing_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Death_Facing0, &EnemyAnimationFrames_EnemyTypeWitchKing_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Death_Facing0, &EnemyAnimationFrames_EnemyTypeWitchKing_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Death_Facing0, &EnemyAnimationFrames_EnemyTypeWitchKing_Death_Facing0.header, NULL, 1 },
};

/** @romaddress 0x08056370 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeWitchKing_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeWitchKing_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeWitchKing_Attack_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeWitchKing_Attack_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeWitchKing_Attack_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeWitchKing_Attack_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeWitchKing_Attack_Facing4.header, NULL, 1 },
};

/** @romaddress 0x080563c0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeWitchKing_Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeWitchKing_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeWitchKing_Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeWitchKing_Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeWitchKing_Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeWitchKing_Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeWitchKing_Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08056410 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeWitchKing_ExtraPose03[5] = {
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose03_Facing0, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose03_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose03_Facing1, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose03_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose03_Facing2, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose03_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose03_Facing3, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose03_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeWitchKing_ExtraPose03_Facing4, &EnemyAnimationFrames_EnemyTypeWitchKing_ExtraPose03_Facing4.header, NULL, 2 },
};

// clang-format on
