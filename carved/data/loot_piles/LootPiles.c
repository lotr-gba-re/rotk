#include "gfx/loot_piles.inc"
#include "spriteAnimation.h"

// clang-format off

/**
 * Ground drop sprites: per LootPileAnimations record its tiles (per frame, LZ77
 * where smaller) and frame set, in ROM order. No palette: a drop draws with the
 * fixed OBJ palette slot actor_applyTypeDefaults picks by LootType.
 *
 * @romaddress 0x08febd84
 */

// [0x00] "Deagol's Skull"
const u8 LootPileDeagolsSkullTiles[] = {LootPileDeagolsSkull_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileDeagolsSkullFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileDeagolsSkull_FRAME0_LZ77, 24, 16,
                               LootPileDeagolsSkull_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-8, 6, -5, 9)},
         .oam = {SPRITE_OAM(-11, -5, 8, 16, 0), SPRITE_OAM(-3, -5, 16, 16, 2)}},
    },
};

// [0x01] "Feather of Gwaihir"
const u8 LootPileFeatherOfGwaihirTiles[] = {LootPileFeatherOfGwaihir_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileFeatherOfGwaihirFrames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileFeatherOfGwaihir_FRAME0_LZ77, 32, 16,
                               LootPileFeatherOfGwaihir_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-7, 7, -7, 4)},
         .oam = {SPRITE_OAM(-16, -9, 32, 16, 0)}},
    },
};

// [0x02] "Saruman's Palantir"
const u8 LootPileSarumansPalantirTiles[] = {LootPileSarumansPalantir_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileSarumansPalantirFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileSarumansPalantir_FRAME0_LZ77, 24, 16,
                               LootPileSarumansPalantir_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-10, 7, -8, 7)},
         .oam = {SPRITE_OAM(-10, -6, 8, 16, 0), SPRITE_OAM(-2, -6, 16, 16, 2)}},
    },
};

// [0x03] "Shelob's Stinger"
const u8 LootPileShelobsStingerTiles[] = {LootPileShelobsStinger_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileShelobsStingerFrames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileShelobsStinger_FRAME0_LZ77, 32, 16,
                               LootPileShelobsStinger_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-12, 6, -9, 6)},
         .oam = {SPRITE_OAM(-16, -6, 32, 16, 0)}},
    },
};

// [0x04] "Galadriel's Lock"
const u8 LootPileGaladrielsLockTiles[] = {LootPileGaladrielsLock_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileGaladrielsLockFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileGaladrielsLock_FRAME0_LZ77, 24, 16,
                               LootPileGaladrielsLock_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-10, 10, -4, 6)},
         .oam = {SPRITE_OAM(-10, -4, 8, 16, 0), SPRITE_OAM(-2, -4, 16, 16, 2)}},
    },
};

// [0x05] "Witch King's Crown"
const u8 LootPileWitchKingsCrownTiles[] = {LootPileWitchKingsCrown_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 4) LootPileWitchKingsCrownFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 24, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(4, LootPileWitchKingsCrown_FRAME0_LZ77, 24, 24,
                               LootPileWitchKingsCrown_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-4, 9, -10, 2)},
         .oam = {SPRITE_OAM(-5, -14, 8, 8, 0),
                 SPRITE_OAM(3, -14, 16, 8, 1),
                 SPRITE_OAM(-5, -6, 8, 16, 3),
                 SPRITE_OAM(3, -6, 16, 16, 5)}},
    },
};

// [0x06] "There and Back Again"
const u8 LootPileThereAndBackAgainTiles[] = {LootPileThereAndBackAgain_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileThereAndBackAgainFrames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileThereAndBackAgain_FRAME0_LZ77, 32, 16,
                               LootPileThereAndBackAgain_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-9, 10, -6, 6)},
         .oam = {SPRITE_OAM(-11, -3, 32, 16, 0)}},
    },
};

// [0x07] "Simbelmyne"
const u8 LootPileSimbelmyneTiles[] = {LootPileSimbelmyne_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileSimbelmyneFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileSimbelmyne_FRAME0_LZ77, 24, 16,
                               LootPileSimbelmyne_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-10, 15, -6, 6)},
         .oam = {SPRITE_OAM(-8, -4, 8, 16, 0), SPRITE_OAM(0, -4, 16, 16, 2)}},
    },
};

// [0x08] "Dwarven Jug"
const u8 LootPileDwarvenJugTiles[] = {LootPileDwarvenJug_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileDwarvenJugFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 32, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileDwarvenJug_FRAME0_LZ77, 24, 32,
                               LootPileDwarvenJug_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-6, 12, -11, 14)},
         .oam = {SPRITE_OAM(-6, -11, 8, 32, 0), SPRITE_OAM(2, -11, 16, 32, 4)}},
    },
};

// [0x09] "Gondor Tree Crest"
const u8 LootPileGondorTreeCrestTiles[] = {LootPileGondorTreeCrest_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileGondorTreeCrestFrames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 32, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileGondorTreeCrest_FRAME0_LZ77, 32, 32,
                               LootPileGondorTreeCrest_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-15, 15, -13, 13)},
         .oam = {SPRITE_OAM(-15, -11, 32, 32, 0)}},
    },
};

