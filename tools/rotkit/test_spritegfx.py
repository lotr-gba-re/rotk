from dataclasses import replace
from struct import pack

import pytest

from rotkit.compression import bios, lz77
from rotkit.png import IndexedImage, bgr555_to_rgb
from rotkit.spritegfx import (
    SPRITE_FRAME_LZ77,
    SPRITE_FRAME_RL,
    Frame,
    OamTemplate,
    decode_frame_tiles,
    encode_frame_tiles,
    encode_sequence,
    frame_bounds,
    rebuild_frame_tiles,
    remap_sprite_palette,
    render_frame,
    transparent_overlap_owners,
    visible_nibbles,
)


PALETTE = tuple(range(16))


def make_frame(
    objects: tuple[OamTemplate, ...] = (OamTemplate(0, 0, 0, 0, 0),),
    tile_bytes: int = 32,
    flags: int = 0,
    tile_offset: int = 0,
) -> Frame:
    return Frame(
        flags | len(objects), 0, 8, 8, tile_offset, tile_bytes, (), (), objects
    )


def make_image(width: int, height: int, pixels: bytes) -> IndexedImage:
    return IndexedImage(
        width, height, pixels, [bgr555_to_rgb(color) for color in PALETTE], 0
    )


def test_bounds_union_and_origin_shift() -> None:
    first = make_frame((OamTemplate(-5, -7, 0, 0, 0),))
    second = make_frame((OamTemplate(4, 3, 0, 0, 0),))
    bounds = frame_bounds((first, second))
    assert bounds == (-5, -7, 12, 11)
    tiles = b"\x21" * 32
    image = render_frame(first, tiles, bounds, PALETTE)
    assert (image.width, image.height) == (17, 18)
    assert image.pixels[:17] == b"\x01\x02" * 4 + bytes(9)
    assert image.pixels[8 * 17 :] == bytes(10 * 17)
    assert image.palette == [bgr555_to_rgb(c) for c in PALETTE]
    assert image.transparent == 0
    assert rebuild_frame_tiles(tiles, first, image, bounds) == tiles


def test_empty_frame_fallback() -> None:
    frame = make_frame((), 0)
    assert frame_bounds(()) == (0, 0, 1, 1)
    assert frame_bounds((frame,)) == (0, 0, 1, 1)
    image = render_frame(frame, b"", frame_bounds((frame,)), PALETTE)
    assert image.pixels == b"\x00"
    assert rebuild_frame_tiles(b"", frame, image, (0, 0, 1, 1)) == b""
    with pytest.raises(ValueError, match="outside frame OAM"):
        rebuild_frame_tiles(b"", frame, replace(image, pixels=b"\x01"), (0, 0, 1, 1))


def test_paint_outside_oam_rejected() -> None:
    frame = make_frame()
    bounds = (-1, -1, 9, 9)
    image = render_frame(frame, bytes(32), bounds, PALETTE)
    edited = replace(image, pixels=b"\x01" + image.pixels[1:])
    with pytest.raises(ValueError, match="outside frame OAM"):
        rebuild_frame_tiles(bytes(32), frame, edited, bounds)


def test_tile_order_and_unused_bytes() -> None:
    frame = make_frame((OamTemplate(0, 0, 1, 0, 1),), 192)
    original = b"".join(bytes([index * 17]) * 32 for index in range(6))
    bounds = frame_bounds((frame,))
    image = render_frame(frame, original, bounds, PALETTE)
    assert image.pixels[:16] == bytes([1]) * 8 + bytes([2]) * 8
    assert image.pixels[128:144] == bytes([3]) * 8 + bytes([4]) * 8
    image = replace(image, pixels=b"\x0f" + image.pixels[1:])
    rebuilt = rebuild_frame_tiles(original, frame, image, bounds)
    assert rebuilt == original[:32] + b"\x1f" + original[33:]


