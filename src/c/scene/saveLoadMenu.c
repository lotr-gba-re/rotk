#include "scene/saveLoadMenu.h"
#include "actor.h"
#include "font.h"
#include "game.h"
#include "gba.h"
#include "gba_io.h"
#include "gfx.h"
#include "input.h"
#include "menu.h"
#include "mission.h"
#include "multiplayer.h"
#include "player.h"
#include "rng.h"
#include "save.h"
#include "scene.h"
#include "sfx.h"
#include "sprite.h"
#include "text.h"
#include "variables.h"

/** Hero index of a slot that shows no hero sprite (empty outside hero select). */
#define SAVE_LOAD_HERO_NONE 8

/**
 * Set up the save/load slot screen (scene 0x09): backgrounds, title, button and slot
 * sprites, and the four slot panels. Co-op and bonus-map loads raise their game flags.
 *
 * @romaddress 0x080305c4
 */
void scene_saveLoadMenu_enter(void)
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
    for (slot = 0; slot < SAVE_SLOT_COUNT; slot++)
    {
        scene_saveLoadMenu_drawSlot(slot, SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
    }
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
 * Per-frame update for the save/load slot screen (scene 0x09).
 *
 * Runs the co-op handshake states and otherwise dispatches dpad/face-key presses
 * for the menu states.
 *
 * @romaddress 0x080306ec
 */
void scene_saveLoadMenu_tick(void)
{
    MpMessageArgs msgArgs;
    // volatile: the ROM reads the mask back from its frame slot at each use
    volatile u32 linkMsg = multiplayer_getMessageMask();

    switch (SCENE_ARG_SAVE_LOAD_STATE)
    {
    case SAVE_LOAD_STATE_COOP_SLOT_EXCHANGE:
        // wait for both sides' done bits, then load the peer's slot
        if ((linkMsg & MP_MSG_FLAG_PEER_READY) != 0 && (linkMsg & MP_MSG_FLAG_LOCAL_READY) != 0)
        {
            SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_COOP_SEED;
            if (multiplayer_getMultiPlayerId() == 0)
            {
                scene_saveLoadMenu_loadPeerSlot(1);
            }
            else
            {
                scene_saveLoadMenu_loadPeerSlot(0);
            }
        }
        else if ((linkMsg & MP_MSG_FLAG_PEER_READY) == 0)
        {
            // msgArgs.words[1]/[2] uninitialized: queued as-is (original-game bug)
            msgArgs.words[0] = 0;
            multiplayer_queueMessage(MP_MSG_SAVE_SLOT_CHUNK, msgArgs);
        }
        break;

    case SAVE_LOAD_STATE_COOP_SEED:
        // host broadcasts the shared seed, the client a plain sync
        SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_COOP_APPLY_OPTIONS;
        if (multiplayer_getMultiPlayerId() == 0)
        {
            msgArgs.words[0] = (u16)rng_mod(0xffff);
            multiplayer_queueMessage(MP_MSG_SEED, msgArgs);
        }
        else
        {
            msgArgs.words[0] = 0;
            multiplayer_queueMessage(MP_MSG_SYNC, msgArgs);
        }
        break;

    case SAVE_LOAD_STATE_COOP_APPLY_OPTIONS:
        SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_COOP_LAUNCH;
        if (multiplayer_getMultiPlayerId() == 0)
        {
            player_applyOptionFlags(0);
        }
        else
        {
            player_applyOptionFlags(1);
        }
        break;

    case SAVE_LOAD_STATE_COOP_LAUNCH:
        game_requestSceneChange(SCENE_ID_MULTIPLAYER_LEVEL_SELECT);
        break;

    default: {
        // menu states: input dispatch
        u32 keys = ACTIVE_MENU_KEYS_PRESSED;

        if ((keys & DPAD_ANY) != 0)
        {
            scene_saveLoadMenu_moveCursor();
        }
        else if ((keys & A_BUTTON) != 0)
        {
            scene_saveLoadMenu_onConfirm();
        }
        else if ((keys & R_BUTTON) != 0)
        {
            scene_saveLoadMenu_onErase();
        }
        else
        {
            u32 back = B_BUTTON | SELECT_BUTTON;

            back &= keys;
            if (back != 0)
            {
                scene_saveLoadMenu_onBack();
            }
        }
        break;
    }
    }
    actor_tickAll();
    sprite_endFrame();
}

/**
 * D-pad handler: moves the cursor for the current state (2x2 slot grid, yes/no pair,
 * or the hero row, skipping locked heroes) and dismisses the result panel.
 *
 * @romaddress 0x08030810
 */
void scene_saveLoadMenu_moveCursor(void)
{
    u32 unlocked = 0;
    u32 firstLockedHero;
    u8 oldSlot;

    switch (SCENE_ARG_SAVE_LOAD_STATE)
    {
    case SAVE_LOAD_STATE_SLOT_SELECT:
        oldSlot = SCENE_ARG_SAVE_LOAD_SELECTED_SLOT;
        switch (ACTIVE_MENU_KEYS_PRESSED & DPAD_ANY)
        {
        case DPAD_LEFT:
        case DPAD_RIGHT:
            if (SCENE_ARG_SAVE_LOAD_SELECTED_SLOT == 0 || SCENE_ARG_SAVE_LOAD_SELECTED_SLOT == 2)
            {
                SCENE_ARG_SAVE_LOAD_SELECTED_SLOT += 1;
            }
            else if (SCENE_ARG_SAVE_LOAD_SELECTED_SLOT == 1 ||
                     SCENE_ARG_SAVE_LOAD_SELECTED_SLOT == 3)
            {
                SCENE_ARG_SAVE_LOAD_SELECTED_SLOT -= 1;
            }
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
        while (unlocked == 0)
        {
            // local bound: a literal here folds to cmp #5 (matching)
            firstLockedHero = HERO_ID_SAM;
            menu_moveCursorHorizontal(&SCENE_ARG_SAVE_LOAD_CURSOR, 0, HERO_ID_SMEAGOL, TRUE,
                                      ACTIVE_PLAYER_INDEX);
            if (SCENE_ARG_SAVE_LOAD_CURSOR != HERO_ID_SAM)
            {
                if (SCENE_ARG_SAVE_LOAD_CURSOR >= firstLockedHero)
                {
                    if (SCENE_ARG_SAVE_LOAD_CURSOR == HERO_ID_SMEAGOL)
                    {
                        unlocked =
                            -(g_SaveGlobals.d.unlockFlags.p & UNLOCK_FLAG_CHARACTER_SMEAGOL) >> 31;
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                unlocked = g_SaveGlobals.d.unlockFlags.p & UNLOCK_FLAG_CHARACTER_SAM;
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
 * A-press handler, switched on the menu state. In slot select the mode picks between
 * loading, the overwrite confirm, and the co-op handshake; the confirm states commit
 * through scene_saveLoadMenu_commitSlot; hero select creates the character.
 *
 * @romaddress 0x08030970
 */
void scene_saveLoadMenu_onConfirm(void)
{
    MpMessageArgs msgArgs;
    u32 zero;
    u8 mission = save_getSlotMission(&g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT]);

    switch (SCENE_ARG_SAVE_LOAD_STATE)
    {
    case SAVE_LOAD_STATE_SLOT_SELECT:
        if (SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_SAVE ||
            SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_SAVE_VICTORY)
        {
            if (mission < SLOT_MISSION_EMPTY)
            {
                SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_CONFIRM_OVERWRITE;
            }
            else
            {
                scene_saveLoadMenu_commitSlot(TRUE);
                SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_RESULT;
            }
            SCENE_ARG_SAVE_LOAD_CURSOR = 0;
            scene_saveLoadMenu_drawActionPanel();
            sfx_play(SFX_MENU_CONFIRM);
        }
        else if (mission < SLOT_MISSION_EMPTY)
        {
            if (SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_LOAD)
            {
                // Sam and Smeagol saves only load into Sam's own missions
                if (g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].d.hero.d.heroId ==
                        HERO_ID_SMEAGOL ||
                    (g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].d.hero.d.heroId ==
                         HERO_ID_SAM &&
                     mission != MISSION_CIRITH_UNGOL_AMBUSH &&
                     mission != MISSION_CIRITH_UNGOL_PARAPET && mission != MISSION_ORC_TOWER &&
                     mission != MISSION_ORC_TOWER_TOP_ROOM))
                {
                    sfx_play(SFX_MENU_ERROR);
                }
                else
                {
                    scene_saveLoadMenu_loadSlot(0);
                    if (PLAYER(0).statusFlags.p & PLAYER_STATUS_FLAG_SAM)
                    {
                        sfx_play(SFX_MENU_ERROR);
                    }
                    else
                    {
                        g_PlayerGlobals.activePlayerCount = 1;
                        player_applyOptionFlags(ACTIVE_PLAYER_INDEX);
                        game_requestSceneChangeWithArgs(SCENE_ID_GAMEPLAY, 5, mission);
                        sfx_play(SFX_MENU_CONFIRM);
                    }
                }
            }
            else if (SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_BONUS_LOAD)
            {
                scene_saveLoadMenu_loadSlot(0);
                player_applyOptionFlags(ACTIVE_PLAYER_INDEX);
                g_PlayerGlobals.activePlayerCount = 1;
                game_requestSceneChange(SCENE_ID_BONUS_MAP_MENU);
                sfx_play(SFX_MENU_CONFIRM);
            }
            else
            {
                if (multiplayer_getMultiPlayerId() == 0)
                {
                    scene_saveLoadMenu_loadSlot(0);
                }
                else
                {
                    scene_saveLoadMenu_loadSlot(1);
                }
                SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_COOP_SLOT_EXCHANGE;
                // msgArgs.words[1]/[2] uninitialized: queued as-is (original-game bug)
                msgArgs.words[0] = SCENE_ARG_SAVE_LOAD_SELECTED_SLOT;
                multiplayer_queueMessage(MP_MSG_SAVE_SLOT_BEGIN, msgArgs);
                sfx_play(SFX_MENU_CONFIRM);
            }
        }
        else if (SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_COOP_LOAD ||
                 SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_BONUS_LOAD)
        {
            SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_HERO_SELECT;
            SCENE_ARG_SAVE_LOAD_CURSOR = 0;
            scene_saveLoadMenu_drawSlot(SCENE_ARG_SAVE_LOAD_SELECTED_SLOT,
                                        SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
            scene_saveLoadMenu_refreshSlot(SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
            scene_saveLoadMenu_drawActionPanel();
        }
        else
        {
            sfx_play(SFX_MENU_ERROR);
        }
        break;

    case SAVE_LOAD_STATE_CONFIRM_OVERWRITE:
    case SAVE_LOAD_STATE_CONFIRM_ERASE:
        if (SCENE_ARG_SAVE_LOAD_CURSOR != 0)
        {
            if (SCENE_ARG_SAVE_LOAD_STATE == SAVE_LOAD_STATE_CONFIRM_OVERWRITE)
            {
                scene_saveLoadMenu_commitSlot(TRUE);
                SCENE_ARG_SAVE_LOAD_CURSOR = 0;
                sfx_play(SFX_MENU_CONFIRM);
            }
            else
            {
                scene_saveLoadMenu_commitSlot(FALSE);
                SCENE_ARG_SAVE_LOAD_CURSOR = 1;
                sfx_play(SFX_MENU_CONFIRM);
            }
            SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_RESULT;
            scene_saveLoadMenu_drawActionPanel();
        }
        else
        {
            SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_SLOT_SELECT;
            scene_saveLoadMenu_drawActionPanel();
            sfx_play(SFX_MENU_BACK);
        }
        break;

    case SAVE_LOAD_STATE_RESULT:
        SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_SLOT_SELECT;
        scene_saveLoadMenu_drawActionPanel();
        sfx_play(SFX_MENU_CONFIRM);
        break;

    case SAVE_LOAD_STATE_HERO_SELECT:
        scene_saveLoadMenu_createCharacter(ACTIVE_PLAYER_INDEX, SCENE_ARG_SAVE_LOAD_CURSOR);
        zero = 0;
        bios_cpuSet(&zero, &ACTIVE_PLAYER,
                    CPU_SET_SRC_FIXED | CPU_SET_32BIT | (sizeof(Player) / 4));
        if (g_GameFlags.p & GAME_FLAG_COOP_ACTIVE)
        {
            if (multiplayer_getMultiPlayerId() == 0)
            {
                scene_saveLoadMenu_loadSlot(0);
            }
            else
            {
                scene_saveLoadMenu_loadSlot(1);
            }
            msgArgs.words[0] = SCENE_ARG_SAVE_LOAD_SELECTED_SLOT;
            multiplayer_queueMessage(MP_MSG_SAVE_SLOT_BEGIN, msgArgs);
            SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_COOP_SLOT_EXCHANGE;
        }
        else
        {
            scene_saveLoadMenu_loadSlot(0);
            player_applyOptionFlags(ACTIVE_PLAYER_INDEX);
            g_PlayerGlobals.activePlayerCount = 1;
            game_requestSceneChange(SCENE_ID_BONUS_MAP_MENU);
        }
        sfx_play(SFX_MENU_CONFIRM);
        break;
    }
}

/**
 * B-button / Select handler for the save/load slot screen.
 *
 * Backs out of the current dialog state.  From slot select the exit depends on
 * SCENE_ARG_SAVE_LOAD_MODE: co-op queues MP_MSG_TO_MAIN_MENU, the victory save
 * returns to the Griptonite-logo cinematic, otherwise to g_MenuReturnScene.
 *
 * @romaddress 0x08030c00
 */
void scene_saveLoadMenu_onBack(void)
{
    MpMessageArgs msgArgs;

    switch (SCENE_ARG_SAVE_LOAD_STATE)
    {
    case SAVE_LOAD_STATE_SLOT_SELECT:
        if (SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_COOP_LOAD)
        {
            // msgArgs.words[1]/[2] uninitialized: queued as-is (original-game bug).
            msgArgs.words[0] = 0;
            multiplayer_queueMessage(MP_MSG_TO_MAIN_MENU, msgArgs);
        }
        else if (SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_SAVE_VICTORY)
        {
            game_requestSceneChange(SCENE_ID_GRIPTONITE_LOGO);
        }
        else
        {
            game_requestSceneChange(g_MenuReturnScene);
        }
        break;

    case SAVE_LOAD_STATE_CONFIRM_OVERWRITE:
    case SAVE_LOAD_STATE_CONFIRM_ERASE:
    case SAVE_LOAD_STATE_RESULT:
        SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_SLOT_SELECT;
        scene_saveLoadMenu_drawActionPanel();
        break;

    case SAVE_LOAD_STATE_HERO_SELECT:
        SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_SLOT_SELECT;
        scene_saveLoadMenu_drawSlot(SCENE_ARG_SAVE_LOAD_SELECTED_SLOT,
                                    SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
        scene_saveLoadMenu_refreshSlot(SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
        scene_saveLoadMenu_drawActionPanel();
        break;
    }
    sfx_play(SFX_MENU_BACK);
}

/** Draw a yes/no prompt: its text at the left, then the pair via drawYesNo. */
static inline void drawConfirmPrompt(s32 promptId)
{
    char *str = text_getString(promptId);

    g_SceneCurrent.args[SCENE_ARG_SAVE_LOAD_FREE_SLOT_INDEX] =
        font_drawString(0x1f, 8, 0x8a, str, 0);
    scene_saveLoadMenu_drawYesNo();
}

/**
 * Redraw the bottom action row for the current state (Load/Save + Delete, a yes/no
 * prompt, or the result text) and show the button sprites only in slot select.
 *
 * @romaddress 0x08030c80
 */
void scene_saveLoadMenu_drawActionPanel(void)
{
    char *str;
    s32 id;

    gfx_fillBgRect(1, 0, 0x11, 0x1e, 2, 0);
    font_setStyle(FONT_STYLE_11PX_B, 3, 0);
    switch (SCENE_ARG_SAVE_LOAD_STATE)
    {
    case SAVE_LOAD_STATE_SLOT_SELECT:
        if (SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_SAVE ||
            SCENE_ARG_SAVE_LOAD_MODE == SAVE_LOAD_MODE_SAVE_VICTORY)
        {
            id = TEXT_ID_SAVE;
        }
        else
        {
            id = TEXT_ID_LOAD;
        }
        str = text_getString(id);
        font_drawString(0x1f, 0x38, 0x8a, str, 0);
        str = text_getString(TEXT_ID_DELETE);
        font_drawString(0x3d, 0xb0, 0x8a, str, 0);
        break;

    case SAVE_LOAD_STATE_CONFIRM_OVERWRITE:
        drawConfirmPrompt(TEXT_ID_OVERWRITE_THIS_SAVED_GAME);
        break;

    case SAVE_LOAD_STATE_CONFIRM_ERASE:
        drawConfirmPrompt(TEXT_ID_DELETE_THIS_SAVED_GAME);
        break;

    case SAVE_LOAD_STATE_RESULT:
        str = text_getString(TEXT_ID_SAVE_LOAD_RESULT_BASE + SCENE_ARG_SAVE_LOAD_CURSOR);
        font_drawString(0x1f, 0x78, 0x8a, str, 1);
        break;
    }
    if (SCENE_ARG_SAVE_LOAD_STATE == SAVE_LOAD_STATE_SLOT_SELECT)
    {
        g_SaveLoadActionButtonObj->flags.p |= ACTOR_FLAG_RENDER;
        g_SaveLoadDeleteButtonObj->flags.p |= ACTOR_FLAG_RENDER;
    }
    else
    {
        g_SaveLoadActionButtonObj->flags.p &= ~ACTOR_FLAG_RENDER;
        g_SaveLoadDeleteButtonObj->flags.p &= ~ACTOR_FLAG_RENDER;
    }
}

/**
 * Draw one slot panel: hero, region, mission and level of an occupied slot; "Empty";
 * or, in hero select, the hero under the cursor. The selected slot draws highlighted.
 *
 * @romaddress 0x08030da0
 */
void scene_saveLoadMenu_drawSlot(u8 slot, u8 selectedSlot)
{
    char levelText[16];
    s32 color;
    s32 tile;
    u32 x;
    u32 y;
    SaveSlot *save;
    u8 mission;
    char *str;
    u32 column;
    u32 regionId;

    color = 12;
    if (slot == selectedSlot)
    {
        color = 6;
    }
    font_setStyle(FONT_STYLE_8PX_A, color, -1);
    tile = slot * 60 + 0x5b;
    column = slot & 1;
    x = 0x44;
    if (column)
    {
        x = 0xac;
    }
    y = 0x1b;
    if (slot > 1)
    {
        y = 0x53;
    }
    gfx_fillBgRect(1, (x >> 3) - 4, y >> 3, 10, 5, 0);
    save = &g_SaveSlots[slot];
    mission = save_getSlotMission(save);
    if (mission < SLOT_MISSION_EMPTY)
    {
        str = text_getString(TEXT_ID_HERO_NAME_BASE + save->d.hero.d.heroId);
        tile = font_drawString(tile, x, y, str, 1);
        regionId = mission_getRegion(mission);
        y += 8;
        str = text_getString(TEXT_ID_REGION_NAME_BASE + regionId);
        tile = font_drawString(tile, x, y, str, 1);
        y += 8;
        str = text_getString(TEXT_ID_MISSION_NAME_BASE + mission);
        tile = font_drawString(tile, x, y, str, 1);
        y += 8;
        str = text_formatInt(save->d.level + 1, levelText);
        font_copyStringToSlot0(str);
        str = text_getString(TEXT_ID_LEVEL_1);
        font_drawString(tile, x, y, str, 1);
    }
    else if (SCENE_ARG_SAVE_LOAD_STATE == SAVE_LOAD_STATE_HERO_SELECT)
    {
        str = text_getString(TEXT_ID_SELECT_HERO);
        tile = font_drawString(tile, x, y, str, 1);
        y += 8;
        str = text_getString(TEXT_ID_HERO_NAME_BASE + SCENE_ARG_SAVE_LOAD_CURSOR);
        font_drawString(tile, x, y, str, 1);
    }
    else
    {
        str = text_getString(TEXT_ID_EMPTY);
        font_drawString(tile, x, y, str, 1);
    }
}

/**
 * Commit the highlighted slot: save the current game into it (save = TRUE) or delete
 * it (save = FALSE), then persist the global header and set g_LastSaveSlot. Deleting
 * the last remaining save also wipes the artifact mask and unlock flags.
 *
 * @romaddress 0x08030f08
 */
void scene_saveLoadMenu_commitSlot(bool save)
{
    u8 slot;

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
    if (!save)
    {
        for (slot = 0; slot < SAVE_SLOT_COUNT; slot++)
        {
            if (save_getSlotMission(&g_SaveSlots[slot]) < SLOT_MISSION_EMPTY)
            {
                break;
            }
        }
        if (slot == SAVE_SLOT_COUNT)
        {
            g_SaveGlobals.d.artifactMask.p = 0;
            g_SaveGlobals.d.unlockFlags.p = 0;
        }
    }
    save_writeGlobalHeader();
    sfx_enableAudio();
    scene_saveLoadMenu_drawSlot(SCENE_ARG_SAVE_LOAD_SELECTED_SLOT,
                                SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
    scene_saveLoadMenu_refreshSlot(SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
    g_LastSaveSlot = SCENE_ARG_SAVE_LOAD_SELECTED_SLOT;
}

/**
 * Create hero heroId as a fresh player and write it into the selected empty slot.
 *
 * @romaddress 0x08030ff4
 */
void scene_saveLoadMenu_createCharacter(u8 playerIndex, u8 heroId)
{
    PLAYER(playerIndex).heroId = heroId;
    player_initHeroStats(PLAYER(playerIndex).heroId, playerIndex);
    player_initHeroLoadout(PLAYER(playerIndex).heroId, playerIndex);
    player_initItemsAndHollow(playerIndex);
    if (heroId == HERO_ID_SAM)
    {
        PLAYER(playerIndex).statusFlags.p = PLAYER_STATUS_FLAG_SAM;
    }
    sfx_disableAudio();
    save_serializePlayer(g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].halfwords, 0);
    scene_saveLoadMenu_stampSlot();
    save_writeSlot(&g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT],
                   SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
    sfx_enableAudio();
    scene_saveLoadMenu_drawSlot(SCENE_ARG_SAVE_LOAD_SELECTED_SLOT,
                                SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
    scene_saveLoadMenu_refreshSlot(SCENE_ARG_SAVE_LOAD_SELECTED_SLOT);
}

/**
 * Create the Load/Save and Delete button sprites.
 *
 * @romaddress 0x08031090
 */
void scene_saveLoadMenu_setupButtonSprites(void)
{
    g_SaveLoadActionButtonObj = actor_allocMain();
    sprite_setPosition(g_SaveLoadActionButtonObj, 0x28 << 16, 0x88 << 16);
    g_SaveLoadActionButtonObj->oam.objMode = 0;
    g_SaveLoadActionButtonObj->oam.colors256 = 0;
    g_SaveLoadActionButtonObj->oam.paletteNumber = 0xe;
    g_SaveLoadActionButtonObj->recordType = 0xe;
    g_SaveLoadActionButtonObj->flags.p |= ACTOR_FLAG_RENDER;
    sprite_setAnimation(g_SaveLoadActionButtonObj, &SaveLoadActionButtonAnimation);

    g_SaveLoadDeleteButtonObj = actor_allocMain();
    sprite_setPosition(g_SaveLoadDeleteButtonObj, 0x98 << 16, 0x88 << 16);
    g_SaveLoadDeleteButtonObj->oam.objMode = 0;
    g_SaveLoadDeleteButtonObj->oam.colors256 = 0;
    g_SaveLoadDeleteButtonObj->oam.paletteNumber = 0xf;
    g_SaveLoadDeleteButtonObj->recordType = 0xf;
    g_SaveLoadDeleteButtonObj->flags.p |= ACTOR_FLAG_RENDER;
    sprite_setAnimation(g_SaveLoadDeleteButtonObj, &SaveLoadDeleteButtonAnimation);
}

/**
 * Create the four slot hero sprites (g_SaveLoadSlotObjs), one OBJ palette each,
 * and refresh them from the slots.
 *
 * @romaddress 0x0803116c
 */
void scene_saveLoadMenu_setupSlotSprites(void)
{
    u32 slotIndex;

    for (slotIndex = 0; slotIndex < SAVE_SLOT_COUNT; slotIndex++)
    {
        g_SaveLoadSlotObjs[slotIndex] = actor_allocMain();
        sprite_setPosition(g_SaveLoadSlotObjs[slotIndex], SaveLoadSlotPositions[slotIndex].x << 16,
                           SaveLoadSlotPositions[slotIndex].y << 16);
        g_SaveLoadSlotObjs[slotIndex]->oam.objMode = 0;
        g_SaveLoadSlotObjs[slotIndex]->oam.colors256 = 0;
        g_SaveLoadSlotObjs[slotIndex]->oam.paletteNumber = slotIndex;
        g_SaveLoadSlotObjs[slotIndex]->recordType = slotIndex;
        g_SaveLoadSlotObjs[slotIndex]->flags.p = ACTOR_FLAG_ANIMATION_PLAYING;
        scene_saveLoadMenu_refreshSlot(slotIndex);
    }
}

/**
 * Refresh one slot's hero sprite: shown for an occupied slot (its hero) or in hero
 * select (the hero under the cursor), hidden otherwise; the selected slot uses the
 * highlighted animation. Also queues the hero's palette into the slot's OBJ palette.
 *
 * @romaddress 0x080311e4
 */
void scene_saveLoadMenu_refreshSlot(s32 slotIndex)
{
    u32 heroId = g_SaveSlots[slotIndex].d.hero.d.heroId;

    if (save_getSlotMission(&g_SaveSlots[slotIndex]) < SLOT_MISSION_EMPTY)
    {
        g_SaveLoadSlotObjs[slotIndex]->flags.p |= ACTOR_FLAG_RENDER;
    }
    else if (SCENE_ARG_SAVE_LOAD_STATE == SAVE_LOAD_STATE_HERO_SELECT)
    {
        g_SaveLoadSlotObjs[slotIndex]->flags.p |= ACTOR_FLAG_RENDER;
        heroId = SCENE_ARG_SAVE_LOAD_CURSOR;
    }
    else
    {
        g_SaveLoadSlotObjs[slotIndex]->flags.p &= ~ACTOR_FLAG_RENDER;
        heroId = SAVE_LOAD_HERO_NONE;
    }
    if (heroId != SAVE_LOAD_HERO_NONE)
    {
        if (slotIndex == (s32)SCENE_ARG_SAVE_LOAD_SELECTED_SLOT)
        {
            sprite_setAnimation(g_SaveLoadSlotObjs[slotIndex],
                                SaveLoadHeroSelectedAnimationPtrs[heroId]);
        }
        else
        {
            sprite_setAnimation(g_SaveLoadSlotObjs[slotIndex], SaveLoadHeroAnimationPtrs[heroId]);
        }
        gfx_queuePaletteWrite(SaveLoadHeroPalettes[heroId], slotIndex << 4, 16);
    }
}

/**
 * Stamp the selected slot as a fresh game for its hero: start mission and variant,
 * spawn point 0, and a recomputed checksum.
 *
 * @romaddress 0x080312b8
 */
void scene_saveLoadMenu_stampSlot(void)
{
    u16 mission;

    mission = HeroStartMissions[g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].d.hero.d.heroId];
    g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].d.mission = mission;
    if (g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].d.hero.d.heroId != HERO_ID_SAM &&
        g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].d.hero.d.heroId != HERO_ID_SMEAGOL)
    {
        g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].d.variant =
            HeroStartVariants[g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].d.hero.d.heroId];
    }
    else
    {
        g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].d.variant = 0;
    }
    g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].d.spawnPoint = 0;
    g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].d.checksum = 0;
    g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].d.checksum = -save_checksum16(
        g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].halfwords, sizeof(SaveSlot));
}

