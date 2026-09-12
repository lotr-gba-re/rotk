; Mod options: an extensible settings page inside the options menu (scene 0x06).
; Conditional-skip trampoline hooks on the options-menu scene functions: a nonzero return
; from the C body means "handled - skip the original". PC-relative literal loads in the
; replayed prologues are hand-resolved to their symbols.

.thumb

mod_options_tick_hook:
    hookEnter
    bl   modOptions_tickHook
    cmp  r0,#0
    bne  mod_options_tick_skip
    hookLeave
    ;   push {r4,lr} ; ldr r1,=g_MenuKeysPressed ; ldr r0,=g_PlayerGlobals ; ldr r2,=0x359
    push {r4,lr}
    ldr  r1,=g_MenuKeysPressed
    ldr  r0,=g_PlayerGlobals
    ldr  r2,=0x359               ; offsetof(PlayerGlobals, activePlayerIndex)
    hookReturn scene_optionsMenu_tick + hookTrampolineSize
mod_options_tick_skip:
    hookLeave
    bx   lr

mod_options_draw_row_hook:
    hookEnter
    bl   modOptions_drawRowHook
    cmp  r0,#0
    bne  mod_options_draw_row_skip
    hookLeave
    ;   push {r4,r5,r6,r7,lr} ; mov r7,r8 ; push {r7} ; sub sp,#0x18
    push {r4,r5,r6,r7,lr}
    mov  r7,r8
    push {r7}
    sub  sp,#0x18
    hookReturn scene_optionsMenu_drawRow + hookTrampolineSize
mod_options_draw_row_skip:
    hookLeave
    bx   lr

mod_options_move_cursor_hook:
    hookEnter
    bl   modOptions_moveCursorHook
    cmp  r0,#0
    bne  mod_options_move_cursor_skip
    hookLeave
    ;   push {r4,r5,r6,r7,lr} ; sub sp,#0x4 ; ldr r4,=g_SceneCurrent ; ldr r7,[r4,#0x8]
    push {r4,r5,r6,r7,lr}
    sub  sp,#0x4
    ldr  r4,=g_SceneCurrent
    ldr  r7,[r4,#0x8]
    hookReturn scene_optionsMenu_moveCursor + hookTrampolineSize
mod_options_move_cursor_skip:
    hookLeave
    bx   lr

mod_options_adjust_value_hook:
    hookEnter
    bl   modOptions_adjustValueHook
    cmp  r0,#0
    bne  mod_options_adjust_value_skip
    hookLeave
    ;   push {r4,r5,lr} ; sub sp,#0x8 ; ldr r5,=g_SceneCurrent ; ldr r1,[r5,#0x8]
    push {r4,r5,lr}
    sub  sp,#0x8
    ldr  r5,=g_SceneCurrent
    ldr  r1,[r5,#0x8]
    hookReturn scene_optionsMenu_adjustValue + hookTrampolineSize
mod_options_adjust_value_skip:
    hookLeave
    bx   lr

mod_options_on_confirm_hook:
    hookEnter
    bl   modOptions_onConfirmHook
    cmp  r0,#0
    bne  mod_options_on_confirm_skip
    hookLeave
    ;   push {r4,lr} ; mov r1,#0 ; ldr r0,=g_SceneCurrent ; ldr r0,[r0,#0x8]
    push {r4,lr}
    mov  r1,#0
    ldr  r0,=g_SceneCurrent
    ldr  r0,[r0,#0x8]
    hookReturn scene_optionsMenu_onConfirm + hookTrampolineSize
mod_options_on_confirm_skip:
    hookLeave
    bx   lr
