#pragma once

#include "types.h"

#define SRAM_BASE ((u8 *)0x0E000000u)
#define SRAM_SIZE 0x8000u

#define REG_BASE 0x04000000u

// Display.
#define REG_OFFSET_DISPCNT 0x000u
#define REG_ADDR_DISPCNT (REG_BASE + REG_OFFSET_DISPCNT)
#define REG_DISPCNT (*(volatile u16 *)REG_ADDR_DISPCNT)

#define REG_OFFSET_DISPSTAT 0x004u
#define REG_ADDR_DISPSTAT (REG_BASE + REG_OFFSET_DISPSTAT)
#define REG_DISPSTAT (*(volatile u16 *)REG_ADDR_DISPSTAT)

// REG_DISPSTAT fields (IRQ enables; VCount trigger line lives in bits 8-15).
#define DISPSTAT_VBLANK_IRQ_ENABLE (1 << 3)
#define DISPSTAT_VCOUNT_IRQ_ENABLE (1 << 5)
#define DISPSTAT_VCOUNT_LINE(line) ((line) << 8)

#define REG_OFFSET_VCOUNT 0x006u // RO: current scanline
#define REG_ADDR_VCOUNT (REG_BASE + REG_OFFSET_VCOUNT)
#define REG_VCOUNT (*(volatile u16 *)REG_ADDR_VCOUNT)

#define REG_OFFSET_BG0CNT 0x008u
#define REG_ADDR_BG0CNT (REG_BASE + REG_OFFSET_BG0CNT)
#define REG_BG0CNT (*(volatile u16 *)REG_ADDR_BG0CNT)
#define REG_OFFSET_BG1CNT 0x00au
#define REG_ADDR_BG1CNT (REG_BASE + REG_OFFSET_BG1CNT)
#define REG_BG1CNT (*(volatile u16 *)REG_ADDR_BG1CNT)
#define REG_OFFSET_BG2CNT 0x00cu
#define REG_ADDR_BG2CNT (REG_BASE + REG_OFFSET_BG2CNT)
#define REG_BG2CNT (*(volatile u16 *)REG_ADDR_BG2CNT)
#define REG_OFFSET_BG3CNT 0x00eu
#define REG_ADDR_BG3CNT (REG_BASE + REG_OFFSET_BG3CNT)
#define REG_BG3CNT (*(volatile u16 *)REG_ADDR_BG3CNT)

#define REG_OFFSET_BG0HOFS 0x010u
#define REG_ADDR_BG0HOFS (REG_BASE + REG_OFFSET_BG0HOFS)
#define REG_BG0HOFS (*(volatile u16 *)REG_ADDR_BG0HOFS)
#define REG_OFFSET_BG0VOFS 0x012u
#define REG_ADDR_BG0VOFS (REG_BASE + REG_OFFSET_BG0VOFS)
#define REG_BG0VOFS (*(volatile u16 *)REG_ADDR_BG0VOFS)
#define REG_OFFSET_BG1HOFS 0x014u
#define REG_ADDR_BG1HOFS (REG_BASE + REG_OFFSET_BG1HOFS)
#define REG_BG1HOFS (*(volatile u16 *)REG_ADDR_BG1HOFS)
#define REG_OFFSET_BG1VOFS 0x016u
#define REG_ADDR_BG1VOFS (REG_BASE + REG_OFFSET_BG1VOFS)
#define REG_BG1VOFS (*(volatile u16 *)REG_ADDR_BG1VOFS)
#define REG_OFFSET_BG2HOFS 0x018u
#define REG_ADDR_BG2HOFS (REG_BASE + REG_OFFSET_BG2HOFS)
#define REG_BG2HOFS (*(volatile u16 *)REG_ADDR_BG2HOFS)
#define REG_OFFSET_BG2VOFS 0x01au
#define REG_ADDR_BG2VOFS (REG_BASE + REG_OFFSET_BG2VOFS)
#define REG_BG2VOFS (*(volatile u16 *)REG_ADDR_BG2VOFS)
#define REG_OFFSET_BG3HOFS 0x01cu
#define REG_ADDR_BG3HOFS (REG_BASE + REG_OFFSET_BG3HOFS)
#define REG_BG3HOFS (*(volatile u16 *)REG_ADDR_BG3HOFS)
#define REG_OFFSET_BG3VOFS 0x01eu
#define REG_ADDR_BG3VOFS (REG_BASE + REG_OFFSET_BG3VOFS)
#define REG_BG3VOFS (*(volatile u16 *)REG_ADDR_BG3VOFS)

