#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/hitdrv.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os/OSFastCast.h>
#include <stdlib.h>
#include <string.h>
#include "libc_decls.h"
#include <math.h>

double sin(double);
double cos(double);

typedef struct effN64Vtx {
    s16 x, y, z;   // 0x0
    s16 s, t;      // 0x6
    u8 r, g, b, a; // 0xA
} effN64Vtx;

void effGetTexObjN64(u32 id, GXTexObj* obj);
void effSetVtxDescN64(effN64Vtx* vtx);
void tri2();

typedef struct EffSstarN64Work {
    s32 type;          // 0x0
    Vec position;      // 0x4
    Vec velocity;      // 0x10
    f32 bounce;        // 0x1c
    f32 rotZ;          // 0x20
    f32 rotZ2;         // 0x24
    f32 unk28;         // 0x28
    f32 rotSpeed;      // 0x2c
    s32 life;          // 0x30
    f32 unk34;         // 0x34
    s32 subtype;       // 0x38
    s32 idx;           // 0x3c
    Mtx mtx[8];        // 0x40
    f32 scale;         // 0x1c0
    s32 c1c4;          // 0x1c4
    s32 c1c8;          // 0x1c8
    s32 c1cc;          // 0x1cc
    effN64Vtx vtx[10]; // 0x1d0
} EffSstarN64Work;

f32 unused_tbl[16] = {
    -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f,
};

static effN64Vtx size64x64_tex64x64_vtx[] = {
    {-30, -30, 0, 0, 0, 0, 0, 0, 191},
    {30, -30, 0, 2048, 0, 0, 0, 0, 191},
    {30, 30, 0, 2048, 2048, 0, 0, 0, 191},
    {-30, 30, 0, 0, 2048, 0, 0, 0, 191},
};

effN64Vtx size128x128_tex32x32_vtx[] = {
    {-128, 0, 0, 0, 0, 255, 255, 255, 0},
    {0, -128, 0, 1024, 0, 255, 255, 255, 0},
    {128, 0, 0, 1024, 1024, 255, 255, 255, 0},
    {0, 128, 0, 0, 1024, 255, 255, 255, 0},
    {0, 0, 0, 0, 0, 255, 255, 255, 255},
};

effN64Vtx shooting_dust_vtx[10] = {
    {0, 8, 0, 0, 0, 0, 0, 0, 191},
    {0, -8, 0, 0, 2048, 0, 0, 0, 191},
    {0, 9, 0, 512, 0, 0, 0, 0, 191},
    {0, -9, 0, 512, 2048, 0, 0, 0, 191},
    {0, 10, 0, 768, 0, 0, 0, 0, 191},
    {0, -10, 0, 768, 2048, 0, 0, 0, 191},
    {0, 10, 0, 768, 0, 0, 0, 0, 191},
    {0, -10, 0, 768, 2048, 0, 0, 0, 191},
    {0, 11, 0, 896, 0, 0, 0, 0, 191},
    {0, -11, 0, 896, 2048, 0, 0, 0, 191},
};

void effSstarMain(EffectEntry* effect);
void effSstarDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effSstarN64Entry(s32 type, f32 x, f32 y, f32 z, f32 tx, f32 ty, f32 tz, f32 speed) {
    EffectEntry* effect;
    EffSstarN64Work* w;
    f32 dx, dy, dz;
    f32 len, inv;
    f32 sq, ang, c, s, l2, dot;
    s32 i;

    effect = effEntry();
    effect->type = "SstarN64";
    effect->count = 1;
    w = __memAlloc(HEAP_EFFECT, sizeof(EffSstarN64Work));
    effect->userdata = w;
    effect->callback = effSstarMain;
    w->type = 1;
    w->position.x = x;
    w->position.y = y;
    w->position.z = z;
    w->bounce = 0.0f;
    w->rotZ2 = 0.0f;
    w->unk28 = 0.0f;
    w->subtype = type;
    if (type == 3) {
        w->scale = 0.2f;
        w->c1c4 = rand() % 256;
        w->c1c8 = rand() % (0xFF - w->c1c4);
        w->c1cc = 0xFF - w->c1c4 - w->c1c8;
    } else {
        w->scale = 0.5f;
        w->c1c4 = 210;
        w->c1c8 = 186;
        w->c1cc = 90;
    }
    dx = tx - x;
    dy = ty - y;
    dz = tz - z;
    len = dx * dx + dy * dy + dz * dz;
    inv = len;
    if (len) {
        len = sqrtf(len);
        inv = speed / len;
    }
    w->velocity.x = dx * inv;
    w->velocity.y = dy * inv;
    w->velocity.z = dz * inv;
    ang = 6.2831855f * camGetPtr(CAMERA_3D)->viewYaw / 360.0f;
    c = -(f32)cos(ang);
    s = -(f32)sin(ang);
    l2 = c * w->velocity.x;
    sq = l2 * l2 + (s * w->velocity.z) * (s * w->velocity.z);
    if (sq) {
        sq = sqrtf(sq);
    }
    dot = c * w->velocity.x + s * w->velocity.z;
    if (dot < 0.0f) {
        sq = -sq;
        w->rotSpeed = -20.0f;
    } else {
        w->rotSpeed = 20.0f;
    }
    sq = -sq;
    ang = angleABf(0.0f, 0.0f, -w->velocity.y, sq);
    w->rotZ2 = ang;
    w->rotZ = ang;
    w->unk34 = sq;
    w->life = (s32)(len / speed);
    for (i = 0; i < 8; i++) {
        PSMTXTrans(w->mtx[i], w->position.x, w->position.y, w->position.z);
    }
    w->idx = -1;
    return effect;
}

