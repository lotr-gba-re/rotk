"""Carves the three per-base-item icons an ItemGfx row points at:

  backpack  .animation   32x24 OBJ sprite, drawn in the backpack cells
  equip     .equipIcon   32x48 or 32x32 BG asset, drawn in the equipment grid
  hollow    .hollowIcon  32x24 BG asset, drawn in the Ranger Hollow stash cells

A backpack icon is one contiguous ROM record: lz77-compressed tile data (zero padded to 4
bytes), a SpriteIconFrameSet, then a 16-color OBJ palette. An equip/hollow icon is a BG
asset (include/gfx.h). Each variant's records are contiguous in ItemGfx row order within a
category, and the categories abut. The carved .c #includes the byte-list macros `rotkit
build gfx` encodes from the PNGs, so an edited PNG flows into the ROM.
"""

import os
import re
import shutil
from dataclasses import dataclass
from struct import Struct

from rotkit import bgasset
from rotkit.carve import (
    CaptionsOption,
    data_symbol,
    doc_comment,
    entry_comment,
    keep_rom_copy,
    report_rom_copies,
    upsert_map,
    write_cfg,
    write_sheet,
    write_table,
)
from rotkit.carve.item_tables import (
    ITEM_GFX_SIZE,
    ITEM_MOD,
    item_label,
    parse_item_gfx,
    parse_item_mod,
    parse_item_type_infos,
)
from rotkit.cheaders import extract_struct_fields
from rotkit.compression import lz77
from rotkit.paths import GFX_LOCAL, ROOT
from rotkit.png import IndexedImage, bgr555_to_rgb, write_indexed
from rotkit.rom import ROMBASE, load_rom
from rotkit.spritegfx import (
    SPRITE_FRAME_LZ77,
    FrameSet,
    OamTemplate,
    parse_frame_set,
    pixels_to_tiles,
    tiles_to_pixels,
)
from rotkit.stores import data_symbols
from rotkit.textdb import decode_strings

GROUP = "items"
GROUPS = (GROUP,)
GFX_DIR = GFX_LOCAL / GROUP
BACKPACK, EQUIP, HOLLOW = "backpack", "equip", "hollow"
BG_VARIANTS = (EQUIP, HOLLOW)
ICON_WIDTH, ICON_HEIGHT = 32, 24
PALETTE_COLORS = 16
# Every backpack icon frame is a 32x8 OBJ over a 32x16 OBJ (tiles 0-3, then 4-11).
ICON_OAM = (
    OamTemplate(x=0, y=0, obj_size=1, obj_shape=1, tile_offset=0),
    OamTemplate(x=0, y=8, obj_size=2, obj_shape=1, tile_offset=4),
)
# the flags every equip/hollow icon carries; ITEM_BG_ICON spells them out
BG_ICON_FLAGS = (
    bgasset.FLAG_PALETTE_16
    | bgasset.FLAG_MAP
    | bgasset.FLAG_TILES
    | bgasset.CODEC_PUCRUNCH
)

# Icons whose PNG does not rebuild byte for byte (keep_rom_copy), with what differs.
TILER_EXCEPTIONS = {
    "items/armor/equip/17_ArmorImperialHelmEquipIcon": (
        "Map cell (3, 0) holds tile 3, the exact hflip of tile 0 (the frame's top-left "
        "corner), which the tiler emits as an hflip entry everywhere else. Only the "
        "tiling differs, pucrunch re-encodes the icon's own tiles byte for byte."
    ),
}

_SPRITE_ANIMATION = Struct("<IIII")  # tiles, frames, palette, frameDuration + pad
_PALETTE = Struct(f"<{PALETTE_COLORS}H")


@dataclass
class BackpackIcon:
    """One .animation record: the OBJ sprite shown in a backpack cell."""

    category: str  # KnifeGfx -> Knife
    index: int
    label: str  # decoded item name
    symbol: str  # KnifeSpoonBackpackIcon; + Tiles / Frames / Palette
    tiles_addr: int
    frames_addr: int
    palette_addr: int
    lz: bytes  # compressed tile data incl. the zero padding to the frame set
    tiles: bytes  # decoded 4bpp character data
    frame_set: FrameSet
    palette: tuple[int, ...]  # BGR555

    @property
    def addr(self) -> int:
        return self.tiles_addr

    @property
    def end(self) -> int:
        return self.palette_addr + _PALETTE.size

    @property
    def file_stem(self) -> str:
        return (
            f"{GROUP}/{self.category.lower()}/{BACKPACK}/{self.index:02d}_{self.symbol}"
        )

    @property
    def pixels(self) -> bytes:
        return tiles_to_pixels(self.tiles, ICON_OAM, ICON_WIDTH, ICON_HEIGHT)


