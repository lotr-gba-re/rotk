"""Carves the enemy animation banks and visual reference sheets.

EnemyAnimationBanks is indexed by EnemyType. Each bank has five facing descriptors.
Shared tile/frame assets have one canonical editable PNG sequence. Per-enemy-type reference
sheets retain the table's aliases and palettes.
"""

import os
import shutil
from dataclasses import dataclass
from functools import cache
from struct import Struct

from rotkit.carve import (
    CaptionsOption,
    TableSymbol,
    data_symbol,
    doc_comment,
    enum_entry_comment,
    upsert_map,
    write_animated_sheet,
    write_cfg,
    write_sheet,
    write_table,
)
from rotkit.cheaders import extract_enum, invert_enum
from rotkit.enemy_animation_metadata import _asset_metadata_and_images, dumps
from rotkit.paths import CARVED_DATA, GFX_LOCAL, GFX_SHEETS, ROOT
from rotkit.png import (
    IndexedImage,
    bgr555_to_rgb,
    write_animation_gif,
    write_indexed,
)
from rotkit.rom import ROMBASE, load_rom
from rotkit.spritegfx import (
    FrameSet,
    decode_frame_tiles,
    encode_sequence_from_images,
    frame_bounds,
    parse_frame_set,
    render_frame,
)
from rotkit.stores import data_symbols

GROUP = "enemy_graphics"
CONFIG_GROUP = "enemy_graphics"
GFX_GROUP = "enemy"
GROUPS = (GFX_GROUP,)
DATA_GROUPS = (GROUP,)
GFX_DIR = GFX_LOCAL / GFX_GROUP
TABLE = "EnemyAnimationBanks"
TABLE_ADDR = 0x08056820
ENEMY_TYPE_COUNT = 83
ANIMATION_BANK_COUNT = 6
FACING_VARIANT_COUNT = 5
REFERENCE_SHEET_MAX_CELL_EDGE = 128
FACING_VARIANTS_ADDR = 0x08050418
ENEMY_PALETTE_IDS_ADDR = 0x0806D27C
# Contiguous enemy-related banks reached by the primary table, secondary part
# matrices, direct code references, or retained alternate sets. The mixed actor
# pool beginning at 0x08058614 is intentionally excluded apart from Unit 65.
BANK_RANGES = (
    (0x0804FBEC, 0x0805040C),
    (0x080512D0, 0x08056820),
    (0x08058754, 0x080588E4),
)
ASSET_SOURCE_SLICES = (
    (0x082ABC44, 0x082AF900, 73, "ExtraPoses18_20"),
    (0x0875101C, 0x08752024, 5, "AdditionalPose"),
    (0x08BC856C, 0x08BE803C, 73, "ExtraPoses00_10"),
    (0x08BE803C, 0x08C0378C, 73, "Primary"),
    (0x08C0378C, 0x08C0EE4C, 73, "ExtraPoses11_17"),
    (0x08CB2FA4, 0x08CC5F20, 5, "Main"),
    (0x08CC60C0, 0x08CED19C, 10, "Body"),
    (0x08D1B7FC, 0x08D214D8, 10, "Part1"),
    (0x08D214D8, 0x08D26EEC, 10, "Part2"),
    (0x08D26EEC, 0x08D2D174, 10, "Part3"),
    (0x08D2D174, 0x08D353B0, 10, "Part5"),
    (0x08D353B0, 0x08D3D4FC, 10, "Part4"),
    (0x08D79474, 0x08DB6604, 62, "Main"),
    (0x08DE6E1C, 0x08DE95F0, 62, "AdditionalPoses"),
    (0x08F3C6F4, 0x08F61D28, 31, "Core"),
    (0x08F61D28, 0x08F6EE54, 31, "AdditionalPoses"),
    (0x08F6EE54, 0x08F80950, 33, "AdditionalPoses"),
    (0x08F80950, 0x08F9FD8C, 33, "Primary"),
)
# Enemy initializers and update routines reference these matrices directly. The ROM has
# no global attachment-matrix registry to carve instead.
SECONDARY_MATRIX_PART_COUNT = 6
SECONDARY_MATRIX_POSE_COUNT = 7
ENEMY_63_PART_MATRIX_ADDR = 0x0805104C
ENEMY_52_PART_MATRIX_ADDR = 0x080511D4
ENEMY_28_ATTACHMENT_MATRIX_ADDR = 0x0806CEE8
ENEMY_10_ATTACHMENT_MATRIX_ADDR = 0x0806CFE4
ENEMY_10_ATTACK_ATTACHMENT_MATRIX_ADDR = 0x0806D0E0
ENEMY_68_PART_MATRIX_ADDR = 0x0806DFBC
ENEMY_31_PART_MATRIX_ADDR = 0x0807050C
SECONDARY_MATRIX_SPECS = (
    (ENEMY_63_PART_MATRIX_ADDR, 63, "PartAnimationBanks", "Parts"),
    (ENEMY_52_PART_MATRIX_ADDR, 52, "PartAnimationBanks", "Parts"),
    (ENEMY_28_ATTACHMENT_MATRIX_ADDR, 28, "FlagBearerAnimationBanks", "FlagBearer"),
    (ENEMY_10_ATTACHMENT_MATRIX_ADDR, 10, "CompositeAnimationBanks", "Composite"),
    (
        ENEMY_10_ATTACK_ATTACHMENT_MATRIX_ADDR,
        10,
        "CompositeAttackAnimationBanks",
        "CompositeAttack",
    ),
    (ENEMY_68_PART_MATRIX_ADDR, 68, "PartAnimationBanks", "Parts"),
    (ENEMY_31_PART_MATRIX_ADDR, 31, "PartAnimationBanks", "Parts"),
)

_SPRITE_ANIMATION = Struct("<IIIB3x")
_ANIMATION_ROW = Struct("<6I")
_ENEMY_TYPE = Struct("<I H H H H 8s")
_PALETTE = Struct("<16H")
_U32 = Struct("<I")


