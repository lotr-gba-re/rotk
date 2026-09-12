#include "menu.h"
#include "save.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off

/**
 * Bonus map select entries: icon record, icon position, name and OAM palette
 * number, and the SaveGlobals unlock flags that reveal the map; [4] is the
 * locked-slot display.
 *
 * @romaddress 0x08050634
 */
const BonusMapMenuEntry BonusMapMenuEntries[5] = {
    // [0] "Moria"
    {.animation = SPRITE_STILL_ANIMATION_NAMED(LevelIconMoria),
     .iconX = 3, .iconY = 18, .nameTextId = TEXT_ID_MORIA,
     .paletteNumber = 3, .unlockMask = UNLOCK_FLAG_BONUS_MAP_MORIA},
    // [1] "Weathertop"
    {.animation = SPRITE_STILL_ANIMATION_NAMED(LevelIconWeathertop),
     .iconX = 39, .iconY = 18, .nameTextId = TEXT_ID_WEATHERTOP_103,
     .paletteNumber = 4, .unlockMask = UNLOCK_FLAG_BONUS_MAP_WEATHERTOP},
    // [2] "Helm's Deep"
    {.animation = SPRITE_STILL_ANIMATION_NAMED(LevelIconHelmsDeep),
     .iconX = 75, .iconY = 18, .nameTextId = TEXT_ID_HELMS_DEEP_104,
     .paletteNumber = 5, .unlockMask = UNLOCK_FLAG_BONUS_MAP_HELMS_DEEP},
    // [3] "Fangorn Forest"
    {.animation = SPRITE_STILL_ANIMATION_NAMED(LevelIconFangornForest),
     .iconX = 111, .iconY = 18, .nameTextId = TEXT_ID_FANGORN_FOREST_105,
     .paletteNumber = 6, .unlockMask = UNLOCK_FLAG_BONUS_MAP_FANGORN},
    // [4] "Locked!"
    {.animation = SPRITE_STILL_ANIMATION_NAMED(LevelIconLocked),
     .iconX = 111, .iconY = 18, .nameTextId = TEXT_ID_LOCKED_106,
     .paletteNumber = 7, .unlockMask = 0},
};
// clang-format on
