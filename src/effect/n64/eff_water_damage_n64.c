#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "mario/mario.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"


typedef struct EffWaterDamageN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 velX;     // 0x10
    f32 velY;     // 0x14
    f32 velZ;     // 0x18
    s32 lifetime; // 0x1C
    s32 frame;    // 0x20
    s32 r;        // 0x24
    s32 g;        // 0x28
    s32 b;        // 0x2C
    s32 alpha;    // 0x30
    f32 scale;    // 0x34
    s32 variant;  // 0x38
} EffWaterDamageN64Work;

#ifdef __MWERKS__
#endif

effN64Vtx size8x8_tex16x16_0_vtx[] = {
    {-4, -4, 0, 0, 0, 0, 0, 0, 255},
    {4, -4, 0, 512, 0, 0, 0, 0, 255},
    {4, 4, 0, 512, 512, 0, 0, 0, 255},
    {-4, 4, 0, 0, 512, 0, 0, 0, 255},
};
effN64Vtx size8x8_tex16x16_1_vtx[] = {
    {-5, -3, 0, 0, 0, 0, 0, 0, 255},
    {5, -3, 0, 512, 0, 0, 0, 0, 255},
    {5, 3, 0, 512, 512, 0, 0, 0, 255},
    {-5, 3, 0, 0, 512, 0, 0, 0, 255},
};
effN64Vtx size8x8_tex16x16_2_vtx[] = {
    {-3, -5, 0, 0, 0, 0, 0, 0, 255},
    {3, -5, 0, 512, 0, 0, 0, 0, 255},
    {3, 5, 0, 512, 512, 0, 0, 0, 255},
    {-3, 5, 0, 0, 512, 0, 0, 0, 255},
};

u32 water_damage_tex[32] __attribute__((aligned(32))) = {
    0x78A31427, 0x572B0B52, 0x5167133A, 0x7FDF7F2F, 0x653E32A7, 0xF5FFFF8B, 0x32670486, 0x8BFFD55E,
    0x60830B26, 0xF8E8FA55, 0x602011C5, 0x2E020278, 0x59A40486, 0xF7155555, 0x90200486, 0xBC555555,
    0xE77F1BFF, 0x6258577A, 0xAE9A0C4F, 0xF5B5D522, 0xF7BF241F, 0x78002AFF, 0xF7DF1BFF, 0x022AFFDD,
    0x8D7A0A92, 0xF5F5F5EC, 0xADFA098B, 0xF555FD25, 0xE77E135A, 0xA0035555, 0xADFA09CE, 0x25D5F5F5
};

void effWaterDamageMain(EffectEntry* effect);
void effWaterDamageDisp(CameraId cameraId, EffectEntry* effect);
void effWaterDamageDisp2(CameraId cameraId, EffectEntry* effect);

EffectEntry* effWaterDamageN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    s32 variant;
    s32 j;
    EffectEntry* effect;
    EffWaterDamageN64Work* p;
    s32 i;
    EffWaterDamageN64Work* work;
    s32 count;
    f32 a;
    MarioWork* mario;

    effect = effEntry();
    variant = 0;
    if (type >= 10) {
        variant = 1;
        type = type % 10;
    }
    switch (type) {
    case 0:
    case 1:
        count = 21;
        break;
    case 2:
        count = 41;
        break;
    case 6:
        count = 9;
        break;
    default:
        count = 11;
        break;
    }
    effect->type = "WaterDamageN64";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, count * sizeof(EffWaterDamageN64Work));
    effect->userdata = work;
    effect->callback = effWaterDamageMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->alpha = 255;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->r = 70;
    work->g = 180;
    work->b = 255;
    work->variant = variant;
    p = work + 1;
    for (i = 1, j = 3; i < effect->count; j += 3, i++, p++) {
        p->position.x = 0.0f;
        p->position.y = 0.0f;
        p->position.z = 0.0f;
        p->scale = 2.0f;
        switch (type) {
        case 0:
            p->velX = rand() % 11 - 2;
            p->velY = rand() % 11 - 5;
            p->velZ = rand() % 11 - 5;
            break;
        case 1:
            p->velX = -(rand() % 11 - 2);
            p->velY = rand() % 11 - 5;
            p->velZ = rand() % 11 - 5;
            break;
        case 2:
            p->velX = 0.07f * (rand() % 100 - 50);
            p->velY = 0.2f * (rand() % 100 + 10);
            p->velZ = 0.07f * (rand() % 100 - 50);
            break;
        case 3:
            p->velX = 0.5f * (rand() % 10 - 5);
            p->velY = 0.5f * (rand() % 10 - 5);
            p->velZ = 0.5f * (rand() % 10 - 5);
            p->scale = 0.4f;
            p->lifetime = i;
            break;
        case 4:
            p->velX = rand() % 7 - 3;
            p->velY = rand() % 3 + 1;
            p->velZ = rand() % 7 - 3;
            break;
        case 5:
            p->velX = 0.3f * (rand() % 10 - 5);
            p->velY = 0.3f * (rand() % 10 + 5);
            p->velZ = 0.3f * (rand() % 10 - 5);
            p->scale = 1.0f;
            p->lifetime = i;
            break;
        case 6:
            a = 6.2831855f * i;
            p->position.x = 10.0f * (f32)sin(a / effect->count);
            p->position.z = 10.0f * (f32)cos(a / effect->count);
            p->velX = p->position.x / 12.0f;
            p->velY = 0.3f * (rand() % 5);
            p->velZ = p->position.z / 12.0f;
            p->scale = 1.0f;
            p->lifetime = i;
            break;
        case 7:
        default:
            mario = marioGetPtr();
            a = irand(90) - 45;
            p->position.x = (f32)sin(6.2831855f * (a + (-mario->directionView + camGetPtr(CAMERA_3D)->viewYaw)) / 360.0f);
            p->position.z = (f32)cos(6.2831855f * (a + (-mario->directionView + camGetPtr(CAMERA_3D)->viewYaw)) / 360.0f);
            p->velX = mario->baseSpeed * p->position.x / 2.0f;
            p->velY = 0.1f * (rand() % 10 + 5);
            p->velZ = mario->baseSpeed * p->position.z / 2.0f;
            p->scale = 1.0f;
            p->lifetime = j;
            break;
        }
        p->alpha = 255;
    }
    return effect;
}

