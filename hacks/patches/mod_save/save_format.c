// Reading and stamping the save header, and the byte-order conversion that goes with it.
//
// Emulator EEPROM .sav files store the raw serial stream, so every 8-byte block is
// byte-reversed vs the game struct order the SRAM backend keeps (see rotkit eeprom-fix).
// Only an imported image is ever in that order; anything this ROM writes is always game
// order.

#include "save_format.h"

#include "memory.h"
#include "save_layout.h"
#include "save_sram.h"
#include "variables.h"

void modSave_reverseBlocks(u8 *buf, u32 numBytes)
{
    for (u32 off = 0; off < numBytes; off += EEPROM_BLOCK_SIZE)
    {
        for (u32 i = 0; i < EEPROM_BLOCK_SIZE / 2; i++)
        {
            u8 tmp = buf[off + i];
            buf[off + i] = buf[off + EEPROM_BLOCK_SIZE - 1 - i];
            buf[off + EEPROM_BLOCK_SIZE - 1 - i] = tmp;
        }
    }
}

bool modSave_isHeaderChecksumValid(const SaveGlobals *header)
{
    return save_checksum16(header->halfwords, sizeof(*header)) == 0;
}

static bool magicMatches(const u8 *header, const char *magic, u32 len)
{
    for (u32 i = 0; i < len; i++)
    {
        if (header[i] != (u8)magic[i])
        {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * Reversal is per-block and the vanilla magic fills block 0 exactly, so reading that block
 * backwards matches an emulator-order header ("ROTKGBA6" reads "6ABGTKOR"). `magic` must be
 * block-sized; this is only ever called with SAVE_HEADER_MAGIC_VANILLA.
 */
static bool magicMatchesReversed(const u8 *header, const char *magic)
{
    for (u32 i = 0; i < EEPROM_BLOCK_SIZE; i++)
    {
        if (header[EEPROM_BLOCK_SIZE - 1 - i] != (u8)magic[i])
        {
            return FALSE;
        }
    }
    return TRUE;
}

/** Takes a game-order header; modSave_readHeader has already settled the byte order. */
static enum SaveMagicFamily classifyMagic(const u8 *header)
{
    if (magicMatches(header, SAVE_HEADER_MAGIC_VANILLA, sizeof(SAVE_HEADER_MAGIC_VANILLA) - 1))
    {
        return SAVE_MAGIC_VANILLA;
    }
    if (magicMatches(header, SAVE_HEADER_MAGIC, SAVE_HEADER_MAGIC_LEN))
    {
        return header[SAVE_HEADER_MAGIC_LEN] == SAVE_HEADER_VERSION ? SAVE_MAGIC_CURRENT
                                                                    : SAVE_MAGIC_NEWER;
    }
    return SAVE_MAGIC_UNRECOGNIZED;
}

enum SaveMagicFamily modSave_readHeader(SaveGlobals *header, bool *reversed)
{
    modSave_copy((u8 *)header, SRAM_BASE, sizeof(*header));

    // Only a vanilla-magic image can arrive reversed, so one probe for the backwards vanilla
    // magic settles the byte order and classification below always sees game order.
    *reversed = magicMatchesReversed(header->d.header, SAVE_HEADER_MAGIC_VANILLA);
    if (*reversed)
    {
        modSave_reverseBlocks((u8 *)header, sizeof(*header));
    }

    return classifyMagic(header->d.header);
}

/** Reversal-invariant, so it reads the same before or after the byte-order probe. */
bool modSave_isHeaderBlank(const SaveGlobals *header, u8 *fill)
{
    const u8 *bytes = (const u8 *)header;

    *fill = bytes[0];
    if (*fill != 0x00u && *fill != SRAM_BLANK)
    {
        return FALSE;
    }
    for (u32 i = 1; i < sizeof(*header); i++)
    {
        if (bytes[i] != *fill)
        {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * The ROM's SaveHeaderTemplate keeps the vanilla magic: its only readers
 * (save_validateHeader, save_formatHeader) are dead code behind the stubbed
 * save_init, so the current magic + version live only in C (save_layout.h).
 */
void modSave_stampHeaderMirror(void)
{
    memory_memcpy(g_SaveGlobals.d.header, SAVE_HEADER_MAGIC, SAVE_HEADER_MAGIC_LEN);
    g_SaveGlobals.d.header[SAVE_HEADER_MAGIC_LEN] = SAVE_HEADER_VERSION;
    // zero the checksum field first so the negated sum over the whole header is zero
    g_SaveGlobals.d.checksum = 0;
    g_SaveGlobals.d.checksum =
        (u16)-save_checksum16(g_SaveGlobals.halfwords, sizeof(g_SaveGlobals));
}

void modSave_loadHeaderTemplate(void)
{
    memory_memcpy(&g_SaveGlobals, SaveHeaderTemplate, sizeof(g_SaveGlobals));
    modSave_stampHeaderMirror();
}
