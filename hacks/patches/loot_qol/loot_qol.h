#pragma once

// Shared loot_qol definitions: the state byte and the mod_options page hosting the loot
// filter (loot_filter.h) and auto loot (auto_loot.h) rows.

#include "types.h"

/**
 * State byte layout: bits 0-2 the "Require Affix" mode (LOOT_FILTER_AFFIX_*, loot_filter.h),
 * bit 3 "Suppress Arrows" (1 = arrows are voided), bit 4 "Auto-Collect Gems", bit 7 the
 * tag: 0 once written, 1 on a blank byte (the 0xFF migration fill, SRAM_BLANK), so an
 * unwritten byte reads as the defaults. 1 bit carries no layout versioning - blank vs
 * written is all it says - and garbage with bit 7 clear misreads as valid (benign: the
 * worst case is wrong prefs). gcc on little-endian ARM allocates bitfields LSB-first, so
 * this overlays the byte deterministically.
 */
struct LootQolState
{
    u8 affix : 3;
    u8 arrows : 1;
    u8 autoLootGems : 1;
    u8 : 2; // spare
    u8 tag : 1;
};

#define LOOT_QOL_TAG 0 // written bytes read tag 0; the 0xFF blank fill reads tag 1

/**
 * The state byte, an absolute symbol over the LOOT_QOL_STATE RAM byte (layout.cfg).
 * Persisted to the global mod options save area: loaded at boot by lootQol_loadState,
 * written through by the option setters.
 */
extern struct LootQolState g_LootQolState;

/**
 * The effective state; an untagged byte (blank/unwritten save area) is initialized to the
 * defaults on first read.
 */
struct LootQolState lootQol_getState(void);

/**
 * Boot task (runTasks): load the persisted state byte from the global mod
 * options save area.
 */
void lootQol_loadState(void);

// The patch's mod_options page entries (compiled when HACK_mod_options).
#include "patches/mod_options/mod_options.h"

#define LOOT_QOL_OPTION_COUNT 6
extern const struct ModOptionEntry LootQolOptions[LOOT_QOL_OPTION_COUNT];

/**
 * The page's extra-drawing callback (ModOptionsPage.draw): the "Loot Filter:" heading and
 * the uniques note, on the spacer rows of LootQolOptions.
 */
void lootQol_drawPageText(void);