// BG rotation/scaling parameters (signed 8.8 fixed) and 28-bit reference points.
#define REG_OFFSET_BG2PA 0x020u
#define REG_ADDR_BG2PA (REG_BASE + REG_OFFSET_BG2PA)
#define REG_BG2PA (*(volatile s16 *)REG_ADDR_BG2PA)
#define REG_OFFSET_BG2PB 0x022u
#define REG_ADDR_BG2PB (REG_BASE + REG_OFFSET_BG2PB)
#define REG_BG2PB (*(volatile s16 *)REG_ADDR_BG2PB)
#define REG_OFFSET_BG2PC 0x024u
#define REG_ADDR_BG2PC (REG_BASE + REG_OFFSET_BG2PC)
#define REG_BG2PC (*(volatile s16 *)REG_ADDR_BG2PC)
#define REG_OFFSET_BG2PD 0x026u
#define REG_ADDR_BG2PD (REG_BASE + REG_OFFSET_BG2PD)
#define REG_BG2PD (*(volatile s16 *)REG_ADDR_BG2PD)
#define REG_OFFSET_BG2X 0x028u
#define REG_ADDR_BG2X (REG_BASE + REG_OFFSET_BG2X)
#define REG_BG2X (*(volatile u32 *)REG_ADDR_BG2X)
#define REG_OFFSET_BG2Y 0x02cu
#define REG_ADDR_BG2Y (REG_BASE + REG_OFFSET_BG2Y)
#define REG_BG2Y (*(volatile u32 *)REG_ADDR_BG2Y)
#define REG_OFFSET_BG3PA 0x030u
#define REG_ADDR_BG3PA (REG_BASE + REG_OFFSET_BG3PA)
#define REG_BG3PA (*(volatile s16 *)REG_ADDR_BG3PA)
#define REG_OFFSET_BG3PB 0x032u
#define REG_ADDR_BG3PB (REG_BASE + REG_OFFSET_BG3PB)
#define REG_BG3PB (*(volatile s16 *)REG_ADDR_BG3PB)
#define REG_OFFSET_BG3PC 0x034u
#define REG_ADDR_BG3PC (REG_BASE + REG_OFFSET_BG3PC)
#define REG_BG3PC (*(volatile s16 *)REG_ADDR_BG3PC)
#define REG_OFFSET_BG3PD 0x036u
#define REG_ADDR_BG3PD (REG_BASE + REG_OFFSET_BG3PD)
#define REG_BG3PD (*(volatile s16 *)REG_ADDR_BG3PD)
#define REG_OFFSET_BG3X 0x038u
#define REG_ADDR_BG3X (REG_BASE + REG_OFFSET_BG3X)
#define REG_BG3X (*(volatile u32 *)REG_ADDR_BG3X)
#define REG_OFFSET_BG3Y 0x03cu
#define REG_ADDR_BG3Y (REG_BASE + REG_OFFSET_BG3Y)
#define REG_BG3Y (*(volatile u32 *)REG_ADDR_BG3Y)

// Window coords pack two u8 edges per register (left<<8 | right / top<<8 | bottom).
#define REG_OFFSET_WIN0H 0x040u
#define REG_ADDR_WIN0H (REG_BASE + REG_OFFSET_WIN0H)
#define REG_WIN0H (*(volatile u16 *)REG_ADDR_WIN0H)
#define REG_OFFSET_WIN1H 0x042u
#define REG_ADDR_WIN1H (REG_BASE + REG_OFFSET_WIN1H)
#define REG_WIN1H (*(volatile u16 *)REG_ADDR_WIN1H)
#define REG_OFFSET_WIN0V 0x044u
#define REG_ADDR_WIN0V (REG_BASE + REG_OFFSET_WIN0V)
#define REG_WIN0V (*(volatile u16 *)REG_ADDR_WIN0V)
#define REG_OFFSET_WIN1V 0x046u
#define REG_ADDR_WIN1V (REG_BASE + REG_OFFSET_WIN1V)
#define REG_WIN1V (*(volatile u16 *)REG_ADDR_WIN1V)
#define REG_OFFSET_WININ 0x048u
#define REG_ADDR_WININ (REG_BASE + REG_OFFSET_WININ)
#define REG_WININ (*(volatile u16 *)REG_ADDR_WININ)
#define REG_OFFSET_WINOUT 0x04au
#define REG_ADDR_WINOUT (REG_BASE + REG_OFFSET_WINOUT)
#define REG_WINOUT (*(volatile u16 *)REG_ADDR_WINOUT)
#define REG_OFFSET_MOSAIC 0x04cu
#define REG_ADDR_MOSAIC (REG_BASE + REG_OFFSET_MOSAIC)
#define REG_MOSAIC (*(volatile u16 *)REG_ADDR_MOSAIC)

