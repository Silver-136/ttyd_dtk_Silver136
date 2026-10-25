#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/n64/eff_glass_n64.h"
#include "eff/n64/eff_vapor_n64.h"
#include "eff/n64/eff_snow_dust_n64.h"

typedef struct EffFreezeN64Work {
    s32 type;           // 0x0
    Vec position;       // 0x4
    s32 lifetime;       // 0x10
    s32 frame;          // 0x14
    GXColor color;      // 0x18
    GXColor color2;     // 0x1C
    f32 scale;          // 0x20
    s8 texIdx;          // 0x24
    s8 timer;           // 0x25
    EffectEntry* dust;  // 0x28
} EffFreezeN64Work;



static effN64Vtx size32x64_tex32x64_vtx[] = {
    {-16, 0, 3, 0, 0, 0, 0, 0, 255},
    {16, 0, 3, 1024, 0, 0, 0, 0, 255},
    {16, 64, 3, 1024, 2048, 0, 0, 0, 255},
    {-16, 64, 3, 0, 2048, 0, 0, 0, 255},
    {-16, 0, 3, 0, 2048, 0, 0, 0, 255},
    {16, 0, 3, 1024, 2048, 0, 0, 0, 255},
    {16, 64, 3, 1024, 4096, 0, 0, 0, 255},
    {-16, 64, 3, 0, 4096, 0, 0, 0, 255},
    {-16, 0, 3, 0, 4096, 0, 0, 0, 255},
    {16, 0, 3, 1024, 4096, 0, 0, 0, 255},
    {16, 64, 3, 1024, 6144, 0, 0, 0, 255},
    {-16, 64, 3, 0, 6144, 0, 0, 0, 255},
    {-16, 0, 3, 0, 6144, 0, 0, 0, 255},
    {16, 0, 3, 1024, 6144, 0, 0, 0, 255},
    {16, 64, 3, 1024, 8192, 0, 0, 0, 255},
    {-16, 64, 3, 0, 8192, 0, 0, 0, 255},
};

void effFreezeMain(EffectEntry* effect);
void effFreezeDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effFreezeN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffFreezeN64Work* work;

    effect = effEntry();
    effect->type = "FreezeN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffFreezeN64Work));
    effect->userdata = work;
    effect->callback = effFreezeMain;
    effect->flags |= 2;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->color.a = 255;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->color.r = 255;
    work->color.g = 255;
    work->color.b = 255;
    work->color2.r = 240;
    work->color2.g = 255;
    work->color2.b = 255;
    work->color2.a = 255;
    work->timer = 0;
    work->texIdx = 0;
    work->dust = effSnowDustN64Entry(1, 4, 0, x, 10.0f + y, z, 32.0f, 30.0f, scale);
    return effect;
}

static inline f32 freezeY(EffFreezeN64Work* a, EffFreezeN64Work* b) {
    return a->scale * (f32)(rand() % 40) + b->position.y;
}

void effFreezeMain(EffectEntry* effect) {
    s32 lifetime;
    EffFreezeN64Work* work = effect->userdata;
    s32 i;
    s32 frame;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->lifetime = 16;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    lifetime = work->lifetime;
    if (lifetime < 0) {
        for (i = 0; i < 10; i++) {
            EffectEntry* glass;
            glass = effGlassN64Entry(i % 2 + 2, i + 30, work->position.x + (f32)(rand() % 20) - 10.0f,
                                     work->scale * (f32)(rand() % 40) + work->position.y, 3.0f + work->position.z,
                                     work->scale * (0.1f * (0.7f * (rand() % 10)) + 0.3f));

            ((EffGlassN64Work*)glass->userdata)->texV = 0.1f * (f32)(rand() % 10);
            ((EffGlassN64Work*)glass->userdata)->texVSpeed = 0.01f * (f32)(rand() % 30) + 0.1f;
            ((EffGlassN64Work*)glass->userdata)->rotZ = rand() % 360;
            ((EffGlassN64Work*)glass->userdata)->rotY = rand() % 20;
            ((EffGlassN64Work*)glass->userdata)->vx = rand() % 10 - 5;
            ((EffGlassN64Work*)glass->userdata)->vy = rand() % 10 - 5;
            ((EffGlassN64Work*)glass->userdata)->vz = 0.0f;
            ((EffGlassN64Work*)glass->userdata)->gravity = -0.1f;
        }
        effDelete(work->dust);
        effDelete(effect);
        return;
    }
    frame = work->frame;
    work->timer--;
    if (work->timer < 0) {
        work->timer = rand() % 60 + 8;
    }
    if (work->timer < 8) {
        work->texIdx = work->timer / 2;
    } else {
        work->texIdx = 0;
    }
    if (lifetime > 16 && frame % 16 == 0) {
        effVaporN64Entry(2, 30, work->position.x, work->position.y + 40.0f * work->scale, work->position.z,
                         work->scale);
    }
    ((EffFreezeN64Work*)work->dust->userdata)->position.x = work->position.x;
    ((EffFreezeN64Work*)work->dust->userdata)->position.y = 10.0f + work->position.y;
    ((EffFreezeN64Work*)work->dust->userdata)->position.z = work->position.z;
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effFreezeDisp, effect);
}

void effFreezeDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffFreezeN64Work* work;
    s32 alpha;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->color.a;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->color.r, work->color.g, work->color.b, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->color2.r, work->color2.g, work->color2.b, work->color2.a});
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.03125f, 0.00390625f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(138, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(&size32x64_tex32x64_vtx[work->texIdx * 4]);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
}
