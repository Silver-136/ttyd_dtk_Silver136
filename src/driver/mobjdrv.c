#include "driver/mobjdrv.h"
f64 __fabs(f64 value);
#include "bowser/koopa_hit.h"
#include "driver/dispdrv.h"

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

u32 gap_08_8041E924_sbss;
s32 koopaRunFlag;
static MobjWork work;

u32 cubeMSJ[5] = {
    0x00010000, 0x00000000, 0x80000000, 0x00000000, 0x00000000,
};
u32 front2MSJ[5] = {
    0x00010000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
};

const char vec3_802c1f58[12] = {0};
const char vec3_802c1f64[12] = {0};
const char vec3_802c1f70[12] = {0};
const u32 vec3_802c1f7c[3] = { 0x3F800000, 0x3F800000, 0x3F800000 };
const char vec3_802c1f88[12] = {0};
const char vec3_802c1f94[12] = {0};
const char vec3_802c1fa0[12] = {0};
const char vec3_802c1fac[12] = {0};
const char vec3_802c1fb8[12] = {0};
const char vec3_802c1fc4[12] = {0};

const char str_MOBJ_KururinFloor_802c1fd0[] = "MOBJ_KururinFloor";
const char str_MOBJ_TreasureBox_802c1fe4[] = "MOBJ_TreasureBox";
const char str_MOBJ_BigTreasureBox_802c1ff8[] = "MOBJ_BigTreasureBox";
const char str_MOBJ_GrayTreasureBox_802c200c[] = "MOBJ_GrayTreasureBox";
const char str_MOBJ_BlackTreasureBo_802c2024[] = "MOBJ_BlackTreasureBox";

extern void* gp;
extern void* mapalloc_base_ptr;

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
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct EntryLocal {
        u32 flags;
        u8 pad4[0x38 - 4];
        VecLocal position;
        VecLocal scale;
        VecLocal rotation;
        u8 pad5c[0x70 - 0x5C];
        s32 poseId;
        u8 pad74[0x19C - 0x74];
        s32 offscreenId;
        u8 pad1a0[0x1E8 - 0x1A0];
        u8 paperTexObj[0x20];
        s32 unk208;
        u8 paperTexMtx[0x30];
    } EntryLocal;
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern void GXReadBoundingBox(u16* left, u16* top, u16* right, u16* bottom);
    extern void offscreenAddBoundingBox(s32 id, u16 left, u16 top, u16 right, u16 bottom);
    extern void animSetPaperTexMtx(void* mtx, s32 a, s32 b);
    extern void animSetPaperTexObj(void* texObj, s32 a, s32 b, s32 texMap,
                                   s32 d, s32 e, s32 f, s32 g);
    extern void animPoseDraw(s32 poseId, f64 x, f64 y, f64 z, f64 rotY,
                             f64 scale, s32 mode);
    extern f32 float_10_804201bc;
    EntryLocal* pEntry = entry;
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;

    sysWaitDrawSync();
    GXClearBoundingBox();
    if ((pEntry->flags & 0x400) != 0) {
        animSetPaperTexMtx(pEntry->paperTexMtx, 0, 0);
        animSetPaperTexObj(pEntry->paperTexObj, 0, 0, pEntry->unk208,
                           0, 0, 1, 1);
    }

    animPoseDraw(pEntry->poseId, pEntry->position.x, pEntry->position.y,
                 pEntry->position.z, pEntry->rotation.y,
                 float_10_804201bc * pEntry->scale.x, 1);
    animPoseDraw(pEntry->poseId, pEntry->position.x, pEntry->position.y,
                 pEntry->position.z, pEntry->rotation.y,
                 float_10_804201bc * pEntry->scale.x, 2);
    animPoseDraw(pEntry->poseId, pEntry->position.x, pEntry->position.y,
                 pEntry->position.z, pEntry->rotation.y,
                 float_10_804201bc * pEntry->scale.x, 3);

    if ((pEntry->flags & 0x400) != 0) {
        animSetPaperTexMtx(0, 0, 0);
        animSetPaperTexObj(0, 0, 0, 0, 0, 0, 1, 1);
    }
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    offscreenAddBoundingBox(pEntry->offscreenId, left, top, right, bottom);
}

