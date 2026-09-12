#include "loot.h"
#include "variables.h"

// clang-format off

/**
 * One region-progression table per heroId; heroes share the underlying tables.
 *
 * @romaddress 0x08287310
 */
const LootRegionProgression *const LootHeroRegionProgressions[8] = {
    LootRegionProgressionsFrodoGandalf, // heroId 0 (HERO_ID_FRODO)
    LootRegionProgressionsAragorn,      // heroId 1 (HERO_ID_LEGOLAS)
    LootRegionProgressionsAragorn,      // heroId 2 (HERO_ID_ARAGORN)
    LootRegionProgressionsFrodoGandalf, // heroId 3 (HERO_ID_GANDALF)
    LootRegionProgressionsEowyn,        // heroId 4 (HERO_ID_EOWYN)
    LootRegionProgressionsAragorn,      // heroId 5 (HERO_ID_GIMLI)
    LootRegionProgressionsFrodoGandalf, // heroId 6 (HERO_ID_SAM)
    LootRegionProgressionsFrodoGandalf, // heroId 7 (HERO_ID_SMEAGOL)
};
// clang-format on
