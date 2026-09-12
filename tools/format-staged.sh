#!/bin/sh
# Format the STAGED content of C files (src/c + hacks, *.c) and write it back into the
# index - without disturbing unstaged working-tree changes. Unlike `clang-format -i + git add`
# (which restages whole files), this is safe for partially-staged files (git add -p): the
# commit gets the formatted staged content; your unstaged edits are left exactly as they are.
#
# How: format the staged blob, store it as a new object, and point the index entry at it via
# `git update-index`. The working tree is only rewritten for files that had NO unstaged changes
# (so it stays in sync); files with unstaged edits keep their working-tree version untouched.
set -eu

# Staged C sources/headers only.
files=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.[ch]$' || true)
[ -z "$files" ] && { echo "format-staged: no staged C files"; exit 0; }

tmp=$(mktemp)
trap 'rm -f "$tmp"' EXIT
n=0
for f in $files; do
	# Record whether the working tree has unstaged changes BEFORE we touch the index.
	if git diff --quiet -- "$f"; then clean_worktree=1; else clean_worktree=0; fi

	git show ":$f" | clang-format --assume-filename="$f" >"$tmp"
	if git show ":$f" | cmp -s - "$tmp"; then
		continue                                   # already formatted
	fi

	blob=$(git hash-object -w "$tmp")
	mode=$(git ls-files --stage -- "$f" | cut -d' ' -f1)
	git update-index --cacheinfo "$mode,$blob,$f"
	[ "$clean_worktree" = 1 ] && cp "$tmp" "$f"     # keep tree in sync only when safe
	echo "  formatted (staged): $f"
	n=$((n + 1))
done
echo "format-staged: reformatted $n staged file(s)"
