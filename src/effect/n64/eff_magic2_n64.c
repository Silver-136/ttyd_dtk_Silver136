#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "eff/n64/eff_magic2_n64.h"
#include "libc_decls.h"

typedef struct EffMagic2N64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    Vec offset;   // 0x10
    f32 scale;    // 0x1C
    f32 scaleTarget; // 0x20
    s32 alpha;    // 0x24
    s32 life;     // 0x28
    s32 age;      // 0x2C
    s32 tex0ScrollX; // 0x30, X translation (/4) of texture matrix GX_TEXMTX0 in effMagic2Disp; += tex0ScrollXStep each frame
    s32 tex0ScrollXStep; // 0x34, per-frame step of tex0ScrollX (always 0)
    s32 tex1ScrollX; // 0x38, X translation (/4) of texture matrix GX_TEXMTX1 in effMagic2Disp; += tex1ScrollXStep each frame
    s32 tex1ScrollXStep; // 0x3C, per-frame step of tex1ScrollX (0x60 for type 1, else 0)
    s32 tex0ScrollY; // 0x40, Y translation (/4) of texture matrix GX_TEXMTX0; += tex0ScrollYStep, wrapped by +0x100 when negative
    s32 tex0ScrollYStep; // 0x44, per-frame step of tex0ScrollY (-2 for type 0, else 0)
    s32 tex1ScrollY; // 0x48, Y translation (/4) of texture matrix GX_TEXMTX1; += tex1ScrollYStep, wrapped by +0x100 when negative
    s32 tex1ScrollYStep; // 0x4C, per-frame step of tex1ScrollY (-4 for type 0, else 0)
    f32 f50;
    f32 f54;
    f32 f58;
    f32 f5c;
} EffMagic2N64Work;

static u8 color_rotation_data[24] = {0xFF, 0xC5, 0x9C, 0xFF, 0x9C, 0xFF, 0xFF, 0x73, 0xD6, 0xFF, 0x9C, 0xFF,
                                     0xFF, 0xC5, 0x7B, 0xFF, 0xB4, 0xFF, 0xDE, 0x73, 0xFF, 0xFF, 0x94, 0x9C};

void effMagic2Main(EffectEntry* effect);
void effMagic2Disp(CameraId cameraId, EffectEntry* effect);
void magic2_2_line_dl(void);
void magic2_4_dl(void);

effN64Vtx stg_magic2_2_line_v[] = {
    {-87, 50, 0, 5461, 2048, 255, 255, 255, 0},
    {-87, 50, -2000, 5461, 0, 255, 255, 255, 255},
    {-50, 87, -2000, 4779, 0, 255, 255, 255, 255},
    {-50, 87, 0, 4779, 2048, 255, 255, 255, 0},
    {-100, 0, 0, 6144, 2048, 255, 255, 255, 0},
    {-100, 0, -2000, 6144, 0, 255, 255, 255, 255},
    {0, 100, -2000, 4096, 0, 255, 255, 255, 255},
    {0, 100, 0, 4096, 2048, 255, 255, 255, 0},
    {-87, -50, 0, 6827, 2048, 255, 255, 255, 0},
    {-87, -50, -2000, 6827, 0, 255, 255, 255, 255},
    {-50, -87, 0, 7509, 2048, 255, 255, 255, 0},
    {-50, -87, -2000, 7509, 0, 255, 255, 255, 255},
    {0, -100, 0, 8192, 2048, 255, 255, 255, 0},
    {0, -100, -2000, 8192, 0, 255, 255, 255, 255},
    {50, 87, -2000, 3413, 0, 255, 255, 255, 255},
    {50, 87, 0, 3413, 2048, 255, 255, 255, 0},
    {87, -50, 0, 1365, 2048, 255, 255, 255, 0},
    {87, -50, -2000, 1365, 0, 255, 255, 255, 255},
    {50, -87, -2000, 683, 0, 255, 255, 255, 255},
    {50, -87, 0, 683, 2048, 255, 255, 255, 0},
    {0, -100, -2000, 0, 0, 255, 255, 255, 255},
    {0, -100, 0, 0, 2048, 255, 255, 255, 0},
    {100, 0, 0, 2048, 2048, 255, 255, 255, 0},
    {100, 0, -2000, 2048, 0, 255, 255, 255, 255},
    {87, 50, 0, 2731, 2048, 255, 255, 255, 0},
    {87, 50, -2000, 2731, 0, 255, 255, 255, 255},
};

