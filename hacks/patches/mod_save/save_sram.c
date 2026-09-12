// Save backend swap: keep the save image in 32 KiB SRAM (0x0E000000) instead of serial
// EEPROM (0x0D000000). Replaces save_eeprom_readBlocks / save_eeprom_writeBlocks wholesale,
// so every caller lands on SRAM with the same block -> byte mapping (block i at byte i*8)
// and the save_layout.h budget unchanged. The serial driver underneath is dead code.
//
// Emulator detection: mGBA autodetects SRAM on the first 0x0E access; string scanners
// (VBA-family, open_agb_firm, flash-cart OSes) find the SRAM_V113 magic save_sram.asm
// stamps over the ROM's leftover EEPROM_V122. An undersized .sav keeps its bytes at
// offset 0 (mGBA extends it to 32 KiB, tail 0xFF-filled).
//
// Scope: this patch moves bytes to and from the chip and never interprets them. Converting
// an imported EEPROM-order .sav is save_migrate's job.

#include "save_sram.h"

#include "memory.h"
#include "save.h"
#include "save_layout.h"
#include "types.h"

/** Last byte of the 32 KiB SRAM area used for poke checks to verify SRAM is available. */
#define SRAM_POKE_CELL ((u8 *)0x0E007FFFu)

typedef void (*CopyFn)(u8 *dst, const u8 *src, u32 numBytes);

// Byte-copy loop (r0 = dst, r1 = src, r2 = numBytes). SRAM reads must run from
// WRAM-executed code per GBATEK (writes have no such restriction, but one routine
// keeps both paths identical), so the C side copies this into a stack buffer (IWRAM)
// and calls it there.
// ARM state, the GBA convention for IWRAM-resident code: IWRAM is a 32-bit bus, so
// ARM fetches at full speed (Thumb's density win is on 16-bit buses), and post-indexed
// addressing keeps the loop at 5 instructions. The Thumb caller switches state via
// bit 0 of the BX target; bx lr returns to Thumb automatically.
asm(".section .text\n"
    ".balign 4\n"
    ".arm\n"
    ".global ModSaveCopyArm\n"
    ".global ModSaveCopyArmEnd\n"
    "ModSaveCopyArm:\n"
    "    cmp   r2, #0\n"
    "    bxeq  lr\n"
    "1:  ldrb  r3, [r1], #1\n"
    "    strb  r3, [r0], #1\n"
    "    subs  r2, r2, #1\n"
    "    bne   1b\n"
    "    bx    lr\n"
    "ModSaveCopyArmEnd:\n"
    ".thumb\n");

extern const u32 ModSaveCopyArm[];
extern const u32 ModSaveCopyArmEnd[];

/**
 * The SRAM waitstate needs no setup here: save_sram.asm patches the literal in rotk_main's
 * boot-time WAITCNT write to include the 8-cycle SRAM wait, and nothing rewrites those bits
 * afterwards.
 */
void modSave_copy(u8 *dst, const u8 *src, u32 numBytes)
{
    // Stack VLA sized from the linked _End - _Start, so editing the routine above needs no
    // change here. The stack lives in IWRAM, satisfying the execute-from-WRAM rule.
    u32 code[(u32)(ModSaveCopyArmEnd - ModSaveCopyArm)];
    CopyFn copyFunction = (CopyFn)(u32)code;

    memory_memcpy(code, ModSaveCopyArm, sizeof(code));
    copyFunction(dst, src, numBytes);
}

void modSave_fill(u32 byteOffset, u32 numBytes, u8 value)
{
    // One chunk of `value` on the stack, then the ordinary copy loop over it. Chunked rather
    // than byte-at-a-time so the copy routine is staged into WRAM once per chunk, not once
    // per byte.
    u8 chunk[256];

    memory_memset(chunk, value, sizeof(chunk));

    while (numBytes != 0)
    {
        u32 n = numBytes < sizeof(chunk) ? numBytes : sizeof(chunk);
        modSave_copy(SRAM_BASE + byteOffset, chunk, n);
        byteOffset += n;
        numBytes -= n;
    }
}

/** Write the given value to the poke test address and read it straight back. */
static bool pokeCell(u8 value)
{
    u8 readback;

    modSave_copy(SRAM_POKE_CELL, &value, 1);
    modSave_copy(&readback, SRAM_POKE_CELL, 1);
    return readback == value;
}

/**
 * Prove a writable chip is really there: two probes with different values, so a bus with no SRAM
 * behind it (a fixed 0x00 or 0xFF readback) fails one of them. Not proof against a bus that
 * echoes the last write.
 *
 * The cell's original byte is saved and put back, so probing leaves the chip as it found it
 * and a caller that only classifies never writes. Both probes always run: the second must
 * not be skipped on a failed first, or the cell would be left holding the probe value.
 */
bool modSave_isChipPresent(void)
{
    u8 original;
    bool tookLow;
    bool tookHigh;

    modSave_copy(&original, SRAM_POKE_CELL, 1);
    tookLow = pokeCell(0x00u);
    tookHigh = pokeCell(SRAM_BLANK);
    modSave_copy(SRAM_POKE_CELL, &original, 1);

    return tookLow && tookHigh;
}

/**
 * Read `count` 8-byte blocks from SRAM (block i at byte i*8) into dst.
 *
 * REPLACES (whole function): save_eeprom_readBlocks @0x0803cf10.
 */
void modSave_readBlocks(u32 startBlock, s32 count, void *dst)
{
    modSave_copy(dst, SRAM_BASE + startBlock * EEPROM_BLOCK_SIZE, count * EEPROM_BLOCK_SIZE);
}

/**
 * Write `count` 8-byte blocks to SRAM (block i at byte i*8) from src.
 * SRAM needs no ready-wait or verify pass (GBATEK: no commands/delays, unlike EEPROM).
 *
 * REPLACES (whole function): save_eeprom_writeBlocks @0x0803cf40.
 */
void modSave_writeBlocks(u32 startBlock, s32 count, void *src)
{
    modSave_copy(SRAM_BASE + startBlock * EEPROM_BLOCK_SIZE, src, count * EEPROM_BLOCK_SIZE);
}
