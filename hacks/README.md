# ROM Hack Authoring

> [!warning]
> **This document is largely AI slop and exists at least in part to make AI agents do what I want.**
> In the future I plan to write better documentation.
> I hate LLM writing as much as the next person but I wanted to get this repo published rather than waiting for perfection that may never arrive.

Each patch is a self-contained directory under `hacks/patches/`.
One global build (`make hack`) composes the selected patches into one ROM.
`make dist` emits a verified BPS.
Tooling (`armips`, `flips`): see [docs/setup.md](../docs/setup.md).

[Mod scenes](docs/mod-scenes.md) (scenes added by patches) have their own doc.

## Building and Selecting

| Command                                   | Effect                                   |
| ----------------------------------------- | ---------------------------------------- |
| `make hack`                               | build from `hacks/patches.cfg`           |
| `make hack HACKS="fast_fade"`             | only these patches                       |
| `make hack HACKS="@cfg demo_table_patch"` | cfg selection plus an extra              |
| `make hack HACKS=""`                      | clean ROM (no patches)                   |
| `make hack HACKS='$(PATCHES)'`            | every patch dir, ignoring the cfg        |
| `make dist [HACKS=...]`                   | rom plus verified BPS at `dist/hack.bps` |

`HACKS` is a space-separated, de-duplicated list.
`@cfg` expands to `patches.cfg` (the default), so `@cfg foo` adds to the cfg selection.
No codegen: the Makefile passes the selection as armips `-equ HACK_<name> 0/1` and gcc `-DHACK_<name>=0/1`, and `hack.asm` wraps each patch in `.if`.
Addresses come from the memmap and symbol stores, so patches hold no hardcoded hex.

## Patch Kinds

A patch is one or more of these, and a directory may mix them.
The hook kinds are macros from `hook_macros.asm`, whose per-macro comments are the reference (bytes written, constraints).

- **Trampoline hook** (`hookTrampoline site, handler`): divert a game function through a handler in the `MEM_HOOK` region (read state, call C, change behaviour).
  Example: `menu_text` draws a string on the main menu via the game's text API.
- **Function replacement** (`hookReplace site, cBody`): point a game function's entry at a same-signature C implementation, leaving the original body dead code.
  Example: `mod_save` replaces `save_readSlot` / `save_writeSlot`; `fast_fade` replaces the twelve `gfx_*` screen transitions.
- **Stub** (`hookStub site`): neutralize a function entry with a bare return, for stock behaviour a patch removes rather than replaces.
- **Trap** (`hookTrap site`): freeze at a function entry, for ROM code a patch has made unreachable and wants to keep that way.
- **In-place edit** (`.org`): overwrite game instructions/bytes to change a constant or instruction.
  No handler, free space, or prologue replay.
  Example: `mod_save` redirects the SRAM wait-state setup and shrinks the EEPROM descriptor.
- **Data-table patch** (`TABLE_ENTRY`): overwrite individual entries of a reverse-engineered table (affixes, enemy stats, ...) as typed C, writing only the ones you touch.
  Example: `demo_table_patch` rewrites `AccessoryPrefixes[77]`.

All entry-hook macros come in an r3-dead variant (<= 3 register args) and an r3-live `*R3` variant (a 4th arg in r3).
The R3 variants need a different handler body, see `hook_macros.asm`.

## Adding a Patch

The wiring is the same for every kind: create `patches/<name>/`, list `<name>` in `patches.cfg`, and add the patch's `.if HACK_<name>` block to `hack.asm`:

- a trampoline hook adds its handler asm to the "hook handlers" include list and a `hookTrampoline` line to the "trampolines" list.
- `hookReplace`/`hookStub`/`hookTrap` calls and in-place `.org` edits go in one `.include` in the "patch includes" list (placed after the trampolines, so their `.org` never disturbs the HOOK-region layout).
- a data-table patch instead adds a `#if HACK_<name>` include to `hacks/tables.c`.

An enabled patch's `.c` files (except `tables.c`) compile and link into the modcode automatically.

### Trampoline Hook

1. Pick a hook site in Ghidra.
   A function entry is easiest: registers are known.
   The trampoline overwrites the first instructions, so they must be relocatable (no PC-relative loads, no branches) and replayed afterward.
   Mid-function sites are harder (flags/state live), so prefer clean boundaries.
