#include "memory.h"
#include "gba_io.h"
#include "variables.h"

// Allocation granularity.
#define HEAP_ALIGN sizeof(HeapBlock)

// Round an allocation request up to the granularity.
#define HEAP_ALIGN_UP(size) (((size) + HEAP_ALIGN - 1) & ~(HEAP_ALIGN - 1))

// HeapBlock.freePrev value marking the block as handed out.
#define HEAP_BLOCK_ALLOCATED ((HeapBlock *)-1)

// Payload of a block, and the block behind a payload pointer.
#define HEAP_PAYLOAD(block) ((u8 *)((block) + 1))
#define HEAP_BLOCK_OF(payload) ((HeapBlock *)(payload) - 1)

// Byte offset of a pointer from its word boundary, and the alignment test built on it.
#define WORD_OFFSET(p) ((u32)(p) & 3)
#define IS_UNALIGNED(p) (WORD_OFFSET(p) != 0)

static u32 usableSize(HeapBlock *block, u32 heapIndex);
static void linkBlock(HeapBlock *block, HeapBlock *newBlock);
static void linkFreeBlock(HeapBlock *block, HeapBlock *newBlock);
static void pushFreeBlock(HeapBlock *block, u32 heapIndex);
static void unlinkBlock(HeapBlock *block, u32 heapIndex);
static void unlinkFreeBlock(HeapBlock *block, u32 heapIndex);
static void mergeIntoPrevBlock(HeapBlock *block, u32 heapIndex);
static void mergeNextBlock(HeapBlock *block, u32 heapIndex);

// Ascending copy, useful for non-overlapping buffers.
static inline void copyForward(void *dest, const void *src, size_t n)
{
    const u8 *s = src;
    u8 *d = dest;
    u32 i;

    // byte by byte when either pointer is unaligned
    if (IS_UNALIGNED(d) || IS_UNALIGNED(s))
    {
        for (i = 0; i < n; i++)
        {
            d[i] = s[i];
        }
    }
    // whole words, then the sub-word tail
    else
    {
        {
            u32 wordCount = n / 4;
            u32 *dWords = (u32 *)d;
            const u32 *sWords = (const u32 *)s;

            for (i = 0; i < wordCount; i++)
            {
                dWords[i] = sWords[i];
            }
        }

        {
            s32 tailBytes = n & 3;

            i = n - tailBytes;
            while (tailBytes-- != 0)
            {
                d[i] = s[i];
                i++;
            }
        }
    }
}

// Descending copy for overlapping buffers.
static inline void copyBackward(void *dest, const void *src, size_t n)
{
    const u8 *s = src;
    u8 *d = dest;

    // byte by byte when either pointer is unaligned
    if (IS_UNALIGNED(d) || IS_UNALIGNED(s))
    {
        s32 i;

        for (i = n - 1; i >= 0; i--)
        {
            d[i] = s[i];
        }
    }
    // the sub-word tail from the top, then whole words
    else
    {
        const u32 *sWords;
        u32 *dWords;
        u32 wordCount;
        s32 i;
        s32 j;
        s32 tailBytes;

        tailBytes = n & 3;
        i = n - 1;
        while (tailBytes-- != 0)
        {
            d[i] = s[i];
            i--;
        }

#ifdef BUGFIX // the words start at the buffer base
        sWords = src;
        dWords = dest;
#else
        // BUG: tailBytes has run down to -1, so both word pointers sit one byte low. The
        // ARM7TDMI rounds unaligned addresses down and rotates the loaded word, so every
        // word is read one word below its source, rotated right by 24 bits, and stored one
        // word below its target; the topmost target word is never written.
        sWords = (const u32 *)((const u8 *)src + tailBytes);
        dWords = (u32 *)((u8 *)dest + tailBytes);
#endif

        wordCount = n / 4;
        for (j = wordCount - 1; j >= 0; j--)
        {
            dWords[j] = sWords[j];
        }
    }
}

/**
 * Copy n bytes from src to dest, copying backwards when dest lies inside [src, src + n].
 *
 * BUG: the backward path corrupts the copy whenever both pointers are word aligned; only
 * the unaligned byte path copies correctly.
 *
 * @return dest
 *
 * @romaddress 0x08032aa8
 */
void *memory_memmove(void *dest, const void *src, size_t n)
{
    const u8 *s = src;
    u8 *d = dest;

    if (s + n < d || s > d)
    {
        copyForward(dest, src, n);
        return dest;
    }
    else
    {
        copyBackward(dest, src, n);
        return dest;
    }
}

/**
 * Register the single game heap over the EWRAM tail.
 *
 * @romaddress 0x08032ba0
 */
void memory_init(void)
{
    memory_addHeap(0, g_HeapArena, EWRAM_BASE + EWRAM_SIZE);
}

