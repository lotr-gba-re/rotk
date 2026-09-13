"""Mission block and mission-script parsing.

Layout reverse-engineered from the mission loader (mission_script_loadMissionBlock) and
the script VM (mission_script_runStream, stream copier mission_script_copyStream, section
parser mission_script_loadStreams):

- MissionTable @0x0806c0f0, 130 entries of 0xc bytes; mission block ptr @+4.
- Mission block: u16 @+0 = header size, u16 @+2 = spawn-record count, then
  8-byte spawn records ({s16 x, s16 y, u8 spawnPointId, u8 playerIndex, ...}).
  At block+headerSize: the variant table: u8 @+0 = chunk-record count,
  u8[11] @+1 = variant index -> chunk-record index (0xff = unused), then
  8-byte chunk records @+0xc; u16 @record+0 = chunk offset from block start.
  Record 0 is the always-loaded chunk; the variant byte (g_MissionVariant)
  selects the second chunk through the index table.
- Variant chunk (decompressed): u16 @+0 = waypoint-path section (u16 path count,
  u16 offsets, then per path a u32 header + 8-byte {s16 x, s16 y, ...} records;
  merged into the state area at g_MissionWaypointPaths), u16 @+2 =
  script-section offset,
  u16 @+4 = mission flags (g_MissionFlags), u8 @scriptSection+0 = stream count,
  u16 offsets @scriptSection+2 per stream.
- Stream = record sequence: u32 opcode, fixed length per opcode from the ROM
  table @0x08058210 (index = opcode), terminated by opcode 0 (4 bytes).
- Script op dispatch table @0x0805083c (69 handlers, opcode-1 indexed).

The VM is strictly sequential per stream; handlers may rewrite their own
record. Cross-stream flow: op 5/52/62/68 call
mission_script_spawnGroupAndStartStream(stop, start).
"""

from dataclasses import dataclass
from struct import unpack_from

from rotkit.compression.chunk import decompress_chunk
from rotkit.rom import ROMBASE

MISSION_TABLE_ADDR = 0x0806C0F0
MISSION_TABLE_STRIDE = 0xC
MISSION_TABLE_BLOCK_OFF = 4
MISSION_COUNT = 130

OPCODE_LENGTH_TABLE_ADDR = 0x08058210
OPCODE_COUNT = 69

# Opcodes with chain/flow meaning for extraction (1-based record opcode values).
OP_END = 0
OP_RUN_STREAMS = (
    5  # if !(flags & 0x20): scene_requestInGameDialog(u16@4) else C4ac(b6, b7)
)
OP_SET_VARIANT = 15  # if b5 == 0 or !(flags & 0x20): g_MissionVariant = b4
OP_ADVANCE_MISSION = (
    19  # spawnPoint=b5; game_requestSceneChangeWithArgs(GAMEPLAY, 0, b4)
)
OP_GOTO_SCENE_0x15 = 44  # spawnPoint=b5; game_requestSceneChangeWithArgs(0x15, b6, b4)
OP_START_CUTSCENE = 28  # spawnPoint=b5; cutscene b6, then mission b4 (scene 7)
OP_RETURN_SCENE_5 = 47  # game_requestSceneChange(5) (main menu); sets bit31
OP_END_TO_SCENE_9 = 46  # game_requestSceneChangeWithArgs(9, 3, 0); sets flag bits
OP_COND_STREAMS_VAR = 52  # compare var[b4] vs b6 (mode b5) -> C4ac(b7,b8)/(b9,ba)
OP_ADVANCE_IF_FLAG20 = 56  # if flags & 0x20: like OP_ADVANCE_MISSION
OP_COND_STREAMS_VAR2 = 62  # compare var[b4] vs b6 (mode b5) -> stream switch
OP_COND_STREAMS_FLAG20 = 68  # if flags & 0x20: C4ac(b4,b5) else C4ac(b6,b7)

