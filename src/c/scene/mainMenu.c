#include "scene/mainMenu.h"
#include "font.h"
#include "game.h"
#include "gba_io.h"
#include "gfx.h"
#include "input.h"
#include "libc.h"
#include "match_hacks.h"
#include "menu.h"
#include "multiplayer.h"
#include "player.h"
#include "rng.h"
#include "save.h"
#include "scene.h"
#include "scene/saveLoadMenu.h"
#include "scene/shop.h"
#include "sfx.h"
#include "sprite.h"
#include "text.h"
#include "variables.h"

/** Draw one menu row: entry text centered at x 0x78, tile base and y derived from the row. */
static inline void drawEntryInline(s32 entryIndex, s32 color)
{
    char *str;
    s32 tileBase;
    s32 y;

    tileBase = entryIndex * 0x1e + 1;
    y = entryIndex * 8 + 0x60;
    str = text_getString(TEXT_ID_MAIN_MENU_ENTRY_BASE + entryIndex);
    font_setStyle(FONT_STYLE_8PX_B, color, 0);
    font_drawString(tileBase, 0x78, y, str, 1);
}

/**
 * Initialise the main menu scene (scene 0x05). Sets up all four BG layers, loads
 * the appropriate BG1 language assets, resets player state, and either shows
 * "Press START" (fresh boot) or arms the redraw delay (returning from submenu).
 *
 * @romaddress 0x08047ef4
 */
void scene_mainMenu_enter(void)
{
    u32 bg0Cnt;
    u32 i;

    sfx_requestSong(0);

    g_CoopUnlockFlags.p = 0;
    g_GameFlags.p &= 0xfdbfbfff;
    memset(&g_PlayerGlobals, 0, 0x35c);
    scene_clearStateBytes();
    g_PlayerGlobals.activePlayerCount = 1;
    player_clearInventory(0);
    scene_shop_clearHollowItems(0);
    player_clearInventory(1);
    scene_shop_clearHollowItems(1);
    player_resetActiveSkills();
    gfx_initDisplay(0);
    sprite_resetTileAllocatorForMode(0);
    gfx_enableDisplayFlags(GFX_DISPCNT_OBJ);

    gfx_setupBg(1, MainMenuBgCnt1);
    gfx_setBgOffset(1, 0);
    gfx_loadBgAsset(1, MainMenuBgAssets[g_SaveGlobals.d.language], 1, 0, 0, 0);

    gfx_disableBg(2);
    gfx_setupBg(2, MainMenuBgCnt2);
    gfx_loadBgAsset(2, MainMenuBg2Asset, 0xfb, 0, 0, 0);

    gfx_disableBg(3);
    gfx_setupBg(3, MainMenuBgCnt3);
    gfx_loadBgAsset(3, MainMenuBg3Asset, 0x1f5, 0, 0, 0);

    bg0Cnt = MainMenuBgCnt0;
    gfx_setupBg(0, bg0Cnt);
    gfx_setBgOffset(0, 0);
    gfx_loadPalette(UiPalette, 0);
    font_setRenderCtxBg(bg0Cnt);

    if (g_ScenePrevious.id <= 4)
    {
        char *str;

        str = text_getString(TEXT_ID_PRESS_START);
        font_setStyle(FONT_STYLE_8PX_B, 0, 0);
        font_drawString(1, 0x78, 0x70, str, 1);
        SCENE_ARG_MAIN_MENU_STATE = MAIN_MENU_STATE_PRESS_START;
    }
    else
    {
        scene_mainMenu_clearPrompt();
    }

    SCENE_ARG_MAIN_MENU_CURSOR = 0;
    g_MenuReturnScene = g_SceneCurrent.id;
    actor_tickAll();
    sprite_endFrame();
    g_GameFlags.p &= 0x03e3fd47;
    multiplayer_clearSessionFlag(1);
    multiplayer_endSession();
    multiplayer_resetLink();
    multiplayer_setAbortScene(5);
    gfx_fadeSetDarken(0x3f, 0);
    gfx_fadeInFromBlack(0x3f);
    gfx_registerPaletteCycles(MainMenuPaletteCycles);

    for (i = 0; i < 2; i++)
    {
        PLAYER(i).ownerActor = NULL;
    }

    player_resetHuds();
}

/**
 * Per-frame update for the main menu scene (scene 0x05): state machine on
 * SCENE_ARG_MAIN_MENU_STATE (enum MainMenuState).
 *
 * @romaddress 0x080480d8
 */
