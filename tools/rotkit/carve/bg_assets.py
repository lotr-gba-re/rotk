"""Carves the BG assets drawn as whole pictures: the stat screen hero portraits, the
in-game dialog speaker pictures and the cutscene pages, with the tables that point at
them.

An asset is a palette, an optional tilemap and pucrunch-compressed tiles (include/gfx.h).
`rotkit build gfx` rebuilds an asset from its PNG alone; the shape a PNG cannot carry
(flags, palette size, map size) is CATEGORIES below. Each category's assets abut in ROM.
"""

import os
import re
import shutil
import textwrap
from dataclasses import dataclass
from struct import Struct

from rotkit import bgasset
from rotkit.carve import (
    COLUMNS,
    CaptionsOption,
    TableSymbol,
    doc_comment,
    macro_lines,
    keep_rom_copy,
    report_rom_copies,
    upsert_map,
    write_cfg,
    write_sheet,
    write_table,
)
from rotkit.carve.sprites import check
from rotkit.cheaders import extract_enum, extract_struct_fields, invert_enum
from rotkit.paths import GFX_LOCAL, ROOT
from rotkit.png import IndexedImage, write_indexed
from rotkit.rom import ROMBASE, load_rom
from rotkit.stores import data_symbols
from rotkit.textdb import decode_strings

PORTRAITS, DIALOG, CUTSCENES = "hero_portraits", "dialog_portraits", "cutscenes"
GROUPS = (PORTRAITS, DIALOG, CUTSCENES)


@dataclass(frozen=True)
class Category:
    """One group of BG assets, and the asset shape a PNG cannot carry."""

    prefix: str  # symbol prefix
    macro: str  # name of the per-category shape macro the carved file defines
    carved_file: str
    what: str  # noun phrase naming the assets, for the data store header
    description: list[str]
    flags: int
    colors: int  # palette entries
    width: int = 0  # map size in tiles; 0 without a map
    height: int = 0
    tiles_wide: int = 0  # tiles per row of a map-less asset
    flags2: int = 0

    def template(self) -> bgasset.BgAsset:
        """An empty asset of this shape, the template `bgasset.rebuild` fills in."""
        return bgasset.BgAsset(
            self.flags,
            self.flags2,
            (0,) * self.colors,
            self.width,
            self.height,
            (),
            b"",
            b"",
        )


_CODEC = bgasset.FLAG_TILES | bgasset.CODEC_PUCRUNCH
CATEGORIES = {
    PORTRAITS: Category(
        "HeroPortrait",
        "HERO_PORTRAIT",
        "HeroPortraits.c",
        "stat screen hero portraits",
        [
            "Stat screen hero portraits: 8x6 tiles, 8bpp, colors 0x60 and up of a 256-color",
            "palette, in HeroId order.",
        ],
        bgasset.FLAG_PALETTE_256 | bgasset.FLAG_MAP | _CODEC | bgasset.FLAG_8BPP,
        256,
        width=8,
        height=6,
    ),
    DIALOG: Category(
        "DialogPortrait",
        "DIALOG_PORTRAIT",
        "DialogPortraits.c",
        "in-game dialog speaker pictures",
        [
            "In-game dialog speaker pictures: 7x8 tiles, 16 colors, no map, in ROM order."
        ],
        bgasset.FLAG_PALETTE_16 | _CODEC,
        16,
        tiles_wide=7,
    ),
    CUTSCENES: Category(
        "CutscenePage",
        "CUTSCENE_PICTURE",
        "CutscenePictures.c",
        "cutscene pictures",
        [
            "Cutscene pictures: 30x20 tiles over 8 palette banks, named by the first",
            "CutscenePages index showing them, in ROM order.",
        ],
        bgasset.FLAG_PALETTE_256 | bgasset.FLAG_MAP | _CODEC,
        256,
        width=30,
        height=20,
    ),
}

# Assets whose PNG does not rebuild byte for byte (keep_rom_copy), with what differs.
TILER_EXCEPTIONS = {
    "cutscenes/47_CutscenePage54BgAsset": (
        "Map cells (1, 17) and (2, 17) hold byte-identical tiles in palette bank 1 that "
        "the tiler emitted twice, as 449 and 450, though it merges identical adjacent "
        "same-bank cells everywhere else, (4, 17) and (5, 17) of that row included. "
        "Only the tiling differs, pucrunch re-encodes the asset's own tiles byte for byte."
    ),
}

