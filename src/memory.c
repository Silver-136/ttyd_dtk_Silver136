// Memory: the OS heaps, the map heap allocator (_mapAlloc/_mapFree) and the smart heap (smartAlloc) used for loaded files.

#include "version.h"
#include "memory.h"
#include "system.h"
#include <dolphin/os.h>
#include <string.h>
#include "manager/dvdmgr.h"

// HEAP_MALLOC block is a series of these nodes
typedef struct MapAllocEntry MapAllocEntry;
struct MapAllocEntry {
    MapAllocEntry* next; //0x0
    u32 size; //0x4
    u16 inuse; //0x8
    u16 currId; //0xA
    u8 pad_C[0x20 - 0xC]; //0xC
};

//.bss
#if VERSION <= VERSION_DEMO_JP
extern SmartWork smartWork;
#else
SmartWork smartWork;
#endif

//.sdata
// {fixed, size}: a fixed heap (1) has its size in KiB, the others (0) get size percent of the arena left over
s32 size_table[HEAP_COUNT][2] = {
#if VERSION <= VERSION_DEMO_JP
    {1, 0x3784},
#else
    {1, 0x1C84}, // 0x721000, 7300 KiB (7.13 MiB)
#endif
    {1, 0xD90},  // 0x364000, 3472 KiB (3.39 MiB)
    {1, 0xE0},   // 0x38000, 224 KiB
    {1, 0x80},   // 0x20000, 128 KiB
#if VERSION < VERSION_US
    {1, 0x100},  // 0x40000, 256 KiB: the battle heap
#endif
    {0, 0x64}    // the smart heap: 100 % of what is left
};

static SmartWork* wp = &smartWork;

//.sbss
OSHeapHandle heapHandle[HEAP_COUNT];
void* heapStart[HEAP_COUNT];
void* heapEnd[HEAP_COUNT];

#if VERSION >= VERSION_US
// uninitialized .sbss is laid out in reverse definition order
u32 mapalloc_size;
u32 R_battlemapalloc_size;
BOOL g_bFirstSmartAlloc;
void* R_battlemapalloc_base_ptr;
void* mapalloc_base_ptr;
// US and later map heap: callers pass the heap base; battle maps get their own sub-heap
#define MAPBASE_PARAM void* base_,
#define MAPBASE ((MapAllocEntry*)base_)
#else
#if VERSION <= VERSION_DEMO_JP
SmartWork smartWork;
#endif
void* mapalloc_base_ptr;
u32 mapalloc_size;
BOOL g_bFirstSmartAlloc;
#if VERSION <= VERSION_DEMO_JP
static u16 count;
#define COUNT count
#else
u32 count;
#define COUNT (*(u16*)&count)
#endif
#define MAPBASE_PARAM
#define MAPBASE mapalloc_base_ptr
#endif

#if VERSION <= VERSION_DEMO_JP
// early demos: the smart heap is a fixed 7 MiB block taken from the default heap
#define SMART_HEAP_SIZE 0x700000
#else
#define SMART_HEAP_SIZE ((u8*)heapEnd[HEAP_SMART] - (u8*)heapStart[HEAP_SMART] - 32)
#endif

// local prototypes
void smartGarbage(void);

/**
 * Carves the OS arena into the game's heaps from size_table:
 * fixed-size heaps (size in KiB) first, then the rest shared out by percentage (the last heap takes 100%).
 * Creates each OS heap and clears it, which also sets up the map heap allocator. Called once at boot.
 */
