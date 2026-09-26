"""Indexed PNG read/write for the carved graphics, on Pillow.

Writers emit what pixel editors expect: color type 3 with a PLTE, a tRNS chunk marking the
transparent index, 4-bit depth for palettes of up to 16 colors. Reading accepts any indexed
PNG an editor saves.
"""

import re
from bisect import bisect_right
from collections.abc import Sequence
from dataclasses import dataclass

from PIL import Image, ImageDraw, ImageFont


CAPTION_SIZE = 9  # points, the default font
CAPTION_LEADING = 10  # pixels per caption line
CAPTION_WIDTH = 110  # minimum cell width when captioning


@dataclass
class IndexedImage:
    width: int
    height: int
    pixels: bytes  # one palette index per pixel, row-major
    palette: list[tuple[int, int, int]]
    transparent: int | None  # palette index with alpha 0 (tRNS), if any


def _pillow_indexed(path: str, image: IndexedImage) -> Image.Image:
    if len(image.palette) > 256:
        raise ValueError(f"{path}: {len(image.palette)} palette entries, max 256")
    if max(image.pixels, default=0) >= len(image.palette):
        raise ValueError(f"{path}: pixel index outside the palette")
    result = Image.frombytes("P", (image.width, image.height), image.pixels)
    result.putpalette([channel for color in image.palette for channel in color])
    if image.transparent is not None:
        result.info["transparency"] = image.transparent
    return result


def write_indexed(path: str, image: IndexedImage) -> None:
    """Write an indexed PNG; depth 4 for up to 16 palette entries, else 8."""
    im = _pillow_indexed(path, image)
    im.save(path, format="PNG", bits=4 if len(image.palette) <= 16 else 8)


def write_animation_gif(
    path: str, frames: Sequence[IndexedImage], duration: int | Sequence[int] = 100
) -> None:
    """Write one indexed animation directly to a transparent, looping GIF."""
    if not frames:
        raise ValueError("animated GIFs need one or more frames")
    durations = [duration] if isinstance(duration, int) else duration
    if not isinstance(duration, int) and len(duration) != len(frames):
        raise ValueError("animated GIF durations must match its frames")
    if any(step < 10 or step % 10 for step in durations):
        raise ValueError("animated GIF durations must be positive multiples of 10 ms")
    first = frames[0]
    if any(
        (frame.width, frame.height, frame.palette, frame.transparent)
        != (first.width, first.height, first.palette, first.transparent)
        for frame in frames[1:]
    ):
        raise ValueError("animated GIF frames must have matching geometry and palettes")
    images = [_pillow_indexed(path, frame) for frame in frames]
    options = {
        "format": "GIF",
        "save_all": True,
        "append_images": images[1:],
        "duration": duration,
        "loop": 0,
        "disposal": 2,
        # Keep the transparent index even when the first frame does not use it.
        "optimize": False,
        "interlace": False,
    }
    if first.transparent is not None:
        options["transparency"] = first.transparent
    images[0].save(path, **options)


def _wrap_field(
    field: str, font: ImageFont.FreeTypeFont | ImageFont.ImageFont, width: int
) -> list[str]:
    """Break one label field into lines, preferring `_` and camel-case boundaries."""
    lines: list[str] = []
    current = ""
    for part in re.split(r"(?<=_)|(?<=[a-z])(?=[A-Z])", field):
        if current and font.getlength(current + part) > width:
            lines.append(current)
            current = ""
        while part and font.getlength(part) > width:
            take = len(part)
            while take > 1 and font.getlength(part[:take]) > width:
                take -= 1
            lines.append(part[:take])
            part = part[take:]
        current += part
    if current:
        lines.append(current)
    return lines


def _wrap(
    label: str,
    font: ImageFont.FreeTypeFont | ImageFont.ImageFont,
    width: int,
    separator: str = " | ",
) -> list[str]:
    """Break a label into lines, keeping separator-delimited fields together where possible."""
    lines: list[str] = []
    current = ""
    for field in label.split(separator):
        field_lines = _wrap_field(field, font, width) or [""]
        candidate = field if not current else current + separator + field
        if len(field_lines) > 1 or (current and font.getlength(candidate) > width):
            if current:
                ending = current + separator.rstrip()
                lines.append(ending if font.getlength(ending) <= width else current)
            lines.extend(field_lines[:-1])
            current = field_lines[-1]
        else:
            current = candidate
    if current:
        lines.append(current)
    return lines


def _label_text(label: str | Sequence[str], separator: str) -> str:
    return label if isinstance(label, str) else separator.join(label)


@dataclass(frozen=True)
class _SheetCell:
    x: int
    y: int
    width: int
    image_height: int


@dataclass
class _SheetLayout:
    background: Image.Image
    cells: list[_SheetCell]


@dataclass
class _SheetSection:
    layout: _SheetLayout
    animations: list[Sequence[IndexedImage]]
    notes: list[str | None]


