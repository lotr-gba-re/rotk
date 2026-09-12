#pragma once

#include "types.h"

typedef struct ActorShieldGandalfState
{
    /** actor the shield bubble glues to (seeded = parent; re-pointed at the absorbed hit) */
    struct Actor *followTarget;
    u8 field_0x88[2];

    /** Gandalf shield absorb pool (spawn 75) */
    s16 shieldHealth;
    u8 field_0x8c[2];

    /**
     * charge level (1-5) of the charged shield skill; selects the absorb FX
     * variant via FxChargeVariants[level - 1] (Gimli's branch of
     * combat_absorbDamageIntoShield)
     */
    s16 chargeLevel;
} ActorShieldGandalfState;

typedef struct ActorShieldGimliState
{
    struct Actor *followTarget;
    u8 field_0x88[2];

    u16 shieldPercent;
    u8 field_0x8c[2];

    /**
     * charge level (1-5) of the charged shield skill; selects the absorb FX
     * variant via FxChargeVariants[level - 1]
     */
    s16 chargeLevel;
} ActorShieldGimliState;
