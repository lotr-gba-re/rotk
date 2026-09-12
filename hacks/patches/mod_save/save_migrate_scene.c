// The save-migration confirmation scene (MOD_SCENE_ID(SAVE_MIGRATE), a row in the modcode's
// ModSceneHandlers table).
//
// Entered once from the boot chain, and classifies the save image itself.
// Every chip write - a migration or the fresh format for an empty/unrecognized
// chip - happens only after a double-confirmed Yes (cursor starts on No). The only ways
// out are a confirmed run or powering off, which leaves the image untouched.

#include "font.h"
#include "game.h"
#include "gba_io.h"
#include "gfx.h"
#include "input.h"
#include "lib/hex.h"
#include "memory.h"
#include "patches/mod_startup/mod_startup.h"
#include "save_layout.h"
#include "save_migrate.h"
#include "scene.h"
#include "scene/languageSelect.h"
#include "scene/mainMenu.h"
#include "sfx.h"
#include "sprite.h"
#include "variables.h"

// BG layers this scene drives.
#define BG_ART_BACK 0u
#define BG_TEXT 2u
#define BG_ART_FRONT 3u

// Text-BG tile partition (TEXT_BGCNT): title at tile 1, the body paragraph from
// BODY_TILE_BASE (font_drawLines packs its lines back to back), the prompt and its two
// option labels from CHOICE_TILE. font_rasterizeString spans (y&7 + height + 7)>>3 tile-rows
// per cell, so an 8px glyph on a non-multiple-of-8 y costs 2 tile-rows: keep every text row
// on an 8px boundary. Stride leaves ~2 tiles above the widest line (~26).
#define BODY_TILE_BASE 0x20u
#define LINE_TILES 0x1cu
#define CHOICE_TILE 0xC8u
/**
 * Longest line of the body copy; font_drawLine's own line buffer holds 63 chars, and the
 * literals are sized to fit the 30-cell screen at ~4.3px/char.
 */
#define LINE_MAX 46

// Band between the art's two red bands. Body blocks are centered in it (bodyTop).
#define CONTENT_TOP 16
#define CONTENT_BOTTOM 144
// 16px pitch (8px FONT_STYLE_8PX_B glyph + 8px gap) keeps tops on 8px boundaries; a skipped
// line index is a 32px paragraph gap.
#define BODY_PITCH 16
// The migratable screen fills the band with body + prompt + options, so its body is
// top-aligned rather than centered.
#define BODY_TOP_WITH_CHOICES 24
#define CHOICE_Y 120
#define OPTION_Y (CHOICE_Y + 16)

/** Every row is centered (font_drawString mode 1), so x values are label centers. */
#define CENTER_X 0x78

enum SaveMigratePhase
{
    PHASE_CONFIRM_1 = 0,
    PHASE_CONFIRM_2 = 1,
    PHASE_DONE = 2,
    PHASE_BLOCKED = 3,
};

// Scene state lives in g_SceneCurrent.args because the modcode has no RAM of its own
// (hack.ld forbids .data and .bss). args[0] holds the SaveMigrateInfo SceneInit allocates
// and SceneExit frees; game_requestSceneChange zeroed all five words on the way in.
#define SCENE_INFO ((SaveMigrateInfo *)g_SceneCurrent.args[0])
#define ARG_PHASE g_SceneCurrent.args[1]
#define ARG_CURSOR g_SceneCurrent.args[2]

#define CURSOR_NO 0
#define CURSOR_YES 1

// Entry index within UiPalette's bank 0: font_setStyle splits its colour argument into an
// entry index (low nibble) and a palette number (high nibble), and all UI text passes a zero
// high nibble. Entry 0 is the highlight the registered colour cycle animates.
#define COLOR_BRIGHT 0
#define COLOR_NORMAL 3

/**
 * Front-art layer cnt (language-select layer 2): block 1 is the only free charbase, 0x1e is the
 * asset's native screenbase, and priority 2 sits between the BG0 back art and the BG2 text.
 */
#define ART_FRONT_BGCNT (BGCNT_PRIORITY(2) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(0x1eu))

// Text layer cnt. Block 2 is unavailable as a charbase because the BG3 art reaches absolute
// tile 1024, whose data sits at that block's base. Block 3 holds the screenbases in its top
// half, leaving 320 tiles for text below them.
#define TEXT_BGCNT (BGCNT_PRIORITY(1) | BGCNT_CHARBASE(3) | BGCNT_SCREENBASE(0x1du))
// One 4bpp tile is 16 halfwords; the BG map is 32x32 cells.
#define TILE_HALFWORDS 16u
#define MAP_CELLS 1024u

