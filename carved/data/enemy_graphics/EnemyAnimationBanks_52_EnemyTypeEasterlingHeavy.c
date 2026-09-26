#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x080512d0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeEasterlingHeavy_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051320 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeEasterlingHeavy_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Death_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Death_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Death_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Death_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Death_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Death_Facing0.header, NULL, 2 },
};

/** @romaddress 0x08051370 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeEasterlingHeavy_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Attack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080513c0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeEasterlingHeavy_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Stand_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Stand_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Stand_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Stand_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Stand_Facing4.header, NULL, 3 },
};

/** @romaddress 0x08051410 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeEasterlingHeavy_Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Stagger_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Stagger_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Stagger_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Stagger_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_Stagger_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051460 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeEasterlingHeavy_AttachedPartWalk[5] = {
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartWalk_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartWalk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartWalk_Facing1, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartWalk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartWalk_Facing2, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartWalk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartWalk_Facing3, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartWalk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartWalk_Facing4, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartWalk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080514b0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeEasterlingHeavy_AttachedPartDeath[5] = {
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartDeath_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartDeath_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartDeath_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartDeath_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartDeath_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartDeath_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartDeath_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartDeath_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartDeath_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartDeath_Facing0.header, NULL, 2 },
};

/** @romaddress 0x08051500 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeEasterlingHeavy_AttachedPartAttack[5] = {
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartAttack_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartAttack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartAttack_Facing1, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartAttack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartAttack_Facing2, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartAttack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartAttack_Facing3, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartAttack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartAttack_Facing4, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartAttack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051550 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeEasterlingHeavy_AttachedPartStand[5] = {
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartStand_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartStand_Facing0.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartStand_Facing1, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartStand_Facing1.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartStand_Facing2, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartStand_Facing2.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartStand_Facing3, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartStand_Facing3.header, NULL, 3 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartStand_Facing4, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartStand_Facing4.header, NULL, 3 },
};

/** @romaddress 0x080515a0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeEasterlingHeavy_AttachedPartStagger[5] = {
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartStagger_Facing0, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartStagger_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartStagger_Facing1, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartStagger_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartStagger_Facing2, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartStagger_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartStagger_Facing3, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartStagger_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeEasterlingHeavy_AttachedPartStagger_Facing4, &EnemyAnimationFrames_EnemyTypeEasterlingHeavy_AttachedPartStagger_Facing4.header, NULL, 2 },
};

// clang-format on
