#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08053b20 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeUruk_Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeUruk_Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeUruk_Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeUruk_Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08053b70 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_ExtraPose00[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose00_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose00_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose00_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose00_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose00_Facing0.header, NULL, 3 },
};

/** @romaddress 0x08053bc0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_Death_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_Death_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_Death_Facing1, &EnemyAnimationFrames_EnemyTypeUruk_Death_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_Death_Facing2, &EnemyAnimationFrames_EnemyTypeUruk_Death_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_Death_Facing3, &EnemyAnimationFrames_EnemyTypeUruk_Death_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_Death_Facing4, &EnemyAnimationFrames_EnemyTypeUruk_Death_Facing4.header, NULL, 3 },
};

/** @romaddress 0x08053c10 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_Stand_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeUruk_Stand_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeUruk_Stand_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeUruk_Stand_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeUruk_Stand_Facing4.header, NULL, 3 },
};

/** @romaddress 0x08053c60 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_ExtraPose01[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose01_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose01_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose01_Facing1, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose01_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose01_Facing2, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose01_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose01_Facing3, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose01_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose01_Facing4, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose01_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08053cb0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_Stagger_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeUruk_Stagger_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeUruk_Stagger_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeUruk_Stagger_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeUruk_Stagger_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08053d00 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeUruk_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeUruk_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeUruk_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeUruk_Attack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08053d50 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_ExtraPose02[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose02_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose02_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose02_Facing1, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose02_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose02_Facing2, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose02_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose02_Facing3, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose02_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose02_Facing4, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose02_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08053da0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_ExtraPose03[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose03_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose03_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose03_Facing1, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose03_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose03_Facing2, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose03_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose03_Facing3, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose03_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose03_Facing4, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose03_Facing4.header, NULL, 3 },
};

/** @romaddress 0x08053df0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_ExtraPose04[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose04_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose04_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose04_Facing1, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose04_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose04_Facing2, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose04_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose04_Facing3, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose04_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose04_Facing4, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose04_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08053e40 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_ExtraPose05[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose05_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose05_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose05_Facing1, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose05_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose05_Facing2, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose05_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose05_Facing3, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose05_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose05_Facing4, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose05_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08053e90 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_ExtraPose06[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose06_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose06_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose06_Facing1, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose06_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose06_Facing2, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose06_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose06_Facing3, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose06_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose06_Facing4, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose06_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08053ee0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_ExtraPose07[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose07_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose07_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose07_Facing1, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose07_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose07_Facing2, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose07_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose07_Facing3, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose07_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose07_Facing4, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose07_Facing4.header, NULL, 3 },
};

/** @romaddress 0x08053f30 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_ExtraPose08[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose08_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose08_Facing0.header, NULL, 4 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose08_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose08_Facing0.header, NULL, 4 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose08_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose08_Facing0.header, NULL, 4 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose08_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose08_Facing0.header, NULL, 4 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose08_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose08_Facing0.header, NULL, 4 },
};

/** @romaddress 0x08053f80 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_ExtraPose09[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose09_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose09_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose09_Facing1, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose09_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose09_Facing2, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose09_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose09_Facing3, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose09_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose09_Facing4, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose09_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08053fd0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeUruk_ExtraPose10[5] = {
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose10_Facing0, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose10_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose10_Facing1, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose10_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose10_Facing2, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose10_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose10_Facing3, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose10_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeUruk_ExtraPose10_Facing4, &EnemyAnimationFrames_EnemyTypeUruk_ExtraPose10_Facing4.header, NULL, 3 },
};

// clang-format on
