// Grow the Ranger Hollow stash from 1 page of 9 cells to HOLLOW_PAGE_COUNT pages.
//
// Player.rangerHollowItems always holds the CURRENT page's 9 items, so every base-game
// accessor (store/take/init/serialize/deserialize) works unchanged. Each page's home is its own
// slice of the SRAM page region (see the mod_save patch), written
// through at swap/exit time. L/R in the Ranger Hollow menu swaps pages (START held jumps 10 at
// once): write through to the old page, load the new one, redraw the 3x3 grid.
//
// Pre-migration saves keep the whole stash in the save-header mirror;
// rangerHollowPages_migrateStash (called from mod_save's migration run) copies it into page
// 1's SRAM home. The mirror stays a cold backup: nothing reads it, but the exit hook parks
// page 1 in the item slots so the stock header serialize keeps it tracking page 1.
//
// Trade-off: pages write through at swap/exit time, so storing or taking an item and powering
// off without saving can duplicate or DESTROY it (the page copy persists, the backpack change
// doesn't) - same class of caveat as save_slot_pages.
//
// TODO: MULTIPLAYER IS ENTIRELY UNTESTED - in co-op all three hooks no-op (the link protocol
// never syncs page views, so paged stashes would diverge) and the stash behaves vanilla, but
// even that is unverified.

#include "combat.h"
#include "enemy.h"
#include "font.h"
#include "game.h"
#include "gba_io.h"
#include "input.h"
#include "item.h"
#include "menu.h"
#include "patches/mod_save/save_layout.h"
#include "patches/mod_save/save_sram.h"
#include "save.h"
#include "scene.h"
#include "scene/shop.h"
#include "sfx.h"
#include "sprite.h"
#include "variables.h"

/** The current page. Kept while the hollow is closed for convenience when re-opening. */
static u8 g_RangerHollowPage;

#define HOLLOW_PAGE_LABEL_X 10
#define HOLLOW_INDICATOR_X 40
#define HOLLOW_INDICATOR_Y 140

// Charbase tiles reserved for the indicator: label and count get 5 cells x 2 rows each,
// contiguous from HOLLOW_INDICATOR_TILE (the count's tiles follow the label's).
#define HOLLOW_INDICATOR_TILE 0x1A2u
#define HOLLOW_INDICATOR_LABEL_TILES 10
#define HOLLOW_INDICATOR_TILES 20

/** The current page index (RAM-only, never saved; an out-of-range value clamps to page 1). */
static s32 currentPage(void)
{
    u32 page = g_RangerHollowPage;
    if (page >= HOLLOW_PAGE_COUNT)
    {
        page = 0;
        g_RangerHollowPage = 0;
    }
    return page;
}

/** Write the item slots through to the page's SRAM home. */
static void writePage(u32 page)
{
    modSave_copy(SRAM_BASE + HOLLOW_PAGE_BYTE_BASE + page * HOLLOW_PAGE_SIZE,
                 (const u8 *)ACTIVE_PLAYER.rangerHollowItems, HOLLOW_PAGE_SIZE);
}

/**
 * Load a page's SRAM home into the item slots. A blank (all-0xFF) page is nine EMPTY_ITEM
 * slots, so there is no validity branch.
 */
static void readPage(u32 page)
{
    modSave_copy((u8 *)ACTIVE_PLAYER.rangerHollowItems,
                 SRAM_BASE + HOLLOW_PAGE_BYTE_BASE + page * HOLLOW_PAGE_SIZE, HOLLOW_PAGE_SIZE);
}

/**
 * Copy the save-header mirror's stash (a pre-migration save's whole stash) into page 1's SRAM
 * home. Called once from mod_save's migration run, after the hollow region is blanked; declared
 * in ranger_hollow_pages.h.
 */
void rangerHollowPages_migrateStash(void)
{
    _Static_assert(sizeof(g_SaveGlobals.d.rangerHollowItems) == HOLLOW_PAGE_SIZE,
                   "a hollow page is exactly the header mirror's stash array");
    modSave_copy(SRAM_BASE + HOLLOW_PAGE_BYTE_BASE, (const u8 *)g_SaveGlobals.d.rangerHollowItems,
                 HOLLOW_PAGE_SIZE);
}

/**
 * Zero the indicator's charbase tile patterns: the rasterizer only rewrites the tiles
 * its current string spans (and sets only non-zero glyph pixels), so stale pixels from a
 * longer count ("46/68" -> "9/68") would ghost through otherwise.
 */
static void clearIndicatorTiles(void)
{
    volatile u32 *tile = (volatile u32 *)(g_FontRenderCtx.charbaseAddr +
                                          g_FontRenderCtx.tileStride * HOLLOW_INDICATOR_TILE);
    u32 totalWords = HOLLOW_INDICATOR_TILES * (g_FontRenderCtx.tileStride / 4);
    for (u32 i = 0; i < totalWords; i++)
    {
        tile[i] = 0;
    }
}

/**
 * HACK-ONLY: draw the "Page" label and the left-aligned "current/total" count (e.g.
 * "3/68") below the gems. Called every frame: with an unchanged count it rewrites identical
 * tile/screenbase bytes, which is visually stable (no per-frame clear - that is what caused
 * flicker).
 */
static void drawIndicator(void)
{
    font_setStyle(FONT_STYLE_11PX_B, 3, 0);
    font_formatIntSlot0(currentPage() + 1);
    font_formatIntSlot1((s32)HOLLOW_PAGE_COUNT);
    font_drawString(HOLLOW_INDICATOR_TILE, HOLLOW_PAGE_LABEL_X, HOLLOW_INDICATOR_Y, "Page", 0);
    font_drawString(HOLLOW_INDICATOR_TILE + HOLLOW_INDICATOR_LABEL_TILES, HOLLOW_INDICATOR_X,
                    HOLLOW_INDICATOR_Y, "@1/@2", 0);
}

