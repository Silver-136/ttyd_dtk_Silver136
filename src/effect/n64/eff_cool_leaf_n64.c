#include "eff/eff_n64.h"
#include "eff/n64/eff_cool_leaf_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mario.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

typedef struct EffCoolLeafN64Work {
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
    f32 ax;        // 0x30
    f32 ay;        // 0x34
    f32 az;        // 0x38
} EffCoolLeafN64Work;

static effN64Vtx size12x12_tex64x64_brown_vtx[] = {
    {-6, -6, 0, 0, 0, 0x19, 0x2A, 0x11, 0xFF},
    {6, -6, 0, 2048, 0, 0x19, 0x2A, 0x11, 0xFF},
    {6, 6, 0, 2048, 2048, 0x68, 0x56, 0x10, 0xFF},
    {-6, 6, 0, 0, 2048, 0x68, 0x56, 0x10, 0xFF},
    {-6, -6, 0, 0, 2048, 0x18, 0x1B, 0x21, 0xFF},
    {6, -6, 0, 2048, 2048, 0x18, 0x1B, 0x21, 0xFF},
    {6, 6, 0, 2048, 4096, 0x30, 0x36, 0x25, 0xFF},
    {-6, 6, 0, 0, 4096, 0x30, 0x36, 0x25, 0xFF},
};

static effN64Vtx size12x12_tex64x64_red_vtx[] = {
    {-6, -6, 0, 0, 0, 0x32, 0x15, 0x11, 0xFF},
    {6, -6, 0, 2048, 0, 0x32, 0x15, 0x11, 0xFF},
    {6, 6, 0, 2048, 2048, 0x68, 0x2B, 0x10, 0xFF},
    {-6, 6, 0, 0, 2048, 0x68, 0x2B, 0x10, 0xFF},
    {-6, -6, 0, 0, 2048, 0x30, 0x0D, 0x21, 0xFF},
    {6, -6, 0, 2048, 2048, 0x30, 0x0D, 0x21, 0xFF},
    {6, 6, 0, 2048, 4096, 0x60, 0x1B, 0x25, 0xFF},
    {-6, 6, 0, 0, 4096, 0x60, 0x1B, 0x25, 0xFF},
};

void effCoolLeafMain(EffectEntry* effect);
void effCoolLeafDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effCoolLeafN64Entry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffCoolLeafN64Work* work;
    s32 i;
    s32 r;
    s32 px;
    s32 py;
    s32 a;

    effect = effEntry();
    effect->type = "CoolLeafN64";
    effect->count = 5;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffCoolLeafN64Work) * 5);
    effect->userdata = work;
    effect->callback = effCoolLeafMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->lifetime = 100;
    work->frame = 0;
    work->alpha = 0;
    work++;
    for (i = 1; i < 5; i++, work++) {
        px = rand() % 51;
        py = rand() % 21;
        a = rand() % 361;
        r = rand() % 101;
        work->position.x = px + 75;
        work->position.y = py + 70;
        work->position.z = 0.0f;
        work->angleZ = a;
        work->angleY = 360 - a;
        switch (type) {
            case 0:
                work->spin = 0.1f * r;
                work->vx = 0.0f;
                work->vy = 0.0f;
                work->ax = -3.0f;
                work->ay = 0.0f;
                work->az = 3.0f;
                break;
            case 1:
                work->spin = 2.0f * (0.1f * r);
                work->vx = 0.0f;
                work->vy = 0.0f;
                work->ax = -6.0f;
                work->ay = 0.0f;
                work->az = 6.0f;
                break;
            case 2:
                work->spin = 2.0f * (0.1f * r);
                work->vx = -work->position.x * (0.05f + 0.008f * (rand() % 11));
                work->vy = -work->position.y * (0.05f + 0.008f * (rand() % 11));
                break;
        }
    }
    return effect;
}

