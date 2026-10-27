#include "eff/eff_machinegun.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "version.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"


typedef struct EffMachinegunWork {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 scale;    // 0x10
    f32 angle;    // 0x14
    f32 speed;    // 0x18
    s32 alpha;    // 0x1C
} EffMachinegunWork;

EffectEntry* effMachinegunEntry(s32 type, f32 x, f32 y, f32 z, f32 angle, f32 speed, f32 scale) {
    EffectEntry* effect;
    EffMachinegunWork* work;

    effect = effEntry();
    effect->type = "Machinegun";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, 0x28);
    effect->userdata = work;
    effect->callback = effMachinegunMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->angle = angle;
    work->scale = scale;
    work->alpha = 255;
    work->speed = speed;
    return effect;
}

void effMachinegunMain(EffectEntry* effect) {
    EffMachinegunWork* work = effect->userdata;
    Vec position;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    work->position.x += work->speed * (f32)cos(6.2831855f * work->angle / 360.0f);
    work->position.y += work->speed * (f32)sin(6.2831855f * work->angle / 360.0f);
    work->speed *= 0.9f;
    work->alpha -= 10;
    if (work->alpha < 0) {
        effDelete(effect);
        return;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effMachinegunDisp, effect);
}

void effMachinegunDisp(CameraId cameraId, EffectEntry* effect) {
    EffMachinegunWork* work = effect->userdata;
    CameraEntry* camera;
    GXTexObj texObj;
    Mtx mtx;
    Mtx rot;
    Mtx scale;
    f32 size, half;

    camera = camGetPtr(cameraId);
    effGetTexObj(EFF_TEX(0x89), &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0x7C, 0x74, 0x6C, work->alpha });
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

    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mtx, rot, mtx);
    PSMTXRotRad(rot, 'z', MTXDegToRad(180.0f + work->angle));
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

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