void mobjInit(void) {
    MobjSet* set = &work.normal;
    extern void* __memAlloc(s32 heap, u32 size);

    set->count = 0x10;
    set->entries = __memAlloc(0, set->count * 0x23C);
    memset(set->entries, 0, set->count * 0x23C);

    set = &work.alt;
    set->count = 8;
    set->entries = __memAlloc(0, set->count * 0x23C);
    memset(set->entries, 0, set->count * 0x23C);

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

#pragma opt_common_subs off
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
    extern const char str_HOBJ_804201d0[6];
    extern const char str_MOBJ_804201f4[6];
    extern const char str_MOBJ_804201fc[6];
    extern u32 headDJ[];
    extern u32 cubeDJ[];
    extern u32 frontDJ[];
    extern u32 front2DJ[];
    extern u32 cubeDVP[];
    VecLocal bboxMin;
    VecLocal bboxMax;
    VecLocal pos;
    VecLocal min;
    VecLocal max;
    f32 trans2[3][4];
    f32 scale2[3][4];
    f32 xrot2[3][4];
    f32 yrot2[3][4];
    f32 zrot2[3][4];
    f32 trans1[3][4];
    f32 scale1[3][4];
    f32 xrot1[3][4];
    f32 yrot1[3][4];
    f32 zrot1[3][4];
    f32 trans0[3][4];
    f32 scale0[3][4];
    f32 xrot0[3][4];
    f32 yrot0[3][4];
    f32 zrot0[3][4];
    f32 result[3][4];
    void* poseData;
    void* hit;
    void* scan;
    s32 i;
    f32 diff;
    f32 ten;
    char* dataBase = (char*)cubeDVP - 0x20;

    poseData = animPoseGetAnimBaseDataPtr(*(s32*)((s32)pMobj + 0x70));
    min = *(VecLocal*)vec3_802c1f58;
    min.x = *(f32*)((s32)poseData + 0xD0);
    min.y = *(f32*)((s32)poseData + 0xD4);
    min.z = *(f32*)((s32)poseData + 0xD8);
    bboxMin = min;

    max = *(VecLocal*)vec3_802c1f64;
    max.x = *(f32*)((s32)poseData + 0xDC);
    max.y = *(f32*)((s32)poseData + 0xE0);
    max.z = *(f32*)((s32)poseData + 0xE4);
    bboxMax = max;

    scan = pMobj;
    for (i = 0; i < 2; i++, scan = (void*)((s32)scan + 0x88)) {
        if (*(void**)((s32)scan + 0x74) != 0) {
            hitDelete((void*)((s32)scan + 0x78));
        }
    }

    PSVECAdd(&bboxMax, &bboxMin, (VecLocal*)((s32)pMobj + 0x190));
    PSVECScale((VecLocal*)((s32)pMobj + 0x190), (VecLocal*)((s32)pMobj + 0x190), (double)float_0p5_804201f0);
    PSVECScale((VecLocal*)((s32)pMobj + 0x190), (VecLocal*)((s32)pMobj + 0x190), (double)float_10_804201bc);
    *(f32*)((s32)pMobj + 0x194) = float_0_804201b8;
    ten = float_10_804201bc;
    diff = (f32)__fabs(bboxMax.x - bboxMin.x);
    *(f32*)((s32)pMobj + 0x184) = ten * (*(f32*)((s32)pMobj + 0x44) * diff);
    diff = (f32)__fabs(bboxMax.y);
    *(f32*)((s32)pMobj + 0x188) = ten * (*(f32*)((s32)pMobj + 0x48) * diff);
    diff = (f32)__fabs(bboxMax.z - bboxMin.z);
    *(f32*)((s32)pMobj + 0x18C) = ten * (*(f32*)((s32)pMobj + 0x4C) * diff);
    if (strncmp((const char*)((s32)poseData + 4), str_HOBJ_804201d0, strlen(str_HOBJ_804201d0)) == 0) {
        *(f32*)((s32)pMobj + 0x198) = float_0_804201b8;
        *(f32*)((s32)pMobj + 0x18C) = float_10_804201bc * (float_2p5_804201d8 * *(f32*)((s32)pMobj + 0x4C));
    }

    switch (param_2) {
    case 1: {
        PSVECAdd((VecLocal*)((s32)pMobj + 0x38), (VecLocal*)((s32)pMobj + 0x190), &pos);
        PSMTXTrans(trans1, (double)pos.x, (double)pos.y, (double)pos.z);
        PSMTXScale(scale1, *(f32*)((s32)pMobj + 0x184), *(f32*)((s32)pMobj + 0x188), *(f32*)((s32)pMobj + 0x18C));
        PSMTXRotRad(xrot1, (double)(float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x50)), 'x');
        PSMTXRotRad(yrot1, (double)(float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x54)), 'y');
        PSMTXRotRad(zrot1, (double)(float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x58)), 'z');
        PSMTXConcat(trans1, zrot1, trans1);
        PSMTXConcat(trans1, yrot1, trans1);
        PSMTXConcat(trans1, xrot1, trans1);
        PSMTXConcat(trans1, scale1, result);
        memcpy((void*)((s32)pMobj + 0x90), dataBase + 0x1758, 0x68);
        strcpy((char*)((s32)pMobj + 0x78), str_MOBJ_804201f4);
        strcat((char*)((s32)pMobj + 0x78), (char*)((s32)pMobj + 5));
        *(char**)((s32)pMobj + 0x90) = (char*)((s32)pMobj + 0x78);
        hit = hitEntryMOBJ((void*)((s32)pMobj + 0x90), result);
        *(void**)((s32)pMobj + 0x74) = hit;
        *(void**)((s32)*(void**)((s32)pMobj + 0x74) + 0xD0) = pMobj;
        break;
    }
    case 0: {
        PSVECAdd((VecLocal*)((s32)pMobj + 0x38), (VecLocal*)((s32)pMobj + 0x190), &pos);
        PSMTXTrans(trans0, (double)pos.x, (double)pos.y, (double)pos.z);
        PSMTXScale(scale0, *(f32*)((s32)pMobj + 0x184), *(f32*)((s32)pMobj + 0x188), *(f32*)((s32)pMobj + 0x18C));
        PSMTXRotRad(xrot0, (double)(float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x50)), 'x');
        PSMTXRotRad(yrot0, (double)(float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x54)), 'y');
        PSMTXRotRad(zrot0, (double)(float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x58)), 'z');
        PSMTXConcat(trans0, zrot0, trans0);
        PSMTXConcat(trans0, yrot0, trans0);
        PSMTXConcat(trans0, xrot0, trans0);
        PSMTXConcat(trans0, scale0, result);
        memcpy((void*)((s32)pMobj + 0x90), dataBase + 0x16A4, 0x68);
        strcpy((char*)((s32)pMobj + 0x78), str_MOBJ_804201f4);
        strcat((char*)((s32)pMobj + 0x78), (char*)((s32)pMobj + 5));
        *(char**)((s32)pMobj + 0x90) = (char*)((s32)pMobj + 0x78);
        hit = hitEntryMOBJ((void*)((s32)pMobj + 0x90), result);
        *(void**)((s32)pMobj + 0x74) = hit;
        *(void**)((s32)*(void**)((s32)pMobj + 0x74) + 0xD0) = pMobj;
        break;
    }
    case 2: {
        PSVECAdd((VecLocal*)((s32)pMobj + 0x38), (VecLocal*)((s32)pMobj + 0x190), &pos);
        PSMTXTrans(trans2, (double)pos.x, (double)pos.y, (double)pos.z);
        PSMTXScale(scale2, *(f32*)((s32)pMobj + 0x184), *(f32*)((s32)pMobj + 0x188), *(f32*)((s32)pMobj + 0x18C));
        PSMTXRotRad(xrot2, (double)(float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x50)), 'x');
        PSMTXRotRad(yrot2, (double)(float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x54)), 'y');
        PSMTXRotRad(zrot2, (double)(float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x58)), 'z');
        PSMTXConcat(trans2, zrot2, trans2);
        PSMTXConcat(trans2, yrot2, trans2);
        PSMTXConcat(trans2, xrot2, trans2);
        PSMTXConcat(trans2, scale2, result);
        memcpy((void*)((s32)pMobj + 0x90), dataBase + 0x180C, 0x68);
        strcpy((char*)((s32)pMobj + 0x78), str_MOBJ_804201f4);
        strcat((char*)((s32)pMobj + 0x78), (char*)((s32)pMobj + 5));
        *(char**)((s32)pMobj + 0x90) = (char*)((s32)pMobj + 0x78);
        hit = hitEntryMOBJ((void*)((s32)pMobj + 0x90), result);
        *(void**)((s32)pMobj + 0x74) = hit;
        *(void**)((s32)*(void**)((s32)pMobj + 0x74) + 0xD0) = pMobj;
        memcpy((void*)((s32)pMobj + 0x118), dataBase + 0x18C0, 0x68);
        strcpy((char*)((s32)pMobj + 0x100), str_MOBJ_804201fc);
        strcat((char*)((s32)pMobj + 0x100), (char*)((s32)pMobj + 5));
        *(char**)((s32)pMobj + 0x118) = (char*)((s32)pMobj + 0x100);
        hit = hitEntryMOBJ((void*)((s32)pMobj + 0x118), result);
        *(void**)((s32)pMobj + 0xFC) = hit;
        *(void**)((s32)*(void**)((s32)pMobj + 0xFC) + 0xD0) = pMobj;
        break;
    }
    default:
        break;
    }
    return 0;
}
#pragma opt_common_subs reset

