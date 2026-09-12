#include "affix_ids.h"
#include "item.h"

// clang-format off

/**
 * Index ranges into WeaponPrefixes per region (loot_rollPrefix); the roll picks a
 * uniform affix index in [loIndex, hiIndex]. padding is always zero.
 *
 * @romaddress 0x08065648
 */
const AffixIndexRange WeaponPrefixRegionRanges[28] = {
    // [0x00] REGION_ANDUIN_RIVER "Anduin River"
    { .loIndex = WEAPON_PREFIX_WARDERS,
      .hiIndex = WEAPON_PREFIX_TREMENDOUS },

    // [0x01] REGION_ORC_TOWER "Orc Tower"
    { .loIndex = WEAPON_PREFIX_FIRED,
      .hiIndex = WEAPON_PREFIX_SKULL_KINGS },

    // [0x02] REGION_CIRITH_UNGOL "Cirith Ungol"
    { .loIndex = WEAPON_PREFIX_FIRED,
      .hiIndex = WEAPON_PREFIX_SKULL_KINGS },

    // [0x03] REGION_CORSAIR_FLAGSHIP "Corsair Flagship"
    { .loIndex = WEAPON_PREFIX_HEAVY_ITHILIEN,
      .hiIndex = WEAPON_PREFIX_OLD_MORGUL },

    // [0x04] REGION_CRACK_OF_DOOM "Crack of Doom"
    { .loIndex = WEAPON_PREFIX_RUBY,
      .hiIndex = WEAPON_PREFIX_ELENDILS },

    // [0x05] REGION_DIMHOLT "Dimholt"
    { .loIndex = WEAPON_PREFIX_MINERS,
      .hiIndex = WEAPON_PREFIX_WARRIORS },

    // [0x06] REGION_DOCKS "Docks"
    { .loIndex = WEAPON_PREFIX_HEAVY_ITHILIEN,
      .hiIndex = WEAPON_PREFIX_OLD_MORGUL },

    // [0x07] REGION_MT_DOOM "Mt Doom"
    { .loIndex = WEAPON_PREFIX_RED,
      .hiIndex = WEAPON_PREFIX_VORPAL },

    // [0x08] REGION_DUNHARROW "Dunharrow"
    { .loIndex = WEAPON_PREFIX_MINERS,
      .hiIndex = WEAPON_PREFIX_WARRIORS },

    // [0x09] REGION_FANGORN "Entwood"
    { .loIndex = WEAPON_PREFIX_TARNISHED,
      .hiIndex = WEAPON_PREFIX_LURTZS },

    // [0x0a] REGION_GORGOROTH "Gorgoroth"
    { .loIndex = WEAPON_PREFIX_COLD,
      .hiIndex = WEAPON_PREFIX_BLACKFLAME },

    // [0x0b] REGION_HELMS_DEEP "Hornburg"
    { .loIndex = WEAPON_PREFIX_WANDERERS,
      .hiIndex = WEAPON_PREFIX_EXOTIC },

    // [0x0c] REGION_ICE_CAVE "Ice Cave"
    { .loIndex = WEAPON_PREFIX_ENGRAVED,
      .hiIndex = WEAPON_PREFIX_ANCIENT_MITHRIL },

    // [0x0d] REGION_ISENGARD "Isengard"
    { .loIndex = WEAPON_PREFIX_TARNISHED,
      .hiIndex = WEAPON_PREFIX_LURTZS },

    // [0x0e] REGION_ITHILIEN "Ithilien"
    { .loIndex = WEAPON_PREFIX_HEAVY_ITHILIEN,
      .hiIndex = WEAPON_PREFIX_OLD_MORGUL },

    // [0x0f] REGION_MINAS_TIRITH_BATTLEMENTS "Minas Tirith"
    { .loIndex = WEAPON_PREFIX_WARDERS,
      .hiIndex = WEAPON_PREFIX_TREMENDOUS },

    // [0x10] REGION_DENETHOR_FALL_BALCONY "Balcony"
    { .loIndex = WEAPON_PREFIX_WARDERS,
      .hiIndex = WEAPON_PREFIX_TREMENDOUS },

    // [0x11] REGION_MINAS_TIRITH_TIERS "Minas Tirith"
    { .loIndex = WEAPON_PREFIX_WARDERS,
      .hiIndex = WEAPON_PREFIX_TREMENDOUS },

    // [0x12] REGION_MORGUL "Morgul"
    { .loIndex = WEAPON_PREFIX_BRONZE,
      .hiIndex = WEAPON_PREFIX_DARKMOON },

    // [0x13] REGION_MORIA "Dwarf Halls"
    { .loIndex = WEAPON_PREFIX_FIRED,
      .hiIndex = WEAPON_PREFIX_SKULL_KINGS },

    // [0x14] REGION_PATHS_OF_THE_DEAD "Paths of the Dead"
    { .loIndex = WEAPON_PREFIX_CURSED,
      .hiIndex = WEAPON_PREFIX_PLATINUM },

    // [0x15] REGION_PELENNOR_BEACON "Pelennor Beacon"
    { .loIndex = WEAPON_PREFIX_WARDERS,
      .hiIndex = WEAPON_PREFIX_TREMENDOUS },

    // [0x16] REGION_PELENNOR_BATTLE "Pelennor Battle"
    { .loIndex = WEAPON_PREFIX_WARDERS,
      .hiIndex = WEAPON_PREFIX_TREMENDOUS },

    // [0x17] REGION_PELENNOR_FIELDS "Pelennor Fields"
    { .loIndex = WEAPON_PREFIX_WARDERS,
      .hiIndex = WEAPON_PREFIX_TREMENDOUS },

    // [0x18] REGION_ROHAN "Rohan"
    { .loIndex = WEAPON_PREFIX_WANDERERS,
      .hiIndex = WEAPON_PREFIX_EXOTIC },

    // [0x19] REGION_SHELOBS_LAIR "Shelob's Lair"
    { .loIndex = WEAPON_PREFIX_SPIDER_IRON,
      .hiIndex = WEAPON_PREFIX_MITHRIL },

    // [0x1a] REGION_WEATHERTOP "Amon Sul"
    { .loIndex = WEAPON_PREFIX_CURSED,
      .hiIndex = WEAPON_PREFIX_PLATINUM },

    // [0x1b] REGION_WHITE_MOUNTAINS "White Mountains"
    { .loIndex = WEAPON_PREFIX_ENGRAVED,
      .hiIndex = WEAPON_PREFIX_ANCIENT_MITHRIL },
};
// clang-format on
