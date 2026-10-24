#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

double sin(double);

typedef struct EffStarPowerN64Work {
    s16 unk0;      // 0x0
    s16 id;        // 0x2
    Vec position;  // 0x4
    s32 lifetime;  // 0x10
    s32 frame;     // 0x14
    s32 c18;       // 0x18
    s32 c1c;       // 0x1C
    s32 c20;       // 0x20
    s32 c24;       // 0x24
    s32 c28;       // 0x28
    s32 c2c;       // 0x2C
    s32 c30;       // 0x30
    s32 c34;       // 0x34
    f32 scale;     // 0x38
    f32 rotX;      // 0x3C
    f32 rotY;      // 0x40
    f32 rotZ;      // 0x44
    f32 prevRotX;  // 0x48
    f32 prevRotY;  // 0x4C
    f32 prevRotZ;  // 0x50
    f32 prevScale; // 0x54
} EffStarPowerN64Work;

void effStarPowerMain(EffectEntry* effect);
void effStarPowerDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effStarPowerN64Entry(s32 id, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffStarPowerN64Work* work;

    effect = effEntry();
    effect->type = "StarPowerN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffStarPowerN64Work));
    effect->userdata = work;
    effect->callback = effStarPowerMain;
    effect->flags |= 2;
    work->unk0 = 1;
    work->id = id;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->c18 = 255;
    work->c1c = 255;
    work->c20 = 120;
    work->c28 = 255;
    work->c2c = 120;
    work->c30 = 0;
    work->c34 = 255;
    work->c24 = 255;
    work->rotX = 0.0f;
    work->rotY = 0.0f;
    work->rotZ = 0.0f;
    return effect;
}

void effStarPowerMain(EffectEntry* effect) {
    EffStarPowerN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 id;
    s32 frame;
    s32 life;
    f32 sn;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    id = work->id;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->lifetime = 16;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    life = work->lifetime;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    if (life < 16) {
        life = life * 16;
        if (work->c24 > life) {
            work->c24 = life;
        }
        if (work->c34 > life) {
            work->c34 = life;
        }
    }
    if (frame < 16) {
        life = frame * 16 + 15;
        if (work->c24 < life) {
            work->c24 = life;
        }
        if (work->c34 < life) {
            work->c34 = life;
        }
    }
    work->prevRotX = work->rotX;
    work->prevRotY = work->rotY;
    work->prevRotZ = work->rotZ;
    work->prevScale = work->scale;
    if (id == 1) {
        work->rotX = (f32)(frame * 4);
        work->rotY = 0.4953f * (4.0f * (f32)frame);
        work->rotZ = 0.2234f * (4.0f * (f32)frame);
        work->c24 = 255;
        sn = (f32)sin(6.2831855f * (7.12343f * (f32)frame) / 360.0f);
        work->c34 = (s32)(127.0f * sn + 128.0f) & 0xFF;
        sn = (f32)sin(6.2831855f * (1.231f * (f32)frame) / 360.0f);
        work->c18 = (s32)(127.0f * sn + 215.0f) & 0xFF;
        sn = (f32)sin(6.2831855f * (0.531f * (f32)frame) / 360.0f);
        work->c1c = (s32)(127.0f * sn + 215.0f) & 0xFF;
        sn = (f32)sin(6.2831855f * (3.231f * (f32)frame) / 360.0f);
        work->c20 = (s32)(127.0f * sn + 215.0f) & 0xFF;
        sn = (f32)sin(6.2831855f * (0.298f * (f32)frame) / 360.0f);
        work->c28 = (s32)(127.0f * sn + 188.0f) & 0xFF;
        sn = (f32)sin(6.2831855f * (0.831f * (f32)frame) / 360.0f);
        work->c2c = (s32)(127.0f * sn + 188.0f) & 0xFF;
        sn = (f32)sin(6.2831855f * (2.231f * (f32)frame) / 360.0f);
        work->c30 = (s32)(127.0f * sn + 188.0f) & 0xFF;
        sn = (f32)sin(6.2831855f * (2.044f * (f32)frame) / 360.0f);
        work->scale = 0.7f + 0.3f * sn;
    }
    if (frame == 1) {
        work->prevRotX = work->rotX;
        work->prevRotY = work->rotY;
        work->prevRotZ = work->rotZ;
        work->prevScale = work->scale;
    }
    dispEntry(4, dispCalcZ(&position), 2, (DispCallback)effStarPowerDisp, effect);
}

