#include "memmap.h"
#include "memory.h"
#include "variables.h"

/**
 * Initialize the game heap, but a bit smaller in order to allow for a romhack .bss section.
 *
 * REPLACES (whole function): memory_init @0x08032ba0.
 */
void modRam_init(void)
{
    memory_addHeap(0, g_HeapArena, (u8 *)MEM_MODCODE_BSS);
}
