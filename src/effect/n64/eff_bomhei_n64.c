// Paper Mario 64 effect: Bob-omb (bomhei) sprite that steps through its texture frames over its duration;
// non-zero types also spin it.

#include "eff/n64/eff_bomhei_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

typedef struct EffBomheiN64Work {
    s16 type;       // 0x0
    Vec position;   // 0x4
    f32 scale;      // 0x10
    s32 timer;      // 0x14
    s32 frame;      // 0x18
    s32 duration;   // 0x1C
    f32 progress;   // 0x20
    f32 rotation;   // 0x24
    f32 rotSpeed;   // 0x28
} EffBomheiN64Work;

/** Camera-facing 32x32 Bob-omb sprite, cross-faded between frames of its texture sheet. */
static effN64Vtx size32x32_tex32x32_vtx[] ATTRIBUTE_ALIGN(8) = {
    {-16, -16, 0, 0, 0, 0, 0, 0, 255},
    {16, -16, 0, 1024, 0, 0, 0, 0, 255},
    {16, 16, 0, 1024, 1024, 0, 0, 0, 255},
    {-16, 16, 0, 0, 1024, 0, 0, 0, 255},
};

static void effBomheiMain(EffectEntry* effect);
static void effBomheiDisp(CameraId cameraId, void* payload);

/**
 * Spawns a Bob-omb sprite at (x, y, z), `scale` times its size, living `duration` frames;
 * type 0 stays upright and any other type spins at 40 degrees per frame.
 */
EffectEntry* effBomheiN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffBomheiN64Work* work;

    effect = effEntry();
    effect->type = "BomheiN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffBomheiN64Work));
    effect->userdata = work;
    effect->callback = effBomheiMain;
    work->timer = duration;
    work->frame = 0;
    work->duration = duration;
    work->progress = 0.0f;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->rotation = 0.0f;
    if (type == 0) {
        work->rotSpeed = 0.0f;
    } else {
        work->rotSpeed = 40.0f;
    }
    return effect;
}

/** Advances a progress value from 0 to 9 across the duration, which Disp uses to cross-fade between sprite frames. */
static void effBomheiMain(EffectEntry* effect) {
    EffBomheiN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->progress = 9.0f * work->frame / work->duration;
    work->frame++;
    work->timer--;
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    work->rotation += work->rotSpeed;
    dispEntry(CAMERA_3D, dispCalcZ(&position), DISP_MODE_XLU, effBomheiDisp, effect);
}

/**
 * Draws the sprite, cross-fading between two frames of one texture sheet by the progress fraction;
 * tinted yellow until progress passes 4, then white.
 */
static void effBomheiDisp(CameraId cameraId, void* payload) {
    EffectEntry* effect = payload;
    CameraEntry* camera;
    EffBomheiN64Work* work;
    s32 alpha;
    s32 idx;
    s32 next;
    Mtx transMtx;
    Mtx scaleMtx;
    Mtx mtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    idx = (s32)work->progress;
    alpha = (s32)(256.0f * (work->progress - idx));
    GXSetNumChans(0);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG2);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG2);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C2, GX_CC_TEXC, GX_CC_A1, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_A2, GX_CA_TEXA, GX_CA_A1, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_C0, GX_CC_ONE, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    effGetTexObjN64(0x35, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 0.03125f, 0.0034722222f, 0.0f);
    PSMTXTrans(transMtx, 0.0f, 32.0f * idx, 0.0f);
    PSMTXConcat(scaleMtx, transMtx, transMtx);
    GXLoadTexMtxImm(transMtx, GX_TEXMTX0, GX_MTX2x4);
    next = (idx + 1 < 8) ? idx + 1 : 8;
    PSMTXTrans(transMtx, 0.0f, 32.0f * next, 0.0f);
    PSMTXConcat(scaleMtx, transMtx, transMtx);
    GXLoadTexMtxImm(transMtx, GX_TEXMTX1, GX_MTX2x4);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size32x32_tex32x32_vtx);
    PSMTXTrans(transMtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(mtx, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scaleMtx, work->scale, work->scale, 1.0f);
    PSMTXConcat(transMtx, mtx, transMtx);
    PSMTXConcat(transMtx, scaleMtx, transMtx);
    PSMTXRotRad(mtx, 'z', 0.017453292f * work->rotation);
    PSMTXConcat(transMtx, mtx, transMtx);
    PSMTXConcat(camera->view, transMtx, transMtx);
    GXLoadPosMtxImm(transMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    if (work->progress > 4.0f) {
        GXSetTevColor(GX_TEVREG0, (GXColor){255, 255, 255, 127});
    } else {
        GXSetTevColor(GX_TEVREG0, (GXColor){255, 255, 128, 127});
    }
    GXSetTevColor(GX_TEVREG1, (GXColor){255, 255, 139, alpha});
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
}
