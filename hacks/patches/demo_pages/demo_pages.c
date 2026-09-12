#include "patches/demo_pages/demo_pages.h"

#if HACK_mod_options
/**
 * The mod_options row accessors (the ModOptionEntry get/set signatures). Out-of-range
 * bytes (uninitialized RAM) read as the first value.
 */
static u8 getChoice(void)
{
    return g_DemoPagesChoice < DEMO_PAGES_CHOICE_COUNT ? g_DemoPagesChoice : 0;
}

static void setChoice(u8 value)
{
    g_DemoPagesChoice = value;
}

static u8 getToggleA(void)
{
    return g_DemoPagesToggleA < 2 ? g_DemoPagesToggleA : 0;
}

static void setToggleA(u8 value)
{
    g_DemoPagesToggleA = value;
}

static u8 getToggleB(void)
{
    return g_DemoPagesToggleB < 2 ? g_DemoPagesToggleB : 0;
}

static void setToggleB(u8 value)
{
    g_DemoPagesToggleB = value;
}

static const char *const DemoPagesChoiceNames[DEMO_PAGES_CHOICE_COUNT] = {"Alpha", "Beta", "Gamma"};
static const char *const DemoPagesOffOnNames[2] = {"Off", "On"};

const struct ModOptionEntry DemoPagesFirstOptions[DEMO_PAGES_FIRST_OPTION_COUNT] = {
    {"Demo Choice", DemoPagesChoiceNames, DEMO_PAGES_CHOICE_COUNT, 0, getChoice, setChoice},
    {"Demo Toggle A", DemoPagesOffOnNames, 2, 0, getToggleA, setToggleA},
};

const struct ModOptionEntry DemoPagesSecondOptions[DEMO_PAGES_SECOND_OPTION_COUNT] = {
    {"Demo Toggle B", DemoPagesOffOnNames, 2, 0, getToggleB, setToggleB},
    MOD_OPTIONS_SPACER,
};
#endif