# the dialog speakers by picture address, in the order the lines they speak reveal
DIALOG_SPEAKERS = {
    0x08D3DCE0: "Frodo",
    0x08D3E1C0: "Legolas",
    0x08D3E688: "Aragorn",
    0x08D3EBFC: "Gandalf",
    0x08D3F124: "Eowyn",
    0x08D3F6C4: "Gimli",
    0x08D3FC38: "Sam",
    0x08D401FC: "Smeagol",
    0x08D40738: "Saruman",
    0x08D40C74: "Eomer",
    0x08D41210: "Theoden",
    0x08D41778: "Villager",
    0x08D41C98: "RohanSoldier",
    0x08D42190: "Elrond",
    0x08D42594: "Ranger",
    0x08D42A68: "KingOfTheDead",
    0x08D42F30: "Elder",
    0x08D43498: "Orc",
    0x08D43A3C: "Woman",
    0x08D43E88: "OrcCaptain",
    0x08D44478: "MouthOfSauron",
    0x08D44984: "GondorSoldier",
    0x08D44D00: "Merry",
    0x08D45250: "WitchKing",
    0x08D455B8: "Pippin",
    0x08D45B28: "Denethor",
    0x08D460B8: "Faramir",
    0x08D4665C: "Gollum",
    0x08D46B6C: "Shagrat",
    0x08D470B8: "Gorbag",
}

_PAGE_FIELDS = [
    "bg0Asset",
    "bg1Asset",
    "bg0Cnt",
    "bg1Cnt",
    "bg0ScrollSpeedX",
    "bg0ScrollSpeedY",
    "bg1ScrollSpeedX",
    "bg1ScrollSpeedY",
    "holdFrames",
    "textId",
    "nextMode",
    "mode",
    "nextPage",
    "field_0x27",
    "field_0x28",
    "bg0PaletteNumber",
    "bg1PaletteNumber",
    "field_0x2b",
]
_PAGE = Struct("<IIIIiiiiHHBBBBBBBB")
_LINE = Struct("<II")
_U32 = Struct("<I")
_LABEL_MAX = 60
PORTRAIT_TABLE = "HeroPortraitBgAssets"
PAGE_TABLE = "CutscenePages"
BGCNT_SYMBOLS = ["CutsceneBgCnt0", "CutsceneBgCnt1"]
LINE_TABLE = "DialogLineInfos"
CREDITS_KINDS = "CreditsLineKinds"


@dataclass
class Asset:
    category: str
    index: int
    label: str
    symbol: str
    addr: int
    asset: bgasset.BgAsset

    @property
    def end(self) -> int:
        return self.addr + self.asset.size

    @property
    def file_stem(self) -> str:
        return f"{self.category}/{self.index:02}_{self.symbol}"


@dataclass
class Carve:
    assets: dict[str, list[Asset]]  # by category, ROM order
    by_addr: dict[int, Asset]
    portrait_table: list[int]  # HeroPortraitBgAssets
    pages: list[tuple]  # CutscenePages rows
    lines: list[tuple[int, int]]  # DialogLineInfos rows


def _symbol(name: str) -> TableSymbol:
    sym = next(s for s in data_symbols() if s.name == name)
    count = re.search(r"\[(\d+)\]", sym.type)
    return TableSymbol(name, sym.addr, int(count.group(1)) if count else 1)


def _hero_names() -> list[str]:
    heroes = sorted(
        extract_enum("include/player.h", "HeroId").items(), key=lambda kv: kv[1]
    )
    return [name.removeprefix("HERO_ID_").capitalize() for name, _ in heroes]


def _read(rom: bytes, addr: int, where: str) -> bgasset.BgAsset:
    try:
        return bgasset.parse(rom, addr - ROMBASE)
    except ValueError as e:
        raise SystemExit(f"{where}: {e}") from None


