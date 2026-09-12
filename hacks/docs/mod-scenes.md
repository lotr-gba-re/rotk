# Mod Scenes

The stock `SceneHandlers` table is full (26 entries, ids `0x00`..`0x19`) and cannot grow in place, so `mod_scenes` replaces the three dispatchers with range-checking versions that route ids `0x1a`+ to the modcode's own `ModSceneHandlers`.

## Adding a Row

A patch adds an `enum ModSceneRow` entry in `hacks/mod_scenes.h` and a `MOD_SCENE_ROW(name, prefix)` initializer in `mod_scenes.c` naming its `<prefix>Enter` / `<prefix>Tick` / `<prefix>Exit` triple, both behind its `#if HACK_<name>`.
Enter the scene with `game_requestSceneChange(MOD_SCENE_ID(name))`, or `game_requestSceneChangeWithArgs` to carry a param.
Ids are internal to one build (nothing persists them), so rows shifting between configs is fine.

## The Per-Frame Contract

A row makes the scene a top-level scene, with per-phase duties nothing enforces:

| phase | must do |
| --- | --- |
| enter | `gfx_initDisplay(0)`, `sprite_resetTileAllocatorForMode(0)`, the BG setup, then `actor_tickAll()`, `sprite_endFrame()`, fade in |
| tick | `actor_tickAll()` and `sprite_endFrame()`, at the end, every frame |
| exit | fade out, `sprite_flushList(&g_ActorListMain)`, `gfx_commitBgs()`, `gfx_clearPaletteAnims()`, `sfx_stopAllOnExit()` |

`sprite_endFrame` is the easiest to miss and the worst to debug: it raises the frame-ready flag (`g_SpriteManager + 0x38`) the VBlank commit task requires, so a scene that skips it commits neither sprites nor BG scroll.
The screen freezes rather than blanks, and the per-frame integrator keeps advancing positions in RAM, so every value you inspect reads correct.

Exit must call `gfx_clearPaletteAnims()` if the scene called `gfx_registerPaletteCycles`, or its palette cycle animates on into the next scene.
Exit as a whole is skipped while `GAME_FLAG_IN_GAME_DIALOG` is set (`modScenes_dispatchExit`, matching the stock dispatcher).

Model a mod scene on `scene_mainMenu_*` or `scene_languageSelect_*`, not the options menu (an overlay: `actor_tickAll` skips main-list work while `GAME_FLAG_OVERLAY` is set).

## Scene State

The modcode has no `.bss` (see the README), so a mod scene keeps nothing in globals.
`g_SceneCurrent.args` is five words that persist for the scene's life, zeroed by `game_requestSceneChange` on the way in.
State too big for that goes on the game heap: allocate in `enter`, keep the pointer in an arg slot, release in `exit`.
