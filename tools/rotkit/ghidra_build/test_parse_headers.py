import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

from rotkit.cheaders import _index
from rotkit.paths import ROOT


def run_ghidra_test(tmp_path: Path, script: str, **variables: str) -> None:
    install = Path(os.environ.get("GHIDRA_INSTALL_DIR", "/opt/ghidra"))
    if not (install / "Ghidra/application.properties").is_file() or not shutil.which(
        "java"
    ):
        pytest.skip("Ghidra and Java are required for the header import test")
    from clang.cindex import LibclangError

    try:
        _index()
    except LibclangError as error:
        pytest.skip(f"libclang is unavailable: {error}")
    env = {
        **os.environ,
        **variables,
        "_JAVA_OPTIONS": os.environ.get("_JAVA_OPTIONS", "")
        + f" -Duser.home={tmp_path}",
    }
    result = subprocess.run(
        [sys.executable, "-c", script],
        cwd=ROOT,
        env=env,
        text=True,
        capture_output=True,
        timeout=90,
    )
    assert result.returncode == 0, result.stdout + result.stderr


def test_ghidra_imports_arm_layout_attributes(tmp_path: Path) -> None:
    headers = tmp_path / "include"
    headers.mkdir()
    (headers / "types.h").write_text("// Minimal base header for the importer.\n")
    (headers / "layout.h").write_text(
        """
typedef union PackedWord {
    unsigned int value;
} __attribute__((packed)) PackedWord;

typedef struct AfterPackedWord {
    unsigned char prefix;
    PackedWord word;
    unsigned char suffix;
} AfterPackedWord;

typedef struct PackedHalfword {
    unsigned char prefix;
    unsigned int word;
} __attribute__((packed, aligned(2))) PackedHalfword;

typedef enum PackedEnum { PACKED_VALUE = 3 } __attribute__((packed)) PackedEnum;
typedef enum PlainEnum { PLAIN_VALUE = 3 } PlainEnum;

typedef struct AfterPackedEnum {
    unsigned char prefix;
    PackedEnum value;
    unsigned char suffix;
} AfterPackedEnum;

typedef struct PlainWord { unsigned int value; } PlainWord;
"""
    )
    run_ghidra_test(
        tmp_path,
        """
import os
from pathlib import Path
import pyghidra
pyghidra.start()
from ghidra.program.model.data import Composite, Enum, StandAloneDataTypeManager
from ghidra.util.task import ConsoleTaskMonitor
from rotkit.ghidra_build import parse_headers

parse_headers.INCLUDE = Path(os.environ["SYNTHETIC_INCLUDE"])
parse_headers.CARVED_INCLUDE = Path(os.environ["SYNTHETIC_CARVED_INCLUDE"])
manager = StandAloneDataTypeManager("synthetic-layout-test")
transaction = manager.startTransaction("parse")
try:
    parse_headers.parse_into(manager, ConsoleTaskMonitor())
    types = {
        str(dt.getName()): dt
        for dt in manager.getAllDataTypes()
        if isinstance(dt, Composite)
    }
    enums = {
        str(dt.getName()): dt.getLength()
        for dt in manager.getAllDataTypes()
        if isinstance(dt, Enum)
    }
    assert enums["PackedEnum"] == 1, enums
    assert enums["PlainEnum"] == 4, enums
    for name, size, alignment in (
        ("PackedWord", 4, 1),
        ("AfterPackedWord", 6, 1),
        ("PackedHalfword", 6, 2),
        ("AfterPackedEnum", 3, 1),
        ("PlainWord", 4, 4),
    ):
        record = types[name]
        assert (record.getLength(), record.getAlignment()) == (size, alignment), name
    for name, expected in (
        ("AfterPackedWord", dict(prefix=0, word=1, suffix=5)),
        ("PackedHalfword", dict(prefix=0, word=1)),
        ("AfterPackedEnum", dict(prefix=0, value=1, suffix=2)),
    ):
        offsets = {
            str(field.getFieldName()): field.getOffset()
            for field in types[name].getComponents()
        }
        assert offsets == expected, (name, offsets)
finally:
    manager.endTransaction(transaction, False)
    manager.close()
""",
        SYNTHETIC_INCLUDE=str(headers),
        SYNTHETIC_CARVED_INCLUDE=str(tmp_path / "carved-include"),
    )


def test_ghidra_imports_project_headers(tmp_path: Path) -> None:
    run_ghidra_test(
        tmp_path,
        """
import pyghidra
pyghidra.start()
from ghidra.program.model.data import StandAloneDataTypeManager
from ghidra.util.task import ConsoleTaskMonitor
from rotkit.ghidra_build.parse_headers import parse_into

manager = StandAloneDataTypeManager("project-header-smoke-test")
transaction = manager.startTransaction("parse")
try:
    assert parse_into(manager, ConsoleTaskMonitor())
finally:
    manager.endTransaction(transaction, False)
    manager.close()
""",
    )
