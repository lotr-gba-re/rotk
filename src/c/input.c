#include "input.h"
#include "gba_io.h"
#include "player.h"
#include "scene/gameplay.h"
#include "variables.h"

/**
 * Zero all input arrays (both edge-detect sets, both players, and the link-current
 * buffer). Called when input is disabled.
 *
 * @romaddress 0x08029a5c
 */
void input_clear(void)
{
    s32 i;

    for (i = 0; i < 2; i++)
    {
        PLAYER_KEYS_CURRENT(i) = 0;
        PLAYER_KEYS_PREV(i) = 0;
        PLAYER_KEYS_PRESSED(i) = 0;
        PLAYER_KEYS_RELEASED(i) = 0;

        MENU_KEYS_CURRENT(i) = 0;
        MENU_KEYS_PREV(i) = 0;
        MENU_KEYS_PRESSED(i) = 0;
        MENU_KEYS_RELEASED(i) = 0;

        g_LinkKeysCurrent[i] = 0;
    }
}

/**
 * Zero one player's slot in every input array and restart the idle counter. The
 * single-player branch of scene_gameplay_enter runs this so a held key from the menu does
 * not carry into gameplay.
 *
 * @romaddress 0x08029aec
 */
void input_clearPlayer(u8 playerIndex)
{
    PLAYER_KEYS_CURRENT(playerIndex) = 0;
    PLAYER_KEYS_PREV(playerIndex) = 0;
    PLAYER_KEYS_PRESSED(playerIndex) = 0;
    PLAYER_KEYS_RELEASED(playerIndex) = 0;

    MENU_KEYS_CURRENT(playerIndex) = 0;
    MENU_KEYS_PREV(playerIndex) = 0;
    MENU_KEYS_PRESSED(playerIndex) = 0;
    MENU_KEYS_RELEASED(playerIndex) = 0;

    g_LinkKeysCurrent[playerIndex] = 0;

    g_InputIdleCounter = 0;
}

/**
 * Clear only the dpad bits of one player's slot in every input array, leaving the face and
 * shoulder buttons alone, and restart the idle counter. The co-op branch of
 * scene_gameplay_enter runs this for both slots.
 *
 * @romaddress 0x08029b58
 */
void input_clearPlayerDpad(u8 playerIndex)
{
    PLAYER_KEYS_CURRENT(playerIndex) &= ~DPAD_ANY;
    PLAYER_KEYS_PREV(playerIndex) &= ~DPAD_ANY;
    PLAYER_KEYS_PRESSED(playerIndex) &= ~DPAD_ANY;
    PLAYER_KEYS_RELEASED(playerIndex) &= ~DPAD_ANY;

    MENU_KEYS_CURRENT(playerIndex) &= ~DPAD_ANY;
    MENU_KEYS_PREV(playerIndex) &= ~DPAD_ANY;
    MENU_KEYS_PRESSED(playerIndex) &= ~DPAD_ANY;
    MENU_KEYS_RELEASED(playerIndex) &= ~DPAD_ANY;

    g_LinkKeysCurrent[playerIndex] &= ~DPAD_ANY;

    g_InputIdleCounter = 0;
}

/**
 * Per-frame input sampling with press/release edge detection using previous frame's input data.
 *
 * @romaddress 0x08029c04
 */
void input_update(void)
{
    u16 keys;
    s32 i;

    if (g_InputDisableFlags[0] != 0)
    {
        input_clear();
    }
    else
    {
        if (g_PlayerGlobals.activePlayerCount > 1)
        {
            for (i = 0; i < 2; i++)
            {
                PLAYER_KEYS_PREV(i) = PLAYER_KEYS_CURRENT(i);
                PLAYER_KEYS_CURRENT(i) = g_LinkKeysCurrent[i];
                PLAYER_KEYS_PRESSED(i) =
                    (PLAYER_KEYS_CURRENT(i) ^ PLAYER_KEYS_PREV(i)) & PLAYER_KEYS_CURRENT(i);
                PLAYER_KEYS_RELEASED(i) =
                    PLAYER_KEYS_PREV(i) & (PLAYER_KEYS_CURRENT(i) ^ PLAYER_KEYS_PREV(i));
            }

            ACTIVE_MENU_KEYS_PREV = ACTIVE_MENU_KEYS_CURRENT;
            ACTIVE_MENU_KEYS_CURRENT = REG_KEYINPUT ^ KEYINPUT_KEY_MASK;
            ACTIVE_MENU_KEYS_PRESSED =
                (ACTIVE_MENU_KEYS_CURRENT ^ ACTIVE_MENU_KEYS_PREV) & ACTIVE_MENU_KEYS_CURRENT;
            ACTIVE_MENU_KEYS_RELEASED =
                ACTIVE_MENU_KEYS_PREV & (ACTIVE_MENU_KEYS_CURRENT ^ ACTIVE_MENU_KEYS_PREV);
        }
        else
        {
            PLAYER_KEYS_PREV(0) = PLAYER_KEYS_CURRENT(0);

            keys = REG_KEYINPUT;
            keys ^= KEYINPUT_KEY_MASK;

            PLAYER_KEYS_CURRENT(0) = keys;
            PLAYER_KEYS_PRESSED(0) = keys & (keys ^ PLAYER_KEYS_PREV(0));
            PLAYER_KEYS_RELEASED(0) = (keys ^ PLAYER_KEYS_PREV(0)) & PLAYER_KEYS_PREV(0);

            MENU_KEYS_PREV(0) = MENU_KEYS_CURRENT(0);
            MENU_KEYS_CURRENT(0) = keys;
            MENU_KEYS_PRESSED(0) = keys & (keys ^ MENU_KEYS_PREV(0));
            MENU_KEYS_RELEASED(0) = (keys ^ MENU_KEYS_PREV(0)) & MENU_KEYS_PREV(0);
        }

        if ((PLAYER_KEYS_CURRENT(ACTIVE_PLAYER_INDEX) & KEYINPUT_KEY_MASK) == 0)
        {
            g_InputIdleCounter += 1;
        }
        else
        {
            g_InputIdleCounter = 0;
        }
    }
}

/**
 * Bring the input subsystem up at boot: enable sampling and zero every key array.
 *
 * @romaddress 0x08029db0
 */
void input_init(void)
{
    input_enable();
    input_clear();
}

/**
 * Clear both slots' input-disable flags, so input_update samples the keypad again.
 *
 * @romaddress 0x08029dc0
 */
void input_enable(void)
{
    s32 i;

    for (i = 0; i < 2; i++)
    {
        g_InputDisableFlags[i] = 0;
    }
}

/**
 * Set both slots' input-disable flags, so input_update clears the key arrays instead
 * of sampling the keypad.
 *
 * @romaddress 0x08029dd8
 */
void input_disable(void)
{
    s32 i;

    for (i = 0; i < 2; i++)
    {
        g_InputDisableFlags[i] = 1;
    }
}

/**
 * The 8-way Direction the player is currently holding on the dpad, or DIRECTION_NONE when
 * nothing or an opposing pair is held (DirectionsFromDpad). Reads the held keys, not the
 * press edge.
 *
 * @romaddress 0x08029df0
 */
u8 input_getPlayerDpadDirection(u8 playerIndex)
{
    return DirectionsFromDpad[(PLAYER_KEYS_CURRENT(playerIndex) & DPAD_ANY) >> 4];
}