/** Zero tile 0 (the transparent tile) and point every map cell at it. */
static void clearTextBg(void)
{
    volatile u16 zero = 0;

    DMA3_FILL16(&zero, BG_CHAR_ADDR(BGCNT_GET_CHARBASE(TEXT_BGCNT)), TILE_HALFWORDS);
    DMA3_FILL16(&zero, BG_SCREEN_ADDR(BGCNT_GET_SCREENBASE(TEXT_BGCNT)), MAP_CELLS);
}

/**
 * Blank a pixel band on the text layer so the cells behind show the art again (gfx_fillBgRect
 * + redraw, the stock idiom: see scene_mainMenu_clearPrompt). Bounds snap to the 8px cell
 * grid; `bottomPx` is exclusive.
 */
static void clearBand(s32 topPx, s32 bottomPx)
{
    s32 row = topPx / TILE_HEIGHT;

    // The BG map is 32 cells wide; only the first DISPLAY_TILE_WIDTH are onscreen.
    gfx_fillBgRect(BG_TEXT, 0, row, DISPLAY_TILE_WIDTH, bottomPx / TILE_HEIGHT - row, 0);
}

/** Lines in a paragraph: one per '\n' plus the last. */
static s32 lineCount(const char *str)
{
    s32 lines = 1;

    for (; *str != '\0'; str++)
    {
        if (*str == '\n')
        {
            lines++;
        }
    }
    return lines;
}

/**
 * Top of a body block spanning `rows` line slots (blank ones included), centered in the band.
 * Snapped down to an 8px boundary to keep each line on one tile-row.
 */
static s32 bodyTop(s32 rows)
{
    s32 height = (rows - 1) * BODY_PITCH + 8;
    return (CONTENT_TOP + (CONTENT_BOTTOM - CONTENT_TOP - height) / 2) & ~7;
}

/**
 * Draw a paragraph from `top`, one line per '\n' at BODY_PITCH, each centered. '@1'..'@3'
 * pull the font scratch slots the caller filled. The screen width as the wrap limit keeps
 * the literal's own line breaks.
 */
static void drawParagraph(s32 top, const char *str)
{
    font_setLineHeight(BODY_PITCH);
    font_drawLines(BODY_TILE_BASE, CENTER_X, top, DISPLAY_WIDTH, (char *)str, 1);
}

/** Draw a paragraph centered in the content band. */
static void drawCenteredParagraph(const char *str)
{
    drawParagraph(bodyTop(lineCount(str)), str);
}

/** The two migratable formats; a ROTKGBA6 image is one or the other. */
static const char *detectedString(void)
{
    return SCENE_INFO->type == SAVE_MIGRATE_TYPE_SAV_SLOT_PAGES
               ? "Found save: SavSlotPages 8 KiB (72-slot)"
               : "Found save: vanilla base-game 4-slot";
}

/** The two types whose Yes formats a fresh save instead of migrating one. */
static bool isFreshStart(void)
{
    return SCENE_INFO->type == SAVE_MIGRATE_TYPE_EMPTY ||
           SCENE_INFO->type == SAVE_MIGRATE_TYPE_UNRECOGNIZED;
}

/**
 * "52 4F 54 4B 47 42 41 36  ROTKGBA6": the raw header block, hex + printable ASCII
 * ('.' elsewhere), so a screenshot alone identifies what sits on the chip. Filled into
 * font scratch slot '@1'.
 */
static void formatHeaderDumpSlot0(const u8 *bytes)
{
    char buf[LINE_MAX + 1];
    char *p = buf;

    for (s32 i = 0; i < 8; i++)
    {
        p = hackLib_formatHexByte(p, bytes[i]);
        *p++ = ' ';
    }
    *p++ = ' ';
    for (s32 i = 0; i < 8; i++)
    {
        *p++ = hackLib_byteToFontChar(bytes[i]);
    }
    *p = '\0';
    font_copyStringToSlot0(buf);
}

/**
 * The body rows for the current state. No-SRAM / newer / blocked / done own the whole
 * body; migratable leaves the rows below for the prompt + choices (drawChoices).
 */
