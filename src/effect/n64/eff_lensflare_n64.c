#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "libc_decls.h"

double cos(double);

// One element per flare: element 0 is the head (type, position, fade and timers),
// elements 1-5 are the rings (angle, scale, distance along the flare axis, speed).
typedef struct EffLensflareN64Elem {
    s32 pad0;     // 0x0
    s32 type;     // 0x4
    Vec position; // 0x8
    s32 alpha;    // 0x14
    s32 lifetime; // 0x18
    s32 frame;    // 0x1C
    f32 angle;    // 0x20
    u8 pad24[4];  // 0x24
    f32 scale;    // 0x28
    f32 offset;   // 0x2C
    f32 speed;    // 0x30
} EffLensflareN64Elem;

static effN64Vtx size48x48_tex32x32_vtx[] = {
    {-24, -24, 0, 0, 0, 0, 0, 0, 255},
    {24, -24, 0, 1024, 0, 0, 0, 0, 255},
    {24, 24, 0, 1024, 1024, 0, 0, 0, 255},
    {-24, 24, 0, 0, 1024, 0, 0, 0, 255},
};

static effN64Vtx size10x10_tex32x32_vtx[] = {
    {0, -5, 0, 0, 0, 0, 0, 0, 255},
    {10, -5, 0, 1024, 0, 0, 0, 0, 255},
    {10, 5, 0, 1024, 1024, 0, 0, 0, 255},
    {0, 5, 0, 0, 1024, 0, 0, 0, 255},
    {0, -5, 0, 0, 1024, 0, 0, 0, 255},
    {10, -5, 0, 1024, 1024, 0, 0, 0, 255},
    {10, 5, 0, 1024, 2048, 0, 0, 0, 255},
    {0, 5, 0, 0, 2048, 0, 0, 0, 255},
    {-5, -5, 0, 0, 2048, 0, 0, 0, 255},
    {5, -5, 0, 1024, 2048, 0, 0, 0, 255},
    {5, 5, 0, 1024, 3072, 0, 0, 0, 255},
    {-5, 5, 0, 0, 3072, 0, 0, 0, 255},
    {-5, -5, 0, 0, 3072, 0, 0, 0, 255},
    {5, -5, 0, 1024, 3072, 0, 0, 0, 255},
    {5, 5, 0, 1024, 4096, 0, 0, 0, 255},
    {-5, 5, 0, 0, 4096, 0, 0, 0, 255},
};

void effLensflareMain(EffectEntry* effect);
void effLensflareDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effLensflareN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffLensflareN64Elem* work;
    EffLensflareN64Elem* e;
    s32 i;

    effect = effEntry();
    effect->type = "LensflareN64";
    effect->count = 6;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffLensflareN64Elem) * 6);
    effect->userdata = work;
    effect->callback = effLensflareMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 100;
    } else {
        work->lifetime = lifetime;
    }
    work->alpha = 0;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    e = work + 1;
    for (i = 1; i < 6; i++, e++) {
        e->angle = (f32)(rand() % 360);
        e->offset = (f32)((i * 2) % 20 + 60);
        e->scale = 0.1f * (f32)(rand() % 16) + 0.4f;
        e->speed = 0.1f * (f32)(rand() % 11) + 0.1f;
    }
    return effect;
}

void effLensflareMain(EffectEntry* effect) {
    EffLensflareN64Elem* e = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    f32 cs;

    tmp.x = e->position.x;
    tmp.y = e->position.y;
    tmp.z = e->position.z;
    position = tmp;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        e->lifetime = 30;
    }
    if (e->lifetime < 100) {
        e->lifetime--;
    }
    e->frame++;
    if (e->lifetime < 0) {
        effDelete(effect);
        return;
    }
    if (20 >= e->frame) {
        e->alpha = e->frame * 12;
    } else if (20 > e->lifetime) {
        e->alpha = e->lifetime * 12;
    }
    for (i = 1, e++; i < effect->count; i++, e++) {
        cs = (f32)cos(6.2831855f * (90.0f * (e->offset - 70.0f) / 10.0f) / 360.0f);
        e->alpha = (s32)(255.0f * cs);
        e->offset += e->speed;
        if (e->offset > 80.0f) {
            e->offset = 60.0f;
            e->scale = 0.1f * (f32)(rand() % 16) + 0.4f;
            e->speed = 0.1f * (f32)(rand() % 11) + 0.1f;
            e->angle = (f32)(rand() % 360);
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effLensflareDisp, effect);
}