def load(rom: bytes) -> Carve:
    strings = decode_strings(rom)
    assets: dict[str, list[Asset]] = {c: [] for c in CATEGORIES}
    by_addr: dict[int, Asset] = {}

    def add(category: str, label: str, name: str, addr: int) -> None:
        if addr in by_addr:
            return
        shape = CATEGORIES[category]
        parsed = _read(rom, addr, f"bg assets: {category} {label}")
        check(
            (
                parsed.flags,
                parsed.flags2,
                len(parsed.palette),
                parsed.width,
                parsed.height,
            )
            == (shape.flags, shape.flags2, shape.colors, shape.width, shape.height),
            f"bg assets: {category} {label} does not have its category's shape",
        )
        asset = Asset(
            category,
            len(assets[category]),
            label,
            f"{shape.prefix}{name}BgAsset",
            addr,
            parsed,
        )
        assets[category].append(asset)
        by_addr[addr] = asset

    table = _symbol(PORTRAIT_TABLE)
    portrait_table = [
        _U32.unpack_from(rom, table.addr - ROMBASE + i * 4)[0]
        for i in range(table.count)
    ]
    for name, addr in zip(_hero_names(), portrait_table):
        add(PORTRAITS, name, name, addr)

    line_table = _symbol(LINE_TABLE)
    lines = [
        _LINE.unpack_from(rom, line_table.addr - ROMBASE + i * _LINE.size)
        for i in range(line_table.count)
    ]
    for picture, _value in sorted({row for row in lines if row[0]}):
        check(
            picture in DIALOG_SPEAKERS,
            f"bg assets: dialog picture {picture:#x} has no speaker name",
        )
        add(DIALOG, DIALOG_SPEAKERS[picture], DIALOG_SPEAKERS[picture], picture)

    page_table = _symbol(PAGE_TABLE)
    pages = [
        _PAGE.unpack_from(rom, page_table.addr - ROMBASE + i * _PAGE.size)
        for i in range(page_table.count)
    ]
    for i, page in enumerate(pages):
        check(page[1] == 0, f"bg assets: cutscene page {i} uses layer 1")
        add(CUTSCENES, strings[page[9]], f"{i:02}", page[0])

    for category, group in assets.items():
        group.sort(key=lambda a: a.addr)
        for i, asset in enumerate(group):
            asset.index = i
        for prev, asset in zip(group, group[1:]):
            check(
                asset.addr == prev.end,
                f"bg assets: {category} {asset.symbol} does not follow {prev.symbol}",
            )
    return Carve(assets, by_addr, portrait_table, pages, lines)


# --- carved-local: the images and sheets -----------------------------------------


def _image(asset: Asset) -> IndexedImage:
    return bgasset.compose(asset.asset, CATEGORIES[asset.category].tiles_wide)


def extract(rom: bytes, captions: bool = True) -> Carve:
    """Replace each group's carved-local/gfx tree with the ROM's PNGs and sheets."""
    carve = load(rom)
    kept: set[str] = set()
    for category, group in carve.assets.items():
        shutil.rmtree(GFX_LOCAL / category, ignore_errors=True)
        os.makedirs(GFX_LOCAL / category, exist_ok=True)
        images = [_image(asset) for asset in group]
        shape = CATEGORIES[category]
        for asset, image in zip(group, images):
            write_indexed(str(GFX_LOCAL / f"{asset.file_stem}.png"), image)
            rebuilt = bgasset.rebuild(image, shape.template(), shape.tiles_wide)
            keep_rom_copy(
                asset.file_stem,
                bgasset.serialize(asset.asset),
                bgasset.serialize(rebuilt),
                TILER_EXCEPTIONS,
                kept,
            )
        names = [asset.file_stem.split("/")[-1] for asset in group]
        write_sheet(category, images, names if captions else None)
    total = sum(len(g) for g in carve.assets.values())
    print(f"  wrote {total} assets under {os.path.relpath(GFX_LOCAL, ROOT)}/")
    report_rom_copies(TILER_EXCEPTIONS, kept)
    return carve


# --- carved/: the C definitions and the data store -------------------------------


def _flags_init(flags: int) -> str:
    names = []
    for bit, name in bgasset.FLAG_NAMES.items():
        if flags & bit == bit:
            names.append(name)
            flags &= ~bit
    check(flags == 0, f"bg assets: flag bits {flags:#04x} have no BG_ASSET_* name")
    return " | ".join(names)


def _shape_macro(category: str) -> list[str]:
    """The category's shape as a macro, so every asset below it is one line."""
    shape = CATEGORIES[category]
    args = ["name", _flags_init(shape.flags), str(shape.colors)]
    if shape.flags & bgasset.FLAG_MAP:
        generic, args = "BG_ASSET_MAPPED", args + [str(shape.width), str(shape.height)]
    else:
        generic = "BG_ASSET_UNMAPPED"
    body = textwrap.wrap(
        f"{generic}({', '.join(args)})",
        width=COLUMNS - 2,
        initial_indent="    ",
        subsequent_indent=" " * (4 + len(generic) + 1),
        break_long_words=False,
    )
    return macro_lines(f"{shape.macro}(name)", body)


