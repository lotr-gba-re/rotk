"""Sprite frame sets and 4bpp OBJ character data (include/spriteAnimation.h), decoded from
the ROM and re-encoded from images.

A frame set is SpriteFrameSetHeader + u16 frame offsets + per frame a SpriteFrame, the
header's hotspotCount hotspots, triggerBoxCount trigger boxes and the frame's OAM templates.
The game maps OBJ tiles one-dimensionally (DISPCNT bit 6), so each template's tiles
are consecutive, row-major within that OBJ.
"""

from dataclasses import dataclass, replace
from struct import Struct

from rotkit.compression import bios, lz77
from rotkit.png import IndexedImage, bgr555_to_rgb, rgb_to_bgr555
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

_HEADER = Struct("<BB4bHHBB")  # SpriteFrameSetHeader
_FRAME = Struct("<BBBBHH")  # SpriteFrame
_HOTSPOT = Struct("<bb")  # SpriteHotspot
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
class SpriteBounds:
    x_min: int
    x_max: int
    y_min: int
    y_max: int


@dataclass(frozen=True)
class Hotspot:
    x: int
    y: int


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
    hotspots: tuple[Hotspot, ...]
    boxes: tuple[TriggerBox, ...]
    oam: tuple[OamTemplate, ...]

    @property
    def oam_count(self) -> int:
        return self.flags & 0x1F


@dataclass(frozen=True)
class FrameSet:
    width: int
    height: int
    movement_collision_box: SpriteBounds
    frame_count: int
    max_frame_tile_bytes: int
    hotspot_count: int
    trigger_box_count: int
    frame_offsets: tuple[int, ...]
    frames: tuple[Frame, ...]
    size: int  # bytes from the header to the end of the last record read


