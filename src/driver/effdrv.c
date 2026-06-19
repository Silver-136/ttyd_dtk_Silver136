#include "driver/effdrv.h"

typedef struct EffEntry {
    u32 flags;
    s32 unk4;
    void* unk8;
    void* data;
    void* unk10;
    void* unk14;
    char name[0x10];
} EffEntry;

typedef struct EffWork {
    s32 count;
    EffEntry* entries;
    void* unk8;
    s32 unkC;
} EffWork;

typedef struct EffGp {
    u8 pad0[0x14];
    s32 unk14;
} EffGp;

extern EffWork* wp;
extern EffGp* gp;

void __memFree(s32 heap, void* ptr);
void UnpackTexPalette(void* tpl);
void DVDMgrClose(void* entry);
s32 strcmp(const char* s1, const char* s2);
char* strcpy(char* dst, const char* src);

void effDeleteMayaAnim(void* ptr) {
    __memFree(3, ptr);
}

void effDelete(EffEntry* entry) {
    if (entry != 0 && entry->flags != 0) {
        __memFree(3, entry->data);
        entry->flags = 0;
    }
}

void effSoftDelete(EffEntry* entry) {
    u32 flags;

    if (entry != 0) {
        flags = entry->flags;
        if (flags != 0) {
            if (flags & 2) {
                entry->flags = flags | 4;
            } else {
                if (entry != 0 && flags != 0) {
                    __memFree(3, entry->data);
                    entry->flags = 0;
                }
            }
        }
    }
}

EffEntry* effEntry(void) {
    EffEntry* entry = wp->entries;
    s32 count = wp->count;
    s32 i;

    for (i = 0; i < count; i++) {
        if (entry->flags == 0) {
            break;
        }
        entry++;
    }

    entry->flags = 1;
    entry->unk4 = (u32)(-gp->unk14 | gp->unk14) >> 31;
    entry->unk14 = 0;
    entry->unk8 = 0;
    entry->data = 0;
    entry->unk10 = 0;
    entry->name[0] = 0;
    return entry;
}

void effSetName(EffEntry* entry, char* name) {
    s32 i = 0;
    s32 count = wp->count;
    EffEntry* scan = wp->entries;

    while (i < count) {
        if (scan->flags != 0 && strcmp(scan->name, name) == 0) {
            break;
        }
        i++;
        scan++;
    }

    strcpy(entry->name, name);
}

void _callback_tpl(void* unk, void* dvdEntry) {
    void* closeEntry = *(void**)((s32)dvdEntry + 0x2C);

    UnpackTexPalette(wp->unk8);
    DVDMgrClose(closeEntry);
    wp->unkC = 1;
}


u8 effCalcMayaAnimMatrix(int param_1, int* param_2, s32 param_3, float* param_4) {
    return 0;
}


u8 effDrawMayaPoly(int param_1) {
    return 0;
}


u8 effMain(void) {
    return 0;
}


u8 effTexSetup(void) {
    return 0;
}


u8 effGetTexObj(int param_1, u32* param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effNameToPtr(char* name) {
    extern EffWork* wp;
    extern s32 strcmp(const char* s1, const char* s2);
    s32 i = 0;
    s32 count = wp->count;
    EffEntry* entry = wp->entries;
    while (i < count) {
        if (entry->flags != 0 && strcmp(entry->name, name) == 0) {
            break;
        }
        i++;
        entry++;
    }
    if (i >= count) {
        return 0;
    }
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effGetSet(char* name) {
    extern s32 strcmp(const char* s1, const char* s2);
    typedef struct EffSetEntry {
        s16 id;
        s16 pad;
        char* name;
    } EffSetEntry;
    extern EffSetEntry eff_set_table[];
    EffSetEntry* entry = eff_set_table;
    s32 i = 0;
    while (entry->id != -1) {
        if (strcmp(entry->name, name) == 0) {
            return &eff_set_table[i];
        }
        entry++;
        i++;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void effAutoRelease(s32 value) {
    extern EffWork* wp;
    extern void __memFree(s32 heap, void* ptr);
    s32 i = 0;
    s32 zero = 0;
    s32 count = wp->count;
    EffEntry* entry = wp->entries;
    while (i < count) {
        if ((entry->flags & 1) && entry->unk4 == value) {
            if (entry != 0 && entry->flags != 0) {
                __memFree(3, entry->data);
                entry->flags = zero;
            }
        }
        i++;
        entry++;
    }
}

void effInit(void) {
    extern EffWork* wp;
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* dst, s32 value, u32 size);
    extern void effInit64(void);
    wp->count = 0x200;
    wp->entries = __memAlloc(0, wp->count * 0x28);
    memset(wp->entries, 0, wp->count * 0x28);
    *(s32*)((s32)wp + 0x10) = 0;
    *(s32*)((s32)wp + 0x14) = -1;
    wp->unk8 = 0;
    wp->unkC = 0;
    effInit64();
}

int* effMayaAnimAlloc(void* effData) {
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* ptr, s32 value, u32 size);
    u32 size;
    void* anim;

    size = *(s32*)(*(s32*)((s32)effData + 8)) * 0x34 + 0x18;
    anim = __memAlloc(3, size);
    memset(anim, 0, size);
    *(void**)((s32)anim + 0x10) = effData;
    return anim;
}

u8 effCalcMayaAnim(void* param_1) {
    return 0;
}


void effPlayMayaAnim(void* anim) {
    extern EffGp* gp;
    extern const f32 float_0_804201a0;
    u64* t;
    u32 bus;
    u32 ticks;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        t = (u64*)((s32)gp + 0x38);
    } else {
        t = (u64*)((s32)gp + 0x40);
    }
    bus = *(u32*)0x800000F8 >> 2;
    ticks = ((u64)bus * 0x10624DD3U) >> 38;
    *(s64*)((s32)anim + 8) = *t / ticks;
    *(f32*)((s32)anim + 4) = float_0_804201a0;
    *(u16*)anim |= 1;
}

