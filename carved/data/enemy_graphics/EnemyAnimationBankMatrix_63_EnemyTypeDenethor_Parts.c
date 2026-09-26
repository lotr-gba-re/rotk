#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x0805104c */
const SpriteAnimation *const EnemyTypeDenethorPartAnimationBanks[6][7] = {
    { EnemyAnimationBank_EnemyTypeDenethor_Walk, EnemyAnimationBank_EnemyTypeDenethor_Stunned, EnemyAnimationBank_EnemyTypeDenethor_Attack, EnemyAnimationBank_EnemyTypeDenethor_ExtraPose00, EnemyAnimationBank_EnemyTypeDenethor_Death, EnemyAnimationBank_EnemyTypeDenethor_ExtraPose01, EnemyAnimationBank_EnemyTypeDenethor_Stand },
    { EnemyAnimationBank_EnemyTypeWitchKing_ExtraPose01, EnemyAnimationBank_EnemyTypeDenethor_ExtraPose04, EnemyAnimationBank_EnemyTypeDenethor_ExtraPose03, EnemyAnimationBank_EnemyTypeDenethor_ExtraPose00, EnemyAnimationBank_EnemyTypeDenethor_Death, EnemyAnimationBank_EnemyTypeWitchKing_ExtraPose00, EnemyAnimationBank_EnemyTypeWitchKing_ExtraPose02 },
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL },
};
// clang-format on
