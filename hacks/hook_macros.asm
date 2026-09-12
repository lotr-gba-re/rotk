; Scaffolding for THUMB function-entry hooks.
;
; A patch's <name>.asm:
;     .thumb
;     <name>_hook:
;         hookEnter                              ; preserve the hooked fn's args (r0-r2) + lr
;         bl <name>_run                          ; C body  - OR inline asm (asm-only patch)
;         hookLeave                              ; restore them
;         <hand-copied original prologue>         ; the clobbered bytes, verified relocatable
;         hookReturn SomeGameFunction + hookTrampolineSize   ; return into the game fn
; and the umbrella emits the trampoline:  hookTrampoline SomeGameFunction, <name>_hook
;
; This is the r3-dead form: valid at <=3-arg entry hooks (r3 is the trampoline's scratch).
; A 4-arg entry hook (r3 = the 4th arg) uses the r3-live forms at the bottom instead.

; The trampoline is always these 8 bytes, so the bytes it clobbers (and thus the return
; offset, for a site whose 8th byte is an instruction boundary) is a constant:
hookTrampolineSize equ 8

; trampoline at the hook site: ldr r3,[pc,#0] ; bx r3 ; .word handler|1
.macro hookTrampoline, site, handler
    .org site
    .thumb
    ldr r3,[pc,#0]             ; load the .word below; r3 is dead at the entry, lr untouched
    bx  r3
    .word handler|1
.endmacro

; whole-function replacement at a fn entry: jump to handler (a same-signature C fn)
; and never come back; it returns to the original caller via bx lr.
; No replay, so no relocatability constraint; r3 must still be dead (<=3 args).
; Unlike hookTrampoline's hardcoded [pc,#0], the assembler computes the PC
; offset and aligns the literal, so this stays correct at a 2-mod-4 site
; (where hookTrampoline would silently load the wrong word).
.macro hookReplace, site, handler
    .org site
    .thumb
    ldr r3, =handler|1
    bx  r3
    .pool
.endmacro

; replace a function entry with a bare return, for a function whose behaviour the patch
; removes rather than replaces.
.macro hookStub, site
    .org site
    .thumb
    bx  lr
.endmacro

; replace a function entry with an endless loop holding 0xDEAD1337 in r0, for a function
; we believe nothing can reach. A call we missed freezes instead of running dead ROM
; code: r0 is the marker, pc says which function, lr says who called it.
.macro hookTrap, site
    .org site
    .thumb
    ldr r0, =0xDEAD1337
    b   .
    .pool
.endmacro

; preserve the hooked fn's args (r0-r2) + caller lr across the handler body
.macro hookEnter
    push {r0-r2,lr}           ; 4 words -> SP stays 8-byte aligned for an AAPCS call
.endmacro

; restore them (THUMB-1 can't `pop {lr}`, so route it through the dead r3)
.macro hookLeave
    pop {r0-r2}
    pop {r3}
    mov lr,r3
.endmacro

; return to an absolute THUMB address (the hook passes site + clobber)
.macro hookReturn, addr
    ldr r3,=(addr|1)
    bx  r3
    .pool
.endmacro

; r3-live forms: entry hooks with 4 register args (r3 = the 4th arg).
; hookTrampolineR3 saves r3 before the jump (the handler pops it back); the literal is
; assembler-computed (.pool), so unlike hookTrampoline it stays correct at a 2-mod-4 site.
; It always clobbers 12 bytes (push/ldr/bx + pad + literal), so the replay covers
; site + hookTrampolineR3Size, and the return must be hookReturnR3.
hookTrampolineR3Size equ 12

.macro hookTrampolineR3, site, handler
    .org site
    .thumb
    push {r3}
    ldr  r3, =handler|1
    bx   r3
    .pool
.endmacro

; whole-function replacement preserving r3 (r3-live sites). r3 is the only register the
; site can `ldr` a literal into (THUMB-1 literal loads encode r0-r7 and r0-r2 hold args),
; so the handler address goes through it into r12 and r3 is put back before the jump.
.macro hookReplaceR3, site, handler
    .org site
    .thumb
    push {r3}
    ldr  r3, =handler|1
    mov  r12, r3
    pop  {r3}
    bx   r12
    .pool
.endmacro

; return to an absolute THUMB address preserving r3 (r3-live sites)
; Uses r4 as scratch: THUMB-1 ldr literal encodes r0-r7 only (r12 is out), and at the
; return point r0-r3 hold the args. Valid only when the replayed prologue saved r4 (the
; site's clobbered bytes contain a push covering r4), so its live value is on the stack.
.macro hookReturnR3, addr
    ldr r4,=(addr|1)
    bx  r4
    .pool
.endmacro
