#pragma once

// Demo pages: a dummy patch registering two mod_options pages, to validate that multiple
// patches can register settings pages and that one patch can own several pages (L/R
// switching + the page counter). Not real settings; the bytes are throwaway.

#include "types.h"

// The patch's mod_options page entries (compiled when HACK_mod_options).
#include "patches/mod_options/mod_options.h"

#define DEMO_PAGES_CHOICE_COUNT 3
#define DEMO_PAGES_FIRST_OPTION_COUNT 2
#define DEMO_PAGES_SECOND_OPTION_COUNT 2
extern const struct ModOptionEntry DemoPagesFirstOptions[DEMO_PAGES_FIRST_OPTION_COUNT];
extern const struct ModOptionEntry DemoPagesSecondOptions[DEMO_PAGES_SECOND_OPTION_COUNT];
