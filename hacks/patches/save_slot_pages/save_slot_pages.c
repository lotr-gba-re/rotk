// Expand the save/load menu to handle the additional save slots by introducing pagination.
// Ported from the original, fully assembly-based SavSlotPages ROM-hack by SuperSaiyajinStackZ.

#include "font.h"
#include "game.h"
#include "gfx.h"
#include "input.h"
#include "menu.h"
#include "patches/mod_save/save_layout.h"
#include "patches/mod_save/save_slots.h"
#include "player.h"
#include "save.h"
#include "scene.h"
#include "scene/saveLoadMenu.h"
#include "sfx.h"
#include "sprite.h"
#include "text.h"
#include "variables.h"

// Page indicator ("@1/@2") on the load-game title row, top-right.
#define PAGE_INDICATOR_X (240 - 24)
#define PAGE_INDICATOR_Y 3

/**
 * Charbase tile index font_rasterizeString writes the indicator glyphs into.
 * Inherited from the original SavSlotPages hack.
 */
#define PAGE_INDICATOR_TILE 0x1A2u

/** Draw the "current/total" page indicator, top-right of the bar. */
static void drawPageIndicator(void)
{
    font_setStyle(FONT_STYLE_11PX_B, 3, 0);
    font_formatIntSlot0((s32)g_SaveSlotPage + 1);
    font_formatIntSlot1((s32)SAVE_SLOT_PAGE_COUNT);
    font_rasterizeString(PAGE_INDICATOR_TILE, PAGE_INDICATOR_X, PAGE_INDICATOR_Y, "@1/@2");
}

/**
 * Switch to `page`, reload its slots into the mirror, and redraw every panel, hero sprite, and the
 * indicator with `highlightedSlot` selected.
 */
static void flipToPage(u32 page, u32 highlightedSlot)
{
    g_SaveSlotPage = page;
    // The stock menu mutes the sound engine around this read as it interferes with
    // EEPROM. mod_save reads SRAM, so the mute is unnecessary.
    save_refreshSlots();
    for (u32 slot = 0; slot < SAVE_SLOTS_PER_PAGE; slot++)
    {
        scene_saveLoadMenu_drawSlot(slot, highlightedSlot);
        scene_saveLoadMenu_refreshSlot(slot);
    }
    drawPageIndicator();
}

/**
 * Set up the save/load slot screen, then draw the page indicator. save_refreshSlots reads
 * the current page through mod_save's save_readSlot replacement.
 *
 * REPLACES (whole function): scene_saveLoadMenu_enter @0x080305c4.
 */