effN64Vtx stg_magic2_1_v[] = {
    {-87, 50, 0, 5461, 512, 255, 255, 255, 255},
    {-87, 50, -50, 5461, 0, 255, 255, 255, 255},
    {-50, 87, -50, 4779, 0, 255, 255, 255, 255},
    {-50, 87, 0, 4779, 512, 255, 255, 255, 255},
    {-100, 0, 0, 6144, 512, 255, 255, 255, 255},
    {-100, 0, -50, 6144, 0, 255, 255, 255, 255},
    {0, 100, -50, 4096, 0, 255, 255, 255, 255},
    {0, 100, 0, 4096, 512, 255, 255, 255, 255},
    {-87, -50, 0, 6827, 512, 255, 255, 255, 255},
    {-87, -50, -50, 6827, 0, 255, 255, 255, 255},
    {-50, -87, 0, 7509, 512, 255, 255, 255, 255},
    {-50, -87, -50, 7509, 0, 255, 255, 255, 255},
    {0, -100, 0, 8192, 512, 255, 255, 255, 255},
    {0, -100, -50, 8192, 0, 255, 255, 255, 255},
    {50, 87, -50, 3413, 0, 255, 255, 255, 255},
    {50, 87, 0, 3413, 512, 255, 255, 255, 255},
    {87, -50, 0, 1365, 512, 255, 255, 255, 255},
    {87, -50, -50, 1365, 0, 255, 255, 255, 255},
    {50, -87, -50, 683, 0, 255, 255, 255, 255},
    {50, -87, 0, 683, 512, 255, 255, 255, 255},
    {0, -100, -50, 0, 0, 255, 255, 255, 255},
    {0, -100, 0, 0, 512, 255, 255, 255, 255},
    {100, 0, 0, 2048, 512, 255, 255, 255, 255},
    {100, 0, -50, 2048, 0, 255, 255, 255, 255},
    {87, 50, 0, 2731, 512, 255, 255, 255, 255},
    {87, 50, -50, 2731, 0, 255, 255, 255, 255},
};

EffectEntry* effMagic2N64Entry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffMagic2N64Work* w;

    effect = effEntry();
    effect->type = "Magic2N64";
    effect->count = 1;
    w = __memAlloc(HEAP_EFFECT, sizeof(EffMagic2N64Work));
    effect->userdata = w;
    effect->callback = effMagic2Main;
    w->type = type;
    w->life = life;
    w->alpha = w->age = 0;
    w->offset.x = x;
    w->offset.y = y;
    w->offset.z = z;
    w->tex0ScrollY = w->tex0ScrollYStep = 0;
    w->tex1ScrollY = w->tex1ScrollYStep = 0;
    w->tex0ScrollX = w->tex0ScrollXStep = 0;
    w->tex1ScrollX = w->tex1ScrollXStep = 0;
    w->position.x = x;
    w->position.y = y;
    w->position.z = z;
    w->scale = 10.0f;
    w->f58 = (f32)(rand() % 360);
    w->f5c = 4.0f;
    w->f50 = 0.0f;
    w->f54 = 0.0f;
    if (type == 1) {
        w->tex1ScrollXStep = 0x60;
        w->scaleTarget = scale;
    } else {
        w->tex0ScrollYStep = -2;
        w->tex1ScrollYStep = -4;
        w->scaleTarget = 0.2f * scale;
    }
    return effect;
}

