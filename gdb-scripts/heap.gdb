# heap.gdb: game heap usage, fragmentation, and integrity in the mGBA session.
#
# Usage (after starting mGBA's stub, e.g. `mgba-qt -g rotk.gba`):
#     gdb -x build/debug/rotk.gdb -x gdb-scripts/heap.gdb
# or from a running session (symbols must already be loaded):
#     (gdb) source gdb-scripts/heap.gdb
#
# Reads only: g_Heaps / g_HeapCount and the HeapBlock headers, no inferior calls, so
# every command works wherever the game is halted (Ctrl-C), no settle needed.
#
# Halt the game first (Ctrl-C in gdb), then:
#   heap [idx]              summary: used / free bytes and blocks, largest free block, the
#                           highest allocated block and the headroom above it, holes (free
#                           bytes below that block, the fragmentation), integrity, peaks
#   heap_map [idx]          one char per KiB of the heap: '#' allocated, '.' free, '+' mixed
#   heap_blocks [idx]       every block: address, state, payload size, free-list position
#   heap_log on|off         log every memory_malloc / zalloc / realloc / free with its caller
#                           and the heap summary at that moment (off by default: a hook
#                           per call slows the game while allocations are frequent)
#   heap_watch on|off [n]   sample the summary every n frames at the main-loop tick (default
#                           60) and print a line whenever used bytes or the top block moved
#                           (breaks every frame internally, so the game runs slower while on)
#   heap_guard on|off [addr]
#                           write-watch the 16-byte block header at addr (default: the first
#                           block) and halt when the writer is not the allocator itself; the
#                           stop names the writing function. Catches header corruption.
#                           mGBA's stub reports the hit as a bare SIGTRAP; the script resumes
#                           allocator writes from a stop handler. If a stop still lands in
#                           linkBlock / merge* / unlinkBlock, that is the allocator and `c`
#                           is safe.
#   heap_reset              forget the used / top peaks (kept since the script loaded)
#
# Stock ROM: two writers overwrite a block header while its neighbours stay intact.
# sfx_init (boot, after the first allocations) re-copies the EWRAM statics image up to
# 0x020027f8, 0x1b8 bytes into the arena, zeroing the first block's header. The gameplay
# exit (scene_gameplay_exit, every pause menu) captures 0x3000 words of VRAM into the
# 0x8000-byte g_VramCapture block, 0x4000 bytes past its end and across the header of the
# block above it; the resume copies the same span back. A header that fails validation is
# bridged by scanning upward for the block whose prev points back at it, which fixes the
# clobbered block's extent without trusting its words; `heap` prints a note per bridged
# header. Should the game free the block above a zeroed header, the allocator merges it
# in, leaving a free-looking block on no free list: reported as "orphan", excluded from
# free space, not a PROBLEM. heap_guard on the first block fires once at boot from
# sfx_init's memory_memcpy; on the block above g_VramCapture it fires from the BIOS
# (CpuFastSet) at every gameplay exit.
#
# The used span (heap start to the end of the highest allocated block) is what the heap
# must at least cover: first fit from the bottom never reaches past it. Holes are the free
# bytes inside that span. mod_ram registers the heap short of EWRAM's end; the summary
# shows the reserved slice.

python
import gdb
import struct

EWRAM_END = 0x02040000
MAIN_LOOP_TICK = 0x080315f6  # rotk_main+0xae, see utils.gdb's settle
BLOCK_HEADER = 16            # sizeof(HeapBlock)
ALLOCATED = 0xffffffff       # HeapBlock.freePrev while handed out
MAP_KIB_PER_CHAR = 1
SCAN_CHUNK = 0x4000          # bytes read per step while scanning for a block header
MAP_CHARS_PER_LINE = 64

_peaks = {}   # heap index -> {"used": bytes, "top": address}
_log_bps = []
_watch = {"bp": None, "every": 60, "frame": 0, "last": None}
_guard = {"bp": None, "ranges": []}


