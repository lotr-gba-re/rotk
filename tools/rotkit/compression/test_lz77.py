import pytest

from rotkit.compression import lz77


def _reference_backref(raw: bytes, pos: int) -> tuple[int, int]:
    best_length, best_distance = 0, 0
    for distance in range(min(pos, lz77.MAX_DISTANCE), 0, -1):
        limit = min(lz77.MAX_BACKREF, len(raw) - pos, distance)
        length = 0
        while length < limit and raw[pos + length] == raw[pos - distance + length]:
            length += 1
        if length > best_length:
            best_length, best_distance = length, distance
    return best_length, best_distance


def _reference_encode(raw: bytes) -> bytes:
    out = bytearray()
    literals = bytearray()
    pos = 0
    while pos < len(raw):
        run = lz77._run_length(raw, pos)
        length, distance = _reference_backref(raw, pos)
        if run >= lz77.MIN_RUN and run >= length:
            lz77._put_literals(out, literals)
            lz77._put_run(out, raw[pos], run)
            pos += run
        elif length >= lz77.MIN_BACKREF and not (
            pos + 1 < len(raw) and lz77._run_length(raw, pos + 1) > length
        ):
            lz77._put_literals(out, literals)
            lz77._put_distance(out, distance, length - 2)
            pos += length
        else:
            literals.append(raw[pos])
            pos += 1
    lz77._put_literals(out, literals)
    out.append(lz77.CMD_END)
    return bytes(out)


@pytest.mark.parametrize(
    ("raw", "expected"),
    [
        (b"", b"\x00"),
        (b"AB", b"\x02AB\x00"),
        (b"ABCDEFABCDEF", bytes.fromhex("0641424344454680c400")),
        (b"BAAxBAAAA", bytes.fromhex("054241417842414100")),
        (bytes(range(64)), b"\x3f" + bytes(range(63)) + b"\x01\x3f\x00"),
        (b"A" * 33, bytes.fromhex("5e4100")),
        (b"A" * 66, bytes.fromhex("7f4100")),
        (b"A" * 67, bytes.fromhex("80204100")),
        (b"A" * 1089, bytes.fromhex("ffe04100")),
    ],
)
def test_encode_known_stream(raw: bytes, expected: bytes) -> None:
    assert lz77.encode(raw) == expected
    assert lz77.decode(expected) == (raw, len(expected))


def test_backref_ties_and_overlap() -> None:
    assert lz77._longest_backref(b"ABCxABCyABC", 8) == (3, 8)
    assert lz77._longest_backref(b"ABCDEqABCDzABCDE", 11) == (5, 11)
    assert lz77._longest_backref(b"ABCDxABCDEyABCDE", 11) == (5, 6)
    assert lz77._longest_backref(b"ABCDxABCDEyABCDEzABCDE", 17) == (5, 12)
    assert lz77._longest_backref(b"abcabcabc", 3) == (3, 3)
    assert lz77._longest_backref(b"ABABAB", 2) == (0, 0)
    assert lz77._longest_backref(b"ABC" + b"X" * 1024 + b"ABC", 1027) == (0, 0)


def test_encode_matches_reference() -> None:
    cases = [b"A" * n for n in (2, 3, 32, 34, 63, 64, 65, 1023, 1090, 2178)]
    cases += [
        bytes(range(256)) * 5,
        b"ABCD" * 256,
        b"ABCxABCyABC",
        b"ABCDxABCDEyABCDEzABCDE" * 40,
    ]
    for raw in cases:
        assert lz77.encode(raw) == _reference_encode(raw)
