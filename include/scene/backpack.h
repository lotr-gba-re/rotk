#pragma once

#include "types.h"

// Scene 0x0a: backpack / inventory.
void scene_backpack_enter(void);
void scene_backpack_tick(void);
void scene_backpack_exit(void);

/**
 * Set the backpack cursor's animation for the grid cell it sits on (equipment rows,
 * backpack cells, whetstone, consumables); a backpack cell's item icon with
 * as.backpackIcon.equippable == FALSE picks the cannot-equip variant.
 *
 * @param cell the cursor's grid cell (g_BackpackCursorObj->actionState)
 */
void scene_backpack_setCursorAnimation(u8 cell);

/** Clear the BG2/BG3 tile rect behind an equipment cell's icon. */
void scene_backpack_clearSlotRect(u8 cell);
