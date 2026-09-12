#pragma once

#include "types.h"

// Scene 0x11: in-game dialog overlay.
void scene_inGameDialog_enter(void);
void scene_inGameDialog_exit(void);
void scene_inGameDialog_tick(void);

/** DialogLineInfos[textId]: what a dialog line shows besides its text. */
typedef struct DialogLineInfo
{
    const void *speakerPicture; // 7x8-tile BG asset; NULL: the active hero's own
    /** formatted into font slot 0 before the text; a credits line points at its
     * CreditsLineKinds entry instead */
    const s16 *slot0Value;
} DialogLineInfo;