static void drawBody(void)
{
    const SaveMigrateInfo *info = SCENE_INFO;

    clearBand(CONTENT_TOP, CONTENT_BOTTOM);
    font_setStyle(FONT_STYLE_8PX_B, COLOR_NORMAL, 0);

    if (ARG_PHASE == PHASE_DONE)
    {
        drawCenteredParagraph(isFreshStart() ? "Format complete.\n"
                                               "A fresh ROTKRCM save is in place.\n"
                                               "\n"
                                               "Press A to continue booting the game."
                                             : "Migration complete.\n"
                                               "The save is now in ROTKRCM format.\n"
                                               "\n"
                                               "Press A to continue booting the game.");
        return;
    }

    if (info->type == SAVE_MIGRATE_TYPE_NO_SRAM)
    {
        // the poke check failed: no save medium, so nothing can be read or written
        drawCenteredParagraph("SRAM save chip not detected.\n"
                              "\n"
                              "This ROM uses the SRAM 32 KiB save type.\n"
                              "\n"
                              "Configure your emulator or flash cart\n"
                              "to use SRAM 32 KiB, then restart.");
        return;
    }

    if (info->type == SAVE_MIGRATE_TYPE_NEWER)
    {
        font_formatIntSlot0(info->version);
        drawCenteredParagraph("Found: NEWER save, version @1\n"
                              "\n"
                              "It was written by a later build of\n"
                              "this ROM, which cannot convert it.\n"
                              "Power off; the save is untouched.");
        return;
    }

    if (info->type == SAVE_MIGRATE_TYPE_EMPTY)
    {
        font_copyStringToSlot0(info->detail == 0x00u ? "0x00" : "0xFF");
        drawParagraph(BODY_TOP_WITH_CHOICES, "No save data found on the chip.\n"
                                             "It reads blank (all @1 bytes).\n"
                                             "\n"
                                             "If you expected a save here, power off\n"
                                             "and check the emulator/cart save type.");
        return;
    }

    if (info->type == SAVE_MIGRATE_TYPE_UNRECOGNIZED)
    {
        formatHeaderDumpSlot0(info->header);
        drawParagraph(BODY_TOP_WITH_CHOICES, info->detail == SAVE_MIGRATE_UNRECOGNIZED_CHECKSUM
                                                 ? "Found: damaged save header (bad checksum)\n"
                                                   "@1\n"
                                                   "\n"
                                                   "Starting a new save ERASES this data.\n"
                                                   "Power off now to keep your save."
                                                 : "Found: unrecognized save data\n"
                                                   "@1\n"
                                                   "\n"
                                                   "Starting a new save ERASES this data.\n"
                                                   "Power off now to keep your save.");
        return;
    }

    // migratable
    font_copyStringToSlot0((char *)detectedString());
    font_formatIntSlot1(SAVE_HEADER_VERSION);
    drawParagraph(BODY_TOP_WITH_CHOICES, "@1\n"
                                         "Will convert to: ROTKRCM version @2\n"
                                         "\n"
                                         "Back up your save file before converting!\n"
                                         "A converted save is rejected by vanilla\n"
                                         "and other hack ROMs. Power off now to cancel.");
}

static void drawTitle(void)
{
    font_setStyle(FONT_STYLE_11PX_A, COLOR_NORMAL, 0);
    font_drawString(1, CENTER_X, 4, "SAVE MIGRATION", 1);
}

/**
 * Draw one option. The selected one uses COLOR_BRIGHT, the entry the registered colour
 * cycle animates: the pulse is the whole selection indicator, so there is no cursor glyph.
 */
static void drawOption(u32 slot, s32 x, bool selected, const char *label)
{
    font_setStyle(FONT_STYLE_8PX_B, selected ? COLOR_BRIGHT : COLOR_NORMAL, 0);
    font_drawString(CHOICE_TILE + slot * LINE_TILES, x, OPTION_Y, (char *)label, 1);
}

/** The prompt + the No/Yes pair (confirmable, not done). */
static void drawChoices(void)
{
    bool yesSelected = ARG_CURSOR == CURSOR_YES;
    const char *prompt;

    clearBand(CHOICE_Y, OPTION_Y + 8);

    if (ARG_PHASE == PHASE_CONFIRM_1)
    {
        prompt = isFreshStart() ? "Erase ALL save data?" : "Convert the save now?";
    }
    else
    {
        prompt = "Really write the save? (writes SRAM)";
    }
    font_setStyle(FONT_STYLE_8PX_B, COLOR_NORMAL, 0);
    font_drawString(CHOICE_TILE, CENTER_X, CHOICE_Y, (char *)prompt, 1);

    drawOption(1, 96, !yesSelected, "No");
    drawOption(2, 144, yesSelected, "Yes");
}

