#include "eff/eff_energy.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "version.h"
#include "driver/effdrv.h"


typedef struct EffEnergyWork {
    s32 type;        // 0x0, index into color_tbl
    Vec position;    // 0x4
    f32 size;        // 0x10
    f32 scale;       // 0x14
    f32 scaleSpeed;  // 0x18
    f32 rotZ;        // 0x1C
    f32 rotSpeed;    // 0x20
    s32 alpha;       // 0x24
    s32 timer;       // 0x28, counts down while fading out
    s32 brightness;  // 0x2C
    s32 brightSpeed; // 0x30
} EffEnergyWork;

// pairs of (TEVREG0, TEVREG1) colors per type
static GXColor color_tbl[] = {
    { 0x38, 0xF2, 0x2C, 0xFF }, { 0xEA, 0xFA, 0xB7, 0xFF },
    { 0x59, 0xB4, 0xFF, 0xFF }, { 0xA2, 0xF8, 0xFD, 0xFF },
    { 0xFF, 0xA3, 0x26, 0xFF }, { 0xFD, 0xF1, 0x89, 0xFF },
    { 0xFD, 0xFB, 0x7E, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xF5, 0x5A, 0xC5, 0xFF }, { 0xFD, 0xBC, 0xFB, 0xFF },
};

EffectEntry* effEnergyEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 size, f32 rotSpeed,
                            f32 scaleSpeed) {
    EffectEntry* effect;
    EffEnergyWork* work;

    effect = effEntry();
    effect->type = "Energy";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffEnergyWork) * effect->count);
    effect->userdata = work;
    effect->callback = effEnergyMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->rotZ = 0.0f;
    work->rotSpeed = rotSpeed;
    work->size = size;
    work->scale = 0.0f;
    work->scaleSpeed = scaleSpeed;
    work->alpha = 255;
    work->brightness = 0;
    work->brightSpeed = 32;
    if (duration <= 0) {
        work->timer = 1000;
    } else {
        work->timer = duration;
    }
    return effect;
}

void effEnergyMain(EffectEntry* effect) {
    EffEnergyWork* work = effect->userdata;
    Vec position;

    position = (Vec){ work->position.x, work->position.y, work->position.z };

    // soft delete: start fading out
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->timer = 16;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    if (work->timer < 16) {
        work->alpha = work->timer * 16;
    }
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    work->rotZ += work->rotSpeed;
    work->scale += work->scaleSpeed;
    // pulse brightness up and down
    work->brightness += work->brightSpeed;
    if (work->brightness > 255) {
        work->brightness = 255;
        work->brightSpeed = -32;
    }
    if (work->brightness < 0) {
        work->brightness = 0;
        work->brightSpeed = 32;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effEnergyDisp, effect);
}

void effEnergyDisp(CameraId cameraId, EffectEntry* effect) {
    EffEnergyWork* work = effect->userdata;
    CameraEntry* camera;
    s32 type;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx final;
    f32 size, half;

    camera = camGetPtr(cameraId);
    type = work->type;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scale, work->scale * work->size, work->scale * work->size, work->scale * work->size);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mtx, rot, mtx);
    PSMTXRotRad(rot, 'z', MTXDegToRad(work->rotZ));
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, final);
    PSMTXConcat(camera->view, final, final);
    GXLoadPosMtxImm(final, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0,
                      (GXColor){ work->brightness, work->brightness, work->brightness, work->alpha });
    GXSetTevColor(GX_TEVREG0, color_tbl[type * 2]);
    GXSetTevColor(GX_TEVREG1, color_tbl[type * 2 + 1]);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    effGetTexObj(EFF_TEX(0x8D), &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    size = 8.0f;
    half = 4.0f;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-size / 2, half, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(half, half, 0.0f);
    GXTexCoord2f32(2.0f, 0.0f);
    GXPosition3f32(half, -size / 2, 0.0f);
    GXTexCoord2f32(2.0f, 1.0f);
    GXPosition3f32(-size / 2, -size / 2, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXEnd();
}
