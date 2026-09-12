.thumb

; save_eeprom_readBlocks entry: byte-copy from SRAM. Takes (startBlock, count, dst)
; like the original and returns via BX LR.
hookReplace save_eeprom_readBlocks, modSave_readBlocks

; save_eeprom_writeBlocks entry: byte-copy to SRAM, same signature.
hookReplace save_eeprom_writeBlocks, modSave_writeBlocks

; SRAM waitstate: rotk_main starts with `WAITCNT = (WAITCNT & 0xFFE3) | lit`, and its
; literal at +0xD8 is 0x4014 (WS0 3/1 + prefetch). Changing it to 0x4017 makes that same
; write set the SRAM wait to 8 cycles (bits 0-1 = 3, GBATEK); the power-on 4 is too fast
; for many carts. It runs before anything touches the chip, and no other WAITCNT write
; in the ROM touches bits 0-1 (the only other one sets WS2 bits), so it holds for the
; whole session and C never sets it.
.org rotk_main + 0xD8
    .word 0x4017

; The EEPROM driver functions below should now be dead code.
; Trap the functions to make sure any stray callers would hang the game.
hookTrap save_eeprom_readBegin
hookTrap save_eeprom_readEnd
hookTrap eeprom_readBlock
hookTrap eeprom_writeBlock
hookTrap eeprom_verifyBlock
hookTrap eeprom_writeAndVerifyBlock

; Save-type magic string for scanners (VBA-family, open_agb_firm, flash-cart OSes):
; overwrite the leftover SDK string so the ROM advertises exactly one save type.
; was: "EEPROM_V122\0"
.org EepromDesc512Byte - 0xC
    .ascii "SRAM_V113"
    .db 0, 0, 0
