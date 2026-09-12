"""Per-region (per-function) byte-match report against the base ROM.

For every committed config/split.cfg region, compare its compiled build/split/<...>.bin
to the same bytes in the base ROM. This is the granular view `make verify` lacks: a
whole-ROM SHA only says pass/fail, this pins a mismatch to one translation unit and the first
diverging offset. A name substring argument adds a hex window around the first diff for the
matching regions.

Bytes come from config/split.cfg + the compiled build/split/*.bin (build them with
`make verify` / `make diff`). Exit 1 if any region diffs or its .bin is missing.
For a deep, disassembled single-function diff while carving, use match.py instead.
"""

import os
from dataclasses import dataclass
from typing import Annotated

import typer

from rotkit.paths import ROOT
from rotkit.rom import ROMBASE, Region, load_rom, regions


@dataclass(frozen=True)
class RegionResult:
    """One region's byte-match outcome against the base ROM."""

    status: str  # OK / DIFF / MISS
    addr: int
    size: int
    rel: str  # repo-relative src path
    diff_pos: int  # byte offset of first diff, or -1
    bin_path: str

    @property
    def ok(self) -> bool:
        return self.status == "OK"


def first_diff(built: bytes, base: bytes) -> int:
    """Index of the first differing byte in the common prefix, or -1 if it matches."""
    for offset in range(min(len(built), len(base))):
        if built[offset] != base[offset]:
            return offset
    return -1


def _hex(row: bytes) -> str:
    return " ".join(f"{x:02x}" for x in row)


def check(rom: bytes, region: Region) -> RegionResult:
    """Compare one region's compiled .bin to the base ROM bytes."""
    rel = os.path.relpath(region.src, ROOT)
    if not os.path.isfile(region.bin_path):
        return RegionResult("MISS", region.addr, 0, rel, -1, region.bin_path)
    built = open(region.bin_path, "rb").read()
    base = rom[region.addr - ROMBASE : region.addr - ROMBASE + len(built)]
    diff_pos = first_diff(built, base)
    ok = diff_pos < 0 and len(base) == len(built)
    return RegionResult(
        "OK" if ok else "DIFF", region.addr, len(built), rel, diff_pos, region.bin_path
    )


def drilldown(rom: bytes, result: RegionResult) -> None:
    """Hex window (8 bytes either side) around the first diff for one region."""
    print(f"\n--- {result.rel} @ 0x{result.addr:08x} ({result.size} B) ---")
    built = open(result.bin_path, "rb").read()
    base = rom[result.addr - ROMBASE : result.addr - ROMBASE + len(built)]
    center = result.diff_pos if result.diff_pos >= 0 else min(len(built), len(base))
    lo, hi = max(0, center - 8), center + 8
    print(f"  first diff at +0x{center:x} (0x{result.addr + center:08x})")
    print(f"    base : {_hex(base[lo:hi])}")
    print(f"    built: {_hex(built[lo:hi])}")
    print(f"           {' ' * (3 * (center - lo))}^^")


def run(func: Annotated[str | None, typer.Argument()] = None) -> None:
    rom = load_rom()
    results = [check(rom, region) for region in regions()]

    total_bytes = sum(r.size for r in results)
    print(
        f"region byte-match vs base ROM - {len(results)} TUs, {total_bytes} B carved:\n"
    )
    for r in results:
        if r.status == "DIFF":
            tail = (
                f"  first diff +0x{r.diff_pos:x} (0x{r.addr + r.diff_pos:08x})"
                if r.diff_pos >= 0
                else "  size mismatch"
            )
        elif r.status == "MISS":
            tail = "  .bin missing - build it (make verify)"
        else:
            tail = ""
        print(f"  {r.status:<4}  0x{r.addr:08x}  {'0x%x' % r.size:>7}  {r.rel}{tail}")

    bad = [r for r in results if not r.ok]
    print(
        f"\n{len(results)} regions: {len(results) - len(bad)} OK, {len(bad)} DIFF/MISS."
    )

    if func:
        shown = [r for r in results if func in r.rel]
        if not shown:
            print(f"\n(no region path contains {func!r})")
        for r in shown:
            if r.status == "OK":
                print(
                    f"\n--- {r.rel} @ 0x{r.addr:08x} ({r.size} B) ---\n  byte-identical to base ROM"
                )
            elif r.status == "MISS":
                print(f"\n--- {r.rel} @ 0x{r.addr:08x} ---\n  .bin missing - build it")
            else:
                drilldown(rom, r)

    if bad:
        raise SystemExit(1)