@dataclass(frozen=True)
class AnimationRecord:
    animation_addr: int
    tiles_addr: int
    frames_addr: int
    palette_addr: int
    frame_duration: int
    frame_set: FrameSet


@dataclass(frozen=True)
class AnimationAsset:
    tiles_addr: int
    frames_addr: int
    frame_set: FrameSet


@dataclass(frozen=True)
class CompositePart:
    part: int
    draw_order: int
    palette_id: int | None


@dataclass(frozen=True)
class CompositeEnemy:
    matrix_addrs: tuple[int, ...]
    parts: tuple[CompositePart, ...]


@dataclass(frozen=True)
class CompositeAnimation:
    matrix_addr: int
    matrix_pose: int
    parts: tuple[CompositePart, ...]


@dataclass(frozen=True)
class Carve:
    records: dict[int, AnimationRecord]
    assets: dict[tuple[int, int], AnimationAsset]
    bank_names: dict[int, str]
    asset_names: dict[tuple[int, int], str]
    palettes: dict[tuple[int, int], tuple[int, ...]]
    groups: list[list[AnimationAsset]]


def _check(condition: bool, message: str) -> None:
    if not condition:
        raise SystemExit(f"enemy graphics: {message}")


def _bank_addresses() -> set[int]:
    return {
        address
        for start, end in BANK_RANGES
        for address in range(start, end, FACING_VARIANT_COUNT * _SPRITE_ANIMATION.size)
    }


def _primary_bank_enemy_types(rom: bytes) -> dict[int, int]:
    enemy_types = {}
    for enemy_type in range(ENEMY_TYPE_COUNT):
        row = _ANIMATION_ROW.unpack_from(
            rom, TABLE_ADDR - ROMBASE + enemy_type * _ANIMATION_ROW.size
        )
        for bank_address in row:
            if bank_address:
                enemy_types.setdefault(bank_address, enemy_type)
    return enemy_types


def _animation_catalog(
    rom: bytes,
) -> tuple[dict[int, AnimationRecord], dict[tuple[int, int], AnimationAsset]]:
    records = {}
    assets = {}
    frame_sets = {}
    for bank_address in sorted(_bank_addresses()):
        for facing in range(FACING_VARIANT_COUNT):
            animation_addr = bank_address + facing * _SPRITE_ANIMATION.size
            tiles_addr, frames_addr, palette_addr, frame_duration = (
                _SPRITE_ANIMATION.unpack_from(rom, animation_addr - ROMBASE)
            )
            if frames_addr not in frame_sets:
                frame_sets[frames_addr] = parse_frame_set(rom, frames_addr)
            frame_set = frame_sets[frames_addr]
            records[animation_addr] = AnimationRecord(
                animation_addr,
                tiles_addr,
                frames_addr,
                palette_addr,
                frame_duration,
                frame_set,
            )
            assets.setdefault(
                (tiles_addr, frames_addr),
                AnimationAsset(tiles_addr, frames_addr, frame_set),
            )
    return records, assets


@cache
def _enemy_type_names() -> tuple[str, ...]:
    names = invert_enum(extract_enum("include/enemy.h", "EnemyType"))
    _check(
        all(enemy_type in names for enemy_type in range(ENEMY_TYPE_COUNT)),
        "EnemyType must name every graphics-table row",
    )
    return tuple(names[enemy_type] for enemy_type in range(ENEMY_TYPE_COUNT))


@cache
def _pose_names() -> tuple[str, ...]:
    names = invert_enum(extract_enum("include/enemy.h", "EnemyAnimationPose"))
    _check(
        names.get(ANIMATION_BANK_COUNT) == "ENEMY_ANIMATION_POSE_COUNT",
        "EnemyAnimationPose count must match the graphics table",
    )
    expected = list(range(ANIMATION_BANK_COUNT))
    _check(
        all(pose in names for pose in expected),
        "EnemyAnimationPose must name every graphics-table pose",
    )
    prefix = "ENEMY_ANIMATION_POSE_"
    return tuple(names[pose][len(prefix) :] for pose in expected)


def _pascal_name(name: str) -> str:
    return "".join(part.title() for part in name.split("_"))


@cache
def _enemy_type_ids() -> dict[str, int]:
    names = [_pascal_name(name) for name in _enemy_type_names()]
    _check(len(set(names)) == len(names), "EnemyType names must remain unique")
    return {name: enemy_type for enemy_type, name in enumerate(names)}


@cache
def _obj_palettes_addr() -> int:
    return next(
        symbol for symbol in data_symbols() if symbol.name == "ObjPalettes"
    ).addr


def _obj_palette(rom: bytes, palette_id: int) -> tuple[int, ...]:
    palette_ptr = _U32.unpack_from(
        rom, _obj_palettes_addr() - ROMBASE + palette_id * _U32.size
    )[0]
    return _PALETTE.unpack_from(rom, palette_ptr - ROMBASE)


def _palette(rom: bytes, enemy_type: int) -> tuple[int, ...]:
    palette_id = rom[ENEMY_PALETTE_IDS_ADDR - ROMBASE + enemy_type]
    return _obj_palette(rom, palette_id)


def _record_palette(
    rom: bytes, record: AnimationRecord, fallback_palette: tuple[int, ...]
) -> tuple[int, ...]:
    return (
        _PALETTE.unpack_from(rom, record.palette_addr - ROMBASE)
        if record.palette_addr
        else fallback_palette
    )


def _animation_frames(
    rom: bytes, record: AnimationRecord, fallback_palette: tuple[int, ...]
) -> tuple[IndexedImage, ...]:
    bounds = frame_bounds(record.frame_set.frames)
    palette = _record_palette(rom, record, fallback_palette)
    tiles = rom[record.tiles_addr - ROMBASE : record.frames_addr - ROMBASE]
    return tuple(
        render_frame(frame, decode_frame_tiles(tiles, frame), bounds, palette)
        for frame in record.frame_set.frames
    )


def _blank_frame(palette: tuple[int, ...]) -> IndexedImage:
    return IndexedImage(1, 1, b"\x00", [bgr555_to_rgb(c) for c in palette], 0)