void scene_mainMenu_tick(void)
{
    MpMessageArgs msgPayload; // uninitialized: queued as-is (original-game bug)
    u32 state;
    u32 i;

    state = SCENE_ARG_MAIN_MENU_STATE;

    if (state == MAIN_MENU_STATE_PRESS_START)
    {
        if ((PLAYER_KEYS_PRESSED(ACTIVE_PLAYER_INDEX) & (A_BUTTON | START_BUTTON)) != 0)
        {
            scene_mainMenu_clearPrompt();
            if (sfx_isPlaying(g_MainMenuSfxHandle))
            {
                sfx_stop(g_MainMenuSfxHandle);
            }
            g_MainMenuSfxHandle = sfx_play(SFX_MENU_CONFIRM);
        }
    }
    else if (state == MAIN_MENU_STATE_REDRAW_DELAY)
    {
        if (SCENE_ARG_MAIN_MENU_REDRAW_DELAY-- == 0)
        {
            SCENE_ARG_MAIN_MENU_STATE = MAIN_MENU_STATE_IDLE;
            for (i = 0; i < MAIN_MENU_ENTRY_COUNT; i++)
            {
                scene_mainMenu_drawEntry(i, SCENE_ARG_MAIN_MENU_CURSOR);
            }
        }
    }
    else
    {
        if (state == MAIN_MENU_STATE_LINK_WAIT)
        {
            s32 linkStatus;

            linkStatus = multiplayer_startSession(2);
            if (linkStatus != 7 && linkStatus != 5)
            {
                SCENE_ARG_MAIN_MENU_STATE = MAIN_MENU_STATE_LINK_HANDSHAKE;
            }
            if ((PLAYER_KEYS_PRESSED(ACTIVE_PLAYER_INDEX) & B_BUTTON) != 0)
            {
                multiplayer_abortSession();
                scene_mainMenu_drawEntries();
            }
        }
        else if (state == MAIN_MENU_STATE_LINK_HANDSHAKE)
        {
            s32 handshake;

            handshake = 1;
            if ((g_GameFlags.p & 0x20) == 0)
            {
                handshake = multiplayer_pollSession();
            }
            if (multiplayer_allPlayersConnected() && handshake != 0 && handshake != 6)
            {
                if (multiplayer_getMultiPlayerId() == 0)
                {
                    multiplayer_syncBarrier();
                }
                SCENE_ARG_MAIN_MENU_STATE = MAIN_MENU_STATE_LINK_LIVE;
                if (multiplayer_getMultiPlayerId() == 0)
                {
                    multiplayer_queueMessage(MP_MSG_COOP_SAVE_MENU, msgPayload);
                }
            }
            if ((PLAYER_KEYS_PRESSED(ACTIVE_PLAYER_INDEX) & B_BUTTON) != 0)
            {
                multiplayer_abortSession();
                scene_mainMenu_drawEntries();
            }
        }
        else if (state != MAIN_MENU_STATE_LINK_LIVE)
        {
            if ((PLAYER_KEYS_PRESSED(ACTIVE_PLAYER_INDEX) & (A_BUTTON | START_BUTTON)) != 0)
            {
                scene_mainMenu_onConfirm();
            }
            else if ((PLAYER_KEYS_PRESSED(ACTIVE_PLAYER_INDEX) & DPAD_ANY) != 0)
            {
                scene_mainMenu_moveCursor();
            }
        }
    }

    actor_tickAll();
    sprite_endFrame();
}

/**
 * Draw the multiplayer link waiting screen ("Waiting for other player" and
 * "Press the B Button to cancel") and arm MAIN_MENU_STATE_LINK_WAIT.
 *
 * @romaddress 0x08048240
 */
void scene_mainMenu_showLinkPrompt(void)
{
    char *str;
    s32 mode, mode2;

    gfx_fillBgRect(0, 0, 0xc, 0x1e, 8, 0);

    mode = 1;

    str = text_getString(TEXT_ID_WAITING_FOR_OTHER_PLAYER);
    font_setStyle(FONT_STYLE_8PX_A, 3, 0);
    // HACK: The constant 1 needs to be in 2 registers which I cannot reproduce without a hack
    MATCH_FRESH(mode2, 1);

    font_drawString(1, 0x78, 0x60, str, mode);

    str = text_getString(TEXT_ID_PRESS_THE_B_BUTTON_TO_CANCEL);
    font_setStyle(FONT_STYLE_8PX_A, 3, 0);
    font_drawString(0x11, 0x78, 0x68, str, mode2);

    SCENE_ARG_MAIN_MENU_STATE = MAIN_MENU_STATE_LINK_WAIT;

    if (sfx_isPlaying(g_MainMenuSfxHandle))
    {
        sfx_stop(g_MainMenuSfxHandle);
    }
    g_MainMenuSfxHandle = sfx_play(SFX_MENU_CONFIRM);
}

/**
 * Redraw all six main menu entries. Clears the text band, draws each entry
 * (highlighted at SCENE_ARG_MAIN_MENU_CURSOR), returns to the idle state, and plays the
 * confirm/back sfx.
 *
 * @romaddress 0x080482cc
 */
void scene_mainMenu_drawEntries(void)
{
    s32 color;
    u32 i;

    gfx_fillBgRect(0, 0, 0xc, 0x1e, 8, 0);

    for (i = 0; i < MAIN_MENU_ENTRY_COUNT; i++)
    {
        u32 cursor;

        cursor = SCENE_ARG_MAIN_MENU_CURSOR;
        color = 3;
        if (i == cursor)
        {
            color = 0;
        }
        drawEntryInline(i, color);
    }

    SCENE_ARG_MAIN_MENU_STATE = MAIN_MENU_STATE_IDLE;

    if (sfx_isPlaying(g_MainMenuSfxHandle))
    {
        sfx_stop(g_MainMenuSfxHandle);
    }
    g_MainMenuSfxHandle = sfx_play(SFX_MENU_CONFIRM);
}

