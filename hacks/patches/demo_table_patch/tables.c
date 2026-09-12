// demo: data table patch
#include "combat.h"     // enum StatIndex (from -Iinclude; also pulls item.h)
#include "tablepatch.h" // TABLE_ENTRY macro (from -Ihacks)

// [77] "Waterstone": was Health +1, Gemfinder +1
TABLE_ENTRY(ItemAffix, AccessoryPrefixes, 77) = { //
    .nameId = 0x0522,
    .tier = 1,
    .stat0 = STAT_HEALTH,
    .val0 = 69,
    .stat1 = STAT_GEMFINDER,
    .val1 = 1,
    .stat2 = STAT_FIREBURST,
    .val2 = 1,
    .stat3 = STAT_EXTRA_TREASURE_PERCENT,
    .val3 = 127};