def _asset_lines(asset: Asset) -> list[str]:
    label = _short(asset.label) if asset.category == CUTSCENES else asset.label
    return [
        f'// [{asset.index}] "{label}"',
        f"{CATEGORIES[asset.category].macro}({asset.symbol});",
    ]


def _short(label: str) -> str:
    return label if len(label) <= _LABEL_MAX else label[: _LABEL_MAX - 3] + "..."


def _emit_category(category: str, group: list[Asset]) -> tuple[int, str]:
    filename, description = (
        CATEGORIES[category].carved_file,
        CATEGORIES[category].description,
    )
    lines = [
        '#include "gfx.h"',
        f'#include "gfx/{category}.inc"',
        "",
        "// clang-format off",
        "",
        *doc_comment(group[0].addr, description),
        "",
        *_shape_macro(category),
        "",
    ]
    for asset in group:
        lines += _asset_lines(asset)
    return group[0].addr, write_table(os.path.join(category, filename), lines)


def _emit_portrait_table(carve: Carve) -> tuple[int, str]:
    table = _symbol(PORTRAIT_TABLE)
    heroes = _hero_names()
    lines = [
        '#include "player.h"',
        '#include "variables.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(table.addr, ["Stat screen portrait per hero."]),
        f"const void *const {table.name}[{table.count}] = {{",
    ]
    for hero, addr in zip(heroes, carve.portrait_table):
        lines.append(f"    [HERO_ID_{hero.upper()}] = {carve.by_addr[addr].symbol},")
    lines.append("};")
    return table.addr, write_table(os.path.join(PORTRAITS, f"{table.name}.c"), lines)


def _emit_pages(
    rom: bytes, carve: Carve, text_names: dict[int, str], strings: dict[int, str]
) -> tuple[int, str]:
    fields = extract_struct_fields("include/scene/cutscene.h", "CutscenePage")
    check(fields == _PAGE_FIELDS, f"bg assets: CutscenePage fields {fields} changed")
    modes = invert_enum(extract_enum("include/scene/gameplay.h", "GameplayMode"))
    page_modes = invert_enum(
        extract_enum("include/scene/cutscene.h", "CutscenePageMode")
    )
    table = _symbol(PAGE_TABLE)
    bgcnt = {_symbol(name).addr: name for name in BGCNT_SYMBOLS}
    lines = [
        '#include "scene/cutscene.h"',
        '#include "scene/gameplay.h"',
        '#include "text_ids.h"',
        '#include "variables.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(
            table.addr,
            [
                "Cutscene pages, chained by nextPage; scene_cutscene shows args[1]'s. Page",
                "58 is the ending's first page for Aragorn; the other heroes' start at 60, 62,",
                "64, 66, 68 by HeroId order (Frodo and Sam 62).",
            ],
        ),
        f"const CutscenePage {table.name}[{table.count}] = {{",
    ]
    for i, page in enumerate(carve.pages):
        row = dict(zip(_PAGE_FIELDS, page))
        check(
            all(
                row[f] == 0
                for f in (
                    "bg1Asset",
                    "bg1Cnt",
                    "holdFrames",
                    "field_0x27",
                    "field_0x28",
                    "bg1PaletteNumber",
                    "field_0x2b",
                )
            )
            and all(row[f] == 0 for f in _PAGE_FIELDS if "ScrollSpeed" in f),
            f"bg assets: cutscene page {i} sets a field the carver leaves out",
        )
        check(row["bg0Cnt"] in bgcnt, f"bg assets: cutscene page {i} BGCNT pointer")
        check(
            row["nextMode"] in modes and row["mode"] in page_modes,
            f"bg assets: cutscene page {i} mode {row['nextMode']}/{row['mode']} unnamed",
        )
        next_page = row["nextPage"]
        lines += [
            f'    // [{i}] "{_short(strings[row["textId"]])}"',
            f"    {{.bg0Asset = {carve.by_addr[row['bg0Asset']].symbol},"
            f" .bg0Cnt = &{bgcnt[row['bg0Cnt']]},",
            f"     .textId = {text_names[row['textId']]},",
            f"     .nextMode = {modes[row['nextMode']]}, .mode = {page_modes[row['mode']]},",
            f"     .nextPage = {'CUTSCENE_PAGE_NONE' if next_page == table.count else next_page},"
            f" .bg0PaletteNumber = {row['bg0PaletteNumber']}}},",
        ]
    lines.append("};")
    return table.addr, write_table(os.path.join(CUTSCENES, f"{table.name}.c"), lines)


