"""Raw mission script dumps, transition graph and chapter chains.

Parses all 130 mission blocks (rotkit.missionscript), extracts the scene-transition
records (advance/cutscene/end ops), and follows the chains from the per-hero start
missions to build the 4 paths of the chapter-select hack.

Progression facts the extraction relies on:
  - op 19 advance:  spawn-point byte = b5; game_requestSceneChangeWithArgs(GAMEPLAY, 0, b4)
  - op 28 cutscene: spawn-point byte = b5; cutscene b6, then mission b4
  - op 56:          op 19 but only when g_GameFlags bit 5 (0x20) is set
  - op 44:          spawn point = b5; game_requestSceneChangeWithArgs(0x15, b6, b4) - scene
                    0x15 is a NULL SceneHandlers entry (dead op; flagged)
  - op 46/47:       chain terminators (scene 9 param 3 / scene 5)
  - type-3 warps:   mission @+0xb, spawn point @+0xa; on trigger the stream @+0xc
                    runs synchronously and its transition ops supersede the raw
                    target (0x00/0x01 = unset placeholder), its op 15s set the
                    pending variant
  - op 15 set_variant writes the variant byte g_MissionVariant (the next
    gameplay load reads it; NOT reset per mission, only at boot/MP); b5 != 0
    gates the write on the multiplayer flag, so it is dead in single-player
"""

import re
from collections import defaultdict
from dataclasses import dataclass, field, replace
from struct import unpack_from
from typing import Annotated

import typer

from rotkit.missionscript import (
    ACTOR_TYPE_CHEST,
    ACTOR_TYPE_MISSION_WARP,
    MISSION_COUNT,
    MissionBlock,
    OPCODE_NAMES,
    OP_ADVANCE_IF_FLAG20,
    OP_ADVANCE_MISSION,
    OP_END_TO_SCENE_9,
    OP_GOTO_SCENE_0x15,
    OP_RETURN_SCENE_5,
    OP_START_CUTSCENE,
    MissionVariant,
)
from rotkit.rom import ROMBASE, load_rom
from rotkit.textdb import decode_strings

# Per-hero start state, indexed by enum HeroId (player.h), from the ROM tables
# HeroStartMissions @0x080507bc and HeroStartVariants @0x080507c2 (the tables
# overlap by 2 bytes; the shared values coincide).
START_MISSIONS = [0x4F, 0x33, 0x33, 0x33, 0x6D, 0x33, 0x06, 0x00]
START_VARIANTS = [6, 0, 0, 2, 4, 0, 50, 0]

# The 4 chapter-select paths and the hero start each follows.
PATHS = {
    "Aragorn": 2,  # shared with Legolas/Gimli (same start mission + variant)
    "Frodo": 0,
    "Gandalf": 3,
    "Eowyn": 4,
}

TRANSITION_OPS = {
    OP_ADVANCE_MISSION: "advance",
    OP_START_CUTSCENE: "cutscene",
    OP_GOTO_SCENE_0x15: "scene0x15",
    OP_ADVANCE_IF_FLAG20: "advance_f20",
    OP_END_TO_SCENE_9: "end_scene9",
    OP_RETURN_SCENE_5: "end_scene5",
}


@dataclass(frozen=True)
class Transition:
    """One transition record found in a mission's scripts."""

    mission: int
    variant: int
    stream: int
    kind: str  # TRANSITION_OPS value
    next_mission: int  # b4 (0 for enders)
    spawn_point: int  # b5
    extra: int  # b6 (cutscene id / scene param)


@dataclass
class Graph:
    transitions: list[Transition] = field(default_factory=list)
    by_mission: dict[int, list[Transition]] = field(
        default_factory=lambda: defaultdict(list)
    )


def build_graph(rom: bytes) -> Graph:
    """Extract every transition record from every mission's script streams."""
    g = Graph()
    for m in range(MISSION_COUNT):
        mb = MissionBlock(rom, m)
        for si in mb.variant_indices():
            ss = mb.variant(si)
            for s in ss.streams:
                for r in s.records:
                    kind = TRANSITION_OPS.get(r.opcode)
                    if kind is None:
                        continue
                    t = Transition(
                        mission=m,
                        variant=si,
                        stream=s.index,
                        kind=kind,
                        next_mission=r.data[4] if len(r.data) > 4 else 0,
                        spawn_point=r.data[5] if len(r.data) > 5 else 0,
                        extra=r.data[6] if len(r.data) > 6 else 0,
                    )
                    g.transitions.append(t)
                    g.by_mission[m].append(t)
            for gi, group in enumerate(ss.groups):
                for a in group:
                    if a.type != ACTOR_TYPE_MISSION_WARP or len(a.data) < 0xD:
                        continue
                    # the linked stream's transition ops supersede the raw target and
                    # its ender ops make the warp a non-mission exit
                    res = warp_stream_exit(ss, a.data[0xC], _all_arms)
                    raw = ExitOp(a.data[0xB], a.data[0xA], "warp", a.data[8])
                    rows = []
                    if res is None:
                        rows.append(raw)
                    elif res.transitions:
                        rows.extend(
                            replace(op, kind=op.kind + "+warp")
                            for op in res.transitions
                        )
                    elif not res.ended:
                        rows.append(raw)
                    for op in rows:
                        t = Transition(
                            mission=m,
                            variant=si,
                            stream=gi,
                            kind=op.kind,
                            next_mission=op.next_mission,
                            spawn_point=op.spawn_point,
                            extra=op.extra,
                        )
                        g.transitions.append(t)
                        g.by_mission[m].append(t)
    return g


