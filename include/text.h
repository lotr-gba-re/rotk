#pragma once

#include "text_ids.h"
#include "types.h"

// Block bases for the id+offset textId access patterns: each is the first string of
// a run, indexed by the value noted on the right (enum TextId lives in text_ids.h).
#define TEXT_ID_MAIN_MENU_ENTRY_BASE TEXT_ID_NEW_GAME       // + entry index (New Game..Bonus Maps)
#define TEXT_ID_HERO_NAME_BASE TEXT_ID_FRODO                // + hero slot (Frodo..Smeagol)
#define TEXT_ID_OPTIONS_ROW_LABEL_BASE TEXT_ID_MUSIC_VOLUME // + OptionsRow
#define TEXT_ID_DIFFICULTY_NAME_BASE TEXT_ID_NORMAL         // + OptionsDifficulty
#define TEXT_ID_SHOP_TITLE_BASE TEXT_ID_FORGE               // + g_ShopType
#define TEXT_ID_BONUS_MISSION_NAME_BASE TEXT_ID_MORIA       // + mission index ([4] = Locked!)
#define TEXT_ID_REGION_NAME_BASE TEXT_ID_ANDUIN_RIVER       // + regionId (enum RegionId)
#define TEXT_ID_MISSION_NAME_BASE TEXT_ID_ANDUIN_BANKS      // + missionId (enum MissionId)
#define TEXT_ID_UNIQUE_DESC_BASE TEXT_ID_UNIQUE_DESC_STING  // + unique baseIndex (< 26)
#define TEXT_ID_LOOT_NAME_BASE TEXT_ID_DEAGOLS_SKULL        // + LootType (< LOOT_TYPE_ITEM_MIN)
#define TEXT_ID_RUNE_NAME_BASE TEXT_ID_DWARF_RUNE_I_DWARFMETAL        // + runeIndex (enum RuneId)
#define TEXT_ID_LANGUAGE_NAME_BASE TEXT_ID_ENGLISH                    // + Language
#define TEXT_ID_SAVE_LOAD_RESULT_BASE TEXT_ID_GAME_SAVED_SUCCESSFULLY // + 0 saved, 1 deleted

/**
 * Options-menu title; id 56, suffixed in the generated enum because the main-menu
 * "Options" entry (TEXT_ID_OPTIONS = 15) claimed the clean slug first.
 */
#define TEXT_ID_OPTIONS_TITLE TEXT_ID_OPTIONS_56

#define DIGIT_TO_CHAR(digit) (digit + '0')
#define CHAR_TO_DIGIT(chr) (chr - '0')

/** Huffman tree node for the per-language compressed text (HuffmanTree* tables). */
typedef struct HuffmanNode
{
    u16 left;
    u16 right;
} HuffmanNode;
/** Used to index into LanguageTextPackages (order of the HuffmanTree* tables). */
typedef enum Language
{
    LANGUAGE_EN = 0,
    LANGUAGE_FR = 1,
    LANGUAGE_DE = 2,
    LANGUAGE_IT = 3,
    LANGUAGE_ES = 4,
} __attribute__((packed)) Language;
#define LANGUAGE_COUNT 5
/**
 * TRUE for languages that render affixes suffix-first ("<base>: <suffix>, <prefix>").
 * It and Es are the last two ids, so the ROM range-checks the id against
 * LANGUAGE_IT..LANGUAGE_ES.
 */
#define LANGUAGE_SUFFIX_FIRST(langId) ((u8)((langId) - LANGUAGE_IT) <= 1)

/** Decode length cap of every text string, and the size of g_TextStringBuffer. */
#define TEXT_STRING_BUFFER_SIZE 0x400

void text_init(void);
void text_setLanguagePackage(const s32 *pkg);
s32 text_decodeHuffmanString(s32 id, u8 *dst, s32 maxLen);
void text_setLanguage(s32 langId);
u8 text_getLanguage(void);
char *text_getString(s32 id);
char *text_decodeString(s32 textId, char *dst);

char *text_appendString(char *dst, TextId id);
char *text_formatInt(s32 value, char *dst);
void *text_objFromString(char **pstr);
/**
 * Rasterize one line of *pstr (advanced past it) into obj's OBJ tiles through two scratch
 * buffers. font_setRenderTarget stays pointed at those freed buffers afterwards, so a
 * scene that also draws BG text calls font_setRenderCtxBg again.
 */
void text_objRasterizeString(void *obj, char **pstr);
void text_objSetPos(void *obj, s32 xCenter, s32 y);

/**
 * Text objects are sprite actors rasterized from a string; menus keep up to 6 of them in
 * g_TextObjSlots. Creating into an occupied slot replaces the pointer only, so the caller
 * flags the old actor for removal first. See text_objRasterizeString for the font render
 * target it leaves behind.
 */
void text_objCreateInSlot(char **pstr, s32 slotIndex);
/** Forget every g_TextObjSlots entry (the actors are freed with their list). */
void text_objClearSlots(void);
