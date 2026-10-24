#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/n64/eff_rothammer_success_n64.h"

double sin(double);
double cos(double);

typedef struct EffRotHammerSuccessWork {
    s16 type;     // 0x0
    Vec position; // 0x4
    Vec vel;      // 0x10
    f32 rot;      // 0x1C
    f32 rotVel;   // 0x20
    s32 lifetime; // 0x24
    s32 frame;    // 0x28
    s32 maxLife;  // 0x2C
    f32 scale;    // 0x30
} EffRotHammerSuccessWork;

static effN64Vtx size16x16_tex32x32_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 64, 0, 0, 0, 0, 255},
    {8, 8, 0, 64, 32, 0, 0, 0, 255},
    {-8, 8, 0, 0, 32, 0, 0, 0, 255},
};

void effRotHammerSuccessMain(EffectEntry* effect);
void effRotHammerSuccessDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effRotHammerSuccessN64Entry(s32 type, f32 x, f32 y, f32 z, s32 count, s32 lifetime) {
    EffectEntry* effect;
    EffRotHammerSuccessWork* work;
    s32 i;
    f32 angle;

    effect = effEntry();
    effect->type = "RotHammerSuccessN64";
    effect->count = count + 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffRotHammerSuccessWork) * (count + 1));
    effect->userdata = work;
    effect->callback = effRotHammerSuccessMain;
    work->lifetime = lifetime;
    work->frame = 0;
    work->maxLife = lifetime;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work++;
    for (i = 1; i < count + 1; i++, work++) {
        work->position.x = 0.0f;
        work->position.y = 0.0f;
        work->position.z = 0.0f;
        angle = 6.2831855f * (f32)((i - 1) * 360 / count) / 360.0f;
        work->vel.x = 8.0f * (f32)sin(angle);
        work->vel.y = 8.0f * (f32)cos(angle);
        work->vel.z = 0.0f;
        work->rot = 0.0f;
        work->rotVel = 20.0f;
        work->scale = 1.0f;
    }
    return effect;
}

void effRotHammerSuccessMain(EffectEntry* effect) {
    EffRotHammerSuccessWork* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 lifetime;
    s32 i;
    s32 maxLife;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->frame++;
    work->lifetime--;
    lifetime = work->lifetime;
    if (lifetime < 0) {
        effDelete(effect);
        return;
    }
    maxLife = work->maxLife;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (lifetime > 5) {
            work->vel.x *= 0.8f;
            work->vel.y *= 0.8f;
            work->vel.z *= 0.8f;
        }
        if (lifetime < 5) {
            work->rotVel *= 0.8f;
            work->vel.x *= 0.7f;
            work->vel.y *= 0.7f;
            work->vel.z *= 0.7f;
        }
        if (lifetime * 2 < maxLife) {
            work->scale += 0.2f * -work->scale;
        }
        work->position.x += work->vel.x;
        work->position.y += work->vel.y;
        work->position.z = 0.0f;
        work->rot += work->rotVel;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effRotHammerSuccessDisp, effect);
}

void effRotHammerSuccessDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffRotHammerSuccessWork* work;
    s32 i;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx billboard;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
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
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(camera->view, mtx, billboard);
    GXSetTevColor(GX_TEVREG0, (GXColor){0xDC, 0xDC, 0x28, 0xE6});
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rot);
        PSMTXScale(scale, work->scale, work->scale, work->scale);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(billboard, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
