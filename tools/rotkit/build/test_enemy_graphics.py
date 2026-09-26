import os
import re
from pathlib import Path
from struct import pack

import pytest

from rotkit.build import enemy_asset_sources, enemy_graphics as encoder
from rotkit.build import gfx
from rotkit.carve import enemy_graphics as carver
from rotkit.enemy_animation_metadata import (
    asset_metadata,
    dumps,
    frame_set_from_data,
    loads,
)
from rotkit.png import IndexedImage, bgr555_to_rgb, write_indexed
from rotkit.rom import ROMBASE
from rotkit.spritegfx import (
    Frame,
    FrameSet,
    OamTemplate,
    SpriteBounds,
    encode_sequence_from_images,
    parse_frame_set,
    render_frame,
)

PALETTE = tuple(range(16))
NAME = "EnemyType0_Walk_Facing0"


def write_category(directory: Path) -> None:
    directory.mkdir(parents=True)
    header = pack("<BB4bHHBB", 8, 8, 0, 0, 0, 0, 2, 32, 0, 0)
    records = b"".join(
        pack("<BBBBHHI", 1, 0, 8, 8, index * 32, 32, 0) for index in range(2)
    )
    template = (
        bytes([0x11]) * 32 + bytes([0x22]) * 32 + header + pack("<2H", 4, 16) + records
    )
    frame_set = parse_frame_set(template, ROMBASE + 64)
    (directory / "metadata.json").write_text(
        dumps({NAME: asset_metadata(template[:64], frame_set, PALETTE)})
    )
    palette = [bgr555_to_rgb(c) for c in PALETTE]
    for index in range(2):
        write_indexed(
            str(directory / f"{NAME}_{index:02d}.png"),
            IndexedImage(8, 8, bytes([index + 1]) * 64, palette, 0),
        )


def test_hidden_pixels_round_trip_without_rom_bytes() -> None:
    frame = Frame(1, 0, 8, 8, 0, 64, (), (), (OamTemplate(0, 0, 0, 0, 0),))
    frame_set = FrameSet(
        8, 8, SpriteBounds(0, 0, 0, 0), 1, 64, 0, 0, (4,), (frame,), 26
    )
    tiles = bytes([0x11]) * 32 + bytes([0x22]) * 32
    metadata = asset_metadata(tiles, frame_set, PALETTE)
    assert frame_set_from_data(metadata["frame_set"]) == frame_set
    assert len(metadata["hidden_nibbles"][0]) == 64
    assert (
        loads(dumps({NAME: metadata}))[NAME]["hidden_nibbles"]
        == metadata["hidden_nibbles"]
    )
    image = IndexedImage(8, 8, bytes([1]) * 64, [bgr555_to_rgb(c) for c in PALETTE], 0)
    assert encode_sequence_from_images(
        64, (frame,), [image], PALETTE, metadata["hidden_nibbles"]
    ) == (tiles, (0,))


def test_transparent_overlap_metadata_encodes_without_rom(tmp_path: Path) -> None:
    directory = tmp_path / "overlap"
    directory.mkdir()
    frame = Frame(
        2,
        0,
        9,
        8,
        0,
        64,
        (),
        (),
        (OamTemplate(0, 0, 0, 0, 0), OamTemplate(1, 0, 0, 0, 1)),
    )
    frame_set = FrameSet(
        9, 8, SpriteBounds(0, 0, 0, 0), 1, 64, 0, 0, (2,), (frame,), 30
    )
    tiles = bytes([0x21]) * 32 + bytes(32)
    metadata = asset_metadata(tiles, frame_set, PALETTE)
    assert metadata["transparent_owners"][0]
    (directory / "metadata.json").write_text(dumps({NAME: metadata}))
    image = render_frame(frame, tiles, (0, 0, 9, 8), PALETTE)
    write_indexed(str(directory / f"{NAME}_00.png"), image)
    assets = encoder.load_assets(directory)
    assert assets[0].transparent_owners == metadata["transparent_owners"]
    text, count = encoder.encode_category(directory)
    assert count == 1
    assert f"#define EnemyAnimation_{NAME}_TILES" in text
    assert (
        bytes(int(value, 16) for value in re.findall(r"0x[0-9a-f]{2}", text)) == tiles
    )


@pytest.fixture
def no_rom_access(monkeypatch: pytest.MonkeyPatch) -> None:
    def fail_load_rom() -> bytes:
        raise AssertionError("ROM accessed")

    monkeypatch.setattr("rotkit.rom.load_rom", fail_load_rom)


@pytest.mark.usefixtures("no_rom_access")
def test_category_encodes_without_rom(tmp_path: Path) -> None:
    directory = tmp_path / "category"
    write_category(directory)
    text, count = encoder.encode_category(directory)
    assert count == 2
    assert f"#define EnemyAnimation_{NAME}_FRAME1_OFFSET 32" in text
    assert "_FRAMES" not in text


