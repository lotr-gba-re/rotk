---
name: byte-matching
description: agbcc codegen quirks and empirical technique for byte-matching decompiled C in this project (rotk-hacking). Use when decompiling a function, when `rotkit match` shows a DIFF, or when choosing between C source forms that should be "equivalent" but produce different bytes. Mechanism-level levers verified against a compiler rebuilt from the agbcc submodule, plus the AGBCC_TRACE_* diagnostics and match harness options (--keep, --cc).
---

# Byte-Matching agbcc Output

## Method

- **Test, don't theorize.** agbcc is a gcc 2.95.x fork; folding and scheduling are path-dependent, not semantic.
  Two equivalent source forms can produce different bytes, so never conclude a form cannot match by reasoning.
  Build a candidate: `uv run rotkit match <Function> <candidate.c> --full` (omit `--full` for an 8-row window around the first divergence).
  Only a leading `MATCH` means the whole split region is byte-identical; `OKSRC` means the named function is fine but sibling functions in the TU diverge.
- **One compiler, one flag set.** The whole ROM is one agbcc `-O2 -mthumb-interwork` compile.
  There is no per-function flag deviation to chase: a refusing function is a source or data-model problem, never flags.
  Sole exception: object-form libraries from an older toolchain, modeled by `old_agbcc` (see below).
- Every lever here was verified against a compiler rebuilt from the `agbcc/` submodule.
  The shipped `tools/agbcc` can lag the submodule (rebuild with `tools/setup_agbcc.sh`); if a lever stops working, check which compiler you are on first.
- **Work top to bottom.** Codegen is sequential; resolve the first DIFF and work down.
  Pass order (`rest_of_compilation`, toplev.c): expand -> jump -> cse -> gcse -> loop (x2) -> cse2 -> flow -> combine -> regmove -> lreg -> greg -> reload -> jump2 (thread + cross-jump) -> mach (`thumb_reorg`: literal pool) -> final.
  The pool is placed only at mach, after jump2, so a one-insn change anywhere can shift every `ldr [pc]` offset, and a cross-jump that moves a shared tail relocates the pool.
- **Keep changes that reduce the diff**; revert ones that raise it.
  Isolation trick: stub a region out (`if (0)`, or delete it) to check whether it causes a far-away spill.
- When candidate rows align 1:1 with the ROM (same opcodes, branch targets, lengths) and only register numbers differ, stop permuting source shapes and read the allocation traces; the remaining problem is pseudo topology, not statement structure.

## Make the Compiler Explain Itself

`uv run rotkit match <F> <cand.c> --keep DIR` writes `DIR/<Function>/`:

- `cand.s`: asm with `-dp` (every insn annotated `@ UID pattern [length]`, branch-provenance comments) plus `cand.elf`.
- `f.i.<pass>`: per-pass RTL dumps.
  Map a divergent row's UID from `cand.s` into the suspected pass's dump.
- `f.i.trace-lalloc` / `trace-galloc`: hard register chosen (or SPILL) per pseudo with the priority inputs (size, calls_crossed, live_length, n_refs).
  The fix is still source-level: change live ranges via declaration order, scoping, types.
- `f.i.trace-loop`: why each count-up loop was NOT reversed (the failed `check_dbra_loop` precondition; two lines per loop since loop opt runs twice).
- `f.i.trace-xjump`: each refused and successful tail merge with the reason.
- `f.i.trace-reload`: reload's register picks and skips with rotation state (see "Reload's spill-register rotation").
- `f.i.trace-combine`: one line per try_combine verdict; `not recognized` prints the exact RTL that failed.
- `f.i.regmove` (plain -da dump): operand-tying decisions with failure reasons.

Individual env vars without `--keep`: `AGBCC_TRACE_ALLOC` / `LOOP` / `XJUMP` / `RELOAD` / `COMBINE`; the harness relays the file to stderr.
Traces cover the whole TU.
Dumps show what the compiler did; traces answer why it did not do the other thing.

## Constants

**The master lever: -O2 force-registers constant operands before the backend sees them.** `flag_expensive_optimizations` makes `preserve_subexpressions_p()` return 1, so `expand_binop`/`emit_cmp_insn` (optabs.c) `force_reg` any constant with `rtx_cost > 2`: every and/or/xor constant, every compare constant >= 256 or negative, every add/sub constant outside -255..255.
The thumb.md constant special cases (the `bic` path, the extzv shift-pair masks, `cmn`) are dead at -O2.
Hence:

1. A ROM `bic` only comes from a runtime complement `x & ~y`, never `x & ~CONST`.
2. A shift-pair where a constant AND would do is a cast or shift+mask, never `x & 0xFFFF` (that is pool + `and`).
3. A `cmn` means hand asm or a non-standard origin; negative compares materialize (`movs #n; negs; cmp`).

**Materialization** (`movsi` splits, thumb.md):

| constant | emitted |
| --- | --- |
| 0..255 | `movs #imm` |
| -255..-1 | `movs #-imm; negs` |
| `c << i`, c < 256, 1 <= i <= 24 | `movs #c; lsls #i` (first i that works wins) |
| anything else | pool `ldr [pc, #off]` |

Add/sub immediates only -255..255; `cmp` immediate only 0..255 (`x == 256` -> `movs #128; lsls #1; cmp`).
`-1` anywhere is `movs #1; negs`.

**Literal pool** (`thumb_reorg`, mach): scans for constants failing the immediate constraints, finds the FIRST barrier within 1000 bytes, rewrites them to `ldr [pc]`, and dumps one table after the barrier (synthesizing a `b`-around barrier if none is near).
Identical constants in one dump share an entry; a function's constants can land in several dumps; HImode pool constants widen to SImode.

**Out-of-range immediate forces a copy.** Thumb's 3-operand `adds rd, rs, #imm` only reaches imm3, so `b = a + C` with C larger and `b != a` lowers to `adds rd, rs, #0; adds rd, #C`.
Two distinct locals keep that register copy; the single-variable `x += C` spelling folds it away.
A ROM `lsls rT, ..; adds rD, rT, #0; adds rD, #C` is therefore a two-local source, not a barrier candidate.

## Masks and Shift-Pairs

A shift-pair over low bits means a cast or a shift+mask, never a constant AND:

1. Casts of register values: `(u16)x` -> `lsls #16; lsrs #16`, `(s8)x` -> `lsls #24; asrs #24` (conversion expands are exempt from constant forcing).
   Memory operands are free (`ldrb`/`ldrh`/`ldrsb`/`ldrsh`).