2. Write the handler in `patches/<name>/<name>.asm`:
    ```asm
    .thumb
    <name>_hook:
        hookEnter                                  ; preserve args (r0-r2) + caller lr
        bl <name>_run                              ; C body, or inline asm for an asm-only patch
        hookLeave                                  ; restore them
        <hand-copied original prologue>            ; the clobbered bytes, hand-relocated
        hookReturn SomeGameFunction + hookTrampolineSize
    ```
    To run the original only conditionally, make the C body return a flag and branch on it before the replay: nonzero -> `hookLeave` + `bx lr` (the original is skipped), zero -> `hookLeave` + replay + `hookReturn`.
    `loot_filter` (r3-live) and `mod_options` use this to keep stock behavior in ROM.
3. For a C body, add `patches/<name>/<name>.c` (split over multiple `.c` files if that reads better) following the rules under "Writing the C Body".

### In-Place Edit

1. Find the site in Ghidra.
   Reference it as `Function + offset` (names from `game_symbols.inc`) so there is no bare hex.
   Note the original bytes in a comment.
2. `patches/<name>/<name>.asm` is just `.org` and the replacement, no macros:
    ```asm
    .thumb
    .org SomeGameFunction + 0x44   ; was: mov r0,#2
        mov r0, #5
    ```
    See `patches/mod_save/save_sram.asm`.

### Data-Table Patch

Write only the entries you touch.
The rest stays base ROM, so no duplication and no drift from the committed `carved/data` table.
The table must have a `config/data.cfg` row (`addr  Name  Type[N]`).
Example, `patches/demo_table_patch/tables.c`:

```c
#include "combat.h"
#include "tablepatch.h"

TABLE_ENTRY(ItemAffix, AccessoryPrefixes, 77) = {
    .nameId = 0x0522, .tier = 1,
    .stat0 = STAT_HEALTH, .val0 = 69,
    .stat1 = STAT_GEMFINDER, .val1 = 1,
    .stat2 = STAT_FIREBURST, .val2 = 1,
    .stat3 = STAT_EXTRA_TREASURE_PERCENT, .val3 = 127,
};
```

How it builds: `make hack` compiles `hacks/tables.c` (each entry lands in a `.tablepatch` section), `rotkit hack tablepatch` extracts each entry's bytes and emits `build/tablepatch.asm` with `.org <Table> + index*sizeof(entry)` splices that `hack.asm` includes.
The table address resolves by name from `game_symbols.inc`.
The stride is the entry's compiled size (exact, since it uses the same byte-verified struct as `carved/data`).
`make dist` encodes only the changed bytes into the BPS.

## Writing the C Body

