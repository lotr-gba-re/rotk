#include "combat.h"
#include "sfx.h"
#include "variables.h"

/**
 * Absorb an incoming melee hit into the active shield bubble (g_ShieldBubble).
 * The melee-hit paths call this when the hit victim carries the shield marker
 * (recordType == 0x26); a TRUE return cancels the hit outright.
 *
 * @param victim the actor the bubble protects; stored as the bubble's follow target
 * @param damage hit damage, drained from the bubble's pool (floored at 0)
 *
 * @return TRUE when a bubble is up (hit fully absorbed), FALSE otherwise
 *
 * @romaddress 0x0803b9b8
 */
bool combat_shieldAbsorbsHit(Actor *victim, s32 damage)
{
    if (g_ShieldBubble == NULL)
    {
        return FALSE;
    }

    {
        s16 *shieldHealth = &g_ShieldBubble->as.shieldGandalf.shieldHealth;
        if (*shieldHealth > 0)
        {
            g_ShieldBubble->as.shieldGandalf.followTarget = victim;

            if (*shieldHealth >= damage)
            {
                *shieldHealth -= damage;
            }
            else
            {
                *shieldHealth = 0;
            }

            if (g_ShieldBubble->actionState == ACTOR_STATE_FX_ATTACHED)
            {
                g_ShieldBubble->actionState = ACTOR_STATE_SHIELD_SPENT;
                sfx_play(SFX_COMBAT_GANDALF_SHIELD_ABSORB);
            }
        }
    }
    return TRUE;
}
