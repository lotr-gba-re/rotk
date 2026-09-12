#include "player.h"
#include "variables.h"

// clang-format off

/**
 * Stat screen portrait per hero.
 *
 * @romaddress 0x08282978
 */
const void *const HeroPortraitBgAssets[8] = {
    [HERO_ID_FRODO] = HeroPortraitFrodoBgAsset,
    [HERO_ID_LEGOLAS] = HeroPortraitLegolasBgAsset,
    [HERO_ID_ARAGORN] = HeroPortraitAragornBgAsset,
    [HERO_ID_GANDALF] = HeroPortraitGandalfBgAsset,
    [HERO_ID_EOWYN] = HeroPortraitEowynBgAsset,
    [HERO_ID_GIMLI] = HeroPortraitGimliBgAsset,
    [HERO_ID_SAM] = HeroPortraitSamBgAsset,
    [HERO_ID_SMEAGOL] = HeroPortraitSmeagolBgAsset,
};
// clang-format on
