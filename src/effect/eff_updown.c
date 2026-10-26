#include "icon_ids.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/eff_magic1_n64.h"
#include "eff/eff_updown.h"
#include "driver/icondrv.h"

typedef struct EffUpdownWork {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 scaleX;   // 0x10
    f32 scaleY;   // 0x14
    s32 number;   // 0x18
    s32 timer;    // 0x1C
    u32 frame;    // 0x20
    u8 alpha;     // 0x24
} EffUpdownWork;

void effUpdownMain(EffectEntry* effect);
void effUpdownDisp(CameraId cameraId, EffectEntry* effect);

static u16 icon_id[10] = { ICON_NUMBER_SMALL_0, ICON_NUMBER_SMALL_1, ICON_NUMBER_SMALL_2, ICON_NUMBER_SMALL_3, ICON_NUMBER_SMALL_4, ICON_NUMBER_SMALL_5, ICON_NUMBER_SMALL_6, ICON_NUMBER_SMALL_7, ICON_NUMBER_SMALL_8, ICON_NUMBER_SMALL_9 };

static u8 scale_data[30] = {
    0x78, 0x00, 0x73, 0x0D, 0x6E, 0x1C, 0x68, 0x2C, 0x63, 0x3B, 0x5E, 0x4A, 0x59, 0x58, 0x55, 0x64,
    0x52, 0x6F, 0x50, 0x78, 0x50, 0x7B, 0x54, 0x78, 0x59, 0x71, 0x5F, 0x69, 0x64, 0x64,
};

EffectEntry* effUpdownEntry(s32 type, f32 x, f32 y, f32 z, s32 number, s32 timer) {
    EffectEntry* effect;
    EffUpdownWork* work;

    effect = effEntry();
    effect->type = "Updown";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffUpdownWork));
    effect->userdata = work;
    effect->callback = effUpdownMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scaleY = 1.0f;
    work->scaleX = 1.0f;
    work->number = number;
    work->timer = timer;
    work->frame = 0;
    work->alpha = 0xFF;
    return effect;
}

void effUpdownMain(EffectEntry* effect) {
    EffUpdownWork* work = effect->userdata;
    s32 type = work->type;
    s32 mode;

    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->timer = 16;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    if (work->timer < 16) {
        work->alpha = work->timer * 16;
    }
    if (type == 0 || type == 1) {
        if (work->frame < 15) {
            work->scaleX = scale_data[work->frame * 2] / 100.0f;
            work->scaleY = scale_data[work->frame * 2 + 1] / 100.0f;
        } else {
            f32 arg = (s32)(work->frame - 15);
            f32 sv;
            work->scaleX = 1.0f;
            sv = (f32)sin(arg *= 0.25f);
            work->scaleY = 1.0f - 0.1f * sv;
        }
    }
    work->frame++;
    mode = 1;
    {
    Vec position = { 0.0f, 0.0f, 0.0f };
    Vec* p = &position;
    p->x = work->position.x;
    p->y = work->position.y;
    p->z = work->position.z;
    if (work->alpha != 0xFF) {
        mode = 2;
    }
    dispEntry(CAMERA_3D, dispCalcZ(p), mode, (DispCallback)effUpdownDisp, effect);
    }
}

static void polygon(s32 type, s32 alpha, f32 x, f32 y, f32 w, f32 h, f32 s, f32 t) {
    u8 r0, g0, b0, a, r1, g1, b1;

    switch (type) {
    case 0:
        a = alpha;
        b0 = 0xFF;
        g0 = 0xFF;
        r0 = 0xFF;
        b1 = 0xFF;
        g1 = 0xFF;
        r1 = 0xFF;
        break;
    case 1:
        a = alpha;
        r0 = 0xE9;
        g0 = 0x4A;
        b0 = 0x3F;
        r1 = 0xFE;
        g1 = 0xE6;
        b1 = 0x3E;
        break;
    case 2:
        a = alpha;
        r0 = 0x3F;
        g0 = 0x3F;
        b0 = 0xE9;
        r1 = 0x74;
        g1 = 0xC9;
        b1 = 0xFF;
        break;
    }
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x, y, 0.0f);
    GXColor4u8(r0, g0, b0, a);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(x + w, y, 0.0f);
    GXColor4u8(r0, g0, b0, a);
    GXTexCoord2f32(s, 0.0f);
    GXPosition3f32(x + w, y - h, 0.0f);
    GXColor4u8(r1, g1, b1, a);
    GXTexCoord2f32(s, t);
    GXPosition3f32(x, y - h, 0.0f);
    GXColor4u8(r1, g1, b1, a);
    GXTexCoord2f32(0.0f, t);
}

void effUpdownDisp(CameraId cameraId, EffectEntry* effect) {
    EffUpdownWork* work = effect->userdata;
    CameraEntry* camera = camGetPtr(cameraId);
    s32 number = work->number;
    s32 type = work->type;
    s32 alpha = work->alpha;
    GXTexObj texObj;
    Mtx m5, m4, m3, m2, m1, m0;


    effGetTexObj(0x1D, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    if (number < 0) {
        PSMTXRotRad(m4, 'z', 3.1415927f);
    } else {
        PSMTXIdentity(m4);
    }
    GXLoadTexMtxImm(m4, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    PSMTXTrans(m5, work->position.x, work->position.y, work->position.z);
    PSMTXScale(m3, work->scaleX, work->scaleY, 1.0f);
    PSMTXRotRad(m4, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXConcat(m5, m3, m2);
    PSMTXConcat(m2, m4, m1);
    PSMTXConcat(m5, m4, m0);
    PSMTXConcat(camera->view, m1, m2);
    GXLoadPosMtxImm(m2, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    if (type == 0 || type == 2) {
        polygon(1, alpha, -12.0f, 24.0f, 24.0f, 24.0f, 1.0f, 1.0f);
    } else {
        polygon(2, alpha, -12.0f, 24.0f, 24.0f, 24.0f, 1.0f, 1.0f);
    }

    iconGetTexObj(&texObj, icon_id[abs(number) % 10]);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, GX_PTIDENTITY);
    if (number >= 0) {
        PSMTXTrans(m5, 5.0f, 0.0f, 1.0f);
    } else {
        PSMTXTrans(m5, 5.0f, 7.0f, 1.0f);
    }
    if (work->frame < 15) {
        PSMTXConcat(m1, m5, m2);
    } else {
        PSMTXConcat(m0, m5, m2);
    }
    PSMTXConcat(camera->view, m2, m2);
    GXLoadPosMtxImm(m2, GX_PNMTX0);
    polygon(0, alpha, -8.0f, 16.0f, 16.0f, 16.0f, 1.0f, 1.0f);

    if (number >= 0) {
        iconGetTexObj(&texObj, ICON_PLUS);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        PSMTXTrans(m5, -5.0f, 0.0f, 1.0f);
    } else {
        iconGetTexObj(&texObj, ICON_MINUS);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        PSMTXTrans(m5, -5.0f, 7.0f, 1.0f);
    }
    if (work->frame < 15) {
        PSMTXConcat(m1, m5, m2);
    } else {
        PSMTXConcat(m0, m5, m2);
    }
    PSMTXConcat(camera->view, m2, m2);
    GXLoadPosMtxImm(m2, GX_PNMTX0);
    polygon(0, alpha, -8.0f, 16.0f, 16.0f, 16.0f, 1.0f, 1.0f);
}