OPCODE_NAMES = {
    OP_END: "end",
    1: "actor_set_state1",
    2: "actor_set_animation",
    3: "actor_flags_or",
    4: "actor_flags_bic",
    OP_RUN_STREAMS: "run_streams_f20",
    6: "window_show?",
    7: "window_hide?",
    8: "sfx?",
    9: "play_song",
    10: "audio_cmd_10",
    11: "audio_cmd_11",
    12: "hud_counter",
    13: "actor_counter_dec",
    14: "actor_counter_inc",
    OP_SET_VARIANT: "set_variant",
    16: "actor_spawn_or_talk",
    17: "actor_teleport",
    18: "camera_or_spawn_multi",
    OP_ADVANCE_MISSION: "advance_mission",
    20: "actor_lookup_nop",
    21: "effect_multi",
    22: "hud_clear+obj",
    23: "actor_move_abs",
    24: "obj_param_set",
    25: "set_loot_tier",
    26: "obj_animation_once",
    27: "party_stat_add",
    OP_START_CUTSCENE: "start_cutscene",
    29: "call_08010538",
    30: "actor_ai_30",
    31: "actor_ai_31",
    32: "actor_ai_32",
    33: "actor_ai_33",
    34: "actor_ai_34",
    35: "actor_ai_35",
    36: "actor_ai_36",
    37: "actor_kill",
    38: "actor_ai_38",
    39: "actor_ai_39",
    40: "actor_ai_40",
    41: "actor_ai_41",
    42: "actor_ai_42",
    43: "actor_give_item?",
    OP_GOTO_SCENE_0x15: "goto_scene_0x15",
    45: "dialog?",
    OP_END_TO_SCENE_9: "end_to_scene_9",
    OP_RETURN_SCENE_5: "return_scene_5",
    48: "group_flags_or",
    49: "group_flags_bic",
    50: "group_kill",
    51: "group_set_state1",
    OP_COND_STREAMS_VAR: "cond_streams_var",
    53: "set_boss_target?",
    54: "clear_boss_target?",
    55: "actor_ai_55",
    OP_ADVANCE_IF_FLAG20: "advance_if_flag20",
    57: "var_call_08018b18",
    58: "var_set",
    59: "var_add",
    60: "var_sub",
    61: "var_rand",
    OP_COND_STREAMS_VAR2: "cond_streams_var2",
    63: "var2_set",
    64: "var2_add",
    65: "var2_sub",
    66: "var2_rand",
    67: "sfx_67",
    OP_COND_STREAMS_FLAG20: "cond_streams_flag20",
    69: "summon?",
}


@dataclass(frozen=True)
class Record:
    opcode: int
    data: bytes  # the full record (length from the ROM table)

    @property
    def b4(self) -> int:
        return self.data[4]

    @property
    def b5(self) -> int:
        return self.data[5]


@dataclass(frozen=True)
class ScriptStream:
    index: int
    records: tuple[Record, ...]


@dataclass(frozen=True)
class GroupActor:
    """One actor record of a stream's spawn group (on-disk layout)."""

    type: int  # u16 @record+0, an enum ActorType (ActorMissionConstructors index)
    data: bytes  # full on-disk record


# The two enum ActorType values (include/mission.h) the analysis needs.
# Mission warp (actor_missionWarp_trigger): mission @+0xb, spawnPoint @+0xa. On trigger,
# after queueing the raw transition, the stream @+0xc runs synchronously: its transition
# ops supersede the raw target and its op 15s set the variant the next mission loads
# with. Records with an unlinked stream and target 0x00/0x01 carry the unset placeholder.
ACTOR_TYPE_MISSION_WARP = 3
# Chest: its record bytes past +8 are chest params, not a warp target.
ACTOR_TYPE_CHEST = 12


@dataclass(frozen=True)
class MissionVariant:
    index: int  # variant byte value (g_MissionVariant)
    chunk_record: int  # chunk-record index it maps to
    streams: tuple[ScriptStream, ...]
    groups: tuple[tuple[GroupActor, ...], ...]  # parallel to streams
    mission_flags: int  # u16 @chunk+4 (g_MissionFlags)


def _u16(data: bytes, off: int) -> int:
    return unpack_from("<H", data, off)[0]