_GRAPH_HEADER = """\
Mission-script transition graph: every way play can move between missions,
extracted from the scripts and actor spawn groups of all 130 mission blocks.
Missions not listed have no exits of their own; they are reached via other
missions' rows. For per-hero chains see `chains`; for chapter-select C
initializers see `chapters`.
"""

_KIND_LABEL = {
    "warp": "warp",
    "advance": "advance",
    "cutscene": "cutscene",
    "advance+warp": "advance (via warp stream)",
    "cutscene+warp": "cutscene (via warp stream)",
    "advance_f20+warp": "advance (mp only, via warp stream)",
    "advance_f20": "advance (multiplayer only)",
    "scene0x15": "scene 0x15 (dead op)",
    "end_scene9": "to save menu",
    "end_scene5": "to main menu",
}


# Variant slots per hero group: even slots are the four paths'
# heroes, odd slots their phase-2 sections (Sam shares Frodo's 6/7).
_SLOT_HEROES = [
    ("Aragorn & co", (0, 1)),
    ("Gandalf", (2, 3)),
    ("Eowyn", (4, 5)),
    ("Frodo & Sam", (6, 7)),
]


def _variant_set(variants: set[int]) -> str:
    """Variant set as the hero groups it covers.

    All groups -> "all heroes"; 1-2 groups missing -> "all heroes except X";
    otherwise the present groups. Falls back to raw slots outside 0-7.
    """
    if not variants or max(variants) > 7:
        return ",".join(str(v) for v in sorted(variants))
    present = [name for name, slots in _SLOT_HEROES if variants & set(slots)]
    missing = [name for name, slots in _SLOT_HEROES if not (variants & set(slots))]
    if not missing:
        return "all heroes"
    if len(missing) <= 2 and len(present) >= 2:
        return "all heroes except " + " and ".join(missing)
    return ", ".join(present)


def run_graph() -> None:
    """Dump the mission-script transition graph."""
    rom = load_rom()
    names = decode_strings(rom)

    def name(m: int) -> str:
        return names.get(0x88 + m, "?")

    g = build_graph(rom)
    print(_GRAPH_HEADER)
    n_unique = 0
    for m in sorted(g.by_mission):
        # collapse identical (kind, target, sp) rows, merging variants and vias
        merged: dict[tuple, set] = {}
        via: dict[tuple, set] = {}
        cutscenes: dict[tuple, set] = {}
        for t in g.by_mission[m]:
            key = (t.kind, t.next_mission, t.spawn_point)
            merged.setdefault(key, set()).add(t.variant)
            via.setdefault(key, set()).add(t.stream)
            if "cutscene" in t.kind:
                cutscenes.setdefault(key, set()).add(t.extra)
        n_unique += len(merged)
        print(f'mission {m:#04x} "{name(m)}"')
        for key, variants in merged.items():
            kind, nxt, sp = key
            label = _KIND_LABEL[kind]
            if "cutscene" in kind:
                label += " " + ",".join(hex(c) for c in sorted(cutscenes[key]))
            if kind in ("end_scene9", "end_scene5"):
                target = ""
            else:
                target = f'-> {nxt:#04x} "{name(nxt)}"  '.ljust(34)
            noun = "groups" if "warp" in kind else "streams"
            vias = ",".join(str(v) for v in sorted(via[key]))
            print(
                f"  {target}"
                + f"spawn point {sp}".ljust(16)
                + label.ljust(26)
                + f"via {noun} {vias}".ljust(26)
                + f"variants {_variant_set(variants)}"
            )
        print()
    print(
        f"{n_unique} unique transitions, {len(g.transitions)} records total,"
        f" {len(g.by_mission)} missions with transitions"
    )


FOLLOW_KINDS = ("advance", "cutscene", "advance_f20")


@dataclass(frozen=True)
class ExitOp:
    """One transition op's payload: target mission, spawn-point byte, kind."""

    next_mission: int
    spawn_point: int
    kind: str
    extra: int = 0  # b6: cutscene id / scene param


@dataclass(frozen=True)
class StreamExit:
    """What a warp's linked trigger stream contributes to the exit."""

    transitions: list[ExitOp]
    g_writes: set[int]  # op 15 targets
    ended: bool  # the stream leaves gameplay (op 46/47)


def _all_arms(r) -> list[tuple[int, int]]:
    """All C4ac (spawn, start) arms of an edge op, undecided."""
    if r.opcode in (5, 12) and len(r.data) > 7:
        return [(r.data[6], r.data[7])]
    if r.opcode == 68 and len(r.data) > 7:
        return [(r.data[4], r.data[5]), (r.data[6], r.data[7])]  # MP + SP arms
    if r.opcode in (52, 57, 62) and len(r.data) > 10:
        return [(r.data[7], r.data[8]), (r.data[9], r.data[10])]
    return []