void effWaterDamageMain(EffectEntry* effect) {
    EffWaterDamageN64Work* work = effect->userdata;
    Vec calcPos;
    Vec pos = { 0.0f, 0.0f, 0.0f };
    s32 i;
    s32 type;
    s32 variant;

    pos.x = work->position.x;
    pos.y = work->position.y;
    pos.z = work->position.z;
    calcPos = pos;
    type = work->type;
    variant = work->variant;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->lifetime = 16;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    if (work->lifetime < 4) {
        work->alpha = work->lifetime << 6;
    }
    for (i = 1, work++; i < effect->count; i++, work++) {
        work->position.x += work->velX;
        work->position.y += work->velY;
        work->position.z += work->velZ;
        work->alpha *= 0.98f;
        if (type == 2) {
            work->velY += -0.8f;
        }
        work->velY += -0.1f;
        work->velX *= 0.98f;
        work->velY *= 0.98f;
        work->velZ *= 0.98f;
        if (type == 3) {
            work->scale *= 1.2f;
        } else {
            work->scale *= 0.9f;
        }
    }
    if (variant == 0) {
        dispEntry(CAMERA_3D, dispCalcZ(&calcPos), 8, (DispCallback)effWaterDamageDisp, effect);
    } else {
        dispEntry(CAMERA_3D, dispCalcZ(&calcPos), 8, (DispCallback)effWaterDamageDisp2, effect);
    }
}

void effWaterDamageDisp(CameraId cameraId, EffectEntry* effect) {
    EffWaterDamageN64Work* work;
    CameraEntry* cam;
    s32 ix;
    Mtx trans;
    Mtx scale;
    Mtx rot;
    Mtx mtx;
    s32 i;
    s32 iy;
    s32 alpha;
    GXTexObj texObj;

    cam = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->alpha;
    ix = work->position.x;
    iy = work->position.y;
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    PSMTXTrans(trans, ix, iy, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(trans, scale, trans);
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(cam->view, trans, mtx);
    GXSetNumChans(0);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_C1);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_APREV, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.0625f, 0.0625f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x89, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    work++;
    GXInitTexObj(&texObj, water_damage_tex, 16, 16, GX_TF_CMPR, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXScale(scale, 0.0625f, 0.0625f, 0.0f);
        GXLoadTexMtxImm(scale, GX_TEXMTX1, GX_MTX2x4);
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, work->scale, work->scale, work->scale);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(mtx, trans, trans);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetTevColor(GX_TEVREG0, (GXColor){ 0xC8, 0xFF, 0xFF, alpha * work->alpha / 255 });
        GXSetTevColor(GX_TEVREG1, (GXColor){ 0x00, 0x20, 0x20, 0x00 });
        switch (i + (gp->retraceCount >> 1) & 3) {
        case 0:
        case 2:
            effSetVtxDescN64(size8x8_tex16x16_0_vtx);
            break;
        case 1:
            effSetVtxDescN64(size8x8_tex16x16_1_vtx);
            break;
        case 3:
            effSetVtxDescN64(size8x8_tex16x16_2_vtx);
            break;
        }
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}

void effWaterDamageDisp2(CameraId cameraId, EffectEntry* effect) {
    EffWaterDamageN64Work* work;
    CameraEntry* cam;
    Mtx trans;
    Mtx scale;
    Mtx rot;
    Mtx mtx;
    GXTexObj texObj;
    s32 i;
    s32 alpha;
    s32 ix;
    s32 iy;

    cam = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->alpha;
    ix = work->position.x;
    iy = work->position.y;
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    PSMTXTrans(trans, ix, iy, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(trans, scale, trans);
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(cam->view, trans, mtx);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xC8, 0xFF, 0xFF, alpha * work->alpha / 255 });
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.0625f, 0.0625f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x89, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetCurrentMtx(GX_PNMTX0);
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, work->scale, work->scale, work->scale);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(mtx, trans, trans);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        switch (i + (gp->retraceCount >> 1) & 3) {
        case 0:
        case 2:
            effSetVtxDescN64(size8x8_tex16x16_0_vtx);
            break;
        case 1:
            effSetVtxDescN64(size8x8_tex16x16_1_vtx);
            break;
        case 3:
            effSetVtxDescN64(size8x8_tex16x16_2_vtx);
            break;
        }
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
