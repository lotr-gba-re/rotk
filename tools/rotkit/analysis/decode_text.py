"""
Decode the Huffman-compressed localized text in
Lord of the Rings: The Return of the King (GBA).

Reverse-engineered from:
  - text_setLanguagePackage @0x0802997c: for a language package `pkg`,
        tree base   = pkg + 4
        offset table = pkg + *(u32)pkg        (header word = table offset)
        blob base    = pkg
  - text_decodeHuffmanString @0x080299a0: per string id,
        stream = pkg + offsetTable[id]   (offsetTable entries are u32, index id*4)
        Huffman: root node = 0x100; node n is 4 bytes at treeBase+(n-0x100)*4
                 = { u16 left (bit==0), u16 right (bit==1) }; leaf when value <= 0xFF.
                 Bits consumed LSB-first within each byte. Stops at NUL.
  - text_setLanguage @0x08047a10: package pointer table @ 0x08289a10, indexed by
        language id (0=English,1=French,2=German,3=Italian,4=Spanish).

Menu labels: scene_mainMenu_enter builds 5 entries; entry i draws localized string id (i+6),
so the 5 main-menu labels are string ids 6..10.
"""

import os
import re
import struct
import unicodedata
from collections.abc import Iterator
from dataclasses import dataclass
from typing import Annotated

import typer

from rotkit import textdb
from rotkit.rom import load_rom

# Package-table order (textdb.TEXT_LANG_PTR_TABLE), matching the Language Select list
# (string ids 6-10):
# English, Francais, Deutsch, Italiano, Espanol  -- note Italian precedes Spanish.
LANGS = ["English", "French", "German", "Italian", "Spanish"]
LANG_TAGS = ["en", "fr", "de", "it", "es"]


# Font codepage -> Unicode: ASCII for 0x20-0x7A, accented Latin glyphs in the high slots
# and a few punctuation slots, mapped by context from the localized text.
CODEMAP = {
    0x7B: "©",  # (c)
    0x7D: "¡",  # inverted !
    0x7E: "¿",  # inverted ?
    0x80: "Á",  # A acute
    0x83: "Ä",  # A umlaut
    0x87: "è",  # e grave
    0x88: "É",  # E acute
    0x91: "Ó",  # O acute
    0x97: "Ú",  # U acute
    0x98: "Ü",  # U umlaut
    0x99: "ß",  # eszett
    0x9A: "à",  # a grave
    0x9B: "á",  # a acute
    0x9C: "â",  # a circumflex
    0x9E: "ä",  # a umlaut
    0xA1: "ç",  # c cedilla
    0xA2: "è",  # e grave
    0xA3: "é",  # e acute
    0xA4: "ê",  # e circumflex
    0xA5: "ë",  # e diaeresis
    0xA6: "ì",  # i grave
    0xA7: "í",  # i acute
    0xA8: "î",  # i circumflex
    0xA9: "ï",  # i diaeresis
    0xAA: "ñ",  # n tilde
    0xAB: "ò",  # o grave
    0xAC: "ó",  # o acute
    0xAD: "ô",  # o circumflex
    0xAF: "ö",  # o umlaut
    0xB1: "ù",  # u grave
    0xB2: "ú",  # u acute
    0xB3: "û",  # u circumflex
    0xB4: "ü",  # u umlaut
    # Unmapped (rare/uncertain): 0xB7 (x2). Rendered as \\xNN.
}


def render(raw: bytes) -> str:
    """Render decoded ROM bytes to Unicode using the game's font codepage."""
    chars = []
    for b in raw:
        if b in CODEMAP:
            chars.append(CODEMAP[b])
        elif b == 0x0A:
            chars.append("\n")
        elif 0x20 <= b < 0x7F:
            chars.append(chr(b))
        else:
            chars.append("\\x%02x" % b)  # unmapped: keep visible for follow-up
    return "".join(chars)


