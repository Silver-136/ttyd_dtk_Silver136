#include "eff/eff_miss_star.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

typedef struct EffMissStarWork {
    Vec position;     // 0x0
    Vec velocity;     // 0xC
    Vec accel;        // 0x18
    s32 life;         // 0x24
    s32 delay;        // 0x28
    f32 rotZ;         // 0x2C
    f32 rotZSpeed;    // 0x30
    f32 rotY;         // 0x34
    f32 scale;        // 0x38
    s32 fadeLife;     // 0x3C
    f32 alpha;        // 0x40
} EffMissStarWork;

Vec spd_tbl[15] = {
    { 0.0f, 4.5f, 0.0f },  { 1.0f, 4.0f, 0.0f },  { 2.0f, 3.0f, 0.0f },
    { 3.0f, 2.0f, 0.0f },  { 3.5f, 1.0f, 0.0f },  { 4.0f, 0.0f, 0.0f },
    { 4.5f, 0.0f, 0.0f },  { 5.0f, 0.0f, 0.0f },  { 4.5f, 0.0f, 0.0f },
    { 4.0f, 0.0f, 0.0f },  { 3.5f, -1.0f, 0.0f }, { 3.0f, -2.0f, 0.0f },
    { 2.0f, -3.0f, 0.0f }, { 1.0f, -4.0f, 0.0f }, { 0.0f, -4.5f, 0.0f },
};

Vec scale_tbl[15] = {
    { 1.0f, 1.0f, 1.0f },    { 0.8f, 0.8f, 0.8f },    { 0.9f, 0.9f, 0.9f },
    { 1.1f, 1.1f, 1.1f },    { 1.0f, 1.0f, 1.0f },    { 0.8f, 0.8f, 0.8f },
    { 0.9f, 0.9f, 0.9f },    { 1.1f, 1.1f, 1.1f },    { 1.0f, 1.0f, 1.0f },
    { 0.8f, 0.8f, 0.8f },    { 0.9f, 0.9f, 0.9f },    { 1.1f, 1.1f, 1.1f },
    { 1.0f, 1.0f, 1.0f },    { 0.8f, 0.8f, 0.8f },    { 0.9f, 0.9f, 0.9f },
};

EffectEntry* effMissStarEntry(s32 count, s32 delay, f32 x, f32 y, f32 z, s32 index) {
    EffectEntry* effect;
    EffMissStarWork* work;
    f32 scaleA;
    f32 scaleB;
    s32 sign;
    s32 i;

    effect = effEntry();
    scaleA = 0.8f;
    scaleB = 1.0f;
    if (count <= 0) {
        scaleA = 0.4f;
        count = 1;
        scaleB = 0.7f;
    }
    sign = 1;
    if (index < 0) {
        index = -index;
        sign = -1;
    }
    // FAKE: endless loop with a break; `while (index > 5) { index -= 5; }` changes the object
    while (TRUE) {
        if (index <= 5) {
            break;
        }
        index -= 5;
    }
    effect->type = "SmallStar";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffMissStarWork) * count);
    effect->userdata = work;
    effect->callback = effMissStarMain;
    for (i = 0; i < count; i++, work++) {
        work->position.x = x;
        work->position.y = y;
        work->position.z = z;
        work->accel.x = scaleB * (sign * (2.0f * spd_tbl[index % 8].x));
        work->accel.y = scaleB * (2.0f * spd_tbl[index % 8].y);
        work->accel.z = scaleB * (2.0f * spd_tbl[index % 8].z);
        work->velocity.x = scaleB * (sign * spd_tbl[index % 8].x);
        work->velocity.y = scaleB * spd_tbl[index % 8].y;
        work->velocity.z = scaleB * spd_tbl[index % 8].z;
        index++;
        work->scale = scaleA * scale_tbl[i % 8].x;
        work->rotZ = 0.0f;
        work->rotZSpeed = sign * 107;
        work->rotY = -camGetPtr(CAMERA_3D)->viewYaw;
        work->life = 14;
        work->delay = delay;
        work->fadeLife = 10;
        work->alpha = 255.0f;
    }
    return effect;
}

void effMissStarMain(EffectEntry* effect) {
    EffMissStarWork* work = effect->userdata;
    Vec position;
    Vec position2;
    s32 i;
    f32 decay = 0.8f;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    position2 = position;
    for (i = 0; i < effect->count; i++, work++) {
        if (work->delay != 0) {
            work->delay--;
            if (work->delay == 0) {
                break;
            }
        }
        if (work->life >= 0) {
            work->position.x += work->accel.x;
            work->position.y += work->accel.y;
            work->position.z += work->accel.z;
        }
        work->rotY = -camGetPtr(CAMERA_3D)->viewYaw;
        work->rotZ += work->rotZSpeed;
        work->rotZ = reviseAngle(work->rotZ);
        work->rotZSpeed *= decay;
        if (work->life < 10) {
            work->velocity.x *= 0.5f;
            work->velocity.y *= 0.5f;
            work->velocity.z *= 0.5f;
            work->accel.x = work->velocity.x;
            work->accel.y = work->velocity.y;
            work->accel.z = work->velocity.z;
        }
        work->life--;
        if (work->life < 0) {
            work->fadeLife--;
            if (work->fadeLife < 0) {
                effDelete(effect);
                return;
            }
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position2), 1, (DispCallback)effMissStarDisp, effect);
}

void effMissStarDisp(CameraId cameraId, EffectEntry* effect) {
    EffMissStarWork* work = effect->userdata;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rotY;
    Mtx rotZ;
    s32 i;

    effGetTexObj(24, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    for (i = 0; i < effect->count; i++, work++) {
        if (work->delay != 0) {
            break;
        }
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rotY, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
        PSMTXRotRad(rotZ, 'z', 0.017453292f * work->rotZ);
        PSMTXScale(scale, work->scale, work->scale, work->scale);
        PSMTXConcat(rotY, mtx, mtx);
        PSMTXConcat(mtx, rotZ, mtx);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(camGetPtr(CAMERA_3D)->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-16.0f, 16.0f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(16.0f, 16.0f, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(16.0f, -16.0f, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-16.0f, -16.0f, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
    }
}
