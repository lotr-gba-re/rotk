#include "scene/optionsMenu.h"
#include "enemy.h"
#include "font.h"
#include "game.h"
#include "gba.h"
#include "gba_io.h"
#include "gfx.h"
#include "input.h"
#include "menu.h"
#include "player.h"
#include "save.h"
#include "scene.h"
#include "sfx.h"
#include "text.h"
#include "variables.h"

/**
 * Initialize the options menu (scene 0x06).
 *
 * If the previous scene was not the credits launcher (scene 0x13), snapshots the
 * current song id so the music can be resumed unchanged on exit.
 *
 * @romaddress 0x080368b0
 */
void scene_optionsMenu_enter(void)
{
    u32 bg3cnt;
    u8 row;

    if (g_ScenePrevious.id != SCENE_ID_CREDITS)
    {
        g_OptionsSongId = g_CurrentSongId;
    }
    sfx_requestSong(g_OptionsSongId);

    gfx_initDisplay(0);
    gfx_enableDisplayFlags(GFX_DISPCNT_OBJ);

    gfx_setupBg(0, OptionsBgCnt0);
    gfx_loadBgAsset(0, MenuBg0Asset, 0, 0, 0, 0);
    gfx_setupBg(1, OptionsBgCnt1);
    gfx_loadBgAsset(1, MenuBg1Asset, 0x1ad, 0, 0, 0);
    gfx_disableBg(1);
    gfx_setupBg(2, OptionsBgCnt2);
    gfx_loadBgAsset(2, OptionsBg2Asset, 1, 0, 0, 0);
    bg3cnt = OptionsBgCnt3;
    gfx_setupBg(3, bg3cnt);
    gfx_loadPalette(UiPalette, 0);
    gfx_setBgOffset(3, 0);
    gfx_setBgScrollSpeedX(1, 0x8000);
    font_setRenderCtxBg(bg3cnt);

    font_setStyle(FONT_STYLE_11PX_A, 3, -1);
    font_drawString(0xe2, 0x70, 0xc, text_getString(TEXT_ID_OPTIONS_TITLE), 1);

    gfx_registerPaletteCycles(OptionsPaletteCycles);

    SCENE_ARG_OPTIONS_CURSOR = 0;

    g_OptionValues[OPTIONS_ROW_QUICK_SKILL_SELECT] =
        (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_QUICK_SKILL_SELECT) >> 3;
    g_OptionValues[OPTIONS_ROW_AUTO_HEALING] =
        (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_AUTO_HEALING) >> 4;
    g_OptionValues[OPTIONS_ROW_MUSIC_VOLUME] =
        bios_div(g_SaveGlobals.d.musicVolume, OPTIONS_MUSIC_VOLUME_STEP);
    g_OptionValues[OPTIONS_ROW_SFX_VOLUME] =
        bios_div(g_SaveGlobals.d.sfxVolume, OPTIONS_SFX_VOLUME_STEP);
    g_OptionValues[OPTIONS_ROW_FAST_DISCARD] =
        g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_FAST_DISCARD;
    if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_GRUELING)
    {
        g_OptionValues[OPTIONS_ROW_DIFFICULTY] = OPTIONS_DIFFICULTY_GRUELING;
    }
    else if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_HARD)
    {
        g_OptionValues[OPTIONS_ROW_DIFFICULTY] = OPTIONS_DIFFICULTY_HARD;
    }
    else
    {
        g_OptionValues[OPTIONS_ROW_DIFFICULTY] = OPTIONS_DIFFICULTY_NORMAL;
    }

    for (row = 0; row < OPTIONS_ROW_COUNT; row++)
    {
        scene_optionsMenu_drawRow(row, SCENE_ARG_OPTIONS_CURSOR);
    }

    sprite_endFrame();
    gfx_fadeInFromBlack(0xd);
    gfx_setBlendControl(2, 1);
    gfx_setBlendAlpha(0, 0x10);
    gfx_unknown08012444(1);
    gfx_alphaBlendIn(2, 1, 6, 4);
}

/**
 * Per-frame update for the options menu: dispatch the active player's pressed keys.
 *
 * @romaddress 0x08036a74
 */
