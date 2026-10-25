#include "eff/n64/eff_expbom_n64.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "driver/camdrv.h"
#include "eff/eff_n64.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

typedef struct effExpBomData {
    s16 age; //0x0, frames shown
    s16 lifeLeft; //0x2, counts down from lifetime; <0 = done
    s16 lifetime; //0x4, rand 16..21
    s16 slowDelay; //0x6, frames before velocity starts decaying (/1.25)
    s32 startDelay; //0x8
    Vec position; //0xC
    f32 offsetX; //0x18
    f32 offsetY; //0x1C
    f32 velX; //0x20, from bom_dir/bom_spd
    f32 velY; //0x24
    u8 field_0x28[0x2C - 0x28]; //0x28
} effExpBomData;

//.sdata
static f32 bom_dir[10] = {
    10.0f, 40.0f, 80.0f, 170.0f, 140.0f,
    100.0f, 25.0f, 155.0f, 60.0f, 120.0f
};

static f32 bom_spd[] = {
    2.2f, 2.7f, 3.0f, 2.2f, 2.7f,
    3.0f, 1.9f, 1.9f, 1.5f, 1.5f
};

f32 bom_scale[10] = {
    1.4f, 1.3f, 1.2f, 1.3f, 1.4f,
    1.3f, 1.6f, 1.6f, 1.6f, 1.6f
};

f32 bom_ry[10] = {
    0.0f, 234.0f, 468.0f, 702.0f, 936.0f,
    1260.0f, 1404.0f, 1638.0f, 1902.0f, 1976.0f
};

//local prototypes
void effExpBomMain(EffectEntry* effect);
void effExpBomDisp(CameraId camId, void* param);

EffectEntry* effExpBomN64Entry(f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    f32 sv, cv;
    f32 rad, spd;
    f32* direction;
    f32* speed;
    effExpBomData* data;
    s16 random;
    int i;

    effect = effEntry();
    effect->type = "ExpBomN64";
    effect->count = 10;
    data = __memAlloc(HEAP_EFFECT, sizeof(effExpBomData) * 10);
    effect->userdata = data;
    effect->callback = effExpBomMain;
    for (i = 0, direction = bom_dir, speed = bom_spd; i < effect->count; direction++, speed++, i++, data++) {
        data->age = 0;
        random = rand() % 6 + 16;
        data->lifetime = random;
        data->lifeLeft = random;
        data->slowDelay = 4;
        data->startDelay = 0;
        data->position.x = x;
        data->position.y = y;
        data->position.z = z;
        data->offsetX = 0.0f;
        data->offsetY = 0.0f;

        spd = 2.0f * *speed;
        rad = ((6.2831855f * (90.0f + *direction)) / 360.0f);
        sv = sin(rad);
        cv = cos(rad);
        data->velX = spd * sv;
        data->velY = -spd * cv;
    }
    return effect;
}

void effExpBomMain(EffectEntry* effect) {
    effExpBomData* data;
    int i;
    BOOL disp;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    disp = FALSE;
    data = effect->userdata;
    tmp.x = data->position.x;
    tmp.y = data->position.y;
    tmp.z = data->position.z;
    position = tmp;

    for (i = 0; i < effect->count; i++, data++) {
        if (data->lifeLeft >= 0) {
            data->startDelay--;
            if (data->startDelay < 0) {
                disp = TRUE;
                data->offsetX += data->velX;
                data->offsetY += data->velY;
                data->age++;
                data->lifeLeft--;
                if (data->lifeLeft >= 0) {
                    data->slowDelay--;
                    if (data->slowDelay < 0) {
                        data->velX /= 1.25f;
                        data->velY /= 1.25f;
                    }
                }
            }
        }
    }
    if (!disp) {
        effDelete(effect);
    } else {
        dispEntry(CAMERA_3D, dispCalcZ(&position), 2, effExpBomDisp, effect);
    }
}

void effExpBomDisp(CameraId camId, void* param) {
    f32* scl;
    f32* ry;
    effExpBomData* data;
    int n;
    int i;
    int tex;
    CameraEntry* camera;
    int alpha;
    f32 a, b, t;
    Mtx mtx, rot, scale, base;
    GXTexObj texObj;

    camera = camGetPtr(camId);
    data = ((EffectEntry*)param)->userdata;
    PSMTXTrans(mtx, data->position.x, data->position.y, data->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(camId)->viewYaw);
    PSMTXConcat(mtx, rot, base);
    PSMTXConcat(camera->view, base, base);
    for (i = 0, scl = bom_scale, ry = bom_ry; i < ((EffectEntry*)param)->count; scl++, ry++, i++, data++) {
        if (data->lifeLeft < 0 || data->startDelay >= 0) continue;
        n = data->lifeLeft;
        if (n > 16) n = 16;
        PSMTXTrans(mtx, data->offsetX, data->offsetY, 0.0f);
        PSMTXScale(scale, *scl, *scl, *scl);
        PSMTXRotRad(rot, 'z', 0.017453292f * *ry);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(mtx, rot, rot);
        PSMTXConcat(base, rot, rot);
        GXLoadPosMtxImm(rot, GX_PNMTX0);
        a = (f32)data->age - 1.0f;
        b = (f32)data->lifetime;
        if (a >= b) {
            alpha = 255;
            tex = 6;
        } else {
            t = (f32)sin((6.2831855f * ((90.0f * ((7.0f * a) / b)) / 7.0f)) / 360.0f);
            t = 7.0f * t;
            alpha = (s32)(255.0f * t) % 256;
            tex = (s32)t;
        }
        GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, alpha});
        GXSetTevColor(GX_TEVREG1, (GXColor){0, 0, 0, (n * 210) >> 4});
        GXSetNumChans(0);
        GXSetNumTexGens(2);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetNumTevStages(3);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_A0, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_A1, GX_CA_ZERO);
        GXSetCullMode(GX_CULL_NONE);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        switch (tex) {
            case 0:
                effGetTexObjN64(0, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                effGetTexObjN64(1, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP1);
                break;
            case 1:
                effGetTexObjN64(1, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                effGetTexObjN64(2, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP1);
                break;
            case 2:
                effGetTexObjN64(2, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                effGetTexObjN64(3, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP1);
                break;
            case 3:
                effGetTexObjN64(3, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                effGetTexObjN64(4, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP1);
                break;
            case 4:
                effGetTexObjN64(4, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                effGetTexObjN64(5, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP1);
                break;
            case 5:
                effGetTexObjN64(5, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                effGetTexObjN64(6, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP1);
                break;
            case 6:
                effGetTexObjN64(6, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                effGetTexObjN64(6, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP1);
                break;
        }
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
