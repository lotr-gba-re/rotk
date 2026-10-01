#include "actor.h"
#include "game.h"
#include "item.h"
#include "loot.h"
#include "player.h"
#include "scene.h"
#include "scene/backpack.h"
#include "sfx.h"
#include "variables.h"

/**
 * Equip a backpack item, replacing equipment and clearing conflicting hand slots.
 *
 * @return Nonzero on success, zero when the item cannot be equipped.
 *
 * @romaddress 0x0802bdb4
 */
u32 item_equipFromBackpack(u8 backpackSlot, u8 playerIndex)
{
    Item item = PLAYER(playerIndex).inventory.array[backpackSlot];
    u8 inventorySlot = item_getEquipSlot(item.word, playerIndex);
#ifdef BUGFIX
    u8 removalSlot = inventorySlot;
#endif

    if (INVENTORY_SLOT_IS_EQUIPMENT(inventorySlot))
    {
        if (inventorySlot == INVENTORY_SLOT_MAINHAND)
        {
            if (ITEM_BASE_METADATA(item).flags.p & ITEM_FLAG_TWO_HANDED)
            {
                if (PLAYER(playerIndex).inventory.slots.offhand.d.itemType != ITEM_TYPE_EMPTY &&
                    PLAYER(playerIndex).inventory.slots.mainhand.d.itemType == ITEM_TYPE_EMPTY)
                {
#ifdef BUGFIX
                    // Remember the removed item's original slot for stat subtraction below.
                    removalSlot = INVENTORY_SLOT_OFFHAND;
#endif
                    item_swapSlots(INVENTORY_SLOT_MAINHAND, INVENTORY_SLOT_OFFHAND, playerIndex);
                }
                else if (PLAYER(playerIndex).inventory.slots.offhand.d.itemType !=
                             ITEM_TYPE_EMPTY &&
                         (u8)item_unequipToBackpack(INVENTORY_SLOT_OFFHAND, playerIndex) == 0)
                {
                    if (playerIndex == ACTIVE_PLAYER_INDEX)
                    {
                        sfx_play(SFX_MENU_ERROR);
                    }
                    return 0;
                }
            }
        }
        else if (inventorySlot == INVENTORY_SLOT_OFFHAND)
        {
            item = PLAYER(playerIndex).inventory.slots.mainhand;
            if (ITEM_TYPE(item) != ITEM_TYPE_EMPTY &&
                (ITEM_BASE_METADATA(item).flags.p & ITEM_FLAG_TWO_HANDED))
            {
#ifdef BUGFIX
                // Remember the removed item's original slot for stat subtraction below.
                removalSlot = INVENTORY_SLOT_MAINHAND;
#endif
                item_swapSlots(INVENTORY_SLOT_OFFHAND, INVENTORY_SLOT_MAINHAND, playerIndex);
            }
        }

        item = PLAYER(playerIndex).inventory.array[inventorySlot];
        if (ITEM_TYPE(item) != ITEM_TYPE_EMPTY)
        {
#ifdef BUGFIX
            item_applyAffixStats(item, removalSlot, playerIndex, TRUE);
#else
            // BUG: The hand-slot swaps above do not preserve the item's original removal slot.
            // Using the new slot subtracts an old offhand's full damage bonus instead of half,
            // leaving a persistent damage deficit. For an old two-handed weapon, it subtracts
            // only half the bonus, leaving persistent extra damage that stacks on repeated swaps.
            item_applyAffixStats(item, inventorySlot, playerIndex, TRUE);
#endif
        }
        item_swapSlots(inventorySlot, backpackSlot, playerIndex);
        item = PLAYER(playerIndex).inventory.array[inventorySlot];
        item_applyAffixStats(item, inventorySlot, playerIndex, FALSE);
        player_updateEquipFlags(playerIndex);
        if (playerIndex == ACTIVE_PLAYER_INDEX && ITEM_BASE_METADATA(item).equipSfx < 0x128)
        {
            sfx_play(ITEM_BASE_METADATA(item).equipSfx);
        }
    }
    else
    {
        if (playerIndex == ACTIVE_PLAYER_INDEX)
        {
            sfx_play(SFX_MENU_ERROR);
        }
        return 0;
    }
    return 1;
}

/**
 * Move equipment to the first free backpack slot and promote a dual-wieldable offhand.
 *
 * @return Nonzero on success, zero when the backpack is full.
 *
 * @romaddress 0x0802bf94
 */
u32 item_unequipToBackpack(u8 slot, u8 playerIndex)
{
    Item offhand = PLAYER(playerIndex).inventory.slots.offhand;
    u8 backpackSlot;

    for (backpackSlot = INVENTORY_SLOT_BACKPACK_0; backpackSlot <= INVENTORY_SLOT_BACKPACK_7;
         backpackSlot++)
    {
        if (ITEM_TYPE_VIA_SHIFT(PLAYER(playerIndex).inventory.array[backpackSlot]) ==
            ITEM_TYPE_EMPTY)
        {
            item_applyAffixStats(PLAYER(playerIndex).inventory.array[slot], slot, playerIndex,
                                 TRUE);
            item_swapSlots(slot, backpackSlot, playerIndex);
            if (slot == INVENTORY_SLOT_MAINHAND && ITEM_TYPE(offhand) != ITEM_TYPE_EMPTY &&
                (ITEM_BASE_METADATA(offhand).flags.p & ITEM_FLAG_DUAL_WIELD_MASK) ==
                    ITEM_FLAG_DUAL_WIELD_MASK)
            {
                item_applyAffixStats(offhand, INVENTORY_SLOT_OFFHAND, playerIndex, TRUE);
                item_swapSlots(INVENTORY_SLOT_OFFHAND, INVENTORY_SLOT_MAINHAND, playerIndex);
                offhand = PLAYER(playerIndex).inventory.slots.mainhand;
                item_applyAffixStats(offhand, INVENTORY_SLOT_MAINHAND, playerIndex, FALSE);
            }
            player_updateEquipFlags(playerIndex);
            return TRUE;
        }
    }
    if (playerIndex == ACTIVE_PLAYER_INDEX)
    {
        sfx_play(SFX_MENU_ERROR);
    }
    return FALSE;
}

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

#ifdef BUGFIX
            PLAYER(playerIndex).uniquePassivesCollected &= ~(1 << bandIndex);
#else
            // BUG: Subtracting the loot-type base from an existing bit index prevents clearing it.
            PLAYER(playerIndex).uniquePassivesCollected &=
                ~(1 << (bandIndex - LOOT_TYPE_UNIQUE_PASSIVE_MIN));
#endif
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

    if (slot == INVENTORY_SLOT_MAINHAND && ITEM_TYPE(offhand) != ITEM_TYPE_EMPTY)
    {
        Item promoted = offhand;

        if ((ITEM_BASE_METADATA(promoted).flags.p & ITEM_FLAG_DUAL_WIELD_MASK) ==
            ITEM_FLAG_DUAL_WIELD_MASK)
        {
            item_applyAffixStats(offhand, INVENTORY_SLOT_OFFHAND, playerIndex, TRUE);
            item_swapSlots(INVENTORY_SLOT_OFFHAND, INVENTORY_SLOT_MAINHAND, playerIndex);
            offhand = PLAYER(playerIndex).inventory.slots.mainhand;
            item_applyAffixStats(offhand, INVENTORY_SLOT_MAINHAND, playerIndex, FALSE);
        }
    }

    player_updateEquipFlags(playerIndex);
    return TRUE;
}