// Color blending: BLDCNT picks the layers/effect, BLDALPHA/BLDY the weights.
#define REG_OFFSET_BLDCNT 0x050u
#define REG_ADDR_BLDCNT (REG_BASE + REG_OFFSET_BLDCNT)
#define REG_BLDCNT (*(volatile u16 *)REG_ADDR_BLDCNT)
#define REG_OFFSET_BLDALPHA 0x052u
#define REG_ADDR_BLDALPHA (REG_BASE + REG_OFFSET_BLDALPHA)
#define REG_BLDALPHA (*(volatile u16 *)REG_ADDR_BLDALPHA)
#define REG_OFFSET_BLDY 0x054u
#define REG_ADDR_BLDY (REG_BASE + REG_OFFSET_BLDY)
#define REG_BLDY (*(volatile u16 *)REG_ADDR_BLDY)

// Sound.
#define REG_OFFSET_SOUNDCNT_L 0x080u
#define REG_ADDR_SOUNDCNT_L (REG_BASE + REG_OFFSET_SOUNDCNT_L)
#define REG_SOUNDCNT_L (*(volatile u16 *)REG_ADDR_SOUNDCNT_L)
#define REG_OFFSET_SOUNDCNT_H 0x082u
#define REG_ADDR_SOUNDCNT_H (REG_BASE + REG_OFFSET_SOUNDCNT_H)
#define REG_SOUNDCNT_H (*(volatile u16 *)REG_ADDR_SOUNDCNT_H)
#define REG_OFFSET_SOUNDCNT_X 0x084u
#define REG_ADDR_SOUNDCNT_X (REG_BASE + REG_OFFSET_SOUNDCNT_X)
#define REG_SOUNDCNT_X (*(volatile u16 *)REG_ADDR_SOUNDCNT_X)
#define REG_OFFSET_SOUNDBIAS 0x088u
#define REG_ADDR_SOUNDBIAS (REG_BASE + REG_OFFSET_SOUNDBIAS)
#define REG_SOUNDBIAS (*(volatile u16 *)REG_ADDR_SOUNDBIAS)
#define REG_OFFSET_FIFO_A 0x0a0u
#define REG_ADDR_FIFO_A (REG_BASE + REG_OFFSET_FIFO_A)
#define REG_FIFO_A (*(volatile u32 *)REG_ADDR_FIFO_A)
#define REG_OFFSET_FIFO_B 0x0a4u
#define REG_ADDR_FIFO_B (REG_BASE + REG_OFFSET_FIFO_B)
#define REG_FIFO_B (*(volatile u32 *)REG_ADDR_FIFO_B)

// DMA channels 0-3: source, destination, then count + control.
#define REG_OFFSET_DMA0SAD 0x0b0u
#define REG_ADDR_DMA0SAD (REG_BASE + REG_OFFSET_DMA0SAD)
#define REG_DMA0SAD (*(volatile u32 *)REG_ADDR_DMA0SAD)
#define REG_OFFSET_DMA0DAD 0x0b4u
#define REG_ADDR_DMA0DAD (REG_BASE + REG_OFFSET_DMA0DAD)
#define REG_DMA0DAD (*(volatile u32 *)REG_ADDR_DMA0DAD)
#define REG_OFFSET_DMA0CNT_L 0x0b8u
#define REG_ADDR_DMA0CNT_L (REG_BASE + REG_OFFSET_DMA0CNT_L)
#define REG_DMA0CNT_L (*(volatile u16 *)REG_ADDR_DMA0CNT_L)
#define REG_OFFSET_DMA0CNT_H 0x0bau
#define REG_ADDR_DMA0CNT_H (REG_BASE + REG_OFFSET_DMA0CNT_H)
#define REG_DMA0CNT_H (*(volatile u16 *)REG_ADDR_DMA0CNT_H)
#define REG_OFFSET_DMA0CNT 0x0b8u
#define REG_ADDR_DMA0CNT (REG_BASE + REG_OFFSET_DMA0CNT)
#define REG_DMA0CNT (*(volatile u32 *)REG_ADDR_DMA0CNT)