@dataclass
class BgIcon:
    """One .equipIcon / .hollowIcon BG asset, named after the first row pointing at it."""

    variant: str
    category: str
    index: int  # the naming row's baseIndex
    label: str
    symbol: str  # KnifeSpoonEquipIcon
    addr: int
    asset: bgasset.BgAsset
    shared_with: list[tuple[int, str]]  # further (baseIndex, label) rows reusing it

    @property
    def end(self) -> int:
        return self.addr + self.asset.size

    @property
    def file_stem(self) -> str:
        return f"{GROUP}/{self.category.lower()}/{self.variant}/{self.index:02d}_{self.symbol}"


@dataclass
class Carve:
    """Everything one carve pass found, keyed the way the emitters want it."""

    backpack: dict[str, list[BackpackIcon]]  # by category, ItemGfx row order
    bg: dict[str, dict[str, list[BgIcon]]]  # [variant][category], ROM order


def _slug(label: str) -> str:
    """ "Flint Knife" -> FlintKnife (symbol part); the file stem lowercases the category."""
    words = re.findall(r"[A-Za-z0-9]+", label)
    if not words:
        raise SystemExit(
            f"item name {label!r} has no alphanumeric characters to name it"
        )
    return "".join(w[:1].upper() + w[1:] for w in words)


def _check(cond: bool, what: str) -> None:
    if not cond:
        raise SystemExit(f"item gfx: {what}")


# --- reading the ROM ------------------------------------------------------------


def _read_backpack_icon(
    rom: bytes, category: str, index: int, label: str, gfx_addr: int
) -> BackpackIcon:
    tiles_addr, frames_addr, palette_addr, duration = _SPRITE_ANIMATION.unpack_from(
        rom, gfx_addr - ROMBASE
    )
    where = f"{category}Gfx[{index}] ({label})"
    _check(duration == 0, f"{where}: frameDuration word 0x{duration:08x}, expected 0")
    frame_set = parse_frame_set(rom, frames_addr)
    _check(
        (frame_set.width, frame_set.height, frame_set.frame_count)
        == (ICON_WIDTH, ICON_HEIGHT, 1),
        f"{where}: frame set is {frame_set.width}x{frame_set.height} x{frame_set.frame_count}",
    )
    _check(
        frame_set.field_0x2 == bytes(4) and frame_set.field_0xa == 0,
        f"{where}: unexpected header fields",
    )
    _check(
        frame_set.frame_offsets == (2,),
        f"{where}: frame offsets {frame_set.frame_offsets}",
    )
    frame = frame_set.frames[0]
    _check(
        frame.flags & 0x60 == SPRITE_FRAME_LZ77,
        f"{where}: frame flags 0x{frame.flags:02x}",
    )
    _check(
        frame.field_0x1 == 0 and frame.tile_offset == 0,
        f"{where}: unexpected frame fields",
    )
    _check(
        (frame.width, frame.height) == (ICON_WIDTH, ICON_HEIGHT), f"{where}: frame size"
    )
    _check(
        frame.oam == ICON_OAM,
        f"{where}: OAM templates {frame.oam} differ from ICON_OAM",
    )
    _check(
        frame_set.frame_count == 1 and frame_set.field_0x8 == frame.tile_bytes,
        f"{where}: field_0x8",
    )
    _check(
        frame_set.trigger_box_count <= 1,
        f"{where}: {frame_set.trigger_box_count} trigger boxes",
    )
    # the palette follows the frame set, which is zero padded to 4 bytes
    _check(
        palette_addr == frames_addr + frame_set.size + (-frame_set.size % 4)
        and not any(
            rom[frames_addr - ROMBASE + frame_set.size : palette_addr - ROMBASE]
        ),
        f"{where}: palette not right after the frame set",
    )

    tiles, lz_end = lz77.decode(rom, tiles_addr - ROMBASE)
    lz_end += ROMBASE
    _check(
        len(tiles) == frame.tile_bytes,
        f"{where}: {len(tiles)} tile bytes, frame says {frame.tile_bytes}",
    )
    _check(
        lz_end <= frames_addr < lz_end + 4
        and not any(rom[lz_end - ROMBASE : frames_addr - ROMBASE]),
        f"{where}: frame set at 0x{frames_addr:08x} does not follow the tile data ending at 0x{lz_end:08x}",
    )
    lz = rom[tiles_addr - ROMBASE : frames_addr - ROMBASE]
    _check(
        lz77.encode(tiles) == rom[tiles_addr - ROMBASE : lz_end - ROMBASE],
        f"{where}: the LZ encoder does not reproduce the ROM stream",
    )
    _check(
        pixels_to_tiles(
            tiles_to_pixels(tiles, ICON_OAM, ICON_WIDTH, ICON_HEIGHT),
            ICON_OAM,
            ICON_WIDTH,
            ICON_HEIGHT,
        )
        == tiles,
        f"{where}: tile layout round trip",
    )

    palette = _PALETTE.unpack_from(rom, palette_addr - ROMBASE)
    _check(
        not any(c & 0x8000 for c in palette), f"{where}: palette color with bit 15 set"
    )
    return BackpackIcon(
        category=category,
        index=index,
        label=label,
        symbol=f"{category}{_slug(label)}{BACKPACK.capitalize()}Icon",
        tiles_addr=tiles_addr,
        frames_addr=frames_addr,
        palette_addr=palette_addr,
        lz=lz,
        tiles=tiles,
        frame_set=frame_set,
        palette=palette,
    )


