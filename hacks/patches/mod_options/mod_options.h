#pragma once
// Shared mod_options types, for patches registering settings pages.
//
// Registering a patch's page (see loot_qol):
//   - in the patch: a `const struct ModOptionEntry <Name>_Options[]` (ROM-resident:
//     strings + get/set function pointers, no .data/.bss), guarded by #if HACK_mod_options,
//     and a <NAME>_OPTION_COUNT define + extern declaration in the patch's header;
//   - in mod_options.c: one ModOptionsPages entry per page under #if HACK_<name> (a patch
//     may register several pages, like demo_pages); the page count is inferred from the
//     array.

#include "font.h"
#include "libc.h"
#include "types.h"

/**
 * One mod-settings row: label in the left column (shifted right labelIndent px, to nest
 * rows under a section heading), valueNames[get()] in the right one; left/right adjusts
 * (wrapping 0..valueCount-1) and commits via set.
 * MOD_OPTIONS_SPACER is a blank, non-selectable row (the cursor skips it).
 */
#define MOD_OPTIONS_SPACER {NULL, NULL, 0, 0, NULL, NULL}

struct ModOptionEntry
{
    const char *label;
    const char *const *valueNames;
    u8 valueCount;
    u8 labelIndent;
    u8 (*get)(void);
    void (*set)(u8 value);
};

/**
 * One mod-settings page (L/R switches pages): title drawn in the header, rows below.
 * draw is an optional extra-drawing callback (section headers, explainer text), run at
 * the end of every full page draw; NULL for none. Single-row redraws (cursor moves,
 * value adjusts) don't re-run it, so draw on spacer rows only - those never redraw
 * alone.
 */
struct ModOptionsPage
{
    const char *title;
    const struct ModOptionEntry *entries;
    u8 entryCount;
    void (*draw)(void);
};

// Row geometry for page draw callbacks (a row's glyph-tile budget is shared with its
// entry's text, hence the spacer-rows-only rule above).
#define MOD_OPTIONS_LABEL_X 0x40
#define MOD_OPTIONS_VALUE_X 0x98

/**
 * Draw str on a page row at column x (MOD_OPTIONS_LABEL_X / MOD_OPTIONS_VALUE_X) with the
 * given font style and color; for page draw callbacks. Row colors: 3 = non-highlight, 0 =
 * the cursor-row color (flashes; only the cursor row should use it), 6 = the greyed-out
 * one. FONT_STYLE_8PX_B is the rows' font, FONT_STYLE_8PX_A the lighter face the stock
 * game uses for secondary prompts (same 8px height; no smaller font exists).
 */
void modOptions_drawRowText(s32 row, s32 x, const char *str, FontStyle style, s32 color);
