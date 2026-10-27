#include "eff/eff_mizutama.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

typedef struct EffMizutamaWork {
    s32 type;       // 0x0
    Vec position;   // 0x4, element 0: origin, others: offset from origin
    f32 velX;       // 0x10
    f32 velY;       // 0x14
    f32 scale;      // 0x18
    s32 field_0x1C; // 0x1C
    s32 field_0x20; // 0x20
    s32 delay;      // 0x24
} EffMizutamaWork;

EffectEntry* effMizutamaEntry(s32 type, f32 x, f32 y, f32 z, f32 angle) {
    s32 radius;
    EffectEntry* effect;
    EffMizutamaWork* work;
    s32 count, i;
    f32 dist, rad;

    effect = effEntry();
    switch (type) {
        case 0:
        case 4:
            count = irand(2) + 3;
            break;
        case 1:
        case 5:
            count = irand(5) + 6;
            break;
        case 2:
        case 6:
            count = irand(10) + 11;
            break;
        case 3:
        case 7:
            count = irand(15) + 16;
            break;
    }
    effect->type = "mizutama";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffMizutamaWork) * effect->count);
    effect->userdata = work;
    effect->callback = effMizutamaMain;
    // FAKE: split computation; `radius = (type + 1) * 10;` changes the object
    radius = type + 1;
    radius *= 10;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 1.0f;
    work->field_0x1C = 0;
    work->field_0x20 = 0;

    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (type < 4) {
            // scattered around a circle
            dist = radius;
            rad = 6.2831855f * irand(360) / 360.0f;
            work->position.x = dist * (f32)sin(rad);
            work->position.y = dist * -(f32)cos(rad);
            work->position.z = 0.0f;
            rad = 6.2831855f * angle / 360.0f;
            work->velX = 2.0f * (f32)sin(rad);
            work->velY = 2.0f * (f32)cos(rad);
            work->scale = (irand(20) + 80) / 100.0f;
            work->delay = i * 2;
        } else {
            // scattered along a line, splashing outwards
            // FAKE: parameter z reused as the half-width; the local dist (as in the circle branch) changes the object
            z = radius;
            irand(360);
            work->position.x = irand((s32)z * 2) - z;
            work->position.y = 0.0f;
            work->position.z = 0.0f;
            angle = work->position.x < 0.0f ? 225.0f : 135.0f;
            rad = 6.2831855f * angle / 360.0f;
            work->velX = 2.0f * (f32)sin(rad);
            work->velY = 2.0f * (f32)cos(rad);
            work->scale = (irand(20) + 80) / 100.0f;
            work->delay = i * 2;
        }
    }
    return effect;
}

void effMizutamaMain(EffectEntry* effect) {
    EffMizutamaWork* work = effect->userdata;
    Vec position;
    s32 done, i;

    done = 0;
    position = (Vec){ work->position.x, work->position.y, work->position.z };
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay) {
            work->delay--;
            continue;
        }
        work->position.x += work->velX;
        work->position.y += work->velY;
        work->velX *= 0.97f;
        work->velY *= 0.97f;
        work->position.y += 0.3f;
        work->scale *= 0.99f;
        if (work->scale < 0.01f) {
            done++;
        }
    }
    if (done >= effect->count - 1) {
        effDelete(effect);
        return;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effMizutamaDisp, effect);
}

void effMizutamaDisp(CameraId cameraId, EffectEntry* effect) {
    EffMizutamaWork* work = effect->userdata;
    CameraEntry* camera;
    f32 baseY, size, half;
    s32 i;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx base;

    camera = camGetPtr(cameraId);
    baseY = work->position.y;
    PSMTXTrans(mtx, work->position.x, baseY, work->position.z);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, scale, base);

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    effGetTexObj(0x3F, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    size = 8.0f;
    half = 4.0f;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay) {
            continue;
        }
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, work->scale, work->scale, work->scale);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(base, mtx, mtx);
        if (baseY > 360.0f) {
            continue;
        }
        PSMTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-size / 2, half, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(half, half, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(half, -size / 2, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-size / 2, -size / 2, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
}
