"""BG assets, the tile graphics gfx_loadBgAsset reads: parse one from ROM bytes, compose it
to an indexed image, and rebuild it from an edited image (tile dedup plus the pucrunch
codec). include/gfx.h documents the byte layout.
"""

from dataclasses import dataclass
from struct import Struct

from rotkit.compression import pucrunch
from rotkit.png import IndexedImage, bgr555_to_rgb, rgb_to_bgr555

FLAG_PALETTE_256 = 1 << 0
FLAG_PALETTE_16 = 1 << 1
FLAG_RAW_BLOCK = 1 << 2
FLAG_MAP = 1 << 3
FLAG_TILES = 1 << 4
CODEC_MASK = 3 << 5
CODEC_PUCRUNCH = 3 << 5
FLAG_8BPP = 1 << 7
FLAG2_BYTE_MAP = 1 << 0

# the include/gfx.h names of the flag bits, for the carved initializers
FLAG_NAMES = {
    FLAG_PALETTE_256: "BG_ASSET_PALETTE_256",
    FLAG_PALETTE_16: "BG_ASSET_PALETTE_16",
    FLAG_MAP: "BG_ASSET_MAP",
    FLAG_TILES: "BG_ASSET_TILES",
    CODEC_PUCRUNCH: "BG_ASSET_CODEC_PUCRUNCH",
    FLAG_8BPP: "BG_ASSET_8BPP",
}

MAP_TILE_MASK = 0x3FF
MAP_HFLIP = 1 << 10
MAP_VFLIP = 1 << 11
MAP_BANK_SHIFT = 12

_U16 = Struct("<H")


@dataclass
class BgAsset:
    flags: int
    flags2: int
    palette: tuple[int, ...]  # bgr555, 16 or 256 colors
    width: int  # map size in tiles; 0 without a map
    height: int
    map: tuple[int, ...]  # width * height entries
    tiles: bytes  # decoded character data
    stream: bytes  # the tiles as stored: pucrunch header word, RLE table, bits

    @property
    def bpp(self) -> int:
        return 8 if self.flags & FLAG_8BPP else 4

    @property
    def tile_bytes(self) -> int:
        return 64 if self.flags & FLAG_8BPP else 32

    @property
    def tile_count(self) -> int:
        return len(self.tiles) // self.tile_bytes

    @property
    def map_entries(self) -> int:
        """Stored entry count: the map is padded to an even number."""
        count = self.width * self.height
        return count + (count & 1)

    @property
    def size(self) -> int:
        return len(serialize(self))


def parse(data: bytes, offset: int) -> BgAsset:
    """The asset at `offset`; supports the palette, u16 map and pucrunch tile parts."""
    flags, flags2 = data[offset], data[offset + 1]
    p = offset + 2
    unsupported = flags & (FLAG_RAW_BLOCK | CODEC_MASK) not in (0, CODEC_PUCRUNCH)
    if flags & FLAG_RAW_BLOCK or unsupported or flags2 & FLAG2_BYTE_MAP:
        raise ValueError(
            f"BG asset at {offset:#x}: unsupported flags {flags:#04x}/{flags2:#04x}"
        )
    if not flags & FLAG_TILES or flags & CODEC_MASK != CODEC_PUCRUNCH:
        raise ValueError(
            f"BG asset at {offset:#x}: no pucrunch tiles (flags {flags:#04x})"
        )
    colors = 256 if flags & FLAG_PALETTE_256 else 16 if flags & FLAG_PALETTE_16 else 0
    palette = Struct(f"<{colors}H").unpack_from(data, p)
    p += colors * 2
    width = height = 0
    entries: tuple[int, ...] = ()
    if flags & FLAG_MAP:
        width, height = _U16.unpack_from(data, p)[0], _U16.unpack_from(data, p + 2)[0]
        p += 4
        count = width * height
        entries = Struct(f"<{count}H").unpack_from(data, p)
        p += (count + (count & 1)) * 2
    size = _U16.unpack_from(data, p)[0]
    p += 2
    stream = bytes(data[p : p + size])
    tiles = pucrunch.decode(stream)
    return BgAsset(flags, flags2, palette, width, height, entries, tiles, stream)


def serialize(asset: BgAsset) -> bytes:
    out = bytearray([asset.flags, asset.flags2])
    out += Struct(f"<{len(asset.palette)}H").pack(*asset.palette)
    if asset.flags & FLAG_MAP:
        out += _U16.pack(asset.width) + _U16.pack(asset.height)
        out += Struct(f"<{asset.map_entries}H").pack(
            *asset.map, *([0] * (asset.map_entries - len(asset.map)))
        )
    out += _U16.pack(len(asset.stream)) + asset.stream
    return bytes(out)


