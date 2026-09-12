#pragma once

#include "types.h"

#include "item.h"

/**
 * g_ShopType values. The shop title is drawn as
 * text_getString(TEXT_ID_SHOP_TITLE_BASE + g_ShopType) (consecutive in this order).
 */
enum ShopType
{
    SHOP_TYPE_FORGE = 0,
    SHOP_TYPE_RANGER_HOLLOW = 1,
    SHOP_TYPE_SHRINE = 2,
    SHOP_TYPE_RUNE_FORGE = 3,
};

// Shop-menu cursor cells. 0..7 = backpack cells (Player.inventory slots 8..15);
// 8 = the shop's service cell; for the Ranger Hollow, 9..17 are the 9 stash cells.
#define SHOP_CURSOR_SERVICE 8
#define SHOP_CURSOR_HOLLOW_FIRST 9
#define SHOP_CURSOR_HOLLOW_LAST 17

#define BACKPACK_SLOT_COUNT 8
#define HOLLOW_SLOT_COUNT 9

/** gem cost of the Ranger Hollow's Ent Water service (revives on KO; one-time purchase) */
#define ENT_WATER_COST 500

/**
 * SCENE_ARG_SHOP_STATE values on the shop scene. The
 * GameCube link scene reuses the same scratch word with its own states.
 */
enum ShopState
{
    SHOP_STATE_NONE = 0,
    SHOP_STATE_CONFIRM_PURCHASE = 1,
    SHOP_STATE_CONFIRM_SELL = 2,
};

/** Yes/No confirm-option indices (strings 78/79) for SCENE_ARG_SHOP_CONFIRM_SEL. */
enum ShopConfirmOption
{
    SHOP_CONFIRM_YES = 0,
    SHOP_CONFIRM_NO = 1,
};

/** Cursor-cell screen position in pixels. */
typedef struct ShopCursorPos
{
    s32 x;
    s32 y;
} ShopCursorPos;

/** SceneState.args scratch-slot roles for the shop menu. */
enum ShopArg
{
    SCENE_ARG_SHOP_TYPE_INDEX = 0, // incoming shop type (enum ShopType)
    SCENE_ARG_SHOP_CONFIRM_SEL_INDEX = 2,
    SCENE_ARG_SHOP_STATE_INDEX = 3,
    SCENE_ARG_SHOP_FREE_SLOT_INDEX = 4,
};

// g_SceneCurrent.args accessors for the slots above.
#define SCENE_ARG_SHOP_TYPE g_SceneCurrent.args[SCENE_ARG_SHOP_TYPE_INDEX]
#define SCENE_ARG_SHOP_CONFIRM_SEL g_SceneCurrent.args[SCENE_ARG_SHOP_CONFIRM_SEL_INDEX]
#define SCENE_ARG_SHOP_STATE g_SceneCurrent.args[SCENE_ARG_SHOP_STATE_INDEX]
#define SCENE_ARG_SHOP_FREE_SLOT ((u8)g_SceneCurrent.args[SCENE_ARG_SHOP_FREE_SLOT_INDEX])

// Scene 0x14: shop menu.
void scene_shop_enter(void);
void scene_shop_tick(void);
void scene_shop_exit(void);

/**
 * A-button commit for the highlighted cell. Ranger Hollow: store (backpack cell, scans
 * for a free stash cell) or take (stash cell, scans for a free backpack cell) directly.
 * Anything else arms a Yes/No confirm (SCENE_ARG_SHOP_STATE) for a purchase or a sell.
 * In co-op the action is sent as a multiplayer message instead of applied locally.
 */
void scene_shop_onConfirm(void);

/** d-pad cursor movement over the shop grid; Yes/No toggle while a confirm is up. */
void scene_shop_moveCursor(void);

void scene_shop_onBack(void);

/**
 * Draw the right-hand info panel for the highlighted cell: item name/min-level/stats for
 * backpack and Hollow cells, or the service name/description/cost for the service cell.
 */
void scene_shop_drawInfoPanel(u8 cursorSlot, u8 playerIndex);

