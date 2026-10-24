#include "eff/n64/eff_cloud_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include <string.h>
#include "libc_decls.h"
#include <math.h>

double sin(double);
double cos(double);

void effCloudMain(EffectEntry* effect);
void effCloudDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effCloudN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffCloudN64Work* work;

    effect = effEntry();
    effect->type = "CloudN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffCloudN64Work));
    effect->userdata = work;
    effect->callback = effCloudMain;
    effect->flags |= 2;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    switch (type) {
    case 0:
        work->r = 255;
        work->g = 255;
        work->b = 255;
        work->r2 = 225;
        work->g2 = 225;
        work->b2 = 225;
        work->fade = 255;
        break;
    case 1:
        work->r = 255;
        work->g = 245;
        work->b = 230;
        work->r2 = 45;
        work->g2 = 35;
        work->b2 = 15;
        work->fade = 255;
        break;
    case 2:
        work->r = 255;
        work->g = 0;
        work->b = 0;
        work->r2 = 155;
        work->g2 = 125;
        work->b2 = 125;
        work->fade = 255;
        break;
    case 3:
        work->r = 255;
        work->g = 255;
        work->b = 255;
        work->r2 = 255;
        work->g2 = 65;
        work->b2 = 20;
        work->fade = 255;
        break;
    }
    work->pos.x = x;
    work->pos.y = y;
    work->pos.z = z;
    work->active[0] = 0;
    work->active[1] = 0;
    work->active[2] = 0;
    work->active[3] = 0;
    work->active[4] = 0;
    work->active[5] = 0;
    work->active[6] = 0;
    work->active[7] = 0;
    work->active[8] = 0;
    work->active[9] = 0;
    work->active[10] = 0;
    work->active[11] = 0;
    work->active[12] = 0;
    work->active[13] = 0;
    work->active[14] = 0;
    work->active[15] = 0;
    work->active[16] = 0;
    work->active[17] = 0;
    work->active[18] = 0;
    work->active[19] = 0;
    work->active[20] = 0;
    work->active[21] = 0;
    work->active[22] = 0;
    work->active[23] = 0;
    work->active[24] = 0;
    work->active[25] = 0;
    work->active[26] = 0;
    work->active[27] = 0;
    work->active[28] = 0;
    work->active[29] = 0;
    work->head = 0;
    work->trailX[0] = x;
    work->trailY[0] = y;
    work->trailZ[0] = z;
    work->active[0] = 1;
    work->age[0] = 0;
    work->len[0] = 0.0f;
    work->vel.x = (f32)(s32)(rand() % 30 + 10);
    work->vel.y = 0.0f;
    work->vel.z = 0.0f;
    return effect;
}

void effCloudMain(EffectEntry* effect) {
    EffCloudN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 type;
    s32 frame;
    s32 idx;
    f32 tx, ty, tz, px, py, pz;
    f32 prev, dx, dy, dz, d;

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
        work->fade = work->lifetime * 16;
    }
    if (type == 3) {
        work->vel.x += (f32)(s32)(rand() % 10 - 5);
        work->vel.y += (f32)(s32)(rand() % 10 - 5);
        work->pos.x += work->vel.x;
        work->pos.y += work->vel.y;
        work->pos.z += work->vel.z;
    }
    px = work->pos.x;
    py = work->pos.y;
    pz = work->pos.z;
    idx = work->head % 30;
    tx = work->trailX[idx];
    ty = work->trailY[idx];
    tz = work->trailZ[idx];
    if (px != tx || py != ty || pz != tz) {
        prev = work->len[idx];
        work->head++;
        if (work->head >= 30) {
            work->head = 0;
        }
        dy = py - ty;
        dx = px - tx;
        dz = pz - tz;
        idx = work->head;
        work->active[idx] = 1;
        work->trailX[idx] = px;
        work->trailY[idx] = py;
        work->trailZ[idx] = pz;
        work->age[idx] = frame;
        d = dz * dz + (dx * dx + dy * dy);
        if (d) {
            d = sqrtf(d);
        }
        work->len[idx] = prev + d;
    }
    dispEntry(4, dispCalcZ(&position), 2, (DispCallback)effCloudDisp, effect);
}

