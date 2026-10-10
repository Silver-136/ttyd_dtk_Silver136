#include "event/evt_nannpc.h"

extern void extLoadShadowMtx(void*);
extern u8 extLoadShadowRenderMode(void);
extern u8 extLoadShadowVertex(void);
extern u8 extLoadShadowTev(void);
extern u8 extDrawShadow(void);
void* nanNPCWork;
s32 evtGetValue(void* event, s32 arg);
extern s32 extGetPoseNum(void);
extern u8* gpGlobals;
char makestring[16];
extern char str_PCT06x_8042285c[];
extern f32 evtGetFloat(void*, s32);

typedef struct NanNpcSortEntry {
    f32 unk0;
    f32 z;
} NanNpcSortEntry;


u8 nanNPCOption(void) {
    typedef NannpcQueryVec Vec;
    extern f32 PSVECDistance(Vec* a, Vec* b);
    extern void PSVECSubtract(Vec* a, Vec* b, Vec* out);
    extern f32 PSVECMag(Vec* v);
    extern void PSVECNormalize(Vec* in, Vec* out);
    extern void PSVECScale(Vec* in, f32 scale, Vec* out);
    extern void PSVECAdd(Vec* a, Vec* b, Vec* out);
    extern void C_VECReflect(Vec* in, Vec* normal, Vec* out);
    extern f32 PSVECDotProduct(Vec* a, Vec* b);

    s32* work;
    char* entry;
    char* other;
    Vec delta;
    Vec oldPos;
    Vec normal;
    f32 dist;
    f32 mag;

    work = (s32*)nanNPCWork;

    for (entry = (char*)work[0]; (u32)entry < (u32)(work[0] + work[3] * 0xC0); entry += 0xC0) {
        if ((*(u16*)(entry + 0x14) & 4) != 0) {
            for (other = (char*)work[0]; (u32)other < (u32)(work[0] + work[3] * 0xC0); other += 0xC0) {
                if ((u32)entry != (u32)other && (*(u16*)(other + 0x14) & 4) != 0) {
                    dist = PSVECDistance((Vec*)(entry + 0x18), (Vec*)(other + 0x18));
                    PSVECSubtract((Vec*)(entry + 0x18), (Vec*)(other + 0x18), &delta);
                    if (dist <= *(f32*)(entry + 0x4C)) {
                        if (PSVECMag(&delta) == 0.0f) {
                            delta.x = 1.0f;
                        }
                        PSVECNormalize(&delta, &delta);
                        delta.y = 0.0f;
                        PSVECScale(&delta, 0.5f, &delta);
                        PSVECAdd((Vec*)(entry + 0x3C), &delta, (Vec*)(entry + 0x3C));
                        PSVECSubtract((Vec*)(other + 0x3C), &delta, (Vec*)(other + 0x3C));
                    }
                }
            }
        }
    }

    for (entry = (char*)work[0]; (u32)entry < (u32)(work[0] + work[3] * 0xC0); entry += 0xC0) {
        if ((*(u16*)(entry + 0x14) & 4) != 0) {
            PSVECSubtract((Vec*)(entry + 0x30), (Vec*)(entry + 0x18), &delta);
            if (PSVECMag(&delta) > 0.8f * *(f32*)(entry + 0x4C)) {
                PSVECNormalize(&delta, &delta);
                delta.y = 0.0f;
                PSVECScale(&delta, 0.5f, &delta);
                PSVECAdd((Vec*)(entry + 0x3C), &delta, (Vec*)(entry + 0x3C));
            }
        }
    }

    for (entry = (char*)work[0]; (u32)entry < (u32)(work[0] + work[3] * 0xC0); entry += 0xC0) {
        if ((*(u16*)(entry + 0x14) & 4) != 0) {
            mag = PSVECMag((Vec*)(entry + 0x3C));
            if (mag != 0.0f) {
                if (mag > 10.0f) {
                    PSVECScale((Vec*)(entry + 0x3C), 5.0f / mag, (Vec*)(entry + 0x3C));
                    mag = 5.0f;
                }

                oldPos = *(Vec*)(entry + 0x18);
                oldPos.y += 1.0f;
                PSVECNormalize((Vec*)(entry + 0x3C), &normal);

                if ((*(u16*)(entry + 0x14) & 8) != 0) {
                    PSVECAdd((Vec*)(entry + 0x18), (Vec*)(entry + 0x3C), (Vec*)(entry + 0x18));
                    PSVECScale((Vec*)(entry + 0x3C), 0.6f, (Vec*)(entry + 0x3C));
                } else {
                    nannpcResolveMotion(entry, oldPos, normal, mag);
                }
            }
        }
    }

    return 0;
}


