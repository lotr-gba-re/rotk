"""Build the regenerable Ghidra project from the ROM (docs/workflow.md).

Headless import of rotk.gba into ghidra_proj/ (gitignored, disposable). No GBA
loader extension is installed, so the memory map is scripted by hand:

  lang ARM:LE:32:v4t, cspec default, image base 0
  02000000-0203ffff WRAMB  rw   (uninit)     05000000-050003ff PALETTE rw (uninit)
  03000000-03007fff WRAMC  rw   (uninit)     06000000-06017fff VRAM    rw (uninit)
  04000000-040003fe IO     rw   (uninit)     07000000-070003ff OAMOBJ  rw (uninit)
  08000000-080000bf header r-x  (file bytes) 080000c0-08ffffff rom     r-x (file bytes)

The MMIO registers (IO_REGISTERS) get a label, a width-typed data unit and an EOL
comment. apply_symbols.py then stamps the stores onto this bare project.
"""

import os
import shutil
from dataclasses import dataclass

from rotkit.paths import GHIDRA_PROJ, ROM
from rotkit.rom import ROMBASE

import rotkit.pyghidra_env as pyghidra_env  # noqa: F401  (GHIDRA_INSTALL_DIR default + headless AWT)

PROJ_NAME = "rotk"


@dataclass(frozen=True)
class Block:
    name: str
    start: int
    size: int | None  # None = remaining file bytes from file_offset
    file_offset: int | None  # None = uninitialized (rwm mirror), else file-backed bytes
    readable: bool
    writable: bool
    executable: bool


BLOCKS = [
    Block("WRAMB", 0x02000000, 0x40000, None, True, True, False),
    Block("WRAMC", 0x03000000, 0x8000, None, True, True, False),
    Block("IO", 0x04000000, 0x3FF, None, True, True, False),
    Block("PALETTE", 0x05000000, 0x400, None, True, True, False),
    Block("VRAM", 0x06000000, 0x18000, None, True, True, False),
    Block("OAMOBJ", 0x07000000, 0x400, None, True, True, False),
    Block("header", ROMBASE, 0xC0, 0x0, True, False, True),
    Block("rom", ROMBASE + 0xC0, None, 0xC0, True, False, True),  # size = filelen-0xc0
]


@dataclass(frozen=True)
class IORegister:
    """A GBATEK MMIO register entry to label + type in the IO block.

    width: byte width of the register data unit: 1, 2 (halfword/word), or 4 (dword).
           None means label only, no defined data unit (e.g. WAVE_RAM, IR).
    """

    offset: int
    name: str
    comment: str
    width: int | None


