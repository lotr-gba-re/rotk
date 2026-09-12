"""Carves the level select icons: the SpriteAnimation assets BonusMapMenuEntries and
MultiplayerLevelSelectEntries embed (the chapter select hack draws its grids from them).

A record is a sprites.py record followed by its 16-color palette. Each menu's records abut
in entry order; the multiplayer select's last four entries and its locked-slot template
reuse the bonus map ones.
"""

import os
import re
import shutil
from dataclasses import dataclass
from struct import Struct

from rotkit.carve import (
    CaptionsOption,
    doc_comment,
    upsert_map,
    write_cfg,
    write_sheet,
    write_table,
)
from rotkit.carve.sprites import (
    Frame,
    check,
    file_stem,
    frame_set_end,
    pascal,
    read_frames,
    strip_image,
)
from rotkit.cheaders import extract_enum, extract_struct_fields, invert_enum
from rotkit.paths import GFX_LOCAL, ROOT
from rotkit.png import IndexedImage, write_indexed
from rotkit.rom import ROMBASE, load_rom
from rotkit.spritegfx import FrameSet, cell_oam
from rotkit.stores import DataSym, data_symbols
from rotkit.textdb import decode_strings

GROUP = "level_icons"
GROUPS = (GROUP,)
GFX_DIR = GFX_LOCAL / GROUP
SYMBOL_PREFIX = "LevelIcon"
PALETTE_COLORS = 16
MACRO = "LEVEL_ICON"  # include/menu.h; spells out the shape every icon shares
ICON_WIDTH, ICON_HEIGHT = 32, 64
# category, entry table, row type; the bonus table first so the records both menus share
# carve under the bonus category
TABLES = [
    ("Bonus", "BonusMapMenuEntries", "BonusMapMenuEntry"),
    ("Multiplayer", "MultiplayerLevelSelectEntries", "MultiplayerLevelSelectEntry"),
]
LOCKED_TEMPLATE = "MultiplayerLevelSelectLockedAnimation"

_ENTRY_FIELDS = [
    "animation",
    "iconX",
    "iconY",
    "nameTextId",
    "paletteNumber",
    "unlockMask",
]
_ENTRY = Struct("<IIIB3xHHHHI")
_SPRITE_ANIMATION = Struct("<IIIB3x")
_PALETTE = Struct(f"<{PALETTE_COLORS}H")


@dataclass
class Entry:
    tiles: int
    frames: int
    palette: int
    frame_duration: int
    icon_x: int
    icon_y: int
    name_text_id: int
    palette_number: int
    unlock_mask: int


@dataclass
class Icon:
    category: str
    index: int
    label: str
    symbol: str  # LevelIconMoria; + Tiles / Frames / Palette
    tiles_addr: int
    frames_addr: int
    palette_addr: int
    frame_set: FrameSet
    frames: list[Frame]
    palette: tuple[int, ...]

    @property
    def end(self) -> int:
        return self.palette_addr + _PALETTE.size

    @property
    def file_stem(self) -> str:
        return (
            f"{GROUP}/{self.category.lower()}/"
            f"{file_stem(self.index, self.symbol, len(self.frames))}"
        )


@dataclass
class Carve:
    icons: dict[str, list[Icon]]  # by category, ROM order
    entries: dict[str, list[Entry]]  # by category
    locked: Entry  # the multiplayer select's locked-slot template (animation only)
    by_tiles: dict[int, Icon]


def _symbol(name: str) -> DataSym:
    return next(s for s in data_symbols() if s.name == name)


def _count(sym: DataSym) -> int:
    return int(re.search(r"\[(\d+)\]", sym.type).group(1))


