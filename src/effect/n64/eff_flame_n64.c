#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os/OSFastCast.h>
#include <string.h>
#include "eff/eff_n64.h"



typedef struct EffFlameN64Work {
    s32 type;         // 0x0
    Vec position;     // 0x4
    Vec smoothed;     // 0x10
    f32 sx;           // 0x1c
    f32 sy;           // 0x20
    f32 sz;           // 0x24
    s32 alpha;        // 0x28
    s32 life;         // 0x2c
    s32 frame;        // 0x30
    f32 u0;           // 0x34
    f32 u0vel;        // 0x38
    f32 u0target;     // 0x3c
    f32 v0;           // 0x40
    f32 v0vel;        // 0x44
    f32 v0target;     // 0x48
    f32 u1;           // 0x4c
    f32 u1vel;        // 0x50
    f32 u1target;     // 0x54
    f32 v1;           // 0x58
    f32 v1vel;        // 0x5c
    f32 v1target;     // 0x60
    f32 rot;          // 0x64
    u8 c0r, c0g, c0b; // 0x68
    u8 c1r, c1g, c1b; // 0x6b
    u8 c1a;           // 0x6e
    u8 pad6f;
    effN64Vtx vtx[8]; // 0x70
} EffFlameN64Work;

void effFlameMain(EffectEntry* effect);
void effFlameDisp(CameraId cameraId, EffectEntry* effect);

static inline void xf(Mtx m, effN64Vtx* dst, effN64Vtx* src) {
    Vec v;
    memcpy(dst, src, sizeof(effN64Vtx));
    OSInitFastCast();
    OSs16tof32(&dst->x, &v.x);
    OSs16tof32(&dst->y, &v.y);
    OSs16tof32(&dst->z, &v.z);
    PSMTXMultVec(m, &v, &v);
    OSInitFastCast();
    OSf32tos16(&v.x, &dst->x);
    OSf32tos16(&v.y, &dst->y);
    OSf32tos16(&v.z, &dst->z);
}



static effN64Vtx stg_flame_v[] = {
    {-50, -20, 2, 0, 0, 255, 255, 255, 255},
    {50, -20, 2, 1024, 0, 255, 255, 255, 255},
    {50, 100, 2, 1024, 1024, 255, 255, 255, 255},
    {-50, 100, 2, 0, 1024, 255, 255, 255, 255},
    {-50, -20, -2, 0, 0, 255, 255, 255, 255},
    {50, -20, -2, 1024, 0, 255, 255, 255, 255},
    {50, 100, -2, 1024, 1024, 255, 255, 255, 255},
    {-50, 100, -2, 0, 1024, 255, 255, 255, 255},
};

static effN64Vtx stg_flame_2_v[] = {
    {-60, 0, 18, 0, 0, 255, 255, 255, 255},
    {60, 0, 18, 2048, 0, 255, 255, 255, 255},
    {60, 120, 18, 2048, 2048, 255, 255, 255, 255},
    {-60, 120, 18, 0, 2048, 255, 255, 255, 255},
    {-60, 0, -18, 0, 0, 255, 255, 255, 255},
    {60, 0, -18, 2048, 0, 255, 255, 255, 255},
    {60, 120, -18, 2048, 2048, 255, 255, 255, 255},
    {-60, 120, -18, 0, 2048, 255, 255, 255, 255},
};

static effN64Vtx stg_flame_3_v[] = {
    {-50, -50, 2, 0, 0, 255, 255, 255, 255},
    {50, -50, 2, 1024, 0, 255, 255, 255, 255},
    {50, 50, 2, 1024, 1024, 255, 255, 255, 255},
    {-50, 50, 2, 0, 1024, 255, 255, 255, 255},
    {-50, -50, -2, 0, 0, 255, 255, 255, 255},
    {50, -50, -2, 1024, 0, 255, 255, 255, 255},
    {50, 50, -2, 1024, 1024, 255, 255, 255, 255},
    {-50, 50, -2, 0, 1024, 255, 255, 255, 255},
};


