#pragma defer_codegen off
#include "eff/n64/eff_ripple_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>



effN64Vtx size16x16_tex32x32_0_vtx[] = {
    {-8, 0, -8, 0, 0, 0, 0, 0, 255},
    {8, 0, -8, 1024, 0, 0, 0, 0, 255},
    {8, 0, 8, 1024, 1024, 0, 0, 0, 255},
    {-8, 0, 8, 0, 1024, 0, 0, 0, 255},
};

void effRippleMain(EffectEntry* effect);
void effRippleDisp(CameraId cameraId, EffectEntry* effect);

__declspec(section ".rodata") static Vec vec3_802fb520 = {0.0f, 0.0f, 0.0f};
void effRippleN64SetRxRz(EffectEntry* effect, f32 rx, f32 rz) {
    EffRippleN64Work* work = effect->userdata;
    work->rx = rx;
    work->rz = rz;
}

void effRippleDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    s32 type;
    EffRippleN64Work* work;
    Mtx mtx;
    Mtx scale;
    Mtx rotX;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    type = ((EffRippleN64Work*)effect->userdata)->type;
    work = effect->userdata;
    if (type == 10 || type == 20 || (u32)(type - 1) <= 2 || type == 30) {
        effGetTexObj(0x5B, &texObj);
    } else {
        effGetTexObjN64(0x21, &texObj);
    }
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObjN64(0x22, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.0625f, 0.0625f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    GXLoadTexMtxImm(scale, GX_TEXMTX1, GX_MTX2x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    if (type == 1 || type == 10 || type == 3 || type == 30) {
        GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, work->alpha});
    } else {
        GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0xFF, work->alpha});
    }
    type = work->offset;
    PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
    PSMTXTrans(mtx, 0.0f, type, 0.0f);
    PSMTXConcat(scale, mtx, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rotX, 'x', 0.017453292f * work->rx);
    PSMTXRotRad(scale, 'z', 0.017453292f * work->rz);
    PSMTXConcat(rotX, scale, rotX);
    PSMTXScale(scale, work->scaleY * work->scale, 1.0f, work->scaleY * work->scale);
    PSMTXConcat(mtx, rotX, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x16_tex32x32_0_vtx);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
}

void effRippleMain(EffectEntry* effect) {
    EffRippleN64Work* work;
    EffectEntry* e;
    EffRippleN64Work* w;
    f32 rz;
    s32 lifetime;
    s32 type;
    Vec position;
    Vec tmp;

    work = effect->userdata;
    tmp = vec3_802fb520;
    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    lifetime = work->lifetime;
    type = work->type;
    work->lifetime--;
    work->frame++;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    if (lifetime > 5) {
        work->alpha += 0.3f * (120 - work->alpha);
    }
    if ((type == 1 || type == 2 || type == 3) && lifetime == 15) {
        e = effRippleN64Entry(type * 10, work->position.x, work->position.y, work->position.z, 0.0f);
        rz = work->rz;
        w = e->userdata;
        w->rx = work->rx;
        w->rz = rz;
    }
    if (type == 2 || type == 20) {
        work->scale *= 1.05f;
    }
    if (type == 3 || type == 30) {
        work->scale *= 1.05f;
    }
    if (lifetime < 32) {
        work->alpha = lifetime * 8;
    }
    work->offset++;
    if (work->offset >= 32) {
        work->offset -= 32;
    }
    work->scale += 0.1f;
    dispEntry(work->cameraId, dispCalcZ(&position), 2, (DispCallback)effRippleDisp, effect);
}

EffectEntry* effRippleN64Entry(s32 type, f32 x, f32 y, f32 z, f32 yOffset) {
    EffectEntry* effect;
    EffRippleN64Work* work;

    effect = effEntry();
    effect->type = "RippleN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffRippleN64Work));
    effect->userdata = work;
    effect->callback = effRippleMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->alpha = 0;
    work->frame = 0;
    work->rx = 0.0f;
    work->rz = 0.0f;
    work->scaleY = 1.0f;
    work->cameraId = 4;
    switch (type) {
        case 1:
        case 10:
            work->scale = 0.6f;
            break;
        case 2:
        case 20:
            work->scale = 10.0f;
            break;
        case 3:
        case 30:
            work->scale = 10.0f;
            break;
        default:
            work->scale = 1.0f;
            break;
    }
    work->lifetime = 32;
    work->offset = rand() % 32;
    work->unk28 = y + yOffset;
    return effect;
}