def load(rom: bytes) -> Carve:
    strings = decode_strings(rom)
    icons: dict[str, list[Icon]] = {}
    entries: dict[str, list[Entry]] = {}
    by_tiles: dict[int, Icon] = {}
    for category, table_name, _row_type in TABLES:
        table = _symbol(table_name)
        rows = [
            Entry(*_ENTRY.unpack_from(rom, table.addr - ROMBASE + i * _ENTRY.size))
            for i in range(_count(table))
        ]
        entries[category] = rows
        icons[category] = []
        for i, row in enumerate(rows):
            label = strings[row.name_text_id]
            where = f"level icons: {table_name}[{i}] ({label})"
            check(row.frame_duration == 0, f"{where}: unexpected frame duration")
            if row.tiles in by_tiles:
                icon = by_tiles[row.tiles]
                check(
                    (icon.frames_addr, icon.palette_addr) == (row.frames, row.palette),
                    f"{where}: shares tiles with {icon.symbol} but not the rest",
                )
                continue
            frame_set, frames = read_frames(rom, row.tiles, row.frames, where)
            check(
                (frame_set.width, frame_set.height, len(frames))
                == (ICON_WIDTH, ICON_HEIGHT, 1)
                and frame_set.trigger_box_count == 0
                and frame_set.frames[0].oam == cell_oam(ICON_WIDTH, ICON_HEIGHT),
                f"{where}: frame set differs from {MACRO}'s shape",
            )
            padded_end = frame_set_end(row.frames, frame_set)
            check(
                row.palette == padded_end
                and not any(
                    rom[row.frames - ROMBASE + frame_set.size : padded_end - ROMBASE]
                ),
                f"{where}: palette does not follow the frame set",
            )
            icon = Icon(
                category,
                len(icons[category]),
                label,
                SYMBOL_PREFIX + pascal(label),
                row.tiles,
                row.frames,
                row.palette,
                frame_set,
                frames,
                _PALETTE.unpack_from(rom, row.palette - ROMBASE),
            )
            by_tiles[row.tiles] = icon
            icons[category].append(icon)
    locked_sym = _symbol(LOCKED_TEMPLATE)
    locked = Entry(
        *_SPRITE_ANIMATION.unpack_from(rom, locked_sym.addr - ROMBASE), 0, 0, 0, 0, 0
    )
    check(
        locked.tiles in by_tiles
        and (by_tiles[locked.tiles].frames_addr, by_tiles[locked.tiles].palette_addr)
        == (locked.frames, locked.palette),
        f"level icons: {LOCKED_TEMPLATE} names no entry's record",
    )
    for category, group in icons.items():
        for prev, icon in zip(group, group[1:]):
            check(
                icon.tiles_addr == prev.end,
                f"level icons: {category} icon {icon.index} does not abut {prev.index}",
            )
    symbols = [icon.symbol for group in icons.values() for icon in group]
    check(len(set(symbols)) == len(symbols), "level icons: duplicate symbol names")
    return Carve(icons, entries, locked, by_tiles)


# --- carved-local: the images ---------------------------------------------------


def _image(icon: Icon) -> IndexedImage:
    return strip_image(icon.frame_set, icon.frames, icon.palette)


def extract(rom: bytes, captions: bool = True) -> Carve:
    """Replace carved-local/gfx/level_icons with the ROM's PNGs and sheets."""
    carve = load(rom)
    shutil.rmtree(GFX_DIR, ignore_errors=True)
    for category, group in carve.icons.items():
        os.makedirs(GFX_DIR / category.lower(), exist_ok=True)
        images = [_image(icon) for icon in group]
        for icon, image in zip(group, images):
            write_indexed(str(GFX_LOCAL / f"{icon.file_stem}.png"), image)
        names = [icon.file_stem.split("/")[-1] for icon in group]
        write_sheet(f"{GROUP}/{category.lower()}", images, names if captions else None)
    total = sum(len(g) for g in carve.icons.values())
    print(f"  wrote {total} icons under {os.path.relpath(GFX_DIR, ROOT)}/")
    return carve


# --- carved/: the C tables and the data store -----------------------------------


def _emit_category(category: str, group: list[Icon]) -> tuple[int, str]:
    lines = [
        f'#include "gfx/{GROUP}/{category.lower()}.inc"',
        '#include "menu.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(
            group[0].tiles_addr,
            [
                f"{category} level select icons: per record its tiles, frame set and palette,",
                f"in ROM order (see {MACRO}).",
            ],
        ),
        "",
    ]
    for icon in group:
        lines += [
            f'// [{icon.index}] "{icon.label}"',
            f"{MACRO}({icon.symbol});",
        ]
    return group[0].tiles_addr, write_table(
        os.path.join(GROUP, f"{category}Icons.c"), lines
    )


