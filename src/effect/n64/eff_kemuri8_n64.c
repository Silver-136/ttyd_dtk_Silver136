#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"


typedef struct EffKemuri8N64Work {
    u16 type;       // 0x0
    s16 pad_2;      // 0x2
    Vec position;   // 0x4
    f32 speed; // 0x10, along (sinAngle, cosAngle)
    f32 accel; // 0x14, added to speed each frame
    f32 accelStep; // 0x18, added to accel
    f32 accelStep2; // 0x1C, added to accelStep
    f32 field_0x20; // 0x20
    f32 field_0x24; // 0x24
    f32 field_0x28; // 0x28
    f32 field_0x2C; // 0x2C
    f32 sinAngle;   // 0x30
    f32 cosAngle;   // 0x34
    s32 field_0x38; // 0x38
    s32 timer;      // 0x3C
    s32 texFrame; // 0x40, current of 16 texture frames; >15 = done
    u8 r;           // 0x44
    u8 g;           // 0x45
    u8 b;           // 0x46
    u8 a;           // 0x47
    s32 texFrameNext; // 0x48
    s32 limit;      // 0x4C
    BOOL flag;      // 0x50
    s32 counter;    // 0x54
    s32 camId;      // 0x58
} EffKemuri8N64Work;

void effKemuri8Main(EffectEntry* effect);
void effKemuri8Disp(CameraId cameraId, EffectEntry* effect);

void effKemuri8N64SetCamId(EffectEntry* effect, s32 camId) {
    ((EffKemuri8N64Work*)effect->userdata)->camId = camId;
}

inline float inline_fn(f32 a, f32 b) { return a * (-b); }

#pragma opt_propagation off
EffectEntry* effKemuri8N64Entry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffKemuri8N64Work* work;
    s32 i;
    s32 count;
    BOOL flag;
    f32 radius;
    f32 rad;

    effect = effEntry();
    flag = FALSE;
    if (type >= 3) {
        flag = TRUE;
        type -= 3;
    }
    switch (type) {
    case 0:
        radius = 14.0f;
        count = 7;
        break;
    case 1:
        radius = 28.0f;
        count = 9;
        break;
    default:
        count = 11;
        radius = 42.0f;
        break;
    }
    effect->type = "Kemuri8N64";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffKemuri8N64Work) * count);
    effect->userdata = work;
    effect->callback = effKemuri8Main;
    work->field_0x38 = 0;
    work->timer = 0;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->texFrame = 0;
    work->texFrameNext = 0;
    work->limit = 17;
    work->camId = 4;
    work++;
    for (i = 1; i < count; i++, work++) {
        work->speed = -3.0f;
        work->accel = 0.19999999f;
        work->accelStep = 0.0f;
        work->accelStep2 = 0.0f;
        work->field_0x20 = 1.75f;
        work->field_0x24 = -0.16f;
        work->field_0x28 = 0.0f;
        work->field_0x2C = 0.0f;
        rad = 6.2831855f * (f32)(((i - 1) * 360) / (count - 1)) / 360.0f;
        work->position.x = inline_fn(radius, (f32)sin(rad));
        work->position.y = radius * -(f32)cos(rad);
        work->position.z = 0.0f;
        work->sinAngle = sin(rad);
        work->cosAngle = cos(rad);
        work->flag = flag;
        if (flag == 0) {
            work->r = 0xEE;
            work->g = 0xDE;
            work->b = 0xD7;
            work->a = 0xA0;
        } else {
            work->r = 0xFF;
            work->g = 0xFF;
            work->b = 0xFF;
            work->a = 0xA0;
            work->counter = 0;
        }
    }
    return effect;
}

#pragma opt_propagation reset
void effKemuri8Main(EffectEntry* effect) {
    EffKemuri8N64Work* work = effect->userdata;
    s32 i;
    s32 cur;
    s32 next;
    s32 camId;
    s32 t;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    t = work->timer;
    camId = work->camId;
    cur = 16.0f * (f32)t / (f32)work->limit;
    t++;
    next = 16.0f * (f32)t / (f32)work->limit;
    work->timer = t;
    if (next > 15) {
        next = 15;
    }
    if (cur > 15) {
        effDelete(effect);
        return;
    }
    work->texFrame = cur;
    work->texFrameNext = next;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        work->accelStep += work->accelStep2;
        work->accel += work->accelStep;
        work->speed += work->accel;
        work->position.x += work->speed * work->sinAngle;
        work->position.y += work->speed * work->cosAngle;
    }
    dispEntry(camId, dispCalcZ(&position), 2, (DispCallback)effKemuri8Disp, effect);
}

void effKemuri8Disp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffKemuri8N64Work* work;
    s32 type;
    s32 cur;
    s32 next;
    s32 i;
    s32 j;
    f32 u0;
    f32 u1;
    f32 hw;
    f32 w;
    f32 h;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx mtxB;
    GXTexObj texObj;

    work = effect->userdata;
    camera = camGetPtr(cameraId);
    type = work->type;
    cur = work->texFrame;
    next = work->texFrameNext;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, mtxB);
    effGetTexObj(0x28, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, (GXColor){0x80, 0x80, 0x80, 0x80});
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    hw = 0.5f;
    u0 = 0.0625f * (f32)cur;
    u1 = 0.0625f * (f32)next;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->texFrame > 15) {
            continue;
        }
        PSMTXTrans(mtx, u0, 0.0f, 0.0f);
        PSMTXScale(scale, 0.0625f, 1.0f, 1.0f);
        PSMTXConcat(mtx, scale, mtx);
        GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
        PSMTXTrans(mtx, u1, 0.0f, 0.0f);
        PSMTXScale(scale, 0.0625f, 1.0f, 1.0f);
        PSMTXConcat(mtx, scale, mtx);
        GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
        for (j = 0; j < 2; j++) {
            if (j == 0) {
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){0, 0, 0, work->a * 96 / 255});
                PSMTXTrans(mtx, 2.0f + work->position.x, work->position.y - 2.0f, work->position.z);
                PSMTXConcat(mtxB, mtx, mtx);
                GXLoadPosMtxImm(mtx, GX_PNMTX0);
            } else {
                if (work->flag) {
                    s32 c5;
                    s32 c6;
                    s32 c7;
                    c6 = 255;
                    c5 = 255;
                    c7 = 255 - work->counter * 60;
                    if (c7 < 0) {
                        c6 = 255 + (s32)(0.8f * (f32)c7);
                        c7 = 0;
                        if (c6 < 0) {
                            c5 = 255 + (s32)(0.4f * (f32)c6);
                            c6 = 0;
                            if (c5 < 0) {
                                c5 = 0;
                            }
                        }
                    }
                    work->r = c5;
                    work->g = c6;
                    work->b = c7;
                    work->counter++;
                }
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){work->r, work->g, work->b, work->a});
                PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
                PSMTXConcat(mtxB, mtx, mtx);
                GXLoadPosMtxImm(mtx, GX_PNMTX0);
            }
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            if (type != 0) {
                w = 32.0f;
                h = w;
            } else {
                w = 24.0f;
                h = w;
            }
            GXPosition3f32(-h * hw, w * hw, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(h * hw, w * hw, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(h * hw, -w * hw, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(-h * hw, -w * hw, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
        }
    }
}