// [0x0a] "Lantern"
const u8 LootPileLanternTiles[] = {LootPileLantern_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileLanternFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 32, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileLantern_FRAME0_LZ77, 16, 32,
                               LootPileLantern_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-9, 8, -16, 15)},
         .oam = {SPRITE_OAM(-7, -16, 16, 32, 0)}},
    },
};

// [0x0b] "Tattered Flag"
const u8 LootPileTatteredFlagTiles[] = {LootPileTatteredFlag_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileTatteredFlagFrames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 24, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileTatteredFlag_FRAME0_LZ77, 32, 24,
                               LootPileTatteredFlag_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-13, 14, -13, 13)},
         .oam = {SPRITE_OAM(-13, -10, 32, 8, 0), SPRITE_OAM(-13, -2, 32, 16, 4)}},
    },
};

// [0x0c] "White Hand Crest"
const u8 LootPileWhiteHandCrestTiles[] = {LootPileWhiteHandCrest_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileWhiteHandCrestFrames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 32, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileWhiteHandCrest_FRAME0_LZ77, 32, 32,
                               LootPileWhiteHandCrest_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-16, 15, -16, 13)},
         .oam = {SPRITE_OAM(-16, -16, 32, 32, 0)}},
    },
};

// [0x0d] "A Gem"
const u8 LootPileGemStackMinTiles[] = {LootPileGemStackMin_TILES};
const SPRITE_FRAME_SET_STRUCT(5, 1, 1) LootPileGemStackMinFrames = {
    .header = SPRITE_FRAME_SET_HEADER(8, 8, 5, 1),
    .frameOffsets = {10, 28, 46, 64, 82},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileGemStackMin_FRAME0_LZ77, 8, 8,
                               LootPileGemStackMin_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-3, 3, -4, 2)},
         .oam = {SPRITE_OAM(-3, -4, 8, 8, 0)}},
        {.frame = SPRITE_FRAME(1, LootPileGemStackMin_FRAME1_LZ77, 8, 8,
                               LootPileGemStackMin_FRAME1_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-3, 3, -4, 2)},
         .oam = {SPRITE_OAM(-3, -4, 8, 8, 0)}},
        {.frame = SPRITE_FRAME(1, LootPileGemStackMin_FRAME2_LZ77, 8, 8,
                               LootPileGemStackMin_FRAME2_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-3, 3, -4, 2)},
         .oam = {SPRITE_OAM(-3, -4, 8, 8, 0)}},
        {.frame = SPRITE_FRAME(1, LootPileGemStackMin_FRAME3_LZ77, 8, 8,
                               LootPileGemStackMin_FRAME3_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-3, 3, -4, 2)},
         .oam = {SPRITE_OAM(-3, -4, 8, 8, 0)}},
        {.frame = SPRITE_FRAME(1, LootPileGemStackMin_FRAME4_LZ77, 8, 8,
                               LootPileGemStackMin_FRAME4_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-3, 3, -4, 2)},
         .oam = {SPRITE_OAM(-3, -4, 8, 8, 0)}},
    },
};

// [0x0e] "A Pile of @1 Gems"
const u8 LootPileGemStackTiles[] = {LootPileGemStack_TILES};
const SPRITE_FRAME_SET_STRUCT(5, 1, 1) LootPileGemStackFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 5, 1),
    .frameOffsets = {10, 28, 46, 64, 82},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileGemStack_FRAME0_LZ77, 16, 16,
                               LootPileGemStack_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-6, 6, -6, 4)},
         .oam = {SPRITE_OAM(-8, -6, 16, 16, 0)}},
        {.frame = SPRITE_FRAME(1, LootPileGemStack_FRAME1_LZ77, 16, 16,
                               LootPileGemStack_FRAME1_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-6, 6, -6, 4)},
         .oam = {SPRITE_OAM(-8, -6, 16, 16, 0)}},
        {.frame = SPRITE_FRAME(1, LootPileGemStack_FRAME2_LZ77, 16, 16,
                               LootPileGemStack_FRAME2_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-6, 6, -6, 4)},
         .oam = {SPRITE_OAM(-8, -6, 16, 16, 0)}},
        {.frame = SPRITE_FRAME(1, LootPileGemStack_FRAME3_LZ77, 16, 16,
                               LootPileGemStack_FRAME3_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-6, 6, -6, 4)},
         .oam = {SPRITE_OAM(-8, -6, 16, 16, 0)}},
        {.frame = SPRITE_FRAME(1, LootPileGemStack_FRAME4_LZ77, 16, 16,
                               LootPileGemStack_FRAME4_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-6, 6, -6, 4)},
         .oam = {SPRITE_OAM(-8, -6, 16, 16, 0)}},
    },
};

// [0x0f] "Some Fruit"
const u8 LootPileFruitTiles[] = {LootPileFruit_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileFruitFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileFruit_FRAME0_LZ77, 16, 16,
                               LootPileFruit_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-7, 7, -8, 4)},
         .oam = {SPRITE_OAM(-6, -7, 16, 16, 0)}},
    },
};

