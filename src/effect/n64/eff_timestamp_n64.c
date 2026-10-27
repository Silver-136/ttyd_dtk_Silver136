#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_n64.h"
#include "eff/n64/eff_timestamp_n64.h"

double sin(double);

typedef struct EffTimestampN64Work {
    s32 type;           // 0x0
    Vec position;       // 0x4
    s32 lifetime;       // 0x10
    s32 frame;          // 0x14
    s32 r;              // 0x18
    s32 g;              // 0x1C
    s32 b;              // 0x20
    s32 bright;         // 0x24
    f32 scale;          // 0x28
    s32 alpha;          // 0x2C
    f32 scale2;         // 0x30
    f32 posX[10];       // 0x34
    f32 posY[10];       // 0x5C
    f32 posZ[10];       // 0x84
    f32 rotX[10];       // 0xAC
    f32 rotY[10];       // 0xD4
    f32 rotZ[10];       // 0xFC
    s32 digitAlpha[10]; // 0x124
    s32 delay[10];      // 0x14C
} EffTimestampN64Work;

static s32 pos_data[30] = {
    -50, -70, 5,  80, -90, 20, 20, -50, 45,  -80, 30,  10,  30,  0,   35,
    120, -30, 50, 10, 50,  30, 100, 30, 15,  40,  100, 40,  140, 90,  25,
};

static u8 scale_data[4] = {120, 80, 40, 60};
static u8 colr[17] = {255, 255, 255, 198, 138, 130, 130, 130, 130, 130, 175, 215, 250, 255, 255, 255, 255};
static u8 colg[17] = {130, 130, 130, 130, 130, 165, 213, 255, 255, 255, 255, 255, 255, 225, 198, 159, 130};
static u8 colb[17] = {130, 196, 255, 255, 255, 255, 255, 255, 202, 144, 130, 130, 130, 130, 130, 130, 130};

void effTimestampMain(EffectEntry* effect);
void effTimestampDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effTimestampN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale, s32 lifetime) {
    EffectEntry* effect;
    EffTimestampN64Work* work;
    s32* p;
    s32 i;

    effect = effEntry();
    effect->type = "TimestampN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffTimestampN64Work));
    effect->userdata = work;
    effect->callback = effTimestampMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->bright = 0;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->r = 70;
    work->g = 180;
    work->b = 120;
    work->alpha = 0;
    work->scale2 = 1.0f;
    for (i = 0; i < 10; i++) {
        work->posX[i] = pos_data[i * 3];
        work->posY[i] = pos_data[i * 3 + 1];
        work->posZ[i] = 0.0f;
        work->rotX[i] = 0.0f;
        work->rotY[i] = 0.0f;
        work->rotZ[i] = 0.0f;
        work->digitAlpha[i] = 0;
        work->delay[i] = pos_data[i * 3 + 2];
    }
    return effect;
}

void effTimestampMain(EffectEntry* effect) {
    EffTimestampN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 life;
    s32 frame;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->lifetime = 64;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    life = work->lifetime;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    if (life < 64) {
        work->scale2 -= 0.05f;
        if (work->scale2 < 0.0f) {
            work->scale2 = 0.0f;
        }
    }
    if (life < 32) {
        work->alpha += 128;
        if (work->alpha > 255) {
            work->alpha = 255;
        }
        work->scale2 = 0.0f;
    }
    if (life < 16) {
        work->bright = life << 4;
    }
    if (frame < 16) {
        work->bright = (frame << 4) + 15;
    }
    for (i = 0; i < 10; i++) {
        // FAKE: comma expression; --work->delay[i] == 0 changes the object
        if (work->delay[i] == 0 || (work->delay[i] -= 1, work->delay[i] == 0)) {
            work->digitAlpha[i] += 8;
            if (work->digitAlpha[i] > 255) {
                work->digitAlpha[i] = 255;
            }
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effTimestampDisp, effect);
}

void effTimestampDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffTimestampN64Work* work;
    GXTexObj texObj;
    Mtx trans;
    Mtx scale;
    Mtx rot;
    Mtx mtx;
    s32 frame;
    s32 bright;
    s32 i;
    effN64Vtx* vtx;
    effN64Vtx* p;
    s32 t;
    s32 k;
    s32 n;
    s32 sx, sy, y;
    f32 sc, a1, a2, s1, s2;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    frame = work->frame;
    bright = work->bright;
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(trans, scale, mtx);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXSetNumChans(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ONE, GX_CC_A1, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    effGetTexObjN64(0x5A, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObjN64(0x5B, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.015625f, 0.015625f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    GXSetTevColor(GX_TEVREG1, (GXColor){0, 0, 0, work->alpha});
    vtx = (effN64Vtx*)smartAlloc(sizeof(effN64Vtx) * 32, 3)->address;
    for (i = 0; i < 16; i++) {
        p = &vtx[i * 2];
        sc = work->scale2;
        a1 = 6.2831855f * (f32)(frame * 10 + i * 60) / 360.0f;
        a2 = 6.2831855f * (f32)(frame * 10 + i * 6) / 360.0f;
        s1 = sin(a1);
        sx = (s32)(sc * (500.0f * s1));
        s2 = sin(a2);
        sy = (s32)(sc * (200.0f * s2));
        p->x = sx - 3200;
        y = sy + i * 400 - 3000;
        t = (i * 2) << 6;
        p->y = y;
        p->z = 0;
        p->s = 0;
        p->t = t;
        p[1].x = sx + 3200;
        p[1].y = y;
        p[1].z = 0;
        p[1].s = 2048;
        p[1].t = t;
    }
    DCFlushRange(vtx, sizeof(effN64Vtx) * 32);
    GXInvalidateVtxCache();
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(vtx);
    for (i = 0; i < 10; i++) {
        PSMTXTrans(trans, work->posX[i], work->posY[i], work->posZ[i]);
        PSMTXScale(scale, 0.01f * (f32)scale_data[i % 4], 0.01f * (f32)scale_data[i % 4], 0.01f * (f32)scale_data[i % 4]);
        PSMTXConcat(trans, scale, trans);
        PSMTXRotRad(rot, 'x', 0.017453292f * work->rotX[i]);
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 'y', 0.017453292f * work->rotY[i]);
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rotZ[i]);
        PSMTXConcat(trans, rot, trans);
        PSMTXScale(scale, 0.01f, 0.01f, 0.01f);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(mtx, trans, trans);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetTevColor(GX_TEVREG0, (GXColor){colr[i], colg[i], colb[i], bright * work->digitAlpha[i] / 255});
        for (n = 0, k = 0; n < 15; n++, k += 2) {
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(k, k + 2, k + 1, k, k + 1, k + 2, k + 3, k);
        }
    }
}
