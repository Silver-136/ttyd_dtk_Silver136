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

void mobjKoopaOn(void) {
    if (koopaRunFlag == 0) {
        MobjSet* set = &work.koopa;

        koopaRunFlag = 1;
        set->count = 0x80;
        set->entries = _mapAlloc(mapalloc_base_ptr, set->count * 0x23C);
        memset(set->entries, 0, set->count * 0x23C);
    }
}

void mobjDispXLU(s32 param_1, void* entry) {
    extern void animSetPaperTexMtx(void* mtx, s32 a, s32 b);
    extern void animSetPaperTexObj(void* texObj, s32 a, s32 b, s32 texMap, s32 d, s32 e, s32 f, s32 g);
    extern void animPoseDraw(s32 poseId, s32 mode, f32 x, f32 y, f32 z, f32 rotY, f32 scale);
    extern f32 float_10_804201bc;

    if (*(u32*)entry & 0x400) {
        animSetPaperTexMtx((void*)((s32)entry + 0x20C), 0, 0);
        animSetPaperTexObj((void*)((s32)entry + 0x1E8), 0, 0, *(s32*)((s32)entry + 0x208), 0, 0, 1, 1);
    }

    animPoseDraw(*(s32*)((s32)entry + 0x70), 1, *(f32*)((s32)entry + 0x38), *(f32*)((s32)entry + 0x3C),
                 *(f32*)((s32)entry + 0x40), *(f32*)((s32)entry + 0x54), float_10_804201bc * *(f32*)((s32)entry + 0x44));
    animPoseDraw(*(s32*)((s32)entry + 0x70), 2, *(f32*)((s32)entry + 0x38), *(f32*)((s32)entry + 0x3C),
                 *(f32*)((s32)entry + 0x40), *(f32*)((s32)entry + 0x54), float_10_804201bc * *(f32*)((s32)entry + 0x44));
    animPoseDraw(*(s32*)((s32)entry + 0x70), 3, *(f32*)((s32)entry + 0x38), *(f32*)((s32)entry + 0x3C),
                 *(f32*)((s32)entry + 0x40), *(f32*)((s32)entry + 0x54), float_10_804201bc * *(f32*)((s32)entry + 0x44));

    if (*(u32*)entry & 0x400) {
        animSetPaperTexMtx(0, 0, 0);
        animSetPaperTexObj(0, 0, 0, 0, 0, 0, 1, 1);
    }
}

void mobjDisp(s32 param_1, void* entry) {
    extern void animSetPaperTexMtx(void* mtx, s32 a, s32 b);
    extern void animSetPaperTexObj(void* texObj, s32 a, s32 b, s32 texMap, s32 d, s32 e, s32 f, s32 g);
    extern void* dispGetCurWork(void);
    extern void animPoseDraw(s32 poseId, s32 mode, f32 x, f32 y, f32 z, f32 rotY, f32 scale);
    extern f32 float_10_804201bc;
    s32 mode;

    if (*(u32*)entry & 0x400) {
        animSetPaperTexMtx((void*)((s32)entry + 0x20C), 0, 0);
        animSetPaperTexObj((void*)((s32)entry + 0x1E8), 0, 0, *(s32*)((s32)entry + 0x208), 0, 0, 1, 1);
    }

    mode = *(u8*)((s32)dispGetCurWork() + 1);
    if (mode == 0) {
        animPoseDraw(*(s32*)((s32)entry + 0x70), 1, *(f32*)((s32)entry + 0x38), *(f32*)((s32)entry + 0x3C),
                     *(f32*)((s32)entry + 0x40), *(f32*)((s32)entry + 0x54), float_10_804201bc * *(f32*)((s32)entry + 0x44));
    } else if (mode == 1) {
        animPoseDraw(*(s32*)((s32)entry + 0x70), 2, *(f32*)((s32)entry + 0x38), *(f32*)((s32)entry + 0x3C),
                     *(f32*)((s32)entry + 0x40), *(f32*)((s32)entry + 0x54), float_10_804201bc * *(f32*)((s32)entry + 0x44));
    } else {
        animPoseDraw(*(s32*)((s32)entry + 0x70), 3, *(f32*)((s32)entry + 0x38), *(f32*)((s32)entry + 0x3C),
                     *(f32*)((s32)entry + 0x40), *(f32*)((s32)entry + 0x54), float_10_804201bc * *(f32*)((s32)entry + 0x44));
    }

    if (*(u32*)entry & 0x400) {
        animSetPaperTexMtx(0, 0, 0);
        animSetPaperTexObj(0, 0, 0, 0, 0, 0, 1, 1);
    }
}

void mobjDisp_OffscreenXLU(s32 param_1, void* entry) {
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern void GXReadBoundingBox(u16* top, u16* bottom, u16* left, u16* right);
    extern void offscreenAddBoundingBox(s32 id, u16 top, u16 bottom, u16 left, u16 right);
    extern void animSetPaperTexMtx(void* mtx, s32 a, s32 b);
    extern void animSetPaperTexObj(void* texObj, s32 a, s32 b, s32 texMap,
                                   s32 d, s32 e, s32 f, s32 g);
    extern void animPoseDraw(s32 poseId, s32 mode, f32 x, f32 y, f32 z,
                             f32 rotY, f32 scale);
    extern f32 float_10_804201bc;
    u16 top;
    u16 bottom;
    u16 left;
    u16 right;
    f32 scale;

    sysWaitDrawSync();
    GXClearBoundingBox();
    if ((*(u32*)entry & 0x400) != 0) {
        animSetPaperTexMtx((void*)((s32)entry + 0x20C), 0, 0);
        animSetPaperTexObj((void*)((s32)entry + 0x1E8), 0, 0,
                           *(s32*)((s32)entry + 0x208), 0, 0, 1, 1);
    }

    scale = float_10_804201bc * *(f32*)((s32)entry + 0x44);
    animPoseDraw(*(s32*)((s32)entry + 0x70), 1,
                 *(f32*)((s32)entry + 0x38), *(f32*)((s32)entry + 0x3C),
                 *(f32*)((s32)entry + 0x40), *(f32*)((s32)entry + 0x54), scale);
    animPoseDraw(*(s32*)((s32)entry + 0x70), 2,
                 *(f32*)((s32)entry + 0x38), *(f32*)((s32)entry + 0x3C),
                 *(f32*)((s32)entry + 0x40), *(f32*)((s32)entry + 0x54), scale);
    animPoseDraw(*(s32*)((s32)entry + 0x70), 3,
                 *(f32*)((s32)entry + 0x38), *(f32*)((s32)entry + 0x3C),
                 *(f32*)((s32)entry + 0x40), *(f32*)((s32)entry + 0x54), scale);

    if ((*(u32*)entry & 0x400) != 0) {
        animSetPaperTexMtx(0, 0, 0);
        animSetPaperTexObj(0, 0, 0, 0, 0, 0, 1, 1);
    }
    sysWaitDrawSync();
    GXReadBoundingBox(&top, &bottom, &left, &right);
    offscreenAddBoundingBox(*(s32*)((s32)entry + 0x19C), top, bottom, left, right);
}

void mobjInit(void) {
    extern void* __memAlloc(s32 heap, u32 size);
    s32 count;

    count = 0x10;
    work.normal.count = count;
    work.normal.entries = __memAlloc(0, count * 0x23C);
    memset(work.normal.entries, 0, work.normal.count * 0x23C);

    count = 8;
    work.alt.count = count;
    work.alt.entries = __memAlloc(0, count * 0x23C);
    memset(work.alt.entries, 0, work.alt.count * 0x23C);

    koopaRunFlag = 0;
}

