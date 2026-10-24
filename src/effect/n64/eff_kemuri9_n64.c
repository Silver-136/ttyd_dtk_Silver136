#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_n64.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

double sin(double x);
double cos(double x);

typedef struct EffKemuri9N64Work {
    u16 type;        // 0x0
    u8 pad_2[2];     // 0x2
    Vec position;    // 0x4
    f32 dirX;        // 0x10
    f32 velY;        // 0x14
    f32 dirZ;        // 0x18
    f32 radius;      // 0x1C
    f32 radiusEnd;   // 0x20
    s32 timer;       // 0x24
    s32 counter;     // 0x28
    s32 pad_2c;      // 0x2C
    s32 texU;        // 0x30
    s32 texV;        // 0x34
    s32 frame;       // 0x38
    s32 frameMax;    // 0x3C
} EffKemuri9N64Work;

void effKemuri9Main(EffectEntry* effect);
void effKemuri9Disp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effKemuri9N64Entry(s32 type, s32 count, s32 life, f32 x, f32 y, f32 z, f32 radiusEnd, f32 angle) {
    EffectEntry* effect;
    EffKemuri9N64Work* work;
    EffKemuri9N64Work* p;
    s32 i;
    s32 ir;
    f32 twoPi;
    f32 zero;
    f32 ang;

    effect = effEntry();
    effect->type = "Kemuri9N64";
    effect->count = count + 1;
    work = __memAlloc(HEAP_EFFECT, (count + 1) * sizeof(EffKemuri9N64Work));
    effect->userdata = work;
    effect->callback = effKemuri9Main;
    zero = 0.0f;
    twoPi = 6.2831855f;
    work->timer = life;
    work->counter = 0;
    work->pad_2c = life;
    work->texU = 0;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->radius = 10.0f;
    work->radiusEnd = radiusEnd;
    work->texU = 0;
    work->texV = 0;
    work->frameMax = life;
    work->frame = 0;
    zero = 0.0f;
    twoPi = 6.2831855f;
    p = work + 1;
    for (i = 1; i < count + 1; i++, p++) {
        p->position.x = zero;
        p->position.y = zero;
        p->position.z = zero;
        ang = twoPi * (angle + (f32)((i - 1) * 360 / count)) / 360.0f;
        p->dirX = sin(ang);
        p->velY = zero;
        p->dirZ = cos(ang);
    }
    return effect;
}

void effKemuri9Main(EffectEntry* effect) {
    EffKemuri9N64Work* work = effect->userdata;
    s32 i;
    s32 ir;

    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    {
        s32 v = (s32)(16.0f * (f32)work->frame / (f32)work->frameMax);
        s32 u = (s32)(16.0f * (f32)++work->frame / (f32)work->frameMax);
        if (u > 15) {
            u = 15;
        }
        if (v > 15) {
            v = 15;
        }
        work->texU = v;
        work->texV = u;
    }
    work->counter++;
    work->timer--;
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    work->radius += 0.1f * (work->radiusEnd - work->radius);
    ir = (s32)work->radius;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        work->velY += 0.04f;
        work->velY *= 0.9f;
        work->position.x = (f32)ir * work->dirX;
        work->position.y += work->velY;
        work->position.z = (f32)ir * work->dirZ;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effKemuri9Disp, effect);
}

void effKemuri9Disp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera = camGetPtr(cameraId);
    EffKemuri9N64Work* work = effect->userdata;
    s32 i;
    s32 u;
    u16 type;
    s32 v;
    GXColor color;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx view;
    f32 w;
    f32 half = 0.5f;
    f32 h;

    u = work->texU;
    v = work->texV;
    type = work->type;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, view);
    effGetTexObj(0x28, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    color = (GXColor){0xE6, 0xDE, 0xDE, 0xFF};
    GXSetChanMatColor(GX_COLOR0A0, color);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    PSMTXTrans(mtx, 0.0625f * (f32)u, 0.0f, 0.0f);
    PSMTXScale(scale, 0.0625f, 1.0f, 1.0f);
    PSMTXConcat(mtx, scale, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    PSMTXTrans(mtx, 0.0625f * (f32)v, 0.0f, 0.0f);
    PSMTXScale(scale, 0.0625f, 1.0f, 1.0f);
    PSMTXConcat(mtx, scale, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXConcat(view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        if ((s32)type != 0) {
            h = 16.0f;
            w = h;
        } else {
            h = 32.0f;
            w = h;
        }
        GXPosition3f32(-w * half, h * half, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(w * half, h * half, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(w * half, -h * half, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-w * half, -h * half, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
    }
}
