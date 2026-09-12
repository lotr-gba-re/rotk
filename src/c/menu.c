#include "menu.h"
#include "gba_io.h"
#include "input.h"
#include "variables.h"

/**
 * Step a cursor value on the player's up/down press, wrapping or hard-clamping at the
 * bounds.
 *
 * @param val   cursor value to adjust
 * @param min   lower bound
 * @param max   upper bound
 * @param wrap  whether to wrap around instead of hard-clamp
 * @param playerIndex player slot index
 * @return      adjusted value
 *
 * @romaddress 0x08029e10
 */
u32 menu_movePlayerCursorVertical(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex)
{
    if (PLAYER_KEYS_PRESSED(playerIndex) & DPAD_UP)
    {
        if (*val <= min)
        {
            if (wrap)
            {
                *val = max;
            }
        }
        else
        {
            *val = *val - 1;
        }
    }
    else if (PLAYER_KEYS_PRESSED(playerIndex) & DPAD_DOWN)
    {
        if (*val >= max)
        {
            if (wrap)
            {
                *val = min;
            }
        }
        else
        {
            *val = *val + 1;
        }
    }
    return *val;
}

/**
 * Step a cursor value every frame the player holds up/down, wrapping or hard-clamping at
 * the bounds.
 *
 * @param val   cursor value to adjust
 * @param min   lower bound
 * @param max   upper bound
 * @param wrap  whether to wrap around instead of hard-clamp
 * @param playerIndex player slot index
 * @return      adjusted value
 *
 * @romaddress 0x08029e6c
 */
u32 menu_movePlayerCursorVerticalHeld(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex)
{
    if (PLAYER_KEYS_CURRENT(playerIndex) & DPAD_UP)
    {
        if (*val <= min)
        {
            if (wrap)
            {
                *val = max;
            }
        }
        else
        {
            *val = *val - 1;
        }
    }
    else if (PLAYER_KEYS_CURRENT(playerIndex) & DPAD_DOWN)
    {
        if (*val >= max)
        {
            if (wrap)
            {
                *val = min;
            }
        }
        else
        {
            *val = *val + 1;
        }
    }
    return *val;
}

/**
 * Step a cursor value on the player's left/right press, wrapping or hard-clamping at the
 * bounds.
 *
 * @param val   cursor value to adjust
 * @param min   lower bound
 * @param max   upper bound
 * @param wrap  whether to wrap around instead of hard-clamp
 * @param playerIndex player slot index
 * @return      adjusted value
 *
 * @romaddress 0x08029ec8
 */
u32 menu_movePlayerCursorHorizontal(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex)
{
    if (PLAYER_KEYS_PRESSED(playerIndex) & DPAD_LEFT)
    {
        if (*val <= min)
        {
            if (wrap)
            {
                *val = max;
            }
        }
        else
        {
            *val = *val - 1;
        }
    }
    else if (PLAYER_KEYS_PRESSED(playerIndex) & DPAD_RIGHT)
    {
        if (*val >= max)
        {
            if (wrap)
            {
                *val = min;
            }
        }
        else
        {
            *val = *val + 1;
        }
    }
    return *val;
}

/**
 * Step a cursor value every frame the player holds left/right, wrapping or hard-clamping
 * at the bounds.
 *
 * @param val   cursor value to adjust
 * @param min   lower bound
 * @param max   upper bound
 * @param wrap  whether to wrap around instead of hard-clamp
 * @param playerIndex player slot index
 * @return      adjusted value
 *
 * @romaddress 0x08029f24
 */
u32 menu_movePlayerCursorHorizontalHeld(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex)
{
    if (PLAYER_KEYS_CURRENT(playerIndex) & DPAD_LEFT)
    {
        if (*val <= min)
        {
            if (wrap)
            {
                *val = max;
            }
        }
        else
        {
            *val = *val - 1;
        }
    }
    else if (PLAYER_KEYS_CURRENT(playerIndex) & DPAD_RIGHT)
    {
        if (*val >= max)
        {
            if (wrap)
            {
                *val = min;
            }
        }
        else
        {
            *val = *val + 1;
        }
    }
    return *val;
}

