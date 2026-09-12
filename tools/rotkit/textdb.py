"""The game's Huffman-compressed localized text: the byte-level decoder, pure over explicit
ROM bytes. The format writeup (tree layout, bit order) lives in
rotkit/analysis/decode_text.py, as does the font-codepage Unicode rendering;
decode_strings() here renders ASCII-only (non-ASCII font bytes -> \\xNN).
"""

from rotkit.rom import ROMBASE, U16, U32, load_rom

# Localized-text package pointers; indexed by language id.
TEXT_LANG_PTR_TABLE = 0x08289A10
LANG_EN = 0  # 1=fr, 2=de, 3=it, 4=es


def pkg_for(rom: bytes, lang: int) -> int:
    """ROM address of a language's text package."""
    return U32.unpack_from(rom, TEXT_LANG_PTR_TABLE - ROMBASE + lang * 4)[0]


def string_count(rom: bytes, pkg: int) -> int:
    """Number of string ids = (first stream offset - header word) / 4."""
    offtab = pkg + U32.unpack_from(rom, pkg - ROMBASE)[0]
    return (
        U32.unpack_from(rom, offtab - ROMBASE)[0]
        - U32.unpack_from(rom, pkg - ROMBASE)[0]
    ) // 4


def stream_addr(rom: bytes, pkg: int, sid: int) -> int:
    """ROM address of one string's compressed stream."""
    offtab = pkg + U32.unpack_from(rom, pkg - ROMBASE)[0]
    return pkg + U32.unpack_from(rom, offtab - ROMBASE + sid * 4)[0]


def decode(rom: bytes, pkg: int, sid: int, maxlen: int = 0x400) -> tuple[bytes, int]:
    """Decode one string -> (raw font bytes, compressed-stream length in bytes)."""
    treebase = pkg + 4
    foff = stream_addr(rom, pkg, sid) - ROMBASE
    out = bytearray()
    bitpos = 0
    bits = 0  # total bits consumed from the stream
    cur = rom[foff]
    foff += 1
    while len(out) < maxlen:
        node = 0x100
        while node > 0xFF:  # walk to a leaf, LSB-first within each byte
            node = U16.unpack_from(
                rom,
                treebase
                + (node - 0x100) * 4
                - ROMBASE
                + (2 if (cur >> bitpos) & 1 else 0),
            )[0]
            bitpos += 1
            bits += 1
            if bitpos == 8:
                bitpos, cur = 0, rom[foff]
                foff += 1
        if node == 0:  # NUL terminator
            break
        out.append(node)
    return bytes(out), (bits + 7) // 8  # streams are byte-aligned; round up


def decode_strings(rom: bytes, lang: int = LANG_EN) -> dict[int, str]:
    """Decode one language's text -> {id: text}; ASCII rendering (font bytes -> \\xNN)."""
    pkg = pkg_for(rom, lang)
    out = {}
    for sid in range(string_count(rom, pkg)):
        raw, _nbytes = decode(rom, pkg, sid)
        out[sid] = "".join(
            chr(b) if 0x20 <= b < 0x7F else "\n" if b == 0x0A else f"\\x{b:02x}"
            for b in raw
        )
    return out


def load_en_text() -> dict[int, str]:
    """{id: english text} decoded from the ROM."""
    return decode_strings(load_rom())