def _read_bg_icon(
    rom: bytes, variant: str, category: str, index: int, label: str, addr: int
) -> BgIcon:
    where = f"{category}Gfx[{index}] ({label}) .{variant}Icon"
    try:
        asset = bgasset.parse(rom, addr - ROMBASE)
    except ValueError as e:
        raise SystemExit(f"item gfx: {where}: {e}") from None
    _check(
        (asset.flags, asset.flags2, len(asset.palette)) == (BG_ICON_FLAGS, 0, 16),
        f"{where}: flags 0x{asset.flags:02x}/0x{asset.flags2:02x} with"
        f" {len(asset.palette)} colors differ from ITEM_BG_ICON's shape",
    )
    _check(
        asset.width * asset.height % 2 == 0,
        f"{where}: {asset.width}x{asset.height} tiles is an odd map; ITEM_BG_ICON sizes"
        " the map array without the pad entry",
    )
    symbol = f"{category}{_slug(label)}{variant.capitalize()}Icon"
    return BgIcon(variant, category, index, label, symbol, addr, asset, [])


def load(rom: bytes) -> Carve:
    """Every ItemGfx row's three icons, checked for the shapes and contiguous layout."""
    names_by_addr = {s.addr: s.name for s in data_symbols()}
    strings = decode_strings(rom)
    backpack: dict[str, list[BackpackIcon]] = {}
    bg: dict[str, dict[str, list[BgIcon]]] = {v: {} for v in BG_VARIANTS}

    for row in parse_item_type_infos(rom, data_symbol("ItemTypeInfo[")):
        category = names_by_addr[row.base_gfx].removesuffix("Gfx")
        icons: list[BackpackIcon] = []
        by_addr: dict[str, dict[int, BgIcon]] = {v: {} for v in BG_VARIANTS}
        for index in range(row.base_item_count & 0xFF):
            label = item_label(
                parse_item_mod(
                    rom, row.base_items - ROMBASE + index * ITEM_MOD.size
                ).name_id,
                strings,
            )
            gfx_addr = row.base_gfx + index * ITEM_GFX_SIZE
            icons.append(_read_backpack_icon(rom, category, index, label, gfx_addr))
            gfx = parse_item_gfx(rom, gfx_addr - ROMBASE)
            for variant, addr in zip(BG_VARIANTS, (gfx.equip_icon, gfx.hollow_icon)):
                if not addr:
                    _check(
                        variant == EQUIP,
                        f"{category}Gfx[{index}] ({label}): .{variant}Icon is NULL",
                    )
                    continue
                if addr in by_addr[variant]:
                    by_addr[variant][addr].shared_with.append((index, label))
                    continue
                by_addr[variant][addr] = _read_bg_icon(
                    rom, variant, category, index, label, addr
                )
        backpack[category] = icons
        for variant in BG_VARIANTS:
            bg[variant][category] = sorted(
                by_addr[variant].values(), key=lambda i: i.addr
            )

    _check_contiguous("backpack", backpack)
    for variant in BG_VARIANTS:
        _check_contiguous(variant, bg[variant])
    symbols = [
        icon.symbol
        for groups in [backpack, *(bg[v] for v in BG_VARIANTS)]
        for group in groups.values()
        for icon in group
    ]
    _check(len(set(symbols)) == len(symbols), "duplicate icon symbol names")
    return Carve(backpack, bg)


