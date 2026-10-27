#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"

typedef struct EffStardustN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 vx;       // 0x10
    f32 vy;       // 0x14
    f32 ay;       // 0x18
    f32 scale;    // 0x1c
    s32 frame;    // 0x20
    s32 lifetime; // 0x24
    f32 cr;       // 0x28
    f32 cg;       // 0x2c
    f32 cb;       // 0x30
    s32 sub;      // 0x34
    s32 variant;  // 0x38
    s32 camera;   // 0x3c
} EffStardustN64Work;

static u8 color_rotation_data[] = {
    0xFE, 0xAC, 0xAC, 0xFE, 0xAC, 0xD5, 0xFE, 0xB4, 0x9A, 0xD5, 0xB4, 0xFE,
    0xB4, 0xB4, 0xFE, 0xB4, 0xDD, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5,
    0xB4, 0xFE, 0xB4, 0xD5, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5, 0xAC,
};

static u8 speed_data[] = {0x1E, 0x3C, 0x50, 0x5F, 0x64, 0x68, 0x6A};

static u8 scale_data[] = "<Zdhjlm";

static effN64Vtx size8x8_tex22x22_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 704, 0, 0, 0, 0, 255},
    {8, 8, 0, 704, 704, 0, 0, 0, 255},
    {-8, 8, 0, 0, 704, 0, 0, 0, 255},
    {-8, -8, 0, 704, 0, 0, 0, 0, 255},
    {8, -8, 0, 1408, 0, 0, 0, 0, 255},
    {8, 8, 0, 1408, 704, 0, 0, 0, 255},
    {-8, 8, 0, 704, 704, 0, 0, 0, 255},
    {-8, -8, 0, 1408, 0, 0, 0, 0, 255},
    {8, -8, 0, 2112, 0, 0, 0, 0, 255},
    {8, 8, 0, 2112, 704, 0, 0, 0, 255},
    {-8, 8, 0, 1408, 704, 0, 0, 0, 255},
    {-8, -8, 0, 2112, 0, 0, 0, 0, 255},
    {8, -8, 0, 2816, 0, 0, 0, 0, 255},
    {8, 8, 0, 2816, 704, 0, 0, 0, 255},
    {-8, 8, 0, 2112, 704, 0, 0, 0, 255},
    {-8, -8, 0, 2816, 0, 0, 0, 0, 255},
    {8, -8, 0, 3520, 0, 0, 0, 0, 255},
    {8, 8, 0, 3520, 704, 0, 0, 0, 255},
    {-8, 8, 0, 2816, 704, 0, 0, 0, 255},
    {-8, -8, 0, 3520, 0, 0, 0, 0, 255},
    {8, -8, 0, 4224, 0, 0, 0, 0, 255},
    {8, 8, 0, 4224, 704, 0, 0, 0, 255},
    {-8, 8, 0, 3520, 704, 0, 0, 0, 255},
    {-8, -8, 0, 4224, 0, 0, 0, 0, 255},
    {8, -8, 0, 4928, 0, 0, 0, 0, 255},
    {8, 8, 0, 4928, 704, 0, 0, 0, 255},
    {-8, 8, 0, 4224, 704, 0, 0, 0, 255},
    {-8, -8, 0, 4928, 0, 0, 0, 0, 255},
    {8, -8, 0, 5632, 0, 0, 0, 0, 255},
    {8, 8, 0, 5632, 704, 0, 0, 0, 255},
    {-8, 8, 0, 4928, 704, 0, 0, 0, 255},
};

