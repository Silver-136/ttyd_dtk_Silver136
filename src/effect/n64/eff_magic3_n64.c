#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/n64/eff_magic3_n64.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include <math.h>

typedef struct EffMagic3N64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    Vec velocity; // 0x10
    f32 rot;      // 0x1C
    f32 scale;    // 0x20
    s32 alpha;    // 0x24
    s32 life;     // 0x28
    s32 age;      // 0x2C
} EffMagic3N64Work;

void effMagic3Main(EffectEntry* effect);
void effMagic3Disp(CameraId cameraId, EffectEntry* effect);

#pragma opt_propagation off
EffectEntry* effMagic3N64Entry(s32 type, f32 x, f32 y, f32 z, s32 life) {
    s32 j;
    EffectEntry* effect;
    EffMagic3N64Work* w;
    f32 nx, ny, nz;
    f32 px, py, pz;
    f32 inv;
    s32 i;
    f32 rad;
    Mtx mtx;
    Vec axis;

    effect = effEntry();
    nz = 0.0f;
    ny = -1.0f;
    nx = 0.0f;
    inv = nz + (nx * nx + 1.0f);
    if (!inv) {
        return NULL;
    }
    inv = -1.0f / sqrtf(inv);
    nx *= inv;
    ny *= inv;
    nz *= inv;
    if (nx) {
        px = -ny / nx;
        py = 1.0f;
        pz = 0.0f;
    } else if (ny) {
        px = 1.0f;
        py = -nx / ny;
        pz = 0.0f;
    } else {
        px = 1.0f;
        py = 0.0f;
        pz = -nx / nz;
    }
    inv = px * px + py * py + pz * pz;
    if (!inv) {
        return NULL;
    }
    if (inv > 0.0f) {
        inv = 1.0f / sqrtf(inv);
    }
    effect->type = "Magic3N64";
    effect->count = 5;
    nx *= 18.0f;
    ny *= 18.0f;
    nz *= 18.0f;
    w = __memAlloc(HEAP_EFFECT, sizeof(EffMagic3N64Work) * 5);
    effect->userdata = w;
    effect->callback = effMagic3Main;
    for (i = 0, j = 0; i < 5; i++, w++, j += 100) {
        rad = 6.2831855f * camGetPtr(CAMERA_3D)->viewYaw / 360.0f;
        {
            Vec dir = {0.0f, 0.0f, 0.0f};
            dir.x = (f32)sin(rad);
            dir.z = -(f32)cos(rad);
            axis = dir;
        }
        PSMTXRotAxisRad(mtx, &axis, 0.017453292f * (i == 1 ? 0.0f : 100.0f * (f32)i / 4.0f - 50.0f));
        w->type = type;
        w->position.x = x;
        w->position.y = y;
        w->position.z = z;
        w->velocity.x = 0.6f * (mtx[0][0] * nx + mtx[0][1] * ny + mtx[0][2] * nz);
        w->velocity.y = mtx[1][0] * nx + mtx[1][1] * ny + mtx[1][2] * nz;
        w->velocity.z = 0.6f * (mtx[2][0] * nx + mtx[2][1] * ny + mtx[2][2] * nz);
        w->rot = -(j / 4 - 50);
        w->scale = 0.5f;
        w->life = life;
        w->age = 0;
        w->alpha = 0;
    }
    return effect;
}

#pragma opt_propagation reset
inline f32 magic3_scale(f32 a, f32 b) { return 3.0f * a + b; }

void effMagic3Main(EffectEntry* effect) {
    s32 life;
    EffMagic3N64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    f32 t;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->life--;
    work->age++;
    life = work->life;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    t = 6.2831855f * (f32)(work->age * 30);
    for (i = 0; i < effect->count; i++, work++) {
        if (life > 5) {
            work->velocity.x *= 0.8f;
            work->velocity.y *= 0.8f;
            work->velocity.z *= 0.8f;
            work->alpha = (s32)(0.4f * (f32)(255 - work->alpha) + (f32)work->alpha);
            work->scale = (f32)sin(t / 360.0f) + 3.0f * (f32)sin(6.2831855f * (f32)(work->alpha / 2) / 360.0f);
        }
        if (life < 5) {
            work->alpha *= 0.6f;
            work->velocity.x *= 0.7f;
            work->velocity.y *= 0.7f;
            work->velocity.z *= 0.7f;
            work->scale = (f32)(255 - work->alpha) / 30.0f +
                          magic3_scale((f32)sin(6.2831855f * (f32)(work->alpha / 2) / 360.0f), (f32)sin(t / 360.0f));
        }
        work->position.x += work->velocity.x;
        work->position.y += work->velocity.y;
        work->position.z += work->velocity.z;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effMagic3Disp, effect);
}

void effMagic3Disp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffMagic3N64Work* work;
    GXTexObj texObj;
    GXColor color;
    Mtx mtx;
    Mtx scale;
    Mtx rotY;
    Mtx rotZ;
    s32 i;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    color = (GXColor){255, 255, 255, 0};
    color.a = work->alpha;
    GXSetTevColor(GX_TEVREG0, color);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.015625f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x1E, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size32x16_tex64x32_vtx);
    for (i = 0; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rotY, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
        PSMTXRotRad(rotZ, 'z', 0.017453292f * work->rot);
        PSMTXScale(scale, work->scale, work->scale, 1.0f);
        PSMTXConcat(mtx, rotY, mtx);
        PSMTXConcat(mtx, rotZ, mtx);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}

effN64Vtx size32x16_tex64x32_vtx[] = {
    {-16, -8, 0, 0, 0, 0, 0, 0, 255},
    {16, -8, 0, 2048, 0, 0, 0, 0, 255},
    {16, 8, 0, 2048, 1024, 0, 0, 0, 255},
    {-16, 8, 0, 0, 1024, 0, 0, 0, 255},
};
