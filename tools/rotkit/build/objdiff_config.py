"""Emit objdiff.json listing one unit per matched src/c/**/*.c.

objdiff (GUI) loads this from the repo root: each unit points at the generated ELF pair under
build/objdiff/<stem>/ (stem = path relative to src/c, so subdirectories nest), and on save
objdiff re-runs its build command (`make <path>`, the grouped rule in make/decomp.mk) and
re-diffs live.
Regenerate with `make objdiff` whenever src/c gains a file.
"""

import glob
import json
import os

from rotkit.paths import ROOT, SRC_C


def run() -> None:
    units = []
    for path in sorted(glob.glob(str(SRC_C / "**/*.c"), recursive=True)):
        stem = os.path.splitext(os.path.relpath(path, SRC_C))[0]
        unit_dir = f"build/objdiff/{stem}"
        units.append(
            {
                "name": stem,
                "target_path": f"{unit_dir}/target.elf",
                "base_path": f"{unit_dir}/base.elf",
                "metadata": {"source_path": f"src/c/{stem}.c"},
            }
        )

    config = {
        "min_version": "2.0.0",
        "custom_make": "make",
        "build_target": True,  # target ELF = ROM bytes, base ELF = your build
        "build_base": True,
        "watch_patterns": [
            "src/c/**/*.c",
            "include/*.h",
            "include/**/*.h",
            "config/*.cfg",
        ],
        "units": units,
    }
    out = ROOT / "objdiff.json"
    with open(out, "w") as fh:
        json.dump(config, fh, indent=2)
        fh.write("\n")
    print(
        f"  wrote objdiff.json - {len(units)} unit(s): {', '.join(unit['name'] for unit in units)}"
    )