// [0x10] "Some Mushrooms"
const u8 LootPileMushroomsTiles[] = {LootPileMushrooms_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileMushroomsFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileMushrooms_FRAME0_LZ77, 16, 16,
                               LootPileMushrooms_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-6, 7, -11, 3)},
         .oam = {SPRITE_OAM(-7, -11, 16, 16, 0)}},
    },
};

// [0x11] "Dried Kingsfoil Herbs" (also 0x14, 0x15)
const u8 LootPileDriedKingsfoilTiles[] = {LootPileDriedKingsfoil_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileDriedKingsfoilFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileDriedKingsfoil_FRAME0_LZ77, 24, 16,
                               LootPileDriedKingsfoil_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-8, 7, -4, 5)},
         .oam = {SPRITE_OAM(-9, -3, 8, 16, 0), SPRITE_OAM(-1, -3, 16, 16, 2)}},
    },
};

// [0x12] "Fresh Kingsfoil Herbs" (also 0x16, 0x17, 0x18)
const u8 LootPileFreshKingsfoilTiles[] = {LootPileFreshKingsfoil_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileFreshKingsfoilFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileFreshKingsfoil_FRAME0_LZ77, 16, 16,
                               LootPileFreshKingsfoil_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-6, 9, -7, 6)},
         .oam = {SPRITE_OAM(-6, -7, 16, 16, 0)}},
    },
};

// [0x13] "Lembas Bread"
const u8 LootPileLembasBreadTiles[] = {LootPileLembasBread_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileLembasBreadFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileLembasBread_FRAME0_LZ77, 16, 16,
                               LootPileLembasBread_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-7, 6, -6, 6)},
         .oam = {SPRITE_OAM(-7, -4, 16, 16, 0)}},
    },
};

// [0x19] "LOOT_TYPE_BOW_TIER_1"
const u8 LootPileBowTier1Tiles[] = {LootPileBowTier1_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileBowTier1Frames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileBowTier1_FRAME0_LZ77, 24, 16,
                               LootPileBowTier1_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-11, 11, -7, 7)},
         .oam = {SPRITE_OAM(-11, -7, 8, 16, 0), SPRITE_OAM(-3, -7, 16, 16, 2)}},
    },
};

// [0x1a] "LOOT_TYPE_BOW_TIER_2"
const u8 LootPileBowTier2Tiles[] = {LootPileBowTier2_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileBowTier2Frames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileBowTier2_FRAME0_LZ77, 24, 16,
                               LootPileBowTier2_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-12, 13, -5, 6)},
         .oam = {SPRITE_OAM(-11, -5, 8, 16, 0), SPRITE_OAM(-3, -5, 16, 16, 2)}},
    },
};

// [0x1b] "LOOT_TYPE_ARROW"
const u8 LootPileArrowTiles[] = {LootPileArrow_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileArrowFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileArrow_FRAME0_LZ77, 16, 16,
                               LootPileArrow_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-8, 7, -6, 7)},
         .oam = {SPRITE_OAM(-8, -6, 16, 16, 0)}},
    },
};

// [0x1c] "LOOT_TYPE_KNIFE_TIER_2"
const u8 LootPileKnifeTier2Tiles[] = {LootPileKnifeTier2_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileKnifeTier2Frames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileKnifeTier2_FRAME0_LZ77, 24, 16,
                               LootPileKnifeTier2_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-9, 8, -3, 4)},
         .oam = {SPRITE_OAM(-9, -3, 8, 16, 0), SPRITE_OAM(-1, -3, 16, 16, 2)}},
    },
};

// [0x1d] "LOOT_TYPE_SWORD_ONE_HAND"
const u8 LootPileSwordOneHandTiles[] = {LootPileSwordOneHand_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileSwordOneHandFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileSwordOneHand_FRAME0_LZ77, 24, 16,
                               LootPileSwordOneHand_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-13, 11, -5, 4)},
         .oam = {SPRITE_OAM(-12, -5, 8, 16, 0), SPRITE_OAM(-4, -5, 16, 16, 2)}},
    },
};

// [0x1e] "LOOT_TYPE_SWORD_TWO_HAND_TIER_1"
const u8 LootPileSwordTwoHandTier1Tiles[] = {LootPileSwordTwoHandTier1_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileSwordTwoHandTier1Frames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileSwordTwoHandTier1_FRAME0_LZ77, 24, 16,
                               LootPileSwordTwoHandTier1_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-10, 12, -7, 7)},
         .oam = {SPRITE_OAM(-10, -7, 8, 16, 0), SPRITE_OAM(-2, -7, 16, 16, 2)}},
    },
};

// [0x1f] "LOOT_TYPE_SWORD_TWO_HAND_TIER_2"
const u8 LootPileSwordTwoHandTier2Tiles[] = {LootPileSwordTwoHandTier2_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileSwordTwoHandTier2Frames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileSwordTwoHandTier2_FRAME0_LZ77, 32, 16,
                               LootPileSwordTwoHandTier2_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-14, 16, -6, 4)},
         .oam = {SPRITE_OAM(-15, -5, 32, 16, 0)}},
    },
};