/**
 * A-press dispatch on the main menu: request the cursor entry's scene.
 * Always reseeds the RNG on exit.
 *
 * @romaddress 0x08048368
 */
void scene_mainMenu_onConfirm(void)
{
    if (sfx_isPlaying(g_MainMenuSfxHandle))
    {
        sfx_stop(g_MainMenuSfxHandle);
    }

    switch (SCENE_ARG_MAIN_MENU_CURSOR)
    {
    case MAIN_MENU_ENTRY_NEW_GAME:
        game_requestSceneChange(SCENE_ID_NEW_GAME);
        g_MainMenuSfxHandle = sfx_play(SFX_MENU_CONFIRM);
        break;
    case MAIN_MENU_ENTRY_LOAD_GAME:
        game_requestSceneChangeWithArgs(SCENE_ID_SAVE_LOAD_MENU, SAVE_LOAD_MODE_LOAD, 0);
        g_MainMenuSfxHandle = sfx_play(SFX_MENU_CONFIRM);
        break;
    case MAIN_MENU_ENTRY_OPTIONS:
        game_requestSceneChange(SCENE_ID_OPTIONS_MENU);
        g_MainMenuSfxHandle = sfx_play(SFX_MENU_CONFIRM);
        break;
    case MAIN_MENU_ENTRY_MULTIPLAYER:
        g_MainMenuSfxHandle = sfx_play(SFX_MENU_CONFIRM);
        scene_mainMenu_showLinkPrompt();
        break;
    case MAIN_MENU_ENTRY_BONUS_MAPS:
        game_requestSceneChangeWithArgs(SCENE_ID_SAVE_LOAD_MENU, SAVE_LOAD_MODE_BONUS_LOAD, 0);
        g_MainMenuSfxHandle = sfx_play(SFX_MENU_CONFIRM);
        break;
    case MAIN_MENU_ENTRY_GCN_LINK:
        g_MainMenuSfxHandle = sfx_play(SFX_MENU_CONFIRM);
        game_requestSceneChange(SCENE_ID_GCN_LINK_MENU);
        break;
    default:
        break;
    }

    rng_reseed();
}

/**
 * Move the main menu cursor on d-pad input and redraw the old and new rows.
 * Plays the cursor-move sfx even if the cursor did not move.
 *
 * @romaddress 0x0804841c
 */
void scene_mainMenu_moveCursor(void)
{
    u32 oldCursor, newCursor;
    s32 color;

    oldCursor = SCENE_ARG_MAIN_MENU_CURSOR;
    menu_movePlayerCursorVertical(&SCENE_ARG_MAIN_MENU_CURSOR, 0, MAIN_MENU_ENTRY_COUNT - 1, TRUE,
                                  ACTIVE_PLAYER_INDEX);

    newCursor = SCENE_ARG_MAIN_MENU_CURSOR;

    color = 3;
    if (oldCursor == newCursor)
    {
        color = 0;
    }

    drawEntryInline(oldCursor, color);
    drawEntryInline(SCENE_ARG_MAIN_MENU_CURSOR, 0);

    if (sfx_isPlaying(g_MainMenuSfxHandle))
    {
        sfx_stop(g_MainMenuSfxHandle);
    }
    g_MainMenuSfxHandle = sfx_play(SFX_MENU_CURSOR_MOVE);
}

/**
 * Exit handler for the main menu scene (scene 0x05).
 *
 * @romaddress 0x080484e8
 */
void scene_mainMenu_exit(void)
{
    gfx_fadeOutToBlackWhileRunning(0x3f);
    sprite_flushList(&g_ActorListMain);
    gfx_commitBgs();
    gfx_clearPaletteAnims();
    sfx_stopAllOnExit();
}

/**
 * Draw one main menu entry at the given index, highlighted when entryIndex
 * equals selectedIndex.
 *
 * @param entryIndex   0-5 menu row index
 * @param selectedIndex currently selected row (SCENE_ARG_MAIN_MENU_CURSOR during state-1 redraw
 * loop)
 *
 * @romaddress 0x0804850c
 */
void scene_mainMenu_drawEntry(s32 entryIndex, s32 selectedIndex)
{
    s32 color;

    color = 3;
    if (entryIndex == selectedIndex)
    {
        color = 0;
    }
    drawEntryInline(entryIndex, color);
}

/**
 * Clear the "Press START" prompt band and arm the redraw delay counter.
 *
 * @romaddress 0x08048550
 */
void scene_mainMenu_clearPrompt(void)
{
    gfx_fillBgRect(0, 4, 14, 22, 2, 0);
    SCENE_ARG_MAIN_MENU_REDRAW_DELAY = 4;
    SCENE_ARG_MAIN_MENU_STATE = MAIN_MENU_STATE_REDRAW_DELAY;
}
