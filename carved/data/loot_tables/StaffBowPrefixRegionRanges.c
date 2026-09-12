#include "affix_ids.h"
#include "item.h"

// clang-format off

/**
 * Index ranges into StaffBowPrefixes per region (loot_rollPrefix); the roll picks a
 * uniform affix index in [loIndex, hiIndex]. padding is always zero.
 *
 * @romaddress 0x08066078
 */
const AffixIndexRange StaffBowPrefixRegionRanges[28] = {
    // [0x00] REGION_ANDUIN_RIVER "Anduin River"
    { .loIndex = STAFF_BOW_PREFIX_OAK,
      .hiIndex = STAFF_BOW_PREFIX_STONE_ETCHED },

    // [0x01] REGION_ORC_TOWER "Orc Tower"
    { .loIndex = STAFF_BOW_PREFIX_SERVANTS,
      .hiIndex = STAFF_BOW_PREFIX_UNGOL },

    // [0x02] REGION_CIRITH_UNGOL "Cirith Ungol"
    { .loIndex = STAFF_BOW_PREFIX_SERVANTS,
      .hiIndex = STAFF_BOW_PREFIX_UNGOL },

    // [0x03] REGION_CORSAIR_FLAGSHIP "Corsair Flagship"
    { .loIndex = STAFF_BOW_PREFIX_APPLEWOOD,
      .hiIndex = STAFF_BOW_PREFIX_HIGH_ELVEN },

    // [0x04] REGION_CRACK_OF_DOOM "Crack of Doom"
    { .loIndex = STAFF_BOW_PREFIX_UNNATURAL,
      .hiIndex = STAFF_BOW_PREFIX_BLACK_LORDS },

    // [0x05] REGION_DIMHOLT "Dimholt"
    { .loIndex = STAFF_BOW_PREFIX_ELM,
      .hiIndex = STAFF_BOW_PREFIX_COURAGEOUS },

    // [0x06] REGION_DOCKS "Docks"
    { .loIndex = STAFF_BOW_PREFIX_APPLEWOOD,
      .hiIndex = STAFF_BOW_PREFIX_HIGH_ELVEN },

    // [0x07] REGION_MT_DOOM "Mt Doom"
    { .loIndex = STAFF_BOW_PREFIX_UNNATURAL,
      .hiIndex = STAFF_BOW_PREFIX_BLACK_LORDS },

    // [0x08] REGION_DUNHARROW "Dunharrow"
    { .loIndex = STAFF_BOW_PREFIX_ELM,
      .hiIndex = STAFF_BOW_PREFIX_COURAGEOUS },

    // [0x09] REGION_FANGORN "Entwood"
    { .loIndex = STAFF_BOW_PREFIX_BONY,
      .hiIndex = STAFF_BOW_PREFIX_WHITE_WIZARD },

    // [0x0a] REGION_GORGOROTH "Gorgoroth"
    { .loIndex = STAFF_BOW_PREFIX_OILED,
      .hiIndex = STAFF_BOW_PREFIX_JADE },

    // [0x0b] REGION_HELMS_DEEP "Hornburg"
    { .loIndex = STAFF_BOW_PREFIX_PINE,
      .hiIndex = STAFF_BOW_PREFIX_OLIVE },

    // [0x0c] REGION_ICE_CAVE "Ice Cave"
    { .loIndex = STAFF_BOW_PREFIX_OAK,
      .hiIndex = STAFF_BOW_PREFIX_STONE_ETCHED },

    // [0x0d] REGION_ISENGARD "Isengard"
    { .loIndex = STAFF_BOW_PREFIX_BONY,
      .hiIndex = STAFF_BOW_PREFIX_WHITE_WIZARD },

    // [0x0e] REGION_ITHILIEN "Ithilien"
    { .loIndex = STAFF_BOW_PREFIX_UNNATURAL,
      .hiIndex = STAFF_BOW_PREFIX_BLACK_LORDS },

    // [0x0f] REGION_MINAS_TIRITH_BATTLEMENTS "Minas Tirith"
    { .loIndex = STAFF_BOW_PREFIX_APPLEWOOD,
      .hiIndex = STAFF_BOW_PREFIX_HIGH_ELVEN },

    // [0x10] REGION_DENETHOR_FALL_BALCONY "Balcony"
    { .loIndex = STAFF_BOW_PREFIX_APPLEWOOD,
      .hiIndex = STAFF_BOW_PREFIX_HIGH_ELVEN },

    // [0x11] REGION_MINAS_TIRITH_TIERS "Minas Tirith"
    { .loIndex = STAFF_BOW_PREFIX_APPLEWOOD,
      .hiIndex = STAFF_BOW_PREFIX_HIGH_ELVEN },

    // [0x12] REGION_MORGUL "Morgul"
    { .loIndex = STAFF_BOW_PREFIX_SERVANTS,
      .hiIndex = STAFF_BOW_PREFIX_UNGOL },

    // [0x13] REGION_MORIA "Dwarf Halls"
    { .loIndex = STAFF_BOW_PREFIX_SERVANTS,
      .hiIndex = STAFF_BOW_PREFIX_UNGOL },

    // [0x14] REGION_PATHS_OF_THE_DEAD "Paths of the Dead"
    { .loIndex = STAFF_BOW_PREFIX_EBONY,
      .hiIndex = STAFF_BOW_PREFIX_GEMSTUDDED },

    // [0x15] REGION_PELENNOR_BEACON "Pelennor Beacon"
    { .loIndex = STAFF_BOW_PREFIX_APPLEWOOD,
      .hiIndex = STAFF_BOW_PREFIX_HIGH_ELVEN },

    // [0x16] REGION_PELENNOR_BATTLE "Pelennor Battle"
    { .loIndex = STAFF_BOW_PREFIX_APPLEWOOD,
      .hiIndex = STAFF_BOW_PREFIX_HIGH_ELVEN },

    // [0x17] REGION_PELENNOR_FIELDS "Pelennor Fields"
    { .loIndex = STAFF_BOW_PREFIX_APPLEWOOD,
      .hiIndex = STAFF_BOW_PREFIX_HIGH_ELVEN },

    // [0x18] REGION_ROHAN "Rohan"
    { .loIndex = STAFF_BOW_PREFIX_PINE,
      .hiIndex = STAFF_BOW_PREFIX_OLIVE },

    // [0x19] REGION_SHELOBS_LAIR "Shelob's Lair"
    { .loIndex = STAFF_BOW_PREFIX_OILED,
      .hiIndex = STAFF_BOW_PREFIX_JADE },

    // [0x1a] REGION_WEATHERTOP "Amon Sul"
    { .loIndex = STAFF_BOW_PREFIX_EBONY,
      .hiIndex = STAFF_BOW_PREFIX_GEMSTUDDED },

    // [0x1b] REGION_WHITE_MOUNTAINS "White Mountains"
    { .loIndex = STAFF_BOW_PREFIX_OAK,
      .hiIndex = STAFF_BOW_PREFIX_STONE_ETCHED },
};
// clang-format on