// [0x20] "LOOT_TYPE_AXE_TIER_1"
const u8 LootPileAxeTier1Tiles[] = {LootPileAxeTier1_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileAxeTier1Frames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileAxeTier1_FRAME0_LZ77, 16, 16,
                               LootPileAxeTier1_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-7, 8, -6, 2)},
         .oam = {SPRITE_OAM(-7, -6, 16, 16, 0)}},
    },
};

// [0x21] "LOOT_TYPE_AXE_TIER_2"
const u8 LootPileAxeTier2Tiles[] = {LootPileAxeTier2_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileAxeTier2Frames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileAxeTier2_FRAME0_LZ77, 24, 16,
                               LootPileAxeTier2_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-13, 10, -6, 5)},
         .oam = {SPRITE_OAM(-12, -6, 8, 16, 0), SPRITE_OAM(-4, -6, 16, 16, 2)}},
    },
};

// [0x22] "LOOT_TYPE_STAFF_TIER_1"
const u8 LootPileStaffTier1Tiles[] = {LootPileStaffTier1_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileStaffTier1Frames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileStaffTier1_FRAME0_LZ77, 24, 16,
                               LootPileStaffTier1_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-11, 11, -7, 7)},
         .oam = {SPRITE_OAM(-11, -6, 8, 16, 0), SPRITE_OAM(-3, -6, 16, 16, 2)}},
    },
};

// [0x23] "LOOT_TYPE_STAFF_TIER_2"
const u8 LootPileStaffTier2Tiles[] = {LootPileStaffTier2_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileStaffTier2Frames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileStaffTier2_FRAME0_LZ77, 32, 16,
                               LootPileStaffTier2_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-11, 12, -8, 6)},
         .oam = {SPRITE_OAM(-12, -7, 32, 16, 0)}},
    },
};

// [0x24] "LOOT_TYPE_KNIFE_TIER_1"
const u8 LootPileKnifeTier1Tiles[] = {LootPileKnifeTier1_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileKnifeTier1Frames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileKnifeTier1_FRAME0_LZ77, 16, 16,
                               LootPileKnifeTier1_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-6, 6, -5, 2)},
         .oam = {SPRITE_OAM(-7, -5, 16, 16, 0)}},
    },
};

// [0x25] "LOOT_TYPE_SPOON"
const u8 LootPileSpoonTiles[] = {LootPileSpoon_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileSpoonFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileSpoon_FRAME0_LZ77, 16, 16,
                               LootPileSpoon_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-7, 6, -5, 5)},
         .oam = {SPRITE_OAM(-7, -5, 16, 16, 0)}},
    },
};

// [0x26] "LOOT_TYPE_ARMOR_TIER_1"
const u8 LootPileArmorTier1Tiles[] = {LootPileArmorTier1_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileArmorTier1Frames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileArmorTier1_FRAME0_LZ77, 24, 16,
                               LootPileArmorTier1_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-9, 8, -6, 5)},
         .oam = {SPRITE_OAM(-11, -7, 8, 16, 0), SPRITE_OAM(-3, -7, 16, 16, 2)}},
    },
};

// [0x27] "LOOT_TYPE_ARMOR_TIER_2"
const u8 LootPileArmorTier2Tiles[] = {LootPileArmorTier2_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileArmorTier2Frames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileArmorTier2_FRAME0_LZ77, 24, 16,
                               LootPileArmorTier2_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-10, 11, -7, 5)},
         .oam = {SPRITE_OAM(-10, -7, 8, 16, 0), SPRITE_OAM(-2, -7, 16, 16, 2)}},
    },
};

// [0x28] "LOOT_TYPE_ARMOR_TIER_3"
const u8 LootPileArmorTier3Tiles[] = {LootPileArmorTier3_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileArmorTier3Frames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileArmorTier3_FRAME0_LZ77, 24, 16,
                               LootPileArmorTier3_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-8, 11, -8, 6)},
         .oam = {SPRITE_OAM(-10, -8, 8, 16, 0), SPRITE_OAM(-2, -8, 16, 16, 2)}},
    },
};

// [0x29] "LOOT_TYPE_CLOAK_TIER_1"
const u8 LootPileCloakTier1Tiles[] = {LootPileCloakTier1_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileCloakTier1Frames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 24, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileCloakTier1_FRAME0_LZ77, 32, 24,
                               LootPileCloakTier1_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-13, 14, -9, 7)},
         .oam = {SPRITE_OAM(-15, -8, 32, 8, 0), SPRITE_OAM(-15, 0, 32, 16, 4)}},
    },
};

// [0x2a] "LOOT_TYPE_CLOAK_TIER_2"
const u8 LootPileCloakTier2Tiles[] = {LootPileCloakTier2_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileCloakTier2Frames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 24, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileCloakTier2_FRAME0_LZ77, 32, 24,
                               LootPileCloakTier2_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-15, 14, -8, 8)},
         .oam = {SPRITE_OAM(-16, -8, 32, 8, 0), SPRITE_OAM(-16, 0, 32, 16, 4)}},
    },
};

