#include "scene/shop.h"
#include "actor.h"
#include "font.h"
#include "game.h"
#include "gba_io.h"
#include "gfx.h"
#include "input.h"
#include "item.h"
#include "mission.h"
#include "multiplayer.h"
#include "player.h"
#include "scene.h"
#include "sprite.h"
#include "text.h"
#include "variables.h"

/**
 * Enter handler for the shop scene: builds the menu screen for the incoming shop type and
 * fades it in. Takes the shop type out of SCENE_ARG_SHOP_TYPE into g_ShopType and clears
 * the scene's scratch slots. For the Rune Forge it also picks the two runes on offer
 * (g_RuneForgeRuneA / g_RuneForgeRuneB): the lowest two rune indices the player has not
 * collected, or RUNE_NONE when that forge slot was already used this mission.
 *
 * @romaddress 0x0801da70
 */
void scene_shop_enter(void)
{
    u8 i;
    u8 playerIndex;
    u32 iconX;
    u32 iconY;
    char *str;

    playerIndex = ACTIVE_PLAYER_INDEX;
    iconX = 0;
    iconY = 0;
    for (i = 0; i < BACKPACK_SLOT_COUNT; i++)
    {
        g_ShopBackpackSlotSprites[i] = NULL;
    }
    SCENE_ARG_SHOP_STATE = SHOP_STATE_NONE;
    SCENE_ARG_SHOP_CONFIRM_SEL = SHOP_CONFIRM_YES;
    g_ShopType = (u8)SCENE_ARG_SHOP_TYPE;
    SCENE_ARG_SHOP_TYPE = 0;

    gfx_initDisplay(0);
    gfx_enableDisplayFlags(GFX_DISPCNT_OBJ);
    gfx_commitBgs();
    gfx_setupBg(0, ShopBgCnt0);
    gfx_loadBgAsset(0, ShopBg0Asset, 1, 0, 0, 0);
    gfx_setupBg(2, ShopBgCnt2);
    gfx_setBgOffset(2, 0);
    gfx_setupBg(3, ShopBgCnt3);
    gfx_setBgOffset(3, 0);

    if (g_ShopType == SHOP_TYPE_FORGE)
    {
        gfx_loadBgAsset(0, ForgeBgAsset, 0x1aa, 6, 0, 2);
        iconX = 4;
        iconY = 5;
        scene_shop_drawUpgradeOffer(playerIndex);
        gfx_loadBgAsset(2, ForgeBargainIconAsset, 0x2aa, 7, 8, 9);
    }
    else if (g_ShopType == SHOP_TYPE_RANGER_HOLLOW)
    {
        gfx_loadBgAsset(0, RangerHollowBgAsset, 0x1aa, 6, 0, 2);
        iconX = 1;
        iconY = 15;
        gfx_loadBgAsset(0, EntWaterIconAsset, 0x2a1, 0, ShopCursorPosHollow[0].x / 8,
                        ShopCursorPosHollow[0].y / 8);
    }
    else if (g_ShopType == SHOP_TYPE_SHRINE)
    {
        gfx_loadBgAsset(0, ShrineBgAsset, 0x1aa, 6, 0, 2);
        iconX = 3;
        iconY = 11;
        gfx_loadBgAsset(0, ShrineAttributePointIconAsset, 0x2a1, 7, 2, 15);
        gfx_loadBgAsset(0, ShrineSkillPointIconAsset, 0x2aa, 7, 6, 15);
        gfx_loadBgAsset(0, ShrineKingsfoilIconAsset, 0x2b3, 8, 10, 15);
    }
    else if (g_ShopType == SHOP_TYPE_RUNE_FORGE)
    {
        gfx_loadBgAsset(0, RuneForgeBgAsset, 0x1aa, 6, 0, 2);
        iconX = 3;
        iconY = 16;
        scene_shop_drawUpgradeOffer(playerIndex);

        if ((PLAYER(playerIndex).statusFlags.p & PLAYER_STATUS_FLAG_RUNE_FORGE_A_USED) == 0)
        {
            i = 0;
            while (i < RUNE_COUNT && ((PLAYER(playerIndex).runes.p >> i) & 1) != 0)
            {
                i++;
            }
            g_RuneForgeRuneA[playerIndex] = i;
        }
        else
        {
            g_RuneForgeRuneA[playerIndex] = RUNE_NONE;
        }
        if (g_RuneForgeRuneA[playerIndex] == RUNE_NONE)
        {
            gfx_loadBgAsset(0, ShopSoldOutIconAsset, 0x2aa, 7, 6, 12);
        }
        else
        {
            gfx_loadBgAsset(0, ShopRuneOfferIconAsset, 0x2aa, 7, 6, 12);
        }

        if ((PLAYER(playerIndex).statusFlags.p & PLAYER_STATUS_FLAG_RUNE_FORGE_B_USED) == 0)
        {
            i = 0;
            while (i < RUNE_COUNT && (((PLAYER(playerIndex).runes.p >> i) & 1) != 0 ||
                                      g_RuneForgeRuneA[playerIndex] == i))
            {
                i++;
            }
            g_RuneForgeRuneB[playerIndex] = i;
        }
        else
        {
            g_RuneForgeRuneB[playerIndex] = RUNE_NONE;
        }
        if (g_RuneForgeRuneB[playerIndex] == RUNE_NONE)
        {
            gfx_loadBgAsset(0, ShopSoldOutIconAsset, 0x2b3, 8, 10, 12);
        }
        else
        {
            gfx_loadBgAsset(0, ShopRuneOfferIconAsset, 0x2b3, 8, 10, 12);
        }
    }

    gfx_loadBgAsset(2, ShopGemPouchIconAsset, 0x298, 0, iconX, iconY);
    gfx_setupBg(1, ShopBgCnt1);
    gfx_loadPalette(ShopPalette, 0);
    gfx_setBgOffset(1, 0);
    font_setRenderCtxBg(ShopBgCnt1);
    font_setStyle(FONT_STYLE_11PX_A, 3, 0);
    str = text_getString(TEXT_ID_SHOP_TITLE_BASE + g_ShopType);
    font_drawString(1, 0x78, 4, str, 1);
    scene_shop_createCursor();
    scene_shop_refreshSlotSprites(FALSE);
    if ((PLAYER(playerIndex).statusFlags.p & PLAYER_STATUS_FLAG_ENT_WATER) != 0)
    {
        gfx_loadBgAsset(0, EntWaterOwnedIconAsset, 0x394, 0, 20, 2);
    }
    scene_shop_adjustGems(0, ACTIVE_PLAYER_INDEX);
    gfx_registerPaletteCycles(ShopPaletteCycles);

    if ((g_GameFlags.p & GAME_FLAG_COOP_ACTIVE) == 0)
    {
        actor_tickAll();
        sprite_endFrame();
    }
    if ((g_GameFlags.p & GAME_FLAG_COOP_ACTIVE) != 0)
    {
        gfx_fadeInFromBlackCoop(GFX_BLEND_ALL_TARGETS);
    }
    else
    {
        gfx_fadeInFromBlack(GFX_BLEND_ALL_TARGETS);
    }
    gfx_setBlendControl(8, 1);
    gfx_setBlendAlpha(8, 8);
    g_GameFlags.p = (g_GameFlags.p | GAME_FLAG_18) & ~(GAME_FLAG_REQUEST_OVERLAY | GAME_FLAG_20);
}

