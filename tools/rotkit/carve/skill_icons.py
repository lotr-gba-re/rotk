"""Carves the skill-tree icons and their pointer tables.

PassiveSkillId indexes 35 shared icons. Active skill icons are six pointers per hero in
the same ROM order as the active skill tables. Both kinds are raw 32x24 4bpp BG tile
assets without an embedded palette or tilemap; the skill tree draws them with the shared
palette from the hero frame assets.
"""

import os
import shutil
from dataclasses import dataclass
from struct import Struct

from rotkit import bgasset
from rotkit.carve import (
    CaptionsOption,
    doc_comment,
    skill_tables,
    upsert_map,
    write_cfg,
    write_sheet,
    write_table,
)
from rotkit.carve.sprites import check, pascal
from rotkit.cheaders import extract_enum, invert_enum
from rotkit.paths import GFX_LOCAL, GFX_SHEETS, ROOT
from rotkit.png import (
    IndexedImage,
    bgr555_to_rgb,
    rgb_to_bgr555,
    write_indexed,
)
from rotkit.rom import ROMBASE, load_rom
from rotkit.textdb import decode_strings

DATA_DIR = "skills"
GFX_GROUP = "skills/icons"
CONFIG_GROUP = "skill_icons"
GROUPS = (GFX_GROUP,)
DATA_GROUPS = (DATA_DIR,)
GFX_DIR = GFX_LOCAL / GFX_GROUP
PASSIVE_DIR = "passive"
ACTIVE_DIR = "active"
_OUTPUT_FILES = (
    f"carved/data/{DATA_DIR}/SkillIcons.c",
    f"carved/data/{DATA_DIR}/SkillIconAssets.c",
)
_OWNED_FILES = [
    *_OUTPUT_FILES,
    "carved/data/skill_icons/SkillIcons.c",
    "carved/data/skill_icons/SkillIconAssets.c",
]
PASSIVE_POINTERS_ADDR = 0x082876B0
ACTIVE_POINTERS_ADDR = 0x0828773C
SKILL_TREE_FRAME_POINTERS_ADDR = 0x08282B94
PASSIVE_POINTERS = "PassiveSkillIconAssets"
ACTIVE_POINTERS = "ActiveSkillIconAssets"
ICON_WIDTH, ICON_HEIGHT = 32, 24
PALETTE_COLORS = 16
ICON_TILE_BYTES = ICON_WIDTH * ICON_HEIGHT // 2
ICON_ASSET_SIZE = 4 + ICON_TILE_BYTES
# TODO: Source this from the icon-frame assets when the skill scene is decompiled.
SHARED_PALETTE = (
    0xE660,
    0xFFFF,
    0xAB7E,
    0x82BF,
    0x8238,
    0x1916,
    0x00AA,
    0x8002,
    0xDFB6,
    0xC32C,
    0x32A6,
    0xADE4,
    0x2122,
    0x9C20,
    0x3E6F,
    0x3E6F,
)

_U16 = Struct("<H")
_U32 = Struct("<I")
_PALETTE = Struct(f"<{PALETTE_COLORS}H")


@dataclass(frozen=True)
class SkillIcon:
    """One physical raw-tile asset, named after its first pointer-table use."""

    index: int
    file_index: int
    label: str
    symbol: str
    addr: int
    tiles: bytes
    hero: str | None = None

    @property
    def end(self) -> int:
        return self.addr + ICON_ASSET_SIZE

    @property
    def file_stem(self) -> str:
        directory = PASSIVE_DIR if self.hero is None else ACTIVE_DIR
        return f"{GFX_GROUP}/{directory}/{self.file_index:02d}_{self.symbol}"


@dataclass(frozen=True)
class Carve:
    passive: list[SkillIcon]
    active: dict[str, list[SkillIcon]]
    records: list[SkillIcon]