void mobjReset(s32 alt) {
    MobjSet* set = &work.normal;

    if (alt != 0) {
        set = &work.alt;
    }
    memset(set->entries, 0, set->count * 0x23C);
    koopaRunFlag = 0;
}

u8 mobjHitEntry(void* pMobj, int param_2) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern void* animPoseGetAnimBaseDataPtr(s32);
    extern void hitDelete(void*);
    extern void PSVECAdd(VecLocal*, VecLocal*, VecLocal*);
    extern void PSVECScale(VecLocal*, VecLocal*, double);
    extern u32 strlen(const char*);
    extern s32 strncmp(const char*, const char*, u32);
    extern void PSMTXTrans(void*, double, double, double);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, double, char);
    extern void PSMTXConcat(void*, void*, void*);
    extern void* memcpy(void*, const void*, u32);
    extern char* strcpy(char*, const char*);
    extern char* strcat(char*, const char*);
    extern void* hitEntryMOBJ(void*, void*);
    extern f32 float_0_804201b8;
    extern f32 float_10_804201bc;
    extern f32 float_deg2rad_804201cc;
    extern f32 float_2p5_804201d8;
    extern f32 float_0p5_804201f0;
    extern const char str_HOBJ_804201d0[];
    extern const char str_MOBJ_804201f4[];
    extern const char str_MOBJ_804201fc[];
    extern char headDJ[];
    extern char cubeDJ[];
    extern char frontDJ[];
    extern char front2DJ[];
    VecLocal min;
    VecLocal max;
    VecLocal pos;
    f32 m0[3][4];
    f32 m1[3][4];
    f32 m2[3][4];
    f32 m3[3][4];
    f32 m4[3][4];
    f32 result[3][4];
    void* poseData;
    void* hit;
    void* scan;
    s32 i;
    f32 diff;

    poseData = animPoseGetAnimBaseDataPtr(*(s32*)((s32)pMobj + 0x70));
    min.x = *(f32*)((s32)poseData + 0xD0);
    min.y = *(f32*)((s32)poseData + 0xD4);
    min.z = *(f32*)((s32)poseData + 0xD8);
    max.x = *(f32*)((s32)poseData + 0xDC);
    max.y = *(f32*)((s32)poseData + 0xE0);
    max.z = *(f32*)((s32)poseData + 0xE4);

    scan = pMobj;
    for (i = 0; i < 2; i++, scan = (void*)((s32)scan + 0x88)) {
        if (*(void**)((s32)scan + 0x74) != 0) {
            hitDelete((void*)((s32)scan + 0x78));
        }
    }

    PSVECAdd(&max, &min, (VecLocal*)((s32)pMobj + 0x190));
    PSVECScale((VecLocal*)((s32)pMobj + 0x190), (VecLocal*)((s32)pMobj + 0x190), (double)float_0p5_804201f0);
    PSVECScale((VecLocal*)((s32)pMobj + 0x190), (VecLocal*)((s32)pMobj + 0x190), (double)float_10_804201bc);
    *(f32*)((s32)pMobj + 0x194) = float_0_804201b8;
    diff = max.x - min.x;
    if (diff < 0.0f) {
        diff = -diff;
    }
    *(f32*)((s32)pMobj + 0x184) = float_10_804201bc * *(f32*)((s32)pMobj + 0x44) * diff;
    diff = max.y;
    if (diff < 0.0f) {
        diff = -diff;
    }
    *(f32*)((s32)pMobj + 0x188) = float_10_804201bc * *(f32*)((s32)pMobj + 0x48) * diff;
    diff = max.z - min.z;
    if (diff < 0.0f) {
        diff = -diff;
    }
    *(f32*)((s32)pMobj + 0x18C) = float_10_804201bc * *(f32*)((s32)pMobj + 0x4C) * diff;
    if (strncmp((const char*)((s32)poseData + 0), str_HOBJ_804201d0, strlen(str_HOBJ_804201d0)) == 0) {
        *(f32*)((s32)pMobj + 0x198) = float_0_804201b8;
        *(f32*)((s32)pMobj + 0x18C) = float_10_804201bc * float_2p5_804201d8 * *(f32*)((s32)pMobj + 0x4C);
    }

    if (param_2 >= 0 && param_2 < 3) {
        PSVECAdd((VecLocal*)((s32)pMobj + 0x38), (VecLocal*)((s32)pMobj + 0x190), &pos);
        PSMTXTrans(m0, (double)pos.x, (double)pos.y, (double)pos.z);
        PSMTXScale(m1, *(f32*)((s32)pMobj + 0x184), *(f32*)((s32)pMobj + 0x188), *(f32*)((s32)pMobj + 0x18C));
        PSMTXRotRad(m2, (double)(float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x50)), 'x');
        PSMTXRotRad(m3, (double)(float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x54)), 'y');
        PSMTXRotRad(m4, (double)(float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x58)), 'z');
        PSMTXConcat(m0, m4, m0);
        PSMTXConcat(m0, m3, m0);
        PSMTXConcat(m0, m2, m0);
        PSMTXConcat(m0, m1, result);
        if (param_2 == 1) {
            memcpy((void*)((s32)pMobj + 0x90), headDJ, 0x68);
            strcpy((char*)((s32)pMobj + 0x78), str_MOBJ_804201f4);
            strcat((char*)((s32)pMobj + 0x78), (char*)((s32)pMobj + 5));
            *(char**)((s32)pMobj + 0x90) = (char*)((s32)pMobj + 0x78);
            hit = hitEntryMOBJ((void*)((s32)pMobj + 0x90), result);
            *(void**)((s32)pMobj + 0x74) = hit;
            *(void**)((s32)hit + 0xD0) = pMobj;
        } else if (param_2 == 0) {
            memcpy((void*)((s32)pMobj + 0x90), cubeDJ, 0x68);
            strcpy((char*)((s32)pMobj + 0x78), str_MOBJ_804201f4);
            strcat((char*)((s32)pMobj + 0x78), (char*)((s32)pMobj + 5));
            *(char**)((s32)pMobj + 0x90) = (char*)((s32)pMobj + 0x78);
            hit = hitEntryMOBJ((void*)((s32)pMobj + 0x90), result);
            *(void**)((s32)pMobj + 0x74) = hit;
            *(void**)((s32)hit + 0xD0) = pMobj;
        } else {
            memcpy((void*)((s32)pMobj + 0x90), frontDJ, 0x68);
            strcpy((char*)((s32)pMobj + 0x78), str_MOBJ_804201f4);
            strcat((char*)((s32)pMobj + 0x78), (char*)((s32)pMobj + 5));
            *(char**)((s32)pMobj + 0x90) = (char*)((s32)pMobj + 0x78);
            hit = hitEntryMOBJ((void*)((s32)pMobj + 0x90), result);
            *(void**)((s32)pMobj + 0x74) = hit;
            *(void**)((s32)hit + 0xD0) = pMobj;
            memcpy((void*)((s32)pMobj + 0x118), front2DJ, 0x68);
            strcpy((char*)((s32)pMobj + 0x100), str_MOBJ_804201fc);
            strcat((char*)((s32)pMobj + 0x100), (char*)((s32)pMobj + 5));
            *(char**)((s32)pMobj + 0x118) = (char*)((s32)pMobj + 0x100);
            hit = hitEntryMOBJ((void*)((s32)pMobj + 0x118), result);
            *(void**)((s32)pMobj + 0xFC) = hit;
            *(void**)((s32)hit + 0xD0) = pMobj;
        }
    }
    return 0;
}