effN64Vtx testsrc_akari_v[] = {
    {147, -202, 0, 0, 0, 255, 255, 255, 255},
    {143, -46, 0, 0, 0, 0, 0, 0, 255},
    {133, -43, 0, 0, 0, 0, 0, 0, 255},
    {136, -187, 0, 0, 0, 255, 255, 255, 255},
    {0, -150, 0, 0, 0, 0, 0, 0, 255},
    {0, -140, 0, 0, 0, 0, 0, 0, 255},
    {238, 77, 0, 0, 0, 255, 255, 255, 255},
    {220, 72, 0, 0, 0, 255, 255, 255, 255},
    {88, 121, 0, 0, 0, 0, 0, 0, 255},
    {82, 113, 0, 0, 0, 0, 0, 0, 255},
    {0, 232, 0, 0, 0, 255, 255, 255, 255},
    {0, 250, 0, 0, 0, 255, 255, 255, 255},
    {-88, 121, 0, 0, 0, 0, 0, 0, 255},
    {-82, 113, 0, 0, 0, 0, 0, 0, 255},
    {-147, -202, 0, 0, 0, 255, 255, 255, 255},
    {-136, -187, 0, 0, 0, 255, 255, 255, 255},
    {-238, 77, 0, 0, 0, 255, 255, 255, 255},
    {-220, 72, 0, 0, 0, 255, 255, 255, 255},
    {-143, -46, 0, 0, 0, 0, 0, 0, 255},
    {-133, -43, 0, 0, 0, 0, 0, 0, 255},
    {168, -231, 0, 1659, -2, 255, 255, 255, 255},
    {163, -53, 0, 1641, 698, 0, 0, 0, 255},
    {107, -35, 0, 1428, 770, 0, 0, 0, 255},
    {109, -150, 0, 1437, 316, 255, 255, 255, 255},
    {0, -171, 0, 1024, 232, 0, 0, 0, 255},
    {0, -112, 0, 1024, 464, 0, 0, 0, 255},
    {271, 88, 0, 2052, 1252, 255, 255, 255, 255},
    {176, 57, 0, 1693, 1132, 255, 255, 255, 255},
    {101, 138, 0, 1405, 1450, 0, 0, 0, 255},
    {66, 91, 0, 1274, 1262, 0, 0, 0, 255},
    {0, 185, 0, 1024, 1636, 255, 255, 255, 255},
    {0, 285, 0, 1024, 2028, 255, 255, 255, 255},
    {-101, 138, 0, 643, 1450, 0, 0, 0, 255},
    {-66, 91, 0, 774, 1262, 0, 0, 0, 255},
    {-168, -231, 0, 389, -2, 255, 255, 255, 255},
    {-109, -150, 0, 611, 316, 255, 255, 255, 255},
    {-271, 88, 0, -4, 1252, 255, 255, 255, 255},
    {-176, 57, 0, 355, 1132, 255, 255, 255, 255},
    {-163, -53, 0, 407, 698, 0, 0, 0, 255},
    {-107, -35, 0, 620, 770, 0, 0, 0, 255},
};

void effStarPowerDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    s32 half;
    EffStarPowerN64Work* work;
    s32 c18, c1c, c20, c24, c28, c2c, c30, c34;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx tmp;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    c18 = work->c18;
    c1c = work->c1c;
    c20 = work->c20;
    c24 = work->c24;
    c28 = work->c28;
    c2c = work->c2c;
    c30 = work->c30;
    c34 = work->c34;
    if (c18 > 255) {
        c18 = 255;
    }
    if (c1c > 255) {
        c1c = 255;
    }
    if (c20 > 255) {
        c20 = 255;
    }
    if (c24 > 255) {
        c24 = 255;
    }
    if (c28 > 255) {
        c28 = 255;
    }
    if (c2c > 255) {
        c2c = 255;
    }
    if (c30 > 255) {
        c30 = 255;
    }
    if (c34 > 255) {
        c34 = 255;
    }
    if (c18 < 0) {
        c18 = 0;
    }
    if (c1c < 0) {
        c1c = 0;
    }
    if (c20 < 0) {
        c20 = 0;
    }
    if (c24 < 0) {
        c24 = 0;
    }
    if (c28 < 0) {
        c28 = 0;
    }
    if (c2c < 0) {
        c2c = 0;
    }
    if (c30 < 0) {
        c30 = 0;
    }
    if (c34 < 0) {
        c34 = 0;
    }
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.015625f, 0.015625f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x49, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    if (c34 != 0) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
        PSMTXScale(scale, 0.4f * work->prevScale, 0.4f * work->prevScale, 0.4f * work->prevScale);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXRotRad(rot, 'x', 0.017453292f * work->prevRotX);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXRotRad(rot, 'y', 0.017453292f * work->prevRotY);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->prevRotZ);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(camera->view, mtx, tmp);
        GXLoadPosMtxImm(tmp, GX_PNMTX0);
        GXSetCurrentMtx(0);
        half = c34 >> 1;
        GXSetTevColor(GX_TEVREG0, (GXColor){c18, c1c, c20, half});
        GXSetTevColor(GX_TEVREG1, (GXColor){c28, c2c, c30, half});
        GXSetNumChans(1);
        GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_RASC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        effSetVtxDescN64(&testsrc_akari_v[20]);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 60);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
        tri2(4, 0, 3, 0, 4, 3, 5, 0);
        tri2(1, 6, 7, 0, 1, 7, 2, 0);
        tri2(6, 8, 9, 0, 6, 9, 7, 0);
        tri2(10, 9, 8, 0, 10, 8, 11, 0);
        tri2(11, 12, 13, 0, 11, 13, 10, 0);
        tri2(14, 4, 5, 0, 14, 5, 15, 0);
        tri2(12, 16, 17, 0, 12, 17, 13, 0);
        tri2(16, 18, 19, 0, 16, 19, 17, 0);
        tri2(18, 14, 15, 0, 18, 15, 19, 0);
    }
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scale, 0.4f * work->scale, 0.4f * work->scale, 0.4f * work->scale);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXRotRad(rot, 'x', 0.017453292f * work->rotX);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXRotRad(rot, 'y', 0.017453292f * work->rotY);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXRotRad(rot, 'z', 0.017453292f * work->rotZ);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(camera->view, mtx, tmp);
    GXLoadPosMtxImm(tmp, GX_PNMTX0);
    GXSetCurrentMtx(0);
    if (c34 != 0) {
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 60);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
        tri2(4, 0, 3, 0, 4, 3, 5, 0);
        tri2(1, 6, 7, 0, 1, 7, 2, 0);
        tri2(6, 8, 9, 0, 6, 9, 7, 0);
        tri2(10, 9, 8, 0, 10, 8, 11, 0);
        tri2(11, 12, 13, 0, 11, 13, 10, 0);
        tri2(14, 4, 5, 0, 14, 5, 15, 0);
        tri2(12, 16, 17, 0, 12, 17, 13, 0);
        tri2(16, 18, 19, 0, 16, 19, 17, 0);
        tri2(18, 14, 15, 0, 18, 15, 19, 0);
    }
    GXSetTevColor(GX_TEVREG0, (GXColor){c18, c1c, c20, c24});
    GXSetTevColor(GX_TEVREG1, (GXColor){c28, c2c, c30, c24});
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    effSetVtxDescN64(testsrc_akari_v);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 60);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
    tri2(4, 0, 3, 0, 4, 3, 5, 0);
    tri2(1, 6, 7, 0, 1, 7, 2, 0);
    tri2(6, 8, 9, 0, 6, 9, 7, 0);
    tri2(10, 9, 8, 0, 10, 8, 11, 0);
    tri2(11, 12, 13, 0, 11, 13, 10, 0);
    tri2(14, 4, 5, 0, 14, 5, 15, 0);
    tri2(12, 16, 17, 0, 12, 17, 13, 0);
    tri2(16, 18, 19, 0, 16, 19, 17, 0);
    tri2(18, 14, 15, 0, 18, 15, 19, 0);
}