static void drawScreen(void)
{
    drawTitle();
    drawBody();
    // drawBody cleared the whole band, so a state without a prompt needs no extra
    // blanking here.
    if (ARG_PHASE == PHASE_CONFIRM_1 || ARG_PHASE == PHASE_CONFIRM_2)
    {
        drawChoices();
    }
}

/**
 * Back out of the second confirm to the first (a No/B on "Really write the save?" returns
 * to "Convert the save now?"). Cursor resets to No.
 */
static void backToFirstStep(void)
{
    ARG_PHASE = PHASE_CONFIRM_1;
    ARG_CURSOR = CURSOR_NO;
    sfx_play(SFX_MENU_BACK);
    drawScreen();
}

/**
 * Set up the language-select background (BG0 static, BG3 drifting) + the BG2 text layer,
 * draw the screen, fade in.
 *
 * ModSceneHandlers[MOD_SCENE_ROW_SAVE_MIGRATE].enter (mod_scenes).
 */
void modSave_migrateSceneEnter(void)
{
    // This scene is the one place that runs with no save state loaded (modSave_loadState
    // mirrors a current image only), and sfx_play tests this bit, so the scene's own
    // cursor and error feedback would otherwise hang on uninitialized RAM. The migration run
    // overwrites the mirror with the real header, taking the player's setting with it.
    g_SaveGlobals.d.optionsFlags.d.sfxEnabled = TRUE;

    // Classify once, before anything can write the chip: after a run the image reads as
    // current, so a later pass would report a different save than the one on screen.
    g_SceneCurrent.args[0] = (u32)memory_zalloc(sizeof(SaveMigrateInfo));
    modSave_classify(SCENE_INFO);

    // BG0 = language-select background layer 1 (LanguageSelectBg3Asset), on the menus'
    // shared backdrop cnt (block 0, screenbase 0x1f, priority 3 back) as language select
    // itself. The tile offset of 1 keeps tile 0 transparent (the bonus-map convention).
    gfx_initDisplay(0);
    sprite_resetTileAllocatorForMode(0);
    gfx_enableDisplayFlags(GFX_DISPCNT_OBJ);
    gfx_setupBg(BG_ART_BACK, MenuBackdropBgCnt);
    gfx_setBgOffset(BG_ART_BACK, 0);
    gfx_loadBgAsset(BG_ART_BACK, LanguageSelectBg3Asset, 1, 0, 0, 0);
    // BG3 = language-select background layer 2 (LanguageSelectBg2Asset), the drifting layer;
    // the BG0 art behind it stays put, so the two separate as it moves. Its native cnt is
    // MenuTextBgCnt (charbase block 2), which the text layer uses, so the tiles move to the
    // free block 1 (ART_FRONT_BGCNT). Tile indices are relative to the charbase, so the art
    // displays identically.
    gfx_setupBg(BG_ART_FRONT, ART_FRONT_BGCNT);
    gfx_setBgOffset(BG_ART_FRONT, 0);
    // Tile offset 0, as scene_languageSelect_enter loads this asset: it reaches tile 1023, the
    // last one addressable at this base, so a shift of 1 would push the top tiles past the
    // 10-bit tile field and wrap them to transparent tile 0.
    gfx_loadBgAsset(BG_ART_FRONT, LanguageSelectBg2Asset, 0, 0, 0, 0);
    // Half of language select's rate (it drifts the same asset at 0x10000, 1 px/frame).
    // TODO: Consider removing the scrolling bg, it looks weird.
    gfx_setBgScrollSpeedX(BG_ART_FRONT, 0x8000);
    // BG2 = text layer; block 2 belongs to the BG3 art, see the define.
    gfx_setupBg(BG_TEXT, TEXT_BGCNT);
    gfx_loadPalette(UiPalette, 0);
    gfx_setBgOffset(BG_TEXT, 0);
    font_setRenderCtxBg(TEXT_BGCNT);
    clearTextBg();

    ARG_PHASE = modSave_canMigrate(SCENE_INFO) ? PHASE_CONFIRM_1 : PHASE_BLOCKED;
    ARG_CURSOR = CURSOR_NO;

    drawScreen();

    // Palette cycle for the highlighted choice. This scene borrows the bonus map's BG setup
    // and UiPalette, so it takes the same table (as chapter_select does).
    gfx_registerPaletteCycles(BonusMapMenuPaletteCycles);

    actor_tickAll();
    sprite_endFrame();

    // previous scene is always the boot stub (id 0): use the variant fade-in
    gfx_fadeInFromWhite(0x3f);
}

