# Workflow

`make help` lists every target.
The help is quite bad, however, and all the makefiles are bad and sloppy, so here's some additional documentation.

## Building the ROM

`make verify` rebuilds the ROM to `build/rom.gba` and checks its SHA-1 against the original.
Everything that is not decompiled yet is included from the original ROM using `.incbin`.

Matched C is built with [a fork of `agbcc`](https://github.com/lotr-gba-re/agbcc), the compiler the Pokemon GBA decomps use.
The fork only adds debugging output, mostly useful to LLMs (or humans familiar with gcc internals).

## Config Files for Function and Data Names

`config/` is the single source of truth for what function or data lives where in the original ROM.
It is used to generate:

- The Ghidra project with symbols (`make ghidra`)
- The debug symbols for `gdb` debugging (`make debug`)
- Symbols for the ROM hack build (`make hack`)

The following files exist:

- `functions.cfg`: `address`, `arm` or `thumb`, `name`.
  The function types are taken from header files.
- `data.cfg`: `address`, `name`, `type`.
  Global variables and ROM data tables alike.
- `split.cfg`: `address`, `file`.
  Which source files replace ROM bytes in `make verify`.

Config files are tab-separated files.

To add a function, add its row to `functions.cfg` and its prototype to `include/<module>.h`.

To add a global or a data table, add its row to `data.cfg`, and declare its type in the module header if it is new.
Every `data.cfg` row becomes an `extern` in the generated `build/include/variables.h`, so C code can use it by name right away.

Naming rules are in [conventions.md](conventions.md).
`make format` sorts the rows, `make check-stores` checks for consistency issues.

## Everyday Building

Run `make check` after every change.
It lints the config files against the headers and `.c` files (`check-stores`) and then runs `verify`.

## Ghidra

`make ghidra` imports the ROM into a fresh Ghidra project (`ghidra_proj/`, gitignored) that contains all the functions and variables already named.
It always rebuilds from scratch, so run it when you want to browse the current state, not after every change.

Do not rename or annotate things in Ghidra, they get lost on the next rebuild.

## Decompiling a Function

1. Add the function to `config/functions.cfg` and declare it in its module header (see [conventions.md](conventions.md)).
2. Write `src/c/<Name>.c` based on the Ghidra output.
3. Iterate until this prints `MATCH`:
    ```sh
    uv run rotkit match <FunctionName> src/c/<Name>.c
    ```
    On `DIFF` it shows a side-by-side window around the first divergence, `--full` shows the whole function.
4. Add the region to `config/split.cfg` (`addr<TAB>src/c/<Name>.c`) and run `make verify`.

`.agents/skills/byte-matching/SKILL.md` is useful for LLMs to work on byte matches.
It is a huge pile of slop in LLM-ese, but it does help them get to an acceptable result faster.
If you use LLMs to byte-match, read the result and point out any weirdness or hacky code to be improved to the LLM.

If `verify` regresses, `make diff` reports the byte match of every split region.
For a GUI diff, run `make objdiff` and then `objdiff -p .` in the repo root (see [objdiff](https://github.com/encounter/objdiff)).
Do note that `objdiff` gets confused in translation units with multiple functions if your function lengths don't match.
I have yet to find out if this is an `objdiff` issue or an issue in my slop tooling.

## Carved Data and Assets

`make carve` re-carves `carved/` and `carved-local/` from the ROM.
Review the `carved/` diff before committing.

Art assets are stored under `carved-local/gfx/`.
For graphics there are reference sheets, e.g. at `carved-local/gfx/reference-sheets/`.

`rotkit` contains encoders that reproduce the game's original compressors, so unedited images rebuild the ROM exactly.
A few exceptions do not work if re-encoded that way (e.g. an unused cutscene asset with weird artifacts under it).
These exceptions keep a `.bin` copy of the original ROM bytes next to the PNG, which the build uses as long as the PNG is unchanged.

This part of the build system and decomp is currently a bit underdeveloped, so this might change in the future.

## Debugging in mGBA

`make debug` writes `build/debug/` with every name and type from `config/` and `include/` as gdb symbols and DWARF.

You can then use `gdb` and `mgba` to debug the original ROM or the ROM hack.

```sh
mgba-qt -g rotk.gba               # GDB stub on :2345, halted at entry
gdb -x build/debug/rotk.gdb       # any ARM-capable gdb
```

From there you can use normal `gdb` commands, e.g.:

```
p g_PlayerGlobals.players[0]
ptype SaveSlot
break system_waitVBlank
```

Before calling a game function or poking state, run `settle` to run the game until the next main loop tick.
Be especially careful when changing things while you're stopped in an interrupt handler.

For the hack build, use `build/rotk_hack.gba` and `build/debug/rotk_hack.gdb` instead.

Shared `gdb` scripts are stored in `gdb-scripts/`, personal/local ones can be stored somewhere under `local/`, e.g. `local/gdb-scripts/`.

`gdb-scripts/scenes.gdb` is a good example for what debugging allows.
It allows changing scenes and warping to other missions.
It also logs scene and mission changes.

```sh
gdb -x build/debug/rotk.gdb -x gdb-scripts/scenes.gdb
(gdb) warp 0             # load Anduin Banks at spawn point 0
```

You can also task LLM agents to write `gdb` scripts.
A relevant LLM agent skill is included at `.agents/skills/gdb-scripts/SKILL.md`.
