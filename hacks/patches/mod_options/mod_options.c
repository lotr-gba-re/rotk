// Mod options: an extensible settings page inside the options menu (scene 0x06).
//
// The stock menu is entirely ROM code; conditional-skip hooks (mod_options.asm) divert only
// what the mod page needs. On the stock page a "Mod Options" pseudo-row (cursor index
// MOD_OPTIONS_ROW) is rendered beside Credits; pressing down past Credits reaches it and
// confirming switches the menu to the mod settings (B switches back).
//
// Mod settings are PAGES switched with L/R (like the ranger hollow pages), one or more per
// patch; the header shows the page title. Pages come from the ModOptionsPages registry
// below; each patch supplies its entries array (see mod_options.h for the contract).
#include "patches/mod_options/mod_options.h"
#include "combat.h"
#include "font.h"
#include "gba_io.h"
#include "gfx.h"
#include "input.h"
#include "menu.h"
#include "scene/optionsMenu.h"
#include "sfx.h"
#include "text.h"
#include "variables.h"

#if HACK_loot_qol
#include "patches/loot_qol/loot_qol.h"
#endif
#if HACK_demo_pages
#include "patches/demo_pages/demo_pages.h"
#endif

/** Stock-page cursor index of the "Mod Options" entry (after Credits). */
#define MOD_OPTIONS_ROW 8

// Page modes (g_ModOptionsMode values); anything else reads as stock.
#define MOD_OPTIONS_MODE_STOCK 0
#define MOD_OPTIONS_MODE_MOD 1

static u8 g_ModOptionsMode;
static u8 g_ModOptionsPage;

// Glyph-tile base of the "Mod Options" label: past the rows' tile budget (8 rows x 0x19
// from 0xf6), so drawing it never collides with row glyphs regardless of draw order. The
// page counter sits 0x10 tiles past it, top-left of the header band (the title's row).
#define MOD_OPTIONS_LABEL_TILE 0x1d8
#define MOD_OPTIONS_COUNTER_TILE (MOD_OPTIONS_LABEL_TILE + 0x10)
#define MOD_OPTIONS_COUNTER_X 0x04
#define MOD_OPTIONS_COUNTER_Y 0xc

/**
 * The registered pages, one entry per page per enabled consumer patch (a patch may own
 * several, like demo_pages). The compiler infers the size; with no consumer enabled the
 * array is empty (a GNU extension the modcode's gnu11 build accepts) and the count-0
 * runtime guards below compile out at -O2.
 */
static const struct ModOptionsPage ModOptionsPages[] = {
#if HACK_loot_qol
    {"Loot QoL", LootQolOptions, LOOT_QOL_OPTION_COUNT, lootQol_drawPageText},
#endif
#if HACK_demo_pages
    {"Demo Page A", DemoPagesFirstOptions, DEMO_PAGES_FIRST_OPTION_COUNT, NULL},
    {"Demo Page B", DemoPagesSecondOptions, DEMO_PAGES_SECOND_OPTION_COUNT, NULL},
#endif
};

#define MOD_OPTIONS_PAGE_COUNT (s32)(sizeof(ModOptionsPages) / sizeof(ModOptionsPages[0]))

// Stock row geometry (scene_optionsMenu_drawRow): 16 px per row from y=0x20, label at
// x=0x40, value at x=0x98, 0x19 glyph tiles per row from 0xf6.
#define MOD_OPTIONS_ROW_TILE_BASE(row) ((row) * 0x19 + 0xf6)
#define MOD_OPTIONS_ROW_Y(row) ((row) * 0x10 + 0x20)

static u32 getMode(void)
{
    if (g_ModOptionsMode == MOD_OPTIONS_MODE_MOD)
    {
        return MOD_OPTIONS_MODE_MOD;
    }
    return MOD_OPTIONS_MODE_STOCK;
}

static const struct ModOptionsPage *currentPage(void)
{
    const struct ModOptionsPage *page = &ModOptionsPages[g_ModOptionsPage];

    // a stale out-of-range index (fewer pages than when the byte was written) reads as
    // the first page
    if (page >= ModOptionsPages + MOD_OPTIONS_PAGE_COUNT)
    {
        page = ModOptionsPages;
    }
    return page;
}

