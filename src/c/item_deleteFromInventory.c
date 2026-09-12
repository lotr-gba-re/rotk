#include "actor.h"
#include "game.h"
#include "item.h"
#include "player.h"
#include "scene.h"
#include "scene/backpack.h"
#include "sfx.h"
#include "variables.h"

/**
 * Delete the item in an inventory slot: un-apply its stats (equipment always, backpack
 * cells only for carried passives), clear the slot, despawn its backpack-scene icon and
 * rune sprites, and promote a dual-wieldable offhand into an emptied weapon slot.
 *
 * @param slot inventory slot (InventorySlot)
 * @return TRUE always
 *
 * @romaddress 0x0802c08c
 */
bool item_deleteFromInventory(u8 slot, u8 playerIndex)
{
    Item item = PLAYER(playerIndex).inventory.array[slot];
    Item offhand = PLAYER(playerIndex).inventory.slots.offhand;

    if (INVENTORY_SLOT_IS_EQUIPMENT(slot) ||
        (ITEM_BASE_METADATA(item).flags.p & ITEM_FLAG_CARRIED_PASSIVE))
    {
        item_applyAffixStats(item, slot, playerIndex, TRUE);
        if (ITEM_BASE_METADATA(item).flags.p & ITEM_FLAG_CARRIED_PASSIVE)
        {
            u8 bandIndex = item.d.baseIndex - UNIQUE_WEAPON_BAND_END;

            // BUG: bandIndex is already the bit index (it equals lootType -
            // LOOT_TYPE_UNIQUE_PASSIVE_MIN), but the code subtracts the band base a
            // second time as if bandIndex were a lootType. The shift count goes
            // negative (the CPU reads it mod 256, acting as a shift of 32+), the 1
            // shifts out to 0, and the clear is a no-op. Probably intended:
            // PLAYER(playerIndex).uniquePassivesCollected &= ~(1 << bandIndex);
            PLAYER(playerIndex).uniquePassivesCollected &=
                ~(1 << (bandIndex - LOOT_TYPE_UNIQUE_PASSIVE_MIN));
        }
    }

    if (INVENTORY_SLOT_IS_EQUIPMENT(slot))
    {
        Actor *rune = g_BackpackRuneSprites[slot];

        if (rune != NULL && playerIndex == ACTIVE_PLAYER_INDEX)
        {
            rune->flags.p = (rune->flags.p | ACTOR_FLAG_PENDING_REMOVE) & ~ACTOR_FLAG_RENDER;
            g_BackpackRuneSprites[slot] = NULL;
        }
    }

    item_clearSlot(&PLAYER(playerIndex).inventory.array[slot].word);

    if (playerIndex == ACTIVE_PLAYER_INDEX && g_SceneCurrent.id == SCENE_ID_BACKPACK)
    {
        Actor *icon = g_BackpackSlotSprites[slot];
        bool *equippable = &icon->as.backpackIcon.equippable;

        if (*equippable == FALSE)
        {
            *equippable = TRUE;
            scene_backpack_setCursorAnimation(g_BackpackCursorObj->actionState);
        }
        if (INVENTORY_SLOT_IS_EQUIPMENT(slot))
        {
            scene_backpack_clearSlotRect(slot);
        }
        {
            Actor *removed = g_BackpackSlotSprites[slot];

            if (removed != NULL)
            {
                removed->flags.p =
                    (removed->flags.p | ACTOR_FLAG_PENDING_REMOVE) & ~ACTOR_FLAG_RENDER;
                g_BackpackSlotSprites[slot] = NULL;
            }
        }
        sfx_play(SFX_MENU_CONFIRM);
    }

    if (slot == INVENTORY_SLOT_WEAPON && ITEM_TYPE(offhand) != ITEM_TYPE_EMPTY)
    {
        Item promoted = offhand;

        if ((ITEM_BASE_METADATA(promoted).flags.p & ITEM_FLAG_DUAL_WIELD_MASK) ==
            ITEM_FLAG_DUAL_WIELD_MASK)
        {
            item_applyAffixStats(offhand, INVENTORY_SLOT_OFFHAND, playerIndex, TRUE);
            item_swapSlots(INVENTORY_SLOT_OFFHAND, INVENTORY_SLOT_WEAPON, playerIndex);
            offhand = PLAYER(playerIndex).inventory.slots.weapon;
            item_applyAffixStats(offhand, INVENTORY_SLOT_WEAPON, playerIndex, FALSE);
        }
    }

    player_updateEquipFlags(playerIndex);
    return TRUE;
}
