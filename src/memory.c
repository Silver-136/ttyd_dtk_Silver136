#include "memory.h"
#include "system.h"
#include <dolphin/os.h>
#include <string.h>
#include "manager/dvdmgr.h"

//HEAP_MALLOC block is a series of these nodes
typedef struct MapAllocEntry MapAllocEntry;
struct MapAllocEntry {
    MapAllocEntry* next; //0x0
    u32 size; //0x4
    u16 inuse; //0x8
    u16 currId; //0xA
    u8 field_0xC[0x20 - 0xC]; //0xC
};

//.bss
SmartWork smartWork;

//.sdata
#ifdef VERSION_G8ME01
// US: no fixed battle heap (battle maps use a map sub-heap); the smart heap is index 4
#define HEAP_COUNT 5
#define SMART_HEAP 4
#else
#define HEAP_COUNT 6
#define SMART_HEAP 5
#endif

// {fixed, size}: a fixed heap (1) has its size in KiB, the others (0) get size percent of the arena left over
s32 size_table[HEAP_COUNT][2] = {
    {1, 0x1C84}, // 0x721000, 7300 KiB (7.13 MiB)
    {1, 0xD90},  // 0x364000, 3472 KiB (3.39 MiB)
    {1, 0xE0},   // 0x38000, 224 KiB
    {1, 0x80},   // 0x20000, 128 KiB
#ifndef VERSION_G8ME01
    {1, 0x100},  // 0x40000, 256 KiB: the battle heap
#endif
    {0, 0x64}    // the smart heap: 100 % of what is left
};

static SmartWork* wp = &smartWork;

//.sbss
OSHeapHandle heapHandle[HEAP_COUNT];
void* heapStart[HEAP_COUNT];
void* heapEnd[HEAP_COUNT];
#ifdef VERSION_G8ME01
// uninitialized .sbss is laid out in reverse definition order
u32 mapalloc_size;
u32 R_battlemapalloc_size;
BOOL g_bFirstSmartAlloc;
void* R_battlemapalloc_base_ptr;
void* mapalloc_base_ptr;
#else
void* mapalloc_base_ptr;
u32 mapalloc_size;
BOOL g_bFirstSmartAlloc;
#endif
#ifdef VERSION_G8ME01
// US map heap: callers pass the heap base; battle maps get their own sub-heap
#define MAPBASE_PARAM void* base_,
#define MAPBASE ((MapAllocEntry*)base_)
#else
u32 count;
#define COUNT (*(u16*)&count)
#define MAPBASE_PARAM
#define MAPBASE mapalloc_base_ptr
#endif

//local prototypes
void smartGarbage(void);

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

static inline void mapallocInit(void) {
#ifdef VERSION_G8ME01
    u32 size;
    MapAllocEntry* entry;
    u32 total;

    size = (u8*)heapEnd[1] - (u8*)heapStart[1] - sizeof(MapAllocEntry);
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
#ifdef VERSION_G8ME01
    // FAKE: total is a spare copy of mapalloc_size (read before the stores); reading mapalloc_size at its use changes the object
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

void memClear(HEAP_TYPE heap) {
    OSDestroyHeap(heapHandle[heap]);
    OSCreateHeap(heapStart[heap], heapEnd[heap]);
    if (heap == HEAP_MALLOC) {
        mapallocInit();
    }
}

void* __memAlloc(HEAP_TYPE heap, u32 size) {
    void* ptr;

    ptr = OSAllocFromHeap(heapHandle[heap], size);
    if (ptr) {
        memset(ptr, 0, size);
        DCFlushRange(ptr, size);
    }
    return ptr;
}

void __memFree(HEAP_TYPE heap, void* ptr) {
    OSFreeToHeap(heapHandle[heap], ptr);
}

#ifdef VERSION_G8ME01
// Descriptive name (not in the symbol maps).
void battleMapAlloc(void) {
    MapAllocEntry* entry;
    u32 size;

    R_battlemapalloc_size = 0x64000;
    entry = (_mapAlloc)(mapalloc_base_ptr, 0x64000);
    // FAKE: size is a spare copy of R_battlemapalloc_size (read before the stores); reading it at its use changes the object
    size = R_battlemapalloc_size;
    R_battlemapalloc_base_ptr = entry;
    entry->next = NULL;
    entry->size = size - sizeof(MapAllocEntry);
    entry->inuse = 0;
}

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
#ifndef VERSION_G8ME01
        bestEntry->currId = COUNT++;
#endif

        restEntry = bestEntry->next;
        restEntry->next = oldNext;
        restEntry->size = bestSize - fullSize;
        restEntry->inuse = 0;
#ifndef VERSION_G8ME01
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
    MapAllocEntry* tailEntry; // next of freeEntry before the split, then the returned data
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
#ifndef VERSION_G8ME01
        freeEntry->currId = 0;
#endif

        usedEntry = freeEntry->next;
        usedEntry->next = tailEntry;
        // FAKE: tailEntry is reused for the returned data pointer; a separate void* changes the object
        tailEntry = (void*)(usedEntry + 1);
        usedEntry->size = usableSize;
        usedEntry->inuse = 1;
#ifndef VERSION_G8ME01
        usedEntry->currId = COUNT++;
#endif

        memset(tailEntry, 0, usableSize);
        DCFlushRange(tailEntry, usableSize);
        return tailEntry;
    } else {
        return NULL;
    }
}