def _tile_pixels(asset: BgAsset, index: int) -> list[int]:
    """One tile's 64 pixel values, row-major."""
    if asset.bpp == 8:
        return list(asset.tiles[index * 64 : index * 64 + 64])
    out = []
    for b in asset.tiles[index * 32 : index * 32 + 32]:
        out += [b & 15, b >> 4]
    return out


def compose(asset: BgAsset, tiles_wide: int = 0) -> IndexedImage:
    """The asset as one indexed image: with a 256-color palette and 4bpp tiles a pixel's
    index is bank * 16 + color; without a map the tiles go left to right, `tiles_wide`
    per row."""
    if asset.flags & FLAG_MAP:
        width, height, entries = asset.width, asset.height, asset.map
    else:
        width, height = tiles_wide, asset.tile_count // tiles_wide
        entries = tuple(range(width * height))
    banked = asset.bpp == 4 and len(asset.palette) == 256
    pixels = bytearray(width * 8 * height * 8)
    for ty in range(height):
        for tx in range(width):
            entry = entries[ty * width + tx]
            src = _tile_pixels(asset, entry & MAP_TILE_MASK)
            bank = (entry >> MAP_BANK_SHIFT) * 16 if banked else 0
            for y in range(8):
                sy = 7 - y if entry & MAP_VFLIP else y
                row = src[sy * 8 : sy * 8 + 8]
                if entry & MAP_HFLIP:
                    row.reverse()
                at = (ty * 8 + y) * width * 8 + tx * 8
                pixels[at : at + 8] = bytes(bank + v for v in row)
    palette = [bgr555_to_rgb(c) for c in asset.palette]
    return IndexedImage(width * 8, height * 8, bytes(pixels), palette, None)


def rebuild(image: IndexedImage, template: BgAsset, tiles_wide: int = 0) -> BgAsset:
    """The asset an image encodes to, in the shape of `template` (flags, palette size, map
    size): with a map the tiles are deduplicated (exact and flipped), without one they are
    taken in order. Two tiles of the same pattern in different palette banks stay apart, as
    the game's own tiler kept them. Raises ValueError on an image the shape cannot hold."""
    banked = template.bpp == 4 and len(template.palette) == 256
    tiles_wide = template.width or tiles_wide
    if template.flags & FLAG_MAP:
        want = (template.width * 8, template.height * 8)
    else:  # without a map the row count is the image's own
        want = (tiles_wide * 8, image.height - image.height % 8)
    if (image.width, image.height) != want:
        raise ValueError(
            f"{image.width}x{image.height}, the asset is {want[0]}x{want[1]}"
        )
    limit = 256 if template.bpp == 8 or banked else 16
    if max(image.pixels) >= limit:
        raise ValueError(
            f"pixel index {max(image.pixels)}; the palette has {limit} colors"
        )
    palette = [rgb_to_bgr555(c) for c in image.palette[: len(template.palette)]]
    palette += [0] * (len(template.palette) - len(palette))

    tiles = bytearray()
    entries = []
    seen: dict[bytes, int] = {}
    for ty in range(image.height // 8):
        for tx in range(tiles_wide):
            rows = [
                image.pixels[(ty * 8 + y) * image.width + tx * 8 :][:8]
                for y in range(8)
            ]
            bank = 0
            if banked:
                banks = {v >> 4 for row in rows for v in row}
                if len(banks) > 1:
                    raise ValueError(
                        f"tile ({tx}, {ty}) mixes palette banks {sorted(banks)}"
                    )
                bank = banks.pop()
                rows = [bytes(v & 15 for v in row) for row in rows]
            if not template.flags & FLAG_MAP:
                tiles += _pack_tile(rows, template.bpp)
                continue
            flipped = {
                0: rows,
                MAP_HFLIP: [row[::-1] for row in rows],
                MAP_VFLIP: rows[::-1],
                MAP_HFLIP | MAP_VFLIP: [row[::-1] for row in rows[::-1]],
            }
            for flip, variant in flipped.items():
                key = bytes([bank]) + b"".join(variant)
                if key in seen:
                    entries.append(seen[key] | flip | (bank << MAP_BANK_SHIFT))
                    break
            else:
                index = len(seen)
                if index > MAP_TILE_MASK:
                    raise ValueError("more than 1024 distinct tiles")
                seen[bytes([bank]) + b"".join(rows)] = index
                tiles += _pack_tile(rows, template.bpp)
                entries.append(index | (bank << MAP_BANK_SHIFT))
    return BgAsset(
        template.flags,
        template.flags2,
        tuple(palette),
        template.width,
        template.height,
        tuple(entries),
        bytes(tiles),
        pucrunch.encode(bytes(tiles)),
    )


def _pack_tile(rows: list[bytes], bpp: int) -> bytes:
    if bpp == 8:
        return b"".join(rows)
    return bytes(row[x] | (row[x + 1] << 4) for row in rows for x in range(0, 8, 2))
