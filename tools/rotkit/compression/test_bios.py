import pytest

from rotkit.compression import bios


def assert_encoding(data: bytes, expected: bytes) -> None:
    encoded = bios.rl_encode(data)
    assert encoded == expected
    assert bios.rl_decode(encoded, 0, len(data)) == data


def test_empty() -> None:
    assert_encoding(b"", b"")


@pytest.mark.parametrize(
    ("data", "expected"),
    [
        (b"A", b"\x00A"),
        (b"AA", b"\x01AA"),
        (b"AB", b"\x01AB"),
        (b"AAA", b"\x80A"),
        (b"ABCCCDE", b"\x01AB\x80C\x01DE"),
    ],
)
def test_short_blocks(data: bytes, expected: bytes) -> None:
    assert_encoding(data, expected)


@pytest.mark.parametrize("length", (127, 128, 129, 255, 256, 257))
def test_literal_boundaries(length: int) -> None:
    data = bytes(index % 256 for index in range(length))
    expected = b"".join(
        bytes([len(data[start : start + 128]) - 1]) + data[start : start + 128]
        for start in range(0, length, 128)
    )
    assert_encoding(data, expected)


@pytest.mark.parametrize("length", (129, 130, 131, 132, 133, 260, 261))
def test_run_boundaries(length: int) -> None:
    full_runs, remainder = divmod(length, 130)
    expected = b"\xffA" * full_runs
    if remainder >= 3:
        expected += bytes([0x80 | (remainder - 3)]) + b"A"
    elif remainder:
        expected += bytes([remainder - 1]) + b"A" * remainder
    assert_encoding(b"A" * length, expected)


def test_pair_crossing_literal_boundary() -> None:
    prefix = bytes(range(127))
    assert_encoding(prefix + b"\xfe\xfe", b"\x7f" + prefix + b"\xfe\x00\xfe")


def test_run_after_127_literals_with_trailing_byte() -> None:
    prefix = bytes(range(127))
    assert_encoding(prefix + b"\xfe" * 3 + b"X", b"\x7e" + prefix + b"\x80\xfe\x00X")


def test_full_run_then_full_literal_then_run() -> None:
    literal = bytes(range(128))
    assert_encoding(
        b"A" * 130 + literal + b"B" * 3,
        b"\xffA\x7f" + literal + b"\x80B",
    )


def test_decode_at_nonzero_offset() -> None:
    assert bios.rl_decode(b"prefix\x01AB\x80C\x01DE", 6, 7) == b"ABCCCDE"
