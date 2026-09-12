# audio.gdb: play SfxId sounds and song tracks in the mGBA session, to check the ids.
#
# Usage (after starting mGBA's stub, e.g. `mgba-qt -g rotk.gba`):
#     gdb -x build/debug/rotk.gdb -x gdb-scripts/audio.gdb
# or from a running session (symbols must already be loaded):
#     (gdb) source gdb-scripts/audio.gdb
#
# Requires the debug bundle: every call starts with `settle` (from
# build/debug/utils.gdb) so the game is in the user-mode main loop, and calls mask
# REG_IME for their duration (a mixer IRQ mid-call corrupts the gdb call frame).
#
# Prereqs: be in a scene (title/menu/level) so audio is armed; SFX option on for
# sounds, music option on for songs.
#
# Halt the game first (Ctrl-C in gdb), then:
#   list_sfx                       print the SfxId enum from include/sfx.h
#   play_sfx <id> [secs]           stop music, play one sound, run freely ~secs
#                                  (default 1.0), halt
#   sweep_sfx [start] [end] [secs] play each sound in the range (default: all named)
#   stop_all_sfx                   sfx_stopAll()
#   list_song                      print valid song ids (0x00-0x21)
#   play_song <id> [secs]          play one track; keeps playing until Ctrl-C,
#                                  or halts after ~secs when given
#   sweep_song [start] [end] [secs]  play each track in the range (default: all,
#                                  8s per track)
#   stop_song                      sfx_stopMusic()
#   audio_log on|off               toggle the sfx_play / sfx_requestSong hooks
#                                  (on by default): game-played sounds and song
#                                  changes print their id at the prompt
#
# A sound only mixes while the CPU runs. Commands with a pause run the game
# freely (a timed Ctrl-C, no stepping) then halt; play_song without secs keeps
# running until you Ctrl-C. Resume with `continue`. sweep_sfx 0 0x7f hears
# every sound-table entry, named or not (ids without a table entry return
# handle 0 and play nothing).

python
import gdb
import os
import re
import signal
import threading

IME = "*(unsigned short*)0x04000208"
SONG_TABLE = 0x082898f0  # song ids 0x00-0x21, 8 bytes each (ptr, arg2, arg3, pad)
SONG_COUNT = 0x22

def _repo_root():
    """Repo root from the debug bundle's main objfile (build/debug/debug.elf)."""
    prog = gdb.current_progspace()
    try:
        objs = prog.objfiles()  # gdb >= 17
    except TypeError:
        objs = prog.objfiles    # gdb <= 16: list property
    for obj in objs:
        if obj.filename.endswith("debug.elf"):
            return os.path.dirname(os.path.dirname(os.path.dirname(obj.filename)))
    return os.getcwd()

def _sfx_table():
    """{id: name} from the SfxId enum in include/sfx.h (None if unparsable)."""
    try:
        text = open(os.path.join(_repo_root(), "include", "sfx.h")).read()
    except OSError:
        return None
    m = re.search(r"typedef enum SfxId\s*{(.*?)}", text, re.S)
    if not m:
        return None
    return {
        int(val, 16): name
        for name, val in re.findall(r"(SFX_[A-Z0-9_]+)\s*=\s*(0x[0-9a-fA-F]+)", m.group(1))
    }

def _song_valid(song_id):
    """True if the song table has a real pointer for song_id."""
    try:
        ptr = int(gdb.parse_and_eval(f"*(unsigned int*){SONG_TABLE + song_id * 8:#x}"))
    except gdb.error:
        return False
    return 0x08000000 <= ptr <= 0x08FFFFFF

_log_bps = []

class _AudioLogBp(gdb.Breakpoint):
    """Log a game-driven sfx_play / sfx_requestSong call, then auto-continue.
    Internal: _with_user_breaks_disabled skips it, so the hooks stay live during
    automated runs."""
    def __init__(self, spec, logfn):
        super().__init__(spec, internal=True)
        self.logfn = logfn
        _log_bps.append(self)

    def stop(self):
        try:
            self.logfn()
        except gdb.error:
            pass
        return False

def _log_sfx_play():
    table = _sfx_table()
    sfx_id = int(gdb.parse_and_eval("$r0")) & 0xff
    name = table.get(sfx_id) if table else None
    gdb.write(f"sfx 0x{sfx_id:02x} {name if name else '(unnamed)'}\n")

def _log_song_play():
    gdb.write(f"song 0x{int(gdb.parse_and_eval('$r0')) & 0xff:02x}\n")

