#include "eff/n64/eff_thruhammer_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

double sin(double x);
double cos(double x);

typedef struct EffThruHammerN64Work {
    s16 type;         // 0x0
    Vec position;     // 0x4
    Vec velocity;     // 0x10
    Vec scale;        // 0x1C
    s32 timer;        // 0x28
    s32 counter;      // 0x2C
    f32 rotY;         // 0x30
    f32 rotZ;         // 0x34
    s32 alpha;        // 0x38
    f32 texS;         // 0x3C
    f32 texSVel;      // 0x40
    f32 texT;         // 0x44
    f32 texTVel;      // 0x48
    s32 delay;        // 0x4C
    u8 r1, g1, b1;    // 0x50
    u8 r2, g2, b2;    // 0x53
    u8 pad[2];        // 0x56
} EffThruHammerN64Work;


static effN64Vtx stg_thruhammer_v[] = {
    {-34, -2, 20, 4267, 1024, 255, 255, 255, 255},
    {-52, 50, 0, 3840, 2048, 255, 255, 255, 0},
    {-34, -2, -20, 3413, 1024, 255, 255, 255, 255},
    {-52, 50, -30, 3413, 2048, 255, 255, 255, 0},
    {-26, 50, -45, 2987, 2048, 255, 255, 255, 0},
    {0, -2, -40, 2560, 1024, 255, 255, 255, 255},
    {0, 50, -60, 2560, 2048, 255, 255, 255, 0},
    {26, 50, -45, 2133, 2048, 255, 255, 255, 0},
    {34, -2, -20, 1707, 1024, 255, 255, 255, 255},
    {-52, 50, 30, 4267, 2048, 255, 255, 255, 0},
    {-26, 50, 45, 4693, 2048, 255, 255, 255, 0},
    {0, -2, 40, 5120, 1024, 255, 255, 255, 255},
    {0, 50, 60, 5120, 2048, 255, 255, 255, 0},
    {26, 50, 45, 5547, 2048, 255, 255, 255, 0},
    {52, 50, -30, 1707, 2048, 255, 255, 255, 0},
    {52, 50, 0, 1280, 2048, 255, 255, 255, 0},
    {34, -2, 20, 853, 1024, 255, 255, 255, 255},
    {52, 50, 30, 853, 2048, 255, 255, 255, 0},
    {26, 50, 45, 427, 2048, 255, 255, 255, 0},
    {0, -2, 40, 0, 1024, 255, 255, 255, 255},
};

void effThruHammerMain(EffectEntry* effect);
void effThruHammerDisp(CameraId cameraId, EffectEntry* effect);
static void main_dl(EffectEntry* effect, Mtx mtx);

EffectEntry* effThruHammerN64Entry(s32 type, f32 x, f32 y, f32 z, f32 angle) {
    EffectEntry* effect;
    EffThruHammerN64Work* work;
    s32 n;
    s32 i;

    effect = effEntry();
    switch (type) {
    case 0:
        n = 4;
        break;
    case 1:
    default:
        n = 1;
        break;
    }
    effect->type = "ThruHammerN64";
    effect->count = n + 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffThruHammerN64Work) * (n + 1));
    effect->userdata = work;
    effect->callback = effThruHammerMain;
    work->timer = 60;
    work->counter = 0;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->alpha = 255;
    work->r1 = 0;
    work->g1 = 255;
    work->b1 = 122;
    work->r2 = 240;
    work->g2 = 255;
    work->b2 = 250;
    work++;
    for (i = 1; i < n + 1; i++, work++) {
        work->delay = -1 - (i - 1) * 4;
        work->rotZ = angle;
        work->rotY = (f32)(i * 30);
    }
    return effect;
}

