#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"


typedef struct EffPacknBfireWork {
    s32 type;     // 0x0
    Vec pos;      // 0x4
    Vec tgt;      // 0x10
    s32 frames;   // 0x1c
    s32 life;     // 0x20
    s32 frame;    // 0x24
    s32 color[8]; // 0x28
    f32 scaleArg; // 0x48
    f32 uvx;      // 0x4c
    f32 uvy;      // 0x50
    f32 duvx;     // 0x54
    f32 duvy;     // 0x58
    f32 unk5c;    // 0x5c
    Vec vel;      // 0x60
    f32 sy;       // 0x6c
    f32 sx;       // 0x70
    f32 rot;      // 0x74
    f32 drot;     // 0x78
} EffPacknBfireWork;

EffectEntry* effPacknBfireN64Entry(s32 type, s32 frames, s32 life, f32 x, f32 y, f32 z, f32 tx, f32 ty, f32 tz,
                                   f32 scale);
void effPacknBfireMain(EffectEntry* effect);
void effPacknBfireDisp(CameraId cameraId, EffectEntry* effect);

static effN64Vtx size64x64_tex32x32_vtx[] = {
    {-32, -32, 0, 0, 0, 0, 0, 0, 255},
    {32, -32, 0, 1024, 0, 0, 0, 0, 255},
    {32, 32, 0, 1024, 1024, 0, 0, 0, 255},
    {-32, 32, 0, 0, 1024, 0, 0, 0, 255},
};

EffectEntry* effPacknBfireN64Entry(s32 type, s32 frames, s32 life, f32 x, f32 y, f32 z, f32 tx, f32 ty, f32 tz,
                                   f32 scale) {
    s32 count;
    EffectEntry* effect;
    EffPacknBfireWork* work;
    s32 i;
    f32 base;
    f32 angle;

    effect = effEntry();
    count = (type == 2) ? 8 : 1;
    effect->type = "PacknBfireN64";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, count * sizeof(EffPacknBfireWork));
    effect->userdata = work;
    effect->callback = effPacknBfireMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    if (life <= 0) {
        work->life = 1000;
    } else {
        work->life = life;
    }
    work->type = type;
    work->frame = 0;
    work->frames = frames;
    work->color[3] = 0;
    work->pos.x = x;
    work->pos.y = y;
    work->pos.z = z;
    work->tgt.x = tx;
    work->tgt.y = ty;
    work->tgt.z = tz;
    work->vel.x = (work->tgt.x - work->pos.x) / (f32)frames;
    work->vel.y = (work->tgt.y - work->pos.y) / (f32)frames;
    work->vel.z = (work->tgt.z - work->pos.z) / (f32)frames;
    work->scaleArg = scale;
    switch (type) {
    case 0:
        break;
    case 1:
        work->color[0] = 255;
        work->color[1] = 88;
        work->color[2] = 70;
        work->color[3] = 55;
        work->color[4] = 11;
        work->color[5] = 51;
        work->color[6] = 123;
        work->color[7] = 255;
        work->uvx = (f32)(rand() % 65);
        work->uvy = (f32)(rand() % 65);
        work->sx = 0.2f;
        work->sy = 0.3f;
        work->duvx = 0.0f;
        work->duvy = 0.0f;
        work->rot = 0.0f;
        work->drot = -10.0f;
        break;
    default:
        work->color[0] = 255;
        work->color[1] = 88;
        work->color[2] = 70;
        work->color[3] = 55;
        work->color[4] = 11;
        work->color[5] = 51;
        work->color[6] = 123;
        work->color[7] = 255;
        base = (f32)(rand() % 361);
        for (i = 0; i < count; i++) {
            work->pos.x = x;
            work->pos.y = y;
            work->pos.z = z;
            angle = 6.2831855f * (base + (f32)((i * 360) / 8)) / 360.0f;
            work->vel.x = 5.0f * (f32)sin(angle);
            work->vel.y = 2.0f * (f32)cos(angle) + 3.0f;
            work->uvx = (f32)(rand() % 65);
            work->uvy = (f32)(rand() % 65);
            work->sx = 1.0f;
            work->sy = 1.0f;
            work->duvx = 0.0f;
            work->duvy = 0.0f;
            work->rot = -45.0f;
            work->drot = -10.0f;
            work++;
        }
        break;
    }
    return effect;
}

