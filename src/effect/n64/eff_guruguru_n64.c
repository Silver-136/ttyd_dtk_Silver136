#include "eff/n64/eff_guruguru_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_n64.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

static effN64Vtx size16x16_tex32x32_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 64, 0, 0, 0, 0, 255},
    {8, 8, 0, 64, 32, 0, 0, 0, 255},
    {-8, 8, 0, 0, 32, 0, 0, 0, 255},
};


void effGuruguruMain(EffectEntry* effect);
void effGuruguruDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effGuruguruN64Entry(s32 type, s32 num, f32 x, f32 y, f32 z, f32 radius) {
    EffectEntry* effect;
    EffGuruguruN64Work* work;
    s32 i;

    effect = effEntry();
    effect->type = "GuruguruN64";
    effect->count = num + 1;
    work = __memAlloc(HEAP_EFFECT, (num + 1) * sizeof(EffGuruguruN64Work));
    effect->userdata = work;
    effect->callback = effGuruguruMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->radius = 0.0f;
    work->angle = 0;
    work->targetRadius = radius;
    work->active = 1;
    work->scale = 1.0f;
    work++;
    for (i = 1; i < num + 1; i++, work++) {
        work->position.x = 0.0f;
        work->position.y = 0.0f;
        work->position.z = 0.0f;
        work->rotZ = (f32)((i - 1) * 360 / num);
        work->rotZSpeed = 20.0f;
        work->rotX = 0.0f;
        work->angle = (i - 1) * 360 / num;
    }
    return effect;
}

void effGuruguruMain(EffectEntry* effect) {
    EffGuruguruN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    f32 radius;
    f32 c;
    f32 t;
    s32 i;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (work->active != 0) {
        work->angle++;
        work->radius += 0.1f * (work->targetRadius - work->radius);
        radius = work->radius;
        work++;
        for (i = 1; i < effect->count; i++, work++) {
            work->position.x = radius * (f32)sin(6.2831855f * work->angle / 360.0f);
            c = (f32)cos(6.2831855f * work->rotX / 360.0f);
            t = radius * (f32)cos(6.2831855f * work->angle / 360.0f);
            work->position.z = t * c;
            c = (f32)sin(6.2831855f * work->rotX / 360.0f);
            t = radius * (f32)cos(6.2831855f * work->angle / 360.0f);
            work->position.y = t * c;
            work->rotZ += work->rotZSpeed;
            work->rotX = 0.0f;
            work->angle += 16;
            if (work->angle > 360) {
                work->angle -= 360;
            }
        }
        dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effGuruguruDisp, effect);
    }
}

void effGuruguruDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera = camGetPtr(cameraId);
    EffGuruguruN64Work* work = effect->userdata;
    Mtx mtx;
    Mtx rot;
    Mtx scaleMtx;
    Mtx viewMtx;
    GXTexObj texObj;
    s32 i;

    if (work->active != 0) {
        GXColor color;
        GXSetNumChans(0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        effGetTexObjN64(0x1D, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        GXSetCullMode(GX_CULL_NONE);
        effSetVtxDescN64(size16x16_tex32x32_vtx);
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
        PSMTXScale(scaleMtx, work->scale, work->scale, work->scale);
        PSMTXConcat(mtx, rot, viewMtx);
        PSMTXConcat(viewMtx, scaleMtx, viewMtx);
        PSMTXConcat(camera->view, viewMtx, viewMtx);
        color = (GXColor){0xDC, 0xDC, 0x28, 0xFF};
        GXSetTevColor(GX_TEVREG0, color);
        work++;
        for (i = 1; i < effect->count; i++, work++) {
            PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
            PSMTXRotRad(rot, 'z', 0.017453292f * work->rotZ);
            PSMTXConcat(mtx, rot, mtx);
            PSMTXConcat(viewMtx, mtx, mtx);
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
        }
    }
}

