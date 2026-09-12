"""pucrunch, the codec of the BG asset tile streams (decompress_pucrunch @0x080002dc).

pucrunch is Pasi "Albert" Ojala's LZ77 + RLE packer (1997-2008, LGPL):
https://github.com/mist64/pucrunch and https://a1bert.kapsi.fi/Dev/pucrunch/. The game
carries the GBA port of its unpacker, whose stream differs from the C64 one: a small
header, a 32-entry RLE byte table, plain distance bits, and MSB-first bits packed into
little-endian u32 words. `_Packer` is the reference packer with those changes.

Stream layout:
  u8   RLE table size in bytes, a multiple of 4
  u8   initial escape, right aligned
  u8   extra distance bits, 0..4
  u8   escape bits E, 0..8
  u8[] RLE table: the most common run bytes, rank 1 first
  bits, zero padded to a whole word

Tokens, with `esc` the E-bit escape, `g(v)` the Elias gamma code of v (1..255: the bit
length of v minus one as one bits, a zero unless there were 7, then the bits of v below
its top one) and `n:` an n-bit field:
  literal          8:                       any byte whose top E bits differ from esc
  copy             esc g(len-1) g(hi+1) extra: 8:
                   len 3..256 bytes from dist back; dist-1 = hi:extra bits:low byte
  short copy       esc g(1) 0 8:            2 bytes from (byte + 1) back
  escaped literal  esc g(1) 1 0 E: (8-E):   esc's byte with those low bits; the E bits
                   are the escape from here on
  run              esc g(1) 1 1 count byte  count: g(count-1) for 2..128, else
                   g(128 | (count-1 & 0xFF) >> 1) 1: g((count-1 >> 8) + 1)
                   byte: g(rank) for a table byte, else g(32 | byte >> 3) 3:
  end              esc g(2) g(255)
"""

from struct import unpack_from

MAX_GAMMA = 7  # gamma codes carry 1..255
MAX_LZ_LEN = 2 << MAX_GAMMA
LZ_RANGE = ((2 << MAX_GAMMA) - 3) * 256  # farthest match the packer looks for
MAX_RLE_LEN = ((2 << MAX_GAMMA) - 2) * 256  # longest run one token covers
RLE_TABLE_RANKS = 31  # the table holds ranks 1..31


def _gamma_len(value: int) -> int:
    k = value.bit_length() - 1
    return 2 * k + 1 if k < MAX_GAMMA else 2 * k


GAMMA_LEN = [0] + [_gamma_len(v) for v in range(1, 256)]  # bits of g(v)


class _BitReader:
    """MSB-first bits from little-endian u32 words."""

    def __init__(self, data: bytes, pos: int) -> None:
        self.data = data
        self.pos = pos
        self.word = 0
        self.left = 0  # unread bits of `word`

    def bits(self, n: int) -> int:
        value = 0
        while n:
            if not self.left:
                self.word = unpack_from("<I", self.data, self.pos)[0]
                self.pos += 4
                self.left = 32
            take = min(n, self.left)
            self.left -= take
            value = (value << take) | ((self.word >> self.left) & ((1 << take) - 1))
            n -= take
        return value

    def bit(self) -> int:
        return self.bits(1)

    def gamma(self) -> int:
        k = 0
        while k < MAX_GAMMA and self.bit():
            k += 1
        return (1 << k) | self.bits(k)


class _BitWriter:
    """MSB-first bits into little-endian u32 words."""

    def __init__(self) -> None:
        self.words: list[int] = []
        self.acc = 0
        self.count = 0  # bits pending in `acc`

    def bits(self, value: int, n: int) -> None:
        self.acc = (self.acc << n) | value
        self.count += n
        while self.count >= 32:
            self.count -= 32
            self.words.append((self.acc >> self.count) & 0xFFFFFFFF)
            self.acc &= (1 << self.count) - 1

    def gamma(self, value: int) -> None:
        k = value.bit_length() - 1
        self.bits((1 << k) - 1, k)
        if k < MAX_GAMMA:
            self.bits(0, 1)
        self.bits(value & ((1 << k) - 1), k)

    def finish(self) -> bytes:
        if self.count:
            self.bits(0, 32 - self.count)
        return b"".join(w.to_bytes(4, "little") for w in self.words)


def _copy_back(out: bytearray, dist: int, length: int) -> None:
    for _ in range(length):
        out.append(out[-dist])