static u32 getPage(void)
{
    return currentPage() - ModOptionsPages;
}

/**
 * Blank a row's BG cells (label and value columns, the full 16px band so taller fonts
 * clear too); the stock DrawRow only ever clears past the value text, which leaves stale
 * glyphs once labels change between pages.
 */
static void clearRow(s32 row)
{
    gfx_fillBgRect(3, MOD_OPTIONS_LABEL_X / 8, MOD_OPTIONS_ROW_Y(row) / 8,
                   0x1e - MOD_OPTIONS_LABEL_X / 8, 2, 0);
}

static s32 drawText(s32 tileBase, s32 x, s32 y, char *str, s32 color)
{
    font_setStyle(FONT_STYLE_8PX_B, color, 0);
    return font_drawString(tileBase, x, y, str, 0);
}

void modOptions_drawRowText(s32 row, s32 x, const char *str, FontStyle style, s32 color)
{
    font_setStyle(style, color, 0);
    font_drawString(MOD_OPTIONS_ROW_TILE_BASE(row), x, MOD_OPTIONS_ROW_Y(row), (char *)str, 0);
}

/**
 * Redraw the header title (cleared first: the stock Enter draws it once and never clears,
 * and page titles differ in length). Same tile base, position, and style as the stock
 * "Options" title.
 */
static void drawTitle(char *title)
{
    gfx_fillBgRect(3, 4, 1, 0x1e - 4, 2, 0);
    font_setStyle(FONT_STYLE_11PX_A, 3, -1);
    font_drawString(0xe2, 0x70, 0xc, title, 1);
}

static void drawModTitle(void)
{
    if (MOD_OPTIONS_PAGE_COUNT == 0)
    {
        // no pages registered: a dummy page with no rows
        drawTitle("No mod options available");
        return;
    }
    drawTitle((char *)currentPage()->title);
    if (MOD_OPTIONS_PAGE_COUNT > 1)
    {
        // the "Page x/y" counter (pages wrap on L/R); save-slot-pages style
        font_setStyle(FONT_STYLE_11PX_B, 3, 0);
        font_formatIntSlot0((s32)getPage() + 1);
        font_formatIntSlot1(MOD_OPTIONS_PAGE_COUNT);
        font_rasterizeString(MOD_OPTIONS_COUNTER_TILE, MOD_OPTIONS_COUNTER_X, MOD_OPTIONS_COUNTER_Y,
                             "Page @1/@2");
    }
}

/**
 * Redraw every visible row of the current page (through the hooked scene_optionsMenu_drawRow,
 * so each row lands in the right mode), then run the page's extra-drawing callback.
 */
static void drawPage(void)
{
    s32 row;

    for (row = 0; row < OPTIONS_ROW_COUNT; row++)
    {
        clearRow(row);
        scene_optionsMenu_drawRow(row, SCENE_ARG_OPTIONS_CURSOR);
    }
    if (getMode() == MOD_OPTIONS_MODE_MOD && MOD_OPTIONS_PAGE_COUNT > 0 &&
        currentPage()->draw != NULL)
    {
        currentPage()->draw();
    }
}

/**
 * First selectable (non-spacer) row of the current page (0 when it has none). Entering a
 * page puts the cursor here: a cursor resting on a spacer highlights nothing, and moving
 * off it redraws the spacer blank, erasing any callback text drawn on it.
 */
static s32 firstSelectableRow(void)
{
    const struct ModOptionsPage *page = currentPage();
    s32 row;

    for (row = 0; row < page->entryCount; row++)
    {
        if (page->entries[row].label != NULL)
        {
            return row;
        }
    }
    return 0;
}

static void enterModPage(void)
{
    g_ModOptionsMode = MOD_OPTIONS_MODE_MOD;
    SCENE_ARG_OPTIONS_CURSOR = MOD_OPTIONS_PAGE_COUNT > 0 ? firstSelectableRow() : 0;
    drawModTitle();
    drawPage();
}

