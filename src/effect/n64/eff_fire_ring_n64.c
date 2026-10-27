#include "eff/eff_n64.h"
#include "eff/n64/eff_fire_ring_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "eff/n64/eff_fire_dust_n64.h"

typedef struct EffFireRingN64Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    f32 unk10[3];  // 0x10
    f32 scale;     // 0x1c
    f32 scaleVel;  // 0x20
    s32 alpha;     // 0x24
    s32 lifetime;  // 0x28
    s32 frame;     // 0x2c
    f32 uv[4];     // 0x30
    f32 uvVel[4];  // 0x40
    u8 r, g, b;    // 0x50
} EffFireRingN64Work;

void effFireRingMain(EffectEntry* effect);
void effFireRingDisp(CameraId cameraId, EffectEntry* effect);

static effN64Vtx stg__s_v[] = {
    {99, 0, 205, 9173, 4096, 255, 255, 255, 255},
    {54, 0, 113, 9641, 2048, 255, 255, 255, 255},
    {0, 0, 125, 10780, 2048, 255, 255, 255, 255},
    {0, 0, 228, 11247, 4096, 255, 255, 255, 255},
    {178, 0, 142, 7566, 4096, 255, 255, 255, 255},
    {98, 0, 78, 8034, 2048, 255, 255, 255, 255},
    {54, 0, 113, 9173, 2048, 255, 255, 255, 255},
    {99, 0, 205, 9640, 4096, 255, 255, 255, 255},
    {222, 0, 51, 5960, 4096, 255, 255, 255, 255},
    {122, 0, 28, 6427, 2048, 255, 255, 255, 255},
    {98, 0, 78, 7566, 2048, 255, 255, 255, 255},
    {178, 0, 142, 8034, 4096, 255, 255, 255, 255},
    {222, 0, -51, 4353, 4096, 255, 255, 255, 255},
    {122, 0, -28, 4820, 2048, 255, 255, 255, 255},
    {122, 0, 28, 5960, 2048, 255, 255, 255, 255},
    {222, 0, 51, 6427, 4096, 255, 255, 255, 255},
    {178, 0, -142, 2746, 4096, 255, 255, 255, 255},
    {98, 0, -78, 3213, 2048, 255, 255, 255, 255},
    {122, 0, -28, 4353, 2048, 255, 255, 255, 255},
    {222, 0, -51, 4820, 4096, 255, 255, 255, 255},
    {99, 0, -205, 1139, 4096, 255, 255, 255, 255},
    {54, 0, -113, 1607, 2048, 255, 255, 255, 255},
    {98, 0, -78, 2746, 2048, 255, 255, 255, 255},
    {178, 0, -142, 3213, 4096, 255, 255, 255, 255},
    {27, 0, -119, 570, 2048, 255, 255, 255, 255},
    {54, 0, -113, 1139, 2048, 255, 255, 255, 255},
    {99, 0, -205, 1607, 4096, 255, 255, 255, 255},
    {49, 0, -216, 570, 4096, 255, 255, 255, 255},
    {0, 0, 228, 10780, 4096, 255, 255, 255, 255},
    {0, 0, 125, 11247, 2048, 255, 255, 255, 255},
    {-54, 0, 113, 12387, 2048, 255, 255, 255, 255},
    {-99, 0, 205, 12854, 4096, 255, 255, 255, 255},
    {0, 0, -228, 22027, 4096, 255, 255, 255, 255},
    {-99, 0, -205, 20420, 4096, 255, 255, 255, 255},
    {-178, 0, -142, 18814, 4096, 255, 255, 255, 255},
    {-222, 0, -51, 17207, 4096, 255, 255, 255, 255},
    {-222, 0, 51, 15600, 4096, 255, 255, 255, 255},
    {-178, 0, 142, 13993, 4096, 255, 255, 255, 255},
    {-99, 0, 205, 12387, 4096, 255, 255, 255, 255},
    {0, 0, -125, 22494, 2048, 255, 255, 255, 255},
    {27, 0, -119, 23064, 2048, 255, 255, 255, 255},
    {49, 0, -216, 23064, 4096, 255, 255, 255, 255},
    {-54, 0, -113, 20888, 2048, 255, 255, 255, 255},
    {0, 0, -125, 22027, 2048, 255, 255, 255, 255},
    {0, 0, -228, 22494, 4096, 255, 255, 255, 255},
    {-98, 0, -78, 19281, 2048, 255, 255, 255, 255},
    {-54, 0, -113, 20420, 2048, 255, 255, 255, 255},
    {-99, 0, -205, 20888, 4096, 255, 255, 255, 255},
    {-122, 0, -28, 17674, 2048, 255, 255, 255, 255},
    {-98, 0, -78, 18814, 2048, 255, 255, 255, 255},
    {-178, 0, -142, 19281, 4096, 255, 255, 255, 255},
    {-122, 0, 28, 16068, 2048, 255, 255, 255, 255},
    {-122, 0, -28, 17207, 2048, 255, 255, 255, 255},
    {-222, 0, -51, 17674, 4096, 255, 255, 255, 255},
    {-98, 0, 78, 14461, 2048, 255, 255, 255, 255},
    {-122, 0, 28, 15600, 2048, 255, 255, 255, 255},
    {-222, 0, 51, 16067, 4096, 255, 255, 255, 255},
    {-54, 0, 113, 12854, 2048, 255, 255, 255, 255},
    {-98, 0, 78, 13993, 2048, 255, 255, 255, 255},
    {-178, 0, 142, 14461, 4096, 255, 255, 255, 255},
};

