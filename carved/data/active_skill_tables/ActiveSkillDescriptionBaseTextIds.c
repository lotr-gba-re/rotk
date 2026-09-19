#include "types.h"

// clang-format off

/**
 * Per-hero base text IDs of complete active skill descriptions, indexed by HeroId.
 * Add the active skill row to get its description text ID.
 *
 * @romaddress 0x082874a4
 */
const u16 ActiveSkillDescriptionBaseTextIds[8] = {
    518, // [HERO_ID_FRODO]
    524, // [HERO_ID_LEGOLAS]
    530, // [HERO_ID_ARAGORN]
    536, // [HERO_ID_GANDALF]
    548, // [HERO_ID_EOWYN]
    542, // [HERO_ID_GIMLI]
    554, // [HERO_ID_SAM]
    560, // [HERO_ID_SMEAGOL]
};
// clang-format on
