; Loot filter: void ground drops that fail the active filter level.
; Conditional-skip trampoline hook on actor_lootPile_create (r3-live site: 4 register args,
; hence the hand-rolled frame below instead of hookEnter/hookLeave); a nonzero return from
; the C body means "handled - skip the original".

.thumb
loot_filter_hook:
    pop  {r3}                    ; the item arg, saved by the hookTrampolineR3 site
    push {r0-r3}                 ; the 4 args
    push {r4,lr}                 ; caller lr + 8-byte-alignment scratch
    mov  r0,r2                   ; lootFilter_shouldVoid(dropType, item)
    mov  r1,r3
    bl   lootFilter_shouldVoid
    cmp  r0,#0
    bne  loot_filter_void
    pop  {r4}                    ; alignment scratch
    pop  {r0}                    ; caller lr (the arg frame still holds the arg r0)
    mov  lr,r0
    pop  {r0-r3}                 ; the args
    ; replay actor_lootPile_create's clobbered 12-byte prologue:
    ;   push {r4,r5,r6,r7,lr} ; mov r7,r10 ; mov r6,r9 ; mov r5,r8 ; push {r5,r6,r7} ; sub sp,#0x8
    push {r4,r5,r6,r7,lr}
    mov  r7,r10
    mov  r6,r9
    mov  r5,r8
    push {r5,r6,r7}
    sub  sp,#0x8
    hookReturnR3 actor_lootPile_create + hookTrampolineR3Size
loot_filter_void:
    pop  {r4}                    ; alignment scratch
    pop  {r0}                    ; caller lr (void return: r0 is caller-saved)
    mov  lr,r0
    add  sp,#0x10                ; drop the arg frame
    bx   lr
