#include "eff/n64/eff_wangflush_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
double sin(double x);

typedef struct EffWangFlushN64Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    f32 rotation;  // 0x10
    f32 rotSpeed;  // 0x14
    f32 scale;     // 0x18
    f32 scaleAdd;  // 0x1C
    f32 flashScale; // 0x20
    s32 alpha;     // 0x24
    s32 flashAlpha; // 0x28
    s32 timer;     // 0x2C
    s32 frame;     // 0x30
} EffWangFlushN64Work;

static effN64Vtx size16x16_tex64x64_vtx[] = {
    {-8, -8, 0, 0, 1024, 0, 0, 0, 255},
    {8, -8, 0, 2048, 1024, 0, 0, 0, 255},
    {8, 8, 0, 2048, 3072, 0, 0, 0, 255},
    {-8, 8, 0, 0, 3072, 0, 0, 0, 255},
};

static u8 color_rotation_data[] = {
    0xFF, 0xFF, 0x52, 0xFF, 0xFF, 0x52, 0xB5, 0x94,
    0x00, 0xB5, 0x94, 0x00, 0xFF, 0xFF, 0xD6, 0xFF,
    0xFF, 0xD6, 0xB5, 0x94, 0x00, 0xB5, 0x94, 0x00,
};

void effWangFlushMain(EffectEntry* effect);
void effWangFlushDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effWangFlushN64Entry(s32 type, f32 x, f32 y, f32 z, s32 timer) {
    EffectEntry* effect;
    EffWangFlushN64Work* work;

    effect = effEntry();
    effect->type = "WangflushN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffWangFlushN64Work));
    effect->userdata = work;
    effect->callback = effWangFlushMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->timer = timer;
    work->frame = 0;
    work->alpha = 0;
    work->rotation = 0.0f;
    if (type == 0) {
        work->rotSpeed = 30.0f;
    } else {
        work->rotSpeed = 0.0f;
    }
    work->scale = 0.0f;
    work->scaleAdd = 0.8f;
    work->flashAlpha = 0;
    work->flashScale = 0.0f;
    return effect;
}

void effWangFlushMain(EffectEntry* effect) {
    s32 timer;
    s32 frame;
    f32 sv;
    EffWangFlushN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->timer--;
    work->frame++;
    timer = work->timer;
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    if (frame <= 6) {
        work->alpha = (s32)(0.5f * (218 - work->alpha) + work->alpha);
    }
    if (frame > 6) {
        work->alpha *= 0.78f;
    }
    work->scale += work->scaleAdd;
    work->scaleAdd += 0.29f;
    work->rotation += work->rotSpeed;
    work->rotSpeed *= 0.93f;
    sv = (f32)sin((6.2831855f * (timer * 50)) / 360.0f);
    work->flashScale = 0.5f * sv + 0.9f;
    work->flashAlpha = (s32)(144.0f + 64.0f * (f32)sin((6.2831855f * (timer * 40)) / 360.0f));
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effWangFlushDisp, effect);
}

void effWangFlushDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffWangFlushN64Work* work;
    s32 idx;
    s32 alpha;
    s32 flashAlpha;
    s32 type;
    Mtx trans;
    Mtx scaleMtx;
    Mtx rot;
    Mtx mtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    type = work->type;
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    flashAlpha = work->flashAlpha;
    idx = work->frame * 3;
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    effGetTexObjN64(0x1C, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(trans, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(trans, GX_TEXMTX0, GX_MTX2x4);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x16_tex64x64_vtx);
    if (type == 0) {
        PSMTXScale(scaleMtx, work->flashScale, work->flashScale, work->flashScale);
        PSMTXConcat(mtx, scaleMtx, scaleMtx);
        GXLoadPosMtxImm(scaleMtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        idx = idx % 24;
        GXSetTevColor(GX_TEVREG0, (GXColor){color_rotation_data[idx], color_rotation_data[idx + 1], color_rotation_data[idx + 2], flashAlpha});
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
    alpha = work->alpha;
    if (alpha > 255) {
        alpha = 255;
    }
    if (alpha > 0) {
        PSMTXScale(scaleMtx, work->scale, work->scale, work->scale);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rotation);
        PSMTXConcat(rot, scaleMtx, scaleMtx);
        PSMTXConcat(mtx, scaleMtx, scaleMtx);
        GXLoadPosMtxImm(scaleMtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetTevColor(GX_TEVREG0, (GXColor){255, 255, 255, alpha});
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
