"""The game's own LZ77 + RLE codec for sprite frame tiles (decompress_lz77 @0x0800d1d4).

A stream is a sequence of commands, each a command byte plus operands, closed by a zero
byte. The top bits of the command byte pick the kind; `d` is one 10-bit distance split
over two bytes, high part first:

  00 n:6                literal    the next n bytes (1..63) verbatim
  01 n:6     byte       short run  byte repeated n+3 times (3..66)
  1 d:7  d:3 n:5        backref    n+2 bytes (3..33) copied from d bytes back (1..1023)
  1 d:7  d:3 0:5  byte  long run   byte repeated d+66 times (67..1089)

The game unpacks into VRAM in halfwords, so a backref stays clear of its own output:
length <= distance.
"""

CMD_END = 0
CMD_RUN = 0x40  # short run; below it the command byte is a literal count
CMD_BACKREF = 0x80  # backref, or a long run when the length field is 0

MAX_LITERAL = 0x3F
MIN_RUN = 3
MAX_SHORT_RUN = 0x3F + MIN_RUN
LONG_RUN_BIAS = MAX_SHORT_RUN  # long run count = distance field + this
MAX_DISTANCE = 0x3FF
MAX_LONG_RUN = MAX_DISTANCE + LONG_RUN_BIAS
MIN_BACKREF = 3
MAX_BACKREF = 0x1F + 2


def _copy_back(out: bytearray, distance: int, length: int) -> None:
    for _ in range(length):
        out.append(out[-distance])


def decode(data: bytes, start: int = 0) -> tuple[bytes, int]:
    """Unpack the stream at `start`; returns the bytes and the offset past the end byte."""
    out = bytearray()
    pos = start
    while True:
        cmd = data[pos]
        pos += 1
        if cmd == CMD_END:
            return bytes(out), pos
        if cmd & CMD_BACKREF:
            second = data[pos]
            pos += 1
            distance = ((cmd & 0x7F) << 3) | (second >> 5)
            length = second & 0x1F
            if length:
                _copy_back(out, distance, length + 2)
            else:
                out += bytes([data[pos]]) * (distance + LONG_RUN_BIAS)
                pos += 1
        elif cmd & CMD_RUN:
            out += bytes([data[pos]]) * ((cmd & 0x3F) + MIN_RUN)
            pos += 1
        else:
            out += data[pos : pos + cmd]
            pos += cmd


def _run_length(raw: bytes, pos: int) -> int:
    """How often the byte at `pos` repeats from there on, capped at one long run."""
    end = min(len(raw), pos + MAX_LONG_RUN)
    n = pos + 1
    while n < end and raw[n] == raw[pos]:
        n += 1
    return n - pos


def _longest_backref(raw: bytes, pos: int) -> tuple[int, int]:
    """Length and distance of the longest earlier copy of the bytes at `pos`, the farthest
    among equal lengths. The copy never overlaps its source (length <= distance)."""
    best_length, best_distance = 0, 0
    for distance in range(min(pos, MAX_DISTANCE), 0, -1):
        limit = min(MAX_BACKREF, len(raw) - pos, distance)
        length = 0
        while length < limit and raw[pos + length] == raw[pos - distance + length]:
            length += 1
        if length > best_length:
            best_length, best_distance = length, distance
    return best_length, best_distance


def _put_distance(out: bytearray, distance: int, length_field: int) -> None:
    out += bytes([CMD_BACKREF | (distance >> 3), ((distance & 7) << 5) | length_field])


def _put_run(out: bytearray, byte: int, count: int) -> None:
    if count <= MAX_SHORT_RUN:
        out.append(CMD_RUN | (count - MIN_RUN))
    else:
        _put_distance(out, count - LONG_RUN_BIAS, 0)
    out.append(byte)


def _put_literals(out: bytearray, literals: bytearray) -> None:
    for i in range(0, len(literals), MAX_LITERAL):
        group = literals[i : i + MAX_LITERAL]
        out.append(len(group))
        out += group
    literals.clear()


def encode(raw: bytes) -> bytes:
    """Pack `raw` the way the game's original packer did.

    Reproduces every lz77 sprite frame in the ROM byte for byte (item icons and the typed
    animation tables), which the carved graphics need to rebuild it. Greedy left to right;
    at each position, with the run starting there and the longest backref:
      - a run of 3 or more wins when it is at least as long as the backref
      - else a backref of 3 or more wins, unless the run starting one byte later is
        longer than the backref: then this byte goes out as a literal
      - else the byte is a literal; literals flush in groups of up to 63
    """
    out = bytearray()
    literals = bytearray()
    n = len(raw)
    pos = 0
    while pos < n:
        run = _run_length(raw, pos)
        length, distance = _longest_backref(raw, pos)
        if run >= MIN_RUN and run >= length:
            _put_literals(out, literals)
            _put_run(out, raw[pos], run)
            pos += run
        elif length >= MIN_BACKREF and not (
            pos + 1 < n and _run_length(raw, pos + 1) > length
        ):
            _put_literals(out, literals)
            _put_distance(out, distance, length - 2)
            pos += length
        else:
            literals.append(raw[pos])
            pos += 1
    _put_literals(out, literals)
    out.append(CMD_END)
    stream = bytes(out)
    if decode(stream) != (raw, len(stream)):
        raise AssertionError("lz77.encode: stream does not decode to its input")
    return stream