def decode(stream: bytes, start: int = 0) -> bytes:
    """Unpack the pucrunch stream at `start`; it ends with its own end marker."""
    table_size, escape, extra_dist_bits, esc_bits = stream[start : start + 4]
    table = stream[start + 4 : start + 4 + table_size]
    bits = _BitReader(stream, start + 4 + table_size)
    low_bits = 8 - esc_bits
    out = bytearray()

    while True:
        top = bits.bits(esc_bits)
        if top != escape:  # literal
            out.append((top << low_bits) | bits.bits(low_bits))
            continue
        length = bits.gamma() + 1
        if length > 2:  # copy, or the end
            hi = bits.gamma() - 1
            if hi == 254:
                return bytes(out)
            hi = (hi << extra_dist_bits) | bits.bits(extra_dist_bits)
            _copy_back(out, ((hi << 8) | bits.bits(8)) + 1, length)
        elif not bits.bit():  # short copy
            _copy_back(out, bits.bits(8) + 1, 2)
        elif not bits.bit():  # escaped literal
            new_escape = bits.bits(esc_bits)
            out.append((escape << low_bits) | bits.bits(low_bits))
            escape = new_escape
        else:  # run
            count = bits.gamma()
            if count >= 128:
                count = ((count << 1) | bits.bit()) & 0xFF
                count |= (bits.gamma() - 1) << 8
            code = bits.gamma()
            byte = table[code - 1] if code < 32 else ((code << 3) | bits.bits(3)) & 0xFF
            out += bytes([byte]) * (count + 1)


def _common_prefix(data: bytes, a: int, b: int, limit: int) -> int:
    """How many bytes from `a` and from `b` agree, at most `limit`."""
    n = 0
    while n < limit and data[a + n] == data[b + n]:
        n += 1
    return n


def _powers_of_two(start: int, below: int):
    k = start
    while k < below:
        yield k
        k <<= 1


LITERAL, LZ77, RLE = range(3)