def _composite_enemy(enemy_type: int) -> CompositeEnemy | None:
    # These parts are attached by enemy runtime initializers rather than the primary
    # EnemyAnimationBanks row.
    if 10 <= enemy_type <= 17:
        # The optional random part 5 is not tied to an enemy type.
        return CompositeEnemy(
            (
                ENEMY_10_ATTACHMENT_MATRIX_ADDR,
                ENEMY_10_ATTACK_ATTACHMENT_MATRIX_ADDR,
            ),
            (CompositePart((enemy_type - 10) % 4 + 1, 5, 2),),
        )
    if enemy_type == 28:
        return CompositeEnemy(
            (ENEMY_28_ATTACHMENT_MATRIX_ADDR,),
            (CompositePart(1, 4, None),),
        )
    if enemy_type in (31, 32):
        return CompositeEnemy(
            (ENEMY_31_PART_MATRIX_ADDR,),
            (CompositePart(1, 4, None),),
        )
    if enemy_type in (35, 36):
        return CompositeEnemy(
            (ENEMY_31_PART_MATRIX_ADDR,),
            (
                CompositePart(2, 5, 2),
                CompositePart(3, 3, 3),
            ),
        )
    if 52 <= enemy_type <= 54:
        return CompositeEnemy(
            (ENEMY_52_PART_MATRIX_ADDR,),
            (CompositePart(1, 4, None),),
        )
    if enemy_type == 63:
        # This part is present in the Steward's Tomb encounter and detached in other states.
        return CompositeEnemy(
            (ENEMY_63_PART_MATRIX_ADDR,),
            (CompositePart(1, 4, 6),),
        )
    if enemy_type == 68:
        return CompositeEnemy(
            (ENEMY_68_PART_MATRIX_ADDR,),
            (CompositePart(1, 5, 2),),
        )
    return None


def _composite_animation(
    rom: bytes, enemy_type: int, pose: int
) -> CompositeAnimation | None:
    composite = _composite_enemy(enemy_type)
    if composite is None:
        return None
    primary_row = _ANIMATION_ROW.unpack_from(
        rom, TABLE_ADDR - ROMBASE + enemy_type * _ANIMATION_ROW.size
    )
    primary_bank = primary_row[pose]
    matches = []
    row_struct = Struct(f"<{SECONDARY_MATRIX_POSE_COUNT}I")
    for matrix_addr in composite.matrix_addrs:
        matrix_row = row_struct.unpack_from(rom, matrix_addr - ROMBASE)
        matches.extend(
            (matrix_addr, matrix_pose)
            for matrix_pose, bank_addr in enumerate(matrix_row)
            if bank_addr == primary_bank
        )
    _check(
        len(matches) == 1,
        f"enemy type {enemy_type} pose {pose} maps to {len(matches)} composite columns",
    )
    matrix_addr, matrix_pose = matches[0]
    return CompositeAnimation(matrix_addr, matrix_pose, composite.parts)


def _matrix_record(
    rom: bytes,
    records: dict[int, AnimationRecord],
    matrix_addr: int,
    part: int,
    pose: int,
    facing: int,
) -> AnimationRecord:
    offset = (part * SECONDARY_MATRIX_POSE_COUNT + pose) * _U32.size
    bank_addr = _U32.unpack_from(rom, matrix_addr - ROMBASE + offset)[0]
    _check(bank_addr != 0, f"null composite bank at 0x{matrix_addr + offset:08x}")
    return records[bank_addr + facing * _SPRITE_ANIMATION.size]


def _composite_animation_frames(
    rom: bytes,
    records: dict[int, AnimationRecord],
    enemy_type: int,
    facing: int,
    composite: CompositeAnimation,
) -> tuple[IndexedImage, ...]:
    enemy_palette = _palette(rom, enemy_type)
    main_record = _matrix_record(
        rom,
        records,
        composite.matrix_addr,
        0,
        composite.matrix_pose,
        facing,
    )
    part_layers = [
        (
            part,
            _matrix_record(
                rom,
                records,
                composite.matrix_addr,
                part.part,
                composite.matrix_pose,
                facing,
            ),
            enemy_palette
            if part.palette_id is None
            else _obj_palette(rom, part.palette_id),
        )
        for part in composite.parts
    ]
    before_main = sorted(
        (layer for layer in part_layers if layer[0].draw_order >= 4),
        key=lambda layer: layer[0].draw_order,
        reverse=True,
    )
    after_main = sorted(
        (layer for layer in part_layers if layer[0].draw_order < 4),
        key=lambda layer: layer[0].draw_order,
        reverse=True,
    )
    layers = [
        *before_main,
        (None, main_record, _record_palette(rom, main_record, enemy_palette)),
        *after_main,
    ]
    frame_count = main_record.frame_set.frame_count
    _check(
        all(
            record.frame_set.frame_count == frame_count
            for _part, record, _palette in layers
        ),
        f"composite frame counts differ for enemy type {enemy_type}",
    )
    bounds = frame_bounds(
        tuple(
            frame
            for _part, record, _palette in layers
            for frame in record.frame_set.frames
        )
    )
    width, height = bounds[2] - bounds[0], bounds[3] - bounds[1]
    palette = [(0, 0, 0)]
    palette_bases = []
    for _part, _record, layer_palette in layers:
        palette_bases.append(len(palette) - 1)
        palette.extend(bgr555_to_rgb(color) for color in layer_palette[1:])
    _check(len(palette) <= 256, "composite palette exceeds 256 colors")

    images = []
    for frame_index in range(frame_count):
        pixels = bytearray(width * height)
        for layer_index, (_part, record, layer_palette) in enumerate(layers):
            frame = record.frame_set.frames[frame_index]
            tiles = rom[record.tiles_addr - ROMBASE : record.frames_addr - ROMBASE]
            image = render_frame(
                frame,
                decode_frame_tiles(tiles, frame),
                bounds,
                layer_palette,
            )
            palette_base = palette_bases[layer_index]
            for index, color in enumerate(image.pixels):
                if color:
                    pixels[index] = palette_base + color
        images.append(IndexedImage(width, height, bytes(pixels), palette, 0))
    return tuple(images)


