#include "eff/n64/eff_stamp_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

double sin(double);
double cos(double);

static effN64Vtx size16x16_tex64x32_vtx[] = {
    {-8, 0, 0, 0, 0, 0, 0, 0, 0},
    {8, 0, 0, 2048, 0, 0, 0, 0, 0},
    {10, 16, 0, 2048, 1024, 0, 0, 0, 255},
    {-10, 16, 0, 0, 1024, 0, 0, 0, 255},
};

void effStampMain(EffectEntry* effect);
void effStampDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effStampN64Entry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffStampN64Work* work;
    s32 i;
    f32 rad;
    f32 angle;
    f32 one = 1.0f;

    effect = effEntry();
    effect->type = "StampN64";
    effect->count = 8;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffStampN64Work) * 8);
    effect->userdata = work;
    effect->callback = effStampMain;
    work->lifetime = 30;
    work->frame = 0;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->alpha = 255;
    switch (type) {
        case 0:
            work->r2 = 20;
            work->r = 20;
            work->g2 = 21;
            work->g = 21;
            work->b2 = 242;
            work->b = 242;
            break;
        case 1:
            work->r2 = 200;
            work->r = 200;
            work->g2 = 21;
            work->g = 21;
            work->b2 = 212;
            work->b = 212;
            break;
        case 2:
            work->r2 = 255;
            work->r = 255;
            work->g2 = 181;
            work->g = 181;
            work->b2 = 218;
            work->b = 218;
            break;
    }
    work++;
    for (i = 1; i < 8; i++, work++) {
        angle = (f32)((i - 1) * 360 / 7);
        rad = 6.2831855f * (90.0f + angle) / 360.0f;
        work->position.x = one * (f32)cos(rad);
        work->position.y = one * (f32)sin(rad);
        work->position.z = 0.0f;
        work->velocity.x = 0.0f;
        work->velocity.y = 0.0f;
        work->velocity.z = 0.0f;
        work->scaleY = 0.1f;
        work->scaleX = 0.1f;
        work->radius = one;
        work->angle = angle;
    }
    return effect;
}

void effStampMain(EffectEntry* effect) {
    EffStampN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 frame;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->frame++;
    work->lifetime--;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    if (frame > 10) {
        work->alpha *= 0.9f;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (frame > 10) {
            work->scaleY += 0.1f * (1.0f - work->scaleY);
            work->scaleX += 0.1f * (2.4f - work->scaleX);
            work->radius += 0.2f * (0.07f * (100.0f - work->radius));
        }
        if (frame < 10) {
            work->scaleY += 0.05f * (7.0f - work->scaleY);
            work->scaleX += 0.1f * (1.0f - work->scaleX);
            work->radius += 0.6f * (0.05f * (10.0f - work->radius));
        }
        work->position.x = work->radius * (f32)cos(6.2831855f * (90.0f + work->angle) / 360.0f);
        work->position.y = work->radius * (f32)sin(6.2831855f * (90.0f + work->angle) / 360.0f);
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effStampDisp, effect);
}

void effStampDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffStampN64Work* work;
    s32 i;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx viewMtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_RASA, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(camera->view, mtx, viewMtx);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, work->alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->r2, work->g2, work->b2});
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x2B, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    work++;
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x16_tex64x32_vtx);
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->angle);
        PSMTXScale(scale, work->scaleX, work->scaleY, 1.0f);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(viewMtx, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