/*
 * Target-backed map-object collision geometry.
 * The unresolved 0x40-byte compiler/static-local prefix is intentionally not
 * hand-authored here.  All relocation-bearing fields below use symbolic
 * target relationships rather than absolute addresses.
 */
u32 cubeDVP[103] = {
    0x00000008, 0xBF000000, 0x00000000, 0x3F000000, 0x3F000000, 0x00000000, 0x3F000000, 0xBF000000,
    0x3F800000, 0x3F000000, 0x3F000000, 0x3F800000, 0x3F000000, 0xBF000000, 0x3F800000, 0xBF000000,
    0x3F000000, 0x3F800000, 0xBF000000, 0xBF000000, 0x00000000, 0xBF000000, 0x3F000000, 0x00000000,
    0xBF000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
};

u32 cubeDVN[55] = {
    0x00000006, 0x00000000, 0x00000000, 0x3F800000, 0x00000000, 0x3F800000, 0x00000000, 0x00000000,
    0x00000000, 0xBF800000, 0x00000000, 0xBF800000, 0x00000000, 0x3F800000, 0x00000000, 0x00000000,
    0xBF800000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
};

u32 cubeDDT[26] = {
    (u32)cubeDVP, (u32)cubeDVN, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000,
};

u32 cubeDN[1230] = {
    0x00000004, 0x00000005, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00060005,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00020005, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00040005, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00070004, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010004, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0x00050004, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0x00030004, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000004, 0x00070003, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0x00060003, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010003,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000003, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004,
    0x00050002, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00040002, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00070002, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0x00060002, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00030001, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00020001, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0x00050001, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00040001,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000004, 0x00010000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00030000, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00020000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
};

u32 cubeDP[19] = {
    0x00000000, 0x00000006, 0x00000000, (u32)cubeDDT, (u32)cubeDN, (u32)&cubeDN[205], (u32)&cubeDN[410], (u32)&cubeDN[615],
    (u32)&cubeDN[820], (u32)&cubeDN[1025], 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000,
};

u32 cubeDJ[26] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3F800000, 0x3F800000,
    0x3F800000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xBF000000,
    0x00000000, 0xBF000000, 0x3F000000, 0x3F800000, 0x3F000000, 0x00000000, (u32)cubeMSJ, 0x00000001,
    0x00000000, (u32)cubeDP,
};

u32 headDP[19] = {
    0x00000000, 0x00000001, 0x00000000, (u32)cubeDDT, (u32)&cubeDN[410], 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000,
};

u32 headDJ[26] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3F800000, 0x3F800000,
    0x3F800000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xBF000000,
    0x00000000, 0xBF000000, 0x3F000000, 0x00000000, 0x3F000000, 0x00000000, (u32)cubeMSJ, 0x00000001,
    0x00000000, (u32)headDP,
};

u32 frontDP[19] = {
    0x00000000, 0x00000001, 0x00000000, (u32)cubeDDT, (u32)&cubeDN[1025], 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000,
};

u32 frontDJ[26] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3F800000, 0x3F800000,
    0x3F800000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xBF000000,
    0x00000000, 0xBF000000, 0x3F000000, 0x3F800000, 0x3F000000, 0x00000000, (u32)cubeMSJ, 0x00000001,
    0x00000000, (u32)frontDP,
};

u32 front2DP[19] = {
    0x00000000, 0x00000005, 0x00000000, (u32)cubeDDT, (u32)cubeDN, (u32)&cubeDN[205], (u32)&cubeDN[410], (u32)&cubeDN[615],
    (u32)&cubeDN[820], 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000,
};

u32 front2DJ[26] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3F800000, 0x3F800000,
    0x3F800000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xBF000000,
    0x00000000, 0xBF000000, 0x3F000000, 0x3F800000, 0x3F000000, 0x00000000, (u32)front2MSJ, 0x00000001,
    0x00000000, (u32)front2DP,
};

typedef struct Vec3Local {
    f32 x;
    f32 y;
    f32 z;
} Vec3Local;

