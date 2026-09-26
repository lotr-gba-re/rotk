"""JSON-with-line-comments metadata for locally carved enemy animations.

The PNGs hold visible pixels. Frame layouts and a few obscured tile nibbles live here
so the build can reproduce the original stream without keeping ROM byte copies.
"""

import json
from dataclasses import asdict

from rotkit.png import IndexedImage
from rotkit.spritegfx import (
    Frame,
    FrameSet,
    Hotspot,
    OamTemplate,
    SpriteBounds,
    TriggerBox,
    apply_hidden_nibbles,
    decode_frame_tiles,
    frame_bounds,
    rebuild_frame_tiles,
    render_frame,
    transparent_overlap_owners,
    visible_nibbles,
)

COMMENT = (
    "// TODO: Find a more natural editable representation for frame layouts and "
    "occluded tile pixels.\n"
)


def dumps(assets: dict) -> str:
    return COMMENT + json.dumps(assets, indent=2) + "\n"


def loads(text: str) -> dict:
    # The carver writes only whole-line // comments, not arbitrary JSONC syntax.
    return json.loads(
        "\n".join(
            line for line in text.splitlines() if not line.lstrip().startswith("//")
        )
    )


def _check_keys(data: dict, keys: set[str]) -> None:
    if not isinstance(data, dict) or set(data) != keys:
        raise ValueError(
            f"expected fields {sorted(keys)}, got {sorted(data) if isinstance(data, dict) else type(data).__name__}"
        )


def frame_set_from_data(data: dict) -> FrameSet:
    _check_keys(
        data,
        {
            "width",
            "height",
            "movement_collision_box",
            "frame_count",
            "max_frame_tile_bytes",
            "hotspot_count",
            "trigger_box_count",
            "frame_offsets",
            "frames",
            "size",
        },
    )
    bounds = data["movement_collision_box"]
    _check_keys(bounds, {"x_min", "x_max", "y_min", "y_max"})
    frames = []
    for row in data["frames"]:
        _check_keys(
            row,
            {
                "flags",
                "field_0x1",
                "width",
                "height",
                "tile_offset",
                "tile_bytes",
                "hotspots",
                "boxes",
                "oam",
            },
        )
        for point in row["hotspots"]:
            _check_keys(point, {"x", "y"})
        for box in row["boxes"]:
            _check_keys(box, {"x_min", "x_max", "y_min", "y_max", "enable", "pad"})
        for obj in row["oam"]:
            _check_keys(obj, {"x", "y", "obj_size", "obj_shape", "tile_offset"})
        frames.append(
            Frame(
                *(
                    row[field]
                    for field in (
                        "flags",
                        "field_0x1",
                        "width",
                        "height",
                        "tile_offset",
                        "tile_bytes",
                    )
                ),
                tuple(Hotspot(**point) for point in row["hotspots"]),
                tuple(TriggerBox(**box) for box in row["boxes"]),
                tuple(OamTemplate(**obj) for obj in row["oam"]),
            )
        )
    result = FrameSet(
        data["width"],
        data["height"],
        SpriteBounds(**bounds),
        data["frame_count"],
        data["max_frame_tile_bytes"],
        data["hotspot_count"],
        data["trigger_box_count"],
        tuple(data["frame_offsets"]),
        tuple(frames),
        data["size"],
    )
    if (
        result.frame_count != len(result.frames)
        or len(result.frame_offsets) != result.frame_count
    ):
        raise ValueError("frame count does not match frame records")
    if any(frame.oam_count != len(frame.oam) for frame in result.frames):
        raise ValueError("frame OAM count does not match its records")
    return result


def hidden_nibbles(
    tiles: bytes, frame: Frame, bounds: tuple[int, int, int, int]
) -> list[list[int]]:
    """Store nonzero nibbles under later OBJs, even if those OBJs are transparent."""
    visible = visible_nibbles(frame, bounds)
    return [
        [index, (tiles[index // 2] >> (4 * (index % 2))) & 15]
        for index in range(len(tiles) * 2)
        if index not in visible and (tiles[index // 2] >> (4 * (index % 2))) & 15
    ]


def _asset_metadata_and_images(
    tile_stream: bytes, frame_set: FrameSet, palette: tuple[int, ...]
) -> tuple[dict, list[IndexedImage]]:
    """Extract validated metadata and the images used to validate it."""
    bounds = frame_bounds(frame_set.frames)
    hidden = []
    transparent_owners = []
    images = []
    for frame in frame_set.frames:
        tiles = decode_frame_tiles(tile_stream, frame)
        image = render_frame(frame, tiles, bounds, palette)
        patches = hidden_nibbles(tiles, frame, bounds)
        owners = transparent_overlap_owners(frame, tiles, bounds)
        restored = rebuild_frame_tiles(
            apply_hidden_nibbles(frame, bounds, patches), frame, image, bounds, owners
        )
        if restored != tiles:
            raise ValueError("frame pixels and hidden nibbles do not restore its tiles")
        hidden.append(patches)
        transparent_owners.append(owners)
        images.append(image)
    metadata = {
        "tiles_size": len(tile_stream),
        "palette": palette,
        "frame_set": asdict(frame_set),
        "hidden_nibbles": hidden,
    }
    if any(transparent_owners):
        metadata["transparent_owners"] = transparent_owners
    return metadata, images


def asset_metadata(
    tile_stream: bytes, frame_set: FrameSet, palette: tuple[int, ...]
) -> dict:
    """Extract structural data and prove each image plus metadata restores its tiles."""
    metadata, _images = _asset_metadata_and_images(tile_stream, frame_set, palette)
    return metadata