void scene_optionsMenu_tick(void)
{
    if ((ACTIVE_MENU_KEYS_PRESSED & A_BUTTON) != 0)
    {
        scene_optionsMenu_onConfirm();
    }
    else if ((ACTIVE_MENU_KEYS_PRESSED & (DPAD_UP | DPAD_DOWN)) != 0)
    {
        scene_optionsMenu_moveCursor();
    }
    else if ((ACTIVE_MENU_KEYS_PRESSED & (DPAD_LEFT | DPAD_RIGHT)) != 0)
    {
        scene_optionsMenu_adjustValue();
    }
    else if ((ACTIVE_MENU_KEYS_PRESSED & (B_BUTTON | SELECT_BUTTON)) != 0)
    {
        game_requestSceneChange(g_MenuReturnScene);
        sfx_play(SFX_MENU_CURSOR_MOVE);
        player_applyOptionFlags(ACTIVE_PLAYER_INDEX);
    }

    sprite_endFrame();
}

/**
 * Row text color: highlighted on the cursor row; the Difficulty row is greyed out
 * when difficulty selection is unavailable (the menu was not entered from the main
 * menu, or co-op is active).
 */
static inline s32 rowColor(s32 row, s32 cursor)
{
    s32 color;

    if ((row == OPTIONS_ROW_DIFFICULTY || cursor == OPTIONS_ROW_DIFFICULTY) &&
        (g_MenuReturnScene != SCENE_ID_MAIN_MENU || (g_GameFlags.p & GAME_FLAG_COOP_ACTIVE) != 0))
    {
        color = 6;
    }
    else
    {
        color = 3;
        if (row == cursor)
        {
            color = 0;
        }
    }
    return color;
}

/**
 * Draw one options-menu row: the row label in the left column and its current
 * value in the right column, then clear the tiles past the value text.
 *
 * @romaddress 0x08036ae8
 */
void scene_optionsMenu_drawRow(s32 row, s32 cursor)
{
    char *str;
    s32 color;
    s32 tileBase;
    s32 xPos;
    s32 rowY;
    s32 y;
    s32 width;
    char buf[16];

    color = rowColor(row, cursor);
    tileBase = row * 0x19 + 0xf6;
    rowY = row * 0x10;
    y = rowY + 0x20;

    // label in the left column
    str = text_getString(TEXT_ID_OPTIONS_ROW_LABEL_BASE + row);
    font_setStyle(FONT_STYLE_8PX_B, color, 0);
    tileBase = font_drawString(tileBase, 0x40, y, str, 0);

    // value in the right column, then clear the tiles past the value text
    xPos = 0x98;
    switch (row)
    {
    case OPTIONS_ROW_QUICK_SKILL_SELECT:
    case OPTIONS_ROW_AUTO_HEALING:
    case OPTIONS_ROW_FAST_DISCARD:
        str = text_getString(TEXT_ID_OFF + g_OptionValues[row]);
        break;

    case OPTIONS_ROW_MUSIC_VOLUME:
    case OPTIONS_ROW_SFX_VOLUME:
        if (g_OptionValues[row] != 0)
        {
            str = text_formatInt(g_OptionValues[row], buf);
        }
        else
        {
            str = text_getString(TEXT_ID_OFF);
        }
        break;

    case OPTIONS_ROW_DIFFICULTY:
        str = text_getString(TEXT_ID_DIFFICULTY_NAME_BASE + g_OptionValues[row]);
        break;

    default:
        str = NULL;
        break;
    }
    if (str != NULL)
    {
        font_drawString(tileBase, xPos, y, str, 0);
        width = font_getStringWidth(str);
        // cell past the end of the value text, rounded up to a whole cell
        xPos = (xPos + width + 7) / 8;
        y = y / 8;
        gfx_fillBgRect(3, xPos, y, 0x1e - xPos, 1, 0);
    }
}

/**
 * Toggle rows set/clear their optionsFlags bit from the row's 0/1 value.
 * Plain block: a do-while(0) wrapper would change agbcc's branch layout.
 */
#define COMMIT_OPTION_TOGGLE(flag)                                                                 \
    {                                                                                              \
        if (val != 0)                                                                              \
        {                                                                                          \
            g_SaveGlobals.d.optionsFlags.p |= (flag);                                              \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            g_SaveGlobals.d.optionsFlags.p &= ~(flag);                                             \
        }                                                                                          \
    }

