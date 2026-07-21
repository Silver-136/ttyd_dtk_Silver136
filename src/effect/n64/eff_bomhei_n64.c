#include "effect/n64/eff_bomhei_n64.h"

void effBomheiMain(void* effect);
void effBomheiDisp(s32 cameraId, void* effect);

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffBomheiWork {
    s16 type;
    u8 pad_02[2];
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    s32 timer;
    s32 frame;
    s32 duration;
    f32 progress;
    f32 angle;
    f32 angleStep;
} EffBomheiWork;

void* effEntry(void);
void effDelete(void* effect);
void* __memAlloc(s32 heap, s32 size);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fabd8;
extern char str_BomheiN64_802fabf0[];
extern f32 float_0_80424db0;
extern f32 float_9_80424dc4;
extern f32 float_40_80424dc8;


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effBomheiN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffBomheiWork* work;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_BomheiN64_802fabf0;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x2C);
    *(EffBomheiWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBomheiMain;
    zero = float_0_80424db0;

    work->timer = duration;
    work->frame = 0;
    work->duration = duration;
    work->progress = zero;
    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->angle = zero;
    if (type == 0) {
        work->angleStep = zero;
    } else {
        work->angleStep = float_40_80424dc8;
    }
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
void effBomheiMain(void* effect) {
    void* entry = effect;
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffBomheiWork* work;

    work = *(EffBomheiWork**)((s32)entry + 0xC);
    zero = &vec3_802fabd8;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    work->progress = (float_9_80424dc4 * (f32)work->frame) / (f32)work->duration;
    work->frame++;
    work->timer--;
    if (work->timer < 0) {
        effDelete(entry);
    } else {
        work->angle += work->angleStep;
        dispEntry(4, 2, effBomheiDisp, entry, dispCalcZ(&dispPos));
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effBomheiDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 coord, s32 type, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void PSMTXRotRad(void* mtx, f32 angle, s32 axis);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    extern f32 float_256_80424da4;
    extern f32 float_0p03125_80424da8;
    extern f32 float_0p0034722_80424dac;
    extern f32 float_0_80424db0;
    extern f32 float_32_80424db4;
    extern f32 float_deg2rad_80424db8;
    extern f32 float_1_80424dbc;
    extern f32 float_4_80424dc0;
    extern u32 dat_80424d98;
    extern u32 dat_80424d9c;
    extern u32 dat_80424da0;
    extern u8 size32x32_tex32x32_vtx[];
    void* cam;
    void* cam3d;
    EffBomheiWork* work;
    u32 intProgress;
    s32 alpha;
    u32 color;
    u8 texObj[0x20];
    f32 texScaleMtx[3][4];
    f32 texTransMtx[3][4];
    f32 posMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];

    cam = camGetPtr(cameraId);
    work = *(EffBomheiWork**)((s32)effect + 0xC);
    intProgress = (u32)work->progress;
    alpha = (s32)(float_256_80424da4 * (work->progress - (f32)intProgress));

    GXSetNumChans(0);
    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 3);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 3);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0xC, 8, 5, 0xF);
    GXSetTevAlphaIn(1, 2, 5, 1, 7);
    GXSetTevOrder(2, 1, 1, 0xFF);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 0xA, 0xE, 0, 0xF);
    GXSetTevAlphaIn(2, 7, 7, 7, 0);

    effGetTexObjN64(0x35, texObj);
    GXLoadTexObj(texObj, 0);
    GXLoadTexObj(texObj, 1);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);

    PSMTXScale(texScaleMtx, float_0p03125_80424da8, float_0p0034722_80424dac, float_0_80424db0);
    PSMTXTrans(texTransMtx, float_0_80424db0, float_32_80424db4 * (f32)intProgress, float_0_80424db0);
    PSMTXConcat(texScaleMtx, texTransMtx, texTransMtx);
    GXLoadTexMtxImm(texTransMtx, 0x1E, 1);

    intProgress++;
    if ((s32)intProgress >= 8) {
        intProgress = 8;
    }
    PSMTXTrans(texTransMtx, float_0_80424db0, float_32_80424db4 * (f32)intProgress, float_0_80424db0);
    PSMTXConcat(texScaleMtx, texTransMtx, texTransMtx);
    GXLoadTexMtxImm(texTransMtx, 0x21, 1);
    GXSetCullMode(0);
    effSetVtxDescN64(size32x32_tex32x32_vtx);

    PSMTXTrans(posMtx, work->x, work->y, work->z);
    cam3d = camGetPtr(4);
    PSMTXRotRad(rotMtx, float_deg2rad_80424db8 * -*(f32*)((s32)cam3d + 0x114), 'y');
    PSMTXScale(scaleMtx, work->scale, work->scale, float_1_80424dbc);
    PSMTXConcat(posMtx, rotMtx, posMtx);
    PSMTXConcat(posMtx, scaleMtx, posMtx);
    PSMTXRotRad(rotMtx, float_deg2rad_80424db8 * work->angle, 'z');
    PSMTXConcat(posMtx, rotMtx, posMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), posMtx, posMtx);
    GXLoadPosMtxImm(posMtx, 0);
    GXSetCurrentMtx(0);

    if (work->progress <= float_4_80424dc0) {
        color = dat_80424d9c;
    } else {
        color = dat_80424d98;
    }
    GXSetTevColor(1, &color);

    color = dat_80424da0;
    ((u8*)&color)[3] = (u8)alpha;
    GXSetTevColor(2, &color);
    GXBegin(0x90, 0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3);
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset
