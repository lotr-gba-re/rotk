# Conventions

Naming, coding, and authoring conventions for the decompiled C code and the config files.

These are general rules, not laws.
Ultimately, you should choose what makes sense.

However, try to follow the guidelines and glossary defined here if possible, and add new commonly used or non-obvious terms to the glossary as they come up.

## Headers

Each module has its own header, with scenes and actors having one header per scene/actor type.

### Generated Headers

Header files in `carved/` or `build/` are auto-generated and should not be edited by hand.

### Function Prototypes

All ROM functions are declared in the corresponding header.

A prototype of a decompiled function does not have a doc comment.
The Doxygen comment goes on the `.c` definition instead.
Functions that are not yet decompiled are allowed to be documented on their prototypes.

### Type Definitions

Structs and unions should be declared in the fused `typedef struct X { ... } X;` style.
In rare cases (e.g. self-references) you can use the `struct X *` style.

## Comments

### Function Comments

Document each decompiled function with a Doxygen comment saying what it does and how it can be used, not how it works internally.
Be terse and do not repeat everything that happens in the function body.
Describe only on a high level.

Use `@param` / `@return` where they are not completely obvious from the name.
All decompiled functions must have a `@romaddress 0x08xxxxxx` annotation that matches the corresponding `config/functions.cfg` row.

Separate the description, `@param` / `@return`, and `@romaddress` with an empty line.

Example:

```c
/**
 * Calculate player's bonus damage against the victim's creature family (animals, wargs, orcs, ...).
 *
 * @param enemyType index into EnemyTypes (its .category selects the stat)
 *
 * @romaddress 0x08011488
 */
u16 combat_getEnemyTypeDamageBonus(s32 playerIndex, u8 enemyType)
```

Comments inside the function body are encouraged for tricky sections, but be terse and do not explain things that are clear from reading the code.

`static` functions may have a Doxygen comment, but it is not mandatory.

### General Comments

Use `//` for all other comments, even if they span multiple lines.

## Naming Conventions

### Functions

Functions are named `<moduleName>_<functionName>`, with an optional middle segment for a submodule.

```
combat_rollVictimEvade        module + function
scene_saveLoadMenu_onConfirm  middle segment: the scene instance
mission_script_runStream      middle segment: a submodule (the script VM inside mission)
```

- Segments are camelCase, underscores only between levels.
- Use a submodule only for a genuine sub-namespace (`item_affix_formatName`), and give its files a directory (`src/c/item/affix_formatName.c`).
  A submodule sharing a span stays in the parent's file.
- Don't create too many modules: one module (such as `combat`) may aggregate related functionality.

A function that returns a boolean value and has no side effects should start with a verb (`isPresent`, `canContinue`).

A `static` file-local helper should be a simple camelCase name with no module prefix (`applyFlinch`, `rowColor`).

### File Names

A `src/c` file covers one contiguous decompiled ROM span.
Try to create one file per module.

If there are still gaps between decompiled translation units, multiple files per module are permissible.
Name such split module files like `<module>_<topic>.c` with the topic being a camelCase segment describing what functions the file contains (e.g. `combat_damageCalc.c`).
If your file only contains a single function, use the full function name (`combat_shieldAbsorbsHit.c`).

Scenes and actors get one file per type (see below).

### Actors

- Create one header and one `.c` file per actor type.
- Conventional functions `actor_<name>_<function>` are:
    - `_create`: Create the actor from given parameters.
    - `_createFromMission`: Create the actor with parameters taken from the mission script.
    - `_tick`: Tick/update function.
      See `Actor.tickCallback`.
    - `_trigger`: Actor trigger function.
      See `Actor.triggerCallbacks[]`.

### Scenes

- Create one header and one `.c` file per scene type.
- Conventional functions `scene_<name>_<function>` are:
    - `_enter`: `SceneEntry.enter`, run on entry into the scene.
    - `_tick`: `SceneEntry.tick`, the per-frame body.
    - `_exit`: `SceneEntry.exit`, run when a scene change is dispatched.
    - `_onConfirm` / `_onBack`: A-button and B/Select handlers.
    - `_moveCursor`: d-pad cursor movement.

### Hack Code

