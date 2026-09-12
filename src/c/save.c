#include "save.h"
#include "game.h"
#include "scene.h"
#include "text.h"
#include "types.h"
#include "variables.h"

/**
 * Initialise the given save slot in RAM. Zeroes the whole struct, inserts the SLOT_MISSION_EMPTY
 * mission sentinel, and calculates the checksum.
 *
 * @romaddress 0x0803cc8c
 */
void save_initEmptySlot(SaveSlot *slot)
{
    memset(slot, 0, sizeof(SaveSlot));
    slot->d.mission = SLOT_MISSION_EMPTY;

    slot->d.checksum = 0; // redundant after the memset, but the ROM stores it
    slot->d.checksum = -save_checksum16(slot->halfwords, sizeof(SaveSlot));
}

/**
 * Re-read every save slot's EEPROM payload into the RAM mirror and refresh each slot's
 * mission. Each slot gets exactly one read attempt.
 *
 * @romaddress 0x0803ccbc
 */
void save_refreshSlots(void)
{
    u32 slot;
    s32 retries;

    for (slot = 0; slot < ARRAY_COUNT(g_SaveSlots); slot++)
    {
        retries = 0; // dead retry loop in the ROM: 0 retries = one read attempt per slot
        do
        {
            save_readSlot(&g_SaveSlots[slot], slot);
            if (save_getSlotMission(&g_SaveSlots[slot]) != SLOT_MISSION_EMPTY)
            {
                break;
            }
        } while (retries-- > 0);
    }
}

/**
 * Read a slot into dst, then return its mission (save_getSlotMission).
 *
 * @romaddress 0x0803cd0c
 */
u8 save_readSlotAndGetMission(SaveSlot *slotBuffer, u8 slotIndex)
{
    save_readSlot(slotBuffer, slotIndex);
    return save_getSlotMission(slotBuffer);
}

/**
 * Return a slot's mission: the slot's own mission if the checksum
 * is valid and it is below the empty marker, else SLOT_MISSION_EMPTY.
 *
 * @romaddress 0x0803cd28
 */
u8 save_getSlotMission(SaveSlot *slot)
{
    u16 checksum = save_checksum16(slot->halfwords, sizeof(SaveSlot));
    if (checksum == 0 && slot->d.mission < SLOT_MISSION_EMPTY)
    {
        return slot->d.mission;
    }
    return SLOT_MISSION_EMPTY;
}

/**
 * Stamp the current language id and a "language set" flag into the RAM global save header,
 * so the next save_writeGlobalHeader persists the chosen language. Returns TRUE.
 *
 * @romaddress 0x0803cd50
 */
bool save_stampLanguage(void)
{
    g_SaveGlobals.d.language = text_getLanguage();
    g_SaveGlobals.d.languageSet = TRUE;
    return TRUE;
}

/**
 * Persist the RAM global save header (g_SaveGlobals) to EEPROM if it changed: read back the
 * EEPROM header, recompute the checksum over g_SaveGlobals, and write only when the stored
 * checksum differs. Returns TRUE if EEPROM was rewritten, FALSE otherwise.
 *
 * @romaddress 0x0803cd6c
 */
bool save_writeGlobalHeader(void)
{
    SaveGlobals readBack;
    u16 checksum = 0;

    save_eeprom_readBlocks(0, sizeof(SaveGlobals) / EEPROM_BLOCK_SIZE, &readBack);

    // the field lies inside the summed range, so it has to read 0 while the sum runs
    g_SaveGlobals.d.checksum = checksum;
    g_SaveGlobals.d.checksum = -(save_checksum16(g_SaveGlobals.halfwords, sizeof(g_SaveGlobals)));

    if (readBack.d.checksum != g_SaveGlobals.d.checksum)
    {
        save_eeprom_writeBlocks(0, sizeof(SaveGlobals) / EEPROM_BLOCK_SIZE, &g_SaveGlobals);
        return TRUE;
    }
    return FALSE;
}

/**
 * Calculate a checksum over the given data.
 *
 * Internal inline helper to support call sites that inline this function.
 */
static inline u16 checksum16Inline(const u16 *data, u32 numBytes)
{
    u16 sum = 0;
    s32 words;

    for (words = numBytes / sizeof(u16); words > 0; words--)
    {
        sum += *data++;
    }

    return sum;
}

/**
 * Calculate a checksum over the given data.
 *
 * @romaddress 0x0803cdb4
 */
u16 save_checksum16(const u16 *data, u32 numBytes)
{
    return checksum16Inline(data, numBytes);
}

/**
 * Read the whole EEPROM image from block 0 into dst.
 *
 * @romaddress 0x0803cdd8
 */