// First fit over every heap's free list, splitting the block when it has room to spare.
static inline void *allocBlock(u32 size)
{
    u32 heapIndex;
    HeapBlock *block;
    u32 usable;

    size = HEAP_ALIGN_UP(size);

    for (heapIndex = 0; heapIndex < g_HeapCount; heapIndex++)
    {
        for (block = g_Heaps[heapIndex].freeList; block != NULL; block = block->link.freeNext)
        {
            usable = usableSize(block, heapIndex);
            if (usable >= size)
            {
                goto found;
            }
        }
        // no block in this heap had room, try the next one
        continue;

    found:
        // split off the surplus as a free block
        if (usable > size)
        {
            HeapBlock *tail = (HeapBlock *)(HEAP_PAYLOAD(block) + size);

            linkBlock(block, tail);
            linkFreeBlock(block, tail);
        }
        unlinkFreeBlock(block, heapIndex);
        block->link.heapIndex = heapIndex;
        block->freePrev = HEAP_BLOCK_ALLOCATED;
        return HEAP_PAYLOAD(block);
    }
    return NULL;
}

/**
 * Allocate size bytes from the game heap, uninitialized.
 *
 * @return the block, or NULL when no heap has room
 *
 * @romaddress 0x08032bb8
 */
void *memory_malloc(size_t size)
{
    return allocBlock(size);
}

/**
 * memory_malloc plus a zero fill.
 *
 * @romaddress 0x08032c38
 */
void *memory_zalloc(size_t size)
{
    void *ptr;

    // Rounded here as well so the fill covers the whole block.
    size = HEAP_ALIGN_UP(size);
    ptr = allocBlock(size);
    if (ptr != NULL)
    {
        memory_memset(ptr, 0, size);
    }
    return ptr;
}

/**
 * Free a block returned by memory_malloc or memory_zalloc.
 *
 * @romaddress 0x08032cdc
 */
void memory_free(void *ptr)
{
    HeapBlock *block = HEAP_BLOCK_OF(ptr);
    u32 heapIndex = block->link.heapIndex;

    pushFreeBlock(block, heapIndex);
    mergeNextBlock(block, heapIndex);
    mergeIntoPrevBlock(block, heapIndex);
}

/**
 * Resize a block in place, splitting off or absorbing the following block as needed.
 *
 * @return ptr, or NULL when the block cannot grow in place
 *
 * @romaddress 0x08032d04
 */
void *memory_realloc(void *ptr, size_t size)
{
    u8 *payload = ptr;
    HeapBlock *block = HEAP_BLOCK_OF(ptr);
    u32 heapIndex = block->link.heapIndex;
    u32 usable;

    size = HEAP_ALIGN_UP(size);
    usable = usableSize(block, heapIndex);

    // shrink: the surplus becomes a free block behind this one
    if (size < usable)
    {
        HeapBlock *tail = (HeapBlock *)(payload + size);

        linkBlock(block, tail);
        pushFreeBlock(tail, heapIndex);
        mergeNextBlock(tail, heapIndex);
    }
    // grow into next when it is free, any surplus staying free in its place
    else if (size > usable)
    {
        // BUG: growing the topmost block of a heap dereferences a NULL next.
        HeapBlock *next = block->next;
        // Absorbing next also reclaims its header.
        u32 gain = usableSize(next, heapIndex) + sizeof(HeapBlock);
        u32 combined = usable + gain;

        if (next->freePrev == HEAP_BLOCK_ALLOCATED || size > combined)
        {
            return NULL;
        }
        if (size < combined)
        {
            HeapBlock *tail = (HeapBlock *)(payload + size);

            linkBlock(next, tail);
            linkFreeBlock(next, tail);
        }
        unlinkFreeBlock(next, heapIndex);
        unlinkBlock(next, heapIndex);
    }
    return ptr;
}

/**
 * Fill the first count bytes of dest with the low byte of value.
 *
 * @return dest
 *
 * @romaddress 0x08032d9c
 */
void *memory_memset(void *dest, u32 value, size_t count)
{
    u8 *d = dest;
    u32 i;
    u32 headBytes = 4 - WORD_OFFSET(d);
    u32 wordCount;
    u32 fill;
    u32 *dWords;
    u32 tailBytes;

    // Bytes up to the next word boundary, then whole words, then the sub-word tail.
    if (headBytes != 4)
    {
        if (count < headBytes)
        {
            headBytes = count;
        }
        for (i = 0; i < headBytes; i++)
        {
            d[i] = value;
        }
        count -= i;
        d += i;
    }

    wordCount = count / 4;
    fill = (value << 8) | value;
    fill |= fill << 16;
    dWords = (u32 *)d;
    for (i = 0; i < wordCount; i++)
    {
        dWords[i] = fill;
    }
    d += i * 4;

    tailBytes = count & 3;
    for (i = 0; i < tailBytes; i++)
    {
        d[i] = value;
    }

    return dest;
}

/**
 * Copy n bytes from src to dest. Undefined for overlapping buffers, use memory_memmove.
 *
 * @return dest
 *
 * @romaddress 0x08032e08
 */
void *memory_memcpy(void *dest, const void *src, size_t n)
{
    copyForward(dest, src, n);
    return dest;
}

/**
 * Always returns TRUE.
 *
 * @romaddress 0x08032e70
 */
bool memory_stubReturnTrue(void)
{
    return TRUE;
}