/**
 * Mosaic out, flush the sprites, drop the palette cycles, and stop the sounds.
 *
 * @romaddress 0x08031368
 */
void scene_saveLoadMenu_exit(void)
{
    gfx_mosaicOutToBlack(GFX_BLEND_ALL_TARGETS);
    sprite_flushList(&g_ActorListMain);
    gfx_clearPaletteAnims();
    sfx_stopAllOnExit();
}

/**
 * Load the co-op peer's slot (received into g_MpPeerSaveSlot) into a player slot and
 * remember the selected slot as last-used.
 *
 * @romaddress 0x08031388
 */
void scene_saveLoadMenu_loadPeerSlot(u32 playerIndex)
{
    u8 player = playerIndex;

    save_deserializePlayer(g_MpPeerSaveSlot.words, player);
    save_loadSlotState(player);
    g_LastSaveSlot = SCENE_ARG_SAVE_LOAD_SELECTED_SLOT;
}

/**
 * R-button handler for the save/load slot screen.
 *
 * From slot select, triggers the erase-slot confirm dialog for occupied
 * slots.  From the result panel, returns to slot pick.
 *
 * @romaddress 0x080313b8
 */
void scene_saveLoadMenu_onErase(void)
{
    u8 mission;

    switch (SCENE_ARG_SAVE_LOAD_STATE)
    {
    case SAVE_LOAD_STATE_SLOT_SELECT:
        mission = save_getSlotMission(&g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT]);
        if (mission < SLOT_MISSION_EMPTY)
        {
            SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_CONFIRM_ERASE;
            SCENE_ARG_SAVE_LOAD_CURSOR = 0;
            scene_saveLoadMenu_drawActionPanel();
            sfx_play(SFX_MENU_CONFIRM);
        }
        else
        {
            sfx_play(SFX_MENU_ERROR);
        }
        break;

    case SAVE_LOAD_STATE_RESULT:
        SCENE_ARG_SAVE_LOAD_STATE = SAVE_LOAD_STATE_SLOT_SELECT;
        scene_saveLoadMenu_drawActionPanel();
        sfx_play(SFX_MENU_BACK);
        break;
    }
}