def _enemy_animation_frames(
    rom: bytes,
    records: dict[int, AnimationRecord],
    enemy_type: int,
    pose: int,
    facing: int,
    frames_cache: dict[tuple[int, int, tuple[int, ...]], tuple[IndexedImage, ...]]
    | None = None,
) -> tuple[IndexedImage, ...]:
    composite = _composite_animation(rom, enemy_type, pose)
    if composite is not None:
        return _composite_animation_frames(
            rom,
            records,
            enemy_type,
            facing,
            composite,
        )
    row = _ANIMATION_ROW.unpack_from(
        rom, TABLE_ADDR - ROMBASE + enemy_type * _ANIMATION_ROW.size
    )
    bank_addr = row[pose]
    if bank_addr == 0:
        return ()
    record = records[bank_addr + facing * _SPRITE_ANIMATION.size]
    fallback_palette = _palette(rom, enemy_type)
    if frames_cache is None:
        return _animation_frames(rom, record, fallback_palette)
    palette = _record_palette(rom, record, fallback_palette)
    key = (record.tiles_addr, record.frames_addr, palette)
    if key not in frames_cache:
        frames_cache[key] = _animation_frames(rom, record, fallback_palette)
    return frames_cache[key]


def _asset_palettes(
    rom: bytes,
    records: dict[int, AnimationRecord],
    bank_names: dict[int, str],
) -> dict[tuple[int, int], tuple[int, ...]]:
    palettes = {}
    for bank, enemy_type in _primary_bank_enemy_types(rom).items():
        for facing in range(FACING_VARIANT_COUNT):
            record = records[bank + facing * _SPRITE_ANIMATION.size]
            palette = (
                _PALETTE.unpack_from(rom, record.palette_addr - ROMBASE)
                if record.palette_addr
                else _palette(rom, enemy_type)
            )
            palettes.setdefault((record.tiles_addr, record.frames_addr), palette)
    enemy_type_ids = _enemy_type_ids()
    for bank in sorted(bank_names):
        enemy_type_name, _pose_name = _animation_bank_name_parts(bank_names[bank])
        enemy_type = enemy_type_ids[enemy_type_name]
        for facing in range(FACING_VARIANT_COUNT):
            record = records[bank + facing * _SPRITE_ANIMATION.size]
            palette = (
                _PALETTE.unpack_from(rom, record.palette_addr - ROMBASE)
                if record.palette_addr
                else _palette(rom, enemy_type)
            )
            palettes.setdefault((record.tiles_addr, record.frames_addr), palette)
    return palettes


def load(rom: bytes) -> Carve:
    records, assets = _animation_catalog(rom)
    bank_names, asset_names = _animation_symbol_names(rom, records)
    palettes = _asset_palettes(rom, records, bank_names)
    _check(len(set(asset_names.values())) == len(assets), "duplicate asset names")
    for asset in assets.values():
        frames = asset.frame_set
        where = f"asset at 0x{asset.tiles_addr:08x}"
        _check(frames.frame_count > 0, f"{where}: empty frame set")
        _check(frames.frames[0].tile_offset == 0, f"{where}: tile data has a prefix")
        offset = frames.frame_count * 2
        tile_offsets = [frame.tile_offset for frame in frames.frames]
        tile_offsets.append(asset.frames_addr - asset.tiles_addr)
        _check(
            all(a < b and a % 4 == 0 for a, b in zip(tile_offsets, tile_offsets[1:])),
            f"{where}: tile chunks are not packed in frame order",
        )
        for index, frame in enumerate(frames.frames):
            _check(
                frames.frame_offsets[index] == offset, f"{where}: frame layout has gaps"
            )
            offset += (
                8
                + frames.hotspot_count * 2
                + frames.trigger_box_count * 6
                + len(frame.oam) * 4
            )
        end = asset.frames_addr + frames.size
        _check(
            not any(rom[end - ROMBASE : _align4(end) - ROMBASE]),
            f"{where}: nonzero tail padding",
        )
    groups = _asset_groups(assets, asset_names)
    group_names = [_asset_group_name(group, asset_names) for group in groups]
    _check(len(set(group_names)) == len(groups), "duplicate asset group names")
    return Carve(records, assets, bank_names, asset_names, palettes, groups)


def _families(rom: bytes, enemy_table: TableSymbol) -> list[str]:
    names = invert_enum(extract_enum("include/enemy.h", "EnemyFamily"))
    families = []
    for enemy_type in range(enemy_table.count):
        fields = _ENEMY_TYPE.unpack_from(
            rom, enemy_table.addr - ROMBASE + enemy_type * _ENEMY_TYPE.size
        )
        family = fields[-1][-1]
        families.append(names.get(family, f"ENEMY_FAMILY_UNKNOWN_{family}"))
    return families


def _align4(address: int) -> int:
    return address + (-address % 4)


def _animation_asset_name_parts(asset_name: str) -> tuple[str, str]:
    animation_name, separator, _facing = asset_name.rpartition("_Facing")
    _check(bool(separator), f"invalid enemy animation asset name {asset_name}")
    enemy_type_name, separator, pose_name = animation_name.rpartition("_")
    _check(bool(separator), f"invalid enemy animation asset name {asset_name}")
    return enemy_type_name, pose_name


def _animation_bank_name_parts(bank_name: str) -> tuple[str, str]:
    prefix = "EnemyAnimationBank_"
    _check(
        bank_name.startswith(prefix), f"invalid enemy animation bank name {bank_name}"
    )
    enemy_type_name, separator, pose_name = bank_name.removeprefix(prefix).rpartition(
        "_"
    )
    _check(bool(separator), f"invalid enemy animation bank name {bank_name}")
    return enemy_type_name, pose_name


def _enemy_type_source_name(enemy_type_name: str) -> str:
    return f"{_enemy_type_ids()[enemy_type_name]:02}_{enemy_type_name}"


