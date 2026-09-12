// The save backend: how to reach the chip, and whether it is there at all. Moves bytes and
// never interprets them; what the bytes mean is save_format.h's business.
#pragma once

#include "types.h"

#include "libc.h"        // offsetof
#include "save_layout.h" // SRAM_BASE / SRAM_SIZE / union GlobalModOptionsArea

#if !HACK_mod_save
#error "requires the mod_save patch for the SRAM backend"
#endif

/**
 * The one safe fill byte. An all-0x00 record passes save_checksum16 (the sum of zeroes is
 * zero), and save_getSlotMission then reports mission 0 rather than SLOT_MISSION_EMPTY, so a
 * zeroed slot region reads back as a screen full of real saves. 0xFF fails every checksum
 * (0xFFC8 over a slot, 0xFFE0 over the header) and reads as empty where no checksum stands
 * guard (hollow pages: nine EMPTY_ITEM slots; mod options: untagged), which is exactly how
 * an untouched region should read. Never memset the chip to 0.
 */
#define SRAM_BLANK 0xFFu

/**
 * Byte-copy src -> dst (either side may be SRAM). A plain pointer dereference does not
 * work: the databus is 8-bit and reads must execute from WRAM (GBATEK). The waitstate is
 * set for the whole session by save_sram.asm's rotk_main literal patch.
 */
void modSave_copy(u8 *dst, const u8 *src, u32 numBytes);

/**
 * Fill numBytes of the chip from byteOffset with `value`, normally SRAM_BLANK. A blanked
 * region reads as empty, which is how an area a save format does not define is retired.
 */
void modSave_fill(u32 byteOffset, u32 numBytes, u8 value);

// Byte-exact save access for one global mod options grant (save_layout.h). Both macros
// take a field designator, not an offset, so a patch cannot express access outside its
// area without naming another tenant's field. The size is the full area, so dst/src
// must be area-sized
#define GLOBAL_MOD_OPTIONS_READ(field, dst)                                                        \
    modSave_copy((u8 *)(dst),                                                                      \
                 SRAM_BASE + GLOBAL_MOD_OPTIONS_OFF +                                              \
                     offsetof(union GlobalModOptionsArea, d.field),                                \
                 sizeof(((union GlobalModOptionsArea *)0)->d.field))
#define GLOBAL_MOD_OPTIONS_WRITE(field, src)                                                       \
    modSave_copy(SRAM_BASE + GLOBAL_MOD_OPTIONS_OFF +                                              \
                     offsetof(union GlobalModOptionsArea, d.field),                                \
                 (const u8 *)(src), sizeof(((union GlobalModOptionsArea *)0)->d.field))

/**
 * TRUE when a writable save chip is there, FALSE when the cart or emulator gave us no
 * SRAM (wrong save type configured, or a cart without one). Nothing can save at all in
 * that case.
 */
bool modSave_isChipPresent(void);