def warp_stream_exit(sub, link: int, edges_of) -> StreamExit | None:
    """Resolve the stream a type-3 warp starts on trigger (record byte +0xc).

    The stream runs synchronously after the warp's raw transition is queued,
    so its transition ops supersede the raw target and its op 15s set the
    variant the next mission loads with. None when the warp has no linked
    stream.
    """
    if not link or link >= len(sub.streams):
        return None
    trans = []
    writes = set()
    ended = False
    seen = set()
    queue = [link]
    while queue:
        si = queue.pop()
        if si in seen or si >= len(sub.streams):
            continue
        seen.add(si)
        for r in sub.streams[si].records:
            kind = TRANSITION_OPS.get(r.opcode)
            if kind in FOLLOW_KINDS:
                trans.append(
                    ExitOp(
                        r.data[4] if len(r.data) > 4 else 0,
                        r.data[5] if len(r.data) > 5 else 0,
                        kind,
                        r.data[6] if len(r.data) > 6 else 0,
                    )
                )
            elif kind in ("end_scene9", "end_scene5"):
                ended = True
            if r.opcode == 15 and len(r.data) > 5 and r.data[5] == 0:
                writes.add(r.b4)
            for _stop, start in edges_of(r):
                if start:
                    queue.append(start)
    return StreamExit(trans, writes, ended)


# Script variable table 1 (MissionScriptVarTable1): index 0 = g_MissionVariant
# (RAM);
# indices 1-8 point to ROM constants. Only the compare ops (52/57) read through this
# table; the read-modify-write ops (58-61) address g_MissionScriptVars + index
# directly, and op 62 reads g_MissionCheckpointVars.
VAR_TABLE1_ADDR = 0x082826F8

# RegionMissionRanges @0x0806d20c: 28 x {u16 loMission, u16 hiMission}; the region
# name text block is TEXT_ID_REGION_NAME_BASE (107) + range index (enum RegionId).
REGION_RANGES_ADDR = 0x0806D20C
REGION_COUNT = 28
TEXT_ID_REGION_NAME_BASE = 107


def mission_region_id(rom: bytes, mission: int) -> int | None:
    """Region id of a mission via RegionMissionRanges."""
    for i in range(REGION_COUNT):
        lo, hi = unpack_from("<HH", rom, REGION_RANGES_ADDR - ROMBASE + i * 4)
        if lo <= mission <= hi:
            return i
    return None


def region_name_of(rom: bytes, names: dict[int, str], mission: int) -> str:
    """Region name of a mission. Distinct region ids can share a name
    (the two Minas Tirith tiers), so group by name, not id."""
    idx = mission_region_id(rom, mission)
    return names.get(TEXT_ID_REGION_NAME_BASE + idx, "?") if idx is not None else "?"


# Compare modes for op 52 (5 modes) and op 57 (6).
_CMP_OP52 = {
    0: lambda v, n: v < n,
    1: lambda v, n: v <= n,
    2: lambda v, n: v == n,
    3: lambda v, n: v >= n,
    4: lambda v, n: v > n,
}
_CMP_OP57 = {**_CMP_OP52, 5: lambda v, n: v != n}