EffectEntry* effFlameN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffFlameN64Work* work;

    effect = effEntry();
    effect->type = "FlameN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffFlameN64Work));
    effect->userdata = work;
    effect->callback = effFlameMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->life = 100;
    work->frame = 0;
    work->alpha = 0;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->rot = 0.0f;
    work->v0 = 0.0f;
    work->u0 = 0.0f;
    work->v1 = 0.0f;
    work->u1 = 0.0f;
    switch (type) {
    case 0:
        work->c0r = 75;
        work->c0g = 75;
        work->c0b = 75;
        work->c1r = 255;
        work->c1g = 247;
        work->c1b = 155;
        work->c1a = 0;
        work->sx = 2.0f * scale;
        work->sy = 0.6f * scale;
        work->sz = scale;
        work->u0vel = -0.9f;
        work->v0vel = 0.04f;
        work->u0target = 0.04f;
        work->v0target = 0.9f;
        work->u1vel = -0.9f;
        work->v1vel = 0.04f;
        work->u1target = 0.04f;
        work->v1target = 0.9f;
        break;
    case 1:
        work->c0r = 75;
        work->c0g = 75;
        work->c0b = 75;
        work->c1r = 255;
        work->c1g = 0;
        work->c1b = 0;
        work->c1a = 0;
        work->sx = scale;
        work->sy = scale;
        work->sz = scale;
        work->u0vel = 0.0f;
        work->v0vel = 0.04f;
        work->u0target = 0.04f;
        work->v0target = 0.9f;
        work->u1vel = 0.0f;
        work->v1vel = 0.04f;
        work->u1target = 0.04f;
        work->v1target = 0.9f;
        break;
    case 2:
        work->c0r = 25;
        work->c0g = 121;
        work->c0b = 142;
        work->c1r = 34;
        work->c1g = 0;
        work->c1b = 255;
        work->c1a = 81;
        work->sx = scale;
        work->sy = scale;
        work->sz = scale;
        work->u0vel = 0.0f;
        work->v0vel = 0.04f;
        work->u0target = 0.2f;
        work->v0target = 1.3f;
        work->u1vel = 0.0f;
        work->v1vel = 0.04f;
        work->u1target = 0.16000001f;
        work->v1target = 1.04f;
        break;
    default:
        work->c0r = 179;
        work->c0g = 169;
        work->c0b = 85;
        work->c1r = 34;
        work->c1g = 0;
        work->c1b = 255;
        work->c1a = 63;
        work->sx = scale;
        work->sy = scale;
        work->sz = scale;
        work->u0vel = 0.0f;
        work->v0vel = 0.04f;
        work->u0target = 0.1f;
        work->v0target = 1.7f;
        work->u1vel = 0.0f;
        work->v1vel = 0.04f;
        work->u1target = 0.080000006f;
        work->v1target = 1.36f;
        break;
    }
    work->smoothed.x = work->position.x;
    work->smoothed.y = work->position.y;
    work->smoothed.z = work->position.z;
    return effect;
}

