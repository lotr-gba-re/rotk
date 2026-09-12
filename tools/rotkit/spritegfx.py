"""Sprite frame sets and 4bpp OBJ character data (include/spriteAnimation.h), decoded from
the ROM and re-encoded from images.

A frame set is SpriteFrameSetHeader + u16 frame offsets + per frame a SpriteFrame, the
header's field_0xa u16s, triggerBoxCount trigger boxes and the frame's OAM templates.
The game maps OBJ tiles one-dimensionally (DISPCNT bit 6), so each template's tiles
are consecutive, row-major within that OBJ.
"""

from dataclasses import dataclass
from struct import Struct

from rotkit.rom import ROMBASE

TILE_BYTES = 32  # one 8x8 4bpp tile

# (objShape, objSize) -> OBJ pixel size (GBATEK OBJ attribute 0/1)
OBJ_SIZES = {
    (0, 0): (8, 8),
    (0, 1): (16, 16),
    (0, 2): (32, 32),
    (0, 3): (64, 64),
    (1, 0): (16, 8),
    (1, 1): (32, 8),
    (1, 2): (32, 16),
    (1, 3): (64, 32),
    (2, 0): (8, 16),
    (2, 1): (8, 32),
    (2, 2): (16, 32),
    (2, 3): (32, 64),
}

_HEADER = Struct("<BB4sHHBB")  # SpriteFrameSetHeader
_FRAME = Struct("<BBBBHH")  # SpriteFrame
_BOX = Struct("<bbbbBB")  # ActorTriggerBox
_U16 = Struct("<H")
_U32 = Struct("<I")

SPRITE_FRAME_RL = 1 << 5
SPRITE_FRAME_LZ77 = 1 << 6


@dataclass(frozen=True)
class OamTemplate:
    """Decoded union SpriteOamTemplate."""

    x: int
    y: int
    obj_size: int
    obj_shape: int
    tile_offset: int

    @classmethod
    def unpack(cls, word: int) -> "OamTemplate":
        def signed9(v: int) -> int:
            return v - 512 if v & 0x100 else v

        return cls(
            x=signed9(word & 0x1FF),
            y=signed9((word >> 9) & 0x1FF),
            obj_size=(word >> 18) & 3,
            obj_shape=(word >> 20) & 3,
            tile_offset=(word >> 22) & 0x3FF,
        )

    def pack(self) -> int:
        return (
            (self.x & 0x1FF)
            | ((self.y & 0x1FF) << 9)
            | (self.obj_size << 18)
            | (self.obj_shape << 20)
            | (self.tile_offset << 22)
        )

    @property
    def size(self) -> tuple[int, int]:
        return OBJ_SIZES[(self.obj_shape, self.obj_size)]


@dataclass(frozen=True)
class TriggerBox:
    x_min: int
    x_max: int
    y_min: int
    y_max: int
    enable: int
    pad: int


@dataclass(frozen=True)
class Frame:
    """One SpriteFrame plus the records that follow it."""

    flags: int
    field_0x1: int
    width: int
    height: int
    tile_offset: int
    tile_bytes: int
    extra: tuple[int, ...]  # header.field_0xa u16s
    boxes: tuple[TriggerBox, ...]
    oam: tuple[OamTemplate, ...]

    @property
    def oam_count(self) -> int:
        return self.flags & 0x1F


@dataclass(frozen=True)
class FrameSet:
    width: int
    height: int
    field_0x2: bytes
    frame_count: int
    field_0x8: int
    field_0xa: int
    trigger_box_count: int
    frame_offsets: tuple[int, ...]
    frames: tuple[Frame, ...]
    size: int  # bytes from the header to the end of the last record read