/**
 * Redraw the yes/no confirmation prompt texts (IDs 0x4e / 0x4f) with colors
 * swapped by the cursor position.  The "yes" text (0x4e) is drawn at
 * (0xa8, 0x8a) in the active color, the "no" text (0x4f) one step right in
 * the inactive color, and the two tile ranges chain via the return value of
 * font_drawString.
 *
 * @romaddress 0x08031418
 */
void scene_saveLoadMenu_drawYesNo(void)
{
    u8 x = 0xa8;
    s32 colorActive, colorInactive;
    u8 tileBase;
    u8 nextTile;

    if (SCENE_ARG_SAVE_LOAD_CURSOR != 0)
    {
        colorActive = 6;
        colorInactive = 3;
    }
    else
    {
        colorActive = 3;
        colorInactive = 6;
    }

    font_setStyle(FONT_STYLE_11PX_B, colorActive, 0);
    tileBase = SCENE_ARG_SAVE_LOAD_FREE_SLOT;
    nextTile = font_drawString(tileBase, x, 0x8a, text_getString(TEXT_ID_YES), 0);
    font_setStyle(FONT_STYLE_11PX_B, colorInactive, 0);
    x += 0x20;
    font_drawString(nextTile, x, 0x8a, text_getString(TEXT_ID_NO), 0);
}

/**
 * Load a player's data from the currently selected save slot: deserialize
 * into the Player struct, apply slot-derived state, and remember the slot
 * index in g_LastSaveSlot.
 *
 * @param playerIndex which player (0 or 1) to load into
 *
 * @romaddress 0x08031490
 */
void scene_saveLoadMenu_loadSlot(u32 playerIndex)
{
    u8 player = playerIndex;

    save_deserializePlayer(g_SaveSlots[SCENE_ARG_SAVE_LOAD_SELECTED_SLOT].words, player);
    save_loadSlotState(player);
    g_LastSaveSlot = SCENE_ARG_SAVE_LOAD_SELECTED_SLOT;
}