/**
 * Per-frame update for the shop menu (scene 0x14): dispatches the active player's pressed
 * keys to scene_shop_onConfirm / scene_shop_moveCursor / scene_shop_onBack, and keeps the
 * world behind the menu ticking while the link flag is set.
 *
 * @romaddress 0x0801df04
 */
void scene_shop_tick(void)
{
    u32 i;
    u32 keys;
    u8 playerIndex = ACTIVE_PLAYER_INDEX;
    // volatile: the ROM reads the mask back from its frame slot at each use
    volatile u32 linkMsg = multiplayer_getMessageMask();

    if ((linkMsg & MP_MSG_FLAG_2) != 0)
    {
        for (i = 0; i < g_PlayerGlobals.activePlayerCount; i++)
        {
            mission_tickCameraPan(i);
            gfx_updatePlayerCameraScroll(i);
        }
    }

    keys = MENU_KEYS_PRESSED(playerIndex);
    if ((keys & A_BUTTON) != 0)
    {
        scene_shop_onConfirm();
    }
    else if ((keys & DPAD_ANY) != 0)
    {
        scene_shop_moveCursor();
    }
    else
    {
        u32 back = B_BUTTON | SELECT_BUTTON;

        back &= keys;
        if (back != 0)
        {
            scene_shop_onBack();
        }
    }

    actor_tickAll();
    if ((linkMsg & MP_MSG_FLAG_2) != 0)
    {
        game_tickFxAndHuds();
    }
    sprite_endFrame();
}