void effFlameMain(EffectEntry* effect) {
    EffFlameN64Work* work;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 life;
    s32 frame;
    s32 type;

    work = effect->userdata;
    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->life = 16;
    }
    work->frame++;
    if (work->frame > 324000) {
        work->frame = 256;
    }
    if (work->life <= 16) {
        work->life--;
    }
    life = work->life;
    if (life < 0) {
        effDelete(effect);
    } else {
    type = work->type;
    frame = work->frame;
    if (type <= 1) {
        if (frame <= 10) {
            work->alpha = (s32)(0.5f * (f32)(128 - work->alpha) + (f32)work->alpha);
        }
    } else {
        if (frame <= 10) {
            work->alpha = frame * 255 / 10;
        }
    }
    if (life < 16) {
        work->alpha = life * 16;
    }
    if (type == 0) {
        work->u0vel += 0.02f * (work->u0target - work->u0vel);
        work->v0vel += 0.02f * (work->v0target - work->v0vel);
        work->sx += 0.04f * (work->sz - work->sx);
        work->sy += 0.04f * (work->sz - work->sy);
    } else {
        work->u0vel += 0.02f * (work->u0target - work->u0vel);
        work->v0vel += 0.02f * (work->v0target - work->v0vel);
        work->u1vel += 0.02f * (work->u1target - work->u1vel);
        work->v1vel += 0.02f * (work->v1target - work->v1vel);
    }
    work->u0 += work->u0vel;
    if (work->u0 < 0.0f) {
        work->u0 += 64.0f;
    } else if (work->u0 > 64.0f) {
        work->u0 -= 64.0f;
    }
    work->v0 += work->v0vel;
    if (work->v0 < 0.0f) {
        work->v0 += 128.0f;
    } else if (work->v0 > 128.0f) {
        work->v0 -= 128.0f;
    }
    work->u1 += work->u1vel;
    if (work->u1 < 0.0f) {
        work->u1 += 64.0f;
    } else if (work->u1 > 64.0f) {
        work->u1 -= 64.0f;
    }
    work->v1 += work->v1vel;
    if (work->v1 < 0.0f) {
        work->v1 += 128.0f;
    } else if (work->v1 > 128.0f) {
        work->v1 -= 128.0f;
    }
    work->smoothed.x += 0.1f * (work->position.x - work->smoothed.x);
    work->smoothed.y += 0.1f * (work->position.y - work->smoothed.y);
    work->smoothed.z += 0.1f * (work->position.z - work->smoothed.z);
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effFlameDisp, effect);
    }
}

