#include "gfx.h"
#include "gfx/dialog_portraits.inc"

// clang-format off

/**
 * In-game dialog speaker pictures: 7x8 tiles, 16 colors, no map, in ROM order.
 *
 * @romaddress 0x08d3dce0
 */

#define DIALOG_PORTRAIT(name)                                                                      \
    BG_ASSET_UNMAPPED(name, BG_ASSET_PALETTE_16 | BG_ASSET_TILES | BG_ASSET_CODEC_PUCRUNCH, 16)

// [0] "Frodo"
DIALOG_PORTRAIT(DialogPortraitFrodoBgAsset);
// [1] "Legolas"
DIALOG_PORTRAIT(DialogPortraitLegolasBgAsset);
// [2] "Aragorn"
DIALOG_PORTRAIT(DialogPortraitAragornBgAsset);
// [3] "Gandalf"
DIALOG_PORTRAIT(DialogPortraitGandalfBgAsset);
// [4] "Eowyn"
DIALOG_PORTRAIT(DialogPortraitEowynBgAsset);
// [5] "Gimli"
DIALOG_PORTRAIT(DialogPortraitGimliBgAsset);
// [6] "Sam"
DIALOG_PORTRAIT(DialogPortraitSamBgAsset);
// [7] "Smeagol"
DIALOG_PORTRAIT(DialogPortraitSmeagolBgAsset);
// [8] "Saruman"
DIALOG_PORTRAIT(DialogPortraitSarumanBgAsset);
// [9] "Eomer"
DIALOG_PORTRAIT(DialogPortraitEomerBgAsset);
// [10] "Theoden"
DIALOG_PORTRAIT(DialogPortraitTheodenBgAsset);
// [11] "Villager"
DIALOG_PORTRAIT(DialogPortraitVillagerBgAsset);
// [12] "RohanSoldier"
DIALOG_PORTRAIT(DialogPortraitRohanSoldierBgAsset);
// [13] "Elrond"
DIALOG_PORTRAIT(DialogPortraitElrondBgAsset);
// [14] "Ranger"
DIALOG_PORTRAIT(DialogPortraitRangerBgAsset);
// [15] "KingOfTheDead"
DIALOG_PORTRAIT(DialogPortraitKingOfTheDeadBgAsset);
// [16] "Elder"
DIALOG_PORTRAIT(DialogPortraitElderBgAsset);
// [17] "Orc"
DIALOG_PORTRAIT(DialogPortraitOrcBgAsset);
// [18] "Woman"
DIALOG_PORTRAIT(DialogPortraitWomanBgAsset);
// [19] "OrcCaptain"
DIALOG_PORTRAIT(DialogPortraitOrcCaptainBgAsset);
// [20] "MouthOfSauron"
DIALOG_PORTRAIT(DialogPortraitMouthOfSauronBgAsset);
// [21] "GondorSoldier"
DIALOG_PORTRAIT(DialogPortraitGondorSoldierBgAsset);
// [22] "Merry"
DIALOG_PORTRAIT(DialogPortraitMerryBgAsset);
// [23] "WitchKing"
DIALOG_PORTRAIT(DialogPortraitWitchKingBgAsset);
// [24] "Pippin"
DIALOG_PORTRAIT(DialogPortraitPippinBgAsset);
// [25] "Denethor"
DIALOG_PORTRAIT(DialogPortraitDenethorBgAsset);
// [26] "Faramir"
DIALOG_PORTRAIT(DialogPortraitFaramirBgAsset);
// [27] "Gollum"
DIALOG_PORTRAIT(DialogPortraitGollumBgAsset);
// [28] "Shagrat"
DIALOG_PORTRAIT(DialogPortraitShagratBgAsset);
// [29] "Gorbag"
DIALOG_PORTRAIT(DialogPortraitGorbagBgAsset);
// clang-format on