void effCloudDisp(CameraId cameraId, EffectEntry* effect) {
    EffCloudN64Work* work;
    f32 scale;
    s32 frame;
    s32 type;
    effN64Vtx* vtx;
    s32 fade;
    s32 i;
    s32 first;
    GXTexObj texObj;
    s32 uo;
    s32 alpha;
    s32 prev;
    s32 u;
    s32 next;
    CameraEntry* camera;
    s32 b;
    s32 k;
    s32 count;
    s32 g;
    Mtx mtx;
    s32 n;
    s32 j;
    effN64Vtx* v;
    s32 c;
    s32 idx;
    f32 zero, ang, a1, a2, diff, x, y, z, w, sn, cs;



    camera = camGetPtr(cameraId);
    work = effect->userdata;
    frame = work->frame;
    fade = work->fade;
    type = work->type;
    scale = work->scale;
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    if (type >= 2) {
        GXSetNumTevStages(2);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    } else {
        GXSetNumTevStages(2);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    }
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(mtx, 0.015625f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x83, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, fade});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->r2, work->g2, work->b2});
    GXLoadPosMtxImm(camera->view, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(GX_CULL_NONE);
    vtx = (effN64Vtx*)smartAlloc(0x690, 3)->address;
    memset(vtx, 0, 0x690);
    effSetVtxDescN64(vtx);
    uo = (frame & 0x3F) << 5;
    first = -1;
    zero = 0.0f;
    count = 0;
    for (i = 0; i < 30; i++) {
        if (work->active[(1 + work->head + i) % 30] != 0) {
            count++;
        }
    }
    if (count - 1 > 0) {

        n = 0;
        for (i = 0; i < 30; i++) {
            v = &vtx[i * 2];
            idx = (1 + work->head + i) % 30;
            sn = (f32)sin(6.2831855f * (f32)(n * 180 / (count - 1)) / 360.0f);
            alpha = (s32)(255.0f * sn);
            if (alpha > 200) {
                alpha = 200;
            }
            if (work->active[idx] != 0) {
                if (first == -1) {
                    next = idx + 1;
                    if (next >= 30) {
                        next = 0;
                    }
                    ang = -angleABf(work->trailY[next], -work->trailX[next], work->trailY[idx], -work->trailX[idx]);
                    first = i;
                } else if (i != 29) {
                    next = idx + 1;
                    prev = idx - 1;
                    if (next >= 30) {
                        next = 0;
                    }
                    if (prev < 0) {
                        prev = 29;
                    }
                    a1 = -angleABf(work->trailY[next], -work->trailX[next], work->trailY[idx], -work->trailX[idx]);
                    a2 = -angleABf(work->trailY[idx], -work->trailX[idx], work->trailY[prev], -work->trailX[prev]);
                    if (a2 - a1 > 180.0f) {
                        a1 += 360.0f;
                    } else if (a2 - a1 < -180.0f) {
                        a2 += 360.0f;
                    }
                    ang = a1 + a2;
                    ang *= 0.5f;
                }
                x = work->trailX[idx];
                y = work->trailY[idx];
                z = work->trailZ[idx];
                k = frame - work->age[idx];
                sn = (f32)sin(6.2831855f * (f32)((frame - work->age[idx] * 80) * 4) / 360.0f);
                w = (f32)k + (3.0f * sn + 16.0f);
                w *= scale;
                if (type == 0) {
                    u = (s32)(24.0f * work->len[idx]) + uo;
                } else {
                    u = (s32)(24.0f * work->len[idx]) + uo;
                }
                c = 255 - k * 100;
                g = 255;
                b = 255;
                if (c < 0) {
                    b = (s32)(0.8f * (f32)c) + 255;
                    c = 0;
                    if (b < 0) {
                        g = (s32)(0.4f * (f32)b) + 255;
                        b = 0;
                        if (g < 0) {
                            g = 0;
                        }
                    }
                }
                a1 = 6.2831855f * ang / 360.0f;
                sn = w * (f32)sin(a1);
                cs = w * (f32)cos(a1);
                v[0].x = (s16)(x + sn);
                v[0].y = (s16)(y + cs);
                v[0].z = (s16)(z + zero);
                v[0].s = u;
                v[0].t = 0x400;
                v[0].r = g;
                v[0].g = b;
                v[0].b = c;
                v[0].a = alpha;
                v[1].x = (s16)(x - sn);
                v[1].y = (s16)(y - cs);
                v[1].z = (s16)(z + zero);
                v[1].s = u;
                v[1].t = 0;
                v[1].r = g;
                v[1].g = b;
                v[1].b = c;
                v[1].a = alpha;
                n++;
            }
        }
        DCFlushRange(vtx, 0x690);
        GXInvalidateVtxCache();
        for (j = first, k = first * 2; j < 29; k += 2, j++) {
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            ((void (*)())tri2)(k, k + 2, k + 1, k, k + 1, k + 2, k + 3, k);
        }
    }
}
