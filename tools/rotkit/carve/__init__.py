"""Shared machinery for the carvers: config/data.cfg symbol lookup, comment and TODO
helpers for carved files, graphics group output, file writing, and the split.cfg upsert.
Table-specific layouts live with their carver.
"""

import os
import re
import textwrap
from collections.abc import Sequence
from dataclasses import dataclass
from typing import Annotated

import typer

from rotkit import png, stores
from rotkit.paths import (
    CARVED_CONFIG,
    CARVED_DATA,
    GFX_LOCAL,
    GFX_SHEETS,
    ROOT,
    SPLIT_CFG,
)

# The graphics carvers' shared CLI flag for their carved-local reference sheets.
CaptionsOption = Annotated[
    bool,
    typer.Option(
        "--captions/--no-captions",
        help="caption each reference-sheet picture with its PNG name",
    ),
]


# --- config/data.cfg symbols ---------------------------------------------------


@dataclass(frozen=True)
class TableSymbol:
    """One config/data.cfg symbol typed `Name[N]`."""

    name: str
    addr: int
    count: int


def table_symbols(type_prefix: str) -> list[TableSymbol]:
    """config/data.cfg symbols whose type starts with type_prefix."""
    tables = []
    for sym in stores.data_symbols(type_prefix):
        match = re.search(r"\[(\d+)\]", sym.type)
        if not match:
            raise SystemExit(f"{type_prefix} symbol {sym.name} has no [N] count")
        tables.append(TableSymbol(sym.name, sym.addr, int(match.group(1))))
    return tables


def data_symbol(type_prefix: str) -> TableSymbol:
    """The single config/data.cfg symbol matching type_prefix; fails unless exactly one."""
    tables = table_symbols(type_prefix)
    if len(tables) != 1:
        raise SystemExit(
            f"expected exactly one {type_prefix} symbol in config/data.cfg, "
            f"found {len(tables)}"
        )
    return tables[0]


def flags_d_members(
    value: int, d_bits: list[str], known_bits: set[int], what: str
) -> list[str]:
    """Flag word -> its set bits' decoded-view (.d) member names, in bit order.

    d_bits is the union's bitfield member list in bit order (extract_struct_fields on the
    `d` member); known_bits is the bit indices that have a mask-view enum name. Fails on a
    set bit outside known_bits: an unnamed bit means new knowledge that should be named in
    the header first, not carved as a field_bit_N placeholder.
    """
    members = []
    for bit in range(32):
        if not value & (1 << bit):
            continue
        if bit not in known_bits:
            raise SystemExit(
                f"{what}: bit {bit} (0x{1 << bit:08x}) set in 0x{value:08x} has no name; "
                "add one and re-run"
            )
        members.append(d_bits[bit])
    return members


def d_init(members: list[str]) -> str:
    """Member names -> a decoded-view initializer `{ .a = 1, .b = 1 }` (`{ 0 }` when empty)."""
    if not members:
        return "{ 0 }"
    return "{ " + ", ".join(f".{m} = 1" for m in members) + " }"


def d_init_lines(head: str, members: list[str], tail: str) -> list[str]:
    """Multi-line d_init: one `.m = 1,` per line, continuations aligned under `head`,
    `tail` closing the last line (2+ members)."""
    cont = " " * len(head)
    return (
        [f"{head}.{members[0]} = 1,"]
        + [f"{cont}.{m} = 1," for m in members[1:-1]]
        + [f"{cont}.{members[-1]} = 1{tail}"]
    )


def check_d_init(members: list[str], value: int, d_bits: list[str]) -> None:
    """Assert the emitted member list evaluates back to the ROM flag word."""
    word = 0
    for member in members:
        word |= 1 << d_bits.index(member)
    if word != value:
        raise SystemExit(f"self-check failed: {d_init(members)} != {value:#010x}")


def bit_indices(mask_names: dict[int, str]) -> set[int]:
    """{bit_mask: name} -> the set of bit indices (for flags_d_members' known_bits).
    Combo masks (ENEMY_ATTACK_MELEE_MASK) name no single bit and are skipped."""
    return {mask.bit_length() - 1 for mask in mask_names if mask & (mask - 1) == 0}


# --- Comments and macros in carved files -----------------------------------------

COLUMNS = 100  # .clang-format ColumnLimit


def macro_lines(signature: str, body: list[str]) -> list[str]:
    """`#define <signature>` continued over body, backslashes at the column limit."""
    lines = [f"#define {signature}", *body]
    return [f"{line:<{COLUMNS - 1}}\\" for line in lines[:-1]] + lines[-1:]


def todo_lines(text: str) -> list[str]:
    """One known-unknown -> `// TODO:` header lines (wrapped) for the top of a carved file."""
    body = textwrap.wrap(text, width=91)
    return [f"// TODO: {body[0]}"] + [f"//       {line}" for line in body[1:]]


def entry_comment(index: int, label: str) -> str:
    """The `// [index] "label"` comment above a carved table entry."""
    return f'// [{index}] "{label}"'


def doc_comment(addr: int, description: list[str] | None = None) -> list[str]:
    """Doxygen doc block for a carved definition, in the src/c docstring format:
    description lines, a blank separator, then @romaddress. An empty string in
    description renders as a blank ` *` line. Collapses to the one-line
    `/** @romaddress ... */` form when there is no description."""
    if not description:
        return [f"/** @romaddress 0x{addr:08x} */"]
    body = [f" * {line}".rstrip() for line in description]
    return ["/**", *body, " *", f" * @romaddress 0x{addr:08x}", " */"]


# --- Graphics groups: reference sheets + the group data store ---------------------


