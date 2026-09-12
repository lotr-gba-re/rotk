// Slot addressing: where a slot lives on the chip, and resetting them all. The chip always
// holds SAVE_SLOT_PAGE_COUNT pages of slots; reaching past the first page is the
// save_slot_pages feature, which moves g_SaveSlotPage.
#pragma once

#if !HACK_mod_save
#error "requires the mod_save patch for slot addressing"
#endif

#include "types.h"

/**
 * Which page of slots the menu is on, and therefore which slots the addressing below
 * reaches. Persistent so the player does not scroll back on every save.
 */
extern u8 g_SaveSlotPage;

/** Read the current page's slot `slotIndex` (0..3) into dst. Replaces save_readSlot. */
void modSave_readSlot(void *dst, u8 slotIndex);

/** Write src to the current page's slot `slotIndex` (0..3). Replaces save_writeSlot. */
void modSave_writeSlot(void *src, u8 slotIndex);

/**
 * Blank the chip and seed an empty slot on every page. The caller has just filled
 * g_SaveGlobals; the header is rewritten from the mirror after the blanking.
 */
void modSave_resetAllSlots(void);
