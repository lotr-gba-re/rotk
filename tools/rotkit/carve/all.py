"""Re-carve everything: every committed carved/ table and header, plus the ROM-carved
carved-local/ files (the graphics PNGs), then variables.h. Backs `make carve`.
"""

from rotkit.build import variables
from rotkit.carve import (
    CaptionsOption,
    actor_tables,
    bg_assets,
    enemy_types,
    item_gfx,
    item_tables,
    level_icons,
    loot_piles,
    loot_tables,
    mission_tables,
    player_tables,
    scene_handlers,
    songs,
    text_ids,
)


def run(captions: CaptionsOption = True) -> None:
    text_ids.run()
    item_gfx.run(captions)
    loot_piles.run(captions)
    level_icons.run(captions)
    bg_assets.run(captions)
    item_tables.run()
    enemy_types.run()
    actor_tables.run()
    player_tables.run()
    mission_tables.run()
    loot_tables.run()
    scene_handlers.run()
    songs.run()
    variables.run()
