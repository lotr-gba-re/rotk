#pragma once

#include "types.h"

// Scene 0x17: Nintendo GameCube link menu. A successful link unlocks Sam for multiplayer and
// ancient items (SaveGlobals.unlockFlags |= UNLOCK_FLAG_CHARACTER_SAM |
// UNLOCK_FLAG_ANCIENT_ITEMS).
void scene_gcnLinkMenu_enter(void);
void scene_gcnLinkMenu_tick(void);
void scene_gcnLinkMenu_exit(void);