/**
 * Swap the item slots to page cur+delta: write through, load, redraw the grid + info panel,
 * and re-decide the cursor's equippability overlay. A page swap replaces the item under a
 * stationary cursor, which vanilla never does - scene_shop_moveCursor only re-decides the
 * overlay on cursor movement, so without this it keeps the old page's variant.
 */
static void swapPage(s32 delta)
{
    s32 cur = currentPage();

    // wrap around the ends: L on page 1 -> last page, R on last page -> page 1
    s32 next = cur + delta;
    if (next < 0)
    {
        next += (s32)HOLLOW_PAGE_COUNT;
    }
    else if (next >= (s32)HOLLOW_PAGE_COUNT)
    {
        next -= (s32)HOLLOW_PAGE_COUNT;
    }

    Item *slots = ACTIVE_PLAYER.rangerHollowItems;

    // the slots still hold the old page: this is the variant the overlay currently shows
    u8 cursor = g_ShopCursor->actionState;
    bool cursorOnStash = cursor >= SHOP_CURSOR_HOLLOW_FIRST && cursor <= SHOP_CURSOR_HOLLOW_LAST;
    u8 oldAnimation = 0;
    if (cursorOnStash)
    {
        oldAnimation = scene_shop_itemCursorAnimation(slots[cursor - SHOP_CURSOR_HOLLOW_FIRST],
                                                      ACTIVE_PLAYER_INDEX);
    }

    writePage(cur);
    readPage(next);
    g_RangerHollowPage = next;

    for (u32 i = 0; i < HOLLOW_SLOT_COUNT; i++)
    {
        if (slots[i].d.itemType == ITEM_TYPE_EMPTY)
        {
            scene_shop_clearHollowSlotItem((u8)i);
        }
        else
        {
            scene_shop_drawHollowSlotItem(slots[i], (u8)i);
        }
    }
    // only swap the descriptor when the variant changed, so an unchanged overlay isn't
    // restarted on frame 0 (same old/new compare as scene_shop_moveCursor)
    if (cursorOnStash)
    {
        u8 animation = scene_shop_itemCursorAnimation(slots[cursor - SHOP_CURSOR_HOLLOW_FIRST],
                                                      ACTIVE_PLAYER_INDEX);
        if (animation != oldAnimation)
        {
            sprite_setAnimation(g_ShopCursor, &ShopCursorAnimations[animation]);
        }
    }
    scene_shop_drawInfoPanel(g_ShopCursor->actionState, ACTIVE_PLAYER_INDEX);

    // the count changed width: zero the indicator tiles (stale screenbase entries then
    // point at blank tiles) and redraw now, not next frame
    clearIndicatorTiles();
    drawIndicator();
    sfx_play(SFX_MENU_CURSOR_MOVE);
}

/**
 * Per-frame hook body (scene_shop_tick entry): draw the page indicator and flip pages
 * on L/R. No-ops on other shops, while a confirm dialog is open, or in co-op (see the
 * TODO in the file header).
 */
void rangerHollowPages_onShopInput(void)
{
    if (g_ShopType != SHOP_TYPE_RANGER_HOLLOW)
    {
        return;
    }

    if (SCENE_ARG_SHOP_STATE != SHOP_STATE_NONE)
    {
        return;
    }

    if (g_GameFlags.d.coopActive)
    {
        return;
    }

    drawIndicator();

    u16 keys = ACTIVE_MENU_KEYS_PRESSED;
    s32 delta = 0;
    if (keys & L_BUTTON)
    {
        delta = -1;
    }
    else if (keys & R_BUTTON)
    {
        delta = 1;
    }

    // Directly use REG_KEYINPUT because g_MenuKeysPressed debounces
    if (~REG_KEYINPUT & START_BUTTON)
    {
        delta *= 10; // Skip 10 pages at once
    }

    if (delta != 0)
    {
        swapPage(delta);
    }
}

/**
 * Shop-enter hook body (scene_shop_enter entry): reload the current page's home into the
 * item slots - on a previous exit the slots were returned to page 1 while the page index was
 * kept, so this restores the page the player left on (the vanilla enter then draws it).
 * Checks SCENE_ARG_SHOP_TYPE (the incoming shop type; g_ShopType is still the PREVIOUS
 * shop's at this point).
 */
void rangerHollowPages_onShopEnter(void)
{
    if (SCENE_ARG_SHOP_TYPE != SHOP_TYPE_RANGER_HOLLOW)
    {
        return;
    }

    if (g_GameFlags.d.coopActive)
    {
        return;
    }

    // stock L/R hints at the top corners (overlay-list actors, ticked/freed with the
    // shop scene); see the cluster-layout TODO above
    menu_createLrButtonIndicators();

    readPage(currentPage());
}

/**
 * Shop-exit hook body (scene_shop_onBack entry): write the current page through to its
 * SRAM home, then park page 1 in the item slots (the page index is kept; the enter hook
 * restores it on the next visit).
 */
void rangerHollowPages_onShopExit(void)
{
    if (g_ShopType != SHOP_TYPE_RANGER_HOLLOW || SCENE_ARG_SHOP_STATE != SHOP_STATE_NONE)
    {
        return;
    }

    if (g_GameFlags.d.coopActive)
    {
        return;
    }

    s32 cur = currentPage();
    writePage(cur);

    // park page 1 in the item slots so the stock header serialize keeps the mirror tracking
    // page 1 (see the file header)
    if (cur != 0)
    {
        readPage(0);
    }
}
