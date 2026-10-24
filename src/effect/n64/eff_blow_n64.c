#include "eff/eff_n64.h"
#include "eff/n64/eff_blow_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include <math.h>

typedef struct EffBlowN64Work {
    s32 type;     // 0x0
    s32 state;    // 0x4
    Vec position; // 0x8
    Vec velocity; // 0x14
    Vec dir;      // 0x20
    Vec accel1;   // 0x2c
    Vec accel2;   // 0x38
    f32 scaleX;   // 0x44
    f32 scaleY;   // 0x48
    f32 rot;      // 0x4c
    f32 rotVel;   // 0x50
    f32 rotInit;  // 0x54
    s32 alpha;    // 0x58
    s32 life;     // 0x5c
    s32 age;      // 0x60
    s32 delay;    // 0x64
} EffBlowN64Work;

static effN64Vtx size16x16_tex32x32_vtx[] = {
    {-8, -8, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {8, -8, 0, 1024, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {8, 8, 0, 1024, 1024, 0xFF, 0xFF, 0xFF, 0xFF},
    {-8, 8, 0, 0, 1024, 0xFF, 0xFF, 0xFF, 0xFF},
};

void effBlowMain(EffectEntry* effect);
void effBlowDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effBlowN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz) {
    EffectEntry* effect;
    EffBlowN64Work* w;
    f32 px, py, pz;
    f32 inv;
    f32 zr;
    s32 i;
    Mtx mtx;
    Vec axis;

    effect = effEntry();
    zr = 0.0f;
    if (count == 0) {
        return NULL;
    }
    inv = dx * dx + dy * dy + dz * dz;
    if (inv == zr) {
        return NULL;
    }
    inv = -1.0f / sqrtf(inv);
    dx *= inv;
    dy *= inv;
    dz *= inv;
    if (dx != zr) {
        px = -dy / dx;
        py = 1.0f;
        pz = 0.0f;
    } else if (dy != zr) {
        px = 1.0f;
        py = -dx / dy;
        pz = 0.0f;
    } else {
        px = 1.0f;
        py = 0.0f;
        pz = -dx / dz;
    }
    inv = px * px + py * py + pz * pz;
    if (inv == zr) {
        return NULL;
    }
    inv = 1.0f / sqrtf(inv);
    effect->type = "BlowN64";
    effect->count = count + 1;
    px *= inv;
    py *= inv;
    pz *= inv;
    dx *= 8.0f;
    dy *= 8.0f;
    dz *= 8.0f;
    px *= 8.0f;
    py *= 8.0f;
    pz *= 8.0f;
    w = __memAlloc(HEAP_EFFECT, sizeof(EffBlowN64Work) * (count + 1));
    effect->userdata = w;
    effect->callback = effBlowMain;
    w->state = 0;
    w->type = type;
    w->position.x = x;
    w->position.y = y;
    w->position.z = z;
    w->life = 10;
    w->age = 0;
    w++;
    for (i = 1; i < count + 1; i++, w++) {
        w->position.x = 0.0f;
        w->position.y = 0.0f;
        w->position.z = 0.0f;
        {
            Vec dir = {0.0f, 0.0f, 0.0f};
            dir.x = dx;
            dir.y = dy;
            dir.z = dz;
            axis = dir;
        }
        PSMTXRotAxisRad(mtx, &axis, 0.017453292f * (f32)(i * 360 / 5));
        w->velocity.x = w->dir.x = 2.0f * dx;
        w->velocity.y = w->dir.y = 2.0f * dy;
        w->velocity.z = w->dir.z = 2.0f * dz;
        w->accel2.x = 4.0f * (0.05f * -dx);
        w->accel2.y = 4.0f * (0.05f * -dy);
        w->accel2.z = 4.0f * (0.05f * -dz);
        w->accel1.x = 4.0f * (0.1f * (mtx[0][0] * px + mtx[0][1] * py + mtx[0][2] * pz));
        w->accel1.y = 4.0f * (0.1f * (mtx[1][0] * px + mtx[1][1] * py + mtx[1][2] * pz));
        w->accel1.z = 4.0f * (0.1f * (mtx[2][0] * px + mtx[2][1] * py + mtx[2][2] * pz));
        w->scaleX = 0.5f;
        w->scaleY = 4.0f;
        w->rotVel = 20.0f;
        w->rot = w->rotInit = (f32)(rand() % 360);
        w->alpha = 255;
        w->delay = (i - 1) / 5 * 2;
        w->life = 10;
    }
    return effect;
}

void effBlowMain(EffectEntry* effect) {
    EffBlowN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 life;
    s32 i;
    s32 state;
    s32 l;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->life--;
    work->age++;
    life = work->life;
    state = work->state;
    if (life < 0 && state == 2) {
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
        work->life--;
        l = work->life;
        if (l < 0) {
            if (state == 0) {
                work->position.x = 0.0f;
                work->position.y = 0.0f;
                work->position.z = 0.0f;
                work->velocity.x = work->dir.x;
                work->velocity.y = work->dir.y;
                work->velocity.z = work->dir.z;
                work->scaleX = 0.5f;
                work->scaleY = 4.0f;
                work->rotVel = 20.0f;
                work->rot = work->rotInit;
                work->alpha = 255;
                work->life = 10;
                work->life--;
                l = work->life;
            } else {
                work->position.x = 0.0f;
                work->position.y = 0.0f;
                work->position.z = 0.0f;
                work->velocity.x = 0.0f;
                work->velocity.y = 0.0f;
                work->velocity.z = 0.0f;
                work->alpha = 0;
                work->life = 0;
            }
        }
        if (l == 5 || l == 6 || l == 4) {
            work->velocity.x += work->accel1.x;
            work->velocity.y += work->accel1.y;
            work->velocity.z += work->accel1.z;
        }
        if (l < 8) {
            work->scaleX += 8.0f * (0.03f * (3.0f - work->scaleX));
            work->scaleY += 6.0f * (0.03f * (0.5f - work->scaleY));
        }
        work->velocity.x += work->accel2.x;
        work->velocity.y += work->accel2.y;
        work->velocity.z += work->accel2.z;
        if (work->velocity.y < 0.0f) {
            work->rotVel *= 0.2f;
            work->velocity.x *= 0.225f;
            work->velocity.y *= 0.45f;
            work->velocity.z *= 0.225f;
            work->alpha *= 0.8f;
        }
        work->position.x += work->velocity.x;
        work->position.y += work->velocity.y;
        work->position.z += work->velocity.z;
        work->rot += work->rotVel;
    }
    if (life < 0) {
        work = effect->userdata;
        if (work->state == 1) {
            work->state = 2;
        }
        work->life = 10;
        work->age = 0;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effBlowDisp, effect);
}

void effBlowDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffBlowN64Work* work;
    s32 i;
    Mtx tmp;
    Mtx scale;
    Mtx rot;
    Mtx mtx;
    Mtx base;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_RASC, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    effGetTexObjN64(0x34, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x16_tex32x32_vtx);
    PSMTXTrans(base, work->position.x, work->position.y, work->position.z);
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        GXSetTevColor(GX_TEVREG0, (GXColor){0xC8, 0xFF, 0xFF, work->alpha});
        PSMTXTrans(tmp, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
        PSMTXConcat(tmp, rot, mtx);
        PSMTXScale(scale, work->scaleX, work->scaleY, work->scaleX);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rot);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(base, mtx, mtx);
        PSMTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
