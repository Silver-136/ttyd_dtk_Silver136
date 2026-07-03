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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effDrawMayaPoly(void* data) {
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(u8 attr, u8 type);
    extern void GXSetVtxAttrFmt(u32 vtxfmt, u32 attr, u32 compCnt, u32 compType, u32 frac);
    extern void GXSetCullMode(u32 mode);
    extern void GXBegin(u8 primitive, u8 vtxfmt, u16 nverts);
    volatile s16* fifo;
    s32 i;
    s32 offset;
    s16* poly;
    s32 count;
    s32 chunks;
    s32 rem;

    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 3, 9);
    GXSetVtxAttrFmt(0, 0xD, 1, 3, 9);
    GXSetCullMode(0);

    i = 0;
    offset = 0;
    fifo = (volatile s16*)0xCC008000;
    while (i < *(s32*)((s32)data + 8)) {
        poly = *(s16**)(*(s32*)((s32)data + 0xC) + offset);
        count = *poly;
        GXBegin(0xA0, 0, count);
        poly++;
        if (count > 0) {
            chunks = (u32)count >> 2;
            while (chunks != 0) {
                fifo[0] = poly[0];
                fifo[0] = poly[1];
                fifo[0] = poly[2];
                fifo[0] = poly[3];
                fifo[0] = poly[4];
                fifo[0] = poly[5];
                fifo[0] = poly[6];
                fifo[0] = poly[7];
                fifo[0] = poly[8];
                fifo[0] = poly[9];
                fifo[0] = poly[10];
                fifo[0] = poly[11];
                fifo[0] = poly[12];
                fifo[0] = poly[13];
                fifo[0] = poly[14];
                fifo[0] = poly[15];
                fifo[0] = poly[16];
                fifo[0] = poly[17];
                fifo[0] = poly[18];
                fifo[0] = poly[19];
                poly += 20;
                chunks--;
            }
            rem = count & 3;
            while (rem != 0) {
                fifo[0] = poly[0];
                fifo[0] = poly[1];
                fifo[0] = poly[2];
                fifo[0] = poly[3];
                fifo[0] = poly[4];
                poly += 5;
                rem--;
            }
        }
        offset += 4;
        i++;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void effMain(void) {
    typedef void (*EffCallback)(void*);
    extern void fileFree(void*);
    extern void* getMarioStDvdRoot(void);
    extern void* fileAsyncf(s32, s32, const char*, ...);
    extern void* fileAllocf(s32, const char*, ...);
    extern char* prefix_tbl[];
    s32 i;
    s32 count;
    void* entry;
    char* prefix;

    count = wp->count;
    entry = wp->entries;
    for (i = 0; i < count; i++, entry = (void*)((s32)entry + 0x28)) {
        if (gp->unk14 != 0) {
            if (*(s32*)((s32)entry + 4) != 1) {
                continue;
            }
        } else if (*(s32*)((s32)entry + 4) != 0) {
            continue;
        }
        if ((*(u32*)entry & 1) && *(EffCallback*)((s32)entry + 0x10) != 0) {
            (*(EffCallback*)((s32)entry + 0x10))(entry);
        }
    }

    if (*(s32*)((s32)wp + 0x14) != *(s32*)((s32)gp + 0x16C)) {
        if (*(void**)((s32)wp + 0x10) != 0) {
            fileFree(*(void**)((s32)wp + 0x10));
            *(void**)((s32)wp + 0x10) = 0;
        }
        prefix = prefix_tbl[*(s32*)((s32)gp + 0x16C)];
        if (fileAsyncf(4, 0, (const char*)0x802C1F20, getMarioStDvdRoot(), prefix, prefix) != 0) {
            prefix = prefix_tbl[*(s32*)((s32)gp + 0x16C)];
            *(void**)((s32)wp + 0x10) = fileAllocf(4, (const char*)0x802C1F20, getMarioStDvdRoot(), prefix, prefix);
            *(s32*)((s32)wp + 0x14) = *(s32*)((s32)gp + 0x16C);
        }
    }
}

void effTexSetup(void) {
    extern void* arcOpen(const char*, s32, s32);
    extern void* getMarioStDvdRoot(void);
    extern s32 sprintf(char*, const char*, ...);
    extern void* DVDMgrOpen(char*, s32, s32);
    extern s32 DVDMgrGetLength(void*);
    extern void* __memAlloc(s32, u32);
    extern void DVDMgrReadAsync(void*, void*, u32, s32, void (*)(void*, void*));
    extern void _callback_tpl(void*, void*);
    extern void UnpackTexPalette(void*);
    extern void effTexSetupN64(void);
    char path[0x80];
    char* lang;
    void* dvd;
    u32 len;

    wp->unk8 = 0;
    wp->unkC = 0;
    wp->unk8 = arcOpen((const char*)0x802C1F38, 0, 0);
    if (wp->unk8 == 0) {
        lang = (char*)0x80420184;
        if (*(u32*)((s32)gp + 0x16C) == 0) {
            lang = (char*)0x80420180;
        }
        sprintf(path, (const char*)0x802C1F44, getMarioStDvdRoot(), lang);
        dvd = DVDMgrOpen(path, 2, 0);
        len = (DVDMgrGetLength(dvd) + 0x1F) & ~0x1F;
        wp->unk8 = __memAlloc(0, len);
        *(void**)((s32)dvd + 0x6C) = dvd;
        DVDMgrReadAsync(dvd, wp->unk8, len, 0, _callback_tpl);
    } else {
        UnpackTexPalette(wp->unk8);
        wp->unkC = 1;
    }
    effTexSetupN64();
}

void effGetTexObj(s32 id, void* texObj) {
    extern void GXInitTexObj(void*, void*, s32, s32, s32, s32, s32, s32);
    extern void TEXGetGXTexObjFromPalette(void*, s32, void*);

    if (id < 0x76) {
        if (wp->unkC == 0) {
            GXInitTexObj(texObj, (void*)0x80420188, 1, 1, 0, 0, 0, 0);
        } else {
            TEXGetGXTexObjFromPalette(wp->unk8, id, texObj);
        }
    } else {
        void* file = *(void**)((s32)wp + 0x10);
        if (file == 0) {
            GXInitTexObj(texObj, (void*)0x8042018C, 1, 1, 0, 0, 0, 0);
        } else {
            TEXGetGXTexObjFromPalette(*(void**)*(void**)((s32)file + 0xA0), id - 0x76, texObj);
        }
    }
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