/**
 * Step a cursor value on the player's L (down) / R (up) press, wrapping or hard-clamping
 * at the bounds.
 *
 * @param val   cursor value to adjust
 * @param min   lower bound
 * @param max   upper bound
 * @param wrap  whether to wrap around instead of hard-clamp
 * @param playerIndex player slot index
 * @return      adjusted value
 *
 * @romaddress 0x08029f80
 */
u32 menu_movePlayerCursorLr(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex)
{
    if (PLAYER_KEYS_PRESSED(playerIndex) & L_BUTTON)
    {
        if (*val <= min)
        {
            if (wrap)
            {
                *val = max;
            }
        }
        else
        {
            *val = *val - 1;
        }
    }
    else if (PLAYER_KEYS_PRESSED(playerIndex) & R_BUTTON)
    {
        if (*val >= max)
        {
            if (wrap)
            {
                *val = min;
            }
        }
        else
        {
            *val = *val + 1;
        }
    }
    return *val;
}

/**
 * Toggle a byte flag, treating any non-zero value as on.
 *
 * @romaddress 0x08029fdc
 */
void menu_toggleFlag(bool *flag)
{
    *flag = !*flag;
}

/**
 * Step a cursor value up by one on the player's L press, with no way back down: at max it
 * wraps to min when wrap is set and holds otherwise.
 *
 * @param val   cursor value to adjust
 * @param min   lower bound, the value wrapped to
 * @param max   upper bound
 * @param wrap  whether to wrap around instead of hard-clamp
 * @param playerIndex player slot index
 * @return      adjusted value
 *
 * @romaddress 0x08029ff0
 */
u32 menu_advancePlayerCursorL(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex)
{
    if (!(PLAYER_KEYS_PRESSED(playerIndex) & L_BUTTON))
    {
        return *val;
    }

    if (*val >= max)
    {
        if (wrap)
        {
            *val = min;
        }
    }
    else
    {
        *val = *val + 1;
    }
    return *val;
}

/**
 * Step a cursor value on the menu's up/down press, wrapping or hard-clamping at the
 * bounds.
 *
 * @param val   cursor value to adjust
 * @param min   lower bound
 * @param max   upper bound
 * @param wrap  whether to wrap around instead of hard-clamp
 * @param playerIndex player slot index
 * @return      adjusted value
 *
 * @romaddress 0x0802a030
 */
u32 menu_moveCursorVertical(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex)
{
    if (MENU_KEYS_PRESSED(playerIndex) & DPAD_UP)
    {
        if (*val <= min)
        {
            if (wrap)
            {
                *val = max;
            }
        }
        else
        {
            *val = *val - 1;
        }
    }
    else if (MENU_KEYS_PRESSED(playerIndex) & DPAD_DOWN)
    {
        if (*val >= max)
        {
            if (wrap)
            {
                *val = min;
            }
        }
        else
        {
            *val = *val + 1;
        }
    }
    return *val;
}

/**
 * Step a cursor value on the menu's left/right press, wrapping or hard-clamping at the
 * bounds.
 *
 * @param val   cursor value to adjust
 * @param min   lower bound
 * @param max   upper bound
 * @param wrap  whether to wrap around instead of hard-clamp
 * @param playerIndex player slot index
 * @return      adjusted value
 *
 * @romaddress 0x0802a08c
 */
u32 menu_moveCursorHorizontal(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex)
{
    if (MENU_KEYS_PRESSED(playerIndex) & DPAD_LEFT)
    {
        if (*val <= min)
        {
            if (wrap)
            {
                *val = max;
            }
        }
        else
        {
            *val = *val - 1;
        }
    }
    else if (MENU_KEYS_PRESSED(playerIndex) & DPAD_RIGHT)
    {
        if (*val >= max)
        {
            if (wrap)
            {
                *val = min;
            }
        }
        else
        {
            *val = *val + 1;
        }
    }
    return *val;
}