#define REG_OFFSET_DMA1SAD 0x0bcu
#define REG_ADDR_DMA1SAD (REG_BASE + REG_OFFSET_DMA1SAD)
#define REG_DMA1SAD (*(volatile u32 *)REG_ADDR_DMA1SAD)
#define REG_OFFSET_DMA1DAD 0x0c0u
#define REG_ADDR_DMA1DAD (REG_BASE + REG_OFFSET_DMA1DAD)
#define REG_DMA1DAD (*(volatile u32 *)REG_ADDR_DMA1DAD)
#define REG_OFFSET_DMA1CNT_L 0x0c4u
#define REG_ADDR_DMA1CNT_L (REG_BASE + REG_OFFSET_DMA1CNT_L)
#define REG_DMA1CNT_L (*(volatile u16 *)REG_ADDR_DMA1CNT_L)
#define REG_OFFSET_DMA1CNT_H 0x0c6u
#define REG_ADDR_DMA1CNT_H (REG_BASE + REG_OFFSET_DMA1CNT_H)
#define REG_DMA1CNT_H (*(volatile u16 *)REG_ADDR_DMA1CNT_H)

#define REG_OFFSET_DMA2SAD 0x0c8u
#define REG_ADDR_DMA2SAD (REG_BASE + REG_OFFSET_DMA2SAD)
#define REG_DMA2SAD (*(volatile u32 *)REG_ADDR_DMA2SAD)
#define REG_OFFSET_DMA2DAD 0x0ccu
#define REG_ADDR_DMA2DAD (REG_BASE + REG_OFFSET_DMA2DAD)
#define REG_DMA2DAD (*(volatile u32 *)REG_ADDR_DMA2DAD)
#define REG_OFFSET_DMA2CNT_L 0x0d0u
#define REG_ADDR_DMA2CNT_L (REG_BASE + REG_OFFSET_DMA2CNT_L)
#define REG_DMA2CNT_L (*(volatile u16 *)REG_ADDR_DMA2CNT_L)
#define REG_OFFSET_DMA2CNT_H 0x0d2u
#define REG_ADDR_DMA2CNT_H (REG_BASE + REG_OFFSET_DMA2CNT_H)
#define REG_DMA2CNT_H (*(volatile u16 *)REG_ADDR_DMA2CNT_H)

#define REG_OFFSET_DMA3SAD 0x0d4u
#define REG_ADDR_DMA3SAD (REG_BASE + REG_OFFSET_DMA3SAD)
#define REG_DMA3SAD (*(volatile u32 *)REG_ADDR_DMA3SAD)
#define REG_OFFSET_DMA3DAD 0x0d8u
#define REG_ADDR_DMA3DAD (REG_BASE + REG_OFFSET_DMA3DAD)
#define REG_DMA3DAD (*(volatile u32 *)REG_ADDR_DMA3DAD)
#define REG_OFFSET_DMA3CNT_L 0x0dcu
#define REG_ADDR_DMA3CNT_L (REG_BASE + REG_OFFSET_DMA3CNT_L)
#define REG_DMA3CNT_L (*(volatile u16 *)REG_ADDR_DMA3CNT_L)
#define REG_OFFSET_DMA3CNT_H 0x0deu
#define REG_ADDR_DMA3CNT_H (REG_BASE + REG_OFFSET_DMA3CNT_H)
#define REG_DMA3CNT_H (*(volatile u16 *)REG_ADDR_DMA3CNT_H)

// REG_DMAnCNT_H fields.
#define DMA_SRC_FIXED (2 << 23)
#define DMA_32BIT (1 << 26)
#define DMA_ENABLE (1u << 31)

