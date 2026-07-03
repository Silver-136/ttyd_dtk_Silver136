#include "effect/n64/eff_balloon_heiho_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffBalloonHeihoWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    f32 rise;
    f32 riseStep;
    f32 scale;
    s32 timer;
    s32 frame;
} EffBalloonHeihoWork;

void* effEntry(void);
void effDelete(void* effect);
void* __memAlloc(s32 heap, s32 size);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fab30;
extern char str_BalloonHeihoN64_802fab3c[];
extern f32 float_0_80424cb0;
extern f32 float_2_80424cb4;

void effBalloonHeihoDisp(int param_1, void* param_2) {
    extern void* camGetPtr(s32 id);
    extern void mapSetMaterialFog(void);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 coord, s32 type, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
    extern f32 float_deg2rad_80424ca0;
    extern f32 float_1_80424ca4;
    extern f32 float_0p03125_80424ca8;
    extern f32 float_0p015625_80424cac;
    extern f32 float_0_80424cb0;
    extern u8 size32x64_tex32x64_vtx[];
    f32 mtxA[3][4];
    f32 mtxB[3][4];
    f32 mtxC[3][4];
    u8 texObj[0x20];
    void* cam;
    EffBalloonHeihoWork* work;
    s32 type;

    cam = camGetPtr(param_1);
    work = *(EffBalloonHeihoWork**)((s32)param_2 + 0xC);
    type = work->type;
    mapSetMaterialFog();

    PSMTXTrans(mtxA, work->x, work->y, work->z);
    PSMTXRotRad(mtxB, 0x79, -(*(f32*)((s32)camGetPtr(4) + 0x114) * float_deg2rad_80424ca0));
    PSMTXScale(mtxC, work->scale, work->scale, float_1_80424ca4);
    PSMTXConcat(mtxA, mtxB, mtxA);
    PSMTXConcat(mtxA, mtxC, mtxA);
    PSMTXConcat((void*)((s32)cam + 0x11C), mtxA, mtxA);
    GXLoadPosMtxImm(mtxA, 0);
    GXSetCurrentMtx(0);

    switch (type) {
        case 0:
            effGetTexObjN64(0x60, texObj);
            break;
        case 1:
            effGetTexObjN64(0x61, texObj);
            break;
        case 2:
            effGetTexObjN64(0x62, texObj);
            break;
    }

    GXLoadTexObj(texObj, 0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(mtxC, float_0p03125_80424ca8, float_0p015625_80424cac, float_0_80424cb0);
    GXLoadTexMtxImm(mtxC, 0x1E, 1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOp(0, 3);
    GXSetCullMode(0);
    effSetVtxDescN64(size32x64_tex32x64_vtx);
    GXBegin(0x90, 0, 0x18);
    tri2(0, 1, 2, 0, 1, 2, 3, 0);
    tri2(0, 1, 2, 0, 1, 2, 3, 0);
    tri2(2, 3, 4, 0, 3, 4, 5, 0);
    tri2(4, 5, 6, 0, 5, 6, 7, 0);
}

void effBalloonHeihoMain(void* effect) {
    void* entry = effect;
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffBalloonHeihoWork* work;

    work = *(EffBalloonHeihoWork**)((s32)entry + 0xC);
    zero = &vec3_802fab30;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    work->timer--;
    work->frame++;
    if (work->timer < 0) {
        effDelete(entry);
    } else {
        f32 maxRise = float_2_80424cb4;

        work->rise += work->riseStep;
        if (work->rise > maxRise) {
            work->rise = maxRise;
        }
        work->y += work->rise;
        dispEntry(4, 2, effBalloonHeihoDisp, entry, dispCalcZ(&dispPos));
    }
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effBalloonHeihoN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 riseStep, f32 scale) {
    void* entry = effEntry();
    EffBalloonHeihoWork* work;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_BalloonHeihoN64_802fab3c;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x24);
    *(EffBalloonHeihoWork**)((s32)entry + 0xC) = work;
    zero = float_0_80424cb0;
    *(void**)((s32)entry + 0x10) = effBalloonHeihoMain;

    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->rise = zero;
    work->timer = timer;
    work->frame = 0;
    work->riseStep = riseStep;
    work->scale = scale;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
