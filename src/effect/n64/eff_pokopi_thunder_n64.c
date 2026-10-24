#include "eff/n64/eff_pokopi_thunder_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "dolphin/os.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include <math.h>
typedef struct effN64Vtx {
    s16 x, y, z;
    s16 s, t;
    u8 r, g, b, a;
} effN64Vtx;
void effGetTexObjN64(u32 id, GXTexObj* obj);
void effSetVtxDescN64(effN64Vtx* vtx);
void tri2();

u8 yuragi_data[11][24] = {
5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 10, 9, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0,
5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0,
35, 5, 25, 5, 0, 25, 35, 5, 15, 0, 0, 0, 10, 13, 15, 12, 11, 13, 10, 12, 5, 1, 0, 0,
45, 15, 35, 15, 0, 35, 45, 55, 25, 5, 2, 0, 13, 10, 12, 10, 13, 10, 13, 10, 12, 9, 5, 1,
15, 15, 15, 15, 15, 15, 0, 15, 15, 5, 2, 0, 14, 10, 15, 10, 13, 30, 32, 30, 24, 50, 40, 60,
1, 5, 15, 15, 35, 45, 40, 35, 25, 5, 2, 0, 4, 6, 10, 5, 8, 14, 6, 10, 14, 35, 50, 50,
1, 15, 25, 15, 25, 35, 45, 40, 35, 15, 5, 0, 2, 2, 23, 12, 8, 15, 15, 5, 6, 18, 38, 60,
30, 25, 15, 5, 15, 15, 25, 30, 35, 25, 5, 0, 3, 3, 3, 11, 3, 12, 4, 5, 2, 12, 42, 50,
33, 33, 20, 12, 25, 17, 15, 20, 22, 20, 5, 0, 3, 3, 3, 2, 10, 2, 2, 12, 2, 2, 2, 20,
30, 30, 25, 25, 35, 20, 15, 10, 15, 15, 5, 0, 2, 2, 2, 2, 2, 2, 6, 2, 2, 2, 6, 5,
27, 27, 22, 22, 32, 17, 12, 7, 12, 12, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0
};
u8 yuragi_data2[11][24] = {
25, 25, 25, 5, 25, 5, 25, 25, 5, 25, 25, 25, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0,
15, 15, 15, 25, 15, 15, 5, 25, 25, 15, 25, 25, 4, 4, 4, 4, 4, 3, 2, 1, 0, 0, 0, 0,
25, 25, 25, 5, 15, 25, 5, 5, 15, 5, 5, 0, 4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 0, 0,
25, 25, 25, 5, 25, 5, 25, 25, 5, 15, 5, 0, 4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 6, 5,
15, 15, 15, 45, 35, 45, 35, 35, 25, 35, 15, 0, 4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 6, 5,
45, 55, 35, 25, 55, 15, 45, 25, 45, 35, 15, 0, 4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 6, 5,
85, 25, 15, 75, 25, 65, 35, 45, 75, 85, 15, 0, 4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 6, 5,
85, 15, 35, 55, 65, 45, 25, 75, 35, 45, 35, 40, 4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 6, 5,
85, 15, 35, 55, 65, 45, 25, 75, 35, 45, 35, 40, 4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 6, 5,
85, 15, 35, 55, 65, 45, 25, 75, 35, 45, 35, 40, 4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 6, 5,
30, 30, 25, 25, 35, 20, 15, 10, 15, 15, 5, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 0
};

static u8 (*yuragi_data_table[2])[24] = {yuragi_data, yuragi_data2};



