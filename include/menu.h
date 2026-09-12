#pragma once

#include "sprite.h"
#include "types.h"

/**
 * Options-menu (scene 0x06) row indices; labels are
 * text_getString(TEXT_ID_OPTIONS_ROW_LABEL_BASE + row).
 */
enum OptionsRow
{
    OPTIONS_ROW_MUSIC_VOLUME = 0,
    OPTIONS_ROW_SFX_VOLUME = 1,
    OPTIONS_ROW_QUICK_SKILL_SELECT = 2,
    OPTIONS_ROW_AUTO_HEALING = 3,
    OPTIONS_ROW_FAST_DISCARD = 4,
    OPTIONS_ROW_DIFFICULTY = 5,
    OPTIONS_ROW_LANGUAGE_SELECT = 6,
    OPTIONS_ROW_CREDITS = 7,
    OPTIONS_ROW_COUNT = 8
};

// Volume rows map the 0-10 UI value onto the hardware volume byte: a linear
// step, with the UI max pinned to the hardware max.
#define OPTIONS_VOLUME_MAX 10
#define OPTIONS_MUSIC_VOLUME_STEP 0x0c
#define OPTIONS_MUSIC_VOLUME_MAX 0x7f
#define OPTIONS_SFX_VOLUME_STEP 0x19
#define OPTIONS_SFX_VOLUME_MAX 0xff

/** OPTIONS_ROW_DIFFICULTY selector values (map to OPTION_FLAG_DIFFICULTY_*). */
enum OptionsDifficulty
{
    OPTIONS_DIFFICULTY_NORMAL = 0,
    OPTIONS_DIFFICULTY_HARD = 1,
    OPTIONS_DIFFICULTY_GRUELING = 2
};

/**
 * One selectable bonus mission on the scene-0x18 bonus-map menu (BonusMapMenuEntries
 * @0x08050634). Entry [4] is the locked-display template (grey animation/palette, name
 * "Locked!", mask 0).
 */
typedef struct BonusMapMenuEntry
{
    SpriteAnimation animation; // embedded mission-icon animation descriptor
    u16 iconX;                 // icon position on the select screen, used <<16 (16.16 fixed point)
    u16 iconY;
    u16 nameTextId;    // TEXT_ID_BONUS_MISSION_NAME_BASE + mission index; [4] = Locked!
    u16 paletteNumber; // OAM palette number of the icon sprite
    u32 unlockMask;    // tested against SaveGlobals.unlockFlags
} BonusMapMenuEntry;
/**
 * Bonus-map mission list entry (per-mission lists hang off BonusMissions @0x082825e8).
 * Every list holds 5 identical entries, so the sub-entry selection has no effect.
 */
typedef struct BonusMissionEntry
{
    u16 missionId; // launched via game_requestSceneChangeWithArgs(1, 5, missionId)
    u8 field_0x2;  // staged as g_SceneNext.args[3] at launch; always 0 in data
    u8 field_0x3;
} BonusMissionEntry;
/**
 * One entry on the multiplayer level select's 2x4 grid (MultiplayerLevelSelectEntries
 * @0x0806c9e4). The icon animation embeds FIRST (entry i's animation precedes its fields); the
 * locked-display template animation trails the table @0x0806cac4.
 */
typedef struct MultiplayerLevelSelectEntry
{
    SpriteAnimation animation; // embedded icon animation descriptor
    u16 iconX;                 // icon position on the select screen, used <<16 (16.16 fixed point)
    u16 iconY;
    u16 nameTextId;
    u16 paletteNumber; // OAM palette number of the icon sprite
    u32 unlockMask;    // 0xffffffff = always unlocked; else tested against g_CoopUnlockFlags
} MultiplayerLevelSelectEntry;

/**
 * Define a level select icon: its tiles, a still 32x64 frame set and a 16-color palette, all
 * named after the icon, from the name##_TILES / _FRAME0_* / _PALETTE lists rotkit build gfx
 * generates from its PNG.
 */
#define LEVEL_ICON(name)                                                                           \
    const u8 name##Tiles[] = {name##_TILES};                                                       \
    const SPRITE_FRAME_SET_STRUCT(1, 0, 1) name##Frames = {                                        \
        .header = SPRITE_FRAME_SET_HEADER(32, 64, 1, 0),                                           \
        .frameOffsets = {2},                                                                       \
        .frames = {{.frame = SPRITE_FRAME(1, name##_FRAME0_LZ77, 32, 64, name##_FRAME0_OFFSET),    \
                    .oam = {SPRITE_OAM(0, 0, 32, 64, 0)}}}};                                       \
    const u16 name##Palette[16] = {name##_PALETTE}

u32 menu_moveCursorVertical(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex);
u32 menu_moveCursorHorizontal(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex);
u32 menu_movePlayerCursorVertical(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex);
u32 menu_movePlayerCursorHorizontal(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex);
u32 menu_movePlayerCursorVerticalHeld(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex);
u32 menu_movePlayerCursorHorizontalHeld(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex);
u32 menu_movePlayerCursorLr(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex);
u32 menu_advancePlayerCursorL(u32 *val, u32 min, u32 max, bool wrap, u8 playerIndex);
void menu_toggleFlag(bool *flag);
void menu_createLrButtonIndicators(void);