// [0x2b] "LOOT_TYPE_CLOAK_TIER_3"
const u8 LootPileCloakTier3Tiles[] = {LootPileCloakTier3_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileCloakTier3Frames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 24, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileCloakTier3_FRAME0_LZ77, 32, 24,
                               LootPileCloakTier3_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-14, 14, -8, 7)},
         .oam = {SPRITE_OAM(-14, -9, 32, 8, 0), SPRITE_OAM(-14, -1, 32, 16, 4)}},
    },
};

// [0x2c] "LOOT_TYPE_SHIELD_TIER_1"
const u8 LootPileShieldTier1Tiles[] = {LootPileShieldTier1_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileShieldTier1Frames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileShieldTier1_FRAME0_LZ77, 24, 16,
                               LootPileShieldTier1_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-8, 7, -5, 5)},
         .oam = {SPRITE_OAM(-9, -5, 8, 16, 0), SPRITE_OAM(-1, -5, 16, 16, 2)}},
    },
};

// [0x2d] "LOOT_TYPE_SHIELD_TIER_2"
const u8 LootPileShieldTier2Tiles[] = {LootPileShieldTier2_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileShieldTier2Frames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileShieldTier2_FRAME0_LZ77, 24, 16,
                               LootPileShieldTier2_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-11, 9, -6, 5)},
         .oam = {SPRITE_OAM(-11, -6, 8, 16, 0), SPRITE_OAM(-3, -6, 16, 16, 2)}},
    },
};

// [0x2e] "LOOT_TYPE_SHOE_TIER_1"
const u8 LootPileShoeTier1Tiles[] = {LootPileShoeTier1_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileShoeTier1Frames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileShoeTier1_FRAME0_LZ77, 16, 16,
                               LootPileShoeTier1_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-8, 7, -6, 5)},
         .oam = {SPRITE_OAM(-7, -6, 16, 16, 0)}},
    },
};

// [0x2f] "LOOT_TYPE_SHOE_TIER_2"
const u8 LootPileShoeTier2Tiles[] = {LootPileShoeTier2_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileShoeTier2Frames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileShoeTier2_FRAME0_LZ77, 24, 16,
                               LootPileShoeTier2_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-7, 6, -8, 5)},
         .oam = {SPRITE_OAM(-9, -8, 8, 16, 0), SPRITE_OAM(-1, -8, 16, 16, 2)}},
    },
};

// [0x30] "LOOT_TYPE_ACCESSORY_TRINKET"
const u8 LootPileAccessoryTrinketTiles[] = {LootPileAccessoryTrinket_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileAccessoryTrinketFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileAccessoryTrinket_FRAME0_LZ77, 16, 16,
                               LootPileAccessoryTrinket_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-5, 5, -5, 5)},
         .oam = {SPRITE_OAM(-6, -5, 16, 16, 0)}},
    },
};

// [0x31] "LOOT_TYPE_ACCESSORY_AMULET"
const u8 LootPileAccessoryAmuletTiles[] = {LootPileAccessoryAmulet_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileAccessoryAmuletFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileAccessoryAmulet_FRAME0_LZ77, 16, 16,
                               LootPileAccessoryAmulet_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-8, 8, -6, 6)},
         .oam = {SPRITE_OAM(-7, -7, 16, 16, 0)}},
    },
};

// [0x32] "LOOT_TYPE_ACCESSORY_COLLAR"
const u8 LootPileAccessoryCollarTiles[] = {LootPileAccessoryCollar_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileAccessoryCollarFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileAccessoryCollar_FRAME0_LZ77, 16, 16,
                               LootPileAccessoryCollar_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-6, 5, -4, 4)},
         .oam = {SPRITE_OAM(-7, -4, 16, 16, 0)}},
    },
};

// [0x33] "LOOT_TYPE_HELM_CROWN"
const u8 LootPileHelmCrownTiles[] = {LootPileHelmCrown_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileHelmCrownFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileHelmCrown_FRAME0_LZ77, 16, 16,
                               LootPileHelmCrown_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-7, 6, -5, 5)},
         .oam = {SPRITE_OAM(-7, -5, 16, 16, 0)}},
    },
};

// [0x34] "LOOT_TYPE_HELM_HOOD"
const u8 LootPileHelmHoodTiles[] = {LootPileHelmHood_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileHelmHoodFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileHelmHood_FRAME0_LZ77, 16, 16,
                               LootPileHelmHood_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-6, 6, -8, 4)},
         .oam = {SPRITE_OAM(-6, -7, 16, 16, 0)}},
    },
};

// [0x35] "LOOT_TYPE_HELM_METAL"
const u8 LootPileHelmMetalTiles[] = {LootPileHelmMetal_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileHelmMetalFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileHelmMetal_FRAME0_LZ77, 16, 16,
                               LootPileHelmMetal_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-7, 6, -9, 5)},
         .oam = {SPRITE_OAM(-8, -9, 16, 16, 0)}},
    },
};

