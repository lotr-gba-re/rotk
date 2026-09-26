#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08053940 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcDrummer_ExtraPose00[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeOrcDrummer_ExtraPose00_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeOrcDrummer_ExtraPose00_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeOrcDrummer_ExtraPose00_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeOrcDrummer_ExtraPose00_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_ExtraPose00_Facing0, &EnemyAnimationFrames_EnemyTypeOrcDrummer_ExtraPose00_Facing0.header, NULL, 2 },
};

/** @romaddress 0x08053990 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcDrummer_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Walk_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Walk_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Walk_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Walk_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Walk_Facing4.header, NULL, 1 },
};

/** @romaddress 0x080539e0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcDrummer_ExtraPose01[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_ExtraPose01_Facing0, &EnemyAnimationFrames_EnemyTypeOrcDrummer_ExtraPose01_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_ExtraPose01_Facing1, &EnemyAnimationFrames_EnemyTypeOrcDrummer_ExtraPose01_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_ExtraPose01_Facing2, &EnemyAnimationFrames_EnemyTypeOrcDrummer_ExtraPose01_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_ExtraPose01_Facing3, &EnemyAnimationFrames_EnemyTypeOrcDrummer_ExtraPose01_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_ExtraPose01_Facing4, &EnemyAnimationFrames_EnemyTypeOrcDrummer_ExtraPose01_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08053a30 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcDrummer_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Stand_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Stand_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Stand_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Stand_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08053a80 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcDrummer_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Death_Facing0.header, NULL, 2 },
};

/** @romaddress 0x08053ad0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcDrummer_Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Stagger_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Stagger_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Stagger_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Stagger_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcDrummer_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeOrcDrummer_Stagger_Facing4.header, NULL, 2 },
};

// clang-format on
