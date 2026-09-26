#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08054f70 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeMouthOfSauron_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08054fc0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeMouthOfSauron_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Stand_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Stand_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Stand_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Stand_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Stand_Facing4.header, NULL, 3 },
};

/** @romaddress 0x08055010 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeMouthOfSauron_ExtraPose00[5] = {
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose00_Facing0.header, NULL, 4 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose00_Facing0.header, NULL, 4 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose00_Facing0.header, NULL, 4 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose00_Facing0.header, NULL, 4 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose00_Facing0.header, NULL, 4 },
};

/** @romaddress 0x08055060 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeMouthOfSauron_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Death_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Death_Facing1, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Death_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Death_Facing2, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Death_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Death_Facing3, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Death_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Death_Facing4, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Death_Facing4.header, NULL, 1 },
};

/** @romaddress 0x080550b0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeMouthOfSauron_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_Attack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08055100 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeMouthOfSauron_ExtraPose01[5] = {
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose01_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose01_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose01_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose01_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose01_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose01_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose01_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose01_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose01_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose01_Facing0.header, NULL, 3 },
};

/** @romaddress 0x08055150 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeMouthOfSauron_ExtraPose02[5] = {
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose02_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose02_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose02_Facing1, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose02_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose02_Facing2, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose02_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose02_Facing3, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose02_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose02_Facing4, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose02_Facing4.header, NULL, 1 },
};

/** @romaddress 0x080551a0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeMouthOfSauron_ExtraPose03[5] = {
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose03_Facing0, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose03_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose03_Facing1, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose03_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose03_Facing2, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose03_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose03_Facing3, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose03_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeMouthOfSauron_ExtraPose03_Facing4, &EnemyAnimationFrames_EnemyTypeMouthOfSauron_ExtraPose03_Facing4.header, NULL, 1 },
};

// clang-format on
