#include "eff/n64/eff_hit_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>

typedef struct EffHitN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 vel10;    // 0x10
    f32 vel14;    // 0x14
    f32 pad18;    // 0x18
    f32 rotZ;     // 0x1C
    f32 pad20;    // 0x20
    f32 rotY;     // 0x24
    f32 scale;    // 0x28
    s32 alpha;    // 0x2C
    s32 lifetime; // 0x30
    s32 frame;    // 0x34
    f32 progress; // 0x38
    u8 color[6];  // 0x3C
    u8 pad_42[2]; // 0x42
} EffHitN64Work;

static effN64Vtx size32x32_tex32x32_vtx[] = {
    {-16, -16, 0, 0, 0, 120, 120, 0, 255},
    {16, -16, 0, 1024, 0, 120, 120, 0, 255},
    {16, 16, 0, 1024, 1024, 120, 120, 0, 255},
    {-16, 16, 0, 0, 1024, 120, 120, 0, 255},
    {-16, -16, 0, 0, 1024, 120, 120, 0, 255},
    {16, -16, 0, 1024, 1024, 120, 120, 0, 255},
    {16, 16, 0, 1024, 2048, 120, 120, 0, 255},
    {-16, 16, 0, 0, 2048, 120, 120, 0, 255},
    {-16, -16, 0, 0, 2048, 120, 120, 0, 255},
    {16, -16, 0, 1024, 2048, 120, 120, 0, 255},
    {16, 16, 0, 1024, 3072, 120, 120, 0, 255},
    {-16, 16, 0, 0, 3072, 120, 120, 0, 255},
    {-16, -16, 0, 0, 3072, 120, 120, 0, 255},
    {16, -16, 0, 1024, 3072, 120, 120, 0, 255},
    {16, 16, 0, 1024, 4096, 120, 120, 0, 255},
    {-16, 16, 0, 0, 4096, 120, 120, 0, 255},
    {-16, -16, 0, 0, 4096, 120, 120, 0, 255},
    {16, -16, 0, 1024, 4096, 120, 120, 0, 255},
    {16, 16, 0, 1024, 5120, 120, 120, 0, 255},
    {-16, 16, 0, 0, 5120, 120, 120, 0, 255},
};

static u8 cnt_table[4] = {13, 13, 13, 13};
static u8 anim_data[13] = {0x40, 0x68, 0x90, 0xA4, 0xB8, 0xC0, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF};
static u8 radius_data[13] = {10, 20, 28, 35, 42, 48, 53, 57, 60, 62, 63, 64, 65};
static u8 fire_data[13] = {255, 255, 200, 130, 60};

void effHitMain(EffectEntry* effect);
void effHitDisp(CameraId cameraId, EffectEntry* effect);
static void main_dl(EffectEntry* effect, Mtx mtx);

EffectEntry* effHitN64Entry(s32 type, s32 size, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffHitN64Work* work;
    EffHitN64Work* p;
    s32 i;
    s32 count;
    f32 r;

    effect = effEntry();
    count = (size * 5 + 20) / 3 + 1;
    effect->type = "HitN64";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, count * sizeof(EffHitN64Work));
    effect->userdata = work;
    effect->callback = effHitMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->lifetime = cnt_table[type];
    work->frame = 0;
    work->progress = 0.0f;
    switch (type) {
    case 0:
        work->color[0] = 255;
        work->color[1] = 255;
        work->color[2] = 255;
        work->color[3] = 255;
        work->color[4] = 255;
        work->color[5] = 255;
        break;
    case 1:
        work->color[0] = 255;
        work->color[1] = 255;
        work->color[2] = 0;
        work->color[3] = 255;
        work->color[4] = 0;
        work->color[5] = 0;
        break;
    case 2:
        work->color[0] = 255;
        work->color[1] = 255;
        work->color[2] = 0;
        work->color[3] = 200;
        work->color[4] = 255;
        work->color[5] = 230;
        break;
    case 3:
    default:
        work->color[0] = 0;
        work->color[1] = 255;
        work->color[2] = 255;
        work->color[3] = 0;
        work->color[4] = 0;
        work->color[5] = 230;
        break;
    }
    work->scale = scale;
    p = work + 1;
    for (i = 1; i < count; i++, p++) {
        r = rand() % 360;
        p->position.x = 0.0f;
        p->position.y = 0.0f;
        p->position.z = 0.0f;
        p->vel10 = 0.0f;
        p->vel14 = 0.0f;
        p->pad20 = 0.0f;
        p->type = 0;
        p->rotZ = (f32)(i * 360 / (count - 1));
        p->rotY = r;
        p->vel10 = 0.0f;
        p->alpha = 255;
    }
    return effect;
}

