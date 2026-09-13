# ROTK GBA - RE / hack / build workflow.
# ROM is never committed; BPS patches are the only redistributable artifact.
# Big subsystems live in their own includes: make/decomp.mk (byte-match) + make/hack.mk (patches).

ROM        := rotk.gba
SHA1FILE   := baserom.sha1
GHIDRA_DIR := $(or $(GHIDRA_INSTALL_DIR),/opt/ghidra)
# Pinned Ghidra release tag (e.g. 12.1_PUBLIC_20260530); installs match on major.minor only,
# so a patch build (12.1.2 vs 12.1) passes. Full tag lives in ghidra_build/VERSION.
GHIDRA_VERSION := $(shell cat tools/rotkit/ghidra_build/VERSION 2>/dev/null)
GHIDRA_VERSION_MAJOR_MINOR := $(word 1,$(subst _, ,$(GHIDRA_VERSION)))

# Compiles are independent per-TU (isolated tempdirs), so parallelize by default; --output-sync
# keeps each recipe's output grouped so parallel failure logs stay readable. Override with -j1.
MAKEFLAGS  += -j$(shell nproc) --output-sync=target

.DEFAULT_GOAL := help
.PHONY: help check check-tools check-base-rom ghidra check-stores symbols carve debug dump \
        clangd format format-staged install-hooks clean clean-ghidra

include make/common.mk make/decomp.mk make/hack.mk

help:
	@echo "ROTK GBA - run 'make <target>':"
	@echo ""
	@echo "  EVERYDAY"
	@echo "    check              check-stores + verify - run after every change"
	@echo ""
	@echo "  SETUP"
	@echo "    check-tools        verify toolchain + ROM checksum + Ghidra version"
	@echo "    check-base-rom     check rotk.gba against the expected SHA-1"
	@echo ""
	@echo "  RE / STORES  (the text stores are the source of truth; edit them by hand)"
	@echo "    ghidra             rebuild the Ghidra browse project from the stores"
	@echo "    check-stores       lint the stores (consistency, header vs .c, @romaddress)"
	@echo "    symbols            regenerate generated files (game_symbols, memmap, variables.h)"
	@echo "    carve              re-carve everything: carved/ from the ROM + carved-local/"
	@echo "    debug              gdb symbol/type bundle for mGBA's stub -> build/debug/"
	@echo "    clangd             (re)generate compile_commands.json for editor whole-tree indexing"
	@echo ""
	@echo "  DECOMP  (does the committed source still rebuild the ROM?)"
	@echo "    verify             rebuild the whole ROM from source; SHA must match the base ROM"
	@echo "    check-bugfixes     compile every src/c fix arm with -DBUGFIX (no byte compare)"
	@echo "    coverage           carved code vs the base-ROM remainder"
	@echo "    diff [FUNC=x]      per-region byte-match vs base ROM; FUNC=<substr> adds a hex window"
	@echo "    objdiff            build objdiff ELF pairs for the interactive GUI diff"
	@echo ""
	@echo "  HACK"
	@echo "    hack [HACKS=...]   build build/rotk_hack.gba from selected patches (default hacks/patches.cfg)"
	@echo "    dist [HACKS=...]   hack + emit a verified BPS -> dist/hack.bps"
	@echo "    check-hacks        ROM-free build of every patch (the pre-commit gate)"
	@echo ""
	@echo "  ANALYSIS  (ROM-derived, gitignored; not part of the build)"
	@echo "    dump               (re)decode game text + affixes -> tools/out/*.tsv (needs the ROM)"
	@echo ""
	@echo "  FORMAT"
	@echo "    format             clang-format the C + uv format the Python"
	@echo "    format-staged      clang-format only staged content (safe for git add -p)"
	@echo "    install-hooks      enable the pre-commit gate (format + make check + hack compile; once per clone)"
	@echo ""
	@echo "  HOUSEKEEPING"
	@echo "    clean              remove build/ + carved-local/ + objdiff.json + dist/"
	@echo "    clean-ghidra       remove the regenerable ghidra_proj/ (rebuild with make ghidra)"
	@echo ""
	@echo "  Re-carve one table family only: uv run rotkit carve <table>"