void effThruHammerMain(EffectEntry* effect) {
    EffThruHammerN64Work* work = effect->userdata;
    s32 i;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    f32 sv;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->counter++;
    work->timer--;
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    if (work->counter >= 8) {
        work->alpha *= 0.8f;
    }
    for (i = 1, work++; i < effect->count; i++, work++) {
        work->delay++;
        if (work->delay < 0) {
            continue;
        }
        if (work->delay == 0) {
            work->position.x = 0.0f;
            work->position.y = 0.0f;
            work->position.z = 0.0f;
            sv = sin(6.2831855f * work->rotZ / 360.0f);
            work->velocity.x = 0.5f * -sv;
            sv = cos(6.2831855f * work->rotZ / 360.0f);
            work->velocity.y = 0.5f * sv;
            work->velocity.z = 0.0f;
            work->texS = 32.0f;
            work->texSVel = 32.0f;
            work->texTVel = 32.0f;
            work->scale.y = 10.0f;
            work->scale.x = 0.5f;
            work->scale.z = 0.5f;
            work->texT = 32.0f;
        }
        work->texS += work->texSVel;
        work->texT += work->texTVel;
        work->texSVel *= 0.9f;
        work->scale.x += 0.2f;
        work->scale.z += 0.2f;
        work->scale.y += 0.2f * (0.1f - work->scale.y);
        if (work->texS > 128.0f) {
            work->texS = 128.0f;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effThruHammerDisp, effect);
}

void effThruHammerDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffThruHammerN64Work* work;
    u8 r1, g1, b1, r2, g2, b2;
    s32 alpha;
    Mtx trans;
    Mtx rot;
    Mtx mtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    r1 = work->r1;
    g1 = work->g1;
    b1 = work->b1;
    r2 = work->r2;
    g2 = work->g2;
    b2 = work->b2;
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    effGetTexObjN64(0x23, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObjN64(0x24, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    effSetVtxDescN64(stg_thruhammer_v);
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(camera->view, trans, mtx);
    alpha = work->alpha;
    GXSetTevColor(GX_TEVREG0, (GXColor){r1, g1, b1, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){r2, g2, b2, 0});
    GXSetCullMode(GX_CULL_BACK);
    main_dl(effect, mtx);
    GXSetTevColor(GX_TEVREG0, (GXColor){r1, g1, b1, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){r2, g2, b2, 0});
    GXSetCullMode(GX_CULL_FRONT);
    main_dl(effect, mtx);
}

#pragma opt_lifetimes off
static void main_dl(EffectEntry* effect, Mtx camMtx) {
    EffThruHammerN64Work* work = effect->userdata;
    s32 i;
    Mtx m;
    Mtx scale;
    Mtx rot;
    f32 t;

    for (i = 1, work++; i < effect->count; i++, work++) {
        if (work->delay < 0) {
            continue;
        }
        PSMTXTrans(m, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rotZ);
        PSMTXConcat(m, rot, m);
        PSMTXScale(scale, 0.3f * work->scale.x, 0.3f * work->scale.y, 0.3f * work->scale.z);
        PSMTXConcat(m, scale, m);
        PSMTXRotRad(rot, 'y', 0.017453292f * work->rotY);
        PSMTXConcat(m, rot, m);
        PSMTXConcat(camMtx, m, m);
        GXLoadPosMtxImm(m, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        PSMTXScale(scale, 0.015873017f, 0.032258064f, 0.0f);
        t = work->texS;
        PSMTXTrans(m, 0.0f, t / 4.0f, 0.0f);
        PSMTXConcat(scale, m, scale);
        GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
        PSMTXScale(scale, 0.12903225f, 0.12903225f, 0.0f);
        t = work->texT;
        PSMTXTrans(m, 0.0f, t / 4.0f, 0.0f);
        PSMTXConcat(scale, m, scale);
        GXLoadTexMtxImm(scale, GX_TEXMTX1, GX_MTX2x4);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 54);
        tri2(0, 1, 2, 0, 2, 3, 4, 0);
        tri2(2, 4, 5, 0, 1, 3, 2, 0);
        tri2(4, 6, 5, 0, 5, 6, 7, 0);
        tri2(5, 7, 8, 0, 0, 9, 1, 0);
        tri2(10, 9, 0, 0, 11, 10, 0, 0);
        tri2(11, 12, 10, 0, 13, 12, 11, 0);
        tri2(8, 14, 15, 0, 7, 14, 8, 0);
        tri2(8, 15, 16, 0, 15, 17, 16, 0);
        tri2(16, 17, 18, 0, 16, 18, 19, 0);
    }
}
