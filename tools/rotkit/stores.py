"""The hand-edited text stores: parsing + canonical form.

The one home for the store formats: config/functions.cfg and config/data.cfg (the RE symbol
maps), the carver-generated carved/config/*.cfg data stores, plus config/split.cfg (the
`make verify` carve manifest). Consumers use
rows()/func_symbols()/data_symbols()/read_split(), which silently skip malformed rows;
check/stores.py keeps its own line-validating parser (flagging malformed rows is its job).
The write_* functions regenerate a whole file in canonical form (header, banners,
address-sorted rows) for `rotkit build format-stores`; they are strict - a format pass must
never silently drop content.
"""

import os
from dataclasses import dataclass

from rotkit.paths import CARVED_CONFIG, DATA_CFG, FUNCS_CFG, SPLIT_CFG

FUNCS_HEADER = """\
# Hand-edited function symbol map (source of truth; applied by rotkit ghidra apply-symbols).
#
# Canonical address->name+mode store (NO types; prototypes live in the per-subsystem
# include/<subsystem>.h headers).
#
# Row order is canonicalized by `rotkit build format-stores` (`make format`).
# Don't put documentation comments for functions in here.
#
# columns: address<TAB>mode(arm|thumb)<TAB>name
"""

DATA_HEADER = """\
# Hand-edited data symbol map; applied by rotkit ghidra apply-symbols.
#
# Excludes the regenerable text-annotation layer (txt_*, ~10k labels stamped by
# apply_symbols.py from rotkit.analysis.decode_text, all 5 languages) and the
# GBA I/O register block (0x040003ff): standard hardware, belongs in a curated gba_io.h.
#
# Row order is canonicalized by `rotkit build format-stores` (`make format`).
# Don't put documentation comments for variables in here.
#
# columns: address<TAB>name<TAB>type  (type names are defined in include/*.h)
"""

# GBA address sections for the data.cfg banners: (start, end inclusive, label).
# A row outside every section is an error (extend the table). The I/O register block is
# deliberately absent - excluded per DATA_HEADER.
SECTIONS = [
    (0x02000000, 0x0203FFFF, "256 KiB EWRAM"),
    (0x03000000, 0x03007FFF, "32 KiB IWRAM"),
    (0x08000000, 0x09FFFFFF, "32 MiB ROM (16 MiB in RotK Retail)"),
    (0x0E000000, 0x0E007FFF, "32 KiB SRAM"),
]

BANNER_WIDTH = 99  # matches the committed banners: "# " + title centered with "="


def rows(path: str, arity: int) -> list[list[str]]:
    """Well-formed `arity`-column tab-rows of a store file (comments/blanks/bad arity skipped)."""
    out: list[list[str]] = []
    if not os.path.isfile(path):
        return out
    with open(path) as fh:
        for line in fh:
            s = line.rstrip("\n")
            if not s.strip() or s.lstrip().startswith("#"):
                continue
            cols = s.split("\t")
            if len(cols) == arity:
                out.append(cols)
    return out


def strict_rows(path: str, arity: int) -> list[list[str]]:
    """All data rows of a store file; raises on any malformed line (format passes)."""
    out: list[list[str]] = []
    with open(path) as fh:
        for n, line in enumerate(fh, 1):
            s = line.rstrip("\n")
            if not s.strip() or s.lstrip().startswith("#"):
                continue
            cols = s.split("\t")
            if len(cols) != arity:
                raise ValueError(f"{path}:{n}: expected {arity} tab columns: {s!r}")
            int(cols[0], 16)  # raises on a bad address
            out.append(cols)
    return out


@dataclass(frozen=True)
class FuncSym:
    """One functions.cfg row."""

    addr: int
    mode: str  # arm | thumb
    name: str


@dataclass(frozen=True)
class DataSym:
    """One data.cfg row."""

    addr: int
    name: str
    type: str


def func_symbols() -> list[FuncSym]:
    """functions.cfg rows, sorted by address."""
    return sorted(
        (FuncSym(int(addr, 16), mode, name) for addr, mode, name in rows(FUNCS_CFG, 3)),
        key=lambda s: s.addr,
    )


