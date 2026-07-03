#include "memory.h"

#include <dolphin/os.h>

extern void* mapalloc_base_ptr;
extern void* R_battlemapalloc_base_ptr;

void _mapFree(void* base, void* ptr);
void GXInitTexObjData(void* texObj, void* data);
void OSFreeToHeap(OSHeapHandle heap, void* ptr);
void* OSAllocFromHeap(OSHeapHandle heap, u32 size);
void DCFlushRange(void* ptr, u32 size);
void* memset(void* ptr, int value, u32 size);

//.sdata
s32 size_table[6][2] = {
	{1, 0x1C84}, //0x721000, 7300 KiB/7.13 MiB
	{1, 0xD90}, //0x364000, 3472 KiB/3.39 MiB
	{1, 0xE0}, //0x38000, 224 KiB
	{1, 0x80}, //0x20000, 128 KiB
	{0, 0x64}
};

//.sbss
u32 mapalloc_size;
void* heapEnd[5];
void* heapStart[5];
OSHeapHandle heapHandle[5];

#pragma optimize_for_size off
#pragma optimize_for_size off
void memInit(void) {
    extern void* OSGetArenaLo(void);
    extern void* OSGetArenaHi(void);
    extern void* OSInitAlloc(void* lo, void* hi, s32 count);
    extern void OSSetArenaLo(void* lo);
    extern OSHeapHandle OSCreateHeap(void* start, void* end);
    extern void OSDestroyHeap(OSHeapHandle heap);
    u32 lo;
    u32 hi;
    u32 alloc;
    u32 address;
    u32 arenaEnd;
    u32 remaining;
    u32 chunk;
    s32 i;
    s32 (*size)[2];
    void** start;
    void** end;
    OSHeapHandle* handle;
    void* ptr;

    lo = (u32)OSGetArenaLo();
    hi = (u32)OSGetArenaHi();
    alloc = (u32)OSInitAlloc((void*)lo, (void*)hi, 5);
    OSSetArenaLo((void*)alloc);

    size = size_table;
    address = (alloc + 0x1F) & ~0x1F;
    arenaEnd = hi & ~0x1F;
    start = heapStart;
    end = heapEnd;

    if (size_table[0][0] == 1) {
        heapStart[0] = (void*)address;
        address += size_table[0][1] << 10;
        heapEnd[0] = (void*)address;
    }
    if (size_table[1][0] == 1) {
        heapStart[1] = (void*)address;
        address += size_table[1][1] << 10;
        heapEnd[1] = (void*)address;
    }
    if (size_table[2][0] == 1) {
        heapStart[2] = (void*)address;
        address += size_table[2][1] << 10;
        heapEnd[2] = (void*)address;
    }
    if (size_table[3][0] == 1) {
        heapStart[3] = (void*)address;
        address += size_table[3][1] << 10;
        heapEnd[3] = (void*)address;
    }
    if (size_table[4][0] == 1) {
        heapStart[4] = (void*)address;
        address += size_table[4][1] << 10;
        heapEnd[4] = (void*)address;
    }

    remaining = arenaEnd - address;
    for (i = 0; i < 5; i++, size++, start++, end++) {
        if ((*size)[0] == 0) {
            chunk = (u32)(((u64)remaining * (u32)(*size)[1]) / 100);
            chunk -= chunk & 0x1F;
            *start = (void*)address;
            *end = (void*)(address + chunk);
            address += chunk;
        }
    }

    handle = heapHandle;
    start = heapStart;
    end = heapEnd;
    for (i = 0; i < 5; i++, start++, end++, handle++) {
        *handle = OSCreateHeap(*start, *end);
    }

    OSSetArenaLo((void*)arenaEnd);

    handle = heapHandle;
    start = heapStart;
    end = heapEnd;
    for (i = 0; i < 5; i++, start++, end++, handle++) {
        OSDestroyHeap(*handle);
        OSCreateHeap(*start, *end);
        if (i == 1) {
            mapalloc_size = (u32)heapEnd[1] - (u32)heapStart[1] - 0x20;
            ptr = OSAllocFromHeap(heapHandle[1], mapalloc_size);
            if (ptr != 0) {
                memset(ptr, 0, mapalloc_size);
                DCFlushRange(ptr, mapalloc_size);
            }
            mapalloc_base_ptr = ptr;
            *(u32*)ptr = 0;
            *(u32*)((u32)ptr + 4) = mapalloc_size - 0x20;
            *(u16*)((u32)ptr + 8) = 0;
        }
    }
}

