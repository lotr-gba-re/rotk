.thumb
auto_loot_hook:
    hookEnter                    ; preserve the hooked fn's arg (r0) + lr
    bl   autoLoot_onDropUpdate
    hookLeave                    ; restore them
    ; replay actor_lootPile_tick's clobbered 8-byte prologue:
    ;   push {r4,r5,r6,r7,lr} ; mov r7,r10 ; mov r6,r9 ; mov r5,r8
    push {r4,r5,r6,r7,lr}
    mov  r7,r10
    mov  r6,r9
    mov  r5,r8
    hookReturn actor_lootPile_tick + hookTrampolineSize
