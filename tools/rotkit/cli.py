"""RotKit - Reverse engineering and ROM hacking toolkit: rotkit [<group>] <command> [args].

This is purely a registration table: a command is a plain typed run() function in the
same-named module (`rotkit check rom` -> rotkit/check/rom.py run()), importable and callable
directly from any script; typer builds the parser and --help from the signature. Adding a
tool = a module with a run() + one table row.

Layout rule: groups are purposes, one subpackage each. Shared code lives in rotkit's
top-level library modules and is imported (`from rotkit.rom import load_rom`), never
sys.path tricks. Command modules do no work at import time - they are all imported at
CLI startup; only option-flag parameters need typer metadata (Annotated[...]).
"""

from collections.abc import Callable, Sequence

import typer

from rotkit import coverage, eeprom, match
from rotkit.analysis import decode_text, dump_affixes, hit_chance, mission_scripts
from rotkit.build import (
    cc,
    clangd,
    debug,
    format_stores,
    gfx,
    layout,
    objdiff,
    objdiff_config,
    symbols,
    variables,
)
from rotkit.build import rom as build_rom
from rotkit.check import rom as check_rom
from rotkit.check import stores as check_stores
from rotkit.carve import (
    actor_tables,
    all as carve_all,
    enemy_types,
    bg_assets,
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
from rotkit.ghidra_build import apply_symbols, gen_project, parse_headers
from rotkit.hack import bps, modcode_syms, tablepatch

app = typer.Typer(
    no_args_is_help=True, help="RotKit - Reverse engineering and ROM hacking toolkit"
)

# The everyday RE-loop commands live at the top level.
app.command(
    "match", help="compile a candidate .c with agbcc and byte-diff it against the ROM"
)(match.run)
app.command("coverage", help="carve progress: split.cfg regions vs base-ROM remainder")(
    coverage.run
)
app.command(
    "eeprom-fix",
    help="convert an emulator .sav to/from the game's EEPROM byte order",
)(eeprom.run)


def group(
    name: str,
    help_: str,
    commands: Sequence[tuple[str, Callable, str]],
) -> typer.Typer:
    sub = typer.Typer(no_args_is_help=True, help=help_)
    app.add_typer(sub, name=name)
    for cmd, fn, h in commands:
        sub.command(cmd, help=h)(fn)
    return sub


group(
    "check",
    "verification gates (make check runs both)",
    [
        ("stores", check_stores.run, "lint the text stores"),
        (
            "rom",
            check_rom.run,
            "per-region byte-match vs the base ROM (optional FUNC arg: hex window)",
        ),
    ],
)
group(
    "carve",
    "carve ROM data into committed C",
    [
        (
            "all",
            carve_all.run,
            "re-carve everything below, plus variables.h (make carve)",
        ),
        ("text-ids", text_ids.run, "enum TextId from the ROM's English text"),
        (
            "item-gfx",
            item_gfx.run,
            "per-base-item backpack/equip/hollow icons (PNGs + ROM records)",
        ),
        ("loot-piles", loot_piles.run, "ground drop sprites (PNGs + tiles/frame sets)"),
        (
            "level-icons",
            level_icons.run,
            "level select icons (PNGs + tiles/frame sets/palettes)",
        ),
        (
            "bg-assets",
            bg_assets.run,
            "hero portraits, dialog pictures, cutscene pages (PNGs + assets)",
        ),
        ("item-tables", item_tables.run, "base items, affixes, runes"),
        ("enemy-types", enemy_types.run, "per-enemy-type stat, SFX and loot tables"),
        ("actor-tables", actor_tables.run, "NpcSfxSets"),
        ("player-tables", player_tables.run, "XpThresholds"),
        ("mission-tables", mission_tables.run, "RegionMissionRanges"),
        ("loot-tables", loot_tables.run, "the loot roll tables"),
        ("scene-handlers", scene_handlers.run, "SceneHandlers"),
        ("songs", songs.run, "the Songs table"),
    ],
)
group(
    "build",
    "machine build steps invoked by make (rarely typed by hand)",
    [
        ("cc", cc.run, "compile matched C to exact ROM bytes"),
        ("rom", build_rom.run, "emit build/rom.s for the whole-ROM reconstruction"),
        ("symbols", symbols.run, "stores -> build/game_symbols.{inc,ld}"),
        (
            "debug",
            debug.run,
            "stores + headers -> build/debug/ gdb bundle for mGBA's stub",
        ),
        ("layout", layout.run, "hacks/layout.cfg -> build/memmap.{inc,h,ld}"),
        (
            "gfx",
            gfx.run,
            "carved-local/gfx PNGs -> build/gfx byte lists for the icon TUs",
        ),
        (
            "variables",
            variables.run,
            "data stores -> build/include/variables.h",
        ),
        (
            "clangd",
            clangd.run,
            "emit compile_commands.json for clangd whole-tree indexing",
        ),
        ("objdiff", objdiff.run, "build an objdiff ELF pair for one src/c file"),
        ("objdiff-config", objdiff_config.run, "write objdiff.json for every pair"),
        (
            "format-stores",
            format_stores.run,
            "sort the hand-edited stores (config/*.cfg) by address",
        ),
    ],
)
group(
    "hack",
    "patch build",
    [
        ("bps", bps.run, "emit a verified BPS from a built hack ROM"),
        ("modcode-syms", modcode_syms.run, "ELF -> armips modcode symbol equates"),
        (
            "tablepatch",
            tablepatch.run,
            "table-patch object -> armips per-entry .org splices",
        ),
    ],
)
analysis = group(
    "analysis",
    "hand-run investigation (carve and ghidra reuse its decoders)",
    [
        (
            "dump-affixes",
            dump_affixes.run,
            "dump item prefix/suffix tables (--show to pretty-print)",
        ),
        (
            "hit-chance",
            hit_chance.run,
            "optimal player accuracy per difficulty vs enemy defense percentiles",
        ),
    ],
)
group(
    "ghidra",
    "regenerable browse project",
    [
        ("gen-project", gen_project.run, "import the ROM into a fresh browse project"),
        (
            "apply-symbols",
            apply_symbols.run,
            "stamp the stores onto the browse project",
        ),
        ("parse-headers", parse_headers.run, "C-parse include/*.h (standalone report)"),
    ],
)

decode_text_app = typer.Typer(no_args_is_help=True, help="decode the Huffman text")
decode_text_app.command("show")(decode_text.run_show)
decode_text_app.command("dump")(decode_text.run_dump)
analysis.add_typer(decode_text_app, name="decode-text")


mission_scripts_app = typer.Typer(
    no_args_is_help=True,
    help="mission-script raw dumps + transition graph + chapter chains",
)
mission_scripts_app.command("graph")(mission_scripts.run_graph)
mission_scripts_app.command("dump")(mission_scripts.run_dump)
mission_scripts_app.command("chains")(mission_scripts.run_chains)
mission_scripts_app.command("chapters")(mission_scripts.run_chapters)
mission_scripts_app.command("from-file")(mission_scripts.run_from_file)
analysis.add_typer(mission_scripts_app, name="mission-scripts")


def main() -> None:
    app()
