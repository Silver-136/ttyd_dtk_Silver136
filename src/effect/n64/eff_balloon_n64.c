#include "effect/n64/eff_balloon_n64.h"

const f32 vec3_802fab50[3] = { 0.0f, 0.0f, 0.0f };

extern const char str_BalloonN64_802fab68[];

void effBalloonMain(void* entry);
void effBalloonDisp(s32 cameraId, void* effect);

typedef struct EffBalloonWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    f32 unk14;
    f32 unk18;
    s32 timer;
    s32 frame;
} EffBalloonWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);

extern const f32 float_0_80424cbc;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effBalloonN64Entry(s32 type, f32 x, f32 y, f32 z, f32 unk14, f32 scale) {
    void* entry = effEntry();
    EffBalloonWork* work;
    f32 zero;

    *(const char**)((s32)entry + 0x14) = str_BalloonN64_802fab68;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0xCC);
    *(EffBalloonWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBalloonMain;
    zero = float_0_80424cbc;
    *(u32*)entry |= 2;

    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->timer = 100;
    work->unk14 = unk14;
    work->unk18 = zero;
    work->frame = 0;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

void effBalloonMain(void* entry) {
    extern void effDelete(void* entry);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 order);
    extern void effBalloonDisp(void);
    extern const f32 float_0_80424cbc;
    extern const f32 float_0p1_80424cc4;
    extern const f32 float_6p2832_80424cc8;
    extern const f32 float_360_80424ccc;
    extern const f32 float_17_80424cd0;
    extern const f32 float_21_80424cd4;
    extern const f32 float_30_80424cd8;
    extern const f32 float_5_80424cdc;
    extern const f32 float_2_80424ce0;
    extern const f32 float_1p5_80424ce4;
    extern const f32 float_50_80424ce8;
    extern const f32 float_1_80424cec;
    extern const f32 float_1p2_80424cf0;
    EffBalloonWork* work = *(EffBalloonWork**)((s32)entry + 0xC);
    f32 pos[3];
    f32* points;
    s32 timer;
    s32 frame;
    s32 i;
    f32 phase;
    f32 amp;

    points = (f32*)((s32)work + 0x24);
    pos[0] = work->x;
    pos[1] = work->y;
    pos[2] = work->z;

    if ((*(u32*)entry & 4) != 0) {
        *(u32*)entry &= ~4;
        work->timer = 99;
    }
    if (work->timer < 100) {
        work->timer--;
    }
    work->frame++;
    if (work->frame > 0x4F1A0) {
        work->frame = 0x100;
    }

    timer = work->timer;
    if (timer < 0) {
        effDelete(entry);
        return;
    }

    frame = work->frame;
    phase = (float_6p2832_80424cc8 * (f32)(frame * 3)) / float_360_80424ccc;
    work->unk18 = float_0p1_80424cc4 * (work->unk14 - work->unk18) + work->unk18;
    amp = float_0p1_80424cc4 * work->unk18 * (f32)sin((f64)phase) + work->unk18;

    for (i = 0; i < 0x14; i++) {
        f32 angle;
        f32 wobble;
        f32 extraAngle;

        angle = (float_6p2832_80424cc8 * (float_17_80424cd0 + (float_360_80424ccc * (f32)i) / float_21_80424cd4)) / float_360_80424ccc;
        points[0] = amp * (f32)sin((f64)angle);
        points[1] = -amp * (f32)cos((f64)angle);

        wobble = float_0p1_80424cc4 * (f32)sin((f64)((float_6p2832_80424cc8 * (f32)(i * 0x35)) / float_360_80424ccc)) + float_2_80424ce0;
        extraAngle = (float_6p2832_80424cc8 * ((float_30_80424cd8 * (float_5_80424cdc + (f32)i)) + ((f32)frame * wobble))) / float_360_80424ccc;
        points[0] += float_1p5_80424ce4 * (f32)sin((f64)extraAngle);

        wobble = float_0p1_80424cc4 * (f32)cos((f64)((float_6p2832_80424cc8 * (f32)(i * 0x24)) / float_360_80424ccc)) + float_2_80424ce0;
        extraAngle = (float_6p2832_80424cc8 * ((float_50_80424ce8 * (float_5_80424cdc + (f32)i)) + ((f32)frame * wobble))) / float_360_80424ccc;
        points[1] += float_1p5_80424ce4 * (f32)cos((f64)extraAngle);

        points += 2;
    }

    *(f32*)((s32)work + 0xC4) = float_0_80424cbc;
    if (timer < 100) {
        *(f32*)((s32)work + 0xC8) += float_1_80424cec;
    } else {
        *(f32*)((s32)work + 0xC8) = float_1p2_80424cf0 * amp;
    }

    dispEntry(4, 2, effBalloonDisp, entry, dispCalcZ(pos));
}

void effBalloonDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    typedef struct SmartAllocationData { void* pMemory; } SmartAllocationData;
    extern void* camGetPtr(s32);
    extern SmartAllocationData* smartAlloc(u32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void PSMTXScale(void*,f32,f32,f32);
    extern void GXLoadTexMtxImm(void*,s32,s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void PSMTXTrans(void*,f32,f32,f32);
    extern void PSMTXRotRad(void*,s32,f32);
    extern void PSMTXConcat(void*,void*,void*);
    extern void PSMTXMultVec(void*,void*,void*);
    extern void DCFlushRange(void*,u32);
    extern void GXInvalidateVtxCache(void);
    extern void GXLoadPosMtxImm(void*,s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32,s32,s32);
    extern void tri1(s32,s32,s32);
    extern void* memcpy(void*, const void*, u32);
    extern s16 balloon_vtx[];
    extern const f32 float_0p0625_80424cb8;
    extern const f32 float_deg2rad_80424cc0;
    u8* entry = (u8*)effect;
    u8* work = *(u8**)(entry + 0xC);
    u8* camera = (u8*)camGetPtr(cameraId);
    SmartAllocationData* allocation = smartAlloc(0x1C0, 3);
    s16* vertices = (s16*)allocation->pMemory;
    u8 texObj[0x20];
    Mtx trans, rot, temp, billboard, model;
    VecLocal vec;
    s32 type = *(s32*)(work + 0x1C);
    s32 i;

    if (type == 0) {
        effGetTexObjN64(0x5F, texObj);
        GXLoadTexObj(texObj, 0);
    } else if (type == 1) {
        effGetTexObjN64(0x5E, texObj);
        GXLoadTexObj(texObj, 0);
    } else {
        effGetTexObjN64(0x5D, texObj);
        GXLoadTexObj(texObj, 0);
    }
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(temp, float_0p0625_80424cb8, float_0p0625_80424cb8, float_0_80424cbc);
    GXLoadTexMtxImm(temp, 0x1E, 1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xC);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    memcpy(vertices, balloon_vtx + 7, 0xE);
    vec.x = vertices[0]; vec.y = vertices[1]; vec.z = vertices[2];
    PSMTXMultVec(trans, &vec, &vec);
    vertices[0] = (s16)vec.x; vertices[1] = (s16)vec.y; vertices[2] = (s16)vec.z;

    PSMTXRotRad(rot, 0x7A, float_deg2rad_80424cc0 * *(f32*)(work + 0x10));
    PSMTXConcat(trans, rot, billboard);
    PSMTXRotRad(rot, 0x79, float_deg2rad_80424cc0 * -*(f32*)((u8*)camGetPtr(4) + 0x114));
    PSMTXTrans(trans, *(f32*)(work + 0xC4), *(f32*)(work + 0xC8), float_0_80424cbc);
    PSMTXConcat(rot, trans, model);
    PSMTXConcat(billboard, model, model);

    memcpy(vertices + 7, balloon_vtx, 0xE);
    vec.x = vertices[7]; vec.y = vertices[8]; vec.z = vertices[9];
    PSMTXMultVec(model, &vec, &vec);
    vertices[7] = (s16)vec.x; vertices[8] = (s16)vec.y; vertices[9] = (s16)vec.z;

    for (i = 0; i < 0x14; i++) {
        s16* dst;
        PSMTXTrans(trans, *(f32*)(work + 0x24 + i * 8), *(f32*)(work + 0x28 + i * 8), float_0_80424cbc);
        PSMTXConcat(model, trans, trans);
        dst = vertices + (i + 2) * 7;
        memcpy(dst, balloon_vtx + (i + 2) * 7, 0xE);
        vec.x = dst[0]; vec.y = dst[1]; vec.z = dst[2];
        PSMTXMultVec(trans, &vec, &vec);
        dst[0] = (s16)vec.x; dst[1] = (s16)vec.y; dst[2] = (s16)vec.z;
    }
    DCFlushRange(vertices, 0x1C0);
    GXInvalidateVtxCache();
    GXLoadPosMtxImm(camera + 0x11C, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(0);
    effSetVtxDescN64(vertices);
    if (type < 100) {
        GXBegin(0x90, 0, 3);
        tri1(0x15, 2, 1);
    } else {
        GXBegin(0x90, 0, 6);
        tri1(0, 1, 2);
        tri1(0, 0x15, 1);
    }
    for (i = 0; i < 0x13; i++) {
        GXBegin(0x90, 0, 3);
        tri1(1, i + 2, i + 3);
    }
}

const char str_BalloonN64_802fab68[] = "BalloonN64";

/* Target-owned 22 x 14-byte balloon vertex records; keep late so renderer
 * references retain external-data codegen. */
s16 balloon_vtx[154] = {
    0, 0, 0, 0, 0, 0, 255,
    0, 0, 0, 0, 480, 0, 255,
    0, 0, 0, 0, 512, 0, 255,
    0, 0, 0, 0, 512, 0, 255,
    0, 0, 0, 0, 512, 0, 255,
    0, 0, 0, 0, 512, 0, 255,
    0, 0, 0, 0, 512, 0, 255,
    0, 0, 0, 0, 512, 0, 255,
    0, 0, 0, 0, 512, 0, 255,
    0, 0, 0, 0, 512, 0, 255,
    0, 0, 0, 0, 512, 0, 255,
    0, 0, 0, 0, 512, 0, 255,
    0, 0, 0, 0, 512, 0, 255,
    0, 0, 0, 160, 512, 0, 255,
    0, 0, 0, 160, 512, 0, 255,
    0, 0, 0, 0, 512, 0, 255,
    0, 0, 0, 160, 512, 0, 255,
    0, 0, 0, 160, 512, 0, 255,
    0, 0, 0, 160, 512, 0, 255,
    0, 0, 0, 160, 512, 0, 255,
    0, 0, 0, 160, 512, 0, 255,
    0, 0, 0, 0, 512, 0, 255
};

/* Target-owned scalar catalog; late definitions preserve external-load codegen
 * in effBalloonN64Entry, effBalloonMain, and effBalloonDisp. */
const f32 float_0p0625_80424cb8 = 0.0625f;
const f32 float_0_80424cbc = 0.0f;
const f32 float_deg2rad_80424cc0 = 0.01745329238474369f;
const f32 float_0p1_80424cc4 = 0.10000000149011612f;
const f32 float_6p2832_80424cc8 = 6.2831854820251465f;
const f32 float_360_80424ccc = 360.0f;
const f32 float_17_80424cd0 = 17.0f;
const f32 float_21_80424cd4 = 21.0f;
const f32 float_30_80424cd8 = 30.0f;
const f32 float_5_80424cdc = 5.0f;
const f32 float_2_80424ce0 = 2.0f;
const f32 float_1p5_80424ce4 = 1.5f;
const f32 float_50_80424ce8 = 50.0f;
const f32 float_1_80424cec = 1.0f;
const f32 float_1p2_80424cf0 = 1.2000000476837158f;
