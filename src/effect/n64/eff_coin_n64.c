#include "eff/n64/eff_coin_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

typedef struct EffCoinN64Work {
    s32 type;     // 0x0
    s32 frame;    // 0x4
    Vec position; // 0x8
    s32 timer;    // 0x14
    s32 counter;  // 0x18
    f32 scale;    // 0x1C
    s32 delay;    // 0x20
} EffCoinN64Work;

typedef struct EffCoinN64Data {
    s32 delay; // 0x0
    s32 x;     // 0x4
    s32 y;     // 0x8
    s32 z;     // 0xC
    s32 scale; // 0x10
} EffCoinN64Data;

static EffCoinN64Data _data[] = {
    {2, 0, 0, 0, 100},
    {4, 4, -2, 0, 100},
    {6, -2, 2, 0, 70},
    {8, 2, 4, 0, 40},
};

static effN64Vtx size16x16_tex32x32_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 1024, 0, 0, 0, 0, 255},
    {8, 8, 0, 1024, 1024, 0, 0, 0, 255},
    {-8, 8, 0, 0, 1024, 0, 0, 0, 255},
};

void effCoinMain(EffectEntry* effect);
void effCoinDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effCoinN64Entry(s32 type, s32 unused, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffCoinN64Work* work;
    EffCoinN64Data* data;
    s32 i;

    effect = effEntry();
    effect->type = "CoinN64";
    effect->count = 5;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffCoinN64Work) * 5);
    effect->userdata = work;
    effect->callback = effCoinMain;
    work->type = type;
    work->counter = 0;
    work->timer = 14;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work++;
    for (i = 1; i < 5; i++, work++) {
        data = &_data[i - 1];
        work->position.x = data->x;
        work->position.y = data->y;
        work->position.z = data->z;
        work->frame = -1;
        work->delay = data->delay;
        work->scale = scale * data->scale * 0.01f;
    }
    return effect;
}

void effCoinMain(EffectEntry* effect) {
    EffCoinN64Work* work = effect->userdata;
    s32 i;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->timer--;
    work->counter++;
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay > 0) {
            work->delay--;
            if (work->delay > 0) {
                continue;
            }
        }
        work->frame++;
        if (work->frame >= 10) {
            work->delay = -1;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effCoinDisp, effect);
}

void effCoinDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffCoinN64Work* work;
    s32 i;
    f32 scale;
    Mtx mtx;
    Mtx tmp;
    Mtx rot;
    Mtx billboard;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, billboard);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_C0, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    effGetTexObjN64(0x84, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x16_tex32x32_vtx);
    GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0x0F, 0xFF});
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay != 0) {
            continue;
        }
        scale = work->scale;
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        if (scale != 1.0f) {
            PSMTXScale(tmp, scale, scale, 1.0f);
            PSMTXConcat(mtx, tmp, mtx);
        }
        PSMTXConcat(billboard, mtx, mtx);
        PSMTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        PSMTXScale(tmp, 0.03125f, 0.00625f, 0.0f);
        PSMTXTrans(mtx, 0.0f, 32.0f * (4 - (work->frame >> 1)), 0.0f);
        PSMTXConcat(tmp, mtx, tmp);
        GXLoadTexMtxImm(tmp, GX_TEXMTX0, GX_MTX2x4);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