void effSstarMain(EffectEntry* effect) {
    EffSstarN64Work* w = effect->userdata;
    MarioWork* mario = marioGetPtr();
    Vec pos;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    f32 len;
    f32 px, py, pz;
    f32 hx, hz, hnx, hny, hnz;

    tmp.x = w->position.x;
    tmp.y = w->position.y;
    tmp.z = w->position.z;
    pos = tmp;
    if (w->subtype >= 2) {
        len = w->velocity.x * w->velocity.x + w->velocity.y * w->velocity.y + w->velocity.z * w->velocity.z;
        if (len > 0.01f) {
            len = 1.0f / sqrtf(len);
        }
        px = w->position.x - 32.0f * (w->velocity.x * len);
        py = w->position.y - 32.0f * (w->velocity.y * len);
        pz = w->position.z - 32.0f * (w->velocity.z * len);
        len = 64.0f;
        if (w->type != 0 && w->velocity.y < 0.0f &&
            hitCheckFilter(NULL, &hx, &hx, &hz, &len, &hnx, &hny, &hnz, px, py, pz, 0.0f, -1.0f, 0.0f) &&
            len < 42.0f) {
            w->velocity.y = 0.6f * -w->velocity.y;
            w->velocity.x *= 0.7f;
            w->unk34 *= 0.7f;
            w->rotSpeed *= 0.7f;
            w->bounce += 1.0f;
            w->type = 0;
            if (w->bounce >= 10.0f) {
                w->life = -1;
            }
        }
        if (w->bounce) {
            w->velocity.y += -0.5f;
            w->rotZ2 += w->rotSpeed;
            w->rotZ = angleABf(0.0f, 0.0f, -w->velocity.y, w->unk34);
        }
    }
    if (gp->inBattle == 0) {
        if (mario->position.y - w->position.y > 300.0f) {
            w->life = -1;
        }
    } else if (w->position.y < -300.0f) {
        w->life = -1;
    }
    w->position.x += w->velocity.x;
    w->position.y += w->velocity.y;
    w->position.z += w->velocity.z;
    if (w->life < 0) {
        effDelete(effect);
    } else {
        dispEntry(4, dispCalcZ(&pos), 2, (DispCallback)effSstarDisp, effect);
    }
}

void effSstarDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffSstarN64Work* w;
    s32 i;
    s32 c1, c2, c3;
    s32 k, base;
    f32 scale;
    Mtx mtx;
    Mtx rot;
    Mtx sc;
    Mtx tmp;
    GXTexObj tex;
    Vec v0;
    Vec v1;

    camera = camGetPtr(cameraId);
    w = effect->userdata;
    scale = w->scale;
    base = w->subtype;
    c1 = w->c1c4;
    c2 = w->c1c8;
    c3 = w->c1cc;
    PSMTXTrans(mtx, w->position.x, w->position.y, w->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXConcat(mtx, rot, tmp);
    PSMTXScale(sc, scale, scale, scale);
    PSMTXConcat(tmp, sc, tmp);
    PSMTXRotRad(rot, 'z', 0.017453292f * w->rotZ2);
    PSMTXConcat(tmp, rot, tmp);
    GXSetCullMode(GX_CULL_NONE);
    GXSetTevColor(GX_TEVREG0, (GXColor){c1, c2, c3, 255});
    GXSetTevColor(GX_TEVREG1, (GXColor){127, 127, 127, 127});
    GXSetTevColor(GX_TEVREG2, (GXColor){80, 80, 80, 80});
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(sc, 0.015625f, 0.015625f, 0.0f);
    GXLoadTexMtxImm(sc, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(9, &tex);
    GXLoadTexObj(&tex, GX_TEXMAP0);
    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 0, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 8, 5, 2);
    GXSetTevAlphaIn(0, 7, 4, 1, 7);
    GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(1, 1, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0xF, 4, 5, 0);
    GXSetTevAlphaIn(1, 7, 7, 7, 0);
    GXSetTevOrder(2, 0xFF, 0xFF, 4);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 6, 0xF, 0xF, 0);
    GXSetTevAlphaIn(2, 7, 0, 5, 7);
    effSetVtxDescN64(size64x64_tex64x64_vtx);
    PSMTXConcat(camera->view, tmp, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorOp(0, 0, 0, 0, 0, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xC);
    GXSetTevAlphaIn(0, 7, 5, 3, 7);
    effSetVtxDescN64(size128x128_tex32x32_vtx);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
    tri2(0, 1, 4, 0, 1, 2, 4, 0);
    tri2(0, 4, 3, 0, 4, 2, 3, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 0, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xC);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(sc, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(sc, GX_TEXMTX0, GX_MTX2x4);
    if (base == 3) {
        effGetTexObjN64(0xB, &tex);
        GXLoadTexObj(&tex, GX_TEXMAP0);
    } else {
        effGetTexObjN64(0xA, &tex);
        GXLoadTexObj(&tex, GX_TEXMAP0);
    }
    w->idx++;
    if (w->idx >= 8) {
        w->idx = 0;
    }
    if (w->bounce <= 1.0f) {
        k = w->idx + 5;
        PSMTXTrans(mtx, w->position.x, w->position.y, w->position.z);
        PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXScale(sc, scale, scale, scale);
        PSMTXConcat(mtx, sc, mtx);
        PSMTXRotRad(rot, 'z', 0.017453292f * w->rotZ);
        PSMTXConcat(mtx, rot, w->mtx[w->idx]);
        for (i = 0; i < 5; i++) {
            memcpy(&w->vtx[i * 2], &shooting_dust_vtx[i * 2], sizeof(effN64Vtx));
            OSInitFastCast();
            OSs16tof32(&w->vtx[i * 2].x, &v0.x);
            OSs16tof32(&w->vtx[i * 2].y, &v0.y);
            OSs16tof32(&w->vtx[i * 2].z, &v0.z);
            PSMTXMultVec(w->mtx[(k - i + 8) % 8], &v0, &v0);
            OSInitFastCast();
            OSf32tos16(&v0.x, &w->vtx[i * 2].x);
            OSf32tos16(&v0.y, &w->vtx[i * 2].y);
            OSf32tos16(&v0.z, &w->vtx[i * 2].z);
            memcpy(&w->vtx[i * 2 + 1], &shooting_dust_vtx[i * 2 + 1], sizeof(effN64Vtx));
            OSInitFastCast();
            OSs16tof32((s16*)&w->vtx[i * 2 + 1], &v1.x);
            OSs16tof32((s16*)&w->vtx[i * 2 + 1] + 1, &v1.y);
            OSs16tof32((s16*)&w->vtx[i * 2 + 1] + 2, &v1.z);
            PSMTXMultVec(w->mtx[(k - i + 8) % 8], &v1, &v1);
            OSInitFastCast();
            OSf32tos16(&v1.x, (s16*)&w->vtx[i * 2 + 1]);
            OSf32tos16(&v1.y, &w->vtx[i * 2 + 1].y);
            OSf32tos16(&v1.z, &w->vtx[i * 2 + 1].z);
        }
        GXInvalidateVtxCache();
        DCFlushRange(w->vtx, 0x8C);
        effSetVtxDescN64(w->vtx);
        GXLoadPosMtxImm(camera->view, GX_PNMTX0);
        for (c1 = 0; c1 < 4; c1++) {
            k = c1 * 2;
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(k, k + 1, k + 2, k, k + 1, k + 3, k + 2, k + 1);
        }
    }
}
