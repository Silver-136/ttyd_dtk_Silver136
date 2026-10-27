#include "eff/n64/eff_bush_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "mario/mario.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

typedef struct EffBushN64Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    f32 vx;        // 0x10
    f32 vy;        // 0x14
    f32 angleZ;    // 0x18
    f32 spin;      // 0x1C
    f32 angleY;    // 0x20
    s32 alpha;     // 0x24
    s32 lifetime;  // 0x28
    s32 frame;     // 0x2C
    u8 pad[4];
} EffBushN64Work;

static effN64Vtx size12x12_tex64x64_vtx[] = {
    {-6, -6, 0, 0, 0, 0x19, 0x55, 0x11, 0xFF},
    {6, -6, 0, 2048, 0, 0x19, 0x55, 0x11, 0xFF},
    {6, 6, 0, 2048, 2048, 0x68, 0xAC, 0x10, 0xFF},
    {-6, 6, 0, 0, 2048, 0x68, 0xAC, 0x10, 0xFF},
    {-6, -6, 0, 0, 2048, 0x18, 0x37, 0x21, 0xFF},
    {6, -6, 0, 2048, 2048, 0x18, 0x37, 0x21, 0xFF},
    {6, 6, 0, 2048, 4096, 0x30, 0x6C, 0x25, 0xFF},
    {-6, 6, 0, 0, 4096, 0x30, 0x6C, 0x25, 0xFF},
};

void effBushMain(EffectEntry* effect);
void effBushDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effBushN64Entry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffBushN64Work* work;
    s32 i;

    effect = effEntry();
    effect->type = "BushN64";
    effect->count = 5;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffBushN64Work) * 5);
    effect->userdata = work;
    effect->callback = effBushMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->lifetime = 100;
    work->frame = 0;
    work->alpha = 0;
    work++;
    for (i = 1; i < 5; i++, work++) {
        work->position.x = rand() % 30 - 15;
        work->position.y = 0.0f;
        work->position.z = (rand() & 0x1E) - 15;
        // BUG: `& 360` masks with 0x168 instead of taking `% 360`, so only 16 start angles (0, 8, 32, 40, ... 360) occur.
        work->angleZ = rand() & 360;
        work->angleY = rand() & 360;
        work->spin = rand() % 100 / 10;
        work->vx = 0.0f;
        work->vy = 0.0f;
    }
    return effect;
}

void effBushMain(EffectEntry* effect) {
    EffBushN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;
    s32 lifetime;
    f32 s;
    f32 s2;
    f32 dx;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    type = work->type;
    if (work->frame >= 5 && work->frame < 15) {
        switch (type) {
            case 1:
                break;
            case 0:
                if (marioGetPtr()->currMotionId == MARIO_MOTION_STAY) {
                    return;
                }
                break;
        }
    }
    work->lifetime--;
    work->frame++;
    if (work->frame > 324000) {
        work->frame = 256;
    }
    lifetime = work->lifetime;
    if (lifetime < 0) {
        effDelete(effect);
        return;
    }
    if (work->frame < 10) {
        work->alpha = work->alpha + 0.3f * (255 - work->alpha);
    }
    if (lifetime < 10) {
        work->alpha *= 0.8f;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        s = (f32)sin(6.2831855f * (2.0f * work->angleZ) / 360.0f);
        dx = 0.2f * s;
        work->vy += -0.05f;
        work->vx += dx;
        if (type == 0) {
            work->vx *= 0.94f;
            s2 = (f32)sin(6.2831855f * work->angleZ / 360.0f);
            work->vy *= 0.05f * s2 + 0.95f;
        } else {
            work->vy += -0.05f;
            work->vx *= 0.92f;
        }
        work->spin += (rand() % 50) - 25.0f;
        work->angleZ += 10.0f * (f32)sin(6.2831855f * work->spin / 360.0f);
        work->angleY += 10.0f * (f32)cos(6.2831855f * (work->spin / 2.0f) / 360.0f);
        work->position.x += work->vx;
        work->position.y += work->vy;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effBushDisp, effect);
}

void effBushDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffBushN64Work* work;
    s32 i;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx viewMtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, viewMtx);
    PSMTXConcat(camera->view, viewMtx, viewMtx);
    GXSetTevColor(GX_TEVREG0, (GXColor){0x14, 0x64, 0x14, work->alpha});
    work++;
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 4.0f, 1.0f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x2A, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 12);
    GXSetArray(GX_VA_POS, &size12x12_tex64x64_vtx[0].x, sizeof(effN64Vtx));
    GXSetArray(GX_VA_CLR0, &size12x12_tex64x64_vtx[0].r, sizeof(effN64Vtx));
    GXSetArray(GX_VA_TEX0, &size12x12_tex64x64_vtx[0].s, sizeof(effN64Vtx));
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->angleZ);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXRotRad(rot, 'y', 0.017453292f * work->angleY);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(viewMtx, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
        tri2(6, 5, 4, 4, 7, 6, 4, 4);
    }
}