int mobjEntry(char* name, char* pAnimPoseName) {
    typedef struct MobjEntryLocal {
        u32 flags;
        u8 camId;
        char name[16];
        char animPoseName[16];
    } MobjEntryLocal;
    extern MobjWork work;
    extern void* gp;
    extern s32 koopaRunFlag;
    extern s32 strcmp(const char* a, const char* b);
    extern char* strcpy(char* dst, const char* src);
    extern int animPoseEntry(char* name, u32 inBattle);
    extern void animPosePeraOff(s32 poseId);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void animPoseSetMaterialLightFlagOn(s32 poseId, u32 flags);
    extern void mobjHitEntry(void* pMobj, s32 param_2);
    extern char vec3_802c1f58[];
    extern f32 float_1_804201c0;
    extern const char str_S_1_804201ec;
    MobjSet* set;
    MobjEntryLocal* entry;
    s32 count;
    s32 index;
    s32 poseId;
    s32 battle;

    if (koopaRunFlag != 0) {
        set = &work.koopa;
    } else {
        set = &work.normal;
        if (*(s32*)((s32)gp + 0x14) != 0) {
            set = &work.alt;
        }
    }

    count = set->count;
    index = 0;
    entry = (MobjEntryLocal*)set->entries;
    while (index < count) {
        if ((entry->flags & 1) && strcmp(entry->name, name) == 0) {
            break;
        }
        index++;
        entry = (MobjEntryLocal*)((s32)entry + 0x23C);
    }

    entry = (MobjEntryLocal*)set->entries;
    index = 0;
    if (count > 0) {
        do {
            if ((entry->flags & 1) == 0) {
                break;
            }
            index++;
            entry = (MobjEntryLocal*)((s32)entry + 0x23C);
            count--;
        } while (count != 0);
    }

    memset(entry, 0, 0x23C);
    entry->flags |= 1;
    strcpy(entry->name, name);
    *(u32*)((s32)entry + 0x38) = *(u32*)((s32)vec3_802c1f58 + 0x18);
    *(u32*)((s32)entry + 0x3C) = *(u32*)((s32)vec3_802c1f58 + 0x1C);
    *(u32*)((s32)entry + 0x40) = *(u32*)((s32)vec3_802c1f58 + 0x20);
    *(u32*)((s32)entry + 0x44) = *(u32*)((s32)vec3_802c1f58 + 0x24);
    *(u32*)((s32)entry + 0x48) = *(u32*)((s32)vec3_802c1f58 + 0x28);
    *(u32*)((s32)entry + 0x4C) = *(u32*)((s32)vec3_802c1f58 + 0x2C);
    *(u32*)((s32)entry + 0x50) = *(u32*)((s32)vec3_802c1f58 + 0x30);
    *(u32*)((s32)entry + 0x54) = *(u32*)((s32)vec3_802c1f58 + 0x34);
    *(u32*)((s32)entry + 0x58) = *(u32*)((s32)vec3_802c1f58 + 0x38);
    *(u32*)((s32)entry + 0x5C) = *(u32*)((s32)vec3_802c1f58 + 0x3C);
    *(u32*)((s32)entry + 0x60) = *(u32*)((s32)vec3_802c1f58 + 0x40);
    *(u32*)((s32)entry + 0x64) = *(u32*)((s32)vec3_802c1f58 + 0x44);
    *(f32*)((s32)entry + 0x68) = float_1_804201c0;
    *(f32*)((s32)entry + 0x6C) = float_1_804201c0;
    *(u8*)((s32)entry + 4) = 4;
    battle = *(s32*)((s32)gp + 0x14);
    poseId = animPoseEntry(pAnimPoseName, ((u32)(-battle) | (u32)battle) >> 31);
    *(s32*)((s32)entry + 0x70) = poseId;
    animPosePeraOff(*(s32*)((s32)entry + 0x70));
    strcpy(entry->animPoseName, pAnimPoseName);
    animPoseSetAnim(*(s32*)((s32)entry + 0x70), &str_S_1_804201ec, 1);
    animPoseSetMaterialLightFlagOn(*(s32*)((s32)entry + 0x70), 2);
    mobjHitEntry(entry, 0);
    *(s32*)((s32)entry + 0x19C) = -1;
    return index;
}

void mobjDelete(char* name) {
    extern s32 strcmp(const char* a, const char* b);
    extern void marioResetHitObj(void* hitObj);
    extern void hitDelete(void* hitObj);
    MobjSet* set;
    s32 i;
    s32 j;
    void* entry;
    void* hitObj;

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
        if ((*(u32*)entry & 1) && strcmp((char*)((s32)entry + 5), name) == 0) {
            hitObj = (void*)((s32)entry + 0x78);
            for (j = 0; j < 2; j++, hitObj = (void*)((s32)hitObj + 0x88)) {
                if (hitObj != 0) {
                    if (*(s32*)((s32)gp + 0x14) == 0) {
                        marioResetHitObj(hitObj);
                    }
                    hitDelete(hitObj);
                }
            }
            *(u32*)entry &= ~1U;
            break;
        }
    }
}

void mobjSetPosition(f32 posX, f32 posY, f32 posZ, char* mobjName) {
    extern f32 float_0p1_804201e8;
    extern s32 strcmp(const char* a, const char* b);
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
        if ((*(u32*)entry & 1) && strcmp((char*)((s32)entry + 5), mobjName) == 0) {
            break;
        }
    }

    *(f32*)((s32)entry + 0x38) = posX;
    *(f32*)((s32)entry + 0x3C) = posY + float_0p1_804201e8;
    *(f32*)((s32)entry + 0x40) = posZ;
}


