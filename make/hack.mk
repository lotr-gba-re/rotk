# hack / build: compose the selected patches (hacks/patches.cfg) into one patched ROM / BPS.
# Make owns the build graph; build_patch.py only does the SHA + BPS round-trip.
.PHONY: hack dist check-hacks

# Memory map (single source hacks/layout.cfg) -> per-consumer fragments (.inc/.h/.ld). No hardcoded
# addresses in the hack files: hack.asm includes .inc, hack.ld INCLUDEs .ld, rom.c includes .h. The C
# header goes in build/hack_include/ (C-only) so -Ibuild/hack_include exposes just headers.
build/memmap.inc build/hack_include/memmap.h build/memmap.ld &: hacks/layout.cfg tools/rotkit/build/layout.py
	uv run rotkit build layout

# --- patch selection: no codegen - the HACK_<name> flags go straight to armips/gcc as -equ/-D, and
# hack.asm lists the patches explicitly (an `.if HACK_<n>` per patch).
# HACKS overrides hacks/patches.cfg (default); every selection must include mod_save + plumbing.
HACKS   ?= @cfg
PATCHES := $(notdir $(wildcard hacks/patches/*))
# @cfg is a composable token that expands to patches.cfg, so HACKS can ADD to (not just replace) the
# default: HACKS="@cfg foo" = cfg + foo, HACKS="foo" = just foo. sort dedups
# (a patch named in both cfg and HACKS, or listed twice) and keeps SEL order deterministic.
CFG     := $(shell sed 's/\#.*//' hacks/patches.cfg)
SEL     := $(sort $(subst @cfg,$(CFG),$(HACKS)))
HAVE_TAB:= $(if $(strip $(foreach p,$(SEL),$(wildcard hacks/patches/$(p)/tables.c))),1,0)
EQU     := $(foreach p,$(PATCHES),-equ HACK_$(p) $(if $(filter $(p),$(SEL)),1,0)) -equ HACK_HAVE_TABLES $(HAVE_TAB)
DEF     := $(foreach p,$(PATCHES),-DHACK_$(p)=$(if $(filter $(p),$(SEL)),1,0)) -DHACK_HAVE_TABLES=$(HAVE_TAB)

# Patched ROM the build writes (armips .open target via -strequ HACK_OUTFILE).
# check-hacks overrides this to a separate file so the hook never clobbers the ROM under test.
HACK_OUT ?= build/rotk_hack.gba

# --- modcode: each enabled patch's .c files compile as their own translation units (macros and
# statics stay patch-local; every non-static symbol is exported to armips via nm, so asm can
# call it by name), then all objects link into the one blob hack.asm .incbins.
# tables.c is excluded: hacks/tables.c #includes it into the tablepatch build instead. hack.c is
# the always-present baseline TU (it roots the blob and guards the required patches).
# hacks/lib/*.c always compile, so they may depend on NO patch - only on the stock ROM
# (HackLib_* prefix, no HACK_<name> in sight). Code that needs a patch to exist belongs to that
# patch instead, exported through hacks/patches/<p>/<p>.h; it then compiles only when its owner
# is selected. See hacks/README.md.
# Every compiled function lands in the blob whether or not anything calls it, so an uncalled
# hacks/lib helper still costs ROM.
C_SRCS  := $(foreach p,$(SEL),$(filter-out %/tables.c,$(wildcard hacks/patches/$(p)/*.c))) \
           $(wildcard hacks/lib/*.c)
C_OBJS  := build/hack_obj/hack.o \
           $(addprefix build/hack_obj/,$(addsuffix .o,$(basename $(notdir $(C_SRCS)))))

# Objects are keyed by basename; two selected .c files sharing a name would silently overwrite.
ifneq ($(words $(sort $(notdir $(C_SRCS)))),$(words $(notdir $(C_SRCS))))
$(error duplicate .c basename across selected patches/lib)
endif
# -fno-short-enums: the game (agbcc) uses 4-byte enums; arm-none-eabi-gcc defaults to 1-byte
# ones, which shifts every field after an enum in shared structs. Also on the tables.c line.
# agbcc also rounds struct sizes up to a multiple of 4; -mstructure-size-boundary=32 would
# mirror that but rounds the hack's own byte-sized RAM state structs too, so it stays off.
MODCODE_CFLAGS := -c -g3 -std=gnu11 -Wall -Wextra -mthumb -mthumb-interwork -mlong-calls -O2 -mcpu=arm7tdmi \
           -ffreestanding -fno-strict-aliasing -fno-short-enums \
           -Iinclude -Icarved/include -Ibuild/include \
           -Ibuild/hack_include -Ihacks $(DEF)

# Selection is a runtime choice, so the modcode is rebuilt each `make hack` (it's small): guard
# hook-site collisions, compile the enabled C bodies into one blob, then armips.
hack: $(if $(SKIP_ROM_CHECK),,check-base-rom) build/game_symbols.inc build/game_symbols.ld \
                     build/memmap.inc build/hack_include/memmap.h build/memmap.ld \
                     build/include/variables.h clangd
	@# hook-site guard: the site arg of every hookTrampoline/hookReplace/hookTrap/hookStub
	@# call (hack.asm + all patch asm) must be unique - armips would let a later write silently
	@# overwrite an earlier one at the same site. Repo-wide on purpose: a shared site must
	@# be chained in one .asm, enabled or not.
	@d=`grep -hE '^[[:space:]]*hook(Trampoline|Replace|Trap|Stub)(R3)?[[:space:]]+' hacks/hack.asm \
	   hacks/patches/*/*.asm | sed -E 's/^[[:space:]]*hook(Trampoline|Replace|Trap|Stub)(R3)?[[:space:]]+//;s/,.*//;s/[[:space:]]//g' | sort | uniq -d`; \
	 [ -z "$$d" ] || { echo "hook-site collision: $$d (chain them in one .asm)"; exit 1; }
	@# -g3: debug info for rotk_hack.gdb (add-symbol-file build/hack.elf); the objcopy'd
	@# modcode bytes are unaffected.
	@mkdir -p build/hack_obj
	@set -e; for f in hacks/hack.c $(C_SRCS); do \
	   arm-none-eabi-gcc $(MODCODE_CFLAGS) $$f -o build/hack_obj/`basename $${f%.c}`.o; done
	@# -nodefaultlibs: use the ROM libc as far as it exists
	@# -lgcc: Do emit libgcc as the ROM used a different compiler
	arm-none-eabi-gcc -nostartfiles -nodefaultlibs -mthumb -mthumb-interwork -T hacks/hack.ld \
	  -Wl,-L,build -o build/hack.elf $(C_OBJS) -lgcc
	arm-none-eabi-objcopy -O binary -j .text -j .rodata build/hack.elf build/modcode.bin
	uv run rotkit hack modcode-syms build/hack.elf build/modcode_syms.inc
	@if [ "$(HAVE_TAB)" = "1" ]; then \
	   arm-none-eabi-gcc -c -std=gnu11 -Wall -Wextra -mthumb -mthumb-interwork -O2 -mcpu=arm7tdmi \
	     -ffreestanding -fno-strict-aliasing -fno-short-enums -Iinclude -Icarved/include -Ibuild/include -Ihacks -Icarved/data $(DEF) hacks/tables.c -o build/tables.o && \
	   uv run rotkit hack tablepatch build/tables.o build/tablepatch.asm; \
	 else : > build/tablepatch.asm; fi
	armips $(EQU) -strequ HACK_OUTFILE $(HACK_OUT) hacks/hack.asm

dist: hack
	uv run rotkit hack bps hack

# ROM-free hack compile gate for the pre-commit hook. `make hack` needs the base ROM and only
# compiles the enabled patches; check-hacks builds every patch (all C bodies, tables.c, all asm)
# with the ROM sha gate skipped and the patched ROM written to build/rotk_hack_check.gba instead
# of the real output, so a commit touching any patch (or a header it includes) fails the gate if
# that patch doesn't compile, without disturbing the ROM under test.
check-hacks:
	$(MAKE) hack SKIP_ROM_CHECK=1 HACK_OUT=build/rotk_hack_check.gba HACKS="$(PATCHES)"
