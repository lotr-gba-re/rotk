#pragma once

#include "types.h"

// Low-level EEPROM driver.
//
// The GBA EEPROM chip lives behind the cartridge bus at 0x0D000000+ (Bit 0 of the
// data bus = EEPROM serial data line). All bit traffic is shipped by DMA3 in 16-bit
// mode (one halfword per bit, bit 0 is significant), with WS2 waitstates set to 8
// cycles. See GBATEK (GBA Cart Backup EEPROM).

s32 eeprom_readBlock(u16 blockIndex, void *dst);

/** bytes per EEPROM block (64 bits / 8 bytes per read or write) */
#define EEPROM_BLOCK_SIZE 8
/** total size of the 512-byte (4-Kbit) EEPROM image */
#define EEPROM_SIZE_BYTES 0x200
/** total blocks in the EEPROM image (EEPROM_SIZE_BYTES / EEPROM_BLOCK_SIZE) */
#define EEPROM_TOTAL_BLOCKS (EEPROM_SIZE_BYTES / EEPROM_BLOCK_SIZE)

/**
 * EEPROM device descriptor selected by eeprom_selectType.
 * Two known variants in ROM: EepromDesc512Byte (4-Kbit) and EepromDesc8KByte (64-Kbit).
 */
typedef struct EepromDesc
{
    /** total chip size in bytes (0x200 / 0x2000) */
    u32 sizeBytes;
    /** max block index (0x40 / 0x400); a block is 8 bytes / 64 bits */
    u16 maxBlock;
    /** WAITCNT bits to OR in before each transfer (0x0300 = WS2 first access = 8 cycles) */
    u16 waitcntBits;
    /** block-address bit width (6 / 14) */
    u8 addrBits;
    u8 field_0x9[3];
} EepromDesc;
/**
 * Timeout-watchdog setup passed to eeprom_startTimeoutTimer by eeprom_writeBlock.
 * Decoded {count=10, TM2CNT_L=0xFFBD, TM2CNT_H=0xC2} -> ~10 ms post-write Ready poll,
 * per gbatek's "10 ms timeout" rule for write completion.
 */
typedef struct EepromTimeoutConfig
{
    /** number of Timer overflows before the timeout flag is raised */
    u16 overflowCount;
    /** TMxCNT_L reload value */
    u16 reload;
    /** TMxCNT_H control (prescaler / IRQ enable / start) */
    u16 control;
} EepromTimeoutConfig;
/** Select the EEPROM device descriptor (param is Kbit count: 4 or 0x40=64); 0 ok, 1 unknown. */
u32 eeprom_selectType(s16 typeCode);
/**
 * Timer-overflow IRQ handler for the post-write Ready poll: counts g_EepromTimeoutCount
 * down; on reaching zero sets g_EepromTimeoutFlag to signal timeout.
 */
void eeprom_timeoutIrqHandler(void);
/**
 * Pick which Timer N (0-3) drives the timeout IRQ; stores the TMxCNT_L register
 * pointer for later Start/Stop, and installs eeprom_timeoutIrqHandler's address at
 * *callbackOut. Returns 1 if channel > 3.
 */
u32 eeprom_configureTimeoutTimer(u8 channel, void **callbackOut);
/** Arm the timeout Timer from cfg; enable its IRQ bit in IE and IME; clear the timeout flag. */
void eeprom_startTimeoutTimer(EepromTimeoutConfig *cfg);
/** Disarm the timeout Timer: clear the TMxCNT registers and restore IME / the IE bit. */
void eeprom_stopTimeoutTimer(void);
/**
 * Write one 8-byte block: build the write bitstream ("10" + addr + 64 data + "0" stop),
 * DMA3 it to 0x0D000000, then poll Bit 0 of 0x0D000000 for the chip Ready signal
 * under a ~10 ms timeout. Returns 0 on success, 0xC001 on timeout, 0x80FF out of range.
 */
u32 eeprom_writeBlock(u16 blockIndex, void *src);
/** Read one 8-byte block and compare it byte-for-byte with expected. Returns 0 on match. */
u32 eeprom_verifyBlock(u16 blockIndex, void *expected);
/** Retry eeprom_writeBlock up to 3x then verify. Returns the verify status. */
u32 eeprom_writeAndVerifyBlock(u16 blockIndex, void *src);
