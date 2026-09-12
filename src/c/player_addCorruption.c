#include "game.h"
#include "math.h"
#include "player.h"
#include "rng.h"
#include "sfx.h"
#include "stats.h"
#include "variables.h"

#define CORRUPTION_MAX 100
#define CORRUPTION_TIER_1_MIN 51
#define CORRUPTION_TIER_2_MIN 81
#define CORRUPTION_TIER_3_MIN 91

// a single gain above this plays SFX_CORRUPTION_GAIN
#define CORRUPTION_GAIN_SFX_MIN 5

/**
 * Add delta to a player's corruption meter (clamped to [0, 100]) and re-evaluate the global
 * GAME_FLAG_CORRUPTION_TIER_* bits from the new value. A gain is scaled by
 * STAT_CORRUPTION_RESIST_PERCENT, which amplifies it while negative.
 *
 * @romaddress 0x0802623c
 */
void player_addCorruption(u8 playerIndex, s8 delta)
{
    u16 amount;
    u16 meter;      // meter before the change
    u32 corruption; // meter after the change, which selects the tier

    if (delta > 0)
    {
        s16 resisted;

        if (PLAYER_STAT(playerIndex, STAT_CORRUPTION_RESIST_PERCENT) < 0)
        {
            resisted = math_percentOf(
                delta, (u16)(-PLAYER_STAT(playerIndex, STAT_CORRUPTION_RESIST_PERCENT)));
            amount = resisted + delta;
        }
        else
        {
            resisted = math_percentOf(
                delta, (u16)PLAYER_STAT(playerIndex, STAT_CORRUPTION_RESIST_PERCENT));
            amount = delta - resisted;
        }
        if ((s16)amount > CORRUPTION_GAIN_SFX_MIN)
        {
            sfx_play(SFX_CORRUPTION_GAIN);
        }
    }
    else
    {
        amount = delta;
    }

    meter = PLAYER(playerIndex).corruption;
    amount = meter + amount;
    if ((s16)amount > CORRUPTION_MAX)
    {
        PLAYER(playerIndex).corruption = CORRUPTION_MAX;
    }
    else if ((s16)amount < 0)
    {
        PLAYER(playerIndex).corruption = 0;
    }
    else
    {
        PLAYER(playerIndex).corruption = amount;
    }

    corruption = PLAYER(playerIndex).corruption;
    if (corruption >= CORRUPTION_TIER_3_MIN)
    {
        if (rng_range(0, 5) == 0)
        {
            sfx_play(SFX_CORRUPTION_TIER_3);
        }
        // BUG: This should also activate tier 1 and 2 so you can't 'skip over' them when a big
        //      hit of corruption pushes from tier 0 to here in one hit.
        g_GameFlags.p |= GAME_FLAG_CORRUPTION_TIER_3;
    }
    else if (corruption >= CORRUPTION_TIER_2_MIN)
    {
        // BUG: This should also activate tier 1 so you can't 'skip over' them when a big hit of
        //      corruption pushes from tier 0 to here in one hit.
        g_GameFlags.p =
            (g_GameFlags.p | GAME_FLAG_CORRUPTION_TIER_2) & ~GAME_FLAG_CORRUPTION_TIER_3;
    }
    else if (corruption >= CORRUPTION_TIER_1_MIN)
    {
        // BUG: This should also clear tier 3 so it doesn't get stuck on when a big negative hit
        //      of corruption is applied.
        //      This scenario is unlikelly to be possible in vanilla but mentioned for completeness.
        g_GameFlags.p =
            (g_GameFlags.p | GAME_FLAG_CORRUPTION_TIER_1) & ~GAME_FLAG_CORRUPTION_TIER_2;
    }
    else
    {
        g_GameFlags.p &= ~(GAME_FLAG_CORRUPTION_TIER_1 | GAME_FLAG_CORRUPTION_TIER_2 |
                           GAME_FLAG_CORRUPTION_TIER_3);
    }
}
