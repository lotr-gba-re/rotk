#pragma once

#include "types.h"

/**
 * g_InterruptFlags bits (the mask view the code tests): IRQ-fired flags (IWRAM), set by
 * the matching system_*Irq handler. No flags union here: standalone sub-u32 words
 * read-modify-write worse through one (packed splits the member into byte loads;
 * unpacked widens the union and RMWs the whole word).
 */
typedef enum InterruptFlag
{
    INTERRUPT_FLAG_VBLANK = 1 << 0,
    INTERRUPT_FLAG_HBLANK = 1 << 1,
    INTERRUPT_FLAG_VCOUNT = 1 << 2,
    INTERRUPT_FLAG_TIMER0 = 1 << 3,
    INTERRUPT_FLAG_TIMER1 = 1 << 4,
    INTERRUPT_FLAG_TIMER3 = 1 << 6,
} InterruptFlag;

/**
 * The engine's per-frame timing/sync state; g_EngineState points at the fixed block set
 * up by the engine init. Only the members with consumers are decoded.
 *
 * The game ticks once every 2 vblanks (~30 ticks per second), paced by vblankCounter and
 * lastTickVblank below.
 */
typedef struct EngineState
{
    /** vblank callback; system_setVblankCallback installs it (NULL -> an empty stub) */
    void (*vblankCallback)(void);

    /** hblank callback; run by system_hBlankIrqHandler */
    void (*hblankCallback)(void);

    /** vcount callback; run by system_vCountIrqHandler (the sfx driver installs its hook) */
    void (*vcountCallback)(void);

    /** bumped by system_timer0IrqHandler */
    u32 timer0Counter;

    u8 field_0x10[8];

    /** bumped by system_timer3SerialIrqHandler (the dispatcher's shared Timer3/Serial slot) */
    u32 timer3Counter;

    /** bumped by system_vBlankIrqHandler; also the mp-link and fade timeout base */
    u32 vblankCounter;

    /** vblankCounter as of the last game tick. The game ticks every other frame. */
    u32 lastTickVblank;

    /** bumped by system_hBlankIrqHandler */
    u32 hblankCounter;

    /** bumped by system_vCountIrqHandler */
    u32 vcountCounter;

    /** vblanks in the current FPS-meter window (published every 60) */
    u32 fpsVblankCounter;

    /**
     * frames in the FPS-meter window (+1 per game_tickFrame; published into fpsValue
     * and reset every 60 vblanks by system_vBlankIrqHandler)
     */
    u32 fpsFrameCounter;

    /** last published FPS-meter value */
    u32 fpsValue;

    /**
     * frame composed, ready to commit (set by sprite_endFrame; gates the main loop's
     * post-commit step)
     */
    u16 frameReady;
} EngineState;

void rotk_main(void);
void system_vBlankIrqHandler(void);
void system_waitVBlank(void);
// Install a g_EngineState IRQ callback; NULL installs system_nullCallback.
void system_setVblankCallback(void (*callback)(void));
void system_setHblankCallback(void (*callback)(void));
void system_setVcountCallback(void (*callback)(void));
// Enable/disable the VCount (scanline-match) IRQ; enable takes the trigger scanline.
void system_enableVcountIrq(s32 scanline);
void system_disableVcountIrq(void);
/** Default vblank callback: the post-commit housekeeping step. */
void system_defaultVblankCallback(void);
/**
 * ARM IRQ dispatcher, installed as the BIOS ISR by system_initEngine; runs from its
 * IWRAM copy at g_IrqHandlerCode and routes to the g_IrqHandlerTable entries.
 */
void system_irqDispatch(void);
void system_hBlankIrqHandler(void);
void system_vCountIrqHandler(void);
void system_timer0IrqHandler(void);
void system_timer3SerialIrqHandler(void);
void system_nullCallback(void);
/**
 * End-of-frame housekeeping run by the default vblank callback: when a frame was
 * composed (frameReady), clear ANIMATION_FRAME_CHANGED on the actors that ticked and
 * commit the sfx channel masks.
 */
void system_endFrame(void);
void system_initEngine(void);
void system_initInterrupts(void);
