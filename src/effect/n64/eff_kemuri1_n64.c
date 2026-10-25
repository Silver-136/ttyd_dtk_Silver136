#include "eff/n64/eff_kemuri1_n64.h"
#include "memory.h"
#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "system.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "driver/dispdrv.h"

// One entry per element: entry 0 is the effect header (type, position, frame counter, duration, texture frames),
// entries 1.. are the puffs (position, speeds, direction, colour).
typedef struct EffKemuri1Work {
    u16 type;           // 0x0
    u8 pad_2[2];        // 0x2
    Vec position;       // 0x4
    f32 hSpeed;         // 0x10, along (dirX, dirZ)
    f32 f14, f18, f1C;  // 0x14, per type: horizontal speed factors
    f32 vSpeed;         // 0x20
    f32 f24, f28, f2C;  // 0x24, per type: vertical speed factors
    f32 dirX;           // 0x30, sin of the puff's direction
    f32 dirZ;           // 0x34, cos of the puff's direction
    s32 field_0x38;     // 0x38, header: 0/3/2 by type, never read
    s32 frame;          // 0x3C, header: frames elapsed
    s32 animFrame;      // 0x40, header: texture frame (0..15); puffs with one above 15 are not drawn
    u8 r, g, b;         // 0x44, puff colour
    u8 pad_47;          // 0x47
    s32 nextAnimFrame;  // 0x48, header: texture frame of the next frame
    s32 duration;       // 0x4C, header: frames for the 16 texture frames
} EffKemuri1Work;

//local prototypes
void effKemuri1Main(EffectEntry* effect);

EffectEntry* effKemuri1N64Entry(s32 type, f32 x, f32 y, f32 z, f32 angleOfs) {

    EffectEntry* effect;
    EffKemuri1Work* data;
    s32 effCount;
    s32 i;
    f32 off; f32 angle; f32 tmp; f32 angle2;

    effect = effEntry();
    switch (type) {
        case 2:
            effCount = 1;
            break;
        case 3:
            effCount = 4;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            effCount = 9;
            break;
        default:
            effCount = 3;
            break;
    }

    effect->type = "Kemuri1N64";
    effect->count = effCount;
    data = __memAlloc(HEAP_EFFECT, sizeof(EffKemuri1Work) * effCount);
    effect->userdata = data;
    effect->callback = effKemuri1Main;
    switch (type) {
        case 0:
        case 1:
            data->field_0x38 = 0;
            data->duration = 15;
            break;
        case 2:
            data->field_0x38 = 3;
            data->duration = 65;
            break;
        case 3:
            data->field_0x38 = 2;
            data->duration = 129;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            data->field_0x38 = 3;
            data->duration = 65;
            break;
    }
    data->frame = 0;
    data->type = type;
    data->position.x = x;
    data->position.y = y;
    data->position.z = z;
    data++;
    switch (type) {
        case 0:
        case 1:
            for (i = 1; i < effCount; i++, data++) {
                data->hSpeed = -3.0f;
                data->f14 = 0.29999998f;
                data->f18 = 0.0f;
                data->f1C = 0.0f;
                data->vSpeed = 1.75f;
                data->f24 = -0.16f;
                data->f28 = 0.0f;
                data->f2C = 0.0f;
                data->position.x = 0.0f;
                data->position.y = 0.0f;
                data->position.z = 4.0f;
                angle = reviseAngle(90.0f + 180.0f * (i - 1));
                angle = 6.2831855f * angle / 360.0f;
                data->dirX = sin(angle);
                data->dirZ = cos(angle);
                data->r = 0xFF;
                data->g = 0xFF;
                data->b = 0xFF;
            }
            break;
        case 3:
            for (i = 1; i < effCount; i++, data++) {
                switch (i - 1) {
                    case 0:
                        angle = 2.3561945f;
                        tmp = sin(angle); data->position.x = 25.0f * -tmp;
                        data->position.y = 0.0f;
                        tmp = cos(angle); data->position.z = 25.0f * -tmp;
                        data->vSpeed = 0.95f;
                        data->f24 = -0.04f;
                        data->f28 = 0.94f;
                        break;
                    case 1:
                        angle = -2.3561945f;
                        tmp = sin(angle); data->position.x = 25.0f * -tmp;
                        data->position.y = 4.0f;
                        tmp = cos(angle); data->position.z = 25.0f * -tmp;
                        data->vSpeed = 0.84999996f;
                        data->f24 = -0.04f;
                        data->f28 = 0.94f;
                        break;
                    case 2:
                        angle = 0.34906587f;
                        tmp = sin(angle); data->position.x = 25.0f * -tmp;
                        data->position.y = 10.0f;
                        tmp = cos(angle); data->position.z = 25.0f * -tmp;
                        data->vSpeed = 0.75f;
                        data->f24 = -0.04f;
                        data->f28 = 0.94f;
                        break;
                }
                data->dirX = 0.0f;
                data->dirZ = 0.0f;
                data->r = 0xFF;
                data->g = 0xFF;
                data->b = 0xFF;
            }
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            for (i = 1; i < effCount; i++, data++) {
                data->hSpeed = -3.0f;
                data->f14 = 0.93f;
                data->vSpeed = 0.0f;
                data->f24 = 0.099999994f;
                data->f28 = 0.94f;
                data->f2C = 0.94f;
                angle2 = (angleOfs + (f32)(((i - 1) * 360) / (effCount - 1))) * 6.2831855f / 360.0f;
                tmp = sin(angle2);
                data->position.x = 10.0f * -tmp;
                data->position.y = 0.0f;
                tmp = cos(angle2);
                data->position.z = 10.0f * -tmp;
                tmp = sin(angle2);
                data->dirX = tmp;
                tmp = cos(angle2);
                data->dirZ = tmp;
                switch (type) {
                    case 4:
                    case 7:
                        data->r = 0xFF;
                        data->g = 0xFF;
                        data->b = 0xFF;
                        break;
                    case 5:
                        data->r = 0xDD;
                        data->g = 0xB7;
                        data->b = 0x05;
                        break;
                    case 6:
                        data->r = 0x21;
                        data->g = 0x8C;
                        data->b = 0x8D;
                        break;
                }
            }
            break;
    }
    return effect;
}

