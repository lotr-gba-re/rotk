"""Carves carved/include/text_ids.h (enum TextId) from the base ROM.

Decodes the English text package (rotkit.textdb; the ids are shared by all 5 language
packages) and names every string id TEXT_ID_<SLUG> so matched C can reference game text
by name instead of raw ids.

Names are deterministic so regeneration never churns matched sources: the English text
slugified and uppercased (max MAX_NAME chars), empty slugs -> TEXT_ID_UNNAMED_<id>,
duplicate names suffixed with _<id>. Block bases (main-menu entries, options row labels,
difficulty names, ...) are just the block's first member; include/text.h aliases them.

Ids in a verified region of the text space are tagged TEXT_ID_<REGION>_<SLUG> instead
(REGIONS below); the tag disambiguates cross-region duplicate slugs for free
("Cheap" the stat name vs "Cheap" the prefix). Two regions name by role instead of text:
UNIQUE_DESC entries after the parallel unique item, SKILL entries after the skill name.
"""

import os
from dataclasses import dataclass
from typing import Callable

from rotkit import textdb
from rotkit.analysis.decode_text import render, slugify
from rotkit.paths import CARVED_INCLUDE, ROOT
from rotkit.rom import load_rom

OUT = CARVED_INCLUDE / "text_ids.h"
MAX_NAME = 40

# A Region.slug maps an id in the region to its name slug, given a sid -> English text
# decode. Most regions slug their own text; the two exceptions are noted on theirs.
SlugFunc = Callable[[int, Callable[[int], str]], str]


def text_slug(sid: int, text: Callable[[int], str]) -> str:
    return slugify(text(sid), MAX_NAME).upper()


def unique_desc_slug(sid: int, text: Callable[[int], str]) -> str:
    """Slug of the unique item this is the flavor text for: item_drawInfo shows
    text_getString(395 + baseIndex) for ITEM_TYPE_UNIQUE items with baseIndex < 26, and
    name id = 748 + baseIndex."""
    return text_slug(sid + 353, text)


def skill_slug(sid: int, text: Callable[[int], str]) -> str:
    """The skill name: "Fearless (Level @1) - You are ..." -> FEARLESS."""
    return slugify(text(sid).split(" (Level")[0], MAX_NAME).upper()


@dataclass(frozen=True)
class Region:
    """A verified run of same-kind text ids, named TEXT_ID_<tag>_<slug>."""

    first: int
    last: int
    tag: str
    slug: SlugFunc = text_slug


# Bounds verified against the code/tables that index them: stat name id = statIdx + 330
# with max statIdx 64, the nameIds of the carved item/unique/prefix/suffix tables
# (item_tables.py), and item_drawInfo's baseIndex < 26 check for UNIQUE_DESC.
REGIONS = (
    Region(330, 394, "STAT"),
    Region(395, 420, "UNIQUE_DESC", unique_desc_slug),
    Region(483, 565, "SKILL", skill_slug),
    Region(594, 747, "ITEM"),
    Region(748, 800, "UNIQUE"),
    Region(801, 1337, "PREFIX"),
    Region(1338, 1497, "SUFFIX"),
)


def region_for(sid: int) -> Region | None:
    for region in REGIONS:
        if region.first <= sid <= region.last:
            return region
    return None


def member_names(count: int) -> list[str]:
    rom = load_rom()
    pkg = textdb.pkg_for(rom, textdb.LANG_EN)

    def text(sid: int) -> str:
        return render(textdb.decode(rom, pkg, sid)[0])

    used: set[str] = set()
    names = []
    for sid in range(count):
        region = region_for(sid)
        slug = (region.slug if region else text_slug)(sid, text)
        if not slug:
            name = f"TEXT_ID_UNNAMED_{sid}"
        else:
            name = "TEXT_ID_" + (f"{region.tag}_" if region else "") + slug
            if name in used:
                name = f"{name}_{sid}"
        used.add(name)
        names.append(name)
    return names


def run() -> None:
    rom = load_rom()
    count = textdb.string_count(rom, textdb.pkg_for(rom, textdb.LANG_EN))
    names = member_names(count)

    with open(OUT, "w") as fh:
        fh.write(
            "// enum TextId, one member per game text string (ids are shared by all 5\n"
            "// language packages), named from the English text by rotkit carve text-ids.\n"
            "// Ids in a verified region of the text space are tagged TEXT_ID_<REGION>_*.\n"
            "// Block-base aliases for id+offset access live in text.h.\n"
        )
        fh.write("#pragma once\n\n")
        fh.write("typedef enum TextId\n{\n")
        fh.write(",\n".join(f"    {name} = {sid}" for sid, name in enumerate(names)))
        fh.write("\n} TextId;\n")

    print(f"wrote {os.path.relpath(OUT, ROOT)}: {count} text ids")