// [0x36] "LOOT_TYPE_UNIQUE_MELEE"
const u8 LootPileUniqueMeleeTiles[] = {LootPileUniqueMelee_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileUniqueMeleeFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileUniqueMelee_FRAME0_LZ77, 24, 16,
                               LootPileUniqueMelee_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-10, 10, -6, 5)},
         .oam = {SPRITE_OAM(-11, -6, 8, 16, 0), SPRITE_OAM(-3, -6, 16, 16, 2)}},
    },
};

// [0x37] "LOOT_TYPE_UNIQUE_BOW"
const u8 LootPileUniqueBowTiles[] = {LootPileUniqueBow_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileUniqueBowFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileUniqueBow_FRAME0_LZ77, 24, 16,
                               LootPileUniqueBow_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-10, 13, -5, 4)},
         .oam = {SPRITE_OAM(-10, -7, 8, 16, 0), SPRITE_OAM(-2, -7, 16, 16, 2)}},
    },
};

// [0x38] "LOOT_TYPE_UNIQUE_STAFF"
const u8 LootPileUniqueStaffTiles[] = {LootPileUniqueStaff_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileUniqueStaffFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileUniqueStaff_FRAME0_LZ77, 24, 16,
                               LootPileUniqueStaff_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-10, 11, -8, 4)},
         .oam = {SPRITE_OAM(-10, -8, 8, 16, 0), SPRITE_OAM(-2, -8, 16, 16, 2)}},
    },
};

// [0x39] "LOOT_TYPE_UNIQUE_SWORD"
const u8 LootPileUniqueSwordTiles[] = {LootPileUniqueSword_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileUniqueSwordFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileUniqueSword_FRAME0_LZ77, 24, 16,
                               LootPileUniqueSword_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-10, 12, -8, 6)},
         .oam = {SPRITE_OAM(-10, -8, 8, 16, 0), SPRITE_OAM(-2, -8, 16, 16, 2)}},
    },
};

// [0x3a] "LOOT_TYPE_GLOVE"
const u8 LootPileGloveTiles[] = {LootPileGlove_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileGloveFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileGlove_FRAME0_LZ77, 16, 16,
                               LootPileGlove_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-10, 12, -6, 8)},
         .oam = {SPRITE_OAM(-5, -3, 16, 16, 0)}},
    },
};

// [0x3b] "LOOT_TYPE_CLUB"
const u8 LootPileClubTiles[] = {LootPileClub_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileClubFrames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileClub_FRAME0_LZ77, 32, 16,
                               LootPileClub_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-13, 14, -7, 8)},
         .oam = {SPRITE_OAM(-13, -5, 32, 16, 0)}},
    },
};

// [0x3c] "LOOT_TYPE_MACE"
const u8 LootPileMaceTiles[] = {LootPileMace_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileMaceFrames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileMace_FRAME0_LZ77, 32, 16,
                               LootPileMace_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-12, 15, -10, 7)},
         .oam = {SPRITE_OAM(-12, -9, 32, 16, 0)}},
    },
};

// [0x3d] "Sting"
const u8 LootPileStingTiles[] = {LootPileSting_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileStingFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileSting_FRAME0_LZ77, 24, 16,
                               LootPileSting_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-11, 8, -4, 5)},
         .oam = {SPRITE_OAM(-10, -4, 8, 16, 0), SPRITE_OAM(-2, -4, 16, 16, 2)}},
    },
};

// [0x3e] "Symbolblade"
const u8 LootPileSymbolbladeTiles[] = {LootPileSymbolblade_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileSymbolbladeFrames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 24, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileSymbolblade_FRAME0_LZ77, 32, 24,
                               LootPileSymbolblade_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-13, 10, -7, 6)},
         .oam = {SPRITE_OAM(-14, -8, 32, 8, 0), SPRITE_OAM(-14, 0, 32, 16, 4)}},
    },
};

// [0x3f] "Glamdring"
const u8 LootPileGlamdringTiles[] = {LootPileGlamdring_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileGlamdringFrames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 24, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileGlamdring_FRAME0_LZ77, 32, 24,
                               LootPileGlamdring_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-13, 11, -8, 6)},
         .oam = {SPRITE_OAM(-12, -9, 32, 8, 0), SPRITE_OAM(-12, -1, 32, 16, 4)}},
    },
};

// [0x40] "Orcrist"
const u8 LootPileOrcristTiles[] = {LootPileOrcrist_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileOrcristFrames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileOrcrist_FRAME0_LZ77, 32, 16,
                               LootPileOrcrist_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-14, 10, -3, 6)},
         .oam = {SPRITE_OAM(-13, -4, 32, 16, 0)}},
    },
};

// [0x41] "Narsil"
const u8 LootPileNarsilTiles[] = {LootPileNarsil_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileNarsilFrames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileNarsil_FRAME0_LZ77, 32, 16,
                               LootPileNarsil_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-13, 11, -5, 8)},
         .oam = {SPRITE_OAM(-13, -4, 32, 16, 0)}},
    },
};