EffectEntry* effFireRingN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffFireRingN64Work* work;

    effect = effEntry();
    effect->type = "FireRingN64";
    effect->count = 32;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffFireRingN64Work) * 32);
    effect->userdata = work;
    effect->callback = effFireRingMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->alpha = 255;
    work->lifetime = lifetime;
    work->frame = 0;
    work->uv[0] = 0.0f;
    work->uv[1] = 0.0f;
    work->uv[2] = 0.0f;
    work->uv[3] = 0.0f;
    work->uvVel[0] = -1.1f;
    work->uvVel[1] = 0.0f;
    work->uvVel[2] = 0.4f;
    work->uvVel[3] = 1.0f;
    work->scale = 0.0f;
    work->scaleVel = 0.1f * scale;
    work->r = 255;
    work->g = 255;
    work->b = 64;
    return effect;
}

void effFireRingMain(EffectEntry* effect) {
    EffFireRingN64Work* work;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 lifetime;
    s32 frame;
    f32 angle;
    s32 r;

    work = effect->userdata;
    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->lifetime--;
    work->frame++;
    lifetime = work->lifetime;
    if (lifetime < 0) {
        effDelete(effect);
    } else {
        frame = work->frame;
        if (frame & 1) {
            r = effTblRandN64(360, frame);
            angle = (6.2831855f * r) / 360.0f;
            sin(angle);
            cos(angle);
            effFireDustN64Entry(0, 10, 20, work->position.x, work->position.y, work->position.z, frame * 8, 10.0f);
        }
        work->scale += work->scaleVel;
        if (lifetime < 10) {
            work->alpha = lifetime * 25;
        }
        work->uv[0] += work->uvVel[0];
        work->uv[1] += work->uvVel[1];
        work->uv[2] += work->uvVel[2];
        work->uv[3] += work->uvVel[3];
        if (work->uv[0] > 128.0f) {
            work->uv[0] -= 128.0f;
        }
        if (work->uv[1] > 128.0f) {
            work->uv[1] -= 128.0f;
        }
        if (work->uv[2] > 128.0f) {
            work->uv[2] -= 128.0f;
        }
        if (work->uv[3] > 128.0f) {
            work->uv[3] -= 128.0f;
        }
        if (work->uv[0] < 0.0f) {
            work->uv[0] += 128.0f;
        }
        if (work->uv[1] < 0.0f) {
            work->uv[1] += 128.0f;
        }
        if (work->uv[2] < 0.0f) {
            work->uv[2] += 128.0f;
        }
        if (work->uv[3] < 0.0f) {
            work->uv[3] += 128.0f;
        }
        dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effFireRingDisp, effect);
    }
}

void effFireRingDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffFireRingN64Work* work;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    PSMTXScale(scale, 1.0f / 63.0f, 1.0f / 63.0f, 0.0f);
    PSMTXTrans(mtx, work->uv[0], work->uv[1], 0.0f);
    PSMTXConcat(scale, mtx, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    PSMTXScale(scale, 1.0f / 31.0f, 1.0f / 31.0f, 0.0f);
    PSMTXTrans(mtx, work->uv[2], work->uv[3], 0.0f);
    PSMTXConcat(scale, mtx, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    effGetTexObjN64(0x2d, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObjN64(0x2e, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xB3, 0x1E, work->alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->r, work->g, work->b, 0x80});
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A1, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_C1);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_TEXC, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(stg__s_v);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 90);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
    tri2(4, 5, 6, 0, 4, 6, 7, 0);
    tri2(8, 9, 10, 0, 8, 10, 11, 0);
    tri2(12, 13, 14, 0, 12, 14, 15, 0);
    tri2(16, 17, 18, 0, 16, 18, 19, 0);
    tri2(20, 21, 22, 0, 20, 22, 23, 0);
    tri2(24, 25, 26, 0, 24, 26, 27, 0);
    tri2(28, 29, 30, 0, 28, 30, 31, 0);
    tri2(32, 39, 40, 0, 32, 40, 41, 0);
    tri2(33, 42, 43, 0, 33, 43, 44, 0);
    tri2(34, 45, 46, 0, 34, 46, 47, 0);
    tri2(35, 48, 49, 0, 35, 49, 50, 0);
    tri2(36, 51, 52, 0, 36, 52, 53, 0);
    tri2(37, 54, 55, 0, 37, 55, 56, 0);
    tri2(38, 57, 58, 0, 38, 58, 59, 0);
}
