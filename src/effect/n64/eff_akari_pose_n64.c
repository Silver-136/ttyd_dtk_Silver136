#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

double sin(double);
double cos(double);

typedef struct EffAkariPoseN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 vx;       // 0x10
    f32 vy;       // 0x14
    f32 targetVx; // 0x18
    f32 targetVy; // 0x1C
    s32 seq;      // 0x20
    f32 rotation; // 0x24
    s32 lifetime; // 0x28
    s32 frame;    // 0x2C
    s32 alpha;    // 0x30
    f32 scale;    // 0x34
} EffAkariPoseN64Work;

static f32 scale_data[] = {
    0.9f, 1.0f, 1.0f, 0.9f, 0.85f, 0.8f, 0.75f, 0.7f, 0.65f, 0.6f, 0.55f, 0.5f,
    0.45f, 0.4f, 0.35f, 0.3f, 0.25f, 0.2f, 0.15f, 0.1f, 0.05f, 0.03f, 0.02f, 0.01f,
};

static f32 scale_data2[] = {
    0.9f, 1.0f, 1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f, 0.4f, 0.3f, 0.2f, 0.1f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
};

static effN64Vtx size16x16_tex32x32_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 1024, 0, 0, 0, 0, 255},
    {8, 8, 0, 1024, 1024, 0, 0, 0, 255},
    {-8, 8, 0, 0, 1024, 0, 0, 0, 255},
};

static s32 max_seq_num[] = {24, 12};

static u8 a_data[] = {
    0xFA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

static u8 a_data2[] = {
    0xFA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#pragma force_active on
u8 gap_07_80412D70_sdata[16] = {0};
#pragma force_active reset

void effAkariPoseMain(EffectEntry* effect);
void effAkariPoseDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effAkariPoseN64Entry(s32 type, s32 count, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    f32 half;
    EffectEntry* effect;
    EffAkariPoseN64Work* work;
    EffAkariPoseN64Work* w;
    s32 i;

    effect = effEntry();
    effect->type = "AkariPoseN64";
    effect->count = count + 1;
    work = __memAlloc(HEAP_EFFECT, (count + 1) * sizeof(EffAkariPoseN64Work));
    effect->userdata = work;
    effect->callback = effAkariPoseMain;
    effect->flags |= 2;
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
    half = max_seq_num[type] / 2.0f;
    w = work + 1;
    for (i = 1; i < count + 1; i++, w++) {
        w->seq = -1.0f - half * (i & 1);
    }
    return effect;
}

void effAkariPoseMain(EffectEntry* effect) {
    EffAkariPoseN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;
    s32 frame;
    f32 a, b, rad, deg;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    type = work->type;
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
    frame = work->frame;
    if (work->lifetime < 16) {
        work->alpha = work->lifetime << 4;
    }
    work++;
    switch (type) {
        case 0:
            a = 0.1f;
            b = 0.2f;
            break;
        default:
            a = 0.2f;
            b = a;
            break;
    }
    for (i = 1; i < effect->count; i++, work++) {
        work->seq++;
        if (work->seq >= max_seq_num[type]) {
            work->seq = 0;
        }
        if (work->seq < 0) {
            continue;
        }
        if (work->seq == 0) {
            deg = 360.0f * i / (effect->count - 1);
            rad = 6.2831855f * deg / 360.0f;
            work->position.x = 16.0f * (f32)sin(rad);
            work->position.y = 16.0f * (f32)cos(rad);
            work->vx = work->position.x * a;
            work->vy = work->position.y * a;
            work->targetVx = 0.0f;
            work->targetVy = 0.0f;
            work->rotation = -deg - 45.0f;
            work->scale = 1.0f;
        }
        switch (type) {
            case 0:
                work->alpha = a_data[work->seq];
                work->scale = scale_data[work->seq];
                break;
            default:
                work->alpha = a_data2[work->seq];
                work->scale = scale_data2[work->seq];
                break;
        }
        if ((frame & 1) == (i & 1)) {
            work->position.x += work->vx;
            work->position.y += work->vy;
        } else {
            work->position.x += 0.2f * work->vx;
            work->position.y += 0.2f * work->vy;
        }
        work->vx += b * (work->targetVx - work->vx);
        work->vy += b * (work->targetVy - work->vy);
    }
    
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effAkariPoseDisp, effect);
}

void effAkariPoseDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* cam;
    EffAkariPoseN64Work* work;
    s32 alpha;
    GXTexObj texObj;
    s32 i;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx rotMtx;
    Mtx camMtx;

    cam = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->alpha;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rotMtx, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scaleMtx, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, rotMtx, mtx);
    PSMTXConcat(mtx, scaleMtx, mtx);
    PSMTXConcat(cam->view, mtx, camMtx);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scaleMtx, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x76, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x16_tex32x32_vtx);
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->seq < 0) {
            continue;
        }
        PSMTXTrans(mtx, work->position.x, work->position.y, 0.0f);
        PSMTXRotRad(rotMtx, 'z', work->rotation * 0.017453292f);
        PSMTXScale(scaleMtx, work->scale, work->scale, work->scale);
        PSMTXConcat(mtx, rotMtx, mtx);
        PSMTXConcat(mtx, scaleMtx, mtx);
        PSMTXConcat(camMtx, mtx, mtx);
        GXLoadPosMtxImm(mtx, 0);
        GXSetCurrentMtx(0);
        GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, (alpha * work->alpha) >> 8});
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}