// [0x42] "Hadhafang"
const u8 LootPileHadhafangTiles[] = {LootPileHadhafang_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileHadhafangFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 8, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileHadhafang_FRAME0_LZ77, 24, 8,
                               LootPileHadhafang_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-11, 9, -6, 3)},
         .oam = {SPRITE_OAM(-11, -4, 8, 8, 0), SPRITE_OAM(-3, -4, 16, 8, 1)}},
    },
};

// [0x43] "Silveraxe"
const u8 LootPileSilveraxeTiles[] = {LootPileSilveraxe_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileSilveraxeFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileSilveraxe_FRAME0_LZ77, 24, 16,
                               LootPileSilveraxe_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-12, 10, -6, 5)},
         .oam = {SPRITE_OAM(-13, -6, 8, 16, 0), SPRITE_OAM(-5, -6, 16, 16, 2)}},
    },
};

// [0x44] "Staff of Five Mages"
const u8 LootPileStaffOfFiveMagesTiles[] = {LootPileStaffOfFiveMages_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileStaffOfFiveMagesFrames = {
    .header = SPRITE_FRAME_SET_HEADER(32, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileStaffOfFiveMages_FRAME0_LZ77, 32, 16,
                               LootPileStaffOfFiveMages_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-11, 12, -8, 6)},
         .oam = {SPRITE_OAM(-12, -7, 32, 16, 0)}},
    },
};

// [0x45] "Starfall Bow"
const u8 LootPileStarfallBowTiles[] = {LootPileStarfallBow_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileStarfallBowFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileStarfallBow_FRAME0_LZ77, 24, 16,
                               LootPileStarfallBow_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-12, 10, -8, 6)},
         .oam = {SPRITE_OAM(-12, -8, 8, 16, 0), SPRITE_OAM(-4, -8, 16, 16, 2)}},
    },
};

// [0x46] "Nightfell Bow"
const u8 LootPileNightfellBowTiles[] = {LootPileNightfellBow_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileNightfellBowFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileNightfellBow_FRAME0_LZ77, 24, 16,
                               LootPileNightfellBow_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-12, 11, -5, 6)},
         .oam = {SPRITE_OAM(-12, -7, 8, 16, 0), SPRITE_OAM(-4, -7, 16, 16, 2)}},
    },
};

// [0x47] "Celeborn's Phial" (also 0x4e)
const u8 LootPileCelebornsPhialTiles[] = {LootPileCelebornsPhial_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileCelebornsPhialFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileCelebornsPhial_FRAME0_LZ77, 16, 16,
                               LootPileCelebornsPhial_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-4, 3, -7, 4)},
         .oam = {SPRITE_OAM(-5, -7, 16, 16, 0)}},
    },
};

// [0x48] "Boromir's Horn"
const u8 LootPileBoromirsHornTiles[] = {LootPileBoromirsHorn_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileBoromirsHornFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileBoromirsHorn_FRAME0_LZ77, 24, 16,
                               LootPileBoromirsHorn_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-10, 7, -7, 5)},
         .oam = {SPRITE_OAM(-10, -7, 8, 16, 0), SPRITE_OAM(-2, -7, 16, 16, 2)}},
    },
};

// [0x49] "Cyclopedia of Herbs"
const u8 LootPileCyclopediaOfHerbsTiles[] = {LootPileCyclopediaOfHerbs_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileCyclopediaOfHerbsFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileCyclopediaOfHerbs_FRAME0_LZ77, 24, 16,
                               LootPileCyclopediaOfHerbs_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-7, 7, -7, 6)},
         .oam = {SPRITE_OAM(-9, -7, 8, 16, 0), SPRITE_OAM(-1, -7, 16, 16, 2)}},
    },
};

// [0x4a] "Sam's Cookpot"
const u8 LootPileSamsCookpotTiles[] = {LootPileSamsCookpot_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileSamsCookpotFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileSamsCookpot_FRAME0_LZ77, 16, 16,
                               LootPileSamsCookpot_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-9, 6, -5, 5)},
         .oam = {SPRITE_OAM(-9, -4, 16, 16, 0)}},
    },
};

// [0x4b] "Fletching Kit"
const u8 LootPileFletchingKitTiles[] = {LootPileFletchingKit_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileFletchingKitFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileFletchingKit_FRAME0_LZ77, 16, 16,
                               LootPileFletchingKit_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-8, 6, -7, 5)},
         .oam = {SPRITE_OAM(-9, -8, 16, 16, 0)}},
    },
};

// [0x4c] "Scrolls of Minas Tirith"
const u8 LootPileScrollsOfMinasTirithTiles[] = {LootPileScrollsOfMinasTirith_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileScrollsOfMinasTirithFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileScrollsOfMinasTirith_FRAME0_LZ77, 24, 16,
                               LootPileScrollsOfMinasTirith_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-8, 7, -7, 6)},
         .oam = {SPRITE_OAM(-9, -7, 8, 16, 0), SPRITE_OAM(-1, -7, 16, 16, 2)}},
    },
};

