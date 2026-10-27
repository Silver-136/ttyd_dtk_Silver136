#include "eff/eff_spark.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "driver/effdrv.h"

typedef struct EffSparkParticle {
    s32 type;   // 0x0
    f32 x;      // 0x4
    f32 y;      // 0x8
    f32 z;      // 0xC
    f32 velX;   // 0x10
    f32 velY;   // 0x14
    f32 destX;  // 0x18
    f32 destY;  // 0x1C
    s32 seq;    // 0x20
    f32 angle;  // 0x24
    s32 life;   // 0x28
    s32 counter; // 0x2C
    s32 alpha;  // 0x30
    f32 scale;  // 0x34
} EffSparkParticle;

typedef struct EffSparkWork {
    s32 type;                  // 0x0
    f32 x;                     // 0x4
    f32 y;                     // 0x8
    f32 z;                     // 0xC
    f32 unk10;                 // 0x10
    f32 unk14;                 // 0x14
    f32 unk18;                 // 0x18
    f32 unk1C;                 // 0x1C
    s32 unk20;                 // 0x20
    f32 unk24;                 // 0x24
    s32 life;                  // 0x28
    s32 counter;               // 0x2C
    s32 alpha;                 // 0x30
    f32 scale;                 // 0x34
    EffSparkParticle part[5];  // 0x38
} EffSparkWork;

static s32 max_seq_num[2] = { 24, 12 };
static u8 a_data[24] = { 0xFA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                         0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
static f32 scale_data[24] = {
    0.9f, 1.0f, 1.0f, 0.9f, 0.85f, 0.8f, 0.75f, 0.7f, 0.65f, 0.6f, 0.55f, 0.5f,
    0.45f, 0.4f, 0.35f, 0.3f, 0.25f, 0.2f, 0.15f, 0.1f, 0.05f, 0.03f, 0.02f, 0.01f,
};
static f32 scale_data2[12] = { 0.9f, 1.0f, 1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f, 0.4f, 0.3f, 0.2f, 0.1f };
static u8 a_data2[12] = { 0xFA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

EffectEntry* effSparkEntry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffSparkWork* work;
    s32 i;
    f32 f;

    effect = effEntry();
    effect->type = "Spark";
    effect->count = 6;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffSparkWork));
    effect->userdata = work;
    effect->callback = effSparkMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->counter = 0;
    work->life = 1000;
    work->alpha = 255;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = 0.9f;
    {
        EffSparkParticle* p = work->part;
        f = max_seq_num[type] / 2.0f;
        for (i = 1; i < 6; i++, p++) {
            p->seq = -1.0f - f * (i & 1);
        }
    }
    return effect;
}

void effSparkMain(EffectEntry* effect) {
    EffSparkParticle* p = effect->userdata;
    s32 i;
    s32 type;
    u8 alpha;
    s32 counter;
    s32 prio;
    Vec position;
    f32 a, b, rad, ang;

    position = (Vec){ p->x, p->y, p->z };
    type = p->type;
    alpha = p->alpha;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        p->life = 16;
    }
    if (p->life < 1000) {
        p->life--;
    }
    if (p->life < 0) {
        effDelete(effect);
        return;
    }
    if (p->life < 16) {
        p->alpha = p->life * 16;
    }
    p->counter++;
    counter = p->counter;
    p++;
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
    for (i = 1; i < effect->count; i++, p++) {
        p->seq++;
        if (p->seq >= max_seq_num[type]) {
            p->seq = 0;
        }
        if (p->seq < 0) {
            continue;
        }
        if (p->seq == 0) {
            ang = 360.0f * i / (effect->count - 1);
            rad = 6.2831855f * ang / 360.0f;
            p->x = 16.0f * (f32)sin(rad);
            p->y = 16.0f * (f32)cos(rad);
            p->velX = p->x * a;
            p->velY = p->y * a;
            p->destX = 0.0f;
            p->destY = 0.0f;
            p->angle = -ang;
            p->scale = 1.0f;
        }
        switch (type) {
        case 0:
            p->alpha = a_data[p->seq];
            p->scale = scale_data[p->seq];
            break;
        default:
            p->alpha = a_data2[p->seq];
            p->scale = scale_data2[p->seq];
            break;
        }
        if ((counter & 1) == (i & 1)) {
            p->x += p->velX;
            p->y += p->velY;
        } else {
            p->x += 0.2f * p->velX;
            p->y += 0.2f * p->velY;
        }
        p->velX += b * (p->destX - p->velX);
        p->velY += b * (p->destY - p->velY);
    }
    prio = 1;
    if (alpha != 255) {
        prio = 2;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), prio, (DispCallback)effSparkDisp, effect);
}

void effSparkDisp(CameraId cameraId, EffectEntry* effect) {
    EffSparkParticle* p = effect->userdata;
    CameraEntry* camera;
    GXTexObj texObj;
    Mtx mtx2;
    Mtx mtx;
    Mtx rot;
    Mtx scale;
    Mtx base;
    s32 i;
    s32 alpha;
    f32 size;

    camera = camGetPtr(cameraId);
    alpha = p->alpha;
    effGetTexObj(0x1C, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    PSMTXTrans(mtx, p->x, p->y, p->z);
    PSMTXScale(scale, p->scale, p->scale, p->scale);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mtx, rot, base);
    PSMTXConcat(base, scale, base);

    size = 16.0f;

    for (i = 1, p++; i < effect->count; i++, p++) {
        if (p->seq < 0) {
            continue;
        }
        PSMTXRotRad(rot, 'z', MTXDegToRad(p->angle));
        PSMTXScale(scale, p->scale, p->scale, p->scale);
        PSMTXTrans(mtx, p->x, p->y, 0.0f);
        PSMTXConcat(mtx, rot, mtx2);
        PSMTXConcat(mtx2, scale, mtx2);
        PSMTXConcat(base, mtx2, mtx2);
        PSMTXConcat(camera->view, mtx2, mtx2);
        GXLoadPosMtxImm(mtx2, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, (alpha * p->alpha) >> 8 });
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-size / 2, size / 2, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(size / 2, size / 2, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(size / 2, -size / 2, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-size / 2, -size / 2, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
}
