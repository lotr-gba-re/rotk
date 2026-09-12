#include "affix_ids.h"
#include "item.h"

// clang-format off

/**
 * Index ranges into CloakPrefixes per region (loot_rollPrefix); the roll picks a
 * uniform affix index in [loIndex, hiIndex]. padding is always zero.
 *
 * @romaddress 0x08067c08
 */
const AffixIndexRange CloakPrefixRegionRanges[28] = {
    // [0x00] REGION_ANDUIN_RIVER "Anduin River"
    { .loIndex = CLOAK_PREFIX_GOLDLACE,
      .hiIndex = CLOAK_PREFIX_GEM_CRUSTED },

    // [0x01] REGION_ORC_TOWER "Orc Tower"
    { .loIndex = CLOAK_PREFIX_TATTERED,
      .hiIndex = CLOAK_PREFIX_PATCHED },

    // [0x02] REGION_CIRITH_UNGOL "Cirith Ungol"
    { .loIndex = CLOAK_PREFIX_TATTERED,
      .hiIndex = CLOAK_PREFIX_PATCHED },

    // [0x03] REGION_CORSAIR_FLAGSHIP "Corsair Flagship"
    { .loIndex = CLOAK_PREFIX_GOLDLACE,
      .hiIndex = CLOAK_PREFIX_GEM_CRUSTED },

    // [0x04] REGION_CRACK_OF_DOOM "Crack of Doom"
    { .loIndex = CLOAK_PREFIX_GOLDLACE,
      .hiIndex = CLOAK_PREFIX_GEM_CRUSTED },

    // [0x05] REGION_DIMHOLT "Dimholt"
    { .loIndex = CLOAK_PREFIX_WOOL,
      .hiIndex = CLOAK_PREFIX_WEBFIBER },

    // [0x06] REGION_DOCKS "Docks"
    { .loIndex = CLOAK_PREFIX_GOLDLACE,
      .hiIndex = CLOAK_PREFIX_GEM_CRUSTED },

    // [0x07] REGION_MT_DOOM "Mt Doom"
    { .loIndex = CLOAK_PREFIX_GOLDLACE,
      .hiIndex = CLOAK_PREFIX_GEM_CRUSTED },

    // [0x08] REGION_DUNHARROW "Dunharrow"
    { .loIndex = CLOAK_PREFIX_WOOL,
      .hiIndex = CLOAK_PREFIX_WEBFIBER },

    // [0x09] REGION_FANGORN "Entwood"
    { .loIndex = CLOAK_PREFIX_WOOL,
      .hiIndex = CLOAK_PREFIX_WEBFIBER },

    // [0x0a] REGION_GORGOROTH "Gorgoroth"
    { .loIndex = CLOAK_PREFIX_WOOL,
      .hiIndex = CLOAK_PREFIX_WEBFIBER },

    // [0x0b] REGION_HELMS_DEEP "Hornburg"
    { .loIndex = CLOAK_PREFIX_WOOL,
      .hiIndex = CLOAK_PREFIX_WEBFIBER },

    // [0x0c] REGION_ICE_CAVE "Ice Cave"
    { .loIndex = CLOAK_PREFIX_WOOL,
      .hiIndex = CLOAK_PREFIX_WEBFIBER },

    // [0x0d] REGION_ISENGARD "Isengard"
    { .loIndex = CLOAK_PREFIX_TATTERED,
      .hiIndex = CLOAK_PREFIX_PATCHED },

    // [0x0e] REGION_ITHILIEN "Ithilien"
    { .loIndex = CLOAK_PREFIX_GOLDLACE,
      .hiIndex = CLOAK_PREFIX_GEM_CRUSTED },

    // [0x0f] REGION_MINAS_TIRITH_BATTLEMENTS "Minas Tirith"
    { .loIndex = CLOAK_PREFIX_GOLDLACE,
      .hiIndex = CLOAK_PREFIX_GEM_CRUSTED },

    // [0x10] REGION_DENETHOR_FALL_BALCONY "Balcony"
    { .loIndex = CLOAK_PREFIX_GOLDLACE,
      .hiIndex = CLOAK_PREFIX_GEM_CRUSTED },

    // [0x11] REGION_MINAS_TIRITH_TIERS "Minas Tirith"
    { .loIndex = CLOAK_PREFIX_GOLDLACE,
      .hiIndex = CLOAK_PREFIX_GEM_CRUSTED },

    // [0x12] REGION_MORGUL "Morgul"
    { .loIndex = CLOAK_PREFIX_TATTERED,
      .hiIndex = CLOAK_PREFIX_PATCHED },

    // [0x13] REGION_MORIA "Dwarf Halls"
    { .loIndex = CLOAK_PREFIX_WOOL,
      .hiIndex = CLOAK_PREFIX_WEBFIBER },

    // [0x14] REGION_PATHS_OF_THE_DEAD "Paths of the Dead"
    { .loIndex = CLOAK_PREFIX_WOOL,
      .hiIndex = CLOAK_PREFIX_WEBFIBER },

    // [0x15] REGION_PELENNOR_BEACON "Pelennor Beacon"
    { .loIndex = CLOAK_PREFIX_GOLDLACE,
      .hiIndex = CLOAK_PREFIX_GEM_CRUSTED },

    // [0x16] REGION_PELENNOR_BATTLE "Pelennor Battle"
    { .loIndex = CLOAK_PREFIX_GOLDLACE,
      .hiIndex = CLOAK_PREFIX_GEM_CRUSTED },

    // [0x17] REGION_PELENNOR_FIELDS "Pelennor Fields"
    { .loIndex = CLOAK_PREFIX_GOLDLACE,
      .hiIndex = CLOAK_PREFIX_GEM_CRUSTED },

    // [0x18] REGION_ROHAN "Rohan"
    { .loIndex = CLOAK_PREFIX_TATTERED,
      .hiIndex = CLOAK_PREFIX_PATCHED },

    // [0x19] REGION_SHELOBS_LAIR "Shelob's Lair"
    { .loIndex = CLOAK_PREFIX_WOOL,
      .hiIndex = CLOAK_PREFIX_WEBFIBER },

    // [0x1a] REGION_WEATHERTOP "Amon Sul"
    { .loIndex = CLOAK_PREFIX_WOOL,
      .hiIndex = CLOAK_PREFIX_WEBFIBER },

    // [0x1b] REGION_WHITE_MOUNTAINS "White Mountains"
    { .loIndex = CLOAK_PREFIX_WOOL,
      .hiIndex = CLOAK_PREFIX_WEBFIBER },
};
// clang-format on
