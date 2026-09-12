---
name: gdb-scripts
description: "The mGBA gdb-stub session and gdb script authoring for this project (rotk-hacking). Covers the debug bundle (`make debug` -> build/debug/rotk.gdb / rotk_hack.gdb / utils.gdb with the `settle` command), what gdb mechanisms work against the mGBA stub (Ctrl-C interrupt yes; inferior calls fragile: settle to the main-loop tick first, mask REG_IME, unwind-on-signal), the gdb 17 python gotchas, and the timed-halt pattern (os.kill SIGINT). Authoring rule: prefer plain gdb; use python only when a script needs iteration, parsing, or wall-clock timing. GDB SCRIPTS ARE ONLY FOR HUMAN CONSUMPTION UNLESS EXPLICITLY SIGNED OFF BY THE USER: agents must not create or edit gdb scripts autonomously (unverifiable headless, version-fragile, can corrupt a live session). Use when asked about gdb/debugging the running game, or when extending gdb-scripts/ or local/gdb-scripts/."
---

# GDB Scripts for the mGBA Session

## Usage Rules (Read First)

- **GDB scripts are only for human consumption unless explicitly signed off by the user.** They cannot be tested headless (no mGBA display here), depend on the gdb version and the mGBA stub's behavior, and a bad one corrupts the live session.
  Agents read and use them freely; creating or editing them requires explicit user signoff.
- Two homes: committed `gdb-scripts/` (shared utilities, user-curated) and gitignored `local/gdb-scripts/` (personal scratch).
  The debug bundle emits `build/debug/utils.gdb` (generated, do not edit) which both rotk.gdb and rotk_hack.gdb source.
- A session corrupted by a failed inferior call (SIGILL, garbage PC, "corrupt stack" backtrace) cannot be repaired in place.
  Restart mGBA + gdb.

## Plain gdb First, Python Only When Needed

- Default to plain gdb: `define`, `break`/`commands`, `printf`, `if`/`while`, convenience vars.
  Model: utils.gdb's `settle` (~12 plain lines).
- Reach for python only when the script needs something plain gdb cannot do cleanly:
    - iterating over data or ranges (a sweep of ids) and parsing repo files (header enums, so name tables cannot drift);
    - wall-clock timing or background work (threading.Timer + os.kill SIGINT, the timed pause);
    - programmatic breakpoint control (a Breakpoint.stop() that auto-continues or stops, internal breakpoints);
    - exact per-breakpoint state preservation and graceful error handling (try/finally).
- Python costs: the gdb API version churn under "gdb Script Mechanics" (objfiles() in 17, .internal unreadable, post_event not processed for remote), the two-tier helper+define split, silent failures (a forgotten timer.start()), and a larger headless-untestable surface.
- Rule of thumb: if it fits a `commands` block or a ~10-line define, keep it plain.
  Escalate when a loop over data, a table lookup, a timer, or per-breakpoint state enters the picture (gdb-scripts/audio.gdb needs all four; settle needs none).

## The Session

- `mgba-qt -g <rom>` starts the gdb stub on :2345 (`-g` also breaks at entry).
  `make debug` emits build/debug/{rotk.gdb, rotk_hack.gdb, debug.elf, debug_stubs.elf, tu/\*.elf, utils.gdb}; load with `gdb -x build/debug/rotk.gdb -x <script>.gdb`.
  The generated scripts use absolute paths, connect with `target remote localhost:2345`, and `set remote interrupt-on-connect on`: mGBA's stub does not halt on attach (DEBUGGER_ENTER_ATTACHED sends no stop), so the interrupt makes the connect deterministic and the following -x scripts (e.g. audio.gdb) load without a manual Ctrl-C.
- Symbols: store functions at ROM addresses (thumb-bit set), DWARF types from include/\*.h, typed stubs for not-yet-decompiled functions.
- The canonical "sane user-mode main loop" stop is the rotk_main per-iteration update call at 0x080315f6 (= rotk_main+0xae).
  `settle` (from utils.gdb) runs the game to it; use before any inferior call or state poke.
  utils.gdb is plain gdb (no embedded python); its `commands` block must stay `silent`-only, because a `disable` inside is restored by gdb's commands bookkeeping and would clobber the trailing `enable`.