void memInit(void) {
    void* arenaLo;
    void* arenaHi;
    u32 restSize;
    int i;
    int size;
    u8* cursor;

    arenaLo = OSGetArenaLo();
    arenaHi = OSGetArenaHi();
    arenaLo = OSInitAlloc(arenaLo, arenaHi, HEAP_COUNT);
    OSSetArenaLo(arenaLo);
    arenaLo = (void*)OSRoundUp32B(arenaLo);
    arenaHi = (void*)OSRoundDown32B(arenaHi);
    cursor = arenaLo;

    for (i = 0; i < HEAP_COUNT; i++) {
        if (size_table[i][0] == 1) {
            heapStart[i] = cursor;
            size = size_table[i][1] << 10;
            heapEnd[i] = cursor + size;
            cursor += size;
        }
    }

    restSize = (u8*)arenaHi - cursor;
    for (i = 0; i < HEAP_COUNT; i++) {
        if (size_table[i][0] == 0) {
            size = (size_table[i][1] * (u64)restSize) / 100Ull;
            heapStart[i] = cursor;
            size -= (size & 0x1F);
            heapEnd[i] = cursor + size;
            cursor += size;
        }
    }
    for (i = 0; i < HEAP_COUNT; i++) {
        heapHandle[i] = OSCreateHeap(heapStart[i], heapEnd[i]);
    }
    OSSetArenaLo(arenaHi);
    for (i = 0; i < HEAP_COUNT; i++) {
        memClear(i);
    }
}

/**
 * Turns the whole HEAP_MALLOC heap into one free map heap block for _mapAlloc.
 * JP and the early demos also reset the allocation id counter.
 */
static inline void mapallocInit(void) {
// the declaration order differs per version (it sets which callee-saved register holds each)
#if VERSION >= VERSION_US
    u32 size;
    MapAllocEntry* entry;
    u32 total;

    size = (u8*)heapEnd[HEAP_MALLOC] - (u8*)heapStart[HEAP_MALLOC] - sizeof(MapAllocEntry);
    mapalloc_size = size;
#else
    MapAllocEntry* entry;
    u32 size;

    size = (u8*)heapEnd[1] - (u8*)heapStart[1] - sizeof(MapAllocEntry);
#endif
    entry = OSAllocFromHeap(heapHandle[HEAP_MALLOC], size);
    if (entry) {
        memset(entry, 0, size);
        DCFlushRange(entry, size);
    }
#if VERSION >= VERSION_US
    total = mapalloc_size;
    mapalloc_base_ptr = entry;
    entry->next = NULL;
    entry->size = total - sizeof(MapAllocEntry);
    entry->inuse = 0;
#else
    entry->next = NULL;
    entry->size = size - sizeof(MapAllocEntry);
    entry->inuse = 0;
    mapalloc_base_ptr = entry;
    mapalloc_size = size;
    COUNT = 0;
#endif
}

/**
 * Throws away everything in a heap by destroying and recreating it;
 * for HEAP_MALLOC also resets the map heap allocator.
 */
void memClear(HEAP_TYPE heap) {
    OSDestroyHeap(heapHandle[heap]);
    OSCreateHeap(heapStart[heap], heapEnd[heap]);
    if (heap == HEAP_MALLOC) {
        mapallocInit();
    }
}

/**
 * Allocates size bytes from one of the game heaps, zeroed and flushed from the data cache;
 * NULL when the heap is full.
 */
void* __memAlloc(HEAP_TYPE heap, u32 size) {
    void* ptr;

    ptr = OSAllocFromHeap(heapHandle[heap], size);
    if (ptr) {
        memset(ptr, 0, size);
        DCFlushRange(ptr, size);
    }
    return ptr;
}

/** Returns a block from __memAlloc to its heap. */
void __memFree(HEAP_TYPE heap, void* ptr) {
    OSFreeToHeap(heapHandle[heap], ptr);
}

#if VERSION >= VERSION_US
/**
 * US and later: carves a 0x64000-byte sub-heap out of the map heap for the battle's own map allocations (battle.c
 * allocates from it). Called when a battle starts (seq_battle); replaces JP's fixed battle heap.
 */
// Descriptive name (not in the symbol maps).
void battleMapAlloc(void) {
    MapAllocEntry* entry;
    u32 size;

    R_battlemapalloc_size = 0x64000;
    entry = (_mapAlloc)(mapalloc_base_ptr, 0x64000);
    size = R_battlemapalloc_size;
    R_battlemapalloc_base_ptr = entry;
    entry->next = NULL;
    entry->size = size - sizeof(MapAllocEntry);
    entry->inuse = 0;
}