# Transcribed from https://problemkaputt.de/gbatek.htm#gbaiomap.
IO_REGISTERS = [
    IORegister(0x000, "DISPCNT", "LCD Control", 2),
    IORegister(0x004, "DISPSTAT", "General LCD Status (STAT,LYC)", 2),
    IORegister(0x006, "VCOUNT", "Vertical Counter (LY)", 2),
    IORegister(0x008, "BG0CNT", "BG0 Control", 2),
    IORegister(0x00A, "BG1CNT", "BG1 Control", 2),
    IORegister(0x00C, "BG2CNT", "BG2 Control", 2),
    IORegister(0x00E, "BG3CNT", "BG3 Control", 2),
    IORegister(0x010, "BG0HOFS", "BG0 X-Offset", 2),
    IORegister(0x012, "BG0VOFS", "BG0 Y-Offset", 2),
    IORegister(0x014, "BG1HOFS", "BG1 X-Offset", 2),
    IORegister(0x016, "BG1VOFS", "BG1 Y-Offset", 2),
    IORegister(0x018, "BG2HOFS", "BG2 X-Offset", 2),
    IORegister(0x01A, "BG2VOFS", "BG2 Y-Offset", 2),
    IORegister(0x01C, "BG3HOFS", "BG3 X-Offset", 2),
    IORegister(0x01E, "BG3VOFS", "BG3 Y-Offset", 2),
    IORegister(0x020, "BG2PA", "BG2 Rotation/Scaling Parameter A (dx)", 2),
    IORegister(0x022, "BG2PB", "BG2 Rotation/Scaling Parameter B (dmx)", 2),
    IORegister(0x024, "BG2PC", "BG2 Rotation/Scaling Parameter C (dy)", 2),
    IORegister(0x026, "BG2PD", "BG2 Rotation/Scaling Parameter D (dmy)", 2),
    IORegister(0x028, "BG2X", "BG2 Reference Point X-Coordinate", 4),
    IORegister(0x02C, "BG2Y", "BG2 Reference Point Y-Coordinate", 4),
    IORegister(0x030, "BG3PA", "BG3 Rotation/Scaling Parameter A (dx)", 2),
    IORegister(0x032, "BG3PB", "BG3 Rotation/Scaling Parameter B (dmx)", 2),
    IORegister(0x034, "BG3PC", "BG3 Rotation/Scaling Parameter C (dy)", 2),
    IORegister(0x036, "BG3PD", "BG3 Rotation/Scaling Parameter D (dmy)", 2),
    IORegister(0x038, "BG3X", "BG3 Reference Point X-Coordinate", 4),
    IORegister(0x03C, "BG3Y", "BG3 Reference Point Y-Coordinate", 4),
    IORegister(0x040, "WIN0H", "Window 0 Horizontal Dimensions", 2),
    IORegister(0x042, "WIN1H", "Window 1 Horizontal Dimensions", 2),
    IORegister(0x044, "WIN0V", "Window 0 Vertical Dimensions", 2),
    IORegister(0x046, "WIN1V", "Window 1 Vertical Dimensions", 2),
    IORegister(0x048, "WININ", "Inside of Window 0 and 1", 2),
    IORegister(0x04A, "WINOUT", "Inside of OBJ Window & Outside of Windows", 2),
    IORegister(0x04C, "MOSAIC", "Mosaic Size", 2),
    IORegister(0x050, "BLDCNT", "Color Special Effects Selection", 2),
    IORegister(0x052, "BLDALPHA", "Alpha Blending Coefficients", 2),
    IORegister(0x054, "BLDY", "Brightness (Fade-In/Out) Coefficient", 2),
    IORegister(0x060, "SOUND1CNT_L", "Sweep register (NR10)", 2),
    IORegister(0x062, "SOUND1CNT_H", "Duty/Length/Envelope (NR11, NR12)", 2),
    IORegister(0x064, "SOUND1CNT_X", "Frequency/Control (NR13, NR14)", 2),
    IORegister(0x068, "SOUND2CNT_L", "Duty/Length/Envelope (NR21, NR22)", 2),
    IORegister(0x06C, "SOUND2CNT_H", "Frequency/Control (NR23, NR24)", 2),
    IORegister(0x070, "SOUND3CNT_L", "Stop/Wave RAM select (NR30)", 2),
    IORegister(0x072, "SOUND3CNT_H", "Length/Volume (NR31, NR32)", 2),
    IORegister(0x074, "SOUND3CNT_X", "Frequency/Control (NR33, NR34)", 2),
    IORegister(0x078, "SOUND4CNT_L", "Length/Envelope (NR41, NR42)", 2),
    IORegister(0x07C, "SOUND4CNT_H", "Frequency/Control (NR43, NR44)", 2),
    IORegister(0x080, "SOUNDCNT_L", "Control Stereo/Volume/Enable (NR50, NR51)", 2),
    IORegister(0x082, "SOUNDCNT_H", "Control Mixing/DMA Control", 2),
    IORegister(0x084, "SOUNDCNT_X", "Control Sound on/off (NR52)", 2),
    IORegister(0x088, "SOUNDBIAS", "Sound PWM Control", 2),
    IORegister(0x090, "WAVE_RAM", "Channel 3 Wave Pattern RAM (2 banks!!)", None),
    IORegister(0x0A0, "FIFO_A", "Channel A FIFO, Data 0-3", 4),
    IORegister(0x0A4, "FIFO_B", "Channel B FIFO, Data 0-3", 4),
    IORegister(0x0B0, "DMA0SAD", "DMA 0 Source Address", 4),
    IORegister(0x0B4, "DMA0DAD", "DMA 0 Destination Address", 4),
    IORegister(0x0B8, "DMA0CNT_L", "DMA 0 Word Count", 2),
    IORegister(0x0BA, "DMA0CNT_H", "DMA 0 Control", 2),
    IORegister(0x0BC, "DMA1SAD", "DMA 1 Source Address", 4),
    IORegister(0x0C0, "DMA1DAD", "DMA 1 Destination Address", 4),
    IORegister(0x0C4, "DMA1CNT_L", "DMA 1 Word Count", 2),
    IORegister(0x0C6, "DMA1CNT_H", "DMA 1 Control", 2),
    IORegister(0x0C8, "DMA2SAD", "DMA 2 Source Address", 4),
    IORegister(0x0CC, "DMA2DAD", "DMA 2 Destination Address", 4),
    IORegister(0x0D0, "DMA2CNT_L", "DMA 2 Word Count", 2),
    IORegister(0x0D2, "DMA2CNT_H", "DMA 2 Control", 2),
    IORegister(0x0D4, "DMA3SAD", "DMA 3 Source Address", 4),
    IORegister(0x0D8, "DMA3DAD", "DMA 3 Destination Address", 4),
    IORegister(0x0DC, "DMA3CNT_L", "DMA 3 Word Count", 2),
    IORegister(0x0DE, "DMA3CNT_H", "DMA 3 Control", 2),
    IORegister(0x100, "TM0CNT_L", "Timer 0 Counter/Reload", 2),
    IORegister(0x102, "TM0CNT_H", "Timer 0 Control", 2),
    IORegister(0x104, "TM1CNT_L", "Timer 1 Counter/Reload", 2),
    IORegister(0x106, "TM1CNT_H", "Timer 1 Control", 2),
    IORegister(0x108, "TM2CNT_L", "Timer 2 Counter/Reload", 2),
    IORegister(0x10A, "TM2CNT_H", "Timer 2 Control", 2),
    IORegister(0x10C, "TM3CNT_L", "Timer 3 Counter/Reload", 2),
    IORegister(0x10E, "TM3CNT_H", "Timer 3 Control", 2),
    IORegister(
        0x120, "SIODATA32", "SIO Data (Normal-32bit Mode; shared with below)", 4
    ),
    IORegister(0x128, "SIOCNT", "SIO Control Register", 2),
    IORegister(
        0x12A, "SIOMLT_SEND", "SIO Data (Local of MultiPlayer; shared below)", 2
    ),
    IORegister(0x12A, "SIODATA8", "SIO Data (Normal-8bit and UART Mode)", 2),
    IORegister(0x130, "KEYINPUT", "Key Status", 2),
    IORegister(0x132, "KEYCNT", "Key Interrupt Control", 2),
    IORegister(0x134, "RCNT", "SIO Mode Select/General Purpose Data", 2),
    IORegister(0x136, "IR", "Ancient - Infrared Register (Prototypes only)", None),
    IORegister(0x140, "JOYCNT", "SIO JOY Bus Control", 2),
    IORegister(0x150, "JOY_RECV", "SIO JOY Bus Receive Data", 4),
    IORegister(0x154, "JOY_TRANS", "SIO JOY Bus Transmit Data", 4),
    IORegister(0x158, "JOYSTAT", "SIO JOY Bus Receive Status", 2),
    IORegister(0x200, "IE", "Interrupt Enable Register", 2),
    IORegister(0x202, "IF", "Interrupt Request Flags / IRQ Acknowledge", 2),
    IORegister(0x204, "WAITCNT", "Game Pak Waitstate Control", 2),
    IORegister(0x208, "IME", "Interrupt Master Enable Register", 2),
    IORegister(0x300, "POSTFLG", "Undocumented - Post Boot Flag", 1),
    IORegister(0x301, "HALTCNT", "Undocumented - Power Down Control", 1),
]

