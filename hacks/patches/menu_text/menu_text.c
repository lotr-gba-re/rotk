#include "font.h"
#include "gfx.h"
#include "scene.h"
#include "scene/mainMenu.h"
#include "text.h"
#include "variables.h"
#include "version.h"

#define MENU_TEXT_STRING "RagingCactus Mod Version " HACK_VERSION

// Top-left corner, screen pixels (240x160).
#define MENU_TEXT_X 8
#define MENU_TEXT_Y 8

/** g_TextObjSlots entry for the object; the main menu keeps no text objects of its own. */
#define MENU_TEXT_OBJ_SLOT 0

/**
 * Scene arg flagging that the object exists. The main menu leaves args[2] alone, and a
 * scene change zeroes every arg, so it reads 0 exactly once per visit.
 */
#define MENU_TEXT_ARG_CREATED g_SceneCurrent.args[2]

void menuText_run(void)
{
    char *str = MENU_TEXT_STRING;
    u32 width;

    if (MENU_TEXT_ARG_CREATED != 0)
    {
        return;
    }
    MENU_TEXT_ARG_CREATED = 1;

    // Text objects draw with OBJ palette 0; the main menu loads no OBJ palette itself.
    gfx_loadObjPaletteColors((const u16 *)(UiPalette + 2), 0, 16);
    font_setStyle(FONT_STYLE_8PX_B, 3, 0);
    // measured first: creating the object advances str past the text it consumed
    width = font_getStringWidth(str);
    text_objCreateInSlot(&str, MENU_TEXT_OBJ_SLOT);
    // text_objSetPos takes the horizontal center
    text_objSetPos(g_TextObjSlots[MENU_TEXT_OBJ_SLOT], MENU_TEXT_X + width / 2, MENU_TEXT_Y);
    // Rasterizing the object points the font at two scratch buffers and leaves it there;
    // the menu entries still draw on BG0 through the same context, so point it back.
    font_setRenderCtxBg(MainMenuBgCnt0);
}