def parse_frame_set(rom: bytes, addr: int) -> FrameSet:
    base = addr - ROMBASE
    (
        width,
        height,
        x_min,
        x_max,
        y_min,
        y_max,
        frame_count,
        max_frame_tile_bytes,
        hotspot_count,
        box_count,
    ) = _HEADER.unpack_from(rom, base)
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
        hotspots = tuple(
            Hotspot(*_HOTSPOT.unpack_from(rom, p + i * _HOTSPOT.size))
            for i in range(hotspot_count)
        )
        p += hotspot_count * _HOTSPOT.size
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
            Frame(flags, field_0x1, fw, fh, tile_off, tile_bytes, hotspots, boxes, oam)
        )
    return FrameSet(
        width,
        height,
        SpriteBounds(x_min, x_max, y_min, y_max),
        frame_count,
        max_frame_tile_bytes,
        hotspot_count,
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
    """Compose OBJs into an indexed image, letting transparent pixels show lower layers."""
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
                        lo, hi = byte & 0xF, byte >> 4
                        if lo:
                            pixels[px] = lo
                        if hi:
                            pixels[px + 1] = hi
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


def frame_bounds(frames: tuple[Frame, ...]) -> tuple[int, int, int, int]:
    """Return the union of the frames' OBJ rectangles, or a 1x1 empty canvas."""
    objects = [obj for frame in frames for obj in frame.oam]
    if not objects:
        return 0, 0, 1, 1
    return (
        min(obj.x for obj in objects),
        min(obj.y for obj in objects),
        max(obj.x + obj.size[0] for obj in objects),
        max(obj.y + obj.size[1] for obj in objects),
    )


def decode_frame_tiles(data: bytes, frame: Frame) -> bytes:
    """Decode a frame's tile stream from its offset in the tile blob."""
    if not 0 <= frame.tile_offset <= len(data) or frame.tile_bytes < 0:
        raise ValueError("frame tile range is outside its tile data")
    try:
        if frame.flags & SPRITE_FRAME_LZ77:
            tiles, _end = lz77.decode(data, frame.tile_offset)
        elif frame.flags & SPRITE_FRAME_RL:
            header = data[frame.tile_offset : frame.tile_offset + 4]
            if len(header) != 4 or header[0] != 0x30:
                raise ValueError("invalid sprite BIOS RL header")
            size = int.from_bytes(header[1:], "little")
            tiles = bytes(bios.rl_decode(data, frame.tile_offset + 4, size))
        else:
            tiles = data[frame.tile_offset : frame.tile_offset + frame.tile_bytes]
    except IndexError as error:
        raise ValueError("invalid or truncated frame tile stream") from error
    if len(tiles) < frame.tile_bytes:
        raise ValueError("frame tile stream is shorter than frame data")
    return tiles[: frame.tile_bytes]


def _canvas_oam(
    frame: Frame, bounds: tuple[int, int, int, int], tile_bytes: int
) -> tuple[tuple[OamTemplate, ...], int, int]:
    left, top, right, bottom = bounds
    width, height = right - left, bottom - top
    if width <= 0 or height <= 0:
        raise ValueError("frame bounds must have positive dimensions")
    objects = tuple(replace(obj, x=obj.x - left, y=obj.y - top) for obj in frame.oam)
    for obj in objects:
        ow, oh = obj.size
        if obj.x < 0 or obj.y < 0 or obj.x + ow > width or obj.y + oh > height:
            raise ValueError("frame OAM lies outside its canvas bounds")
        end = (obj.tile_offset + (ow // 8) * (oh // 8)) * TILE_BYTES
        if obj.tile_offset < 0 or end > tile_bytes:
            raise ValueError("frame OAM references tiles outside its tile data")
    return objects, width, height


def render_frame(
    frame: Frame,
    tiles: bytes,
    bounds: tuple[int, int, int, int],
    palette: tuple[int, ...],
) -> IndexedImage:
    """Compose OBJs in record order, skipping transparent pixels."""
    objects, width, height = _canvas_oam(frame, bounds, len(tiles))
    return IndexedImage(
        width,
        height,
        tiles_to_pixels(tiles, objects, width, height),
        [bgr555_to_rgb(color) for color in palette],
        0,
    )


def remap_sprite_palette(image: IndexedImage, palette: tuple[int, ...]) -> IndexedImage:
    """Map used colors to the fixed sprite palette without changing transparency."""
    if image.transparent not in (None, 0):
        raise ValueError("sprite transparency must use index 0")
    if not 1 <= len(palette) <= 16:
        raise ValueError("sprite palettes require 1-16 colors")
    canonical = [color & 0x7FFF for color in palette]
    mapping = {}
    for index in set(image.pixels):
        if index >= 16:
            raise ValueError("sprite pixels require palette indexes 0-15")
        if index >= len(image.palette):
            raise ValueError(f"pixel index {index} is absent from the PNG palette")
        color = rgb_to_bgr555(image.palette[index]) & 0x7FFF
        matches = [
            i
            for i, candidate in enumerate(canonical)
            if candidate == color and (i == 0) == (index == 0)
        ]
        if not matches:
            raise ValueError(
                f"palette index {index} color {image.palette[index]} has no "
                "sprite palette match preserving transparent index 0"
            )
        mapping[index] = index if index in matches else matches[0]
    return IndexedImage(
        image.width,
        image.height,
        bytes(mapping[index] for index in image.pixels),
        [bgr555_to_rgb(color) for color in palette],
        image.transparent,
    )


def _pixel_owners(
    objects: tuple[OamTemplate, ...], width: int, height: int
) -> list[int]:
    """Map each canvas pixel to its last OAM layer's tile nibble."""
    owners = [-1] * (width * height)
    for obj in objects:
        ow, oh = obj.size
        row_start = obj.y * width + obj.x
        tile_row_width = (ow // 8) * 64
        columns = [(x // 8) * 64 + x % 8 for x in range(ow)]
        for y in range(oh):
            base = obj.tile_offset * 64 + (y // 8) * tile_row_width + (y % 8) * 8
            owners[row_start : row_start + ow] = [base + col for col in columns]
            row_start += width
    return owners


def _overlap_nibbles(
    objects: tuple[OamTemplate, ...], width: int, height: int
) -> dict[int, list[int]]:
    """Tile nibbles at canvas pixels covered by more than one OBJ, in record order."""
    owners = [-1] * (width * height)
    overlaps: dict[int, list[int]] = {}
    for obj in objects:
        ow, oh = obj.size
        for y in range(oh):
            for x in range(ow):
                nibble = (
                    obj.tile_offset * 64
                    + (y // 8) * (ow // 8) * 64
                    + (x // 8) * 64
                    + (y % 8) * 8
                    + x % 8
                )
                pixel = (obj.y + y) * width + obj.x + x
                if owners[pixel] >= 0:
                    overlaps.setdefault(pixel, [owners[pixel]]).append(nibble)
                owners[pixel] = nibble
    return overlaps


def transparent_overlap_owners(
    frame: Frame, tiles: bytes, bounds: tuple[int, int, int, int]
) -> list[list[int]]:
    """Record pixels whose visible nibble lies behind transparent foreground OBJs."""
    objects, width, height = _canvas_oam(frame, bounds, len(tiles))
    if len(objects) < 2:
        return []
    result = []
    for pixel, layers in _overlap_nibbles(objects, width, height).items():
        if _nibble(tiles, layers[-1]):
            continue
        for nibble in reversed(layers[:-1]):
            if _nibble(tiles, nibble):
                result.append([pixel, nibble])
                break
    return result


def _nibble(tiles: bytes | bytearray, index: int) -> int:
    return (tiles[index // 2] >> (4 * (index % 2))) & 15


def visible_nibbles(frame: Frame, bounds: tuple[int, int, int, int]) -> set[int]:
    """Tile nibbles on the last OAM layer at each canvas pixel."""
    objects, width, height = _canvas_oam(frame, bounds, frame.tile_bytes)
    return set(_pixel_owners(objects, width, height)) - {-1}


def apply_hidden_nibbles(
    frame: Frame, bounds: tuple[int, int, int, int], entries: list[list[int]]
) -> bytes:
    """Restore pixels that no visible PNG pixel can carry."""
    tiles = bytearray(frame.tile_bytes)
    visible = visible_nibbles(frame, bounds)
    seen = set()
    for entry in entries:
        if not isinstance(entry, list) or len(entry) != 2:
            raise ValueError("hidden nibble must be an [index, value] pair")
        index, value = entry
        if (
            type(index) is not int
            or type(value) is not int
            or not 0 <= index < frame.tile_bytes * 2
            or not 0 <= value < 16
            or index in visible
            or index in seen
        ):
            raise ValueError(f"invalid hidden tile nibble {entry!r}")
        seen.add(index)
        tiles[index // 2] |= value << (4 * (index % 2))
    return bytes(tiles)


def rebuild_frame_tiles(
    original: bytes,
    frame: Frame,
    image: IndexedImage,
    bounds: tuple[int, int, int, int],
    transparent_owners: list[list[int]] = (),
) -> bytes:
    """Apply PNG edits, preserving nibbles obscured by opaque OAM pixels."""
    objects, width, height = _canvas_oam(frame, bounds, len(original))
    if (image.width, image.height) != (width, height):
        raise ValueError("sprite image dimensions do not match its frame bounds")
    if len(image.pixels) != width * height:
        raise ValueError("sprite pixel count does not match its dimensions")
    if max(image.pixels, default=0) >= 16:
        raise ValueError("sprite pixels require palette indexes 0-15")
    owners = _pixel_owners(objects, width, height)
    overlaps = _overlap_nibbles(objects, width, height) if len(objects) > 1 else {}
    hints = {}
    for entry in transparent_owners:
        if (
            not isinstance(entry, list)
            or len(entry) != 2
            or any(type(value) is not int for value in entry)
        ):
            raise ValueError(f"invalid transparent overlap owner {entry!r}")
        pixel, nibble = entry
        if pixel in hints or nibble not in overlaps.get(pixel, ()):
            raise ValueError(f"invalid transparent overlap owner {entry!r}")
        hints[pixel] = nibble
    tiles = bytearray(original)
    for pixel, owner in enumerate(owners):
        value = image.pixels[pixel]
        if owner < 0:
            if value:
                raise ValueError("painted pixel lies outside frame OAM")
            continue
        # A transparent PNG pixel must clear every overlapping OBJ, or the rear
        # pixel would become visible on hardware.
        targets = (
            overlaps.get(pixel, (owner,)) if value == 0 else (hints.get(pixel, owner),)
        )
        for nibble in targets:
            offset = nibble >> 1
            shift = (nibble & 1) * 4
            tiles[offset] = (tiles[offset] & ~(15 << shift)) | (value << shift)
    rebuilt = bytes(tiles)
    if len(objects) > 1:
        # Shared tile data can make otherwise independent pixel edits conflict.
        spans = sorted(
            (obj.tile_offset, obj.tile_offset + obj.size[0] * obj.size[1] // 64)
            for obj in objects
        )
        if any(a[1] > b[0] for a, b in zip(spans, spans[1:])) and (
            render_frame(frame, rebuilt, bounds, ()).pixels != image.pixels
        ):
            raise ValueError(
                "sprite edits conflict through overlapping or shared tiles"
            )
    return rebuilt


def encode_frame_tiles(original: bytes, frame: Frame, tiles: bytes) -> bytes:
    """Preserve an unchanged chunk verbatim, otherwise encode and pad to four bytes."""
    if len(tiles) != frame.tile_bytes:
        raise ValueError("tile data length does not match the frame")
    if decode_frame_tiles(original, replace(frame, tile_offset=0)) == tiles:
        return original
    if frame.flags & SPRITE_FRAME_LZ77:
        encoded = lz77.encode(tiles)
    elif frame.flags & SPRITE_FRAME_RL:
        header = (0x30 | (len(tiles) << 8)).to_bytes(4, "little")
        encoded = header + bios.rl_encode(tiles)
    else:
        encoded = tiles
    return encoded + bytes(-len(encoded) % 4)


def encode_sequence_from_images(
    tiles_size: int,
    frames: tuple[Frame, ...],
    images: list[IndexedImage],
    palette: tuple[int, ...],
    hidden_nibbles: list[list[list[int]]],
    transparent_owners: list[list[list[int]]] | None = None,
) -> tuple[bytes, tuple[int, ...]]:
    """Encode PNGs plus structural metadata without reading original ROM bytes."""
    if transparent_owners is None:
        transparent_owners = [[] for _ in frames]
    if (
        len(images) != len(frames)
        or len(hidden_nibbles) != len(frames)
        or len(transparent_owners) != len(frames)
    ):
        raise ValueError("frame images and metadata must match the frame count")
    bounds = frame_bounds(frames)
    output = bytearray()
    offsets = []
    for index, (frame, image, patches, owners) in enumerate(
        zip(frames, images, hidden_nibbles, transparent_owners)
    ):
        try:
            image = remap_sprite_palette(image, palette)
            base = apply_hidden_nibbles(frame, bounds, patches)
            tiles = rebuild_frame_tiles(base, frame, image, bounds, owners)
            if frame.flags & SPRITE_FRAME_LZ77:
                encoded = lz77.encode(tiles)
            elif frame.flags & SPRITE_FRAME_RL:
                encoded = (0x30 | (len(tiles) << 8)).to_bytes(
                    4, "little"
                ) + bios.rl_encode(tiles)
            else:
                encoded = tiles
        except (ValueError, IndexError) as error:
            raise ValueError(f"frame {index}: {error}") from error
        offsets.append(len(output))
        output.extend(encoded)
        output.extend(bytes(-len(output) % 4))
    if len(output) > tiles_size:
        raise ValueError(
            f"animation needs {len(output)} tile bytes, but its ROM block has {tiles_size}"
        )
    output.extend(bytes(tiles_size - len(output)))
    return bytes(output), tuple(offsets)


def encode_sequence(
    original: bytes,
    frames: tuple[Frame, ...],
    images: list[IndexedImage],
    palette: tuple[int, ...],
) -> tuple[bytes, tuple[int, ...]]:
    """Encode common-canvas frames within the original tile block and return new offsets.

    Frames index aligned, consecutive chunks in original. Unchanged chunks stay verbatim,
    edited chunks retain their codecs, and the total storage remains fixed.
    """
    if len(images) != len(frames):
        raise ValueError(f"expected {len(frames)} frames, got {len(images)}")
    bounds = frame_bounds(frames)
    output = bytearray()
    offsets = []
    for index, (frame, image) in enumerate(zip(frames, images)):
        try:
            image = remap_sprite_palette(image, palette)
            decoded = decode_frame_tiles(original, frame)
            tiles = rebuild_frame_tiles(decoded, frame, image, bounds)
            end = (
                frames[index + 1].tile_offset
                if index + 1 < len(frames)
                else len(original)
            )
            chunk = original[frame.tile_offset : end]
            encoded = encode_frame_tiles(chunk, frame, tiles)
        except (ValueError, IndexError) as error:
            raise ValueError(f"frame {index}: {error}") from error
        offsets.append(len(output))
        output.extend(encoded)
    if len(output) > len(original):
        raise ValueError(
            f"animation needs {len(output)} tile bytes, "
            f"but its ROM block has {len(original)}"
        )
    output.extend(bytes(len(original) - len(output)))
    return bytes(output), tuple(offsets)