void effStardustMain(EffectEntry* effect);
void effStardustDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effStardustN64Entry(s32 type, f32 x, f32 y, f32 z, f32 a) {
    s32 lifeA;
    s32 lifeB;
    s32 randA;
    s32 randB;
    EffectEntry* effect;
    EffStardustN64Work* p;
    s32 count;
    s32 i;
    EffStardustN64Work* work;
    f32 ay;
    f32 sn;
    f32 radius;
    f32 angle;
    f32 a1;
    f32 a2;
    f32 fa;
    f32 fb;

    effect = effEntry();
    switch (type) {
    case 0:
        radius = a;
        count = 36;
        break;
    case 1:
        radius = a;
        count = 4;
        break;
    case 2:
        radius = a;
        count = 8;
        break;
    case 3:
        radius = 8.0f;
        count = 4;
        break;
    default:
        count = 8;
        radius = 8.0f;
        break;
    }
    effect->type = "StardustN64";
    effect->count = count + 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffStardustN64Work) * (count + 1));
    effect->userdata = work;
    effect->callback = effStardustMain;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->frame = 0;
    work->type = type;
    work->lifetime = 30;
    work->cr = 1.0f;
    work->cg = 1.0f;
    work->cb = 1.0f;
    work->camera = CAMERA_3D;
    work->scale = 1.0f;
    // FAKE: i set before the loop setup; for (i = 1; ...) changes the object
    i = 1;
    randA = (s32)(a * 1000.0f);
    randB = (s32)(radius * 1000.0f);
    p = work + 1;
    lifeA = 0x20;
    lifeB = 0x14;
    for (; i < count + 1; lifeA += 2, lifeB += 2, i++, p++) {
        fa = (f32)(rand() % randA) / 1000.0f;
        fb = (f32)(rand() % randB) / 1000.0f;
        switch (type) {
        case 0: {
            p->ay = 0.0f;
            a1 = 6.2831855f * (1800.0f / (f32)count * (f32)i - 90.0f) / 360.0f;
            sn = (f32)sin(a1);
            a2 = 6.2831855f * (360.0f / (f32)count * (f32)i) / 360.0f;
            angle = (f32)sin(a2);
            p->vx = (angle * (85.0f - 15.0f * sn - (f32)((i & 1) * 5)) / 100.0f) * a;
            sn = (f32)sin(a1);
            angle = (f32)cos(a2);
            p->vy = (angle * (85.0f - 15.0f * sn - (f32)((i & 1) * 5)) / 100.0f) * a;
            p->position.z = 0.0f;
            p->sub = 0;
            break;
        }
        case 1:
        case 2:
            angle = 360.0f / (f32)count * (f32)i + (f32)(rand() % 360);
            if (i & 1) {
                // BUG: rand() % 50 / 1000 is an integer division and always 0, so ay is always -0.05
                ay = -(0.05f + (f32)(rand() % 50 / 1000));
            } else {
                ay = 0.0f;
            }
            p->ay = ay;
            p->vx = 0.0f;
            p->vy = 0.0f;
            angle = 6.2831855f * angle / 360.0f;
            p->position.x = (f32)sin(angle) * fa;
            p->position.y = (f32)cos(angle) * fb;
            p->position.z = 0.0f;
            p->sub = i % 3;
            p->scale = 1.0f;
            p->lifetime = lifeA;
            p->frame = 0;
            break;
        case 3:
        case 4:
        default:
            angle = 360.0f / (f32)count * (f32)i + (f32)(rand() % 360);
            // BUG: rand() % 50 / 1000 is an integer division and always 0, so ay is always -0.05
            p->ay = -(0.05f + (f32)(rand() % 50 / 1000));
            angle = 6.2831855f * angle / 360.0f;
            p->vx = 0.0f;
            p->vy = 0.0f;
            p->position.x = (f32)sin(angle) * fa;
            p->position.y = (f32)cos(angle) * fb;
            p->position.z = 0.0f;
            p->sub = i % 3;
            p->scale = 1.0f;
            p->lifetime = lifeB;
            p->frame = 0;
            break;
        }
    }
    return effect;
}

