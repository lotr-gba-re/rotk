// Main file for data-table patches. #includes each enabled patch's tables.c so their
// TABLE_ENTRY entries compile into one .tablepatch section; `rotkit hack tablepatch` then splices
// each entry over its live table in the ROM. HACK_<name> are -D defines from the Makefile. To add
// a data-table patch, add a `#if` below (mirrors hack.asm). Compiled only when an enabled
// patch ships a tables.c (HAVE_TABLES), so no anchor is needed.

#if HACK_demo_table_patch
#include "patches/demo_table_patch/tables.c"
#endif
