"""Carves the ground drop sprites: the assets LootPileAnimations points at.

A drop's record is one sprites.py record without a palette: a drop draws with the fixed OBJ
palette slot actor_applyTypeDefaults picks by LootType, whose base palette
(ObjPalettes[slot]) the PNG gets for viewing. The records abut in LootType order; some
LootTypes share one.
"""

import os
import re
import shutil
from dataclasses import dataclass
from struct import Struct

from rotkit.carve import (
    CaptionsOption,
    data_symbol,
    doc_comment,
    macro_lines,
    upsert_map,
    write_cfg,
    write_sheet,
    write_table,
)
from rotkit.carve.item_tables import (
    ITEM_MOD,
    item_label,
    loot_type_names,
    parse_item_gfx,
    parse_item_mod,
    parse_item_type_infos,
)
from rotkit.carve.sprites import (
    Frame,
    check,
    file_stem,
    frame_set_lines,
    pascal,
    read_frames,
    strip_image,
)
from rotkit.cheaders import extract_enum
from rotkit.paths import GFX_LOCAL, ROOT
from rotkit.png import IndexedImage, write_indexed
from rotkit.rom import ROMBASE, load_rom
from rotkit.spritegfx import FrameSet
from rotkit.stores import data_symbols
from rotkit.textdb import decode_strings

GROUP = "loot_piles"
GROUPS = (GROUP,)
GFX_DIR = GFX_LOCAL / GROUP
SYMBOL_PREFIX = "LootPile"
PALETTE_COLORS = 16
TABLE = "LootPileAnimations"
MACRO = "LOOT_PILE_ANIMATION"  # the shape every row shares, defined in the carved file

_SPRITE_ANIMATION = Struct("<IIIB3x")  # tiles, frames, palette, frameDuration + pad
_PALETTE = Struct(f"<{PALETTE_COLORS}H")
_U32 = Struct("<I")
_ITEM_GFX_SIZE = 0x1C

# OBJ palette slot actor_applyTypeDefaults gives an ACTOR_TYPE_LOOT_PILE actor by LootType;
# every other LootType takes slot 4.
_PALETTE_SLOT_OVERRIDES = {
    3: {
        0,
        1,
        2,
        4,
        5,
        8,
        0xA,
        0xB,
        0xC,
        0x19,
        0x1A,
        0x1C,
        0x24,
        0x26,
        0x29,
        0x2B,
        0x2D,
        0x2E,
        0x30,
        0x32,
        0x42,
        0x4B,
        0x54,
    },
    5: {
        0x1B,
        0x1D,
        0x1E,
        0x1F,
        0x22,
        0x23,
        0x25,
        0x2C,
        0x34,
        0x3D,
        0x3F,
        0x45,
        0x48,
        0x4A,
        0x4C,
        0x4D,
        0x4E,
    },
    6: {3, 0x3A},
    7: {9, 0x3C},
}
_DEFAULT_PALETTE_SLOT = 4


@dataclass
class LootPile:
    loot_types: list[int]  # every LootType naming this record, ascending
    labels: list[str]  # per loot_types entry
    symbol: str  # LootPileSting; + Tiles / Frames
    tiles_addr: int
    frames_addr: int
    frame_set: FrameSet
    frames: list[Frame]
    frame_duration: int
    palette_slot: int
    palette: tuple[int, ...]  # BGR555, the slot's base palette

    @property
    def index(self) -> int:
        return self.loot_types[0]

    @property
    def end(self) -> int:
        return self.frames_addr + self.frame_set.size

    @property
    def file_stem(self) -> str:
        return f"{GROUP}/{file_stem(self.index, self.symbol, len(self.frames))}"