def audio_log_cmd(arg):
    want = arg.strip().lower() in ("on", "1", "true")
    for bp in _log_bps:
        bp.enabled = want
    gdb.write(f"audio log {'on' if want else 'off'} ({len(_log_bps)} hooks)\n")

def _arm_hooks():
    for spec, logfn in (("sfx_play", _log_sfx_play),
                        ("sfx_requestSong", _log_song_play)):
        try:
            _AudioLogBp(spec, logfn)
        except gdb.error as exc:
            gdb.write(f"audio.gdb: hook {spec} failed: {exc}\n")
    if _log_bps:
        gdb.write(f"audio.gdb: logging sfx_play / sfx_requestSong "
                  f"({len(_log_bps)} hooks); audio_log off to silence\n")

def _is_internal(bp):
    try:
        return bp.internal
    except AttributeError:
        return bp.number < 0

def _with_user_breaks_disabled():
    """Disable user breakpoints for an automated run; returns a restore callable."""
    saved = [(bp, bp.enabled) for bp in gdb.breakpoints() or [] if not _is_internal(bp)]
    for bp, _ in saved:
        bp.enabled = False
    def restore():
        for bp, enabled in saved:
            bp.enabled = enabled
    return restore

def _target_call(expr):
    """Inferior-call expr with REG_IME masked and the logging hooks off (an entry
    breakpoint firing mid-call would break the call); restores both."""
    try:
        ime = int(gdb.parse_and_eval(IME))
    except gdb.error:
        ime = 1
    try:
        gdb.execute(f"set {IME} = 0")
    except gdb.error as exc:
        gdb.write(f"call setup failed: {exc}\n")
        gdb.write("hint: halt the game first (Ctrl-C in gdb)\n")
        return False, None
    for bp in _log_bps:
        bp.enabled = False
    try:
        return True, gdb.parse_and_eval(expr)
    except gdb.error as exc:
        gdb.write(f"call failed: {exc}\n")
        gdb.write("hint: halt the game first (Ctrl-C in gdb)\n")
        return False, None
    finally:
        for bp in _log_bps:
            bp.enabled = True
        try:
            gdb.execute(f"set {IME} = {ime}")
        except gdb.error:
            pass

def _settle():
    """Run the game to the main-loop tick (utils.gdb's `settle`) so calls start
    from a sane user-mode context."""
    try:
        gdb.execute("settle")
        return True
    except gdb.error as exc:
        gdb.write(f"settle failed: {exc}\n")
        gdb.write("hint: source the debug bundle first (rotk.gdb defines settle)\n")
        return False

def _run_free(secs):
    """Run the game freely for ~secs wall-clock, then halt (timed Ctrl-C)."""
    if secs <= 0:
        gdb.write("    (secs <= 0, not running)\n")
        return
    restore = _with_user_breaks_disabled()
    timer = threading.Timer(secs, lambda: os.kill(os.getpid(), signal.SIGINT))
    timer.daemon = True
    timer.start()
    try:
        gdb.execute("continue")
    finally:
        timer.cancel()
        restore()

# --- sfx ---

def list_sfx_cmd():
    table = _sfx_table()
    if table is None:
        gdb.write("list_sfx: could not parse include/sfx.h\n")
        return
    for sfx_id in sorted(table):
        gdb.write(f"0x{sfx_id:02x}  {table[sfx_id]}\n")

def play_sfx_cmd(id_str, secs):
    table = _sfx_table()
    try:
        sfx_id = int(id_str, 0)
    except ValueError:
        gdb.write(f"play_sfx: bad id {id_str!r} (hex like 0x11, or decimal)\n")
        return
    if not 0 <= sfx_id <= 0xff:
        gdb.write(f"play_sfx: id {sfx_id} out of range (0..0xff)\n")
        return
    if not _settle():
        return
    ok, _ = _target_call("(void) sfx_stopMusic()")
    if not ok:
        return
    ok, handle = _target_call(f"(u32) sfx_play({sfx_id})")
    if not ok:
        return
    name = table.get(sfx_id) if table else None
    gdb.write(f">>> 0x{sfx_id:02x} "
              f"{name or '(unnamed; not in SfxId enum)'}: handle 0x{int(handle):x}\n")
    if int(handle) == 0:
        gdb.write("    handle 0: SFX option off, or the id is not in the sound table\n")
    _run_free(secs)
    gdb.write(f"<<< halted after {secs:g}s\n")

