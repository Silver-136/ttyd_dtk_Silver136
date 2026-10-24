#include "eff/n64/eff_rappa_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>

double sin(double);
double cos(double);

typedef struct EffRappaN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    Vec velocity; // 0x10
    f32 scale;    // 0x1C (unused)
    f32 pad20;    // 0x20
    f32 scaleOfs; // 0x24
    f32 speed;    // 0x28
    s32 alpha;    // 0x2C
    s32 lifetime; // 0x30
    s32 frame;    // 0x34
} EffRappaN64Work;

static effN64Vtx size16x16_tex32x32_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 1024, 0, 0, 0, 0, 255},
    {8, 8, 0, 1024, 1024, 0, 0, 0, 255},
    {-8, 8, 0, 0, 1024, 0, 0, 0, 255},
};

static u8 color_rotation_data[] = {
    0xFE, 0xAC, 0xAC, 0xFE, 0xAC, 0xD5, 0xFE, 0xB4, 0x9A, 0xD5, 0xB4, 0xFE,
    0xB4, 0xB4, 0xFE, 0xB4, 0xDD, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5,
    0xB4, 0xFE, 0xB4, 0xD5, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5, 0xAC,
};



void effRappaMain(EffectEntry* effect);
void effRappaDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effRappaN64Entry(s32 type, s32 num, f32 x, f32 y, f32 z, f32 pitch, f32 yaw) {
    EffectEntry* effect;
    EffRappaN64Work* work;
    s32 i;
    f32 base = 5.0f;
    f32 radP;
    f32 speed;
    f32 sinA;
    f32 rad;

    effect = effEntry();
    if (num == 0) {
        return NULL;
    }
    effect->type = "RappaN64";
    effect->count = num + 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffRappaN64Work) * (num + 1));
    effect->userdata = work;
    effect->callback = effRappaMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->lifetime = 30;
    work->frame = 0;
    work->alpha = 255;
    work++;
    for (i = 1; i < num + 1; i++, work++) {
        speed = (f32)(rand() % 10) / 4.0f + base;
        work->type = type;
        work->position.x = x;
        work->position.y = y;
        work->position.z = z;
        radP = 6.2831855f * pitch / 360.0f;
        sinA = sin(radP);
        rad = 6.2831855f * (yaw + (f32)((i & 2) * (i / 2) * 4)) / 360.0f;
        { f32 c = cos(rad); work->velocity.x = c * sinA; }
        work->velocity.y = (f32)sin(rad);
        work->velocity.z = (f32)cos(rad) * (f32)cos(radP);
        work->speed = speed;
        work->scaleOfs = speed / 10.0f;
    }
    return effect;
}

void effRappaMain(EffectEntry* effect) {
    EffRappaN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 life;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->lifetime--;
    work->frame++;
    life = work->lifetime;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    if (life < 10) {
        work->alpha *= 0.8f;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        work->position.x += work->speed * work->velocity.x;
        work->position.y += work->speed * work->velocity.y;
        work->position.z += work->speed * work->velocity.z;
        work->speed *= 0.9f;
        if (life < 10) {
            work->scaleOfs += work->speed / 10.0f;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effRappaDisp, effect);
}

static inline GXColor mkcol(u8 r, u8 g, u8 b, u8 a) {
    GXColor c = {0};
    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
    return c;
}

void effRappaDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    s32 j0, j1, j2;
    EffRappaN64Work* work;
    s32 i;
    s32 alpha;
    s32 idx;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    idx = (work->frame - 1) * 3; idx %= 36;
    alpha = work->alpha;
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.0625f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x1D, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x16_tex32x32_vtx);
    work++;
    j0 = idx + 3;
    j1 = idx + 4;
    j2 = idx + 5;
    for (i = 1; i < effect->count; j0 += 3, j1 += 3, j2 += 3, i++, work++) {
        do { GXSetTevColor(GX_TEVREG0, mkcol(color_rotation_data[j0 % 36], color_rotation_data[j1 % 36], color_rotation_data[j2 % 36], alpha));
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
        PSMTXScale(scale, work->scaleOfs, work->scaleOfs, 1.0f); } while (0);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