int mobjEntry(char* name, char* pAnimPoseName) {
    typedef struct MapObjectEntryLocal {
        u32 flags;
        u8 camId;
        char name[16];
        char animName[16];
        u8 unk25[0x38 - 0x25];
        Vec3Local position;
        Vec3Local scale;
        Vec3Local rotation;
        Vec3Local unk5C;
        f32 unk68;
        f32 unk6C;
        s32 poseId;
        u8 unk74[0x19C - 0x74];
        s32 offscreenId;
        u8 unk1A0[0x23C - 0x1A0];
    } MapObjectEntryLocal;
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
    extern f32 float_1_804201c0;
    extern const char str_S_1_804201ec;
    MobjSet* wp;
    s32 entryCount;
    s32 idx;
    MapObjectEntryLocal* entry;

    if (koopaRunFlag != 0) {
        wp = &work.koopa;
    } else {
        wp = &work.normal;
        if (*(s32*)((s32)gp + 0x14) != 0) {
            wp = &work.alt;
        }
    }

    entryCount = wp->count;
    for (idx = 0, entry = wp->entries; idx < entryCount; idx++, entry++) {
        if ((entry->flags & 1) && (strcmp(entry->name, name) == 0)) {
            break;
        }
    }

    for (idx = 0, entry = wp->entries; idx < entryCount; idx++, entry++) {
        if ((entry->flags & 1) == 0) {
            break;
        }
    }

    memset(entry, 0, sizeof(*entry));
    entry->flags |= 1;
    strcpy(entry->name, name);

    entry->position = *(Vec3Local*)vec3_802c1f70;
    entry->scale = *(Vec3Local*)vec3_802c1f7c;
    entry->rotation = *(Vec3Local*)vec3_802c1f88;
    entry->unk5C = *(Vec3Local*)vec3_802c1f94;
    entry->unk68 = float_1_804201c0;
    entry->unk6C = float_1_804201c0;
    entry->camId = 4;
    entry->poseId = animPoseEntry(pAnimPoseName, *(s32*)((s32)gp + 0x14) != 0);
    animPosePeraOff(entry->poseId);
    strcpy(entry->animName, pAnimPoseName);
    animPoseSetAnim(entry->poseId, &str_S_1_804201ec, 1);
    animPoseSetMaterialLightFlagOn(entry->poseId, 2);
    mobjHitEntry(entry, 0);
    entry->offscreenId = -1;
    return idx;
}

