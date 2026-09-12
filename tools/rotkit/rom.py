"""The base ROM: cached bytes IO, little-endian readers, and the carve region list
(split.cfg rows joined with their compiled .bin paths). split.cfg itself lives in stores.py."""

import os
import struct
from dataclasses import dataclass

from rotkit.paths import BUILD, ROM
from rotkit.stores import read_split

ROMBASE = 0x08000000
# compiled TU -> split/<stem>.bin (stem = path minus src/ or carved/)
BIN_DIR = BUILD / "split"

_rom: bytes | None = None


def load_rom() -> bytes:
    """The base ROM bytes, read once per process; exits if the gitignored dump is absent."""
    global _rom
    if _rom is None:
        if not os.path.isfile(ROM):
            raise SystemExit(f"{ROM} missing (gitignored - provide your dump)")
        _rom = open(ROM, "rb").read()
    return _rom


# Little-endian readers; u32() takes an absolute ROM address, the Structs a ROM offset.
U16 = struct.Struct("<H")
U32 = struct.Struct("<I")


def u32(addr: int) -> int:
    return U32.unpack_from(load_rom(), addr - ROMBASE)[0]


@dataclass(frozen=True)
class Region:
    """One split.cfg region: ROM address, compiled .bin path, src path."""

    addr: int
    bin_path: str
    src: str


def regions() -> list[Region]:
    """split.cfg regions, sorted by address."""
    rows = []
    for src, addr in read_split().items():
        rel = src.removeprefix("src/").removeprefix("carved/")
        stem = os.path.splitext(rel)[0]
        rows.append(Region(addr, os.path.join(BIN_DIR, stem + ".bin"), src))
    return sorted(rows, key=lambda r: r.addr)
