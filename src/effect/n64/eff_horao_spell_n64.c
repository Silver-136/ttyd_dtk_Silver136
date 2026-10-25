#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os/OSFastCast.h>
#include <string.h>
#include "eff/eff_n64.h"

double sin(double);



typedef struct EffHoraoSpellN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    s32 lifetime; // 0x10
    s32 frame;    // 0x14
    s32 colorR;   // 0x18
    s32 colorG;   // 0x1C
    s32 colorB;   // 0x20
    s32 alpha;    // 0x24
    s32 color2R;  // 0x28
    s32 color2G;  // 0x2C
    s32 color2B;  // 0x30
    s32 color2A;  // 0x34
    f32 scale;    // 0x38
} EffHoraoSpellN64Work;

effN64Vtx horao_spell_vtx[] = {
    {0, -8, 0, 0, 0, 0, 0, 0, 0},
    {0, 8, 0, 0, 1024, 0, 0, 0, 0},
    {0, -8, 0, 512, 0, 0, 0, 0, 50},
    {0, 8, 0, 512, 1024, 0, 0, 0, 50},
    {0, -8, 0, 1024, 0, 0, 0, 0, 100},
    {0, 8, 0, 1024, 1024, 0, 0, 0, 100},
    {0, -8, 0, 1536, 0, 0, 0, 0, 150},
    {0, 8, 0, 1536, 1024, 0, 0, 0, 150},
    {0, -8, 0, 2048, 0, 0, 0, 0, 200},
    {0, 8, 0, 2048, 1024, 0, 0, 0, 200},
    {0, -8, 0, 2560, 0, 0, 0, 0, 250},
    {0, 8, 0, 2560, 1024, 0, 0, 0, 250},
    {0, -8, 0, 3072, 0, 0, 0, 0, 255},
    {0, 8, 0, 3072, 1024, 0, 0, 0, 255},
    {0, -8, 0, 3584, 0, 0, 0, 0, 255},
    {0, 8, 0, 3584, 1024, 0, 0, 0, 255},
    {0, -8, 0, 4096, 0, 0, 0, 0, 255},
    {0, 8, 0, 4096, 1024, 0, 0, 0, 255},
    {0, -8, 0, 4608, 0, 0, 0, 0, 255},
    {0, 8, 0, 4608, 1024, 0, 0, 0, 255},
    {0, -8, 0, 5120, 0, 0, 0, 0, 255},
    {0, 8, 0, 5120, 1024, 0, 0, 0, 255},
    {0, -8, 0, 5632, 0, 0, 0, 0, 255},
    {0, 8, 0, 5632, 1024, 0, 0, 0, 255},
    {0, -8, 0, 6144, 0, 0, 0, 0, 255},
    {0, 8, 0, 6144, 1024, 0, 0, 0, 255},
    {0, -8, 0, 6656, 0, 0, 0, 0, 200},
    {0, 8, 0, 6656, 1024, 0, 0, 0, 200},
    {0, -8, 0, 7168, 0, 0, 0, 0, 100},
    {0, 8, 0, 7168, 1024, 0, 0, 0, 100},
    {0, -8, 0, 7680, 0, 0, 0, 0, 0},
    {0, 8, 0, 7680, 1024, 0, 0, 0, 0},
};

static u8 colr[17] = {0x78, 0x82, 0x8C, 0x96, 0xA0, 0xAA, 0xB4, 0xBE, 0xC8, 0xC8, 0xBE, 0xB4, 0xAA, 0xA0, 0x96, 0x8C, 0x82};
static u8 colg[17] = {0xAA, 0xA0, 0x96, 0x8C, 0x82, 0x78, 0x82, 0x8C, 0x96, 0xA0, 0xAA, 0xB4, 0xBE, 0xC8, 0xC8, 0xBE, 0xB4};
static u8 colb[17] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void effHoraoSpellMain(EffectEntry* effect);
void effHoraoSpellDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effHoraoSpellN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffHoraoSpellN64Work* work;

    effect = effEntry();
    effect->type = "HoraoSpellN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffHoraoSpellN64Work));
    effect->userdata = work;
    effect->callback = effHoraoSpellMain;
    effect->flags |= 2;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->alpha = 0;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->colorR = 70;
    work->colorG = 180;
    work->colorB = 120;
    work->color2R = 20;
    work->color2G = 230;
    work->color2B = 50;
    work->color2A = 255;
    return effect;
}

