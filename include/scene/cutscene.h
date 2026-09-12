#pragma once

#include "types.h"

// Scene 0x07: cutscene. args[1] is the CutscenePages index, args[2] the mission arg the
// last page hands to gameplay.
void scene_cutscene_enter(void);
void scene_cutscene_tick(void);
void scene_cutscene_exit(void);

/** How a page comes and goes (CutscenePage.mode). Below 4: picture only, no caption. */
enum CutscenePageMode
{
    CUTSCENE_PAGE_FIRST = 4,  // shown at once with its caption
    CUTSCENE_PAGE_MIDDLE = 5, // crossfades in over the previous page
    CUTSCENE_PAGE_LAST = 6,   // crossfades in; the next press fades out to gameplay
};

/** CutscenePage.nextPage of a chain's last page. */
#define CUTSCENE_PAGE_NONE 71

/** One cutscene page: its picture(s), caption and how it chains to the next. */
typedef struct CutscenePage
{
    const void *bg0Asset; // BG asset per layer; NULL: layer unused
    const void *bg1Asset;
    const u32 *bg0Cnt; // BGCNT word per layer
    const u32 *bg1Cnt;
    fp16_16 bg0ScrollSpeedX; // scroll phase of the caption-less modes
    fp16_16 bg0ScrollSpeedY;
    fp16_16 bg1ScrollSpeedX;
    fp16_16 bg1ScrollSpeedY;
    u16 holdFrames; // 0x20: frames before and after the scroll phase
    u16 textId;     // 0x22: caption; the scroll phase also reads it as its length
    /** 0x24: args[0] of the next scene: GameplayMode on the hand-off, else nonzero keeps
     * the display set up */
    u8 nextMode;
    u8 mode;       // 0x25: enum CutscenePageMode
    u8 nextPage;   // 0x26: CutscenePages index, CUTSCENE_PAGE_NONE ends the chain
    u8 field_0x27; // 0x27: both layers' BG transform slot word at +0xc, as a 16.16 integer
    u8 field_0x28; // 0x28: same for the word at +0x10
    /** 0x29: also the page's position in its chain: bit 0 picks the layer and palette
     * half the crossfade modes draw into */
    u8 bg0PaletteNumber;
    u8 bg1PaletteNumber;
    u8 field_0x2b;
} CutscenePage;
