#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/n64/eff_rothammer_success_n64.h"

typedef struct EffFireFlowerN64Work {
    s32 type;     // 0x0
    s32 state;    // 0x4
    Vec position; // 0x8
    f32 velX;     // 0x14
    f32 velY;     // 0x18
    f32 velZ;     // 0x1C
    f32 accelY;   // 0x20
    f32 rotY;     // 0x24
    f32 rotX;     // 0x28
    f32 offsetY;  // 0x2C
    f32 scale;    // 0x30
    s32 stage;    // 0x34
    s32 alpha2;   // 0x38
    s32 alpha;    // 0x3C
    s32 maxTime;  // 0x40
    s32 timer;    // 0x44
    s32 timer2;   // 0x48
    s32 delay;    // 0x4C
    s32 flip;     // 0x50
} EffFireFlowerN64Work;

#ifdef __MWERKS__
#endif

effN64Vtx size4x4_tex4x4_vtx[] = {
    {-2, -2, 0, 0, 0, 255, 0, 0, 255},
    {2, -2, 0, 512, 0, 255, 0, 0, 255},
    {2, 2, 0, 512, 512, 255, 0, 0, 255},
    {-2, 2, 0, 0, 512, 255, 0, 0, 255},
};

static effN64Vtx size16x16_tex16x16_vtx[] = {
    {-8, -8, 0, 0, 0, 255, 255, 255, 255},
    {8, -8, 0, 512, 0, 255, 255, 255, 255},
    {8, 8, 0, 512, 512, 255, 255, 255, 255},
    {-8, 8, 0, 0, 512, 255, 255, 255, 255},
};

static effN64Vtx size32x32_tex32x32_vtx[] = {
    {-16, 0, 2, 0, 0, 255, 255, 255, 255},
    {16, 0, 2, 1024, 0, 255, 255, 255, 255},
    {16, 32, 2, 1024, 1024, 255, 255, 255, 255},
    {-16, 32, 2, 0, 1024, 255, 255, 255, 255},
};

static s8 scale_data[] = "27<AFKPUZ_dgigd";

void effFireFlowerMain(EffectEntry* effect);
void effFireFlowerDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effFireFlowerN64Entry(s32 type, f32 x, f32 y, f32 z, s32 lifetime) {
    EffectEntry* effect;
    EffFireFlowerN64Work* work;
    EffFireFlowerN64Work* p;
    s32 i;

    effect = effEntry();
    effect->type = "FireFlowerN64";
    effect->count = 31;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffFireFlowerN64Work) * 31);
    effect->userdata = work;
    effect->callback = effFireFlowerMain;
    if (type == 1) {
        type = 0;
        work->flip = 1;
    }
    work->state = 0;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    if (lifetime <= 0) {
        lifetime = 1000;
    }
    work->maxTime = lifetime;
    work->timer = 0;
    work->timer2 = 0;
    work->alpha = 255;
    work->scale = 0.5f;
    work->stage = 0;
    work->alpha2 = 0;
    work->rotX = 0.0f;
    work->offsetY = -10.0f;
    p = work + 1;
    for (i = 1; i < 31;) {
        p->type = 1;
        p->position.x = 0.0f;
        p->position.y = 0.0f;
        p->position.z = 0.0f;
        p->velX = 8.0f;
        p->velY = 0.0f;
        p->velZ = (f32)(rand() % 20 - 8);
        p->accelY = 0.05f * (f32)(rand() % 10 - 5);
        p->rotY = (f32)(rand() % 80 + 5);
        p->alpha = 255;
        p->delay = i++ - 1;
        p->timer = 10;
        p->velX /= 1.5f;
        p->velY /= 1.5f;
        p->velZ /= 1.5f;
        p->delay <<= 1;
        p++;
    }
    return effect;
}

