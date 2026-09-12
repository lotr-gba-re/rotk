# decomp split: reconstruct the ROM from committed source + base-ROM remainder, byte-for-byte.
# Carving (ROM bytes -> readable committed C) is a manual, one-off step done with the per-section
# `rotkit carve` commands - NOT part of the build. The build only checks the invariant: compile
# the committed carved/data/*.c listed in config/split.cfg, splice them over the base ROM, and assert
# the reconstruction's SHA == base.
.PHONY: verify coverage diff objdiff

# -fno-toplevel-reorder: a merged carved file holds several ROM-adjacent tables; keep their
# .rodata emission in source (address) order, not gcc's default reverse varpool order.
SPLIT_CFLAGS := -c -std=gnu11 -Wall -Wextra -mthumb -mthumb-interwork -O2 -mcpu=arm7tdmi \
                -ffreestanding -fno-toplevel-reorder \
                -Iinclude -Icarved/include -Ibuild/include -Ibuild
SPLIT_SRC := $(shell awk 'NF && $$1 !~ /^#/ {print $$2}' config/split.cfg 2>/dev/null)
SPLIT_HDR := $(shell find src include carved -name '*.h' 2>/dev/null)
# Mirror <root>/<p>.c -> build/split/<p>.bin, root being src/ or carved/ (% spans subdirs).
SPLIT_BIN := $(foreach s,$(SPLIT_SRC),build/split/$(basename $(patsubst carved/%,%,$(s:src/%=%))).bin)

# Carved graphics: the committed graphics tables (carved/data/<group>/) #include
# build/gfx/<category>.inc, byte lists `rotkit build gfx` encodes from the PNGs in
# carved-local/gfx/ (extracting a group from the base ROM first when it has no images). It
# owns the staleness check and runs while make parses, before any prerequisite is weighed:
# a rule would stat the .inc first and miss the rewrite until the next run. The TU's depfile
# then ties it to the .inc it includes.
ifeq ($(filter clean% help,$(MAKECMDGOALS)),)
$(shell uv run rotkit build gfx >&2)
ifneq ($(.SHELLSTATUS),0)
$(error rotkit build gfx failed)
endif
endif

# Two source kinds, distinguished by path; each -> build/split/<same subpath>.bin:
#   carved/data/*.c  reverse-engineered data tables  -> modern gcc, .rodata
#   src/c/*.c        matched decompiled C (agbcc)    -> rotkit build cc (exact ROM bytes)
#
# build/game_symbols.ld (produced by make/common.mk's grouped rule) PROVIDEs every store symbol at
# its ROM address; data tables that reference other carved symbols by name need a link step to
# resolve those references.
build/split/data/%.bin: carved/data/%.c $(SPLIT_HDR) build/game_symbols.ld
	@mkdir -p $(dir $@)
	@echo "  CC   $<"
	@arm-none-eabi-gcc $(SPLIT_CFLAGS) -MMD -MP -MT $@ -MF $(@:.bin=.d) $< -o $(@:.bin=.o)
	@arm-none-eabi-ld -T build/game_symbols.ld $(@:.bin=.o) -o $(@:.bin=.elf)
	@arm-none-eabi-objcopy -O binary -j .rodata $(@:.bin=.elf) $@
-include $(patsubst %.bin,%.d,$(filter build/split/data/%,$(SPLIT_BIN)))

# Matched decompiled C -> exact ROM bytes via the period compiler (agbcc); see tools/rotkit/compile.py (prints its own line).
build/split/c/%.bin: src/c/%.c build/include/variables.h $(SPLIT_HDR)
	@mkdir -p $(dir $@)
	@uv run rotkit build cc $< $@

# Reconstruct + check the ROM. First a per-region byte-match report (report.py: localizes any
# mismatch to one TU + first diverging offset); then the whole ROM (decompiled .bins + base-ROM
# gaps) assembled with standard tools, asserting sha1 == base as the final end-to-end invariant.
verify: $(SPLIT_BIN) clangd
	@if [ ! -f $(ROM) ]; then echo "verify: $(ROM) missing (gitignored) - skipping ROM build."; exit 0; fi
	@uv run rotkit check rom
	@uv run rotkit build rom >/dev/null
	@arm-none-eabi-as -mcpu=arm7tdmi build/rom.s -o build/rom.o
	@arm-none-eabi-objcopy -O binary build/rom.o build/rom.gba
	@got=$$(sha1sum build/rom.gba | cut -d' ' -f1); base=$$(cut -d' ' -f1 $(SHA1FILE)); \
	 if [ "$$got" = "$$base" ]; then echo "verify: whole-ROM SHA-1 OK ($$got) == base"; \
	 else echo "verify: whole-ROM MISMATCH $$got != $$base (see per-region report above)"; exit 1; fi

# Per-region (per-function) byte-match report vs the base ROM. `make diff` for the full table;
# `make diff FUNC=<substr>` adds a hex window around the first diff for that region.
diff: $(SPLIT_BIN)
	@uv run rotkit check rom $(FUNC)

# Address-ordered view of split.cfg with the base-ROM regions between carved TUs made explicit
# (region lengths from the compiled build/split/*.bin). Surfaces the un-decompiled remainder.
coverage: $(SPLIT_BIN)
	@uv run rotkit coverage

# objdiff integration (interactive, register-normalized instruction diff with live matching %).
# For each src/c/**/*.c we synthesize an ELF pair (base = your build, target = original ROM
# bytes); objdiff's build command rebuilds these on save via this grouped rule and re-diffs.
build/objdiff/%/base.elf build/objdiff/%/target.elf &: src/c/%.c $(SPLIT_HDR)
	@uv run rotkit build objdiff $<

# Generate objdiff.json + build every pair, then point the objdiff GUI at this directory.
objdiff: $(patsubst src/c/%.c,build/objdiff/%/base.elf,$(shell find src/c -name '*.c'))
	@uv run rotkit build objdiff-config
	@echo "objdiff ready - launch the GUI here (File > current dir), or: objdiff-cli diff -p . -u <Name>"