s32 nannpc_zcompare(NanNpcSortEntry* param_1, NanNpcSortEntry* param_2) {
    if (param_1->z < param_2->z) {
        return 1;
    }
    if (param_1->z > param_2->z) {
        return -1;
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma opt_common_subs off
#pragma optimize_for_size off
void nannpc_ext_disp(void) {
    typedef struct NpcEntry {
        u8 pad00[0x20];
        f32 z;
        u8 pad24[0x98];
        u32 color;
    } NpcEntry;
    typedef struct SortEntry {
        s32 pose;
        f32 z;
    } SortEntry;
    typedef struct Work {
        NpcEntry* entries;
        SortEntry* sort;
        void* unk8;
        s32 count;
        u32 flags;
    } Work;
    extern void extLoadRenderMode(void);
    extern void extLoadVertex(void);
    extern void extLoadTexture(void);
    extern void extLoadTev(void);
    extern void extDraw(void);
    extern void extLoadTextureExit(void);
    extern void extPoseDraw(s32 pose);
    extern void qqsort(void* base, s32 count, s32 size, void* compare);
    extern s32 nannpc_zcompare(void*, void*);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetFog(s32, f32, f32, f32, f32, void*);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevColor(s32, void*);
    extern u32 dat_80422850;
    extern f32 float_0_80422870;
    Work* work;
    u32 renderColor;
    u32 color;
    s32 count;
    s32 i;
    u32 blocks;
    u32 remainder;
    s32 npcOffset;
    s32 sortOffset;
    SortEntry* sort;
    NpcEntry* npc;
    s32 pose;

    work = (Work*)nanNPCWork;
    if ((work->flags & 1) != 0) {
        extLoadVertex();
        extLoadTexture();
        GXSetTevOrder(0, 0, 0, 4);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 15, 8, 2, 15);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaIn(0, 7, 4, 1, 7);
        GXSetNumChans(0);
        GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
        color = dat_80422850;
        GXSetFog(0, float_0_80422870, float_0_80422870,
                 float_0_80422870, float_0_80422870, &color);
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);

        count = extGetPoseNum();
        i = count - 1;
        npcOffset = i * sizeof(NpcEntry);
        sortOffset = i * sizeof(SortEntry);
        for (; i >= 0;) {
            sort = (SortEntry*)((u8*)work->sort + sortOffset);
            npc = (NpcEntry*)((u8*)work->entries + npcOffset);
            sortOffset -= sizeof(SortEntry);
            sort->pose = i;
            i--;
            npcOffset -= sizeof(NpcEntry);
            sort->z = npc->z;
        }
        qqsort(work->sort, work->count, sizeof(SortEntry), nannpc_zcompare);
        count = extGetPoseNum();
        for (i = count - 1, sortOffset = i * sizeof(SortEntry); i >= 0; sortOffset -= sizeof(SortEntry), i--) {
            pose = ((SortEntry*)((u8*)work->sort + sortOffset))->pose;
            renderColor = work->entries[pose].color;
            GXSetTevColor(1, &renderColor);
            extPoseDraw(pose);
        }
        extLoadTextureExit();
    } else {
        extLoadRenderMode();
        extLoadVertex();
        extLoadTexture();
        extLoadTev();
        extDraw();
        extLoadTextureExit();
    }
}
#pragma optimize_for_size reset
#pragma opt_common_subs reset
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset


u8 nannpc_ext_shadow_disp(void) {
    static f32 mt[3][4] = {
        { 10.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 10.0f, 0.0f, 0.1f },
        { 0.0f, 0.0f, 10.0f, 0.0f },
    };

    extLoadShadowMtx(mt);
    extLoadShadowRenderMode();
    extLoadShadowVertex();
    extLoadShadowTev();
    return extDrawShadow();
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void nannpc_ext_init(void) {
    extern void* extGetPosePtr(void);
    extern s32 extGetPoseNum(void);
    extern void PSMTXIdentity(void* matrix);
    extern f32 float_0_80422870;
    extern f32 float_neg1000_80422874;
    s32 count;
    char* pose;
    s32 type;
    f32 offscreen;
    f32 zero;

    pose = (char*)extGetPosePtr();
    count = extGetPoseNum() - 1;
    zero = float_0_80422870;
    offscreen = float_neg1000_80422874;
    type = 3;
    while (count >= 0) {
        *(s32*)(pose + 0x00) = type;
        *(f32*)(pose + 0x04) = zero;
        PSMTXIdentity(*(void**)(pose + 0x0C));
        *(f32*)((char*)*(void**)(pose + 0x0C) + 0x0C) = zero;
        count--;
        *(f32*)((char*)*(void**)(pose + 0x0C) + 0x1C) = offscreen;
        *(f32*)((char*)*(void**)(pose + 0x0C) + 0x2C) = zero;
        pose += 0x20;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void nannpc_ext_main_sub(void) {
    s32* work;
    char* entry;
    void (*callback)(void);
    s32 i;
    s32 offset;

    work = (s32*)nanNPCWork;
    for (i = extGetPoseNum() - 1; i >= 0; i--) {
        entry = (char*)work[0] + i * 0xC0;
        if (*(s8*)entry != 0) {
            callback = *(void (**)(void))(entry + 0x64);
            if (callback != 0) {
                callback();
            }
        }
    }
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void nannpc_ext_main_sub_fast(void) {
    extern void* gp;
    s32* work;
    s8* entry;
    s32 frame;
    s32 count;
    s32 i;
    s32 offset;

    work = (s32*)nanNPCWork;
    count = extGetPoseNum();
    for (i = count - 1, offset = i * 0xC0; i >= 0; offset -= 0xC0, i--) {
        entry = (s8*)work[0] + offset;
        if (entry[0] == 0) {
            continue;
        }
        if (*(void**)(entry + 0x64) != 0) {
            ((void (*)(void*))*(void**)(entry + 0x64))(entry);
        }
        if (*(u32*)(entry + 0x58) != 0) {
            frame = *(u32*)((u8*)gp + 0x1C) + i * i;
            frame = (u32)frame % (u32)*(s16*)(entry + 0x5C);
            *(s16*)(entry + 0x5E) = *(s16*)(*(s32*)(entry + 0x58) + frame * 2);
        }
        *(s32*)(*(s32*)(entry + 0x10) + 0x10) = *(s16*)(entry + 0x5E);
        *(f32*)(*(s32*)(entry + 0x10) + 4) = -*(f32*)(entry + 0x60);
        if ((*(u16*)(entry + 0x14) & 2) != 0) {
            *(f32*)(*(s32*)(*(s32*)(entry + 0x10) + 0xC) + 0x0C) = 0.0f;
            *(f32*)(*(s32*)(*(s32*)(entry + 0x10) + 0xC) + 0x1C) = -1000.0f;
            *(f32*)(*(s32*)(*(s32*)(entry + 0x10) + 0xC) + 0x2C) = 0.0f;
        } else {
            *(f32*)(*(s32*)(*(s32*)(entry + 0x10) + 0xC) + 0x0C) = *(f32*)(entry + 0x18);
            *(f32*)(*(s32*)(*(s32*)(entry + 0x10) + 0xC) + 0x1C) = *(f32*)(entry + 0x1C);
            *(f32*)(*(s32*)(*(s32*)(entry + 0x10) + 0xC) + 0x2C) = *(f32*)(entry + 0x20);
        }
    }
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

void nannpc_ext_main(void) {
    void (**work)(void) = (void (**)(void))nanNPCWork;
    if (work[5] != 0) work[5]();
    if (work[6] != 0) work[6]();
    if (work[7] != 0) work[7]();
}

void nannpc_ext_dispent(void) {
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void nannpc_ext_disp(void);
    extern void nannpc_ext_shadow_disp(void);
    extern f32 float_0_80422870;

    if ((*(u32*)((u8*)nanNPCWork + 0x10) & 1) == 0) {
        dispEntry(4, 1, nannpc_ext_disp, 0, float_0_80422870);
        dispEntry(4, 2, nannpc_ext_shadow_disp, 0, float_0_80422870);
    } else {
        dispEntry(4, 2, nannpc_ext_disp, 0, float_0_80422870);
        dispEntry(4, 2, nannpc_ext_shadow_disp, 0, float_0_80422870);
    }
}

s32 evt_nannpc_init(void* event, s32 firstCall) {
    typedef struct { u32 x, y, z; } InitVector;
    extern void* mapalloc_base_ptr;
    extern void* _mapAlloc(void*, u32);
    extern void* memset(void*, s32, u32);
    extern void extEntry(s32, void*, void*, s32, s32);
    extern void nannpc_ext_init(void);
    extern void nannpc_ext_main(void);
    extern void nannpc_ext_main_sub(void);
    extern void nannpc_ext_dispent(void);
    extern u32 vec3_802f35f8[];
    extern u32 dat_80422854;
    extern f32 float_0_80422870;
    s32* args;
    void* poseData;
    s16* table;
    void* initFunc;
    void* mainFunc;
    void* dispFunc;
    s32* work;
    char* entry;
    u32* defaults;
    s32 i, offset;
    f32 size, zero;

    defaults = vec3_802f35f8;
    args = *(s32**)((u8*)event + 0x18);
    poseData = (void*)evtGetValue(event, args[0]);
    table = (s16*)evtGetValue(event, args[1]);
    initFunc = (void*)evtGetValue(event, args[2]);
    mainFunc = (void*)evtGetValue(event, args[3]);
    dispFunc = nannpc_ext_dispent;
    if (firstCall) return 0;
    nanNPCWork = _mapAlloc(mapalloc_base_ptr, 0x20);
    memset(nanNPCWork, 0, 0x20);
    work = nanNPCWork;
    work[3] = evtGetValue(event, args[4]);
    work[0] = (s32)_mapAlloc(mapalloc_base_ptr, work[3] * 0xC0);
    memset((void*)work[0], 0, work[3] * 0xC0);
    work[1] = (s32)_mapAlloc(mapalloc_base_ptr, work[3] * 8);
    memset((void*)work[1], 0, work[3] * 8);
    work[2] = (s32)_mapAlloc(mapalloc_base_ptr, 0x14);
    memset((void*)work[2], 0, 0x14);
    work[5] = 0;
    ((s32*)nanNPCWork)[6] = (s32)nannpc_ext_main_sub;
    work[7] = 0;
    if ((u32)initFunc == 0) initFunc = nannpc_ext_init;
    if ((u32)mainFunc == 0) mainFunc = nannpc_ext_main;
    extEntry(work[3], poseData, initFunc, (s32)mainFunc, (s32)dispFunc);
    size = 32.0f;
    zero = float_0_80422870;
    for (i = 0, offset = 0; i < work[3]; offset += 0xC0, i++) {
        entry = (char*)work[0] + offset;
        entry[0] = 0;
        *(s32*)(entry + 0x10) = 0;
        *(u16*)(entry + 0x14) = 0;
        *(u16*)(entry + 0x16) = 0;
        *(InitVector*)(entry + 0x18) = *(InitVector*)(defaults + 0);
        *(InitVector*)(entry + 0x30) = *(InitVector*)(defaults + 3);
        *(InitVector*)(entry + 0x24) = *(InitVector*)(defaults + 6);
        *(f32*)(entry + 0x48) = size;
        *(f32*)(entry + 0x4C) = size;
        *(u32*)(entry + 0x54) = 0;
        *(u32*)(entry + 0x50) = 0;
        *(u32*)(entry + 0x58) = 0;
        *(u16*)(entry + 0x5C) = 0;
        *(u16*)(entry + 0x5E) = 0;
        *(f32*)(entry + 0x60) = zero;
        *(u32*)(entry + 0x64) = 0;
        *(u32*)(entry + 0xBC) = dat_80422854;
    }
    if ((u32)table == 0) return 2;
    if (table[0] == -1) return 2;
    *(s16*)(work[2] + 0) = table[0];
    *(s16*)(work[2] + 2) = table[1];
    if (table[2] == -1) return 2;
    *(s16*)(work[2] + 4) = table[2];
    *(s16*)(work[2] + 6) = table[3];
    if (table[4] == -1) return 2;
    *(s16*)(work[2] + 8) = table[4];
    *(s16*)(work[2] + 10) = table[5];
    if (table[6] == -1) return 2;
    *(s16*)(work[2] + 12) = table[6];
    *(s16*)(work[2] + 14) = table[7];
    if (table[8] == -1) return 2;
    *(s16*)(work[2] + 16) = table[8];
    *(s16*)(work[2] + 18) = table[9];
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_nannpc_set_subfunc(void* pEvt) {
    s32* args;
    s32* work;
    s32 type;

    args = *(s32**)((s32)pEvt + 0x18);
    type = evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    switch (type) {
        case 0:
            work[5] = evtGetValue(pEvt, args[1]);
            break;
        case 1:
            work[6] = evtGetValue(pEvt, args[1]);
            break;
        case 2:
            work[7] = evtGetValue(pEvt, args[1]);
            break;
    }
    return 2;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

s32 evt_nannpc_entry(void* event) {
    typedef struct { u32 x, y, z; } EntryVector;
    extern s32 extGetPosePtr(void);
    extern s32 sprintf(char*, const char*, ...);
    extern s32 strcmp(const char*, const char*);
    extern char* strcpy(char*, const char*);
    extern u32 vec3_802f35f8[];
    extern u32 dat_80422858;
    extern f32 float_32_8042286c;
    extern f32 float_0_80422870;
    s32* args;
    s32* work;
    u32* defaults;
    char* name;
    char* lookup;
    char* entry;
    s32 poseBase;
    s32 i;
    s32 offset;
    f32 size;
    f32 zero;

    defaults = vec3_802f35f8;
    args = *(s32**)((u8*)event + 0x18);
    poseBase = extGetPosePtr();
    name = (char*)evtGetValue(event, args[0]);
    work = (s32*)nanNPCWork;
    for (i = 0, offset = 0; i < work[3]; offset += 0xC0, i++) {
        entry = (char*)work[0] + offset;
        lookup = name;
        if (((u32)name & 0x80000000) == 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            lookup = makestring;
        }
        strcmp(entry, lookup);
        if (*(u32*)(entry + 0x10) == 0) {
            strcpy(entry, lookup);
            *(s32*)(entry + 0x10) = poseBase + i * 0x20;
            zero = float_0_80422870;
            size = float_32_8042286c;
            *(u16*)(entry + 0x14) = 0;
            *(u16*)(entry + 0x16) = 0;
            *(EntryVector*)(entry + 0x18) = *(EntryVector*)(defaults + 9);
            *(EntryVector*)(entry + 0x30) = *(EntryVector*)(defaults + 12);
            *(EntryVector*)(entry + 0x24) = *(EntryVector*)(defaults + 15);
            *(f32*)(entry + 0x48) = size;
            *(f32*)(entry + 0x4C) = size;
            *(u32*)(entry + 0x54) = 0;
            *(u32*)(entry + 0x50) = 0;
            *(u16*)(entry + 0x5C) = 0;
            *(f32*)(entry + 0x60) = zero;
            *(f32*)(*(s32*)(entry + 0x10) + 4) = -*(f32*)(entry + 0x60);
            *(u32*)(entry + 0xBC) = dat_80422858;
            break;
        }
    }
    return 2;
}

s32 evt_nannpc_set_position(void* pEvt) {
    s32* args;
    char* name;
    char* entry;
    f32 value;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    entry = nannpcLookupName(name);

    value = evtGetFloat(pEvt, args[1]);
    *(f32*)(entry + 0x18) = value;
    *(f32*)(entry + 0x30) = value;
    value = evtGetFloat(pEvt, args[2]);
    *(f32*)(entry + 0x1C) = value;
    *(f32*)(entry + 0x34) = value;
    value = evtGetFloat(pEvt, args[3]);
    *(f32*)(entry + 0x20) = value;
    *(f32*)(entry + 0x38) = value;
    return 2;
}

s32 evt_nannpc_get_position(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern void evtSetFloat(void*, s32, f32);
    s32* args;
    char* name;
    char* entry;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    entry = nannpcLookupName(name);

    evtSetFloat(pEvt, args[1], *(f32*)(entry + 0x18));
    evtSetFloat(pEvt, args[2], *(f32*)(entry + 0x1C));
    evtSetFloat(pEvt, args[3], *(f32*)(entry + 0x20));
    return 2;
}

s32 evt_nannpc_set_hosei_position(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    s32* args;
    char* name;
    char* entry;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    entry = nannpcLookupName(name);

    *(f32*)(entry + 0x24) = evtGetFloat(pEvt, args[1]);
    *(f32*)(entry + 0x28) = evtGetFloat(pEvt, args[2]);
    *(f32*)(entry + 0x2C) = evtGetFloat(pEvt, args[3]);
    return 2;
}

s32 evt_nannpc_set_size(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    s32* args;
    char* name;
    char* entry;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    entry = nannpcLookupName(name);

    *(f32*)(entry + 0x48) = evtGetFloat(pEvt, args[1]);
    *(f32*)(entry + 0x4C) = evtGetFloat(pEvt, args[2]);
    return 2;
}

u8 evt_nannpc_set_animtbl_sub(int param_1, short* param_2) {
    s32 count = 0;

    *(short**)(param_1 + 0x58) = param_2;
loop:
    if (*param_2 == -1) {
        goto done;
    }
    param_2++;
    count++;
    goto loop;
done:
    *(s16*)(param_1 + 0x5C) = count;
    if (*(s16*)(param_1 + 0x5C) == 0) {
        *(s32*)(param_1 + 0x58) = 0;
    }
}

s32 evt_nannpc_set_animtbl(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    s32* args;
    char* name;
    char* entry;
    s16* tbl;
    s32 count;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    entry = nannpcLookupName(name);

    tbl = (s16*)evtGetValue(pEvt, args[1]);
    *(s16**)(entry + 0x58) = tbl;
    count = 0;
loop:
    if (*tbl == -1) {
        goto done;
    }
    tbl++;
    count++;
    goto loop;
done:
    *(s16*)(entry + 0x5C) = count;
    if (*(s16*)(entry + 0x5C) == 0) {
        *(s32*)(entry + 0x58) = 0;
    }
    return 2;
}

s32 evt_nannpc_set_dir(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    s32* args;
    char* name;
    char* entry;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    entry = nannpcLookupName(name);

    *(f32*)(entry + 0x60) = (f32)evtGetValue(pEvt, args[1]);
    return 2;
}

s32 evt_nannpc_set_work(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    s32* args;
    char* name;
    char* entry;
    s32 idx;
    s32 value;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    entry = nannpcLookupName(name);
    idx = evtGetValue(pEvt, args[1]);
    value = evtGetValue(pEvt, args[2]);
    entry += idx * 4;
    *(s32*)(entry + 0xA4) = value;
    return 2;
}

s32 evt_nannpc_get_work(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern void evtSetValue(void*, s32, s32);
    s32* args;
    char* name;
    char* entry;
    s32 idx;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    entry = nannpcLookupName(name);

    idx = evtGetValue(pEvt, args[1]);
    entry += idx * 4;
    evtSetValue(pEvt, args[2], *(s32*)(entry + 0xA4));
    return 2;
}

#pragma optimize_for_size off
s32 evt_nannpc_move_position2(void* event, s32 firstCall) {
    typedef struct Vec3f { f32 x, y, z; } Vec3f;
    extern s32 sprintf(char*, const char*, ...);
    extern s32 strcmp(const char*, const char*);
    extern void PSVECSubtract(Vec3f*, Vec3f*, Vec3f*);
    extern f32 PSVECMag(Vec3f*);
    extern void PSVECScale(Vec3f*, f32, Vec3f*);
    extern void PSVECAdd(Vec3f*, Vec3f*, Vec3f*);
    extern void* gp;
    s32* args;
    char* name;
    char* entry;
    Vec3f target;
    Vec3f delta;
    Vec3f movement;
    f32 speed;
    f32 distance;
    u64 now;
    s32 elapsed;
    s32 duration;

    args = *(s32**)((u8*)event + 0x18);
    name = (char*)evtGetValue(event, args[0]);
    entry = nannpcLookupName(name);
    if (*(s32*)((u8*)gp + 0x14) != 0) {
        now = *(u64*)((u8*)gp + 0x38);
    } else {
        now = *(u64*)((u8*)gp + 0x40);
    }
    target.x = (f32)evtGetValue(event, args[1]);
    target.y = (f32)evtGetValue(event, args[2]);
    target.z = (f32)evtGetValue(event, args[3]);
    speed = evtGetFloat(event, args[4]);
    if (firstCall != 0) {
        *(u64*)((u8*)event + 0x198) = now;
        *(Vec3f*)(entry + 0x88) = *(Vec3f*)(entry + 0x18);
    }
    PSVECSubtract(&target, (Vec3f*)(entry + 0x88), &delta);
    distance = PSVECMag(&delta);
    duration = (s32)(1000.0f * distance / speed);
    elapsed = ((s32)(u32)now - *(s32*)((u8*)event + 0x19C)) /
              (u64)((*(u32*)0x800000F8 >> 2) / 1000);
    PSVECScale(&delta, (f32)elapsed / (f32)duration, &movement);
    if (elapsed < duration) {
        PSVECAdd((Vec3f*)(entry + 0x88), &movement, (Vec3f*)(entry + 0x18));
        return 0;
    }
    *(Vec3f*)(entry + 0x18) = target;
    return 2;
}
#pragma optimize_for_size reset

u8 evt_nannpc_jump_position(void* event, s32 isFirstCall) {
    extern s32 sprintf(char* buf, char* fmt, ...);
    extern s32 strcmp(char* a, char* b);
    extern s32 evtGetValue(void* event, s32 arg);
    extern f32 intplGetValue(f32 start, f32 end, s32 mode, s32 curStep, s32 maxStep);
    extern void* gp;

    s32* args;
    char* name;
    char* entry;
    f32 x;
    f32 y;
    f32 z;
    s32 steps;
    f32 peak;
    s32 curStep;
    f32 ratio;

    args = *(s32**)((s32)event + 0x18);
    name = (char*)evtGetValue(event, args[0]);
    entry = nannpcLookupName(name);

    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    steps = evtGetValue(event, args[4]);
    peak = (f32)evtGetValue(event, args[5]);

    if (isFirstCall != 0) {
        *(s32*)(entry + 0x54) = *(s32*)((s32)gp + 0x3C);
        *(s32*)(entry + 0x50) = *(s32*)((s32)gp + 0x38);
        *(s32*)(entry + 0x68) = (s32)*(f32*)(entry + 0x18);
        *(s32*)(entry + 0x6C) = (s32)*(f32*)(entry + 0x1C);
        *(s32*)(entry + 0x70) = (s32)*(f32*)(entry + 0x20);
        *(f32*)((s32)event + 0x7C) = -4.0f * peak;
        *(f32*)((s32)event + 0x80) = (y - (f32)*(s32*)(entry + 0x6C)) - *(f32*)((s32)event + 0x7C);
    }

    curStep = (*(s32*)((s32)gp + 0x3C) - *(s32*)(entry + 0x54)) / ((*(u32*)0x800000F8 >> 2) / 4000);
    if (curStep < steps) {
        ratio = (f32)curStep / (f32)steps;
        *(f32*)(entry + 0x18) = intplGetValue((f32)*(s32*)(entry + 0x68), x, 0, curStep, steps);
        *(f32*)(entry + 0x20) = intplGetValue((f32)*(s32*)(entry + 0x70), z, 0, curStep, steps);
        *(f32*)(entry + 0x1C) = ratio * (*(f32*)((s32)event + 0x7C) * ratio + *(f32*)((s32)event + 0x80)) +
                                (f32)*(s32*)(entry + 0x6C);
        return 0;
    }

    *(f32*)(entry + 0x18) = x;
    *(f32*)(entry + 0x1C) = y;
    *(f32*)(entry + 0x20) = z;
    return 2;
}

s32 evt_nannpc_flag_onoff(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    s32* args;
    char* name;
    char* entry;
    u16 mask;
    s32 onoff;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    onoff = evtGetValue(pEvt, args[1]);
    entry = nannpcLookupName(name);
    mask = (u16)evtGetValue(pEvt, args[2]);
    if (onoff != 0) {
        *(u16*)(entry + 0x14) &= ~mask;
    } else {
        *(u16*)(entry + 0x14) |= mask;
    }
    return 2;
}

s32 evt_nannpc_ppflag_onoff(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    s32* args;
    char* name;
    char* entry;
    u16 mask;
    u32* flags;
    u32 oldFlags;
    s32 onoff;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    onoff = evtGetValue(pEvt, args[1]);
    entry = nannpcLookupName(name);
    mask = (u16)evtGetValue(pEvt, args[2]);
    if (onoff != 0) {
        flags = *(u32**)(entry + 0x10);
        oldFlags = *flags;
        *flags = oldFlags & ~mask;
    } else {
        flags = *(u32**)(entry + 0x10);
        *flags |= mask;
    }
    return 2;
}

s32 evt_nannpc_set_func(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    s32* args;
    char* name;
    char* entry;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    entry = nannpcLookupName(name);
    *(u32*)(entry + 0x64) = evtGetValue(pEvt, args[1]);
    return 2;
}

s32 evt_nannpc_set_shadow_position(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    s32* args;
    char* name;
    char* entry;
    void* pp;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    entry = nannpcLookupName(name);
    pp = *(void**)(entry + 0x10);
    *(u32*)pp |= 0x10;
    {
        f32 height = evtGetFloat(pEvt, args[1]);
        *(f32*)((s32)*(void**)(entry + 0x10) + 8) = height;
    }
    return 2;
}

#pragma optimize_for_size off
s32 evt_nannpc_set_color(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 i;
    s32 offset;
    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    i = 0;
    offset = i;
    for (; i < work[3]; offset += 0xC0, i++) {
        entry = (char*)(work[0] + offset);
        if (((u32)name & 0x80000000) == 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) != 0) goto next;
        goto found;
next:
        ;
    }
    entry = NULL;
found:
    entry[0xBC] = evtGetValue(pEvt, args[1]);
    entry[0xBD] = evtGetValue(pEvt, args[2]);
    entry[0xBE] = evtGetValue(pEvt, args[3]);
    entry[0xBF] = evtGetValue(pEvt, args[4]);
    return 2;
}
#pragma optimize_for_size reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_nannpcwork_flag_onoff(void* pEvt) {
    s32* args;
    void* work;
    s32 onoff;
    u16 mask;

    args = *(s32**)((s32)pEvt + 0x18);
    onoff = evtGetValue(pEvt, args[0]);
    work = nanNPCWork;
    mask = evtGetValue(pEvt, args[1]);

    if (onoff != 0) {
        *(u32*)((s32)work + 0x10) = *(u32*)((s32)work + 0x10) & ~mask;
    } else {
        *(u32*)((s32)work + 0x10) = *(u32*)((s32)work + 0x10) | mask;
    }

    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

__declspec(section ".rodata") u32 vec3_802f35f8[3] = {0x00000000, 0xC47A0000, 0x00000000};

__declspec(section ".rodata") u32 vec3_802f3604[3] = {0x00000000, 0xC47A0000, 0x00000000};

__declspec(section ".rodata") u32 vec3_802f3610[3] = {0x00000000, 0x00000000, 0x00000000};

__declspec(section ".rodata") u32 vec3_802f361c[3] = {0x00000000, 0xC47A0000, 0x00000000};

__declspec(section ".rodata") u32 vec3_802f3628[3] = {0x00000000, 0xC47A0000, 0x00000000};

__declspec(section ".rodata") u32 vec3_802f3634[3] = {0x00000000, 0x00000000, 0x00000000};
