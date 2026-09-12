#include "affix_ids.h"
#include "item.h"

// clang-format off

/**
 * Index ranges into ItemSuffixes per rolled suffix tier (loot_rollSuffix); the roll picks a
 * uniform affix index in [loIndex, hiIndex]. padding is always zero.
 *
 * @romaddress 0x08068528
 */
const AffixIndexRange SuffixTierRanges[10] = {
    // [0]
    { .loIndex = ITEM_SUFFIX_OF_THE_FOX,
      .hiIndex = ITEM_SUFFIX_OF_THE_DEEP },

    // [1]
    { .loIndex = ITEM_SUFFIX_OF_MIDGEWATER,
      .hiIndex = ITEM_SUFFIX_OF_MITHRANDIR },

    // [2]
    { .loIndex = ITEM_SUFFIX_OF_MERRY,
      .hiIndex = ITEM_SUFFIX_OF_THE_HARAD },

    // [3]
    { .loIndex = ITEM_SUFFIX_OF_THE_RIDERS,
      .hiIndex = ITEM_SUFFIX_OF_GONDOR },

    // [4]
    { .loIndex = ITEM_SUFFIX_OF_THE_NORTH,
      .hiIndex = ITEM_SUFFIX_OF_THEODRED },

    // [5]
    { .loIndex = ITEM_SUFFIX_OF_WOOD_ELVES,
      .hiIndex = ITEM_SUFFIX_OF_AMON_SUL },

    // [6]
    { .loIndex = ITEM_SUFFIX_OF_DAMROD,
      .hiIndex = ITEM_SUFFIX_OF_NUMENOR },

    // [7]
    { .loIndex = ITEM_SUFFIX_OF_LEGEND,
      .hiIndex = ITEM_SUFFIX_OF_CIRITH_UNGOL },

    // [8]
    { .loIndex = ITEM_SUFFIX_OF_ANORIEN,
      .hiIndex = ITEM_SUFFIX_OF_ECTHELION_TOWER },

    // [9]
    { .loIndex = ITEM_SUFFIX_OF_THE_GHOST_KING,
      .hiIndex = ITEM_SUFFIX_OF_MORDOR },
};
// clang-format on