ROM hack code in `hacks/` follows the same naming conventions.
Helper functions that could be useful for many hacks can be put into `hacks/lib/`.

Hack module names must never reuse a game module name, so it is always clear where a function comes from.

### Globals and Tables

- Data symbols in the ROM are PascalCase.
    - Arrays get plural names (`EnemyTypes`, `WeaponPrefixes`, `DirectionMoveVectors`)
    - Singular values get a descriptive singular name (`SaveHeaderTemplate`).
- RAM globals carry a `g_` prefix (`g_PlayerGlobals`, `g_SaveSlots`).
- ROM data constants never use the `g_` prefix.
- A table consumed by a single subsystem may spell that subsystem inside its PascalCase name (e.g. `FxChargeVariants`).
- Globals with an unknown purpose are named `g_Unknown<Hint>`, with the hint being your best guess.
  Add the address when the hint alone would be ambiguous (e.g. `g_UnknownSfxHandle_030035c4`).

### Identifier Style

- Prefer full words instead of abbreviations, e.g. `damage` (not `dmg`), `percent` (not `pct`), `count` (not `cnt`).
- Universal acronyms and terms are fine as abbreviations, e.g. ID, HP, RNG, SFX, ROM, RAM.
- Don't use `Struct` or `_t` suffix for structs.

## Code Style

### Formatting

- C: `clang-format` as configured in the repo (run via `make format`).
- Python: `uv format` (run via `make format`).
- Markdown: one line per sentence, no hard wrapping.

### Python

For Python code, use type hints for function arguments and return types.

### Types

- **Integers**: use `u8` / `u16` / `u32` / `u64` (unsigned) and `s8` / `s16` / `s32` / `s64` (signed) instead of `int`, `short`, and so on.
- Use `bool` for boolean values. `TRUE` and `FALSE` macros are available.
- Use `u8` as a generic type for bytes and yet-undecoded fields in structs.

### Enums

- Always assign enum values explicitly (e.g. `OPTIONS_ROW_DIFFICULTY = 5`), even when the implicit sequence would produce the same values.
- Use placeholder constants (e.g. `STAT_UNKNOWN_75 = 75`) for constants you do not yet know the meaning of.
  Defining these early makes it easy to rename them all at once later.
  Placeholder bits in a flag enum are named by bit index (`AFFIX_FLAG_14 = 1 << 14`) once they get a consumer.

### Named Constants vs Magic Numbers

- Prefer `#define`d constants over magic numbers for gameplay values.
    - For example, define a `BURST_PROC_PERCENT` constant instead of using a literal 10 in the code.
    - On the other hand, a literal 2 for the `maxDamage / 2` to `maxDamage` damage range is fine to use as a literal.
    - Literal x/y coordinates for UI code are also fine (unless used multiple times for alignment of course).
    - Apply your best judgement.

### Bitfields and Bit Constants

