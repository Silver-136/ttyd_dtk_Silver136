/* Mahon = Wizzerd */

#include "eff/eff_mahorn2.h"
#include "eff/eff_hit.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <string.h>
#include "version.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"


void effMahorn2Main(EffectEntry *effect);
void effMahorn2Disp(CameraId cameraId, EffectEntry *effect);

#define TWO_PI 6.283184f

EffectEntry *effMahorn2Entry(BOOL unused, f32 posX, f32 posY, f32 posZ, f32 dstX, f32 dstY, f32 dstZ, f32 scale, f32 radius) {
    EffectEntry *effect;
    effMahorn2Data *data;
    f32 temp;
    int i;

    effect = effEntry();
    effect->type = "Mahorn2";
    effect->count = 14;
    data = __memAlloc(HEAP_EFFECT, sizeof(effMahorn2Data) * 14);
    effect->userdata = data;
    effect->callback = effMahorn2Main;
    data->unused = unused;
    data->unused2 = 0;
    data->intplStep = 0;
    data->position.x = posX;
    data->position.y = posY;
    data->position.z = posZ;
    data->target.x = dstX;
    data->target.y = dstY;
    data->target.z = dstZ;
    data->scale = scale;
    data->hitDistance = radius;
    data->alpha = 255;
    data->type = 0;
    data->hasHit = FALSE;
    data++;

    for (i = 1; i < 4; i++, data++) {
        memset(data, 0, sizeof(effMahorn2Data));
        data->type = 1;
        data->position.x = 0.0f;
        data->position.y = 0.0f;
        data->position.z = 0.0f;
        data->scale = 6.0f;
        data->alpha = 0;
        data->counter = 15 * (i - 1);
    }

    for (i = 4; i < 5; i++, data++) {
        memset(data, 0, sizeof(effMahorn2Data));
        data->type = 2;
        data->position.x = 0.0f;
        data->position.y = 0.0f;
        data->position.z = 0.0f;
        data->scale = 0.0f;
        data->alpha = 153;
        data->counter = 15;
    }

    for (i = 5; i < 6; i++, data++) {
        memset(data, 0, sizeof(effMahorn2Data));
        data->type = 3;
        data->position.x = 0.0f;
        data->position.y = 0.0f;
        data->position.z = 0.0f;
        data->scale = 0.0f;
        data->alpha = 153;
        data->counter = 60;
    }

    for (i = 6; i < effect->count; i++, data++) {
        memset(data, 0, sizeof(effMahorn2Data));
        data->type = 4;
        data->position.x = dstX - posX;
        data->position.y = dstY - posY;
        data->position.z = dstZ - posZ;
        temp = TWO_PI * (f32)(i - 6);
        data->target.x = 6.0f * (f32)sin(temp / (f32)(effect->count - 6));
        data->target.y = 6.0f * (f32)cos(temp / (f32)(effect->count - 6));
        data->target.z = 0.0f;
        data->scale = 0.0f;
        data->alpha = 255;
        data->counter = 0;
    }

    return effect;
}

