#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os/OSFastCast.h>
#include <stdlib.h>
#include "eff/eff_n64.h"




typedef struct EffOpukuWcurtainN64Work {
    s32 type;       // 0x0
    Vec position;   // 0x4
    s32 lifetime;   // 0x10
    s32 frame;      // 0x14
    s32 colorR;     // 0x18
    s32 colorG;     // 0x1C
    s32 colorB;     // 0x20
    s32 alpha;      // 0x24
    f32 scale;      // 0x28
    f32 offset[12]; // 0x2C
    f32 speed[12];  // 0x5C
} EffOpukuWcurtainN64Work;

void effOpukuWcurtainMain(EffectEntry* effect);
void effOpukuWcurtainDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effOpukuWcurtainN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffOpukuWcurtainN64Work* work;
    s32 i;

    effect = effEntry();
    effect->type = "OpukuWcurtainN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffOpukuWcurtainN64Work));
    effect->userdata = work;
    effect->callback = effOpukuWcurtainMain;
    effect->flags |= 2;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->alpha = 0;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->colorR = 20;
    work->colorG = 120;
    work->colorB = 255;
    for (i = 0; i < 12; i++) {
        work->offset[i] = 0.0f;
        work->speed[i] = -1.0f + 0.01f * (f32)(rand() % 11);
        work->speed[i] *= 0.5f;
    }
    return effect;
}

void effOpukuWcurtainMain(EffectEntry* effect) {
    EffOpukuWcurtainN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & 4) {
        effect->flags &= ~4;
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
    i = work->frame;
    if (work->lifetime < 16) {
        work->alpha = work->lifetime << 4;
    }
    if (i < 16) {
        work->alpha = (i << 4) + 15;
    }
    for (i = 0; i < 12; i++) {
        work->offset[i] += work->speed[i];
        work->speed[i] += -0.05f;
        if (work->speed[i] > 5.0f) {
            work->speed[i] += 0.1f * (5.0f - work->speed[i]);
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effOpukuWcurtainDisp, effect);
}

void effOpukuWcurtainDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffOpukuWcurtainN64Work* work;
    Mtx mtx;
    Mtx scale;
    GXTexObj texObj;
    Vec v;
    f32 s;
    s32 i;
    effN64Vtx* vtx;
    effN64Vtx* p;
    s32 j;
    s32 frame;
    s32 m;
    s32 n;
    s32 t;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    frame = work->frame;
    s = 0.1f * work->scale;
    GXSetTevColor(GX_TEVREG0, (GXColor){work->colorR, work->colorG, work->colorB, work->alpha});
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    effGetTexObjN64(0x8F, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    GXSetCullMode(GX_CULL_NONE);
    p = (effN64Vtx*)smartAlloc(sizeof(effN64Vtx) * 48, 3)->address;
    vtx = p;
    for (i = 0; i < 12; i++) {
        t = (frame + i * 4) << 5;
        p[0].x = (i * 5 - 30) * 10;
        p[0].y = 600;
        p[0].z = 0;
        p[0].s = t;
        p[0].t = (s32)work->offset[i] << 5;
        p[0].r = 255;
        p[0].g = 255;
        p[0].b = 255;
        p[0].a = 255 - i * 20;
        p[1].x = (i * 5 - 30) * 10;
        p[1].y = (s32)(10.0f * (60.0f + work->offset[i]));
        p[1].z = 0;
        p[1].s = t;
        p[1].t = 0x400;
        p[1].r = 255;
        p[1].g = 255;
        p[1].b = 255;
        p[1].a = 255 - i * 20;
        p += 2;
    }
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scale, s, s, s);
    PSMTXConcat(mtx, scale, mtx);
    for (j = 0; j < i * 2; j++) {
        OSInitFastCast();
        OSs16tof32(&vtx[j].x, &v.x);
        OSs16tof32(&vtx[j].y, &v.y);
        OSs16tof32(&vtx[j].z, &v.z);
        PSMTXMultVec(mtx, &v, &v);
        OSInitFastCast();
        OSf32tos16(&v.x, &vtx[j].x);
        OSf32tos16(&v.y, &vtx[j].y);
        OSf32tos16(&v.z, &vtx[j].z);
    }
    DCFlushRange(vtx, sizeof(effN64Vtx) * 48);
    GXInvalidateVtxCache();
    effSetVtxDescN64(vtx);
    GXLoadPosMtxImm(camera->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    for (j = 0; j < i - 1; j++) {
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        frame = j << 1;
        tri2(frame, frame + 2, frame + 1, frame, frame + 1, frame + 2, frame + 3, frame);
    }
}


