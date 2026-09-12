#!/usr/bin/env bash
# Install the period-appropriate compiler agbcc.
#
# agbcc is the old-gcc fork used by the pret Pokemon decomps. RoTK is a 2003 gcc-family title
# and its bytes match agbcc's codegen idioms where a modern arm-none-eabi-gcc diverges.

set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"

git -C "$ROOT" submodule update --init agbcc
( cd "$ROOT/agbcc" && ./build.sh && ./install.sh "$ROOT" )
echo "agbcc ($(git -C "$ROOT/agbcc" rev-parse --short HEAD)) installed -> $ROOT/tools/agbcc"
