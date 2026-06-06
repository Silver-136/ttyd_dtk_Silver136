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
        MobjSet* set = &work.koopa;

        koopaRunFlag = 1;
        set->count = 0x80;
        set->entries = _mapAlloc(mapalloc_base_ptr, set->count * 0x23C);
        memset(set->entries, 0, set->count * 0x23C);
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


u8 mobjMain(void) {
    return 0;
}


u32* mobjNearDistCheck2(double param_1, double param_2, double param_3, double param_4, char** param_5) {
    return 0;
}


int mobjEntry(char* name, char* pAnimPoseName) {
    return 0;
}


u8 mobjDisp_OffscreenXLU(s32 param_1, u32* param_2) {
    return 0;
}


u8 mobjDisp(s32 param_1, u32* param_2) {
    return 0;
}


u8 mobjDispXLU(s32 param_1, u32* param_2) {
    return 0;
}


u8 mobjCalcMtx2(int param_1) {
    return 0;
}


u8 mobjSetPosition(float posX, float posY, float posZ, s32 mobjName) {
    return 0;
}


void mobjDelete(s32 name) {
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* mobjNameToPtrNoAssert(char* name) {
    extern MobjWork work;
    extern void* gp;
    extern s32 koopaRunFlag;
    extern s32 strcmp(const char* a, const char* b);
    MobjSet* set;
    s32 i;
    s32 count;
    void* entry;
    if (koopaRunFlag != 0) {
        set = &work.koopa;
    } else {
        set = &work.normal;
        if (*(s32*)((s32)gp + 0x14) != 0) {
            set = &work.alt;
        }
    }
    count = set->count;
    i = 0;
    entry = set->entries;
    while (i < count) {
        if ((*(u32*)entry & 1) && strcmp((char*)((s32)entry + 5), name) == 0) {
            break;
        }
        i++;
        entry = (void*)((s32)entry + 0x23C);
    }
    if (i >= count) {
        return 0;
    }
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* mobjNameToPtr(char* name) {
    extern MobjWork work;
    extern void* gp;
    extern s32 koopaRunFlag;
    extern s32 strcmp(const char* a, const char* b);
    MobjSet* set;
    s32 count;
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
    count = set->count;
    i = 0;
    entry = set->entries;
    while (i < count) {
        if ((*(u32*)entry & 1) && strcmp((char*)((s32)entry + 5), name) == 0) {
            break;
        }
        i++;
        entry = (void*)((s32)entry + 0x23C);
    }
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

int mobjCheckItemboxOpen(int param_1) {
    return 0;
}


u8 mobjInit(void) {
    return 0;
}


u8 mobjCalcMtx(void* pMobj) {
    return 0;
}


u8 mobjHitEntry(void* pMobj, int param_2) {
    return 0;
}


void* mobjRunEvent(void* pMobj, void* evtCode) {
    return 0;
}
