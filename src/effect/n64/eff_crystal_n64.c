#include "eff/n64/eff_crystal_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

double sin(double);

void effCrystalMain(EffectEntry* effect);
void effCrystalDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effCrystalN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffCrystalN64Work* work;

    effect = effEntry();
    effect->type = "CrystalN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffCrystalN64Work));
    effect->userdata = work;
    effect->callback = effCrystalMain;
    effect->flags |= 2;
    work->type = type;
    work->frame = 0;
    work->camera = 4;
    if (lifetime <= 0) {
        work->lifetime = 100;
    } else {
        work->lifetime = lifetime;
    }
    work->a = 0;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->reserved_18 = 0;
    switch (type) {
    case 0:
        work->variant = 0;
        break;
    case 1:
        work->variant = 2;
        break;
    case 2:
        work->variant = 4;
        break;
    case 3:
        work->variant = 6;
        break;
    case 4:
        work->variant = 7;
        break;
    case 5:
        work->variant = 8;
        break;
    case 6:
        work->variant = 9;
        break;
    default:
        work->variant = 10;
        work->scaleVel = -(scale - 0.1f) / (f32)lifetime;
        break;
    }
    return effect;
}

void effCrystalMain(EffectEntry* effect) {
    EffCrystalN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 alpha;
    s32 fade;
    s32 life;
    s32 frame;
    s32 variant;
    f32 sn;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->lifetime = 30;
    }
    if (work->lifetime < 100) {
        work->lifetime--;
    }
    work->frame++;
    life = work->lifetime;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    variant = work->variant;
    alpha = 255;
    fade = 255;
    if (frame < 16) {
        alpha = frame * 16 - 1;
    }
    if (life < 16) {
        alpha = alpha * life * 16 / 256;
    }
    if (life < 16) {
        fade = life * 16;
    }
    switch (variant) {
    case 0:
        work->r = 70;
        work->g = 180;
        work->b = 120;
        sn = (f32)sin(6.2831855f * (f32)(frame * 2) / 360.0f);
        work->a = 190 + (s32)(10.0f * sn);
        work->a = work->a * alpha / 255;
        break;
    case 1:
        work->r = 70;
        work->g = 255;
        work->b = 230;
        work->a = 255;
        work->a = work->a * alpha / 255;
        break;
    case 2:
        work->r = 170;
        work->g = 40;
        work->b = 110;
        sn = (f32)sin(6.2831855f * (f32)(frame * 2) / 360.0f);
        work->a = 190 + (s32)(10.0f * sn);
        work->a = work->a * alpha / 255;
        break;
    case 3:
        work->r = 220;
        work->g = 40;
        work->b = 170;
        work->a = 255;
        work->a = work->a * alpha / 255;
        break;
    case 4:
        work->r = 235;
        work->g = 235;
        work->b = 200;
        sn = (f32)sin(6.2831855f * (f32)(frame * 2) / 360.0f);
        work->a = 215 + (s32)(15.0f * sn);
        work->a = work->a * alpha / 255;
        break;
    case 5:
        work->r = 235;
        work->g = 235;
        work->b = 200;
        work->a = 255;
        work->a = work->a * alpha / 255;
        break;
    case 6:
        work->r = 70;
        work->g = 180;
        work->b = 120;
        work->a = 127;
        work->a = work->a * fade / 255;
        work->scale *= 1.1f;
        break;
    case 7:
        work->r = 162;
        work->g = 189;
        work->b = 174;
        work->a = 160;
        work->a = work->a * fade / 255;
        work->scale *= 1.1f;
        break;
    case 8:
        work->r = 170;
        work->g = 40;
        work->b = 110;
        work->a = 127;
        work->a = work->a * fade / 255;
        work->scale *= 1.1f;
        break;
    case 9:
        work->r = 180;
        work->g = 178;
        work->b = 174;
        work->a = 160;
        work->a = work->a * fade / 255;
        work->scale *= 1.1f;
        break;
    default:
        work->r = 50;
        work->g = 50;
        work->b = 255;
        work->a = 160;
        work->a = work->a * fade / 255;
        work->scale += work->scaleVel;
        break;
    }
    dispEntry(work->camera, dispCalcZ(&position), 2, (DispCallback)effCrystalDisp, effect);
}

effN64Vtx size108x108_tex32x32_vtx[] = {
    {-54, -54, 1, 0, 0, 0, 0, 0, 255},
    {54, -54, 1, 1024, 0, 0, 0, 0, 255},
    {54, 54, 1, 1024, 1024, 0, 0, 0, 255},
    {-54, 54, 1, 0, 1024, 0, 0, 0, 255},
};

effN64Vtx size100x100_tex32x32_vtx[] = {
    {-50, -50, -1, 0, 0, 0, 0, 0, 255},
    {50, -50, -1, 1024, 0, 0, 0, 0, 255},
    {50, 50, -1, 1024, 1024, 0, 0, 0, 255},
    {-50, 50, -1, 0, 1024, 0, 0, 0, 255},
};

void effCrystalDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffCrystalN64Work* work;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx tmp;
    s32 frame;
    s32 alpha;
    s32 type;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    frame = work->frame;
    alpha = work->a;
    type = work->type;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, scale, tmp);
    PSMTXConcat(camera->view, tmp, tmp);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, alpha});
    GXSetCullMode(GX_CULL_NONE);
    if (type < 3) {
        GXSetNumChans(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ONE, GX_CC_A0, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
        PSMTXScale(scale, 0.03125f, 0.0625f, 0.0f);
        GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
        effGetTexObjN64(0x6D, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        PSMTXRotRad(rot, 'z', 0.017453292f * (f32)frame);
        PSMTXConcat(tmp, rot, rot);
        GXLoadPosMtxImm(rot, GX_PNMTX0);
        GXSetCurrentMtx(0);
        effSetVtxDescN64(size108x108_tex32x32_vtx);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
        PSMTXRotRad(rot, 'z', 0.017453292f * (f32)(-frame * 8));
        PSMTXConcat(tmp, rot, rot);
        GXLoadPosMtxImm(rot, GX_PNMTX0);
        GXSetCurrentMtx(0);
        effSetVtxDescN64(size100x100_tex32x32_vtx);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    } else {
        GXSetNumChans(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
        PSMTXScale(scale, 0.0625f, 0.0625f, 0.0f);
        GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
        effGetTexObjN64(0x6E, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXLoadPosMtxImm(tmp, GX_PNMTX0);
        GXSetCurrentMtx(0);
        effSetVtxDescN64(size100x100_tex32x32_vtx);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