def data_store_paths() -> list[str]:
    """Every data store: the hand-edited data.cfg, then the carver-generated
    carved/config/*.cfg (same columns; a carver owns each and rewrites it whole)."""
    return [str(DATA_CFG), *sorted(str(p) for p in CARVED_CONFIG.glob("*.cfg"))]


def data_rows() -> list[list[str]]:
    """Well-formed rows of every data store, data.cfg first."""
    return [row for path in data_store_paths() for row in rows(path, 3)]


def data_symbols(type_prefix: str = "") -> list[DataSym]:
    """Data store rows whose type starts with type_prefix."""
    return [
        DataSym(int(addr, 16), name, sym_type)
        for addr, name, sym_type in data_rows()
        if sym_type.startswith(type_prefix)
    ]


def _write_if_changed(path: str, text: str) -> bool:
    if os.path.isfile(path) and open(path).read() == text:
        return False
    with open(path, "w") as fh:
        fh.write(text)
    return True


def _banner(title: str) -> str:
    return "# " + f" {title} ".center(BANNER_WIDTH - 2, "=") + "\n"


def write_funcs(rows: list[list[str]]) -> bool:
    """Canonically rewrite functions.cfg (header + address-sorted rows); True if changed."""
    rows = sorted(rows, key=lambda cols: int(cols[0], 16))
    body = "".join(
        f"0x{int(addr, 16):08x}\t{mode}\t{name}\n" for addr, mode, name in rows
    )
    return _write_if_changed(FUNCS_CFG, FUNCS_HEADER + "\n" + body)


def write_data(rows: list[list[str]]) -> bool:
    """Canonically rewrite data.cfg (header, section banners, address-sorted rows).

    Rows are bucketed into SECTIONS by address, so a hand-misfiled row lands in the
    right section; a row outside every section raises.
    """
    rows = sorted(rows, key=lambda cols: int(cols[0], 16))
    blocks: list[str] = []
    placed: set[int] = set()
    for start, end, label in SECTIONS:
        section = [c for c in rows if start <= int(c[0], 16) <= end]
        placed.update(id(c) for c in section)
        if not section:
            continue
        block = _banner(f"0x{start:08x} - 0x{end:08x}: {label}")
        for addr, name, sym_type in section:
            block += f"0x{int(addr, 16):08x}\t{name}\t{sym_type}\n"
        blocks.append(block.rstrip("\n"))
    stray = [c[0] for c in rows if id(c) not in placed]
    if stray:
        raise ValueError(f"{DATA_CFG}: rows outside every SECTIONS range: {stray}")
    return _write_if_changed(DATA_CFG, DATA_HEADER + "\n" + "\n\n".join(blocks) + "\n")


# --- split.cfg: the carve manifest (addr<TAB>src rows) --------------------------------------

SPLIT_HEADER = (
    "# Committed manifest for `make verify`: which carved source\n"
    "# files are spliced into the base ROM to reconstruct it. Columns: addr<TAB>src.\n"
    "# Hand-editable; the `rotkit carve` carvers keep their own rows here.\n"
)


def read_split() -> dict[str, int]:
    """split.cfg -> {src path: addr}."""
    rows: dict[str, int] = {}
    with open(SPLIT_CFG) as fh:
        for line in fh:
            if not line.strip() or line.lstrip().startswith("#"):
                continue
            addr, src = line.split()[:2]
            rows[src] = int(addr, 16)
    return rows


def write_split(rows: dict[str, int]) -> bool:
    """Write split.cfg in its canonical committed form: header + rows sorted by address.
    Returns True if the file changed."""
    text = SPLIT_HEADER + "".join(
        f"0x{rows[src]:08x}\t{src}\n"
        for src in sorted(rows, key=lambda path: rows[path])
    )
    if os.path.isfile(SPLIT_CFG) and open(SPLIT_CFG).read() == text:
        return False
    os.makedirs(os.path.dirname(SPLIT_CFG), exist_ok=True)
    with open(SPLIT_CFG, "w") as fh:
        fh.write(text)
    return True