void effStardustMain(EffectEntry* effect) {
    EffStardustN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;
    s32 dead;
    s32 frame;
    s32 camera;
    s32 idx;
    f32 angle;
    f32 mag;
    f32 vx;
    f32 vy;

    dead = 0;
    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    camera = work->camera;
    work->lifetime--;
    work->frame++;
    type = work->type;
    frame = work->frame;
    if ((type == 0 || type == 10) && work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    work++;
    for (i = 0; i < effect->count - 1; i++, work++) {
        work->variant = (frame + i) & 7;
        switch (type) {
        case 0:
            work->ay += -0.02f;
            idx = frame - 1;
            if (idx < 7) {
                work->position.x = 0.01f * (work->vx * (f32)speed_data[idx]);
                work->position.y = 0.01f * (work->vy * (f32)speed_data[idx]) + work->ay;
                work->scale = 0.01f * (f32)scale_data[idx];
            } else {
                angle = (f32)(s32)effTblRandN64(360, i + 177);
                mag = 1.0f + 0.5f * (f32)(i & 3);
                angle = 6.2831855f * angle / 360.0f;
                work->type = 10;
                work->vx = mag * (f32)sin(angle);
                work->vy = mag * (f32)cos(angle);
                work->ay = -0.05f;
            }
            break;
        case 10:
            work->vx *= 0.96f;
            work->vy *= 0.96f;
            work->vy += work->ay;
            work->position.x += work->vx;
            work->position.y += work->vy;
            work->scale += 0.1f * (0.1f - work->scale);
            break;
        default:
            work->vx *= 0.9f;
            work->vy += work->ay;
            vx = work->vx;
            vy = work->vy;
            if (work->lifetime < 0) {
                dead++;
            }
            work->position.x += vx;
            work->position.y += vy;
            work->lifetime--;
            if (work->lifetime > 20 || work->lifetime < 0) {
                work->variant = -1;
            } else {
                work->frame++;
                frame = work->frame;
                idx = frame - 1;
                if (idx < 7) {
                    work->scale = 0.01f * (f32)scale_data[idx];
                } else {
                    work->scale += 0.1f * (0.1f - work->scale);
                }
            }
            break;
        }
    }
    if (type != 0 && type != 10 && dead >= effect->count - 1) {
        effDelete(effect);
    } else {
        dispEntry(camera, dispCalcZ(&position), 2, (DispCallback)effStardustDisp, effect);
    }
}

void effStardustDisp(CameraId cameraId, EffectEntry* effect) {
    EffStardustN64Work* work;
    s32 i;
    CameraEntry* camera;
    s32 variant;
    s32 idx;
    effN64Vtx* vtx;
    s32 r, g, b;
    f32 cr, cg, cb;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx rot;
    Mtx base;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    cr = work->cr;
    cg = work->cg;
    cb = work->cb;
    idx = (work->frame - 1) * 3;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXScale(scaleMtx, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, rot, base);
    PSMTXConcat(base, scaleMtx, base);
    PSMTXConcat(camera->view, base, base);
    work++;
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 0.0056818184f, 0.045454547f, 0.0f);
    GXLoadTexMtxImm(scaleMtx, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x20, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    for (i = 0; i < effect->count - 1; i++, work++) {
        variant = work->variant;
        if (variant < 0) {
            continue;
        }
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        mtx[0][0] = work->scale;
        mtx[1][1] = work->scale;
        mtx[2][2] = work->scale;
        PSMTXConcat(base, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        idx += 3;
        idx %= 36;
        r = (s32)((f32)color_rotation_data[idx++] * cr);
        g = (s32)((f32)color_rotation_data[idx++] * cg);
        b = (s32)((f32)color_rotation_data[idx++] * cb);
        if (r > 255) {
            r = 255;
        }
        if (g > 255) {
            g = 255;
        }
        if (b > 255) {
            b = 255;
        }
        GXSetTevColor(GX_TEVREG0, (GXColor){r, g, b, 255});
        switch (variant & 7) {
        case 0:
            vtx = &size8x8_tex22x22_vtx[12];
            break;
        case 1:
            vtx = &size8x8_tex22x22_vtx[16];
            break;
        case 2:
            vtx = &size8x8_tex22x22_vtx[20];
            break;
        case 3:
            vtx = &size8x8_tex22x22_vtx[24];
            break;
        case 4:
            vtx = &size8x8_tex22x22_vtx[28];
            break;
        case 5:
            vtx = &size8x8_tex22x22_vtx[0];
            break;
        case 6:
            vtx = &size8x8_tex22x22_vtx[4];
            break;
        case 7:
            vtx = &size8x8_tex22x22_vtx[8];
            break;
        }
        effSetVtxDescN64(vtx);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}

void effStardustN64SetColor(EffectEntry* effect, f32 r, f32 g, f32 b) {
    EffStardustN64Work* work = effect->userdata;
    work->cr = r;
    work->cg = g;
    work->cb = b;
}

void effStardustN64SetDrawCam(EffectEntry* effect, s32 camera) {
    EffStardustN64Work* work = effect->userdata;
    work->camera = camera;
}

void effStardustN64SetScale(EffectEntry* effect, f32 scale) {
    EffStardustN64Work* work = effect->userdata;
    work->scale = scale;
}