# ---------------------------------------------------------------- setup / env --
# java + ghidra are reported but never fail the gate: only `make ghidra` needs them.
check-tools:
	@ok=1; \
	for t in armips flips arm-none-eabi-gcc arm-none-eabi-ld arm-none-eabi-objcopy clang-format uv git; do \
	  if command -v $$t >/dev/null 2>&1; then printf "  ok   %-20s %s\n" "$$t" "$$(command -v $$t)"; \
	  else printf "  MISS %-20s (install)\n" "$$t"; ok=0; fi; done; \
	if command -v java >/dev/null 2>&1; then printf "  ok   %-20s %s\n" "java" "$$(command -v java)"; \
	else printf "  opt  %-20s (only for make ghidra)\n" "java"; fi; \
	props="$(GHIDRA_DIR)/Ghidra/application.properties"; \
	if [ ! -d "$(GHIDRA_DIR)" ]; then echo "  opt  ghidra               $(GHIDRA_DIR) not found (set GHIDRA_INSTALL_DIR)"; \
	elif [ ! -f "$$props" ]; then echo "  opt  ghidra               $$props missing (not a Ghidra install?)"; \
	else inst=$$(sed -n 's/^application.version=//p' "$$props"); \
	  case "$$inst" in $(GHIDRA_VERSION_MAJOR_MINOR)|$(GHIDRA_VERSION_MAJOR_MINOR).*) \
	    echo "  ok   ghidra               $(GHIDRA_DIR) (installed $$inst, pinned $(GHIDRA_VERSION))" ;; \
	    *) echo "  opt  ghidra               installed $$inst, pinned $(GHIDRA_VERSION)" ;; esac; fi; \
	$(MAKE) -s check-base-rom; \
	[ $$ok = 1 ] || { echo "check-tools: missing tools"; exit 1; }

check-base-rom:
	@if [ ! -f $(ROM) ]; then echo "  $(ROM) missing (gitignored - provide your dump)"; exit 1; fi
	@sha1sum -c $(SHA1FILE) && echo "  ROM ok"

# ------------------------------------------------------------- RE / stores -----
# Build the browse project from the stores: import the ROM into a fresh project (gen_project), then
# stamp on the stores - names, types, prototypes, plates (apply_symbols). Always a clean rebuild:
# apply_symbols sets the THUMB/ARM context register, which is safe only on a fresh import, so it is
# NOT re-runnable onto an already-populated project - there is no incremental fast path. Since
# gen_project recreates the project each time, a clean rebuild is cheap enough to always do.
ghidra: check-base-rom
	uv run rotkit ghidra gen-project
	uv run rotkit ghidra apply-symbols

check-stores:
	uv run rotkit check stores

# The everyday gate after any edit: the cheap lints first (fail fast), then the byte-match
# reconstruction. `verify` skips itself if the ROM is absent, so `check` still lints without a dump.
check: check-stores check-bugfixes verify

# One-stop regenerate of every stores-derived generated file (the incremental file rules for the
# hack build inputs live in make/hack.mk; this forces all three at once, incl. variables.h).
symbols:
	uv run rotkit build symbols
	uv run rotkit build layout
	uv run rotkit build variables

# Re-carve everything: the committed carved/ tables + headers from the ROM (review the git
# diff before committing) and the ROM-carved carved-local/ files. One family only:
# uv run rotkit carve <table>.
carve: check-base-rom
	uv run rotkit carve all

# gdb bundle (build/debug/debug.elf + rotk.gdb) for mGBA's GDB stub; regenerated from the
# stores, so re-run after editing config/*.cfg or include/*.h.
debug:
	uv run rotkit build debug

# compile_commands.json for clangd (whole-tree indexing / workspace symbols) + hacks/
# compile_flags.txt (the -DHACK_* selection from patches.cfg). Both gitignored, per-clone. Phony
# prereq of `verify` and `hack`, so every build keeps both in step with the on-disk .c set and the
# patch selection; `make clangd` also regenerates by hand.
clangd:
	uv run rotkit build clangd

# ROM-derived analysis dumps (gitignored tools/out/). Not part of the build, and no tool depends on
# them - a convenience for grepping.
dump: check-base-rom
	uv run rotkit analysis decode-text dump --all -o tools/out/strings.tsv
	uv run rotkit analysis dump-affixes

# ------------------------------------------------------------------ format -----
# Format C + headers with clang-format (brace/whitespace/comment only - never changes compiled
# bytes) and Python with `uv format` (uv's built-in ruff-backed formatter).
FORMAT_SRC := $(shell find src include carved hacks \( -name '*.c' -o -name '*.h' \) 2>/dev/null)
format:
	@clang-format -i $(FORMAT_SRC) && echo "clang-format done ($(words $(FORMAT_SRC)) C candidates)"
	@uv format --preview-features format-command && echo "uv format done"
	@uv run rotkit build format-stores

# Format only the STAGED content (index), preserving unstaged edits - safe for `git add -p`.
format-staged:
	@sh tools/format-staged.sh

# Activate the tracked git hooks (pre-commit: clang-format check). Per-clone, run once.
install-hooks:
	@git config core.hooksPath tools/githooks
	@echo "git hooks active from tools/githooks/ (pre-commit: clang-format check + make check + hack compile)"

# ------------------------------------------------------------- housekeeping ----
# carved-local is wiped except its committed meta files (.gitignore, README.md).
clean:
	rm -rf build objdiff.json dist
	find carved-local -mindepth 1 -maxdepth 1 ! -name .gitignore ! -name README.md -exec rm -rf {} +
	@echo "cleaned build/ + carved-local/ + objdiff.json + dist/ (ghidra_proj kept; see clean-ghidra)"

clean-ghidra:
	rm -rf ghidra_proj
	@echo "removed ghidra_proj/ (rebuild with make ghidra)"
