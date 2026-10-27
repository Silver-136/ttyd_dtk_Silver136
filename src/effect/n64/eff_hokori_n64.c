#include "eff/n64/eff_hokori_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>

effN64Vtx size16x32_tex32x64_vtx[] = {
    {-8, -32, 0, 0, 1024, 0, 0, 0, 255},
    {8, -32, 0, 1024, 1024, 0, 0, 0, 255},
    {8, 0, 0, 1024, 3040, 0, 0, 0, 255},
    {-8, 0, 0, 0, 3040, 0, 0, 0, 255},
};

static effN64Vtx size16x16_tex32x32_vtx[] = {
    {-8, -16, 0, 0, 0, 0, 0, 0, 255},
    {8, -16, 0, 1024, 0, 0, 0, 0, 255},
    {8, 0, 0, 1024, 1024, 0, 0, 0, 255},
    {-8, 0, 0, 0, 1024, 0, 0, 0, 255},
};

static effN64Vtx size8x8_tex32x32_vtx[] = {
    {-4, -4, 0, 0, 0, 0, 0, 0, 255},
    {4, -4, 0, 1024, 0, 0, 0, 0, 255},
    {4, 4, 0, 1024, 1024, 0, 0, 0, 255},
    {-4, 4, 0, 0, 1024, 0, 0, 0, 255},
};

void effHokoriMain(EffectEntry* effect);
void effHokoriDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effHokoriN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z) {
    s32 count;
    EffectEntry* effect;
    EffHokoriN64Work* work;
    EffHokoriN64Work* p;
    s32 i;

    effect = effEntry();
    count = 4 + (type <= 1);
    effect->type = "HokoriN64";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, count * sizeof(EffHokoriN64Work));
    effect->userdata = work;
    effect->callback = effHokoriMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->lifetime = lifetime;
    work->alpha = 255;
    work->r = 160;
    work->g = 160;
    work->b = 140;
    work->r2 = 160;
    work->g2 = 160;
    work->b2 = 140;
    p = work + 1;
    for (i = 1; i < count; i++, p++) {
        p->position.x = rand() % 200 - 100;
        p->position.y = rand() % 50;
        p->position.z = 0.0f;
        p->velY = -1.0f;
        p->gravity = 0.0f;
        p->scaleY = 1.0f;
        p->scaleX = 1.0f;
        p->growth = (rand() % 10) / 200.0f;
        switch (type) {
            case 0:
                p->growth2 = 0.1f * p->growth;
                break;
            case 1:
                p->growth2 = 0.2f * p->growth;
                break;
            default:
                p->growth2 = p->growth = 0.4f;
                p->velY = 0.0f;
                p->position.x = 0.1f * (rand() % 40 - 20);
                p->position.y = 0.1f * (rand() % 40 - 20);
                break;
        }
    }
    return effect;
}

void effHokoriMain(EffectEntry* effect) {
    EffHokoriN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 type;
    s32 i;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->lifetime--;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    type = work->type;
    if (work->lifetime < 24) {
        work->alpha *= 0.9f;
    }
    i = 1;
    work++;
    if (type <= 1) {
        for (; i < effect->count; i++, work++) {
            work->velY += work->gravity;
            work->position.y += work->velY;
            work->growth += 0.005f;
            work->scaleY += work->growth;
            work->scaleX += work->growth2;
        }
    } else {
        for (; i < effect->count; i++, work++) {
            work->velY += work->gravity;
            work->position.y += work->velY;
            work->scaleY += work->growth;
            work->scaleX += work->growth2;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effHokoriDisp, effect);
}

void effHokoriDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffHokoriN64Work* work;
    GXTexObj tex;
    s32 i;
    s32 type;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx base;
    effN64Vtx* vtx;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.03125f, 0.010416667f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, work->alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->r2, work->g2, work->b2, 0});
    switch (work->type) {
        case 0:
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            effGetTexObjN64(21, &tex);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            break;
        default:
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            effGetTexObjN64(22, &tex);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            break;
    }
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, base);
    PSMTXConcat(camera->view, base, base);
    type = work->type;
    work++;
    GXSetCullMode(GX_CULL_NONE);
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, work->scaleX, work->scaleY, work->scaleX);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(base, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        if (type <= 1) {
            if (i & 1) {
                vtx = size16x32_tex32x64_vtx;
            } else {
                vtx = size16x16_tex32x32_vtx;
            }
        } else {
            vtx = size8x8_tex32x32_vtx;
        }
        effSetVtxDescN64(vtx);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