class NodeSim:
    """Abstract interpreter for one mission load at a given variant byte.

    Walks every stream in the loaded variant, applying the VM's nested
    mission_script_spawnGroupAndStartStream(spawn, start) edges: groups spawn from
    stop-args (init streams 0/1 spawn groups 0/1), streams start from start-args. Streams
    not started by any script edge still run via gameplay triggers (region
    /boss/NPC code), so all streams are evaluated; only groups never
    spawned by any edge are assumed trigger-spawned (present). Conditional
    ops on script-var table 1 are statically decidable (var 0 = the variant
    byte, vars 1-8 = ROM consts; op 52/57 modes resolved); op 62 reads the
    runtime checkpoint vars and forks both arms with the node marked
    uncertain. Hero routing (op 52 on var 0) is decided exactly.
    """

    def __init__(self, rom: bytes, sub, g_entry: int) -> None:
        self.rom = rom
        self.sub = sub
        self.g_entry = g_entry
        self.g = g_entry  # per-stream variant byte (op 15 rewrites)
        self.init_streams = {0, 1} if sub.mission_flags & 1 else {0}
        self.spawned_groups = set(self.init_streams)
        self.edge_spawned = set()  # groups spawned by a decided edge arm
        self.g_writes_init = set()  # op 15 targets in init streams
        self.g_writes_active = set()  # op 15 targets in script-activated streams
        self.g_writes_all = set()  # op 15 targets anywhere
        self.stream_g_writes: dict[int, set] = defaultdict(set)  # per stream
        self.stream_started_by: dict[int, set] = defaultdict(set)  # start edges
        self.active_streams: set[int] = set()
        self.uncertain = False

    def _var1(self, idx: int) -> int:
        if idx == 0:
            return self.g
        ptr = unpack_from("<I", self.rom, VAR_TABLE1_ADDR - ROMBASE + idx * 4)[0]
        return self.rom[ptr - ROMBASE]

    def _edges(self, r) -> list[tuple[int, int]]:
        op = r.opcode
        if op == 5 or op == 12:
            # dialog-close / timer-expiry: fires unconditionally (SP)
            return [(r.data[6], r.data[7])]
        if op == 52:
            hit = _CMP_OP52[r.b5](self._var1(r.b4), r.data[6])
            return [(r.data[7], r.data[8])] if hit else [(r.data[9], r.data[10])]
        if op == 57:
            hit = _CMP_OP57[r.data[6]](self._var1(r.b4), r.b5)
            return [(r.data[7], r.data[8])] if hit else [(r.data[9], r.data[10])]
        if op == 62:
            # runtime checkpoint var: follow both arms, mark uncertain if the
            # fork can change any transition (see _op62_matters)
            self.uncertain = self.uncertain or self._op62_matters(r)
            return [(r.data[7], r.data[8]), (r.data[9], r.data[10])]
        if op == 68:
            return [(r.data[6], r.data[7])]  # SP arm
        return []

    def warp_stream_exit(self, link: int):
        """Resolve this warp's trigger stream (record byte +0xc), decided."""
        return warp_stream_exit(self.sub, link, self._edges)

    def _op62_matters(self, r) -> bool:
        """Whether an op-62 fork can change this node's emitted transitions.

        True iff either arm spawns a group containing a mission warp, or starts
        a stream that (transitively) reaches a transition op.
        """
        sub = self.sub
        for stop in (r.data[7], r.data[9]):
            if (
                stop
                and stop < len(sub.groups)
                and any(a.type == ACTOR_TYPE_MISSION_WARP for a in sub.groups[stop])
            ):
                return True
        # transitive closure over stream start edges (both arms of cond ops)
        starts_of: dict[int, set] = defaultdict(set)
        for s in sub.streams:
            for rec in s.records:
                if rec.opcode in (5, 12, 68) and len(rec.data) > 7:
                    starts_of[s.index].add(rec.data[7])
                elif rec.opcode in (52, 57, 62) and len(rec.data) > 10:
                    starts_of[s.index].add(rec.data[8])
                    starts_of[s.index].add(rec.data[10])
        seen = set()
        queue = [st for st in (r.data[8], r.data[10]) if st]
        while queue:
            cur = queue.pop()
            if cur in seen or cur >= len(sub.streams):
                continue
            seen.add(cur)
            if any(rec.opcode in TRANSITION_OPS for rec in sub.streams[cur].records):
                return True
            queue.extend(starts_of.get(cur, ()))
        return False

    @property
    def g_base(self) -> set[int]:
        """Pending variant byte from certain writes (load-time/activated streams)."""
        return (self.g_writes_active | self.g_writes_init) or {self.g_entry}

    @property
    def g_trigger(self) -> set[int]:
        """op 15 targets in trigger streams (may run before a trigger-gated exit).

        If the entry byte is among them, the current hero's own trigger stream
        just re-affirms it and the rest belong to other heroes: keep the entry.
        """
        trig = self.g_writes_all - self.g_writes_active - self.g_writes_init
        if self.g_entry in trig:
            return {self.g_entry}
        return trig

    @property
    def g_trigger_agreed(self) -> set[int]:
        """Trigger op 15 targets written by 2+ distinct trigger streams.

        Redundant normalization (0x47's two set_variant-3 streams) is the
        reliable kind; a lone trigger write is as often bookkeeping that must
        not leak into walkable exits (0x75's set_variant 7).
        """
        trig = self.g_trigger
        return {
            t
            for t in trig
            if sum(t in self.stream_g_writes[s] for s in self.trigger_streams) >= 2
        }

    @property
    def trigger_streams(self) -> set[int]:
        return set(self.stream_g_writes) - self.init_streams - self.active_streams

    def paired_g_writes(self, stream: int) -> set[int]:
        """op 15 targets in `stream` or in streams that (transitively) start it.

        A scripted exit's normalization bookkeeping rides in the exit stream
        itself (0x6e's s9) or in a prefix stream linked by a decidable start
        edge (0x5a's timer: set_variant stream starts the advance stream).
        """
        out = set(self.stream_g_writes.get(stream, ()))
        seen = set()
        queue = [stream]
        while queue:
            cur = queue.pop()
            if cur in seen:
                continue
            seen.add(cur)
            for prev in self.stream_started_by.get(cur, ()):
                out |= self.stream_g_writes.get(prev, set())
                queue.append(prev)
        return out

    def g_pending(self, trigger_gated: bool) -> set[int]:
        """Pending variant for a warp exit; trigger writes overwrite base when
        the warp group is spawned by a stream edge (the exit is unlocked by a
        trigger, so that trigger's op 15 bookkeeping ran first)."""
        if trigger_gated and self.g_trigger:
            return set(self.g_trigger)
        return self.g_base

    def run(self, sub) -> "NodeSim":
        # Phase A: stream activation from init streams through decidable
        # SpawnGroupAndStartStream edges; op 15s here are the reliable pending-g writes.
        active = set(self.init_streams)
        queue = sorted(active)
        self.g = self.g_entry
        while queue:
            si = queue.pop(0)
            if si >= len(sub.streams):
                continue
            for r in sub.streams[si].records:
                if r.opcode == 15 and len(r.data) > 5 and r.data[5] == 0:
                    # b5 != 0 gates the write on the multiplayer flag: dead in SP
                    self.g = r.b4
                    self.g_writes_active.add(r.b4)
                    continue
                for stop, start in self._edges(r):
                    if stop:
                        self.spawned_groups.add(stop)
                        self.edge_spawned.add(stop)
                    if start and start not in active:
                        active.add(start)
                        queue.append(start)
        # Phase B: every stream (trigger streams run eventually) for spawn
        # routing and op 15 bookkeeping.
        referenced = set()  # groups named as stop-arg by any edge op (either arm)
        for s in sub.streams:
            self.g = self.g_entry  # stream-local variant byte
            for r in s.records:
                if r.opcode in (52, 57, 62) and len(r.data) > 10:
                    referenced.add(r.data[7])
                    referenced.add(r.data[9])
                elif r.opcode in (5, 12) and len(r.data) > 6:
                    referenced.add(r.data[6])
                elif r.opcode == 68 and len(r.data) > 6:
                    referenced.add(r.data[4])  # MP arm
                    referenced.add(r.data[6])  # SP arm
                if r.opcode == 15 and len(r.data) > 5 and r.data[5] == 0:
                    self.g = r.b4
                    self.g_writes_all.add(r.b4)
                    self.stream_g_writes[s.index].add(r.b4)
                    if s.index in self.init_streams:
                        self.g_writes_init.add(r.b4)
                    continue
                for stop, start in self._edges(r):
                    if stop:
                        self.spawned_groups.add(stop)
                        self.edge_spawned.add(stop)
                    if start:
                        self.stream_started_by[start].add(s.index)
        # Routing-gated groups (named by an edge op but not selected) stay out;
        # groups never named by any edge are gameplay-trigger spawns: present.
        for gi in range(len(sub.groups)):
            if gi not in referenced:
                self.spawned_groups.add(gi)
        self.active_streams = active
        return self