void mobjMain(void) {
    typedef struct Vec3Local {
        f32 x;
        f32 y;
        f32 z;
    } Vec3Local;
    typedef f32 Mtx34Local[3][4];
    typedef struct AnimBaseLocal {
        s32 size;
        char fileName[64];
        char texFileName[64];
        char buildTime[64];
        s32 dispModeMask;
        s32 radius;
        s32 height;
        Vec3Local bboxMin;
        Vec3Local bboxMax;
    } AnimBaseLocal;
    typedef s32 (*MobjMainFunc)(void*);

    extern MobjWork work;
    extern void* gp;
    extern s32 koopaRunFlag;

    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void* fbatGetPointer(void);
    extern char* hitGetName(void* hit);
    extern s32 strcmp(const char* a, const char* b);

    extern s32 mobjCheckItemboxOpen(void* entry);
    extern void mobjCalcMtx(void* entry);
    extern void mobjCalcMtx2(void* entry);
    extern s32 hitCheckFilter(f64 x, f64 y, f64 z, f64 dx, f64 dy, f64 dz, s32 flags,
                              void* out0, f32* outY, void* out2, f32* dist, void* out4, void* out5, void* out6);
    extern f32 __fabsf(f32 value);
    extern void PSVECAdd(Vec3Local* a, Vec3Local* b, Vec3Local* out);
    extern void* animPoseGetAnimBaseDataPtr(s32 poseId);
    extern u32 strlen(const char* s);
    extern s32 strncmp(const char* a, const char* b, u32 n);
    extern void PSMTXTrans(Mtx34Local m, f32 x, f32 y, f32 z);
    extern void PSMTXScale(Mtx34Local m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx34Local m, char axis, f32 rad);
    extern void PSMTXConcat(Mtx34Local a, Mtx34Local b, Mtx34Local out);
    extern void hitReCalcMatrix(void* hitEntry, Mtx34Local mtx);

    extern void* kpaGetHitobjHead(s32 index);
    extern void* kpaGetHitobjRide(s32 index);
    extern s32 kpaHipAttackCheck(void);
    extern void* kpaGetHitobj(void);
    extern s32 kpaGetLevel(void);
    extern void* kpaGetHitFlame(void);
    extern s32 marioChkHipBump(void);
    extern s32 unk_8005ca2c(void);
    extern f32 PSVECDistance(void* a, void* b);
    extern s32 marioStGetSystemLevel(void);

    extern void* mapGetWork(void);
    extern void mapGetBlend(void* color);
    extern void mapGetBlend2(void* color);
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern s32 shadowEntry(f64 x, f64 y, f64 z, f64 size);
    extern void shadowSetType(s32 id, s32 type);
    extern void shadowSetSize(f64 size, s32 id);
    extern void animPoseMain(s32 poseId);
    extern u32 animPoseGetMaterialEvtColor(s32 poseId);
    extern void* camGetPtr(s32 camId);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSMTX44MultVec(void* mtx, void* src, void* dst);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 order);
    extern void mobjDisp(s32 cameraId, void* entry);
    extern void mobjDispXLU(s32 cameraId, void* entry);
    extern void mobjDisp_OffscreenXLU(s32 cameraId, void* entry);

    extern f32 float_0_804201b8;
    extern f32 float_1_804201c0;
    extern f32 float_10_804201bc;
    extern f32 float_neg1_804201c4;
    extern f32 float_neg0p5_804201c8;
    extern f32 float_5_804201dc;
    extern f32 float_20_804201e4;
    extern f32 float_25_804201e0;
    extern f64 double_1p5_802c2048;
    extern f64 double_1_802c2050;
    extern f64 double_2_802c2040;
    extern f32 float_2p5_804201d8;
    extern f32 float_deg2rad_804201cc;
    extern const char str_HOBJ_804201d0[];
    extern const char vec3_802c1f58[];

    MobjSet* set;
    void* player;
    void* party;
    void* fbat;
    void* newHeadHit;
    s32 partyUsing;
    s32 fbatMode;
    s32 count;
    s32 i;
    void* entry;
    const char* dataBase = vec3_802c1f58;

    if (koopaRunFlag != 0) {
        set = &work.koopa;
    } else {
        set = &work.normal;
        if (*(s32*)((s32)gp + 0x14) != 0) {
            set = &work.alt;
        }
    }

    count = set->count;
    entry = set->entries;

    player = marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    fbat = fbatGetPointer();
    fbatMode = *(s16*)fbat;
    newHeadHit = 0;
    partyUsing = 0;
    if (party != 0 && (*(u32*)party & 0x100) != 0) {
        partyUsing = 1;
    }

    if (*(s32*)((s32)gp + 0x14) == 0) {
        void* hit;
        char* hitName;

        hit = *(void**)((s32)player + 0x200);
        if (hit != 0) {
            if (*(void**)((s32)set + 0x08) == 0) {
                *(void**)((s32)set + 0x08) = *(void**)((s32)player + 0x200);
            } else {
                hitName = hitGetName(hit);
                if (strcmp(hitGetName(*(void**)((s32)set + 0x08)), hitName) != 0) {
                    *(void**)((s32)set + 0x08) = *(void**)((s32)player + 0x200);
                }
            }
        } else {
            *(void**)((s32)set + 0x08) = 0;
        }

        hit = *(void**)((s32)player + 0x1F8);
        if (hit != 0) {
            if (*(void**)((s32)set + 0x0C) == 0) {
                *(void**)((s32)set + 0x0C) = *(void**)((s32)player + 0x1F8);
                newHeadHit = *(void**)((s32)set + 0x0C);
            } else {
                hitName = hitGetName(hit);
                if (strcmp(hitGetName(*(void**)((s32)set + 0x0C)), hitName) != 0) {
                    *(void**)((s32)set + 0x0C) = *(void**)((s32)player + 0x1F8);
                    newHeadHit = *(void**)((s32)set + 0x0C);
                }
            }
        } else {
            *(void**)((s32)set + 0x0C) = 0;
        }

        hit = *(void**)((s32)player + 0x1E8);
        if (hit != 0) {
            if (*(void**)((s32)set + 0x10) == 0) {
                *(void**)((s32)set + 0x10) = *(void**)((s32)player + 0x1E8);
            } else {
                hitName = hitGetName(hit);
                if (strcmp(hitGetName(*(void**)((s32)set + 0x10)), hitName) != 0) {
                    *(void**)((s32)set + 0x10) = *(void**)((s32)player + 0x1E8);
                }
            }
        } else {
            *(void**)((s32)set + 0x10) = 0;
        }

        hit = *(void**)((s32)player + 0x1E4);
        if (hit != 0) {
            if (*(void**)((s32)set + 0x14) == 0) {
                *(void**)((s32)set + 0x14) = *(void**)((s32)player + 0x1E4);
            } else {
                hitName = hitGetName(hit);
                if (strcmp(hitGetName(*(void**)((s32)set + 0x14)), hitName) != 0) {
                    *(void**)((s32)set + 0x14) = *(void**)((s32)player + 0x1E4);
                }
            }
        } else {
            *(void**)((s32)set + 0x14) = 0;
        }

        if (party == 0 || *(void**)((s32)party + 0x130) == 0) {
            *(void**)((s32)set + 0x18) = 0;
        } else {
            hit = *(void**)((s32)party + 0x130);
            if (*(void**)((s32)set + 0x18) == 0) {
                *(void**)((s32)set + 0x18) = *(void**)((s32)party + 0x130);
            } else {
                hitName = hitGetName(hit);
                if (strcmp(hitGetName(*(void**)((s32)set + 0x18)), hitName) != 0) {
                    *(void**)((s32)set + 0x18) = *(void**)((s32)party + 0x130);
                }
            }
        }
    }

    for (i = 0; i < count; i++, entry = (void*)((s32)entry + 0x23C)) {
        u32 flags = *(u32*)entry;
        s32 skipDisplay = 0;

        if ((flags & 1) != 0) {
            if (*(s32*)((s32)gp + 0x14) != 0 || fbatMode < 2 || fbatMode == 8 || (flags & 0x400) != 0) {
                if ((flags & 4) != 0) {
                    s32 itemboxOpen = 0;
                    if (strcmp((char*)((s32)entry + 0x15), dataBase + 0x8C) == 0 ||
                        strcmp((char*)((s32)entry + 0x15), dataBase + 0xA0) == 0 ||
                        strcmp((char*)((s32)entry + 0x15), dataBase + 0xB4) == 0 ||
                        strcmp((char*)((s32)entry + 0x15), dataBase + 0xCC) == 0) {
                        itemboxOpen = (*(s32*)((s32)entry + 0x1DC) == 99) + 1;
                    }
                    if (itemboxOpen == 2) {
                            Vec3Local calcTrans;
                            Mtx34Local calcTransMtx;
                            Mtx34Local calcScaleMtx;
                            Mtx34Local calcRotXMtx;
                            Mtx34Local calcRotYMtx;
                            Mtx34Local calcRotZMtx;
                            Mtx34Local calcOutMtx;
                            s32 calcI;
                            void* calcEntry;

                            PSVECAdd((Vec3Local*)((s32)entry + 0x38), (Vec3Local*)((s32)entry + 0x190), &calcTrans);
                            PSMTXTrans(calcTransMtx, calcTrans.x, calcTrans.y, calcTrans.z);
                            PSMTXScale(calcScaleMtx, *(f32*)((s32)entry + 0x184), *(f32*)((s32)entry + 0x188), *(f32*)((s32)entry + 0x18C));
                            PSMTXRotRad(calcRotXMtx, 'x', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x50));
                            PSMTXRotRad(calcRotYMtx, 'y', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x54));
                            PSMTXRotRad(calcRotZMtx, 'z', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x58));
                            PSMTXConcat(calcTransMtx, calcRotZMtx, calcTransMtx);
                            PSMTXConcat(calcTransMtx, calcRotYMtx, calcTransMtx);
                            PSMTXConcat(calcTransMtx, calcRotXMtx, calcTransMtx);
                            PSMTXConcat(calcTransMtx, calcScaleMtx, calcOutMtx);

                            calcEntry = entry;
                            for (calcI = 0; calcI < 2; calcI++) {
                                if (*(void**)((s32)calcEntry + 0x74) != 0) {
                                    hitReCalcMatrix(*(void**)((s32)calcEntry + 0x74), calcOutMtx);
                                }
                                calcEntry = (void*)((s32)calcEntry + 0x88);
                            }
                        } else {
                            Vec3Local calcTrans;
                            Mtx34Local calcTransMtx;
                            Mtx34Local calcScaleMtx;
                            Mtx34Local calcRotXMtx;
                            Mtx34Local calcRotYMtx;
                            Mtx34Local calcRotZMtx;
                            Mtx34Local calcOutMtx;
                            AnimBaseLocal* calcBase;
                            s32 calcI;
                            void* calcEntry;

                            PSVECAdd((Vec3Local*)((s32)entry + 0x38), (Vec3Local*)((s32)entry + 0x190), &calcTrans);
                            calcBase = animPoseGetAnimBaseDataPtr(*(s32*)((s32)entry + 0x70));
                            *(f32*)((s32)entry + 0x184) = float_10_804201bc * *(f32*)((s32)entry + 0x44) *
                                                          __fabsf(calcBase->bboxMax.x - calcBase->bboxMin.x);
                            *(f32*)((s32)entry + 0x188) = float_10_804201bc * *(f32*)((s32)entry + 0x48) *
                                                          __fabsf(calcBase->bboxMax.y);
                            *(f32*)((s32)entry + 0x18C) = float_10_804201bc * *(f32*)((s32)entry + 0x4C) *
                                                          __fabsf(calcBase->bboxMax.z - calcBase->bboxMin.z);

                            if (strncmp(calcBase->fileName, str_HOBJ_804201d0, strlen(str_HOBJ_804201d0)) == 0) {
                                *(f32*)((s32)entry + 0x198) = float_0_804201b8;
                                *(f32*)((s32)entry + 0x18C) = float_10_804201bc * float_2p5_804201d8 * *(f32*)((s32)entry + 0x4C);
                            }

                            PSMTXTrans(calcTransMtx, calcTrans.x, calcTrans.y, calcTrans.z);
                            PSMTXScale(calcScaleMtx, *(f32*)((s32)entry + 0x184), *(f32*)((s32)entry + 0x188), *(f32*)((s32)entry + 0x18C));
                            PSMTXRotRad(calcRotXMtx, 'x', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x50));
                            PSMTXRotRad(calcRotYMtx, 'y', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x54));
                            PSMTXRotRad(calcRotZMtx, 'z', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x58));
                            PSMTXConcat(calcTransMtx, calcRotZMtx, calcTransMtx);
                            PSMTXConcat(calcTransMtx, calcRotYMtx, calcTransMtx);
                            PSMTXConcat(calcTransMtx, calcRotXMtx, calcTransMtx);
                            PSMTXConcat(calcTransMtx, calcScaleMtx, calcOutMtx);

                            calcEntry = entry;
                            for (calcI = 0; calcI < 2; calcI++) {
                                if (*(void**)((s32)calcEntry + 0x74) != 0) {
                                    hitReCalcMatrix(*(void**)((s32)calcEntry + 0x74), calcOutMtx);
                                }
                                calcEntry = (void*)((s32)calcEntry + 0x88);
                            }
                        }
                    *(u32*)entry &= ~4U;
                    flags = *(u32*)entry;
                }

                if ((flags & 0x40) != 0) {
                    f32 velY = *(f32*)((s32)entry + 0x60);
                    if (velY < float_0_804201b8) {
                        u32 out0;
                        f32 outY;
                        u32 out2;
                        f32 checkDist;
                        u32 out4;
                        u32 out5;
                        u32 out6;
                        checkDist = float_10_804201bc + float_1_804201c0 + __fabsf(velY);
                        if (hitCheckFilter(*(f32*)((s32)entry + 0x38), float_10_804201bc + *(f32*)((s32)entry + 0x3C),
                                           *(f32*)((s32)entry + 0x40), float_0_804201b8, float_neg1_804201c4,
                                           float_0_804201b8, 0, &out0, &outY, &out2, &checkDist, &out4, &out5, &out6) != 0) {
                            *(f32*)((s32)entry + 0x3C) = outY;
                            *(f32*)((s32)entry + 0x60) = float_neg0p5_804201c8 * *(f32*)((s32)entry + 0x6C) * velY;
                            if ((f64)__fabsf(*(f32*)((s32)entry + 0x60)) <= double_2_802c2040) {
                                *(f32*)((s32)entry + 0x60) = float_0_804201b8;
                            }
                        } else {
                            *(f32*)((s32)entry + 0x3C) += velY;
                            *(f32*)((s32)entry + 0x60) = velY - *(f32*)((s32)entry + 0x68);
                        }
                    } else {
                        *(f32*)((s32)entry + 0x3C) += velY;
                        *(f32*)((s32)entry + 0x60) = velY - *(f32*)((s32)entry + 0x68);
                    }
{
                                        Vec3Local calcTrans;
                    Mtx34Local calcTransMtx;
                    Mtx34Local calcScaleMtx;
                    Mtx34Local calcRotXMtx;
                    Mtx34Local calcRotYMtx;
                    Mtx34Local calcRotZMtx;
                    Mtx34Local calcOutMtx;
                    AnimBaseLocal* calcBase;
                    s32 calcI;
                    void* calcEntry;

                    PSVECAdd((Vec3Local*)((s32)entry + 0x38), (Vec3Local*)((s32)entry + 0x190), &calcTrans);
                    calcBase = animPoseGetAnimBaseDataPtr(*(s32*)((s32)entry + 0x70));
                    *(f32*)((s32)entry + 0x184) = float_10_804201bc * *(f32*)((s32)entry + 0x44) *
                                                  __fabsf(calcBase->bboxMax.x - calcBase->bboxMin.x);
                    *(f32*)((s32)entry + 0x188) = float_10_804201bc * *(f32*)((s32)entry + 0x48) *
                                                  __fabsf(calcBase->bboxMax.y);
                    *(f32*)((s32)entry + 0x18C) = float_10_804201bc * *(f32*)((s32)entry + 0x4C) *
                                                  __fabsf(calcBase->bboxMax.z - calcBase->bboxMin.z);

                    if (strncmp(calcBase->fileName, str_HOBJ_804201d0, strlen(str_HOBJ_804201d0)) == 0) {
                                            *(f32*)((s32)entry + 0x198) = float_0_804201b8;
                        *(f32*)((s32)entry + 0x18C) = float_10_804201bc * float_2p5_804201d8 * *(f32*)((s32)entry + 0x4C);
                    }

                    PSMTXTrans(calcTransMtx, calcTrans.x, calcTrans.y, calcTrans.z);
                    PSMTXScale(calcScaleMtx, *(f32*)((s32)entry + 0x184), *(f32*)((s32)entry + 0x188), *(f32*)((s32)entry + 0x18C));
                    PSMTXRotRad(calcRotXMtx, 'x', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x50));
                    PSMTXRotRad(calcRotYMtx, 'y', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x54));
                    PSMTXRotRad(calcRotZMtx, 'z', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x58));
                    PSMTXConcat(calcTransMtx, calcRotZMtx, calcTransMtx);
                    PSMTXConcat(calcTransMtx, calcRotYMtx, calcTransMtx);
                    PSMTXConcat(calcTransMtx, calcRotXMtx, calcTransMtx);
                    PSMTXConcat(calcTransMtx, calcScaleMtx, calcOutMtx);

                    calcEntry = entry;
                    for (calcI = 0; calcI < 2; calcI++) {
                                            if (*(void**)((s32)calcEntry + 0x74) != 0) {
                                                hitReCalcMatrix(*(void**)((s32)calcEntry + 0x74), calcOutMtx);
                        }
                        calcEntry = (void*)((s32)calcEntry + 0x88);
                    }
                        }
                    flags = *(u32*)entry;
                }

                if ((flags & 0x8) == 0) {
                    void* hit = *(void**)((s32)entry + 0x74);
                    s32 activated = 0;
                    u32 activation = 0;

                    if (*(s8*)((s32)player + 0x3C) == 2) {
                        s32 j;
                        if ((flags & 0x04000000) != 0) {
                            for (j = 0; j < 10; j++) {
                                if (kpaGetHitobjHead(j) == hit) {
                                    activated = 1;
                                    activation = 0x04000000;
                                    break;
                                }
                            }
                        }
                        if (!activated && (flags & 0x10000000) != 0) {
                            for (j = 0; j < 10; j++) {
                                if (kpaGetHitobjRide(j) == hit && kpaHipAttackCheck() != 0) {
                                    activated = 1;
                                    activation = 0x10000000;
                                    break;
                                }
                            }
                        }
                        if (!activated && (flags & 0x08000000) != 0 && kpaGetHitobj() == hit && kpaGetLevel() == 3) {
                            activated = 1;
                            activation = 0x08000000;
                        } else if (!activated && (flags & 0x20000000) != 0 && kpaGetHitFlame() == hit) {
                            activated = 1;
                            activation = 0x20000000;
                        } else if (!activated && (flags & 0x00080000) != 0 && *(void**)((s32)player + 0x1E4) == hit) {
                            activated = 1;
                            activation = 0x00080000;
                        } else if (!activated && (flags & 0x00200000) != 0 && *(void**)((s32)player + 0x1E8) == hit) {
                            activated = 1;
                            activation = 0x00200000;
                        }
                    } else if ((flags & 0x00010000) != 0 && *(void**)((s32)player + 0x200) == hit &&
                               *(f32*)((s32)marioGetPtr() + 0x90) <= float_5_804201dc + *(f32*)((s32)entry + 0x3C)) {
                        activated = 1;
                        activation = 0x00010000;
                    } else if ((flags & 0x00400000) != 0 && newHeadHit == hit) {
                        activated = 1;
                        activation = 0x00400000;
                    } else if ((flags & 0x00200000) != 0 && *(void**)((s32)player + 0x1E8) == hit) {
                        activated = 1;
                        activation = 0x00200000;
                    } else if ((flags & 0x01000000) != 0 && *(void**)((s32)player + 0x1E8) == hit && marioChkHipBump() != 0) {
                        activated = 1;
                        activation = 0x01000000;
                    } else if (unk_8005ca2c() != 0 && (flags & 0x00040000) != 0 && *(void**)((s32)player + 0x1E4) == hit &&
                               (*(u16*)((s32)player + 0x24C) & 0x100) != 0 && partyUsing == 0) {
                        activated = 1;
                        activation = 0x00040000;
                    } else if ((flags & 0x00800000) != 0 && party != 0) {
                        void* partyHit = *(void**)((s32)party + 0x130);
                        if (partyHit == hit) {
                            activated = 1;
                            activation = 0x00800000;
                        } else if (partyHit != 0 && *(u8*)((s32)party + 0x31) == 3 &&
                                   PSVECDistance((void*)((s32)party + 0x58), (void*)((s32)entry + 0x38)) <
                                       *(f32*)((s32)*(void**)((s32)entry + 0x74) + 0xCC) + *(f32*)((s32)party + 0xF4)) {
                            activated = 1;
                            activation = 0x00800000;
                        }
                    } else if ((flags & 0x02000000) != 0 && party != 0 && *(u8*)((s32)party + 0x31) == 3) {
                        void* partyHit = *(void**)((s32)party + 0x130);
                        if (partyHit == hit) {
                            activated = 1;
                            activation = 0x02000000;
                        } else if (partyHit != 0 &&
                                   PSVECDistance((void*)((s32)party + 0x58), (void*)((s32)entry + 0x38)) <
                                       *(f32*)((s32)*(void**)((s32)entry + 0x74) + 0xCC) + *(f32*)((s32)party + 0xF4)) {
                            activated = 1;
                            activation = 0x00800000;
                        }
                    }

                    if (activated != 0) {
                        *(u32*)((s32)entry + 0x1E0) = activation;
                        *(u32*)entry |= 0x8;
                    }
                }

                if ((*(u32*)entry & 0x100) != 0) {
                    *(u32*)entry &= ~0x8U;
                }
                if (marioStGetSystemLevel() != 0) {
                    *(u32*)entry &= ~0x8U;
                }
                {
                    void* checkPlayer = marioGetPtr();
                    u16 motion = *(u16*)((s32)checkPlayer + 0x2E);
                    if (motion == 0x1F || motion == 0x20) {
                        *(u32*)entry &= ~0x8U;
                    }
                }
                if (*(void**)((s32)entry + 0x1D0) != 0) {
                    MobjMainFunc fn = *(MobjMainFunc*)((s32)entry + 0x1D0);
                    if (fn(entry) == -1) {
                        skipDisplay = 1;
                    }
                }
            }

            if (!skipDisplay && (*(u32*)entry & 1) != 0) {
                u32 materialColor;
                if (koopaRunFlag == 0 && ((*(u32*)entry & 0x802) == 0)) {
                    void* map = mapGetWork();
                    u32 blend;
                    u32 blend2;
                    s32 allowShadow = 1;
                    mapGetBlend(&blend);
                    mapGetBlend2(&blend2);
                    if ((*(u16*)((s32)map + 4) & 2) != 0) {
                        if ((animPoseGetMaterialFlag(*(s32*)((s32)entry + 0x70)) & 0x800) == 0 && ((u8*)&blend)[3] != 0xFF) {
                            allowShadow = 0;
                        }
                    }
                    if (allowShadow && ((*(u16*)((s32)map + 4) & 4) != 0)) {
                        if ((animPoseGetMaterialFlag(*(s32*)((s32)entry + 0x70)) & 0x1000) == 0 && ((u8*)&blend2)[3] != 0xFF) {
                            allowShadow = 0;
                        }
                    }
                    if (allowShadow) {
                        s32 shadowId = shadowEntry(*(f32*)((s32)entry + 0x38), *(f32*)((s32)entry + 0x3C), *(f32*)((s32)entry + 0x40),
                                                  (float_10_804201bc * *(f32*)((s32)entry + 0x44) * *(f32*)((s32)entry + 0x184)) / float_25_804201e0);
                        if ((*(u32*)entry & 0x00400000) != 0) {
                            shadowSetType(shadowId, 1);
                            shadowSetSize((float_10_804201bc * *(f32*)((s32)entry + 0x44) * *(f32*)((s32)entry + 0x184)) / float_20_804201e4, shadowId);
                        }
                    }
                }

                animPoseMain(*(s32*)((s32)entry + 0x70));
                materialColor = animPoseGetMaterialEvtColor(*(s32*)((s32)entry + 0x70));

                if ((*(u32*)entry & 0x4000) == 0) {
                    s32 visible = 1;
                    if ((*(u32*)entry & 0x80) == 0) {
                        Vec3Local clip;
                        void* cam = camGetPtr(4);
                        PSMTXMultVec((void*)((s32)cam + 0x11C), (void*)((s32)entry + 0x38), &clip);
                        cam = camGetPtr(4);
                        PSMTX44MultVec((void*)((s32)cam + 0x15C), &clip, &clip);
                        if ((f64)__fabsf(clip.x) > double_1p5_802c2048 ||
                            (f64)__fabsf(clip.y) > double_1p5_802c2048 ||
                            (f64)__fabsf(clip.z) > double_1_802c2050) {
                            visible = 0;
                        }
                    }
                    if (visible && (((*(u32*)entry & 0x2000) == 0) || (((*(u32*)((s32)gp + 0x1C) >> 3) & 1) == 0))) {
                        u32 matFlags = animPoseGetMaterialFlag(*(s32*)((s32)entry + 0x70));
                        s32 xlu = ((matFlags & 0x40) != 0 && ((u8*)&materialColor)[3] != 0xFF);
                        if (*(s32*)((s32)entry + 0x19C) == -1) {
                            if (xlu) {
                                dispEntry(*(u8*)((s32)entry + 4), 2, mobjDispXLU, entry, dispCalcZ((void*)((s32)entry + 0x38)));
                            } else {
                                dispEntry(*(u8*)((s32)entry + 4), 0, mobjDisp, entry, dispCalcZ((void*)((s32)entry + 0x38)));
                                dispEntry(*(u8*)((s32)entry + 4), 1, mobjDisp, entry, dispCalcZ((void*)((s32)entry + 0x38)));
                                dispEntry(*(u8*)((s32)entry + 4), 2, mobjDisp, entry, dispCalcZ((void*)((s32)entry + 0x38)));
                            }
                        } else {
                            if (xlu) {
                                dispEntry(1, 2, mobjDisp_OffscreenXLU, entry, dispCalcZ((void*)((s32)entry + 0x38)));
                            } else {
                                dispEntry(1, 0, mobjDisp_OffscreenXLU, entry, dispCalcZ((void*)((s32)entry + 0x38)));
                                dispEntry(1, 1, mobjDisp_OffscreenXLU, entry, dispCalcZ((void*)((s32)entry + 0x38)));
                                dispEntry(1, 2, mobjDisp_OffscreenXLU, entry, dispCalcZ((void*)((s32)entry + 0x38)));
                            }
                        }
                    }
                }
            }
        }
    }
}
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


