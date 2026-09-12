# scenes.gdb: jump the live session to any mission or scene, and trace scene changes.
#
# Usage (after starting mGBA's stub, e.g. `mgba-qt -g rotk.gba`):
#     gdb -x build/debug/rotk.gdb -x gdb-scripts/scenes.gdb
# or from a running session (symbols must already be loaded):
#     (gdb) source gdb-scripts/scenes.gdb
#
# Halt the game first (Ctrl-C in gdb), then:
#   warp <missionId> [variant] [spawnPoint]
#       Load a mission the way a gameplay exit would (enum MissionId, include/mission.h).
#       The variant byte is kept unless given, spawnPoint defaults to 0.
#   scene <sceneId> [arg0] [arg1]
#       Request any scene (enum SceneId, include/scene.h) with the two handoff args.
#       For gameplay prefer warp, which also sets the spawn point and exit flags.
#   trace_scenes on|off
#       Print every scene change as it dispatches (on by default), e.g.
#       scene 5 (MAIN_MENU) -> 1 (GAMEPLAY) | mission 0x00 "Anduin Banks" | variant 0 | ...
#
# Both commands `settle` first, so the pokes happen in the user-mode main loop.
# Scene names come from enum SceneId in include/scene.h, mission names from the game
# via text_getString (an inferior call, masked behind REG_IME).

set unwind-on-signal on

# The trace breaks twice per change, never per frame: once inside game_tickFrame at the
# game_dispatchSceneEnter call, after the g_ScenePrevious/g_SceneCurrent swap (captures
# the transition), then once at the return address of rotk_main's game_tickFrame call
# (prints, after the enter handler ran so g_CurrentMissionId is authoritative). Offsets
# into the two functions; the thumb bit is stripped before adding them.
python
import gdb
import os
import re

REG_IME = "*(unsigned short*)0x04000208"
SCENE_ID_GAMEPLAY = 0x01
SCENE_ID_CUTSCENE = 0x07
TEXT_ID_MISSION_NAMES = 0x88  # TEXT_ID_ANDUIN_BANKS, one string per MissionId
MISSION_ID_MAX = 0x81  # MISSION_WHITE_MOUNTAINS_STORM

def _code_addr(symbol, offset):
    return (int(gdb.parse_and_eval(f"(unsigned long){symbol}")) & ~1) + offset

def _repo_root():
    prog = gdb.current_progspace()
    try:
        objs = prog.objfiles()  # gdb >= 17
    except TypeError:
        objs = prog.objfiles    # gdb <= 16: list property
    for obj in objs:
        if obj.filename.endswith("debug.elf"):
            return os.path.dirname(os.path.dirname(os.path.dirname(obj.filename)))
    return os.getcwd()

def _scene_names():
    try:
        with open(os.path.join(_repo_root(), "include", "scene.h")) as fh:
            text = fh.read()
    except OSError:
        return {}
    m = re.search(r"enum SceneId\s*{(.*?)}", text, re.S)
    if not m:
        return {}
    return {
        int(val, 16): name[len("SCENE_ID_"):]
        for name, val in re.findall(
            r"(SCENE_ID_[A-Z0-9_]+)\s*=\s*(0x[0-9a-fA-F]+)", m.group(1)
        )
    }

SCENE_NAMES = _scene_names()

def _val(expr):
    return int(gdb.parse_and_eval(expr))

def _scene_text(scene_id):
    name = SCENE_NAMES.get(scene_id)
    return f"{scene_id} ({name})" if name else f"{scene_id}"

def _user_breakpoints_disabled():
    saved = []
    for bp in gdb.breakpoints() or []:
        if bp.number >= 0:
            saved.append((bp, bp.enabled))
            bp.enabled = False
    def restore():
        for bp, enabled in saved:
            bp.enabled = enabled
    return restore

def _mission_name(mission):
    if mission > MISSION_ID_MAX:
        return None
    try:
        ime = _val(REG_IME)
    except gdb.error:
        ime = 1
    restore_bps = _user_breakpoints_disabled()
    try:
        gdb.execute(f"set {REG_IME} = 0")
        result = gdb.parse_and_eval(f"(char*)text_getString({TEXT_ID_MISSION_NAMES} + {mission})")
        if int(result) == 0:
            return None
        return result.string()
    except gdb.error:
        return None
    finally:
        try:
            gdb.execute(f"set {REG_IME} = {ime}")
        except gdb.error:
            pass
        restore_bps()

trace_state = {}