static void leaveModPage(void)
{
    g_ModOptionsMode = MOD_OPTIONS_MODE_STOCK;
    SCENE_ARG_OPTIONS_CURSOR = MOD_OPTIONS_ROW;
    drawTitle(text_getString(TEXT_ID_OPTIONS_TITLE));
    drawPage();
}

/** The cursor-move sfx every successful move/adjust ends with. */
static void playCursorSfx(void)
{
    sfx_stop(g_OptionsSfxHandle);
    g_OptionsSfxHandle = sfx_play(SFX_MENU_CURSOR_MOVE);
}

/**
 * A row of the mod page (label + current value); the caller clears the row first.
 * Spacer rows stay blank.
 */
static void drawModRow(s32 row, s32 cursor)
{
    const struct ModOptionEntry *entry = &currentPage()->entries[row];
    s32 color = row == cursor ? 0 : 3;
    s32 tileBase;

    if (entry->label == NULL)
    {
        return;
    }
    tileBase = drawText(MOD_OPTIONS_ROW_TILE_BASE(row), MOD_OPTIONS_LABEL_X + entry->labelIndent,
                        MOD_OPTIONS_ROW_Y(row), (char *)entry->label, color);
    drawText(tileBase, MOD_OPTIONS_VALUE_X, MOD_OPTIONS_ROW_Y(row),
             (char *)entry->valueNames[entry->get()], color);
}

/**
 * scene_optionsMenu_tick hook: on the mod page, intercept L/R (switch page) and B/Select
 * (back to the stock page instead of leaving the scene). Everything else - the whole key
 * dispatch included - is the original's; its calls into the other scene functions pass
 * through these hooks.
 */
u32 modOptions_tickHook(void)
{
    u32 keys = ACTIVE_MENU_KEYS_PRESSED;

    if (getMode() != MOD_OPTIONS_MODE_MOD)
    {
        return FALSE;
    }
    if (MOD_OPTIONS_PAGE_COUNT > 1 && (keys & (L_BUTTON | R_BUTTON)))
    {
        s32 page = getPage();

        if (keys & R_BUTTON)
        {
            page++;
            if (page >= MOD_OPTIONS_PAGE_COUNT)
            {
                page = 0;
            }
        }
        else
        {
            page--;
            if (page < 0)
            {
                page = MOD_OPTIONS_PAGE_COUNT - 1;
            }
        }
        g_ModOptionsPage = page;
        SCENE_ARG_OPTIONS_CURSOR = firstSelectableRow();
        drawModTitle();
        drawPage();
        playCursorSfx();
        return TRUE;
    }
    if (keys & (B_BUTTON | SELECT_BUTTON))
    {
        leaveModPage();
        sfx_play(SFX_MENU_CURSOR_MOVE);
        player_applyOptionFlags(ACTIVE_PLAYER_INDEX);
        return TRUE;
    }
    return FALSE;
}

/**
 * scene_optionsMenu_drawRow hook: on the mod page, draw the current page's row (blank rows
 * past it). On the stock page, only add the "Mod Options" label beside Credits (own tile
 * base, so the original's Credits draw can't clobber it); the row itself is the
 * original's.
 */
u32 modOptions_drawRowHook(s32 row, s32 cursor)
{
    if (getMode() == MOD_OPTIONS_MODE_MOD)
    {
        clearRow(row);
        if (MOD_OPTIONS_PAGE_COUNT > 0 && row < currentPage()->entryCount)
        {
            drawModRow(row, cursor);
        }
        return TRUE;
    }
    if (row == OPTIONS_ROW_CREDITS)
    {
        drawText(MOD_OPTIONS_LABEL_TILE, MOD_OPTIONS_VALUE_X, MOD_OPTIONS_ROW_Y(row), "Mod Options",
                 cursor == MOD_OPTIONS_ROW ? 0 : 3);
    }
    return FALSE;
}

/**
 * scene_optionsMenu_moveCursor hook: on the mod page, move within the current page's rows.
 * On the stock page, intercept only the moves involving the Mod Options pseudo-row
 * (Credits <-> Mod Options <-> Music Volume; it shares the Credits row's rendering, so
 * moving to or from it redraws that row); every other move, difficulty skip included, is
 * the original's.
 */