/** US and later: returns the battle map sub-heap to the map heap when the battle ends (seq_battle). */
// Descriptive name (not in the symbol maps).
void battleMapFree(void) {
    (_mapFree)(mapalloc_base_ptr, R_battlemapalloc_base_ptr);
    R_battlemapalloc_base_ptr = NULL;
}
#endif

void* (_mapAlloc)(MAPBASE_PARAM u32 size) {
    MapAllocEntry* restEntry;
    MapAllocEntry* entry;
    MapAllocEntry* oldNext; // next of the chosen block before the split
    MapAllocEntry* bestEntry; // smallest free block that fits
    void* usableData; // returned to the caller (after the header)
    u32 usableSize; // request rounded up to 32 bytes, without the header
    u32 bestSize; // size of bestEntry, 0 while none fits
    u32 fullSize; // usableSize plus the header

    usableSize = OSRoundUp32B(size);
    if (!usableSize) {
        return NULL;
    }
    entry = MAPBASE;
    fullSize = usableSize + sizeof(MapAllocEntry);
    bestSize = 0;
    oldNext = NULL;
    while (1) {
        if (!entry->inuse && entry->size >= fullSize) {
            if (bestSize > entry->size || !bestSize) {
                oldNext = entry->next;
                bestEntry = entry;
                bestSize = entry->size;
            }
        }
        if (!entry->next) {
            break;
        }
        entry = entry->next;
    }
    if (bestSize) {
        usableData = (void*)(bestEntry + 1);

        bestEntry->next = (void*)((u8*)bestEntry + fullSize);
        bestEntry->size = usableSize;
        bestEntry->inuse = 1;
#if VERSION < VERSION_US
        bestEntry->currId = COUNT++;
#endif

        restEntry = bestEntry->next;
        restEntry->next = oldNext;
        restEntry->size = bestSize - fullSize;
        restEntry->inuse = 0;
#if VERSION < VERSION_US
        restEntry->currId = 0;
#endif

        memset(usableData, 0, usableSize);
        DCFlushRange(usableData, usableSize);
        return usableData;
    } else {
        return NULL;
    }
}

void* (_mapAllocTail)(MAPBASE_PARAM u32 size) {
    MapAllocEntry* usedEntry;
    MapAllocEntry* entry;
    MapAllocEntry* tailEntry; // next of freeEntry before the split
    void* result;
    MapAllocEntry* freeEntry; // last free block that fits; the allocation is cut from its end
    u32 freeSize; // size of freeEntry, 0 while none fits
    u32 fullSize; // usableSize plus the header
    u32 usableSize; // request rounded up to 32 bytes, without the header

    usableSize = OSRoundUp32B(size);
    if (!usableSize) {
        return NULL;
    }
    entry = MAPBASE;
    fullSize = usableSize + sizeof(MapAllocEntry);
    freeSize = 0;
    tailEntry = NULL;
    while (1) {
        if (!entry->inuse && entry->size >= fullSize) {
            tailEntry = entry->next;
            freeEntry = entry;
            freeSize = entry->size;
        }
        if (!entry->next) {
            break;
        }
        entry = entry->next;
    }
    if (freeSize) {
        memset(freeEntry, 0, fullSize);
        DCFlushRange(freeEntry, fullSize);

        freeEntry->next = (MapAllocEntry*)(((u8*)freeEntry + freeSize) - fullSize);
        freeEntry->size = freeSize - fullSize;
        freeEntry->inuse = 0;
#if VERSION < VERSION_US
        freeEntry->currId = 0;
#endif

        usedEntry = freeEntry->next;
        usedEntry->next = tailEntry;
        result = (void*)(usedEntry + 1);
        usedEntry->size = usableSize;
        usedEntry->inuse = 1;
#if VERSION < VERSION_US
        usedEntry->currId = COUNT++;
#endif

        memset(result, 0, usableSize);
        DCFlushRange(result, usableSize);
        return result;
    } else {
        return NULL;
    }
}