void effCoolLeafMain(EffectEntry* effect) {
    EffCoolLeafN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;
    s32 lifetime;
    f32 c;
    s32 frame;
    f32 s;
    f32 s2;
    f32 dx;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->lifetime--;
    work->frame++;
    lifetime = work->lifetime;
    if (lifetime < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    type = work->type;
    if (frame < 10) {
        work->alpha = work->alpha + 0.3f * (255 - work->alpha);
    }
    if (lifetime < 10) {
        work->alpha *= 0.8f;
    }
    work++;
    switch (type) {
        case 0:
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
                work->spin += (f32)((s32)effTblRandN64(50, frame + i * 20) - 25);
                work->angleZ += 10.0f * (f32)sin(6.2831855f * work->spin / 360.0f);
                work->angleY += 10.0f * (f32)cos(6.2831855f * (0.5f * work->spin) / 360.0f);
                work->position.x += work->vx;
                work->position.y += work->vy;
                if (work->position.y < 30.0f) {
                    if (work->position.x > -200.0f) {
                        work->ax -= 0.4f;
                    } else {
                        work->ax += 0.4f;
                    }
                    if (work->position.z > 0.0f) {
                        work->az -= 0.4f;
                    } else {
                        work->az += 0.4f;
                    }
                    work->ax *= 0.98f;
                    work->az *= 0.98f;
                    work->vy *= 0.98f;
                } else {
                    if (work->position.x > 0.0f) {
                        work->ax -= 0.4f;
                    } else {
                        work->ax += 0.4f;
                    }
                    if (work->position.z > 0.0f) {
                        work->az -= 0.4f;
                    } else {
                        work->az += 0.4f;
                    }
                    work->ax *= 0.98f;
                    work->az *= 0.98f;
                }
                work->position.x += work->ax;
                work->position.y += work->ay;
                work->position.z += work->az;
            }
            break;
        case 1:
            for (i = 1; i < effect->count; i++, work++) {
                s = (f32)sin(6.2831855f * (2.0f * work->angleZ) / 360.0f);
                dx = 0.2f * s * 2.0f;
                work->vy += -0.05f;
                work->vx += dx;
                work->vy += -0.05f;
                work->vx *= 0.8464f;
                work->spin += (f32)((s32)effTblRandN64(50, frame + i * 20) - 25);
                work->angleZ += 10.0f * (f32)sin(6.2831855f * work->spin / 360.0f);
                work->angleY += 10.0f * (f32)cos(6.2831855f * (0.5f * work->spin) / 360.0f);
                work->position.x += work->vx;
                work->position.y += work->vy;
                if (work->position.y < 30.0f) {
                    if (work->position.x > -200.0f) {
                        work->ax -= 0.8f;
                    } else {
                        work->ax += 0.8f;
                    }
                    if (work->position.z > 0.0f) {
                        work->az -= 0.8f;
                    } else {
                        work->az += 0.8f;
                    }
                    work->ax *= 0.96040004f;
                    work->az *= 0.96040004f;
                    work->vy *= 0.96040004f;
                } else {
                    if (work->position.x > 0.0f) {
                        work->ax -= 0.8f;
                    } else {
                        work->ax += 0.8f;
                    }
                    if (work->position.z > 0.0f) {
                        work->az -= 0.8f;
                    } else {
                        work->az += 0.8f;
                    }
                    work->ax *= 0.96040004f;
                    work->az *= 0.96040004f;
                }
                work->position.x += work->ax;
                work->position.y += work->ay;
                work->position.z += work->az;
            }
            break;
        case 2:
            for (i = 1; i < effect->count; i++, work++) {
                work->spin += (f32)((s32)effTblRandN64(50, frame + i * 20) - 25);
                s = (f32)sin(6.2831855f * work->spin / 360.0f);
                work->angleZ += 10.0f * s;
                c = (f32)cos(6.2831855f * (0.5f * work->spin) / 360.0f);
                work->angleY += 10.0f * c;
                work->position.x += work->vx;
                work->position.y += work->vy;
            }
            break;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effCoolLeafDisp, effect);
}

void effCoolLeafDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffCoolLeafN64Work* work;
    s32 type;
    s32 i;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx viewMtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    type = work->type;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, viewMtx);
    PSMTXConcat(camera->view, viewMtx, viewMtx);
    GXSetTevColor(GX_TEVREG0, (GXColor){20, 100, 20, work->alpha});
    effGetTexObjN64(0x14, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 4.0f, 1.0f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
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
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->angleZ);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXRotRad(rot, 'y', 0.017453292f * work->angleY);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(viewMtx, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        switch (type) {
            case 0:
                GXSetArray(GX_VA_POS, &size12x12_tex64x64_brown_vtx[0].x, sizeof(effN64Vtx));
                GXSetArray(GX_VA_CLR0, &size12x12_tex64x64_brown_vtx[0].r, sizeof(effN64Vtx));
                GXSetArray(GX_VA_TEX0, &size12x12_tex64x64_brown_vtx[0].s, sizeof(effN64Vtx));
                break;
            case 1:
                GXSetArray(GX_VA_POS, &size12x12_tex64x64_red_vtx[0].x, sizeof(effN64Vtx));
                GXSetArray(GX_VA_CLR0, &size12x12_tex64x64_red_vtx[0].r, sizeof(effN64Vtx));
                GXSetArray(GX_VA_TEX0, &size12x12_tex64x64_red_vtx[0].s, sizeof(effN64Vtx));
                break;
        }
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
        tri2(6, 5, 4, 4, 7, 6, 4, 4);
    }
}
