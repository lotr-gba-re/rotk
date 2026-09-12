/* Shared linker script for the modcode (all enabled patches' C bodies, linked into one
 * blob). Addresses come from the central memmap; no hardcoded hex. The flat binary
 * (objcopy -j .text -j .rodata) is .incbin'd by hack.asm at MEM_MODCODE. */

INCLUDE game_symbols.ld          /* PROVIDE(game funcs/globals) - from the stores */
INCLUDE memmap.ld                /* MEM_* + PROVIDE(hack state globals) - from hacks/layout.cfg */

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

    /* Injected .bss is DISABLED: MODCODE_BSS (top of EWRAM) is only confirmed free on the main
     * menu, gameplay-unverified, so it is commented out in hacks/layout.cfg. An empty .bss links
     * fine; adding a mutable global fails here. To enable: find free RAM, re-add MODCODE_BSS in
     * hacks/layout.cfg, and place this section at MEM_MODCODE_BSS again. */
    .bss (NOLOAD) : { *(.bss .bss.* COMMON) }
    ASSERT(SIZEOF(.bss) == 0,
           "injected .bss is disabled: MODCODE_BSS is unverified for gameplay -- find free RAM and re-enable it in hacks/layout.cfg")

    /DISCARD/ : { *(.ARM.exidx*) *(.comment) *(.note*) *(.ARM.attributes) }
}
