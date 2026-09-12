"""The GBA BIOS decompression calls (SWI 0x11, 0x13, 0x14) as chunk codecs use them.

Each decoder takes the stream after the chunk header and the size that header promised.
"""

from struct import unpack_from


def lz77_decode(data: bytes, start: int, size: int) -> bytearray:
    """SWI 0x11 LZ77UnCompWRAM: flag byte, MSB first, 1 = backref (len-3:4 dist-1:12)."""
    out = bytearray()
    pos = start
    while len(out) < size:
        flags = data[pos]
        pos += 1
        for bit in range(8):
            if len(out) >= size:
                break
            if flags & (0x80 >> bit):
                b1, b2 = data[pos], data[pos + 1]
                pos += 2
                distance = (((b1 & 0xF) << 8) | b2) + 1
                for _ in range((b1 >> 4) + 3):
                    out.append(out[-distance])
            else:
                out.append(data[pos])
                pos += 1
    return out


def rl_decode(data: bytes, start: int, size: int) -> bytearray:
    """SWI 0x14 RLUnCompWRAM: 1 count-3:7 byte for a run, 0 count-1:7 then the bytes."""
    out = bytearray()
    pos = start
    while len(out) < size:
        b = data[pos]
        pos += 1
        if b & 0x80:
            out += bytes([data[pos]]) * ((b & 0x7F) + 3)
            pos += 1
        else:
            count = b + 1
            out += data[pos : pos + count]
            pos += count
    return out


def huffman_decode(data: bytes, start: int, size: int) -> bytearray:
    """SWI 0x13 HuffUnComp with 8-bit symbols and an MSB-first u32 bit stream.

    Tree: u8 size byte (tree = (size+1)*2 bytes), then nodes of 2 bytes each: byte k of a
    node is the child for bit k, bits 0-5 its offset in 2-byte units from the current
    node's end, bit 6 set when the child is a leaf holding that value instead.
    """
    out = bytearray()
    tree = start + 1
    pos = tree + (data[start] + 1) * 2
    bitbuf = 0
    bitcnt = 0
    while len(out) < size:
        node = 0  # byte offset of the current node within the tree
        while True:
            if bitcnt == 0:
                bitbuf = unpack_from("<I", data, pos)[0]
                pos += 4
                bitcnt = 32
            bit = bitbuf >> 31
            bitbuf = (bitbuf << 1) & 0xFFFFFFFF
            bitcnt -= 1
            child = data[tree + node + bit]
            if child & 0x40:
                out.append(child & 0x3F)
                break
            node += (child & 0x3F) * 2 + 2
    return out
