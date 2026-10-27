#include "eff/eff_mahorn.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "version.h"


typedef struct EffMahornWork {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 scale;    // 0x10
    f32 scale2;   // 0x14
    s32 alpha;    // 0x18
    s32 timer;    // 0x1C, element 0 only, counts down while fading out
    u32 frame;    // 0x20
    s32 delay;    // 0x24
} EffMahornWork;

// per-frame scale curves (percent), alternating between rings
static u8 scale_dt[] = {
    0x00, 0x07, 0x0E, 0x16, 0x1E, 0x26, 0x2F, 0x37, 0x3F, 0x47, 0x4E,
    0x54, 0x5A, 0x5E, 0x62, 0x64, 0x64, 0x55, 0x33, 0x10, 0x00,
};
static u8 scale_dt2[] = {
    0x00, 0x13, 0x29, 0x3F, 0x52, 0x5F, 0x64, 0x63, 0x5F, 0x59, 0x51,
    0x47, 0x3D, 0x33, 0x28, 0x1E, 0x14, 0x0C, 0x06, 0x02, 0x00
};

EffectEntry* effMahornEntry(s32 type, s32 count, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffMahornWork* work;
    EffMahornWork* ring;
    s32 i, num;

    effect = effEntry();
    effect->type = "Mahorn";
    num = count + 1;
    effect->count = num;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffMahornWork) * num);
    effect->userdata = work;
    effect->callback = effMahornMain;
    work->type = type;
    work->frame = 0;
    if (duration <= 0) {
        work->timer = 1000;
    } else {
        work->timer = duration;
    }
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->alpha = 255;

    ring = work + 1;
    for (i = 1; i < effect->count; i++, ring++) {
        ring->position.x = 0.0f;
        ring->position.y = 0.0f;
        ring->position.z = 0.0f;
        ring->scale = 0.0f;
        ring->scale2 = 0.0f;
        ring->alpha = 255;
        ring->delay = i * 4;
    }
    return effect;
}

void effMahornMain(EffectEntry* effect) {
    EffMahornWork* work = effect->userdata;
    Vec position;
    s32 i;
    f32 scale;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    if (work->timer < 1000) {
        work->timer--;
    }
    work->frame++;
    if (work->timer < 16) {
        work->alpha = work->timer * 16;
    }
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay) {
            work->delay--;
            continue;
        }
        work->frame++;
        if (i & 1) {
            scale = scale_dt[work->frame % 21] / 100.0f;
        } else {
            scale = scale_dt2[work->frame % 21] / 100.0f;
        }
        work->scale = scale;
        work->scale2 += (20.0f - work->scale2) / 8.0f;
        work->alpha += -work->alpha / 64;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effMahornDisp, effect);
}

void effMahornDisp(CameraId cameraId, EffectEntry* effect) {
    BOOL odd;
    EffMahornWork* work = effect->userdata;
    s32 alpha, i;
    GXTexObj texObj;
    Mtx mtx;
    Mtx rot;
    Mtx scale;
    Mtx base;
    f32 size, half;

    alpha = work->alpha;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mtx, rot, base);
    PSMTXConcat(base, scale, base);

    effGetTexObj(EFF_TEX(0x82), &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    size = 8.0f;
    half = 4.0f;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay) {
            continue;
        }
        odd = i & 1;
        if (odd) {
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFD, 0xE8, 0x3C, work->alpha * alpha / 255 });
        } else {
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFC, 0xE7, work->alpha * alpha / 255 });
        }
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        if (odd) {
            PSMTXScale(scale, work->scale * work->scale2, work->scale * work->scale2,
                       work->scale * work->scale2);
        } else {
            PSMTXScale(scale, work->scale * work->scale2, -work->scale * work->scale2,
                       work->scale * work->scale2);
        }
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(base, mtx, mtx);
        PSMTXConcat(camGetPtr(cameraId)->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-size / 2, half, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(half, half, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(half, -size / 2, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-size / 2, -size / 2, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
}