def _sheet_layout(
    animations: list[Sequence[IndexedImage]],
    labels: list[str | Sequence[str]] | None,
    columns: int,
    label_separator: str,
    per_row: bool,
) -> _SheetLayout:
    if not animations or any(not animation for animation in animations):
        raise ValueError("reference sheets need one or more images")
    if columns <= 0:
        raise ValueError("reference sheet columns must be positive")
    if labels is not None and len(labels) != len(animations):
        raise ValueError("reference sheet labels must match its cells")
    if not label_separator:
        raise ValueError("reference sheet label separator must not be empty")

    font = ImageFont.load_default(CAPTION_SIZE)
    all_images = [image for animation in animations for image in animation]
    uniform_width = max(
        max(image.width for image in all_images) + 4,
        CAPTION_WIDTH if labels else 0,
    )
    uniform_image_height = max(image.height for image in all_images) + 4
    uniform_captions = (
        [
            _wrap(
                _label_text(label, label_separator),
                font,
                uniform_width - 4,
                label_separator,
            )
            for label in labels
        ]
        if labels
        else []
    )
    uniform_text_height = (
        max((len(caption) for caption in uniform_captions), default=0) * CAPTION_LEADING
    )
    rows = []
    for start in range(0, len(animations), columns):
        end = min(start + columns, len(animations))
        row_animations = animations[start:end]
        row_images = [image for animation in row_animations for image in animation]
        cell_width = (
            max(
                max(image.width for image in row_images) + 4,
                CAPTION_WIDTH if labels else 0,
            )
            if per_row
            else uniform_width
        )
        image_height = (
            max(image.height for image in row_images) + 4
            if per_row
            else uniform_image_height
        )
        captions = (
            [
                _wrap(
                    _label_text(label, label_separator),
                    font,
                    cell_width - 4,
                    label_separator,
                )
                for label in labels[start:end]
            ]
            if labels and per_row
            else uniform_captions[start:end]
        )
        text_height = (
            max((len(caption) for caption in captions), default=0) * CAPTION_LEADING
            if per_row
            else uniform_text_height
        )
        cell_count = end - start if per_row else columns
        rows.append(
            (start, end, cell_width, image_height, text_height, captions, cell_count)
        )

    width = max(
        cell_width * cell_count for _, _, cell_width, _, _, _, cell_count in rows
    )
    height = sum(
        image_height + text_height for _, _, _, image_height, text_height, _, _ in rows
    )
    sheet = Image.new("RGB", (width, height), (0x20, 0x20, 0x28))
    draw = ImageDraw.Draw(sheet)
    cells = []
    y = 0
    for start, end, cell_width, image_height, text_height, captions, cell_count in rows:
        x0 = (width - cell_width * cell_count) // 2
        for column in range(end - start):
            x = x0 + column * cell_width
            cells.append(_SheetCell(x, y, cell_width, image_height))
            if labels:
                line_y = y + image_height
                for line in captions[column]:
                    draw.text(
                        (x + cell_width // 2, line_y),
                        line,
                        font=font,
                        fill=(0xC8, 0xC8, 0xD0),
                        anchor="ma",
                    )
                    line_y += CAPTION_LEADING
        y += image_height + text_height
    return _SheetLayout(sheet, cells)


def _render_sheet(
    layout: _SheetLayout,
    images: Sequence[IndexedImage],
    notes: Sequence[str | None],
) -> Image.Image:
    sheet = layout.background.copy()
    background = bytes((0x20, 0x20, 0x28))
    for cell, image in zip(layout.cells, images):
        ox = cell.x + (cell.width - image.width) // 2
        oy = cell.y + 2
        if image.pixels and max(image.pixels) >= max(1, len(image.palette)):
            raise IndexError("list index out of range")
        # Pillow accepts at most 256 entries, the range of an 8-bit pixel index.
        palette = [background, *(bytes(color) for color in image.palette[1:256])]
        sprite = Image.frombytes("P", (image.width, image.height), image.pixels)
        sprite.putpalette(b"".join(palette))
        sprite = sprite.convert("RGB")
        sheet.paste(sprite, (ox, oy))
    draw = ImageDraw.Draw(sheet)
    font = ImageFont.load_default(CAPTION_SIZE)
    for cell, note in zip(layout.cells, notes):
        if note is not None:
            x = cell.x + cell.width // 2
            y = cell.y + cell.image_height // 2
            draw.text(
                (x, y),
                note,
                font=font,
                fill=(0xC8, 0xC8, 0xD0),
                anchor="mm",
            )
    return sheet


def _sheet_sections(
    animations: list[Sequence[IndexedImage]],
    labels: list[str | Sequence[str]] | None,
    columns: int,
    label_separator: str,
    isolated: Sequence[int],
    per_row: bool,
) -> list[_SheetSection]:
    count = len(animations)
    isolated = tuple(isolated)
    if len(set(isolated)) != len(isolated) or any(
        index < 0 or index >= count for index in isolated
    ):
        raise ValueError("isolated reference sheet cells must be unique valid indexes")
    if labels is not None and len(labels) != count:
        raise ValueError("reference sheet labels must match its cells")

    isolated_set = set(isolated)
    blank_height = CAPTION_LEADING if labels is not None else 1
    blank = IndexedImage(
        1,
        blank_height,
        bytes(blank_height),
        [(0x20, 0x20, 0x28)],
        0,
    )
    main_animations = [
        (blank,) if index in isolated_set else animation
        for index, animation in enumerate(animations)
    ]
    main_notes = [
        "(see below)" if labels is not None and index in isolated_set else None
        for index in range(count)
    ]
    sections = [
        _SheetSection(
            _sheet_layout(
                main_animations,
                labels,
                columns,
                label_separator,
                per_row,
            ),
            main_animations,
            main_notes,
        )
    ]
    for index in isolated:
        animation = animations[index]
        section_labels = [labels[index]] if labels is not None else None
        sections.append(
            _SheetSection(
                _sheet_layout([animation], section_labels, 1, label_separator, False),
                [animation],
                [None],
            )
        )
    return sections


def _render_sections(sections: list[_SheetSection], frame_index: int) -> Image.Image:
    rendered = [
        _render_sheet(
            section.layout,
            [
                animation[min(frame_index, len(animation) - 1)]
                for animation in section.animations
            ],
            section.notes,
        )
        for section in sections
    ]
    if len(rendered) == 1:
        return rendered[0]

    main, isolated = rendered[0], rendered[1:]
    isolated_width = sum(sheet.width for sheet in isolated)
    isolated_height = max(sheet.height for sheet in isolated)
    width = max(main.width, isolated_width)
    result = Image.new(
        "RGB", (width, main.height + isolated_height), (0x20, 0x20, 0x28)
    )
    result.paste(main, ((width - main.width) // 2, 0))
    x = (width - isolated_width) // 2
    for sheet in isolated:
        result.paste(sheet, (x, main.height + isolated_height - sheet.height))
        x += sheet.width
    return result


def write_sheet(
    path: str,
    images: list[IndexedImage],
    labels: list[str | Sequence[str]] | None = None,
    columns: int = 8,
    label_separator: str = " | ",
    isolated: Sequence[int] = (),
    per_row: bool = False,
) -> None:
    """Viewing aid: the images on a dark grid in list order, cells sized to the largest,
    palette index 0 left as background. With labels, each image gets its name captioned
    underneath, wrapped to the cell. Isolated cells leave a note in the grid and share
    a separate row below it."""
    animations = [(image,) for image in images]
    sections = _sheet_sections(
        animations, labels, columns, label_separator, isolated, per_row
    )
    _render_sections(sections, 0).save(path, format="PNG")


def write_animated_sheet(
    path: str,
    animations: list[Sequence[IndexedImage]],
    labels: list[str | Sequence[str]] | None = None,
    columns: int = 8,
    duration: int = 100,
    label_separator: str = " | ",
    isolated: Sequence[int] = (),
    per_row: bool = False,
    frame_times: Sequence[Sequence[int]] | None = None,
) -> None:
    """Write an animated reference sheet from equal-cell animation sequences.

    Shorter sequences hold their last frame until the longest finishes, then the whole
    sheet loops. `frame_times` gives each cell's frame boundaries in milliseconds;
    otherwise all cells use `duration`. Isolated cells share a row below the grid.
    """
    if not animations or any(not animation for animation in animations):
        raise ValueError(
            "animated reference sheets need one or more non-empty animations"
        )
    if duration < 10 or duration % 10:
        raise ValueError(
            "animated reference sheet duration must be a positive multiple of 10 ms"
        )
    if frame_times is not None:
        if len(frame_times) != len(animations) or any(
            len(boundaries) != len(animation) + 1
            or boundaries[0] != 0
            or any(
                stop <= start or stop % 10
                for start, stop in zip(boundaries, boundaries[1:])
            )
            for animation, boundaries in zip(animations, frame_times)
        ):
            raise ValueError(
                "animated reference sheet needs centisecond frame boundaries per cell"
            )
        times = sorted(
            {max(boundaries[-1] for boundaries in frame_times)}
            | {time for boundaries in frame_times for time in boundaries[:-1]}
        )
        frame_durations = [stop - start for start, stop in zip(times, times[1:])]
        animations = [
            [
                animation[min(bisect_right(boundaries, time) - 1, len(animation) - 1)]
                for time in times[:-1]
            ]
            for animation, boundaries in zip(animations, frame_times)
        ]
    else:
        frame_durations = duration
    sections = _sheet_sections(
        animations, labels, columns, label_separator, isolated, per_row
    )
    sheets = [
        _render_sections(sections, frame_index)
        for frame_index in range(max(len(animation) for animation in animations))
    ]
    sheets[0].save(
        path,
        format="GIF",
        save_all=True,
        append_images=sheets[1:],
        duration=frame_durations,
        loop=0,
    )


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
