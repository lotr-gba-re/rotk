// Where a slot lives on the chip. The stock game addresses 4 slots through
// SaveSlotStartBlocks; this addresses SAVE_SLOT_PAGE_COUNT pages of 4, indexed by
// g_SaveSlotPage, so the storage exists whether or not anything navigates to it.
//
// Slot addressing ported from the fully assembly-based SavSlotPages ROM-hack by
// SuperSaiyajinStackZ.

#include "save_slots.h"

#include "game.h"
#include "save.h"
#include "save_layout.h"
#include "save_sram.h"
#include "variables.h"

/**
 * First EEPROM block index for (page, on-page slot): globals header, then
 * SAVE_SLOT_BLOCK_COUNT blocks per slot.
 */
#define SAVE_SLOT_EEPROM_BLOCK(page, slot)                                                         \
    (SAVE_GLOBALS_BLOCK_COUNT + ((page) * SAVE_SLOTS_PER_PAGE + (slot)) * SAVE_SLOT_BLOCK_COUNT)

/**
 * REPLACES (whole function): save_readSlot @0x0803cde8.
 */
void modSave_readSlot(void *dst, u8 slotIndex)
{
    g_GameFlags.d.saveOpInProgress = TRUE;
    save_eeprom_readBlocks(SAVE_SLOT_EEPROM_BLOCK(g_SaveSlotPage, slotIndex), SAVE_SLOT_BLOCK_COUNT,
                           dst);
    g_GameFlags.d.saveOpInProgress = FALSE;
}

/**
 * REPLACES (whole function): save_writeSlot @0x0803ce24.
 */
void modSave_writeSlot(void *src, u8 slotIndex)
{
    g_GameFlags.d.saveOpInProgress = TRUE;
    save_eeprom_writeBlocks(SAVE_SLOT_EEPROM_BLOCK(g_SaveSlotPage, slotIndex),
                            SAVE_SLOT_BLOCK_COUNT, src);
    g_GameFlags.d.saveOpInProgress = FALSE;
}

void modSave_resetAllSlots(void)
{
    // Blank the whole chip before seeding it. The stock reset only rewrites the slots it
    // knows about, which leaves every region a later save format added (hollow pages, the
    // spare tail) holding whatever the cart came with - and an all-zero region is the one
    // pattern that reads back as real data (see SRAM_BLANK).
    modSave_fill(0, SRAM_SIZE, SRAM_BLANK);

    // The fill above erased the header; g_SaveGlobals still holds it.
    save_eeprom_writeBlocks(0, SAVE_GLOBALS_BLOCK_COUNT, &g_SaveGlobals);

    // One cleared buffer covers all writes (identical contents).
    save_initEmptySlot(&g_SaveSlots[0]);
    for (u32 page = 0; page < SAVE_SLOT_PAGE_COUNT; page++)
    {
        g_SaveSlotPage = page;
        for (u32 slot = 0; slot < SAVE_SLOTS_PER_PAGE; slot++)
        {
            modSave_writeSlot(&g_SaveSlots[0], slot);
        }
    }
    g_SaveSlotPage = 0;
}