class MissionBlock:
    """One parsed mission block (lazy chunk decompression)."""

    def __init__(self, rom: bytes, mission_id: int) -> None:
        self.rom = rom
        self.mission_id = mission_id
        table_off = MISSION_TABLE_ADDR - ROMBASE + mission_id * MISSION_TABLE_STRIDE
        self.addr = unpack_from("<I", rom, table_off + MISSION_TABLE_BLOCK_OFF)[0]
        off = self.addr - ROMBASE
        self.header_size = _u16(rom, off)
        st = off + self.header_size
        self.chunk_record_count = rom[st]
        self.variant_map = rom[st + 1 : st + 12]
        self.chunk_offsets = [
            _u16(rom, st + 0xC + i * 8) for i in range(self.chunk_record_count)
        ]

    def chunk(self, record_index: int) -> bytes:
        """Decompress chunk-record `record_index` (0 = always-loaded chunk)."""
        return decompress_chunk(
            self.rom, self.addr - ROMBASE + self.chunk_offsets[record_index]
        )

    def variant_indices(self) -> list[int]:
        return [i for i, r in enumerate(self.variant_map) if r != 0xFF]

    def variant(self, index: int) -> MissionVariant:
        record_index = self.variant_map[index]
        chunk = self.chunk(record_index)
        streams = list(parse_script_section(chunk))
        groups = list(parse_group_section(chunk))
        flags = _u16(chunk, 4)
        if flags & 1:
            # g_MissionFlags bit 0: the VM's stream/group 0 is the shared chunk's
            # (mission_script_loadStreams / mission_script_loadGroups); the variant
            # chunk's shift up by one.
            chunk0 = self.chunk(0)
            chunk0_streams = parse_script_section(chunk0)
            chunk0_groups = parse_group_section(chunk0)
            streams = [chunk0_streams[0]] + [
                ScriptStream(s.index + 1, s.records) for s in streams
            ]
            groups = [chunk0_groups[0]] + groups
        return MissionVariant(
            index=index,
            chunk_record=record_index,
            streams=tuple(streams),
            groups=tuple(groups),
            mission_flags=flags,
        )


def parse_group_section(chunk: bytes) -> tuple[tuple[GroupActor, ...], ...]:
    """Parse the actor-spawn groups of a decompressed chunk (mission_script_copyGroup).

    Section at chunk+8: u8 group count, u16 offsets @+2. Group: u8 actor count,
    u8 sizes[count] @+2, records at group + align4(count + 2).
    """
    count = chunk[8]
    groups = []
    for gi in range(count):
        group = 8 + _u16(chunk, 8 + 2 + gi * 2)
        n = chunk[group]
        sizes = chunk[group + 2 : group + 2 + n]
        pos = group + ((n + 2 + 3) & ~3)
        actors = []
        for size in sizes:
            record = chunk[pos : pos + size]
            actors.append(GroupActor(type=_u16(record, 0), data=record))
            pos += size
        groups.append(tuple(actors))
    return tuple(groups)


def parse_script_section(chunk: bytes) -> tuple[ScriptStream, ...]:
    """Parse the script section of a decompressed variant chunk."""
    section = _u16(chunk, 2)
    count = chunk[section]
    streams = []
    for i in range(count):
        pos = section + _u16(chunk, section + 2 + i * 2)
        records = []
        while True:
            opcode = unpack_from("<I", chunk, pos)[0]
            if opcode > OPCODE_COUNT:
                raise ValueError(f"bad opcode {opcode} at chunk+{pos:#x} (stream {i})")
            length = opcode_lengths()[opcode]
            records.append(Record(opcode, chunk[pos : pos + length]))
            pos += length
            if opcode == OP_END:
                break
        streams.append(ScriptStream(i, tuple(records)))
    return tuple(streams)


_OPCODE_LENGTHS: bytes | None = None


def opcode_lengths() -> bytes:
    """Per-opcode record lengths, carved from the ROM table @0x08058210."""
    global _OPCODE_LENGTHS
    if _OPCODE_LENGTHS is None:
        from rotkit.rom import load_rom

        rom = load_rom()
        base = OPCODE_LENGTH_TABLE_ADDR - ROMBASE
        _OPCODE_LENGTHS = rom[base : base + OPCODE_COUNT + 1]
    return _OPCODE_LENGTHS