/** Difficulty is a 3-way selector: set the picked bit, clear the other. */
#define COMMIT_DIFFICULTY(setFlag, clearFlag)                                                      \
    {                                                                                              \
        u32 flags = g_SaveGlobals.d.optionsFlags.p | (setFlag);                                    \
        g_SaveGlobals.d.optionsFlags.p = flags & ~(clearFlag);                                     \
    }

/**
 * Adjust the current cursor row's value left/right, commit the change to
 * g_SaveGlobals, and redraw the row. Rows past Difficulty (Language Select,
 * Credits) are actions, not values, and return immediately.
 *
 * @romaddress 0x08036c1c
 */
void scene_optionsMenu_adjustValue(void)
{
    u32 val;
    u8 oldSong;

    if (SCENE_ARG_OPTIONS_CURSOR > OPTIONS_ROW_DIFFICULTY)
    {
        return;
    }

    val = g_OptionValues[SCENE_ARG_OPTIONS_CURSOR];
    menu_moveCursorHorizontal(&val, 0, OptionMaxValues[SCENE_ARG_OPTIONS_CURSOR], TRUE,
                              ACTIVE_PLAYER_INDEX);
    g_OptionValues[SCENE_ARG_OPTIONS_CURSOR] = val;

    switch (SCENE_ARG_OPTIONS_CURSOR)
    {
    case OPTIONS_ROW_MUSIC_VOLUME:
        if (val == OPTIONS_VOLUME_MAX)
        {
            val = OPTIONS_MUSIC_VOLUME_MAX;
        }
        else
        {
            val *= OPTIONS_MUSIC_VOLUME_STEP;
        }
        if ((g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_MUSIC_ENABLED) && val == 0)
        {
            sfx_stopMusic();
            g_SaveGlobals.d.optionsFlags.p &= ~OPTION_FLAG_MUSIC_ENABLED;
        }
        else if (!(g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_MUSIC_ENABLED) && val != 0)
        {
            g_SaveGlobals.d.optionsFlags.p |= OPTION_FLAG_MUSIC_ENABLED;
            oldSong = g_CurrentSongId;
            g_CurrentSongId = SONG_ID_NONE;
            sfx_requestSong(oldSong);
            sfx_setMusicVolume(val);
        }
        else
        {
            sfx_setMusicVolume(val);
        }
        g_SaveGlobals.d.musicVolume = val;
        break;

    case OPTIONS_ROW_SFX_VOLUME:
        if (val == OPTIONS_VOLUME_MAX)
        {
            val = OPTIONS_SFX_VOLUME_MAX;
        }
        else
        {
            val *= OPTIONS_SFX_VOLUME_STEP;
        }
        if ((g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_SFX_ENABLED) && val == 0)
        {
            sfx_stopAll();
            g_SaveGlobals.d.optionsFlags.p &= ~OPTION_FLAG_SFX_ENABLED;
        }
        else if (!(g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_SFX_ENABLED) && val != 0)
        {
            g_SaveGlobals.d.optionsFlags.p |= OPTION_FLAG_SFX_ENABLED;
            sfx_setVolume(val);
        }
        else
        {
            sfx_setVolume(val);
        }
        g_SaveGlobals.d.sfxVolume = val;
        break;

    case OPTIONS_ROW_QUICK_SKILL_SELECT:
        COMMIT_OPTION_TOGGLE(OPTION_FLAG_QUICK_SKILL_SELECT);
        break;

    case OPTIONS_ROW_AUTO_HEALING:
        COMMIT_OPTION_TOGGLE(OPTION_FLAG_AUTO_HEALING);
        break;

    case OPTIONS_ROW_FAST_DISCARD:
        COMMIT_OPTION_TOGGLE(OPTION_FLAG_FAST_DISCARD);
        break;

    case OPTIONS_ROW_DIFFICULTY:
        if (val == OPTIONS_DIFFICULTY_GRUELING)
        {
            COMMIT_DIFFICULTY(OPTION_FLAG_DIFFICULTY_GRUELING, OPTION_FLAG_DIFFICULTY_HARD);
        }
        else if (val == OPTIONS_DIFFICULTY_HARD)
        {
            COMMIT_DIFFICULTY(OPTION_FLAG_DIFFICULTY_HARD, OPTION_FLAG_DIFFICULTY_GRUELING);
        }
        else
        {
            g_SaveGlobals.d.optionsFlags.p &=
                ~(OPTION_FLAG_DIFFICULTY_HARD | OPTION_FLAG_DIFFICULTY_GRUELING);
        }
        break;

    default:
        break;
    }

    scene_optionsMenu_drawRow(SCENE_ARG_OPTIONS_CURSOR, SCENE_ARG_OPTIONS_CURSOR);
    sfx_stop(g_OptionsSfxHandle);
    g_OptionsSfxHandle = sfx_play(SFX_MENU_CURSOR_MOVE);
}

