#include "loot.h"

// clang-format off

/** @romaddress 0x0806db94 */
const LootHealthDropTable LootHealthDropChances = {
    .count = 5,
    .field_0x2 = 0,
    .entries = {
        // [0x00]
        { .maxRoll = 45, .lootType = LOOT_TYPE_FRUIT },

        // [0x01]
        { .maxRoll = 70, .lootType = LOOT_TYPE_MUSHROOMS },

        // [0x02]
        { .maxRoll = 85, .lootType = LOOT_TYPE_DRIED_KINGSFOIL },

        // [0x03]
        { .maxRoll = 95, .lootType = LOOT_TYPE_FRESH_KINGSFOIL },

        // [0x04]
        { .maxRoll = 100, .lootType = LOOT_TYPE_LEMBAS_BREAD },
    },
};

/**
 * Per-hero 1..100 roll thresholds picking a dropped item's itemType
 * (loot_generateItem): the first threshold >= the roll wins; column i is the itemType
 * picked when threshold[i-1] < roll <= threshold[i]. Columns are the ItemType
 * values 0..0xf; byte 0xf (.empty) is never read. The end-of-line comments give
 * the absolute per-itemType drop share (threshold[i] - threshold[i-1]).
 *
 * @romaddress 0x0806dbac
 */
const LootItemTypeRolls LootHeroItemTypeRolls[8] = {
    // heroId 0 (HERO_ID_FRODO)
    { .d = { .knife = 15,      // 15%
             .sword1H = 18,    //  3%
             .sword2H = 21,    //  3%
             .axe = 31,        // 10%
             .mace = 41,       // 10%
             .staff = 44,      //  3%
             .bow = 47,        //  3%
             .arrow = 50,      //  3%
             .armor = 65,      // 15%
             .shield = 75,     // 10%
             .accessory = 83,  //  8%
             .cloak = 90,      //  7%
             .gloves = 96,     //  6%
             .shoes = 99,      //  3%
             .unique = 100,    //  1%
             .empty = 0 } },   // never read

    // heroId 1 (HERO_ID_LEGOLAS)
    { .d = { .knife = 10,      // 10%
             .sword1H = 13,    //  3%
             .sword2H = 16,    //  3%
             .axe = 19,        //  3%
             .mace = 22,       //  3%
             .staff = 25,      //  3%
             .bow = 40,        // 15%
             .arrow = 50,      // 10%
             .armor = 67,      // 17%
             .shield = 70,     //  3%
             .accessory = 79,  //  9%
             .cloak = 89,      // 10%
             .gloves = 92,     //  3%
             .shoes = 99,      //  7%
             .unique = 100,    //  1%
             .empty = 0 } },   // never read

    // heroId 2 (HERO_ID_ARAGORN)
    { .d = { .knife = 5,       //  5%
             .sword1H = 16,    // 11%
             .sword2H = 21,    //  5%
             .axe = 26,        //  5%
             .mace = 31,       //  5%
             .staff = 34,      //  3%
             .bow = 37,        //  3%
             .arrow = 40,      //  3%
             .armor = 55,      // 15%
             .shield = 70,     // 15%
             .accessory = 78,  //  8%
             .cloak = 85,      //  7%
             .gloves = 92,     //  7%
             .shoes = 99,      //  7%
             .unique = 100,    //  1%
             .empty = 0 } },   // never read

    // heroId 3 (HERO_ID_GANDALF)
    { .d = { .knife = 10,      // 10%
             .sword1H = 20,    // 10%
             .sword2H = 23,    //  3%
             .axe = 26,        //  3%
             .mace = 29,       //  3%
             .staff = 39,      // 10%
             .bow = 42,        //  3%
             .arrow = 45,      //  3%
             .armor = 60,      // 15%
             .shield = 63,     //  3%
             .accessory = 71,  //  8%
             .cloak = 80,      //  9%
             .gloves = 90,     // 10%
             .shoes = 99,      //  9%
             .unique = 100,    //  1%
             .empty = 0 } },   // never read

    // heroId 4 (HERO_ID_EOWYN)
    { .d = { .knife = 5,       //  5%
             .sword1H = 16,    // 11%
             .sword2H = 21,    //  5%
             .axe = 26,        //  5%
             .mace = 31,       //  5%
             .staff = 34,      //  3%
             .bow = 37,        //  3%
             .arrow = 40,      //  3%
             .armor = 55,      // 15%
             .shield = 70,     // 15%
             .accessory = 78,  //  8%
             .cloak = 85,      //  7%
             .gloves = 92,     //  7%
             .shoes = 99,      //  7%
             .unique = 100,    //  1%
             .empty = 0 } },   // never read

    // heroId 5 (HERO_ID_GIMLI)
    { .d = { .knife = 4,       //  4%
             .sword1H = 7,     //  3%
             .sword2H = 10,    //  3%
             .axe = 25,        // 15%
             .mace = 35,       // 10%
             .staff = 38,      //  3%
             .bow = 41,        //  3%
             .arrow = 44,      //  3%
             .armor = 59,      // 15%
             .shield = 70,     // 11%
             .accessory = 78,  //  8%
             .cloak = 85,      //  7%
             .gloves = 92,     //  7%
             .shoes = 99,      //  7%
             .unique = 100,    //  1%
             .empty = 0 } },   // never read

    // heroId 6 (HERO_ID_SAM)
    { .d = { .knife = 15,      // 15%
             .sword1H = 18,    //  3%
             .sword2H = 21,    //  3%
             .axe = 31,        // 10%
             .mace = 41,       // 10%
             .staff = 44,      //  3%
             .bow = 47,        //  3%
             .arrow = 50,      //  3%
             .armor = 65,      // 15%
             .shield = 75,     // 10%
             .accessory = 83,  //  8%
             .cloak = 90,      //  7%
             .gloves = 96,     //  6%
             .shoes = 99,      //  3%
             .unique = 100,    //  1%
             .empty = 0 } },   // never read

    // heroId 7 (HERO_ID_SMEAGOL)
    { .d = { .knife = 15,      // 15%
             .sword1H = 18,    //  3%
             .sword2H = 21,    //  3%
             .axe = 31,        // 10%
             .mace = 41,       // 10%
             .staff = 44,      //  3%
             .bow = 47,        //  3%
             .arrow = 50,      //  3%
             .armor = 65,      // 15%
             .shield = 75,     // 10%
             .accessory = 83,  //  8%
             .cloak = 90,      //  7%
             .gloves = 96,     //  6%
             .shoes = 99,      //  3%
             .unique = 100,    //  1%
             .empty = 0 } },   // never read
};