void effHoraoSpellMain(EffectEntry* effect) {
    EffHoraoSpellN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & 4) {
        effect->flags &= ~4;
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
    {
    int frame;
    frame = work->frame;
    if (work->lifetime < 16) {
        work->alpha = work->lifetime * 16;
    }
    if (frame < 16) {
        work->alpha = frame * 16 + 15;
    }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effHoraoSpellDisp, effect);
}

void effHoraoSpellDisp(CameraId cameraId, EffectEntry* effect) {
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx base;
    Mtx mtx2;
    Vec v1;
    Vec v2;
    effN64Vtx* p;
    effN64Vtx* q;
    EffHoraoSpellN64Work* work;
    s32 alpha;
    s32 frame;
    s32 k;
    s32 i;
    s32 j;
    effN64Vtx* vtx;
    CameraEntry* camera;


    f32 s;
    camera = camGetPtr(cameraId);
    work = effect->userdata;
    frame = work->frame;
    alpha = work->alpha;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, base);

    GXSetTevColor(GX_TEVREG0, (GXColor){work->colorR, work->colorG, work->colorB, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->color2R, work->color2G, work->color2B, work->color2A});
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_RASA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C0, GX_CC_C1, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    effGetTexObjN64(0x73, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.0078125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    GXSetCullMode(GX_CULL_NONE);
    for (i = 0; i < 2; i++) {
        k = (frame + i * 4) % 17;
        PSMTXRotRad(rot, 'z', 0.017453292f * (-282 + i * 36));
        PSMTXConcat(base, rot, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetTevColor(GX_TEVREG0, (GXColor){colr[k], colg[k], colb[k], work->alpha});
        GXSetTevColor(GX_TEVREG1, (GXColor){255, 255, 255, 0});
        PSMTXScale(scale, 0.0078125f, 0.03125f, 0.0f);
        PSMTXTrans(mtx, (f32)(-frame * 2), 0.0f, 0.0f);
        PSMTXConcat(scale, mtx, scale);
        GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
        PSMTXRotRad(rot, 'x', 0.34906584f);
        PSMTXTrans(mtx, 6.0f, 0.0f, 0.0f);
        PSMTXConcat(rot, mtx, mtx);
        s = sin(6.2831855f * (78.75f + (f32)(frame * 8)) / 360.0f);
        PSMTXRotRad(rot, 'y', 0.017453292f * (-30.0f * s));
        PSMTXConcat(mtx, rot, mtx2);
        vtx = (effN64Vtx*)smartAlloc(sizeof(effN64Vtx) * 32, 3)->address;
        for (j = 0; j < 16; j++) {
            k = j * 2;
            PSMTXTrans(mtx, 8.0f, 0.0f, 0.0f);
            s = sin(6.2831855f * (f32)(frame * 8 + (j * 2 * 360) / 15) / 360.0f);
            PSMTXRotRad(rot, 'y', 0.017453292f * (30.0f * s));
            PSMTXConcat(mtx, rot, mtx);
            PSMTXRotRad(rot, 'z', 0.10471976f);
            PSMTXConcat(mtx, rot, mtx);
            s = 0.01f * (f32)j + 1.0f;
            PSMTXScale(scale, s, s, s);
            PSMTXConcat(mtx, scale, mtx);
            PSMTXConcat(mtx2, mtx, mtx2);
            p = &vtx[k];
            memcpy(p, &horao_spell_vtx[k], sizeof(effN64Vtx));

            OSInitFastCast();
            OSs16tof32(&p->x, &v1.x);
            OSs16tof32(&p->y, &v1.y);
            OSs16tof32(&p->z, &v1.z);
            PSMTXMultVec(mtx2, &v1, &v1);
            OSInitFastCast();
            OSf32tos16(&v1.x, &p->x);
            OSf32tos16(&v1.y, &p->y);
            OSf32tos16(&v1.z, &p->z);
            q = &vtx[k + 1];
            memcpy(q, &horao_spell_vtx[k + 1], sizeof(effN64Vtx));

            OSInitFastCast();
            OSs16tof32(&q->x, &v2.x);
            OSs16tof32(&q->y, &v2.y);
            OSs16tof32(&q->z, &v2.z);
            PSMTXMultVec(mtx2, &v2, &v2);
            OSInitFastCast();
            OSf32tos16(&v2.x, &q->x);
            OSf32tos16(&v2.y, &p[1].y);
            OSf32tos16(&v2.z, &p[1].z);
        }
        DCFlushRange(vtx, sizeof(effN64Vtx) * 32);
        GXInvalidateVtxCache();
        effSetVtxDescN64(vtx);
        for (j = 0, k = 0; j < 15; j++, k += 2) {
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(k, k + 2, k + 1, k, k + 1, k + 2, k + 3, k);
        }

    }
}