def _enemy_type_symbol_name(enemy_type: int) -> str:
    return _pascal_name(_enemy_type_names()[enemy_type])


def _asset_source_slices() -> tuple[tuple[int, int, str], ...]:
    return tuple(
        (start, end, f"{enemy_type:02}_{_enemy_type_symbol_name(enemy_type)}_{suffix}")
        for start, end, enemy_type, suffix in ASSET_SOURCE_SLICES
    )


def _secondary_matrix_specs() -> tuple[tuple[int, str, str], ...]:
    return tuple(
        (
            address,
            f"{_enemy_type_symbol_name(enemy_type)}{symbol_suffix}",
            f"{enemy_type:02}_{_enemy_type_symbol_name(enemy_type)}_{source_suffix}",
        )
        for address, enemy_type, symbol_suffix, source_suffix in SECONDARY_MATRIX_SPECS
    )


def _asset_group_name(
    group: list[AnimationAsset], asset_names: dict[tuple[int, int], str]
) -> str:
    start = group[0].tiles_addr
    last = group[-1]
    end = _align4(last.frames_addr + last.frame_set.size)
    slice_names = {
        (slice_start, slice_end): name
        for slice_start, slice_end, name in _asset_source_slices()
    }
    if (start, end) in slice_names:
        return slice_names[(start, end)]
    key = (group[0].tiles_addr, group[0].frames_addr)
    enemy_type_name, _pose_name = _animation_asset_name_parts(asset_names[key])
    return _enemy_type_source_name(enemy_type_name)


def _asset_groups(
    assets: dict[tuple[int, int], AnimationAsset],
    asset_names: dict[tuple[int, int], str],
) -> list[list[AnimationAsset]]:
    groups = []
    slice_starts = {start for start, _end, _name in _asset_source_slices()}
    for asset in sorted(assets.values(), key=lambda item: item.tiles_addr):
        key = (asset.tiles_addr, asset.frames_addr)
        enemy_type_name, _pose_name = _animation_asset_name_parts(asset_names[key])
        if groups:
            previous = groups[-1][-1]
            previous_key = (previous.tiles_addr, previous.frames_addr)
            previous_end = _align4(previous.frames_addr + previous.frame_set.size)
            previous_enemy_type, _previous_pose = _animation_asset_name_parts(
                asset_names[previous_key]
            )
            if (
                asset.tiles_addr != previous_end
                or enemy_type_name != previous_enemy_type
                or asset.tiles_addr in slice_starts
            ):
                groups.append([])
        else:
            groups.append([])
        groups[-1].append(asset)
    return groups


def _bank_groups(
    bank_addresses: set[int], bank_names: dict[int, str]
) -> list[list[int]]:
    groups = []
    for address in sorted(bank_addresses):
        enemy_type_name, _pose_name = _animation_bank_name_parts(bank_names[address])
        if groups:
            previous = groups[-1][-1]
            previous_enemy_type, _previous_pose = _animation_bank_name_parts(
                bank_names[previous]
            )
            contiguous = (
                address == previous + FACING_VARIANT_COUNT * _SPRITE_ANIMATION.size
            )
            if not contiguous or enemy_type_name != previous_enemy_type:
                groups.append([])
        else:
            groups.append([])
        groups[-1].append(address)
    return groups


def _animation_asset_symbols(asset_name: str) -> tuple[str, str]:
    return (
        f"EnemyAnimationTiles_{asset_name}",
        f"EnemyAnimationFrames_{asset_name}",
    )


def _known_extra_bank_names() -> dict[int, str]:
    def bank_name(enemy_type: int, suffix: str) -> str:
        return f"EnemyAnimationBank_{_enemy_type_symbol_name(enemy_type)}_{suffix}"

    names = {
        0x0804FFFC: bank_name(73, "AlternateAttack"),
        0x08051460: bank_name(52, "AttachedPartWalk"),
        0x080514B0: bank_name(52, "AttachedPartDeath"),
        0x08051500: bank_name(52, "AttachedPartAttack"),
        0x08051550: bank_name(52, "AttachedPartStand"),
        0x080515A0: bank_name(52, "AttachedPartStagger"),
        0x080517D0: bank_name(10, "KnockdownState30"),
        0x08051870: bank_name(10, "RecoveryState22"),
        0x08051960: bank_name(10, "KnockedDown"),
        0x080529A0: bank_name(6, "StunnedCopy"),
        0x08052FE0: bank_name(26, "SpecialAttack"),
        0x080532B0: bank_name(28, "ExtraPose"),
        0x08053350: bank_name(28, "FlagPartWalk"),
        0x080533A0: bank_name(28, "FlagPartStand"),
        0x080533F0: bank_name(28, "FlagPartDeath"),
        0x08053440: bank_name(28, "FlagPartStagger"),
        0x08053490: bank_name(28, "FlagPartExtraPose"),
        0x080534E0: bank_name(28, "FlagPartAttack"),
        0x08054700: bank_name(58, "StandCopy"),
        0x080587A4: bank_name(65, "ExtraPose"),
    }
    alternate_poses = ("Walk", "Stand", "Death", "Stagger", "Stunned", "Attack")
    for index, pose in enumerate(alternate_poses):
        names[0x08052A40 + index * 0x50] = bank_name(18, f"Alternate{pose}")

    component_rows = (
        (1, 0x08051A00, 0x08051A50),
        (2, 0x08051C80, 0x08051CD0),
        (3, 0x08051F00, 0x08051F50),
        (4, 0x08052400, 0x08052450),
        (5, 0x08052180, 0x080521D0),
    )
    component_poses = (
        "KnockdownState30",
        "Death",
        "RecoveryState22",
        "Stagger",
        "Stand",
        "KnockedDown",
        "Walk",
    )
    for part, attack_address, regular_address in component_rows:
        names[attack_address] = bank_name(10, f"Part{part}Attack")
        for pose, pose_name in enumerate(component_poses):
            names[regular_address + pose * 0x50] = bank_name(
                10, f"Part{part}{pose_name}"
            )
    return names