def _names(rom: bytes, count: int) -> dict[int, tuple[str, str]]:
    """LootType -> (label, symbol part): the drop's text name where the game has one, the
    base item's name for the named-unique band, else the LootType enumerator."""
    strings = decode_strings(rom)
    text_h = open(ROOT / "include" / "text.h").read()
    base_name = re.search(r"#define TEXT_ID_LOOT_NAME_BASE (\w+)", text_h).group(1)
    text_base = extract_enum("carved/include/text_ids.h", "TextId")[base_name]
    enum_names = loot_type_names()
    item_min = min(v for v, k in enum_names.items() if k == "LOOT_TYPE_BOW_TIER_1")
    item_names: dict[int, str] = {}
    for row in parse_item_type_infos(rom, data_symbol("ItemTypeInfo[")):
        for i in range(row.base_item_count & 0xFF):
            gfx = parse_item_gfx(rom, row.base_gfx - ROMBASE + i * _ITEM_GFX_SIZE)
            if gfx.loot_type in enum_names:
                continue
            mod = parse_item_mod(rom, row.base_items - ROMBASE + i * ITEM_MOD.size)
            item_names.setdefault(gfx.loot_type, item_label(mod.name_id, strings))
    names = {}
    for loot_type in range(count):
        if loot_type in enum_names:
            symbol = pascal(enum_names[loot_type].removeprefix("LOOT_TYPE_"))
            label = enum_names[loot_type]
            if loot_type < item_min:
                label = strings[text_base + loot_type]
        elif loot_type in item_names:
            label = item_names[loot_type]
            symbol = pascal(label)
        else:
            label = strings[text_base + loot_type]
            symbol = pascal(label)
        names[loot_type] = (label, symbol)
    return names


def load_piles(rom: bytes) -> list[LootPile]:
    """Every LootPileAnimations record in ROM order, checked for the contiguous layout."""
    table = next(s for s in data_symbols("SpriteAnimation[") if s.name == TABLE)
    count = int(re.search(r"\[(\d+)\]", table.type).group(1))
    names = _names(rom, count)
    obj_palettes = next(s for s in data_symbols() if s.name == "ObjPalettes")
    by_tiles: dict[int, LootPile] = {}
    for loot_type in range(count):
        tiles_addr, frames_addr, palette_addr, duration = _SPRITE_ANIMATION.unpack_from(
            rom, table.addr - ROMBASE + loot_type * _SPRITE_ANIMATION.size
        )
        label, symbol = names[loot_type]
        where = f"loot piles: LootType 0x{loot_type:02x} ({label})"
        check(palette_addr == 0, f"{where}: has a palette")
        if tiles_addr in by_tiles:
            pile = by_tiles[tiles_addr]
            check(
                pile.frames_addr == frames_addr and pile.frame_duration == duration,
                f"{where}: shares tiles with 0x{pile.index:02x} but not the rest",
            )
            pile.loot_types.append(loot_type)
            pile.labels.append(label)
            continue
        frame_set, frames = read_frames(rom, tiles_addr, frames_addr, where)
        check(frame_set.trigger_box_count == 1, f"{where}: no trigger box")
        slot = next(
            (s for s, types in _PALETTE_SLOT_OVERRIDES.items() if loot_type in types),
            _DEFAULT_PALETTE_SLOT,
        )
        palette_ptr = _U32.unpack_from(rom, obj_palettes.addr - ROMBASE + slot * 4)[0]
        by_tiles[tiles_addr] = LootPile(
            [loot_type],
            [label],
            SYMBOL_PREFIX + symbol,
            tiles_addr,
            frames_addr,
            frame_set,
            frames,
            duration,
            slot,
            _PALETTE.unpack_from(rom, palette_ptr - ROMBASE),
        )
    piles = sorted(by_tiles.values(), key=lambda p: p.tiles_addr)
    for prev, pile in zip(piles, piles[1:]):
        check(
            pile.tiles_addr == prev.end,
            f"loot piles: LootType 0x{pile.index:02x} does not abut 0x{prev.index:02x}",
        )
    symbols = [p.symbol for p in piles]
    check(len(set(symbols)) == len(symbols), "loot piles: duplicate symbol names")
    return piles


# --- carved-local: the images ---------------------------------------------------


def _image(pile: LootPile) -> IndexedImage:
    return strip_image(pile.frame_set, pile.frames, pile.palette)