/**
 * Move the options cursor up or down, wrapping between the first and last row.
 * The Difficulty row is skipped when the return scene is not the main menu or
 * co-op is active, requiring a second press to advance past it. Redraws the old
 * and new cursor rows and plays the cursor-move sound effect.
 *
 * @romaddress 0x08036e64
 */
void scene_optionsMenu_moveCursor(void)
{
    u32 old = SCENE_ARG_OPTIONS_CURSOR;

    SCENE_ARG_OPTIONS_CURSOR =
        menu_moveCursorVertical(&SCENE_ARG_OPTIONS_CURSOR, OPTIONS_ROW_MUSIC_VOLUME,
                                OPTIONS_ROW_CREDITS, TRUE, ACTIVE_PLAYER_INDEX);

    // Skip the difficulty row when the menu was not entered from the main menu
    // (or co-op is active) by processing the input a second time.
    if (SCENE_ARG_OPTIONS_CURSOR == OPTIONS_ROW_DIFFICULTY &&
        (g_MenuReturnScene != SCENE_ID_MAIN_MENU || (g_GameFlags.p & GAME_FLAG_COOP_ACTIVE) != 0))
    {
        SCENE_ARG_OPTIONS_CURSOR =
            menu_moveCursorVertical(&SCENE_ARG_OPTIONS_CURSOR, OPTIONS_ROW_MUSIC_VOLUME,
                                    OPTIONS_ROW_CREDITS, TRUE, ACTIVE_PLAYER_INDEX);
    }

    scene_optionsMenu_drawRow(old, SCENE_ARG_OPTIONS_CURSOR);
    scene_optionsMenu_drawRow(SCENE_ARG_OPTIONS_CURSOR, SCENE_ARG_OPTIONS_CURSOR);

    sfx_stop(g_OptionsSfxHandle);
    g_OptionsSfxHandle = sfx_play(SFX_MENU_CURSOR_MOVE);
}

/**
 * Exit handler for the options menu: fade out to black via alpha blend,
 * mosaic out, and stop all SFX. Runs once on a pending scene change.
 *
 * @romaddress 0x08036ef4
 */
void scene_optionsMenu_exit(void)
{
    gfx_alphaBlendOut(2, 1, 6, 4);
    gfx_disableBg(1);
    gfx_mosaicOutToBlack(GFX_BLEND_ALL_TARGETS);
    gfx_commitBgs();
    gfx_clearPaletteAnims();
    sfx_stopAllOnExit();
}

/**
 * A-button dispatch: Language Select and Credits request their scenes; the value
 * rows are a silent no-op.
 *
 * @romaddress 0x08036f20
 */
void scene_optionsMenu_onConfirm(void)
{
    bool played = FALSE;

    switch (SCENE_ARG_OPTIONS_CURSOR)
    {
    case OPTIONS_ROW_LANGUAGE_SELECT:
        game_requestSceneChange(SCENE_ID_LANGUAGE_SELECT);
        played = TRUE;
        break;

    case OPTIONS_ROW_CREDITS:
        game_requestSceneChange(SCENE_ID_GRIPTONITE_LOGO);
        played = TRUE;
        break;

    case OPTIONS_ROW_MUSIC_VOLUME:
    case OPTIONS_ROW_SFX_VOLUME:
    case OPTIONS_ROW_QUICK_SKILL_SELECT:
    case OPTIONS_ROW_AUTO_HEALING:
    case OPTIONS_ROW_FAST_DISCARD:
    case OPTIONS_ROW_DIFFICULTY:
        break;
    }

    if (!played)
    {
        return;
    }

    sfx_stop(g_OptionsSfxHandle);
    g_OptionsSfxHandle = sfx_play(SFX_MENU_CONFIRM);
}