/**
 * Purchase the highlighted shop service for a player: Ent Water (Hollow), equipment
 * tier upgrade or adventurer's bargain random item (Forge), attribute/skill point or
 * Kingsfoil Herb (Shrine), rune unlock (Rune Forge). Deducts gems and applies the effect.
 * A Rune Forge purchase unlocks the offered rune for carving (sets its Player.runes bit)
 * instead of finding its drop; each forge slot sells one rune per mission (flagged in
 * Player.statusFlags bits 4/5, cleared on entering a different mission). Carving an
 * unlocked rune into an item is a separate, free inventory action.
 *
 * @param backpackSlot free backpack cell pre-scanned by scene_shop_drawInfoPanel
 *        (SCENE_ARG_SHOP_FREE_SLOT); target of the bargain's random item
 * @param runeIndex selected rune for the Rune Forge services
 * @return TRUE if the purchase went through
 */
bool scene_shop_purchaseService(u8 playerIndex, u8 cursorSlot, u8 backpackSlot, u8 runeIndex,
                                u8 stationType);

/**
 * Sell the item in an inventory slot (8..15 = backpack) for scene_shop_computeGemValue gems.
 *
 * @param cursorSlot UI cell whose sprite is cleared on success
 *
 * @return FALSE if the slot is empty
 */
bool scene_shop_sellItem(u8 playerIndex, u8 inventorySlot, u8 cursorSlot);

/** An item's sale value in gems. */
s32 scene_shop_computeGemValue(u32 itemHandle);

/**
 * Add delta (may be negative) to a player's gem balance, clamped to 0..0xFFFF, and redraw
 * the gem counter when the player is the active one.
 */
void scene_shop_adjustGems(s16 delta, u8 playerIndex);

/**
 * Redraw the backpack cell sprites whose inventory slot holds an item, and refresh the info
 * panel when the cursor sits on one. Also draws the nine Ranger Hollow stash cells unless
 * backpackOnly is set.
 */
void scene_shop_refreshSlotSprites(bool backpackOnly);

/**
 * Draw the equipment-upgrade service icon for the active player: the next whetstone tier
 * (Player.whetstoneLevel + 1), or the sold-out icon at max tier. Draws at the Forge's first
 * service cell, or the Rune Forge's when g_ShopType is SHOP_TYPE_RUNE_FORGE.
 */
void scene_shop_drawUpgradeOffer(u8 playerIndex);

/**
 * Create the shop cursor actor into g_ShopCursor, parked on backpack cell 0 with the
 * animation scene_shop_itemCursorAnimation picks for the item there.
 */
void scene_shop_createCursor(void);

/** Spawn the item sprite object for a backpack cell (0..7) into g_ShopBackpackSlotSprites. */
void scene_shop_spawnBackpackSlotSprite(u8 backpackSlot);

/** Draw the item tiles of one Ranger Hollow cell (0..8). */
void scene_shop_drawHollowSlotItem(Item item, u8 hollowSlot);

/** Clear the item tiles of one Ranger Hollow cell (0..8). */
void scene_shop_clearHollowSlotItem(u8 hollowSlot);

/**
 * Move an item between a player's inventory and the Ranger Hollow stash. inventorySlot is
 * a Player.inventory index (8..15, i.e. backpack cell + 8); hollowSlot is 0..8. Both
 * refresh the shop UI only for the active player, so the link peer's moves stay silent.
 */
void scene_shop_storeItemInHollow(u32 playerIndex, u32 inventorySlot, u32 hollowSlot);
void scene_shop_takeItemFromHollow(u32 playerIndex, u32 inventorySlot, u32 itemHandle,
                                   u32 hollowSlot);

/** Reset a player's 9 Ranger Hollow stash slots to empty. */
void scene_shop_clearHollowItems(u32 playerIndex);

/** Draw one Yes/No confirm option; highlighted when optionIndex == selectedIndex. */
void scene_shop_drawConfirmOption(u8 optionIndex, u8 selectedIndex);

/**
 * Cursor-sprite variant for the highlighted item: 2 = normal or empty cell, 7 = carried
 * passive (ITEM_FLAG_CARRIED_PASSIVE) or not usable by the player. Indexes the cursor animation
 * table.
 */
u8 scene_shop_itemCursorAnimation(Item item, u8 playerIndex);

// Shop service costs in gems.
u32 scene_shop_attributePointCost(u8 playerIndex);
u32 scene_shop_skillPointCost(u8 playerIndex);
u32 scene_shop_bargainCost(u8 playerIndex);
