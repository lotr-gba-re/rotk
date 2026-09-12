"""The functions defined in src/c: their prototypes, Doxygen plates and @romaddress tags,
plus the prototype parser for the hand-written headers. A function is matched iff it is
defined at column 0 in some src/c/*.c; `inline` helpers are excluded. All src/c facts come
from one memoized parse (`_defs`).
"""

import glob
import os
import re
from collections.abc import Iterator

from rotkit.paths import SRC_C
from rotkit.stores import func_symbols


_DEF = re.compile(r"^[A-Za-z_].*?(\w+)\s*\(")  # column-0 `<type> name(` - a definition


def _is_inline_def(line: str) -> bool:
    """A column-0 definition marked `inline`: a helper with no ROM address of its own. `static`
    alone does not qualify, a static function can still be a real ROM function."""
    head = line[: line.index("(")] if "(" in line else line
    return "inline" in re.findall(r"[A-Za-z_]\w*", head)


def _join_paramlist(lines: list[str], start: int) -> tuple[str, int]:
    """Join lines from `start` until the param list's parens balance -> (text, last line index)."""
    buf = lines[start].strip()
    end = start
    while buf.count("(") > buf.count(")") and end + 1 < len(lines):
        end += 1
        buf += " " + lines[end].strip()
    return buf, end


def _signature_of(buf: str) -> tuple[str, str] | None:
    """(name, prototype) from a joined signature buffer, or None. Prototype is whitespace-collapsed
    through the closing `)` (no return `{`/trailing `;`); name is the identifier before the params."""
    if "(" not in buf or ")" not in buf:
        return None
    name = re.findall(r"\w+", buf[: buf.index("(")])
    if not name:
        return None
    return name[-1], " ".join(buf[: buf.rindex(")") + 1].split())


def _clean_doxygen(buf: list[str]) -> str:
    """Strip the /** */ and leading-* markers from a Doxygen block and drop the @romaddress
    line; returns the plate body."""
    out = []
    for raw in buf:
        s = raw.strip()
        if s.startswith("/**"):
            s = s[3:]
        if s.endswith("*/"):
            s = s[:-2]
        s = s.strip()
        if s.startswith("*"):
            s = s[1:].strip()
        if s.startswith("@romaddress"):
            continue
        out.append(s)
    while out and not out[0]:
        out.pop(0)
    while out and not out[-1]:
        out.pop()
    return "\n".join(out)


# One parsed definition: (name, prototype, doc block lines). Prototype/doc are None when absent.
_Def = tuple[str, str | None, list[str] | None]


def _parse_file(path: str) -> Iterator[_Def]:
    """Yield one record per column-0 non-inline function definition in the file.

    A /** */ block binds to the definition immediately following it; the binding survives
    blank lines but is cancelled by any intervening code (declaration, #include, brace)."""
    with open(path, encoding="utf-8") as fh:
        lines = fh.read().splitlines()
    pending = None  # raw lines of the most recent /** */ block, still unbound
    in_block = False
    buf = []
    idx, count = 0, len(lines)
    while idx < count:
        line = lines[idx].rstrip()
        s = line.strip()
        if in_block:
            buf.append(line)
            if s.endswith("*/"):
                in_block, pending, buf = False, buf, []
            idx += 1
            continue
        if s.startswith("/**"):
            buf = [line]
            if s.endswith("*/") and len(s) > 4:  # single-line /** ... */
                pending, buf = buf, []
            else:
                in_block = True
            idx += 1
            continue
        if not s:
            idx += 1
            continue  # blank line keeps the pending block
        match = None if line[0] in " \t#/}*" or line.endswith(";") else _DEF.match(line)
        if match is None or _is_inline_def(line):
            pending = None  # any non-definition code cancels the block
            idx += 1
            continue
        joined, end = _join_paramlist(lines, idx)
        sig = _signature_of(joined)
        yield match.group(1), sig[1] if sig else None, pending
        pending = None
        idx = end + 1


_defs_cache: list[_Def] | None = None


def _defs() -> list[_Def]:
    """Every column-0 non-inline definition in src/c, parsed once per process."""
    global _defs_cache
    if _defs_cache is None:
        _defs_cache = [
            rec
            for path in glob.glob(os.path.join(SRC_C, "**", "*.c"), recursive=True)
            for rec in _parse_file(path)
        ]
    return _defs_cache


def defined_in_c() -> set[str]:
    """Function names defined at column 0 in any src/c/*.c."""
    return {name for name, _sig, _doc in _defs()}


def signatures_in_c() -> dict[str, str]:
    """Function name -> its prototype as written in src/c (no trailing `{`)."""
    return {name: sig for name, sig, _doc in _defs() if sig is not None}


def docs_in_c() -> dict[str, str]:
    """Function name -> cleaned Doxygen plate text, for column-0 definitions with a leading block."""
    return {name: _clean_doxygen(doc) for name, _sig, doc in _defs() if doc is not None}


def romaddrs_in_c() -> dict[str, int]:
    """Function name -> the address in its @romaddress Doxygen tag, for functions that carry one."""
    out = {}
    for name, _sig, doc in _defs():
        if doc is None:
            continue
        for raw in doc:
            match = re.search(r"@romaddress\s+0x([0-9a-fA-F]+)", raw)
            if match:
                out[name] = int(match.group(1), 16)
                break
    return out


def prototypes_in_header(path: str) -> dict[str, str]:
    """Function name -> its prototype as declared in a hand-written header (no trailing `;`)."""
    out = {}
    with open(path, encoding="utf-8") as fh:
        lines = fh.read().splitlines()
    idx, count = 0, len(lines)
    while idx < count:
        raw = lines[idx]
        line = raw.strip()
        # column-0 only: an indented `name(args);` is a call inside an inline helper's body
        if not line or line[0] in "#/*" or raw[0] in " \t" or "(" not in line:
            idx += 1
            continue
        buf, end = _join_paramlist(lines, idx)
        if buf.rstrip().endswith(";"):  # a prototype terminates as a declaration
            sig = _signature_of(buf)
            if sig:
                out[sig[0]] = sig[1]
        idx = end + 1
    return out


def matched_docs() -> dict[int, str]:
    """addr -> cleaned Doxygen plate text, for matched functions that carry a /** */ block."""
    docs = docs_in_c()
    out = {}
    for sym in func_symbols():
        if sym.name in docs:
            out[sym.addr] = docs[sym.name]
    return out
