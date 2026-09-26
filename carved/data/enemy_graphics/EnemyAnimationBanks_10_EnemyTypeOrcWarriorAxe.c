#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08051780 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Attack_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Attack_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Attack_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Attack_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Attack_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Attack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080517d0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_KnockdownState30[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_KnockdownState30_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_KnockdownState30_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_KnockdownState30_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_KnockdownState30_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_KnockdownState30_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_KnockdownState30_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_KnockdownState30_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_KnockdownState30_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_KnockdownState30_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_KnockdownState30_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051820 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Death[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Death_Facing0.header, NULL, 2 },
};

/** @romaddress 0x08051870 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_RecoveryState22[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_RecoveryState22_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_RecoveryState22_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_RecoveryState22_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_RecoveryState22_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_RecoveryState22_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_RecoveryState22_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_RecoveryState22_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_RecoveryState22_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_RecoveryState22_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_RecoveryState22_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080518c0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08051910 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Stand_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Stand_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Stand_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Stand_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051960 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_KnockedDown[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_KnockedDown_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_KnockedDown_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_KnockedDown_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_KnockedDown_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_KnockedDown_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_KnockedDown_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_KnockedDown_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_KnockedDown_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_KnockedDown_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_KnockedDown_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080519b0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Walk_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Walk_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Walk_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Walk_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Walk_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051a00 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part1Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Attack_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Attack_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Attack_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Attack_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Attack_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Attack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051a50 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part1KnockdownState30[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1KnockdownState30_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1KnockdownState30_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1KnockdownState30_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1KnockdownState30_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1KnockdownState30_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1KnockdownState30_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1KnockdownState30_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1KnockdownState30_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1KnockdownState30_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1KnockdownState30_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051aa0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part1Death[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Death_Facing0.header, NULL, 2 },
};

/** @romaddress 0x08051af0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part1RecoveryState22[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1RecoveryState22_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1RecoveryState22_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1RecoveryState22_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1RecoveryState22_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1RecoveryState22_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1RecoveryState22_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1RecoveryState22_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1RecoveryState22_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1RecoveryState22_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1RecoveryState22_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051b40 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part1Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08051b90 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part1Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Stand_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Stand_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Stand_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Stand_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051be0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part1KnockedDown[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1KnockedDown_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1KnockedDown_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1KnockedDown_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1KnockedDown_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1KnockedDown_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1KnockedDown_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1KnockedDown_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1KnockedDown_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1KnockedDown_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1KnockedDown_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051c30 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part1Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Walk_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Walk_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Walk_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Walk_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part1Walk_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part1Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051c80 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part2Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Attack_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Attack_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Attack_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Attack_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Attack_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Attack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051cd0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part2KnockdownState30[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2KnockdownState30_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2KnockdownState30_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2KnockdownState30_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2KnockdownState30_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2KnockdownState30_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2KnockdownState30_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2KnockdownState30_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2KnockdownState30_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2KnockdownState30_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2KnockdownState30_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051d20 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part2Death[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Death_Facing0.header, NULL, 2 },
};

/** @romaddress 0x08051d70 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part2RecoveryState22[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2RecoveryState22_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2RecoveryState22_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2RecoveryState22_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2RecoveryState22_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2RecoveryState22_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2RecoveryState22_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2RecoveryState22_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2RecoveryState22_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2RecoveryState22_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2RecoveryState22_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051dc0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part2Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08051e10 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part2Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Stand_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Stand_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Stand_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Stand_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051e60 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part2KnockedDown[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2KnockedDown_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2KnockedDown_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2KnockedDown_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2KnockedDown_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2KnockedDown_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2KnockedDown_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2KnockedDown_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2KnockedDown_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2KnockedDown_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2KnockedDown_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051eb0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part2Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Walk_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Walk_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Walk_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Walk_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part2Walk_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part2Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051f00 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part3Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Attack_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Attack_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Attack_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Attack_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Attack_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Attack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051f50 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part3KnockdownState30[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3KnockdownState30_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3KnockdownState30_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3KnockdownState30_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3KnockdownState30_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3KnockdownState30_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3KnockdownState30_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3KnockdownState30_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3KnockdownState30_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3KnockdownState30_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3KnockdownState30_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08051fa0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part3Death[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Death_Facing0.header, NULL, 2 },
};

/** @romaddress 0x08051ff0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part3RecoveryState22[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3RecoveryState22_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3RecoveryState22_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3RecoveryState22_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3RecoveryState22_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3RecoveryState22_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3RecoveryState22_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3RecoveryState22_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3RecoveryState22_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3RecoveryState22_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3RecoveryState22_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08052040 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part3Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052090 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part3Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Stand_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Stand_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Stand_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Stand_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080520e0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part3KnockedDown[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3KnockedDown_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3KnockedDown_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3KnockedDown_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3KnockedDown_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3KnockedDown_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3KnockedDown_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3KnockedDown_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3KnockedDown_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3KnockedDown_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3KnockedDown_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08052130 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part3Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Walk_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Walk_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Walk_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Walk_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part3Walk_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part3Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08052180 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part5Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Attack_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Attack_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Attack_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Attack_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Attack_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Attack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080521d0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part5KnockdownState30[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5KnockdownState30_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5KnockdownState30_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5KnockdownState30_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5KnockdownState30_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5KnockdownState30_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5KnockdownState30_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5KnockdownState30_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5KnockdownState30_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5KnockdownState30_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5KnockdownState30_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08052220 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part5Death[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Death_Facing0.header, NULL, 2 },
};

/** @romaddress 0x08052270 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part5RecoveryState22[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5RecoveryState22_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5RecoveryState22_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5RecoveryState22_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5RecoveryState22_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5RecoveryState22_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5RecoveryState22_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5RecoveryState22_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5RecoveryState22_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5RecoveryState22_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5RecoveryState22_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080522c0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part5Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052310 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part5Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Stand_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Stand_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Stand_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Stand_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08052360 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part5KnockedDown[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5KnockedDown_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5KnockedDown_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5KnockedDown_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5KnockedDown_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5KnockedDown_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5KnockedDown_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5KnockedDown_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5KnockedDown_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5KnockedDown_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5KnockedDown_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080523b0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part5Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Walk_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Walk_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Walk_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Walk_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part5Walk_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part5Walk_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08052400 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part4Attack[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Attack_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Attack_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Attack_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Attack_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Attack_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Attack_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Attack_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Attack_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Attack_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Attack_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08052450 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part4KnockdownState30[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4KnockdownState30_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4KnockdownState30_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4KnockdownState30_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4KnockdownState30_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4KnockdownState30_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4KnockdownState30_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4KnockdownState30_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4KnockdownState30_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4KnockdownState30_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4KnockdownState30_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080524a0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part4Death[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Death_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Death_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Death_Facing0.header, NULL, 2 },
};

/** @romaddress 0x080524f0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part4RecoveryState22[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4RecoveryState22_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4RecoveryState22_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4RecoveryState22_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4RecoveryState22_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4RecoveryState22_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4RecoveryState22_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4RecoveryState22_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4RecoveryState22_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4RecoveryState22_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4RecoveryState22_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08052540 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part4Stagger[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Stagger_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Stagger_Facing0.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Stagger_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Stagger_Facing1.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Stagger_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Stagger_Facing2.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Stagger_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Stagger_Facing3.header, NULL, 1 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Stagger_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Stagger_Facing4.header, NULL, 1 },
};

/** @romaddress 0x08052590 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part4Stand[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Stand_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Stand_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Stand_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Stand_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Stand_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Stand_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Stand_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Stand_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Stand_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Stand_Facing4.header, NULL, 2 },
};

/** @romaddress 0x080525e0 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part4KnockedDown[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4KnockedDown_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4KnockedDown_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4KnockedDown_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4KnockedDown_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4KnockedDown_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4KnockedDown_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4KnockedDown_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4KnockedDown_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4KnockedDown_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4KnockedDown_Facing4.header, NULL, 2 },
};

/** @romaddress 0x08052630 */
const SpriteAnimation EnemyAnimationBank_EnemyTypeOrcWarriorAxe_Part4Walk[5] = {
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Walk_Facing0, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Walk_Facing0.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Walk_Facing1, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Walk_Facing1.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Walk_Facing2, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Walk_Facing2.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Walk_Facing3, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Walk_Facing3.header, NULL, 2 },
    { EnemyAnimationTiles_EnemyTypeOrcWarriorAxe_Part4Walk_Facing4, &EnemyAnimationFrames_EnemyTypeOrcWarriorAxe_Part4Walk_Facing4.header, NULL, 2 },
};

// clang-format on
