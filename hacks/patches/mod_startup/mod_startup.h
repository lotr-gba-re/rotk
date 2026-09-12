// What mod_startup exports to the patches that have a say in where boot goes.
#pragma once

#include "types.h"

#if !HACK_mod_startup
#error "requires the mod_startup patch for the game_initBoot entry"
#endif

/**
 * Run the startup tasks and request the first scene of the game proper. A scene that ran
 * ahead of the boot chain (mod_save's migration scene) calls this to hand control back.
 */
void modStartup_resumeBoot(void);
