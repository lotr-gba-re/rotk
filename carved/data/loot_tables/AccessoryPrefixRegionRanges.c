#include "affix_ids.h"
#include "item.h"

// clang-format off

/**
 * Index ranges into AccessoryPrefixes per region (loot_rollPrefix); the roll picks a
 * uniform affix index in [loIndex, hiIndex]. padding is always zero.
 *
 * @romaddress 0x080671d8
 */
const AffixIndexRange AccessoryPrefixRegionRanges[28] = {
    // [0x00] REGION_ANDUIN_RIVER "Anduin River"
    { .loIndex = ACCESSORY_PREFIX_VICTORY_STONE,
      .hiIndex = ACCESSORY_PREFIX_ARAGORNS },

    // [0x01] REGION_ORC_TOWER "Orc Tower"
    { .loIndex = ACCESSORY_PREFIX_COPPER,
      .hiIndex = ACCESSORY_PREFIX_GLITTERING },

    // [0x02] REGION_CIRITH_UNGOL "Cirith Ungol"
    { .loIndex = ACCESSORY_PREFIX_COPPER,
      .hiIndex = ACCESSORY_PREFIX_GLITTERING },

    // [0x03] REGION_CORSAIR_FLAGSHIP "Corsair Flagship"
    { .loIndex = ACCESSORY_PREFIX_VICTORY_STONE,
      .hiIndex = ACCESSORY_PREFIX_ARAGORNS },

    // [0x04] REGION_CRACK_OF_DOOM "Crack of Doom"
    { .loIndex = ACCESSORY_PREFIX_CRYSTAL,
      .hiIndex = ACCESSORY_PREFIX_DOOMROCK },

    // [0x05] REGION_DIMHOLT "Dimholt"
    { .loIndex = ACCESSORY_PREFIX_SPINEL,
      .hiIndex = ACCESSORY_PREFIX_ELRONDS },

    // [0x06] REGION_DOCKS "Docks"
    { .loIndex = ACCESSORY_PREFIX_VICTORY_STONE,
      .hiIndex = ACCESSORY_PREFIX_ARAGORNS },

    // [0x07] REGION_MT_DOOM "Mt Doom"
    { .loIndex = ACCESSORY_PREFIX_PLATINUM,
      .hiIndex = ACCESSORY_PREFIX_ISILDURS },

    // [0x08] REGION_DUNHARROW "Dunharrow"
    { .loIndex = ACCESSORY_PREFIX_SPINEL,
      .hiIndex = ACCESSORY_PREFIX_ELRONDS },

    // [0x09] REGION_FANGORN "Entwood"
    { .loIndex = ACCESSORY_PREFIX_UGLUKS,
      .hiIndex = ACCESSORY_PREFIX_PALANTIR_CHIP },

    // [0x0a] REGION_GORGOROTH "Gorgoroth"
    { .loIndex = ACCESSORY_PREFIX_TROLLBONE,
      .hiIndex = ACCESSORY_PREFIX_HAVEN_STONE },

    // [0x0b] REGION_HELMS_DEEP "Hornburg"
    { .loIndex = ACCESSORY_PREFIX_MORWENS,
      .hiIndex = ACCESSORY_PREFIX_YELLOW_SAPPHIRE },

    // [0x0c] REGION_ICE_CAVE "Ice Cave"
    { .loIndex = ACCESSORY_PREFIX_WHITE_DIAMOND,
      .hiIndex = ACCESSORY_PREFIX_DWARFRUNED },

    // [0x0d] REGION_ISENGARD "Isengard"
    { .loIndex = ACCESSORY_PREFIX_UGLUKS,
      .hiIndex = ACCESSORY_PREFIX_PALANTIR_CHIP },

    // [0x0e] REGION_ITHILIEN "Ithilien"
    { .loIndex = ACCESSORY_PREFIX_BLACK_DIAMOND,
      .hiIndex = ACCESSORY_PREFIX_LORDSTONE },

    // [0x0f] REGION_MINAS_TIRITH_BATTLEMENTS "Minas Tirith"
    { .loIndex = ACCESSORY_PREFIX_VICTORY_STONE,
      .hiIndex = ACCESSORY_PREFIX_ARAGORNS },

    // [0x10] REGION_DENETHOR_FALL_BALCONY "Balcony"
    { .loIndex = ACCESSORY_PREFIX_VICTORY_STONE,
      .hiIndex = ACCESSORY_PREFIX_ARAGORNS },

    // [0x11] REGION_MINAS_TIRITH_TIERS "Minas Tirith"
    { .loIndex = ACCESSORY_PREFIX_VICTORY_STONE,
      .hiIndex = ACCESSORY_PREFIX_ARAGORNS },

    // [0x12] REGION_MORGUL "Morgul"
    { .loIndex = ACCESSORY_PREFIX_CHEAP,
      .hiIndex = ACCESSORY_PREFIX_BLACK_GLASS },

    // [0x13] REGION_MORIA "Dwarf Halls"
    { .loIndex = ACCESSORY_PREFIX_CHEAP,
      .hiIndex = ACCESSORY_PREFIX_BLACK_GLASS },

    // [0x14] REGION_PATHS_OF_THE_DEAD "Paths of the Dead"
    { .loIndex = ACCESSORY_PREFIX_MOONSTONE,
      .hiIndex = ACCESSORY_PREFIX_ETHEREAL },

    // [0x15] REGION_PELENNOR_BEACON "Pelennor Beacon"
    { .loIndex = ACCESSORY_PREFIX_VICTORY_STONE,
      .hiIndex = ACCESSORY_PREFIX_ARAGORNS },

    // [0x16] REGION_PELENNOR_BATTLE "Pelennor Battle"
    { .loIndex = ACCESSORY_PREFIX_VICTORY_STONE,
      .hiIndex = ACCESSORY_PREFIX_ARAGORNS },

    // [0x17] REGION_PELENNOR_FIELDS "Pelennor Fields"
    { .loIndex = ACCESSORY_PREFIX_VICTORY_STONE,
      .hiIndex = ACCESSORY_PREFIX_ARAGORNS },

    // [0x18] REGION_ROHAN "Rohan"
    { .loIndex = ACCESSORY_PREFIX_MORWENS,
      .hiIndex = ACCESSORY_PREFIX_YELLOW_SAPPHIRE },

    // [0x19] REGION_SHELOBS_LAIR "Shelob's Lair"
    { .loIndex = ACCESSORY_PREFIX_SPIDERSILVER,
      .hiIndex = ACCESSORY_PREFIX_SPIDERKIN },

    // [0x1a] REGION_WEATHERTOP "Amon Sul"
    { .loIndex = ACCESSORY_PREFIX_MOONSTONE,
      .hiIndex = ACCESSORY_PREFIX_ETHEREAL },

    // [0x1b] REGION_WHITE_MOUNTAINS "White Mountains"
    { .loIndex = ACCESSORY_PREFIX_WHITE_DIAMOND,
      .hiIndex = ACCESSORY_PREFIX_DWARFRUNED },
};
// clang-format on
