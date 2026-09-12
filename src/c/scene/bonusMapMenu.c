#include "scene/bonusMapMenu.h"
#include "actor.h"
#include "font.h"
#include "game.h"
#include "gba_io.h"
#include "gfx.h"
#include "input.h"
#include "match_hacks.h"
#include "menu.h"
#include "mission.h"
#include "player.h"
#include "save.h"
#include "scene.h"
#include "scene/gameplay.h"
#include "sfx.h"
#include "sprite.h"
#include "text.h"
#include "variables.h"

/**
 * Init handler for the bonus map selection scene: BGs, mission/cursor sprites, and the
 * side-panel text (title, entry label, sub-entries, difficulty rows).
 *
 * @romaddress 0x08012f90
 */
void scene_bonusMapMenu_enter(void)
{
    u8 rowIndex;
    u8 count;
    char *str;

    gfx_initDisplay(0);
    gfx_enableDisplayFlags(GFX_DISPCNT_OBJ);
    gfx_setupBg(0, BonusMapMenuBgCnt0);
    gfx_setBgOffset(0, 0);
    gfx_loadBgAsset(0, BonusMapMenuBg0Asset, 1, 0, 0, 0);
    scene_bonusMapMenu_setupSprites();
    gfx_setupBg(2, BonusMapMenuBgCnt2);
    gfx_loadPalette(UiPalette, 0);
    gfx_setBgOffset(2, 0);
    SCENE_ARG_BONUS_MAP_STATE = BONUS_MAP_STATE_MISSION_SELECT;
    SCENE_ARG_BONUS_MAP_CURSOR = 0;
    SCENE_ARG_BONUS_MAP_SUB_ENTRY = 0;
    SCENE_ARG_BONUS_MAP_DIFFICULTY = 0;
    scene_bonusMapMenu_setupCursor();
    font_setRenderCtxBg(BonusMapMenuBgCnt2);
    font_setStyle(FONT_STYLE_11PX_A, 3, 0);
    str = text_getString(TEXT_ID_CHOOSE_YOUR_PATH);
    font_drawString(1, 0x78, 4, str, 1);
    scene_bonusMapMenu_drawEntryLabel();
    count = scene_bonusMapMenu_subEntryCount();
    for (rowIndex = 0; rowIndex < count; rowIndex++)
    {
        scene_bonusMapMenu_drawSubEntry(rowIndex, (u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY);
    }
    for (rowIndex = 0; rowIndex < 3; rowIndex++)
    {
        scene_bonusMapMenu_drawDifficultyRow(rowIndex, (u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY);
    }
    gfx_registerPaletteCycles(BonusMapMenuPaletteCycles);
    actor_tickAll();
    sprite_endFrame();
    gfx_fadeInFromBlack(GFX_BLEND_ALL_TARGETS);
}

/**
 * Per-frame update for the bonus map selection scene.
 * Launch-armed state: transition to gameplay on the selected mission.
 * Otherwise: A confirms, d-pad moves the cursor, B/select backs out.
 *
 * @romaddress 0x08013084
 */
void scene_bonusMapMenu_tick(void)
{
    if (SCENE_ARG_BONUS_MAP_STATE == BONUS_MAP_STATE_LAUNCH_MISSION)
    {
        game_requestSceneChangeWithArgs(
            SCENE_ID_GAMEPLAY, GAMEPLAY_MODE_START_HEALED,
            BonusMissions[SCENE_ARG_BONUS_MAP_CURSOR][SCENE_ARG_BONUS_MAP_SUB_ENTRY].missionId);
        // handed to the gameplay scene as its args[3]
        g_SceneNext.args[3] =
            BonusMissions[SCENE_ARG_BONUS_MAP_CURSOR][SCENE_ARG_BONUS_MAP_SUB_ENTRY].field_0x2;
    }
    // ROM asymmetry: A/d-pad read player 0's keys, B/select read the active player's.
    else if ((PLAYER_KEYS_PRESSED(0) & A_BUTTON) != 0)
    {
        scene_bonusMapMenu_onConfirm();
    }
    else if ((PLAYER_KEYS_PRESSED(0) & DPAD_ANY) != 0)
    {
        scene_bonusMapMenu_moveCursor();
    }
    else if ((PLAYER_KEYS_PRESSED(ACTIVE_PLAYER_INDEX) & B_BUTTON) != 0 ||
             (PLAYER_KEYS_PRESSED(ACTIVE_PLAYER_INDEX) & SELECT_BUTTON) != 0)
    {
        scene_bonusMapMenu_onBack();
    }
    actor_tickAll();
    sprite_endFrame();
}

/**
 * Allocate the four mission-icon sprites at their select-screen positions.
 * Locked missions get the grey locked-template animation/palette (entry [BONUS_MAP_LOCKED]).
 *
 * @romaddress 0x0801312c
 */
void scene_bonusMapMenu_setupSprites(void)
{
    s32 i;
    Actor *actor;

    for (i = BONUS_MAP_COUNT - 1; i >= 0; i--)
    {
        actor = actor_allocMain();
        sprite_setPosition(actor, (u32)BonusMapMenuEntries[i].iconX << 16,
                           (u32)BonusMapMenuEntries[i].iconY << 16);
        actor->flags.p = ACTOR_FLAG_RENDER;
        actor->oam.objMode = 0;
        actor->oam.colors256 = 0;
        actor->oam.bgPriority = 1;
        actor->recordType = 0;
        if (((u32)g_SaveGlobals.d.unlockFlags.p & BonusMapMenuEntries[i].unlockMask) != 0)
        {
            actor->oam.paletteNumber = (u8)BonusMapMenuEntries[i].paletteNumber;
            sprite_setAnimation(actor, &BonusMapMenuEntries[i].animation);
        }
        else
        {
            actor->oam.paletteNumber = (u8)BonusMapMenuEntries[BONUS_MAP_LOCKED].paletteNumber;
            sprite_setAnimation(actor, &BonusMapMenuEntries[BONUS_MAP_LOCKED].animation);
        }
    }
}

/**
 * Confirm-button handler for the bonus map selection scene.
 * Mission select: enter the sub-entry selector (error sfx if the mission is locked).
 * Sub-entry select: advance to the difficulty selector.
 * Difficulty select: store the chosen difficulty in the save's option flags and
 * arm the mission launch (SCENE_ARG_BONUS_MAP_STATE_LAUNCH_MISSION; Update acts on it).
 *
 * @romaddress 0x08013208
 */
void scene_bonusMapMenu_onConfirm(void)
{
    switch (SCENE_ARG_BONUS_MAP_STATE)
    {
    case BONUS_MAP_STATE_MISSION_SELECT:
        if (((u32)g_SaveGlobals.d.unlockFlags.p &
             BonusMapMenuEntries[SCENE_ARG_BONUS_MAP_CURSOR].unlockMask) != 0)
        {
            SCENE_ARG_BONUS_MAP_STATE = BONUS_MAP_STATE_SUB_ENTRY_SELECT;
            scene_bonusMapMenu_drawSubEntry((u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY,
                                            (u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY);
        }
        else
        {
            sfx_play(SFX_MENU_ERROR);
        }
        break;
    case BONUS_MAP_STATE_SUB_ENTRY_SELECT:
        SCENE_ARG_BONUS_MAP_STATE = BONUS_MAP_STATE_DIFFICULTY_SELECT;
        scene_bonusMapMenu_drawEntryLabel();
        scene_bonusMapMenu_drawDifficultyRow((u8)SCENE_ARG_BONUS_MAP_DIFFICULTY,
                                             (u8)SCENE_ARG_BONUS_MAP_DIFFICULTY);
        break;
    case BONUS_MAP_STATE_DIFFICULTY_SELECT:
        if (SCENE_ARG_BONUS_MAP_DIFFICULTY == OPTIONS_DIFFICULTY_GRUELING)
        {
            g_SaveGlobals.d.optionsFlags.p |= OPTION_FLAG_DIFFICULTY_GRUELING;
            g_SaveGlobals.d.optionsFlags.p &= ~OPTION_FLAG_DIFFICULTY_HARD;
        }
        else if (SCENE_ARG_BONUS_MAP_DIFFICULTY == OPTIONS_DIFFICULTY_HARD)
        {
            g_SaveGlobals.d.optionsFlags.p |= OPTION_FLAG_DIFFICULTY_HARD;
            g_SaveGlobals.d.optionsFlags.p &= ~OPTION_FLAG_DIFFICULTY_GRUELING;
        }
        else
        {
            g_SaveGlobals.d.optionsFlags.p &=
                ~(OPTION_FLAG_DIFFICULTY_HARD | OPTION_FLAG_DIFFICULTY_GRUELING);
        }
        SCENE_ARG_BONUS_MAP_STATE = BONUS_MAP_STATE_LAUNCH_MISSION;
        break;
    }
    sfx_play(SFX_MENU_CONFIRM);
}

/**
 * D-pad handler for the bonus map selection scene: move the active selector's cursor
 * (mission / sub-entry / difficulty) and redraw the affected rows.
 *
 * @romaddress 0x080132c4
 */
void scene_bonusMapMenu_moveCursor(void)
{
    u8 oldIndex;
    u8 count;
    u8 rowIndex;
    bool moved = FALSE;

    switch (SCENE_ARG_BONUS_MAP_STATE)
    {
    case BONUS_MAP_STATE_MISSION_SELECT:
        oldIndex = (u8)SCENE_ARG_BONUS_MAP_CURSOR;
        SCENE_ARG_BONUS_MAP_CURSOR = menu_movePlayerCursorHorizontal(&SCENE_ARG_BONUS_MAP_CURSOR, 0,
                                                                     BONUS_MAP_COUNT - 1, TRUE, 0);
        if (oldIndex != SCENE_ARG_BONUS_MAP_CURSOR)
        {
            moved = TRUE;
        }
        sprite_setPosition(g_BonusMapCursorObj,
                           (u32)BonusMapMenuEntries[SCENE_ARG_BONUS_MAP_CURSOR].iconX << 16,
                           (u32)BonusMapMenuEntries[SCENE_ARG_BONUS_MAP_CURSOR].iconY << 16);
        gfx_fillBgRect(2, 0xc, 3, 0x14, 7, 0);
        count = scene_bonusMapMenu_subEntryCount();
        scene_bonusMapMenu_drawEntryLabel();
        for (rowIndex = 0; rowIndex < count; rowIndex++)
        {
            scene_bonusMapMenu_drawSubEntry(rowIndex, (u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY);
        }
        SCENE_ARG_BONUS_MAP_SUB_ENTRY = 0;
        break;
    case BONUS_MAP_STATE_SUB_ENTRY_SELECT:
        oldIndex = (u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY;
        count = scene_bonusMapMenu_subEntryCount();
        SCENE_ARG_BONUS_MAP_SUB_ENTRY =
            menu_movePlayerCursorVertical(&SCENE_ARG_BONUS_MAP_SUB_ENTRY, 0, count - 1, TRUE, 0);
        if (oldIndex != SCENE_ARG_BONUS_MAP_SUB_ENTRY)
        {
            moved = TRUE;
        }
        scene_bonusMapMenu_drawSubEntry(oldIndex, (u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY);
        scene_bonusMapMenu_drawSubEntry((u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY,
                                        (u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY);
        break;
    case BONUS_MAP_STATE_DIFFICULTY_SELECT:
        oldIndex = (u8)SCENE_ARG_BONUS_MAP_DIFFICULTY;
        SCENE_ARG_BONUS_MAP_DIFFICULTY = menu_movePlayerCursorVertical(
            &SCENE_ARG_BONUS_MAP_DIFFICULTY, 0, OPTIONS_DIFFICULTY_GRUELING, TRUE, 0);
        if (oldIndex != SCENE_ARG_BONUS_MAP_DIFFICULTY)
        {
            moved = TRUE;
        }
        scene_bonusMapMenu_drawDifficultyRow(oldIndex, (u8)SCENE_ARG_BONUS_MAP_DIFFICULTY);
        scene_bonusMapMenu_drawDifficultyRow((u8)SCENE_ARG_BONUS_MAP_DIFFICULTY,
                                             (u8)SCENE_ARG_BONUS_MAP_DIFFICULTY);
        break;
    }
    if (moved)
    {
        sfx_play(SFX_MENU_CURSOR_MOVE);
    }
}

/**
 * Allocate and position the cursor sprite at the current mission's icon position.
 *
 * @romaddress 0x080133d8
 */
void scene_bonusMapMenu_setupCursor(void)
{
    Actor *cursor;

    cursor = actor_allocMain();
    g_BonusMapCursorObj = cursor;
    sprite_setPosition(g_BonusMapCursorObj,
                       (u32)BonusMapMenuEntries[SCENE_ARG_BONUS_MAP_CURSOR].iconX << 16,
                       (u32)BonusMapMenuEntries[SCENE_ARG_BONUS_MAP_CURSOR].iconY << 16);
    g_BonusMapCursorObj->flags.p = ACTOR_FLAG_RENDER | ACTOR_FLAG_ANIMATION_PLAYING;
    g_BonusMapCursorObj->oam.objMode = 0;
    g_BonusMapCursorObj->oam.colors256 = 0;
    g_BonusMapCursorObj->oam.paletteNumber = 0;
    g_BonusMapCursorObj->recordType = 0;
    sprite_setAnimation(g_BonusMapCursorObj, &BonusMapCursorAnimation);
}

/**
 * Exit handler for the bonus map selection scene.
 *
 * @romaddress 0x08013450
 */
void scene_bonusMapMenu_exit(void)
{
    gfx_mosaicOutToBlack(GFX_BLEND_ALL_TARGETS);
    sprite_flushList(&g_ActorListMain);
    gfx_commitBgs();
    gfx_clearPaletteAnims();
    sfx_stopAllOnExit();
}

/**
 * Back-button handler for the bonus map selection scene.
 * From the sub-entry or difficulty selector steps back one state and redraws;
 * from the top level returns to the main menu.
 *
 * @romaddress 0x08013474
 */
void scene_bonusMapMenu_onBack(void)
{
    switch (SCENE_ARG_BONUS_MAP_STATE)
    {
    case BONUS_MAP_STATE_MISSION_SELECT:
        game_requestSceneChange(SCENE_ID_MAIN_MENU);
        break;
    case BONUS_MAP_STATE_SUB_ENTRY_SELECT:
        SCENE_ARG_BONUS_MAP_STATE = BONUS_MAP_STATE_MISSION_SELECT;
        scene_bonusMapMenu_drawEntryLabel();
        scene_bonusMapMenu_drawSubEntry((u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY,
                                        (u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY);
        break;
    case BONUS_MAP_STATE_DIFFICULTY_SELECT:
        SCENE_ARG_BONUS_MAP_STATE = BONUS_MAP_STATE_SUB_ENTRY_SELECT;
        scene_bonusMapMenu_drawEntryLabel();
        scene_bonusMapMenu_drawDifficultyRow((u8)SCENE_ARG_BONUS_MAP_DIFFICULTY,
                                             (u8)SCENE_ARG_BONUS_MAP_DIFFICULTY);
        break;
    }
    sfx_play(SFX_MENU_BACK);
}

/**
 * Draw the current mission's name or "Locked!" label on the side panel.
 *
 * @romaddress 0x080134c4
 */
void scene_bonusMapMenu_drawEntryLabel(void)
{
    u16 textId;
    char *str;

    if (((u32)g_SaveGlobals.d.unlockFlags.p &
         BonusMapMenuEntries[SCENE_ARG_BONUS_MAP_CURSOR].unlockMask) != 0)
    {
        textId = BonusMapMenuEntries[SCENE_ARG_BONUS_MAP_CURSOR].nameTextId;
    }
    else
    {
        textId = BonusMapMenuEntries[BONUS_MAP_LOCKED].nameTextId;
    }

    str = text_getString(textId);
    font_setStyle(FONT_STYLE_8PX_B, BONUS_MAP_TEXT_COLOR_NORMAL, 0);
    font_drawString(0x51, BONUS_MAP_PANEL_TEXT_X, 0x18, str, 1);
}

/**
 * Draw one sub-entry row (the mission's map name) on the side panel.
 * Drawn only while the current mission is unlocked.
 *
 * @param rowIndex       which row
 * @param selectedIndex  the currently selected row (highlight color when == rowIndex
 *                       AND SCENE_ARG_BONUS_MAP_STATE == BONUS_MAP_STATE_SUB_ENTRY_SELECT)
 *
 * @romaddress 0x08013528
 */
void scene_bonusMapMenu_drawSubEntry(u8 rowIndex, u8 selectedIndex)
{
    s32 color = BONUS_MAP_TEXT_COLOR_NORMAL;
    s32 tileBase;
    s32 y;
    u32 regionId;
    char *str;

    if (rowIndex == selectedIndex && SCENE_ARG_BONUS_MAP_STATE == BONUS_MAP_STATE_SUB_ENTRY_SELECT)
    {
        color = BONUS_MAP_TEXT_COLOR_SELECTED;
    }

    tileBase = rowIndex * 0x14 + 0x92;
    y = rowIndex * 8 + 0x28;
    regionId = mission_getRegion(BonusMissions[SCENE_ARG_BONUS_MAP_CURSOR][rowIndex].missionId);
    if (((u32)g_SaveGlobals.d.unlockFlags.p &
         BonusMapMenuEntries[SCENE_ARG_BONUS_MAP_CURSOR].unlockMask) != 0)
    {
        str = text_getString(TEXT_ID_REGION_NAME_BASE + regionId);
        font_setStyle(FONT_STYLE_8PX_B, color, 0);
        font_drawString(tileBase, BONUS_MAP_PANEL_TEXT_X, y, str, 1);
    }
}

/**
 * Draw one difficulty row on the side panel. Texts 0x35/0x36/0x37 = Normal/Hard/Grueling.
 *
 * @param rowIndex       which row (0 = Normal, 1 = Hard, 2 = Grueling)
 * @param selectedIndex  the currently selected row (highlight color when == rowIndex
 *                       AND SCENE_ARG_BONUS_MAP_STATE == BONUS_MAP_STATE_DIFFICULTY_SELECT)
 *
 * @romaddress 0x080135c8
 */
void scene_bonusMapMenu_drawDifficultyRow(u8 rowIndex, u8 selectedIndex)
{
    s32 color = BONUS_MAP_TEXT_COLOR_NORMAL;
    s32 tileBase;
    s32 y;
    char *str;

    if (rowIndex == selectedIndex && SCENE_ARG_BONUS_MAP_STATE == BONUS_MAP_STATE_DIFFICULTY_SELECT)
    {
        color = BONUS_MAP_TEXT_COLOR_SELECTED;
    }

    tileBase = rowIndex * 0xf + 0x65;
    y = rowIndex * 8 + 0x68;
    str = text_getString(TEXT_ID_DIFFICULTY_NAME_BASE + rowIndex);
    font_setStyle(FONT_STYLE_8PX_B, color, 0);
    font_drawString(tileBase, BONUS_MAP_PANEL_TEXT_X, y, str, 1);
}

/**
 * Return the number of sub-entries for the current mission.
 * Constant 1: every BonusMissions list holds identical entries.
 *
 * @romaddress 0x0801361c
 */
u32 scene_bonusMapMenu_subEntryCount(void)
{
    return 1;
}
