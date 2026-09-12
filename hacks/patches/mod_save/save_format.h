// Reading the save header, and the byte order it may arrive in. Both the state loader and
// the migration need this, so it belongs to neither.
#pragma once

#if !HACK_mod_save
#error "requires the mod_save patch for the save format"
#endif

#include "save.h"
#include "types.h"

/** Which format family a header's magic bytes name. */
enum SaveMagicFamily
{
    SAVE_MAGIC_UNRECOGNIZED = 0,
    SAVE_MAGIC_VANILLA = 1, // "ROTKGBA6": a vanilla or SavSlotPages image
    SAVE_MAGIC_CURRENT = 2,
    SAVE_MAGIC_NEWER = 3,
};

/**
 * Read the header in whichever byte order it is in and report its magic family; `header`
 * comes back in game order, and `reversed` says whether the chip holds it backwards. Magic
 * only: the checksum verdict stays with the callers, which need to tell a damaged header
 * from an unknown one.
 */
enum SaveMagicFamily modSave_readHeader(SaveGlobals *header, bool *reversed);

/** TRUE when the header's own checksum covers it. */
bool modSave_isHeaderChecksumValid(const SaveGlobals *header);

/**
 * All-0x00 / all-0xFF header: a chip no save format has touched. `fill` gets the fill
 * byte, which the migration scene reports.
 */
bool modSave_isHeaderBlank(const SaveGlobals *header, u8 *fill);

/**
 * Reverse every 8-byte block in place, converting between emulator EEPROM order and the
 * game's struct order. Reversing twice gives the original back, so one routine does both
 * directions.
 */
void modSave_reverseBlocks(u8 *buf, u32 numBytes);

/**
 * Stamp the current magic + version over the mirrored header and refresh its checksum,
 * making whatever the mirror holds a valid current header.
 */
void modSave_stampHeaderMirror(void);

/**
 * Load the ROM template's defaults into the mirror, stamped current: the exact header a
 * fresh format writes.
 */
void modSave_loadHeaderTemplate(void);