def test_overlap_preserves_hidden_nibbles() -> None:
    frame = make_frame((OamTemplate(0, 0, 0, 0, 0), OamTemplate(1, 0, 0, 0, 1)), 64)
    original = b"\x21" * 32 + b"\x00" * 32
    bounds = frame_bounds((frame,))
    image = render_frame(frame, original, bounds, PALETTE)
    assert image.pixels == (b"\x01\x02" * 4 + b"\x00") * 8
    hints = transparent_overlap_owners(frame, original, bounds)
    assert hints
    assert rebuild_frame_tiles(original, frame, image, bounds, hints) == original
    image = replace(image, pixels=b"\x03\x04" + image.pixels[2:])
    rebuilt = rebuild_frame_tiles(original, frame, image, bounds, hints)
    assert rebuilt == b"\x43" + original[1:]
    assert render_frame(frame, rebuilt, bounds, PALETTE).pixels == image.pixels


def test_clearing_overlapping_pixel_clears_rear_oam() -> None:
    frame = make_frame((OamTemplate(0, 0, 0, 0, 0), OamTemplate(0, 0, 0, 0, 1)), 64)
    original = bytes([0xCC]) * 64
    bounds = frame_bounds((frame,))
    image = render_frame(frame, original, bounds, PALETTE)
    assert image.pixels[0] == 12
    edited = replace(image, pixels=b"\x00" + image.pixels[1:])
    rebuilt = rebuild_frame_tiles(original, frame, edited, bounds)
    assert rebuilt[0] & 15 == rebuilt[32] & 15 == 0
    assert render_frame(frame, rebuilt, bounds, PALETTE).pixels == edited.pixels


def test_visible_nibbles_obey_last_layer_and_tile_order() -> None:
    frame = make_frame((OamTemplate(0, 0, 1, 0, 1), OamTemplate(8, 0, 0, 0, 5)), 192)
    visible = visible_nibbles(frame, frame_bounds((frame,)))
    assert visible == set(range(64, 128)) | set(range(192, 384))


def test_shared_tiles_allow_consistent_edits() -> None:
    frame = make_frame((OamTemplate(0, 0, 0, 0, 0), OamTemplate(8, 0, 0, 0, 0)))
    bounds = frame_bounds((frame,))
    image = make_image(16, 8, bytes([3]) * 128)
    assert rebuild_frame_tiles(bytes(32), frame, image, bounds) == b"\x33" * 32


def test_shared_tiles_reject_conflicting_edits() -> None:
    frame = make_frame((OamTemplate(0, 0, 0, 0, 0), OamTemplate(8, 0, 0, 0, 0)))
    image = make_image(16, 8, (bytes([1]) * 8 + bytes([2]) * 8) * 8)
    with pytest.raises(ValueError, match="conflict"):
        rebuild_frame_tiles(bytes(32), frame, image, frame_bounds((frame,)))


def test_partially_shared_tiles_reject_conflicting_edits() -> None:
    frame = make_frame((OamTemplate(0, 0, 1, 0, 0), OamTemplate(16, 0, 0, 0, 3)), 128)
    image = make_image(24, 16, b"\x00" * 16 + b"\x04" + bytes(24 * 16 - 17))
    with pytest.raises(ValueError, match="conflict"):
        rebuild_frame_tiles(bytes(128), frame, image, frame_bounds((frame,)))


def test_overlapping_shared_tiles_reject_conflicting_edits() -> None:
    frame = make_frame((OamTemplate(0, 0, 0, 0, 0), OamTemplate(4, 0, 0, 0, 0)))
    original = b"\x21" * 32
    bounds = frame_bounds((frame,))
    image = render_frame(frame, original, bounds, PALETTE)
    assert rebuild_frame_tiles(original, frame, image, bounds) == original
    with pytest.raises(ValueError, match="conflict"):
        rebuild_frame_tiles(
            original,
            frame,
            replace(image, pixels=b"\x03" + image.pixels[1:]),
            bounds,
        )


