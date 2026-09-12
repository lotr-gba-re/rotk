"""Read enum values, struct field names and #define values back out of the headers with
libclang, so carved C names identifiers exactly as the compiler sees them. The headers are
host-portable C89, so a host libclang yields the same enumerator values agbcc would.
"""

import os
import re
from typing import Any

from rotkit.paths import INCLUDE, ROOT


def _index() -> Any:
    """Return a configured clang.cindex.Index, locating libclang if not auto-found."""
    from clang import cindex

    try:
        return cindex.Index.create()
    except cindex.LibclangError:
        # The pip `libclang` package usually auto-configures; if it didn't, probe common system
        # paths so a distro libclang.so works too.
        for cand in (
            "/usr/lib/llvm/lib/libclang.so",
            "/usr/lib/x86_64-linux-gnu/libclang.so",
        ):
            if os.path.exists(cand):
                cindex.Config.set_library_file(cand)
                return cindex.Index.create()
        raise


def extract_enum(
    header: str, enum_name: str, include_dirs: tuple[str, ...] = ()
) -> dict[str, int]:
    """Parse `header` (path under ROOT if relative) with libclang; return {enumerator: int}.

    `include_dirs` defaults to [ROOT/include], which resolves the module headers' sibling
    `#include "types.h"` and cross-includes. Raises KeyError if `enum_name` is absent in
    `header`, and ValueError if two enumerators share a value (a header bug).
    """
    from clang import cindex

    path = header if os.path.isabs(header) else str(ROOT / header)
    args = ["-I" + d for d in (include_dirs or (str(INCLUDE),))]
    tu = _index().parse(path, args=args)
    target_file = os.path.abspath(path)

    members = {}
    # a typedef'd enum's ENUM_DECL shows up standalone and under the TYPEDEF_DECL
    seen = set()
    for node in tu.cursor.walk_preorder():
        if node.kind != cindex.CursorKind.ENUM_DECL or node.spelling != enum_name:
            continue
        if node.hash in seen:
            continue
        seen.add(node.hash)
        # Only the enum defined *in this header*, not one pulled in via a sibling #include that
        # happens to reuse the name.
        loc_file = node.location.file.name if node.location.file else None
        if loc_file and os.path.abspath(loc_file) != target_file:
            continue
        for child in node.get_children():
            if child.kind == cindex.CursorKind.ENUM_CONSTANT_DECL:
                if child.spelling in members:
                    raise ValueError(
                        f"duplicate enumerator {child.spelling} in {enum_name}"
                    )
                members[child.spelling] = int(child.enum_value)
    if not members:
        raise KeyError(f"enum {enum_name!r} not found in {path}")
    return members


def extract_struct_fields(
    header: str, type_name: str, member: str | None = None
) -> list[str]:
    """Parse `header` with libclang; return the field names of struct/union `type_name`.

    With `member`, returns the fields of that struct-typed member instead (e.g. the `d`
    decoded view of a type-punning union). Raises KeyError if the type or member is
    absent in `header`.
    """
    from clang import cindex

    path = header if os.path.isabs(header) else str(ROOT / header)
    tu = _index().parse(path, args=["-I" + str(INCLUDE)])
    target_file = os.path.abspath(path)

    for node in tu.cursor.walk_preorder():
        if node.kind not in (
            cindex.CursorKind.STRUCT_DECL,
            cindex.CursorKind.UNION_DECL,
        ):
            continue
        if node.spelling != type_name or not node.is_definition():
            continue
        loc_file = node.location.file.name if node.location.file else None
        if loc_file and os.path.abspath(loc_file) != target_file:
            continue
        target = node
        if member is not None:
            target = next(
                (
                    child.type.get_declaration()
                    for child in node.get_children()
                    if child.kind == cindex.CursorKind.FIELD_DECL
                    and child.spelling == member
                ),
                None,
            )
            if target is None:
                break
        fields = [
            child.spelling
            for child in target.get_children()
            if child.kind == cindex.CursorKind.FIELD_DECL
        ]
        if fields:
            return fields
    raise KeyError(
        f"struct/union {type_name!r} (member {member!r}) not found in {path}"
    )


def invert_enum(enum_map: dict[str, int]) -> dict[int, str]:
    """{name: value} -> {value: name}; raises ValueError on duplicate values (ambiguous reverse)."""
    inverted = {}
    for name, value in enum_map.items():
        if value in inverted:
            raise ValueError(
                f"value {value} has two names: {inverted[value]} and {name}"
            )
        inverted[value] = name
    return inverted


_INT_EXPR = re.compile(r"^[0-9xXa-fA-FuUlL\s|+()<>]+$")
_INT_SUFFIX = re.compile(r"(?i)(\d)[ul]+\b")


def extract_defines(header: str, names: set[str]) -> dict[str, int]:
    """Parse object-like `#define NAME <int-expr>` macros in `header`; return {name: value}.

    The macro body tokens libclang hands back are evaluated when the body is a plain
    integer expression (literals, |, +, <<, parentheses). Raises KeyError for a name
    absent in `header`, ValueError for one whose body is not a plain integer expression
    (e.g. a function-like macro).
    """
    from clang import cindex

    path = header if os.path.isabs(header) else str(ROOT / header)
    tu = _index().parse(
        path,
        args=["-I" + str(INCLUDE)],
        options=cindex.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD,
    )
    target_file = os.path.abspath(path)

    values = {}
    for node in tu.cursor.walk_preorder():
        if (
            node.kind != cindex.CursorKind.MACRO_DEFINITION
            or node.spelling not in names
        ):
            continue
        loc_file = node.location.file.name if node.location.file else None
        if not loc_file or os.path.abspath(loc_file) != target_file:
            continue
        tokens = list(tu.get_tokens(extent=node.extent))
        # Function-like macro: the parameter-list '(' butts directly against the name.
        # (No clang.cindex bindings wrap clang_Cursor_isMacroFunctionLike, so detect it
        # from the tokens.)
        functionlike = (
            len(tokens) > 1
            and tokens[1].spelling == "("
            and tokens[0].extent.end.offset == tokens[1].extent.start.offset
        )
        body = _INT_SUFFIX.sub(r"\1", " ".join(t.spelling for t in tokens[1:]))
        if functionlike or not _INT_EXPR.match(body):
            raise ValueError(
                f"#define {node.spelling} is not a plain integer expression"
            )
        values[node.spelling] = eval(body, {"__builtins__": {}}, {})
    missing = names - values.keys()
    if missing:
        raise KeyError(f"#define(s) not found in {path}: {', '.join(sorted(missing))}")
    return values
