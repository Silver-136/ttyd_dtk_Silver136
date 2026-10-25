#include "eff/n64/eff_kemuri12_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>


static effN64Vtx size32x32_tex32x32_vtx[] = {
    {-16, -16, 0, 0, 0, 0, 0, 0, 255},
    {16, -16, 0, -1024, 0, 0, 0, 0, 255},
    {16, 16, 0, -1024, 1024, 0, 0, 0, 255},
    {-16, 16, 0, 0, 1024, 0, 0, 0, 255},
};

effN64Vtx size32x32_tex32x32_yougan_vtx[] = {
    {-16, -16, 15, 0, 0, 0, 0, 0, 255},
    {16, -16, 15, -1024, 0, 0, 0, 0, 255},
    {16, 16, 15, -1024, 1024, 0, 0, 0, 255},
    {-16, 16, 15, 0, 1024, 0, 0, 0, 255},
    {-6, -16, -15, 0, 0, 0, 0, 0, 255},
    {26, -16, -15, -1024, 0, 0, 0, 0, 255},
    {26, 16, -15, -1024, 1024, 0, 0, 0, 255},
    {-6, 16, -15, 0, 1024, 0, 0, 0, 255},
    {-26, -16, -15, 0, 0, 0, 0, 0, 255},
    {6, -16, -15, -1024, 0, 0, 0, 0, 255},
    {6, 16, -15, -1024, 1024, 0, 0, 0, 255},
    {-26, 16, -15, 0, 1024, 0, 0, 0, 255},
};

void effKemuri12Main(EffectEntry* effect);
void effKemuri12Disp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effKemuri12N64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffKemuri12N64Work* work;

    effect = effEntry();
    effect->type = "Kemuri12N64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffKemuri12N64Work));
    effect->userdata = work;
    effect->callback = effKemuri12Main;
    work->timer = duration;
    work->frame = 0;
    work->duration = duration;
    work->progress = 0.0f;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    switch (type) {
    case 0:
        work->b = 0;
        work->g = 0;
        work->r = 0;
        work->a = 130;
        break;
    case 1:
        work->r = 255;
        work->b = 0;
        work->g = 0;
        work->a = 130;
        break;
    default:
        work->r = 225;
        work->g = 215;
        work->b = 255;
        work->a = 180;
        break;
    }
    return effect;
}

void effKemuri12Main(EffectEntry* effect) {
    EffKemuri12N64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->progress = 8.0f * work->frame / work->duration;
    work->frame++;
    work->timer--;
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effKemuri12Disp, effect);
}


void effKemuri12Disp(CameraId cameraId, EffectEntry *effect) {
    s32 type;
    CameraEntry *camera;
    s32 last;
    EffKemuri12N64Work *work;
    s32 idx;
    s32 ofs;
    s32 alpha;
    effN64Vtx *vtx;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx rot;
    GXTexObj texObj;
    camera = camGetPtr(cameraId);
    work = effect->userdata;
    idx = (s32) work->progress;
    alpha = 256.0f * (work->progress - ((f32) idx));
    // FAKE: integer-cast load makes type an unnamed temp (numbered above the camera copy, coloured first)
    type = (u16)(u32)work->type;
    last = idx == 7;
    GXSetNumChans(0);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A1, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    effGetTexObjN64(0xE, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * (-camGetPtr(CAMERA_3D)->viewYaw));
    PSMTXScale(scaleMtx, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scaleMtx, mtx);
    PSMTXRotRad(rot, 'z', 0.34906584f);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, work->a});
    GXSetTevColor(GX_TEVREG1, (GXColor){0, 0, 0, alpha});
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    ofs = idx << 5;
    PSMTXScale(scaleMtx, 0.00390625f, 0.03125f, 0.0f);
    PSMTXTrans(mtx, ofs, 0.0f, 0.0f);
    PSMTXConcat(scaleMtx, mtx, scaleMtx);
    GXLoadTexMtxImm(scaleMtx, GX_TEXMTX0, GX_MTX2x4);
    if (last) {
        PSMTXScale(scaleMtx, 0.00390625f, 0.03125f, 0.0f);
        PSMTXTrans(mtx, ofs + 32, 32.0f, 0.0f);
        PSMTXConcat(scaleMtx, mtx, scaleMtx);
        GXLoadTexMtxImm(scaleMtx, GX_TEXMTX1, GX_MTX2x4);
    } else {
        PSMTXScale(scaleMtx, 0.00390625f, 0.03125f, 0.0f);
        PSMTXTrans(mtx, ofs + 32, 0.0f, 0.0f);
        PSMTXConcat(scaleMtx, mtx, scaleMtx);
        GXLoadTexMtxImm(scaleMtx, GX_TEXMTX1, GX_MTX2x4);
    }
    switch (type) {
        case 0:

        case 2:
            vtx = size32x32_tex32x32_vtx;
            break;

        default:
            work->position.z = work->position.z;
            // FAKE: float self-assign keeps the camera web live through the switch pre-RA (gives it degree >= K); emits no code
            camera->near = camera->near;
            vtx = size32x32_tex32x32_yougan_vtx;
            break;

    }

    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(vtx);
    switch (type) {
        case 0:

        case 2:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            break;

        default:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 18);
            tri2(8, 9, 10, 0, 8, 10, 11, 0);
            tri2(4, 5, 6, 0, 4, 6, 7, 0);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            break;

    }

}