@dataclass(frozen=True)
class ChainEdge:
    target: tuple[int, int]  # (mission, variant)
    spawn_point: int
    kind: str  # TRANSITION_OPS value or "warp"
    certain: bool


@dataclass(frozen=True)
class ChainNode:
    """One chain node: a mission loaded at a given variant byte."""

    mission: int
    g: int  # g_MissionVariant at load
    spawn_point: int
    certain: bool  # False past an op-62 runtime fork

    @property
    def key(self) -> tuple[int, int]:
        return (self.mission, self.g)


@dataclass(frozen=True)
class WarpEdge:
    """One spawned warp's exit: raw, or superseded by its linked trigger stream."""

    op: ExitOp
    pending: set[int] | None  # linked stream's op 15 targets; None = unlinked
    group: int


ChainEdges = dict[tuple[int, int], list[ChainEdge]]  # keyed by source (mission, g)


def path_chains(rom: bytes) -> tuple[dict[str, list[ChainNode]], dict[str, ChainEdges]]:
    """Follow each path as a (mission, variant byte) reachability graph.

    The loaded variant supplies each node's streams/groups; a NodeSim decides
    which streams run and which groups spawn (hero-routing via op 52 on var 0
    is the common case). Transitions come only from active streams (ops) and
    spawned groups (warps); pending g = entry g plus op 15 targets in active
    streams. Returns (per-path nodes in BFS first-visit order, per-path edges).
    """
    result = {}
    edges_by_path: dict[str, ChainEdges] = {}
    for path, hero in PATHS.items():
        start = START_MISSIONS[hero]
        start_g = START_VARIANTS[hero]
        seen = set()
        order = []
        edges: ChainEdges = defaultdict(list)
        edges_by_path[path] = edges
        queue = [ChainNode(start, start_g, 0, True)]
        while queue:
            node = queue.pop(0)
            if node.key in seen or node.mission >= MISSION_COUNT:
                continue
            seen.add(node.key)
            order.append(node)
            mission, g_entry = node.mission, node.g
            mb = MissionBlock(rom, mission)
            if g_entry >= len(mb.variant_map) or mb.variant_map[g_entry] == 0xFF:
                print(f"  !! {path}: mission {mission:#x} invalid at g={g_entry}")
                continue
            sub = mb.variant(g_entry)
            sim = NodeSim(rom, sub, g_entry).run(sub)
            node_certain = node.certain and not sim.uncertain

            # Path terminator: a victory cutscene (op 28 with cutscene id >=
            # 0x3a rolls the victory scene) anywhere in the loaded variant.
            # Detected per node: 0x3a only terminates in its phase-2 variant.
            terminator = any(
                r.opcode == OP_START_CUTSCENE and len(r.data) > 6 and r.data[6] >= 0x3A
                for s in sub.streams
                for r in s.records
            )
            if terminator:
                continue

            # Scripted exits (op 19/28/56) in any stream; trigger streams run
            # eventually. A scripted exit none of this hero's spawned warps
            # reach is another hero's trigger (NPC/dialog-gated, e.g. 0x7b's
            # advance -> 0x5c for non-Gandalf heroes): drop it.
            scripted = []  # (ExitOp, stream index)
            for s in sub.streams:
                for r in s.records:
                    kind = TRANSITION_OPS.get(r.opcode)
                    if kind not in FOLLOW_KINDS:
                        continue
                    scripted.append(
                        (
                            ExitOp(
                                r.data[4] if len(r.data) > 4 else 0,
                                r.data[5] if len(r.data) > 5 else 0,
                                kind,
                            ),
                            s.index,
                        )
                    )
            # Mission warps in spawned groups; a linked stream's transition ops
            # supersede the raw target and its op 15s set the pending variant.
            warp_edges: list[WarpEdge] = []
            for gi in sorted(sim.spawned_groups):
                if gi >= len(sub.groups):
                    continue
                for a in sub.groups[gi]:
                    if a.type != ACTOR_TYPE_MISSION_WARP or len(a.data) < 0xD:
                        continue
                    res = sim.warp_stream_exit(a.data[0xC])
                    raw = ExitOp(a.data[0xB], a.data[0xA], "warp")
                    if res is None:
                        warp_edges.append(WarpEdge(raw, None, gi))
                    elif res.transitions:
                        for op in res.transitions:
                            warp_edges.append(
                                WarpEdge(
                                    replace(op, kind=op.kind + "+warp"),
                                    res.g_writes,
                                    gi,
                                )
                            )
                    elif not res.ended:
                        warp_edges.append(WarpEdge(raw, res.g_writes, gi))
            has_scripted_ops = bool(scripted)
            warp_targets = {w.op.next_mission for w in warp_edges}
            kept_scripted = []  # (ExitOp, paired op 15 targets)
            for op, si in scripted:
                paired = sim.paired_g_writes(si)
                # A scripted exit none of this hero's warps reach, with no
                # op 15 in its own trigger chain, is another hero's
                # NPC/dialog trigger (e.g. 0x7b's advance for non-Gandalf).
                if warp_edges and op.next_mission not in warp_targets and not paired:
                    continue
                kept_scripted.append((op, paired))
            scripted_targets = {op.next_mission for op, _paired in kept_scripted}

            def emit(op: ExitOp, g_out: set[int]) -> None:
                if op.next_mission == mission:
                    g_out |= sim.g_writes_all
                for g2 in sorted(g_out):
                    edges[(mission, g_entry)].append(
                        ChainEdge(
                            (op.next_mission, g2), op.spawn_point, op.kind, node_certain
                        )
                    )
                    queue.append(
                        ChainNode(op.next_mission, g2, op.spawn_point, node_certain)
                    )

            for op, paired in kept_scripted:
                emit(op, paired or sim.g_trigger or sim.g_base)
            for w in warp_edges:
                if w.pending is not None:
                    # linked warp: the trigger stream's own op 15s are precise
                    g_out = w.pending or sim.g_base
                elif w.group in sim.edge_spawned and (
                    not has_scripted_ops or w.op.next_mission in scripted_targets
                ):
                    # an unlinked warp unlocked by a trigger: the trigger's op
                    # 15 bookkeeping ran first (e.g. 0x3e's dialog spawn)
                    g_out = sim.g_pending(True)
                else:
                    # walkable from load: only agreed-upon trigger writes
                    # (2+ streams) count as normalization
                    g_out = sim.g_trigger_agreed or sim.g_base
                emit(w.op, g_out)
        result[path] = order
    return result, edges_by_path


