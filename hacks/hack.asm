; Main hack build file: composes the selected patches into one ROM.
;
; Pay attention to the section order. Files that have to introduce custom assembler routines
; (e.g. for hookTrampoline) must be included before all other files.

.gba
.open "rotk.gba",HACK_OUTFILE,0x08000000

; ---------------------------------- Symbols and Macros ----------------------------------
.include "build/game_symbols.inc"        ; game funcs/globals/struct fields by name
.include "build/modcode_syms.inc"        ; modcode entries by name (nm -> equates)
.include "build/memmap.inc"              ; MEM_HOOK / MEM_MODCODE / ... (hacks/layout.cfg)
.include "hacks/hook_macros.asm"         ; hookTrampoline / hookEnter / hookLeave / hookReturn

; ---------------------------- Hook Handlers / MEM_HOOK code -----------------------------
; Asm bodies, mostly used for hookTrampoline. These are all put into MEM_HOOK.
.org MEM_HOOK
.thumb
.if HACK_menu_text
    .include "hacks/patches/menu_text/menu_text.asm"
.endif
.if HACK_ranger_hollow_pages
    .include "hacks/patches/ranger_hollow_pages/ranger_hollow_pages.asm"
.endif
.if HACK_loot_qol
    .include "hacks/patches/loot_qol/loot_filter.asm"
    .include "hacks/patches/loot_qol/auto_loot.asm"
.endif
.if HACK_mod_options
    .include "hacks/patches/mod_options/mod_options.asm"
.endif
.if . > MEM_HOOK_END+1
    .error "hook region overflow - raise the HOOK budget in hacks/layout.cfg"
.endif

; ------------------------------------- Trampolines --------------------------------------
.if HACK_menu_text
    hookTrampoline scene_mainMenu_tick, menu_text_hook
.endif
.if HACK_ranger_hollow_pages
    hookTrampoline scene_shop_tick, ranger_hollow_pages_tick_hook
    hookTrampoline scene_shop_enter, ranger_hollow_pages_enter_hook
    hookTrampoline scene_shop_onBack, ranger_hollow_pages_exit_hook
.endif
.if HACK_loot_qol
    hookTrampolineR3 actor_lootPile_create, loot_filter_hook
    hookTrampoline actor_lootPile_tick, auto_loot_hook
.endif
.if HACK_mod_options
    hookTrampoline scene_optionsMenu_tick, mod_options_tick_hook
    hookTrampoline scene_optionsMenu_drawRow, mod_options_draw_row_hook
    hookTrampoline scene_optionsMenu_moveCursor, mod_options_move_cursor_hook
    hookTrampoline scene_optionsMenu_adjustValue, mod_options_adjust_value_hook
    hookTrampoline scene_optionsMenu_onConfirm, mod_options_on_confirm_hook
.endif

; ------------------------------------ Patch Includes ------------------------------------
; hookReplace/hookTrap/hookStub calls and in-place .org edits. The replacement bodies they name
; are C in the modcode, so these files write at game addresses only.
.if HACK_fast_fade
    .include "hacks/patches/fast_fade/fast_fade.asm"
.endif
.if HACK_mod_ram                         ; required: every build, see hacks/hack.c
    .include "hacks/patches/mod_ram/mod_ram.asm"
.endif
.if HACK_mod_scenes                      ; required: mod_save, see save_migrate.h
    .include "hacks/patches/mod_scenes/mod_scenes.asm"
.endif
.if HACK_mod_startup                     ; required: mod_save, see save_migrate.h
    .include "hacks/patches/mod_startup/mod_startup.asm"
.endif
.if HACK_mod_save                        ; required: every build, see hacks/hack.c
    .include "hacks/patches/mod_save/save_sram.asm"
    .include "hacks/patches/mod_save/save_slots.asm"
    .include "hacks/patches/mod_save/save_state.asm"
.endif
.if HACK_save_slot_pages
    .include "hacks/patches/save_slot_pages/save_slot_pages.asm"
.endif
.if HACK_chapter_select
    .include "hacks/patches/chapter_select/chapter_select.asm"
.endif

; ---------------------------------- Data-Table Patches ----------------------------------
; Table patches generated from hacks/tables.c by rotkit hack tablepatch.
.if HACK_HAVE_TABLES
    .include "build/tablepatch.asm"
.endif

; ----------------------------------- Compiled C Code ------------------------------------
.org MEM_MODCODE
    .incbin "build/modcode.bin"

; ------------------------------------- ROM Padding --------------------------------------
; Pad the output to the 32 MiB ceiling (0x08000000..0x09ffffff). The patches can push the ROM past
; the 16 MiB base size. A ROM sized between the standard steps can sometimes confuse emulators or
; flashcarts, so we pad to 32 MiB. We do this by setting the last byte to 0, the assembler pads
; the space in between automatically.
; The stock ROM with EEPROM save files would break with ROM size larger than 16 MiB, but as this
; mod uses SRAM-based saves (see mod_save) this is not an issue.
.org 0x09ffffff
    .db 0
.close
