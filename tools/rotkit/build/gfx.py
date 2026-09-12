"""PNGs under carved-local/gfx -> build/gfx/<category>.inc, the byte-list macros the carved
graphics .c files expand. A category is one directory of PNGs, keyed by its path under
carved-local/gfx, which is also its .inc path and its reference-sheet path.

Part of the build (make/decomp.mk runs it while parsing, ahead of the data TUs): a
carver whose groups have no images yet is extracted from the base ROM first, so a fresh
clone builds. Without a ROM and without images, placeholder macros keep the TUs compiling
(verify skips the ROM compare in that case anyway). A category re-encodes when any of its
PNGs, or any rotkit module loaded here, is newer than its .inc; its reference sheet is
redrawn from the PNGs at the same time.
"""

import os
import re
import sys
from pathlib import Path
from types import ModuleType
from typing import Callable

from rotkit import bgasset
from rotkit.carve import bg_assets, item_gfx, level_icons, loot_piles, write_sheet
from rotkit.compression import lz77
from rotkit.paths import BUILD, CARVED_DATA, GFX_LOCAL, ROM, ROOT
from rotkit.png import IndexedImage, read_indexed, rgb_to_bgr555
from rotkit.rom import load_rom
from rotkit.spritegfx import cell_oam, pixels_to_tiles

Encoder = Callable[[Path], tuple[str, IndexedImage]]


def _macro(name: str, values: list[int], width: int, per_line: int) -> str:
    rows = [
        ", ".join(f"0x{v:0{width}x}" for v in values[i : i + per_line])
        for i in range(0, len(values), per_line)
    ]
    return f"#define {name} \\\n    " + ", \\\n    ".join(rows) + "\n"


def _read_4bpp(png: Path) -> IndexedImage:
    image = read_indexed(str(png))
    rel = os.path.relpath(png, ROOT)
    if max(image.pixels) >= 16:
        raise SystemExit(
            f"{rel}: pixel index {max(image.pixels)}; sprites use indices 0-15"
        )
    if image.transparent not in (None, 0):
        raise SystemExit(
            f"{rel}: transparent index {image.transparent}; the hardware uses index 0"
        )
    return image


def encode_icon(png: Path) -> tuple[str, IndexedImage]:
    """One backpack icon PNG -> its <Symbol>_TILES / _PALETTE macros (and the image, for the sheet)."""
    rel = os.path.relpath(png, ROOT)
    name = re.fullmatch(r"\d+_([A-Za-z0-9]+)", png.stem)
    if name is None:
        raise SystemExit(f"{rel}: expected <index>_<Symbol>.png")
    image = _read_4bpp(png)
    if (image.width, image.height) != (item_gfx.ICON_WIDTH, item_gfx.ICON_HEIGHT):
        raise SystemExit(f"{rel}: {image.width}x{image.height}, icons are 32x24")
    tiles = pixels_to_tiles(image.pixels, item_gfx.ICON_OAM, image.width, image.height)
    lz = lz77.encode(tiles)
    lz += bytes(-len(lz) % 4)
    # editors may pad the PLTE to 256 entries; only the first 16 can be indexed
    palette = [rgb_to_bgr555(c) for c in image.palette[: item_gfx.PALETTE_COLORS]]
    palette += [0] * (item_gfx.PALETTE_COLORS - len(palette))
    symbol = name.group(1)
    return (
        _macro(f"{symbol}_TILES", list(lz), 2, 16)
        + _macro(f"{symbol}_PALETTE", palette, 4, 8),
        image,
    )


