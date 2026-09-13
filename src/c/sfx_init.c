#include "memory.h"
#include "sfx.h"
#include "variables.h"

// Low 24 bits of a RAM address: its offset from the region base.
#define RAM_OFFSET_MASK 0x00ffffffu

/**
 * Re-copy the IWRAM and EWRAM statics from their ROM images, as crt0 does at reset, then
 * bring up the sound driver. Runs once from rotk_main, after memory_init and the boot
 * allocations.
 *
 * @romaddress 0x08047064
 */
void sfx_init(void)
{
    g_UnknownMusic_0300765c = 8;
    memory_memcpy(g_IwramStaticsStart, IwramStaticsImage, (u32)g_IwramStaticsEnd & RAM_OFFSET_MASK);
    // BUG (assumed): g_EwramStaticsEnd lies 0x1b8 bytes inside g_HeapArena, so this copy
    // overwrites the first heap block's header and the start of its payload with the
    // image tail (zeros in the stock ROM). The header then reads as a free block on no
    // free list. Not yet understood: what those 0x1b8 bytes are meant to hold, and whether
    // anything relies on the re-zeroing.
    memory_memcpy(g_EwramStaticsStart, EwramStaticsImage, (u32)g_EwramStaticsEnd & RAM_OFFSET_MASK);
    sfx_initDriver();
}