def _animation_symbol_names(
    rom: bytes,
    records: dict[int, AnimationRecord],
) -> tuple[dict[int, str], dict[tuple[int, int], str]]:
    enemy_type_names = _enemy_type_names()
    pose_names = _pose_names()
    bank_names: dict[int, str] = {}
    asset_names: dict[tuple[int, int], str] = {}
    for enemy_type in range(ENEMY_TYPE_COUNT):
        row = _ANIMATION_ROW.unpack_from(
            rom, TABLE_ADDR - ROMBASE + enemy_type * _ANIMATION_ROW.size
        )
        enemy_type_name = _pascal_name(enemy_type_names[enemy_type])
        for pose, bank_address in enumerate(row):
            if bank_address == 0:
                continue
            pose_name = _pascal_name(pose_names[pose])
            bank_names.setdefault(
                bank_address,
                f"EnemyAnimationBank_{enemy_type_name}_{pose_name}",
            )
            for facing in range(FACING_VARIANT_COUNT):
                record = records[bank_address + facing * _SPRITE_ANIMATION.size]
                key = (record.tiles_addr, record.frames_addr)
                asset_names.setdefault(
                    key,
                    f"{enemy_type_name}_{pose_name}_Facing{facing}",
                )

    for bank_address, name in _known_extra_bank_names().items():
        if bank_address in records:
            bank_names.setdefault(bank_address, name)
    extra_indices: dict[int, int] = {}
    primary_bank_enemy_types = _primary_bank_enemy_types(rom)
    for bank_address in sorted(_bank_addresses()):
        if bank_address in bank_names:
            continue
        nearest_primary = min(
            primary_bank_enemy_types,
            key=lambda address: (abs(address - bank_address), address),
        )
        enemy_type = primary_bank_enemy_types[nearest_primary]
        extra_index = extra_indices.get(enemy_type, 0)
        enemy_type_name = _pascal_name(enemy_type_names[enemy_type])
        bank_names[bank_address] = (
            f"EnemyAnimationBank_{enemy_type_name}_ExtraPose{extra_index:02}"
        )
        extra_indices[enemy_type] = extra_index + 1

    prefix = "EnemyAnimationBank_"
    for bank_address in sorted(_bank_addresses()):
        animation_name = bank_names[bank_address].removeprefix(prefix)
        for facing in range(FACING_VARIANT_COUNT):
            record = records[bank_address + facing * _SPRITE_ANIMATION.size]
            key = (record.tiles_addr, record.frames_addr)
            asset_names.setdefault(key, f"{animation_name}_Facing{facing}")
    return bank_names, asset_names


def _asset_source_entries(carve: Carve) -> list[tuple[int, str]]:
    from rotkit.build.enemy_asset_sources import source_for_group

    return [
        (
            group[0].tiles_addr,
            source_for_group(_asset_group_name(group, carve.asset_names)),
        )
        for group in carve.groups
    ]


def _emit_bank_sources(
    records: dict[int, AnimationRecord],
    bank_addresses: set[int],
    bank_names: dict[int, str],
    asset_names: dict[tuple[int, int], str],
) -> list[tuple[int, str]]:
    entries = []
    for group in _bank_groups(bank_addresses, bank_names):
        lines = [
            '#include "spriteAnimation.h"',
            '#include "variables.h"',
            "",
            "// clang-format off",
            "",
        ]
        for bank_address in group:
            symbol = bank_names[bank_address]
            lines += doc_comment(bank_address)
            lines.append(f"const SpriteAnimation {symbol}[{FACING_VARIANT_COUNT}] = {{")
            for facing in range(FACING_VARIANT_COUNT):
                record = records[bank_address + facing * _SPRITE_ANIMATION.size]
                tile_name, frame_name = _animation_asset_symbols(
                    asset_names[(record.tiles_addr, record.frames_addr)]
                )
                palette = (
                    "NULL"
                    if record.palette_addr == 0
                    else f"(const u16 *)0x{record.palette_addr:08x}"
                )
                lines.append(
                    "    { "
                    f"{tile_name}, "
                    f"&{frame_name}.header, "
                    f"{palette}, "
                    f"{record.frame_duration} }},"
                )
            lines += ["};", ""]
        enemy_type_name, _pose_name = _animation_bank_name_parts(bank_names[group[0]])
        group_name = _enemy_type_source_name(enemy_type_name)
        source = write_table(f"{GROUP}/EnemyAnimationBanks_{group_name}.c", lines)
        entries.append((group[0], source))
    return entries


def _emit_secondary_matrix_sources(
    rom: bytes, bank_names: dict[int, str]
) -> list[tuple[int, str]]:
    entries = []
    row_size = SECONDARY_MATRIX_POSE_COUNT * _U32.size
    for address, symbol, source_name in _secondary_matrix_specs():
        lines = [
            '#include "spriteAnimation.h"',
            '#include "variables.h"',
            "",
            "// clang-format off",
            "",
            *doc_comment(address),
            f"const SpriteAnimation *const {symbol}"
            f"[{SECONDARY_MATRIX_PART_COUNT}]"
            f"[{SECONDARY_MATRIX_POSE_COUNT}] = {{",
        ]
        for part in range(SECONDARY_MATRIX_PART_COUNT):
            row = Struct(f"<{SECONDARY_MATRIX_POSE_COUNT}I").unpack_from(
                rom, address - ROMBASE + part * row_size
            )
            values = ", ".join(
                "NULL" if bank_address == 0 else bank_names[bank_address]
                for bank_address in row
            )
            lines.append(f"    {{ {values} }},")
        lines.append("};")
        source = write_table(f"{GROUP}/EnemyAnimationBankMatrix_{source_name}.c", lines)
        entries.append((address, source))
    return entries