Each enabled patch's `.c` files compile as their own translation units.
The objects link into one blob (`hack.ld`) that `hack.asm` `.incbin`s.
(`hacks/hack.c` is the modcode's always-present baseline TU: it roots the blob and guards the required patches.)
`modcode-syms` writes an armips equate for every non-`static` symbol at its linked address, so asm can `bl` any of them by name.

**Naming.** Prefix every link-global (non-`static`) function with the patch's camelCase name (`saveSlotPages_*`, `rangerHollowPages_*`).
`static` helpers stay translation-unit-local and need no prefix.
Macros get the patch's `UPPER_SNAKE` name.
`hacks/lib/` functions get the `hackLib_` prefix instead (`hackLib_createLrButtonIndicatorsInMainPool`).
Non-static symbols are link-global across the whole modcode, and one named exactly like a game function silently satisfies its `game_symbols.ld` PROVIDE and shadows the ROM function.

**Calling C from the glue.** armips `.incbin`s a flat binary with no symbol table, so after linking `rotkit hack modcode-syms` runs `nm` and writes `build/modcode_syms.inc` (armips equates at the linked addresses).
The glue then does `bl <name>_run` by name.

**Calling game functions.** A THUMB `bl` reaches only +-4 MiB and ARMv4T has no `blx`, but the modcode region is ~15 MiB from the game.
Compile with `-mlong-calls` and gcc emits `ldr rN,=target ; bl _call_via_rN` (the libgcc interwork veneer, available because we link with the gcc driver).
Game addresses come from `build/game_symbols.ld` (`PROVIDE(name = addr|1)`).
The C `#include`s the relevant subsystem header (e.g. `combat.h`) for prototypes and calls by name.

**libc is the ROM's.** `memcpy`, `malloc` and friends come from `include/libc.h` like any other game function, so the modcode links `-nodefaultlibs`: a `PROVIDE` only binds a symbol still undefined after the input files, so otherwise the host newlib claims those names and links a second copy into the modcode.
Compiler helpers still come from the host (`-lgcc`), since the ROM's set is agbcc's and lacks the unsigned division this compiler emits.

**Variables.**

- Locals on the stack and `const`/string literals in `.rodata` (ROM): both fine.
- Mutable globals (`.bss`) are **disabled**: the only candidate region (EWRAM top) is unverified for gameplay, so `MODCODE_BSS` is commented out in `layout.cfg` and `hack.ld` fails the build if a patch adds a global.
  Find free RAM, re-enable `MODCODE_BSS` before using it.
  `menu_text` avoids it by drawing idempotently.
- Initialized globals (`.data`) are forbidden: no ROM-to-RAM copy exists, and `hack.ld` `ASSERT`s `.data` is empty.

## Cooperating Patches

### Dependencies

A patch that builds on another guards its C body with `#if !HACK_<dep>` + `#error` (the Makefile defines `HACK_<name>` for every patch dir, enabled or not).
Example: `save_slot_pages` and `ranger_hollow_pages` both need a bigger save chip, provided by `mod_save`.
Their check lives in `patches/mod_save/save_layout.h`, which also derives the chip budget they share, so resizing one region rebases the others.

Disabling a patch takes its dependents with it: the build errors naming the missing patch instead of producing a broken ROM.

### Startup Tasks

Once-per-boot work goes in `runTasks` (`mod_startup.c`), one `#if HACK_<patch>` call per patch, with the function declared in that patch's own `<p>.h`.
It runs after `mod_save` has settled the save: a task sees a current-format image, once per boot, whether or not migration just ran.
Task order is unspecified.
If two tasks ever need sequencing, express that as a dependency between the patches.

`mod_save` itself is the one startup task that cannot live here: it decides whether the save image is trustworthy, so it runs ahead of the list.
`save_init` is stubbed out entirely, and `mod_startup`'s first-scene decision asks `modSave_isUpToDate` to either load the state or hand the image to the migration scene.

### Where Shared Code Lives

Shared code goes where its dependencies point, not where its callers are:

| Home                           | Depends on                 | Compiles                            |
| ------------------------------ | -------------------------- | ----------------------------------- |
| `hacks/lib/<n>.{c,h}`          | the stock ROM only         | always (in the blob even if unused) |
| `hacks/patches/<p>/<module>.h` | patch `<p>`, which owns it | with `<p>`                          |
| `hacks/<contract>.h`           | several patches, no owner  | with each includer                  |

`hacks/lib/` holds plain `.c`/`.h` pairs included as `lib/<name>.h`, with no build wiring.
Code that needs a `#if HACK_<dep>` guard belongs to that patch, not to `hacks/lib/`.

A patch exports its API through headers included as `patches/<p>/<module>.h`, each carrying the `#if !HACK_<p>` + `#error` guard.
Most patches export one header named after the patch.
A layered patch exports one header per layer so a dependent includes only what it needs: `mod_save` exports `save_sram.h`, `save_slots.h`, `save_format.h`, `save_state.h`, and `save_migrate.h`.

Ownerless contracts sit at the `hacks/` root: `mod_scenes.h` holds the scene-id table patches add rows to.
`save_layout.h` is not one of these: `mod_save` owns the save chip, so the chip budget lives in that patch and dependents include it from there.

### Conflicts

- **Same hook site**: two hook macros naming the same site fail `make hack`, which greps the site args across `hack.asm` and all patch asm.
  The check is textual, so name the site by its canonical game symbol.
  To run two bodies at one site, write one handler that calls each in turn.
- **Free space**: handlers grow in `MEM_HOOK`, C in `MEM_MODCODE` (`layout.cfg`).
  Overflow fails with an armips `.error` or an ld `ASSERT`.
- **Same table entry**: two `TABLE_ENTRY` for one `Table[index]` fail as a C redefinition of `__tp_<Table>__<index>`, or in `rotkit hack tablepatch` as a duplicate `(Table, index)`.
- **Same C symbol**: a duplicate non-static symbol fails at link time; the prefix rules above avoid it.
- **Same function, different mechanisms**: one patch `hookReplace`s a function while another `.org`s inside it.
  Nothing detects this: the `.org` lands in the dead body and does nothing.
  Give the function one owner that replaces it; other patches add their change inside that C body behind `#if HACK_<patch>`.
  `mod_startup` owns `game_initBoot` this way and `mod_save` adds its part there; `mod_scenes` does the same for `SceneHandlers`.
