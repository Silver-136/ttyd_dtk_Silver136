#include "eff/n64/eff_stone_smoke_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

static effN64Vtx size16x16_tex32x32_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 1024, 0, 0, 0, 0, 255},
    {8, 8, 0, 1024, 1024, 0, 0, 0, 255},
    {-8, 8, 0, 0, 1024, 0, 0, 0, 255},
};

static u8 scale_data[7] = {0, 30, 80, 120, 112, 104, 100};
static u8 scale_data2[7] = {94, 92, 100, 105, 107, 105, 100};

void effStoneSmokeMain(EffectEntry* effect);
void effStoneSmokeDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effStoneSmokeN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffStoneSmokeN64Work* work;

    effect = effEntry();
    effect->type = "StoneSmokeN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffStoneSmokeN64Work));
    effect->userdata = work;
    effect->callback = effStoneSmokeMain;
    effect->flags |= 2;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->timer = 1000;
    } else {
        work->timer = lifetime;
    }
    work->alpha = 255;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->baseScale = scale;
    work->r = 255;
    work->g = 255;
    work->b = 255;
    work->r2 = 10;
    work->g2 = 10;
    work->b2 = 120;
    return effect;
}

void effStoneSmokeMain(EffectEntry* effect) {
    EffStoneSmokeN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 frame;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->timer = 16;
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
    if (work->timer < 16) {
        work->alpha = work->timer * 16;
    }
    work->position.y += 0.6f;
    if (frame < 7) {
        work->scale = 0.01f * scale_data[frame];
    } else {
        work->scale = 0.01f * scale_data2[(frame - 7) % 7];
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effStoneSmokeDisp, effect);
}

void effStoneSmokeDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffStoneSmokeN64Work* work;
    s32 alpha;
    f32 scale;
    Mtx mtx;
    Mtx scaleMtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    scale = work->baseScale * work->scale;
    alpha = work->alpha;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scaleMtx, scale, scale, scale);
    PSMTXConcat(mtx, scaleMtx, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->r2, work->g2, work->b2});
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scaleMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    effGetTexObjN64(0x5C, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x16_tex32x32_vtx);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
}