// [0x4d] "Evenstar of Arwen"
const u8 LootPileEvenstarOfArwenTiles[] = {LootPileEvenstarOfArwen_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileEvenstarOfArwenFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileEvenstarOfArwen_FRAME0_LZ77, 24, 16,
                               LootPileEvenstarOfArwen_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-8, 9, -6, 9)},
         .oam = {SPRITE_OAM(-9, -5, 8, 16, 0), SPRITE_OAM(-1, -5, 16, 16, 2)}},
    },
};

// [0x4f] "Forgotten Palantir"
const u8 LootPileForgottenPalantirTiles[] = {LootPileForgottenPalantir_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileForgottenPalantirFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 8, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileForgottenPalantir_FRAME0_LZ77, 16, 8,
                               LootPileForgottenPalantir_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-4, 5, -2, 5)},
         .oam = {SPRITE_OAM(-4, -2, 16, 8, 0)}},
    },
};

// [0x50] "Cracked Palantir"
const u8 LootPileCrackedPalantirTiles[] = {LootPileCrackedPalantir_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileCrackedPalantirFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileCrackedPalantir_FRAME0_LZ77, 16, 16,
                               LootPileCrackedPalantir_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-4, 4, -4, 4)},
         .oam = {SPRITE_OAM(-5, -3, 16, 16, 0)}},
    },
};

// [0x51] "Orc Drum"
const u8 LootPileOrcDrumTiles[] = {LootPileOrcDrum_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileOrcDrumFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileOrcDrum_FRAME0_LZ77, 16, 16,
                               LootPileOrcDrum_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-5, 5, -8, 4)},
         .oam = {SPRITE_OAM(-6, -8, 16, 16, 0)}},
    },
};

// [0x52] "Woven Goblin Doll"
const u8 LootPileWovenGoblinDollTiles[] = {LootPileWovenGoblinDoll_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileWovenGoblinDollFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileWovenGoblinDoll_FRAME0_LZ77, 16, 16,
                               LootPileWovenGoblinDoll_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-5, 6, -8, 9)},
         .oam = {SPRITE_OAM(-4, -6, 16, 16, 0)}},
    },
};

// [0x53] "Oliphaunt Ivory Statuette"
const u8 LootPileOliphauntIvoryStatuetteTiles[] = {LootPileOliphauntIvoryStatuette_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileOliphauntIvoryStatuetteFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileOliphauntIvoryStatuette_FRAME0_LZ77, 16, 16,
                               LootPileOliphauntIvoryStatuette_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-8, 6, -7, 5)},
         .oam = {SPRITE_OAM(-7, -9, 16, 16, 0)}},
    },
};

// [0x54] "Orc Head"
const u8 LootPileOrcHeadTiles[] = {LootPileOrcHead_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileOrcHeadFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 32, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileOrcHead_FRAME0_LZ77, 24, 32,
                               LootPileOrcHead_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-7, 6, -9, 8)},
         .oam = {SPRITE_OAM(-8, -17, 8, 32, 0), SPRITE_OAM(0, -17, 16, 32, 4)}},
    },
};

// [0x55] "Swollen Spider Glands"
const u8 LootPileSwollenSpiderGlandsTiles[] = {LootPileSwollenSpiderGlands_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileSwollenSpiderGlandsFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileSwollenSpiderGlands_FRAME0_LZ77, 24, 16,
                               LootPileSwollenSpiderGlands_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-7, 8, -6, 6)},
         .oam = {SPRITE_OAM(-9, -6, 8, 16, 0), SPRITE_OAM(-1, -6, 16, 16, 2)}},
    },
};

// [0x56] "Featherwood Flute"
const u8 LootPileFeatherwoodFluteTiles[] = {LootPileFeatherwoodFlute_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 2) LootPileFeatherwoodFluteFrames = {
    .header = SPRITE_FRAME_SET_HEADER(24, 8, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(2, LootPileFeatherwoodFlute_FRAME0_LZ77, 24, 8,
                               LootPileFeatherwoodFlute_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-13, 9, -2, 3)},
         .oam = {SPRITE_OAM(-8, 0, 8, 8, 0), SPRITE_OAM(0, 0, 16, 8, 1)}},
    },
};

// [0x57] "LOOT_TYPE_ENT_WATER"
const u8 LootPileEntWaterTiles[] = {LootPileEntWater_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileEntWaterFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileEntWater_FRAME0_LZ77, 16, 16,
                               LootPileEntWater_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-6, 6, -7, 4)},
         .oam = {SPRITE_OAM(-5, -6, 16, 16, 0)}},
    },
};

// [0x58] "LOOT_TYPE_RUNE"
const u8 LootPileRuneTiles[] = {LootPileRune_TILES};
const SPRITE_FRAME_SET_STRUCT(1, 1, 1) LootPileRuneFrames = {
    .header = SPRITE_FRAME_SET_HEADER(16, 16, 1, 1),
    .frameOffsets = {2},
    .frames = {
        {.frame = SPRITE_FRAME(1, LootPileRune_FRAME0_LZ77, 16, 16,
                               LootPileRune_FRAME0_OFFSET),
         .triggerBoxes = {TRIGGER_BOX(-7, 9, -6, 7)},
         .oam = {SPRITE_OAM(-7, -5, 16, 16, 0)}},
    },
};

// clang-format on
