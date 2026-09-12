#include "system.h"
#include "gba_io.h"
#include "gfx.h"
#include "scene.h"
#include "sfx.h"
#include "variables.h"

/**
 * VBlank interrupt service (nesting-guarded): flip the shadow-OAM page and DMA the
 * current page to OAM, run the frame-commit calls when a frame was composed, tick the
 * vblank callback and the sfx driver, keep the FPS meter, then ack.
 *
 * @romaddress 0x0802a0e8
 */
void system_vBlankIrqHandler(void)
{
    g_VBlankIrqNesting += 1;
    if (g_VBlankIrqNesting == 1)
    {
        g_OamShadowParity ^= 1;
        if (g_EngineState->frameReady != 0)
        {
            // frame composed into the other shadow set: commit that one
            if (g_OamShadowApplied == g_OamShadowA)
            {
                DMA0_COPY32(g_OamShadowB + g_OamShadowParity * 0x400, OAM, 0x100);
            }
            else
            {
                DMA0_COPY32(g_OamShadowA + g_OamShadowParity * 0x400, OAM, 0x100);
            }
            g_SpriteAllocCountSnapshot = g_SpriteAllocCount;
            g_SpriteAllocCount = 0;
            gfx_commitBgRegisters();
            gfx_flushPaletteWrites();
            g_OamShadowApplied = g_OamShadow;
            if (g_OamShadow == g_OamShadowA)
            {
                g_OamShadow = g_OamShadowB;
            }
            else
            {
                g_OamShadow = g_OamShadowA;
            }
            transition_tick();
        }
        else
        {
            // no frame composed: re-commit the applied shadow page
            if (g_OamShadowApplied == g_OamShadowA)
            {
                DMA0_COPY32(g_OamShadowApplied + g_OamShadowParity * 0x400, OAM, 0x100);
            }
            else
            {
                DMA0_COPY32(g_OamShadowB + g_OamShadowParity * 0x400, OAM, 0x100);
            }
        }
        sfx_vBlankTick();
        if (g_EngineState->vblankCallback != NULL)
        {
            g_EngineState->vblankCallback();
        }
        gfx_commitPaletteAnims();
        g_EngineState->vblankCounter += 1;
        g_EngineState->fpsVblankCounter += 1;
        if (g_EngineState->fpsVblankCounter > 59)
        {
            g_EngineState->fpsValue = g_EngineState->fpsFrameCounter;
            g_EngineState->fpsFrameCounter = 0;
            g_EngineState->fpsVblankCounter = 0;
        }
        if (g_EngineState->frameReady != 0)
        {
            g_EngineState->frameReady = 0;
        }
        REG_IF = INTERRUPT_FLAG_VBLANK;
        g_InterruptFlags |= INTERRUPT_FLAG_VBLANK;
        sfx_libVBlank();
    }
    g_VBlankIrqNesting -= 1;
    REG_IME = 0;
    g_InterruptCheckFlags |= INTERRUPT_FLAG_VBLANK;
    REG_IME = 1;
}

/**
 * Engine bring-up, run once before system_initInterrupts: point g_EngineState at its fixed
 * block, DMA the ARM IRQ dispatcher into IWRAM (a fixed 0x200 bytes, past its own end) and
 * install it as the BIOS ISR, then DMA the IRQ handler-pointer table into RAM. Both copies
 * use DMA3 (32-bit, CNT readback fence).
 *
 * @romaddress 0x0802a290
 */
void system_initEngine(void)
{
    g_EngineState = &g_EngineStateStorage;

    DMA3_COPY32(system_irqDispatch, g_IrqHandlerCode, 0x80);
    g_BiosIsrPointer = g_IrqHandlerCode;
    DMA3_COPY32(IrqHandlerTable, g_IrqHandlerTable, 0x0d);
}

/**
 * Master interrupt enable at boot: IME on, IE = vblank + Timer1 (the sfx FIFO feeder),
 * vblank IRQ on in DISPSTAT.
 *
 * @romaddress 0x0802a2e4
 */
void system_initInterrupts(void)
{
    REG_IME = 1;
    REG_IE = INTERRUPT_FLAG_VBLANK | INTERRUPT_FLAG_TIMER1;
    REG_DISPSTAT = DISPSTAT_VBLANK_IRQ_ENABLE;
    g_InterruptFlags &= ~INTERRUPT_FLAG_VBLANK;
}

/**
 * Install the vblank callback (g_EngineState->vblankCallback); NULL installs
 * system_nullCallback.
 *
 * @romaddress 0x0802a314
 */