void effLensflareDisp(CameraId cameraId, EffectEntry* effect) {
    s32 ang1;
    s32 odd;
    s32 ang2;
    EffLensflareN64Elem* e;
    s32 frame;
    CameraEntry* camera;
    s32 i;
    s32 alpha;
    Mtx A;
    Mtx D;
    Mtx B;
    Mtx C;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    e = effect->userdata;
    frame = e->frame;
    alpha = e->alpha;
    odd = frame & 1;
    if (odd) {
        alpha = (s32)(0.95f * alpha);
    }
    PSMTXTrans(A, e->position.x, e->position.y, e->position.z);
    PSMTXRotRad(B, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(A, B, C);
    PSMTXConcat(camera->view, C, C);
    GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0x40, 0x40, alpha});
    PSMTXRotRad(B, 'z', 0.017453292f * (f32)(frame + odd * 2));
    PSMTXConcat(C, B, B);
    GXLoadPosMtxImm(B, GX_PNMTX0);
    GXSetCurrentMtx(0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(D, 0.0625f, 0.0625f, 0.0f);
    GXLoadTexMtxImm(D, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x6F, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0x40, 0x40, alpha});
    effSetVtxDescN64(size48x48_tex32x32_vtx);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
    effGetTexObjN64(0x86, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    PSMTXScale(D, 0.03125f, 0.0078125f, 0.0f);
    GXLoadTexMtxImm(D, GX_TEXMTX0, GX_MTX2x4);
    effSetVtxDescN64(size10x10_tex32x32_vtx);
    for (i = 1, ang1 = 10, ang2 = 35, e++; i < effect->count; ang1 += 10, ang2 += 35, i++, e++) {
        f32 scale = e->scale;
        if (!scale) {
            continue;
        }
        GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0xFF, (e->alpha * alpha) >> 8});
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        PSMTXRotRad(B, 'z', 0.017453292f * e->angle);
        PSMTXTrans(A, e->offset, 0.0f, 0.0f);
        PSMTXConcat(B, A, A);
        PSMTXScale(D, scale, scale, scale);
        PSMTXConcat(A, D, A);
        PSMTXConcat(C, A, A);
        GXLoadPosMtxImm(A, GX_PNMTX0);
        GXSetCurrentMtx(0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(12, 13, 14, 0, 12, 14, 15, 0);
        PSMTXRotRad(B, 'z', 0.017453292f * e->angle);
        PSMTXTrans(A, 5.0f + e->offset, 0.0f, 0.0f);
        PSMTXConcat(B, A, A);
        PSMTXScale(D, 2.3f - scale, 2.3f - scale, 2.3f - scale);
        PSMTXConcat(A, D, A);
        PSMTXConcat(C, A, A);
        GXLoadPosMtxImm(A, GX_PNMTX0);
        GXSetCurrentMtx(0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(8, 9, 10, 0, 8, 10, 11, 0);
        GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0x00, 0xFF, (e->alpha * alpha) >> 9});
        GXSetTevColor(GX_TEVREG1, (GXColor){0xFF, 0x80, 0xFF, 0x00});
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        PSMTXRotRad(B, 'z', 0.017453292f * (14.0f + e->angle));
        PSMTXRotRad(A, 'y', 0.017453292f * (f32)(frame + ang1 % 45));
        PSMTXConcat(B, A, B);
        PSMTXTrans(A, 20.0f, 0.0f, 0.0f);
        PSMTXConcat(B, A, A);
        PSMTXScale(D, 1.0f + 0.05f * (e->offset - 60.0f), 2.0f - 0.05f * (e->offset - 60.0f), 1.0f);
        PSMTXConcat(A, D, A);
        PSMTXConcat(C, A, A);
        GXLoadPosMtxImm(A, GX_PNMTX0);
        GXSetCurrentMtx(0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(4, 5, 6, 0, 4, 6, 7, 0);
        GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0x40, 0x40, (e->alpha * alpha) >> 8});
        PSMTXRotRad(B, 'z', 0.017453292f * (34.0f + e->angle + (f32)frame));
        PSMTXRotRad(A, 'y', 0.017453292f * (f32)ang2);
        PSMTXConcat(B, A, B);
        PSMTXTrans(A, 15.0f, 0.0f, 0.0f);
        PSMTXConcat(B, A, A);
        PSMTXScale(D, 0.1f * (e->offset - 60.0f) + 2.0f, 1.0f, 1.0f);
        PSMTXConcat(A, D, A);
        PSMTXConcat(C, A, A);
        GXLoadPosMtxImm(A, GX_PNMTX0);
        GXSetCurrentMtx(0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