#if VERSION >= VERSION_US
#pragma dont_inline on // battleMapFree (US and later) calls it
#endif

void (_mapFree)(MAPBASE_PARAM void* alloc) {
    MapAllocEntry* loopEntry;
    MapAllocEntry* nextEntry;
    u32 newSize;
    MapAllocEntry* entry;

    if (!alloc) {
        return;
    }
    entry = (MapAllocEntry*)alloc - 1;
    if (!entry->inuse) {
        return;
    }
    nextEntry = entry->next;
    newSize = entry->size;
    if (nextEntry && !nextEntry->inuse) {
        newSize += nextEntry->size + sizeof(MapAllocEntry);
        nextEntry = nextEntry->next;
    }
    loopEntry = MAPBASE;
    while (1) {
        if (loopEntry->next == entry) {
            if (!loopEntry->inuse) {
                newSize += loopEntry->size + sizeof(MapAllocEntry);
                entry = loopEntry;
            }
            break;
        }
        if (loopEntry > entry || !loopEntry->next) {
            break;
        }
        loopEntry = loopEntry->next;
    }
    entry->next = nextEntry;
    entry->size = newSize;
    entry->inuse = 0;
}

#if VERSION >= VERSION_US
#pragma dont_inline reset
#endif

/**
 * Allocates a fixed 7 MiB block from the default heap in the early demos, or the dedicated smart heap minus 32 bytes in JP and later,
 * and resets its 0x800 block descriptors: all free, nothing allocated.
 * The first smartAlloc after this also frees group 3.
 */
void smartInit(void) {
    smartEntry* e;
    u32 size;
    void* ptr;
    int i;

    size = SMART_HEAP_SIZE;
    ptr = OSAllocFromHeap(heapHandle[SMART_ALLOC_HEAP], size);
    if (ptr) {
        memset(ptr, 0, size);
        DCFlushRange(ptr, size);
    }
    wp->ptr = ptr;
    wp->allocHead = NULL;
    wp->allocTail = NULL;
    wp->bytesLeft = SMART_HEAP_SIZE;
    memset(wp->ptr, 0, SMART_HEAP_SIZE);
    memset(wp->work, 0, sizeof(wp->work));
    e = wp->work;
    for (i = 0; i < (s32)(sizeof(wp->work) / sizeof(wp->work[0])); i++, e++) {
        e->next = e + 1;
        e->prev = e - 1;
    }
    wp->head = &wp->work[0];
    wp->head->prev = NULL;
    wp->tail = &wp->work[sizeof(wp->work) / sizeof(wp->work[0]) - 1];
    wp->tail->next = NULL;
    wp->waitsync = 0;
    g_bFirstSmartAlloc = FALSE;
}

#if VERSION >= VERSION_JP
/**
 * JP and later: empties the smart heap again without reallocating it.
 * Used by the crash screen to get memory for the ROM font.
 */
void smartReInit(void) {
    smartEntry* e;
    int i;

    wp->allocHead = NULL;
    wp->allocTail = NULL;
    wp->bytesLeft = SMART_HEAP_SIZE;
    memset(wp->ptr, 0, SMART_HEAP_SIZE);
    memset(wp->work, 0, sizeof(wp->work));
    e = wp->work;
    for (i = 0; i < (s32)(sizeof(wp->work) / sizeof(wp->work[0])); i++, e++) {
        e->next = e + 1;
        e->prev = e - 1;
    }
    wp->head = &wp->work[0];
    wp->head->prev = NULL;
    wp->tail = &wp->work[sizeof(wp->work) / sizeof(wp->work[0]) - 1];
    wp->tail->next = NULL;
    g_bFirstSmartAlloc = FALSE;
}
#endif

// kept: the target calls smartAutoFree from smartAlloc; a function this small is auto-inlined otherwise, and
// -inline noauto for the file changes other functions
#pragma dont_inline on
/**
 * Frees the smart heap blocks of group type; group 4 blocks are demoted to group 3 instead of freed.
 * A group 3 request also demotes the remaining group 4 blocks.
 * Called with group 1 on map changes (and by the demo movie), group 2 when a battle starts and ends.
 */