/**
 * Indexed by the region (enum RegionId); the campaign previous/next links of heroId 1 (HERO_ID_LEGOLAS), 2 (HERO_ID_ARAGORN), 5 (HERO_ID_GIMLI)
 * (via LootHeroRegionProgressions). field_0x2 is always zero.
 *
 * @romaddress 0x0806dc2c
 */
const LootRegionProgression LootRegionProgressionsAragorn[28] = {
    // [0x00] REGION_ANDUIN_RIVER "Anduin River"
    { .previous = REGION_WHITE_MOUNTAINS, .next = REGION_CORSAIR_FLAGSHIP },

    // [0x01] REGION_ORC_TOWER "Orc Tower"
    { .previous = REGION_SHELOBS_LAIR, .next = REGION_GORGOROTH },

    // [0x02] REGION_CIRITH_UNGOL "Cirith Ungol"
    { .previous = REGION_MORGUL, .next = REGION_SHELOBS_LAIR },

    // [0x03] REGION_CORSAIR_FLAGSHIP "Corsair Flagship"
    { .previous = REGION_ANDUIN_RIVER, .next = REGION_DOCKS },

    // [0x04] REGION_CRACK_OF_DOOM "Crack of Doom"
    { .previous = REGION_MT_DOOM, .next = REGION_CRACK_OF_DOOM },

    // [0x05] REGION_DIMHOLT "Dimholt"
    { .previous = REGION_DUNHARROW, .next = REGION_PATHS_OF_THE_DEAD },

    // [0x06] REGION_DOCKS "Docks"
    { .previous = REGION_CORSAIR_FLAGSHIP, .next = REGION_MINAS_TIRITH_BATTLEMENTS },

    // [0x07] REGION_MT_DOOM "Mt Doom"
    { .previous = REGION_GORGOROTH, .next = REGION_CRACK_OF_DOOM },

    // [0x08] REGION_DUNHARROW "Dunharrow"
    { .previous = REGION_ROHAN, .next = REGION_DIMHOLT },

    // [0x09] REGION_FANGORN "Entwood"
    { .previous = REGION_DUNHARROW, .next = REGION_PATHS_OF_THE_DEAD },

    // [0x0a] REGION_GORGOROTH "Gorgoroth"
    { .previous = REGION_ORC_TOWER, .next = REGION_MT_DOOM },

    // [0x0b] REGION_HELMS_DEEP "Hornburg"
    { .previous = REGION_ISENGARD, .next = REGION_ROHAN },

    // [0x0c] REGION_ICE_CAVE "Ice Cave"
    { .previous = REGION_WHITE_MOUNTAINS, .next = REGION_DIMHOLT },

    // [0x0d] REGION_ISENGARD "Isengard"
    { .previous = REGION_ISENGARD, .next = REGION_ROHAN },

    // [0x0e] REGION_ITHILIEN "Ithilien"
    { .previous = REGION_MINAS_TIRITH_BATTLEMENTS, .next = REGION_ITHILIEN },

    // [0x0f] REGION_MINAS_TIRITH_BATTLEMENTS "Minas Tirith"
    { .previous = REGION_DOCKS, .next = REGION_ITHILIEN },

    // [0x10] REGION_DENETHOR_FALL_BALCONY "Balcony"
    { .previous = REGION_DOCKS, .next = REGION_ITHILIEN },

    // [0x11] REGION_MINAS_TIRITH_TIERS "Minas Tirith"
    { .previous = REGION_DOCKS, .next = REGION_ITHILIEN },

    // [0x12] REGION_MORGUL "Morgul"
    { .previous = REGION_MORGUL, .next = REGION_CIRITH_UNGOL },

    // [0x13] REGION_MORIA "Dwarf Halls"
    { .previous = REGION_ISENGARD, .next = REGION_WHITE_MOUNTAINS },

    // [0x14] REGION_PATHS_OF_THE_DEAD "Paths of the Dead"
    { .previous = REGION_DIMHOLT, .next = REGION_WHITE_MOUNTAINS },

    // [0x15] REGION_PELENNOR_BEACON "Pelennor Beacon"
    { .previous = REGION_PELENNOR_BATTLE, .next = REGION_MINAS_TIRITH_BATTLEMENTS },

    // [0x16] REGION_PELENNOR_BATTLE "Pelennor Battle"
    { .previous = REGION_PELENNOR_FIELDS, .next = REGION_PELENNOR_BATTLE },

    // [0x17] REGION_PELENNOR_FIELDS "Pelennor Fields"
    { .previous = REGION_WHITE_MOUNTAINS, .next = REGION_MINAS_TIRITH_BATTLEMENTS },

    // [0x18] REGION_ROHAN "Rohan"
    { .previous = REGION_ISENGARD, .next = REGION_WHITE_MOUNTAINS },

    // [0x19] REGION_SHELOBS_LAIR "Shelob's Lair"
    { .previous = REGION_CIRITH_UNGOL, .next = REGION_ORC_TOWER },

    // [0x1a] REGION_WEATHERTOP "Amon Sul"
    { .previous = REGION_MINAS_TIRITH_BATTLEMENTS, .next = REGION_ITHILIEN },

    // [0x1b] REGION_WHITE_MOUNTAINS "White Mountains"
    { .previous = REGION_PATHS_OF_THE_DEAD, .next = REGION_ANDUIN_RIVER },
};