def _check_contiguous(variant: str, groups: dict[str, list]) -> None:
    """Each category's records abut in ROM, and the categories abut each other."""
    for category, group in groups.items():
        for prev, icon in zip(group, group[1:]):
            _check(
                icon.addr == prev.end,
                f"{variant}: {category} icon {icon.index} does not follow {prev.index}",
            )
    ordered = sorted((g for g in groups.values() if g), key=lambda g: g[0].addr)
    for prev, group in zip(ordered, ordered[1:]):
        _check(
            group[0].addr == prev[-1].end,
            f"{variant}: {group[0].category} icons do not abut {prev[0].category}",
        )


# --- carved-local: the images ---------------------------------------------------


def _backpack_image(icon: BackpackIcon) -> IndexedImage:
    return IndexedImage(
        ICON_WIDTH,
        ICON_HEIGHT,
        icon.pixels,
        [bgr555_to_rgb(c) for c in icon.palette],
        transparent=0,
    )


def _write_variant(
    category: str, variant: str, images: list[IndexedImage], stems: list[str], captions
) -> None:
    os.makedirs(GFX_DIR / category.lower() / variant, exist_ok=True)
    for stem, image in zip(stems, images):
        write_indexed(str(GFX_LOCAL / f"{stem}.png"), image)
    names = [stem.split("/")[-1] for stem in stems]
    write_sheet(
        f"{GROUP}/{category.lower()}/{variant}", images, names if captions else None
    )


def extract(rom: bytes, captions: bool = True) -> Carve:
    """Replace carved-local/gfx/items with the ROM's PNGs and sheets; returns the icons."""
    carve = load(rom)
    shutil.rmtree(GFX_DIR, ignore_errors=True)
    kept: set[str] = set()
    total = 0
    for category, group in carve.backpack.items():
        images = [_backpack_image(icon) for icon in group]
        _write_variant(
            category, BACKPACK, images, [i.file_stem for i in group], captions
        )
        total += len(group)
    for variant in BG_VARIANTS:
        for category, group in carve.bg[variant].items():
            images = [bgasset.compose(icon.asset) for icon in group]
            _write_variant(
                category, variant, images, [i.file_stem for i in group], captions
            )
            total += len(group)
            for icon, image in zip(group, images):
                keep_rom_copy(
                    icon.file_stem,
                    bgasset.serialize(icon.asset),
                    bgasset.serialize(bgasset.rebuild(image, icon.asset)),
                    TILER_EXCEPTIONS,
                    kept,
                )
    print(f"  wrote {total} icons under {os.path.relpath(GFX_DIR, ROOT)}/")
    report_rom_copies(TILER_EXCEPTIONS, kept)
    return carve


# --- carved/: the C tables and the data store -----------------------------------

# Field names the emitter relies on, checked against the header at carve time.
_EXPECTED_FIELDS = {
    "SpriteFrameSetHeader": [
        "width",
        "height",
        "field_0x2",
        "frameCount",
        "field_0x8",
        "field_0xa",
        "triggerBoxCount",
    ],
    "SpriteFrame": ["flags", "field_0x1", "width", "height", "tileOffset", "tileBytes"],
    "SpriteIconFrameSet": ["header", "frameOffsets", "frame", "oam", "padding"],
    "SpriteIconBoxedFrameSet": [
        "header",
        "frameOffsets",
        "frame",
        "triggerBoxes",
        "oam",
    ],
    "ActorTriggerBox": ["xMin", "xMax", "yMin", "yMax", "enable", "pad"],
}