void system_setVblankCallback(void (*callback)(void))
{
    if (callback == NULL)
    {
        g_EngineState->vblankCallback = system_nullCallback;
    }
    else
    {
        g_EngineState->vblankCallback = callback;
    }
}

/**
 * Install the hblank callback (g_EngineState->hblankCallback); NULL installs
 * system_nullCallback.
 *
 * @romaddress 0x0802a340
 */
void system_setHblankCallback(void (*callback)(void))
{
    if (callback == NULL)
    {
        g_EngineState->hblankCallback = system_nullCallback;
    }
    else
    {
        g_EngineState->hblankCallback = callback;
    }
}

/**
 * Install the vcount callback (g_EngineState->vcountCallback); NULL installs
 * system_nullCallback.
 *
 * @romaddress 0x0802a36c
 */
void system_setVcountCallback(void (*callback)(void))
{
    if (callback == NULL)
    {
        g_EngineState->vcountCallback = system_nullCallback;
    }
    else
    {
        g_EngineState->vcountCallback = callback;
    }
}

/**
 * Wait for the next vblank: clear the vblank bit in the IRQ-fired flags and spin until
 * the vblank handler sets it.
 *
 * @romaddress 0x0802a398
 */
void system_waitVBlank(void)
{
    g_InterruptFlags &= ~INTERRUPT_FLAG_VBLANK;
    while (TRUE)
    {
        if ((g_InterruptFlags & INTERRUPT_FLAG_VBLANK) != 0)
        {
            break;
        }
    }
}

/**
 * Enable the VCount (scanline-match) IRQ with the given trigger scanline.
 *
 * @romaddress 0x0802a3c0
 */
void system_enableVcountIrq(s32 scanline)
{
    REG_IE |= INTERRUPT_FLAG_VCOUNT;
    REG_DISPSTAT = (REG_DISPSTAT | DISPSTAT_VCOUNT_IRQ_ENABLE) | DISPSTAT_VCOUNT_LINE(scanline);
}

/**
 * Disable the VCount IRQ.
 *
 * @romaddress 0x0802a3e4
 */
void system_disableVcountIrq(void)
{
    REG_IE &= ~INTERRUPT_FLAG_VCOUNT;
    REG_DISPSTAT &= ~DISPSTAT_VCOUNT_IRQ_ENABLE;
}

/**
 * Default vblank callback: the post-commit housekeeping step.
 *
 * @romaddress 0x0802a40c
 */
void system_defaultVblankCallback(void)
{
    system_endFrame();
}

/**
 * HBlank interrupt service: run the installed hblank callback, bump the hblank counter,
 * and ack the IRQ (REG_IF + the IRQ-fired flags mirror).
 *
 * @romaddress 0x0802a418
 */
void system_hBlankIrqHandler(void)
{
    if (g_EngineState->hblankCallback != NULL)
    {
        g_EngineState->hblankCallback();
    }
    g_EngineState->hblankCounter += 1;
    REG_IF = INTERRUPT_FLAG_HBLANK;
    g_InterruptFlags |= INTERRUPT_FLAG_HBLANK;
}

/**
 * VCount interrupt service: run the installed vcount callback, bump the vcount counter,
 * and ack.
 *
 * @romaddress 0x0802a450
 */
void system_vCountIrqHandler(void)
{
    if (g_EngineState->vcountCallback != NULL)
    {
        g_EngineState->vcountCallback();
    }
    g_EngineState->vcountCounter += 1;
    REG_IF = INTERRUPT_FLAG_VCOUNT;
    g_InterruptFlags |= INTERRUPT_FLAG_VCOUNT;
}

/**
 * Timer0 interrupt service: bump the timer0 counter and ack.
 *
 * @romaddress 0x0802a488
 */
void system_timer0IrqHandler(void)
{
    g_EngineState->timer0Counter += 1;
    REG_IF = INTERRUPT_FLAG_TIMER0;
    g_InterruptFlags |= INTERRUPT_FLAG_TIMER0;
}

/**
 * Timer3/Serial interrupt service (the dispatcher's combined bits 6/7 slot; acks Timer3
 * only): bump the timer3 counter and ack.
 *
 * @romaddress 0x0802a4b0
 */
void system_timer3SerialIrqHandler(void)
{
    g_EngineState->timer3Counter += 1;
    REG_IF = INTERRUPT_FLAG_TIMER3;
    g_InterruptFlags |= INTERRUPT_FLAG_TIMER3;
}

/**
 * Shared empty callback: the setters' NULL default and the unused IRQ table slots.
 *
 * @romaddress 0x0802a4d8
 */
void system_nullCallback(void)
{
}
