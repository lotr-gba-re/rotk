// Converting an older save image to the current one: what a classification pass found, the
// run that converts it, and the scene that asks before running it.
#pragma once

#if !HACK_mod_save
#error "requires the mod_save patch for save migration"
#endif
#if !HACK_mod_scenes
#error "requires the mod_scenes patch for the confirmation scene's ModSceneHandlers row"
#endif
#if !HACK_mod_startup
#error "requires the mod_startup patch for the boot entry that starts the migration scene"
#endif

#include "types.h"

/** Detected source format. */
enum SaveMigrateType
{
    SAVE_MIGRATE_TYPE_NONE = 0,    // valid current image: nothing to do
    SAVE_MIGRATE_TYPE_VANILLA = 1, // base-game 512B image (prefix-compatible)
    // SavSlotPages 8 KiB image; its 72 slots match the canonical slot region byte for byte
    SAVE_MIGRATE_TYPE_SAV_SLOT_PAGES = 2,
    SAVE_MIGRATE_TYPE_NEWER = 3,        // newer ROTKRCM version this build does not know
    SAVE_MIGRATE_TYPE_NO_SRAM = 4,      // the poke check failed: no save medium at all
    SAVE_MIGRATE_TYPE_EMPTY = 5,        // blank chip (header all one fill byte)
    SAVE_MIGRATE_TYPE_UNRECOGNIZED = 6, // unknown magic or damaged header checksum
};

/** Why an UNRECOGNIZED image failed (SaveMigrateInfo.detail). */
enum SaveMigrateUnrecognizedReason
{
    SAVE_MIGRATE_UNRECOGNIZED_MAGIC = 0,    // no known magic in either byte order
    SAVE_MIGRATE_UNRECOGNIZED_CHECKSUM = 1, // known magic, header checksum invalid
};

/**
 * Everything the scene and the migration run need to know about the image on the chip.
 * Only `type` is always meaningful: `version` belongs to NEWER, `detail` and `header` to
 * EMPTY / UNRECOGNIZED. The rest read zero. The byte order is not carried: the migration
 * run re-reads the header and gets it from the same probe.
 */
typedef struct SaveMigrateInfo
{
    u8 type;      // enum SaveMigrateType
    u8 version;   // header version byte (the newer-version screen shows it)
    u8 detail;    // EMPTY: the fill byte; UNRECOGNIZED: enum SaveMigrateUnrecognizedReason
    u8 header[8]; // UNRECOGNIZED: raw first header block as read, for the scene's dump
} SaveMigrateInfo;

/**
 * Classify the save image on the chip for the scene. Leaves the chip as it found it (the
 * presence probe restores the cell it drives); every write waits for the scene's confirmed
 * Yes.
 */
void modSave_classify(SaveMigrateInfo *out);

/**
 * TRUE when the classified image has a Yes path the scene can offer (migrate, or a fresh
 * format for EMPTY / UNRECOGNIZED). FALSE for NEWER and NO_SRAM, which cannot be written,
 * and for NONE, which is already current and would lose its slots to a migration run.
 */
bool modSave_canMigrate(const SaveMigrateInfo *info);

/**
 * Run the confirmed migration: convert the classified image to the current format and
 * reload the save state. The caller has already confirmed; this writes the save chip.
 */
void modSave_migrate(const SaveMigrateInfo *info);

/**
 * Start a fresh save: the current-format header, every slot reset, state reloaded. The
 * confirmed-Yes path for EMPTY / UNRECOGNIZED, and the ROM's only format writer.
 */
void modSave_format(void);

// The confirmation scene (a ModSceneHandlers row, see hacks/mod_scenes.h).
void modSave_migrateSceneEnter(void);
void modSave_migrateSceneTick(void);
void modSave_migrateSceneExit(void);