def _pointer(rom: bytes, table_addr: int, index: int) -> int:
    return _U32.unpack_from(rom, table_addr - ROMBASE + index * _U32.size)[0]


def _read_tiles(rom: bytes, addr: int, where: str) -> bytes:
    offset = addr - ROMBASE
    flags, flags2 = rom[offset : offset + 2]
    check(
        (flags, flags2) == (bgasset.FLAG_TILES, 0),
        f"skill icons: {where}: expected raw tile-only asset, got flags "
        f"{flags:#04x}/{flags2:#04x}",
    )
    size = _U16.unpack_from(rom, offset + 2)[0]
    check(
        size == ICON_TILE_BYTES,
        f"skill icons: {where}: {size:#x} tile bytes, expected {ICON_TILE_BYTES:#x}",
    )
    return rom[offset + 4 : offset + 4 + size]


def _check_shared_palette(rom: bytes) -> None:
    """Check the palette embedded in each hero's skill-tree frame asset."""
    expected_flags = bgasset.FLAG_PALETTE_16 | bgasset.FLAG_MAP | bgasset.FLAG_TILES
    for hero_id in range(len(skill_tables.HEROES_MEMORY_ORDER)):
        addr = _pointer(rom, SKILL_TREE_FRAME_POINTERS_ADDR, hero_id)
        offset = addr - ROMBASE
        flags, flags2 = rom[offset : offset + 2]
        check(
            (flags, flags2) == (expected_flags, 0),
            f"skill icons: HeroId {hero_id} frame asset has flags "
            f"{flags:#04x}/{flags2:#04x}",
        )
        palette = _PALETTE.unpack_from(rom, offset + 2)
        check(
            palette == SHARED_PALETTE,
            f"skill icons: HeroId {hero_id} frame has an unexpected palette",
        )


def _passive_labels(rom: bytes, count: int) -> list[str]:
    strings = decode_strings(rom)
    text_ids = extract_enum("carved/include/text_ids.h", "TextId")
    base = text_ids["TEXT_ID_SKILL_FEARLESS"]
    return [strings[base + i].split(" (Level", 1)[0] for i in range(count)]


def load(rom: bytes) -> Carve:
    passive_count = extract_enum("include/skill.h", "PassiveSkillId")[
        "PASSIVE_SKILL_COUNT"
    ]
    check(
        ACTIVE_POINTERS_ADDR == PASSIVE_POINTERS_ADDR + passive_count * _U32.size,
        "skill icons: active pointer table no longer follows the passive pointer table",
    )

    passive = []
    records = []
    for skill_id, label in enumerate(_passive_labels(rom, passive_count)):
        addr = _pointer(rom, PASSIVE_POINTERS_ADDR, skill_id)
        icon = SkillIcon(
            index=skill_id,
            file_index=skill_id,
            label=label,
            symbol=f"PassiveSkill{pascal(label)}Icon",
            addr=addr,
            tiles=_read_tiles(rom, addr, f"PassiveSkillId {skill_id} ({label})"),
        )
        passive.append(icon)
        records.append(icon)

    active = {}
    labels = skill_tables.skill_labels(rom)
    by_addr: dict[int, SkillIcon] = {}
    for hero_index, hero in enumerate(skill_tables.HEROES_MEMORY_ORDER):
        active[hero] = []
        for row, label in enumerate(labels[hero]):
            pointer_index = hero_index * skill_tables.ROW_COUNT + row
            addr = _pointer(rom, ACTIVE_POINTERS_ADDR, pointer_index)
            icon = by_addr.get(addr)
            if icon is None:
                icon = SkillIcon(
                    index=row,
                    file_index=len(by_addr),
                    label=label,
                    symbol=f"ActiveSkill{hero}{pascal(label)}Icon",
                    addr=addr,
                    tiles=_read_tiles(
                        rom, addr, f"{hero} active skill {row} ({label})"
                    ),
                    hero=hero,
                )
                by_addr[addr] = icon
                records.append(icon)
            active[hero].append(icon)

    records.sort(key=lambda icon: icon.addr)
    for previous, icon in zip(records, records[1:]):
        check(
            icon.addr == previous.end,
            f"skill icons: {icon.symbol} does not abut {previous.symbol}",
        )
    check(
        len({icon.symbol for icon in records}) == len(records),
        "skill icons: duplicate symbol names",
    )
    _check_shared_palette(rom)
    return Carve(passive, active, records)


