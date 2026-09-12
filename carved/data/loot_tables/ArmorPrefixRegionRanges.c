#include "affix_ids.h"
#include "item.h"

// clang-format off

/**
 * Index ranges into ArmorPrefixes per region (loot_rollPrefix); the roll picks a
 * uniform affix index in [loIndex, hiIndex]. padding is always zero.
 *
 * @romaddress 0x080667a8
 */
const AffixIndexRange ArmorPrefixRegionRanges[28] = {
    // [0x00] REGION_ANDUIN_RIVER "Anduin River"
    { .loIndex = ARMOR_PREFIX_CORAL,
      .hiIndex = ARMOR_PREFIX_SHARKSKIN },

    // [0x01] REGION_ORC_TOWER "Orc Tower"
    { .loIndex = ARMOR_PREFIX_THUGS,
      .hiIndex = ARMOR_PREFIX_SERGEANTS },

    // [0x02] REGION_CIRITH_UNGOL "Cirith Ungol"
    { .loIndex = ARMOR_PREFIX_THUGS,
      .hiIndex = ARMOR_PREFIX_SERGEANTS },

    // [0x03] REGION_CORSAIR_FLAGSHIP "Corsair Flagship"
    { .loIndex = ARMOR_PREFIX_SHARKSKIN,
      .hiIndex = ARMOR_PREFIX_SHARKSKIN },

    // [0x04] REGION_CRACK_OF_DOOM "Crack of Doom"
    { .loIndex = ARMOR_PREFIX_CRYSTALLINE,
      .hiIndex = ARMOR_PREFIX_ELENDILS },

    // [0x05] REGION_DIMHOLT "Dimholt"
    { .loIndex = ARMOR_PREFIX_GATEKEEPERS,
      .hiIndex = ARMOR_PREFIX_DECORATED },

    // [0x06] REGION_DOCKS "Docks"
    { .loIndex = ARMOR_PREFIX_CORAL,
      .hiIndex = ARMOR_PREFIX_SHARKSKIN },

    // [0x07] REGION_MT_DOOM "Mt Doom"
    { .loIndex = ARMOR_PREFIX_FIRE_RED,
      .hiIndex = ARMOR_PREFIX_HEARTY },

    // [0x08] REGION_DUNHARROW "Dunharrow"
    { .loIndex = ARMOR_PREFIX_GATEKEEPERS,
      .hiIndex = ARMOR_PREFIX_DECORATED },

    // [0x09] REGION_FANGORN "Entwood"
    { .loIndex = ARMOR_PREFIX_URUK,
      .hiIndex = ARMOR_PREFIX_LURTZS },

    // [0x0a] REGION_GORGOROTH "Gorgoroth"
    { .loIndex = ARMOR_PREFIX_DARK_FAE,
      .hiIndex = ARMOR_PREFIX_WARRIORS },

    // [0x0b] REGION_HELMS_DEEP "Hornburg"
    { .loIndex = ARMOR_PREFIX_ROHIRRIM,
      .hiIndex = ARMOR_PREFIX_BRUSH_LORDS },

    // [0x0c] REGION_ICE_CAVE "Ice Cave"
    { .loIndex = ARMOR_PREFIX_FROZEN,
      .hiIndex = ARMOR_PREFIX_MASSIVE },

    // [0x0d] REGION_ISENGARD "Isengard"
    { .loIndex = ARMOR_PREFIX_URUK,
      .hiIndex = ARMOR_PREFIX_LURTZS },

    // [0x0e] REGION_ITHILIEN "Ithilien"
    { .loIndex = ARMOR_PREFIX_CRYSTALLINE,
      .hiIndex = ARMOR_PREFIX_ELENDILS },

    // [0x0f] REGION_MINAS_TIRITH_BATTLEMENTS "Minas Tirith"
    { .loIndex = ARMOR_PREFIX_COURAGEOUS,
      .hiIndex = ARMOR_PREFIX_BOROMIRS },

    // [0x10] REGION_DENETHOR_FALL_BALCONY "Balcony"
    { .loIndex = ARMOR_PREFIX_COURAGEOUS,
      .hiIndex = ARMOR_PREFIX_BOROMIRS },

    // [0x11] REGION_MINAS_TIRITH_TIERS "Minas Tirith"
    { .loIndex = ARMOR_PREFIX_COURAGEOUS,
      .hiIndex = ARMOR_PREFIX_BOROMIRS },

    // [0x12] REGION_MORGUL "Morgul"
    { .loIndex = ARMOR_PREFIX_SHODDY,
      .hiIndex = ARMOR_PREFIX_TORTURERS },

    // [0x13] REGION_MORIA "Dwarf Halls"
    { .loIndex = ARMOR_PREFIX_SHODDY,
      .hiIndex = ARMOR_PREFIX_TORTURERS },

    // [0x14] REGION_PATHS_OF_THE_DEAD "Paths of the Dead"
    { .loIndex = ARMOR_PREFIX_SURVEYORS,
      .hiIndex = ARMOR_PREFIX_ETHEREAL },

    // [0x15] REGION_PELENNOR_BEACON "Pelennor Beacon"
    { .loIndex = ARMOR_PREFIX_COURAGEOUS,
      .hiIndex = ARMOR_PREFIX_BOROMIRS },

    // [0x16] REGION_PELENNOR_BATTLE "Pelennor Battle"
    { .loIndex = ARMOR_PREFIX_COURAGEOUS,
      .hiIndex = ARMOR_PREFIX_BOROMIRS },

    // [0x17] REGION_PELENNOR_FIELDS "Pelennor Fields"
    { .loIndex = ARMOR_PREFIX_COURAGEOUS,
      .hiIndex = ARMOR_PREFIX_BOROMIRS },

    // [0x18] REGION_ROHAN "Rohan"
    { .loIndex = ARMOR_PREFIX_ROHIRRIM,
      .hiIndex = ARMOR_PREFIX_BRUSH_LORDS },

    // [0x19] REGION_SHELOBS_LAIR "Shelob's Lair"
    { .loIndex = ARMOR_PREFIX_FETID,
      .hiIndex = ARMOR_PREFIX_MITHRIL },

    // [0x1a] REGION_WEATHERTOP "Amon Sul"
    { .loIndex = ARMOR_PREFIX_SURVEYORS,
      .hiIndex = ARMOR_PREFIX_ETHEREAL },

    // [0x1b] REGION_WHITE_MOUNTAINS "White Mountains"
    { .loIndex = ARMOR_PREFIX_FROZEN,
      .hiIndex = ARMOR_PREFIX_MASSIVE },
};
// clang-format on
