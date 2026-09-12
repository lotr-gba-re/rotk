"""Forward sync: text stores -> regenerable Ghidra project.

Opens ghidra_proj/ (built by gen_project.py) and stamps the stores onto it:

  1. C-Parse include/*.h into the program DTM (parse_headers.parse_into).
  2. Per config/functions.cfg row: force TMode per the mode column, disassemble, create and
     name the function, apply its prototype from the parsed headers.
  3. Per data store row: create the label and apply its datatype.
  4. Per decompiled src/c function: stamp its .c Doxygen plate onto the address.
  5. With --analyze, run auto-analysis afterwards, seeded from the correctly decoded code.

Exits nonzero on any error.
"""

import os
import sys

from rotkit.analysis import decode_text
from rotkit.decomp import matched_docs
from rotkit.paths import FUNCS_CFG, GHIDRA_PROJ
from rotkit.stores import data_rows, rows

import rotkit.pyghidra_env as pyghidra_env  # noqa: F401  (GHIDRA_INSTALL_DIR default + headless AWT)
from rotkit.ghidra_build import parse_headers


PROJ_NAME = "rotk"


def run(analyze: bool = False) -> None:
    import pyghidra

    pyghidra.start()

    from ghidra.base.project import GhidraProject
    from ghidra.program.flatapi import FlatProgramAPI
    from ghidra.program.model.symbol import SourceType
    from ghidra.program.model.data import ArrayDataType, PointerDataType
    from ghidra.app.cmd.disassemble import ArmDisassembleCommand
    from ghidra.app.cmd.function import CreateFunctionCmd, ApplyFunctionSignatureCmd
    from ghidra.util.task import ConsoleTaskMonitor
    from java.math import BigInteger

    monitor = ConsoleTaskMonitor()
    errors = []

    project = GhidraProject.openProject(str(GHIDRA_PROJ), PROJ_NAME, False)
    program = project.openProgram("/", PROJ_NAME, False)  # read-write
    flat = FlatProgramAPI(program)
    space = program.getAddressFactory().getDefaultAddressSpace()

    def addr(off):
        return space.getAddress(int(off, 16))

    fm = program.getFunctionManager()
    st = program.getSymbolTable()
    dtm = program.getDataTypeManager()
    ctx = program.getProgramContext()
    tmode = program.getRegister("TMode")

    typemap = {}

    def resolve_type(s: str):
        s = s.strip()
        # Ghidra can't express C qualifiers on data types: strip `const` suffixes
        # (e.g. `LootRegionProgression *const [8]`) and `volatile` prefixes (e.g. `volatile u16`).
        if s.endswith("const"):
            s = s[:-5].strip()
        if s.startswith("volatile "):
            s = s[len("volatile ") :].strip()
        if not s:
            return None
        if s.endswith("*"):
            base = resolve_type(s[:-1].strip())
            return PointerDataType(
                base if base is not None else typemap.get("undefined")
            )
        if s.endswith("]"):
            i = s.rindex("[")
            base = resolve_type(s[:i].strip())
            if base is None:
                return None
            inner = s[i + 1 : -1].strip()
            if not inner:
                # unsized arrays (e.g. `u8[]`) have no fixed length to apply
                return None
            n = int(inner, 0)
            return ArrayDataType(base, n, base.getLength())
        dt = typemap.get(s)
        if dt is None:
            errors.append(f"data type {s!r} not found in DTM")
        return dt

    tx = program.startTransaction("apply_symbols")
    try:
        # 1. types
        fdefs = parse_headers.parse_into(dtm, monitor)
        print(f"parsed headers: {len(fdefs)} prototypes into DTM")
        # Ghidra's C parser hardcodes every enum to 4 bytes (Ghidra issue #4518), ignoring
        # __attribute__((packed)). Resize each enum to its minimal width so packed 1-byte
        # enum fields land at the right offset and their structs repack (community ResizeEnums.py).
        from ghidra.program.model.data import Enum

        alldt = []
        it = dtm.getAllDataTypes()
        while it.hasNext():
            alldt.append(it.next())
        nresized = 0
        for dt in alldt:
            if not isinstance(dt, Enum):
                continue
            vals = [int(v) for v in dt.getValues()]
            if not vals:
                continue
            lo, hi = min(vals), max(vals)
            want = 8
            for nb in (1, 2, 4):
                bits = nb * 8
                if (lo >= 0 and hi < (1 << bits)) or (
                    lo < 0 and -(1 << (bits - 1)) <= lo and hi < (1 << (bits - 1))
                ):
                    want = nb
                    break
            if want != dt.getLength():
                edit = dt.copy(dtm)
                edit.setLength(want)
                dt.replaceWith(edit)
                nresized += 1
        print(f"resized {nresized} enum(s) to packed width")
        # build name->DataType map (CParserUtils files types under category paths,
        # not /Name, so resolve by simple name across all categories)
        it = dtm.getAllDataTypes()
        while it.hasNext():
            dt = it.next()
            typemap.setdefault(dt.getName(), dt)

        # 2. functions.
        # Phase A: set every function's TMode (arm/thumb) BEFORE any disassembly -
        # setting context at a still-undecoded address never conflicts; setting it
        # after an earlier function's disassembly flowed through would throw
        # ContextChangeExceptions.
        func_rows = rows(FUNCS_CFG, 3)
        for off, mode, name in func_rows:
            a = addr(off)
            try:
                ctx.setValue(
                    tmode, a, a, BigInteger.ONE if mode == "thumb" else BigInteger.ZERO
                )
            except Exception as e:
                errors.append(f"{name}@{off}: setValue TMode: {e}")
        # Phase B: disassemble (forced mode), create, name, type.
        # ApplyFunctionSignatureCmd locks param storage but leaves the convention "unknown";
        # pin every function to the cspec's default convention so the decompiler is clean.
        default_conv = program.getCompilerSpec().getDefaultCallingConvention().getName()
        nfunc = 0
        for off, mode, name in func_rows:
            a = addr(off)
            thumb = mode == "thumb"
            ArmDisassembleCommand(a, None, thumb).applyTo(program, monitor)
            CreateFunctionCmd(name, a, None, SourceType.USER_DEFINED).applyTo(
                program, monitor
            )
            func = fm.getFunctionAt(a)
            if func is None:
                errors.append(f"{name}@{off}: function not created (disasm failed?)")
                continue
            func.setName(name, SourceType.USER_DEFINED)
            fdef = fdefs.get(name)
            if fdef is not None:
                ApplyFunctionSignatureCmd(a, fdef, SourceType.USER_DEFINED).applyTo(
                    program, monitor
                )
            try:
                func.setCallingConvention(default_conv)
            except Exception as e:
                errors.append(f"{name}@{off}: convention: {e}")
            nfunc += 1

        # 3. data
        drows = data_rows()
        ndata = 0
        for off, name, tn in drows:
            a = addr(off)
            try:
                st.createLabel(a, name, SourceType.USER_DEFINED)
            except Exception as e:
                errors.append(f"data {name}@{off}: label: {e}")
                continue
            dt = resolve_type(tn) if tn else None
            if dt is not None:
                try:
                    flat.clearListing(a, a.add(dt.getLength() - 1))
                    flat.createData(a, dt)
                except Exception as e:
                    errors.append(f"data {name}@{off}: createData {tn!r}: {e}")
            ndata += 1

        # 3b. ROM entry + cart-header struct. 0x08000000 holds the ARM branch the BIOS
        # jumps to; disassembling it seeds the boot chain (crt0 and everything it reaches),
        # which no store row references. The rest of the header (include/gba.h
        # `struct CartridgeHeader`, 0xBC bytes from 0x08000004) is data.
        entry_a = space.getAddress(0x08000000)
        ArmDisassembleCommand(entry_a, None, False).applyTo(program, monitor)
        if flat.getInstructionAt(entry_a) is None:
            errors.append("rom entry @0x08000000: disassembly failed")
        header_dt = typemap.get("CartridgeHeader")
        if header_dt is not None:
            header_a = space.getAddress(0x08000004)
            try:
                flat.clearListing(header_a, header_a.add(header_dt.getLength() - 1))
                flat.createData(header_a, header_dt)
            except Exception as e:
                errors.append(f"cart header @0x08000004: createData: {e}")

        # 4. plate comments from the src/c Doxygen blocks
        from ghidra.program.model.listing import CommentType

        listing = program.getListing()
        ncomment = 0
        for off, text in matched_docs().items():
            try:
                listing.setComment(space.getAddress(off), CommentType.PLATE, text)
                ncomment += 1
            except Exception as e:
                errors.append(f"plate @0x{off:08x}: {e}")

        # 5. localized text (all 5 languages): a labeled byte[N] plus an EOL comment per string
        from ghidra.program.model.data import ByteDataType

        byte_dt = ByteDataType.dataType
        nstr = 0
        for s in decode_text.iter_strings(list(range(len(decode_text.LANGS)))):
            a = space.getAddress(s.addr)
            label = f"txt_{s.lang}_{s.sid}" + (f"_{s.slug}" if s.slug else "")
            try:
                st.createLabel(a, label, SourceType.USER_DEFINED)
            except Exception as e:
                errors.append(f"string {label}@0x{s.addr:08x}: label: {e}")
                continue
            if s.nbytes > 0:
                try:
                    arr = ArrayDataType(byte_dt, s.nbytes, byte_dt.getLength())
                    flat.clearListing(a, a.add(s.nbytes - 1))
                    flat.createData(a, arr)
                except Exception as e:
                    errors.append(f"string {label}@0x{s.addr:08x}: data: {e}")
            try:
                listing.setComment(
                    a, CommentType.EOL, f'[{s.lang} #{s.sid}] "{s.text}"'
                )
            except Exception as e:
                errors.append(f"string {label}@0x{s.addr:08x}: comment: {e}")
            nstr += 1
    finally:
        program.endTransaction(tx, True)

    print(
        f"applied: {nfunc}/{len(func_rows)} functions, "
        f"{ndata}/{len(drows)} data symbols, {ncomment} comments, {nstr} strings"
    )

    if analyze:
        from ghidra.app.plugin.core.analysis import AutoAnalysisManager

        print("running auto-analysis (apply-first, so seeded from correct THUMB)...")
        tx = program.startTransaction("analyze")
        try:
            mgr = AutoAnalysisManager.getAnalysisManager(program)
            mgr.initializeOptions()
            mgr.reAnalyzeAll(None)
            mgr.startAnalysis(monitor)
        finally:
            program.endTransaction(tx, True)
        print(f"analysis done: {fm.getFunctionCount()} total functions")

    project.save(program)
    try:
        project.close(program)
    except Exception:
        pass
    try:
        project.close()
    except Exception as e:
        print(f"  (ignored project.close bookkeeping: {type(e).__name__})")
    for fn in os.listdir(GHIDRA_PROJ):
        if fn.endswith(".lock") or fn.endswith(".lock~"):
            try:
                os.remove(os.path.join(GHIDRA_PROJ, fn))
            except OSError:
                pass

    # dedup the "not found" type spam
    uniq = sorted(set(errors))
    if uniq:
        print(f"\n{len(uniq)} distinct issue(s):")
        for e in uniq[:40]:
            print(f"  ERROR: {e}")
        sys.exit(1)
    print("apply_symbols: OK (zero errors)")
