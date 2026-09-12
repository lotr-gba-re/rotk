.thumb
menu_text_hook:
    hookEnter
    bl   menuText_run
    hookLeave
    ; replay scene_mainMenu_tick's clobbered 8-byte prologue, PC-relative ldr hand-relocated:
    ;   push {r4,r5,lr} ; sub sp,#0xc ; ldr r5,[0x08048120] ; ldr r0,[r5,#0x10]
    push {r4,r5,lr}
    sub  sp,#0xc
    ldr  r5,=0x08048120         ; load the literal's address ...
    ldr  r5,[r5]                ; ... then deref == original `ldr r5,[0x08048120]`
    ldr  r0,[r5,#0x10]
    hookReturn scene_mainMenu_tick + hookTrampolineSize