def encode_sprite(png: Path) -> tuple[str, IndexedImage]:
    """One sprite strip PNG -> <Symbol>_TILES plus per frame _FRAME<k>_LZ77 / _OFFSET (the
    frame's tiles LZ77 compressed when that is smaller, each chunk padded to 4 bytes) and
    <Symbol>_PALETTE, which records without a palette of their own leave unused."""
    rel = os.path.relpath(png, ROOT)
    name = re.fullmatch(r"\d+_([A-Za-z0-9]+?)(?:_(\d+)frames)?", png.stem)
    if name is None:
        raise SystemExit(f"{rel}: expected <index>_<Symbol>[_<n>frames].png")
    symbol, frame_count = name.group(1), int(name.group(2) or 1)
    image = _read_4bpp(png)
    if image.width % frame_count:
        raise SystemExit(
            f"{rel}: {image.width} px wide, not {frame_count} equal frames"
        )
    width, height = image.width // frame_count, image.height
    try:
        layout = cell_oam(width, height)
    except ValueError as e:
        raise SystemExit(f"{rel}: {e}") from None
    text = ""
    data = b""
    for k in range(frame_count):
        pixels = b"".join(
            image.pixels[
                y * image.width + k * width : y * image.width + (k + 1) * width
            ]
            for y in range(height)
        )
        tiles = pixels_to_tiles(pixels, layout, width, height)
        lz = lz77.encode(tiles)
        lz += bytes(-len(lz) % 4)
        compressed = len(lz) < len(tiles)
        text += f"#define {symbol}_FRAME{k}_LZ77 {int(compressed)}\n"
        text += f"#define {symbol}_FRAME{k}_OFFSET {len(data)}\n"
        data += lz if compressed else tiles
    palette = [rgb_to_bgr555(c) for c in image.palette[:16]]
    palette += [0] * (16 - len(palette))
    return (
        text
        + _macro(f"{symbol}_TILES", list(data), 2, 16)
        + _macro(f"{symbol}_PALETTE", palette, 4, 8),
        image,
    )


def _encode_bg(
    png: Path, image: IndexedImage, template: bgasset.BgAsset, tiles_wide: int
) -> tuple[str, IndexedImage]:
    """A BG asset PNG in `template`'s shape -> <Symbol>_PALETTE, _MAP (with a tilemap),
    _TILES_SIZE and _TILES. Tiles are deduplicated and packed with pucrunch.encode, the
    ROM's own packer. The `.bin` ROM copy that `make carve` writes where the game's tiler
    cannot be reproduced wins while the PNG still shows the same picture."""
    rel = os.path.relpath(png, ROOT)
    name = re.fullmatch(r"\d+_([A-Za-z0-9]+)", png.stem)
    if name is None:
        raise SystemExit(f"{rel}: expected <index>_<Symbol>.png")
    asset = None
    rom_copy = png.with_suffix(".bin")
    if rom_copy.exists():
        original = bgasset.parse(rom_copy.read_bytes(), 0)
        reference = bgasset.compose(original, tiles_wide)
        if (image.pixels, image.palette[: len(reference.palette)]) == (
            reference.pixels,
            reference.palette,
        ):
            asset = original
    if asset is None:
        try:
            asset = bgasset.rebuild(image, template, tiles_wide)
        except ValueError as e:
            raise SystemExit(f"{rel}: {e}") from None
    symbol = name.group(1)
    text = _macro(f"{symbol}_PALETTE", list(asset.palette), 4, 8)
    if asset.flags & bgasset.FLAG_MAP:
        entries = list(asset.map) + [0] * (asset.map_entries - len(asset.map))
        text += _macro(f"{symbol}_MAP", entries, 4, 8)
    text += f"#define {symbol}_TILES_SIZE {len(asset.stream)}\n"
    text += _macro(f"{symbol}_TILES", list(asset.stream), 2, 16)
    return text, image


def encode_bg_asset(png: Path) -> tuple[str, IndexedImage]:
    """One whole-picture BG asset PNG; its shape comes from bg_assets.CATEGORIES."""
    category = bg_assets.CATEGORIES[png.parent.name]
    return _encode_bg(
        png, read_indexed(str(png)), category.template(), category.tiles_wide
    )


def encode_item_gfx(png: Path) -> tuple[str, IndexedImage]:
    """One item icon PNG; the variant directory picks the encoding, and an equip/hollow
    icon's map size is the PNG's own (they vary per equip slot)."""
    if png.parent.name == item_gfx.BACKPACK:
        return encode_icon(png)
    image = read_indexed(str(png))
    if image.width % 8 or image.height % 8:
        raise SystemExit(
            f"{os.path.relpath(png, ROOT)}: {image.width}x{image.height},"
            " BG icons are whole 8x8 tiles"
        )
    template = bgasset.BgAsset(
        item_gfx.BG_ICON_FLAGS,
        0,
        (0,) * item_gfx.PALETTE_COLORS,
        image.width // 8,
        image.height // 8,
        (),
        b"",
        b"",
    )
    return _encode_bg(png, image, template, 0)


CARVERS: dict[ModuleType, Encoder] = {
    item_gfx: encode_item_gfx,
    loot_piles: encode_sprite,
    level_icons: encode_sprite,
    bg_assets: encode_bg_asset,
}