void __memFree(s32 heap, void* ptr) {
    OSFreeToHeap(heapHandle[heap], ptr);
}

void N_battleMapFree(void) {
    _mapFree(mapalloc_base_ptr, R_battlemapalloc_base_ptr);
    R_battlemapalloc_base_ptr = 0;
}

void* smartTexObj(void* texObj, void** data) {
    if (data != 0) {
        GXInitTexObjData(texObj, *data);
    }
    return texObj;
}


void* smartAlloc(u32 param_1, u8 param_2) {
    return 0;
}


void smartInit(void) {
    extern u32* wp;
    extern s32 g_bFirstSmartAlloc;
    u32 size;
    u32* alloc;
    u32* p;
    s32 i;

    size = (u32)heapEnd[4] - (u32)heapStart[4] - 0x20;
    alloc = OSAllocFromHeap(heapHandle[4], size);
    if (alloc != 0) {
        memset(alloc, 0, size);
        DCFlushRange(alloc, size);
    }
    wp[0] = (u32)alloc;
    wp[0x3802] = 0;
    wp[0x3803] = 0;
    wp[0x3801] = (u32)heapEnd[4] - (u32)heapStart[4] - 0x20;
    memset((void*)wp[0], 0, (u32)heapEnd[4] - (u32)heapStart[4] - 0x20);
    memset(wp + 1, 0, 0xE000);
    p = wp + 1;
    i = 0x100;
    do {
        p[5] = (u32)(p + 7);
        p[6] = (u32)(p - 7);
        p[0xC] = (u32)(p + 0xE);
        p[0xD] = (u32)p;
        p[0x13] = (u32)(p + 0x15);
        p[0x14] = (u32)(p + 7);
        p[0x1A] = (u32)(p + 0x1C);
        p[0x1B] = (u32)(p + 0xE);
        p[0x21] = (u32)(p + 0x23);
        p[0x22] = (u32)(p + 0x15);
        p[0x28] = (u32)(p + 0x2A);
        p[0x29] = (u32)(p + 0x1C);
        p[0x2F] = (u32)(p + 0x31);
        p[0x30] = (u32)(p + 0x23);
        p[0x36] = (u32)(p + 0x38);
        p[0x37] = (u32)(p + 0x2A);
        p += 0x38;
        i--;
    } while (i != 0);
    wp[0x3804] = (u32)(wp + 1);
    *(u32*)(wp[0x3804] + 0x18) = 0;
    wp[0x3805] = (u32)(wp + 0x37FA);
    *(u32*)(wp[0x3805] + 0x14) = 0;
    wp[0x3806] = 0;
    g_bFirstSmartAlloc = 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void smartGarbage(void) {
    extern void sysWaitDrawSync(void);
    extern void* OSGetCurrentThread(void);
    extern void* memmove(void* dst, void* src, u32 size);
    extern void fileGarbageMoveMem(void* dst, char* file);
    extern u8 dvdmgr_thread[];
    extern u32* wp;
    void* src;
    void* dst;
    u32* entry;
    u32* prev;

    sysWaitDrawSync();
    if (wp[0x3802] != 0) {
        wp[0x3801] = 0;
    } else {
        wp[0x3801] = (u32)heapEnd[4] - (u32)heapStart[4] - 0x20;
    }

    dst = (void*)wp[0];
    prev = 0;
    entry = (u32*)wp[0x3802];
    while (entry != 0) {
        src = (void*)entry[0];
        if (src == dst) {
            goto moved;
        }
        if (entry[2] != 0) {
            if (*(u8*)entry[2] != 3) {
                fileGarbageMoveMem(dst, (char*)entry[2]);
                goto moved;
            }
            if (*(u32*)(entry[2] + 0xAC) == 0) {
                fileGarbageMoveMem(dst, (char*)entry[2]);
                goto moved;
            }
            if (OSGetCurrentThread() == dvdmgr_thread) {
                fileGarbageMoveMem(dst, (char*)entry[2]);
                goto moved;
            }
            if (prev != 0) {
                prev[4] = (u32)src - (u32)dst;
            } else {
                wp[0x3801] = (u32)src - (u32)dst;
            }
            entry[4] = 0;
            dst = (void*)(entry[0] + entry[1]);
            goto next;
        }
        memmove(dst, src, entry[1]);
        entry[0] = (u32)dst;
moved:
        entry[4] = 0;
        dst = (void*)(entry[0] + entry[1]);
next:
        prev = entry;
        entry = (u32*)entry[5];
    }

    prev = (u32*)wp[0x3803];
    if (prev != 0) {
        prev[4] = (((u32)heapEnd[4] - (u32)heapStart[4]) + wp[0] - 0x20) - prev[0] - prev[1];
    }
    DCFlushRange((void*)wp[0], (u32)heapEnd[4] - (u32)heapStart[4] - 0x20);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void L_smartReInit(void) {
    extern u32* wp;
    extern s32 g_bFirstSmartAlloc;
    extern void* memset(void* ptr, int value, u32 size);
    u32* p;
    s32 i;

    wp[0x3802] = 0;
    wp[0x3803] = 0;
    wp[0x3801] = (u32)heapEnd[4] - (u32)heapStart[4] - 0x20;
    memset((void*)wp[0], 0, (u32)heapEnd[4] - (u32)heapStart[4] - 0x20);
    memset(wp + 1, 0, 0xE000);

    p = wp + 1;
    i = 0x100;
    do {
        p[5] = (u32)(p + 7);
        p[6] = (u32)(p - 7);
        p[0xC] = (u32)(p + 0xE);
        p[0xD] = (u32)p;
        p[0x13] = (u32)(p + 0x15);
        p[0x14] = (u32)(p + 7);
        p[0x1A] = (u32)(p + 0x1C);
        p[0x1B] = (u32)(p + 0xE);
        p[0x21] = (u32)(p + 0x23);
        p[0x22] = (u32)(p + 0x15);
        p[0x28] = (u32)(p + 0x2A);
        p[0x29] = (u32)(p + 0x1C);
        p[0x2F] = (u32)(p + 0x31);
        p[0x30] = (u32)(p + 0x23);
        p[0x36] = (u32)(p + 0x38);
        p[0x37] = (u32)(p + 0x2A);
        p += 0x38;
        i--;
    } while (i != 0);

    wp[0x3804] = (u32)(wp + 1);
    *(u32*)(wp[0x3804] + 0x18) = 0;
    wp[0x3805] = (u32)(wp + 0x37FA);
    *(u32*)(wp[0x3805] + 0x14) = 0;
    g_bFirstSmartAlloc = 0;
}

void smartFree(void* entry) {
    extern u32* wp;
    void* prev;
    void* next;
    u32 size;

    if (*(u8*)((s32)entry + 0x0E) == 4) {
        *(u8*)((s32)entry + 0x0E) = 3;
        return;
    }

    prev = *(void**)((s32)entry + 0x18);
    if (prev == 0) {
        wp[0x3802] = *(u32*)((s32)entry + 0x14);
        prev = (void*)wp[0x3802];
        if (prev != 0) {
            *(u32*)((s32)prev + 0x18) = 0;
        }
    } else {
        *(u32*)((s32)prev + 0x14) = *(u32*)((s32)entry + 0x14);
    }

    next = *(void**)((s32)entry + 0x14);
    if (next == 0) {
        wp[0x3803] = *(u32*)((s32)entry + 0x18);
        next = (void*)wp[0x3803];
        if (next != 0) {
            *(u32*)((s32)next + 0x14) = 0;
        }
    } else {
        *(u32*)((s32)next + 0x18) = *(u32*)((s32)entry + 0x18);
    }

    prev = *(void**)((s32)entry + 0x18);
    size = *(u32*)((s32)entry + 4) + *(u32*)((s32)entry + 0x10);
    if (prev != 0) {
        *(u32*)((s32)prev + 0x10) += size;
    } else {
        wp[0x3801] += size;
    }

    if (wp[0x3804] == 0) {
        wp[0x3804] = (u32)entry;
        *(u32*)((s32)entry + 0x18) = 0;
    } else {
        *(u32*)(wp[0x3805] + 0x14) = (u32)entry;
        *(u32*)((s32)entry + 0x18) = wp[0x3805];
    }

    *(u16*)((s32)entry + 0x0C) = 0;
    *(u32*)((s32)entry + 0x14) = 0;
    *(u32*)((s32)entry + 4) = 0;
    *(u32*)((s32)entry + 0x10) = 0;
    *(u32*)((s32)entry + 8) = 0;
    wp[0x3805] = (u32)entry;
    wp[0x3806]++;
}

void* _mapAllocTail(void* base, u32 size) {
    extern void* memset(void* ptr, int value, u32 size);
    extern void DCFlushRange(void* ptr, u32 size);
    u32 aligned;
    u32 need;
    u32 bestSize;
    void* bestNext;
    void* best;
    void* block;
    void* result;
    u32 remain;

    aligned = (size + 0x1F) & ~0x1F;
    if (aligned == 0) {
        return 0;
    }

    need = aligned + 0x20;
    bestSize = 0;
    bestNext = 0;
    best = 0;

    while (base != 0) {
        if (*(u16*)((s32)base + 8) == 0 && *(u32*)((s32)base + 4) >= need) {
            bestNext = *(void**)base;
            best = base;
            bestSize = *(u32*)((s32)base + 4);
        }
        base = *(void**)base;
    }

    if (bestSize == 0) {
        return 0;
    }

    memset(best, 0, need);
    DCFlushRange(best, need);
    remain = bestSize - need;
    block = (void*)((s32)best + remain);
    *(void**)best = block;
    *(u32*)((s32)best + 4) = remain;
    *(u16*)((s32)best + 8) = 0;
    *(void**)block = bestNext;
    result = (void*)((s32)block + 0x20);
    *(u32*)((s32)block + 4) = aligned;
    *(u16*)((s32)block + 8) = 1;
    memset(result, 0, aligned);
    DCFlushRange(result, aligned);
    return result;
}

void* _mapAlloc(void* base, u32 size) {
    extern void* memset(void* ptr, int value, u32 size);
    extern void DCFlushRange(void* ptr, u32 size);
    u32 aligned;
    u32 need;
    u32 bestSize;
    void* next;
    void* bestNext;
    void* best;
    void* result;
    u32 remain;

    aligned = (size + 0x1F) & ~0x1F;
    if (aligned == 0) {
        return 0;
    }

    need = aligned + 0x20;
    bestSize = 0;
    bestNext = 0;
    best = 0;

    while (base != 0) {
        if (*(u16*)((s32)base + 8) == 0 && *(u32*)((s32)base + 4) >= need) {
            if (bestSize >= *(u32*)((s32)base + 4) || bestSize == 0) {
                bestNext = *(void**)base;
                best = base;
                bestSize = *(u32*)((s32)base + 4);
            }
        }
        base = *(void**)base;
    }

    if (bestSize == 0) {
        return 0;
    }

    *(void**)best = (void*)((s32)best + need);
    result = (void*)((s32)best + 0x20);
    remain = bestSize - need;
    *(u32*)((s32)best + 4) = aligned;
    *(u16*)((s32)best + 8) = 1;
    **(void***)best = bestNext;
    *(u32*)(*(s32*)best + 4) = remain;
    *(u16*)(*(s32*)best + 8) = 0;
    memset(result, 0, aligned);
    DCFlushRange(result, aligned);
    return result;
}

void memClear(s32 heap) {
    u32 size;
    void* ptr;

    OSDestroyHeap(heapHandle[heap]);
    OSCreateHeap(heapStart[heap], heapEnd[heap]);

    if (heap == 1) {
        size = (u32)heapEnd[1] - (u32)heapStart[1] - 0x20;
        mapalloc_size = size;
        ptr = OSAllocFromHeap(heapHandle[1], size);
        if (ptr != NULL) {
            memset(ptr, 0, size);
            DCFlushRange(ptr, size);
        }
        mapalloc_base_ptr = ptr;
        *(s32*)ptr = 0;
        *(u32*)((s32)ptr + 4) = mapalloc_size - 0x20;
        *(u16*)((s32)ptr + 8) = 0;
    }
}


void smartAutoFree(s32 kind) {
    extern u32* wp;
    extern void smartFree(void*);
    void* entry;
    void* next;
    u16 id;

    id = kind;
    entry = (void*)wp[0x3802];
    while (entry != 0) {
        next = *(void**)((s32)entry + 0x14);
        if (*(u8*)((s32)entry + 0x0E) == id) {
            smartFree(entry);
        }
        entry = next;
    }

    if (kind == 3) {
        entry = (void*)wp[0x3802];
        while (entry != 0) {
            next = *(void**)((s32)entry + 0x14);
            if (*(u8*)((s32)entry + 0x0E) == 4) {
                smartFree(entry);
            }
            entry = next;
        }
    }
}

void _mapFree(void* base, void* ptr) {
    void* block;
    void* next;
    u32 size;

    if (ptr == 0) {
        return;
    }

    block = (void*)((s32)ptr - 0x20);
    if (*(u16*)((s32)ptr - 0x18) == 0) {
        return;
    }

    next = *(void**)block;
    size = *(u32*)((s32)block + 4);
    if (next != 0 && *(u16*)((s32)next + 8) == 0) {
        size += *(u32*)((s32)next + 4) + 0x20;
        next = *(void**)next;
    }

    while (1) {
        if (*(void**)base == block) {
            if (*(u16*)((s32)base + 8) == 0) {
                block = base;
                size += *(u32*)((s32)base + 4) + 0x20;
            }
            break;
        }
        if (base > block || *(void**)base == 0) {
            break;
        }
        base = *(void**)base;
    }

    *(void**)block = next;
    *(u32*)((s32)block + 4) = size;
    *(u16*)((s32)block + 8) = 0;
}

void* __memAlloc(s32 heap, u32 size) {
    void* ptr;

    ptr = OSAllocFromHeap(heapHandle[heap], size);
    if (ptr != NULL) {
        memset(ptr, 0, size);
        DCFlushRange(ptr, size);
    }
    return ptr;
}

void N_battleMapAlloc(void) {
    extern void* mapalloc_base_ptr;
    extern void* R_battlemapalloc_base_ptr;
    extern unsigned int R_battlemapalloc_size;
    extern void* _mapAlloc(void*, unsigned int);
    void* ptr;
    unsigned int size;
    unsigned int zero;

    R_battlemapalloc_size = 0x64000;
    ptr = _mapAlloc(mapalloc_base_ptr, R_battlemapalloc_size);
    size = R_battlemapalloc_size;
    zero = 0;
    R_battlemapalloc_base_ptr = ptr;
    *(unsigned int*)ptr = zero;
    *(unsigned int*)((int)ptr + 4) = size - 0x20;
    *(unsigned short*)((int)ptr + 8) = (unsigned short)zero;
}

