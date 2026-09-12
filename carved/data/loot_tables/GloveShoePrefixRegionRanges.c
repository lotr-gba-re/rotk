#include "affix_ids.h"
#include "item.h"

// clang-format off

/**
 * Index ranges into GloveShoePrefixes per region (loot_rollPrefix); the roll picks a
 * uniform affix index in [loIndex, hiIndex]. padding is always zero.
 *
 * @romaddress 0x08067eb8
 */
const AffixIndexRange GloveShoePrefixRegionRanges[28] = {
    // [0x00] REGION_ANDUIN_RIVER "Anduin River"
    { .loIndex = GLOVE_SHOE_PREFIX_MOUNTAINEERS,
      .hiIndex = GLOVE_SHOE_PREFIX_ILLUSTRIOUS },

    // [0x01] REGION_ORC_TOWER "Orc Tower"
    { .loIndex = GLOVE_SHOE_PREFIX_EASTERLING,
      .hiIndex = GLOVE_SHOE_PREFIX_BLESSED },

    // [0x02] REGION_CIRITH_UNGOL "Cirith Ungol"
    { .loIndex = GLOVE_SHOE_PREFIX_EASTERLING,
      .hiIndex = GLOVE_SHOE_PREFIX_BLESSED },

    // [0x03] REGION_CORSAIR_FLAGSHIP "Corsair Flagship"
    { .loIndex = GLOVE_SHOE_PREFIX_MOUNTAINEERS,
      .hiIndex = GLOVE_SHOE_PREFIX_ILLUSTRIOUS },

    // [0x04] REGION_CRACK_OF_DOOM "Crack of Doom"
    { .loIndex = GLOVE_SHOE_PREFIX_RUNE_SEWN,
      .hiIndex = GLOVE_SHOE_PREFIX_ELRONDS },

    // [0x05] REGION_DIMHOLT "Dimholt"
    { .loIndex = GLOVE_SHOE_PREFIX_FIELD_GREEN,
      .hiIndex = GLOVE_SHOE_PREFIX_PLAIN_KINGS },

    // [0x06] REGION_DOCKS "Docks"
    { .loIndex = GLOVE_SHOE_PREFIX_MOUNTAINEERS,
      .hiIndex = GLOVE_SHOE_PREFIX_ILLUSTRIOUS },

    // [0x07] REGION_MT_DOOM "Mt Doom"
    { .loIndex = GLOVE_SHOE_PREFIX_RUNE_SEWN,
      .hiIndex = GLOVE_SHOE_PREFIX_ELRONDS },

    // [0x08] REGION_DUNHARROW "Dunharrow"
    { .loIndex = GLOVE_SHOE_PREFIX_FIELD_GREEN,
      .hiIndex = GLOVE_SHOE_PREFIX_PLAIN_KINGS },

    // [0x09] REGION_FANGORN "Entwood"
    { .loIndex = GLOVE_SHOE_PREFIX_MANFLESH,
      .hiIndex = GLOVE_SHOE_PREFIX_GANDALFS },

    // [0x0a] REGION_GORGOROTH "Gorgoroth"
    { .loIndex = GLOVE_SHOE_PREFIX_NOBLE,
      .hiIndex = GLOVE_SHOE_PREFIX_GIL_GALADS },

    // [0x0b] REGION_HELMS_DEEP "Hornburg"
    { .loIndex = GLOVE_SHOE_PREFIX_FIELD_GREEN,
      .hiIndex = GLOVE_SHOE_PREFIX_PLAIN_KINGS },

    // [0x0c] REGION_ICE_CAVE "Ice Cave"
    { .loIndex = GLOVE_SHOE_PREFIX_MOUNTAINEERS,
      .hiIndex = GLOVE_SHOE_PREFIX_ILLUSTRIOUS },

    // [0x0d] REGION_ISENGARD "Isengard"
    { .loIndex = GLOVE_SHOE_PREFIX_MANFLESH,
      .hiIndex = GLOVE_SHOE_PREFIX_GANDALFS },

    // [0x0e] REGION_ITHILIEN "Ithilien"
    { .loIndex = GLOVE_SHOE_PREFIX_SOLDIERS,
      .hiIndex = GLOVE_SHOE_PREFIX_BOROMIRS },

    // [0x0f] REGION_MINAS_TIRITH_BATTLEMENTS "Minas Tirith"
    { .loIndex = GLOVE_SHOE_PREFIX_SOLDIERS,
      .hiIndex = GLOVE_SHOE_PREFIX_BOROMIRS },

    // [0x10] REGION_DENETHOR_FALL_BALCONY "Balcony"
    { .loIndex = GLOVE_SHOE_PREFIX_SOLDIERS,
      .hiIndex = GLOVE_SHOE_PREFIX_BOROMIRS },

    // [0x11] REGION_MINAS_TIRITH_TIERS "Minas Tirith"
    { .loIndex = GLOVE_SHOE_PREFIX_SOLDIERS,
      .hiIndex = GLOVE_SHOE_PREFIX_BOROMIRS },

    // [0x12] REGION_MORGUL "Morgul"
    { .loIndex = GLOVE_SHOE_PREFIX_EASTERLING,
      .hiIndex = GLOVE_SHOE_PREFIX_BLESSED },

    // [0x13] REGION_MORIA "Dwarf Halls"
    { .loIndex = GLOVE_SHOE_PREFIX_EASTERLING,
      .hiIndex = GLOVE_SHOE_PREFIX_BLESSED },

    // [0x14] REGION_PATHS_OF_THE_DEAD "Paths of the Dead"
    { .loIndex = GLOVE_SHOE_PREFIX_SPECTRAL,
      .hiIndex = GLOVE_SHOE_PREFIX_ETHEREAL },

    // [0x15] REGION_PELENNOR_BEACON "Pelennor Beacon"
    { .loIndex = GLOVE_SHOE_PREFIX_SOLDIERS,
      .hiIndex = GLOVE_SHOE_PREFIX_BOROMIRS },

    // [0x16] REGION_PELENNOR_BATTLE "Pelennor Battle"
    { .loIndex = GLOVE_SHOE_PREFIX_SOLDIERS,
      .hiIndex = GLOVE_SHOE_PREFIX_BOROMIRS },

    // [0x17] REGION_PELENNOR_FIELDS "Pelennor Fields"
    { .loIndex = GLOVE_SHOE_PREFIX_SOLDIERS,
      .hiIndex = GLOVE_SHOE_PREFIX_BOROMIRS },

    // [0x18] REGION_ROHAN "Rohan"
    { .loIndex = GLOVE_SHOE_PREFIX_FIELD_GREEN,
      .hiIndex = GLOVE_SHOE_PREFIX_PLAIN_KINGS },

    // [0x19] REGION_SHELOBS_LAIR "Shelob's Lair"
    { .loIndex = GLOVE_SHOE_PREFIX_NOBLE,
      .hiIndex = GLOVE_SHOE_PREFIX_GIL_GALADS },

    // [0x1a] REGION_WEATHERTOP "Amon Sul"
    { .loIndex = GLOVE_SHOE_PREFIX_SPECTRAL,
      .hiIndex = GLOVE_SHOE_PREFIX_ETHEREAL },

    // [0x1b] REGION_WHITE_MOUNTAINS "White Mountains"
    { .loIndex = GLOVE_SHOE_PREFIX_MOUNTAINEERS,
      .hiIndex = GLOVE_SHOE_PREFIX_ILLUSTRIOUS },
};
// clang-format on
