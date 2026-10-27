#include "eff/eff_toge_flush.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"

typedef struct EffTogeFlushWork {
    s32 type;       // 0x0
    Vec position;   // 0x4
    f32 rotZ;       // 0x10
    f32 rotZSpeed;  // 0x14
    f32 scale;      // 0x18
    f32 scaleSpeed; // 0x1C
    f32 scale2;     // 0x20
    s32 alpha;      // 0x24
    s32 alpha2;     // 0x28
    s32 life;       // 0x2C
    s32 age;        // 0x30
} EffTogeFlushWork;

static u8 color_rotation_data[24] = {
    0xFF, 0xFF, 0x52, 0xFF, 0xFF, 0x52, 0xB5, 0x94, 0x00, 0xB5, 0x94, 0x00,
    0xFF, 0xFF, 0xD6, 0xFF, 0xFF, 0xD6, 0xB5, 0x94, 0x00, 0xB5, 0x94, 0x00,
};

EffectEntry* effTogeFlushEntry(s32 type, f32 x, f32 y, f32 z, s32 life) {
    EffectEntry* effect;
    EffTogeFlushWork* work;

    effect = effEntry();
    effect->type = "TogeFlush";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffTogeFlushWork));
    effect->userdata = work;
    effect->callback = effTogeFlushMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->life = life;
    work->age = 0;
    work->alpha = 0;
    work->rotZ = 0.0f;
    if (type == 0 || type == 3) {
        work->rotZSpeed = 30.0f;
    } else {
        work->rotZSpeed = 0.0f;
    }
    work->scale = 0.0f;
    work->scaleSpeed = 0.8f;
    work->alpha2 = 0;
    work->scale2 = 0.0f;
    return effect;
}

void effTogeFlushMain(EffectEntry* effect) {
    s32 life;
    EffTogeFlushWork* work = effect->userdata;
    s32 age;
    f32 s;

    work->life--;
    work->age++;
    life = work->life;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    age = work->age;
    if (age <= 6) {
        work->alpha = 0.5f * (218 - work->alpha) + work->alpha;
    }
    if (age > 6) {
        work->alpha *= 0.78f;
    }
    work->scale += work->scaleSpeed;
    work->scaleSpeed += 0.29f;
    work->rotZ += work->rotZSpeed;
    work->rotZSpeed *= 0.93f;
    s = sin((6.2831855f * (life * 50)) / 360.0f);
    work->scale2 = 0.5f * s + 0.9f;
    work->alpha2 = 64.0f * (f32)sin((6.2831855f * (life * 40)) / 360.0f) + 144.0f;
    {
        Vec position = { 0.0f, 0.0f, 0.0f };

        position.x = work->position.x;
        position.y = work->position.y;
        position.z = work->position.z;
        dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effTogeFlushDisp, effect);
    }
}

void effTogeFlushDisp(CameraId cameraId, EffectEntry* effect) {
    EffTogeFlushWork* work = effect->userdata;
    s32 type = work->type;
    GXTexObj texObj;
    Mtx trans;
    Mtx rot;
    Mtx scale;
    Mtx out;
    Mtx tmp;
    s32 idx;
    s32 alpha;

    if (work->life > 10) {
        effGetTexObj(25, &texObj);
    } else if (work->life > 5) {
        effGetTexObj(26, &texObj);
    } else {
        effGetTexObj(27, &texObj);
    }
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXConcat(trans, rot, tmp);
    alpha = work->alpha2;
    idx = work->age * 3;
    PSMTXScale(scale, work->scale2, work->scale2, work->scale2);
    PSMTXConcat(tmp, scale, out);
    PSMTXConcat(camGetPtr(cameraId)->view, out, out);
    GXLoadPosMtxImm(out, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    switch (type) {
    case 0: {
        idx %= 24;
        GXSetTevColor(GX_TEVREG0, (GXColor){ color_rotation_data[idx], color_rotation_data[idx + 1],
                                             color_rotation_data[idx + 2], alpha });
        break;
    }
    case 1: {
        GXSetTevColor(GX_TEVREG0, (GXColor){ 0xFF, 0x00, 0x00, alpha });
        break;
    }
    case 2: {
        GXSetTevColor(GX_TEVREG0, (GXColor){ 0x00, 0x00, 0xFF, alpha });
        break;
    }
    case 3: {
        GXSetTevColor(GX_TEVREG0, (GXColor){ 0xFF, 0x00, 0x00, alpha });
        break;
    }
    case 4: {
        GXSetTevColor(GX_TEVREG0, (GXColor){ 0xFF, 0xFF, 0xFF, alpha });
        break;
    }
    }
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-8.0f, 8.0f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(8.0f, 8.0f, 0.0f);
    GXTexCoord2f32(2.0f, 0.0f);
    GXPosition3f32(8.0f, -8.0f, 0.0f);
    GXTexCoord2f32(2.0f, 2.0f);
    GXPosition3f32(-8.0f, -8.0f, 0.0f);
    GXTexCoord2f32(0.0f, 2.0f);
    alpha = work->alpha;
    if (alpha > 255) {
        alpha = 255;
    }
    if (alpha > 0) {
        PSMTXScale(scale, work->scale, work->scale, work->scale);
        PSMTXConcat(tmp, scale, out);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rotZ);
        PSMTXConcat(out, rot, out);
        PSMTXConcat(camGetPtr(cameraId)->view, out, out);
        GXLoadPosMtxImm(out, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        switch (type) {
        case 3: {
            GXSetTevColor(GX_TEVREG0, (GXColor){ 0xF1, 0x4B, 0x41, alpha });
            break;
        }
        case 4: {
            GXSetTevColor(GX_TEVREG0, (GXColor){ 0xC9, 0xC0, 0xCF, alpha });
            break;
        }
        default: {
            GXSetTevColor(GX_TEVREG0, (GXColor){ 0xFF, 0xFF, 0xFF, alpha });
            break;
        }
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-4.0f, 4.0f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(4.0f, 4.0f, 0.0f);
        GXTexCoord2f32(2.0f, 0.0f);
        GXPosition3f32(4.0f, -4.0f, 0.0f);
        GXTexCoord2f32(2.0f, 2.0f);
        GXPosition3f32(-4.0f, -4.0f, 0.0f);
        GXTexCoord2f32(0.0f, 2.0f);
    }
}