- Define a bitfield as a union of a packed mask view `p` and a decoded view `d` (`union UnlockFlags { u32 p; struct { ... } d; };`, see [Type Punning](#type-punning)):
    - Matched code often needs to test the packed view (`unlockFlags.p & UNLOCK_FLAG_ANCIENT_ITEMS`) as that seems to be what the original code did.
    - Carved tables and hack code use the decoded view (`.flags.d = { .ancient = 1 }`, `unlockFlags.d.ancientItems`) for clarity. The decoded view is also handy for `gdb` debugging.
- The bit values also get an `enum` for the packed-view tests, named after the bitfield (e.g. `enum UnlockFlag`).
  When the bitfield has a union, place the enum directly above it.
- A sub-u32 bitfield (u8/u16) needs `__attribute__((packed))` on both the union and the bitfield struct member (see `union PlayerCombatFlags`).
- Write single-bit constants as `1 << n` (bit position) in enum/#define values and in comments (`bit 15`, `1 << 15`), not as hex values.
  List only bits that have a consumer or a known meaning, gaps in the positions are fine.
- Use `1u << 31` for the top bit (plain `1 << 31` overflows signed int).

### Access Macros for Global Variables and Tables

Frequent, cluttered accesses to global variables or tables get a shorthand macro next to the struct or table they read.
Prefer the macros if they are defined for the variable or table.

Macros of note include:

- **`PLAYER` and `PLAYER_STAT`**: access a player by index, or index into their `.stats`.
- **`ACTIVE_PLAYER`**: the active player (via `g_PlayerGlobals.activePlayerIndex`)
- **Item access**: each takes an `Item`.
    - **`ITEM_TYPE_INFO`**: Get the corresponding `ItemTypeInfo` for its type
    - **`ITEM_BASE`**: Get the item's `ItemBase`.
    - **`ITEM_PREFIX`/`ITEM_SUFFIX`**: Get the item's prefix/suffix `ItemAffix`.
    - **`ITEM_BASE_METADATA`**: Get the item's `ItemMetadata`.

### Type Punning

Matched C sometimes reads a buffer at a different element width than its type suggests, e.g. summing a save slot as `u16` words although it is logically a struct.
Model this as a `union` of the typed views, not as a raw pointer cast.

Example (see `SaveSlot` and `SaveGlobals` in `save.h`):

```c
union SaveSlot
{
    u8 bytes[0x70];      // byte view
    u16 halfwords[0x38]; // halfword view
    struct // decoded view
    {
        u8 exampleField1;
        u8 exampleArray[0x6f];
    } d;
};
```

- The decoded view is a named member (`d`) because agbcc (gcc 2.9) does not support anonymous struct members.
- Use the raw/halfword views for checksum / `memset` / `memcpy` type-punning and the `d` view for named per-field access at call sites.
- Use placeholder `u8[]` members named by offset (e.g. `field_0x31`) for spans whose purpose is not yet known.
- Bitfields use the same idiom with a packed view and a decoded bitfield `d` view (`union UnlockFlags`, `union ItemFlags`); see [Bitfields and Bit Constants](#bitfields-and-bit-constants).

## Glossary

This is a list of recurring or non-obvious names for concepts.
Try to always use the canonical name.

When you coin a new term or concept, consider adding it here.

Keep table entries to one line where possible.

### General

| concept                     | canonical name | meaning                                                                                                      | notes                                                                             |
| --------------------------- | -------------- | ------------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------------------- |
| scene                       | scene          | A scene or screen of the game, e.g. the main menu, the pause menu, the backpack, or the main gameplay scene. | each scene has its own header and `.c` file                                       |
| passive skill (stat bonus)  | passive skill  | Passive, non castable skills from the left side of the skill tree.                                           | see `passiveSkill.h` (`PASSIVE_SKILL_*`); levels in `Player.passiveSkillLevels[]` |
| active skill (move / spell) | active skill   | Castable active skills from the right side of the skill tree.                                                | see `activeSkill.h`; `ACTIVE_SKILL_HERBAL_HEALING` = 5 on every hero              |
| melee weapon damage type    | slash / impale | melee attacks are either slash damage (most weapons) or impale damage (e.g. daggers)                         |                                                                                   |

### Missions, Regions, and Maps

| concept                                        | canonical name | meaning                                                                                                                                             | notes                                                                                            |
| ---------------------------------------------- | -------------- | --------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------ |
| one of the 4 hero routes on the chapter select | path           | Aragorn's (shared with Legolas/Gimli) / Frodo's / Gandalf's / Eowyn's                                                                               |                                                                                                  |
| one playable level                             | mission        | mission ids index `MissionTable[130]`, whose block holds the script streams, spawn groups, and waypoints; the unit a warp or chapter launch targets | `enum MissionId` in `mission.h` (`MISSION_<REGION>_<NAME>`); current one in `g_CurrentMissionId` |
| named world area grouping missions             | region         | a contiguous `MissionId` range with a name text; region ids index `RegionMissionRanges[28]`                                                         | `enum RegionId` in `mission.h` (`REGION_*`)                                                      |
| which visit/version of a mission loads         | `variant`      | selects the variant chunks of the mission block (story visits, flooded revisit, ...)                                                                | `g_MissionVariant`; stamped into the save slot at save time                                      |
| start point within a mission                   | `spawnPoint`   | per-player progress byte selecting where players enter the mission                                                                                  | `g_MissionSpawnPoint[2]`; stamped into the save slot at save time                                |
| (mission, variant, spawnPoint) launch triple   | chapter        | Full triple required to start a playable mission                                                                                                    |                                                                                                  |
