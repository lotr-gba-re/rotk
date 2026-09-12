// The hack's central C translation unit, the modcode counterpart to hack.asm: always compiled,
// so it roots the modcode blob's section (objcopy needs one) and is where the build's
// required-patch checks fire, ahead of the link. The anchor is never called; it is non-static,
// so the compiler emits it regardless.

// The hack build is a 32 MiB ROM, where the stock EEPROM save path targets the wrong window and
// fails silently. mod_save replaces it with the SRAM backend, so every hack build requires it.
#if !HACK_mod_save
#error "hack build requires the mod_save patch (stock EEPROM save breaks past 16 MiB)"
#endif

void __hack_anchor(void)
{
}
