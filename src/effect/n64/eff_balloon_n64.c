#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os/OSFastCast.h>
#include <string.h>

double sin(double);
double cos(double);

typedef struct EffBalloonN64Point {
    f32 x, y;
} EffBalloonN64Point;

typedef struct EffBalloonN64Work {
    s32 type;                      // 0x0
    Vec position;                  // 0x4
    f32 rotation;                  // 0x10
    f32 targetScale;               // 0x14
    f32 scale;                     // 0x18
    s32 lifetime;                  // 0x1C
    s32 frame;                     // 0x20
    EffBalloonN64Point points[20]; // 0x24
    f32 offsetX;                   // 0xC4
    f32 offsetY;                   // 0xC8
} EffBalloonN64Work;

typedef struct effN64Vtx {
    s16 x, y, z;   // 0x0
    s16 s, t;      // 0x6
    u8 r, g, b, a; // 0xA
} effN64Vtx;

void effGetTexObjN64(u32 id, GXTexObj* obj);
void effSetVtxDescN64(effN64Vtx* vtx);
void tri1();

static effN64Vtx balloon_vtx[] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 255},
    {0, 0, 0, 0, 480, 0, 0, 0, 255},
    {0, 0, 0, 0, 512, 0, 0, 0, 255},
    {0, 0, 0, 0, 512, 0, 0, 0, 255},
    {0, 0, 0, 0, 512, 0, 0, 0, 255},
    {0, 0, 0, 0, 512, 0, 0, 0, 255},
    {0, 0, 0, 0, 512, 0, 0, 0, 255},
    {0, 0, 0, 0, 512, 0, 0, 0, 255},
    {0, 0, 0, 0, 512, 0, 0, 0, 255},
    {0, 0, 0, 0, 512, 0, 0, 0, 255},
    {0, 0, 0, 0, 512, 0, 0, 0, 255},
    {0, 0, 0, 0, 512, 0, 0, 0, 255},
    {0, 0, 0, 0, 512, 0, 0, 0, 255},
    {0, 0, 0, 160, 512, 0, 0, 0, 255},
    {0, 0, 0, 160, 512, 0, 0, 0, 255},
    {0, 0, 0, 0, 512, 0, 0, 0, 255},
    {0, 0, 0, 160, 512, 0, 0, 0, 255},
    {0, 0, 0, 160, 512, 0, 0, 0, 255},
    {0, 0, 0, 160, 512, 0, 0, 0, 255},
    {0, 0, 0, 160, 512, 0, 0, 0, 255},
    {0, 0, 0, 160, 512, 0, 0, 0, 255},
    {0, 0, 0, 0, 512, 0, 0, 0, 255},
};

void effBalloonMain(EffectEntry* effect);
void effBalloonDisp(CameraId cameraId, EffectEntry* effect);


EffectEntry* effBalloonN64Entry(s32 type, f32 x, f32 y, f32 z, f32 targetScale, f32 rotation) {
    EffectEntry* effect;
    EffBalloonN64Work* work;

    effect = effEntry();
    effect->type = "BalloonN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffBalloonN64Work));
    effect->userdata = work;
    effect->callback = effBalloonMain;
    effect->flags |= 2;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->rotation = rotation;
    work->lifetime = 100;
    work->targetScale = targetScale;
    work->scale = 0.0f;
    work->frame = 0;
    return effect;
}

void effBalloonMain(EffectEntry* effect) {
    s32 lifetime;
    EffBalloonN64Work* work = effect->userdata;
    EffBalloonN64Point* pt = work->points;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    f32 scale;
    f32 sp;
    s32 i;
    s32 frame;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->lifetime = 99;
    }
    if (work->lifetime < 100) {
        work->lifetime--;
    }
    work->frame++;
    if (work->frame > 0x50000 - 0xE60) {
        work->frame = 0x100;
    }
    lifetime = work->lifetime;
    if (lifetime < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    work->scale += 0.1f * (work->targetScale - work->scale);
    scale = work->scale;
    scale = scale + 0.1f * (scale * (f32)sin(6.2831855f * (f32)(frame * 3) / 360.0f));
    for (i = 0; i < 20; i++, pt++) {
        pt->x = scale * (f32)sin(6.2831855f * (17.0f + 360.0f * (f32)i / 21.0f) / 360.0f);
        pt->y = -scale * (f32)cos(6.2831855f * (17.0f + 360.0f * (f32)i / 21.0f) / 360.0f);
        sp = (f32)sin(6.2831855f * (f32)(i * 53) / 360.0f);
        sp = (f32)sin(6.2831855f * (30.0f * (5.0f + (f32)i) + (f32)frame * (2.0f + 0.1f * sp)) / 360.0f);
        pt->x += 1.5f * sp;
        sp = (f32)cos(6.2831855f * (f32)(i * 36) / 360.0f);
        sp = (f32)cos(6.2831855f * (50.0f * (5.0f + (f32)i) + (f32)frame * (2.0f + 0.1f * sp)) / 360.0f);
        pt->y += 1.5f * sp;
    }
    work->offsetX = 0.0f;
    if (lifetime < 100) {
        work->offsetY += 1.0f;
    } else {
        work->offsetY = 1.2f * scale;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effBalloonDisp, effect);
}

void effBalloonDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera = camGetPtr(cameraId);
    EffBalloonN64Work* work = effect->userdata;
    s32 type;
    effN64Vtx* vtx = (effN64Vtx*)smartAlloc(sizeof(effN64Vtx) * 32, 3)->address;
    Vec v0;
    Vec v1;
    Vec v2;
    GXTexObj texObj;
    Mtx mtx;
    Mtx tex;
    Mtx rot;
    Mtx a;
    Mtx b;
    s32 i;

    type = work->lifetime;
    if (type == 0) {
        effGetTexObjN64(0x5F, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
    } else if (type == 1) {
        effGetTexObjN64(0x5E, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
    } else {
        effGetTexObjN64(0x5D, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
    }
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(tex, 0.0625f, 0.0625f, 0.0f);
    GXLoadTexMtxImm(tex, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    memcpy(vtx, &balloon_vtx[1], sizeof(effN64Vtx));
    OSInitFastCast();
    OSs16tof32(&vtx->x, &v0.x);
    OSs16tof32(&vtx->y, &v0.y);
    OSs16tof32(&vtx->z, &v0.z);
    PSMTXMultVec(mtx, &v0, &v0);
    OSInitFastCast();
    OSf32tos16(&v0.x, &vtx->x);
    OSf32tos16(&v0.y, &vtx->y);
    OSf32tos16(&v0.z, &vtx->z);
    PSMTXRotRad(rot, 'z', 0.017453292f * work->rotation);
    PSMTXConcat(mtx, rot, a);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXTrans(mtx, work->offsetX, work->offsetY, 0.0f);
    PSMTXConcat(rot, mtx, b);
    PSMTXConcat(a, b, b);
    memcpy(vtx + 1, &balloon_vtx[0], sizeof(effN64Vtx));
    OSInitFastCast();
    OSs16tof32(&vtx[1].x, &v1.x);
    OSs16tof32(&vtx[1].y, &v1.y);
    OSs16tof32(&vtx[1].z, &v1.z);
    PSMTXMultVec(b, &v1, &v1);
    OSInitFastCast();
    OSf32tos16(&v1.x, &vtx[1].x);
    OSf32tos16(&v1.y, &vtx[1].y);
    OSf32tos16(&v1.z, &vtx[1].z);
    for (i = 0; i < 20; i++) {
        PSMTXTrans(mtx, work->points[i].x, work->points[i].y, 0.0f);
        PSMTXConcat(b, mtx, mtx);
        memcpy(&vtx[i + 2], &balloon_vtx[i + 2], sizeof(effN64Vtx));
        OSInitFastCast();
        OSs16tof32(&vtx[i + 2].x, &v2.x);
        OSs16tof32(&vtx[i + 2].y, &v2.y);
        OSs16tof32(&vtx[i + 2].z, &v2.z);
        PSMTXMultVec(mtx, &v2, &v2);
        OSInitFastCast();
        OSf32tos16(&v2.x, (s16*)&vtx[i + 2]);
        OSf32tos16(&v2.y, &vtx[i + 2].y);
        OSf32tos16(&v2.z, &vtx[i + 2].z);
    }
    DCFlushRange(vtx, sizeof(effN64Vtx) * 32);
    GXInvalidateVtxCache();
    GXLoadPosMtxImm(camera->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(vtx);
    if (type < 100) {
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
        tri1(0x15, 2, 1, 0);
        for (i = 0; i < 19; i++) {
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
            tri1(1, i + 2, i + 3, 1);
        }
    } else {
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri1(0, 1, 2, 0);
        tri1(0, 0x15, 1, 0);
        for (i = 0; i < 19; i++) {
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
            tri1(1, i + 2, i + 3, 1);
        }
    }
}