def _categories(group: str) -> list[Path]:
    """A group's image directories: every directory holding PNGs, at whatever depth (item
    graphics nest a variant under the item type, a group with no split holds its own
    images). Their paths relative to carved-local/gfx are the category keys."""
    root = GFX_LOCAL / group
    return sorted(
        d for d in [root, *root.rglob("*")] if d.is_dir() and any(d.glob("*.png"))
    )


def _category(category_dir: Path) -> str:
    return category_dir.relative_to(GFX_LOCAL).as_posix()


def _write_inc(category: str, text: str) -> None:
    inc = BUILD / "gfx" / f"{category}.inc"
    inc.parent.mkdir(parents=True, exist_ok=True)
    inc.write_text(text)


def encode_category(encoder: Encoder, category_dir: Path) -> int:
    """Every PNG of one category -> build/gfx/<category>.inc + its reference sheet."""
    pngs = sorted(category_dir.glob("*.png"))
    text = ""
    images = []
    for png in pngs:
        macros, image = encoder(png)
        text += macros
        images.append(image)
    category = _category(category_dir)
    _write_inc(category, text)
    write_sheet(category, images, [p.stem for p in pngs])
    return len(pngs)


_MACRO_REF = re.compile(
    r"\b([A-Za-z0-9]+)_(TILES|TILES_SIZE|PALETTE|MAP|FRAME\d+_(?:LZ77|OFFSET))\b"
)
# item icon macros name their symbol once and expand its byte lists internally
_ICON_MACRO = re.compile(r"\bITEM_(?:BACKPACK_ICON(?:_BOXED)?|BG_ICON)\((\w+)")


def _placeholders(carver: ModuleType) -> int:
    """No ROM, no images: define every macro the carver's committed TUs expand."""
    count = 0
    for group in carver.GROUPS:
        for src in (CARVED_DATA / group).glob("*.c"):
            text = src.read_text()
            inc = re.search(r'#include "gfx/([\w/]+)\.inc"', text)
            if inc is None:
                continue
            macros = {
                f"{symbol}_{suffix}"
                for symbol in _ICON_MACRO.findall(text)
                for suffix in ("TILES", "TILES_SIZE", "PALETTE", "MAP")
            }
            macros |= {
                f"{symbol}_{suffix}" for symbol, suffix in _MACRO_REF.findall(text)
            }
            # a sized tiles array needs at least one element
            lines = "".join(
                f"#define {macro} {4 if macro.endswith('_TILES_SIZE') else 0}\n"
                for macro in sorted(macros)
            )
            path = BUILD / "gfx" / f"{inc.group(1)}.inc"
            if not (path.exists() and path.read_text() == lines):  # keep the TU's mtime
                _write_inc(inc.group(1), lines)
            count += len(macros)
    return count


def _tool_mtime() -> float:
    """Newest source among the rotkit modules loaded here: the encoders and everything
    they build on."""
    return max(
        os.path.getmtime(m.__file__)
        for name, m in sys.modules.items()
        if name.startswith("rotkit") and getattr(m, "__file__", None)
    )


def _stale(category_dir: Path, tool_mtime: float) -> bool:
    inc = BUILD / "gfx" / f"{_category(category_dir)}.inc"
    if not inc.exists():
        return True
    newest = max(
        tool_mtime, *(png.stat().st_mtime for png in category_dir.glob("*.png"))
    )
    return newest > inc.stat().st_mtime


def run() -> None:
    rom = None
    tool_mtime = _tool_mtime()
    for carver, encoder in CARVERS.items():
        groups = {g: _categories(g) for g in carver.GROUPS}
        if not all(groups.values()):
            if os.path.isfile(ROM):
                rom = rom or load_rom()
                carver.extract(rom)
                groups = {g: _categories(g) for g in carver.GROUPS}
            else:
                n = _placeholders(carver)
                print(
                    f"build gfx: {', '.join(carver.GROUPS)}: no ROM and no images;"
                    f" {n} placeholders"
                )
                continue
        categories = [d for dirs in groups.values() for d in dirs]
        encoded = sum(
            encode_category(encoder, d) for d in categories if _stale(d, tool_mtime)
        )
        print(
            f"build gfx: {', '.join(carver.GROUPS)}:"
            f" {len(categories)} categories, {encoded} encoded"
        )