#ifdef VERSION_G8ME01
#pragma dont_inline on // US battleMapFree calls it
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
#ifdef VERSION_G8ME01
#pragma dont_inline reset
#endif

void smartInit(void) {
    smartEntry* e;
    u32 size;
    void* ptr;
    int i;

    size = (u8*)heapEnd[SMART_HEAP] - (u8*)heapStart[SMART_HEAP] - 32;
    ptr = OSAllocFromHeap(heapHandle[SMART_HEAP], size);
    if (ptr) {
        memset(ptr, 0, size);
        DCFlushRange(ptr, size);
    }
    wp->ptr = ptr;
    wp->allocHead = NULL;
    wp->allocTail = NULL;
    wp->bytesLeft = (u8*)heapEnd[SMART_HEAP] - (u8*)heapStart[SMART_HEAP] - 32;
    memset(wp->ptr, 0, (u8*)heapEnd[SMART_HEAP] - (u8*)heapStart[SMART_HEAP] - 32);
    memset(wp->work, 0, sizeof(wp->work));
    e = wp->work;
    for (i = 0; i < 0x800; i++, e++) {
        e->next = e + 1;
        e->prev = e - 1;
    }
    wp->head = &wp->work[0];
    wp->head->prev = NULL;
    wp->tail = &wp->work[0x7FF];
    wp->tail->next = NULL;
    wp->waitsync = 0;
    g_bFirstSmartAlloc = FALSE;
}

void smartReInit(void) {
    smartEntry* e;
    int i;

    wp->allocHead = NULL;
    wp->allocTail = NULL;
    wp->bytesLeft = (u8*)heapEnd[SMART_HEAP] - (u8*)heapStart[SMART_HEAP] - 32;
    memset(wp->ptr, 0, (u8*)heapEnd[SMART_HEAP] - (u8*)heapStart[SMART_HEAP] - 32);
    memset(wp->work, 0, sizeof(wp->work));
    e = wp->work;
    for (i = 0; i < 0x800; i++, e++) {
        e->next = e + 1;
        e->prev = e - 1;
    }
    wp->head = &wp->work[0];
    wp->head->prev = NULL;
    wp->tail = &wp->work[0x7FF];
    wp->tail->next = NULL;
    g_bFirstSmartAlloc = FALSE;
}

#pragma dont_inline on
void smartAutoFree(s32 allocType) {
    smartEntry* entry;
    smartEntry* next;
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
            if ((s32)entry->type == 4) {
                smartFree(entry);
            }
        }
    }
}

#pragma dont_inline reset
void smartFree(smartEntry* entry) {
    u32 total;

    if (entry->type == 4) {
        entry->type = 3;
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
    entry->reserved_C = 0;
    entry->next = NULL;
    entry->size = 0;
    entry->spaceAfter = 0;
    entry->file = NULL;
    wp->tail = entry;
    wp->waitsync++;
}

smartEntry* smartAlloc(u32 size, u8 type) {
    smartEntry* prevEntry;
    smartEntry* entry;
    int i;
    u32 sz = size;

    if (!g_bFirstSmartAlloc) {
        g_bFirstSmartAlloc = TRUE;
        smartAutoFree(3);
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

    entry->reserved_C = 1;
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
            entry->spaceAfter = ((u8*)heapEnd[SMART_HEAP] - (u8*)heapStart[SMART_HEAP] - 32) + (u8*)wp->ptr - (u8*)entry->address - entry->size;
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

void smartGarbage(void) {
    u8* addr;
    u8* ptr;
    smartEntry* entry;
    smartEntry* prev;

    sysWaitDrawSync();
    if (wp->allocHead) {
        wp->bytesLeft = 0;
    } else {
        wp->bytesLeft = (u8*)heapEnd[SMART_HEAP] - (u8*)heapStart[SMART_HEAP] - 32;
    }
    prev = NULL;
    ptr = wp->ptr;
    for (entry = wp->allocHead; entry; prev = entry, entry = entry->next) {
        addr = entry->address;
        if (addr != ptr) {
            if (entry->file) {
                if (entry->file->state == 3 && entry->file->entry && OSGetCurrentThread() != &dvdmgr_thread) {
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
        entry->spaceAfter = ((u8*)heapEnd[SMART_HEAP] - (u8*)heapStart[SMART_HEAP] - 32) + (u8*)wp->ptr - (u8*)entry->address - entry->size;
    }
    DCFlushRange(wp->ptr, (u8*)heapEnd[SMART_HEAP] - (u8*)heapStart[SMART_HEAP] - 32);
}

GXTexObj* smartTexObj(GXTexObj* obj, void** image_ptr) {
    if (image_ptr) {
        GXInitTexObjData(obj, *image_ptr);
    }
    return obj;
}