/**
 * Indexed by the region (enum RegionId); the campaign previous/next links of heroId 4 (HERO_ID_EOWYN)
 * (via LootHeroRegionProgressions). field_0x2 is always zero.
 *
 * @romaddress 0x0806dc9c
 */
const LootRegionProgression LootRegionProgressionsEowyn[28] = {
    // [0x00] REGION_ANDUIN_RIVER "Anduin River"
    { .previous = REGION_WHITE_MOUNTAINS, .next = REGION_CORSAIR_FLAGSHIP },

    // [0x01] REGION_ORC_TOWER "Orc Tower"
    { .previous = REGION_SHELOBS_LAIR, .next = REGION_GORGOROTH },

    // [0x02] REGION_CIRITH_UNGOL "Cirith Ungol"
    { .previous = REGION_MORGUL, .next = REGION_SHELOBS_LAIR },

    // [0x03] REGION_CORSAIR_FLAGSHIP "Corsair Flagship"
    { .previous = REGION_ANDUIN_RIVER, .next = REGION_DOCKS },

    // [0x04] REGION_CRACK_OF_DOOM "Crack of Doom"
    { .previous = REGION_MT_DOOM, .next = REGION_CRACK_OF_DOOM },

    // [0x05] REGION_DIMHOLT "Dimholt"
    { .previous = REGION_DUNHARROW, .next = REGION_PATHS_OF_THE_DEAD },

    // [0x06] REGION_DOCKS "Docks"
    { .previous = REGION_CORSAIR_FLAGSHIP, .next = REGION_MINAS_TIRITH_BATTLEMENTS },

    // [0x07] REGION_MT_DOOM "Mt Doom"
    { .previous = REGION_GORGOROTH, .next = REGION_CRACK_OF_DOOM },

    // [0x08] REGION_DUNHARROW "Dunharrow"
    { .previous = REGION_ROHAN, .next = REGION_WHITE_MOUNTAINS },

    // [0x09] REGION_FANGORN "Entwood"
    { .previous = REGION_DUNHARROW, .next = REGION_PATHS_OF_THE_DEAD },

    // [0x0a] REGION_GORGOROTH "Gorgoroth"
    { .previous = REGION_ORC_TOWER, .next = REGION_MT_DOOM },

    // [0x0b] REGION_HELMS_DEEP "Hornburg"
    { .previous = REGION_ISENGARD, .next = REGION_ROHAN },

    // [0x0c] REGION_ICE_CAVE "Ice Cave"
    { .previous = REGION_WHITE_MOUNTAINS, .next = REGION_DIMHOLT },

    // [0x0d] REGION_ISENGARD "Isengard"
    { .previous = REGION_ISENGARD, .next = REGION_ROHAN },

    // [0x0e] REGION_ITHILIEN "Ithilien"
    { .previous = REGION_MINAS_TIRITH_BATTLEMENTS, .next = REGION_ITHILIEN },

    // [0x0f] REGION_MINAS_TIRITH_BATTLEMENTS "Minas Tirith"
    { .previous = REGION_PELENNOR_FIELDS, .next = REGION_MINAS_TIRITH_BATTLEMENTS },

    // [0x10] REGION_DENETHOR_FALL_BALCONY "Balcony"
    { .previous = REGION_PELENNOR_FIELDS, .next = REGION_MINAS_TIRITH_BATTLEMENTS },

    // [0x11] REGION_MINAS_TIRITH_TIERS "Minas Tirith"
    { .previous = REGION_PELENNOR_FIELDS, .next = REGION_MINAS_TIRITH_BATTLEMENTS },

    // [0x12] REGION_MORGUL "Morgul"
    { .previous = REGION_MORGUL, .next = REGION_CIRITH_UNGOL },

    // [0x13] REGION_MORIA "Dwarf Halls"
    { .previous = REGION_ISENGARD, .next = REGION_WHITE_MOUNTAINS },

    // [0x14] REGION_PATHS_OF_THE_DEAD "Paths of the Dead"
    { .previous = REGION_DIMHOLT, .next = REGION_WHITE_MOUNTAINS },

    // [0x15] REGION_PELENNOR_BEACON "Pelennor Beacon"
    { .previous = REGION_PELENNOR_BATTLE, .next = REGION_MINAS_TIRITH_BATTLEMENTS },

    // [0x16] REGION_PELENNOR_BATTLE "Pelennor Battle"
    { .previous = REGION_PELENNOR_FIELDS, .next = REGION_PELENNOR_BATTLE },

    // [0x17] REGION_PELENNOR_FIELDS "Pelennor Fields"
    { .previous = REGION_WHITE_MOUNTAINS, .next = REGION_PELENNOR_BATTLE },

    // [0x18] REGION_ROHAN "Rohan"
    { .previous = REGION_ROHAN, .next = REGION_WHITE_MOUNTAINS },

    // [0x19] REGION_SHELOBS_LAIR "Shelob's Lair"
    { .previous = REGION_CIRITH_UNGOL, .next = REGION_ORC_TOWER },

    // [0x1a] REGION_WEATHERTOP "Amon Sul"
    { .previous = REGION_MINAS_TIRITH_BATTLEMENTS, .next = REGION_ITHILIEN },

    // [0x1b] REGION_WHITE_MOUNTAINS "White Mountains"
    { .previous = REGION_DUNHARROW, .next = REGION_PELENNOR_FIELDS },
};

