#include "actor.h"
#include "item.h"
#include "player.h"
#include "scene.h"
#include "scene/backpack.h"
#include "variables.h"

/**
 * Swap two inventory slots and update their backpack-scene sprites.
 * Stat modifiers are not adjusted.
 *
 * @romaddress 0x0802e940
 */
u32 item_swapSlots(u8 slotA, u8 slotB, u8 playerIndex)
{
    Item temp;

    scene_backpack_updateRuneSprites(slotA, slotB, playerIndex);

    temp = PLAYER(playerIndex).inventory.array[slotA];
    PLAYER(playerIndex).inventory.array[slotA] = PLAYER(playerIndex).inventory.array[slotB];
    PLAYER(playerIndex).inventory.array[slotB] = temp;

    if (playerIndex == ACTIVE_PLAYER_INDEX && g_SceneCurrent.id == SCENE_ID_BACKPACK)
    {
        scene_backpack_swapSlotSprites(slotA, slotB);
        if (slotA >= INVENTORY_SLOT_BACKPACK_0 && slotA <= INVENTORY_SLOT_BACKPACK_7 &&
            g_BackpackSlotSprites[slotA]->as.backpackIcon.equippable == FALSE)
        {
            scene_backpack_setCursorAnimation(g_BackpackCursorObj->actionState);
        }
    }
    return TRUE;
}
