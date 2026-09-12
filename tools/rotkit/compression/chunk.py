"""Chunk decompression (decompress_chunk @0x08019d54).

A chunk is one member of a larger record, e.g. the always-loaded and variant chunks of a
mission block. Every chunk starts with a u32 header:
  byte 0, high nibble: codec, bit 3 of it the diff16 post-filter flag
  bytes 1-3: decompressed size (u24)

The codec, (byte0 >> 4) & 7:
  0: raw u32 copy (bios_cpuSet)
  1: BIOS LZ77 (SWI 0x11)
  2: BIOS Huffman (SWI 0x13)
  3: BIOS RL (SWI 0x14)
  4: the game's lz77 (decompress_lz77, IWRAM-copied)
  6: pucrunch (decompress_pucrunch, IWRAM-copied)

With the diff16 flag set, FUN_08019fbc then integrates the output as s16 running sums.

BG asset tiles and sprite frames do not come through here: they carry no chunk header and
take their codec from the asset flags and the frame flag instead.
"""

from struct import unpack_from

from rotkit.compression import bios, lz77, pucrunch

CODEC_RAW = 0
CODEC_BIOS_LZ77 = 1
CODEC_BIOS_HUFFMAN = 2
CODEC_BIOS_RL = 3
CODEC_LZ77 = 4
CODEC_PUCRUNCH = 6
CODEC_DIFF16 = 8  # flag on top of the codec


def _diff16(out: bytearray) -> None:
    """FUN_08019fbc: out[i] += out[i-1] over s16 units, wrapping."""
    for i in range(2, len(out) - 1, 2):
        v = (unpack_from("<h", out, i)[0] + unpack_from("<h", out, i - 2)[0]) & 0xFFFF
        out[i : i + 2] = v.to_bytes(2, "little")


def decompress_chunk(data: bytes, offset: int) -> bytes:
    """Decompress the chunk at ROM offset `offset`."""
    header = unpack_from("<I", data, offset)[0]
    size = header >> 8
    codec = (header & 0xFF) >> 4
    diff = codec & CODEC_DIFF16
    codec &= ~CODEC_DIFF16
    stream = offset + 4
    if codec == CODEC_RAW:
        out = bytearray(data[stream : stream + size])
    elif codec == CODEC_BIOS_LZ77:
        out = bios.lz77_decode(data, stream, size)
    elif codec == CODEC_BIOS_HUFFMAN:
        out = bios.huffman_decode(data, stream, size)
    elif codec == CODEC_BIOS_RL:
        out = bios.rl_decode(data, stream, size)
    elif codec == CODEC_LZ77:
        out = bytearray(lz77.decode(data, stream)[0])
    elif codec == CODEC_PUCRUNCH:
        out = bytearray(pucrunch.decode(data, stream))
    else:
        raise ValueError(f"unknown chunk codec {codec} at {offset:#x}")
    if diff:
        _diff16(out)
    if codec != CODEC_PUCRUNCH and len(out) != size:
        raise ValueError(
            f"codec {codec} at {offset:#x}: got {len(out)} bytes, want {size}"
        )
    return bytes(out)
