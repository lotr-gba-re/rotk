from collections.abc import Callable
import math
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont, ImageSequence
import pytest
from rotkit.png import (
    CAPTION_LEADING,
    CAPTION_SIZE,
    CAPTION_WIDTH,
    IndexedImage,
    _wrap,
    write_animated_sheet,
    write_animation_gif,
    write_sheet,
)


BACKGROUND = (0x20, 0x20, 0x28)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)


def sprite(
    color: tuple[int, int, int], width: int = 1, height: int = 1
) -> IndexedImage:
    return IndexedImage(
        width, height, bytes([1]) * (width * height), [BACKGROUND, color], 0
    )


@pytest.fixture(params=[write_sheet, write_animated_sheet], ids=["static", "animated"])
def sheet_writer(request: pytest.FixtureRequest) -> Callable[..., None]:
    return request.param


@pytest.fixture
def font() -> ImageFont.FreeTypeFont | ImageFont.ImageFont:
    return ImageFont.load_default(CAPTION_SIZE)


@pytest.mark.parametrize("final_color", [GREEN, BLUE], ids=["merged", "distinct"])
def test_short_animation_holds_last_frame(
    tmp_path: Path, final_color: tuple[int, int, int]
) -> None:
    path = tmp_path / "sheet.gif"
    write_animated_sheet(
        str(path),
        [
            [sprite(RED), sprite(GREEN)],
            [sprite(BLUE), sprite(GREEN), sprite(final_color)],
        ],
        columns=2,
        duration=100,
    )
    timeline = []
    with Image.open(path) as sheet:
        assert sheet.size == (10, 5)
        assert sheet.info["loop"] == 0
        for frame in ImageSequence.Iterator(sheet):
            duration = frame.info["duration"]
            assert duration % 100 == 0
            rgb = frame.convert("RGB")
            colors = (rgb.getpixel((2, 2)), rgb.getpixel((7, 2)))
            timeline.extend([colors] * (duration // 100))
    assert timeline == [(RED, BLUE), (GREEN, GREEN), (GREEN, final_color)]


def test_sheet_cells_advance_at_independent_speeds(tmp_path: Path) -> None:
    path = tmp_path / "sheet.gif"
    write_animated_sheet(
        str(path),
        [[sprite(RED), sprite(GREEN), sprite(BLUE)], [sprite(BLUE), sprite(RED)]],
        columns=2,
        frame_times=[[0, 30, 70, 100], [0, 70, 140]],
    )
    timeline = []
    frame_durations = []
    with Image.open(path) as sheet:
        for frame in ImageSequence.Iterator(sheet):
            duration = frame.info["duration"]
            frame_durations.append(duration)
            assert duration % 10 == 0
            rgb = frame.convert("RGB")
            colors = (rgb.getpixel((2, 2)), rgb.getpixel((7, 2)))
            timeline.extend([colors] * (duration // 10))
    assert timeline == ([(RED, BLUE)] * 3 + [(GREEN, BLUE)] * 4 + [(BLUE, RED)] * 7)
    assert min(frame_durations) >= 30


@pytest.mark.parametrize(
    ("label", "lines"),
    [
        ("Enemy | Idle", ("Enemy | Idle",)),
        (["Enemy", "Idle"], ("Enemy | Idle",)),
        (["MMMMMMMMMMMM", "MMMMMMMMMMMM"], ("MMMMMMMMMMMM |", "MMMMMMMMMMMM")),
    ],
    ids=["string", "fields", "wrapped-fields"],
)
def test_static_and_animated_captions_share_layout(
    tmp_path: Path, label: str | list[str], lines: tuple[str, ...]
) -> None:
    image = sprite(RED, 3, 2)
    png = tmp_path / "sheet.png"
    gif = tmp_path / "sheet.gif"
    write_sheet(str(png), [image], labels=[label], columns=1)
    write_animated_sheet(str(gif), [[image]], labels=[label], columns=1)
    expected_caption = Image.new(
        "RGB", (CAPTION_WIDTH, len(lines) * CAPTION_LEADING), BACKGROUND
    )
    draw = ImageDraw.Draw(expected_caption)
    for index, line in enumerate(lines):
        draw.text(
            (CAPTION_WIDTH // 2, index * CAPTION_LEADING),
            line,
            font=ImageFont.load_default(CAPTION_SIZE),
            fill=(0xC8, 0xC8, 0xD0),
            anchor="ma",
        )
    with Image.open(png) as static, Image.open(gif) as animated:
        assert static.size == (CAPTION_WIDTH, 6 + len(lines) * CAPTION_LEADING)
        assert animated.size == static.size
        assert animated.convert("RGB").tobytes() == static.tobytes()
        assert static.getpixel(((CAPTION_WIDTH - 3) // 2, 2)) == RED
        caption = static.crop((0, 6, CAPTION_WIDTH, static.height))
        assert caption.tobytes() == expected_caption.tobytes()


def test_isolated_cell_gets_bottom_row(
    tmp_path: Path, sheet_writer: Callable[..., None]
) -> None:
    images = [sprite(RED, 3, 2), sprite(GREEN, 200, 20), sprite(BLUE, 3, 2)]
    cells = (
        [[image] for image in images]
        if sheet_writer is write_animated_sheet
        else images
    )
    suffix = "gif" if sheet_writer is write_animated_sheet else "png"
    path = tmp_path / f"sheet.{suffix}"
    sheet_writer(str(path), cells, labels=["A", "B", "C"], columns=2, isolated=[1])
    with Image.open(path) as sheet:
        rgb = sheet.convert("RGB")
        assert rgb.size == (220, 82)
        assert rgb.getpixel((53, 2)) == RED
        assert rgb.getpixel((53, 26)) == BLUE
        assert GREEN not in set(rgb.crop((0, 0, 220, 48)).get_flattened_data())
        assert set(rgb.crop((110, 0, 220, 14)).get_flattened_data()) != {BACKGROUND}
        assert rgb.getpixel((10, 50)) == GREEN


def test_isolated_cells_share_bottom_row(
    tmp_path: Path, sheet_writer: Callable[..., None]
) -> None:
    images = [sprite(RED, 3, 2), sprite(GREEN, 200, 20), sprite(BLUE, 150, 20)]
    cells = (
        [[image] for image in images]
        if sheet_writer is write_animated_sheet
        else images
    )
    suffix = "gif" if sheet_writer is write_animated_sheet else "png"
    path = tmp_path / f"sheet.{suffix}"
    sheet_writer(str(path), cells, labels=["A", "B", "C"], columns=3, isolated=[1, 2])
    with Image.open(path) as sheet:
        rgb = sheet.convert("RGB")
        assert rgb.size == (358, 58)
        assert rgb.getpixel((67, 2)) == RED
        assert rgb.getpixel((2, 26)) == GREEN
        assert rgb.getpixel((206, 26)) == BLUE


def test_per_row_sizing(tmp_path: Path, sheet_writer: Callable[..., None]) -> None:
    images = [
        sprite(RED, 100, 20),
        sprite(GREEN),
        sprite(BLUE, 10, 5),
        sprite(RED),
    ]
    cells = (
        [[image] for image in images]
        if sheet_writer is write_animated_sheet
        else images
    )
    suffix = "gif" if sheet_writer is write_animated_sheet else "png"
    path = tmp_path / f"sheet.{suffix}"
    sheet_writer(str(path), cells, columns=2, per_row=True)
    with Image.open(path) as sheet:
        rgb = sheet.convert("RGB")
        assert rgb.size == (208, 33)
        assert rgb.getpixel((2, 2)) == RED
        assert rgb.getpixel((155, 2)) == GREEN
        assert rgb.getpixel((92, 26)) == BLUE
        assert rgb.getpixel((110, 26)) == RED


def test_grid_placement_and_index_zero_background(tmp_path: Path) -> None:
    images = [sprite(RED), sprite(GREEN, 3, 2), sprite(BLUE, 2)]
    images[2].pixels = bytes([0, 1])
    images[2].palette[0] = RED
    images[2].transparent = 1
    path = tmp_path / "sheet.png"
    write_sheet(str(path), images, columns=2)
    with Image.open(path) as sheet:
        assert sheet.size == (14, 12)
        assert sheet.getpixel((3, 2)) == RED
        assert sheet.getpixel((9, 2)) == GREEN
        assert sheet.getpixel((11, 3)) == GREEN
        assert sheet.getpixel((2, 8)) == BACKGROUND
        assert sheet.getpixel((3, 8)) == BLUE
        assert sheet.getpixel((9, 8)) == BACKGROUND


def test_sheet_rejects_missing_used_palette_color(
    tmp_path: Path, sheet_writer: Callable[..., None]
) -> None:
    image = IndexedImage(1, 1, b"\x02", [BACKGROUND, RED], 0)
    animations = [[image]] if sheet_writer is write_animated_sheet else [image]
    suffix = ".gif" if sheet_writer is write_animated_sheet else ".png"
    path = tmp_path / f"invalid{suffix}"
    with pytest.raises((IndexError, ValueError)):
        sheet_writer(str(path), animations)
    assert not path.exists()


def test_animated_layout_uses_largest_frame(tmp_path: Path) -> None:
    path = tmp_path / "sheet.gif"
    write_animated_sheet(str(path), [[sprite(RED), sprite(BLUE, 5, 3)]], columns=1)
    with Image.open(path) as sheet:
        assert sheet.size == (9, 7)
        assert sheet.convert("RGB").getpixel((4, 2)) == RED
        sheet.seek(1)
        assert sheet.convert("RGB").getpixel((2, 2)) == BLUE
        assert sheet.convert("RGB").getpixel((6, 4)) == BLUE


def test_indexed_animation_gif_preserves_transparency(tmp_path: Path) -> None:
    path = tmp_path / "animation.gif"
    frames = [
        IndexedImage(2, 1, bytes([1, 0]), [BACKGROUND, RED, BLUE], 0),
        IndexedImage(2, 1, bytes([0, 0]), [BACKGROUND, RED, BLUE], 0),
        IndexedImage(2, 1, bytes([0, 2]), [BACKGROUND, RED, BLUE], 0),
    ]
    write_animation_gif(str(path), frames, duration=70)
    with Image.open(path) as animation:
        assert animation.size == (2, 1)
        assert animation.n_frames == 3
        assert animation.info["loop"] == 0
        actual = []
        for frame in ImageSequence.Iterator(animation):
            assert frame.info["duration"] == 70
            actual.append(list(frame.convert("RGBA").get_flattened_data()))
    assert actual == [
        [(255, 0, 0, 255), (32, 32, 40, 0)],
        [(32, 32, 40, 0), (32, 32, 40, 0)],
        [(32, 32, 40, 0), (0, 0, 255, 255)],
    ]


@pytest.mark.parametrize("transparent", [0, 2])
def test_indexed_animation_gif_opaque_first_frame(
    tmp_path: Path, transparent: int
) -> None:
    path = tmp_path / "animation.gif"
    palette = [BACKGROUND, RED, BLUE]
    frames = [
        IndexedImage(2, 1, bytes([1, 1]), palette, transparent),
        IndexedImage(2, 1, bytes([transparent, 1]), palette, transparent),
        IndexedImage(2, 1, bytes([1, transparent]), palette, transparent),
    ]
    write_animation_gif(str(path), frames)
    with Image.open(path) as animation:
        actual = [
            list(frame.convert("RGBA").get_flattened_data())
            for frame in ImageSequence.Iterator(animation)
        ]
    clear = (*palette[transparent], 0)
    red = (*RED, 255)
    assert actual == [[red, red], [clear, red], [red, clear]]


@pytest.mark.parametrize(
    ("frames", "duration"),
    [
        ([], 100),
        ([sprite(RED)], 0),
        ([sprite(RED)], 5),
        ([sprite(RED)], 15),
        ([sprite(RED)], [30, 40]),
        ([sprite(RED)], [5]),
        ([sprite(RED), sprite(RED, 2)], 100),
    ],
    ids=[
        "empty",
        "zero-duration",
        "sub-centisecond",
        "fractional-centisecond",
        "duration-count",
        "frame-duration-resolution",
        "geometry",
    ],
)
def test_indexed_animation_gif_validation(
    tmp_path: Path, frames: list[IndexedImage], duration: int | list[int]
) -> None:
    with pytest.raises(ValueError):
        write_animation_gif(str(tmp_path / "invalid.gif"), frames, duration)
    assert list(tmp_path.iterdir()) == []


@pytest.mark.parametrize(
    "options",
    [
        {"columns": 0},
        {"columns": -1},
        {"labels": []},
        {"labels": ["a", "b"]},
        {"label_separator": ""},
        {"isolated": [-1]},
        {"isolated": [1]},
        {"isolated": [0, 0]},
    ],
    ids=[
        "zero-columns",
        "negative-columns",
        "few-labels",
        "many-labels",
        "empty-separator",
        "negative-isolated",
        "large-isolated",
        "duplicate-isolated",
    ],
)
def test_sheet_validation(
    tmp_path: Path,
    sheet_writer: Callable[..., None],
    options: dict[str, int | str | list[int] | list[str]],
) -> None:
    image = sprite(RED)
    cells = [[image]] if sheet_writer is write_animated_sheet else [image]
    with pytest.raises(ValueError):
        sheet_writer(str(tmp_path / "invalid"), cells, **options)
    assert list(tmp_path.iterdir()) == []


def test_sheet_rejects_empty_cells(tmp_path: Path) -> None:
    with pytest.raises(ValueError):
        write_sheet(str(tmp_path / "invalid"), [])
    assert list(tmp_path.iterdir()) == []


@pytest.mark.parametrize("animations", [[], [[]], [[sprite(RED)], []]])
def test_sheet_rejects_empty_animations(
    tmp_path: Path, animations: list[list[IndexedImage]]
) -> None:
    with pytest.raises(ValueError):
        write_animated_sheet(str(tmp_path / "invalid"), animations)
    assert list(tmp_path.iterdir()) == []


@pytest.mark.parametrize("duration", [0, -100, 5, 15])
def test_sheet_rejects_invalid_duration(tmp_path: Path, duration: int) -> None:
    with pytest.raises(ValueError):
        write_animated_sheet(
            str(tmp_path / "invalid"), [[sprite(RED)]], duration=duration
        )
    assert list(tmp_path.iterdir()) == []


@pytest.mark.parametrize(
    "frame_times",
    [
        [[0, 30]],
        [[0, 30], [0, 5]],
        [[0, 30], [0, 15]],
        [[30, 60], [0, 30]],
        [[0, 30, 60], [0, 30]],
    ],
)
def test_sheet_rejects_invalid_frame_times(
    tmp_path: Path, frame_times: list[list[int]]
) -> None:
    path = tmp_path / "invalid.gif"
    with pytest.raises(ValueError):
        write_animated_sheet(
            str(path), [[sprite(RED)], [sprite(BLUE)]], frame_times=frame_times
        )
    assert not path.exists()


@pytest.mark.parametrize("separator", [" | ", " / "])
@pytest.mark.parametrize(
    "following", ["Tail", "VeryLongFollowingFieldWith_Underscores"]
)
def test_separator_does_not_overflow_at_line_break(
    font: ImageFont.FreeTypeFont | ImageFont.ImageFont, separator: str, following: str
) -> None:
    field = "MMMMMMMM"
    width = math.ceil(font.getlength(field))
    lines = _wrap(field + separator + following, font, width, separator)
    assert lines[0] == field
    assert "".join(lines[1:]) == following
    assert all(font.getlength(line) <= width for line in lines)


def test_separator_is_retained_when_it_fits(
    font: ImageFont.FreeTypeFont | ImageFont.ImageFont,
) -> None:
    width = math.ceil(max(font.getlength(text) for text in ("left |", "right")))
    assert _wrap("left | right", font, width) == ["left |", "right"]


@pytest.mark.parametrize(
    "label", ["abcdefghijklmno", "EnemyAnimation_WithLongName", "MMMMMMMMMMMM"]
)
def test_long_fields(
    font: ImageFont.FreeTypeFont | ImageFont.ImageFont, label: str
) -> None:
    lines = _wrap(label, font, 30)
    assert len(lines) > 1
    assert "".join(lines) == label
    assert all(font.getlength(line) <= 30 for line in lines)


def test_empty_captions(font: ImageFont.FreeTypeFont | ImageFont.ImageFont) -> None:
    assert _wrap("", font, 30) == []
