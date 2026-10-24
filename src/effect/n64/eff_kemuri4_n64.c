#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_n64.h"
#include "mario/mario.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>

typedef struct EffKemuri4N64Work {
    s32 active;      // 0x0
    u16 type;        // 0x4
    s16 timer;       // 0x6
    u8 alpha;        // 0x8
    s8 texFlag;      // 0x9
    u8 pad_A[2];     // 0xA
    Vec position;    // 0xC
    Vec scale;       // 0x18
    Vec rotation;    // 0x24
    Mtx mtx;         // 0x30
    f32 rotSpeed;    // 0x60
    f32 moveScale;   // 0x64
    f32 field_68;    // 0x68
    f32 field_6C;    // 0x6C
    f32 velocity;    // 0x70
    f32 accel;       // 0x74
    f32 accel2;      // 0x78
    f32 accel3;      // 0x7C
    f32 dirX;        // 0x80
    f32 dirZ;        // 0x84
} EffKemuri4N64Work;

double sin(double);
double cos(double);

effN64Vtx flowers_v[41] = {
    {-9, 8, -7, 0, 512, 255, 255, 255, 255},
    {-9, 4, 2, 512, 512, 255, 255, 255, 255},
    {0, 0, 0, 512, 0, 255, 255, 255, 255},
    {0, 4, -9, 0, 0, 255, 255, 255, 255},
    {0, 4, 9, 0, 0, 255, 255, 255, 255},
    {-9, 4, -2, 512, 512, 255, 255, 255, 255},
    {-9, 8, 7, 0, 512, 255, 255, 255, 255},
    {9, 8, 7, 0, 512, 255, 255, 255, 255},
    {9, 4, -2, 0, 0, 255, 255, 255, 255},
    {0, 4, 9, 512, 512, 255, 255, 255, 255},
    {0, 4, -9, 512, 512, 255, 255, 255, 255},
    {9, 4, 2, 0, 0, 255, 255, 255, 255},
    {9, 8, -7, 0, 512, 255, 255, 255, 255},
    {0, 0, -1, 0, 0, 100, 160, 60, 255},
    {0, 0, 1, 0, 0, 100, 160, 60, 255},
    {-1, -7, 2, 0, 0, 100, 160, 60, 255},
    {-1, -7, 0, 0, 0, 100, 160, 60, 255},
    {-1, -17, 2, 0, 0, 100, 160, 60, 255},
    {-1, -17, 0, 0, 0, 100, 160, 60, 255},
    {1, 0, 0, 0, 0, 100, 160, 60, 255},
    {-1, 0, 0, 0, 0, 100, 160, 60, 255},
    {-2, -7, 0, 0, 0, 100, 160, 60, 255},
    {0, -7, 0, 0, 0, 100, 160, 60, 255},
    {-2, -17, 0, 0, 0, 100, 160, 60, 255},
    {0, -17, 0, 0, 0, 100, 160, 60, 255},
    {10, 0, 10, 1024, -512, 255, 255, 255, 255},
    {10, 0, -10, 1024, 512, 255, 255, 255, 255},
    {-10, 0, -10, 0, 512, 255, 255, 255, 255},
    {-10, 0, 10, 0, -512, 255, 255, 255, 255},
    {0, -1, 0, 0, 0, 100, 160, 60, 255},
    {2, -17, 0, 0, 0, 100, 160, 60, 255},
    {8, 0, 8, 1024, 0, 255, 255, 255, 255},
    {8, 0, -8, 1024, 1024, 255, 255, 255, 255},
    {-8, 0, -8, 0, 1024, 255, 255, 255, 255},
    {-8, 0, 8, 0, 0, 255, 255, 255, 255},
    {12, -10, 0, 0, 0, 100, 160, 60, 255},
    {5, -9, 0, 0, 0, 100, 160, 60, 255},
    {3, -16, 0, 0, 0, 100, 160, 60, 255},
    {-3, -16, 0, 0, 0, 100, 160, 60, 255},
    {-4, -7, 0, 0, 0, 100, 160, 60, 255},
    {-11, -5, 0, 0, 0, 100, 160, 60, 255},
};