/**
 * Start a 32-bit DMA copy on the channel whose SAD register is at `sad` (REG_ADDR_DMAnSAD) and
 * fence it with a CNT readback: `count` words from `src` to `dst`, enable + 32-bit folded in.
 * `count` occupies CNT_L, so it must fit 16 bits. Materializes one base register +
 * displacement (not the per-register REG_DMAn* macros, which each fold to a separate literal).
 */
#define DMA_COPY32(sad, src, dst, count)                                                           \
    {                                                                                              \
        volatile u32 *dma_ = (volatile u32 *)(sad);                                                \
        dma_[0] = (u32)(src);                                                                      \
        dma_[1] = (u32)(dst);                                                                      \
        dma_[2] = DMA_ENABLE | DMA_32BIT | (count);                                                \
        dma_[2];                                                                                   \
    }
#define DMA0_COPY32(src, dst, count) DMA_COPY32(REG_ADDR_DMA0SAD, src, dst, count)
#define DMA3_COPY32(src, dst, count) DMA_COPY32(REG_ADDR_DMA3SAD, src, dst, count)

/**
 * Same shape for a 16-bit fill: `count` halfwords of the value at `src` (source address held
 * fixed) to `dst`.
 */
#define DMA_FILL16(sad, src, dst, count)                                                           \
    {                                                                                              \
        volatile u32 *dma_ = (volatile u32 *)(sad);                                                \
        dma_[0] = (u32)(src);                                                                      \
        dma_[1] = (u32)(dst);                                                                      \
        dma_[2] = DMA_ENABLE | DMA_SRC_FIXED | (count);                                            \
        dma_[2];                                                                                   \
    }
#define DMA3_FILL16(src, dst, count) DMA_FILL16(REG_ADDR_DMA3SAD, src, dst, count)

// Timers 0-3: counter value (RO) / reload (WO) + control.
#define REG_OFFSET_TM0CNT_L 0x100u
#define REG_ADDR_TM0CNT_L (REG_BASE + REG_OFFSET_TM0CNT_L)
#define REG_TM0CNT_L (*(volatile u16 *)REG_ADDR_TM0CNT_L)
#define REG_OFFSET_TM0CNT_H 0x102u
#define REG_ADDR_TM0CNT_H (REG_BASE + REG_OFFSET_TM0CNT_H)
#define REG_TM0CNT_H (*(volatile u16 *)REG_ADDR_TM0CNT_H)
#define REG_OFFSET_TM1CNT_L 0x104u
#define REG_ADDR_TM1CNT_L (REG_BASE + REG_OFFSET_TM1CNT_L)
#define REG_TM1CNT_L (*(volatile u16 *)REG_ADDR_TM1CNT_L)
#define REG_OFFSET_TM1CNT_H 0x106u
#define REG_ADDR_TM1CNT_H (REG_BASE + REG_OFFSET_TM1CNT_H)
#define REG_TM1CNT_H (*(volatile u16 *)REG_ADDR_TM1CNT_H)
#define REG_OFFSET_TM2CNT_L 0x108u
#define REG_ADDR_TM2CNT_L (REG_BASE + REG_OFFSET_TM2CNT_L)
#define REG_TM2CNT_L (*(volatile u16 *)REG_ADDR_TM2CNT_L)
#define REG_OFFSET_TM2CNT_H 0x10au
#define REG_ADDR_TM2CNT_H (REG_BASE + REG_OFFSET_TM2CNT_H)
#define REG_TM2CNT_H (*(volatile u16 *)REG_ADDR_TM2CNT_H)
#define REG_OFFSET_TM3CNT_L 0x10cu
#define REG_ADDR_TM3CNT_L (REG_BASE + REG_OFFSET_TM3CNT_L)
#define REG_TM3CNT_L (*(volatile u16 *)REG_ADDR_TM3CNT_L)
#define REG_OFFSET_TM3CNT_H 0x10eu
#define REG_ADDR_TM3CNT_H (REG_BASE + REG_OFFSET_TM3CNT_H)
#define REG_TM3CNT_H (*(volatile u16 *)REG_ADDR_TM3CNT_H)

