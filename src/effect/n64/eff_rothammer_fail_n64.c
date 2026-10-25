#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

#pragma opt_propagation off


typedef struct EffRotHammerFailN64Work {
    s16 type;      // 0x0
    Vec position;  // 0x4
    Vec velocity;  // 0x10
    f32 scaleY;    // 0x1C
    f32 scaleX;    // 0x20
    s32 lifetime;  // 0x24
    s32 frame;     // 0x28
    s32 total;     // 0x2C
    f32 progress;  // 0x30
} EffRotHammerFailN64Work;

static effN64Vtx size24x24_tex32x32_vtx[] = {
    {-12, -12, 0, 0, 0, 0, 0, 0, 255},
    {12, -12, 0, -1024, 0, 0, 0, 0, 255},
    {12, 12, 0, -1024, 1024, 0, 0, 0, 255},
    {-12, 12, 0, 0, 1024, 0, 0, 0, 255},
};

void effRotHammerFailMain(EffectEntry* effect);
void effRotHammerFailDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effRotHammerFailN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffRotHammerFailN64Work* child;
    f32 zero;
    EffRotHammerFailN64Work* work;


    effect = effEntry();
    effect->type = "RotHammerFailN64";
    effect->count = 2;
    effect->userdata = work = __memAlloc(HEAP_EFFECT, sizeof(EffRotHammerFailN64Work) * 2);
    zero = 0.0f;
    effect->callback = effRotHammerFailMain;
    work->lifetime = time;
    child = work++;
    child = work;
    work--;
    work->frame = 0;
    work->total = time;
    work->progress = zero;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scaleY = 10.0f;
    work->scaleX = 30.0f;
    work[1].position.x = zero;
    work[1].position.y = zero;
    work[1].position.z = zero;
    child->velocity.x = (f32)sin(zero);
    child->velocity.y = zero;
    child->velocity.z = (f32)cos(zero);
    return effect;
}

void effRotHammerFailMain(EffectEntry* effect) {
    EffRotHammerFailN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    f32 scale;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->progress = 7.0f * (f32)work->frame / (f32)work->total;
    work->frame++;
    work->lifetime--;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    work->scaleY += 0.1f * (work->scaleX - work->scaleY);
    scale = work->scaleY;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        work->velocity.y += 0.04f;
        work->velocity.y *= 0.9f;
        work->position.x = scale * work->velocity.x;
        work->position.y += work->velocity.y;
        work->position.z = scale * work->velocity.z;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effRotHammerFailDisp, effect);
}

void effRotHammerFailDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffRotHammerFailN64Work* work;
    s32 isSix;
    s32 i;
    s32 idx;
    s32 x;
    s32 alpha;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx viewMtx;
    GXTexObj texObj;
    static const GXColor c0 = {0xE6, 0xDE, 0xDE, 0x82};

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    idx = work->progress;
    alpha = 256.0f * (work->progress - (f32)idx);
    isSix = idx == 6;
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    effGetTexObjN64(0xD, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A1, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size24x24_tex32x32_vtx);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(camera->view, mtx, viewMtx);
    GXSetTevColor(GX_TEVREG0, c0);
    GXSetTevColor(GX_TEVREG1, (GXColor){0, 0, 0, alpha});
    x = idx * 32;
    PSMTXScale(scale, 0.004464286f, 0.03125f, 0.0f);
    PSMTXTrans(mtx, (f32)x, 0.0f, 0.0f);
    PSMTXConcat(scale, mtx, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    if (isSix) {
        PSMTXScale(scale, 0.004464286f, 0.03125f, 0.0f);
        PSMTXTrans(mtx, (f32)(x + 32), 32.0f, 0.0f);
        PSMTXConcat(scale, mtx, mtx);
        GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
    } else {
        PSMTXScale(scale, 0.004464286f, 0.03125f, 0.0f);
        PSMTXTrans(mtx, (f32)(x + 32), 0.0f, 0.0f);
        PSMTXConcat(scale, mtx, mtx);
        GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXConcat(viewMtx, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