/**
 * Register [base, end) as heap heapIndex: one free block over the region past a leading
 * HEAP_ALIGN gap.
 *
 * @romaddress 0x08032e74
 */
void memory_addHeap(u32 heapIndex, u8 *base, u8 *end)
{
    HeapBlock *firstBlock = (HeapBlock *)(base + HEAP_ALIGN);
    u32 size = end - base - HEAP_ALIGN;
    HeapBlock *block;

    g_Heaps[heapIndex].firstBlock = firstBlock;
    g_Heaps[heapIndex].freeList = firstBlock;
    g_Heaps[heapIndex].size = size;
    g_Heaps[heapIndex].end = end;

    block = g_Heaps[heapIndex].firstBlock;
    block->next = NULL;
    block->prev = NULL;
    block->link.freeNext = NULL;
    block->freePrev = NULL;

    g_HeapCount++;
}

/**
 * Payload bytes of a block: the gap up to the next block or the end of the heap.
 *
 * @romaddress 0x08032ebc
 */
static u32 usableSize(HeapBlock *block, u32 heapIndex)
{
    u8 *end = (u8 *)block->next;

    if (end == NULL)
    {
        end = g_Heaps[heapIndex].end;
    }

    return end - (u8 *)block - sizeof(HeapBlock);
}

/**
 * Insert newBlock behind block in the address-ordered list.
 *
 * @romaddress 0x08032edc
 */
static void linkBlock(HeapBlock *block, HeapBlock *newBlock)
{
    HeapBlock *next;

    newBlock->prev = block;
    next = block->next;
    newBlock->next = next;
    block->next = newBlock;
    if (next != NULL)
    {
        next->prev = newBlock;
    }
}

/**
 * Insert newBlock behind block in the free list.
 *
 * @romaddress 0x08032ef0
 */
static void linkFreeBlock(HeapBlock *block, HeapBlock *newBlock)
{
    HeapBlock *freeNext;

    newBlock->freePrev = block;
    freeNext = block->link.freeNext;
    newBlock->link.freeNext = freeNext;
    block->link.freeNext = newBlock;
    if (freeNext != NULL)
    {
        freeNext->freePrev = newBlock;
    }
}

/**
 * Push block onto the head of the heap's free list.
 *
 * @romaddress 0x08032f04
 */
static void pushFreeBlock(HeapBlock *block, u32 heapIndex)
{
    HeapBlock *head;

    block->freePrev = NULL;
    head = g_Heaps[heapIndex].freeList;
    block->link.freeNext = head;
    g_Heaps[heapIndex].freeList = block;
    if (head != NULL)
    {
        head->freePrev = block;
    }
}

/**
 * Remove block from the address-ordered list.
 *
 * @romaddress 0x08032f28
 */
static void unlinkBlock(HeapBlock *block, u32 heapIndex)
{
    if (block->prev != NULL)
    {
        block->prev->next = block->next;
    }
    else
    {
        g_Heaps[heapIndex].firstBlock = block->next;
    }

    if (block->next != NULL)
    {
        block->next->prev = block->prev;
    }
}

/**
 * Remove block from the free list.
 *
 * @romaddress 0x08032f54
 */
static void unlinkFreeBlock(HeapBlock *block, u32 heapIndex)
{
    if (block->freePrev != NULL)
    {
        block->freePrev->link.freeNext = block->link.freeNext;
    }
    else
    {
        g_Heaps[heapIndex].freeList = block->link.freeNext;
    }

    if (block->link.freeNext != NULL)
    {
        block->link.freeNext->freePrev = block->freePrev;
    }
}

/**
 * Fold block into the preceding block when that one is free.
 *
 * Expects block to head the heap's free list.
 *
 * @romaddress 0x08032f84
 */
static void mergeIntoPrevBlock(HeapBlock *block, u32 heapIndex)
{
    HeapBlock *prev = block->prev;

    if (prev != NULL && prev->freePrev != HEAP_BLOCK_ALLOCATED)
    {
        HeapBlock *head = g_Heaps[heapIndex].freeList;
        HeapBlock *freeNext = block->link.freeNext;

        if (head == block)
        {
            g_Heaps[heapIndex].freeList = freeNext;
        }
        // prev is free, so it sits on the list behind block and freeNext is non-NULL.
        freeNext->freePrev = NULL;

        prev->next = block->next;
        if (block->next != NULL)
        {
            block->next->prev = prev;
        }
    }
}

/**
 * Fold the following block into block when that one is free.
 *
 * Expects block to head the heap's free list, so the absorbed block is never the head.
 *
 * @romaddress 0x08032fc8
 */
static void mergeNextBlock(HeapBlock *block, u32 heapIndex __attribute__((unused)))
{
    HeapBlock *next = block->next;

    if (next != NULL && next->freePrev != HEAP_BLOCK_ALLOCATED)
    {
        if (next->freePrev != NULL)
        {
            next->freePrev->link.freeNext = next->link.freeNext;
        }
        if (next->link.freeNext != NULL)
        {
            next->link.freeNext->freePrev = next->freePrev;
        }

        block->next = next->next;
        if (next->next != NULL)
        {
            next->next->prev = block;
        }
    }
}
