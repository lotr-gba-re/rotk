// The save manager, replacing the stubbed save_init (save_state.asm).
//
// The stock function formats the header and resets every slot the moment validation fails,
// destroying anything it does not recognize with no confirmation. Nothing here writes the
// chip: state loads from a save that is already current, and a save that is not reaches the
// migration scene with no RAM state derived from it.

#include "save_state.h"

#include "save.h"
#include "save_format.h"
#include "save_slots.h"
#include "save_sram.h"
#include "text.h"
#include "variables.h"

bool modSave_isUpToDate(void)
{
    SaveGlobals header;
    bool reversed;

    if (!modSave_isChipPresent())
    {
        return FALSE;
    }
    return modSave_readHeader(&header, &reversed) == SAVE_MAGIC_CURRENT &&
           modSave_isHeaderChecksumValid(&header);
}

/** Copied from: the stock save_init tail. */
void modSave_loadState(void)
{
    bool reversed;

    modSave_readHeader(&g_SaveGlobals, &reversed);
    text_setLanguage(g_SaveGlobals.d.language);
    save_refreshSlots();
    g_LastSaveSlot = 0;
    // hack RAM, so nothing clears it at power-on
    g_SaveSlotPage = 0;
}
