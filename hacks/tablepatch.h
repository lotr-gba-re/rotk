// Patch author-facing macro for data-table patches.
//
// Define one changed table entry with a normal typed initializer; the build extracts its bytes
// and splices them at <Table> + index*sizeof(entry) in the ROM (rotkit hack tablepatch). Only the
// entries you write are emitted - every other entry stays as base ROM.
//
//     TABLE_ENTRY(ItemAffix, CloakPrefixes, 0) = { .nameId = 0x04f4, ... };
//
// `type` is the array element type (from its include/ header), `table` names a config/data.cfg
// data symbol, `index` is the array index.
#pragma once

/**
 * Emits `const <type> __tp_<table>__<index>` in an isolated section the extractor reads by name.
 */
#define TABLE_ENTRY(type, table, index)                                                            \
    __attribute__((section(".tablepatch"), used)) const type __tp_##table##__##index
