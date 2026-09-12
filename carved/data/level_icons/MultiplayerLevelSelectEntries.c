#include "menu.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off

/**
 * Multiplayer level select entries: icon record, grid position, name and OAM
 * palette number, and the g_CoopUnlockFlags mask that reveals the level
 * (0xffffffff = always).
 *
 * @romaddress 0x0806c9e4
 */
const MultiplayerLevelSelectEntry MultiplayerLevelSelectEntries[8] = {
    // [0] "Paths of the Dead"
    {.animation = SPRITE_STILL_ANIMATION_NAMED(LevelIconPathsOfTheDead),
     .iconX = 3, .iconY = 18, .nameTextId = TEXT_ID_PATHS_OF_THE_DEAD,
     .paletteNumber = 3, .unlockMask = 0xffffffff},
    // [1] "Pelennor Fields"
    {.animation = SPRITE_STILL_ANIMATION_NAMED(LevelIconPelennorFields),
     .iconX = 39, .iconY = 18, .nameTextId = TEXT_ID_PELENNOR_FIELDS,
     .paletteNumber = 4, .unlockMask = 0xffffffff},
    // [2] "Minas Tirith"
    {.animation = SPRITE_STILL_ANIMATION_NAMED(LevelIconMinasTirith),
     .iconX = 75, .iconY = 18, .nameTextId = TEXT_ID_MINAS_TIRITH,
     .paletteNumber = 5, .unlockMask = 0xffffffff},
    // [3] "Mt Doom"
    {.animation = SPRITE_STILL_ANIMATION_NAMED(LevelIconMtDoom),
     .iconX = 111, .iconY = 18, .nameTextId = TEXT_ID_MT_DOOM,
     .paletteNumber = 6, .unlockMask = 0xffffffff},
    // [4] "Moria"
    {.animation = SPRITE_STILL_ANIMATION_NAMED(LevelIconMoria),
     .iconX = 3, .iconY = 86, .nameTextId = TEXT_ID_MORIA,
     .paletteNumber = 7, .unlockMask = 0x10000000},
    // [5] "Weathertop"
    {.animation = SPRITE_STILL_ANIMATION_NAMED(LevelIconWeathertop),
     .iconX = 39, .iconY = 86, .nameTextId = TEXT_ID_WEATHERTOP_103,
     .paletteNumber = 8, .unlockMask = 0x00000004},
    // [6] "Helm's Deep"
    {.animation = SPRITE_STILL_ANIMATION_NAMED(LevelIconHelmsDeep),
     .iconX = 75, .iconY = 86, .nameTextId = TEXT_ID_HELMS_DEEP_104,
     .paletteNumber = 9, .unlockMask = 0x00000008},
    // [7] "Fangorn Forest"
    {.animation = SPRITE_STILL_ANIMATION_NAMED(LevelIconFangornForest),
     .iconX = 111, .iconY = 86, .nameTextId = TEXT_ID_FANGORN_FOREST_105,
     .paletteNumber = 10, .unlockMask = 0x00000002},
};

/**
 * Animation of a locked grid slot, the bonus map select's locked icon.
 *
 * @romaddress 0x0806cac4
 */
const SpriteAnimation MultiplayerLevelSelectLockedAnimation =
    SPRITE_STILL_ANIMATION_NAMED(LevelIconLocked);
// clang-format on
