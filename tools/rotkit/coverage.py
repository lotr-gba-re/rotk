"""Print config/split.cfg in address order with the base-ROM regions between translation units
made explicit, so the un-decompiled remainder is visible at a glance.

Region lengths are the compiled build/split/<...>.bin sizes, so a `<base rom>` line is a
stretch still taken from the base ROM. Run via `make coverage` (builds the .bins first).
"""

import os
import sys

from rotkit.paths import ROOT
from rotkit.rom import ROM, ROMBASE, regions


def run() -> None:
    rows = []
    for r in regions():
        if not os.path.isfile(r.bin_path):
            sys.exit(
                f"coverage: {os.path.relpath(r.bin_path, ROOT)} missing - "
                f"run `make coverage` (builds it) or `make verify`"
            )
        rows.append((r.addr, os.path.getsize(r.bin_path), r.src))
    romsize = os.path.getsize(ROM) if os.path.isfile(ROM) else 0x1000000
    romend = ROMBASE + romsize
    carved = sum(size for _addr, size, _src in rows)
    print(
        f"split.cfg coverage - {len(rows)} TUs, {carved} B carved "
        f"({100.0 * carved / romsize:.3f}% of ROM), address order:\n"
    )

    def row(start: int, end: int, size: int, label: str) -> None:
        print(f"  0x{start:08x}..0x{end:08x}  {'0x%x' % size:>8}  {label}")

    prev_end = ROMBASE  # start at ROM base so the head (start -> first TU) is shown
    between = 0
    leading = True
    for addr, size, src in rows:
        if addr > prev_end:
            gap = addr - prev_end
            if gap < 4:
                label = "<base rom> (alignment pad)"
            else:
                label = "<base rom>"
                if not leading:
                    between += 1
            row(prev_end, addr, gap, label)
        elif addr < prev_end:
            row(addr, prev_end, prev_end - addr, "!! OVERLAP with previous region")
        row(addr, addr + size, size, os.path.relpath(src, ROOT))
        prev_end = addr + size
        leading = False
    if prev_end < romend:
        row(prev_end, romend, romend - prev_end, "<base rom> (to end of ROM)")

    print(f"\n{between} base-rom region(s) between TUs.")