void effFireFlowerMain(EffectEntry* effect) {
    EffFireFlowerN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 timer;
    s32 state;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->timer++;
    work->timer2++;
    state = work->state;
    if (state == 7) {
        effDelete(effect);
        return;
    }
    timer = work->timer;
    switch (state) {
    case 0:
        effRotHammerSuccessN64Entry(0, work->flip == 0 ? work->position.x - 10.0f : 10.0f + work->position.x, work->position.y, work->position.z, 7, 20);
        work->stage = 0;
        work->alpha2 = 0;
        work->state++;
        work->timer = 0;
        break;
    case 1:
        if (timer >= 16) {
            work->state++;
            work->timer = 0;
        } else {
            work->scale = 0.01f * (f32)scale_data[timer - 1];
            work->stage = 0;
            work->alpha2 = 0;
        }
        break;
    case 2:
        if (timer >= 4) {
            work->state++;
            work->timer = 0;
        } else {
            work->stage = 1;
        }
        break;
    case 3:
        if (timer >= 30) {
            work->state++;
            work->timer = 0;
        } else {
            work->stage = 2;
        }
        break;
    case 4:
        if (timer > work->maxTime) {
            work->state++;
            work->timer = 0;
        } else {
            work->offsetY = 0.0f;
            work->stage = 3;
            if ((timer >> 2) & 1) {
                work->scale = 1.0f;
            } else {
                work->scale = 1.05f;
            }
        }
        break;
    case 5:
        if (timer >= 20) {
            work->state++;
            work->timer = 0;
        } else {
            work->stage = 0;
            work->offsetY = -10.0f;
        }
        break;
    case 6:
        work->stage = 0;
        work->rotX += 18.0f;
        work->alpha *= 0.9f;
        if (work->rotX >= 720.0f) {
            work->rotX = 720.0f;
            work->state++;
        }
        break;
    }
    timer = work->state;
    work++;
    if (timer >= 4 && timer <= 6) {
        for (i = 1; i < effect->count; i++, work++) {
            if (work->delay > 0) {
                work->delay--;
                if (work->delay > 0) {
                    continue;
                }
            }
            work->velY += work->accelY;
            work->position.x += work->velX;
            work->position.y += work->velY;
            work->position.z += work->velZ;
            if (timer == 4 && work->position.x > 200.0f) {
                work->position.x = 0.0f;
                work->position.y = 0.0f;
                work->velY = 0.0f;
                work->position.z = 0.0f;
            }
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effFireFlowerDisp, effect);
}

void effFireFlowerDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffFireFlowerN64Work* work;
    s32 i;
    s32 state;
    s32 stage;
    f32 scale;
    f32 rotX;
    f32 offsetY;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx rot;
    Mtx rot2;
    Mtx pre;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    stage = work->stage;
    scale = work->scale;
    state = work->state;
    rotX = work->rotX;
    offsetY = work->offsetY;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scaleMtx, work->flip != 0 ? -1.0f : 1.0f, 1.0f, 1.0f);
    PSMTXConcat(mtx, rot, rot2);
    PSMTXConcat(rot2, scaleMtx, rot2);
    GXSetTevColor(GX_TEVREG0, (GXColor){255, 0, 0, work->alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){255, 0, 0, work->alpha2});
    GXSetCullMode(GX_CULL_NONE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    if (state >= 4 && state <= 6) {
        PSMTXTrans(mtx, 0.0f, 16.0f, 0.0f);
        PSMTXConcat(rot2, mtx, pre);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        effGetTexObjN64(0x36, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
        PSMTXScale(scaleMtx, 0.0625f, 0.0625f, 0.0f);
        GXLoadTexMtxImm(scaleMtx, GX_TEXMTX0, GX_MTX2x4);
        work++;
        for (i = 1; i < effect->count; i++, work++) {
            if (work->delay == 0) {
                PSMTXRotRad(rot, 'z', 0.017453292f * work->rotY);
                PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
                PSMTXConcat(rot, mtx, mtx);
                PSMTXConcat(pre, mtx, mtx);
                PSMTXConcat(camera->view, mtx, mtx);
                GXLoadPosMtxImm(mtx, GX_PNMTX0);
                GXSetCurrentMtx(GX_PNMTX0);
                if (work->type != 0) {
                    effSetVtxDescN64(size16x16_tex16x16_vtx);
                } else {
                    effSetVtxDescN64(size4x4_tex4x4_vtx);
                }
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
            }
        }
    }
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_A1, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A1, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_A0, GX_CA_APREV, GX_CA_ZERO);
    effGetTexObjN64(0x37, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 0.03125f, 0.0078125f, 0.0f);
    PSMTXTrans(mtx, 0.0f, 32.0f * (f32)(3 - stage), 0.0f);
    PSMTXConcat(scaleMtx, mtx, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    PSMTXTrans(mtx, 0.0f, 32.0f * (f32)(2 - stage), 0.0f);
    PSMTXConcat(scaleMtx, mtx, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
    PSMTXTrans(mtx, offsetY, 0.0f, 0.0f);
    PSMTXScale(scaleMtx, scale, scale, scale);
    PSMTXConcat(mtx, scaleMtx, mtx);
    PSMTXRotRad(rot, 'y', 0.017453292f * rotX);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(rot2, mtx, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    effSetVtxDescN64(size32x32_tex32x32_vtx);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
}