_CHAINS_HEADER = """\
Per-path chains, one node per line, in first-visit order. Each row is one
(mission, variant) load: v = the g_MissionVariant byte at load, sp = the
spawn point it enters at, then the outgoing transitions. "?" marks rows
gated by an op-62 runtime checkpoint-var fork upstream, "(revisit)" a
mission seen earlier on the path.
"""


def run_chains() -> None:
    """Follow the per-path (mission, variant) chains from the hero starts."""
    rom = load_rom()
    names = decode_strings(rom)
    chains, edges_by_path = path_chains(rom)
    print(_CHAINS_HEADER)
    for path, order in chains.items():
        edges = edges_by_path[path]
        print(f"== {path}'s path ==")
        seen_missions = set()
        current_region = None
        for node in story_sequence(order, edges):
            region_name = region_name_of(rom, names, node.mission)
            if region_name != current_region:
                current_region = region_name
                print(f"  -- {region_name} --")
            name = names.get(0x88 + node.mission, "?")
            tags = ""
            if not node.certain:
                tags += " ?"
            if node.mission in seen_missions:
                tags += " (revisit)"
            if is_stub_record(rom, node.mission, node.g):
                tags += " (stub record)"
            seen_missions.add(node.mission)
            # merge edges by (target, spawn point), joining kinds
            merged: dict[tuple, tuple[int, list[str], bool]] = {}
            for e in edges.get(node.key, []):
                if e.target == node.key:
                    continue  # trivial self-loop
                key = (e.target, e.spawn_point)
                _, kinds, all_certain = merged.setdefault(
                    key, (e.spawn_point, [], True)
                )
                if e.kind not in kinds:
                    kinds.append(e.kind)
                merged[key] = (e.spawn_point, kinds, all_certain and e.certain)
            parts = []
            for (tgt, sp), (_, kinds, tgt_certain) in merged.items():
                tm, tv = tgt
                tname = names.get(0x88 + tm, "?")
                tmark = "" if tgt_certain else " ?"
                parts.append(
                    f"-> {tm:#04x} {tname} v{tv} sp{sp} ({'+'.join(kinds)}){tmark}"
                )
            head = f"{node.mission:#04x} {name}  v{node.g} sp{node.spawn_point}{tags}"
            tail = "  ".join(parts) if parts else "(no exits)"
            print(f"  {head:<46}  {tail}")
        print()