def _animation_init(icon: Icon) -> str:
    """The `.animation = ...` initializer; every level icon is a still single frame."""
    return f".animation = SPRITE_STILL_ANIMATION_NAMED({icon.symbol}),"


def _unlock_mask(mask: int, flag_names: dict[int, str] | None) -> str:
    """UNLOCK_FLAG_* ored together when every bit has a name, else the hex word."""
    if flag_names and mask:
        bits = [1 << b for b in range(32) if mask & (1 << b)]
        if all(bit in flag_names for bit in bits):
            return " | ".join(flag_names[bit] for bit in bits)
    return f"0x{mask:08x}" if mask else "0"


def _emit_table(
    carve: Carve,
    category: str,
    table_name: str,
    row_type: str,
    text_names: dict[int, str],
) -> tuple[int, str]:
    fields = extract_struct_fields("include/menu.h", row_type)
    check(fields == _ENTRY_FIELDS, f"level icons: {row_type} fields {fields} changed")
    table = _symbol(table_name)
    bonus = category == "Bonus"
    flag_names = (
        invert_enum(extract_enum("include/save.h", "UnlockFlag")) if bonus else None
    )
    description = (
        [
            "Bonus map select entries: icon record, icon position, name and OAM palette",
            "number, and the SaveGlobals unlock flags that reveal the map; [4] is the",
            "locked-slot display.",
        ]
        if bonus
        else [
            "Multiplayer level select entries: icon record, grid position, name and OAM",
            "palette number, and the g_CoopUnlockFlags mask that reveals the level",
            "(0xffffffff = always).",
        ]
    )
    lines = [
        '#include "menu.h"',
        '#include "text_ids.h"',
        '#include "variables.h"',
        *(['#include "save.h"'] if bonus else []),
        "",
        "// clang-format off",
        "",
        *doc_comment(table.addr, description),
        f"const {row_type} {table_name}[{_count(table)}] = {{",
    ]
    for i, row in enumerate(carve.entries[category]):
        icon = carve.by_tiles[row.tiles]
        lines += [
            f'    // [{i}] "{icon.label}"',
            "    {" + _animation_init(icon),
            f"     .iconX = {row.icon_x}, .iconY = {row.icon_y},"
            f" .nameTextId = {text_names[row.name_text_id]},",
            f"     .paletteNumber = {row.palette_number},"
            f" .unlockMask = {_unlock_mask(row.unlock_mask, flag_names)}}},",
        ]
    lines.append("};")
    if not bonus:
        icon = carve.by_tiles[carve.locked.tiles]
        lines += [
            "",
            *doc_comment(
                _symbol(LOCKED_TEMPLATE).addr,
                [
                    "Animation of a locked grid slot, the bonus map select's locked icon."
                ],
            ),
            f"const SpriteAnimation {LOCKED_TEMPLATE} =",
            f"    SPRITE_STILL_ANIMATION_NAMED({icon.symbol});",
        ]
    return table.addr, write_table(os.path.join(GROUP, f"{table_name}.c"), lines)


def _write_cfg(carve: Carve) -> None:
    rows = []
    for group in carve.icons.values():
        for icon in group:
            size = sum(len(f.chunk) for f in icon.frames)
            rows += [
                (icon.tiles_addr, f"{icon.symbol}Tiles", f"u8[0x{size:x}]"),
                (icon.frames_addr, f"{icon.symbol}Frames", "SpriteFrameSetHeader"),
                (icon.palette_addr, f"{icon.symbol}Palette", f"u16[{PALETTE_COLORS}]"),
            ]
    write_cfg(GROUP, "level-icons", "the level select icons", rows)


def run(captions: CaptionsOption = True) -> None:
    carve = extract(load_rom(), captions)
    text_names = invert_enum(extract_enum("carved/include/text_ids.h", "TextId"))
    entries = [_emit_category(c, g) for c, g in carve.icons.items()]
    entries += [
        _emit_table(carve, category, table_name, row_type, text_names)
        for category, table_name, row_type in TABLES
    ]
    for _addr, src in entries:
        print(f"  carved {src}")
    _write_cfg(carve)
    upsert_map(entries, owned_dirs=list(GROUPS))
    print("  updated config/split.cfg. Now run: make verify")