void save_eeprom_readAll(void *dst)
{
    save_eeprom_readBlocks(0, EEPROM_TOTAL_BLOCKS, dst);
}

/**
 * Read one save slot's 14-block EEPROM payload into dst while parking the
 * render interrupt (sets GAME_FLAG_SAVE_OP_IN_PROGRESS around the read).
 *
 * @romaddress 0x0803cde8
 */
void save_readSlot(void *dst, u8 slotIndex)
{
    u32 startBlock = SaveSlotStartBlocks[slotIndex];
    g_GameFlags.p |= GAME_FLAG_SAVE_OP_IN_PROGRESS;
    save_eeprom_readBlocks(startBlock, sizeof(SaveSlot) / EEPROM_BLOCK_SIZE, dst);
    g_GameFlags.p &= ~GAME_FLAG_SAVE_OP_IN_PROGRESS;
}

/**
 * Write src to one save slot's 14-block EEPROM payload, parking the render
 * interrupt (sets GAME_FLAG_SAVE_OP_IN_PROGRESS around the write).
 *
 * @romaddress 0x0803ce24
 */
void save_writeSlot(void *src, u8 slotIndex)
{
    u32 startBlock = SaveSlotStartBlocks[slotIndex];
    g_GameFlags.p |= GAME_FLAG_SAVE_OP_IN_PROGRESS;
    save_eeprom_writeBlocks(startBlock, sizeof(SaveSlot) / EEPROM_BLOCK_SIZE, src);
    g_GameFlags.p &= ~GAME_FLAG_SAVE_OP_IN_PROGRESS;
}

/**
 * Read the EEPROM global header into g_SaveGlobals and validate it: the header is good if its
 * halfword checksum is zero AND its first 8 bytes equal SaveHeaderTemplate.
 *
 * @return TRUE if the header is good
 *
 * @romaddress 0x0803ce60
 */
bool save_validateHeader(void)
{
    u16 checksum;
    s32 i;

    save_eeprom_readBlocks(0, sizeof(SaveGlobals) / EEPROM_BLOCK_SIZE, &g_SaveGlobals);

    checksum = checksum16Inline(g_SaveGlobals.halfwords, sizeof(g_SaveGlobals));
    if (checksum != 0)
    {
        return FALSE;
    }

    for (i = 0; i < (s32)sizeof(g_SaveGlobals.d.header); i++)
    {
        if (g_SaveGlobals.d.header[i] != SaveHeaderTemplate[i])
        {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * Reset the global save header to SaveHeaderTemplate and write it to EEPROM: copy the template
 * into g_SaveGlobals, calculate the checksum, zalloc a staging buffer the size of the
 * EEPROM, copy the header into it, and write the full image to block 0.
 *
 * @romaddress 0x0803ceb8
 */
void save_formatHeader(void)
{
    void *buf;

    memcpy(&g_SaveGlobals, &SaveHeaderTemplate, sizeof(SaveGlobals));

    g_SaveGlobals.d.checksum = -checksum16Inline(g_SaveGlobals.halfwords, sizeof(g_SaveGlobals));

    buf = zalloc(EEPROM_SIZE_BYTES);
    memcpy(buf, &g_SaveGlobals, sizeof(SaveGlobals));
    save_eeprom_writeBlocks(0, EEPROM_TOTAL_BLOCKS, buf);
    free(buf);
}

/**
 * Read `count` consecutive 8-byte EEPROM blocks starting at `startBlock` into `dst`,
 * bracketed by the read window (save_eeprom_readBegin / save_eeprom_readEnd).
 *
 * @romaddress 0x0803cf10
 */
void save_eeprom_readBlocks(u32 startBlock, s32 count, void *dst)
{
    u8 *cursor = dst;
    save_eeprom_readBegin();
    for (; count != 0; count--)
    {
        eeprom_readBlock(startBlock, cursor);
        cursor += EEPROM_BLOCK_SIZE;
        startBlock++;
    }
    save_eeprom_readEnd();
}

/**
 * Write `count` consecutive 8-byte EEPROM blocks starting at `startBlock` from `src`,
 * verifying each, bracketed by the read window (save_eeprom_readBegin / save_eeprom_readEnd).
 *
 * @romaddress 0x0803cf40
 */
void save_eeprom_writeBlocks(u32 startBlock, s32 count, void *src)
{
    u8 *cursor = src;
    save_eeprom_readBegin();
    for (; count != 0; count--)
    {
        eeprom_writeBlock(startBlock, cursor);
        eeprom_verifyBlock(startBlock, cursor);
        cursor += EEPROM_BLOCK_SIZE;
        startBlock++;
    }
    save_eeprom_readEnd();
}