def _image(icon: SkillIcon) -> IndexedImage:
    asset = bgasset.BgAsset(
        bgasset.FLAG_PALETTE_16 | bgasset.FLAG_TILES,
        0,
        SHARED_PALETTE,
        0,
        0,
        (),
        icon.tiles,
        b"",
    )
    return bgasset.compose(asset, ICON_WIDTH // 8)


def remap_to_shared_palette(image: IndexedImage) -> IndexedImage:
    """Map the PNG's used colors to the fixed skill-tree palette."""
    canonical = [color & 0x7FFF for color in SHARED_PALETTE]
    mapping = {}
    for index in set(image.pixels):
        if index >= len(image.palette):
            raise ValueError(f"pixel index {index} is absent from the PNG palette")
        color = rgb_to_bgr555(image.palette[index]) & 0x7FFF
        matches = [i for i, candidate in enumerate(canonical) if candidate == color]
        if not matches:
            rgb = image.palette[index]
            raise ValueError(
                f"palette index {index} color {rgb} is not in the shared skill palette"
            )
        target = index if index in matches else matches[0]
        if (index == 0) != (target == 0):
            raise ValueError(
                "palette remapping must preserve hardware-transparent index 0"
            )
        mapping[index] = target
    pixels = bytes(mapping[index] for index in image.pixels)
    return IndexedImage(
        image.width,
        image.height,
        pixels,
        [bgr555_to_rgb(color) for color in SHARED_PALETTE],
        image.transparent,
    )


def rebuild_tiles(image: IndexedImage) -> bytes:
    """Convert a normalized icon image to raw row-major BG tiles."""
    template = bgasset.BgAsset(
        bgasset.FLAG_TILES,
        0,
        (),
        0,
        0,
        (),
        b"",
        b"",
    )
    return bgasset.rebuild(image, template, ICON_WIDTH // 8).tiles


def extract(rom: bytes, captions: bool = True) -> Carve:
    """Replace carved-local/gfx/skills/icons with the ROM's PNGs and sheets."""
    carve = load(rom)
    shutil.rmtree(GFX_LOCAL / "skill_icons", ignore_errors=True)
    shutil.rmtree(GFX_SHEETS / "skill_icons", ignore_errors=True)
    shutil.rmtree(GFX_DIR, ignore_errors=True)
    shutil.rmtree(GFX_SHEETS / GFX_GROUP, ignore_errors=True)
    images = {icon.addr: _image(icon) for icon in carve.records}
    for icon in carve.records:
        image = images[icon.addr]
        check(
            rebuild_tiles(remap_to_shared_palette(image)) == icon.tiles,
            f"skill icons: {icon.symbol} PNG does not rebuild its raw tiles",
        )
        png = GFX_LOCAL / f"{icon.file_stem}.png"
        os.makedirs(png.parent, exist_ok=True)
        write_indexed(str(png), image)

    active_records = [icon for icon in carve.records if icon.hero is not None]
    for directory, icons in (
        (PASSIVE_DIR, carve.passive),
        (ACTIVE_DIR, active_records),
    ):
        names = [icon.file_stem.split("/")[-1] for icon in icons]
        write_sheet(
            f"{GFX_GROUP}/{directory}",
            [images[icon.addr] for icon in icons],
            names if captions else None,
        )
    print(f"  wrote {len(carve.records)} icons under {os.path.relpath(GFX_DIR, ROOT)}/")
    return carve


def _record_comment(carve: Carve, icon: SkillIcon) -> str:
    if icon.hero is None:
        return f'// [{icon.index}] "{icon.label}"'
    shared = [
        hero
        for hero, icons in carve.active.items()
        if hero != icon.hero and icon in icons
    ]
    also = f" (also {', '.join(shared)})" if shared else ""
    return f'// {icon.hero}[{icon.index}] "{icon.label}"{also}'


def _emit_records(carve: Carve) -> tuple[int, str]:
    includes = sorted(
        {
            f'#include "gfx/{os.path.dirname(icon.file_stem)}.inc"'
            for icon in carve.records
        }
    )
    lines = [
        '#include "gfx.h"',
        *includes,
        "",
        "// clang-format off",
        "",
        *doc_comment(
            carve.records[0].addr,
            [
                "Passive and active skill icons: raw 32x24 4bpp BG tiles without a",
                "palette or tilemap, in ROM order.",
            ],
        ),
        "",
    ]
    for icon in carve.records:
        lines += [
            _record_comment(carve, icon),
            f"BG_ASSET_TILES_ONLY({icon.symbol}, BG_ASSET_TILES | BG_ASSET_CODEC_RAW);",
        ]
    return carve.records[0].addr, write_table(
        os.path.join(DATA_DIR, "SkillIcons.c"), lines
    )


def _emit_tables(carve: Carve) -> tuple[int, str]:
    passive_names = invert_enum(extract_enum("include/skill.h", "PassiveSkillId"))
    lines = [
        '#include "skill.h"',
        '#include "variables.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(
            PASSIVE_POINTERS_ADDR,
            ["Raw 32x24 icon asset per PassiveSkillId."],
        ),
        f"const void *const {PASSIVE_POINTERS}[PASSIVE_SKILL_COUNT] = {{",
    ]
    for skill_id, icon in enumerate(carve.passive):
        lines.append(f"    [{passive_names[skill_id]}] = {icon.symbol},")
    lines += [
        "};",
        "",
        *doc_comment(
            ACTIVE_POINTERS_ADDR,
            [
                "Six raw 32x24 active skill icon assets per hero, in active skill table",
                "ROM order rather than HeroId order.",
            ],
        ),
        f"const void *const {ACTIVE_POINTERS}[{len(carve.active)}]"
        "[HERO_ACTIVE_SKILL_COUNT] = {",
    ]
    for hero, icons in carve.active.items():
        lines.append(f"    // {hero}")
        lines.append("    {" + ", ".join(icon.symbol for icon in icons) + "},")
    lines.append("};")
    return PASSIVE_POINTERS_ADDR, write_table(
        os.path.join(DATA_DIR, "SkillIconAssets.c"), lines
    )


def _write_cfg(carve: Carve) -> None:
    rows = [
        (icon.addr, icon.symbol, f"u8[0x{ICON_ASSET_SIZE:x}]") for icon in carve.records
    ]
    rows += [
        (
            PASSIVE_POINTERS_ADDR,
            PASSIVE_POINTERS,
            f"void *const [{len(carve.passive)}]",
        ),
        (
            ACTIVE_POINTERS_ADDR,
            ACTIVE_POINTERS,
            f"void *const [{len(carve.active)}][{skill_tables.ROW_COUNT}]",
        ),
    ]
    write_cfg(
        CONFIG_GROUP,
        "skill-icons",
        "the passive and active skill icons",
        rows,
        data_dirs=[DATA_DIR],
    )


def run(captions: CaptionsOption = True) -> None:
    carve = extract(load_rom(), captions)
    entries = [_emit_records(carve), _emit_tables(carve)]
    for _addr, src in entries:
        print(f"  carved {src}")
    _write_cfg(carve)
    upsert_map(entries, owned_files=_OWNED_FILES)
    print("  updated config/split.cfg. Now run: make verify")
