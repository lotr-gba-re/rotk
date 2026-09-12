# Agent Instructions

This file contains agent-specific instructions.
Please read the human documentation for specifics if needed:

- `docs/conventions.md` (naming and code conventions)
- `docs/workflow.md` (build and dev workflow)
- `docs/setup.md` (toolchain setup)

## Source Layout

Three kinds of source, encoded by directory:

- `src/`, `include/`, `config/`: hand-written.
  Edit freely.
- `carved/`: carved from the ROM and committed.
  Never edit by hand; change the carver in `tools/rotkit/carve/` and re-run `make carve`, then review the diff.
- `carved-local/`: assets carved from the ROM into editable formats (graphics, sound, animations, ...), not committed.
  Never commit; regenerate with `make carve`.
  Edit one only to try a change, `make carve` overwrites it.

## Always Read the Conventions Doc

Before writing or editing any C in `src/c/`, `include/`, `carved/`, `hacks/`, or `config/*.cfg`, you must read `docs/conventions.md` and follow its naming, types, and canonical-names glossary.

If you encounter a new concept or name that should be included in the glossary, ask the user to confirm it and do it.

## Use Ghidra First

When reverse-engineering a function or decompiling code that is not yet in C, always start from Ghidra using GhidrAssist MCP.
If no Ghidra MCP connection is available, stop and ask the user before proceeding; that is almost certainly a mistake.

Do not use terminal tools to disassemble or decompile functions or to find x-refs unless Ghidra cannot accomplish your goal.

Do not rename or annotate symbols in Ghidra; the project is periodically regenerated from the config files, so names only stick when written to `config/*.cfg` and `include/*.h`.

## RE / Decomp / Byte-Matching Workflow

Discover a function or struct in Ghidra, author it in the config files (`config/functions.cfg`, `config/data.cfg`, `include/*.h`), reimplement it in `src/c/<Name>.c`, then match bytes against the original ROM with `uv run rotkit match <Name> <candidate.c> --full` until it prints `MATCH`.

Read the `byte-matching` skill (`.agents/skills/byte-matching/SKILL.md`) when matching bytes: it catalogs the agbcc codegen quirks that decide matches (constant-index folding paths, biased-base folds, prologue ordering, branch layout) and the technique for settling them.
Feed newly discovered quirks back into it per its "Maintaining This Skill" section.

`--full` prints the whole side-by-side with diverging rows marked `*`; omit it for an 8-row window around the first diverging byte when less context is needed.

Add the address to `config/split.cfg` and run `make verify` to confirm the whole ROM still reconstructs byte for byte.

`docs/workflow.md` has the full loop and the deeper project workflow.

## Research Logs

Keep in-progress research notes (unconfirmed hypotheses, partially-reversed structures, addresses not yet typed) in `local/wip-docs/`, which is gitignored scratch and not committed.
A note there is a staging area, not a source of truth; delete it once the knowledge lands in code.
Committed files never depend on `local/` contents as other repo users won't have these files; naming the `local/` convention directories themselves (personal scratch locations) is fine.

Prefer making the code and data structures speak for themselves (good names, types, terse field comments) over committed prose, since prose drifts from the code and an agent can re-derive facts from reversed structures.

## Comment and Text Rules

Do not use em-dashes anywhere in repository files.
Aim for code comments and docstrings to be on the terse side, LLM docs tend to be too verbose rather than too terse.
Don't document things that can easily be learned from reading the code.
Do not append parenthetical consumer references to comments (e.g. `(combat_rollVictimEvade, combat.h)`); consumers are greppable.
Avoid absence claims like "no writer found in the ROM" or "dead code"; these assumptions can change and xrefs can be hard to find.

Comments are used to explain the current state of the code, not argue or defend the approach taken.
It's fine to state why the code is how it is, but only do it when absolutely necessary.
Do not reference prior approaches or revisions in comments.
Especially do not refer to user change requests in phrases like "now with less repetition", "no separate function needed", or "as requested; without prefix".

## Git Command Usage

Do not run state changing Git commands unless explicitly instructed to.
This includes using `git checkout`, `git stash`, or `git reset` to manipulate the index or working copy.
Do not use git to roll back files unless explicitly instructed.

Read operations (such as reading status or the log) are fine, but do not use git to, for example, reset any files unless explicitly instructed to.
