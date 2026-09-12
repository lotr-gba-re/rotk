// Converting an older save image to the current one. Nothing here runs unattended: the
// classification leaves the chip as it found it, and every write waits for the migration
// scene's double-confirmed Yes.

#include "save_migrate.h"

#include "memory.h"
#if HACK_ranger_hollow_pages
#include "patches/ranger_hollow_pages/ranger_hollow_pages.h"
#endif
#include "save.h"
#include "save_format.h"
#include "save_layout.h"
#include "save_slots.h"
#include "save_sram.h"
#include "save_state.h"
#include "variables.h"

/**
 * The largest EEPROM-emulator .sav is 8 KiB, and those are the only reversed images, so
 * byte-order reversal never needs to reach further.
 */
#define EEPROM_IMAGE_BLOCK_COUNT (0x2000u / EEPROM_BLOCK_SIZE)

/** Slots a vanilla image defines past the header. */
#define VANILLA_SLOT_COUNT 4u

/** The reversal operates per 8-byte EEPROM-order block, hence the block-at-a-time staging. */
static void reverseImage(void)
{
    u8 buf[EEPROM_BLOCK_SIZE];

    for (u32 block = 0; block < EEPROM_IMAGE_BLOCK_COUNT; block++)
    {
        u8 *sram = SRAM_BASE + block * EEPROM_BLOCK_SIZE;
        modSave_copy(buf, sram, EEPROM_BLOCK_SIZE);
        modSave_reverseBlocks(buf, sizeof(buf));
        modSave_copy(sram, buf, EEPROM_BLOCK_SIZE);
    }
}

/**
 * All-0x00 / all-0xFF: blank cells whose checksum trivially validates, so a flash-cart or
 * emulator that inits SRAM to either is never mistaken for a meaningful slot.
 */
static bool isSlotBlank(const SaveSlot *slot)
{
    bool allFF = TRUE;
    bool all00 = TRUE;
    for (u32 i = 0; i < sizeof(slot->bytes); i++)
    {
        if (slot->bytes[i] != 0xFF)
        {
            allFF = FALSE;
        }
        if (slot->bytes[i] != 0x00)
        {
            all00 = FALSE;
        }
    }
    return allFF || all00;
}

static bool isSlotChecksumValid(const SaveSlot *slot)
{
    return save_checksum16(slot->halfwords, sizeof(*slot)) == 0;
}

/**
 * Read a 14-block slot by its flat index (the source image has no pages yet), undoing
 * emulator byte order on the way out when the image is in it.
 */
static void readFlatSlot(u32 slotIndex, SaveSlot *dst, bool reversed)
{
    save_eeprom_readBlocks(SAVE_GLOBALS_BLOCK_COUNT + slotIndex * SAVE_SLOT_BLOCK_COUNT,
                           SAVE_SLOT_BLOCK_COUNT, dst);
    if (reversed)
    {
        modSave_reverseBlocks(dst->bytes, sizeof(*dst));
    }
}

/**
 * Split a vanilla-magic (ROTKGBA6) image into vanilla vs SavSlotPages, into the type field.
 *
 * The base game and SavSlotPages are the only such images in the wild, and vanilla defines
 * slots 0..3 only, so a valid slot checksum anywhere past those (a save, or the reset
 * image a SavSlotPages format writes to every slot) marks the 72-slot layout. The blank
 * skip matters: a vanilla .sav zero-padded by an emulator would otherwise checksum-validate
 * in every padded slot and misdetect as SavSlotPages, preserving the zeros instead of
 * blanking them.
 */
static void classifyVanillaMagic(SaveMigrateInfo *out, bool reversed)
{
    SaveSlot slot;

    out->type = SAVE_MIGRATE_TYPE_VANILLA;
    for (u32 i = VANILLA_SLOT_COUNT; i < SAVE_SLOT_PAGE_COUNT * SAVE_SLOTS_PER_PAGE; i++)
    {
        readFlatSlot(i, &slot, reversed);
        if (!isSlotBlank(&slot) && isSlotChecksumValid(&slot))
        {
            out->type = SAVE_MIGRATE_TYPE_SAV_SLOT_PAGES;
            return;
        }
    }
}

static void classifyUnrecognized(SaveMigrateInfo *out, u8 reason)
{
    out->type = SAVE_MIGRATE_TYPE_UNRECOGNIZED;
    out->detail = reason;
    // the first block as it sits on the chip (not normalized), for the scene's dump
    modSave_copy(out->header, SRAM_BASE, sizeof(out->header));
}