void effHitMain(EffectEntry* effect) {
    EffHitN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 idx;
    s32 frame;
    s32 type;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    type = work->type;
    work->lifetime--;
    work->frame++;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    if (frame <= cnt_table[type]) {
        work->progress = anim_data[frame - 1] / 256.0f;
    } else {
        work->progress = 1.0f;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (frame <= cnt_table[type]) {
            idx = frame - 1;
        } else {
            idx = 12;
        }
        work->position.x = radius_data[idx];
        work->position.y = 0.0f;
        work->position.z = 0.0f;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effHitDisp, effect);
}

void effHitDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffHitN64Work* work;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scaleM;
    Mtx rotY;
    Mtx rot2;
    Mtx rot;
    f32 prog;
    f32 scale;
    s32 idx;
    s32 frac;
    s32 alpha;
    s32 i;
    s32 frame;
    s32 type;
    s32 t;
    s32 r0, g0, b0, r1, g1, b1;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    prog = 5.0f * work->progress;
    frame = work->frame;
    type = work->type;
    idx = (s32)prog;
    if (idx > 4) {
        return;
    }
    scale = work->scale;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rotY, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scaleM, scale, scale, scale);
    PSMTXConcat(mtx, rotY, mtx);
    PSMTXConcat(mtx, scaleM, rot2);
    alpha = work->alpha;
    frac = (s32)(256.0f * (prog - (f32)idx));
    switch (type) {
    case 1:
        if (frame - 1 < 13) {
            t = fire_data[frame - 1];
        } else {
            t = 0;
        }
        r0 = work->color[0];
        g0 = (work->color[1] * t) >> 8;
        b0 = (work->color[2] * t) >> 8;
        r1 = (work->color[3] * t) >> 8;
        g1 = (work->color[4] * t) >> 8;
        b1 = (work->color[5] * t) >> 8;
        break;
    case 2:
        if (frame & 1) {
            r0 = work->color[0];
            g0 = work->color[1];
            b0 = work->color[2];
            r1 = work->color[3];
            g1 = work->color[4];
            b1 = work->color[5];
        } else {
            r0 = 0;
            g0 = 0;
            b0 = 0;
            r1 = work->color[0];
            g1 = work->color[1];
            b1 = work->color[2];
        }
        break;
    case 0:
    default:
        r0 = work->color[0];
        g0 = work->color[1];
        b0 = work->color[2];
        r1 = work->color[3];
        g1 = work->color[4];
        b1 = work->color[5];
        break;
    }
    GXSetNumChans(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C1, GX_CC_C0, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A1, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, (GXColor){r0, g0, b0, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){r1, g1, b1, frac});
    effGetTexObjN64(0x4A, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    GXSetCullMode(GX_CULL_NONE);
    for (i = 0; i < 3; i++) {
        Vec axis = {1.0f, 1.0f, 1.0f};
        PSMTXRotAxisRad(rot, &axis, 0.017453292f * (f32)(i * 120));
        PSMTXConcat(rot2, rot, rot);
        main_dl(effect, rot);
    }
}

static void main_dl(EffectEntry* effect, Mtx mtx) {
    Mtx texTrans;
    Mtx texMtx;
    Mtx trans;
    Mtx rotZ;
    Mtx rotY;
    EffHitN64Work* work = effect->userdata;
    effN64Vtx* vtx;
    s32 idx;
    EffHitN64Work* p;
    s32 flag;
    s32 i;
    f32 f;

    f = 5.0f * work->progress;
    idx = (s32)f;
    if (f >= 4.0f) {
        flag = TRUE;
    } else {
        flag = FALSE;
    }
    PSMTXScale(texMtx, 0.03125f, 0.0078125f, 0.0f);
    PSMTXTrans(texTrans, 0.0f, 96.0f, 0.0f);
    PSMTXConcat(texMtx, texTrans, texMtx);
    GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
    PSMTXScale(texMtx, 0.03125f, 0.0078125f, 0.0f);
    GXLoadTexMtxImm(texMtx, GX_TEXMTX1, GX_MTX2x4);
    if (flag) {
        PSMTXScale(texMtx, 0.0f, 0.0f, 0.0f);
        GXLoadTexMtxImm(texMtx, GX_TEXMTX1, GX_MTX2x4);
    }
    vtx = size32x32_tex32x32_vtx + idx * 4;
    p = work + 1;
    for (i = 1; i < effect->count; i++, p++) {
        PSMTXRotRad(rotY, 'y', 0.017453292f * p->rotY);
        PSMTXRotRad(rotZ, 'z', 0.017453292f * p->rotZ);
        PSMTXTrans(trans, p->position.x, p->position.y, p->position.z);
        PSMTXConcat(rotY, rotZ, rotZ);
        PSMTXConcat(rotZ, trans, trans);
        PSMTXConcat(mtx, trans, trans);
        PSMTXConcat(camGetPtr(CAMERA_3D)->view, trans, trans);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        effSetVtxDescN64(vtx);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