def _emit_bgcnts(rom: bytes) -> tuple[int, str]:
    """The BGCNT words in their own file: the split link resolves a pointer to a symbol
    of the same file at offset 0, so the pages must reference them across files."""
    symbols = [_symbol(name) for name in BGCNT_SYMBOLS]
    lines = ['#include "types.h"', "", "// clang-format off"]
    for sym in symbols:
        value = _U32.unpack_from(rom, sym.addr - ROMBASE)[0]
        lines += [
            "",
            *doc_comment(
                sym.addr, [f"BGCNT word of the cutscene pages' layer {sym.name[-1]}."]
            ),
            f"const u32 {sym.name} = 0x{value:04x};",
        ]
    return symbols[0].addr, write_table(
        os.path.join(CUTSCENES, "CutsceneBgCnts.c"), lines
    )


def _emit_lines(
    carve: Carve, text_names: dict[int, str], strings: dict[int, str]
) -> tuple[int, str]:
    fields = extract_struct_fields("include/scene/inGameDialog.h", "DialogLineInfo")
    check(
        fields == ["speakerPicture", "slot0Value"],
        f"bg assets: DialogLineInfo fields {fields}",
    )
    kinds_sym = _symbol(CREDITS_KINDS)
    kinds = invert_enum(extract_enum("include/scene/credits.h", "CreditsLineKind"))
    table = _symbol(LINE_TABLE)
    lines = [
        '#include "scene/credits.h"',
        '#include "scene/inGameDialog.h"',
        '#include "text_ids.h"',
        '#include "variables.h"',
        "",
        "// clang-format off",
        "",
        *doc_comment(
            table.addr,
            [
                "Per text id, the speaker picture the in-game dialog draws with the line and",
                "the value it formats into slot 0; the credits lines use the latter to tag",
                "their kind. Lines without either are left out.",
            ],
        ),
        f"const DialogLineInfo {table.name}[{table.count}] = {{",
    ]
    for text_id, (picture, value) in enumerate(carve.lines):
        if not picture and not value:
            continue
        init = []
        if picture:
            init.append(f".speakerPicture = {carve.by_addr[picture].symbol}")
        if value:
            kind = (value - kinds_sym.addr) // 2
            check(
                kinds_sym.addr <= value < kinds_sym.addr + 6 and value % 2 == 0,
                f"bg assets: text {text_id} slot value {value:#x} is not a CreditsLineKinds entry",
            )
            init.append(f".slot0Value = &{kinds_sym.name}[{kinds[kind]}]")
        entry = f"    [{text_names[text_id]}] = {{{', '.join(init)}}},"
        if len(entry) > 100:
            entry = entry.replace(" = ", " =\n        ", 1)
        lines += [f'    // [{text_id}] "{_short(strings[text_id])}"', entry]
    lines.append("};")
    return table.addr, write_table(os.path.join(DIALOG, f"{table.name}.c"), lines)


def _write_cfg(carve: Carve) -> None:
    for category, group in carve.assets.items():
        write_cfg(
            category,
            "bg-assets",
            CATEGORIES[category].what,
            [(a.addr, a.symbol, f"u8[0x{a.asset.size:x}]") for a in group],
        )


def run(captions: CaptionsOption = True) -> None:
    rom = load_rom()
    carve = extract(rom, captions)
    strings = decode_strings(rom)
    text_names = invert_enum(extract_enum("carved/include/text_ids.h", "TextId"))
    entries = [_emit_category(c, g) for c, g in carve.assets.items()]
    entries += [
        _emit_portrait_table(carve),
        _emit_bgcnts(rom),
        _emit_pages(rom, carve, text_names, strings),
        _emit_lines(carve, text_names, strings),
    ]
    for _addr, src in entries:
        print(f"  carved {src}")
    _write_cfg(carve)
    upsert_map(entries, owned_dirs=list(GROUPS))
    print("  updated config/split.cfg. Now run: make verify")
