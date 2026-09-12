#include "scene/languageSelect.h"
#include "font.h"
#include "game.h"
#include "gfx.h"
#include "input.h"
#include "menu.h"
#include "save.h"
#include "scene.h"
#include "sfx.h"
#include "sprite.h"
#include "text.h"
#include "variables.h"

/**
 * Set up the language select screen (scene 0x12): cursor on the saved language, the two
 * background layers, and one text object per language.
 *
 * @romaddress 0x0802fae0
 */
void scene_languageSelect_enter(void)
{
    u32 entryIndex;

    SCENE_ARG_LANGUAGE_SELECT_CURSOR = g_SaveGlobals.d.language;
    gfx_initDisplay(0);
    sprite_resetTileAllocatorForMode(0);
    gfx_enableDisplayFlags(GFX_DISPCNT_OBJ);
    gfx_setupBg(3, MenuBackdropBgCnt);
    gfx_loadBgAsset(3, LanguageSelectBg3Asset, 0, 0, 0, 0);
    gfx_setupBg(2, MenuTextBgCnt);
    gfx_loadBgAsset(2, LanguageSelectBg2Asset, 0, 0, 0, 0);
    gfx_setBgScrollSpeedX(2, 0x10000);
    // the UI palette's 16 colors, past its 2-byte asset header
    gfx_loadObjPaletteColors((const u16 *)(UiPalette + 2), 0, 16);
    for (entryIndex = 0; entryIndex < LANGUAGE_COUNT; entryIndex++)
    {
        scene_languageSelect_drawEntry(entryIndex);
    }
    actor_tickAll();
    sprite_endFrame();
    if (g_ScenePrevious.id == SCENE_ID_NO_OP_0)
    {
        gfx_fadeInFromWhite(GFX_BLEND_ALL_TARGETS);
    }
    else
    {
        gfx_fadeInFromBlack(GFX_BLEND_ALL_TARGETS);
    }
}

/**
 * Per-frame update: dispatch the active player's pressed keys.
 *
 * @romaddress 0x0802fba0
 */
void scene_languageSelect_tick(void)
{
    if (ACTIVE_MENU_KEYS_PRESSED & A_BUTTON)
    {
        scene_languageSelect_onConfirm();
    }
    else if (ACTIVE_MENU_KEYS_PRESSED & B_BUTTON)
    {
        scene_languageSelect_onBack();
    }
    else if (ACTIVE_MENU_KEYS_PRESSED & SELECT_BUTTON)
    {
        scene_languageSelect_onBack();
    }
    else if (ACTIVE_MENU_KEYS_PRESSED & (DPAD_UP | DPAD_DOWN))
    {
        scene_languageSelect_moveCursor();
    }
    actor_tickAll();
    sprite_endFrame();
}

/**
 * Fade out, flush the sprites, and forget the language text objects.
 *
 * @romaddress 0x0802fbfc
 */
void scene_languageSelect_exit(void)
{
    if (g_ScenePrevious.id == SCENE_ID_NO_OP_0)
    {
        gfx_fadeOutToWhite(GFX_BLEND_ALL_TARGETS);
    }
    else
    {
        gfx_fadeOutToBlack(GFX_BLEND_ALL_TARGETS);
    }
    gfx_commitBgs();
    sprite_flushList(&g_ActorListMain);
    text_objClearSlots();
}

/**
 * Language chosen: apply it, stamp it into the save globals, and leave the scene.
 *
 * Entered from the boot stub, it writes out the global header (audio muted across the
 * EEPROM write) and continues boot into the copyright scene. Entered from the options
 * menu, it returns there.
 *
 * @romaddress 0x0802fc30
 */
void scene_languageSelect_onConfirm(void)
{
    text_setLanguage(SCENE_ARG_LANGUAGE_SELECT_CURSOR);
    save_stampLanguage();
    if (g_ScenePrevious.id == SCENE_ID_NO_OP_0)
    {
        sfx_disableAudio();
        save_writeGlobalHeader();
        sfx_enableAudio();
        game_requestSceneChange(SCENE_ID_COPYRIGHT);
    }
    else
    {
        // hands (1, old cursor) back, but scene_optionsMenu_enter overwrites both
        game_requestSceneChangeWithArgs(g_ScenePrevious.id, 1, g_ScenePrevious.args[1]);
    }
    sfx_play(SFX_MENU_CONFIRM);
}

/**
 * B/Select: back to the options menu when entered from there; ignored during boot.
 *
 * @romaddress 0x0802fc78
 */
void scene_languageSelect_onBack(void)
{
    if (g_ScenePrevious.id == SCENE_ID_OPTIONS_MENU)
    {
        game_requestSceneChange(SCENE_ID_OPTIONS_MENU);
        sfx_play(SFX_MENU_BACK);
    }
}

/**
 * D-pad up/down: move the cursor (wrapping) and redraw the two affected entries.
 *
 * @romaddress 0x0802fc98
 */
void scene_languageSelect_moveCursor(void)
{
    u32 oldIndex = SCENE_ARG_LANGUAGE_SELECT_CURSOR;

    menu_moveCursorVertical(&SCENE_ARG_LANGUAGE_SELECT_CURSOR, 0, LANGUAGE_COUNT - 1, TRUE,
                            ACTIVE_PLAYER_INDEX);
    scene_languageSelect_redrawEntry(oldIndex);
    scene_languageSelect_redrawEntry(SCENE_ARG_LANGUAGE_SELECT_CURSOR);
    sfx_play(SFX_MENU_CURSOR_MOVE);
}

/**
 * Create the text object for one language row, centered at x 0x78 in rows of 16 px from
 * y 0x20; the cursor row uses the bold style and sits one pixel higher.
 */
static inline void drawEntryInline(s32 entryIndex)
{
    FontStyle styleIndex;
    s32 color;
    u16 yOffset;
    char *str;
    Actor *obj;
    s32 y;

    if (entryIndex == (s32)SCENE_ARG_LANGUAGE_SELECT_CURSOR)
    {
        styleIndex = FONT_STYLE_11PX_A;
        color = 0;
        yOffset = -1;
    }
    else
    {
        styleIndex = FONT_STYLE_11PX_B;
        color = 3;
        yOffset = 0;
    }
    font_setStyle(styleIndex, color, 0);
    str = text_getString(TEXT_ID_LANGUAGE_NAME_BASE + entryIndex);
    text_objCreateInSlot(&str, entryIndex);
    obj = g_TextObjSlots[entryIndex];
    y = entryIndex * 16 + 0x20;
    // a u16 -1 adds 0xffff; the s16 cast folds it back to y - 1
    y += yOffset;
    text_objSetPos(obj, 0x78, (s16)y);
}

/**
 * Draw language row entryIndex into a fresh text object slot.
 *
 * @romaddress 0x0802fce0
 */
void scene_languageSelect_drawEntry(s32 entryIndex)
{
    drawEntryInline(entryIndex);
}

/**
 * Redraw language row entryIndex: retire its current text object and draw a new one.
 *
 * @romaddress 0x0802fd40
 */
void scene_languageSelect_redrawEntry(s32 entryIndex)
{
    g_TextObjSlots[entryIndex]->flags.p |=
        ACTOR_FLAG_PENDING_REMOVE | ACTOR_FLAG_ANIMATION_FRAME_CHANGED;
    drawEntryInline(entryIndex);
}