/**
 * Indexed by the region (enum RegionId); the campaign previous/next links of heroId 0 (HERO_ID_FRODO), 3 (HERO_ID_GANDALF), 6 (HERO_ID_SAM), 7 (HERO_ID_SMEAGOL)
 * (via LootHeroRegionProgressions). field_0x2 is always zero.
 *
 * @romaddress 0x0806dd0c
 */
const LootRegionProgression LootRegionProgressionsFrodoGandalf[28] = {
    // [0x00] REGION_ANDUIN_RIVER "Anduin River"
    { .previous = REGION_WHITE_MOUNTAINS, .next = REGION_CORSAIR_FLAGSHIP },

    // [0x01] REGION_ORC_TOWER "Orc Tower"
    { .previous = REGION_SHELOBS_LAIR, .next = REGION_GORGOROTH },

    // [0x02] REGION_CIRITH_UNGOL "Cirith Ungol"
    { .previous = REGION_MORGUL, .next = REGION_SHELOBS_LAIR },

    // [0x03] REGION_CORSAIR_FLAGSHIP "Corsair Flagship"
    { .previous = REGION_ANDUIN_RIVER, .next = REGION_DOCKS },

    // [0x04] REGION_CRACK_OF_DOOM "Crack of Doom"
    { .previous = REGION_MT_DOOM, .next = REGION_CRACK_OF_DOOM },

    // [0x05] REGION_DIMHOLT "Dimholt"
    { .previous = REGION_DUNHARROW, .next = REGION_PATHS_OF_THE_DEAD },

    // [0x06] REGION_DOCKS "Docks"
    { .previous = REGION_ANDUIN_RIVER, .next = REGION_MINAS_TIRITH_BATTLEMENTS },

    // [0x07] REGION_MT_DOOM "Mt Doom"
    { .previous = REGION_GORGOROTH, .next = REGION_CRACK_OF_DOOM },

    // [0x08] REGION_DUNHARROW "Dunharrow"
    { .previous = REGION_ROHAN, .next = REGION_DIMHOLT },

    // [0x09] REGION_FANGORN "Entwood"
    { .previous = REGION_DUNHARROW, .next = REGION_PATHS_OF_THE_DEAD },

    // [0x0a] REGION_GORGOROTH "Gorgoroth"
    { .previous = REGION_ORC_TOWER, .next = REGION_MT_DOOM },

    // [0x0b] REGION_HELMS_DEEP "Hornburg"
    { .previous = REGION_ISENGARD, .next = REGION_ROHAN },

    // [0x0c] REGION_ICE_CAVE "Ice Cave"
    { .previous = REGION_WHITE_MOUNTAINS, .next = REGION_DIMHOLT },

    // [0x0d] REGION_ISENGARD "Isengard"
    { .previous = REGION_ISENGARD, .next = REGION_ROHAN },

    // [0x0e] REGION_ITHILIEN "Ithilien"
    { .previous = REGION_MINAS_TIRITH_BATTLEMENTS, .next = REGION_ITHILIEN },

    // [0x0f] REGION_MINAS_TIRITH_BATTLEMENTS "Minas Tirith"
    { .previous = REGION_PELENNOR_FIELDS, .next = REGION_MINAS_TIRITH_BATTLEMENTS },

    // [0x10] REGION_DENETHOR_FALL_BALCONY "Balcony"
    { .previous = REGION_PELENNOR_FIELDS, .next = REGION_MINAS_TIRITH_BATTLEMENTS },

    // [0x11] REGION_MINAS_TIRITH_TIERS "Minas Tirith"
    { .previous = REGION_PELENNOR_FIELDS, .next = REGION_MINAS_TIRITH_BATTLEMENTS },

    // [0x12] REGION_MORGUL "Morgul"
    { .previous = REGION_MORGUL, .next = REGION_CIRITH_UNGOL },

    // [0x13] REGION_MORIA "Dwarf Halls"
    { .previous = REGION_ISENGARD, .next = REGION_WHITE_MOUNTAINS },

    // [0x14] REGION_PATHS_OF_THE_DEAD "Paths of the Dead"
    { .previous = REGION_DIMHOLT, .next = REGION_WHITE_MOUNTAINS },

    // [0x15] REGION_PELENNOR_BEACON "Pelennor Beacon"
    { .previous = REGION_PELENNOR_BATTLE, .next = REGION_MINAS_TIRITH_BATTLEMENTS },

    // [0x16] REGION_PELENNOR_BATTLE "Pelennor Battle"
    { .previous = REGION_MINAS_TIRITH_BATTLEMENTS, .next = REGION_PELENNOR_BEACON },

    // [0x17] REGION_PELENNOR_FIELDS "Pelennor Fields"
    { .previous = REGION_WHITE_MOUNTAINS, .next = REGION_MINAS_TIRITH_BATTLEMENTS },

    // [0x18] REGION_ROHAN "Rohan"
    { .previous = REGION_ISENGARD, .next = REGION_WHITE_MOUNTAINS },

    // [0x19] REGION_SHELOBS_LAIR "Shelob's Lair"
    { .previous = REGION_CIRITH_UNGOL, .next = REGION_ORC_TOWER },

    // [0x1a] REGION_WEATHERTOP "Amon Sul"
    { .previous = REGION_MINAS_TIRITH_BATTLEMENTS, .next = REGION_ITHILIEN },

    // [0x1b] REGION_WHITE_MOUNTAINS "White Mountains"
    { .previous = REGION_ROHAN, .next = REGION_PELENNOR_FIELDS },
};
// clang-format on
