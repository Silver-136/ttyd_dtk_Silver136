#include "version.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/n64/eff_sunlight_n64.h"
#include "eff/eff_magic1_n64.h"


typedef struct EffSunlightN64Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    s32 lifetime;  // 0x10
    s32 frame;     // 0x14
    u8 color[7];   // 0x18
    f32 offset[5]; // 0x20
    f32 unk34;     // 0x34
    s32 alpha;     // 0x38
    s32 alphaMax;  // 0x3C
} EffSunlightN64Work;

void effSunlightMain(EffectEntry* effect);
void effSunlightDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effSunlightN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 w) {
    EffectEntry* effect;
    EffSunlightN64Work* work;

    effect = effEntry();
    effect->type = "SunlightN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffSunlightN64Work));
    effect->userdata = work;
    effect->callback = effSunlightMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->unk34 = w;
    work->color[0] = 255;
    work->color[1] = 255;
    work->color[2] = 255;
    work->color[3] = 255;
    work->color[4] = 255;
    work->color[5] = 255;
    work->color[6] = 255;
    work->offset[0] = 0.0f;
    work->offset[1] = 0.0f;
    work->offset[2] = 0.0f;
    work->offset[3] = 0.0f;
    work->offset[4] = 0.0f;
    work->alphaMax = 255;
    work->alpha = 0;
    return effect;
}

void effSunlightMain(EffectEntry* effect) {
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    EffSunlightN64Work* work;
    s32 i;
    f32 base;
    s32 frame;

    work = effect->userdata;
    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->lifetime = 16;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    if (work->frame > 0x50000 - 0xE60) {
        work->frame = 256;
    }
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    if (work->lifetime < 16) {
        work->alpha -= 16;
        if (work->alpha < 0) {
            work->alpha = 0;
        }
    }
    if (work->alphaMax < 0) {
        work->alphaMax = 0;
    } else if (work->alphaMax > 255) {
        work->alphaMax = 255;
    }
    if (work->alpha > work->alphaMax) {
        work->alpha -= 8;
        if (work->alpha < work->alphaMax) {
            work->alpha = work->alphaMax;
        }
    } else if (work->alpha < work->alphaMax) {
        work->alpha += 8;
        if (work->alpha > work->alphaMax) {
            work->alpha = work->alphaMax;
        }
    }
    base = 0.25f * frame;
    for (i = 0; i < 5; i++) {
        f32 a = (f32)sin(6.2831855f * (base + i * i * 20) / 360.0f);
        f32 b = (f32)sin(6.2831855f * (frame * 2 + i * 20) / 360.0f);
        work->offset[i] -= 4.0f * (0.01f * b + 0.05f) * a;
        if (work->offset[i] < 0.0f) {
            work->offset[i] += 256.0f;
        }
        if (work->offset[i] > 256.0f) {
            work->offset[i] -= 256.0f;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), VERSION_PICK(2, 10), (DispCallback)effSunlightDisp, effect);
}

void effSunlightDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera = camGetPtr(cameraId);
    EffSunlightN64Work* work = effect->userdata;
    s32 alpha = work->alpha;
    s32 type = work->type;
    Vec target = {0.0f, 0.0f, 0.0f};
    Vec pos = {0.0f, 0.0f, 100.0f};
    Vec up = {0.0f, 1.0f, 0.0f};
    Mtx trans;
    Mtx scale;
    Mtx44 ortho;
    Mtx view;
    GXTexObj texObj;
    f32 proj[7];
    s32 i;

    if (alpha == 0) {
        return;
    }
    GXGetProjectionv(proj);
    if (type != 0) {
        MTXOrtho(ortho, 1200.0f, -1200.0f, -1600.0f, 1600.0f, 1.0f, 32768.0f);
    } else {
        MTXOrtho(ortho, 1200.0f, -1200.0f, 1600.0f, -1600.0f, 1.0f, 32768.0f);
    }
    GXSetProjection(ortho, GX_ORTHOGRAPHIC);
    MTXLookAt(view, &pos, &up, &target);
    GXLoadPosMtxImm(view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->color[0], work->color[1], work->color[2], alpha >> 1});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->color[3], work->color[4], work->color[5], work->color[6]});
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_APREV);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.0625f, 0.015625f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x42, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObjN64(0x43, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(testsrc__s_v);
    for (i = 0; i < 5; i++) {
        PSMTXScale(scale, 0.00390625f, 0.125f, 0.0f);
        PSMTXTrans(trans, work->offset[i] + (f32)(i * 11), 0.0f, 0.0f);
        PSMTXConcat(scale, trans, scale);
        GXLoadTexMtxImm(scale, GX_TEXMTX1, GX_MTX2x4);
        switch (i) {
        case 0:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            tri2(4, 5, 6, 0, 4, 6, 7, 0);
            break;
        case 1:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(8, 9, 10, 8, 8, 10, 11, 8);
            break;
        case 2:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(12, 13, 14, 12, 12, 14, 15, 12);
            break;
        case 3:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
            tri2(16, 17, 18, 16, 16, 18, 19, 16);
            tri2(20, 21, 22, 16, 20, 22, 23, 16);
            break;
        case 4:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(24, 25, 26, 24, 24, 26, 27, 24);
            break;
        }
    }
    GXSetProjectionv(proj);
}

effN64Vtx testsrc__s_v[] = {
    {531, 442, 25, 0, 0, 255, 255, 255, 255},
    {707, 284, 25, 512, 0, 255, 255, 255, 255},
    {1610, 1287, 25, 512, 2048, 255, 255, 255, 255},
    {1434, 1445, 25, 0, 2048, 255, 255, 255, 255},
    {1351, -188, 25, 0, 0, 255, 255, 255, 255},
    {1508, -248, 25, 512, 0, 255, 255, 255, 255},
    {1750, 382, 25, 512, 2048, 255, 255, 255, 255},
    {1593, 442, 25, 0, 2048, 255, 255, 255, 255},
    {-676, 699, 2, 0, 0, 255, 255, 255, 126},
    {-490, 377, 2, 512, 0, 255, 255, 255, 126},
    {1264, 1390, 2, 512, 2048, 255, 255, 255, 126},
    {1078, 1711, 2, 0, 2048, 255, 255, 255, 126},
    {-991, 848, 27, 0, 0, 255, 255, 255, 229},
    {-949, 755, 27, 512, 0, 255, 255, 255, 229},
    {583, 1437, 27, 512, 2048, 255, 255, 255, 229},
    {542, 1530, 27, 0, 2048, 255, 255, 255, 229},
    {571, -206, 25, 0, 0, 255, 255, 255, 255},
    {711, -301, 25, 512, 0, 255, 255, 255, 255},
    {1768, 1266, 25, 512, 745, 255, 255, 255, 255},
    {1628, 1361, 25, 0, 745, 255, 255, 255, 255},
    {-508, 202, 25, 0, 0, 255, 255, 255, 255},
    {-407, 68, 25, 512, 0, 255, 255, 255, 255},
    {1462, 1476, 25, 512, 2048, 255, 255, 255, 255},
    {1361, 1611, 25, 0, 2048, 255, 255, 255, 255},
    {483, -973, 2, -559, 68, 255, 255, 255, 126},
    {811, -1148, 2, -47, 68, 255, 255, 255, 126},
    {2078, 1236, 2, -47, 2116, 255, 255, 255, 126},
    {1750, 1411, 2, -559, 2116, 255, 255, 255, 126},};