void mobjDelete(char* name) {
    extern s32 strcmp(const char* a, const char* b);
    extern void marioResetHitObj(void* hitObj);
    extern void hitDelete(void* hitObj);
    MobjSet* set;
    s32 entryCount;
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

    entryCount = set->count;
    entry = set->entries;
    for (i = 0; i < entryCount; i++, entry = (void*)((s32)entry + 0x23C)) {
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


#pragma opt_common_subs off
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
    extern void* hitCheckFilter(f64 x, f64 y, f64 z, f64 dx, f64 dy, f64 dz, s32 flags,
                              void* out0, f32* outY, void* out2, f32* dist, void* out4, void* out5, void* out6);
    extern void PSVECAdd(Vec3Local* a, Vec3Local* b, Vec3Local* out);
    extern void* animPoseGetAnimBaseDataPtr(s32 poseId);
    extern u32 strlen(const char* s);
    extern s32 strncmp(const char* a, const char* b, u32 n);
    extern void PSMTXTrans(Mtx34Local m, f32 x, f32 y, f32 z);
    extern void PSMTXScale(Mtx34Local m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx34Local m, char axis, f32 rad);
    extern void PSMTXConcat(Mtx34Local a, Mtx34Local b, Mtx34Local out);
    extern void hitReCalcMatrix(void* hitEntry, Mtx34Local mtx);

    extern s32 kpaHipAttackCheck(void);
    extern s32 kpaGetLevel(void);
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
    extern const char str_HOBJ_804201d0[6];

    MobjSet* set;
    void* player;
    void* party;
    void* fbat;
    void* newHeadHit;
    s32 partyUsing;
    s16 fbatMode;
    s32 count;
    s32 i;
    void* entry;
    const char* dataBase = vec3_802c1f58;
    Vec3Local clip;
    Mtx34Local calcOut0;
    Mtx34Local calcOut1;
    Mtx34Local calcOut2;

    if (koopaRunFlag != 0) {
        set = &work.koopa;
    } else {
        s32 battle = *(s32*)((s32)gp + 0x14);
        set = &work.normal;
        if (battle != 0) {
            set++;
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

        if ((flags & 1) != 0) {
            if (*(s32*)((s32)gp + 0x14) != 0 || (u32)fbatMode < 2 || fbatMode == 8 || (flags & 0x400) != 0) {
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
                            Mtx34Local calcRotZMtx;
                            Mtx34Local calcRotYMtx;
                            Mtx34Local calcRotXMtx;
                            Mtx34Local calcScaleMtx;
                            Mtx34Local calcTransMtx;
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
                            PSMTXConcat(calcTransMtx, calcScaleMtx, calcOut0);

                            calcEntry = entry;
                            for (calcI = 0; calcI < 2; calcI++) {
                                if (*(void**)((s32)calcEntry + 0x74) != 0) {
                                    hitReCalcMatrix(*(void**)((s32)calcEntry + 0x74), calcOut0);
                                }
                                calcEntry = (void*)((s32)calcEntry + 0x88);
                            }
                        } else {
                            Mtx34Local calcRotZMtx;
                            Mtx34Local calcRotYMtx;
                            Mtx34Local calcRotXMtx;
                            Mtx34Local calcScaleMtx;
                            Mtx34Local calcTransMtx;
                            AnimBaseLocal* calcBase;
                            Vec3Local calcMax;
                            Vec3Local calcMin;
                            Vec3Local calcBBoxMax;
                            Vec3Local calcBBoxMin;
                            Vec3Local calcTrans;
                            f32 calcTen;
                            s32 calcI;
                            void* calcEntry;

                            PSVECAdd((Vec3Local*)((s32)entry + 0x38), (Vec3Local*)((s32)entry + 0x190), &calcTrans);
                            calcBase = animPoseGetAnimBaseDataPtr(*(s32*)((s32)entry + 0x70));
                            calcMin = *(Vec3Local*)(dataBase + 0x48);
                            calcMin.x = calcBase->bboxMin.x;
                            calcMin.y = calcBase->bboxMin.y;
                            calcMin.z = calcBase->bboxMin.z;
                            calcBBoxMin = calcMin;
                            calcMax = *(Vec3Local*)(dataBase + 0x54);
                            calcMax.x = calcBase->bboxMax.x;
                            calcMax.y = calcBase->bboxMax.y;
                            calcMax.z = calcBase->bboxMax.z;
                            calcBBoxMax = calcMax;
                            calcTen = float_10_804201bc;
                            *(f32*)((s32)entry + 0x184) = calcTen * (*(f32*)((s32)entry + 0x44) * (f32)__fabs(calcBBoxMax.x - calcBBoxMin.x));
                            *(f32*)((s32)entry + 0x188) = calcTen * (*(f32*)((s32)entry + 0x48) * (f32)__fabs(calcBBoxMax.y));
                            *(f32*)((s32)entry + 0x18C) = calcTen * (*(f32*)((s32)entry + 0x4C) * (f32)__fabs(calcBBoxMax.z - calcBBoxMin.z));

                            if (strncmp(calcBase->fileName, str_HOBJ_804201d0, strlen(str_HOBJ_804201d0)) == 0) {
                                *(f32*)((s32)entry + 0x198) = float_0_804201b8;
                                *(f32*)((s32)entry + 0x18C) = float_10_804201bc * (float_2p5_804201d8 * *(f32*)((s32)entry + 0x4C));
                            }

                            PSMTXTrans(calcTransMtx, calcTrans.x, calcTrans.y, calcTrans.z);
                            PSMTXScale(calcScaleMtx, *(f32*)((s32)entry + 0x184), *(f32*)((s32)entry + 0x188), *(f32*)((s32)entry + 0x18C));
                            PSMTXRotRad(calcRotXMtx, 'x', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x50));
                            PSMTXRotRad(calcRotYMtx, 'y', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x54));
                            PSMTXRotRad(calcRotZMtx, 'z', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x58));
                            PSMTXConcat(calcTransMtx, calcRotZMtx, calcTransMtx);
                            PSMTXConcat(calcTransMtx, calcRotYMtx, calcTransMtx);
                            PSMTXConcat(calcTransMtx, calcRotXMtx, calcTransMtx);
                            PSMTXConcat(calcTransMtx, calcScaleMtx, calcOut1);

                            calcEntry = entry;
                            for (calcI = 0; calcI < 2; calcI++) {
                                if (*(void**)((s32)calcEntry + 0x74) != 0) {
                                    hitReCalcMatrix(*(void**)((s32)calcEntry + 0x74), calcOut1);
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
                        u32 out6;
                        u32 out5;
                        u32 out4;
                        f32 checkDist;
                        u32 out2;
                        f32 outY;
                        u32 out0;
                        checkDist = float_10_804201bc + (float_1_804201c0 + (f32)__fabs(velY));
                        if (hitCheckFilter(*(f32*)((s32)entry + 0x38), float_10_804201bc + *(f32*)((s32)entry + 0x3C),
                                           *(f32*)((s32)entry + 0x40), float_0_804201b8, float_neg1_804201c4,
                                           float_0_804201b8, 0, &out0, &outY, &out2, &checkDist, &out4, &out5, &out6) != 0) {
                            *(f32*)((s32)entry + 0x3C) = outY;
                            *(f32*)((s32)entry + 0x60) = float_neg0p5_804201c8 * (*(f32*)((s32)entry + 0x6C) * *(f32*)((s32)entry + 0x60));
                            if (__fabs(*(f32*)((s32)entry + 0x60)) <= double_2_802c2040) {
                                *(f32*)((s32)entry + 0x60) = float_0_804201b8;
                            }
                        } else {
                            *(f32*)((s32)entry + 0x3C) += *(f32*)((s32)entry + 0x60);
                            *(f32*)((s32)entry + 0x60) = *(f32*)((s32)entry + 0x60) - *(f32*)((s32)entry + 0x68);
                        }
                    } else {
                        *(f32*)((s32)entry + 0x3C) += *(f32*)((s32)entry + 0x60);
                        *(f32*)((s32)entry + 0x60) = *(f32*)((s32)entry + 0x60) - *(f32*)((s32)entry + 0x68);
                    }
{
                    Mtx34Local calcRotZMtx;
                    Mtx34Local calcRotYMtx;
                    Mtx34Local calcRotXMtx;
                    Mtx34Local calcScaleMtx;
                    Mtx34Local calcTransMtx;
                    AnimBaseLocal* calcBase;
                    Vec3Local calcMax;
                    Vec3Local calcMin;
                    Vec3Local calcBBoxMax;
                    Vec3Local calcBBoxMin;
                                        Vec3Local calcTrans;
                            f32 calcTen;
                    s32 calcI;
                    void* calcEntry;

                    PSVECAdd((Vec3Local*)((s32)entry + 0x38), (Vec3Local*)((s32)entry + 0x190), &calcTrans);
                    calcBase = animPoseGetAnimBaseDataPtr(*(s32*)((s32)entry + 0x70));
                    calcMin = *(Vec3Local*)(dataBase + 0x48);
                    calcMin.x = calcBase->bboxMin.x;
                    calcMin.y = calcBase->bboxMin.y;
                    calcMin.z = calcBase->bboxMin.z;
                    calcBBoxMin = calcMin;
                    calcMax = *(Vec3Local*)(dataBase + 0x54);
                    calcMax.x = calcBase->bboxMax.x;
                    calcMax.y = calcBase->bboxMax.y;
                    calcMax.z = calcBase->bboxMax.z;
                    calcBBoxMax = calcMax;
                    calcTen = float_10_804201bc;
                    *(f32*)((s32)entry + 0x184) = calcTen * (*(f32*)((s32)entry + 0x44) * (f32)__fabs(calcBBoxMax.x - calcBBoxMin.x));
                    *(f32*)((s32)entry + 0x188) = calcTen * (*(f32*)((s32)entry + 0x48) * (f32)__fabs(calcBBoxMax.y));
                    *(f32*)((s32)entry + 0x18C) = calcTen * (*(f32*)((s32)entry + 0x4C) * (f32)__fabs(calcBBoxMax.z - calcBBoxMin.z));

                    if (strncmp(calcBase->fileName, str_HOBJ_804201d0, strlen(str_HOBJ_804201d0)) == 0) {
                                            *(f32*)((s32)entry + 0x198) = float_0_804201b8;
                        *(f32*)((s32)entry + 0x18C) = float_10_804201bc * (float_2p5_804201d8 * *(f32*)((s32)entry + 0x4C));
                    }

                    PSMTXTrans(calcTransMtx, calcTrans.x, calcTrans.y, calcTrans.z);
                    PSMTXScale(calcScaleMtx, *(f32*)((s32)entry + 0x184), *(f32*)((s32)entry + 0x188), *(f32*)((s32)entry + 0x18C));
                    PSMTXRotRad(calcRotXMtx, 'x', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x50));
                    PSMTXRotRad(calcRotYMtx, 'y', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x54));
                    PSMTXRotRad(calcRotZMtx, 'z', float_deg2rad_804201cc * *(f32*)((s32)entry + 0x58));
                    PSMTXConcat(calcTransMtx, calcRotZMtx, calcTransMtx);
                    PSMTXConcat(calcTransMtx, calcRotYMtx, calcTransMtx);
                    PSMTXConcat(calcTransMtx, calcRotXMtx, calcTransMtx);
                    PSMTXConcat(calcTransMtx, calcScaleMtx, calcOut2);

                    calcEntry = entry;
                    for (calcI = 0; calcI < 2; calcI++) {
                                            if (*(void**)((s32)calcEntry + 0x74) != 0) {
                                                hitReCalcMatrix(*(void**)((s32)calcEntry + 0x74), calcOut2);
                        }
                        calcEntry = (void*)((s32)calcEntry + 0x88);
                    }
                        }
                    flags = *(u32*)entry;
                }

                if ((flags & 0x8) == 0) {
                    void* hit = *(void**)((s32)entry + 0x74);
                    s32 activated = 0;

                    if (*(s8*)((s32)player + 0x3C) != 2) {
                        if ((flags & 0x00010000) != 0 && *(void**)((s32)player + 0x200) == hit &&
                            *(f32*)((s32)marioGetPtr() + 0x90) <= float_5_804201dc + *(f32*)((s32)entry + 0x3C)) {
                            activated = 1;
                            *(u32*)((s32)entry + 0x1E0) = 0x00010000;
                        } else if ((flags & 0x00400000) != 0 && newHeadHit == hit) {
                            activated = 1;
                            *(u32*)((s32)entry + 0x1E0) = 0x00400000;
                        } else if ((flags & 0x00200000) != 0 && *(void**)((s32)player + 0x1E8) == hit) {
                            activated = 1;
                            *(u32*)((s32)entry + 0x1E0) = 0x00200000;
                        } else if ((flags & 0x01000000) != 0 && *(void**)((s32)player + 0x1E8) == hit && marioChkHipBump() != 0) {
                            activated = 1;
                            *(u32*)((s32)entry + 0x1E0) = 0x01000000;
                        } else if (unk_8005ca2c() != 0 && (flags & 0x00040000) != 0 && *(void**)((s32)player + 0x1E4) == hit &&
                                   (*(u16*)((s32)player + 0x24C) & 0x100) != 0 && partyUsing == 0) {
                            activated = 1;
                            *(u32*)((s32)entry + 0x1E0) = 0x00040000;
                        } else if ((flags & 0x00800000) != 0 && party != 0) {
                            void* partyHit = *(void**)((s32)party + 0x130);
                            if (partyHit == hit) {
                                activated = 1;
                                *(u32*)((s32)entry + 0x1E0) = 0x00800000;
                            } else if (partyHit != 0 && *(u8*)((s32)party + 0x31) == 3 &&
                                       PSVECDistance((void*)((s32)party + 0x58), (void*)((s32)entry + 0x38)) <
                                           *(f32*)((s32)*(void**)((s32)entry + 0x74) + 0xCC) + *(f32*)((s32)party + 0xF4)) {
                                activated = 1;
                                *(u32*)((s32)entry + 0x1E0) = 0x00800000;
                            }
                        } else if ((flags & 0x02000000) != 0 && party != 0 && *(u8*)((s32)party + 0x31) == 3) {
                            void* partyHit = *(void**)((s32)party + 0x130);
                            if (partyHit == hit) {
                                activated = 1;
                                *(u32*)((s32)entry + 0x1E0) = 0x02000000;
                            } else if (partyHit != 0 &&
                                       PSVECDistance((void*)((s32)party + 0x58), (void*)((s32)entry + 0x38)) <
                                           *(f32*)((s32)*(void**)((s32)entry + 0x74) + 0xCC) + *(f32*)((s32)party + 0xF4)) {
                                activated = 1;
                                *(u32*)((s32)entry + 0x1E0) = 0x00800000;
                            }
                        }
                    } else {
                        s32 j;
                        if ((flags & 0x04000000) != 0) {
                            for (j = 0; j < 10; j++) {
                                if (kpaGetHitobjHead(j) == hit) {
                                    activated = 1;
                                    *(u32*)((s32)entry + 0x1E0) = 0x04000000;
                                    break;
                                }
                            }
                        }
                        if ((flags & 0x10000000) != 0) {
                            for (j = 0; j < 10 && !activated; j++) {
                                if (kpaGetHitobjRide(j) == hit && kpaHipAttackCheck() != 0) {
                                    activated = 1;
                                    *(u32*)((s32)entry + 0x1E0) = 0x10000000;
                                    break;
                                }
                            }
                        }
                        if ((flags & 0x08000000) != 0 && kpaGetHitobj() == hit && kpaGetLevel() == 3) {
                            activated = 1;
                            *(u32*)((s32)entry + 0x1E0) = 0x08000000;
                        } else if ((flags & 0x20000000) != 0 && kpaGetHitFlame() == hit) {
                            activated = 1;
                            *(u32*)((s32)entry + 0x1E0) = 0x20000000;
                        } else if ((flags & 0x00080000) != 0 && *(void**)((s32)player + 0x1E4) == hit) {
                            activated = 1;
                            *(u32*)((s32)entry + 0x1E0) = 0x00080000;
                        } else if ((flags & 0x00200000) != 0 && *(void**)((s32)player + 0x1E8) == hit) {
                            activated = 1;
                            *(u32*)((s32)entry + 0x1E0) = 0x00200000;
                        }
                    }

                    if (activated != 0) {
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
                        goto next_entry;
                    }
                }
            }

            if ((*(u32*)entry & 1) != 0) {
                u32 materialColor;
                if (koopaRunFlag == 0 && ((*(u32*)entry & 0x802) == 0)) {
                    void* map = mapGetWork();
                    u32 blend;
                    u32 blend2;
                    mapGetBlend(&blend);
                    mapGetBlend2(&blend2);
                    if ((*(u16*)((s32)map + 4) & 2) != 0) {
                        if ((animPoseGetMaterialFlag(*(s32*)((s32)entry + 0x70)) & 0x800) == 0 && ((u8*)&blend)[3] != 0xFF) {
                            goto shadow_done;
                        }
                    }
                    if ((*(u16*)((s32)map + 4) & 4) != 0) {
                        if ((animPoseGetMaterialFlag(*(s32*)((s32)entry + 0x70)) & 0x1000) == 0 && ((u8*)&blend2)[3] != 0xFF) {
                            goto shadow_done;
                        }
                    }
                    {
                        s32 shadowId = shadowEntry(*(f32*)((s32)entry + 0x38), *(f32*)((s32)entry + 0x3C), *(f32*)((s32)entry + 0x40),
                                                  (float_10_804201bc * *(f32*)((s32)entry + 0x44) * *(f32*)((s32)entry + 0x184)) / float_25_804201e0);
                        if ((*(u32*)entry & 0x00400000) != 0) {
                            shadowSetType(shadowId, 1);
                            shadowSetSize((float_10_804201bc * *(f32*)((s32)entry + 0x44) * *(f32*)((s32)entry + 0x184)) / float_20_804201e4, shadowId);
                        }
                    }
                }

shadow_done:
                animPoseMain(*(s32*)((s32)entry + 0x70));
                materialColor = animPoseGetMaterialEvtColor(*(s32*)((s32)entry + 0x70));

                if ((*(u32*)entry & 0x4000) == 0) {
                    if ((*(u32*)entry & 0x80) == 0) {
                        void* cam = camGetPtr(4);
                        PSMTXMultVec((void*)((s32)cam + 0x11C), (void*)((s32)entry + 0x38), &clip);
                        cam = camGetPtr(4);
                        PSMTX44MultVec((void*)((s32)cam + 0x15C), &clip, &clip);
                        if (__fabs(clip.x) > double_1p5_802c2048 ||
                            __fabs(clip.y) > double_1p5_802c2048 ||
                            __fabs(clip.z) > double_1_802c2050) {
                            goto next_entry;
                        }
                    }
                    if (((*(u32*)entry & 0x2000) == 0) || (((*(u32*)((s32)gp + 0x1C) >> 3) & 1) == 0)) {
                        if (*(s32*)((s32)entry + 0x19C) == -1) {
                            u32 matFlags = animPoseGetMaterialFlag(*(s32*)((s32)entry + 0x70));
                            if ((matFlags & 0x40) != 0 && ((u8*)&materialColor)[3] != 0xFF) {
                                dispEntry(*(u8*)((s32)entry + 4), 2, mobjDispXLU, entry, dispCalcZ((void*)((s32)entry + 0x38)));
                            } else {
                                dispEntry(*(u8*)((s32)entry + 4), 0, mobjDisp, entry, dispCalcZ((void*)((s32)entry + 0x38)));
                                dispEntry(*(u8*)((s32)entry + 4), 1, mobjDisp, entry, dispCalcZ((void*)((s32)entry + 0x38)));
                                dispEntry(*(u8*)((s32)entry + 4), 2, mobjDisp, entry, dispCalcZ((void*)((s32)entry + 0x38)));
                            }
                        } else {
                            u32 matFlags = animPoseGetMaterialFlag(*(s32*)((s32)entry + 0x70));
                            if ((matFlags & 0x40) != 0 && ((u8*)&materialColor)[3] != 0xFF) {
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
next_entry:
        ;
    }
}
#pragma opt_common_subs reset

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
    extern f32 float_10_804201bc;
    extern f32 float_0_804201b8;
    extern f32 float_2p5_804201d8;
    extern f32 float_deg2rad_804201cc;
    extern const char str_HOBJ_804201d0[6];
    Vec3 trans;
    Vec3 bboxMin;
    Vec3 bboxMax;
    Vec3 min;
    Vec3 max;
    Mtx34 outMtx;
    Mtx34 rotZMtx;
    Mtx34 rotYMtx;
    Mtx34 rotXMtx;
    Mtx34 scaleMtx;
    Mtx34 transMtx;
    AnimBaseLocal* base;
    s32 i;
    f32 ten;

    PSVECAdd((Vec3*)((s32)pMobj + 0x38), (Vec3*)((s32)pMobj + 0x190), &trans);
    base = animPoseGetAnimBaseDataPtr(*(s32*)((s32)pMobj + 0x70));
    min = *(Vec3*)vec3_802c1fa0;
    min.x = base->bboxMin.x;
    min.y = base->bboxMin.y;
    min.z = base->bboxMin.z;
    bboxMin = min;
    max = *(Vec3*)vec3_802c1fac;
    max.x = base->bboxMax.x;
    max.y = base->bboxMax.y;
    max.z = base->bboxMax.z;
    bboxMax = max;
    ten = float_10_804201bc;
    *(f32*)((s32)pMobj + 0x184) = ten * (*(f32*)((s32)pMobj + 0x44) * (f32)__fabs(bboxMax.x - bboxMin.x));
    *(f32*)((s32)pMobj + 0x188) = ten * (*(f32*)((s32)pMobj + 0x48) * (f32)__fabs(bboxMax.y));
    *(f32*)((s32)pMobj + 0x18C) = ten * (*(f32*)((s32)pMobj + 0x4C) * (f32)__fabs(bboxMax.z - bboxMin.z));

    if (strncmp(base->fileName, str_HOBJ_804201d0, strlen(str_HOBJ_804201d0)) == 0) {
        *(f32*)((s32)pMobj + 0x198) = float_0_804201b8;
        *(f32*)((s32)pMobj + 0x18C) = float_10_804201bc * (float_2p5_804201d8 * *(f32*)((s32)pMobj + 0x4C));
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
    typedef struct Entry {
        u8 pad0[0x15];
        char animName[16];
        u8 pad25[0x1B7];
        s32 itemStatus;
    } Entry;
    Entry* work = entry;

    if (strcmp(work->animName, str_MOBJ_TreasureBox_802c1fe4) == 0 ||
        strcmp(work->animName, str_MOBJ_BigTreasureBox_802c1ff8) == 0 ||
        strcmp(work->animName, str_MOBJ_GrayTreasureBox_802c200c) == 0 ||
        strcmp(work->animName, str_MOBJ_BlackTreasureBo_802c2024) == 0) {
        return (work->itemStatus == 99) + 1;
    }
    return 0;
}

u32* mobjNearDistCheck2(f32 param_1, f32 param_2, f32 param_3, f64 param_4, char** param_5) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef struct Entry {
        u32 flags;
        u8 camId;
        char name[16];
        char animName[16];
        u8 pad25[0x13];
        Vec3 position;
        u8 pad44[0x198];
        u32 itemStatus;
        u8 pad1e0[4];
        s32 eventValue;
        u8 pad1e8[0x54];
    } Entry;
    extern MobjWork work;
    extern void* gp;
    extern s32 koopaRunFlag;
    extern s32 strcmp(const char* a, const char* b);
    extern void* itemNameToPtr(char* name);
    extern u32 evtGetValue(void* event, s32 value);
    extern void PSVECSubtract(Vec3* a, Vec3* b, Vec3* out);
    extern f32 PSVECMag(Vec3* vec);
    MobjSet* set;
    s32 i;
    s32 count;
    Entry* entry;
    u32* best;
    char** list;
    Vec3 pos;
    Vec3 temp;
    Vec3 diff;
    u8* rodata;
    f32 dist;

    rodata = (u8*)vec3_802c1f58;
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
    entry = (Entry*)set->entries;
    temp = *(Vec3*)(rodata + 0x6C);
    temp.x = (f32)param_1;
    temp.y = (f32)param_2;
    temp.z = (f32)param_3;
    pos = temp;

    while (i < count) {
        if ((entry->flags & 1) != 0) {
            list = param_5;
            while (*list != 0) {
                if (strcmp(entry->animName, *list) == 0) {
                    if (strcmp(entry->animName, (char*)(rodata + 0x8C)) == 0 ||
                        strcmp(entry->animName, (char*)(rodata + 0xA0)) == 0 ||
                        strcmp(entry->animName, (char*)(rodata + 0xB4)) == 0 ||
                        strcmp(entry->animName, (char*)(rodata + 0xCC)) == 0) {
                        s32 check;

                        if (strcmp(entry->animName, (char*)(rodata + 0x8C)) == 0 ||
                            strcmp(entry->animName, (char*)(rodata + 0xA0)) == 0 ||
                            strcmp(entry->animName, (char*)(rodata + 0xB4)) == 0 ||
                            strcmp(entry->animName, (char*)(rodata + 0xCC)) == 0) {
                            if (entry->itemStatus == 99) {
                                check = 2;
                            } else {
                                check = 1;
                            }
                        } else {
                            check = 0;
                        }
                        if (check != 1) {
                            list++;
                            continue;
                        }
                    } else if (strcmp(entry->animName, (char*)(rodata + 0x78)) == 0) {
                        s32 check;
                        void* item;
                        u32 value;

                        check = 0;
                        if (strcmp(entry->animName, (char*)(rodata + 0x78)) == 0) {
                            item = itemNameToPtr(entry->name);
                            if (item == 0 || ((*(u16*)item & 1) == 0) || evtGetValue(0, entry->eventValue) != 0) {
                                value = evtGetValue(0, entry->eventValue);
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

                    PSVECSubtract(&entry->position, &pos, &diff);
                    dist = PSVECMag(&diff);
                    if ((double)dist < param_4) {
                        best = (u32*)entry;
                        param_4 = (double)dist;
                    }
                    break;
                }
                list++;
            }
        }
        i++;
        entry++;
    }
    return best;
}

/* Late-defined exact mobjdrv small-data catalog. */
const f32 float_0_804201b8 = 0.0f;
const f32 float_10_804201bc = 10.0f;
const f32 float_1_804201c0 = 1.0f;
const f32 float_neg1_804201c4 = -1.0f;
const f32 float_neg0p5_804201c8 = -0.5f;
const f32 float_deg2rad_804201cc = 0.017453292f;
const char str_HOBJ_804201d0[] = "HOBJ_";
const f32 float_2p5_804201d8 = 2.5f;
const f32 float_5_804201dc = 5.0f;
const f32 float_25_804201e0 = 25.0f;
const f32 float_20_804201e4 = 20.0f;
const f32 float_0p1_804201e8 = 0.1f;
const char str_S_1_804201ec[] = "S_1";
const f32 float_0p5_804201f0 = 0.5f;
const char str_MOBJ_804201f4[] = "MOBJ_";
const char str_MOBJ_804201fc[] = "MOBJ~";

/* Late-defined exact mobjdrv read-only double tail. */
const f64 double_2_802c2040 = 2.0;
const f64 double_1p5_802c2048 = 1.5;
const f64 double_1_802c2050 = 1.0;

