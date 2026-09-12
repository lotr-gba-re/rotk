"""Regenerate the hand-edited stores in canonical form (`make format`).

config/functions.cfg + config/data.cfg are fully rewritten (header, section banners,
address-sorted rows) via the stores.py writers.
"""

import os

from rotkit import stores
from rotkit.paths import DATA_CFG, FUNCS_CFG, SPLIT_CFG


def run() -> None:
    results = [
        (FUNCS_CFG, stores.write_funcs(stores.strict_rows(FUNCS_CFG, 3))),
        (DATA_CFG, stores.write_data(stores.strict_rows(DATA_CFG, 3))),
        (SPLIT_CFG, stores.write_split(stores.read_split())),
    ]
    for path, changed in results:
        print(f"  {'formatted' if changed else 'ok':6} {os.path.relpath(path)}")
