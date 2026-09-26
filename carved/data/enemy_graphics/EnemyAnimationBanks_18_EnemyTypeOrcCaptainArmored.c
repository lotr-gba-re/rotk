#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08052a40 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcCaptainArmored_AlternateWalk[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Walk_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Walk_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Walk_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Walk_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Walk_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052a90 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcCaptainArmored_AlternateStand[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052ae0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcCaptainArmored_AlternateDeath[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Death_Facing1, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Death_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Death_Facing2, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Death_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Death_Facing3, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Death_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Death_Facing4, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Death_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052b30 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcCaptainArmored_AlternateStagger[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052b80 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcCaptainArmored_AlternateStunned[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052bd0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcCaptainArmored_AlternateAttack[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Attack_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Attack_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Attack_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Attack_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Attack_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052c20 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcCaptainArmored_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Walk_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Walk_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Walk_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Walk_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Walk_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052c70 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcCaptainArmored_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052cc0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcCaptainArmored_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Death_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Death_Facing1, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Death_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Death_Facing2, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Death_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Death_Facing3, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Death_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Death_Facing4, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Death_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052d10 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcCaptainArmored_Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052d60 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcCaptainArmored_Stunned[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Stand_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052db0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcCaptainArmored_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcCaptainArmored_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeOrcCaptainArmored_Attack_Facing4.header, NULL, 2 },
};

// clang-format on