void smartAutoFree(s32 allocType) {
    smartEntry* entry;
    smartEntry* next;

    // FAKE: u16 copy of allocType for the compare; comparing allocType directly changes the object
    u16 type = allocType;

    for (entry = wp->allocHead; entry; entry = next) {
        next = entry->next;
        if (entry->type == type) {
            smartFree(entry);
        }
    }
    if (allocType == 3) {
        for (entry = wp->allocHead; entry; entry = next) {
            next = entry->next;

            // FAKE: no-op (s32) cast; the plain compare changes the object
            if ((s32)entry->type == SMART_GROUP_TEMP_KEEP) {
                smartFree(entry);
            }
        }
    }
}

#pragma dont_inline reset
/**
 * Frees a smart heap block, giving its bytes to the block before it (or back to the start of the heap)
 * and returning the descriptor to the free list.
 * A group 4 block is only demoted to group 3, so it lives until the next group 3 free.
 * The memory is compacted later by smartGarbage.
 */
void smartFree(smartEntry* entry) {
    u32 total;

    if (entry->type == SMART_GROUP_TEMP_KEEP) {
        entry->type = SMART_GROUP_TEMP;
        return;
    }
    if (!entry->prev) {
        wp->allocHead = entry->next;
        if (wp->allocHead) {
            wp->allocHead->prev = NULL;
        }
    } else {
        entry->prev->next = entry->next;
    }
    if (!entry->next) {
        wp->allocTail = entry->prev;
        if (wp->allocTail) {
            wp->allocTail->next = NULL;
        }
    } else {
        entry->next->prev = entry->prev;
    }
    total = entry->size + entry->spaceAfter;
    if (entry->prev) {
        entry->prev->spaceAfter += total;
    } else {
        wp->bytesLeft += total;
    }
    if (!wp->head) {
        wp->head = entry;
        entry->prev = NULL;
    } else {
        wp->tail->next = entry;
        entry->prev = wp->tail;
    }
    entry->inUse = 0;
    entry->next = NULL;
    entry->size = 0;
    entry->spaceAfter = 0;
    entry->file = NULL;
    wp->tail = entry;
    wp->waitsync++;
}

/**
 * Allocates size bytes (rounded to 32) in the smart heap, tagged with the auto-free group type.
 * Tries the free space at the start and after each block;
 * failing that, compacts the heap (smartGarbage),
 * first plainly and then after the file manager drops unused files, and returns NULL if it still does not fit.
 * Blocks can move during compaction, so callers keep the entry, not the address.
 */