void effFlameDisp(CameraId cameraId, EffectEntry* effect) {
    s32 alpha;
    s32 type;
    EffFlameN64Work* work[1];
    CameraEntry* camera;
    Mtx rot;
    Mtx scl;
    Mtx trn;
    Mtx mtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work[0] = effect->userdata;
    type = work[0]->type;
    alpha = work[0]->alpha;
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    switch (type) {
    case 0:
    case 1:
        GXSetNumTevStages(4);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG2);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG2);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_C2, GX_CC_C2, GX_CC_ONE);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_TEXA, GX_CA_ZERO, GX_CA_A2, GX_CA_TEXA);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_C2, GX_CC_TEXC, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_A2, GX_CA_APREV, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_C0, GX_CC_CPREV, GX_CC_C1);
        GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_A0, GX_CA_APREV, GX_CA_ZERO);
        effGetTexObjN64(47, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        effGetTexObjN64(48, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP1);
        break;
    case 2:
        GXSetNumTevStages(5);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG2);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG2);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_C0);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A2, GX_CA_TEXA, GX_CA_A2);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_A1, GX_CA_A2, GX_CA_APREV);
        GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE4, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE4, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE4, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE4, GX_CA_ZERO, GX_CA_A1, GX_CA_A0, GX_CA_APREV);
        effGetTexObjN64(49, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        effGetTexObjN64(50, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP1);
        break;
    case 3:
        break;
    default:
        break;
    }
    PSMTXTrans(trn, work[0]->position.x, work[0]->position.y, work[0]->position.z);
    if (type == 2) {
        PSMTXRotRad(rot, 'y', 0.017453292f * work[0]->rot);
    } else {
        PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    }
    PSMTXConcat(trn, rot, mtx);
    PSMTXScale(scl, work[0]->sx, work[0]->sy, 1.0f);
    PSMTXConcat(mtx, scl, mtx);
    if (type == 0) {
        PSMTXTrans(trn, 10.0f * (-(work[0]->sx - work[0]->sz) / work[0]->sz), 0.0f, 0.0f);
        PSMTXConcat(mtx, trn, mtx);
    }
    switch (type) {
    case 0:
        xf(mtx, &work[0]->vtx[2], &stg_flame_v[2]);
        xf(mtx, &work[0]->vtx[3], &stg_flame_v[3]);
        xf(mtx, &work[0]->vtx[6], &stg_flame_v[6]);
        xf(mtx, &work[0]->vtx[7], &stg_flame_v[7]);
        break;
    case 1:
        xf(mtx, &work[0]->vtx[2], &stg_flame_3_v[2]);
        xf(mtx, &work[0]->vtx[3], &stg_flame_3_v[3]);
        xf(mtx, &work[0]->vtx[6], &stg_flame_3_v[6]);
        xf(mtx, &work[0]->vtx[7], &stg_flame_3_v[7]);
        break;
    default:
        xf(mtx, &work[0]->vtx[2], &stg_flame_2_v[2]);
        xf(mtx, &work[0]->vtx[3], &stg_flame_2_v[3]);
        xf(mtx, &work[0]->vtx[6], &stg_flame_2_v[6]);
        xf(mtx, &work[0]->vtx[7], &stg_flame_2_v[7]);
        break;
    }
    PSMTXTrans(trn, work[0]->position.x, work[0]->position.y, work[0]->position.z);
    if (type == 2) {
        PSMTXRotRad(rot, 'y', 0.017453292f * work[0]->rot);
    } else {
        PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    }
    PSMTXConcat(trn, rot, mtx);
    PSMTXScale(scl, work[0]->sx, work[0]->sy, 1.0f);
    PSMTXConcat(mtx, scl, mtx);
    if (type == 0) {
        PSMTXTrans(trn, 10.0f * (-(work[0]->sx - work[0]->sz) / work[0]->sz), 0.0f, 0.0f);
        PSMTXConcat(mtx, trn, mtx);
    }
    switch (type) {
    case 0:
        xf(mtx, &work[0]->vtx[0], &stg_flame_v[0]);
        xf(mtx, &work[0]->vtx[1], &stg_flame_v[1]);
        xf(mtx, &work[0]->vtx[4], &stg_flame_v[4]);
        xf(mtx, &work[0]->vtx[5], &stg_flame_v[5]);
        break;
    case 1:
        xf(mtx, &work[0]->vtx[0], &stg_flame_3_v[0]);
        xf(mtx, &work[0]->vtx[1], &stg_flame_3_v[1]);
        xf(mtx, &work[0]->vtx[4], &stg_flame_3_v[4]);
        xf(mtx, &work[0]->vtx[5], &stg_flame_3_v[5]);
        break;
    default:
        xf(mtx, &work[0]->vtx[0], &stg_flame_2_v[0]);
        xf(mtx, &work[0]->vtx[1], &stg_flame_2_v[1]);
        xf(mtx, &work[0]->vtx[4], &stg_flame_2_v[4]);
        xf(mtx, &work[0]->vtx[5], &stg_flame_2_v[5]);
        break;
    }
    GXInvalidateVtxCache();
    DCFlushRange(work[0]->vtx, sizeof(work[0]->vtx));
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(work[0]->vtx);
    GXLoadPosMtxImm(camera->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetTevColor(GX_TEVREG0, (GXColor){work[0]->c0r, work[0]->c0g, work[0]->c0b, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work[0]->c1r, work[0]->c1g, work[0]->c1b, work[0]->c1a});
    PSMTXScale(scl, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scl, GX_TEXMTX0, GX_MTX2x4);
    PSMTXScale(scl, 0.03125f, 0.0078125f, 0.0f);
    PSMTXTrans(trn, work[0]->u0, work[0]->v0, 0.0f);
    PSMTXConcat(scl, trn, trn);
    GXLoadTexMtxImm(trn, GX_TEXMTX1, GX_MTX2x4);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(4, 5, 6, 0, 4, 6, 7, 0);
    PSMTXScale(scl, 0.03125f, 0.0078125f, 0.0f);
    PSMTXTrans(trn, 30.0f + work[0]->u1, 30.0f + work[0]->v1, 0.0f);
    PSMTXConcat(scl, trn, trn);
    GXLoadTexMtxImm(trn, GX_TEXMTX1, GX_MTX2x4);
    switch (type) {
    case 0:
    case 1:
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
        break;
    default:
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(2, 1, 0, 0, 3, 2, 0, 0);
        break;
    }
}