def parse_frame_set(rom: bytes, addr: int) -> FrameSet:
    base = addr - ROMBASE
    width, height, field_0x2, frame_count, field_0x8, field_0xa, box_count = (
        _HEADER.unpack_from(rom, base)
    )
    offsets = tuple(
        _U16.unpack_from(rom, base + _HEADER.size + i * 2)[0]
        for i in range(frame_count)
    )
    frames = []
    end = base + _HEADER.size + frame_count * 2
    for off in offsets:
        p = base + _HEADER.size + off
        flags, field_0x1, fw, fh, tile_off, tile_bytes = _FRAME.unpack_from(rom, p)
        p += _FRAME.size
        extra = tuple(_U16.unpack_from(rom, p + i * 2)[0] for i in range(field_0xa))
        p += field_0xa * 2
        boxes = tuple(
            TriggerBox(*_BOX.unpack_from(rom, p + i * _BOX.size))
            for i in range(box_count)
        )
        p += box_count * _BOX.size
        oam = tuple(
            OamTemplate.unpack(_U32.unpack_from(rom, p + i * 4)[0])
            for i in range(flags & 0x1F)
        )
        p += (flags & 0x1F) * 4
        end = max(end, p)
        frames.append(
            Frame(flags, field_0x1, fw, fh, tile_off, tile_bytes, extra, boxes, oam)
        )
    return FrameSet(
        width,
        height,
        field_0x2,
        frame_count,
        field_0x8,
        field_0xa,
        box_count,
        offsets,
        tuple(frames),
        end - base,
    )


def _cell_parts(size: int) -> list[int]:
    if size in (8, 16, 32, 64):
        return [size]
    if size == 24:
        return [8, 16]
    raise ValueError(f"no OBJ decomposition for a {size} pixel cell edge")


def cell_oam(
    width: int, height: int, x: int = 0, y: int = 0
) -> tuple[OamTemplate, ...]:
    """The OBJ templates covering a width x height cell whose top-left is at (x, y): each
    edge is one OBJ when a power of two, an 8 then a 16 when 24, laid out row-major with
    consecutive tiles. The game's assets decompose their cells this way."""
    oam = []
    tile = 0
    oy = 0
    for ph in _cell_parts(height):
        ox = 0
        for pw in _cell_parts(width):
            shape, size = next(k for k, v in OBJ_SIZES.items() if v == (pw, ph))
            oam.append(OamTemplate(x + ox, y + oy, size, shape, tile))
            tile += (pw // 8) * (ph // 8)
            ox += pw
        oy += ph
    return tuple(oam)


def tiles_to_pixels(
    tiles: bytes, oam: tuple[OamTemplate, ...], width: int, height: int
) -> bytes:
    """Compose a frame's OBJs into one width x height index image (row-major, one byte
    per pixel). Every OBJ must lie inside the cell."""
    pixels = bytearray(width * height)
    for obj in oam:
        ow, oh = obj.size
        if obj.x < 0 or obj.y < 0 or obj.x + ow > width or obj.y + oh > height:
            raise ValueError(f"OAM template {obj} outside the {width}x{height} cell")
        tile = obj.tile_offset
        for ty in range(oh // 8):
            for tx in range(ow // 8):
                src = tile * TILE_BYTES
                tile += 1
                for row in range(8):
                    for col in range(4):
                        byte = tiles[src + row * 4 + col]
                        px = (obj.y + ty * 8 + row) * width + obj.x + tx * 8 + col * 2
                        pixels[px] = byte & 0xF
                        pixels[px + 1] = byte >> 4
    return bytes(pixels)


def pixels_to_tiles(
    pixels: bytes, oam: tuple[OamTemplate, ...], width: int, height: int
) -> bytes:
    """Inverse of tiles_to_pixels: the frame's character data in OBJ tile order."""
    tile_count = max(
        obj.tile_offset + (obj.size[0] // 8) * (obj.size[1] // 8) for obj in oam
    )
    tiles = bytearray(tile_count * TILE_BYTES)
    for obj in oam:
        ow, oh = obj.size
        tile = obj.tile_offset
        for ty in range(oh // 8):
            for tx in range(ow // 8):
                dst = tile * TILE_BYTES
                tile += 1
                for row in range(8):
                    for col in range(4):
                        px = (obj.y + ty * 8 + row) * width + obj.x + tx * 8 + col * 2
                        tiles[dst + row * 4 + col] = pixels[px] | (pixels[px + 1] << 4)
    return bytes(tiles)