2. `(x >> k) & (2^n - 1)`: combine composes the counts into `lsls #(32-n-k); lsrs #(32-n)`.
3. `(x << k) >> k` stays a literal pair (`asrs` signed, `lsrs` unsigned).
4. A decrement-and-test re-extending after the store (`ldrh; subs; strh; lsls #16; asrs #16; cmp`) does not imply an unsigned field plus cast: a signed field emits the same instructions (combine defers the load's extend past the sub), differing only in gcse-time insn count (see spill-slot buckets).

A cast on an already-shifted value folds: `(s16)(x >> 16)` on a register is just `asrs #16`; on memory it becomes `ldrsh [x, #2]`.
To keep an explicit `lsls #16; asrs #16` on a register value, store through an s16 LOCAL with the shift and the assignment as separate statements (`v >>= 16; x16 = v;`) and pass the local to the wider param: the HImode store is a free truncation and the widening read is a real conversion combine cannot fold.
The fused `s16 x16 = v >> 16;` diverges.

Signed div/mod by 2^k emit the round-up shape `cmp #0; bge; add #(2^k - 1); asr #k` (mod subtracts back).
Non-power-of-two `/` and `%` are `bl __divsi3`/`__udivsi3` libcalls (the `movs #K` right before is the divisor); agbcc never emits reciprocal-multiply magic.
Unsigned `n / 4` and `n >> 2` are interchangeable, and so are `n % 4` and `n & 3` into an unsigned local; `n % 4` into a SIGNED local adds a register copy after the `ands` (the mod result is a separate pseudo from the converted store) that `n & 3` does not.

cse associates nested shifts: `(lshiftrt (lshiftrt X 16) k)` composes to `(lshiftrt X 16+k)` whenever the outer operand's class holds a same-code shift, so a u16 return fed to `>> 1` normally emits the fused count.
A ROM keeping them separate means the shifted operand carries its own narrowing; adding a redundant cast on that operand is free (cse folds the second narrowing away) and splits the quantity.

## Tests and Booleans

- `if (x & mask)`: materialize mask, `ands` a copy, `cmp #0; b<cc>` (the cc0 model still emits the compare).
  Never `lsls; bmi`.
- `if (x & 0x80000000)` folds to a sign test: write `if (x < 0)`.
- `== 0`/`!= 0` skip the compare after a flag-setting producer; after a load or high-reg move the `cmp #0` appears.
- Combine can narrow a low-bit test to a QImode-subreg compare, and rewrite `(1 << x) & y` to `(y >> x) & 1`.
- `x < 0 ? -x : x` is ABS_EXPR: `copy; cmp #0; bge; negs`.
- Thumb has no setcc, so a comparison as a VALUE is branchy: default-set `movs #0; cmp; b<inv>; movs #1`.
  `(a != 0)` reuses the value's register with no leading `movs #0`.
  Constant rewrites collapse to 0: `x < 1` -> `x <= 0`, `x > -1` -> `x >= 0`, etc., so `if (a > -1)` emits `cmp #0; bge`.
- `cond ? 1 : 0` is just the comparison; `!x` in value context is `eor #1`.
  A comparison feeding an `if` never takes the value path.
- fold rewrites `x < C` / `x >= C` (positive C) to `x <= C-1` / `x > C-1` at the tree level, so a literal bound always emits `cmp #(C-1); bls/bhi`.
  A ROM `cmp #C; bcc/bcs` means the bound reached RTL unfolded: assign it to a local in the SAME basic block right before the test (cse substitutes the constant into the compare; combine leaves LTU/GEU alone).
  A function-scope init behind a tablejump stays a register compare.
- `(x & M) != 0` as a value, M a single bit: do_store_flag's bit-extract path emits `lsrs #n; ands #1`, and the `#1` pseudo CSEs with other `& 1` sites (inviting cross-jumps between arms).
  The ROM's `ands; negs; lsrs #31` is `-(x & M) >> 31`.
- A ROM `cmp #C; b<cc>; movs #1; b; movs #0; cmp #0; b<cc>` materializes a flag and immediately re-tests it.
  Storing 0/1 into a local and testing it in the next statement does NOT survive: jump-opt folds the store-test pair into the first branch.
  The roundtrip comes from a `bool`-returning `static inline` whose result feeds the caller's `if`: the two-arm set is emitted for the return value in r0 and the inliner's boundary blocks the fold.
  Reach for this shape before pinning the pair with a scheduling barrier.
  An out-of-line twin of that helper needs its own spelled-out body: `return helper(x);` inverts the arm order (`ble; movs #1` for the ROM's `bgt; movs #0`), and the twin usually cannot share the definition anyway, since file order tracks ROM order and the helper must precede its first call site to inline.
- **cse deletes a test a preceding guard dominates.** `cse_insn` ends with `record_jump_equiv` for every conditional jump, recording the fall-through condition onto the compared quantity; `fold_rtx` then deletes any later test that record dominates (`comparison_dominates_p`: EQ->LE/LEU/GE/GEU, LT->LE/NE, GT->GE/NE, LTU->LEU/NE, GTU->GEU/NE).
  So a ROM re-testing the same register right after a guard had either a cse block boundary or a different quantity there.
  A plain copy does NOT make a new quantity (`insert_regs`/`make_regs_eqv` merge it); a narrowing cast does.
  A dead negative clamp under a positive guard needs BOTH that and combine's `nonzero_bits` fold of `(lt X 0)` defeated, so it usually stays a `MATCH_BARRIER`.
- **A cse block boundary is harder to obtain than it looks**: `-fcse-follow-jumps` stitches back across a CODE_LABEL with exactly one user (recording the taken condition instead), and `cse_basic_block` walks through a label whose use count falls to 0.
  Only a label with two or more users, or a pre-loop-opt NOTE_INSN_LOOP_END, really ends one.

## Branch Layout

- `if (cond) A; else B` expands as branch-if-FALSE to else, A, `b` endif, B: the top branch is the inverted condition.
- For a fail path that jumps forward to a `movs r0, #0` tail: `if (ok) { ...; return success; } return 0;`, not early-return-fail.
- `return x > c;` and `return cond ? a : b;` pick different branch senses and layouts than the explicit `if (...) return a; return b;`.
  Use the ifs.
- A bare `if (c) goto L;` expands jumpif-TRUE plus a fixup `b`.
  For the condjump-FALSE + `b L` shape, wrap: `if (c) { MATCH_SCHED_BARRIER(); goto L; }`.
  In a `||` chain the barrier only works on the LAST operand; split into standalone ifs and test each (some match bare).
- Branch distance: conditional 2 bytes to +-254, 4 (`b<inv>; b`) to +-2044, 6 beyond (`b<inv>; bl`, a "far jump"); unconditional beyond +-2044 becomes `bl`.
  A far jump clobbers LR and forces `push {lr}` even in a leaf, so gaining or losing one changes the whole prologue.
- `if (x > c) x = 1; else x = 0;` reusing `x` emits the two-arm form; a fresh destination variable gets the default-set form.
  Try a plain s32 copy local before pinning.

## Loops

- Count-only induction variables reverse into down-counters (`check_dbra_loop`): `for (i = 0; i < n; i++)` becomes decrement-n with a bottom `cmp #0; bne` plus one top guard.
  Keep the source loop; trace-loop names a failed precondition.
- Address induction variables get their own register, initialized before the loop, stepped by the element size (strength reduction); givs stepping together merge.
- LICM hoists invariant pool loads; a global MEM load hoists only when the loop has no calls (or the load is RTX_UNCHANGING).
  Calls, multiple exits, tablejumps, or volatile refs disable invariants/strength reduction and change allocation.
- Test placement: `while`/`for` compile check-first as a jump down to a bottom test; the entry test can be duplicated ahead to remove the entry jump (refused on calls/labels/asm/>40 insns).
  `do-while` is check-last.
  The keyword is unobservable, the structure is: prefer `for` for counts, `while` over `do-while` unless the at-least-once run is load-bearing.
- The moved "test" is everything from the loop top to the LAST jump to the loop end within the first 30 insns (`expand_end_loop`).
  A `break` inside that window drags the call and the arms before it to the bottom, the arm after it lands ahead of the test, and the entry-test duplication is then refused (call in the exit block).
  ROM signature of a duplicated entry test: `cmp; bne exit` BEFORE the hoisted literals.
  Push the break's jump past the window by nesting so it follows the other arm's code (`if (a) { if (b) {...} else break; }`).
- An induction step emitted BEFORE intervening calls, off a caller-saved extend temp, is a statement in the loop body ahead of those calls; a `for` increment is emitted at the bottom off the counter's own (callee-saved) register.
- `while (*s++) ...` is load; `add #1`; `cmp`; `bne` with a `b` entry to the bottom test (load-then-add, not pre-increment addressing).
- The counter's signedness is load-bearing, the bound's spelling is not: an s32 counter against a sizeof-derived bound casts the bound only to silence -Werror sign-compare and is byte-identical to the signed literal, while switching the counter to u32 diverges.

## Shared Tails and Gotos

A cached local can suppress a ROM re-load and kill a cross-jump.
When two arms share a tail that re-reads a struct member, caching that member makes the redundancy eliminator rewrite one arm's read into a register copy, so the tails stop matching textually and jump2 refuses the merge.
Read the member directly at every site; the dispatch value still stays in a register via cse, while each arm keeps a real load.

jump2 cross-jumps identical tails into one block, usually at the LAST arm with earlier arms `b`-ing in.
Requirements: tails textually identical INCLUDING what follows them (`call; b` does not merge with `call; fallthrough`); CALL_INSN_FUNCTION_USAGE must match; asm/volatile never merge; REG_EQUAL/REG_EQUIV constant notes let differently-ordered constant setups merge; conditional jumps need 2 matching insns plus the opposing-jump pattern, simple jumps 1; branch lengths are ignored, so a merge can lengthen a branch.
A switch inside a static inline never cross-jumps (the inliner pre-merges returns through r0).

Merge direction can go BACKWARD: `find_cross_jump` needs `minimum` matching insns, and a CODE_LABEL met on the jump-side walk, or a jump-around-jump, gives free reductions; an exit that is a fall-through join (carrying a label) can therefore merge backward into an earlier exit.
Candidates are tried LIFO, later-in-stream sites first.

**goto ladders are structured control flow in disguise.** Arms branching to a common join plus early-outs skipping past it need no source-level goto:

- `if (c1) { A; goto L; } if (c2) { B; goto L; } ... L:` == the `if / else if / ... / else` ladder: the nested endif labels emit back to back at the join and jump-opt collapses the chain.
- `if (u == a || u == b) goto L;` == hanging the rest off `else if (u != a && u != b)`: TRUTH_ANDIF's jump-if-FALSE chain to the else label is the same bytes.
- `if (A && B) goto after; body;` == `if (!A || !B) { body; }` whenever the skipped code re-tests the same value: jump threading retargets the branch to the goto's old destination.
  This is also the general story for ROM early-outs to a shared tail: a guard branch straight to the tail is threading over a dominated re-test, not a goto.

Convert a whole ladder in one edit; partial conversions are not informative (the layout is decided by the collapsed label chain).

Gotos into a shared error/deny tail convert by duplicating the tail at each arm's END: invert the guard so the success path breaks out and the arm falls into its own tail copy; jump2 merges at the LAST arm's copy.
A duplicate left mid-arm (inside the guard `if`) flips the arm's branch sense; the placement decides, not the site count, and sites convert independently.
Merging two such arms with `||` plus an inner re-test does not work: thread_jumps never removes the redundant inner test.

An if/else-if chain with the shared suffix duplicated per arm merges the same way (signature: an arm branches FORWARD into the middle of the next arm).
Two `bl` sites to one callee where one is followed by a skip-branch: place that arm as the `else if` directly before the `else { shared call }`; its call lowers to `bl; b <past>` while the shared call falls through.

A constant call arg is delayed by copy-prop into the arg setup after the r2/r3 loads; if the arm must jump into a shared tail before those loads, materialize it early with MATCH_FRESH (not needed when jumping to a source-level label).

`goto L` and `return x;` are interchangeable per site while at least one jump to L survives; removing the LAST jump deletes the shared tail and can flip allocation far upstream.
When the last site cannot be a return, set a flag the downstream code already tests.

## Switches

Thumb has no casesi; `CASE_VALUES_THRESHOLD` is 5.
Five or more cases (range <= 10x count, index nonconstant) get subtract-min, `bhi`/`bls` bounds check, `mov pc` tablejump, table in case order with default-filled gaps.
Fewer get a balanced compare tree; a 1..3-case ROM switch is a tree.

`switch (x = expr)` is not interchangeable with hoisting the assignment above the switch (whole-block pseudo topology change).
Inside a compare-tree arm, record_jump_equiv makes the switch register a known constant: a constant equal to the case label that goes through force_reg (and/or/xor operands) CSEs onto it with no `movs`, but a shift count does not (the shift expander materializes its own), so a ROM shift whose count register is the switch value means the source names the switch variable there.

## Store-to-Load Forwarding

cse forwards a store to any later same-address, same-mode read within one extended basic block; the block scan stops only at a CODE_LABEL.
cse2 runs with after_loop set, so LOOP_END notes (`do {} while (0)`) do not survive it, and the 1000-insn hash flush is per-basic-block, never mid-function.
A ROM re-load right after a same-address store therefore means a control-flow join jump2 later erased, a second live address pseudo, or a `volatile`-qualified LOCAL (only `MEM_VOLATILE_P` makes `canon_hash` refuse to record).
Volatile tells: two reloads back to back inside one basic block, and free callee-saved registers in the prologue (a call-crossing SImode pseudo gets r8-r10 long before it spills, since `CALL_USED_REGISTERS` makes those callee-saved and the alt-class rule always admits HI_REGS at a 4-vs-8 cost ratio).
Confirm by checking every ROM call site of the value's producer: uniform store-to-frame-slot across all callers means the qualifier is source, so keep it.
Address escape, pointer locals, struct/union-typed returns and a volatile-qualified RETURN type all forward; only the local's own qualifier counts.
Otherwise, when neither is affordable (the label costs gcse insns, the alias costs a register), the load is a MATCH_MEM_BARRIER.
A different-mode read of the stored word (bitfield/byte view) does survive but changes the load width.

## Calls, Params, Prologue/Epilogue

- Caller-side arg extension reveals the PROTOTYPE param type: `lsls/lsrs #16` u16, `lsls/asrs #16` s16, `lsls/lsrs #24` u8.
  A callee reading with the other signedness means a call-site cast.
  Use this to type unknown callees.
- No extension insn, but a sharing tell: two u8 params fed the same wider value CSE into ONE truncation pseudo (one `adds rN, rM, #0` plus per-call copies); a u8/non-u8 pair gives two independent r0-coalesced pseudos.
  Can expose a wrongly-guessed u8 param on an undecompiled callee.
- Arg-copy POSITION reveals the caller LOCAL's width: a u8 local's arg-0 copy lands after all other args (its promote is free); a u32 local converts at precompute time and lands before them.
  Signature: only the arg-0 copy moved across other setup, instructions identical.
  Fix by widening the local (transitively inside static inline helpers too), never by pinning.
- **A constant materialised in a CALLEE-SAVED register before a call, consumed after it, comes from a function-scope local with an initialiser.** C89 declaration initialisers expand at block entry, ahead of the call, so the pseudo has `calls_crossed != 0` and is barred from r0-r3; a literal at the use site instead gets a short-lived pseudo in r0 after the call.
  Signature: the `push` gains a callee-saved reg and the `movs rN, #K` sits above the `bl` with its only use below.
  The local's type is free (u8/u16/u32 all promote to one SImode pseudo), but it must not be reassigned before that use: a promoted sub-word variable with a second def takes a real `lsls/lsrs` extend at the reassignment that combine will not fold away.
- Entry: param copies in declaration order, then ALL deferred promote-extends as one block.
  A `T x = param;` initializer orders after the extension block, so if the ROM copies before the mask, use the params directly.
  Register-save order follows first use; insert copy locals until the entry order matches.
- By-value structs <= 16 bytes travel in r0-r3 as a register block, stored to the stack at entry when address-taken or used piecewise.
  Entry movs can be member materialization, not source copies; struct params change pseudo topology vs scalars.
- **Arg emission order identifies a by-value struct param.** `precompute_register_parameters` (calls.c) walks args in order before any hard reg is touched and `copy_to_mode_reg`s every non-REG arg whose `rtx_cost > 2`; a MEM costs 10, so array/memory args always land there, while a `const_ok_for_arm` CONST_INT costs exactly 2 and is only emitted later in `load_register_parameters`.
  All-scalar args therefore give the loads FIRST and `movs rArg, #imm` last.
  BLKmode is an explicit precompute exception, so an aggregate arg is skipped and the order flips: `movs r0, #K; ldr r1,[sp,#0]; ldr r2,[sp,#4]; ldr r3,[sp,#8]; bl` is a `(scalar, 12-byte struct)` signature, not four scalars.
  No local-width or live-range change reaches this; the rule is cost-based.
  Once modelled, a member store just before the call forwards into the block load (a register or `movs rN, #0` replacing one `ldr`) while the store survives as a dead memory store, and an intervening call re-introduces the `ldr`.
- Returns > 4 bytes go via a hidden pointer in r0 and the first real arg shifts to r1.
  A <= 4-byte single-member struct returns in r0.
- Epilogue pop register encodes return size (interwork): void -> `pop {r0}; bx r0`, <= 4 bytes -> r1, <= 8 -> r2, > 8 -> r3.
- LR is pushed iff a callee-saved reg is pushed, the function is non-leaf, or a far jump is used, and on the standard compiler ANY branch counts (`far_jump_used_p` runs before branch shortening and caches the pessimistic answer).
  Every branchy leaf pushes lr; only straight-line leaves keep bare `bx lr`.
  Push order is ascending r0-r7 then lr; r8-r12 spill through low regs.
- Frame: one `add sp, #-N` (N < 512, else via a register).
  The outgoing-arg area is reserved function-wide if any call needs it.
  gcc 2.95 has NO dead-store elimination for memory: dead local stores stay; keep the ROM's useless-looking stores.
- A volatile asm anywhere forces by-value struct params into callee-saved regs.

**Single-use static inline helpers are codegen-neutral, with traps**: scalar (s32) params can reorder the caller's expansion where a macro with the same body matches (prefer macros for tail arithmetic); an incoming array/pointer arg materializes its address at body entry, not at the use (declare buffers inside the helper, and give pointer params the caller's own pointer type: a `void *` caller arg passed as a `u8 *` param adds a boundary copy that reshuffles the prologue, while a `void *` param with the `u8 *` local cast inside the body is neutral); a call result returned through a pinned local value-numbers back to r0 (put MATCH_BARRIER between assignment and return so each arm keeps its copy).

## Multiplies

Constant multiplies are expand_mult shift/add chains (`a*3` -> `lsls #1; add`; `a*7` -> `lsls #3; sub`) unless the constant itself must materialize, then `muls`.
Write the plain `x * K`; hand-expanded chains DIFF and `x*3` == `x*2+x` anyway.
`mul` needs dest == an input; its dest is earlyclobber and never ties, so it takes the LOWEST free register.
A ROM `muls` dest that is not the lowest free reg there is usually unreachable from source; pin it.

## Commutative Operand Order

A ROM `ands rConst, rValue` at the LAST test of an if/else-if chain, where the tested value dies there, means the destination local was preloaded with the constant (`t = CONST; t &= x;`): `expand_binop` swaps on `target == op1`.
The plain `if ((x & CONST) != 0)` spelling retargets the dying value's register instead.
Earlier tests in the same chain match plainly because the value is still live.

**regmove decides which operand the dest ties to.** For a commutative pattern that ties dest to operand 1 (thumb `andsi3`, `iorsi3`, ...), regmove's forward pass renames the dest into the first operand that is a plain REG *and* carries a REG_DEAD note on that insn; `if (GET_CODE (src) != REG) continue;` skips SUBREGs outright.
In a chain of `value & CONST` tests this means every test but the last ties to the constant's register (the value is still live) while the last ties to the value.
Reading a narrower-than-int memory rvalue directly, with no widened local, hands the AND `(subreg:SI (reg:HI N))` and every test ties to the constant.
ROM tell: the LAST `ands` in such a chain writing the mask register rather than the value register means there is no widened local; the narrow global is re-read at each test (gcse still merges the loads into one).

`expand_binop` swaps a commutative op's operands when op0 is not a REG while op1 is, when `target == op1`, or when op0 is a CONST_INT; otherwise source order reaches the asm.
Assigning to a sub-word local also makes the front end narrow BOTH operands (`(u16)(a + b)` == `(u16)a + (u16)b`), so an operand of another width arrives as a SUBREG and always loses the first slot: `narrow = mem + narrow` and `narrow = narrow + mem` emit the same `adds rD, rNarrow, rMem`.
A ROM `adds rD, rN, rM` whose rN is the freshly loaded value therefore means the other addend was a same-width local that dies at the add (`t = mem; dst = t + dst;`), which also keeps the following truncation writing straight into dst's register.
Read the inverse too: a ROM `lsls #16; asrs #16; add; lsls #16; lsrs #16` around a sub-word local means the add ran in a separate word-width temp (`s32 t = narrow + wide; narrow = t;`); the compound-assignment spelling on the sub-word lvalue narrows both operands and drops the leading extend.

## Data Model and Addressing

- Constant-index folding is path-dependent: direct `arr[1]` as a call arg folds to one biased literal; the same constant index through a static inline body, or initializing a loop induction variable, keeps `ldr =base; adds #off`.
  Wrappers with per-index constants match via one shared inline body.
- A runtime index with a constant bias does NOT fold into the base for typed arrays (gcc 2.95 has no extract_muldiv): `slots[ch - 49]` keeps `subs #49; lsl; add base` with the plain base literal.
  Only the pointer-cast form `((T *)&blob)[ch - 49]` folds the bias into the literal.
  A `subs` before the scaling means a source-level index subtraction.
- Several words reached off ONE base symbol (`ldr =base; ldr [rN, #off]`) are members of one struct-typed global: base+disp comes free, including per-iteration base reloads.
  Separate per-word externs each fold to their own literal and force hacks; model scratch words as one member array indexed by named constants.
  Overlays from `&sym.member` are the same base in disguise; use the typed member.
  Byte reads of a word member stay base+disp (`ldrb [base, #off]`); the `(u8 *)&sym.m` deref form materializes a biased literal.
- Address offset limits (LEGITIMATE_OFFSET): byte 0..31, halfword 0..62, word 0..124, SP-relative word 0..1020; beyond needs an explicit `adds` on the base first.
- **Bitfield accesses narrow to the smallest containing unit** (`store_bit_field`/`extract_bit_field` -> `get_best_mode`, `SLOW_BYTE_ACCESS 0`): a `u16`-based bitfield lying wholly inside one byte emits `ldrb/and/orr/strb` on that byte, byte for byte what a per-byte union view gives.
  So model a hardware register or table entry as ONE struct of `u16` (or `u32`) bitfields covering the whole unit, fields crossing byte boundaries included; slicing it into per-byte unions to chase observed byte-width ROM accesses buys nothing and costs a `.d.` level at every site.
  A field set to a constant matches the ROM's mask-free `orr` when the other bits are already known zero (`f = K` == `byte |= K<<shift`).
- Unions and lone-bitfield structs pad to 4-byte alignment and size; embedding one mid-struct shifts every later field and the array stride, DIFFing every index site of the type.
  For sub-u32 flag words, mark BOTH the union and the bitfield member struct `__attribute__((packed))` (either alone is not enough; `#pragma pack` is ignored).
  Splitting the blob into plain sibling fields also works.
  u32-word unions need no packing.
- `char` is unsigned (plain `ldrb`, no cast needed for codegen), but the build is -Werror and -Wchar-subscripts fires: index with `arr[(u8)*str]`.

## Register Allocation
- **cse cannot keep two live copies of one value in the same mode.** `insert_regs` merges a copy's dest into the source's quantity and `make_regs_eqv` promotes whichever register lives longest past `cse_basic_block_end` to `qty_first_reg`; `canon_reg` then rewrites every in-block use to it.
  Any chain of plain same-mode register copies collapses to one register, whichever variable names the source where, so stop permuting variable placement when the ROM shows a surviving `adds rN, rM, #0`.
  The escape hatch is the mode guard: `insert_regs` only joins a class element with `GET_MODE (classp->exp) == GET_MODE (x)`.
  A narrowing `u8 b = <word-sized local>` expands as a QImode truncation temp whose dest finds no QImode element in the SImode class, so no equivalence is recorded and the copy survives as two live pseudos (one caller-saved for pre-call uses, one callee-saved for a use past a join).
  `u8 b = <u8 param>` does NOT: the promoted subreg makes it a plain SImode move that folds.
  The word-sized intermediate is what makes the conversion real.

- **calls_crossed picks the register class**: `calls_crossed == 0` seeds the scan at r0; `!= 0` masks r0-r3 and makes r4 the first candidate.
  Nothing else makes a low callee-saved reg win.
  A ROM value sitting in r4 for three insns means the original local was ALSO live across a call further down: find a later site reading a value the ROM never recomputes and merge the two uses into ONE local.
  The long range then takes r4 legitimately.
  The negative form matters as much: `calls_crossed != 0` is the ONLY route to a callee-saved register for a short pseudo, and it needs a real reference PAST a call, so when the ROM's block has nothing after its last `bl` the shape is unreachable and the register must be pinned.
- **A zero bitfield store keeps its mask in SImode**: `u.d.bit = 0` lowers through store_bit_field as `dest = (dest & ~M) | 0`, so the SImode mask survives combine and emits `movs rN, #(M+1); negs rN, rN`, where the mask-view `u.p &= ~M` narrows to the field's mode and emits one `movs rN, #<byte>`.
  A negs pair on a sub-word mask therefore names the bitfield spelling; `= 1` folds to a bare `orrs` either way and does not distinguish the two.
- **The phantom const-0 pseudo**: every QImode member store expands via store_bit_field as `dest = (dest & 0) | src`; the forced const-0 operands become pseudos with REG_EQUIV 0, cse2 unifies them across sites into one long-lived pseudo, combine folds the masks away but the dead `set reg = 0` survives (no DCE after flow).
  Allocated, it emits a real useless `movs rN, #0` (+2 bytes, everything shifts); SPILLED, reload rematerializes from the note and emits NOTHING.
  The ROM is always the spill.
  Signature: one extra `movs #0` with no source counterpart; confirm via trace-galloc (`pseudo N -> SPILL` vs `-> rN`).
  A phantom cannot be pinned (no name in the source); it spills only when every call-preserved reg conflicts over its range, so several MATCH_PINs may exist solely to plug its holes, and filling one hole legitimately (see calls_crossed above) can delete them all at once.
- **`live_length` and `calls_crossed` are frozen at flow (pre-combine); only `n_refs` is recomputed.** Any pre-combine insn that combine later folds away still lengthens every live range spanning it, which makes it a byte-neutral allocation and gcse-bucket mover.
  The cheapest one: a u16/s16 local initialised from WORD arithmetic (`u16 t = g + 1; g = t;`) adds 2 pre-combine insns, since its HImode truncation temp survives cse's mode guard and combine folds it back.
  A u32 local is neutral, a u8 local is +3.
  Use it when a greg steal is losing by a sub-percent ratio margin.
- **greg can kick out lreg's assignment**: an allocno failing its preferred class scans hard regs from the END and steals any reg whose local n_refs/live_length ratio is lower (summed over every lreg pseudo in that hard reg, no floor_log2 factor; margins can be a fraction of a percent).
  Signature: the same pseudo assigned twice across trace-lalloc and trace-galloc.
  `regs_ever_live[i]` zeroes the local side, so ANY hard-reg use makes the reg un-stealable: that is how a MATCH_PIN protects a register it does not naturally own.
  Probe by shortening the local pseudo's live range (move its last use up): if the steal stops, only a live-range change fixes it, and when the ROM's statement order forbids that, pin and document the ratio in the comment.
- **Sub-word locals assign through a temp**: a u8/u16 local assigned from any expression expands as temp + copy (the promoted-subreg path) where a 32-bit local receives the value directly.
  When cse then substitutes the temp into the uses inside its own extended basic block but not into one past a CODE_LABEL, both pseudos stay live and the copy survives as a real `adds rN, rM, #0`.
  Widening the local to 32 bits deletes it.
- **gcse spill-slot buckets**: a DIFF that is ONLY paired `[sp, #a]` <-> `[sp, #b]` swaps can be hash-bucket fallout: reaching_reg pseudos allocate in bucket order, frame slots by ascending regno, and the bucket count is `n_insns/2 | 1` at gcse time.
  `hash_expr_1` is hand-computable (per level `code + mode`; REG adds `(56<<7)+regno`, CONST_INT adds `(51<<7)+mode+value`; ASHIFT=82, PLUS=70, SImode=6), so the insn-count windows giving the ROM order can be derived rather than guessed.
  The orders are valid only in narrow insn-count windows a few counts wide, so a restructure can land 1-2 insns short of the ROM's window with no near miss.
  Byte-neutral insn-count movers: a cross-jump duplicate (jump2 deletes it), spelling a double-read bitfield as an explicit shift, splitting a fused `--field == K` test on a signed sub-word field into decrement-then-test (+1: the re-read forwards to the store but keeps its extend).
  Dead reads do NOT shift the count: `delete_trivially_dead_insns` removes them before gcse.
  The count window is not the only re-bucketer: `hash_expr_1` mixes pseudo REGNOs, so a change in PSEUDO count ahead of an expression re-buckets it even with the bucket count identical.
  Check `max pseudo` in trace-galloc alongside the insn count.
  If no natural spelling lands in the window, something else is off (often a prototype width); prefer finding that.
  Two "irreducible" hacks can be mutually compensating movers: when a lone removal yields only sp-slot swaps, measure `grep -c '^(insn' f.i.gcse` and pair it with a second change that returns the count to the ROM's `n_insns/2|1` window instead of reverting (a per-arm cross-jump duplicate is a positive mover, a deleted pin or pointer local a negative one).
- **`find_reg` pass 0 skips callee-saved regs not yet in `regs_used_so_far`**, which is seeded with `regs_ever_live[i] || call_used_regs[i]`.
  On thumb that makes r0-r3 and **r12 candidates from the start while r4-r7 are not**, so a `calls_crossed == 0` pseudo whose low regs all conflict lands in `ip` rather than a free r6/r7.
  Signature: an address pseudo living in `ip` with `mov ip, rN` / `mov rN, ip` either side.
- **A sub-word local's promote temp is a 3-insn mover.** `u8 v; v = x >> 24;` expands at flow time as `t1 = x>>24; t2 = (QI)t1; v = zero_extend(t2)` where a u32 local is one insn.
  Those insns land inside neighbouring live ranges and can flip two allocnos' priority order; worth trying on a same-insns/wrong-registers diff that resists statement reordering.
- **`zero_extendqisi2` / `extendhisi2` always allocate a fresh intermediate** (the thumb.md expanders call `gen_reg_rtx` unconditionally for non-MEM operands), and lreg hands that 2-ref quantity the first free hard reg.
  A ROM shift pair running in place in the destination register is therefore unreachable from a cast: the intermediate has to be named and pinned.
- **Reload's spill-register rotation**: `allocate_reload_reg` scans spill_regs circularly from `last_spill_reg + 1`, carried function-wide, so scratch picks (mul fix-ups, extend zeros, const regs) rotate.
  "Same insns, wrong scratch regs" is a rotation-position difference: the traced-reload count before the divergence differs; walk trace-reload backward to where the counts diverge.
  Only reloads through `allocate_reload_reg` advance the rotation; find_dummy_reload "0"-matches (the base copy of `adds rN, rM, #0`) do not.
  Byte-neutral rotation advances: write a shared tail inline in an earlier arm (its duplicate's reloads run, then jump2 deletes it; the duplicate must merge exactly or the layout explodes), or turn a greg-level copy into a traced reload copy (barrier the local so combine cannot fold the expand_mult accumulator copy away, hand-write the chain step, pin as needed).
- **greg can steal a hard reg lreg already gave out**, when `local_reg_n_refs[r] / local_reg_live_length[r] < allocno_n_refs[a] / allocno_live_length[a]` (`retry_global_alloc`, raw ratios, no floor_log2); the local pseudo is then spilled and re-placed, typically in a high reg.
  Margins run under 1%, so one insn decides it.
- **live_length is stale, n_refs is fresh.** `life_analysis` runs BEFORE combine and is never re-run; the only refresh before regclass/local_alloc is `recompute_reg_usage`, which resets REG_N_SETS and REG_N_REFS alone.
  So insns that combine later deletes still inflate the live_length of every pseudo live across them, while their refs vanish, and that shifts allocation with zero byte cost.
  This is the lever behind "add a dead-looking temp to fix the registers", and it has two hard constraints.
  The temp must be a NAMED sub-word local holding a COMPUTED value (that is what forces the promote zero-extend pair plus a copy; a constant store adds nothing, and `x++`, `++x`, `x += 1`, `x = x + 1` are all the same insns since none of them names an object).
  And it must sit ABOVE the def of the pseudo whose register you want protected: placed inside that pseudo's range it inflates BOTH ratios' denominators and the steal survives.
  Together those usually pin the lever to one statement, so read the two ratios out of trace-galloc before reaching for a pin, and treat the position as derived rather than searched.
- **lreg picks by `floor_log2(n_refs) * n_refs / live_length`** (ties by qty order): a set-once-used-once constant (2 refs over 1 insn) outranks a base literal with two uses and takes r0.
  A ROM with identical insns but base in r0 and the constant in r1 is not reachable by reordering statements (the constant's init is moved next to its single use anyway); a block-scoped `MATCH_PIN` of the base is the cheap fix.
  Do not reach for `MATCH_FRESH(x, CONST)` there: its asm-input constant is a traced reload and advances the spill rotation for every later scratch pick in the function.
- **Keep a rolled value in ONE variable**: `x = f(); x -= y;` fuses into one long-lived pseudo that allocates early; splitting makes two short pseudos that allocate late and reshuffle the downstream chain.
  Across arms: one function-scope variable for several `x = f(); use(x);` sites fuses a high-n_refs allocno into a low callee-saved reg, where per-block locals each tie to r0 and the copies vanish.
  ROM signature: repeated `adds rN, r0, #0` right after calls.
  Loop counters are the same lever: a fresh per-loop counter is a short, dense allocno that wins an early pick and a low register, while one counter shared by several loops sums live_length and n_refs over all its segments (no live-range splitting) and drops down the pick order.
  ROM signature: several independent loops in one function all initialising their counter into the SAME register.
  Widening the counter's type is a different lever, not a substitute, and typically explodes the diff.

## Pointer Locals and Aggregates

A pointer-to-member intermediate (`u16 *p = &obj->f; *p |= x;`) SUPPRESSES the in-place address chain: dropping it lets the whole `base + idx*stride + off` chain accumulate in the destination's register, which is usually the ROM shape.
This is the inverse of the entry below; a pin compensating for such an intermediate goes away with it, and the two are mutually exclusive rather than additive.
A plain `T *b = a;` copy is usually folded away: cse's `make_regs_eqv` keeps a quantity's FIRST register canonical unless the new register's last mention is both past the cse block end and at/after the old one's.
A copy whose uses all precede the original's last use therefore keeps `n_refs 2` and loses its hard register to any competing base literal.
ROM signature that the copy is real: a pointer copied before a switch, one arm addressing off the copy (its register DYING at an in-place `adds rN, #off`) and a sibling arm addressing off the original, plus a call result landing in the copy's register in that sibling arm.
That is ONE scratch pointer reused for both roles: the later reassignment moves its last use past the original's, cse canonicalizes on it, the arm's whole address chain hangs off it, and its ref count buys the low register.
Try this before pinning a copy, a derived member pointer or a call result: one reassignment can delete a whole pin cluster.


- Pointer locals keep addresses alive: `T *p = &obj->f; *p -= v;` computes the address once where the direct form recomputes or chains off a neighboring access.
  Also fixes writeback reload/forward shapes.
  Ghidra's pointer temps are usually real pointer locals.
- A pointer can be load-bearing through its assignment POSITION alone: when the address spills, cse unifies direct member reads into the same address pseudo either way, so instructions are identical and only the pseudo's birth point differs; that decides its regno and hence its `alter_reg` frame slot.
  Probe in order: (1) direct reads with the assignment left in place, (2) assignment moved down to first use, (3) removed entirely.
  If (1) matches and (2)/(3) rotate spill slots, keep the ordinary dereferencing pointer (an assigned-never-read pointer is not plausible source).
- Removability does not track use count: a single-use pointer and a many-use pointer across calls can both be byte-neutral to delete (the load re-emits per site in both spellings).
  Test each one.
- An insn INTERLEAVED between a store's address chain and its value can only come from a real dereference whose address an earlier statement computed; every single-statement spelling puts the foreign insn wholly before or after the pair.
  This is the one ROM signature that settles a pointer before probing; absent it, the pointer is at best position-load-bearing.
- Address clusters are all-or-nothing: the ROM's temp-compute-then-copy shape (`ldr r0; adds r0, #off; ldrb; adds rN, r0, #0`) is produced by plain direct access at every site (cse computes the address once, greg supplies the copy) or by a scratch+pointer pair, but NOT by one intermediate pointer local (which pulls the chain in place, `ldr rN; adds rN, #off`).
  Try zero locals before two.
- A base literal in a register that DIES at the index add was its own local (`T *base = g.arr; T *p = &base[i];`).
  Direct `&g.arr[i]` folds the base load after the multiply; a single reused local gets the base load tied to its own register (combine_regs).
  Base in the RESULT register means one local.
- Statement splits steer expand order: `d = x; d += MEM;` expands each operand's address chain in statement order; the fused single expression hoists the extend first and diverges with whole-block fallout.
  Try split compound assignments, then value-read locals, before pinning; a pin's reserved reg also shifts greg priorities and can mask this simpler lever.
- An assignment expands the LHS address chain before the RHS.
  A ROM that loads the RHS array's base literal FIRST (before the LHS's own literals) means an RHS temp: `v = arr[idx]; obj.f = v;`.
  The literal born first takes the pick order with it (allocation and pool order differ from the fused form).
- Narrowing stores make extends unobservable: with an s16 lvalue, `x -= mem;` == `x = (u16)x - mem;`.
  A width cast is load-bearing only where the use is wider than the lvalue (e.g. `(u16)field` passed to an s32 param keeps a one-insn `ldrh` and CSEs with a preceding guard on the same field).
- Struct-member stores into a by-value PARAM block copy-prop (a partial def of the live multi-word pseudo): `local = expr; param.m = local;` keeps two registers alive where plain locals fold to one.
  Signature: the same value read from two registers in sibling arms.
  Only params work; a local struct's dead member stores get deleted.
- 8-byte copies through a two-word struct type (`*(Pair *)&dst = *(Pair *)src;`) give the `ldr; ldr; str; str` quad; two separate u32 assignments interleave with surrounding code instead.
- Two-word aggregates are DImode pseudos; their stack slot ORDER is allocation fallout (addressof homes in stream order, spills by ascending regno, slots grow upward).
  Restructure declarations (function vs block scope) instead of chasing slot order with hacks.

**Member stores survive as "dead" moves**: flow tests liveness of the WHOLE aggregate pseudo, so `a.x = dx` on a register-resident 2-word struct survives dead-code elimination even when overwritten before any read.
This is the clean source of C-level-dead ROM moves (besides MATCH_MOV and reload copies); scalars cannot do it (they coalesce or die).
Related levers:

- Mutate the by-value param itself (`a.x -= b.x`): identity-allocated to its incoming register pair, no entry copies, minimal pushes.
  A fresh aggregate local instead inherits conflicts from every live pseudo (combine sinks the single-use arithmetic into the member store, stretching the range) and lands in callee-saved regs.
- A whole-struct copy between OVERLAPPING pairs emits the HIGH word first; two y-then-x moves across a 2-word struct are one struct assignment, not two member stores.
- `m.x = ABS(m.x)` on the members (ternary macro; folds to ABS_EXPR) leaves a surviving temp copy on the second member (the first folds back onto its source; the store invalidates the aggregate in cse).
  The `if (m.x < 0) m.x = -m.x;` form diverges.
  Arm order of a following compare stays load-bearing.
- A local initialized by a whole-struct COPY of a param joins the param's register pair (combine_regs joins copy source and dest): the clean way to a named local there.
  Works only as the local's FIRST def.
- Struct-in/struct-out static inline helpers launder these shapes at the call site (the hidden-pointer return ABI does not apply when inlined); chained calls in a declaration initializer also match.

## Reading the ROM for Source Hints

- A dead global load followed by an unconditional store = an inlined helper/macro whose branches assign the same value.
- `movs r0, #0` re-materialized each loop iteration = no register was dedicated to the invariant zero; needs a pinned zero.
- Ghidra deletes unreachable blocks the ROM keeps (gcc 2.95 keeps dead code): if the disasm shows a dead clamp or test, keep it in the source.
- `ldmia/stmia` with 3 registers = 12-byte copy, 2 = 8 bytes; memcpy <= 48 bytes inlines as 12/8/4/2/1 blocks.

More tells live with their mechanisms: caller-side extensions and sharing (param types), arg-copy position (local widths), epilogue pop register (return size), a branchy leaf without `push {lr}` (old_agbcc), `subs` before scaling (source-level index bias), dying base registers and interleaved insns (pointer locals), repeated `adds rN, r0, #0` after calls (one shared variable).

## old_agbcc: The Second Origin

`tools/agbcc/bin/old_agbcc` (same source built with `-DOLD_COMPILER`) models the toolchain the object-form libraries were compiled with.
Complete delta: (1) no far-jump cache, so branchy leaves keep bare `bx lr` (the distinguishing byte vs the standard compiler); (2) `s_register_operand` accepts SUBREG(MEM), occasionally more reloading; (3) no `(use r0)` at function end (liveness only); (4) a check_dbra_loop robustness guard (no byte effect).
Test with `--cc old_agbcc`; a function matching only there is library code: keep the split region, record the origin in the `.c`, and do not force standard-compiler levers on it.

## Match Hacks: Last Resort, Macros Only

Exhaust clean options first: (1) write it the way a programmer would (plain expressions, inline helpers, macros for repeated expressions, proper types); (2) vary expression forms and types (bitfield vs shift vs mask, u8 vs u32 locals, declaration order); (3) vary structure (scoping, layout, loop shape, guards).
Persistent hacks usually signal wrong data-structure assumptions: if the bytes need a hack or an ad-hoc cast, the global struct is probably wrong or insufficient.
Never write local struct overlays or raw index casts; ask before changing a global struct (interactive) or declare a proper parallel struct (non-interactive).

`MATCH_BARRIER(x)` on a value that is dead afterwards is deleted before regmove (non-volatile asm with an unused output), so it cannot be used to probe whether keeping a value live fixes an allocation.

**Retest hacks whenever the surrounding source changes.** A hack pins one codegen decision; improving a neighbouring shape can make it dead weight that then survives as noise.
Bulk probe: copy the TU, prepend `#define ROTK_MATCH_HACKS 0`, and match each function against that copy.
`MATCH`/`OKSRC` means every hack in that function is stale and can just be deleted.

Apply hacks only via the `include/match_hacks.h` macros (never raw asm; no `#if ROTK_MATCH_HACKS` guards around use sites, the macros already compile away).
A macro is self-tagging: comment one only when the mechanism is non-obvious, one line, no numbers (evidence is re-derivable from the traces).
Reserve `// HACK:` comments for plain-C constructs that do not look like hacks (a load-bearing dead store, a destructive re-spelling, a per-arm duplicated tail).

| macro | use for |
| --- | --- |
| `MATCH_PIN(type, x, "rN")` | pin a value to a hard register |
| `MATCH_FRESH(x, expr)` | evaluate with the result opaque to CSE (kills cross-call caches). Wrapped in a statement expression around a CONSTANT (`({ MATCH_FRESH(x, CONST); x; })`), it forces expand-time materialization in the arm where constant folding would otherwise defer the add to reload and shift the whole allocation |
| `MATCH_BARRIER(x)` | make a variable opaque to CSE from here on (stops hoists and dead-test folds) |
| `MATCH_SCHED_BARRIER()` | stop code motion across this point (jump-threading) |
| `MATCH_MEM_BARRIER()` | invalidate CSE'd global loads |
| `MATCH_DEFINE(x)` | define with no code (live range, -Wuninitialized) |
| `MATCH_MOV("rN", "rM")` | a genuinely dead register move (volatile; a dead MATCH_FRESH is deleted, and flow deletes dead pseudo copies even pinned) |

**What a pin is often really doing.** On SMALL_REGISTER_CLASSES targets `canon_hash` sets `do_not_record` for any non-fixed hard register, so a pinned variable's value never enters cse's hash table at all.
Before accepting a pin, ask whether the ROM shape is simply "cse did not record this value", and reach for a source-level narrowing or a one-variable reuse instead.

### Pin Artifacts

lreg runs before greg, so a pinned variable already holds its hard reg at lreg time: address pseudos inherit it in place (`adds rN, #off`) instead of getting fresh registers, and CSE address folding changes.
A pin alters codegen beyond its own variable.
Signature: residual in-place address chains or register swaps around the pinned var's accesses.
Prefer dropping the pin and fixing the source structure or using pointer locals; as a last resort pin the artifacts too and tag them as artifacts of the primary pin.

Pins compensate for pins: a pin whose lone removal DIFFs can be byte-neutral once ANOTHER pin is gone.
Before recording N pins as irreducible, retest pairs and all-dropped; the residual of a coupled cluster is often one register swap, one allocation question (frequently the phantom const-0 pseudo above).

When the ROM runs a widening/truncation shift pair in place in one register, pin the pair's INTERMEDIATE (write the shifts as separate statements so it has a name); pinning the conversion result or the destination local leaves the same diff or worse.

### Two Live Registers Holding the Same Constant

Between two PSEUDOS this is unreachable from plain C, so stop early.
The collapse is on the USE side: `insert_regs`/`make_regs_eqv` fold the second dest into the first's quantity, `canon_reg` then rewrites every later use to the quantity's first (EARLIER) register unconditionally, and the orphaned `(set reg (const_int K))` dies.
`cse_end_of_basic_block` breaks only at a CODE_LABEL, at NOTE_INSN_SETJMP, and (only while `after_loop == 0`) at NOTE_INSN_LOOP_END, so two same-constant pseudos in one extended block always merge; `do {} while (0)` blocks cse1 but not cse2.
No local width, type, helper placement or statement order changes this.
A ROM that materializes one small constant into two pseudos, each used once, needs a MATCH_FRESH.

Hard registers are exempt, which is why argument setups repeat a `movs` the pseudos would have shared.
`canon_reg` never rewrites a hard reg, and the equivalence class is cost-ordered (`CHEAPER`), where thumb's CONST_COSTS scores a SET-source CONST_INT under 256 at 0 against 1 for a pseudo: the constant heads the class, so cse normalizes each arg-register set back to the literal even where the source named a local holding it.
At 256 and above the cost flips, the pseudo heads the class and cse substitutes it, giving one materialization plus `adds rD, rS, #0` per arg.
So a repeated `movs rN, #K`, K under 256, across call sites says nothing about the source (literal and local compile alike); a copy out of a held register means K is 256 or more.

### Dead Global Load Then Unconditional Store

Both arms of a conditional storing the same constant, with the ROM keeping the global load but no compare, is a `static inline` taking that global as its argument and returning the constant from both arms.
Argument evaluation is emitted before the body, the arms cross-jump at jump2, and the compare and branch die with the merged block while the load survives (no DCE that late).
A ternary, a local from a ternary, and a bare duplicated `if/else` all fail; only the inline boundary produces it.
Anything letting cse substitute a constant into one arm breaks the merge and leaves the branch.

### Is a MATCH_DEFINE Source-Faithful?

For an uninitialized-looking value reaching a cross-jump-merged tail: (1) substituting the constant it happens to hold kills the merge (copy-prop makes the tails differ), so a merging ROM with per-arm materialization means one arm assigns a variable; (2) substituting a provably-equal EXPRESSION costs a redundant `movs` (cse rewrites the introduced assignment to a constant); (3) reusing the very same LOCAL introduces no insn and is usually the real answer: the "uninitialized" read is the other arm's variable reaching the tail; (4) an uninitialized local WITHOUT the macro supplies no def for the shared pseudo and the merge dies.
If a MATCH_DEFINE stays, leave it at its use site; hoisting it blows up the live range.

## Maintaining This Skill

Add newly discovered quirks here when they are mechanism-level (name the pass and the deciding rule) and verified against the rebuilt compiler.
Fold them into the existing section they qualify; only add a section for a genuinely new mechanism.
Keep entries terse, describe code shapes generically, and do not cite repo functions or symbols (they go stale); a ROM byte pattern or minimal C shape is the durable form of evidence.
Every token here is read by every byte-matching task: when adding, look for something to trim.