u32 modOptions_moveCursorHook(void)
{
    u32 cursor = SCENE_ARG_OPTIONS_CURSOR;
    u32 keys = ACTIVE_MENU_KEYS_PRESSED;
    s32 next = -1;

    if (getMode() == MOD_OPTIONS_MODE_MOD)
    {
        if (MOD_OPTIONS_PAGE_COUNT > 0)
        {
            const struct ModOptionsPage *page = currentPage();
            s32 steps;

            // spacer rows are not selectable: keep stepping in the direction of travel
            for (steps = 0; steps < page->entryCount; steps++)
            {
                menu_moveCursorVertical(&SCENE_ARG_OPTIONS_CURSOR, 0, page->entryCount - 1, TRUE,
                                        ACTIVE_PLAYER_INDEX);
                if (page->entries[SCENE_ARG_OPTIONS_CURSOR].label != NULL)
                {
                    break;
                }
            }
            scene_optionsMenu_drawRow(cursor, SCENE_ARG_OPTIONS_CURSOR);
            scene_optionsMenu_drawRow(SCENE_ARG_OPTIONS_CURSOR, SCENE_ARG_OPTIONS_CURSOR);
            playCursorSfx();
        }
        return TRUE;
    }

    if (keys & DPAD_DOWN)
    {
        if (cursor == OPTIONS_ROW_CREDITS)
        {
            next = MOD_OPTIONS_ROW;
        }
        else if (cursor == MOD_OPTIONS_ROW)
        {
            next = OPTIONS_ROW_MUSIC_VOLUME;
        }
    }
    else if (keys & DPAD_UP)
    {
        if (cursor == MOD_OPTIONS_ROW)
        {
            next = OPTIONS_ROW_CREDITS;
        }
        else if (cursor == OPTIONS_ROW_MUSIC_VOLUME)
        {
            next = MOD_OPTIONS_ROW;
        }
    }
    if (next < 0)
    {
        return FALSE;
    }

    SCENE_ARG_OPTIONS_CURSOR = next;
    scene_optionsMenu_drawRow(cursor == MOD_OPTIONS_ROW ? OPTIONS_ROW_CREDITS : cursor, next);
    scene_optionsMenu_drawRow(next == MOD_OPTIONS_ROW ? OPTIONS_ROW_CREDITS : next, next);
    playCursorSfx();
    return TRUE;
}

/**
 * scene_optionsMenu_adjustValue hook: on the mod page, adjust and commit the row. Stock
 * rows - volumes, toggles, the difficulty selector, and the action/pseudo rows' no-op -
 * are the original's.
 */
u32 modOptions_adjustValueHook(void)
{
    if (getMode() != MOD_OPTIONS_MODE_MOD)
    {
        return FALSE;
    }
    if (MOD_OPTIONS_PAGE_COUNT > 0 && SCENE_ARG_OPTIONS_CURSOR < currentPage()->entryCount &&
        currentPage()->entries[SCENE_ARG_OPTIONS_CURSOR].label != NULL)
    {
        const struct ModOptionEntry *entry = &currentPage()->entries[SCENE_ARG_OPTIONS_CURSOR];
        u32 val = entry->get();

        menu_moveCursorHorizontal(&val, 0, entry->valueCount - 1, TRUE, ACTIVE_PLAYER_INDEX);
        entry->set(val);
        scene_optionsMenu_drawRow(SCENE_ARG_OPTIONS_CURSOR, SCENE_ARG_OPTIONS_CURSOR);
        playCursorSfx();
    }
    return TRUE;
}

/**
 * scene_optionsMenu_onConfirm hook: the Mod Options pseudo-row enters the mod page; on the
 * mod page A is a silent no-op (rows adjust with left/right). Language Select and Credits
 * are the original's.
 */
u32 modOptions_onConfirmHook(void)
{
    if (getMode() == MOD_OPTIONS_MODE_MOD)
    {
        return TRUE;
    }
    if (SCENE_ARG_OPTIONS_CURSOR == MOD_OPTIONS_ROW)
    {
        enterModPage();
        sfx_stop(g_OptionsSfxHandle);
        g_OptionsSfxHandle = sfx_play(SFX_MENU_CONFIRM);
        return TRUE;
    }
    return FALSE;
}
