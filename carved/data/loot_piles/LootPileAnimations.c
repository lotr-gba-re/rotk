#include "libc.h"
#include "spriteAnimation.h"
#include "variables.h"

// clang-format off

/**
 * Ground drop sprite per LootType: its LootPiles.c tiles and frame set (shared where
 * drops look alike), no palette, and the frame duration.
 *
 * @romaddress 0x080691e4
 */

#define LOOT_PILE_ANIMATION(name, frameDuration_)                                                  \
    {.tiles = name##Tiles, .frames = &name##Frames, .palette = NULL,                               \
     .frameDuration = frameDuration_}

const SpriteAnimation LootPileAnimations[89] = {
    // [0x00] "Deagol's Skull"
    LOOT_PILE_ANIMATION(LootPileDeagolsSkull, 1),
    // [0x01] "Feather of Gwaihir"
    LOOT_PILE_ANIMATION(LootPileFeatherOfGwaihir, 1),
    // [0x02] "Saruman's Palantir"
    LOOT_PILE_ANIMATION(LootPileSarumansPalantir, 1),
    // [0x03] "Shelob's Stinger"
    LOOT_PILE_ANIMATION(LootPileShelobsStinger, 1),
    // [0x04] "Galadriel's Lock"
    LOOT_PILE_ANIMATION(LootPileGaladrielsLock, 1),
    // [0x05] "Witch King's Crown"
    LOOT_PILE_ANIMATION(LootPileWitchKingsCrown, 1),
    // [0x06] "There and Back Again"
    LOOT_PILE_ANIMATION(LootPileThereAndBackAgain, 1),
    // [0x07] "Simbelmyne"
    LOOT_PILE_ANIMATION(LootPileSimbelmyne, 1),
    // [0x08] "Dwarven Jug"
    LOOT_PILE_ANIMATION(LootPileDwarvenJug, 1),
    // [0x09] "Gondor Tree Crest"
    LOOT_PILE_ANIMATION(LootPileGondorTreeCrest, 1),
    // [0x0a] "Lantern"
    LOOT_PILE_ANIMATION(LootPileLantern, 1),
    // [0x0b] "Tattered Flag"
    LOOT_PILE_ANIMATION(LootPileTatteredFlag, 1),
    // [0x0c] "White Hand Crest"
    LOOT_PILE_ANIMATION(LootPileWhiteHandCrest, 1),
    // [0x0d] "A Gem"
    LOOT_PILE_ANIMATION(LootPileGemStackMin, 3),
    // [0x0e] "A Pile of @1 Gems"
    LOOT_PILE_ANIMATION(LootPileGemStack, 3),
    // [0x0f] "Some Fruit"
    LOOT_PILE_ANIMATION(LootPileFruit, 3),
    // [0x10] "Some Mushrooms"
    LOOT_PILE_ANIMATION(LootPileMushrooms, 3),
    // [0x11] "Dried Kingsfoil Herbs"
    LOOT_PILE_ANIMATION(LootPileDriedKingsfoil, 3),
    // [0x12] "Fresh Kingsfoil Herbs"
    LOOT_PILE_ANIMATION(LootPileFreshKingsfoil, 3),
    // [0x13] "Lembas Bread"
    LOOT_PILE_ANIMATION(LootPileLembasBread, 3),
    // [0x14] "Shire Herbs"
    LOOT_PILE_ANIMATION(LootPileDriedKingsfoil, 3),
    // [0x15] "Elven Herbs"
    LOOT_PILE_ANIMATION(LootPileDriedKingsfoil, 3),
    // [0x16] "Mirkwood Herbs"
    LOOT_PILE_ANIMATION(LootPileFreshKingsfoil, 3),
    // [0x17] "Rivendell Herbs"
    LOOT_PILE_ANIMATION(LootPileFreshKingsfoil, 3),
    // [0x18] "Lothlorien Herbs"
    LOOT_PILE_ANIMATION(LootPileFreshKingsfoil, 3),
    // [0x19] "LOOT_TYPE_BOW_TIER_1"
    LOOT_PILE_ANIMATION(LootPileBowTier1, 3),
    // [0x1a] "LOOT_TYPE_BOW_TIER_2"
    LOOT_PILE_ANIMATION(LootPileBowTier2, 3),
    // [0x1b] "LOOT_TYPE_ARROW"
    LOOT_PILE_ANIMATION(LootPileArrow, 3),
    // [0x1c] "LOOT_TYPE_KNIFE_TIER_2"
    LOOT_PILE_ANIMATION(LootPileKnifeTier2, 3),
    // [0x1d] "LOOT_TYPE_SWORD_ONE_HAND"
    LOOT_PILE_ANIMATION(LootPileSwordOneHand, 3),
    // [0x1e] "LOOT_TYPE_SWORD_TWO_HAND_TIER_1"
    LOOT_PILE_ANIMATION(LootPileSwordTwoHandTier1, 3),
    // [0x1f] "LOOT_TYPE_SWORD_TWO_HAND_TIER_2"
    LOOT_PILE_ANIMATION(LootPileSwordTwoHandTier2, 3),
    // [0x20] "LOOT_TYPE_AXE_TIER_1"
    LOOT_PILE_ANIMATION(LootPileAxeTier1, 3),
    // [0x21] "LOOT_TYPE_AXE_TIER_2"
    LOOT_PILE_ANIMATION(LootPileAxeTier2, 3),
    // [0x22] "LOOT_TYPE_STAFF_TIER_1"
    LOOT_PILE_ANIMATION(LootPileStaffTier1, 3),
    // [0x23] "LOOT_TYPE_STAFF_TIER_2"
    LOOT_PILE_ANIMATION(LootPileStaffTier2, 3),
    // [0x24] "LOOT_TYPE_KNIFE_TIER_1"
    LOOT_PILE_ANIMATION(LootPileKnifeTier1, 3),
    // [0x25] "LOOT_TYPE_SPOON"
    LOOT_PILE_ANIMATION(LootPileSpoon, 1),
    // [0x26] "LOOT_TYPE_ARMOR_TIER_1"
    LOOT_PILE_ANIMATION(LootPileArmorTier1, 3),
    // [0x27] "LOOT_TYPE_ARMOR_TIER_2"
    LOOT_PILE_ANIMATION(LootPileArmorTier2, 3),
    // [0x28] "LOOT_TYPE_ARMOR_TIER_3"
    LOOT_PILE_ANIMATION(LootPileArmorTier3, 3),
    // [0x29] "LOOT_TYPE_CLOAK_TIER_1"
    LOOT_PILE_ANIMATION(LootPileCloakTier1, 3),
    // [0x2a] "LOOT_TYPE_CLOAK_TIER_2"
    LOOT_PILE_ANIMATION(LootPileCloakTier2, 3),
    // [0x2b] "LOOT_TYPE_CLOAK_TIER_3"
    LOOT_PILE_ANIMATION(LootPileCloakTier3, 3),
    // [0x2c] "LOOT_TYPE_SHIELD_TIER_1"
    LOOT_PILE_ANIMATION(LootPileShieldTier1, 3),
    // [0x2d] "LOOT_TYPE_SHIELD_TIER_2"
    LOOT_PILE_ANIMATION(LootPileShieldTier2, 3),
    // [0x2e] "LOOT_TYPE_SHOE_TIER_1"
    LOOT_PILE_ANIMATION(LootPileShoeTier1, 3),
    // [0x2f] "LOOT_TYPE_SHOE_TIER_2"
    LOOT_PILE_ANIMATION(LootPileShoeTier2, 3),
    // [0x30] "LOOT_TYPE_ACCESSORY_TRINKET"
    LOOT_PILE_ANIMATION(LootPileAccessoryTrinket, 3),
    // [0x31] "LOOT_TYPE_ACCESSORY_AMULET"
    LOOT_PILE_ANIMATION(LootPileAccessoryAmulet, 3),
    // [0x32] "LOOT_TYPE_ACCESSORY_COLLAR"
    LOOT_PILE_ANIMATION(LootPileAccessoryCollar, 3),
    // [0x33] "LOOT_TYPE_HELM_CROWN"
    LOOT_PILE_ANIMATION(LootPileHelmCrown, 3),
    // [0x34] "LOOT_TYPE_HELM_HOOD"
    LOOT_PILE_ANIMATION(LootPileHelmHood, 3),
    // [0x35] "LOOT_TYPE_HELM_METAL"
    LOOT_PILE_ANIMATION(LootPileHelmMetal, 3),
    // [0x36] "LOOT_TYPE_UNIQUE_MELEE"
    LOOT_PILE_ANIMATION(LootPileUniqueMelee, 3),
    // [0x37] "LOOT_TYPE_UNIQUE_BOW"
    LOOT_PILE_ANIMATION(LootPileUniqueBow, 3),
    // [0x38] "LOOT_TYPE_UNIQUE_STAFF"
    LOOT_PILE_ANIMATION(LootPileUniqueStaff, 3),
    // [0x39] "LOOT_TYPE_UNIQUE_SWORD"
    LOOT_PILE_ANIMATION(LootPileUniqueSword, 3),
    // [0x3a] "LOOT_TYPE_GLOVE"
    LOOT_PILE_ANIMATION(LootPileGlove, 3),
    // [0x3b] "LOOT_TYPE_CLUB"
    LOOT_PILE_ANIMATION(LootPileClub, 3),
    // [0x3c] "LOOT_TYPE_MACE"
    LOOT_PILE_ANIMATION(LootPileMace, 3),
    // [0x3d] "Sting"
    LOOT_PILE_ANIMATION(LootPileSting, 1),
    // [0x3e] "Symbolblade"
    LOOT_PILE_ANIMATION(LootPileSymbolblade, 1),
    // [0x3f] "Glamdring"
    LOOT_PILE_ANIMATION(LootPileGlamdring, 1),
    // [0x40] "Orcrist"
    LOOT_PILE_ANIMATION(LootPileOrcrist, 1),
    // [0x41] "Narsil"
    LOOT_PILE_ANIMATION(LootPileNarsil, 1),
    // [0x42] "Hadhafang"
    LOOT_PILE_ANIMATION(LootPileHadhafang, 1),
    // [0x43] "Silveraxe"
    LOOT_PILE_ANIMATION(LootPileSilveraxe, 1),
    // [0x44] "Staff of Five Mages"
    LOOT_PILE_ANIMATION(LootPileStaffOfFiveMages, 1),
    // [0x45] "Starfall Bow"
    LOOT_PILE_ANIMATION(LootPileStarfallBow, 1),
    // [0x46] "Nightfell Bow"
    LOOT_PILE_ANIMATION(LootPileNightfellBow, 1),
    // [0x47] "Celeborn's Phial"
    LOOT_PILE_ANIMATION(LootPileCelebornsPhial, 1),
    // [0x48] "Boromir's Horn"
    LOOT_PILE_ANIMATION(LootPileBoromirsHorn, 1),
    // [0x49] "Cyclopedia of Herbs"
    LOOT_PILE_ANIMATION(LootPileCyclopediaOfHerbs, 1),
    // [0x4a] "Sam's Cookpot"
    LOOT_PILE_ANIMATION(LootPileSamsCookpot, 1),
    // [0x4b] "Fletching Kit"
    LOOT_PILE_ANIMATION(LootPileFletchingKit, 1),
    // [0x4c] "Scrolls of Minas Tirith"
    LOOT_PILE_ANIMATION(LootPileScrollsOfMinasTirith, 1),
    // [0x4d] "Evenstar of Arwen"
    LOOT_PILE_ANIMATION(LootPileEvenstarOfArwen, 1),
    // [0x4e] "Galadriel's Phial"
    LOOT_PILE_ANIMATION(LootPileCelebornsPhial, 1),
    // [0x4f] "Forgotten Palantir"
    LOOT_PILE_ANIMATION(LootPileForgottenPalantir, 1),
    // [0x50] "Cracked Palantir"
    LOOT_PILE_ANIMATION(LootPileCrackedPalantir, 1),
    // [0x51] "Orc Drum"
    LOOT_PILE_ANIMATION(LootPileOrcDrum, 1),
    // [0x52] "Woven Goblin Doll"
    LOOT_PILE_ANIMATION(LootPileWovenGoblinDoll, 1),
    // [0x53] "Oliphaunt Ivory Statuette"
    LOOT_PILE_ANIMATION(LootPileOliphauntIvoryStatuette, 1),
    // [0x54] "Orc Head"
    LOOT_PILE_ANIMATION(LootPileOrcHead, 1),
    // [0x55] "Swollen Spider Glands"
    LOOT_PILE_ANIMATION(LootPileSwollenSpiderGlands, 1),
    // [0x56] "Featherwood Flute"
    LOOT_PILE_ANIMATION(LootPileFeatherwoodFlute, 1),
    // [0x57] "LOOT_TYPE_ENT_WATER"
    LOOT_PILE_ANIMATION(LootPileEntWater, 1),
    // [0x58] "LOOT_TYPE_RUNE"
    LOOT_PILE_ANIMATION(LootPileRune, 1),
};
// clang-format on