def story_sequence(order: list[ChainNode], edges: ChainEdges) -> list[ChainNode]:
    """Chain nodes in story order: DFS from the start, earlier-discovered
    targets first, so the main line reads top to bottom and side/parallel
    branches trail. Nodes unreachable that way keep BFS order."""
    first_visit = {node.key: i for i, node in enumerate(order)}
    by_key = {node.key: node for node in order}
    sequence: list[tuple[int, int]] = []
    printed: set[tuple[int, int]] = set()

    def visit(key: tuple[int, int]) -> None:
        if key in printed or key not in by_key:
            return
        printed.add(key)
        sequence.append(key)
        kids = sorted(
            edges.get(key, []),
            key=lambda e: (
                first_visit.get(e.target, 0x10000),
                e.spawn_point,
            ),
        )
        for e in kids:
            visit(e.target)

    visit(order[0].key)
    for node in order:
        visit(node.key)
    return [by_key[key] for key in sequence]


def is_stub_record(rom: bytes, mission: int, variant: int) -> bool:
    """Variant record with no script content and no actors: a defensive
    slot never loadable in practice (e.g. 0x64's odd variants)."""
    sub = MissionBlock(rom, mission).variant(variant)
    return all(len(s.records) <= 1 for s in sub.streams) and not any(sub.groups)


def run_chapters() -> None:
    """Per path: (mission, variant, spawnPoint) chapter lists as C initializers."""
    rom = load_rom()
    names = decode_strings(rom)
    chains, edges_by_path = path_chains(rom)
    for path, order in chains.items():
        rows = []
        seen = set()
        for node in story_sequence(order, edges_by_path[path]):
            if node.key in seen or is_stub_record(rom, node.mission, node.g):
                continue
            seen.add(node.key)
            rows.append(node)
        print(f"/* {path}'s path: {len(rows)} chapters */")
        current_region = None
        for node in rows:
            region_name = region_name_of(rom, names, node.mission)
            if region_name != current_region:
                current_region = region_name
                print(f"    /* {region_name} */")
            name = names.get(0x88 + node.mission, "?")
            mark = "" if node.certain else " UNCERTAIN"
            print(
                f"    {{0x{node.mission:02x}, {node.g}, {node.spawn_point}}}, /* {name}{mark} */"
            )
        print()


@dataclass(frozen=True)
class ChapterRow:
    """One `<mission> <variant> <spawnPoint>  # comment` row of the region file."""

    mission: int
    variant: int
    spawn_point: int
    comment: str


@dataclass
class RegionSpec:
    """One `region <Name>` section and its chapter rows."""

    name: str
    rows: list[ChapterRow] = field(default_factory=list)


@dataclass
class PathSpec:
    """One `path <Name>` section and its regions."""

    name: str
    regions: list[RegionSpec] = field(default_factory=list)


def parse_region_file(text: str) -> list[PathSpec]:
    """Parse the hand-editable region file: `path <Name>` / `region <Name>` /
    `<missionId> <variant> <spawnPoint>  # comment`."""
    paths: list[PathSpec] = []
    for lineno, line in enumerate(text.splitlines(), 1):
        if line.startswith("#"):
            continue
        body, _, comment = line.partition("#")
        body = body.strip()
        if not body:
            continue
        if body.startswith("path "):
            paths.append(PathSpec(body[5:].strip()))
        elif body.startswith("region "):
            if not paths:
                raise ValueError(f"line {lineno}: region before any path")
            paths[-1].regions.append(RegionSpec(body[7:].strip()))
        else:
            parts = body.split()
            if len(parts) != 3 or not paths or not paths[-1].regions:
                raise ValueError(f"line {lineno}: bad row {line!r}")
            mission, variant, spawn_point = (int(p, 0) for p in parts)
            paths[-1].regions[-1].rows.append(
                ChapterRow(mission, variant, spawn_point, comment.strip())
            )
    return paths


