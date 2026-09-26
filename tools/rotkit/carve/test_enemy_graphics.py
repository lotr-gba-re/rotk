from dataclasses import replace
from pathlib import Path
from struct import pack_into

from PIL import Image
import pytest

from rotkit.carve import enemy_graphics as carver
from rotkit import enemy_animation_metadata as animation_metadata
from rotkit.enemy_animation_metadata import (
    _asset_metadata_and_images,
    asset_metadata,
    dumps,
    loads,
)
from rotkit.png import IndexedImage, read_indexed
from rotkit.rom import ROMBASE
from rotkit.spritegfx import (
    Frame,
    FrameSet,
    OamTemplate,
    SpriteBounds,
    encode_sequence_from_images,
)

PALETTE = tuple(range(16))
NAME = "EnemyType0_Walk_Facing0"


def _frame_set(frames: tuple[Frame, ...]) -> FrameSet:
    return FrameSet(
        8,
        8,
        SpriteBounds(0, 0, 0, 0),
        len(frames),
        32,
        0,
        0,
        tuple(4 + index * 12 for index in range(len(frames))),
        frames,
        12 + len(frames) * 14,
    )


def _frame(
    offset: int,
    objects: tuple[OamTemplate, ...] = (OamTemplate(0, 0, 0, 0, 0),),
) -> Frame:
    return Frame(len(objects), 0, 8, 8, offset, 32, (), (), objects)


def test_preview_frame_cache_shares_aliases_but_not_palettes(
    monkeypatch: pytest.MonkeyPatch,
) -> None:
    rom = bytearray(carver.TABLE_ADDR - ROMBASE + 2 * carver._ANIMATION_ROW.size)
    bank_a, bank_b, bank_c = ROMBASE + 0x100, ROMBASE + 0x200, ROMBASE + 0x300
    for enemy_type in range(2):
        pack_into(
            "<3I",
            rom,
            carver.TABLE_ADDR - ROMBASE + enemy_type * 24,
            bank_a,
            bank_b,
            bank_c,
        )
    frame_set = _frame_set((_frame(0),))
    record_a = carver.AnimationRecord(bank_a, ROMBASE, ROMBASE + 32, 0, 5, frame_set)
    record_b = replace(record_a, animation_addr=bank_b)
    record_c = replace(record_a, animation_addr=bank_c, palette_addr=ROMBASE + 64)
    records = {bank_a: record_a, bank_b: record_b, bank_c: record_c}
    monkeypatch.setattr(
        carver,
        "_palette",
        lambda rom, enemy_type: PALETTE if enemy_type == 0 else PALETTE[::-1],
    )
    original_decode = carver.decode_frame_tiles
    decoded = []

    def count_decode(tiles, frame):
        decoded.append(frame)
        return original_decode(tiles, frame)

    monkeypatch.setattr(carver, "decode_frame_tiles", count_decode)
    rom[:32] = b"\x11" * 32
    pack_into("<16H", rom, 64, *PALETTE)
    cache = {}
    first = carver._enemy_animation_frames(rom, records, 0, 0, 0, cache)
    alias = carver._enemy_animation_frames(rom, records, 0, 1, 0, cache)
    other_palette = carver._enemy_animation_frames(rom, records, 1, 0, 0, cache)
    explicit_palette = carver._enemy_animation_frames(rom, records, 1, 2, 0, cache)
    assert first is alias is explicit_palette
    assert other_palette is not first
    assert first[0].pixels == other_palette[0].pixels == b"\x01" * 64
    assert first[0].palette != other_palette[0].palette
    assert len(decoded) == 2
    assert carver._enemy_animation_frames(rom, records, 0, 1, 0) == first


@pytest.mark.parametrize(
    ("frame_duration", "gif_duration", "frame_count"),
    [(0, 10, 1), (2, 70, 2)],
)
def test_animation_preview_uses_frame_countdown(
    tmp_path: Path,
    monkeypatch: pytest.MonkeyPatch,
    frame_duration: int,
    gif_duration: int,
    frame_count: int,
) -> None:
    bank = ROMBASE + 0x100
    rom = bytearray(carver.TABLE_ADDR - ROMBASE + carver._ANIMATION_ROW.size)
    pack_into("<I", rom, carver.TABLE_ADDR - ROMBASE, bank)
    frame_set = _frame_set((_frame(0), _frame(32)))
    record = carver.AnimationRecord(
        bank, ROMBASE, ROMBASE + 64, 0, frame_duration, frame_set
    )
    carve = carver.Carve({bank: record}, {}, {}, {}, {}, [])
    palette = [(0, 0, 0), (255, 255, 255)]
    frames = (
        IndexedImage(1, 1, b"\x00", palette, None),
        IndexedImage(1, 1, b"\x01", palette, None),
    )
    monkeypatch.setattr(carver, "ENEMY_TYPE_COUNT", 1)
    monkeypatch.setattr(carver, "FACING_VARIANT_COUNT", 1)
    monkeypatch.setattr(carver, "GFX_SHEETS", tmp_path)
    monkeypatch.setattr(carver, "_pose_names", lambda: ("WALK",))
    monkeypatch.setattr(carver, "_enemy_type_symbol_name", lambda enemy_type: "Test")
    monkeypatch.setattr(carver, "_enemy_type_source_name", lambda name: name)
    monkeypatch.setattr(carver, "_enemy_animation_frames", lambda *args: frames)
    assert carver._write_animation_previews(rom, carve) == 1
    path = tmp_path / carver.GROUP / "animations/Test/Test_Walk_Facing0.gif"
    with Image.open(path) as preview:
        assert preview.n_frames == frame_count
        if frame_count > 1:
            assert preview.info["duration"] == gif_duration


