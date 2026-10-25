#include "eff/n64/eff_kemuri3_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"


typedef struct EffKemuri3N64Work {
    BOOL active;      // 0x0
    s16 field_0x4;    // 0x4
    s16 timer;        // 0x6
    u8 alpha;         // 0x8
    u8 pad_9[3];      // 0x9
    Vec position;     // 0xC
    Vec scale;        // 0x18
    Vec rotation;     // 0x24
    Mtx mtx;          // 0x30
    f32 speed;        // 0x60
    f32 field_0x64;   // 0x64
    f32 field_0x68;   // 0x68
    f32 field_0x6C;   // 0x6C
    f32 field_0x70;   // 0x70
    f32 field_0x74;   // 0x74
    f32 field_0x78;   // 0x78
    f32 field_0x7C;   // 0x7C
    f32 sinAngle;     // 0x80
    f32 cosAngle;     // 0x84
} EffKemuri3N64Work;

void effKemuri3Main(EffectEntry* effect);
void effKemuri3Disp(CameraId cameraId, EffectEntry* effect);

#pragma opt_propagation off
#pragma opt_lifetimes off
EffectEntry* effKemuri3N64Entry(f32 x, f32 y, f32 z, f32 angle) {
    s32 ofs;
    EffectEntry* effect;
    EffKemuri3N64Work* work;
    s32 i;
    f32 rad;
    f32 t;

    effect = effEntry();
    effect->type = "Kemuri3N64";
    effect->count = 5;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffKemuri3N64Work) * 5);
    effect->userdata = work;
    effect->callback = effKemuri3Main;
    for (i = 0, ofs = 0; i < 5; i++, ofs += 72, work++) {
        work->active = TRUE;
        work->field_0x64 = 0.29999998f;
        work->field_0x68 = 0.0f;
        work->field_0x6C = 0.0f;
        work->position.x = x;
        work->position.y = y;
        work->position.z = z;
        work->scale.x = 1.0f;
        work->scale.y = 1.0f;
        work->scale.z = 1.0f;
        work->alpha = 255;
        work->timer = 60;
        work->rotation.x = 0.0f;
        work->rotation.y = angle + (f32)ofs;
        work->rotation.z = 0.0f;
        work->field_0x70 = 1.75f;
        work->field_0x74 = -0.08f;
        work->field_0x78 = 0.0f;
        work->field_0x7C = 0.0f;
        work->speed = -3.9f;
        rad = reviseAngle(work->rotation.y);
        t = 6.2831855f * rad;
        rad = t / 360.0f;
        work->sinAngle = sin(rad);
        work->cosAngle = cos(rad);
    }
    return effect;
}

#pragma opt_propagation reset
#pragma opt_lifetimes reset
void effKemuri3Main(EffectEntry* effect) {
    EffKemuri3N64Work* work = effect->userdata;
    s32 i;
    BOOL alive;
    Mtx rotZ;
    Mtx rotY;
    Mtx rotX;
    Mtx trans;
    Mtx scale;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    alive = FALSE;
    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    for (i = 0; i < effect->count; i++, work++) {
        if (!work->active) {
            continue;
        }
        if (--work->timer <= 0) {
            work->active = FALSE;
            continue;
        }
        alive = TRUE;
        work->speed *= 0.85f;
        work->position.x += work->speed * work->sinAngle;
        work->position.z += work->speed * work->cosAngle;
        work->field_0x78 += work->field_0x7C;
        work->field_0x74 += work->field_0x78;
        work->field_0x70 += work->field_0x74;
        work->position.y += work->field_0x70;
        if (work->field_0x70 < 0.0f) {
            work->field_0x78 = 0.004f;
        }
        work->rotation.x -= 1.0f;
        if (work->rotation.x < -100.0f) {
            work->rotation.x = -100.0f;
        }
        PSMTXRotRad(rotX, 'x', 0.017453292f * work->rotation.x);
        PSMTXRotRad(rotY, 'y', 0.017453292f * work->rotation.y);
        PSMTXRotRad(rotZ, 'z', 0.017453292f * work->rotation.z);
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, work->scale.x, work->scale.y, work->scale.z);
        PSMTXConcat(rotX, rotZ, rotZ);
        PSMTXConcat(rotY, rotZ, rotZ);
        PSMTXConcat(rotZ, scale, scale);
        PSMTXConcat(trans, scale, work->mtx);
    }
    if (!alive) {
        effDelete(effect);
        return;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effKemuri3Disp, effect);
}

void effKemuri3Disp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffKemuri3N64Work* work;
    s32 i;
    Mtx mtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    effGetTexObjN64(0xF, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    for (i = 0; i < effect->count; i++, work++) {
        if (!work->active) {
            continue;
        }
        GXSetTevColor(GX_TEVREG0, (GXColor){0x70, 0x60, 0x19, work->alpha});
        PSMTXConcat(camera->view, work->mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(8.0f, 0.0f, 8.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(8.0f, 0.0f, -8.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-8.0f, 0.0f, -8.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(0.0f, 1.0f);
        GXPosition3f32(-8.0f, 0.0f, 8.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(0.0f, 0.0f);
    }
}