IO_BASE = 0x04000000


def run() -> None:
    import pyghidra

    pyghidra.start()

    from ghidra.base.project import GhidraProject
    from ghidra.program.flatapi import FlatProgramAPI
    from ghidra.program.model.lang import LanguageID
    from ghidra.program.model.data import ByteDataType, WordDataType, DWordDataType
    from ghidra.program.model.symbol import SourceType
    from ghidra.program.model.listing import Program
    from ghidra.program.util import DefaultLanguageService
    from ghidra.program.database import ProgramDB
    from ghidra.util.task import ConsoleTaskMonitor
    from java.io import FileInputStream, File
    from java.lang import Object

    monitor = ConsoleTaskMonitor()
    romlen = os.path.getsize(ROM)
    print(f"ROM {ROM} ({romlen} bytes = 0x{romlen:x})")

    # fresh, disposable: wipe any prior regenerable project
    shutil.rmtree(GHIDRA_PROJ, ignore_errors=True)
    os.makedirs(GHIDRA_PROJ, exist_ok=True)

    ls = DefaultLanguageService.getLanguageService()
    lang = ls.getLanguage(LanguageID("ARM:LE:32:v4t"))
    cspec = lang.getDefaultCompilerSpec()

    consumer = Object()
    program = ProgramDB(PROJ_NAME, lang, cspec, consumer)
    space = program.getAddressFactory().getDefaultAddressSpace()

    def addr(off):
        return space.getAddress(off)

    mem = program.getMemory()
    flat = FlatProgramAPI(program)

    tx = program.startTransaction("init memory map")
    try:
        fb = mem.createFileBytes(
            "rotk.gba", 0, romlen, FileInputStream(File(str(ROM))), monitor
        )
        for blk in BLOCKS:
            size = blk.size if blk.size is not None else (romlen - blk.file_offset)
            if blk.file_offset is not None:
                block = mem.createInitializedBlock(
                    blk.name, addr(blk.start), fb, blk.file_offset, size, False
                )
            else:
                block = mem.createUninitializedBlock(
                    blk.name, addr(blk.start), size, False
                )
            block.setRead(blk.readable)
            block.setWrite(blk.writable)
            block.setExecute(blk.executable)
            tag = "init" if blk.file_offset is not None else "uninit"
            print(f"  block {blk.name:8s} {blk.start:#010x} +{size:#x} {tag}")

        # GBA boots in ARM at ROMBASE (the header branch) -> analysis seed
        program.getSymbolTable().addExternalEntryPoint(addr(ROMBASE))

        type_by_width = {
            1: ByteDataType.dataType,
            2: WordDataType.dataType,
            4: DWordDataType.dataType,
        }
        st = program.getSymbolTable()
        for reg in IO_REGISTERS:
            a = addr(IO_BASE + reg.offset)
            try:
                st.createLabel(a, reg.name, SourceType.USER_DEFINED)
            except Exception as e:
                print(f"  WARN: label {reg.name}@{a}: {e}")
                continue
            if reg.comment:
                flat.setEOLComment(a, reg.comment)
            dt = type_by_width.get(reg.width) if reg.width is not None else None
            if dt is not None:
                try:
                    flat.clearListing(a, a.add(dt.getLength() - 1))
                    flat.createData(a, dt)
                except Exception as e:
                    print(f"  WARN: data {reg.name}@{a} ({dt.getName()}): {e}")
        print(f"  labelled {len(IO_REGISTERS)} MMIO registers in IO block")

        # Ghidra's "ARM Symbol" analyzer reads bit 0 of ANY symbol address as the ELF Thumb
        # flag and shifts odd-addressed symbols down by one. The ROM block is RX and ~half
        # of the txt_* labels sit at odd addresses, so each analysis pass corrupts them.
        # Safe to disable: a raw ROM import has no ELF symbols; apply_symbols sets TMode.
        # https://github.com/NationalSecurityAgency/ghidra/issues/7237
        program.getOptions(Program.ANALYSIS_PROPERTIES).setBoolean("ARM Symbol", False)
    finally:
        program.endTransaction(tx, True)

    # save into a fresh project
    project = GhidraProject.createProject(str(GHIDRA_PROJ), PROJ_NAME, False)
    project.saveAs(program, "/", PROJ_NAME, True)
    print(f"saved project: {GHIDRA_PROJ}/{PROJ_NAME}.gpr")

    # `program` has its own consumer, so GhidraProject.close() raises on the consumer
    # bookkeeping after saveAs already persisted the DB; close best-effort, drop stale locks.
    try:
        program.release(consumer)
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
    print("OK")
