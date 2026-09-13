#pragma once

#include "types.h"

/**
 * Header of one heap block, immediately followed by its payload.
 *
 * Blocks form two doubly-linked lists: next/prev in ascending address order over every block of
 * a heap, and link.freeNext/freePrev over the free ones only.
 */
typedef struct HeapBlock
{
    struct HeapBlock *next; // next block by address, NULL for the topmost one
    struct HeapBlock *prev;
    union {
        struct HeapBlock *freeNext; // while free: next block on the heap's free list
        u32 heapIndex;              // while allocated: index of the owning heap
    } link;
    struct HeapBlock *freePrev; // while free: previous block on the free list; -1 while allocated
} HeapBlock;

/** One registered heap region. */
typedef struct Heap
{
    HeapBlock *firstBlock; // lowest block, head of the address-ordered list
    HeapBlock *freeList;
    u32 size;  // end - firstBlock
    void *end; // one past the last byte of the region
} Heap;

void memory_init(void);
void memory_addHeap(u32 heapIndex, void *base, void *end);
void *memory_malloc(size_t size);
void *memory_zalloc(size_t size);
void memory_free(void *ptr);
void *memory_realloc(void *ptr, size_t size);
void *memory_memset(void *dest, u32 value, size_t count);
void *memory_memcpy(void *dest, const void *src, size_t n);
void *memory_memmove(void *dest, const void *src, size_t n);
bool memory_stubReturnTrue(void);