def _emit_table(rom: bytes, bank_names: dict[int, str]) -> str:
    enemy_type_names = _enemy_type_names()
    lines = [
        '#include "spriteAnimation.h"',
        '#include "variables.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(
            TABLE_ADDR,
            [
                "Per-enemy-type animation banks. Each row has one pointer per pose to five",
                "directional SpriteAnimation descriptors.",
            ],
        ),
        f"const SpriteAnimation *const {TABLE}[ENEMY_TYPE_COUNT][ENEMY_ANIMATION_POSE_COUNT] = {{",
    ]
    for enemy_type in range(ENEMY_TYPE_COUNT):
        row = _ANIMATION_ROW.unpack_from(
            rom, TABLE_ADDR - ROMBASE + enemy_type * _ANIMATION_ROW.size
        )
        values = ", ".join("NULL" if value == 0 else bank_names[value] for value in row)
        lines.append(f"    {enum_entry_comment(enemy_type, enemy_type_names)}")
        lines.append(f"    {{ {values} }},")
    lines.append("};")
    return write_table(os.path.join(GROUP, f"{TABLE}.c"), lines)


def _write_reference_sheets(rom: bytes, carve: Carve, captions: bool) -> None:
    enemy_table = data_symbol("EnemyTypeInfo[")
    enemy_type_names = _enemy_type_names()
    sheet_enemy_type_names = tuple(
        name.removeprefix("ENEMY_TYPE_") for name in enemy_type_names
    )
    pose_names = _pose_names()
    pose_stems = [name.lower() for name in pose_names]
    families = _families(rom, enemy_table)
    sheet_families = tuple(family.removeprefix("ENEMY_FAMILY_") for family in families)
    facing_variants = rom[
        FACING_VARIANTS_ADDR - ROMBASE : FACING_VARIANTS_ADDR - ROMBASE + 8
    ]
    _check(
        max(facing_variants) == FACING_VARIANT_COUNT - 1,
        "unexpected facing variant table",
    )

    summary_images = []
    summary_labels = []
    pose_sheet_animations = [[] for _ in range(ANIMATION_BANK_COUNT)]
    pose_sheet_frame_times = [[] for _ in range(ANIMATION_BANK_COUNT)]
    pose_sheet_labels = [[] for _ in range(ANIMATION_BANK_COUNT)]
    metadata = [
        "id\tenum_name\tfamily\tpose\tfacing\tframes\tframe_duration\tanimation_address\tasset"
    ]
    for enemy_type in range(ENEMY_TYPE_COUNT):
        row = _ANIMATION_ROW.unpack_from(
            rom, TABLE_ADDR - ROMBASE + enemy_type * _ANIMATION_ROW.size
        )
        palette = _palette(rom, enemy_type)
        target_summary = None
        for pose, pose_addr in enumerate(row):
            if pose_addr == 0:
                pose_sheet_animations[pose].append((_blank_frame(palette),))
                pose_sheet_frame_times[pose].append((0, 10))
                pose_sheet_labels[pose].append(
                    (
                        f"{enemy_type}",
                        sheet_enemy_type_names[enemy_type],
                        sheet_families[enemy_type],
                        pose_names[pose],
                        "no animation",
                    )
                )
                continue
            for facing in range(FACING_VARIANT_COUNT):
                animation_addr = pose_addr + facing * _SPRITE_ANIMATION.size
                record = carve.records[animation_addr]
                asset_name = carve.asset_names[(record.tiles_addr, record.frames_addr)]
                metadata.append(
                    f"{enemy_type}\t{enemy_type_names[enemy_type]}\t{families[enemy_type]}\t"
                    f"{pose_names[pose]}\t{facing}\t{record.frame_set.frame_count}\t"
                    f"{record.frame_duration}\t0x{record.animation_addr:08x}\t{asset_name}"
                )
            frames = _enemy_animation_frames(
                rom,
                carve.records,
                enemy_type,
                pose,
                3,
            )
            _check(bool(frames), f"enemy type {enemy_type} pose {pose} has no frames")
            record = carve.records[pose_addr + 3 * _SPRITE_ANIMATION.size]
            if record.frame_duration == 0:
                frames = frames[:1]
            pose_sheet_animations[pose].append(frames)
            pose_sheet_frame_times[pose].append(
                _preview_frame_times(len(frames), record.frame_duration)
            )
            pose_sheet_labels[pose].append(
                (
                    f"{enemy_type}",
                    sheet_enemy_type_names[enemy_type],
                    sheet_families[enemy_type],
                    pose_names[pose],
                    "facing 3",
                )
            )
            if pose_names[pose] == "STAND":
                target_summary = frames[0]
        if target_summary is None:
            target_summary = _blank_frame(palette)
        summary_images.append(target_summary)
        summary_labels.append(
            (
                f"{enemy_type}",
                sheet_enemy_type_names[enemy_type],
                sheet_families[enemy_type],
            )
        )
    isolated = [
        enemy_type
        for enemy_type in range(ENEMY_TYPE_COUNT)
        if any(
            max(frame.width, frame.height) > REFERENCE_SHEET_MAX_CELL_EDGE
            for pose_animations in pose_sheet_animations
            for frame in pose_animations[enemy_type]
        )
    ]
    (GFX_DIR / "animations.tsv").write_text("\n".join(metadata) + "\n")
    write_sheet(
        GROUP,
        summary_images,
        summary_labels if captions else None,
        isolated=isolated,
        per_row=True,
    )
    for pose in range(ANIMATION_BANK_COUNT):
        write_animated_sheet(
            f"{GROUP}/{pose_stems[pose]}_facing_3",
            pose_sheet_animations[pose],
            pose_sheet_labels[pose] if captions else None,
            isolated=isolated,
            per_row=True,
            frame_times=pose_sheet_frame_times[pose],
        )


