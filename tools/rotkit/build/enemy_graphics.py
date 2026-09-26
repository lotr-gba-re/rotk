"""Enemy PNG sequences and structural metadata -> tile and offset macros.

Edits may redistribute space within an animation's fixed tile block without moving
its frame set or subsequent ROM symbols.
"""

import re
from dataclasses import dataclass
from pathlib import Path
from struct import error as StructError

from rotkit.enemy_animation_metadata import frame_set_from_data, loads
from rotkit.png import read_indexed
from rotkit.spritegfx import FrameSet, encode_sequence_from_images


@dataclass(frozen=True)
class Asset:
    name: str
    tiles_size: int
    frame_set: FrameSet
    palette: tuple[int, ...]
    hidden_nibbles: list[list[list[int]]]
    transparent_owners: list[list[list[int]]]
    paths: tuple[Path, ...]


def load_assets(directory: Path) -> list[Asset]:
    """Read local metadata and require exactly one complete PNG sequence per asset."""
    manifest = directory / "metadata.json"
    if not manifest.is_file():
        raise ValueError(
            f"{manifest}: missing canonical asset manifest; run rotkit carve enemy-graphics"
        )
    metadata = loads(manifest.read_text())
    if not isinstance(metadata, dict) or not metadata:
        raise ValueError(f"{manifest}: expected a nonempty asset map")
    assets = []
    expected = {manifest.name}
    for name, fields in metadata.items():
        if not re.fullmatch(r"[A-Za-z][A-Za-z0-9_]*", name):
            raise ValueError(f"{manifest}: invalid asset name {name!r}")
        required = {"tiles_size", "palette", "frame_set", "hidden_nibbles"}
        if (
            not isinstance(fields, dict)
            or not required <= fields.keys()
            or fields.keys() - required - {"transparent_owners"}
        ):
            raise ValueError(f"{manifest}: {name}: expected palette and frame metadata")
        tiles_size = fields["tiles_size"]
        palette = fields["palette"]
        if (
            not isinstance(palette, list)
            or len(palette) != 16
            or any(
                type(color) is not int or not 0 <= color <= 0xFFFF for color in palette
            )
        ):
            raise ValueError(
                f"{manifest}: {name}: expected sixteen BGR555 palette entries"
            )
        if type(tiles_size) is not int or tiles_size <= 0 or tiles_size % 4:
            raise ValueError(f"{manifest}: {name}: invalid tile block size")
        frame_set = frame_set_from_data(fields["frame_set"])
        transparent_owners = fields.get(
            "transparent_owners", [[] for _ in range(frame_set.frame_count)]
        )
        if (
            not frame_set.frames
            or not isinstance(fields["hidden_nibbles"], list)
            or len(fields["hidden_nibbles"]) != frame_set.frame_count
            or not isinstance(transparent_owners, list)
            or len(transparent_owners) != frame_set.frame_count
        ):
            raise ValueError(f"{manifest}: {name}: invalid frame metadata")
        offsets = [frame.tile_offset for frame in frame_set.frames] + [tiles_size]
        if offsets[0] != 0 or any(
            a >= b or a % 4 for a, b in zip(offsets, offsets[1:])
        ):
            raise ValueError(
                f"{manifest}: {name}: tile chunks must be aligned and in frame order"
            )
        paths = tuple(
            directory / f"{name}_{index:02d}.png"
            for index in range(frame_set.frame_count)
        )
        expected.update(path.name for path in paths)
        assets.append(
            Asset(
                name,
                tiles_size,
                frame_set,
                tuple(palette),
                fields["hidden_nibbles"],
                transparent_owners,
                paths,
            )
        )
    # Obsolete .bin templates must not silently become build inputs again.
    actual = {
        path.name
        for path in directory.iterdir()
        if path.suffix in (".png", ".bin", ".json")
    }
    if actual != expected:
        missing = sorted(expected - actual)
        extra = sorted(actual - expected)
        raise ValueError(
            f"{directory}: incomplete or unexpected assets (missing {missing}, extra {extra})"
        )
    return assets


def encode_category(directory: Path) -> tuple[str, int]:
    """Encode complete sequences, without reading the original ROM or mutable global state."""
    try:
        assets = load_assets(directory)
        lines = []
        count = 0
        for asset in assets:
            images = [read_indexed(str(path)) for path in asset.paths]
            try:
                # TODO: Replace occluded-nibble patches with a natural editable sprite representation.
                tiles, offsets = encode_sequence_from_images(
                    asset.tiles_size,
                    asset.frame_set.frames,
                    images,
                    asset.palette,
                    asset.hidden_nibbles,
                    asset.transparent_owners,
                )
            except ValueError as error:
                raise ValueError(f"{asset.name}: {error}") from error
            prefix = f"EnemyAnimation_{asset.name}"
            lines.extend(
                f"#define {prefix}_FRAME{index}_OFFSET {offset}\n"
                for index, offset in enumerate(offsets)
            )
            rows = [
                ", ".join(f"0x{value:02x}" for value in tiles[index : index + 16])
                for index in range(0, len(tiles), 16)
            ]
            lines.append(
                f"#define {prefix}_TILES \\\n    " + ", \\\n    ".join(rows) + "\n"
            )
            count += len(images)
        return "".join(lines), count
    except (ValueError, OSError, IndexError, StructError) as error:
        raise SystemExit(f"enemy graphics: {directory}: {error}") from error
