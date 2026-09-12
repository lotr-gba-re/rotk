"""Shared library for the repo tooling. Keep this module import-side-effect-free:
rotkit.pyghidra_env must stay the first thing that touches JVM-related environment.

Canonical repo paths (including ROOT) live in rotkit.paths.
"""