@pytest.mark.parametrize(
    ("objects", "edits"),
    [
        (
            (OamTemplate(-2, 0, 1, 0, 2), OamTemplate(6, 4, 0, 0, 8)),
            ((0, 0, 3), (8, 4, 4)),
        ),
        (
            (OamTemplate(0, 0, 0, 0, 0), OamTemplate(8, 0, 0, 0, 0)),
            ((0, 0, 3), (8, 0, 3)),
        ),
        (
            (OamTemplate(0, 0, 0, 0, 0), OamTemplate(8, 0, 0, 0, 0)),
            ((0, 0, 3),),
        ),
        (
            (OamTemplate(0, 0, 0, 0, 0), OamTemplate(16, 0, 0, 0, 1)),
            ((8, 0, 4),),
        ),
    ],
    ids=["shifted-overlap", "shared-consistent", "shared-conflict", "outside-oam"],
)
def test_rebuild_matches_pixelwise_reference_for_varied_oam(
    objects: tuple[OamTemplate, ...], edits: tuple[tuple[int, int, int], ...]
) -> None:
    frame = make_frame(objects, 64 * 32)
    bounds = frame_bounds((frame,))
    width, height = bounds[2] - bounds[0], bounds[3] - bounds[1]
    original = bytes(index % 256 for index in range(frame.tile_bytes))
    layers = [[] for _ in range(width * height)]
    for obj in objects:
        ow, oh = obj.size
        for y in range(oh):
            for x in range(ow):
                tile = obj.tile_offset + (y // 8) * (ow // 8) + x // 8
                nibble = tile * 64 + (y % 8) * 8 + x % 8
                pixel = (obj.y - bounds[1] + y) * width + obj.x - bounds[0] + x
                layers[pixel].append(nibble)
    assert visible_nibbles(frame, bounds) == {
        nibbles[-1] for nibbles in layers if nibbles
    }

    def reference_pixels(tiles: bytes) -> bytes:
        values = []
        for nibbles in layers:
            value = 0
            for nibble in nibbles:
                candidate = (tiles[nibble // 2] >> (4 * (nibble % 2))) & 15
                if candidate:
                    value = candidate
            values.append(value)
        return bytes(values)

    image = make_image(width, height, reference_pixels(original))
    assert render_frame(frame, original, bounds, PALETTE).pixels == image.pixels
    pixels = bytearray(image.pixels)
    for x, y, value in edits:
        pixels[y * width + x] = value
    image = replace(image, pixels=bytes(pixels))
    expected = bytearray(original)
    hints = dict(transparent_overlap_owners(frame, original, bounds))
    error = None
    try:
        for pixel, nibbles in enumerate(layers):
            value = image.pixels[pixel]
            if not nibbles:
                if value:
                    raise ValueError("painted pixel lies outside frame OAM")
                continue
            targets = nibbles if not value else (hints.get(pixel, nibbles[-1]),)
            for nibble in targets:
                offset, half = divmod(nibble, 2)
                shift = half * 4
                expected[offset] = (expected[offset] & ~(15 << shift)) | (
                    value << shift
                )
        if reference_pixels(expected) != image.pixels:
            raise ValueError(
                "sprite edits conflict through overlapping or shared tiles"
            )
    except ValueError as exc:
        error = str(exc)
    if error is None:
        assert (
            rebuild_frame_tiles(
                original, frame, image, bounds, [list(x) for x in hints.items()]
            )
            == expected
        )
    else:
        with pytest.raises(ValueError, match=error):
            rebuild_frame_tiles(
                original, frame, image, bounds, [list(x) for x in hints.items()]
            )


@pytest.mark.parametrize(
    "image",
    [
        make_image(9, 8, bytes(64)),
        make_image(8, 9, bytes(64)),
        make_image(8, 8, bytes(63)),
        make_image(8, 8, b"\x10" + bytes(63)),
    ],
    ids=["width", "height", "pixel-count", "pixel-index"],
)
def test_invalid_dimensions_and_pixels(image: IndexedImage) -> None:
    with pytest.raises(ValueError):
        rebuild_frame_tiles(bytes(32), make_frame(), image, (0, 0, 8, 8))


@pytest.mark.parametrize(
    ("tiles", "bounds"),
    [
        (bytes(31), (0, 0, 8, 8)),
        (bytes(32), (0, 0, 7, 8)),
        (bytes(32), (0, 0, 0, 0)),
    ],
    ids=["short-tiles", "small-canvas", "empty-canvas"],
)
def test_oam_outside_tiles_or_canvas_rejected(
    tiles: bytes, bounds: tuple[int, int, int, int]
) -> None:
    with pytest.raises(ValueError):
        render_frame(make_frame(), tiles, bounds, PALETTE)


@pytest.mark.parametrize(
    ("frame", "tiles", "bounds", "message"),
    [
        (make_frame(), bytes(31), (0, 0, 8, 8), "outside its tile data"),
        (make_frame(), bytes(32), (0, 0, 7, 8), "outside its canvas bounds"),
        (make_frame(), bytes(32), (0, 0, 0, 8), "positive dimensions"),
        (
            make_frame((OamTemplate(0, 0, 0, 0, -1),)),
            bytes(32),
            (0, 0, 8, 8),
            "outside its tile data",
        ),
    ],
)
def test_reconstruction_rejects_invalid_oam_before_image(
    frame: Frame, tiles: bytes, bounds: tuple[int, int, int, int], message: str
) -> None:
    with pytest.raises(ValueError, match=message):
        rebuild_frame_tiles(tiles, frame, make_image(1, 1, b""), bounds)
    with pytest.raises(ValueError, match=message):
        visible_nibbles(replace(frame, tile_bytes=len(tiles)), bounds)


def test_reordered_palette() -> None:
    image = make_image(3, 1, b"\x00\x01\x02")
    image.palette[1], image.palette[2] = image.palette[2], image.palette[1]
    normalized = remap_sprite_palette(image, PALETTE)
    assert normalized.pixels == b"\x00\x02\x01"
    assert normalized.palette == make_image(1, 1, b"\x00").palette


def test_duplicate_colors_preserve_matching_index() -> None:
    palette = (0, 1, 1, 0)
    image = IndexedImage(
        4, 1, bytes(range(4)), [bgr555_to_rgb(c) for c in palette], None
    )
    assert remap_sprite_palette(image, palette) == image


def test_bit15_ignored() -> None:
    image = make_image(2, 1, b"\x01\x02")
    image.palette[1] = bgr555_to_rgb(PALETTE[1] | 0x8000)
    assert remap_sprite_palette(image, PALETTE).pixels == image.pixels
    assert (
        remap_sprite_palette(image, tuple(c | 0x8000 for c in PALETTE)).pixels
        == image.pixels
    )


def test_unknown_used_color_rejected_unused_color_allowed() -> None:
    image = make_image(1, 1, b"\x01")
    image.palette[2] = (0, 255, 0)
    assert remap_sprite_palette(image, PALETTE).pixels == image.pixels
    image.palette[1] = (0, 255, 0)
    with pytest.raises(ValueError):
        remap_sprite_palette(image, PALETTE)


@pytest.mark.parametrize(("index", "color"), [(0, 1), (1, 0)])
def test_transparency_cannot_remap_to_opaque_or_reverse(index: int, color: int) -> None:
    image = make_image(1, 1, bytes([index]))
    image.palette[index] = bgr555_to_rgb(color)
    with pytest.raises(ValueError, match="transparent index 0"):
        remap_sprite_palette(image, PALETTE)


def test_opaque_color_uses_opaque_duplicate_not_zero() -> None:
    image = make_image(1, 1, b"\x01")
    image.palette[1] = (0, 0, 0)
    assert remap_sprite_palette(image, (0, 1, 0)).pixels == b"\x02"


@pytest.mark.parametrize(
    "image",
    [
        replace(make_image(1, 1, b"\x01"), transparent=1),
        replace(
            make_image(1, 1, b"\x10"),
            palette=make_image(1, 1, b"\x01").palette + [(0, 0, 0)],
        ),
        replace(make_image(1, 1, b"\x01"), palette=[]),
    ],
    ids=["transparency", "pixel-index", "empty-palette"],
)
def test_invalid_transparency_indices_and_palette_references(
    image: IndexedImage,
) -> None:
    with pytest.raises(ValueError):
        remap_sprite_palette(image, PALETTE)


@pytest.mark.parametrize(
    ("flags", "stream"),
    [
        (0, bytes(range(32))),
        (SPRITE_FRAME_LZ77, lz77.encode(bytes(range(32)))),
        (SPRITE_FRAME_RL, b"\x30\x20\x00\x00" + bios.rl_encode(bytes(range(32)))),
        (SPRITE_FRAME_LZ77 | SPRITE_FRAME_RL, lz77.encode(bytes(range(32)))),
    ],
    ids=["raw", "lz77", "rl", "lz77-precedence"],
)
def test_decode_offset_and_codec_precedence(flags: int, stream: bytes) -> None:
    frame = make_frame(flags=flags, tile_offset=6)
    assert decode_frame_tiles(b"prefix" + stream + b"padding", frame) == bytes(
        range(32)
    )


@pytest.mark.parametrize(
    ("flags", "stream"),
    [
        (SPRITE_FRAME_RL, b"\x30\x20\x00\x00\xff\x33"),
        (SPRITE_FRAME_LZ77, lz77.encode(b"\x33" * 130)),
    ],
    ids=["rl", "lz77"],
)
def test_overproducing_stream_truncated(flags: int, stream: bytes) -> None:
    assert decode_frame_tiles(stream, make_frame(flags=flags)) == b"\x33" * 32


@pytest.mark.parametrize(
    ("flags", "stream"),
    [
        (0, bytes(31)),
        (SPRITE_FRAME_LZ77, lz77.encode(bytes(31))),
        (SPRITE_FRAME_LZ77, b"\x40"),
        (SPRITE_FRAME_RL, b"\x00\x01"),
        (SPRITE_FRAME_RL, b"\x30\x20\x00\x00\x00\x01"),
        (SPRITE_FRAME_RL, b"\x30\x1f\x00\x00" + bios.rl_encode(bytes(31))),
    ],
    ids=[
        "short-raw",
        "short-lz77",
        "invalid-lz77",
        "invalid-rl",
        "truncated-rl",
        "short-rl",
    ],
)
def test_short_and_invalid_streams_rejected(flags: int, stream: bytes) -> None:
    with pytest.raises(ValueError):
        decode_frame_tiles(stream, make_frame(flags=flags))


def test_negative_tile_offset_rejected() -> None:
    with pytest.raises(ValueError):
        decode_frame_tiles(bytes(32), make_frame(tile_offset=-1))


@pytest.mark.parametrize(
    ("flags", "chunk"),
    [
        (0, b"\x11" * 32 + b"padding!"),
        (SPRITE_FRAME_LZ77, b"\x20" + b"\x11" * 32 + b"\x00\xab\xcd"),
        (SPRITE_FRAME_RL, b"\x30\x20\x00\x00\x1f" + b"\x11" * 32 + b"\xab\xcd\xef"),
        (SPRITE_FRAME_RL, b"\x30\x20\x00\x00\xff\x11\xab\xcd"),
    ],
    ids=["raw", "lz77", "rl-literal", "rl-run"],
)
def test_unchanged_original_chunk_preserved_verbatim(flags: int, chunk: bytes) -> None:
    frame = make_frame(flags=flags, tile_offset=1234)
    assert encode_frame_tiles(chunk, frame, b"\x11" * 32) == chunk


def test_encode_wrong_tile_count_rejected() -> None:
    with pytest.raises(ValueError):
        encode_frame_tiles(bytes(32), make_frame(), bytes(31))


def padded(data: bytes) -> bytes:
    return data + bytes(-len(data) % 4)


def make_sequence(
    chunks: list[bytes], flags: list[int]
) -> tuple[bytes, tuple[Frame, ...], list[IndexedImage]]:
    frames = []
    offset = 0
    for chunk, flag in zip(chunks, flags, strict=True):
        frames.append(make_frame(flags=flag, tile_offset=offset))
        offset += len(chunk)
    data = b"".join(chunks)
    bounds = frame_bounds(tuple(frames))
    images = [
        render_frame(frame, decode_frame_tiles(data, frame), bounds, PALETTE)
        for frame in frames
    ]
    return data, tuple(frames), images


def image_for(tiles: bytes) -> IndexedImage:
    return render_frame(make_frame(), tiles, (0, 0, 8, 8), PALETTE)


def test_sequence_unchanged_chunks_and_offsets_are_exact() -> None:
    original, frames, images = make_sequence(
        [bytes([0x11]) * 32, bytes([0x22]) * 32], [0, 0]
    )
    tiles, offsets = encode_sequence(original, frames, images, PALETTE)
    assert tiles == original
    assert offsets == (0, 32)


def test_sequence_edit_preserves_unrelated_rl_frame() -> None:
    # A valid, intentionally nonminimal literal encoding must stay byte-identical.
    rl = padded(pack("<I", 0x30 | (32 << 8)) + b"\x1f" + bytes([0x22]) * 32)
    original, frames, images = make_sequence(
        [bytes([0x11]) * 32, rl], [0, SPRITE_FRAME_RL]
    )
    images[0] = image_for(bytes([0x33]) * 32)
    tiles, offsets = encode_sequence(original, frames, images, PALETTE)
    assert offsets == (0, 32)
    assert tiles[:32] == bytes([0x33]) * 32
    assert tiles[32:] == rl


def test_sequence_frames_can_redistribute_space() -> None:
    dense, empty = bytes(range(32)), bytes(32)
    chunks = [padded(lz77.encode(dense)), padded(lz77.encode(empty))]
    original, frames, _ = make_sequence(chunks, [SPRITE_FRAME_LZ77] * 2)
    tiles, offsets = encode_sequence(
        original, frames, [image_for(empty), image_for(dense)], PALETTE
    )
    assert len(tiles) == len(original)
    assert offsets == (0, len(chunks[1]))
    assert decode_frame_tiles(tiles, frames[0]) == empty
    assert (
        decode_frame_tiles(tiles, replace(frames[1], tile_offset=offsets[1])) == dense
    )


def test_sequence_total_capacity_overflow_is_rejected() -> None:
    original, frames, _ = make_sequence(
        [padded(lz77.encode(bytes(32)))], [SPRITE_FRAME_LZ77]
    )
    with pytest.raises(ValueError, match="ROM block"):
        encode_sequence(original, frames, [image_for(bytes(range(32)))], PALETTE)


def test_sequence_new_rl_encoding_has_bios_header() -> None:
    chunk = padded(pack("<I", 0x30 | (32 << 8)) + b"\x1f" + bytes(32))
    original, frames, _ = make_sequence([chunk], [SPRITE_FRAME_RL])
    tiles, _ = encode_sequence(
        original, frames, [image_for(bytes([0x11]) * 32)], PALETTE
    )
    assert tiles[:4] == pack("<I", 0x30 | (32 << 8))
    assert bytes(bios.rl_decode(tiles, 4, 32)) == bytes([0x11]) * 32
    assert len(tiles) == len(original)
    assert tiles[6:] == bytes(len(original) - 6)


def test_sequence_palette_reordering_preserves_tiles() -> None:
    original, frames, images = make_sequence([bytes([0x11]) * 32], [0])
    image = images[0]
    palette = list(image.palette)
    palette[1], palette[2] = palette[2], palette[1]
    reordered = replace(image, pixels=bytes([2]) * 64, palette=palette)
    assert encode_sequence(original, frames, [reordered], PALETTE)[0] == original


@pytest.mark.parametrize("image_count", [0, 2])
def test_sequence_wrong_image_count_rejected(image_count: int) -> None:
    original, frames, images = make_sequence([bytes(32)], [0])
    with pytest.raises(ValueError, match=f"expected 1 frames, got {image_count}"):
        encode_sequence(original, frames, images * image_count, PALETTE)


def test_sequence_uses_common_canvas_for_shifted_frames() -> None:
    frames = (
        make_frame((OamTemplate(-5, -7, 0, 0, 0),)),
        make_frame((OamTemplate(4, 3, 0, 0, 0),), tile_offset=32),
    )
    original = bytes([0x11]) * 32 + bytes([0x22]) * 32
    bounds = frame_bounds(frames)
    images = [
        render_frame(frame, decode_frame_tiles(original, frame), bounds, PALETTE)
        for frame in frames
    ]
    assert encode_sequence(original, frames, images, PALETTE) == (original, (0, 32))
    images[1] = replace(images[1], pixels=b"\x01" + images[1].pixels[1:])
    with pytest.raises(
        ValueError, match="frame 1: painted pixel lies outside frame OAM"
    ):
        encode_sequence(original, frames, images, PALETTE)