/**
 * Cursor navigation, shared by both confirm prompts. The pair sits on one row, so movement
 * is positional: left picks the left label, right the right one.
 */
static void moveCursor(u16 keys)
{
    u32 wanted;

    if ((keys & (DPAD_LEFT | DPAD_RIGHT)) == 0)
    {
        return;
    }
    wanted = (keys & DPAD_RIGHT) != 0 ? CURSOR_YES : CURSOR_NO;
    if (ARG_CURSOR == wanted)
    {
        return;
    }
    ARG_CURSOR = wanted;
    sfx_play(SFX_MENU_CURSOR_MOVE);
    drawChoices();
}

/**
 * "Convert the save now?": Yes advances to the second prompt. No and B only beep, since
 * leaving without migrating is not on offer.
 */
static void updateConfirm1(u16 keys)
{
    moveCursor(keys);

    if (keys & B_BUTTON)
    {
        sfx_play(SFX_MENU_ERROR);
        return;
    }
    if ((keys & A_BUTTON) == 0)
    {
        return;
    }
    if (ARG_CURSOR != CURSOR_YES)
    {
        sfx_play(SFX_MENU_ERROR);
        return;
    }
    ARG_PHASE = PHASE_CONFIRM_2;
    ARG_CURSOR = CURSOR_NO; // anti-mash
    sfx_play(SFX_MENU_CONFIRM);
    drawScreen();
}

/** "Really write the save?": Yes runs the migration, No and B back out to the first prompt. */
static void updateConfirm2(u16 keys)
{
    moveCursor(keys);

    if (keys & B_BUTTON)
    {
        backToFirstStep();
        return;
    }
    if ((keys & A_BUTTON) == 0)
    {
        return;
    }
    if (ARG_CURSOR != CURSOR_YES)
    {
        backToFirstStep();
        return;
    }
    if (isFreshStart())
    {
        modSave_format();
    }
    else
    {
        modSave_migrate(SCENE_INFO);
    }
    ARG_PHASE = PHASE_DONE;
    ARG_CURSOR = CURSOR_NO;
    sfx_play(SFX_MENU_CONFIRM);
    drawScreen();
}

/** Migration finished: A hands back to the boot chain. */
static void updateDone(u16 keys)
{
    if (keys & A_BUTTON)
    {
        // the image is in the current format now, so the boot chain picks whatever it
        // would have picked without a migration
        modStartup_resumeBoot();
    }
}

/** Newer / no SRAM: no migration to offer, so every input is the error sfx. */
static void updateBlocked(u16 keys)
{
    if (keys & (A_BUTTON | B_BUTTON))
    {
        sfx_play(SFX_MENU_ERROR);
    }
}

/** ModSceneHandlers[MOD_SCENE_ROW_SAVE_MIGRATE].tick (mod_scenes). */
void modSave_migrateSceneTick(void)
{
    u16 keys = ACTIVE_MENU_KEYS_PRESSED;

    switch (ARG_PHASE)
    {
    case PHASE_CONFIRM_1:
        updateConfirm1(keys);
        break;
    case PHASE_CONFIRM_2:
        updateConfirm2(keys);
        break;
    case PHASE_DONE:
        updateDone(keys);
        break;
    case PHASE_BLOCKED:
        updateBlocked(keys);
        break;
    }

    // The per-frame tail every top-level scene ends with (scene_mainMenu_tick,
    // scene_languageSelect_tick). sprite_endFrame sets the frame-ready flag the VBlank task
    // waits for; without it nothing is committed, BG scroll included.
    actor_tickAll();
    sprite_endFrame();
}

/**
 * Fade out, commit BGs, release the classification.
 *
 * ModSceneHandlers[MOD_SCENE_ROW_SAVE_MIGRATE].exit (mod_scenes).
 */
void modSave_migrateSceneExit(void)
{
    gfx_fadeOutToWhite(0x3f);
    sprite_flushList(&g_ActorListMain);
    gfx_commitBgs();
    gfx_clearPaletteAnims();
    sfx_stopAllOnExit();
    memory_free(SCENE_INFO);
}
