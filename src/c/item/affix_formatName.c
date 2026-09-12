#include "font.h"
#include "game.h"
#include "item.h"
#include "player.h"
#include "save.h"
#include "scene.h"
#include "text.h"
#include "text_ids.h"
#include "variables.h"

/** HUD item-name color for the active player; gameplay and menu scenes can differ. */
static inline void setItemNameColor(u8 playerIndex, u8 gameplayColor, u8 menuColor)
{
    if (g_SceneCurrent.id == SCENE_ID_GAMEPLAY)
    {
        g_PlayerHuds[playerIndex].textColor = gameplayColor;
    }
    else
    {
        g_PlayerHuds[playerIndex].textColor = menuColor;
    }
}

/** Trim the name to maxWidth and append " ...", returning the new dst. */
static inline char *truncateNameWithEllipsis(char *dst, char *nameStart, u16 maxWidth)
{
    if (font_getStringWidth(nameStart) > maxWidth)
    {
        do
        {
            *--dst = '\0';
        } while (font_getStringWidth(nameStart) > maxWidth);

        {
            char *end = dst;
            while (dst > end - 4 || (*dst != ' ' && dst != nameStart))
            {
                *dst-- = '\0';
            }
        }
        *dst++ = ' ';
        *dst++ = '.';
        *dst++ = '.';
        *dst = '.';
    }
    return dst;
}

/**
 * Format an item's display name ("<prefix> <base> <suffix>" in language-dependent order, plus
 * two-handed marker and arrow count) into dst, truncated with " ..." when it renders wider
 * than maxWidth pixels. Also sets the active player's HUD item-name color.
 *
 * @param dst        destination buffer
 * @param item The item to format the name of
 * @param maxWidth   maximum rendered width in pixels
 *
 * @return dst
 *
 * @romaddress 0x0802b064
 */
