#include "gba_io.h"
#include "system.h"
#include "variables.h"

// Bytes of IWRAM cleared from its base; the stacks and the BIOS IRQ vector above stay.
#define IWRAM_CLEAR_SIZE 0x7a00u

static void clearWorkRam(void);
static void clearVideoRam(void);
static void clearVram(void);
static void clearOam(void);
static void clearPalette(void);

/**
 * Zero the RAM the boot does not initialize: IWRAM and EWRAM past their statics, then VRAM,
 * OAM, and the palettes. Runs first thing in rotk_main.
 *
 * @romaddress 0x0803a170
 */
void system_clearRam(void)
{
    clearWorkRam();
    clearVideoRam();
}

/**
 * DMA-fill IWRAM up to IWRAM_CLEAR_SIZE and all of EWRAM with zero, starting past the statics
 * crt0 copied in from ROM.
 *
 * @romaddress 0x0803a180
 */
static void clearWorkRam(void)
{
    u32 iwramStaticsSize = IwramStaticsEnd - IwramStaticsStart;
    u32 ewramStaticsSize = EwramStaticsEnd - EwramStaticsStart;

    {
        volatile u32 zero = 0;

        DMA3_FILL32(&zero, IWRAM_BASE + iwramStaticsSize,
                    (IWRAM_CLEAR_SIZE - iwramStaticsSize) / 4);
    }
    {
        volatile u16 zero = 0;

        DMA3_FILL16(&zero, EWRAM_BASE + ewramStaticsSize, (EWRAM_SIZE - ewramStaticsSize) / 2);
    }
}

/**
 * @romaddress 0x0803a1f8
 */
static void clearVideoRam(void)
{
    clearVram();
    clearOam();
    clearPalette();
}

/**
 * @romaddress 0x0803a20c
 */
static void clearVram(void)
{
    volatile u16 zero = 0;

    DMA3_FILL16(&zero, VRAM, VRAM_SIZE / 2);
}

/**
 * @romaddress 0x0803a234
 */
static void clearOam(void)
{
    volatile u32 zero = 0;

    DMA3_FILL32(&zero, OAM, OAM_SIZE / 4);
}

/**
 * @romaddress 0x0803a258
 */
static void clearPalette(void)
{
    volatile u16 zero = 0;

    DMA3_FILL16(&zero, PALETTE_RAM, PALETTE_RAM_SIZE / 2);
}
