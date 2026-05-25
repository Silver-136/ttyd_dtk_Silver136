#include "driver/mobjdrv.h"

typedef struct MobjSet {
    s32 count;
    void* entries;
    u8 pad8[0x14];
} MobjSet;

typedef struct MobjWork {
    MobjSet normal;
    MobjSet alt;
    MobjSet koopa;
} MobjWork;

extern MobjWork work;
extern void* gp;
extern s32 koopaRunFlag;
extern void* mapalloc_base_ptr;

void* memset(void* dst, int value, u32 size);
void* _mapAlloc(void* heap, u32 size);

s32 mobjGetHint(void* mobj) {
    if (mobj == 0) {
        return 0;
    }
    return (*(u32*)mobj >> 5) & 1;
}

void* mobjHitObjPtrToPtr(void* hitObj) {
    if (*(u32*)((s32)hitObj + 4) & 0x80000000) {
        return *(void**)((s32)hitObj + 0xD0);
    }
    return 0;
}

void mobjReset(s32 alt) {
    MobjSet* set = &work.normal;

    if (alt != 0) {
        set = &work.alt;
    }
    memset(set->entries, 0, set->count * 0x23C);
    koopaRunFlag = 0;
}

void mobjKoopaOn(void) {
    if (koopaRunFlag == 0) {
        koopaRunFlag = 1;
        work.koopa.count = 0x80;
        work.koopa.entries = _mapAlloc(mapalloc_base_ptr, 0x80 * 0x23C);
        memset(work.koopa.entries, 0, work.koopa.count * 0x23C);
    }
}

s32 mobjCheckExec(void) {
    MobjSet* set;
    s32 i;
    void* entry;

    if (koopaRunFlag != 0) {
        set = &work.koopa;
    } else {
        set = &work.normal;
        if (*(s32*)((s32)gp + 0x14) != 0) {
            set = &work.alt;
        }
    }

    entry = set->entries;
    for (i = 0; i < set->count; i++, entry = (void*)((s32)entry + 0x23C)) {
        if (*(u32*)entry & 0x200) {
            return 1;
        }
    }
    return 0;
}