def write_sheet(
    category: str, images: list[png.IndexedImage], labels: list[str] | None
) -> None:
    """Draw one category's viewing aid at carved-local/gfx/reference-sheets/<category>.png,
    mirroring the category's own path under carved-local/gfx/."""
    path = GFX_SHEETS / f"{category}.png"
    os.makedirs(path.parent, exist_ok=True)
    png.write_sheet(str(path), images, labels)


def keep_rom_copy(
    stem: str,
    rom_bytes: bytes,
    rebuilt: bytes,
    exceptions: dict[str, str],
    kept: set[str],
) -> None:
    """Fail unless carved-local/gfx/<stem>.png rebuilds its ROM asset byte for byte or
    `exceptions` names it; an exception keeps a .bin ROM copy beside the PNG."""
    if rebuilt == rom_bytes:
        return
    if stem not in exceptions:
        raise SystemExit(
            f"{stem} does not rebuild from its image and is not in TILER_EXCEPTIONS; find"
            " what the game's tiler did differently, then either teach bgasset.rebuild or"
            " add it with a reason"
        )
    (GFX_LOCAL / f"{stem}.bin").write_bytes(rom_bytes)
    kept.add(stem)


def report_rom_copies(exceptions: dict[str, str], kept: set[str]) -> None:
    """Fail on a stale exception (its asset rebuilds now) and list the kept copies."""
    stale = sorted(set(exceptions) - kept)
    if stale:
        raise SystemExit(
            f"{', '.join(stale)} rebuild byte for byte now; drop them from TILER_EXCEPTIONS"
        )
    if kept:
        print(f"  {len(kept)} kept a .bin ROM copy: {', '.join(sorted(kept))}")


def write_cfg(
    group: str, command: str, what: str, rows: list[tuple[int, str, str]]
) -> None:
    """Write a graphics group's data store at carved/config/<group>.cfg."""
    path = CARVED_CONFIG / f"{group}.cfg"
    os.makedirs(path.parent, exist_ok=True)
    header = textwrap.wrap(
        f"Written by `rotkit carve {command}`: data store rows for {what}"
        f" (carved/data/{group}/), same columns as config/data.cfg:",
        width=88,
    )
    with open(path, "w") as fh:
        fh.write("".join(f"# {line}\n" for line in header))
        fh.write("# address<TAB>name<TAB>type\n\n")
        for addr, name, type_str in sorted(rows):
            fh.write(f"0x{addr:08x}\t{name}\t{type_str}\n")
    print(f"  wrote {os.path.relpath(path, ROOT)}: {len(rows)} symbols")


# --- File writing + split.cfg ----------------------------------------------------


@dataclass
class Section:
    """One table's slice of a merged carved file: its includes and everything between the
    include block and the clang-format tail."""

    addr: int
    includes: list[str]
    body: list[str]


def write_source(relpath: str, text: str) -> str:
    """Write committed carved source at carved/data/<relpath> (data carvers)."""
    path = CARVED_DATA / relpath
    os.makedirs(os.path.dirname(path), exist_ok=True)
    open(path, "w").write(text)
    return path


def _sort_includes(lines: list[str]) -> list[str]:
    """Sort the leading #include block the way clang-format's SortIncludes does (plain ASCII),
    so carved files are format-clean as written and `make format` is a no-op on them."""
    includes = [line for line in lines if line.startswith('#include "')]
    if not includes:
        return lines
    first, last = lines.index(includes[0]), lines.index(includes[-1])
    block = lines[first : last + 1]
    if any(not line.startswith('#include "') for line in block):
        return lines  # includes not contiguous; leave as-is
    return lines[:first] + sorted(block) + lines[last + 1 :]


def write_table(relpath: str, lines: list[str]) -> str:
    """Append the clang-format tail, write carved/data/<relpath>, return the committed path."""
    write_source(
        relpath, "\n".join([*_sort_includes(lines), "// clang-format on"]) + "\n"
    )
    return f"carved/data/{relpath}"


class GroupWriter:
    """Accumulates Sections and writes them as one merged carved file, ordered by address.
    The build splices the file's .rodata over one split.cfg region, so the sections must
    cover one contiguous address run."""

    def __init__(self, relpath: str) -> None:
        self.relpath = relpath
        self.sections: list[Section] = []

    def add(self, section: Section) -> None:
        self.sections.append(section)

    def write(self) -> tuple[int, str]:
        """Write the merged file -> (start addr, committed src path) for the split.cfg row."""
        sections = sorted(self.sections, key=lambda s: s.addr)
        includes = sorted({i for s in sections for i in s.includes})
        lines = [f'#include "{i}"' for i in includes] + ["", "// clang-format off"]
        for section in sections:
            lines += ["", *section.body]
        return sections[0].addr, write_table(self.relpath, lines)


def upsert_map(entries: list[tuple[int, str]], owned_dirs: Sequence[str] = ()) -> str:
    """Merge (addr, 'carved/data/...') rows into split.cfg, keyed by path. Rows under
    owned_dirs that no entry re-creates are stale (merged or renamed files): they are
    dropped and their source files deleted."""
    rows = stores.read_split() if os.path.isfile(SPLIT_CFG) else {}
    fresh = {src for _addr, src in entries}
    for src in list(rows):
        if (
            any(src.startswith(f"carved/data/{d}/") for d in owned_dirs)
            and src not in fresh
        ):
            del rows[src]
            (ROOT / src).unlink(missing_ok=True)
    for addr, src in entries:
        rows[src] = addr
    stores.write_split(rows)
    return SPLIT_CFG
