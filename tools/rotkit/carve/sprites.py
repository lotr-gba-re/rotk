"""Sprite records the graphics carvers share: a frame set's tile data (per frame the 4bpp
tiles, lz77 compressed when that is smaller than the raw tiles, each chunk zero
padded to 4 bytes) followed by the frame set itself, which the carved .c spells out with
the spriteAnimation.h SPRITE_* macros. Every frame has the cell's canonical OBJ layout
(cell_oam) and at most one trigger box, so a PNG of the frames side by side carries the
pixels and the .c the rest; `rotkit build gfx` encodes <Symbol>_TILES plus per frame
<Symbol>_FRAME<k>_LZ77 and _OFFSET from it.
"""

import re
from dataclasses import dataclass

from rotkit.compression import lz77
from rotkit.png import IndexedImage, bgr555_to_rgb
from rotkit.rom import ROMBASE
from rotkit.spritegfx import (
    SPRITE_FRAME_LZ77,
    FrameSet,
    cell_oam,
    parse_frame_set,
    tiles_to_pixels,
)

SPRITE_FRAME_SIZE = 8
TRIGGER_BOX_SIZE = 6


@dataclass
class Frame:
    lz77: bool
    chunk: bytes  # the frame's ROM tile data including the zero padding
    tiles: bytes  # decoded 4bpp character data


def check(cond: bool, what: str) -> None:
    if not cond:
        raise SystemExit(what)


def pascal(text: str) -> str:
    """ "Deagol's Skull" / "GEM_STACK_MIN" -> DeagolsSkull / GemStackMin."""
    words = re.findall(r"[A-Za-z0-9]+", text.replace("'", ""))
    check(bool(words), f"name {text!r} has no alphanumeric characters")
    return "".join(w.capitalize() for w in words)


def file_stem(index: int, symbol: str, frame_count: int) -> str:
    """<index>_<Symbol>[_<n>frames], the PNG name `rotkit build gfx` parses."""
    suffix = f"_{frame_count}frames" if frame_count > 1 else ""
    return f"{index:02}_{symbol}{suffix}"


def read_frames(
    rom: bytes, tiles_addr: int, frames_addr: int, where: str
) -> tuple[FrameSet, list[Frame]]:
    """Parse and check one record's frame set and the tile data it indexes."""
    frame_set = parse_frame_set(rom, frames_addr)
    width, height = frame_set.width, frame_set.height
    cell = width * height // 2
    check(
        frame_set.field_0x2 == bytes(4)
        and frame_set.field_0xa == 0
        and frame_set.field_0x8 == cell
        and frame_set.trigger_box_count <= 1,
        f"{where}: unexpected frame set header",
    )
    first = frame_set.frames[0]
    layout = cell_oam(width, height, first.oam[0].x, first.oam[0].y)
    boxes = first.boxes
    check(
        all(box.enable == 1 and box.pad == 0 for box in boxes),
        f"{where}: unexpected trigger box",
    )
    frames = []
    start = tiles_addr - ROMBASE
    cursor = start
    for k, frame in enumerate(frame_set.frames):
        check(
            frame.flags & ~(0x1F | SPRITE_FRAME_LZ77) == 0
            and frame.field_0x1 == 0
            and not frame.extra
            and (frame.width, frame.height, frame.tile_bytes) == (width, height, cell)
            and frame.boxes == boxes,
            f"{where}: frame {k} differs from the record's shape",
        )
        check(frame.oam == layout, f"{where}: frame {k} OBJ layout is not the cell's")
        check(
            frame.tile_offset == cursor - start,
            f"{where}: frame {k} tile data does not follow frame {k - 1}'s",
        )
        compressed = bool(frame.flags & SPRITE_FRAME_LZ77)
        if compressed:
            tiles, end = lz77.decode(rom, cursor)
            check(
                len(tiles) == cell and lz77.encode(tiles) == rom[cursor:end],
                f"{where}: frame {k} does not re-encode",
            )
        else:
            end = cursor + cell
            tiles = rom[cursor:end]
        padded = end + (-end % 4)
        check(not any(rom[end:padded]), f"{where}: frame {k} padding is not zero")
        packed = len(lz77.encode(tiles))
        check(
            compressed == (packed + (-packed % 4) < cell),
            f"{where}: frame {k} is not compressed exactly when that is smaller",
        )
        frames.append(Frame(compressed, rom[cursor:padded], tiles))
        cursor = padded
    check(
        cursor == frames_addr - ROMBASE, f"{where}: frame set does not follow the tiles"
    )
    stride = SPRITE_FRAME_SIZE + TRIGGER_BOX_SIZE * len(boxes) + 4 * len(layout)
    check(
        frame_set.frame_offsets
        == tuple(2 * len(frames) + k * stride for k in range(len(frames))),
        f"{where}: frames are not packed the way SPRITE_FRAME_SET_STRUCT lays them out",
    )
    return frame_set, frames


def frame_set_end(frames_addr: int, frame_set: FrameSet) -> int:
    """First address after the frame set's 4-byte tail padding."""
    end = frames_addr + frame_set.size
    return end + (-end % 4)


def strip_image(
    frame_set: FrameSet, frames: list[Frame], palette: tuple[int, ...]
) -> IndexedImage:
    """The frames side by side, one cell each, index 0 transparent."""
    width, height = frame_set.width, frame_set.height
    strip_width = width * len(frames)
    strip = bytearray(strip_width * height)
    layout = cell_oam(width, height)
    for k, frame in enumerate(frames):
        pixels = tiles_to_pixels(frame.tiles, layout, width, height)
        for y in range(height):
            at = y * strip_width + k * width
            strip[at : at + width] = pixels[y * width : (y + 1) * width]
    return IndexedImage(
        strip_width, height, bytes(strip), [bgr555_to_rgb(c) for c in palette], 0
    )


def frame_set_lines(symbol: str, frame_set: FrameSet) -> list[str]:
    """`const SPRITE_FRAME_SET_STRUCT(...) <symbol>Frames = {...};` for the record."""
    frames = frame_set.frames
    oam_count = len(frames[0].oam)
    box_count = len(frames[0].boxes)
    width, height = frame_set.width, frame_set.height
    stride = SPRITE_FRAME_SIZE + TRIGGER_BOX_SIZE * box_count + 4 * oam_count
    offsets = ", ".join(str(2 * len(frames) + k * stride) for k in range(len(frames)))
    lines = [
        f"const SPRITE_FRAME_SET_STRUCT({len(frames)}, {box_count}, {oam_count})"
        f" {symbol}Frames = {{",
        f"    .header = SPRITE_FRAME_SET_HEADER({width}, {height}, {len(frames)}, {box_count}),",
        f"    .frameOffsets = {{{offsets}}},",
        "    .frames = {",
    ]
    for k, frame in enumerate(frames):
        oam = (",\n                 " if oam_count > 2 else ", ").join(
            f"SPRITE_OAM({o.x}, {o.y}, {o.size[0]}, {o.size[1]}, {o.tile_offset})"
            for o in frame.oam
        )
        lines += [
            f"        {{.frame = SPRITE_FRAME({oam_count}, {symbol}_FRAME{k}_LZ77, {width},"
            f" {height},",
            f"                               {symbol}_FRAME{k}_OFFSET),",
        ]
        if frame.boxes:
            boxes = ", ".join(
                f"TRIGGER_BOX({b.x_min}, {b.x_max}, {b.y_min}, {b.y_max})"
                for b in frame.boxes
            )
            lines.append(f"         .triggerBoxes = {{{boxes}}},")
        lines.append(f"         .oam = {{{oam}}}}},")
    return lines + ["    },", "};"]