// Serial IO (multiplayer link). SIODATA32 overlays SIOMULTI0-3.
#define REG_OFFSET_SIODATA32 0x120u
#define REG_ADDR_SIODATA32 (REG_BASE + REG_OFFSET_SIODATA32)
#define REG_SIODATA32 (*(volatile u32 *)REG_ADDR_SIODATA32)
#define REG_OFFSET_SIOMULTI0 0x120u
#define REG_ADDR_SIOMULTI0 (REG_BASE + REG_OFFSET_SIOMULTI0)
#define REG_SIOMULTI0 (*(volatile u16 *)REG_ADDR_SIOMULTI0)
#define REG_OFFSET_SIOMULTI1 0x122u
#define REG_ADDR_SIOMULTI1 (REG_BASE + REG_OFFSET_SIOMULTI1)
#define REG_SIOMULTI1 (*(volatile u16 *)REG_ADDR_SIOMULTI1)
#define REG_OFFSET_SIOMULTI2 0x124u
#define REG_ADDR_SIOMULTI2 (REG_BASE + REG_OFFSET_SIOMULTI2)
#define REG_SIOMULTI2 (*(volatile u16 *)REG_ADDR_SIOMULTI2)
#define REG_OFFSET_SIOMULTI3 0x126u
#define REG_ADDR_SIOMULTI3 (REG_BASE + REG_OFFSET_SIOMULTI3)
#define REG_SIOMULTI3 (*(volatile u16 *)REG_ADDR_SIOMULTI3)
#define REG_OFFSET_SIOCNT 0x128u
#define REG_ADDR_SIOCNT (REG_BASE + REG_OFFSET_SIOCNT)
#define REG_SIOCNT (*(volatile u16 *)REG_ADDR_SIOCNT)
#define REG_OFFSET_SIOMLT_SEND 0x12au
#define REG_ADDR_SIOMLT_SEND (REG_BASE + REG_OFFSET_SIOMLT_SEND)
#define REG_SIOMLT_SEND (*(volatile u16 *)REG_ADDR_SIOMLT_SEND)

// Keypad input (RO). Active-LOW: a bit reads 0 while its button is held.
// input_update flips the polarity: the key values it publishes for menu handlers
// (g_MenuKeysPressed, ...) are ACTIVE-HIGH with the same button bits as below.
#define REG_OFFSET_KEYINPUT 0x130u
#define REG_ADDR_KEYINPUT (REG_BASE + REG_OFFSET_KEYINPUT)
#define REG_KEYINPUT (*(volatile u16 *)REG_ADDR_KEYINPUT)

// REG_KEYINPUT bits.
#define A_BUTTON (1 << 0)
#define B_BUTTON (1 << 1)
#define SELECT_BUTTON (1 << 2)
#define START_BUTTON (1 << 3)
#define DPAD_RIGHT (1 << 4)
#define DPAD_LEFT (1 << 5)
#define DPAD_UP (1 << 6)
#define DPAD_DOWN (1 << 7)
#define R_BUTTON (1 << 8)
#define L_BUTTON (1 << 9)
#define DPAD_ANY (DPAD_RIGHT | DPAD_LEFT | DPAD_UP | DPAD_DOWN)

/**
 * All ten key bits; the rest of the word is reserved. Inverting these bits turns
 * the active-low register into active-high key state.
 */
#define KEYINPUT_KEY_MASK                                                                          \
    (A_BUTTON | B_BUTTON | SELECT_BUTTON | START_BUTTON | DPAD_ANY | R_BUTTON | L_BUTTON)

// Keypad IRQ control (RW): selects which keys raise an IRQ.
#define REG_OFFSET_KEYCNT 0x132u
#define REG_ADDR_KEYCNT (REG_BASE + REG_OFFSET_KEYCNT)
#define REG_KEYCNT (*(volatile u16 *)REG_ADDR_KEYCNT)

// REG_KEYCNT fields.
#define KEY_INTR_ENABLE 0x4000
#define KEY_OR_INTR 0x0000
#define KEY_AND_INTR 0x8000