void modSave_classify(SaveMigrateInfo *out)
{
    SaveGlobals header;
    enum SaveMagicFamily magic;
    bool reversed;

    // every path below sets only the fields its type defines; the rest read zero
    memory_memset(out, 0, sizeof(*out));

    if (!modSave_isChipPresent())
    {
        // no save medium: nothing can be read or written, so the scene says so
        out->type = SAVE_MIGRATE_TYPE_NO_SRAM;
        return;
    }

    magic = modSave_readHeader(&header, &reversed);
    if (modSave_isHeaderBlank(&header, &out->detail))
    {
        out->type = SAVE_MIGRATE_TYPE_EMPTY; // detail = the fill byte
        return;
    }
    if (magic == SAVE_MAGIC_UNRECOGNIZED || !modSave_isHeaderChecksumValid(&header))
    {
        classifyUnrecognized(out, magic == SAVE_MAGIC_UNRECOGNIZED
                                      ? SAVE_MIGRATE_UNRECOGNIZED_MAGIC
                                      : SAVE_MIGRATE_UNRECOGNIZED_CHECKSUM);
        return;
    }

    switch (magic)
    {
    case SAVE_MAGIC_CURRENT:
        out->type = SAVE_MIGRATE_TYPE_NONE; // nothing to do; boot skips the scene
        break;
    case SAVE_MAGIC_NEWER:
        // a newer save is not garbage: the scene offers neither Yes nor No
        out->type = SAVE_MIGRATE_TYPE_NEWER;
        out->version = header.d.header[SAVE_HEADER_MAGIC_LEN];
        break;
    case SAVE_MAGIC_VANILLA:
        classifyVanillaMagic(out, reversed);
        break;
    default:
        // a family with no case above: refuse it rather than guess at its layout
        classifyUnrecognized(out, SAVE_MIGRATE_UNRECOGNIZED_MAGIC);
        break;
    }
}

bool modSave_canMigrate(const SaveMigrateInfo *info)
{
    return info->type != SAVE_MIGRATE_TYPE_NONE && info->type != SAVE_MIGRATE_TYPE_NEWER &&
           info->type != SAVE_MIGRATE_TYPE_NO_SRAM;
}

/**
 * Blank everything the source format never defined, out to the end of the chip.
 *
 * Without this a migrated image keeps whatever the cart or .sav happened to hold beyond the
 * old layout, and the stock format path will not clean it up later: it only runs when the
 * header fails to validate, which after a migration it no longer does. An all-zero region is
 * the dangerous case - it passes save_checksum16, so save_getSlotMission reports mission 0
 * and the load menu fills with saves that were never made (see SRAM_BLANK).
 */
static void blankUndefinedRegion(u8 type)
{
    // vanilla defines the header + its 4 slots and nothing else; SavSlotPages defines the
    // whole slot region, and the hollow region above it is new.
    u32 offset = type == SAVE_MIGRATE_TYPE_SAV_SLOT_PAGES
                     ? HOLLOW_PAGE_BYTE_BASE
                     : SAVE_BLOCKS_TO_BYTES(SAVE_GLOBALS_BLOCK_COUNT +
                                            VANILLA_SLOT_COUNT * SAVE_SLOT_BLOCK_COUNT);

    modSave_fill(offset, SRAM_SIZE - offset, SRAM_BLANK);
}

void modSave_migrate(const SaveMigrateInfo *info)
{
    bool reversed;

    // The migrated header is the source header with the magic restamped (options, unlocks,
    // hollow items all carry over), and the restamp works on the mirror, so the mirror has
    // to hold the source header before the image moves. Only the two migratable types reach
    // this far, so the read is a validated one.
    modSave_readHeader(&g_SaveGlobals, &reversed);

    if (reversed)
    {
        reverseImage(); // all reads/writes below are now game-order
    }

    blankUndefinedRegion(info->type);

#if HACK_ranger_hollow_pages
    rangerHollowPages_migrateStash();
#endif

    modSave_stampHeaderMirror();
    save_eeprom_writeBlocks(0, SAVE_GLOBALS_BLOCK_COUNT, &g_SaveGlobals);

    // the save is current now, so the state loads the way it would have at boot
    modSave_loadState();
}

void modSave_format(void)
{
    // modSave_resetAllSlots blanks the whole chip and then writes the header from the
    // mirror, so loading the template into the mirror is the only header work needed.
    modSave_loadHeaderTemplate();
    modSave_resetAllSlots();
    modSave_loadState();
}
