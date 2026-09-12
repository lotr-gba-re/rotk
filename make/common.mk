# Shared generated build inputs consumed by both the decomp reconstruction (make/decomp.mk)
# and the hack build (make/hack.mk). Defining both here breaks what would otherwise be an
# inverted dependency - decomp (the project's core invariant) reaching into hack for a rule.
#
# Regenerate all of these at once with `make symbols`.

# variables.h: extern decls for game globals, auto-generated from the data stores into
# build/include/, so the build recreates it before compiling any src/c (or hack C
# body) that #includes it.
DATA_STORES := config/data.cfg $(wildcard carved/config/*.cfg)
build/include/variables.h: $(DATA_STORES) tools/rotkit/build/variables.py
	uv run rotkit build variables

# Generated symbol files (both produced by one export run - grouped target). Consumed by:
#   decomp: data tables that reference other carved symbols by name link against the .ld so the
#           references resolve to store addresses.
#   hack:   hack.asm includes the .inc, hack.ld INCLUDEs the .ld, so patch code references game
#           funcs/globals by name with no hardcoded addresses.
build/game_symbols.inc build/game_symbols.ld &: config/functions.cfg $(DATA_STORES) \
                                                 $(wildcard include/*.h carved/include/*.h) tools/rotkit/build/symbols.py
	uv run rotkit build symbols