// Serial IO control + GameCube joybus.
#define REG_OFFSET_RCNT 0x134u // SIO mode / direct GPIO control
#define REG_ADDR_RCNT (REG_BASE + REG_OFFSET_RCNT)
#define REG_RCNT (*(volatile u16 *)REG_ADDR_RCNT)
#define REG_OFFSET_JOYCNT 0x140u
#define REG_ADDR_JOYCNT (REG_BASE + REG_OFFSET_JOYCNT)
#define REG_JOYCNT (*(volatile u16 *)REG_ADDR_JOYCNT)
#define REG_OFFSET_JOY_RECV 0x150u
#define REG_ADDR_JOY_RECV (REG_BASE + REG_OFFSET_JOY_RECV)
#define REG_JOY_RECV (*(volatile u32 *)REG_ADDR_JOY_RECV)
#define REG_OFFSET_JOY_TRANS 0x154u
#define REG_ADDR_JOY_TRANS (REG_BASE + REG_OFFSET_JOY_TRANS)
#define REG_JOY_TRANS (*(volatile u32 *)REG_ADDR_JOY_TRANS)
#define REG_OFFSET_JOYSTAT 0x158u
#define REG_ADDR_JOYSTAT (REG_BASE + REG_OFFSET_JOYSTAT)
#define REG_JOYSTAT (*(volatile u16 *)REG_ADDR_JOYSTAT)

// Interrupt/system. Bit 0 = VBlank, bit 1 = HBlank in both IE and IF.
#define REG_OFFSET_IE 0x200u
#define REG_ADDR_IE (REG_BASE + REG_OFFSET_IE)
#define REG_IE (*(volatile u16 *)REG_ADDR_IE)

// Interrupt flags (1 = request latched; write 1 to ack).
#define REG_OFFSET_IF 0x202u
#define REG_ADDR_IF (REG_BASE + REG_OFFSET_IF)
#define REG_IF (*(volatile u16 *)REG_ADDR_IF)

// Game Pak waitstate control (RW). Bits 0-1 = SRAM wait (0..3 = 4,3,2,8 cycles);
// GBATEK recommends 8 cycles (3) for SRAM/FRAM access.
#define REG_OFFSET_WAITCNT 0x204u
#define REG_ADDR_WAITCNT (REG_BASE + REG_OFFSET_WAITCNT)
#define REG_WAITCNT (*(volatile u16 *)REG_ADDR_WAITCNT)

// Interrupt master enable.
#define REG_OFFSET_IME 0x208u
#define REG_ADDR_IME (REG_BASE + REG_OFFSET_IME)
#define REG_IME (*(volatile u16 *)REG_ADDR_IME)

// System control: POSTFLG = the boot flag the BIOS sets, HALTCNT = power-down requests.
#define REG_OFFSET_POSTFLG 0x300u
#define REG_ADDR_POSTFLG (REG_BASE + REG_OFFSET_POSTFLG)
#define REG_POSTFLG (*(volatile u8 *)REG_ADDR_POSTFLG)
#define REG_OFFSET_HALTCNT 0x301u
#define REG_ADDR_HALTCNT (REG_BASE + REG_OFFSET_HALTCNT)
#define REG_HALTCNT (*(volatile u8 *)REG_ADDR_HALTCNT)

// Palette RAM: the 256-entry BG and OBJ palettes.
#define PALETTE_RAM 0x05000000u
#define BG_PALETTE_RAM PALETTE_RAM
#define OBJ_PALETTE_RAM 0x05000200u

/** OAM (object attribute memory). */
#define OAM 0x07000000u

// VRAM block layout.
#define VRAM 0x06000000u
#define BG_VRAM VRAM
#define BG_CHAR_SIZE 0x4000u
#define BG_SCREEN_SIZE 0x800u
#define BG_CHAR_ADDR(n) (BG_VRAM + BG_CHAR_SIZE * (n))
#define BG_SCREEN_ADDR(n) (BG_VRAM + BG_SCREEN_SIZE * (n))

// BGnCNT fields. The game passes whole cnt words around,
// so both directions are needed: BGCNT_* compose one, BGCNT_GET_* read blocks back out.
#define BGCNT_PRIORITY(n) (n)          // 0-3; lower draws on top
#define BGCNT_CHARBASE(n) ((n) << 2)   // 0-3, 16 KiB blocks
#define BGCNT_SCREENBASE(n) ((n) << 8) // 0-31, 2 KiB blocks
#define BGCNT_GET_CHARBASE(cnt) (((cnt) >> 2) & 3u)
#define BGCNT_GET_SCREENBASE(cnt) (((cnt) >> 8) & 0x1fu)

// Screen geometry.
#define TILE_WIDTH 8
#define TILE_HEIGHT 8
#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 160
#define DISPLAY_TILE_WIDTH (DISPLAY_WIDTH / TILE_WIDTH)
#define DISPLAY_TILE_HEIGHT (DISPLAY_HEIGHT / TILE_HEIGHT)