void* mobjHitObjPtrToPtr(void* hitObj) {
    if (*(u32*)((s32)hitObj + 4) & 0x80000000) {
        return *(void**)((s32)hitObj + 0xD0);
    }
    return 0;
}

void* mobjRunEvent(void* pMobj, void* evtCode) {
    extern s32 evtCheckID(s32 id);
    extern void* evtEntryType(void* evtCode, s32 priority, s32 flags, s32 type);
    void* event;

    if (evtCode == 0) {
        return 0;
    }
    if (evtCheckID(*(s32*)((s32)pMobj + 0x1CC)) != 0) {
        return 0;
    }
    *(s32*)((s32)pMobj + 0x1CC) = 0;
    event = evtEntryType(evtCode, 0x1E, 0, 0x1A);
    *(void**)((s32)event + 0x174) = pMobj;
    *(s32*)((s32)pMobj + 0x1CC) = *(s32*)((s32)event + 0x15C);
    return event;
}

void mobjCalcMtx(void* pMobj) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef f32 Mtx34[3][4];
    typedef struct AnimBaseLocal {
        s32 size;
        char fileName[64];
        char texFileName[64];
        char buildTime[64];
        s32 dispModeMask;
        s32 radius;
        s32 height;
        Vec3 bboxMin;
        Vec3 bboxMax;
    } AnimBaseLocal;
    extern void PSVECAdd(Vec3* a, Vec3* b, Vec3* out);
    extern void* animPoseGetAnimBaseDataPtr(s32 poseId);
    extern u32 strlen(const char* s);
    extern s32 strncmp(const char* a, const char* b, u32 n);
    extern void PSMTXTrans(Mtx34 m, f32 x, f32 y, f32 z);
    extern void PSMTXScale(Mtx34 m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx34 m, char axis, f32 rad);
    extern void PSMTXConcat(Mtx34 a, Mtx34 b, Mtx34 out);
    extern void hitReCalcMatrix(void* hitEntry, Mtx34 mtx);
    extern f32 __fabsf(f32 value);
    extern f32 float_10_804201bc;
    extern f32 float_0_804201b8;
    extern f32 float_2p5_804201d8;
    extern f32 float_deg2rad_804201cc;
    extern const char str_HOBJ_804201d0[];
    Vec3 trans;
    Mtx34 transMtx;
    Mtx34 scaleMtx;
    Mtx34 rotXMtx;
    Mtx34 rotYMtx;
    Mtx34 rotZMtx;
    Mtx34 outMtx;
    AnimBaseLocal* base;
    s32 i;

    PSVECAdd((Vec3*)((s32)pMobj + 0x38), (Vec3*)((s32)pMobj + 0x190), &trans);
    base = animPoseGetAnimBaseDataPtr(*(s32*)((s32)pMobj + 0x70));
    *(f32*)((s32)pMobj + 0x184) = float_10_804201bc * *(f32*)((s32)pMobj + 0x44) * __fabsf(base->bboxMax.x - base->bboxMin.x);
    *(f32*)((s32)pMobj + 0x188) = float_10_804201bc * *(f32*)((s32)pMobj + 0x48) * __fabsf(base->bboxMax.y);
    *(f32*)((s32)pMobj + 0x18C) = float_10_804201bc * *(f32*)((s32)pMobj + 0x4C) * __fabsf(base->bboxMax.z - base->bboxMin.z);

    if (strncmp(base->fileName, str_HOBJ_804201d0, strlen(str_HOBJ_804201d0)) == 0) {
        *(f32*)((s32)pMobj + 0x198) = float_0_804201b8;
        *(f32*)((s32)pMobj + 0x18C) = float_10_804201bc * float_2p5_804201d8 * *(f32*)((s32)pMobj + 0x4C);
    }

    PSMTXTrans(transMtx, trans.x, trans.y, trans.z);
    PSMTXScale(scaleMtx, *(f32*)((s32)pMobj + 0x184), *(f32*)((s32)pMobj + 0x188), *(f32*)((s32)pMobj + 0x18C));
    PSMTXRotRad(rotXMtx, 'x', float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x50));
    PSMTXRotRad(rotYMtx, 'y', float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x54));
    PSMTXRotRad(rotZMtx, 'z', float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x58));
    PSMTXConcat(transMtx, rotZMtx, transMtx);
    PSMTXConcat(transMtx, rotYMtx, transMtx);
    PSMTXConcat(transMtx, rotXMtx, transMtx);
    PSMTXConcat(transMtx, scaleMtx, outMtx);

    for (i = 0; i < 2; i++) {
        if (*(void**)((s32)pMobj + 0x74) != 0) {
            hitReCalcMatrix(*(void**)((s32)pMobj + 0x74), outMtx);
        }
        pMobj = (void*)((s32)pMobj + 0x88);
    }
}

