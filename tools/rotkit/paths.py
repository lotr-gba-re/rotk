"""Canonical repo paths: ROOT and the directory/file constants shared by more than one
module. A single-use path stays local to its module, built with `/` from these.
"""

from pathlib import Path

# Repo root: this file is tools/rotkit/paths.py -> up three parents.
ROOT = Path(__file__).resolve().parent.parent.parent

# --- Top-level directories -----------------------------------------------------
BUILD = ROOT / "build"
INCLUDE = ROOT / "include"
CONFIG = ROOT / "config"
SRC = ROOT / "src"
TOOLS = ROOT / "tools"
CARVED = ROOT / "carved"  # carved from the ROM, committed
CARVED_LOCAL = ROOT / "carved-local"  # carved from the ROM, not committed (gfx/)

# --- Subdirectories ------------------------------------------------------------
SRC_C = SRC / "c"  # matched decompiled C translation units
CARVED_INCLUDE = CARVED / "include"  # committed carved headers (text_ids.h)
CARVED_DATA = CARVED / "data"  # committed carved data tables
CARVED_CONFIG = CARVED / "config"  # carver-generated data stores (data.cfg rows)
GFX_LOCAL = CARVED_LOCAL / "gfx"  # editable carved images, one directory per group
GFX_SHEETS = GFX_LOCAL / "reference-sheets"  # viewing aids, mirroring the group tree
BUILD_INCLUDE = BUILD / "include"  # build-generated headers (variables.h)
TOOLS_OUT = TOOLS / "out"  # ROM-derived analysis dumps (gitignored)
GHIDRA_PROJ = ROOT / "ghidra_proj"  # regenerable browse project (gitignored)

# --- Common files --------------------------------------------------------------
ROM = ROOT / "rotk.gba"  # the base ROM (gitignored; never committed)
FUNCS_CFG = CONFIG / "functions.cfg"
DATA_CFG = CONFIG / "data.cfg"
SPLIT_CFG = CONFIG / "split.cfg"
