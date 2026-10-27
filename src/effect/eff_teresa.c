#include "eff/eff_teresa.h"
#include "eff/eff_kemu_test.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "pmario_sound.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "sfx_name_word.h"

typedef struct EffTeresaWork {
    s32 type;                   // 0x0
    Vec position;               // 0x4
    u8 field_0x10[0x28 - 0x10]; // 0x10
    f32 alpha;                  // 0x28
    s32 field_0x2C;             // 0x2C
    s32 field_0x30;             // 0x30
    f32 scale;                  // 0x34
    f32 rotZ;                   // 0x38
    BOOL vanished;              // 0x3C
} EffTeresaWork;

EffectEntry* effTeresaEntry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffTeresaWork* work;

    effect = effEntry();
    effect->type = "Teresa";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffTeresaWork));
    effect->userdata = work;
    effect->callback = effTeresaMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = type == 0 ? 30.0f : 0.0f;
    work->field_0x30 = 0;
    work->field_0x2C = 0;
    work->alpha = type == 0 ? 0 : 255;
    work->vanished = FALSE;
    if (type == 0) {
        Vec position = { 0.0f, 0.0f, 0.0f };

        position.x = work->position.x;
        position.y = work->position.y;
        position.z = work->position.z;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG4_TELESA_LIGHT1), &position);
    }
    return effect;
}

void effTeresaMain(EffectEntry* effect) {
    EffTeresaWork* work = effect->userdata;
    Vec position;
    Vec tmp = { 0.0f, 0.0f, 0.0f };

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;

    if (work->type == 0) {
        if (!work->vanished) {
            // shrink, then puff into smoke
            if (work->scale > 0.1f) {
                work->scale *= 0.8f;
            } else {
                work->vanished = TRUE;
                effKemuTestEntry(8, work->position.x, work->position.y, work->position.z, 1.0f);
                {
                    Vec sfxPosition = { 0.0f, 0.0f, 0.0f };

                    sfxPosition.x = work->position.x;
                    sfxPosition.y = work->position.y;
                    sfxPosition.z = work->position.z;
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG4_TELESA_A_APPEAR1), &sfxPosition);
                }
            }
            work->rotZ += 10.0f;
            work->alpha = 178.5f;
        } else {
            work->rotZ = 0.0f;
            work->scale += (20.0f - work->scale) / 30.0f;
            work->alpha *= 0.8f;
            if (work->alpha < 3.0f) {
                effDelete(effect);
                return;
            }
        }
    } else {
        work->scale += (20.0f - work->scale) / 30.0f;
        work->alpha *= 0.98f;
        if (work->alpha < 3.0f) {
            effDelete(effect);
            return;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effTeresaDisp, effect);
}

void effTeresaDisp(CameraId cameraId, EffectEntry* effect) {
    EffTeresaWork* work = effect->userdata;
    CameraEntry* camera;
    s32 type;
    GXTexObj texObj;
    Mtx mtx;
    Mtx rot;
    Mtx scale;
    f32 size, half;

    camera = camGetPtr(cameraId);
    type = work->type;
    effGetTexObj(0x45, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    if (type == 0) {
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha });
    } else {
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0x00, 0x00, 0x00, work->alpha });
    }
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);

    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mtx, rot, mtx);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXRotRad(rot, 'z', MTXDegToRad(work->rotZ));
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

    size = 32.0f;
    half = 16.0f;
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