void saveSlotPages_enter(void)
{
    u32 bg1cnt;
    s32 titleId;
    u8 slot;

    g_TransitionState.flags &= ~TRANSITION_FLAG_SFX_ACTIVE;
    gfx_initDisplay(0);
    gfx_enableDisplayFlags(GFX_DISPCNT_OBJ);
    gfx_setupBg(0, MenuBackdropBgCnt);
    gfx_loadBgAsset(0, SaveLoadBg0Asset, 1, 0, 0, 0);
    bg1cnt = MenuTextBgCnt;
    gfx_setupBg(1, bg1cnt);
    gfx_loadPalette(UiPalette, 0);
    gfx_setBgOffset(1, 0);
    sfx_disableAudio();
    save_refreshSlots();
    sfx_enableAudio();
    SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_SLOT_SELECT;
    font_setRenderCtxBg(bg1cnt);
    font_setStyle(FONT_STYLE_11PX_A, 3, 0);
    if (SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_SAVE ||
        SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_SAVE_VICTORY)
    {
        titleId = TEXT_ID_SAVE_GAME;
    }
    else
    {
        titleId = TEXT_ID_LOAD_GAME;
    }
    font_drawString(1, 0x78, 3, text_getString(titleId), 1);
    SCENE_ARG_SAVE_LOAD_SELECTED_SLOT = g_LastSaveSlot;
    scene_saveLoadMenu_setupButtonSprites();
    scene_saveLoadMenu_drawActionPanel();
    scene_saveLoadMenu_setupSlotSprites();
    for (slot = 0; slot < SAVE_SLOTS_PER_PAGE; slot++)
    {
        scene_saveLoadMenu_drawSlot(slot, SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
    }

    // CHANGE START (page indicator)
    drawPageIndicator();
    // CHANGE END

    gfx_registerPaletteCycles(SaveLoadPaletteCycles);
    if (SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_COOP_LOAD)
    {
        g_GameFlags.p |= GAME_FLAG_COOP_ACTIVE;
    }
    if (SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_BONUS_LOAD)
    {
        g_GameFlags.p |= GAME_FLAG_BONUS_MAP_ACTIVE;
    }
    actor_tickAll();
    sprite_endFrame();
    gfx_fadeInFromBlack(GFX_BLEND_ALL_TARGETS);
}

/**
 * D-pad handler with page flips: left/right still toggle the column, and moving off the
 * grid's outer edge flips to the previous/next page (wrapping at the ends).
 *
 * REPLACES (whole function): scene_saveLoadMenu_moveCursor @0x08030810.
 */
void saveSlotPages_moveCursor(void)
{
    u32 unlocked = 0;
    u8 oldSlot;
    bool rightColumn;
    bool pageFlipped = FALSE;
    u32 nextPage;

    switch (SCENE_ARG_SAVE_LOAD_STATE)
    {
    case SAVE_LOAD_STATE_SLOT_SELECT:
        oldSlot = SCENE_ARG_SAVE_LOAD_SELECTED_SLOT;
        switch (ACTIVE_MENU_KEYS_PRESSED & DPAD_ANY)
        {
        case DPAD_LEFT:
        case DPAD_RIGHT:
            // CHANGE START (page flip): stock only toggles the column (0 <-> 1, 2 <-> 3).
            // Right on the right column or left on the left column also flips the page,
            // so the cursor walks the slots of every page in order.
            rightColumn = SCENE_ARG_SAVE_LOAD_SELECTED_SLOT & 1;
            if (ACTIVE_MENU_KEYS_PRESSED & DPAD_RIGHT)
            {
                pageFlipped = rightColumn;
                nextPage = (g_SaveSlotPage + 1) % SAVE_SLOT_PAGE_COUNT;
            }
            else
            {
                pageFlipped = !rightColumn;
                nextPage = (g_SaveSlotPage + SAVE_SLOT_PAGE_COUNT - 1) % SAVE_SLOT_PAGE_COUNT;
            }
            // CHANGE END

            if (SCENE_ARG_SAVE_LOAD_SELECTED_SLOT == 0 || SCENE_ARG_SAVE_LOAD_SELECTED_SLOT == 2)
            {
                SCENE_ARG_SAVE_LOAD_SELECTED_SLOT += 1;
            }
            else if (SCENE_ARG_SAVE_LOAD_SELECTED_SLOT == 1 ||
                     SCENE_ARG_SAVE_LOAD_SELECTED_SLOT == 3)
            {
                SCENE_ARG_SAVE_LOAD_SELECTED_SLOT -= 1;
            }

            // CHANGE START (page flip)
            if (pageFlipped)
            {
                flipToPage(nextPage, SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
            }
            // CHANGE END

            break;
        case DPAD_UP:
        case DPAD_DOWN:
            if (SCENE_ARG_SAVE_LOAD_SELECTED_SLOT < 2)
            {
                SCENE_ARG_SAVE_LOAD_SELECTED_SLOT += 2;
            }
            else if (SCENE_ARG_SAVE_LOAD_SELECTED_SLOT < 4)
            {
                SCENE_ARG_SAVE_LOAD_SELECTED_SLOT -= 2;
            }
            break;
        }

        // CHANGE START (page flip): a flip already redrew every slot of the new page.
        if (pageFlipped)
        {
            break;
        }
        // CHANGE END

        scene_saveLoadMenu_drawSlot(oldSlot, SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
        scene_saveLoadMenu_drawSlot(SCENE_ARG_SAVE_LOAD_SELECTED_SLOT,
                                    SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
        scene_saveLoadMenu_refreshSlot(oldSlot);
        scene_saveLoadMenu_refreshSlot(SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
        break;

    case SAVE_LOAD_STATE_RESULT:
        SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_SLOT_SELECT;
        scene_saveLoadMenu_drawActionPanel();
        sfx_play(SFX_MENU_CONFIRM);
        break;

    case SAVE_LOAD_STATE_CONFIRM_OVERWRITE:
    case SAVE_LOAD_STATE_CONFIRM_ERASE:
        if (SCENE_ARG_SAVE_LOAD_CURSOR != 0)
        {
            SCENE_ARG_SAVE_LOAD_CURSOR = 0;
        }
        else
        {
            SCENE_ARG_SAVE_LOAD_CURSOR = 1;
        }
        scene_saveLoadMenu_drawYesNo();
        break;

    case SAVE_LOAD_STATE_HERO_SELECT:
        // skip Sam and Smeagol until unlocked
        while (unlocked == 0)
        {
            menu_moveCursorHorizontal(&SCENE_ARG_SAVE_LOAD_CURSOR, 0, HERO_ID_SMEAGOL, TRUE,
                                      ACTIVE_PLAYER_INDEX);
            if (SCENE_ARG_SAVE_LOAD_CURSOR < HERO_ID_SAM)
            {
                break;
            }
            if (SCENE_ARG_SAVE_LOAD_CURSOR == HERO_ID_SAM)
            {
                unlocked = g_SaveGlobals.d.unlockFlags.p & UNLOCK_FLAG_CHARACTER_SAM;
            }
            else if (SCENE_ARG_SAVE_LOAD_CURSOR == HERO_ID_SMEAGOL)
            {
                unlocked = g_SaveGlobals.d.unlockFlags.p & UNLOCK_FLAG_CHARACTER_SMEAGOL;
            }
        }
        scene_saveLoadMenu_drawSlot(SCENE_ARG_SAVE_LOAD_SELECTED_SLOT,
                                    SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
        scene_saveLoadMenu_refreshSlot(SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
        break;
    }
    sfx_play(SFX_MENU_CURSOR_MOVE);
}

/**
 * Commit the highlighted slot. Deleting a save never wipes the artifact mask and unlock
 * flags: stock does that once its four slots are empty, which with pages would fire on
 * the first page emptied while others still hold saves.
 *
 * REPLACES (whole function): scene_saveLoadMenu_commitSlot @0x08030f08.
 */
void saveSlotPages_commitSlot(bool save)
{
    sfx_disableAudio();
    if (save)
    {
        save_serializePlayer(g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].halfwords,
                             g_CurrentMissionId);
        if (SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_SAVE_VICTORY &&
            (g_GameFlags.p & (GAME_FLAG_COOP_ACTIVE | GAME_FLAG_BONUS_MAP_ACTIVE |
                              GAME_FLAG_COOP_LEVEL_SELECTED)) == 0)
        {
            scene_saveLoadMenu_stampSlot();
        }
    }
    else
    {
        save_initEmptySlot(&g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT]);
    }
    save_writeSlot(&g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT],
                   SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);

    // CHANGE START (no wipe): the stock last-save wipe is deliberately deactivated. A
    // whole-chip scan would be the faithful version; a full save wipe, if wanted, belongs
    // in the options / mod options menu instead.
    // if (!save)
    // {
    //     for (slot = 0; slot < SAVE_SLOT_COUNT; slot++)
    //     {
    //         if (save_getSlotMission(&g_SaveSlots[slot]) < SLOT_MISSION_EMPTY)
    //         {
    //             break;
    //         }
    //     }
    //     if (slot == SAVE_SLOT_COUNT)
    //     {
    //         g_SaveGlobals.d.artifactMask.p = 0;
    //         g_SaveGlobals.d.unlockFlags.p = 0;
    //     }
    // }
    // CHANGE END

    save_writeGlobalHeader();
    sfx_enableAudio();
    scene_saveLoadMenu_drawSlot(SCENE_ARG_SAVE_LOAD_SELECTED_SLOT,
                                SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
    scene_saveLoadMenu_refreshSlot(SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
    g_LastSaveSlot = SCENE_ARG_SAVE_LOAD_SELECTED_SLOT;
}