def esc(s: str) -> str:
    return s.replace("\t", "\\t").replace("\r", "\\r").replace("\n", "\\n")


def slugify(s: str, maxlen: int = 28) -> str:
    """Short identifier-safe summary of the (already rendered) text for a Ghidra label.

    Apostrophes drop out rather than becoming separators, so "Balin's Tomb" slugs to
    BALINS_TOMB."""
    ascii_text = (
        unicodedata.normalize("NFKD", s).encode("ascii", "ignore").decode("ascii")
    )
    slug = re.sub(r"[^A-Za-z0-9]+", "_", ascii_text.replace("'", "")).strip("_")
    if len(slug) > maxlen:
        slug = slug[:maxlen].rstrip("_")
    return slug


@dataclass(frozen=True)
class DecodedString:
    """One decoded Huffman string: its ROM address, language, id, and bytes."""

    addr: int
    lang: str  # language tag, e.g. "en"
    sid: int
    text: str  # single-line escaped rendering
    slug: str
    nbytes: int  # compressed-stream length (byte[N] carve size)


def iter_strings(lang_ids: list[int]) -> Iterator[DecodedString]:
    """Decode every string of the given languages."""
    rom = load_rom()
    for lang in lang_ids:
        pkg = textdb.pkg_for(rom, lang)
        tag = LANG_TAGS[lang]
        for sid in range(textdb.string_count(rom, pkg)):
            raw, nbytes = textdb.decode(rom, pkg, sid)
            text = render(raw)
            yield DecodedString(
                addr=textdb.stream_addr(rom, pkg, sid),
                lang=tag,
                sid=sid,
                text=esc(text),
                slug=slugify(text),
                nbytes=nbytes,
            )


def cmd_dump(out_path: str, lang_ids: list[int]) -> None:
    """Write a greppable TSV: one line per string (addr, lang, id, escaped text, slug, nbytes)."""
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    counts = {LANG_TAGS[l]: 0 for l in lang_ids}
    with open(out_path, "w", encoding="utf-8") as f:
        f.write("# addr\tlang\tid\ttext\tslug\tnbytes\n")
        for s in iter_strings(lang_ids):
            f.write(
                "0x%08x\t%s\t%d\t%s\t%s\t%d\n"
                % (s.addr, s.lang, s.sid, s.text, s.slug, s.nbytes)
            )
            counts[s.lang] += 1
    for lang in lang_ids:
        print(f"  {LANGS[lang]}: {counts[LANG_TAGS[lang]]} strings")
    print(f"Wrote {sum(counts.values())} strings to {out_path}")


def cmd_show(ids: list[int]) -> None:
    rom = load_rom()
    pkgs = [textdb.pkg_for(rom, lang) for lang in range(len(LANGS))]
    for sid in ids:
        print(f"id {sid}:")
        for lang, pkg in enumerate(pkgs):
            try:
                raw, nbytes = textdb.decode(rom, pkg, sid)
                s = f"{len(raw)}B ({nbytes}B comp): {render(raw)!r}"
            except (IndexError, struct.error) as e:
                # an id past the package's string count reads out of bounds
                s = f"<err {e}>"
            print(f"  {LANG_TAGS[lang]}: {s}")


def run_show(
    ids: Annotated[
        list[str] | None,
        typer.Argument(help="string ids, decimal or 0x hex"),
    ] = None,
) -> None:
    """Decode and print string ids for every language, with decoded and compressed lengths."""
    if not ids:
        raise typer.BadParameter("at least one string id is required")
    cmd_show([int(i, 0) for i in ids])


def run_dump(
    out: Annotated[str, typer.Option("-o", "--out")] = "tools/out/strings.tsv",
    all_langs: Annotated[
        bool, typer.Option("--all", help="dump all 5 languages (default: English only)")
    ] = False,
) -> None:
    """Write a TSV of every string, for grepping."""
    cmd_dump(out, list(range(len(LANGS))) if all_langs else [0])
