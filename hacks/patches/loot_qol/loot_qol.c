// Loot QoL: quality-of-life loot patches (loot filter + auto loot) sharing one state
// byte and one mod_options page ("Loot QoL"). This file holds the state byte and the page
// registry; the features live in loot_filter.c / auto_loot.c.
#include "patches/loot_qol/loot_qol.h"
#include "patches/loot_qol/auto_loot.h"
#include "patches/loot_qol/loot_filter.h"
#include "patches/mod_save/save_sram.h"

struct LootQolState g_LootQolState;

struct LootQolState lootQol_getState(void)
{
    struct LootQolState state = g_LootQolState;

    if (state.tag != LOOT_QOL_TAG)
    {
        state = (struct LootQolState){.affix = LOOT_FILTER_AFFIX_OFF,
                                      .arrows = FALSE,
                                      .autoLootGems = TRUE,
                                      .tag = LOOT_QOL_TAG};
        g_LootQolState = state;
    }
    return state;
}

void lootQol_loadState(void)
{
    // a blank/unwritten area reads untagged, so the defaults above still apply
    GLOBAL_MOD_OPTIONS_READ(lootQolState, &g_LootQolState);
}

#if HACK_mod_options
/** Shared Off/On value names of the toggle rows. */
static const char *const LootQolOffOnNames[2] = {"Off", "On"};

/**
 * The spacer rows carry the heading/note text drawn by lootQol_drawPageText; its row
 * indices must stay in sync with this array.
 */
const struct ModOptionEntry LootQolOptions[LOOT_QOL_OPTION_COUNT] = {
    MOD_OPTIONS_SPACER,
    MOD_OPTIONS_SPACER,
    {"Require Affix", LootFilterAffixNames, LOOT_FILTER_AFFIX_COUNT, 0x8, lootFilter_getAffix,
     lootFilter_setAffix},
    {"Suppress Arrows", LootQolOffOnNames, 2, 0x8, lootFilter_getSuppressArrows,
     lootFilter_setSuppressArrows},
    MOD_OPTIONS_SPACER,
    {"Auto-Collect Gems", LootQolOffOnNames, 2, 0, autoLoot_getGems, autoLoot_setGems},
};

void lootQol_drawPageText(void)
{
    // heading in the static non-highlight color (the cursor color 0 flashes); the note
    // small-print below it, on the options' indent level
    modOptions_drawRowText(0, MOD_OPTIONS_LABEL_X, "Loot Filter:", FONT_STYLE_8PX_B, 3);
    modOptions_drawRowText(1, MOD_OPTIONS_LABEL_X + 0x8, "(Uniques are never filtered)",
                           FONT_STYLE_8PX_A, 3);
}
#endif