void mobjCalcMtx2(void* pMobj) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef f32 Mtx34[3][4];
    extern void PSVECAdd(Vec3* a, Vec3* b, Vec3* out);
    extern void PSMTXTrans(Mtx34 m, f32 x, f32 y, f32 z);
    extern void PSMTXScale(Mtx34 m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx34 m, char axis, f32 rad);
    extern void PSMTXConcat(Mtx34 a, Mtx34 b, Mtx34 out);
    extern void hitReCalcMatrix(void* hitEntry, Mtx34 mtx);
    extern f32 float_deg2rad_804201cc;
    Vec3 trans;
    Mtx34 transMtx;
    Mtx34 scaleMtx;
    Mtx34 rotXMtx;
    Mtx34 rotYMtx;
    Mtx34 rotZMtx;
    Mtx34 outMtx;
    s32 i;

    PSVECAdd((Vec3*)((s32)pMobj + 0x38), (Vec3*)((s32)pMobj + 0x190), &trans);
    PSMTXTrans(transMtx, trans.x, trans.y, trans.z);
    PSMTXScale(scaleMtx, *(f32*)((s32)pMobj + 0x184), *(f32*)((s32)pMobj + 0x188), *(f32*)((s32)pMobj + 0x18C));
    PSMTXRotRad(rotXMtx, 'x', float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x50));
    PSMTXRotRad(rotYMtx, 'y', float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x54));
    PSMTXRotRad(rotZMtx, 'z', float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x58));
    PSMTXConcat(transMtx, rotZMtx, transMtx);
    PSMTXConcat(transMtx, rotYMtx, transMtx);
    PSMTXConcat(transMtx, rotXMtx, transMtx);
    PSMTXConcat(transMtx, scaleMtx, outMtx);

    for (i = 0; i < 2; i++) {
        if (*(void**)((s32)pMobj + 0x74) != 0) {
            hitReCalcMatrix(*(void**)((s32)pMobj + 0x74), outMtx);
        }
        pMobj = (void*)((s32)pMobj + 0x88);
    }
}

