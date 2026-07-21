#include "effect/n64/eff_akari_sflush_n64.h"
void effAkariSflushMain(void* effect);
void effAkariSflushDisp(s32 cameraId, void* entry);

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffAkariSflushWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    s32 frame;
    s32 r;
    s32 g;
    s32 b;
    s32 alpha;
    f32 scale;
    f32 scale2;
    s32 unk30;
} EffAkariSflushWork;

void* effEntry(void);
void effDelete(void* effect);
void* __memAlloc(s32 heap, s32 size);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fab08;
extern char str_AkariSflushN64_802fab20[];
extern f32 float_1_80424c8c;
extern f32 float_0p3_80424c98;


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effAkariSflushN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffAkariSflushWork* work;
    f32 scale2;

    *(char**)((s32)entry + 0x14) = str_AkariSflushN64_802fab20;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x34);
    *(EffAkariSflushWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effAkariSflushMain;
    *(u32*)entry |= 2;

    work->type = type;
    work->frame = 0;
    if (timer <= 0) {
        work->timer = 1000;
    } else {
        work->timer = timer;
    }
    work->alpha = 0xFF;
    scale2 = float_1_80424c8c;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->r = 0xFF;
    work->g = 0xE6;
    work->b = 0x32;
    work->unk30 = 0;
    work->scale2 = scale2;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

void effAkariSflushMain(void* effect) {
    void* entry = effect;
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffAkariSflushWork* work;
    s32 timer;
    s32 frame;

    work = *(EffAkariSflushWork**)((s32)entry + 0xC);
    zero = &vec3_802fab08;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    if (*(u32*)entry & 4) {
        *(u32*)entry &= ~4;
        work->timer = 0x10;
    }

    if (work->timer < 1000) {
        work->timer--;
    }
    work->frame++;

    timer = work->timer;
    if (timer < 0) {
        effDelete(entry);
    } else {
        frame = work->frame;
        if (timer < 8) {
            work->scale2 += float_0p3_80424c98 * (f32)(8 - timer);
        }
        if (frame < 0x10) {
            work->alpha = frame * 0x10 + 0xF;
        }
        if (timer < 8) {
            work->alpha = timer * 0x20 + 0x1F;
        }
        dispEntry(4, 2, effAkariSflushDisp, entry, dispCalcZ(&dispPos));
    }
}

void effAkariSflushDisp(s32 cameraId, void* entry) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, f32 angle, s32 axis);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    extern f64 cos(f64 x);
    extern f32 float_deg2rad_80424c7c;
    extern f32 float_6p2832_80424c80;
    extern f32 float_360_80424c84;
    extern f32 float_0p1_80424c88;
    extern f32 float_1_80424c8c;
    extern f32 float_30_80424c90;
    extern f32 float_0_80424c94;
    extern u32 dat_80424c78;
    extern u32 unk_80429648;
    extern u8 size16x16_tex32x32_vtx[];

    void* cam;
    void* cam3d;
    EffAkariSflushWork* work;
    s32 frame;
    s32 alpha;
    s32 i;
    s32 j;
    s32 baseAngle;
    s32 spinAngle;
    u32 color;
    f32 ringScale;
    f32 baseMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    f32 viewMtx[3][4];
    f32 ringMtx[3][4];
    f32 segMtx[3][4];
    f32 tmpMtx[3][4];

    cam = camGetPtr(cameraId);
    work = *(EffAkariSflushWork**)((s32)entry + 0xC);
    frame = work->frame;
    alpha = work->alpha;

    PSMTXTrans(baseMtx, work->x, work->y, work->z);
    cam3d = camGetPtr(4);
    PSMTXRotRad(rotMtx, float_deg2rad_80424c7c * -*(f32*)((s32)cam3d + 0x114), 'y');
    PSMTXScale(scaleMtx, work->scale, work->scale, work->scale);
    PSMTXConcat(baseMtx, rotMtx, baseMtx);
    PSMTXConcat(baseMtx, scaleMtx, baseMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), baseMtx, viewMtx);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevAlphaIn(0, 7, 1, 5, 7);
    GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 2, 4, 0, 0xF);
    GXSetTevAlphaIn(1, 7, 7, 7, 0);
    GXSetNumTexGens(0);

    color = unk_80429648;
    ((u8*)&color)[0] = (u8)work->r;
    ((u8*)&color)[1] = (u8)work->g;
    ((u8*)&color)[2] = (u8)work->b;
    ((u8*)&color)[3] = (u8)alpha;
    GXSetTevColor(1, &color);
    color = dat_80424c78;
    GXSetTevColor(2, &color);
    GXSetCullMode(0);
    effSetVtxDescN64(size16x16_tex32x32_vtx);

    baseAngle = frame * 8;
    spinAngle = 0;
    for (i = 0; i < work->unk30 + 1; i++) {
        ringScale = ((float_0p1_80424c88 * (f32)cos((f64)((float_6p2832_80424c80 * (f32)(baseAngle + ((i * 0xB4) / (work->unk30 + 1)))) / float_360_80424c84))) + (f32)(i + 1)) * work->scale2;
        PSMTXRotRad(rotMtx, float_deg2rad_80424c7c * (f32)spinAngle, 'z');
        PSMTXScale(scaleMtx, ringScale, ringScale, ringScale);
        PSMTXConcat(rotMtx, scaleMtx, ringMtx);

        for (j = 0; j < 5; j++) {
            PSMTXRotRad(rotMtx, float_deg2rad_80424c7c * (f32)(j * 0x48 + 0x5A), 'z');
            PSMTXScale(scaleMtx, float_1_80424c8c, float_1_80424c8c, float_1_80424c8c);
            PSMTXConcat(rotMtx, scaleMtx, rotMtx);
            PSMTXTrans(tmpMtx, float_30_80424c90, float_0_80424c94, float_0_80424c94);
            PSMTXConcat(rotMtx, tmpMtx, tmpMtx);
            PSMTXRotRad(rotMtx, float_0_80424c94, 'x');
            PSMTXConcat(tmpMtx, rotMtx, segMtx);
            PSMTXConcat(viewMtx, ringMtx, tmpMtx);
            PSMTXConcat(tmpMtx, segMtx, tmpMtx);
            GXLoadPosMtxImm(tmpMtx, 0);
            GXSetCurrentMtx(0);
            GXBegin(0x90, 0, 0xC);
            tri2(0, 1, 2, 0, 1, 2, 3);
            tri2(2, 3, 4, 0, 3, 4, 5);
        }

        baseAngle += 0xB4;
        spinAngle += 0x24;
    }
}
