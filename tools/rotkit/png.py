"""Indexed PNG read/write for the carved graphics, on Pillow.

Writers emit what pixel editors expect: color type 3 with a PLTE, a tRNS chunk marking the
transparent index, 4-bit depth for palettes of up to 16 colors. Reading accepts any indexed
PNG an editor saves.
"""

import re
from dataclasses import dataclass

from PIL import Image, ImageDraw, ImageFont


CAPTION_SIZE = 9  # points, the default font
CAPTION_LEADING = 10  # pixels per caption line
CAPTION_WIDTH = 76  # minimum cell width when captioning


@dataclass
class IndexedImage:
    width: int
    height: int
    pixels: bytes  # one palette index per pixel, row-major
    palette: list[tuple[int, int, int]]
    transparent: int | None  # palette index with alpha 0 (tRNS), if any


def write_indexed(path: str, image: IndexedImage) -> None:
    """Write an indexed PNG; depth 4 for up to 16 palette entries, else 8."""
    if len(image.palette) > 256:
        raise ValueError(f"{path}: {len(image.palette)} palette entries, max 256")
    if max(image.pixels, default=0) >= len(image.palette):
        raise ValueError(f"{path}: pixel index outside the palette")
    im = Image.frombytes("P", (image.width, image.height), image.pixels)
    im.putpalette([channel for color in image.palette for channel in color])
    options = {"bits": 4 if len(image.palette) <= 16 else 8}
    if image.transparent is not None:
        options["transparency"] = image.transparent
    im.save(path, format="PNG", **options)


def _wrap(label: str, font, width: int) -> list[str]:
    """Break a label into lines fitting `width`, preferring the `_` and camel-case
    boundaries."""
    lines: list[str] = []
    for part in re.split(r"(?<=_)|(?=[A-Z])", label):
        while part:
            take = len(part)
            while take > 1 and font.getlength(part[:take]) > width:
                take -= 1
            if lines and font.getlength(lines[-1] + part[:take]) <= width:
                lines[-1] += part[:take]
            else:
                lines.append(part[:take])
            part = part[take:]
    return lines


def write_sheet(
    path: str,
    images: list[IndexedImage],
    labels: list[str] | None = None,
    columns: int = 8,
) -> None:
    """Viewing aid: the images on a dark grid in list order, cells sized to the largest,
    palette index 0 left as background. With labels, each image gets its name captioned
    underneath, wrapped to the cell."""
    font = ImageFont.load_default(CAPTION_SIZE)
    cell_w = max(max(i.width for i in images) + 4, CAPTION_WIDTH if labels else 0)
    cell_h = max(i.height for i in images) + 4
    captions = [_wrap(label, font, cell_w - 4) for label in labels] if labels else []
    text_h = max((len(c) for c in captions), default=0) * CAPTION_LEADING
    rows = (len(images) + columns - 1) // columns
    width, height = columns * cell_w, rows * (cell_h + text_h)
    background = (0x20, 0x20, 0x28)
    rgb = bytearray(bytes(background) * (width * height))
    for i, image in enumerate(images):
        ox = (i % columns) * cell_w + (cell_w - image.width) // 2
        oy = (i // columns) * (cell_h + text_h) + 2
        for y in range(image.height):
            for x in range(image.width):
                index = image.pixels[y * image.width + x]
                if index == 0:
                    continue
                at = ((oy + y) * width + ox + x) * 3
                rgb[at : at + 3] = bytes(image.palette[index])
    sheet = Image.frombytes("RGB", (width, height), bytes(rgb))
    draw = ImageDraw.Draw(sheet)
    for i, caption in enumerate(captions):
        x = (i % columns) * cell_w + cell_w // 2
        y = (i // columns) * (cell_h + text_h) + cell_h
        for line in caption:
            draw.text((x, y), line, font=font, fill=(0xC8, 0xC8, 0xD0), anchor="ma")
            y += CAPTION_LEADING
    sheet.save(path, format="PNG")


def read_indexed(path: str) -> IndexedImage:
    """Read an indexed (color type 3) PNG."""
    im = Image.open(path)
    if im.mode != "P":
        raise ValueError(f"{path}: mode {im.mode}, expected an indexed (P) image")
    flat = im.getpalette()
    palette = [tuple(flat[i : i + 3]) for i in range(0, len(flat), 3)]
    transparency = im.info.get("transparency")
    if isinstance(transparency, bytes):
        zeros = [i for i, alpha in enumerate(transparency) if alpha == 0]
        if len(zeros) > 1 or any(0 < alpha < 255 for alpha in transparency):
            raise ValueError(f"{path}: only one fully transparent index is supported")
        transparency = zeros[0] if zeros else None
    return IndexedImage(im.width, im.height, im.tobytes(), palette, transparency)


def bgr555_to_rgb(color: int) -> tuple[int, int, int]:
    """GBA 16-bit color -> 8-bit RGB, losslessly.

    Each channel is its 5 bits replicated. Bit 15, which the hardware ignores but the BG
    asset palettes set on about half their entries, flips the red channel's low bit: the
    5-bit value survives `>> 3` either way, so the color moves by 1/255.
    """
    r, g, b = color & 31, (color >> 5) & 31, (color >> 10) & 31
    return (
        ((r << 3) | (r >> 2)) ^ (color >> 15),
        (g << 3) | (g >> 2),
        (b << 3) | (b >> 2),
    )


def rgb_to_bgr555(rgb: tuple[int, int, int]) -> int:
    r, g, b = rgb
    return (r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10) | (((r ^ (r >> 5)) & 1) << 15)
