#include "eff/eff_spirit.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "version.h"
#include "eff/eff_magic1_n64.h"
#include "driver/effdrv.h"
// US effect texture ids are 0x1C lower for these entries.
#define EFF_TEX(id) ((id) - VERSION_PICK(0, 0x1C))


typedef struct EffSpiritWork {
    s32 type;       // 0x0
    s32 state;      // 0x4
    s32 timer;      // 0x8
    s32 frame;      // 0xC
    Vec position;   // 0x10
    Vec startPos;   // 0x1C
    Vec endPos;     // 0x28
    Vec velocity;   // 0x34
    s32 step;       // 0x40
    s32 steps;      // 0x44
    s32 r;          // 0x48
    s32 g;          // 0x4C
    s32 b;          // 0x50
    s32 a;          // 0x54
    s32 colorState; // 0x58
    s32 colorStep;  // 0x5C
    f32 size;       // 0x60
    f32 scale;      // 0x64
} EffSpiritWork;

EffectEntry* effSpiritEntry(s32 type, s32 time, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 scale) {
    EffectEntry* effect;
    EffSpiritWork* work;

    effect = effEntry();
    effect->type = "Spirit";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffSpiritWork));
    effect->userdata = work;
    effect->callback = effSpiritMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x1;
    work->position.y = y1;
    work->position.z = z1;
    work->size = scale;
    work->scale = 1.0f;
    work->frame = 0;
    work->timer = 1000;
    work->state = 0;
    work->velocity.x = 0.0f;
    work->velocity.y = 2.0f;
    work->velocity.z = 0.0f;
    work->startPos.x = work->position.x;
    work->startPos.y = work->position.y;
    work->startPos.z = work->position.z;
    work->endPos.x = x2;
    work->endPos.y = y2;
    work->endPos.z = z2;
    work->r = 255;
    work->g = 242;
    work->b = 0;
    work->a = 204;
    work->colorState = 0;
    work->colorStep = 0;
    work->step = 0;
    work->steps = time;
    return effect;
}

void effSpiritMain(EffectEntry* effect) {
    EffSpiritWork* work = effect->userdata;
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
    work->frame++;
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    if (work->timer < 16) {
        work->a = work->timer * 16;
    }
    switch (work->state) {
        case 0:
            work->position.x += (f32)sin((f32)work->frame / 2.0f);
            work->position.y += work->velocity.y;
            work->velocity.y *= 0.95f;
            if (work->velocity.y < 0.1f) {
                work->startPos.x = work->position.x;
                work->startPos.y = work->position.y;
                work->startPos.z = work->position.z;
                work->state++;
            }
            break;
        case 1:
            if (work->step < work->steps) {
                work->step++;
                work->position.x = intplGetValue(EASING_COS_IN_OUT, work->startPos.x, work->endPos.x, work->step, work->steps);
                work->position.y = intplGetValue(EASING_COS_IN_OUT, work->startPos.y, work->endPos.y, work->step, work->steps);
                work->position.z = intplGetValue(EASING_COS_IN_OUT, work->startPos.z, work->endPos.z, work->step, work->steps);
                {
                    f32 ang = 3.1415927f * ((f32)work->step / (f32)work->steps);
                    ang = sin(ang);
                    work->position.y += 50.0f * ang;
                }
                work->scale = intplGetValue(EASING_COS_IN_OUT, 1.0f, 0.0f, work->step, work->steps);
            } else {
                effDelete(effect);
                return;
            }
            break;
    }
    switch (work->colorState) {
        case 0:
            work->r = intplGetValue(EASING_COS_IN_OUT, 255.0f, 37.0f, work->colorStep, 32);
            work->g = intplGetValue(EASING_COS_IN_OUT, 242.0f, 254.0f, work->colorStep, 32);
            work->b = intplGetValue(EASING_COS_IN_OUT, 0.0f, 246.0f, work->colorStep, 32);
            work->colorStep++;
            if (work->colorStep >= 32) {
                work->colorState = 1;
                work->colorStep = 0;
            }
            break;
        case 1:
            work->r = intplGetValue(EASING_COS_IN_OUT, 37.0f, 226.0f, work->colorStep, 32);
            work->g = intplGetValue(EASING_COS_IN_OUT, 254.0f, 65.0f, work->colorStep, 32);
            work->b = intplGetValue(EASING_COS_IN_OUT, 246.0f, 255.0f, work->colorStep, 32);
            work->colorStep++;
            if (work->colorStep >= 32) {
                work->colorState = 2;
                work->colorStep = 0;
            }
            break;
        case 2:
            work->r = intplGetValue(EASING_COS_IN_OUT, 226.0f, 255.0f, work->colorStep, 32);
            work->g = intplGetValue(EASING_COS_IN_OUT, 65.0f, 242.0f, work->colorStep, 32);
            work->b = intplGetValue(EASING_COS_IN_OUT, 255.0f, 0.0f, work->colorStep, 32);
            work->colorStep++;
            if (work->colorStep >= 32) {
                work->colorState = 0;
                work->colorStep = 0;
            }
            break;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effSpiritDisp, effect);
}

void effSpiritDisp(CameraId cameraId, EffectEntry* effect) {
    EffSpiritWork* work = effect->userdata;
    CameraEntry* camera;
    GXTexObj texObj;
    Mtx mtx;
    Mtx rot;
    Mtx scale;
    f32 size, half;

    camera = camGetPtr(cameraId);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    size = work->size * work->scale;
    PSMTXScale(scale, size, size, size);
    PSMTXConcat(mtx, rot, rot);
    PSMTXConcat(rot, scale, scale);
    PSMTXConcat(camera->view, scale, scale);
    GXLoadPosMtxImm(scale, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    effGetTexObj(EFF_TEX(0x83), &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){ work->r, work->g, work->b, 0.8f * work->a });
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
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
    GXTexCoord2f32(2.0f, 2.0f);
    GXPosition3f32(-size / 2, -size / 2, 0.0f);
    GXTexCoord2f32(0.0f, 2.0f);
    GXEnd();
}