void effKemuri4Main(EffectEntry* effect);
void effKemuri4Disp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effKemuri4N64Entry(s32 type, f32 x, f32 y, f32 z, f32 angle, f32 flip) {
    EffectEntry* effect;
    EffKemuri4N64Work* work;
    f32 zero = 0.0f;
    f32 rad;
    s32 i;

    effect = effEntry();
    effect->type = "Kemuri4N64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffKemuri4N64Work));
    effect->userdata = work;
    effect->callback = effKemuri4Main;
    
    for (i = 0; i < 1; i++, work++) {
    work->active = 1;
    work->type = type;
    work->field_6C = 0.0f;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale.x = 1.0f;
    work->scale.y = 1.0f;
    work->scale.z = 1.0f;
    work->alpha = 255;
    work->field_68 = 1.0f;
    work->moveScale = 5.4f;
    work->rotSpeed = (f32)(flip != zero ? -10 : 10);
    work->velocity = 0.5f;
    work->accel = -0.07f;
    work->accel2 = 0.003f;
    work->accel3 = 0.0f;
    work->timer = 15;
    work->rotation.y = angle;
    work->rotation.x = 0.0f;
    work->rotation.z = 0.0f;
    work->texFlag = (rand() % 100 >> 4) & 1;
    rad = 6.2831855f * reviseAngle(angle + (f32)(flip != zero ? -90 : 90)) / 360.0f;
    work->dirX = (f32)sin(rad);
    work->dirZ = (f32)cos(rad);
    }
    return effect;
}

void effKemuri4Main(EffectEntry* effect) {
    EffKemuri4N64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    Mtx rotZ;
    Mtx rotY;
    Mtx rotX;
    Mtx trans;
    Mtx scale;
    s32 i;
    s32 anyActive = 0;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    for (i = 0; i < effect->count; i++, work++) {
        if (work->active == 0) {
            continue;
        }
        if (--work->timer <= 0) {
            work->active = 0;
            continue;
        }
        anyActive = 1;
        if (work->type == 0 && work->field_68 && marioGetPtr()->currMotionId == 0U) {
            work->timer++;
        } else {
            work->accel2 = work->accel2 + work->accel3;
            work->accel = work->accel + work->accel2;
            work->velocity = work->velocity + work->accel;
            work->rotation.z = work->rotation.z + work->velocity * work->rotSpeed;
            work->position.x = work->position.x - work->dirX * (work->velocity * work->moveScale);
            work->position.z = work->position.z - work->dirZ * (work->velocity * work->moveScale);
            work->alpha -= 12;
            if (work->velocity < 0.0f) {
                work->field_68 = 0.0f;
            }
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
    if (anyActive == 0) {
        effDelete(effect);
    } else {
        dispEntry(CAMERA_3D, dispCalcZ(&position), 1, (DispCallback)effKemuri4Disp, effect);
    }
}

void effKemuri4Disp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera = camGetPtr(cameraId);
    effN64Vtx* vtx1;
    effN64Vtx* vtx2;
    EffKemuri4N64Work* work;
    s32 i;
    Mtx mtx;
    Mtx scaleMtx;
    GXTexObj texObj;
    GXColor color;

    work = effect->userdata;
    GXSetCullMode(GX_CULL_NONE);
    vtx1 = &flowers_v[25];
    vtx2 = &flowers_v[35];
    for (i = 0; i < effect->count; i++, work++) {
        if (work->active == 0) {
            continue;
        }
        if (work->texFlag != 0) {
            effGetTexObjN64(0x4C, &texObj);
        } else {
            effGetTexObjN64(0x4D, &texObj);
        }
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
        PSMTXScale(scaleMtx, 0.0625f, 0.0625f, 0.0f);
        GXLoadTexMtxImm(scaleMtx, GX_TEXMTX0, GX_MTX2x4);
        GXSetNumChans(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
        PSMTXConcat(camera->view, work->mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        color = (GXColor){0xF0, 0xF0, 0xF0, 0xFF};
        GXSetTevColor(GX_TEVREG0, color);
        effSetVtxDescN64(vtx1);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        effSetVtxDescN64(vtx2);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 3, 4, 5, 0);
    }
}
