# ROM Hack - Changelog

## 2026.09.14 - Memory Management Fixes

This version should contain no user-facing changes.

Behind the scenes, the way the ROM hack's functions manage memory was changed.
During that process, I found out that there was a small chance that my earlier way of organizing the ROM and memory might have corrupted base game memory.
However, I'm reasonably sure it only affected scratch buffers and did not cause any issues.
Nonetheless, I wanted to get this version out as it removes this possibility altogether.

## 2026.09.12 - Initial Release

Initial release of the ROM hack.
See [the user documentation](./romhack-user.md) for more information.