void effPokopiThunderMain(EffectEntry* effect);
void effPokopiThunderDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effPokopiThunderN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 x2, f32 y2, f32 z2, f32 scale) {
    EffectEntry* effect;
    EffPokopiThunderN64Work* work;
    s32 cam;
    f32 len;
    f32 dx;
    f32 nv;
    f32 dy;
    f32 dz;

    effect = effEntry();
    cam = 4;
    if (type >= 10) {
        cam = 8;
    }
    type = type % 10;
    effect->type = "PokopiThunderN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffPokopiThunderN64Work));
    effect->userdata = work;
    effect->callback = effPokopiThunderMain;
    effect->flags |= 2;
    work->type = type;
    work->frame = 0;
    if (duration <= 0) {
        work->unk28 = 1000;
    } else {
        work->unk28 = duration;
    }
    work->alpha = 255;
    dy = y - y2;
    dx = x - x2;
    dz = z - z2;
    len = dx * dx + dy * dy + dz * dz;
    work->start.x = x;
    work->start.y = y;
    work->start.z = z;
    work->end.x = x2;
    work->end.y = y2;
    work->end.z = z2;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    nv = 0.0f;
    work->scale = scale;
    work->cameraId = cam;
    if (len != nv) {
        work->length = sqrtf(len);
    } else {
        return NULL;
    }
    work->length = 0.005f * work->length;
    work->r = 255;
    work->g = 220;
    work->b = 20;
    work->r2 = 255;
    work->g2 = 255;
    work->b2 = 255;
    work->seed = rand() % 360;
    return effect;
}

inline float inline_fn(s32 arg0, float arg1) { return ((f32)arg0) * arg1; }

void effPokopiThunderMain(EffectEntry* effect) {
    EffPokopiThunderN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 type;
    s32 frame;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    type = work->type;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->unk28 = 16;
    }
    if (work->unk28 < 1000) {
        work->unk28--;
    }
    work->frame++;
    if (work->unk28 < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    if (type == 2) {
        if (frame >= 40) {
            work->alpha = (s32)inline_fn(work->alpha, 0.5f);
        }
    } else if (frame >= 11) {
        work->alpha = (s32)inline_fn(work->alpha, 0.5f);
    }
    dispEntry(work->cameraId, dispCalcZ(&position), 2, (DispCallback)effPokopiThunderDisp, effect);
}

inline float pt_dy(EffPokopiThunderN64Work* a, EffPokopiThunderN64Work* b) { return a->end.y - b->start.y; }

void effPokopiThunderDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    s32 alpha;
    f32 scale;
    s32 idx;
    f32 zero;
    s32 frame;
    EffPokopiThunderN64Work* work;
    s32 type;
    s32 i;
    s32 n;
    effN64Vtx* vtx;
    effN64Vtx* v;
    u8* data;
    s32 u;

    GXTexObj texObj;
    Mtx mtx;
    f32 proj[7];
    f32 vp[6];

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    zero = 0.0f;
    frame = work->frame;
    alpha = work->alpha;
    type = work->type;
    scale = work->scale;
    if (work->cameraId != 4) {
        camera = camGetPtr(CAMERA_3D);
        GXGetProjectionv(proj);
        GXGetViewportv(vp);
        GXSetProjection(camera->projection, camera->projType);
    }
    PSMTXScale(mtx, 0.1f, 0.1f, 0.1f);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->r2, work->g2, work->b2, 128});
    GXSetNumChans(0);
    GXSetNumTevStages(4);
    GXSetTevOrder(0, 0, 0, 0xff);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 3);
    GXSetTevColorIn(0, 0xf, 8, 8, 0xf);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevOrder(1, 0xff, 0xff, 0xff);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 2, 4, 0, 0xf);
    GXSetTevAlphaIn(1, 3, 7, 2, 7);
    GXSetTevOrder(2, 1, 1, 0xff);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 0xf, 0xf, 0xf, 0);
    GXSetTevAlphaIn(2, 7, 3, 4, 0);
    GXSetTevOrder(3, 0xff, 0xff, 0xff);
    GXSetTevColorOp(3, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(3, 0, 0, 0, 1, 0);
    GXSetTevColorIn(3, 0xf, 0xf, 0xf, 0);
    GXSetTevAlphaIn(3, 7, 1, 0, 7);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7d);
    PSMTXScale(mtx, 0.0078125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(mtx, 0x1e, 1);
    GXLoadTexMtxImm(mtx, 0x21, 1);
    effGetTexObjN64(0x54, &texObj);
    GXLoadTexObj(&texObj, 0);
    GXLoadTexObj(&texObj, 1);
    GXSetCullMode(0);
    idx = frame - 1;
    if (type != 2) {
        if (idx < 11) {
            data = yuragi_data_table[type][idx];
        } else {
            data = yuragi_data_table[type][10];
        }
    } else if (idx < 8) {
        data = yuragi_data_table[0][idx];
    } else if (idx < 36) {
        data = yuragi_data_table[0][idx % 6 + 2];
    } else {
        data = yuragi_data_table[0][10];
    }
    for (i = 0; i < 12; i++) {
        f32 w, dx, dy, a, s, c, off;
        w = (f32)data[i];
        dx = 0.09090909f * (work->end.x - work->start.x);
        dy = 0.09090909f * pt_dy(work, work);
        a = 6.2831855f * (f32)((work->seed + (i - frame) * 10) % 120 - 60) / 360.0f;
        s = (f32)sin(a);
        off = 0.04f * (w * s);
        s = dy * off;
        c = (f32)cos(a);
        c = 0.04f * (w * c);
        work->x[i] = (dx * c - s) + (0.09090909f * ((f32)i * (work->end.x - work->start.x)) + work->start.x);
        work->y[i] = (dx * off + dy * c) + (0.09090909f * ((f32)i * pt_dy(work, work)) + work->start.y);
        work->z[i] = zero + (0.09090909f * ((f32)i * (work->end.z - work->start.z)) + work->start.z);
        work->w[i] = 0.1f * (f32)data[i + 12];
    }
    vtx = (effN64Vtx*)smartAlloc(sizeof(effN64Vtx) * 48, 3)->address;
    v = vtx;
    for (i = 0; i < 12; i++) {
        f32 ox, a, oy, r, ang2;
        if (i == 0) {
            r = 8.0f;
            a = -angleABf(work->y[i + 1], -work->x[i + 1], work->y[i], -work->x[i]);
        } else {
            r = 8.0f;
            if (i == 11) {
                a = -90.0f;
            } else {
                f32 d;
                ang2 = -angleABf(work->y[i + 1], -work->x[i + 1], work->y[i], -work->x[i]);
                d = -angleABf(work->y[i], -work->x[i], work->y[i - 1], -work->x[i - 1]);
                if (d - ang2 > 180.0f) {
                    ang2 += 360.0f;
                } else if (d - ang2 < -180.0f) {
                    d += 360.0f;
                }
                a = ang2 + d;
                a /= 2.0f;
            }
        }
        r = r * (scale * work->w[i]);
        ang2 = 6.2831855f * a / 360.0f;
        u = (0x80 - i * 12) << 5;
        ox = r * (f32)sin(ang2);
        oy = r * (f32)cos(ang2);
        v->x = (s16)(10.0f * (work->x[i] + ox));
        v->y = (s16)(10.0f * (work->y[i] + oy));
        v->z = (s16)(10.0f * (work->z[i] + zero));
        v->s = u;
        v->t = 0;
        v->r = i * 50;
        v->g = i * 120;
        v->b = i * 30;
        v[1].x = (s16)(10.0f * (work->x[i] - ox));
        v[1].y = (s16)(10.0f * (work->y[i] - oy));
        v[1].z = (s16)(10.0f * (work->z[i] + zero));
        v[1].s = u;
        v[1].t = 0x400;
        v[1].r = i * 50;
        v[1].g = i * 120;
        v[1].b = i * 30;
        v += 2;
    }
    DCFlushRange(vtx, 0x2a0);
    GXInvalidateVtxCache();
    effSetVtxDescN64(vtx);
    for (idx = 0; idx < i - 1; idx++) {
        n = idx * 2;
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(n, n + 2, n + 1, 2, n + 1, n + 2, n + 3, 0);
    }
    if (work->cameraId != 4) {
        GXSetProjectionv(proj);
        GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
    }
}

