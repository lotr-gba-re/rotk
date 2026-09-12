#pragma once

#include "types.h"

/**
 * GBA cartridge header (ROM @0x08000004). Standard hardware layout, minus its first word
 * at 0x08000000: that is the ARM entry branch, disassembled as code.
 */
typedef struct CartridgeHeader
{
    u8 nintendoLogo[156];
    u8 gameTitle[12];
    u8 gameCode[4];
    char makerCode[2];
    u8 fixedValue;
    u8 mainUnitCode;
    u8 deviceType;
    u8 reserved[7];
    u8 softwareVersion;
    u8 complementCheck;
    u8 reserved2[2];
} CartridgeHeader;

/** OamEntry.affineMode */
enum OamAffineMode
{
    OAM_AFFINE_OFF = 0,
    OAM_AFFINE_ON = 1,
    OAM_AFFINE_HIDDEN = 2, // OBJ not displayed
    OAM_AFFINE_DOUBLE = 3, // affine with a doubled bounding box
};

/** OamEntry.matrixIndex bits while affineMode is OAM_AFFINE_OFF */
enum OamMatrixFlip
{
    OAM_MATRIX_H_FLIP = 1 << 3,
    OAM_MATRIX_V_FLIP = 1 << 4,
};

/**
 * One OBJ's OAM entry (GBATEK "OBJ Attributes"): the three attribute halfwords plus the
 * affine parameter halfword that shares the 8-byte slot.
 */
typedef struct OamEntry
{
    u16 y : 8;
    u16 affineMode : 2; // enum OamAffineMode
    u16 objMode : 2;    // 0 normal, 1 semi-transparent, 2 OBJ window
    u16 mosaic : 1;
    u16 colors256 : 1; // 1 = 256/1, 0 = 16/16
    u16 objShape : 2;  // 0 square, 1 horizontal, 2 vertical

    u16 x : 9;
    u16 matrixIndex : 5; // affine matrix 0-31; while affineMode is OAM_AFFINE_OFF only its
                         // enum OamMatrixFlip bits apply
    u16 objSize : 2;     // paired with objShape

    u16 tileNumber : 10;
    u16 bgPriority : 2; // 0 = highest
    u16 paletteNumber : 4;

    u16 affineParam; // the slot's rotation/scaling parameter, unrelated to this OBJ
} OamEntry;

/** swi 0x0c wrapper: CpuFastSet (src, dst, control word: count<<2 | mode bits). */
void bios_cpuFastSet(const void *src, void *dst, u32 control);
/** swi 0x0b wrapper: CpuSet (src, dst, control word: count<<2 | mode bits). */
void bios_cpuSet(const void *src, void *dst, u32 control);
// bios_cpuSet / bios_cpuFastSet control-word mode bits (count occupies the low bits).
#define CPU_SET_SRC_FIXED (1 << 24)
#define CPU_SET_32BIT (1 << 26)
/** swi 0x06 wrapper: signed division, returns dividend / divisor. */
s32 bios_div(s32 dividend, s32 divisor);
/** swi 0x06 wrapper: signed division, returns dividend % divisor. */
s32 bios_divMod(s32 dividend, s32 divisor);
/** swi 0x0a wrapper: ArcTan2(x, y), returns the angle 0..0xffff for 0..2*pi. */
s32 bios_arcTan2(s32 x, s32 y);
