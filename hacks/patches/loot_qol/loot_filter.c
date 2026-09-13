// Loot filter: void ground drops that fail the active filter settings, before the drop
// actor is spawned. The decision body; hooked from actor_lootPile_create (loot_filter.asm),
// which all drop paths funnel through.
//
// Whitelist design: a drop spawns unless it specifically fails a setting. Uniques (incl.
// Ent Water / Dwarf Runes and the carried-passive backpack items), gems, and health drops
// always bypass it. Arrows can roll neither affix, so the affix requirement would always
// reject them; the "Suppress Arrows" toggle alone decides them.
//
// The filter state lives in g_LootQolState (loot_qol.h), persisted to the global mod
// options save area: loaded at boot (lootQol_loadState), written through by the option
// accessors below.
#include "patches/loot_qol/loot_filter.h"
#include "item.h"
#include "loot.h"
#include "patches/loot_qol/loot_qol.h"
#include "patches/mod_save/save_sram.h"
#include "variables.h"

const char *const LootFilterAffixNames[LOOT_FILTER_AFFIX_COUNT] = {
    "Off", "Any", "Prefix", "Suffix", "Both",
};

#if HACK_mod_options
/** The mod_options row accessors (the ModOptionEntry get/set signatures). */
u8 lootFilter_getAffix(void)
{
    return g_LootQolState.affix;
}

void lootFilter_setAffix(u8 value)
{
    g_LootQolState.affix = value;
    GLOBAL_MOD_OPTIONS_WRITE(lootQolState, &g_LootQolState);
}

u8 lootFilter_getSuppressArrows(void)
{
    return g_LootQolState.arrows;
}

void lootFilter_setSuppressArrows(u8 value)
{
    g_LootQolState.arrows = value;
    GLOBAL_MOD_OPTIONS_WRITE(lootQolState, &g_LootQolState);
}
#endif

/** Returns nonzero when the drop fails the active filter settings and must not spawn. */
bool lootFilter_shouldVoid(u8 lootType, Item item)
{
    // bypass: gem stacks, health drops, artifact pickups and the no-drop kind (below
    // LOOT_TYPE_ITEM_MIN), the Ent Water / Dwarf Rune specials (above LOOT_TYPE_ITEM_MAX)
    if (lootType < LOOT_TYPE_ITEM_MIN || lootType > LOOT_TYPE_ITEM_MAX)
    {
        return FALSE;
    }

    // arrows ignore the affix requirement; the Suppress Arrows toggle alone decides them
    if (item.d.itemType == ITEM_TYPE_ARROW)
    {
        return g_LootQolState.arrows;
    }

    // category bypasses: carried-passive backpack items and uniques
    if (ITEM_BASE_METADATA(item).flags.d.carriedPassive)
    {
        return FALSE;
    }
    if (item.d.itemType == ITEM_TYPE_UNIQUE)
    {
        return FALSE;
    }

    // TODO: Decide if I want an option to filter ancient items.
    //       Alternatively I could just turn off the unlock flag for these.

    // if (ITEM_BASE_METADATA(item).flags.d.ancient)
    // {
    //     return TRUE;
    // }

    switch (g_LootQolState.affix)
    {
    case LOOT_FILTER_AFFIX_ANY:
        return !ITEM_HAS_PREFIX(item) && !ITEM_HAS_SUFFIX(item);
    case LOOT_FILTER_AFFIX_PREFIX:
        return !ITEM_HAS_PREFIX(item);
    case LOOT_FILTER_AFFIX_SUFFIX:
        return !ITEM_HAS_SUFFIX(item);
    case LOOT_FILTER_AFFIX_BOTH:
        return !ITEM_HAS_PREFIX(item) || !ITEM_HAS_SUFFIX(item);
    default: // LOOT_FILTER_AFFIX_OFF and out-of-range values
        return FALSE;
    }
}