def test_extract_reuses_metadata_images_and_checks_rom_round_trip(
    tmp_path: Path,
    monkeypatch: pytest.MonkeyPatch,
) -> None:
    frames = (_frame(0), _frame(32))
    tile_stream = b"\x11" * 32 + b"\x22" * 32
    frame_set = _frame_set(frames)
    key = (ROMBASE, ROMBASE + len(tile_stream))
    asset = carver.AnimationAsset(*key, frame_set)
    carve = carver.Carve({}, {key: asset}, {}, {key: NAME}, {key: PALETTE}, [[asset]])
    monkeypatch.setattr(carver, "load", lambda rom: carve)
    monkeypatch.setattr(carver, "_asset_group_name", lambda group, names: "sample")
    monkeypatch.setattr(carver, "_write_animation_previews", lambda rom, data: 0)
    monkeypatch.setattr(carver, "_write_reference_sheets", lambda *args: None)
    monkeypatch.setattr(carver, "GFX_DIR", tmp_path / "enemy")
    monkeypatch.setattr(carver, "GFX_SHEETS", tmp_path / "sheets")
    real_encoder = carver.encode_sequence_from_images
    calls = []

    def check_encoder(*args):
        result = real_encoder(*args)
        calls.append(result)
        return result

    monkeypatch.setattr(carver, "encode_sequence_from_images", check_encoder)
    assert carver.extract(tile_stream + bytes(frame_set.size), False) is carve
    directory = tmp_path / "enemy/sample"
    metadata = loads((directory / "metadata.json").read_text())[NAME]
    expected = asset_metadata(tile_stream, frame_set, PALETTE)
    assert metadata == loads(dumps({NAME: expected}))[NAME]
    assert [
        read_indexed(str(directory / f"{NAME}_{index:02d}.png")).pixels
        for index in range(2)
    ] == [b"\x01" * 64, b"\x02" * 64]
    assert calls == [(tile_stream, (0, 32))]

    def bad_encoder(*args):
        data, offsets = real_encoder(*args)
        return b"\x00" + data[1:], offsets

    monkeypatch.setattr(carver, "encode_sequence_from_images", bad_encoder)
    with pytest.raises(SystemExit, match="PNGs and metadata do not reproduce"):
        carver.extract(tile_stream + bytes(frame_set.size), False)


def test_occluded_tile_nibbles_survive_extraction() -> None:
    objects = (OamTemplate(0, 0, 0, 0, 0), OamTemplate(1, 0, 0, 0, 1))
    frame = replace(_frame(0, objects), tile_bytes=64)
    tile_stream = b"\x21" * 32 + b"\x00" * 32
    metadata, images = _asset_metadata_and_images(
        tile_stream, _frame_set((frame,)), PALETTE
    )
    assert metadata["hidden_nibbles"][0]
    assert metadata["transparent_owners"][0]
    assert images[0].pixels[:9] == b"\x01\x02" * 4 + b"\x00"
    assert encode_sequence_from_images(
        64,
        (frame,),
        images,
        PALETTE,
        metadata["hidden_nibbles"],
        metadata["transparent_owners"],
    ) == (tile_stream, (0,))


def test_shared_tiles_still_reject_conflicting_pixels(
    monkeypatch: pytest.MonkeyPatch,
) -> None:
    objects = (OamTemplate(0, 0, 0, 0, 0), OamTemplate(8, 0, 0, 0, 0))
    frame = _frame(0, objects)
    stream = b"\x11" * 32
    frame_set = _frame_set((frame,))
    metadata, images = _asset_metadata_and_images(stream, frame_set, PALETTE)
    assert asset_metadata(stream, frame_set, PALETTE) == metadata
    assert encode_sequence_from_images(
        32, (frame,), images, PALETTE, metadata["hidden_nibbles"]
    ) == (stream, (0,))
    conflict = replace(images[0], pixels=b"\x02" + images[0].pixels[1:])
    with pytest.raises(ValueError, match="conflict"):
        encode_sequence_from_images(
            32, (frame,), [conflict], PALETTE, metadata["hidden_nibbles"]
        )

    original = animation_metadata.render_frame

    def conflicting_image(*args):
        image = original(*args)
        return replace(image, pixels=b"\x02" + image.pixels[1:])

    monkeypatch.setattr(animation_metadata, "render_frame", conflicting_image)
    with pytest.raises(ValueError, match="conflict"):
        _asset_metadata_and_images(stream, frame_set, PALETTE)
