#pragma once

#include "types.h"

// Scene 0x16: unknown (multi-BG title-style screen; no literal game_requestSceneChange(0x16)
// caller found, likely reached via an id-variable path).
void scene_unknown16_enter(void);
void scene_unknown16_tick(void);
void scene_unknown16_exit(void);
