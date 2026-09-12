#include "game.h"
#include "gba.h"
#include "math.h"
#include "player.h"
#include "save.h"
#include "scene.h"
#include "sfx.h"
#include "variables.h"

/**
 * Award initialXp split evenly among the active players: applies the difficulty and
 * STAT_EXTRA_EXP_PERCENT bonuses, doubles a share flagged by PLAYER_COMBAT_FLAG_XP_DOUBLE
 * (consuming the flag), advances the kill counter with its per-500-kill reward and 2000-kill Moria
 * unlock, and processes level-ups (cap 50).
 *
 * @romaddress 0x08010538
 */
void player_awardXp(s16 initialXp)
{
    u8 playerIndex;

    for (playerIndex = 0; playerIndex < g_PlayerGlobals.activePlayerCount; playerIndex++)
    {
        Actor *ownerActor;
        bool leveled = FALSE;
        s32 xp = initialXp / g_PlayerGlobals.activePlayerCount;
        Player *player = &PLAYER(playerIndex);
        u16 *killCountPtr;

        if (player->combatFlags.p & PLAYER_COMBAT_FLAG_XP_DOUBLE)
        {
            xp *= 2;
            player->combatFlags.p &= ~PLAYER_COMBAT_FLAG_XP_DOUBLE;
        }

        ownerActor = player->ownerActor;
        killCountPtr = &player->killCount;
        if (*killCountPtr != 0xffff)
        {
            *killCountPtr += 1;
            if (bios_divMod(*killCountPtr, 500) == 0)
            {
                // every 500 kills: +2 attribute points
                player->attributePoints += 2;
                if (playerIndex == ACTIVE_PLAYER_INDEX)
                {
                    player->statusFlags.p |= PLAYER_STATUS_FLAG_UNSPENT_POINTS;
                }
                if (g_PlayerHuds[playerIndex].field_0x76 == 0)
                {
                    g_PlayerHuds[playerIndex].field_0x76 = 4;
                }
            }
            if (PLAYER(playerIndex).killCount > 1999 &&
                (g_SaveGlobals.d.unlockFlags.p & UNLOCK_FLAG_BONUS_MAP_MORIA) == 0 &&
                playerIndex == ACTIVE_PLAYER_INDEX)
            {
                g_SaveGlobals.d.unlockFlags.p |= UNLOCK_FLAG_BONUS_MAP_MORIA;
                sfx_disableAudio();
                save_writeGlobalHeader();
                sfx_enableAudio();
                sfx_play(SFX_UNLOCK);
            }
        }

        if (PLAYER(playerIndex).level < 50)
        {
            s16 bonus;
            u32 extra;

            if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_GRUELING)
            {
                bonus = math_percentOf(xp, 50);
            }
            else if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_HARD)
            {
                bonus = math_percentOf(xp, 25);
            }
            else
            {
                bonus = 0;
            }
            if (g_GameFlags.p & GAME_FLAG_COOP_ACTIVE)
            {
                s32 sum = bonus + math_percentOf(xp, 25);

                bonus = sum;
            }
            xp += bonus;
            extra = math_percentOf(xp, PLAYER_STAT(playerIndex, STAT_EXTRA_EXP_PERCENT));
            xp += PLAYER(playerIndex).experience + (s16)extra;
            while ((u32)xp >= XpThresholds[PLAYER(playerIndex).level])
            {
                xp -= XpThresholds[PLAYER(playerIndex).level];
                PLAYER(playerIndex).level += 1;
                PLAYER(playerIndex).attributePoints += 5;
                PLAYER(playerIndex).skillPoints += 1;
                leveled = TRUE;
            }
            if (leveled)
            {
                player_addHp(playerIndex, PLAYER_STAT(playerIndex, STAT_MAX_HP), FALSE);
                player_addSpirit(playerIndex, PLAYER_STAT(playerIndex, STAT_MAX_SPIRIT));
                player_spawnLevelUpFx(ownerActor);
                if (playerIndex == ACTIVE_PLAYER_INDEX)
                {
                    PLAYER(playerIndex).statusFlags.p |= PLAYER_STATUS_FLAG_UNSPENT_POINTS;
                }
                if (g_PlayerHuds[playerIndex].field_0x76 == 0)
                {
                    g_PlayerHuds[playerIndex].field_0x76 = 4;
                }
                sfx_play(SFX_LEVEL_UP);
            }
            PLAYER(playerIndex).experience = xp;
            g_PlayerHuds[playerIndex].flags.p |= 1 << 5;
        }
    }
}
