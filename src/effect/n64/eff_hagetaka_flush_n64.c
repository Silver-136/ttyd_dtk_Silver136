#include "eff/n64/eff_hagetaka_flush_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

typedef struct EffHagetakaFlushN64Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    s32 timer;     // 0x10
    s32 frame;     // 0x14
    s32 r;         // 0x18
    s32 g;         // 0x1C
    s32 b;         // 0x20
    s32 alpha;     // 0x24
    f32 baseScale; // 0x28
    f32 scale;     // 0x2C
} EffHagetakaFlushN64Work;

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

void effHagetakaFlushMain(EffectEntry* effect);
void effHagetakaFlushDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effHagetakaFlushN64Entry(s32 type, s32 unused, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffHagetakaFlushN64Work* work;

    effect = effEntry();
    effect->type = "HagetakaFlushN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffHagetakaFlushN64Work));
    effect->userdata = work;
    effect->callback = effHagetakaFlushMain;
    effect->flags |= 2;
    work->type = type;
    work->frame = 0;
    work->timer = 10;
    work->alpha = 255;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->baseScale = scale;
    work->r = 70;
    work->g = 180;
    work->b = 120;
    work->scale = 0.0f;
    return effect;
}

void effHagetakaFlushMain(EffectEntry* effect) {
    EffHagetakaFlushN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 frame;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->timer = 10;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    work->frame++;
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    if (work->timer < 4) {
        work->scale *= 0.5f;
    } else {
        work->scale = 0.3f + 0.2f * frame;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effHagetakaFlushDisp, effect);
}

void effHagetakaFlushDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffHagetakaFlushN64Work* work;
    s32 alpha;
    f32 scale;
    s32 idx;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx rot;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    scale = work->baseScale * work->scale;
    alpha = work->alpha;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scaleMtx, scale, scale, scale);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scaleMtx, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scaleMtx, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x1C, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    idx = work->frame * 3;
    idx %= 24;
    GXSetTevColor(GX_TEVREG0, (GXColor){color_rotation_data[idx], color_rotation_data[idx + 1], color_rotation_data[idx + 2], alpha});
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x16_tex64x64_vtx);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
}
