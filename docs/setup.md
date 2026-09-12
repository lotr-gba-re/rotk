# Environment Setup

## Required

- `git`
- A normal (host system) C toolchain (`cc`, `make`) to build `agbcc`
- `arm-none-eabi-gcc` and `arm-none-eabi-binutils`
- `agbcc`: buildable using `tools/setup_agbcc.sh`
- `uv`: Python package management for `rotkit`
- `libclang`: Used by `rotkit`. Typically you get it when you install `clang`.
- `clang-format`: automatic formatting using `make format`
- `armips` (for assembling the ROM hack): [https://github.com/Kingcom/armips](https://github.com/Kingcom/armips)
- `flips` (for building ROM hack patch files): [https://git.disroot.org/Sir_Walrus/Flips](https://git.disroot.org/Sir_Walrus/Flips)
- The original game ROM as `rotk.gba`

## Optional

- Ghidra (if you want to use the `make ghidra` functionality)
- [objdiff](https://github.com/encounter/objdiff): interactive matched-function diff, paired with
  `make objdiff` (see [workflow.md](workflow.md)).
- `clangd`: editor indexing. `make clangd` regenerates `compile_commands.json` and
  `hacks/compile_flags.txt` (whose `-DHACK_*` lines track `hacks/patches.cfg`).

## Checks

- `make check-tools` checks for missing tools, reports the Ghidra version, and verifies the ROM checksum.
- `make verify` rebuilds the whole ROM from source and is the real test that the setup works.

## Git Hooks

`make install-hooks` installs git hooks to format/check before committing.
