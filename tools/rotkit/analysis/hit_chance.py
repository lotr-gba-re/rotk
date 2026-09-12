"""Optimal player accuracy per difficulty, derived from EnemyTypes.

The to-hit equation (combat_rollAccuracy @0x08011ac0):

    hit% = clamp(attackerAccuracy + 50 - victimDefense, 10, 95)

so a player needs accuracy = enemyDefense + 45 to pin the 95% cap against a given enemy.
combat_getEnemyDefense scales the base EnemyTypes.defense by difficulty: normal x1,
hard x1.5, grueling x2 (truncating (base*pct)/100 via math_percentOf).

This reads the 83-entry EnemyTypes table (@0x08057518) straight from the ROM and prints,
per difficulty, the accuracy that caps the hit chance against the enemy at defense
percentile p (nearest-rank: the cheapest defense value that still covers p% of all types).
--spread adds the accuracy/defense spread.
"""

import math
import statistics
from typing import Annotated

import typer

from rotkit.rom import ROMBASE, load_rom

ENEMY_TYPES_ADDR = 0x08057518
ENEMY_TYPE_COUNT = 83
ENEMY_TYPE_SIZE = 0x14
ACCURACY_OFF = 0x0F  # EnemyTypeInfo.accuracy
DEFENSE_OFF = 0x10  # EnemyTypeInfo.defense

MAX_HIT_CHANCE = 95  # combat_rollAccuracy clamps to 10..95
PERCENTILES = (50, 80, 90, 95, 98, 99, 100)

# (name, percent bonus): combat_getEnemy{Accuracy,Defense} add base*pct/100 (truncated).
DIFFICULTIES = (("normal", 0), ("hard", 50), ("grueling", 100))


def scaled_stat(field_off: int, pct: int) -> list[int]:
    """A difficulty-scaled EnemyTypes u8 stat column, sorted ascending."""
    rom = load_rom()
    base = ENEMY_TYPES_ADDR - ROMBASE
    vals = [
        rom[base + i * ENEMY_TYPE_SIZE + field_off] for i in range(ENEMY_TYPE_COUNT)
    ]
    return sorted(v + v * pct // 100 for v in vals)


def optimal_accuracy(defs: list[int], percentile: int) -> tuple[int, int]:
    """(defense at nearest-rank percentile, accuracy for the hit cap vs it)."""
    rank = math.ceil(percentile / 100 * len(defs))  # 1-indexed
    defense = defs[rank - 1]
    return defense, defense - 50 + MAX_HIT_CHANCE


def run(
    spread: Annotated[
        bool,
        typer.Option(
            help="also print the accuracy/defense min/max/mean per difficulty"
        ),
    ] = False,
) -> None:
    print(
        f"Player accuracy for the {MAX_HIT_CHANCE}% hit cap "
        f"(acc = defense + {MAX_HIT_CHANCE - 50}) vs the enemy at defense percentile p\n"
        f"of all {ENEMY_TYPE_COUNT} EnemyTypes (nearest-rank):\n"
    )
    cols = {name: scaled_stat(DEFENSE_OFF, pct) for name, pct in DIFFICULTIES}
    header = f"{'p':>4} |" + "|".join(f" {name:^16} " for name in cols)
    print(header)
    print("-" * len(header))
    for p in PERCENTILES:
        row = f"p{p:<3} |"
        for defs in cols.values():
            defense, acc = optimal_accuracy(defs, p)
            row += f" {f'{acc:>3} (def {defense:>3})':^16} |"
        print(row)

    if spread:
        print("\nEnemy spread per difficulty (min/max/mean/median):")
        for name, pct in DIFFICULTIES:
            defs = scaled_stat(DEFENSE_OFF, pct)
            accs = scaled_stat(ACCURACY_OFF, pct)
            print(
                f"  {name:>8}: accuracy {accs[0]:>3}/{accs[-1]:>3}"
                f"/{sum(accs) / len(accs):5.1f}/{statistics.median(accs):5.1f}"
                f"   defense {defs[0]:>3}/{defs[-1]:>3}"
                f"/{sum(defs) / len(defs):5.1f}/{statistics.median(defs):5.1f}"
            )