def _preview_frame_times(frame_count: int, frame_duration: int) -> tuple[int, ...]:
    """Round cumulative game ticks (two 60 Hz vblanks each) to GIF centiseconds."""
    if frame_duration == 0:
        return (0, 10)
    return tuple(
        ((index * frame_duration * 10 + 1) // 3) * 10
        for index in range(frame_count + 1)
    )


def _write_animation_previews(rom: bytes, carve: Carve) -> int:
    animation_dir = GFX_SHEETS / GROUP / "animations"
    pose_names = tuple(_pascal_name(name) for name in _pose_names())
    count = 0
    for enemy_type in range(ENEMY_TYPE_COUNT):
        row = _ANIMATION_ROW.unpack_from(
            rom, TABLE_ADDR - ROMBASE + enemy_type * _ANIMATION_ROW.size
        )
        enemy_type_name = _enemy_type_symbol_name(enemy_type)
        directory = animation_dir / _enemy_type_source_name(enemy_type_name)
        directory.mkdir(parents=True, exist_ok=True)
        frames_cache: dict[
            tuple[int, int, tuple[int, ...]], tuple[IndexedImage, ...]
        ] = {}
        for pose, pose_addr in enumerate(row):
            if pose_addr == 0:
                continue
            for facing in range(FACING_VARIANT_COUNT):
                frames = _enemy_animation_frames(
                    rom,
                    carve.records,
                    enemy_type,
                    pose,
                    facing,
                    frames_cache,
                )
                _check(
                    bool(frames),
                    f"enemy type {enemy_type} pose {pose} facing {facing} has no frames",
                )
                name = f"{enemy_type_name}_{pose_names[pose]}_Facing{facing}.gif"
                record = carve.records[pose_addr + facing * _SPRITE_ANIMATION.size]
                if record.frame_duration == 0:
                    frames = frames[:1]
                times = _preview_frame_times(len(frames), record.frame_duration)
                write_animation_gif(
                    str(directory / name),
                    frames,
                    duration=[stop - start for start, stop in zip(times, times[1:])],
                )
                count += 1
    return count


def extract(rom: bytes, captions: bool = True) -> Carve:
    carve = load(rom)
    shutil.rmtree(GFX_DIR, ignore_errors=True)
    shutil.rmtree(GFX_SHEETS / GROUP, ignore_errors=True)
    for group in carve.groups:
        group_name = _asset_group_name(group, carve.asset_names)
        directory = GFX_DIR / group_name
        directory.mkdir(parents=True, exist_ok=True)
        metadata = {}
        for asset in group:
            key = (asset.tiles_addr, asset.frames_addr)
            name = carve.asset_names[key]
            palette = carve.palettes[key]
            tile_size = asset.frames_addr - asset.tiles_addr
            end = _align4(asset.frames_addr + asset.frame_set.size)
            original = rom[asset.tiles_addr - ROMBASE : end - ROMBASE]
            # TODO: Find a natural editable format for occluded pixels and frame layouts.
            metadata[name], images = _asset_metadata_and_images(
                original[:tile_size], asset.frame_set, palette
            )
            for index, image in enumerate(images):
                write_indexed(str(directory / f"{name}_{index:02d}.png"), image)
            rebuilt, offsets = encode_sequence_from_images(
                tile_size,
                asset.frame_set.frames,
                images,
                palette,
                metadata[name]["hidden_nibbles"],
                metadata[name].get("transparent_owners"),
            )
            _check(
                rebuilt == original[:tile_size]
                and offsets
                == tuple(frame.tile_offset for frame in asset.frame_set.frames),
                f"{name}: PNGs and metadata do not reproduce the original tile stream",
            )
        (directory / "metadata.json").write_text(dumps(metadata))
    preview_count = _write_animation_previews(rom, carve)
    _write_reference_sheets(rom, carve, captions)
    print(
        f"  wrote {len(carve.assets)} canonical animations under {os.path.relpath(GFX_DIR, ROOT)}/"
    )
    print(f"  wrote {preview_count} runtime animation previews")
    return carve


def _write_cfg(
    assets: dict[tuple[int, int], AnimationAsset],
    bank_addresses: set[int],
    bank_names: dict[int, str],
    asset_names: dict[tuple[int, int], str],
) -> None:
    rows = [
        (
            address,
            bank_names[address],
            f"SpriteAnimation[{FACING_VARIANT_COUNT}]",
        )
        for address in sorted(bank_addresses)
    ]
    rows += [
        (
            address,
            symbol,
            "SpriteAnimation *const "
            f"[{SECONDARY_MATRIX_PART_COUNT}][{SECONDARY_MATRIX_POSE_COUNT}]",
        )
        for address, symbol, _source_name in _secondary_matrix_specs()
    ]
    for asset in assets.values():
        tiles_size = asset.frames_addr - asset.tiles_addr
        tile_name, frame_name = _animation_asset_symbols(
            asset_names[(asset.tiles_addr, asset.frames_addr)]
        )
        rows.extend(
            [
                (asset.tiles_addr, tile_name, f"u8[0x{tiles_size:x}]"),
                (asset.frames_addr, frame_name, "SpriteFrameSet"),
            ]
        )
    write_cfg(
        CONFIG_GROUP,
        "enemy-graphics",
        "the enemy-related animation banks and tile/frame assets",
        rows,
        data_dirs=[GROUP],
    )


def run(captions: CaptionsOption = True) -> None:
    rom = load_rom()
    carve = extract(rom, captions)
    bank_addresses = set(carve.bank_names)
    entries = [(TABLE_ADDR, _emit_table(rom, carve.bank_names))]
    entries += _asset_source_entries(carve)
    entries += _emit_bank_sources(
        carve.records, bank_addresses, carve.bank_names, carve.asset_names
    )
    entries += _emit_secondary_matrix_sources(rom, carve.bank_names)
    expected_sources = {source for _address, source in entries}
    for path in (CARVED_DATA / GROUP).glob("*.c"):
        relative = str(path.relative_to(ROOT))
        if relative not in expected_sources:
            path.unlink()
    for _address, source in entries:
        print(f"  carved {source}")
    _write_cfg(carve.assets, bank_addresses, carve.bank_names, carve.asset_names)
    upsert_map(
        entries,
        owned_dirs=[GROUP],
        owned_prefixes=["build/generated/data/enemy_graphics/"],
    )
    from rotkit.build.enemy_asset_sources import run as generate_source

    for _address, source in entries:
        if source.startswith("build/generated/data/enemy_graphics/"):
            generate_source(source)