smartEntry* smartAlloc(u32 size, u8 type) {
    smartEntry* prevEntry;
    smartEntry* entry;
    int i;
    u32 sz = size;

    if (!g_bFirstSmartAlloc) {
        g_bFirstSmartAlloc = TRUE;
        smartAutoFree(SMART_GROUP_TEMP);
    }

    if (wp->waitsync) {
        sysWaitDrawSync();
        wp->waitsync = 0;
    }

    entry = wp->head;

    if (!entry->prev) {
        wp->head = entry->next;
        if (wp->head) {
            wp->head->prev = NULL;
        }
    } else {
        entry->prev->next = entry->next;
    }

    if (!entry->next) {
        wp->tail = entry->prev;
        if (wp->tail) {
            wp->tail->next = NULL;
        }
    } else {
        entry->next->prev = entry->prev;
    }

    if (sz & 31) {
        sz += 32 - (sz & 31);
    }

    entry->inUse = 1;
    entry->type = type;
    entry->size = sz;
    entry->file = NULL;
    if (wp->bytesLeft >= sz) {
        entry->address = wp->ptr;
        entry->spaceAfter = wp->bytesLeft - sz;
        entry->next = wp->allocHead;
        entry->prev = NULL;
        wp->bytesLeft = 0;
        if (wp->allocHead) {
            wp->allocHead->prev = entry;
        }
        wp->allocHead = entry;
        if (!entry->next) {
            entry->spaceAfter = SMART_HEAP_SIZE + (u8*)wp->ptr - (u8*)entry->address - entry->size;
            wp->allocTail = entry;
        }
        return entry;
    } else {
        for (prevEntry = wp->allocHead; prevEntry; prevEntry = prevEntry->next) {
            if (prevEntry->spaceAfter >= sz) {
                entry->address = (u8*)prevEntry->address + prevEntry->size;
                entry->spaceAfter = prevEntry->spaceAfter - sz;
                entry->next = prevEntry->next;
                entry->prev = prevEntry;
                prevEntry->spaceAfter = 0;
                if (prevEntry->next) {
                    prevEntry->next->prev = entry;
                } else {
                    wp->allocTail = entry;
                }
                prevEntry->next = entry;
                return entry;
            }
        }
        for (i = 0; i < 3; i++) {
            switch (i) {
                case 0:
                    break;
                case 1:
                    _fileGarbage(1);
                    break;
                case 2:
                    _fileGarbage(0);
                    break;
            }
            smartGarbage();

            // BUG: no NULL check: when no block is left in use allocTail is NULL and is read through here (the
            // bytesLeft path above is not retried).
            prevEntry = wp->allocTail;
            if (prevEntry->spaceAfter >= sz) {
                entry->address = (u8*)prevEntry->address + prevEntry->size;
                entry->spaceAfter = prevEntry->spaceAfter - sz;
                entry->next = prevEntry->next;
                entry->prev = prevEntry;
                prevEntry->spaceAfter = 0;
                if (prevEntry->next) {
                    prevEntry->next->prev = entry;
                } else {
                    wp->allocTail = entry;
                }
                prevEntry->next = entry;
                return entry;
            }
        }
        return NULL;
    }
}

/**
 * Compacts the smart heap by moving blocks toward its start and updating the free gaps.
 * File-backed blocks move through fileGarbageMoveMem; other blocks use memmove.
 * A block with file state 3 and a DVD entry is skipped unless called from dvdmgr_thread.
 * Waits for the GPU first, since textures may be in use.
 */
void smartGarbage(void) {
    u8* addr;
    u8* ptr;
    smartEntry* entry;
    smartEntry* prev;

    sysWaitDrawSync();
    if (wp->allocHead) {
        wp->bytesLeft = 0;
    } else {
        wp->bytesLeft = SMART_HEAP_SIZE;
    }
    prev = NULL;
    ptr = wp->ptr;
    for (entry = wp->allocHead; entry; prev = entry, entry = entry->next) {
        addr = entry->address;
        if (addr != ptr) {
            if (entry->file) {
                if (entry->file->state == FILE_STATE_PRELOADED && entry->file->entry && OSGetCurrentThread() != &dvdmgr_thread) {
                    if (prev) {
                        prev->spaceAfter = addr - ptr;
                    } else {
                        wp->bytesLeft = addr - ptr;
                    }
                    ptr = (u8*)entry->address + entry->size;
                    entry->spaceAfter = 0;
                    continue;
                }
                fileGarbageMoveMem(ptr, entry->file);
            } else {
                memmove(ptr, addr, entry->size);
                entry->address = ptr;
            }
        }
        entry->spaceAfter = 0;
        ptr = (u8*)entry->address + entry->size;
    }
    entry = wp->allocTail;
    if (entry) {
        entry->spaceAfter = SMART_HEAP_SIZE + (u8*)wp->ptr - (u8*)entry->address - entry->size;
    }
    DCFlushRange(wp->ptr, SMART_HEAP_SIZE);
}

/**
 * Points a texture object at its smart heap image again (the data may have moved during compaction)
 * and returns it; passing NULL leaves the object unchanged.
 */
GXTexObj* smartTexObj(GXTexObj* obj, void** image_ptr) {
    if (image_ptr) {
        GXInitTexObjData(obj, *image_ptr);
    }
    return obj;
}
