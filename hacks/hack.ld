/* Shared linker script for the modcode (all enabled patches' C bodies, linked into one
 * blob). Addresses come from the central memmap; no hardcoded hex. The flat binary
 * (objcopy -j .text -j .rodata) is .incbin'd by hack.asm at MEM_MODCODE. */

INCLUDE game_symbols.ld          /* PROVIDE(game funcs/globals) - from the stores */
INCLUDE memmap.ld                /* MEM_* - from hacks/layout.cfg */

SECTIONS
{
    /* Modcode code+rodata in ROM free space */
    . = MEM_MODCODE;
    .text : { *(.text .text.* .glue_7 .glue_7t) }
    .rodata : { *(.rodata .rodata.*) }
    ASSERT(. <= MEM_MODCODE_END + 1, "modcode overflowed MEM_MODCODE (hacks/layout.cfg)")

    /* No crt0 => no ROM->RAM copy: forbid initialised .data. */
    .data : { *(.data .data.*) }
    ASSERT(SIZEOF(.data) == 0, "initialised .data unsupported (no crt0 to copy ROM->RAM)")

    /* Mutable globals in the RAM slice mod_ram takes off the game heap (zeroed at boot by system_clearRam). */
    . = MEM_MODCODE_BSS;
    .bss (NOLOAD) : { *(.bss .bss.* COMMON) }
    ASSERT(. <= MEM_MODCODE_BSS_END + 1, "modcode .bss overflowed MEM_MODCODE_BSS (hacks/layout.cfg)")

    /DISCARD/ : { *(.ARM.exidx*) *(.comment) *(.note*) *(.ARM.attributes) }
}
