#include "eff/n64/eff_vapor_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_n64.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>


f32 vaporUnused[16] = {-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f};
static s8 dx_data[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -8, -15, -21, -26, -30, -34, -37, -39, -40, -41, -42, -43, -44, -45, -46, -47, -48, -49, -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static u8 scale_data[] = {
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 52, 55, 57, 60, 62, 65, 67, 70, 72, 75, 77, 80, 82, 85, 87, 90, 92, 95, 97, 100, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69
};
static u8 seq_data[] = {
    0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 190, 195, 210, 208, 211, 214, 217, 220, 223, 226, 229, 232, 235, 238, 241, 244, 247, 250, 253, 255, 0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 190, 195, 210, 208, 211, 214, 217, 220, 223, 226, 229, 232, 235, 238, 241, 244, 247, 250, 253, 255, 0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 190, 195, 210, 208, 211, 214, 217, 220, 223, 226, 229, 232, 235, 238, 241, 244, 247, 250, 253, 255
};
static u8 rz_data[3][30] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180
};

static effN64Vtx size64x64_tex32x32_vtx[] = {
    {-32, 0, 2, 0, 1024, 120, 120, 0, 255},
    {32, 0, 2, 1024, 1024, 120, 120, 0, 255},
    {32, 64, 2, 1024, 0, 120, 120, 0, 255},
    {-32, 64, 2, 0, 0, 120, 120, 0, 255},
    {-32, 0, 2, 0, 2048, 120, 120, 0, 255},
    {32, 0, 2, 1024, 2048, 120, 120, 0, 255},
    {32, 64, 2, 1024, 1024, 120, 120, 0, 255},
    {-32, 64, 2, 0, 1024, 120, 120, 0, 255},
    {-32, 0, 2, 0, 3072, 120, 120, 0, 255},
    {32, 0, 2, 1024, 3072, 120, 120, 0, 255},
    {32, 64, 2, 1024, 2048, 120, 120, 0, 255},
    {-32, 64, 2, 0, 2048, 120, 120, 0, 255},
    {-32, 0, 2, 0, 4096, 120, 120, 0, 255},
    {32, 0, 2, 1024, 4096, 120, 120, 0, 255},
    {32, 64, 2, 1024, 3072, 120, 120, 0, 255},
    {-32, 64, 2, 0, 3072, 120, 120, 0, 255},
    {-32, 0, 2, 0, 5120, 120, 120, 0, 255},
    {32, 0, 2, 1024, 5120, 120, 120, 0, 255},
    {32, 64, 2, 1024, 4096, 120, 120, 0, 255},
    {-32, 64, 2, 0, 4096, 120, 120, 0, 255},
};



static const GXColor kColor1;
static const GXColor kColor2;

void effVaporMain(EffectEntry* effect);
void effVaporDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effVaporN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffVaporN64Work* work;

    effect = effEntry();
    effect->type = "VaporN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffVaporN64Work));
    effect->userdata = work;
    effect->callback = effVaporMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->alpha = 255;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->r1 = 255;
    work->g1 = 255;
    work->b1 = 255;
    work->r2 = 255;
    work->g2 = 255;
    work->b2 = 255;
    work->a2 = 255;
    work->anim = 0.0f;
    work->vel = 0.0f;
    return effect;
}

void effVaporMain(EffectEntry* effect) {
    EffVaporN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 type;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    type = work->type;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->lifetime = 16;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    if (work->lifetime < 16) {
        work->alpha = work->lifetime << 4;
    }
    switch (type) {
    case 0:
    case 1:
        work->vel += 0.02f;
        break;
    }
    work->position.y += work->vel;
        if (work->vel > 0.5f) {
            work->vel = 0.5f;
        }
        if (work->vel < -1.5f) {
            work->vel = -1.5f;
        }
    work->anim += 1.0f;
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effVaporDisp, effect);
}

void effVaporDisp(CameraId cameraId, EffectEntry* effect) {
    EffVaporN64Work* work;
    s32 frame;
    CameraEntry* camera;
    s32 alpha;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx viewMtx;
    s32 n;
    s32 type;
    f32 s;
    f32 f;
    s32 a2;
    s32 sub;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    frame = work->frame;
    alpha = work->alpha;
    type = work->type;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, viewMtx);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r1, work->g1, work->b1, 0.5f * alpha});
    switch (type) {
    case 0:
    case 2:
        GXSetNumChans(0);
        GXSetNumTevStages(3);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_A1, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A1, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_C1, GX_CC_C0, GX_CC_CPREV, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_A0, GX_CA_APREV, GX_CA_ZERO);
        effGetTexObjN64(0x69, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXLoadTexObj(&texObj, GX_TEXMAP1);
        GXSetNumTexGens(2);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
        break;
    default:
        GXSetNumChans(0);
        GXSetNumTevStages(3);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_A1, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A1, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_C1, GX_CC_C0, GX_CC_CPREV, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        effGetTexObjN64(0x6A, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXLoadTexObj(&texObj, GX_TEXMAP1);
        GXSetNumTexGens(2);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
        break;
    }
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size64x64_tex32x32_vtx);
    n = frame;
    if (n >= 30) {
        n = 29;
    }
        s = 0.01f * (scale_data + type * 30)[n];
    f = -(f32)rz_data[type][n];
    PSMTXTrans(mtx, (f32)(dx_data + type * 30)[n], 0.0f, 0.0f);
    PSMTXRotRad(rot, 'z', 0.017453292f * f);
    PSMTXScale(scale, s, s, s);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(viewMtx, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    f = 5.0f * (seq_data + type * 30)[n];
    f *= 0.00390625f;
    sub = (s32)f;
    a2 = 256.0f * (f - sub);
    PSMTXScale(scale, 0.032258064f, 0.007874016f, 0.0f);
    PSMTXTrans(mtx, 0.0f, 32.0f, 0.0f);
    PSMTXConcat(scale, mtx, scale);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    if (sub <= 4) {
        if (sub >= 4) {
            PSMTXScale(scale, 0.0f, 0.0f, 0.0f);
            GXLoadTexMtxImm(scale, GX_TEXMTX1, GX_MTX2x4);
        } else {
            PSMTXScale(scale, 0.032258064f, 0.007874016f, 0.0f);
            GXLoadTexMtxImm(scale, GX_TEXMTX1, GX_MTX2x4);
        }
        GXSetTevColor(GX_TEVREG1, (GXColor){work->r2, work->g2, work->b2, a2});
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(sub * 4, sub * 4 + 1, sub * 4 + 2, 0, sub * 4, sub * 4 + 2, sub * 4 + 3, 0);
    }
}