def sweep_sfx_cmd(start, end, secs):
    table = _sfx_table()
    if table is None:
        gdb.write("sweep_sfx: could not parse include/sfx.h\n")
        return
    if start < 0:
        ids = sorted(table)
    else:
        if end < start:
            gdb.write(f"sweep_sfx: end {end} < start {start}\n")
            return
        ids = range(start, end + 1)
    gdb.write(f"sweep_sfx: {len(ids)} sounds, {secs:g}s each\n")
    for sfx_id in ids:
        play_sfx_cmd(f"{sfx_id}", secs)
    gdb.write("sweep_sfx: done\n")

def stop_all_sfx_cmd():
    if not _settle():
        return
    ok, _ = _target_call("(void) sfx_stopAll()")
    if ok:
        gdb.write("sfx_stopAll() done\n")

# --- songs ---

def list_song_cmd():
    for song_id in range(SONG_COUNT):
        if _song_valid(song_id):
            gdb.write(f"0x{song_id:02x}\n")

def play_song_cmd(id_str, secs):
    try:
        song_id = int(id_str, 0)
    except ValueError:
        gdb.write(f"play_song: bad id {id_str!r} (hex like 0x05, or decimal)\n")
        return
    if not 0 <= song_id < SONG_COUNT:
        gdb.write(f"play_song: id {song_id} out of range (0x00..0x{SONG_COUNT - 1:x})\n")
        return
    if not _song_valid(song_id):
        gdb.write(f"play_song: 0x{song_id:02x} is not in the song table (bad id?)\n")
        return
    if not _settle():
        return
    ok, _ = _target_call(f"(void) sfx_requestSong({song_id})")
    if not ok:
        return
    gdb.write(f">>> song 0x{song_id:02x}\n")
    if secs > 0:
        _run_free(secs)
        gdb.write(f"<<< halted after {secs:g}s (song keeps playing; stop_song to cut it)\n")
    else:
        gdb.write("playing; Ctrl-C to halt, stop_song to cut it\n")
        gdb.execute("continue")
        gdb.write("<<< halted\n")

def sweep_song_cmd(start, end, secs):
    if start < 0:
        ids = [i for i in range(SONG_COUNT) if _song_valid(i)]
    else:
        if end < start:
            gdb.write(f"sweep_song: end {end} < start {start}\n")
            return
        ids = range(start, min(end, SONG_COUNT - 1) + 1)
    gdb.write(f"sweep_song: {len(ids)} tracks, {secs:g}s each\n")
    for song_id in ids:
        play_song_cmd(f"{song_id}", secs)
    gdb.write("sweep_song: done\n")

def stop_song_cmd():
    if not _settle():
        return
    ok, _ = _target_call("(void) sfx_stopMusic()")
    if ok:
        gdb.write("sfx_stopMusic() done\n")

try:
    gdb.execute("set unwind-on-signal on")
except gdb.error:
    pass
_arm_hooks()
gdb.write("audio.gdb loaded: list_sfx / play_sfx / sweep_sfx / stop_all_sfx, "
          "list_song / play_song / sweep_song / stop_song, "
          "audio_log on|off\n")
end

define play_sfx
    set $sfx_secs = 1.0
    if $argc >= 2
        set $sfx_secs = $arg1
    end
    python play_sfx_cmd("$arg0", float(gdb.parse_and_eval("$sfx_secs")))
end

define sweep_sfx
    set $sfx_secs = 1.0
    if $argc >= 2
        set $sfx_start = $arg0
        set $sfx_end = $arg1
    else
        set $sfx_start = -1
        set $sfx_end = -1
    end
    if $argc >= 3
        set $sfx_secs = $arg2
    end
    python sweep_sfx_cmd(int(gdb.parse_and_eval("$sfx_start")), int(gdb.parse_and_eval("$sfx_end")), float(gdb.parse_and_eval("$sfx_secs")))
end

define list_sfx
    python list_sfx_cmd()
end

define stop_all_sfx
    python stop_all_sfx_cmd()
end

define play_song
    set $song_secs = 0
    if $argc >= 2
        set $song_secs = $arg1
    end
    python play_song_cmd("$arg0", float(gdb.parse_and_eval("$song_secs")))
end

define sweep_song
    set $song_secs = 8.0
    if $argc >= 2
        set $song_start = $arg0
        set $song_end = $arg1
    else
        set $song_start = -1
        set $song_end = -1
    end
    if $argc >= 3
        set $song_secs = $arg2
    end
    python sweep_song_cmd(int(gdb.parse_and_eval("$song_start")), int(gdb.parse_and_eval("$song_end")), float(gdb.parse_and_eval("$song_secs")))
end

define list_song
    python list_song_cmd()
end

define stop_song
    python stop_song_cmd()
end

define audio_log
    python audio_log_cmd("$arg0")
end