char *item_affix_formatName(char *dst, Item item, u16 maxWidth)
{
    char *nameStart;
    u8 playerIndex = ACTIVE_PLAYER_INDEX;

    nameStart = dst;

    if (ITEM_HAS_SUFFIX(item) || (item.d.itemType == ITEM_TYPE_UNIQUE && item.d.baseIndex <= 0x19))
    {
        // suffixed item (or a special low unique); both arms store 6. Kept inline, not via
        // setItemNameColor: the identical-arms fold materializes the constant at the store,
        // which the helper's inlined body would hoist to its entry.
        if (g_SceneCurrent.id == SCENE_ID_GAMEPLAY)
        {
            g_PlayerHuds[playerIndex].textColor = HUD_TEXT_COLOR_GAMEPLAY_RED;
        }
        else
        {
            g_PlayerHuds[playerIndex].textColor = HUD_TEXT_COLOR_MENU_RED;
        }
        if (item.d.itemType == ITEM_TYPE_UNIQUE && item.d.baseIndex <= 0x19)
        {
            text_appendString(dst, ITEM_BASE(item).nameId);
            {
                u32 baseIndex = item.d.baseIndex;
                if (baseIndex > 9)
                {
                    return nameStart;
                }
            }
            setItemNameColor(playerIndex, HUD_TEXT_COLOR_GAMEPLAY_BLUE, HUD_TEXT_COLOR_MENU_BLUE);
            return nameStart;
        }
    }
    else
    {
        if ((ITEM_HAS_PREFIX(item) && item.d.itemType != ITEM_TYPE_ARROW) ||
            (item.d.itemType == ITEM_TYPE_UNIQUE && item.d.baseIndex <= 0x34))
        {
            // prefixed item (or a special low unique)
            setItemNameColor(playerIndex, HUD_TEXT_COLOR_GAMEPLAY_GREEN, HUD_TEXT_COLOR_MENU_GREEN);

            if (item.d.itemType == ITEM_TYPE_UNIQUE && item.d.baseIndex <= 0x34)
            {
                if ((u8)(item.d.baseIndex - 10) > 0xf)
                {
                    setItemNameColor(playerIndex, HUD_TEXT_COLOR_GAMEPLAY_BLUE,
                                     HUD_TEXT_COLOR_MENU_BLUE);
                }
                text_appendString(dst, ITEM_BASE(item).nameId);
                return nameStart;
            }
        }
        else
        {
            // plain item
            if (ITEM_BASE_METADATA(item).flags.p & ITEM_FLAG_ANCIENT)
            {
                setItemNameColor(playerIndex, HUD_TEXT_COLOR_GAMEPLAY_BLUE,
                                 HUD_TEXT_COLOR_MENU_BLUE);
            }
            else
            {
                setItemNameColor(playerIndex, HUD_TEXT_COLOR_GAMEPLAY_YELLOW,
                                 HUD_TEXT_COLOR_MENU_WHITE);
            }
        }
    }

    if (g_SaveGlobals.d.language != LANGUAGE_EN)
    {
        dst = text_appendString(dst, ITEM_BASE(item).nameId);
        {
            u32 noAffix = ITEM_AFFIXES_NONE;
            if (item.halfwords.affixes != noAffix)
            {
                // French typography: space before ':'
                if (g_SaveGlobals.d.language == LANGUAGE_FR)
                {
                    *dst++ = ' ';
                }
                if (!LANGUAGE_SUFFIX_FIRST(g_SaveGlobals.d.language))
                {
                    *dst++ = ':';
                }
                *dst++ = ' ';
            }
        }
        if (!LANGUAGE_SUFFIX_FIRST(g_SaveGlobals.d.language))
        {
            if (ITEM_HAS_PREFIX(item) && item.d.itemType != ITEM_TYPE_ARROW)
            {
                dst = text_appendString(dst, ITEM_PREFIX(item).nameId);
            }
            if (ITEM_HAS_SUFFIX(item))
            {
                if (ITEM_HAS_PREFIX(item))
                {
                    *dst++ = ',';
                    *dst++ = ' ';
                }
                dst = text_appendString(dst, ITEM_SUFFIX(item).nameId);
            }
        }
        else
        {
            if (ITEM_HAS_SUFFIX(item))
            {
                dst = text_appendString(dst, ITEM_SUFFIX(item).nameId);
                if (ITEM_HAS_PREFIX(item))
                {
                    *dst++ = ',';
                    *dst++ = ' ';
                }
            }
            if (ITEM_HAS_PREFIX(item) && item.d.itemType != ITEM_TYPE_ARROW)
            {
                dst = text_appendString(dst, ITEM_PREFIX(item).nameId);
            }
        }
    }
    else
    {
        if (ITEM_HAS_PREFIX(item) && item.d.itemType != ITEM_TYPE_ARROW)
        {
            dst = text_appendString(dst, ITEM_PREFIX(item).nameId);
            *dst++ = ' ';
        }
        dst = text_appendString(dst, ITEM_BASE(item).nameId);
        if (ITEM_HAS_SUFFIX(item))
        {
            *dst++ = ' ';
            dst = text_appendString(dst, ITEM_SUFFIX(item).nameId);
        }
    }

    if (ITEM_BASE_METADATA(item).flags.p & ITEM_FLAG_TWO_HANDED)
    {
        *dst++ = ' ';
        dst = text_appendString(dst, 0x148);
    }

    {
        if (item.d.itemType == ITEM_TYPE_ARROW)
        {
            // French typography: space before ':'
            if (g_SaveGlobals.d.language == LANGUAGE_FR)
            {
                *dst++ = ' ';
            }
            *dst++ = ':';
            *dst++ = ' ';
            dst = text_formatInt(item.d.prefixIndex, dst);
            while (*dst != '\0')
            {
                dst++;
            }
            *dst++ = ' ';
            dst = text_appendString(dst, 0x146);
        }
    }

    dst = truncateNameWithEllipsis(dst, nameStart, maxWidth);

    if (ITEM_BASE_METADATA(item).flags.p & ITEM_FLAG_ANCIENT)
    {
        setItemNameColor(playerIndex, HUD_TEXT_COLOR_GAMEPLAY_BLUE, HUD_TEXT_COLOR_MENU_BLUE);
    }
    return nameStart;
}