def _capture():
    trace_state.update(
        old_scene=_val("g_ScenePrevious.id"),
        new_scene=_val("g_SceneCurrent.id"),
        mode=_val("g_SceneCurrent.args[0]"),
        mission=_val("g_SceneCurrent.args[1]"),
        variant=_val("g_MissionVariant"),
        spawn_point=_val("g_MissionSpawnPoint[0]"),
    )

def _print_transition():
    s = trace_state
    mission = s["mission"]
    if s["new_scene"] == SCENE_ID_GAMEPLAY:
        # The transition arg can be stale after an overlay; the enter handler has run.
        mission = _val("g_CurrentMissionId")
    if s["new_scene"] in (SCENE_ID_GAMEPLAY, SCENE_ID_CUTSCENE):
        name = _mission_name(mission)
        name_text = f' "{name}"' if name else ""
        mission_text = f"mission 0x{mission:02x}{name_text}"
    else:
        mission_text = "mission n/a"
    gdb.write(
        "scene %s -> %s | %s | variant %u | spawn point %u | transition mode %u\n"
        % (_scene_text(s["old_scene"]), _scene_text(s["new_scene"]), mission_text,
           s["variant"], s["spawn_point"], s["mode"])
    )

class AfterTick(gdb.Breakpoint):
    def __init__(self):
        super().__init__(f"*0x{_code_addr('rotk_main', 0xb2):08x}", internal=True)
        self.enabled = False

    def stop(self):
        self.enabled = False
        try:
            _print_transition()
        except gdb.error as exc:
            gdb.write(f"trace_scenes: print failed: {exc}\n")
        return False

class OnSceneSwap(gdb.Breakpoint):
    def __init__(self, after_tick):
        super().__init__(f"*0x{_code_addr('game_tickFrame', 0xa0):08x}", internal=True)
        self.after_tick = after_tick

    def stop(self):
        try:
            _capture()
            self.after_tick.enabled = True
        except gdb.error as exc:
            gdb.write(f"trace_scenes: capture failed: {exc}\n")
        return False

after_tick = AfterTick()
on_scene_swap = OnSceneSwap(after_tick)

def trace_scenes_cmd(arg):
    want = arg.strip().lower() in ("on", "1", "true")
    on_scene_swap.enabled = want
    if not want:
        after_tick.enabled = False
    gdb.write(f"trace_scenes {'on' if want else 'off'}\n")
end

# Stage g_SceneNext like game_requestSceneChangeWithArgs: id | SCENE_NEXT_PENDING, args.
define _request_scene
    set g_SceneNext.id = $arg0 | 0x80
    set g_SceneNext.args[0] = $arg1
    set g_SceneNext.args[1] = $arg2
    set g_SceneNext.args[2] = 0
    set g_SceneNext.args[3] = 0
    set g_SceneNext.args[4] = 0
end

define warp
    if $argc < 1 || $argc > 3
        printf "usage: warp <missionId> [variant] [spawnPoint]\n"
    else
        if $arg0 < 0 || $arg0 > 0x81
            printf "warp: missionId 0x%x out of range (0..0x81)\n", $arg0
        else
            set $variant = -1
            if $argc >= 2
                set $variant = $arg1
            end
            set $spawnPoint = 0
            if $argc >= 3
                set $spawnPoint = $arg2
            end
            settle
            set g_MissionSpawnPoint[0] = $spawnPoint
            set g_MissionSpawnPoint[1] = $spawnPoint
            if $variant >= 0
                set g_MissionVariant = $variant
            end
            # SCENE_ID_GAMEPLAY, GAMEPLAY_MODE_START, mission
            _request_scene 0x01 0 $arg0
            # GAME_FLAG_31: gameplay exit warps set it alongside the request
            set g_GameFlags.p = g_GameFlags.p | 0x80000000
            printf "warp: mission 0x%02x variant %u spawn point %u\n", $arg0, g_MissionVariant, $spawnPoint
            continue
        end
    end
end

define scene
    if $argc < 1 || $argc > 3
        printf "usage: scene <sceneId> [arg0] [arg1]\n"
    else
        if $arg0 < 0 || $arg0 > 0x19 || $arg0 == 0x15
            printf "scene: sceneId 0x%x out of range (0..0x19, 0x15 is a NULL entry)\n", $arg0
        else
            set $a0 = 0
            if $argc >= 2
                set $a0 = $arg1
            end
            set $a1 = 0
            if $argc >= 3
                set $a1 = $arg2
            end
            settle
            _request_scene $arg0 $a0 $a1
            printf "scene: request 0x%02x args %u %u\n", $arg0, $a0, $a1
            continue
        end
    end
end

define trace_scenes
    python trace_scenes_cmd("$arg0")
end

printf "scenes.gdb loaded: warp, scene, trace_scenes (on)\n"