def extract(rom: bytes, captions: bool = True) -> list[LootPile]:
    """Replace carved-local/gfx/loot_piles with the ROM's PNGs and sheet; returns the piles."""
    piles = load_piles(rom)
    shutil.rmtree(GFX_DIR, ignore_errors=True)
    os.makedirs(GFX_DIR, exist_ok=True)
    images = [_image(pile) for pile in piles]
    for pile, image in zip(piles, images):
        write_indexed(str(GFX_LOCAL / f"{pile.file_stem}.png"), image)
    names = [pile.file_stem.split("/")[-1] for pile in piles]
    write_sheet(GROUP, images, names if captions else None)
    print(f"  wrote {len(piles)} drops under {os.path.relpath(GFX_DIR, ROOT)}/")
    return piles


# --- carved/: the C tables and the data store -----------------------------------


def _entry_comment(pile: LootPile) -> str:
    also = ", ".join(f"0x{t:02x}" for t in pile.loot_types[1:])
    shared = f" (also {also})" if also else ""
    return f'// [0x{pile.index:02x}] "{pile.labels[0]}"{shared}'


def _emit_records(piles: list[LootPile]) -> tuple[int, str]:
    lines = [
        f'#include "gfx/{GROUP}.inc"',
        '#include "spriteAnimation.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(
            piles[0].tiles_addr,
            [
                "Ground drop sprites: per LootPileAnimations record its tiles (per frame, LZ77",
                "where smaller) and frame set, in ROM order. No palette: a drop draws with the",
                "fixed OBJ palette slot actor_applyTypeDefaults picks by LootType.",
            ],
        ),
        "",
    ]
    for pile in piles:
        lines += [
            _entry_comment(pile),
            f"const u8 {pile.symbol}Tiles[] = {{{pile.symbol}_TILES}};",
            *frame_set_lines(pile.symbol, pile.frame_set),
            "",
        ]
    return piles[0].tiles_addr, write_table(os.path.join(GROUP, "LootPiles.c"), lines)


def _emit_animations(piles: list[LootPile]) -> tuple[int, str]:
    table = next(s for s in data_symbols("SpriteAnimation[") if s.name == TABLE)
    count = int(re.search(r"\[(\d+)\]", table.type).group(1))
    by_type = {
        t: (pile, label)
        for pile in piles
        for t, label in zip(pile.loot_types, pile.labels)
    }
    lines = [
        '#include "spriteAnimation.h"',
        '#include "variables.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(
            table.addr,
            [
                "Ground drop sprite per LootType: its LootPiles.c tiles and frame set (shared where",
                "drops look alike), no palette, and the frame duration.",
            ],
        ),
        "",
        *macro_lines(
            f"{MACRO}(name, frameDuration_)",
            [
                "    {.tiles = name##Tiles, .frames = &name##Frames, .palette = NULL,",
                "     .frameDuration = frameDuration_}",
            ],
        ),
        "",
        f"const SpriteAnimation {TABLE}[{count}] = {{",
    ]
    for loot_type in range(count):
        pile, label = by_type[loot_type]
        lines += [
            f'    // [0x{loot_type:02x}] "{label}"',
            f"    {MACRO}({pile.symbol}, {pile.frame_duration}),",
        ]
    lines.append("};")
    return table.addr, write_table(os.path.join(GROUP, f"{TABLE}.c"), lines)


def _write_cfg(piles: list[LootPile]) -> None:
    rows = []
    for pile in piles:
        size = sum(len(f.chunk) for f in pile.frames)
        rows += [
            (pile.tiles_addr, f"{pile.symbol}Tiles", f"u8[0x{size:x}]"),
            (pile.frames_addr, f"{pile.symbol}Frames", "SpriteFrameSetHeader"),
        ]
    write_cfg(GROUP, "loot-piles", "the ground drop sprites", rows)


def run(captions: CaptionsOption = True) -> None:
    piles = extract(load_rom(), captions)
    entries = [_emit_records(piles), _emit_animations(piles)]
    for _addr, src in entries:
        print(f"  carved {src}")
    _write_cfg(piles)
    upsert_map(entries, owned_dirs=list(GROUPS))
    print("  updated config/split.cfg. Now run: make verify")
