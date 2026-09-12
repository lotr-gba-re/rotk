.thumb
ranger_hollow_pages_enter_hook:
    hookEnter
    bl   rangerHollowPages_onShopEnter
    hookLeave
    ; replay scene_shop_enter's clobbered 8-byte prologue (no PC-relative loads):
    ;   push {r4,r5,r6,r7,lr} ; mov r7,r9 ; mov r6,r8 ; push {r6,r7}
    push {r4,r5,r6,r7,lr}
    mov  r7,r9
    mov  r6,r8
    push {r6,r7}
    hookReturn scene_shop_enter + hookTrampolineSize

.thumb
ranger_hollow_pages_tick_hook:
    hookEnter
    bl   rangerHollowPages_onShopInput
    hookLeave
    ; replay scene_shop_tick's clobbered 8-byte prologue, PC-relative ldrs hand-relocated:
    ;   push {r4,r5,r6,r7,lr} ; sub sp,#0x4 ; ldr r4,[SITE+0x5c] ; ldr r1,[SITE+0x60]
    push {r4,r5,r6,r7,lr}
    sub  sp,#0x4
    ldr  r4,=scene_shop_tick+0x5c          ; load the literal's address ...
    ldr  r4,[r4]                             ; ... then deref (g_PlayerGlobals base)
    ldr  r1,=scene_shop_tick+0x60
    ldr  r1,[r1]                             ; (activePlayerIndex offset in g_PlayerGlobals = 0x359)
    hookReturn scene_shop_tick + hookTrampolineSize

.thumb
ranger_hollow_pages_exit_hook:
    hookEnter
    bl   rangerHollowPages_onShopExit
    hookLeave
    ; replay scene_shop_onBack's clobbered 8-byte prologue, PC-relative ldr hand-relocated:
    ;   push {lr} ; ldr r1,[SITE+0x18] ; ldr r0,[r1,#0x10] ; cmp r0,#0x0
    push {lr}
    ldr  r1,=scene_shop_onBack+0x18
    ldr  r1,[r1]
    ldr  r0,[r1,#0x10]
    cmp  r0,#0x0
    hookReturn scene_shop_onBack + hookTrampolineSize
