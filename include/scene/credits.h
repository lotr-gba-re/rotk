#pragma once

#include "types.h"

// Scene 0x13: credits.
void scene_credits_enter(void);
void scene_credits_tick(void);
void scene_credits_exit(void);

/**
 * Index into CreditsLineKinds, whose entry a credits line's DialogLineInfo.slot0Value
 * points at: the pointer identity picks the line's font and ends the roll.
 */
enum CreditsLineKind
{
    CREDITS_LINE_HEADER = 0, // larger font
    CREDITS_LINE_NAME = 1,
    CREDITS_LINE_LAST = 2, // ends the credits after it scrolled out
};