void effPacknBfireMain(EffectEntry* effect) {
    EffPacknBfireWork* work;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 type;
    s32 i;

    work = effect->userdata;
    tmp.x = work->pos.x;
    tmp.y = work->pos.y;
    tmp.z = work->pos.z;
    position = tmp;
    type = work->type;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->life = 16;
    }
    if (work->life < 1000) {
        work->life--;
    }
    work->frame++;
    if (work->life == 1 && type == 1) {
        effPacknBfireN64Entry(2, 32, 32, work->pos.x, work->pos.y, work->pos.z, work->tgt.x, work->tgt.y,
                              work->tgt.z, 1.0f);
    }
    if (work->life < 0) {
        effDelete(effect);
    } else {
        s32 frame = work->frame;
        switch (type) {
        case 0:
            if ((frame & 3) == 0) {
                effPacknBfireN64Entry(1, work->frames, work->frames, work->pos.x, work->pos.y, work->pos.z,
                                      work->tgt.x, work->tgt.y, work->tgt.z, 1.0f);
            }
            break;
        case 1:
            work->vel.x *= 0.97f;
            work->vel.z *= 0.97f;
            work->vel.y += 0.02f * (1.0f - work->vel.y);
            work->pos.x += work->vel.x;
            work->pos.y += work->vel.y;
            work->pos.z += work->vel.z;
            work->sx += 0.05f * (1.0f - work->sx);
            work->sy += 0.1f * (1.0f - work->sy);
            work->rot += work->drot;
            break;
        case 2:
            for (i = 0; i < effect->count; i++) {
                work->vel.x *= 0.9f;
                work->vel.y -= 0.2f;
                work->pos.x += work->vel.x;
                work->pos.y += work->vel.y;
                if (work->pos.y < 0.0f) {
                    work->pos.y = 0.0f;
                    if (work->vel.y < 0.0f) {
                        work->vel.y *= -0.6f;
                    }
                }
                work->sx += 0.07f * -work->sx;
                work->sy += 0.05f * -work->sy;
                work->rot += work->drot;
                work++;
            }
            break;
        }
        dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effPacknBfireDisp, effect);
    }
}

void effPacknBfireDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffPacknBfireWork* work;
    GXTexObj texObj;
    Mtx scale;
    Mtx mtx;
    Mtx rot;
    f32 angle;
    f32 sc;
    s32 i;
    s32 a;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    a = work->color[3];
    sc = work->scaleArg;
    if (work->type != 0) {
        GXSetTevColor(GX_TEVREG0, (GXColor){work->color[0], work->color[1], work->color[2], a});
        GXSetTevColor(GX_TEVREG1, (GXColor){work->color[4], work->color[1], work->color[6], work->color[7]});
        GXSetNumChans(0);
        GXSetNumTevStages(5);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG2);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG2);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_C2, GX_CC_TEXC, GX_CC_C1);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A1, GX_CA_TEXA, GX_CA_A2);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_C2, GX_CC_C2, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_A0, GX_CA_A1, GX_CA_APREV);
        GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_C0);
        GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_APREV, GX_CA_A2, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE4, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE4, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE4, GX_CC_ZERO, GX_CC_C2, GX_CC_TEXC, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE4, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetNumTexGens(2);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
        PSMTXScale(mtx, 1.0f / 32.0f, 1.0f / 32.0f, 0.0f);
        GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
        effGetTexObjN64(0x71, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        effGetTexObjN64(0x72, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP1);
        GXSetCullMode(GX_CULL_NONE);
        effSetVtxDescN64(size64x64_tex32x32_vtx);
        for (i = 0; i < effect->count; i++, work++) {
            work->uvx += work->duvx;
            work->uvy += work->duvy;
            angle = 6.2831855f * (work->rot - angleABf(0.0f, 0.0f, work->vel.x, work->vel.y)) / 360.0f;
            work->duvx = (f32)sin(angle);
            work->duvy = (f32)cos(angle);
            if (work->uvx < 0.0f) {
                work->uvx += 128.0f;
            }
            if (work->uvy > 128.0f) {
                work->uvy -= 128.0f;
            }
            PSMTXTrans(scale, work->pos.x, work->pos.y, work->pos.z);
            PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
            PSMTXScale(mtx, sc, sc, sc);
            PSMTXConcat(scale, rot, scale);
            PSMTXConcat(scale, mtx, scale);
            PSMTXScale(mtx, work->sx, work->sy, 1.0f);
            PSMTXRotRad(rot, 'z', 0.017453292f * work->rot);
            PSMTXConcat(mtx, rot, mtx);
            PSMTXConcat(scale, mtx, scale);
            PSMTXConcat(camera->view, scale, scale);
            GXLoadPosMtxImm(scale, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            PSMTXScale(mtx, 1.0f / 256.0f, 1.0f / 64.0f, 0.0f);
            PSMTXTrans(scale, work->uvx, work->uvy, 0.0f);
            PSMTXConcat(mtx, scale, mtx);
            GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
        }
    }
}
