#pragma once

// Byte-matching hacks: register pinning and CSE-breaking asm barriers that steer agbcc's
// register allocation toward the original ROM bytes. Once the whole ROM is decompiled and
// byte-exactness no longer matters, build with ROTK_MATCH_HACKS=0 to expand them to plain C.
//
// See .agents/skills/byte-matching/SKILL.md for why each pattern exists.
#ifndef ROTK_MATCH_HACKS
#define ROTK_MATCH_HACKS 1
#endif

#if ROTK_MATCH_HACKS

/** Declare `name` pinned to hard register `reg` ("r0".."r7", "r8", "r9", "sl"). */
#define MATCH_PIN(type, name, reg) register type name asm(reg)

/** Make `x` opaque to CSE from this point on (emits no code). */
#define MATCH_BARRIER(x) asm("" : "+r"(x))

/** Evaluate `expr` into `x` with the result opaque to CSE (emits no code of its own). */
#define MATCH_FRESH(x, expr) asm("" : "=r"(x) : "0"(expr))

/** Scheduling barrier that agbcc cannot move code across (emits no code). */
#define MATCH_SCHED_BARRIER() __asm__ volatile("")

/**
 * Memory-clobbering barrier: invalidates CSE'd loads of global memory, forcing the next
 * use of a global expression to re-load from memory (emits no code of its own).
 */
#define MATCH_MEM_BARRIER() __asm__ volatile("" ::: "memory")

/**
 * Define x at this point with no code emitted (opaque asm output). Use to extend x's
 * live range to this point / silence -Wuninitialized for a value that is only assigned
 * on one branch. Expands to `x = 0` when off, so the plain-C build keeps the value
 * semantics (the ROM's register content there is coincidentally 0).
 */
#define MATCH_DEFINE(x) __asm__("" : "=r"(x))

/**
 * Emit a real register-to-register move with literal registers (MATCH_MOV("r0", "r4") ->
 * `movs r0, r4`). For copies that exist in the ROM but are dead at C level, so flow
 * deletes any source form. Expands to nothing when off.
 */
#define MATCH_MOV(dst_reg, src_reg) __asm__ volatile("movs " dst_reg ", " src_reg)

#else

#define MATCH_PIN(type, name, reg) type name
#define MATCH_BARRIER(x) ((void)0)
#define MATCH_FRESH(x, expr) ((x) = (expr))
#define MATCH_SCHED_BARRIER() ((void)0)
#define MATCH_MEM_BARRIER() ((void)0)
#define MATCH_DEFINE(x) ((x) = 0)
#define MATCH_MOV(dst_reg, src_reg) ((void)0)

#endif