void effMahorn2Main(EffectEntry *effect) {
    effMahorn2Data *data = effect->userdata;
    Vec position;
    Vec target;
    s32 i;
    f32 hitDistance;
    f32 scale;
    f32 rad;

    position = (Vec){ data->position.x, data->position.y, data->position.z };
    target = (Vec){ data->target.x, data->target.y, data->target.z };
    hitDistance = data->hitDistance;
    scale = data->scale;

    for (i = 0; i < effect->count; i++, data++) {
        if (data->counter != 0) {
            data->counter--;
            continue;
        }
        switch (data->type) {
            case 0:
                break;
            case 1:
                data->alpha += 4;
                if (data->alpha > 255) {
                    data->alpha = 255;
                }
                data->scale += -data->scale / 16.0f;
                break;
            case 2:
                if (data->intplStep < 90) {
                    data->scale = intplGetValue(EASING_SINE_IN_OUT, 0.0f, 3.0f, ++data->intplStep, 45);
                }
                break;
            case 3:
                if (PSVECDistance(&target, &(Vec){ position.x + data->position.x, position.y + data->position.y, position.z + data->position.z }) > hitDistance) {
                    rad = (6.2831855f * angleABf(position.x + data->position.x, position.y + data->position.y,
                                             target.x, target.y)) / 360.0f;
                    data->position.x += hitDistance * (f32)sin(rad);
                    data->position.y += hitDistance * (f32)cos(rad);
                    data->scale += (4.0f - data->scale) / 8.0f;
                }
                else {
                    data->scale += -data->scale / 8.0f;
                    if (!((effMahorn2Data*)effect->userdata)->hasHit) {
                        ((effMahorn2Data*)effect->userdata)->hasHit = TRUE;
                        effHitEntry(0, 4, target.x, target.y, target.z, scale);
                    }
                }
                break;
            case 4:
                if (((effMahorn2Data*)effect->userdata)->hasHit) {
                    data->position.x += data->target.x;
                    data->position.y += data->target.y;
                    data->position.z += data->target.z;
                    data->target.x *= 0.88f;
                    data->target.y *= 0.88f;
                    data->target.z *= 0.88f;
                    data->scale = 1.0f;
                    data->alpha -= 4;
                    if (data->alpha < 0) {
                        data->alpha = 0;
                        effDelete(effect);
                        return;
                    }
                }
                break;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effMahorn2Disp, effect);
}

void effMahorn2Disp(CameraId cameraId, EffectEntry *effect) {
    effMahorn2Data *data = effect->userdata;
    Mtx mtx;
    Mtx rot;
    Mtx scaleMtx;
    Mtx base;
    GXTexObj texObj;
    CameraEntry *cam;
    s32 alpha;
    f32 scale;
    f32 w, h, u, v;
    s32 i, type;

    cam = camGetPtr(cameraId);
    alpha = data->alpha;
    scale = data->scale;
    PSMTXTrans(base, data->position.x, data->position.y, data->position.z);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    data++;
    for (i = 1; i < effect->count; i++, data++) {
        if (data->counter != 0) {
            continue;
        }
        switch (data->type) {
            case 0:
                break;
            case 1:
                effGetTexObj(EFF_TEX(0x8F), &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                break;
            case 2:
            case 3:
            case 4:
                effGetTexObj(EFF_TEX(0x90), &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                break;
        }
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0x00, 0xDA, 0x00, data->alpha * alpha / 255 });
        PSMTXTrans(mtx, data->position.x, data->position.y, data->position.z);
        PSMTXScale(scaleMtx, data->scale * scale, data->scale * scale, data->scale * scale);
        PSMTXConcat(mtx, scaleMtx, mtx);
        PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(base, mtx, mtx);
        PSMTXConcat(cam->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);

        switch (type = data->type) {
            case 0:
                break;
            case 1:
                h = w = 16.0f;
                v = u = 2.0f;
                break;
            case 2:
                h = w = 16.0f;
                v = u = 2.0f;
                break;
            case 3:
                v = u = h = 2.0f;
                w = 16.0f;
                break;
            case 4:
                h = w = 8.0f;
                v = u = 2.0f;
                break;
        }

        if (type != 3) {
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(-w / 2.0f, h / 2.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(w / 2.0f, h / 2.0f, 0.0f);
            GXTexCoord2f32(u, 0.0f);
            GXPosition3f32(w / 2.0f, -h / 2.0f, 0.0f);
            GXTexCoord2f32(u, v);
            GXPosition3f32(-w / 2.0f, -h / 2.0f, 0.0f);
            GXTexCoord2f32(0.0f, v);
        }
        else {
            s32 j;
            for (j = 0; j < 3; j++) {
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                {
                f32 xs[3][2] = { { 0.0f, 2.0f }, { 2.0f, 12.0f }, { 12.0f, 14.0f } };
                f32 us[3][2] = { { 0.0f, 0.5f }, { 0.5f, 0.5f }, { 0.5f, 1.0f } };
                GXPosition3f32(xs[j][0], h / 2.0f, 0.0f);
                GXTexCoord2f32(2.0f * us[j][0], 0.0f);
                GXPosition3f32(xs[j][1], h / 2.0f, 0.0f);
                GXTexCoord2f32(2.0f * us[j][1], 0.0f);
                GXPosition3f32(xs[j][1], -h / 2.0f, 0.0f);
                GXTexCoord2f32(2.0f * us[j][1], v);
                GXPosition3f32(xs[j][0], -h / 2.0f, 0.0f);
                GXTexCoord2f32(2.0f * us[j][0], v);
                }
            }
        }
    }
}
