#include "eff/eff_batfukidashi.h"
#include "driver/dispdrv.h"
#include "manager/fontmgr.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdio.h>

typedef struct EffCoinFukidashiWork {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 offsetY;  // 0x10, bounce
    s32 number;   // 0x14
    f32 scale;    // 0x18
    s32 alpha;    // 0x1C
    s32 timer;    // 0x20, counts down while fading out
    u32 frame;    // 0x24
} EffCoinFukidashiWork;

// bounce offsets (hundredths of a unit), one per frame
static u8 offset_tbl[] = {
    0x28, 0x23, 0x1E, 0x19, 0x14, 0x0F, 0x0A, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04,
    0x06, 0x08, 0x0A, 0x0C, 0x0F, 0x11, 0x14, 0x16, 0x19, 0x1C, 0x1E, 0x21, 0x23, 0x25, 0x28, 0x28,
    0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,
    0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28
};

EffectEntry* effCoinFukidashiEntry(s32 type, s32 number, s32 duration, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffCoinFukidashiWork* work;

    effect = effEntry();
    effect->type = "CoinFukidashi";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffCoinFukidashiWork) * effect->count);
    effect->userdata = work;
    effect->callback = effCoinFukidashiMain;
    effect->flags |= 2;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->offsetY = 0.0f;
    work->scale = 1.0f;
    work->number = number;
    if (duration <= 0) {
        work->timer = 1000;
    } else {
        work->timer = duration;
    }
    work->frame = 0;
    work->alpha = 0;
    return effect;
}

void effCoinFukidashiMain(EffectEntry* effect) {
    EffCoinFukidashiWork* work = effect->userdata;
    Vec position;

    position = (Vec){ work->position.x, work->position.y, work->position.z };

    // soft delete: start fading out
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->timer = 16;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    if (work->timer < 16) {
        work->alpha = work->timer * 16;
    } else {
        work->alpha += 16;
        if (work->alpha > 255) {
            work->alpha = 255;
        }
    }
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    work->offsetY = offset_tbl[work->frame] / 100.0f;
    work->frame++;
    if (work->frame >= 61) {
        work->frame = 0;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effCoinFukidashiDisp, effect);
}

void effCoinFukidashiDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffCoinFukidashiWork* work;
    s32 alpha;
    f32 width, height, textWidth;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    char text[64];
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->alpha;

    // bubble
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    effGetTexObj(0x25, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, alpha });
    PSMTXTrans(mtx, work->position.x, work->position.y + work->offsetY, work->position.z);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    width = 0.4f * GXGetTexObjWidth(&texObj);
    height = 0.4f * GXGetTexObjHeight(&texObj);
    GXPosition3f32(-width / 2, height, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(width / 2, height, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(width / 2, 0.0f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-width / 2, 0.0f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXEnd();

    // number text, centered in the bubble
    sprintf(text, "%d", work->number);
    textWidth = work->scale * FontGetMessageWidth(text) / 3.0f;
    PSMTXTrans(mtx, -textWidth / 2, -28.0f, 0.0f);
    PSMTXScale(scale, work->scale / 3.0f, work->scale / 3.0f, work->scale / 3.0f);
    PSMTXConcat(mtx, scale, scale);
    PSMTXTrans(mtx, textWidth / 2, 28.0f, 0.0f);
    PSMTXConcat(scale, mtx, scale);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mtx, rot, rot);
    PSMTXTrans(mtx, 3.0f, 34.0f + work->offsetY, 0.0f);
    PSMTXConcat(rot, mtx, mtx);
    PSMTXConcat(mtx, scale, mtx);
    FontDrawStart_alpha(alpha);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    FontDrawStringMtx(mtx, text);
}
