"""Generate enemy animation C from local PNG metadata and committed symbol addresses.

This step does not read the ROM or re-carve images. The source is disposable; the
metadata and config/split.cfg survive deletion of build/.
"""

from pathlib import Path

from rotkit.build.enemy_graphics import load_assets
from rotkit.carve import doc_comment
from rotkit.carve.sprites import variable_frame_set_lines
from rotkit.paths import ENEMY_ASSET_SOURCES, GFX_LOCAL, ROOT
from rotkit.stores import data_symbols, read_split

PREFIX = "EnemyAnimationAssets_"


def group_for_source(source: str) -> str | None:
    path = Path(source)
    if path.is_absolute():
        try:
            path = path.relative_to(ROOT)
        except ValueError:
            return None
    if path.parent != ENEMY_ASSET_SOURCES.relative_to(ROOT):
        return None
    if not path.name.startswith(PREFIX) or path.suffix != ".c":
        return None
    return path.stem.removeprefix(PREFIX)


def source_for_group(group: str) -> str:
    return (ENEMY_ASSET_SOURCES / f"{PREFIX}{group}.c").relative_to(ROOT).as_posix()


def generate(source: str) -> str:
    group = group_for_source(source)
    if group is None or source_for_group(group) != source:
        raise ValueError(f"not an enemy animation asset source: {source}")
    split = read_split()
    if source not in split:
        raise ValueError(f"{source}: missing config/split.cfg row")
    directory = GFX_LOCAL / "enemy" / group
    assets = load_assets(directory)
    symbols = {symbol.name: symbol for symbol in data_symbols()}
    records = []
    for asset in assets:
        tile_name = f"EnemyAnimationTiles_{asset.name}"
        frame_name = f"EnemyAnimationFrames_{asset.name}"
        tiles = symbols.get(tile_name)
        frames = symbols.get(frame_name)
        if (
            tiles is None
            or frames is None
            or tiles.type != f"u8[0x{asset.tiles_size:x}]"
            or frames.type != "SpriteFrameSet"
            or frames.addr != tiles.addr + asset.tiles_size
        ):
            raise ValueError(
                f"{directory}: {asset.name}: symbol store disagrees with metadata"
            )
        records.append((tiles.addr, frames.addr, asset))
    records.sort(key=lambda record: record[0])
    if not records or records[0][0] != split[source]:
        raise ValueError(
            f"{source}: first animation address disagrees with config/split.cfg"
        )
    for (_, frames_addr, asset), (next_addr, _, _) in zip(records, records[1:]):
        if ((frames_addr + asset.frame_set.size + 3) & ~3) != next_addr:
            raise ValueError(
                f"{directory}: animation assets are not contiguous in ROM order"
            )

    lines = [
        f'#include "gfx/enemy/{group}.inc"',
        '#include "spriteAnimation.h"',
        "",
        "// clang-format off",
        "",
    ]
    for tiles_addr, frames_addr, asset in records:
        prefix = f"EnemyAnimation_{asset.name}"
        lines.extend(
            [
                *doc_comment(tiles_addr),
                f"const u8 EnemyAnimationTiles_{asset.name}[] = {{{prefix}_TILES}};",
                "",
                *doc_comment(frames_addr),
                *variable_frame_set_lines(
                    f"EnemyAnimationFrames_{asset.name}", asset.frame_set, prefix
                ),
                "",
            ]
        )
    return "\n".join([*lines, "// clang-format on"]) + "\n"


def run(source: str) -> None:
    text = generate(source)
    target = ROOT / source
    target.parent.mkdir(parents=True, exist_ok=True)
    if not target.exists() or target.read_text() != text:
        target.write_text(text)
    else:
        target.touch()
    print(f"  generated {source}")
