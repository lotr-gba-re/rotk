"""Emit a verified BPS from an already-built patched ROM.

Expects build/rotk_<name>.gba to already exist (`make dist` builds it and passes name
`hack`).

Steps:
  1. verify the base ROM SHA-1 (never patch the wrong image).
  2. flips --create rotk.gba build/rotk_<name>.gba dist/<name>.bps
  3. round-trip: flips --apply dist/<name>.bps rotk.gba build/rotk_<name>_rt.gba,
     and assert its SHA-1 == the patched ROM (the BPS re-applies cleanly).
"""

import hashlib
import os
import subprocess
import sys

from rotkit.paths import BUILD, ROOT
from rotkit.rom import ROM

DIST = ROOT / "dist"  # BPS output dir (single consumer)


def sha1(path: str) -> str:
    hasher = hashlib.sha1()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            hasher.update(chunk)
    return hasher.hexdigest()


def sh(*cmd: str | os.PathLike) -> None:
    print("  $", " ".join(str(c) for c in cmd))
    subprocess.run(cmd, check=True, cwd=ROOT)


def run(name: str) -> None:
    patched = BUILD / f"rotk_{name}.gba"
    roundtrip = BUILD / f"rotk_{name}_rt.gba"
    bps = DIST / f"{name}.bps"
    if not os.path.isfile(patched):
        sys.exit(f"{patched} missing - build it first (`make dist`)")

    # 1. base ROM identity
    want_sha = (ROOT / "baserom.sha1").read_text().split()[0]
    base_sha = sha1(ROM)
    if base_sha != want_sha:
        sys.exit(f"base ROM SHA-1 mismatch: {base_sha} != {want_sha}")
    print(f"base ROM ok ({base_sha})")

    os.makedirs(DIST, exist_ok=True)

    # 2. BPS
    sh("flips", "--create", ROM, patched, bps)

    # 3. round-trip verify
    sh("flips", "--apply", bps, ROM, roundtrip)
    if sha1(roundtrip) != sha1(patched):
        sys.exit("ROUND-TRIP FAIL: re-applied BPS != patched ROM")
    print(
        f"\nhack bps OK: {os.path.relpath(bps, ROOT)} "
        f"({os.path.getsize(bps)} bytes) re-applies cleanly (sha {sha1(patched)})"
    )
