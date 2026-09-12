"""Compile matched C to exact ROM bytes:

    uv run rotkit build cc src/c/<File>.c <out.bin>

Thin wrapper over rotkit.compile.compile_file for the Makefile's `build/split/c/%.bin` rule.
"""

import os

from rotkit.compile import compile_file
from rotkit.paths import ROOT


def run(cfile: str, out: str) -> None:
    base, data = compile_file(cfile)
    os.makedirs(os.path.dirname(out), exist_ok=True)
    open(out, "wb").write(data)
    print(f"  CC   {os.path.relpath(cfile, ROOT):<40} @0x{base:08x}  {len(data)} B")