def _check_header_fields() -> None:
    """The ITEM_BACKPACK_ICON macros spell the frame set out; check the types still match."""
    for type_name, expected in _EXPECTED_FIELDS.items():
        fields = extract_struct_fields("include/spriteAnimation.h", type_name)
        if fields != expected:
            raise SystemExit(
                f"spriteAnimation.h {type_name} fields {fields} differ from the carver's {expected}"
            )
    if extract_struct_fields("include/spriteAnimation.h", "SpriteOamTemplate", "d") != [
        "x",
        "y",
        "objSize",
        "objShape",
        "tileOffset",
    ]:
        raise SystemExit(
            "spriteAnimation.h SpriteOamTemplate.d fields differ from the carver's"
        )


def _icon_comment(icon) -> str:
    """The entry comment; a shared BG icon names the further rows reusing it."""
    comment = entry_comment(icon.index, icon.label)
    shared = getattr(icon, "shared_with", [])
    if shared:
        comment += ", also " + ", ".join(f'[{i}] "{label}"' for i, label in shared)
    return comment


def _backpack_line(icon: BackpackIcon) -> str:
    fs = icon.frame_set
    if fs.trigger_box_count == 0:
        return f"ITEM_BACKPACK_ICON({icon.symbol});"
    box = fs.frames[0].boxes[0]
    return (
        f"ITEM_BACKPACK_ICON_BOXED({icon.symbol}, {box.x_min}, {box.x_max}, {box.y_min},"
        f" {box.y_max}, {'TRUE' if box.enable else 'FALSE'});"
    )


_DESCRIPTIONS = {
    BACKPACK: [
        "{category} backpack icons: per row of {category}Gfx its tiles, frame set and",
        "palette, in ROM order (see ITEM_BACKPACK_ICON).",
    ],
    EQUIP: [
        "{category} equipment-grid icons: the BG asset each row of {category}Gfx points",
        "at, in ROM order (see ITEM_BG_ICON).",
    ],
    HOLLOW: [
        "{category} Ranger Hollow icons: the BG asset each row of {category}Gfx points",
        "at, in ROM order (see ITEM_BG_ICON).",
    ],
}


def _emit_category(category: str, variant: str, group: list) -> tuple[int, str]:
    lines = [
        '#include "item.h"',
        f'#include "gfx/{GROUP}/{category.lower()}/{variant}.inc"',
        "",
        "// clang-format off",
        "",
        *doc_comment(
            group[0].addr,
            [line.format(category=category) for line in _DESCRIPTIONS[variant]],
        ),
    ]
    for icon in group:
        lines.append(_icon_comment(icon))
        if variant == BACKPACK:
            lines.append(_backpack_line(icon))
        else:
            lines.append(
                f"ITEM_BG_ICON({icon.symbol}, {icon.asset.width}, {icon.asset.height});"
            )
    return group[0].addr, write_table(
        os.path.join(GROUP, f"{category}{variant.capitalize()}Icons.c"), lines
    )


def _cfg_rows(carve: Carve) -> list[tuple[int, str, str]]:
    rows = []
    for group in carve.backpack.values():
        for icon in group:
            frames_type = (
                "SpriteIconBoxedFrameSet"
                if icon.frame_set.trigger_box_count
                else "SpriteIconFrameSet"
            )
            rows += [
                (icon.tiles_addr, f"{icon.symbol}Tiles", f"u8[0x{len(icon.lz):x}]"),
                (icon.frames_addr, f"{icon.symbol}Frames", frames_type),
                (icon.palette_addr, f"{icon.symbol}Palette", f"u16[{PALETTE_COLORS}]"),
            ]
    for variant in BG_VARIANTS:
        for group in carve.bg[variant].values():
            for icon in group:
                rows.append((icon.addr, icon.symbol, f"u8[0x{icon.asset.size:x}]"))
    return rows


def _write_cfg(carve: Carve) -> None:
    write_cfg(GROUP, "item-gfx", "the per-base-item icon assets", _cfg_rows(carve))


def run(captions: CaptionsOption = True) -> None:
    _check_header_fields()
    carve = extract(load_rom(), captions)
    entries = []
    for variant in (BACKPACK, *BG_VARIANTS):
        groups = carve.backpack if variant == BACKPACK else carve.bg[variant]
        for category, group in groups.items():
            addr, src = _emit_category(category, variant, group)
            entries.append((addr, src))
            print(f"  carved {src}  ({len(group)} icons)")
    _write_cfg(carve)
    upsert_map(entries, owned_dirs=list(GROUPS))
    print("  updated config/split.cfg. Now run: make verify")
