"""Reverse / unreverse emulator EEPROM .sav files for the ROTK save chip.

An emulator EEPROM .sav stores the raw serial stream: every 8-byte block is byte-reversed
vs the game struct order (a 512B base-game .sav or an 8 KiB SavSlotPages .sav from an
EEPROM-emulator flash cart). Reversal is an involution: one pass converts either way.

Only vanilla-magic ("ROTKGBA6") images ever exist in EEPROM order - the base game and
SavSlotPages. Anything else (the ROM's own SRAM-backed ROTKRCM format included) is out of
scope and errors early: reversing such an image only produces garbage.
"""

from pathlib import Path
from typing import Annotated

import typer

EEPROM_BLOCK_SIZE = 8
MAGIC_VANILLA = b"ROTKGBA6"


def convert(data: bytes) -> bytes:
    """Reverse each 8-byte EEPROM block (emulator serial order <-> game struct order)."""
    if len(data) % EEPROM_BLOCK_SIZE != 0:
        raise typer.BadParameter(
            f"file size {len(data)} is not a multiple of {EEPROM_BLOCK_SIZE} bytes"
        )
    out = bytearray(data)
    for off in range(0, len(out), EEPROM_BLOCK_SIZE):
        out[off : off + EEPROM_BLOCK_SIZE] = out[off : off + EEPROM_BLOCK_SIZE][::-1]
    return bytes(out)


def run(
    input: Annotated[Path, typer.Argument(help=".sav file to convert")],
    output: Annotated[Path, typer.Argument(help="converted output file")],
) -> None:
    """Reverse each 8-byte EEPROM block to convert between emulator and game byte order."""
    data = input.read_bytes()
    magic = data[:EEPROM_BLOCK_SIZE]
    if magic == MAGIC_VANILLA:
        direction = "game -> emulator byte order"
    elif magic[::-1] == MAGIC_VANILLA:
        direction = "emulator -> game byte order"
    else:
        raise typer.BadParameter(
            f"{input}: header magic {magic!r} is not {MAGIC_VANILLA.decode()!r} in either "
            "byte order; only EEPROM-era (base game / SavSlotPages) saves are in scope"
        )
    output.write_bytes(convert(data))
    print(f"{input}: {len(data)} bytes, {direction}")
    print(f"  wrote {output}")