class _Packer:
    """The reference packer (pucrunch.c: PackLz77) with the GBA port's stream layout.

    Phases: find the byte runs and the best match at every position, pick the escape bit
    count, choose the cheapest token chain back to front, pick the extra distance bits,
    assign the escape values, re-rank the RLE table by the runs actually used, move each
    match to its nearest source, write the tokens.

    The cost model prices a literal at a flat 8 bits and leaves the escapes to their own
    pass, so the chain is optimal only under that model. `compatible` leaves out two
    refinements the game's build predates, and so reproduces its streams byte for byte:
    reconsidering the longest match at a position once a nearer shorter one beat it on
    its own cost, and rescanning a match that does not outrun the byte run at its
    position.

    Per-position arrays, with the reference's names:
      run_len[p]     rle      bytes of the run at p still ahead, 0 outside runs
      run_offset[p]  elr      how far p is into its run
      lz_len[p]      lzlen    the match chosen at p, 0 if none
      lz_dist[p]     lzpos    its distance back
      max_len[p]     lzmlen   the longest match at p, whatever it costs
      max_dist[p]    lzmpos   its distance back
      prev_pair[p]   backSkip distance back to the previous occurrence of the byte
                              pair at p, 0 if none; the list the match search walks
      cost[p]        length   bits to encode p to the end
      mode[p]                 the token starting at p
      new_escape[p]  newesc   the escape to switch to when the literal at p is escaped
    """

    def __init__(self, data: bytes, compatible: bool) -> None:
        self.data = data
        self.n = n = len(data)
        self.compatible = compatible
        self.esc_bits = 0
        self.esc_mask = 0
        self.extra_dist_bits = 0
        self.rle_table = [0] * (RLE_TABLE_RANKS + 1)  # by rank; rank 0 is unused
        self.rle_rank: dict[int, int] = {}
        self.rle_used = 0
        self.rle_byte_len = [0] * 256  # bits of the run token's byte code, per byte
        self.saved = 0  # bits the shortening in optimize_lengths saved
        self.run_len = [0] * n
        self.run_offset = [0] * n
        self.lz_len = [0] * n
        self.lz_dist = [0] * n
        self.max_len = [0] * n
        self.max_dist = [0] * n
        self.prev_pair = [0] * n
        self.cost = [0] * (n + 1)
        self.mode = [LITERAL] * n
        self.new_escape = [0] * n
        self.run_hist = [0] * 256  # runs found, per byte

    def pack(self) -> bytes:
        self.find_matches()
        self.rank_rle_table(self.run_hist)
        self.choose_escape_bits()
        self.optimize_lengths(optimize=True)
        self.choose_extra_dist_bits()
        escape, _, _ = self.optimize_escapes()
        self.rank_rle_table(self.rle_token_hist())
        if self.saved:
            self.rescan()
        return self.emit(escape)

    # --- costs ---------------------------------------------------------------

    def _set_escape_bits(self, bits: int) -> None:
        self.esc_bits = bits
        self.esc_mask = (0xFF00 >> bits) & 0xFF

    def _rle_cost(self, count: int, byte: int) -> int:
        """Bits for a run, in tokens of at most MAX_RLE_LEN each; a byte left over at
        the end goes as a literal."""
        head = self.esc_bits + 3  # escape, g(1), 1, 1
        byte_len = self.rle_byte_len[byte]
        bits = 0
        while count > 1 << MAX_GAMMA:
            part = min(count, MAX_RLE_LEN)
            bits += head + MAX_GAMMA + 8 + GAMMA_LEN[((part - 1) >> 8) + 1] + byte_len
            count -= part
        if count == 1:
            bits += head + 8
        elif count:
            bits += head + GAMMA_LEN[count - 1] + byte_len
        return bits

    def _lz_cost(self, length: int, dist: int) -> int:
        if length == 2:
            return self.esc_bits + 2 + 8 if dist <= 256 else 100000
        extra = self.extra_dist_bits
        hi = ((dist - 1) >> (8 + extra)) + 1
        return self.esc_bits + 8 + extra + GAMMA_LEN[hi] + GAMMA_LEN[length - 1]

    def _lz_gain(self, length: int, dist: int) -> int:
        return length * 8 - self._lz_cost(length, dist)

    # --- match search ----------------------------------------------------------

    def find_matches(self) -> None:
        """Fill the run arrays, prev_pair and the run histogram, and find the best match
        at every position: the longest, unless a nearer shorter one gains more."""
        data, n = self.data, self.n
        run_len, run_offset, prev_pair = self.run_len, self.run_offset, self.prev_pair
        last_pair = [0] * 65536  # 1 + the last position of each byte pair, 0 if none

        # a hash of the 3 bytes at each position, checked before comparing bytes
        hashes = [0] * n
        a = b = 0
        for p in range(n - 1, -1, -1):
            a, b, c = data[p], a, b
            hashes[p] = (a * 3 + b * 5 + c * 7) & 0xFF

        for p in range(n):
            if not run_len[p]:
                length = 1 + _common_prefix(data, p, p + 1, n - p - 1)
                if length >= 2:
                    self.run_hist[data[p]] += 1
                    for i in range(length):
                        run_len[p + i] = length - i
                        run_offset[p + i] = i

            if p + run_len[p] + 1 < n:
                self._find_match(p, hashes, last_pair)

            if p + 1 < n:
                pair = (data[p] << 8) | data[p + 1]
                back = p + 1 - last_pair[pair]
                prev_pair[p] = back if last_pair[pair] and back <= 0xFFFF else 0
                last_pair[pair] = p + 1

    def _find_match(self, p: int, hashes: list[int], last_pair: list[int]) -> None:
        data, n = self.data, self.n
        run_len, run_offset, prev_pair = self.run_len, self.run_offset, self.prev_pair
        # The bytes at p are a run of `head` A's then a B. Candidates come from the list
        # of the pair (A, B), far shorter than the list of (A, A), and must be preceded
        # by the same number of A's.
        head = self.run_len[p] or 1
        floor = max(p - LZ_RANGE, 0) + head - 1
        i = last_pair[(data[p] << 8) | data[p + 1]] - 1
        if i < floor:
            return
        best_len, best_dist = 2, p - i
        want_hash = hashes[p]
        i = last_pair[(data[p + head - 1] << 8) | data[p + head]] - 1
        while i >= floor:
            # compare bytes only when the byte after the current best could match too
            same_head = head == 1 or run_len[i - head + 1] == head
            if same_head and hashes[i + best_len - head - 1] == want_hash:
                tail = n - p - head - 1
                length = head + 1 + _common_prefix(data, i + 2, p + head + 1, tail)
                if length > best_len:
                    length = min(length, MAX_LZ_LEN)
                    dist = p - i + head - 1
                    if self.max_len[p] < length:
                        self.max_len[p], self.max_dist[p] = length, dist
                    if self._lz_gain(length, dist) > self._lz_gain(best_len, best_dist):
                        best_len, best_dist = length, dist
                        want_hash = hashes[p + best_len - 2]
                    if best_len == MAX_LZ_LEN:
                        break
            if not prev_pair[i]:
                break
            i -= prev_pair[i]

        # the run before p also works as a match at distance 1, rarely beating RLE
        if p and run_len[p - 1] > best_len:
            best_len, best_dist = run_len[p - 1] - 1, 1
        # a longer stretch of the same run somewhere earlier
        if best_len < MAX_LZ_LEN and head > best_len:
            floor = max(p - LZ_RANGE, 0)
            i = last_pair[data[p] * 257] - 1
            while i >= floor:
                if run_offset[i] + 2 > best_len:
                    best_len = min(run_offset[i] + 2, head)
                    best_dist = p - i + best_len - 2
                    if best_len == head:
                        break
                i -= run_offset[i]
                if not prev_pair[i]:
                    break
                i -= prev_pair[i]

        best_len = min(best_len, n - p, MAX_LZ_LEN)
        if self.max_len[p] < best_len:
            self.max_len[p], self.max_dist[p] = best_len, best_dist
        if best_dist <= 256 or best_len > 2:
            self.lz_len[p], self.lz_dist[p] = best_len, best_dist

    # --- token chain -----------------------------------------------------------

    def optimize_lengths(self, optimize: bool) -> None:
        """Pick the cheapest token at every position, back to front, into cost and mode.

        With `optimize`, a run or match is also tried shorter, at the powers of two
        where a gamma code is cheapest, and a match at the nearest 2-byte match too: a
        shorter token can let a cheaper chain follow.
        """
        data, n = self.data, self.n
        run_len, run_offset, lz_len = self.run_len, self.run_offset, self.lz_len
        cost, mode = self.cost, self.mode
        cost[n] = 0
        i = n - 1
        while i >= 0:
            literal = 8 + cost[i + 1]
            if not lz_len[i] and not run_len[i]:
                cost[i], mode[i] = literal, LITERAL
                i -= 1
                continue

            if run_len[i] > MAX_LZ_LEN and run_offset[i] > 1:
                # a run too long for a match: price it from its start, mark it through
                offset = run_offset[i]
                i -= offset
                rle = self._rle_cost(run_len[i], data[i]) + cost[i + run_len[i]]
                if optimize:
                    shortest = max(run_len[i] - (1 << MAX_GAMMA), 2)
                    counts = range(run_len[i] - 1, shortest - 1, -1)
                    rle = self._shorten_rle(i, rle, counts)
                for z in range(offset + 1):
                    cost[i + z], mode[i + z] = rle, RLE
                i -= 1
                continue

            rle = lz = literal + 1000
            if run_len[i]:
                rle = self._rle_cost(run_len[i], data[i]) + cost[i + run_len[i]]
                if optimize:
                    rle = self._shorten_rle(i, rle, _powers_of_two(2, run_len[i]))
            if lz_len[i]:
                lz = self._lz_cost(lz_len[i], self.lz_dist[i]) + cost[i + lz_len[i]]
                if optimize and lz_len[i] > 2:
                    lz = self._shorten_lz(i, lz, rle)

            if rle <= literal and rle <= lz:
                cost[i], mode[i] = rle, RLE
            elif lz <= literal:
                cost[i], mode[i] = lz, LZ77
            else:
                cost[i], mode[i] = literal, LITERAL
            i -= 1

    def _shorten_rle(self, i: int, rle: int, counts) -> int:
        """Try the run at i at each count in `counts`, keeping the cheapest; returns
        its cost."""
        best, best_count = rle, self.run_len[i]
        for count in counts:
            v = self._rle_cost(count, self.data[i]) + self.cost[i + count]
            if v < best:
                best, best_count = v, count
        if best != rle:
            self.saved += rle - best
            self.run_len[i] = best_count
        return best

    def _shorten_lz(self, i: int, lz: int, rle: int) -> int:
        """Try the match at i shorter, and from other sources; returns its cost."""
        cost, near = self.cost, self.prev_pair[i]
        length, dist = self.lz_len[i], self.lz_dist[i]
        best, best_len, best_dist = lz, length, dist
        for k in _powers_of_two(4, length):
            v = self._lz_cost(k, dist) + cost[i + k]
            if v < best:
                best, best_len = v, k
        if not self.compatible:
            # the longest match here lost to a nearer shorter one on its own cost, but
            # the chain after it may pay for it
            for k in range(3, self.max_len[i] + 1):
                v = self._lz_cost(k, self.max_dist[i]) + cost[i + k]
                if v < best:
                    best, best_len, best_dist = v, k, self.max_dist[i]
        if 0 < near <= 256:
            v = self._lz_cost(2, near) + cost[i + 2]
            if v < best:
                best = v
                self.lz_len[i], self.lz_dist[i], lz = 2, near, v
        if best != lz and best < rle:
            self.saved += lz - best
            self.lz_len[i], self.lz_dist[i], lz = best_len, best_dist, best
        return lz

    def tokens(self):
        """(position, mode) of every token in the chain, front to back."""
        p = 0
        while p < self.n:
            m = self.mode[p]
            yield p, m
            p += self.lz_len[p] if m == LZ77 else self.run_len[p] if m == RLE else 1

    # --- parameters ------------------------------------------------------------

    def choose_escape_bits(self) -> None:
        """Try 1..8 escape bits until the escape overhead stops falling, then 0 bits
        (every literal escaped) against a 1-bit winner."""
        best_bits, best_overhead = 0, 1 << 30
        for bits in range(1, 9):
            self._set_escape_bits(bits)
            self.optimize_lengths(optimize=False)
            _, others, escaped = self.optimize_escapes()
            overhead = (bits + 3) * escaped + others * bits
            if overhead >= best_overhead:
                break
            best_bits, best_overhead = bits, overhead
        if best_bits == 1:
            self._set_escape_bits(0)
            self.optimize_lengths(optimize=False)
            _, _, escaped = self.optimize_escapes()
            if 3 * escaped < best_overhead:
                best_bits = 0
        self._set_escape_bits(best_bits)

    def choose_extra_dist_bits(self) -> None:
        """The extra distance bit count that prices the chain's matches cheapest."""
        totals = [0] * 5
        for p, m in self.tokens():
            if m == LZ77:
                for bits in range(5):
                    self.extra_dist_bits = bits
                    totals[bits] += self._lz_cost(self.lz_len[p], self.lz_dist[p])
        self.extra_dist_bits = min(range(5), key=totals.__getitem__)
        if self.extra_dist_bits:
            self.optimize_lengths(optimize=True)

    def optimize_escapes(self) -> tuple[int, int, int]:
        """Choose the initial escape and the escape each escaped literal switches to.

        Back to front over the chain's literals: the literal at i with top bits k needs
        an escape token if k is the escape, and then switches to the value with the
        fewest escaped literals still ahead. Returns (initial escape, count of other
        tokens, count of escaped literals).
        """
        data, n, states = self.data, self.n, 1 << self.esc_bits
        low_bits = 8 - self.esc_bits
        is_literal = [False] * n
        others = 0
        for p, m in self.tokens():
            if m == LITERAL:
                is_literal[p] = True
            else:
                others += 1

        ahead = [-1] * 256  # escaped literals from here on if the escape is k
        escaped = [-1] * 256
        best = best_ahead = 0
        for i in range(n - 1, -1, -1):
            if not is_literal[i]:
                continue
            k = data[i] >> low_bits
            self.new_escape[i] = best << low_bits
            ahead[k] = best_ahead + 1
            escaped[k] = escaped[best] + 1
            if k == best:
                best_ahead += 1
                for k in range(states - 1, -1, -1):
                    if ahead[k] < best_ahead:
                        best_ahead, best = ahead[k], k
                        break

        start, fewest = 0, n
        for k in range(states - 1, -1, -1):
            if ahead[k] <= fewest:
                start, fewest = k, ahead[k]
        return start << low_bits, others, escaped[start]

    def rle_token_hist(self) -> list[int]:
        hist = [0] * 256
        for p, m in self.tokens():
            if m == RLE:
                hist[self.data[p]] += 1
        return hist

    def rank_rle_table(self, hist: list[int]) -> None:
        """Rank the counted bytes into the table, most frequent first, and price the run
        token's byte code by it. Ranks past the used ones keep their old values."""
        hist = hist[:]
        self.rle_rank = {}
        for rank in range(1, RLE_TABLE_RANKS + 1):
            byte = max(range(256), key=lambda b: (hist[b], -b))
            if hist[byte] <= 0:
                break
            self.rle_table[rank] = byte
            self.rle_rank[byte] = rank
            hist[byte] = -1
        self.rle_used = len(self.rle_rank)
        self.rle_byte_len = [GAMMA_LEN[32] + 3] * 256
        for rank in range(1, RLE_TABLE_RANKS + 1):
            self.rle_byte_len[self.rle_table[rank]] = GAMMA_LEN[rank]

    def rescan(self) -> None:
        """A shortened match may reach further back than it needs to: move each match to
        the nearest source that still covers it, where the distance can cost less."""
        data, n = self.data, self.n
        run_len, lz_len, lz_dist = self.run_len, self.lz_len, self.lz_dist
        prev_pair = self.prev_pair
        for p, m in self.tokens():
            if m != LZ77 or lz_len[p] <= 2:
                continue
            if self.compatible and lz_len[p] <= run_len[p]:
                continue
            head = run_len[p] or 1
            floor = max(p - lz_dist[p] + 1, 0) + head - 1
            i = p - prev_pair[p]
            while i >= floor:
                if head == 1 or run_len[i - head + 1] == head:
                    tail = n - p - head
                    if head + _common_prefix(data, i + 1, p + head, tail) >= lz_len[p]:
                        lz_dist[p] = p - i + head - 1
                        break
                if not prev_pair[i]:
                    break
                i -= prev_pair[i]

    # --- output ------------------------------------------------------------------

    def _put_escape(self, out: _BitWriter, escape: int) -> None:
        out.bits(escape >> (8 - self.esc_bits), self.esc_bits)

    def _put_literal(self, out: _BitWriter, escape: int, byte: int, new: int) -> int:
        """Returns the escape in force after the literal."""
        if byte & self.esc_mask != escape:
            out.bits(byte, 8)
            return escape
        self._put_escape(out, escape)
        out.gamma(1)
        out.bits(0b10, 2)
        self._put_escape(out, new)
        out.bits(byte & ~self.esc_mask & 0xFF, 8 - self.esc_bits)
        return new

    def _put_rle(self, out: _BitWriter, escape: int, byte: int, count: int) -> None:
        while count:
            part = min(count, MAX_RLE_LEN)
            if part == 1:
                self._put_literal(out, escape, byte, escape)
                return
            self._put_escape(out, escape)
            out.gamma(1)
            out.bits(0b11, 2)
            if part <= 1 << MAX_GAMMA:
                out.gamma(part - 1)
            else:
                out.gamma((1 << MAX_GAMMA) | ((part - 1) & 0xFF) >> 1)
                out.bits((part - 1) & 1, 1)
                out.gamma(((part - 1) >> 8) + 1)
            rank = self.rle_rank.get(byte)
            if rank:
                out.gamma(rank)
            else:
                out.gamma(32 | (byte >> 3))
                out.bits(byte & 7, 3)
            count -= part

    def _put_lz(self, out: _BitWriter, escape: int, length: int, dist: int) -> None:
        self._put_escape(out, escape)
        out.gamma(length - 1)
        if length == 2:
            out.bits(0, 1)
        else:
            extra = self.extra_dist_bits
            out.gamma(((dist - 1) >> (8 + extra)) + 1)
            out.bits(((dist - 1) >> 8) & ((1 << extra) - 1), extra)
        out.bits((dist - 1) & 0xFF, 8)

    def emit(self, escape: int) -> bytes:
        data, esc = self.data, escape
        out = _BitWriter()
        for p, m in self.tokens():
            if m == LZ77:
                self._put_lz(out, esc, self.lz_len[p], self.lz_dist[p])
            elif m == RLE:
                self._put_rle(out, esc, data[p], self.run_len[p])
            else:
                esc = self._put_literal(out, esc, data[p], self.new_escape[p])
        self._put_escape(out, esc)
        out.gamma(2)
        out.gamma(255)

        table_size = (self.rle_used + 4) & ~3
        table = bytes(self.rle_table[1 : 1 + table_size]).ljust(table_size, b"\0")
        escape >>= 8 - self.esc_bits
        header = bytes([table_size, escape, self.extra_dist_bits, self.esc_bits])
        return header + table + out.finish()


def encode(raw: bytes, compatible: bool = True) -> bytes:
    """Pack `raw` as a pucrunch stream.

    `compatible` reproduces the game's own packer, and with it every BG asset tile
    stream in the ROM byte for byte, which the carved assets need to rebuild it. False
    turns on the two later refinements of the reference packer (see _Packer) for a
    slightly smaller stream the same unpacker reads; use it for hack assets.
    """
    stream = _Packer(raw, compatible).pack()
    if decode(stream) != raw:
        raise AssertionError("pucrunch.encode: stream does not decode to its input")
    return stream
