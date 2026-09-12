"""C-Parse the headers under include/ and carved/include/ into a Ghidra DataTypeManager, so
structs become datatypes and prototypes FunctionDefinitions for apply_symbols.py.
`rotkit ghidra parse-headers` parses into a throwaway manager and reports.

Re-parsing changed headers into an existing DTM accretes `.conflict` types, so the DTM
must be fresh (gen_project.py wipes ghidra_proj/ first).
"""

import os
from typing import Any

from rotkit.paths import CARVED_INCLUDE, INCLUDE, TOOLS

import rotkit.pyghidra_env  # noqa: F401  (GHIDRA_INSTALL_DIR default + headless AWT)


HEADERS = ["types.h"]  # base typedefs first; the glob adds every other header


def parse_into(dtm: Any, monitor: Any, debug: bool = False) -> dict[str, Any]:
    """Parse the curated headers into dtm. Returns {func_name: FunctionDefinition}."""
    import jpype
    from ghidra.app.util.cparser.C import CParserUtils
    from ghidra.program.model.data import FunctionDefinition, DataTypeManager

    manual = sorted(
        p
        for root in (INCLUDE, CARVED_INCLUDE)
        for p in root.rglob("*.h")
        if os.path.basename(p) != "types.h"
    )
    filenames = [str(INCLUDE / h) for h in HEADERS] + [str(p) for p in manual]
    # Ghidra's preprocessor searches only these -I paths, so the toolchain include dir
    # is needed for compiler-provided headers (libc.h's `#include <stddef.h>`)
    args = [
        "-I" + str(INCLUDE),
        "-I" + str(CARVED_INCLUDE),
        "-I" + str(TOOLS / "agbcc" / "include"),
    ]
    DTMgrArr = jpype.JArray(DataTypeManager)
    StrArr = jpype.JArray(jpype.JString)
    msg = CParserUtils.parseHeaderFiles(
        DTMgrArr(0), StrArr(filenames), StrArr(args), dtm, monitor
    )
    if debug:
        print("  parse messages:\n" + (str(msg) if msg else "(none)"))
        print(f"  dtm datatype count: {dtm.getDataTypeCount(True)}")

    fdefs = {}
    it = dtm.getAllDataTypes()
    while it.hasNext():
        dt = it.next()
        if isinstance(dt, FunctionDefinition):
            fdefs[dt.getName()] = dt
    return fdefs


def run() -> None:
    import pyghidra

    pyghidra.start()
    from ghidra.program.model.data import StandAloneDataTypeManager, Structure
    from ghidra.util.task import ConsoleTaskMonitor

    monitor = ConsoleTaskMonitor()
    dtm = StandAloneDataTypeManager("parse-test")
    tx = dtm.startTransaction("parse")
    try:
        fdefs = parse_into(dtm, monitor, debug=True)
    finally:
        dtm.endTransaction(tx, True)

    structs = []
    it = dtm.getAllDataTypes()
    while it.hasNext():
        dt = it.next()
        if isinstance(dt, Structure):
            structs.append((dt.getName(), dt.getLength()))
    print(f"structs parsed: {len(structs)}")
    for n, ln in sorted(structs):
        print(f"  {n}  len=0x{ln:x}")
    print(f"function definitions parsed: {len(fdefs)}")
    # spot-check a couple of prototypes round-trip
    for name in list(sorted(fdefs))[:5]:
        print(f"  {fdefs[name].getPrototypeString()}")