## What Works, What Is Fragile

- Always: breakpoints, `continue`, Ctrl-C interrupt (the stub answers \x03 while running), memory/register reads, `set {unsigned short}0xADDR = ...` writes.
- Inferior `call` is fragile on the GBA: the software mixer runs on a timer IRQ, and (a) an IRQ firing mid-call corrupts gdb's call frame, (b) a call STARTING inside the IWRAM IRQ handler runs in IRQ mode and corrupts the IRQ stack/regs.
  Recipe: `settle` first (user-mode main loop), mask REG_IME (0x04000208) around the call and restore the prior value, `set unwind-on-signal on` so a signal during a call unwinds cleanly instead of poisoning the session.
  Symptoms when skipped: SIGILL at addresses past the callee's end, stop PCs in weird regions (0x188, IWRAM mid-data), "previous frame identical (corrupt stack?)".
- Idempotent game functions (e.g. sfx_stopMusic) are safe to call repeatedly; a crash after repeated calls is the call machinery, not the function.
  Verify with the disassembly before blaming game code.
- The timed pause: `threading.Timer(secs, lambda: os.kill(os.getpid(), signal.SIGINT))` around `gdb.execute("continue")` is a timed Ctrl-C and works against the stub.
  `gdb.post_event(interrupt)` does NOT reliably halt a remote target (works native, not remote).
  Do not forget `timer.start()`.
- A user breakpoint firing during an automated run silently ends it early: disable user breakpoints (skip internal ones, identified by negative `bp.number`) for the run, restore in a finally.

## gdb Script Mechanics (Verified on gdb 17.2)

- `$argN` substitution in `define`: referencing `$arg1` with < 2 args errors at substitution time, even in dead branches.
  Guard with `if $argc >= 2 ... end` and route optional args through convenience variables.
- Multiline `python ... end` blocks do not work inside `define` (gdb's `end` terminates the define).
  Define helpers in a top-level python block; call them from single-line `python helper("$arg0", int(gdb.parse_and_eval("$iters")))`.
- `$arg0` substitutes textually into python code; convenience variables like `$iters` are not python and must be read via `gdb.parse_and_eval`.
- `__file__` is undefined in gdb python.
  Locate the repo via the main objfile: `gdb.current_progspace().objfiles()` (method in gdb 17, list property in 16).
  `gdb.objfile` is gone in 17; `gdb.Breakpoint.internal` is not readable in 17.
- Use raw even addresses for `break *0xADDR`: thumb-bit symbols have odd values, so `*rotk_main+0xae` lands one byte off.
- Inferior calls return to the stop PC; if that PC is in ROM, gdb falls back to hardware breakpoints for the return breakpoint ("automatically using hardware breakpoints for read-only addresses").
  Works on mGBA.
- A generated .gdb's python must not rely on `\n` escapes surviving a generator round-trip: use `print()` (adds its own newline) inside gdb python.

## Patterns Worth Copying

- Logging breakpoints that print and never halt: plain `commands ... silent ... continue` behind a convenience-var flag, or a `gdb.Breakpoint` whose `stop()` prints and returns False, toggled by a `define` (audio.gdb's `audio_log`, scenes.gdb's `trace_scenes`).
- Parse enum name tables from include/\*.h at load time (regex over the enum body) so scripts never drift from the headers (audio.gdb's `list_sfx`).
- House style: usage header comment, `define` commands, register reads via `$r0`, memory writes via `{type}0xADDR`, absolute `file` / `add-symbol-file` paths.
- Python helpers in a top-level block, thin `define` wrappers; every automated run disables user breakpoints and restores them in a finally.
- Plain gdb is the default; escalate to python only for iteration, parsing, timing, or exact breakpoint-state handling (see "Plain gdb First" above).
  `settle` is the plain model (~12 lines), gdb-scripts/audio.gdb the python model (timer + header parsing + sweep).