@pytest.mark.usefixtures("no_rom_access")
def test_edited_png_encodes_without_original_bytes(tmp_path: Path) -> None:
    directory = tmp_path / "category"
    write_category(directory)
    palette = [bgr555_to_rgb(color) for color in PALETTE]
    write_indexed(
        str(directory / f"{NAME}_00.png"),
        IndexedImage(8, 8, bytes([3]) * 64, palette, 0),
    )
    text, count = encoder.encode_category(directory)
    assert count == 2
    assert f"#define EnemyAnimation_{NAME}_FRAME1_OFFSET 32" in text
    assert "0x33, 0x33" in text


@pytest.mark.parametrize("mutation", ["missing", "extra"])
def test_missing_or_extra_frame_rejected(tmp_path: Path, mutation: str) -> None:
    directory = tmp_path / "category"
    write_category(directory)
    first = directory / f"{NAME}_00.png"
    if mutation == "missing":
        first.unlink()
    else:
        (directory / "alias_00.png").write_bytes(first.read_bytes())
    with pytest.raises(SystemExit, match="incomplete or unexpected"):
        encoder.encode_category(directory)


@pytest.mark.usefixtures("no_rom_access")
def test_incremental_build_tracks_metadata_and_deletions(
    tmp_path: Path, monkeypatch: pytest.MonkeyPatch
) -> None:
    local, build, carved = tmp_path / "local", tmp_path / "build", tmp_path / "carved"
    directory = local / "enemy" / "category"
    write_category(directory)
    sources = carved / "enemy_graphics"
    sources.mkdir(parents=True)
    (sources / "Test.c").write_text('#include "gfx/enemy/category.inc"\n')
    monkeypatch.setattr(gfx, "GFX_LOCAL", local)
    monkeypatch.setattr(gfx, "BUILD", build)
    monkeypatch.setattr(gfx, "CARVED_DATA", carved)
    monkeypatch.setattr(gfx, "ROM", tmp_path / "absent.gba")
    monkeypatch.setattr(
        gfx,
        "read_split",
        lambda: {
            "build/generated/data/enemy_graphics/EnemyAnimationAssets_category.c": 0
        },
    )
    monkeypatch.setattr(gfx, "CARVERS", {})
    monkeypatch.setattr(gfx, "CATEGORY_ENCODERS", {carver: encoder.encode_category})
    gfx.run()
    inc = build / "gfx/enemy/category.inc"
    before = inc.read_bytes(), inc.stat().st_mtime_ns
    gfx.run()
    assert (inc.read_bytes(), inc.stat().st_mtime_ns) == before
    manifest = directory / "metadata.json"
    stat = manifest.stat()
    os.utime(manifest, ns=(stat.st_atime_ns, stat.st_mtime_ns + 1_000_000))
    assert gfx._stale(directory, gfx._tool_mtime())
    gfx.run()
    assert not gfx._stale(directory, gfx._tool_mtime())
    (directory / f"{NAME}_00.png").unlink()
    assert gfx._stale(directory, gfx._tool_mtime())
    with pytest.raises(SystemExit, match="missing"):
        gfx.run()
    assert inc.read_bytes() == before[0]


def test_unchanged_generated_source_advances_timestamp(
    tmp_path: Path, monkeypatch: pytest.MonkeyPatch
) -> None:
    source = enemy_asset_sources.source_for_group("category")
    monkeypatch.setattr(enemy_asset_sources, "ROOT", tmp_path)
    monkeypatch.setattr(enemy_asset_sources, "generate", lambda source: "unchanged\n")
    enemy_asset_sources.run(source)
    target = tmp_path / source
    os.utime(target, ns=(1_000_000_000, 1_000_000_000))
    enemy_asset_sources.run(source)
    assert target.read_text() == "unchanged\n"
    assert target.stat().st_mtime_ns > 1_000_000_000


def test_placeholder_regex_excludes_engine_flags() -> None:
    assert gfx._MACRO_REF.findall("BG_ASSET_TILES | BG_ASSET_MAP") == []
    assert gfx._MACRO_REF.findall(f"EnemyAnimation_{NAME}_FRAME2_OFFSET") == [
        (f"EnemyAnimation_{NAME}", "FRAME2_OFFSET")
    ]


def test_placeholder_generation_uses_only_asset_macros(
    tmp_path: Path, monkeypatch: pytest.MonkeyPatch
) -> None:
    sources = tmp_path / "carved/enemy_graphics"
    sources.mkdir(parents=True)
    (sources / "Test.c").write_text(
        '#include "gfx/enemy/category.inc"\n'
        f"const u8 Tiles[] = {{EnemyAnimation_{NAME}_TILES}};\n"
        f"const u16 Offset = EnemyAnimation_{NAME}_FRAME0_OFFSET;\n"
        "BG_ASSET_TILES_ONLY(SkillIcon, BG_ASSET_TILES | BG_ASSET_MAP);\n"
    )
    monkeypatch.setattr(gfx, "BUILD", tmp_path / "build")
    monkeypatch.setattr(gfx, "CARVED_DATA", tmp_path / "carved")
    gfx._placeholders(carver)
    text = (tmp_path / "build/gfx/enemy/category.inc").read_text()
    assert "#define BG_ASSET" not in text
    assert f"#define EnemyAnimation_{NAME}_TILES 0" in text
    assert "#define SkillIcon_TILES_SIZE 4" in text