void effMagic2Main(EffectEntry* effect) {
    EffMagic2N64Work* w = effect->userdata;
    Vec position;
    Vec pos2;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 life;

    tmp.x = w->offset.x;
    tmp.y = w->offset.y;
    tmp.z = w->offset.z;
    position = tmp;
    w->life--;
    w->age++;
    life = w->life;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    if (w->age <= 10) {
        w->alpha = w->age * 255 / 10;
    }
    if (life < 6) {
        w->alpha = life * 255 / 6;
    }
    if (life < 10 && w->type == 0) {
        w->scale *= 0.7f;
    }
    if (w->type == 1) {
        f32 sn = (f32)sin(6.2831855f * (f32)(life * 10) / 360.0f);
        f32 st = w->scaleTarget;
        w->scale = 0.4f * ((st * (0.1f * sn) + st) - w->scale) + w->scale;
    } else {
        f32 sn = (f32)sin(6.2831855f * (f32)(life * 10) / 360.0f);
        f32 st = w->scaleTarget;
        w->scale = 0.3f * ((st * (0.1f * sn) + st) - w->scale) + w->scale;
    }
    {
        Vec tmp2 = {0.0f, 0.0f, 0.0f};

        tmp2.x = w->position.x;
        tmp2.y = w->position.y;
        tmp2.z = w->position.z;
        pos2 = tmp2;
    }
    PSMTXMultVec(camGetPtr(CAMERA_3D)->view, &pos2, &pos2);
    PSMTX44MultVec(camGetPtr(CAMERA_3D)->projection, &pos2, &pos2);
    w->offset.x = 304.0f * pos2.x;
    w->offset.y = 240.0f * pos2.y;
    w->offset.z = 0.0f;
    for (i = 0; i < effect->count; i++, w++) {
        w->f58 += w->f5c;
        w->f50 += w->f54;
        w->tex0ScrollX += w->tex0ScrollXStep;
        w->tex1ScrollX += w->tex1ScrollXStep;
        w->tex0ScrollY += w->tex0ScrollYStep;
        w->tex1ScrollY += w->tex1ScrollYStep;
        if (w->tex0ScrollY < 0) {
            w->tex0ScrollY += 0x100;
        }
        if (w->tex1ScrollY < 0) {
            w->tex1ScrollY += 0x100;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effMagic2Disp, effect);
}

void effMagic2Disp(CameraId cameraId, EffectEntry* effect) {
    s32 fov;
    s32 half;
    EffMagic2N64Work* w;
    s32 i;
    s32 idx;
    s32 alpha;
    s32 type;
    Mtx mtx;
    Mtx texScale;
    Mtx44 persp;
    Mtx44 model;
    Mtx44 modelScale;
    f32 proj[7];
    f32 vp[6];

    camGetPtr(cameraId);
    w = effect->userdata;
    type = w->type;
    GXGetProjectionv(proj);
    GXGetViewportv(vp);
    PSMTX44Trans(model, w->offset.x, w->offset.y, w->offset.z);
    PSMTX44Scale(modelScale, w->scale, w->scale, 1.0f);
    PSMTX44Concat(model, modelScale, model);
    fov = (type == 1) ? 130 : 30;
    C_MTXPerspective(persp, fov, 1.2666667f, 1.0f, 32768.0f);
    PSMTX44Concat(persp, model, persp);
    GXSetProjection(persp, GX_PERSPECTIVE);
    switch (type) {
        case 1:
            PSMTXTrans(mtx, 0.0f, 0.0f, -70.0f);
            break;
        case 0:
            PSMTXTrans(mtx, 0.0f, 0.0f, -80.0f);
            break;
    }
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    alpha = w->alpha;
    idx = w->age * 3;
    half = alpha / 2;
    for (i = 0; i < effect->count; i++, w++) {
        u8* d;
        s32 tex0X;
        s32 tex0Y;
        s32 tex1X;
        s32 tex1Y;

        idx += 30;
        idx = idx % 24;
        d = color_rotation_data + idx;
        GXSetTevColor(GX_TEVREG0, (GXColor){d[0], 0, d[2], alpha});
        GXSetTevColor(GX_TEVREG1, (GXColor){0, 0, 0, half});
        tex0X = w->tex0ScrollX;
        tex0Y = w->tex0ScrollY;
        tex1X = w->tex1ScrollX;
        tex1Y = w->tex1ScrollY;
        if (type == 0) {
            PSMTXScale(texScale, 0.015625f, 0.015625f, 1.0f);
            PSMTXTrans(mtx, (f32)(tex0X / 4), (f32)(tex0Y / 4), 0.0f);
            PSMTXConcat(texScale, mtx, mtx);
            GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
            PSMTXScale(texScale, 0.015625f, 0.015625f, 1.0f);
            PSMTXTrans(mtx, (f32)(tex1X / 4), (f32)(tex1Y / 4), 0.0f);
            PSMTXConcat(texScale, mtx, mtx);
            GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
        } else {
            PSMTXScale(texScale, 0.015625f, 0.0625f, 0.0f);
            PSMTXTrans(mtx, (f32)(tex0X / 4), (f32)(tex0Y / 4), 0.0f);
            PSMTXConcat(texScale, mtx, mtx);
            GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
            PSMTXScale(texScale, 0.015625f, 0.0625f, 1.0f);
            PSMTXTrans(mtx, (f32)(tex1X / 4), (f32)(tex1Y / 4), 0.0f);
            PSMTXConcat(texScale, mtx, mtx);
            GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
        }
        switch (type) {
            case 0:
                magic2_2_line_dl();
                break;
            case 1:
                magic2_4_dl();
                break;
        }
    }
    GXSetProjectionv(proj);
    GXSetScissorBoxOffset(0, 0);
}

void magic2_2_line_dl(void) {
    GXTexObj texObj;
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    effGetTexObjN64(17, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG2);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C1, GX_CC_TEXC, GX_CC_ONE);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C2, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_APREV);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(stg_magic2_2_line_v);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 72);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
    tri2(4, 5, 1, 0, 4, 1, 0, 0);
    tri2(3, 2, 6, 0, 3, 6, 7, 0);
    tri2(8, 9, 5, 0, 8, 5, 4, 0);
    tri2(10, 11, 9, 0, 10, 9, 8, 0);
    tri2(12, 13, 11, 0, 12, 11, 10, 0);
    tri2(7, 6, 14, 0, 7, 14, 15, 0);
    tri2(16, 17, 18, 0, 16, 18, 19, 0);
    tri2(19, 18, 20, 0, 19, 20, 21, 0);
    tri2(22, 23, 17, 0, 22, 17, 16, 0);
    tri2(24, 25, 23, 0, 24, 23, 22, 0);
    tri2(15, 14, 25, 0, 15, 25, 24, 0);
}

void magic2_4_dl(void) {
    GXTexObj texObj;
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    effGetTexObjN64(18, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObjN64(19, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG2);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C1, GX_CC_TEXC, GX_CC_ONE);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C2, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_APREV);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(stg_magic2_1_v);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 72);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
    tri2(4, 5, 1, 0, 4, 1, 0, 0);
    tri2(3, 2, 6, 0, 3, 6, 7, 0);
    tri2(8, 9, 5, 0, 8, 5, 4, 0);
    tri2(10, 11, 9, 0, 10, 9, 8, 0);
    tri2(12, 13, 11, 0, 12, 11, 10, 0);
    tri2(7, 6, 14, 0, 7, 14, 15, 0);
    tri2(16, 17, 18, 0, 16, 18, 19, 0);
    tri2(19, 18, 20, 0, 19, 20, 21, 0);
    tri2(22, 23, 17, 0, 22, 17, 16, 0);
    tri2(24, 25, 23, 0, 24, 23, 22, 0);
    tri2(15, 14, 25, 0, 15, 25, 24, 0);
}
