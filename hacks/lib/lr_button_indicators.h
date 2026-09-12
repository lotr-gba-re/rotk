#pragma once

#include "actor.h"
#include "types.h"

// Indicator cell size in px (40x16 frame: rounded button with the letter, plus the sliding
// chevron); handy for laying out text next to a button.
#define LR_INDICATOR_WIDTH 40
#define LR_INDICATOR_HEIGHT 16

/** Which indicator: index into MenuLrButtonIndicatorAnimations (and the actor's recordType). */
enum LrButtonIndicatorIndex
{
    // "L" button, chevron sliding rightward (toward the next page)
    LR_INDICATOR_L = 0,
    // "R" button, chevron sliding leftward
    LR_INDICATOR_R = 1
};

/**
 * Create one of the character screens' animated L/R button indicators. Same sprite setup
 * as the ROM's menu_createLrButtonIndicators @0x08027af0, but pool, side, and position are
 * the caller's choice: overlay pool for hooks into the overlay menus (shop/ranger
 * hollow), main pool for custom scenes without GAME_FLAG_OVERLAY (chapter select).
 *
 * @param mainPool allocate from the main pool/list instead of the overlay pool/list
 * @param index    which button to show (enum LrButtonIndicatorIndex)
 * @param x        screen position in pixels
 * @param y        screen position in pixels
 * @return         the new actor
 */
Actor *hackLib_createLrButtonIndicator(bool mainPool, enum LrButtonIndicatorIndex index, s32 x,
                                       s32 y);

/**
 * Create both indicators at the stock top-corner positions on the main actor list. For
 * overlay menus call the ROM's menu_createLrButtonIndicators by name instead.
 */
void hackLib_createLrButtonIndicatorsInMainPool(void);
