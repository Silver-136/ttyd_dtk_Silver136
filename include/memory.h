#pragma once

#include <dolphin/gx.h>
#include "manager/filemgr.h"

typedef enum HEAP_TYPE {
    HEAP_DEFAULT = 0,
    HEAP_MALLOC = 1, //for memInit/Clear to do its allocation
    HEAP_EXT = 2,
    HEAP_EFFECT = 3,
    HEAP_BATTLE = 4,
    HEAP_UNK5 = 5
} HEAP_TYPE;

typedef struct smartEntry {
    void* address; //0x0
    u32 size; //0x4
    FileEntry* file; //0x8, file whose data lives in this block (smartGarbage checks its state)
    u16 reserved_C; //0xC
    u8 type; //0xE, allocation type passed to smartAlloc
    u8 pad_F; //0xF
    u32 spaceAfter; //0x10, free bytes after this block (heap size - 32 at init; prev->spaceAfter += size on free)
    struct smartEntry* next; //0x14
    struct smartEntry* prev; //0x18
} smartEntry;

typedef struct SmartWork {
    void* ptr; //0x0
    smartEntry work[0x800]; //0x4-0xE004
    u32 bytesLeft; //0xE004
    smartEntry* allocHead; //0xE008, list of allocated blocks (smartAutoFree walks it)
    smartEntry* allocTail; //0xE00C
    smartEntry* head; //0xE010
    smartEntry* tail; //0xE014
    s32 waitsync; //0xE018
} SmartWork;

void memInit(void);
void memClear(HEAP_TYPE heap);
void* __memAlloc(HEAP_TYPE heap, u32 size);
void __memFree(HEAP_TYPE heap, void* ptr);
#ifdef VERSION_G8ME01 // US: the map heap functions take the heap base; these macros pass mapalloc_base_ptr
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
void smartAutoFree(s32 allocType);
void smartFree(smartEntry* entry);
smartEntry* smartAlloc(u32 size, u8 type);
GXTexObj* smartTexObj(GXTexObj* obj, void** image_ptr);

extern BOOL g_bFirstSmartAlloc;

#ifdef VERSION_G8ME01
extern void* R_battlemapalloc_base_ptr;
#endif
void battleMapAlloc(void);
void battleMapFree(void);
