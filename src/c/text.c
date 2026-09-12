#include "text.h"
#include "memory.h"
#include "text_ids.h"
#include "variables.h"

/**
 * Format value as decimal with thousands separators (g_TextThousandsSeparator, set per
 * language by text_setLanguage) into dst.
 *
 * @return dst
 *
 * @romaddress 0x080477f0
 */
char *text_formatInt(s32 value, char *dst)
{
    char *cursor = dst;
    s32 num = value;
    s32 digit = -1;

    if (num < 0)
    {
        num = -num;
        *cursor++ = '-';
    }
    if (num > 999999999)
    {
        digit = num / 1000000000;
        num %= 1000000000;
        *cursor++ = DIGIT_TO_CHAR(digit);
        *cursor++ = g_TextThousandsSeparator;
    }
    if (num > 99999999 || digit != -1)
    {
        digit = num / 100000000;
        num %= 100000000;
        *cursor++ = DIGIT_TO_CHAR(digit);
    }
    if (num > 9999999 || digit != -1)
    {
        digit = num / 10000000;
        num %= 10000000;
        *cursor++ = DIGIT_TO_CHAR(digit);
    }
    if (num > 999999 || digit != -1)
    {
        digit = num / 1000000;
        num %= 1000000;
        *cursor++ = DIGIT_TO_CHAR(digit);
        *cursor++ = g_TextThousandsSeparator;
    }
    if (num > 99999 || digit != -1)
    {
        digit = num / 100000;
        num %= 100000;
        *cursor++ = DIGIT_TO_CHAR(digit);
    }
    if (num > 9999 || digit != -1)
    {
        digit = num / 10000;
        num %= 10000;
        *cursor++ = DIGIT_TO_CHAR(digit);
    }
    if (num > 999 || digit != -1)
    {
        digit = num / 1000;
        num %= 1000;
        *cursor++ = DIGIT_TO_CHAR(digit);
        *cursor++ = g_TextThousandsSeparator;
    }
    if (num > 99 || digit != -1)
    {
        digit = num / 100;
        num %= 100;
        *cursor++ = DIGIT_TO_CHAR(digit);
    }
    if (num > 9 || digit != -1)
    {
        digit = num / 10;
        num %= 10;
        *cursor++ = DIGIT_TO_CHAR(digit);
    }
    *cursor++ = num + '0';
    *cursor++ = '\0';
    return dst;
}

/**
 * Boot-time text setup: select English and allocate the shared decode buffer.
 *
 * @romaddress 0x080479f0
 */
void text_init(void)
{
    text_setLanguage(LANGUAGE_EN);
    g_TextStringBuffer = memory_malloc(TEXT_STRING_BUFFER_SIZE);
}

/**
 * Select the language: remember it, switch the Huffman text package, and pick the
 * thousands separator (French a space, English a comma, the rest a period).
 *
 * @romaddress 0x08047a10
 */
void text_setLanguage(s32 langId)
{
    g_TextLanguage = langId;
    text_setLanguagePackage(LanguageTextPackages[langId]);
    if (langId == LANGUAGE_FR)
    {
        g_TextThousandsSeparator = ' ';
    }
    else if (langId != LANGUAGE_EN)
    {
        g_TextThousandsSeparator = '.';
    }
    else
    {
        g_TextThousandsSeparator = ',';
    }
}

/**
 * The language selected by text_setLanguage.
 *
 * @romaddress 0x08047a5c
 */
u8 text_getLanguage(void)
{
    return g_TextLanguage;
}

/**
 * Decode a text string into the shared g_TextStringBuffer; each call overwrites the last.
 *
 * @return g_TextStringBuffer, or NULL when the decode fails
 *
 * @romaddress 0x08047a68
 */
char *text_getString(s32 id)
{
    if (text_decodeHuffmanString(id, (u8 *)g_TextStringBuffer, TEXT_STRING_BUFFER_SIZE) == 0)
    {
        return g_TextStringBuffer;
    }
    return NULL;
}

/**
 * Append the decoded string for the given id at the end of dst.
 *
 * @return pointer to dst's (new) terminator, 0 when decoding failed
 *
 * @romaddress 0x08047a8c
 */
char *text_appendString(char *dst, TextId id)
{
    while (*dst != '\0')
    {
        dst++;
    }
    if (text_decodeHuffmanString(id, (u8 *)dst, TEXT_STRING_BUFFER_SIZE) == 0)
    {
        while (*dst != '\0')
        {
            dst++;
        }
        return dst;
    }
    return 0;
}

/**
 * Decode a text string into the caller's dst.
 *
 * @return dst, or NULL when the decode fails
 *
 * @romaddress 0x08047ac0
 */
char *text_decodeString(s32 textId, char *dst)
{
    if (text_decodeHuffmanString(textId, (u8 *)dst, TEXT_STRING_BUFFER_SIZE) == 0)
    {
        return dst;
    }
    return NULL;
}