s32 mobjGetHint(void* mobj) {
    if (mobj == 0) {
        return 0;
    }
    return (*(u32*)mobj >> 5) & 1;
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
s32 mobjCheckItemboxOpen(void* entry) {
    extern s32 strcmp(const char* a, const char* b);
    extern const char str_MOBJ_TreasureBox_802c1fe4[];
    extern const char str_MOBJ_BigTreasureBox_802c1ff8[];
    extern const char str_MOBJ_GrayTreasureBox_802c200c[];
    extern const char str_MOBJ_BlackTreasureBo_802c2024[];

    if (strcmp((char*)((s32)entry + 0x15), str_MOBJ_TreasureBox_802c1fe4) == 0 ||
        strcmp((char*)((s32)entry + 0x15), str_MOBJ_BigTreasureBox_802c1ff8) == 0 ||
        strcmp((char*)((s32)entry + 0x15), str_MOBJ_GrayTreasureBox_802c200c) == 0 ||
        strcmp((char*)((s32)entry + 0x15), str_MOBJ_BlackTreasureBo_802c2024) == 0) {
        return (*(s32*)((s32)entry + 0x1DC) == 99) + 1;
    }
    return 0;
}

u32* mobjNearDistCheck2(double param_1, double param_2, double param_3, double param_4, char** param_5) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern MobjWork work;
    extern void* gp;
    extern s32 koopaRunFlag;
    extern s32 strcmp(const char* a, const char* b);
    extern void* itemNameToPtr(char* name);
    extern u32 evtGetValue(void* event, s32 value);
    extern void PSVECSubtract(Vec3* a, Vec3* b, Vec3* out);
    extern f32 PSVECMag(Vec3* vec);
    extern const char str_MOBJ_TreasureBox_802c1fe4[];
    extern const char str_MOBJ_BigTreasureBox_802c1ff8[];
    extern const char str_MOBJ_GrayTreasureBox_802c200c[];
    extern const char str_MOBJ_BlackTreasureBo_802c2024[];
    extern const char str_MOBJ_KururinFloor_802c1fd0[];
    MobjSet* set;
    s32 i;
    s32 count;
    u32* entry;
    u32* best;
    char** list;
    Vec3 pos;
    Vec3 diff;
    f32 dist;
    s32 check;
    void* item;
    u32 value;

    if (koopaRunFlag != 0) {
        set = &work.koopa;
    } else {
        set = &work.normal;
        if (*(s32*)((s32)gp + 0x14) != 0) {
            set = &work.alt;
        }
    }

    best = 0;
    i = 0;
    count = set->count;
    entry = (u32*)set->entries;
    pos.x = (f32)param_1;
    pos.y = (f32)param_2;
    pos.z = (f32)param_3;

    while (i < count) {
        if ((*entry & 1) != 0) {
            list = param_5;
            while (*list != 0) {
                if (strcmp((char*)((s32)entry + 0x15), *list) == 0) {
                    if (strcmp((char*)((s32)entry + 0x15), str_MOBJ_TreasureBox_802c1fe4) == 0 ||
                        strcmp((char*)((s32)entry + 0x15), str_MOBJ_BigTreasureBox_802c1ff8) == 0 ||
                        strcmp((char*)((s32)entry + 0x15), str_MOBJ_GrayTreasureBox_802c200c) == 0 ||
                        strcmp((char*)((s32)entry + 0x15), str_MOBJ_BlackTreasureBo_802c2024) == 0) {
                        if (*(u32*)((s32)entry + 0x1DC) == 99) {
                            check = 2;
                        } else {
                            check = 1;
                        }
                        if (check != 1) {
                            list++;
                            continue;
                        }
                    } else if (strcmp((char*)((s32)entry + 0x15), str_MOBJ_KururinFloor_802c1fd0) == 0) {
                        check = 0;
                        if (strcmp((char*)((s32)entry + 0x15), str_MOBJ_KururinFloor_802c1fd0) == 0) {
                            item = itemNameToPtr((char*)((s32)entry + 5));
                            if (item == 0 || ((*(u16*)item & 1) == 0) || evtGetValue(0, *(s32*)((s32)entry + 0x1E4)) != 0) {
                                value = evtGetValue(0, *(s32*)((s32)entry + 0x1E4));
                                if (value == 0) {
                                    check = 2;
                                } else {
                                    check = 1;
                                }
                            } else {
                                check = 1;
                            }
                        }
                        if (check != 2) {
                            list++;
                            continue;
                        }
                    }

                    PSVECSubtract((Vec3*)((s32)entry + 0x38), &pos, &diff);
                    dist = PSVECMag(&diff);
                    if ((double)dist < param_4) {
                        best = entry;
                        param_4 = (double)dist;
                    }
                    break;
                }
                list++;
            }
        }
        i++;
        entry = (u32*)((s32)entry + 0x23C);
    }
    return best;
}

