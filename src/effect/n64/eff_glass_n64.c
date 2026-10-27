#include "driver/effdrv.h"
#include "eff/n64/eff_glass_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>


static effN64Vtx size16x16_tex32x32_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 1024, 0, 0, 0, 0, 255},
    {8, 8, 0, 1024, 1024, 0, 0, 0, 255},
    {-8, 8, 0, 0, 1024, 0, 0, 0, 255},
};

static effN64Vtx size48x48_tex32x32_vtx[5] = {
    {-32, 0, 0, 0, 0, 255, 255, 255, 0},
    {0, -32, 0, 1024, 0, 255, 255, 255, 0},
    {32, 0, 0, 1024, 1024, 255, 255, 255, 0},
    {0, 32, 0, 0, 1024, 255, 255, 255, 0},
    {0, 0, 0, 0, 0, 255, 255, 255, 200},
};

void effGlassDisp(CameraId cameraId, EffectEntry* effect);
void effGlassMain(EffectEntry* effect);

EffectEntry* effGlassN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffGlassN64Work* work;

    effect = effEntry();
    effect->type = "GlassN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffGlassN64Work));
    effect->userdata = work;
    effect->callback = effGlassMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->alpha = 255;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    switch (type) {
        case 0:
        case 1:
            work->r1 = 255;
            work->g1 = 255;
            work->b1 = 255;
            work->r2 = 255;
            work->g2 = 255;
            work->b2 = 255;
            work->alpha2 = 255;
            break;
        case 2:
        case 3:
            work->r1 = 0;
            work->g1 = 255;
            work->b1 = 0;
            work->r2 = 0;
            work->g2 = 255;
            work->b2 = 0;
            work->alpha2 = 255;
            break;
    }
    work->texV = 0.0f;
    work->texVSpeed = (0.25f * (f32)(rand() % 2 * 2 - 1)) * (0.1f + 0.1f * (f32)(rand() % 5));
    work->rotZ = rand() % 360;
    work->vx = rand() % 11 - 5;
    work->vy = rand() % 11 - 5;
    work->vz = rand() % 11 - 5;
    work->rotY = rand() % 41 - 20;
    work->gravity = -0.1f;
    return effect;
}

void effGlassMain(EffectEntry* effect) {
    EffGlassN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->lifetime = 16;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    if (work->lifetime < 16) {
        work->alpha = work->lifetime * 16;
    }
    work->texV += work->texVSpeed;
    if (work->texV < 0.0f) {
        work->texV += 1.0f;
    }
    if (work->texV > 1.0f) {
        work->texV -= 1.0f;
    }
    work->vy += work->gravity;
    work->position.x += work->vx;
    work->position.y += work->vy;
    work->position.z += work->vz;
    work->rotZ += work->rotY;
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effGlassDisp, effect);
}

void effGlassDisp(CameraId cameraId, EffectEntry* effect) {
    s32 type;
    EffGlassN64Work* work;
    s32 alpha;
    CameraEntry* cam;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx rotMtx;
    Mtx viewMtx;
    GXTexObj texObj;
    s32 texU;
    s32 texV;
    s32 alpha2;
    s32 whole;
    f32 t4;
    cam = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->alpha;
    type = work->type;

    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rotMtx, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scaleMtx, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, rotMtx, mtx);
    PSMTXConcat(mtx, scaleMtx, mtx);
    PSMTXConcat(cam->view, mtx, viewMtx);
    PSMTXRotRad(rotMtx, 'z', 0.017453292f * work->rotZ);
    PSMTXConcat(viewMtx, rotMtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetNumChans(0);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_A1, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A1, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_C1, GX_CC_C0, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_A0, GX_CA_APREV, GX_CA_ZERO);
    effGetTexObjN64(0x59, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    type = type % 2;
    texU = type * 32;
    t4 = 4.0f * work->texV;
    whole = (s32)t4;
    texV = whole * 32;
    alpha2 = (s32)(256.0f * (t4 - (f32)((s32)work->texV * 4)));
    PSMTXScale(scaleMtx, 0.015625f, 0.0078125f, 0.0f);
    PSMTXTrans(mtx, texU, texV, 0.0f);
    PSMTXConcat(scaleMtx, mtx, scaleMtx);
    GXLoadTexMtxImm(scaleMtx, GX_TEXMTX0, GX_MTX2x4);
    PSMTXScale(scaleMtx, 0.015625f, 0.0078125f, 0.0f);
    PSMTXTrans(mtx, texU, texV + 32, 0.0f);
    PSMTXConcat(scaleMtx, mtx, scaleMtx);
    GXLoadTexMtxImm(scaleMtx, GX_TEXMTX1, GX_MTX2x4);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r1, work->g1, work->b1, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->r2, work->g2, work->b2, alpha2});
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x16_tex32x32_vtx);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
    if (type == 1 && (s32)(4.0f * work->texV) == 3) {
        GXSetTevColor(GX_TEVREG0, (GXColor){work->r1, work->g1, work->b1, alpha * alpha2 / 255});
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_RASA, GX_CA_ZERO);
        effSetVtxDescN64(size48x48_tex32x32_vtx);
        GXLoadPosMtxImm(viewMtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
        tri2(0, 1, 4, 0, 1, 2, 4, 0);
        tri2(0, 4, 3, 0, 4, 2, 3, 0);
    }
}