void effKemuri1Disp(CameraId cameraId, EffectEntry* effect);

void effKemuri1Main(EffectEntry* effect) {
    EffKemuri1Work* work = effect->userdata;
    Vec pos2;
    Vec pos = {0.0f, 0.0f, 0.0f};
    s32 type, i, t, b, a;

    pos.x = work->position.x;
    pos.y = work->position.y;
    pos.z = work->position.z;
    pos2 = pos;

    t = work->frame;
    type = work->type;
    b = 16.0f * t / work->duration;
    a = 16.0f * (t + 1) / work->duration;
    work->frame = t + 1;
    if (a > 15) {
        a = 15;
    }
    if (b > 15) {
        effDelete(effect);
        return;
    }
    work->animFrame = b;
    work->nextAnimFrame = a;
    for (i = 1, work++; i < effect->count; i++, work++) {
        switch (type) {
            case 0:
            case 1:
                work->f18 += work->f1C;
                work->f14 += work->f18;
                work->hSpeed += work->f14;
                work->position.x += work->hSpeed * work->dirX;
                work->position.z += work->hSpeed * work->dirZ;
                work->f28 += work->f2C;
                work->f24 += work->f28;
                work->vSpeed += work->f24;
                work->position.y += work->vSpeed;
                break;
            case 3:
                work->position.y += work->vSpeed;
                work->vSpeed += work->f24;
                work->f24 *= work->f28;
                break;
            case 4:
            case 5:
            case 6:
            case 7:
                work->position.x += work->hSpeed * work->dirX;
                work->position.z += work->hSpeed * work->dirZ;
                work->hSpeed *= work->f14;
                work->position.y += work->vSpeed;
                work->vSpeed += work->f24;
                work->f24 *= work->f28;
                work->vSpeed *= work->f2C;
                break;
        }
    }
    if (effect->count - 1 <= 0) {
        effDelete(effect);
        return;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&pos2), 2, (DispCallback)effKemuri1Disp, effect);
}

void effKemuri1Disp(CameraId cameraId, EffectEntry* effect) {
    Mtx mtx, scale, rot, view;
    GXTexObj tex;
    EffKemuri1Work* work;
    CameraEntry* camera;
    s32 type;
    s32 i, j;
    f32 s, ofsS, ofsT;
    s32 a, b;

    work = effect->userdata;
    camera = camGetPtr(cameraId);
    type = work->type;
    a = work->animFrame;
    b = work->nextAnimFrame;
    s = 1.0f;
    switch (type) {
        case 5:
        case 6:
        case 7:
            s = 0.5f;
            break;
    }
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXScale(scale, s, s, s);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, view);
    effGetTexObj(0x28, &tex);
    GXLoadTexObj(&tex, GX_TEXMAP0);
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
    ofsS = 0.0625f * a;
    ofsT = 0.0625f * b;
    for (i = 1, work++; i < effect->count; i++, work++) {
        f32 h;
        f32 w;
        if (work->animFrame > 15) {
            continue;
        }
        PSMTXTrans(mtx, ofsS, 0.0f, 0.0f);
        PSMTXScale(scale, 0.0625f, 1.0f, 1.0f);
        PSMTXConcat(mtx, scale, mtx);
        GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
        PSMTXTrans(mtx, ofsT, 0.0f, 0.0f);
        PSMTXScale(scale, 0.0625f, 1.0f, 1.0f);
        PSMTXConcat(mtx, scale, mtx);
        GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
        s = 0.5f;
        for (j = 0; j < 2; j++) {
            if (j == 0) {
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){0, 0, 0, 0x60});
                PSMTXTrans(mtx, work->position.x + 2.0f, work->position.y - 2.0f, work->position.z);
                PSMTXConcat(view, mtx, mtx);
                GXLoadPosMtxImm(mtx, GX_PNMTX0);
            } else {
                { GXColor* c; GXSetChanMatColor(GX_COLOR0A0, *(c = &(GXColor){0, 0, 0, 0}, c->r = work->r, c->g = work->g, c->b = work->b, c->a = 0xFF, c)); }
                PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
                PSMTXConcat(view, mtx, mtx);
                GXLoadPosMtxImm(mtx, GX_PNMTX0);
            }
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            switch (type) {
                case 0:
                    w = h = 16.0f;
                    break;
                case 1:
                    w = h = 24.0f;
                    break;
                default:
                    w = h = 32.0f;
                    break;
            }
            GXPosition3f32(-w * s, h * s, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(w * s, h * s, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(w * s, -h * s, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(-w * s, -h * s, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
        }
    }
}