def _val(expr):
    return int(gdb.parse_and_eval(expr))


def _read_block(addr):
    """(next, prev, link, freePrev) of the HeapBlock header at addr."""
    mem = gdb.selected_inferior().read_memory(addr, BLOCK_HEADER)
    return struct.unpack("<IIII", bytes(mem))


class HeapStats:
    """One walk over a heap's address-ordered block list."""

    def __init__(self, index):
        self.index = index
        self.start = _val(f"(unsigned long)g_Heaps[{index}].firstBlock")
        self.free_head = _val(f"(unsigned long)g_Heaps[{index}].freeList")
        self.size = _val(f"g_Heaps[{index}].size")
        self.end = _val(f"(unsigned long)g_Heaps[{index}].end")
        self.blocks = []       # (addr, payload, allocated)
        self.problems = []
        self._walk()
        self._free_list_check()
        self._tally()
        self._update_peaks()

    def _in_heap(self, addr):
        return self.start <= addr and addr + BLOCK_HEADER <= self.end and addr & 3 == 0

    def _sane(self, addr, hdr, prev):
        """Header hdr at addr is consistent with predecessor prev (0 for the first block)."""
        nxt, back, link, free_prev = hdr
        if back != prev:
            return False
        if nxt != 0 and (nxt <= addr or nxt & 15 != addr & 15 or not self._in_heap(nxt)):
            return False
        if free_prev == ALLOCATED:
            return link == self.index
        return (free_prev == 0 or self._in_heap(free_prev)) and (link == 0 or self._in_heap(link))

    def _find_successor(self, addr):
        """Scan upward from addr for the header of the block above it, identified by its
        prev pointing back at addr; None when nothing up to the heap end does. A merged-away
        block leaves its stale header behind, free-looking and pointing back at the block
        that absorbed it, so a free-looking candidate counts only when it is on the free list."""
        pos = addr + BLOCK_HEADER
        while pos + BLOCK_HEADER <= self.end:
            n = min(SCAN_CHUNK, self.end - pos)
            chunk = bytes(gdb.selected_inferior().read_memory(pos, n))
            for off in range(0, n - BLOCK_HEADER + 1, 16):
                hdr = struct.unpack_from("<IIII", chunk, off)
                if hdr[1] != addr or not self._sane(pos + off, hdr, addr):
                    continue
                if hdr[3] == ALLOCATED or (pos + off) in self.free_set:
                    return pos + off
            pos += n - BLOCK_HEADER + 16
        return None

    def _walk(self):
        # Two stock writers leave a block header unreadable while its neighbours stay
        # intact: sfx_init zeroes the first block's header at boot, and the gameplay exit
        # screen capture overruns g_VramCapture across the header above it. A header that
        # fails validation is bridged by scanning upward for the block whose prev points
        # back at it, which fixes the clobbered block's extent without trusting its words.
        self.clobbered = []
        self.free_set = set(self._free_list_addrs())
        free_max = max(self.free_set, default=0)
        addr = self.start
        prev = 0
        for _ in range(self.size // BLOCK_HEADER + 1):
            if addr == 0:
                return
            if not self._in_heap(addr):
                self.problems.append(f"address chain leaves the heap at 0x{addr:08x}")
                return
            hdr = _read_block(addr)
            ok = self._sane(addr, hdr, prev)
            # a block claiming to be the topmost while a free block lies above it, or
            # while some block above still points back at it (a zeroed header)
            if ok and hdr[0] == 0 and (free_max > addr or self._find_successor(addr) is not None):
                ok = False
            if not ok:
                # either this header is overwritten, or prev's next pointer was and led here
                if prev and (successor := self._find_successor(prev)) is not None and successor != addr:
                    self.blocks.pop()
                    self.blocks.append((prev, successor - prev - BLOCK_HEADER, True))
                    self.clobbered.append(prev)
                    addr = successor
                    continue
                successor = self._find_successor(addr)
                self.blocks.append((addr, (successor or self.end) - addr - BLOCK_HEADER, True))
                self.clobbered.append(addr)
                if successor is None:
                    if free_max > addr:
                        words = " ".join(f"{w:08x}" for w in hdr)
                        self.problems.append(f"header of block 0x{addr:08x} overwritten (words {words}) "
                                             f"and no block above points back at it")
                    return
                prev = addr
                addr = successor
                continue
            self._add(addr, hdr[0], hdr)
            prev = addr
            addr = hdr[0]
        self.problems.append("address chain does not terminate")

    def _free_list_addrs(self):
        addrs = []
        addr = self.free_head
        while addr != 0 and self._in_heap(addr) and len(addrs) <= self.size // BLOCK_HEADER:
            addrs.append(addr)
            addr = _read_block(addr)[2]
        return addrs

    def _add(self, addr, nxt, hdr):
        payload = (nxt if nxt else self.end) - addr - BLOCK_HEADER
        self.blocks.append((addr, payload, hdr[3] == ALLOCATED))

    def _free_list_check(self):
        free_by_addr = {addr for addr, _, allocated in self.blocks if not allocated}
        listed = []
        addr = self.free_head
        prev = 0
        while addr != 0 and len(listed) <= len(self.blocks):
            if addr not in free_by_addr:
                self.problems.append(f"free list holds 0x{addr:08x}, which is not a free block")
                break
            _, _, free_next, free_prev = _read_block(addr)
            if free_prev != prev:
                self.problems.append(f"free block 0x{addr:08x} freePrev 0x{free_prev:08x}, expected 0x{prev:08x}")
            listed.append(addr)
            prev = addr
            addr = free_next
        self.free_pos = {addr: i for i, addr in enumerate(listed)}
        # Reads as free but sits on no free list: the allocator can neither hand it out
        # nor merge it from below. In stock the clobbered first block ends up like this
        # once the block above it is freed and merged into it.
        self.orphans = free_by_addr - set(listed)

    def _tally(self):
        self.used = 0            # payload bytes handed out
        self.headers = 0         # header bytes of every block
        self.free = 0
        self.alloc_count = 0
        self.free_count = 0
        self.largest_free = 0
        self.orphan_bytes = 0
        self.orphan_count = 0
        self.top = self.start    # end of the highest allocated block
        for addr, payload, allocated in self.blocks:
            self.headers += BLOCK_HEADER
            if allocated:
                self.used += payload
                self.alloc_count += 1
                self.top = addr + BLOCK_HEADER + payload
            elif addr in self.orphans:
                self.orphan_bytes += payload
                self.orphan_count += 1
            else:
                self.free += payload
                self.free_count += 1
                self.largest_free = max(self.largest_free, payload)
        self.holes = 0
        self.hole_count = 0
        for addr, payload, allocated in self.blocks:
            if not allocated and addr not in self.orphans and addr < self.top:
                self.holes += payload
                self.hole_count += 1
        self.tail = self.free - self.holes
        self.span = self.top - self.start
        self.headroom = self.end - self.top

    def _update_peaks(self):
        peak = _peaks.setdefault(self.index, {"used": 0, "top": self.start})
        peak["used"] = max(peak["used"], self.used)
        peak["top"] = max(peak["top"], self.top)

    def one_line(self):
        if self.problems:
            return f"WALK BROKEN: {self.problems[0]}"
        line = (f"used {self.used:7d} in {self.alloc_count:3d} blocks | free {self.free:7d} "
                f"largest {self.largest_free:7d} | top 0x{self.top:08x} headroom {self.headroom:7d} "
                f"| holes {self.holes:6d}")
        if self.orphan_count:
            line += f" | orphan {self.orphan_bytes}"
        return line


def _heap_count():
    count = _val("g_HeapCount")
    if count == 0:
        gdb.write("heap: g_HeapCount is 0, memory_init has not run yet\n")
    return count


def _stats(index):
    if index < 0 or index >= _heap_count():
        return None
    return HeapStats(index)


def heap_cmd(index):
    s = _stats(index)
    if s is None:
        return
    peak = _peaks[s.index]
    frag = 100.0 * s.holes / s.free if s.free else 0.0
    gdb.write(f"heap {s.index}: 0x{s.start:08x}..0x{s.end:08x}, {s.size} bytes")
    if s.end < EWRAM_END:
        gdb.write(f" ({EWRAM_END - s.end} bytes below the EWRAM end reserved)")
    gdb.write("\n")
    gdb.write(f"  used     {s.used:7d} bytes in {s.alloc_count} blocks, plus {s.headers} bytes of headers\n")
    gdb.write(f"  free     {s.free:7d} bytes in {s.free_count} blocks, largest {s.largest_free}\n")
    gdb.write(f"  top      0x{s.top:08x}: used span {s.span} bytes, headroom {s.headroom} bytes\n")
    gdb.write(f"  holes    {s.holes:7d} bytes in {s.hole_count} free blocks below the top "
              f"({frag:.1f}% of free space), tail {s.tail}\n")
    gdb.write(f"  peak     used {peak['used']} bytes, top 0x{peak['top']:08x} "
              f"(headroom {s.end - peak['top']})\n")
    if s.orphan_count:
        gdb.write(f"  orphan   {s.orphan_bytes:7d} bytes in {s.orphan_count} blocks that read as free "
                  f"but sit on no free list (unusable)\n")
    for addr in s.clobbered:
        gdb.write(f"  note     header of block 0x{addr:08x} clobbered, extent inferred from its "
                  f"neighbours (0x{s.start:08x}: sfx_init at boot; the block above "
                  f"g_VramCapture: the gameplay exit screen capture)\n")
    if s.problems:
        for problem in s.problems:
            gdb.write(f"  PROBLEM  {problem}\n")
    else:
        gdb.write("  check    ok\n")


def heap_map_cmd(index):
    s = _stats(index)
    if s is None:
        return
    kib = MAP_KIB_PER_CHAR * 1024
    cells = (s.end - s.start + kib - 1) // kib
    alloc = [0] * cells
    free = [0] * cells
    orphan = [0] * cells
    for addr, payload, allocated in s.blocks:
        lo = addr
        hi = addr + BLOCK_HEADER + payload
        bucket = alloc if allocated else (orphan if addr in s.orphans else free)
        for cell in range(max(0, (lo - s.start) // kib), min(cells, (hi - 1 - s.start) // kib + 1)):
            cell_lo = s.start + cell * kib
            cell_hi = cell_lo + kib
            bucket[cell] += min(hi, cell_hi) - max(lo, cell_lo)
    chars = []
    for cell in range(cells):
        if alloc[cell] and free[cell]:
            chars.append("+")
        elif alloc[cell]:
            chars.append("#")
        elif orphan[cell]:
            chars.append("o")
        else:
            chars.append(".")
    gdb.write(f"heap {s.index}: {MAP_KIB_PER_CHAR} KiB per char, '#' allocated, '.' free, "
              "'+' mixed, 'o' orphan\n")
    for line in range(0, cells, MAP_CHARS_PER_LINE):
        addr = s.start + line * kib
        gdb.write(f"  0x{addr:08x}  {''.join(chars[line:line + MAP_CHARS_PER_LINE])}\n")
    gdb.write("  " + s.one_line() + "\n")


def heap_blocks_cmd(index):
    s = _stats(index)
    if s is None:
        return
    gdb.write(f"heap {s.index}: {len(s.blocks)} blocks\n")
    for addr, payload, allocated in s.blocks:
        if allocated:
            state = "alloc"
        elif addr in s.orphans:
            state = "orphan"
        else:
            state = f"free #{s.free_pos.get(addr, '?')}"
        note = "  header clobbered, extent inferred" if addr in s.clobbered else ""
        gdb.write(f"  0x{addr:08x}  {state:10s} {payload:7d}{note}\n")
    gdb.write("  " + s.one_line() + "\n")
    for problem in s.problems:
        gdb.write(f"  PROBLEM  {problem}\n")


def heap_reset_cmd():
    _peaks.clear()
    gdb.write("heap: peaks reset\n")


# --- heap_log: hooks on the allocator entry points ---

def _caller(lr):
    try:
        text = gdb.execute(f"info symbol 0x{lr & ~1:08x}", to_string=True).strip()
    except gdb.error:
        return f"0x{lr:08x}"
    return text.split(" in section")[0]


class _HeapLogBp(gdb.Breakpoint):
    """Print one allocator call and the heap summary before it, then auto-continue."""

    def __init__(self, spec, fmt):
        super().__init__(spec, internal=True)
        self.fmt = fmt
        self.enabled = False
        _log_bps.append(self)

    def stop(self):
        try:
            r0 = _val("$r0")
            r1 = _val("$r1")
            lr = _val("$lr")
            gdb.write(self.fmt.format(r0=r0, r1=r1) + f" from {_caller(lr)}\n")
            if _heap_count():
                gdb.write("    " + HeapStats(0).one_line() + "\n")
        except gdb.error as exc:
            gdb.write(f"heap_log: {exc}\n")
        return False


def _arm_log_hooks():
    # Raw entry addresses: a symbol breakpoint lands after the prologue, where
    # memory_malloc has already overwritten r0 with the alignment mask.
    for symbol, fmt in (("memory_malloc", "malloc  {r0}"),
                        ("memory_zalloc", "zalloc  {r0}"),
                        ("memory_realloc", "realloc 0x{r0:08x} -> {r1}"),
                        ("memory_free", "free    0x{r0:08x}")):
        try:
            entry = _val(f"(unsigned long){symbol}") & ~1
            _HeapLogBp(f"*0x{entry:08x}", fmt)
        except gdb.error as exc:
            gdb.write(f"heap.gdb: hook {symbol} failed: {exc}\n")


def heap_log_cmd(arg):
    want = arg.strip().lower() in ("on", "1", "true")
    for bp in _log_bps:
        bp.enabled = want
    gdb.write(f"heap_log {'on' if want else 'off'} ({len(_log_bps)} hooks)\n")


# --- heap_watch: sample at the main-loop tick ---

class _HeapWatchBp(gdb.Breakpoint):
    def __init__(self):
        super().__init__(f"*0x{MAIN_LOOP_TICK:08x}", internal=True)
        self.enabled = False

    def stop(self):
        _watch["frame"] += 1
        if _watch["frame"] % _watch["every"] != 0:
            return False
        try:
            if not _heap_count():
                return False
            s = HeapStats(0)
            key = (s.used, s.top)
            if key != _watch["last"]:
                _watch["last"] = key
                gdb.write(f"frame {_watch['frame']:6d}: {s.one_line()}\n")
                for problem in s.problems:
                    gdb.write(f"    PROBLEM  {problem}\n")
        except gdb.error as exc:
            gdb.write(f"heap_watch: {exc}\n")
        return False


def heap_watch_cmd(arg, every):
    want = arg.strip().lower() in ("on", "1", "true")
    if _watch["bp"] is None:
        try:
            _watch["bp"] = _HeapWatchBp()
        except gdb.error as exc:
            gdb.write(f"heap_watch: breakpoint failed: {exc}\n")
            return
    if every > 0:
        _watch["every"] = every
    _watch["bp"].enabled = want
    if want:
        _watch["frame"] = 0
        _watch["last"] = None
    gdb.write(f"heap_watch {'on' if want else 'off'} (every {_watch['every']} frames)\n")


# --- heap_guard: write watchpoint on a block header, ignoring the allocator ---

def _allocator_ranges():
    """PC ranges of the allocator: the public entry points and the static helpers."""
    ranges = []
    for lo, hi in (("memory_init", "memory_memset"), ("memory_addHeap", "game_tickFrame")):
        try:
            ranges.append((_val(f"(unsigned long){lo}") & ~1, _val(f"(unsigned long){hi}") & ~1))
        except gdb.error as exc:
            gdb.write(f"heap_guard: {exc}\n")
    return ranges


def _guard_report(addr, pc):
    """True when the write at pc is the allocator's own; otherwise print the writer."""
    if any(lo <= pc < hi for lo, hi in _guard["ranges"]):
        return True
    words = " ".join(f"{w:08x}" for w in _read_block(addr))
    gdb.write(f"heap_guard: header 0x{addr:08x} written by {_caller(pc)} "
              f"(pc 0x{pc:08x}), now {words}\n")
    return False


class _HeapGuardBp(gdb.Breakpoint):
    def __init__(self, addr):
        super().__init__(f"*(unsigned int (*)[{BLOCK_HEADER // 4}])0x{addr:08x}",
                         type=gdb.BP_WATCHPOINT, wp_class=gdb.WP_WRITE, internal=True)
        self.addr = addr

    def stop(self):
        return not _guard_report(self.addr, _val("$pc"))


def _guard_on_stop(event):
    """mGBA's stub reports a watchpoint hit as a bare SIGTRAP, so gdb never calls
    stop() above. Filter here instead and resume for the allocator's own writes."""
    if _guard["bp"] is None or not isinstance(event, gdb.SignalEvent):
        return
    if event.stop_signal != "SIGTRAP":
        return
    try:
        if _guard_report(_guard["bp"].addr, _val("$pc")):
            gdb.post_event(lambda: gdb.execute("continue"))
    except gdb.error as exc:
        gdb.write(f"heap_guard: {exc}\n")


def heap_guard_cmd(arg, addr):
    want = arg.strip().lower() in ("on", "1", "true")
    if _guard["bp"] is not None:
        _guard["bp"].delete()
        _guard["bp"] = None
    if not want:
        gdb.write("heap_guard off\n")
        return
    _guard["ranges"] = _allocator_ranges()
    if addr == 0:
        if not _heap_count():
            return
        addr = _val("(unsigned long)g_Heaps[0].firstBlock")
    try:
        _guard["bp"] = _HeapGuardBp(addr)
    except gdb.error as exc:
        gdb.write(f"heap_guard: watchpoint failed: {exc}\n")
        return
    gdb.write(f"heap_guard on: header 0x{addr:08x}, halting on writes from outside the allocator\n")


_arm_log_hooks()
gdb.events.stop.connect(_guard_on_stop)
gdb.write("heap.gdb loaded: heap, heap_map, heap_blocks, heap_log on|off, "
          "heap_watch on|off [n], heap_guard on|off [addr], heap_reset\n")
end

define heap
    set $heap_idx = 0
    if $argc >= 1
        set $heap_idx = $arg0
    end
    python heap_cmd(int(gdb.parse_and_eval("$heap_idx")))
end

define heap_map
    set $heap_idx = 0
    if $argc >= 1
        set $heap_idx = $arg0
    end
    python heap_map_cmd(int(gdb.parse_and_eval("$heap_idx")))
end

define heap_blocks
    set $heap_idx = 0
    if $argc >= 1
        set $heap_idx = $arg0
    end
    python heap_blocks_cmd(int(gdb.parse_and_eval("$heap_idx")))
end

define heap_log
    python heap_log_cmd("$arg0")
end

define heap_watch
    set $heap_every = 0
    if $argc >= 2
        set $heap_every = $arg1
    end
    python heap_watch_cmd("$arg0", int(gdb.parse_and_eval("$heap_every")))
end

define heap_guard
    set $heap_guard_addr = 0
    if $argc >= 2
        set $heap_guard_addr = $arg1
    end
    python heap_guard_cmd("$arg0", int(gdb.parse_and_eval("$heap_guard_addr")))
end

define heap_reset
    python heap_reset_cmd()
end
