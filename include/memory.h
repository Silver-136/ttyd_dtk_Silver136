// memory.c: the heaps (HEAP_TYPE, __memAlloc), the map heap (_mapAlloc) and the smart heap for loaded files.

#pragma once

#include "version.h"
#include <dolphin/gx.h>
#include "manager/filemgr.h"

// Heap indexes for __memAlloc/__memFree (sizes in memory.c size_table). JP and the early demos have a fixed battle
// heap at index 4; US and later have none (battle memory comes from a map sub-heap, see BattleAlloc), so the last
// heap moves from 5 to 4.
typedef enum HEAP_TYPE {
    HEAP_DEFAULT = 0,
    HEAP_MALLOC = 1, // memInit/memClear turn it into the map heap (_mapAlloc)
    HEAP_EXT = 2,
    HEAP_EFFECT = 3,
#if VERSION >= VERSION_US
    HEAP_SMART = 4, // the smart heap for loaded files (smartAlloc)
    HEAP_COUNT = 5
#else
    HEAP_BATTLE = 4,
    HEAP_SMART = 5, // the smart heap for loaded files (smartAlloc); the early demos leave it unused and take the
                    // smart heap from HEAP_DEFAULT instead
    HEAP_COUNT = 6
#endif
} HEAP_TYPE;

// The heap smartAlloc's block comes from.
#if VERSION <= VERSION_DEMO_JP
#define SMART_ALLOC_HEAP HEAP_DEFAULT
#else
#define SMART_ALLOC_HEAP HEAP_SMART
#endif

typedef struct smartEntry {
    void* address; // 0x0
    u32 size; // 0x4
    FileEntry* file; // 0x8, file whose data lives in this block (smartGarbage checks its state)
    u16 inUse; // 0xC, 1 from smartAlloc, 0 when freed; written only
    u8 type; // 0xE, allocation type passed to smartAlloc
    u8 pad_F; // 0xF
    u32 spaceAfter; // 0x10, free bytes after this block (set from bytesLeft on allocation; prev->spaceAfter += size on free)
    struct smartEntry* next; // 0x14
    struct smartEntry* prev; // 0x18
} smartEntry;

typedef struct SmartWork {
    void* ptr; // 0x0
    smartEntry work[0x800]; // 0x4-0xE004
    u32 bytesLeft; // 0xE004
    smartEntry* allocHead; // 0xE008, list of allocated blocks (smartAutoFree walks it)
    smartEntry* allocTail; // 0xE00C
    smartEntry* head; // 0xE010
    smartEntry* tail; // 0xE014
    s32 waitsync; // 0xE018
} SmartWork;

void memInit(void);
void memClear(HEAP_TYPE heap);
void* __memAlloc(HEAP_TYPE heap, u32 size);
void __memFree(HEAP_TYPE heap, void* ptr);

#if VERSION >= VERSION_US // US and later: the map heap functions take the heap base; these macros pass mapalloc_base_ptr
extern void* mapalloc_base_ptr;
void* _mapAlloc(void* base, u32 size);
void* _mapAllocTail(void* base, u32 size);
void _mapFree(void* base, void* alloc);
#define _mapAlloc(size) _mapAlloc(mapalloc_base_ptr, size)
#define _mapAllocTail(size) _mapAllocTail(mapalloc_base_ptr, size)
#define _mapFree(alloc) _mapFree(mapalloc_base_ptr, alloc)
#else
void* _mapAlloc(u32 size);
void* _mapAllocTail(u32 size);
void _mapFree(void* alloc);
#endif

void smartInit(void);
void smartReInit(void);

// smartAlloc groups: smartAutoFree frees a whole group.
enum {
    SMART_GROUP_PERMANENT = 0,  // never auto-freed
    SMART_GROUP_MAP = 1,        // seq_mapchange, seq_demo
    SMART_GROUP_BATTLE = 2,     // seq_battle
    SMART_GROUP_TEMP = 3,       // also freed by the first smartAlloc after smartInit
    SMART_GROUP_TEMP_KEEP = 4,  // smartFree demotes it to TEMP instead of freeing
};

void smartAutoFree(s32 allocType);
void smartFree(smartEntry* entry);
smartEntry* smartAlloc(u32 size, u8 type);
GXTexObj* smartTexObj(GXTexObj* obj, void** image_ptr);

extern BOOL g_bFirstSmartAlloc;

#if VERSION >= VERSION_US
extern void* R_battlemapalloc_base_ptr;
#endif

void battleMapAlloc(void);
void battleMapFree(void);
