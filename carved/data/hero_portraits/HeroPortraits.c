#include "gfx.h"
#include "gfx/hero_portraits.inc"

// clang-format off

/**
 * Stat screen hero portraits: 8x6 tiles, 8bpp, colors 0x60 and up of a 256-color
 * palette, in HeroId order.
 *
 * @romaddress 0x08b91fd4
 */

#define HERO_PORTRAIT(name)                                                                        \
    BG_ASSET_MAPPED(name, BG_ASSET_PALETTE_256 | BG_ASSET_MAP | BG_ASSET_TILES |                   \
                    BG_ASSET_CODEC_PUCRUNCH | BG_ASSET_8BPP, 256, 8, 6)

// [0] "Frodo"
HERO_PORTRAIT(HeroPortraitFrodoBgAsset);
// [1] "Legolas"
HERO_PORTRAIT(HeroPortraitLegolasBgAsset);
// [2] "Aragorn"
HERO_PORTRAIT(HeroPortraitAragornBgAsset);
// [3] "Gandalf"
HERO_PORTRAIT(HeroPortraitGandalfBgAsset);
// [4] "Eowyn"
HERO_PORTRAIT(HeroPortraitEowynBgAsset);
// [5] "Gimli"
HERO_PORTRAIT(HeroPortraitGimliBgAsset);
// [6] "Sam"
HERO_PORTRAIT(HeroPortraitSamBgAsset);
// [7] "Smeagol"
HERO_PORTRAIT(HeroPortraitSmeagolBgAsset);
// clang-format on
