from pathlib import Path

import pytest

from rotkit.cheaders import _index, extract_record_alignments, extract_type_layouts


@pytest.fixture
def libclang() -> None:
    from clang.cindex import LibclangError

    try:
        _index()
    except LibclangError as error:
        pytest.skip(f"libclang is unavailable: {error}")


def test_explicit_record_alignments(tmp_path: Path, libclang: None) -> None:
    header = tmp_path / "records.h"
    header.write_text(
        """
#define HALFWORD __attribute__((packed, aligned(2)))
struct Plain { char value; };
union PackedWord { unsigned int value; } __attribute__((packed));
struct Box { char values[5]; } HALFWORD;
union Flags { unsigned char value; } HALFWORD;
typedef struct Frame {
    union Flags flags;
    char width;
    char height;
} HALFWORD Frame;
struct Pointer { void *value; } __attribute__((aligned(2)));
struct Container {
    struct { char value; } __attribute__((aligned(8))) anonymous;
};
"""
    )
    assert extract_record_alignments([str(header)], []) == {
        "Box": 2,
        "Flags": 2,
        "PackedWord": 1,
        "Frame": 2,
        "Pointer": 4,
    }
    assert extract_type_layouts([str(header)], [])["record_packings"] == {
        "PackedWord": 1,
        "Box": 1,
        "Flags": 1,
        "Frame": 1,
    }


def test_enum_sizes_follow_arm_abi(tmp_path: Path, libclang: None) -> None:
    header = tmp_path / "enums.h"
    header.write_text(
        """
typedef enum Small { SMALL = 3 } Small;
typedef enum Packed { PACKED = 3 } __attribute__((packed)) Packed;
typedef enum LargePacked { LARGE_PACKED = 300 } __attribute__((packed)) LargePacked;
"""
    )
    assert extract_type_layouts([str(header)], [])["enum_sizes"] == {
        "Small": 4,
        "Packed": 1,
        "LargePacked": 2,
    }


def test_alignment_parse_errors_are_not_ignored(tmp_path: Path, libclang: None) -> None:
    header = tmp_path / "invalid.h"
    header.write_text('#include "missing.h"\n')
    with pytest.raises(ValueError, match="cannot read header layouts"):
        extract_record_alignments([str(header)], [])