def run_from_file(
    path: Annotated[str, typer.Argument(help="hand-edited region file")],
) -> None:
    """Compile a hand-edited region file into C chapter data."""
    rom = load_rom()
    names = decode_strings(rom)
    chains, _ = path_chains(rom)
    chain_nodes = {p: {n.key for n in nodes} for p, nodes in chains.items()}
    region_name_to_id = {
        names.get(TEXT_ID_REGION_NAME_BASE + i, "").lower(): i
        for i in range(REGION_COUNT)
    }

    def c_ident(text: str) -> str:
        return re.sub(r"[^0-9A-Za-z]", "", text)

    def region_id_for(region_name: str, first_mission: int) -> tuple[int, bool]:
        base = region_name.split("(")[0].strip().lower()
        if base in region_name_to_id:
            return region_name_to_id[base], True
        idx = mission_region_id(rom, first_mission)
        return (idx if idx is not None else 0), False

    n_warnings = 0
    with open(path) as f:
        paths = parse_region_file(f.read())
    for spec in paths:
        if spec.name not in chains:
            print(f"!! unknown path {spec.name!r} (want one of {sorted(chains)})")
            n_warnings += 1
            continue
        if len(spec.regions) > 8:
            print(f"!! {spec.name}: {len(spec.regions)} regions > 8")
            n_warnings += 1
        print(f"/* {spec.name}'s path: {len(spec.regions)} regions */")
        region_rows = []
        for region in spec.regions:
            ident = c_ident(spec.name) + c_ident(region.name)
            print(
                f"static const struct ChapterSelectMission "
                f"ChapterSelectMissions{ident}[] = {{"
            )
            for row in region.rows:
                if (row.mission, row.variant) not in chain_nodes[spec.name]:
                    print(
                        f"!! {spec.name}/{region.name}: ({row.mission:#04x}, "
                        f"v{row.variant}) not on the extracted chain"
                    )
                    n_warnings += 1
                name = names.get(0x88 + row.mission, "?")
                if row.comment.lower().startswith(name.lower()):
                    name = row.comment
                elif row.comment:
                    name += f" - {row.comment}"
                print(
                    f"    {{0x{row.mission:02x}, {row.variant}, {row.spawn_point}}},"
                    f" /* {name} */"
                )
            print("};")
            region_id, exact = region_id_for(region.name, region.rows[0].mission)
            note = "" if exact else f" /* {region.name} */"
            region_rows.append(
                f"    {{ChapterSelectMissions{ident}, {len(region.rows)},"
                f" TEXT_ID_REGION_NAME_BASE + {region_id}}},{note}"
            )
        print(
            f"static const struct ChapterSelectRegion "
            f"ChapterSelectRegions{c_ident(spec.name)}[] = {{"
        )
        print("\n".join(region_rows))
        print("};\n")
    print(f"{n_warnings} warnings")


_ACTOR_TYPE_NAMES = {
    ACTOR_TYPE_MISSION_WARP: "mission warp",
    ACTOR_TYPE_CHEST: "chest",
}


def _dump_variant(ss: MissionVariant) -> None:
    """Raw dump of one variant: streams (record offset, opcode, arg bytes)
    and actor-spawn groups (type + raw record bytes)."""
    shared = " (stream/group 0 from the shared chunk)" if ss.mission_flags & 1 else ""
    print(
        f"variant {ss.index}  chunk record {ss.chunk_record}"
        f"  flags {ss.mission_flags:#06x}{shared}"
    )
    for s in ss.streams:
        print(f"  stream {s.index}")
        off = 0
        for r in s.records:
            print(
                f"    +{off:#06x} {r.opcode:2d}"
                f" {OPCODE_NAMES.get(r.opcode, '?'):<22} {r.data[4:].hex(' ')}"
            )
            off += len(r.data)
    for gi, group in enumerate(ss.groups):
        print(f"  group {gi}")
        for ai, a in enumerate(group):
            note = _ACTOR_TYPE_NAMES.get(a.type, "")
            print(f"    {ai}: type {a.type:<2d} {note:<11} {a.data.hex(' ')}")
    print()


def run_dump(
    mission: Annotated[str, typer.Argument(help="mission id (0x.. or decimal)")],
    variant: Annotated[
        int, typer.Option("--variant", "-v", help="dump only this variant index")
    ] = -1,
) -> None:
    """Dump the raw script (streams, records, actor groups) of one mission block."""
    rom = load_rom()
    try:
        m = int(mission, 0)
    except ValueError:
        raise typer.BadParameter(f"bad mission id {mission!r}") from None
    if not 0 <= m < MISSION_COUNT:
        raise typer.BadParameter(
            f"mission id {m:#x} out of range 0..{MISSION_COUNT - 1:#x}"
        )
    mb = MissionBlock(rom, m)
    indices = mb.variant_indices()
    if variant >= 0 and variant not in indices:
        raise typer.BadParameter(
            f"mission {m:#x} has no variant {variant} (has {indices})"
        )

    names = decode_strings(rom)
    print(f'mission {m:#04x} "{names.get(0x88 + m, "?")}"  block @{mb.addr:#010x}')
    print(
        f"header size {mb.header_size:#x}, {mb.chunk_record_count} chunk records,"
        f" variant map {list(mb.variant_map)}"
    )
    # Spawn records live in the block header: 8-byte
    # {s16 x, s16 y, u8 spawnPointId, u8 playerIndex, ...}.
    off = mb.addr - ROMBASE
    n_spawns = unpack_from("<H", rom, off + 2)[0]
    if n_spawns:
        print(f"spawn records ({n_spawns}):")
        for i in range(n_spawns):
            x, y = unpack_from("<hh", rom, off + 4 + i * 8)
            spawn_point, player = rom[off + 8 + i * 8], rom[off + 9 + i * 8]
            rest = rom[off + 10 + i * 8 : off + 12 + i * 8]
            print(
                f"  {i}: x={x} y={y} spawn point {spawn_point}"
                f" player {player}  {rest.hex(' ')}"
            )
    print()
    for si in indices:
        if variant >= 0 and si != variant:
            continue
        _dump_variant(mb.variant(si))
