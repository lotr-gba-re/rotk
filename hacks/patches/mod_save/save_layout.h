// Save-chip budget every save-consuming patch shares.
// Only meaningful with the SRAM backend (block i maps to byte i*8); the regions below
// partition its 4096 blocks. Every base is derived from the previous region's end, so
// resizing = turning one knob here and all consumers rebase.
#pragma once

#if !HACK_mod_save
#error "requires the mod_save patch: consumers span past the stock 4-Kbit chip"
#endif

#include "gba_io.h"     // SRAM_SIZE: the chip the regions below partition
#include "save.h"       // SaveGlobals / SaveSlot / EEPROM_BLOCK_SIZE
#include "scene/shop.h" // HOLLOW_SLOT_COUNT

/**
 * The SRAM backend maps block i to byte i*8. The region table is block-denominated; byte
 * offsets (HOLLOW_REGION_END, region 4) convert through this.
 */
#define SAVE_BLOCKS_TO_BYTES(blocks) ((blocks) * EEPROM_BLOCK_SIZE)

/** Region 1: the SaveGlobals header (blocks 0..7, as in the base game). */
#define SAVE_GLOBALS_BLOCK_COUNT (sizeof(SaveGlobals) / EEPROM_BLOCK_SIZE)

// Region 2: paged save slots (save_slot_pages). 1 page = 4 on-screen slots;
// 18 pages = 72 slots, the SavSlotPages layout, so a SavSlotPages migration keeps every
// slot in place. The page count is the knob.
#define SAVE_SLOTS_PER_PAGE ((u32)SAVE_SLOT_COUNT)
#define SAVE_SLOT_PAGE_COUNT 18u
#define SAVE_SLOT_BLOCK_COUNT (sizeof(SaveSlot) / EEPROM_BLOCK_SIZE)
#define SAVE_SLOT_REGION_END                                                                       \
    (SAVE_GLOBALS_BLOCK_COUNT + SAVE_SLOT_PAGE_COUNT * SAVE_SLOTS_PER_PAGE * SAVE_SLOT_BLOCK_COUNT)

// Region 3: ranger hollow tabs (ranger_hollow_pages). One tab = the 9 stash items
#define HOLLOW_PAGE_SIZE (HOLLOW_SLOT_COUNT * sizeof(Item))
#define HOLLOW_PAGE_BLOCK_BASE SAVE_SLOT_REGION_END // save_migrate's blank boundary
#define HOLLOW_PAGE_BYTE_BASE SAVE_BLOCKS_TO_BYTES(HOLLOW_PAGE_BLOCK_BASE)
#define HOLLOW_PAGE_COUNT 99u
/** The hollow region's end as a byte offset: region 4's base. */
#define HOLLOW_REGION_END (HOLLOW_PAGE_BYTE_BASE + HOLLOW_PAGE_COUNT * HOLLOW_PAGE_SIZE)

_Static_assert(HOLLOW_REGION_END <= SRAM_SIZE, "save regions overrun the save chip");

// Region 4 (last): global mod options, 128 B, byte-addressed. Field order in
// GlobalModOptionsArea IS the save format: append new tenants before reserved[], never
// reorder, never #if a field out (a compiled-out patch keeps its field, so offsets never
// shift between builds). Raw u8/u8[N] fields only: typed fields would pull patch headers
// in here, and a compiled-out patch's type vanishes while its grant must persist. A
// tenant overlaying u16/u32 members asks for an aligned grant AT GRANT TIME (alignment
// of an existing grant never changes). The migration blank fill is SRAM_BLANK (0xFF), so
// every tenant's tag encoding must read an all-0xFF field as uninitialized.
#define GLOBAL_MOD_OPTIONS_OFF HOLLOW_REGION_END
#define GLOBAL_MOD_OPTIONS_SIZE 128u

/**
 * The global mod options area's offset map. A type only, never instantiated: tenants
 * keep their state in their own globals and reach the chip through
 * GLOBAL_MOD_OPTIONS_READ/WRITE (save_sram.h).
 */
union GlobalModOptionsArea {
    u8 bytes[GLOBAL_MOD_OPTIONS_SIZE];
    struct
    {
        u8 lootQolState; // loot_qol: struct LootQolState overlays this byte
        u8 reserved[GLOBAL_MOD_OPTIONS_SIZE - 1];
    } d;
};

_Static_assert(sizeof(union GlobalModOptionsArea) == GLOBAL_MOD_OPTIONS_SIZE,
               "global mod options area overrun");
_Static_assert(GLOBAL_MOD_OPTIONS_OFF + GLOBAL_MOD_OPTIONS_SIZE <= SRAM_SIZE,
               "save regions overrun the save chip");

// Save-format header magic. The base game and SavSlotPages both carry "ROTKGBA6"; the
// current header is built entirely in C (modSave_stampHeaderMirror), so bumping the
// version here carries every writer and validator with it.
//
// Version table:
//   0x00  "ROTKRCM" + 0x00  this layout (72 slots + 99 hollow pages).
#define SAVE_HEADER_MAGIC "ROTKRCM"
#define SAVE_HEADER_MAGIC_LEN (sizeof(SAVE_HEADER_MAGIC) - 1)
#define SAVE_HEADER_VERSION 0x00u
#define SAVE_HEADER_MAGIC_VANILLA "ROTKGBA6"
