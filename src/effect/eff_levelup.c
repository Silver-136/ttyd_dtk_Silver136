#include "eff/eff_levelup.h"
#include "driver/dispdrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "mario/mariost.h"
#include "memory.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

USER_FUNC(evt_cam_shake);

typedef struct EffLevelupWork {
    s32 type;     // 0x0
    f32 x;        // 0x4
    f32 y;        // 0x8
    f32 z;        // 0xC
    f32 xOffset;  // 0x10
    s32 timer;    // 0x14
    s32 state;    // 0x18
    f32 scaleX;   // 0x1C
    f32 scaleY;   // 0x20
} EffLevelupWork;

#ifdef VERSION_G8ME01
static s32 texid_tbl[6][6] = { { 118, 119, 120, 121, 122, 123 }, { 118, 119, 120, 121, 122, 123 }, { 118, 119, 120, 121, 122, 123 }, { 118, 119, 120, 121, 122, 123 }, { 118, 119, 120, 121, 122, 123 }, { 118, 119, 120, 121, 122, 123 } };
u16 y_trans[] = { 500, 500, 479, 458, 437, 416, 395, 375, 354, 333, 312, 291, 270, 250, 229, 208, 187, 166, 145, 125, 104, 83, 62, 41, 20, 0, 5, 10, 15, 20, 25, 20, 15, 10, 4, 0, 5, 10, 15, 20, 25, 20, 15, 10, 4, 0 };
#else
u16 y_trans[] = { 500, 500, 479, 458, 437, 416, 395, 375, 354, 333, 312, 291, 270, 250, 229, 208, 187, 166, 145, 125, 104, 83, 62, 41, 20, 0, 5, 10, 15, 20, 25, 20, 15, 10, 4, 0, 5, 10, 15, 20, 25, 20, 15, 10, 4, 0 };

static s32 texid_tbl[2][6] = { { 146, 147, 148, 149, 150, 151 }, { 146, 147, 148, 149, 150, 151 } };
#endif
#ifdef VERSION_G8ME01
u8 y_scale[] = { 0, 13, 40, 69, 98, 126, 149, 166, 175, 165, 136, 100, 67, 50, 73, 117, 130, 113, 100 };
#else
u8 y_scale[] = { 0, 13, 40, 69, 98, 126, 149, 166, 175, 165, 136, 100, 67, 50, 73, 117, 130, 113, 100};
#endif


static EVT_BEGIN(evt_shake)
    CALL(evt_cam_shake, 4, FLOAT(0.001953125f), FLOAT(0.001953125f), 50)
    RETURN
EVT_END()

#ifndef VERSION_G8ME01
static inline s32 getX(s32 i) {
    s32 arr[2][5] = { { 0, 64, 188, 260, 136 }, { 0, 88, 210, 256, 134 } };
    return arr[(gp->language ? 0 : 1) == 0][i] - arr[(gp->language ? 0 : 1) == 0][4];
}
#endif

EffectEntry* effLevelupEntry(s32 type, f32 x, f32 y, f32 z, s32 limit) {
    EffectEntry* effect;
    EffLevelupWork* work;
    EffLevelupWork* w;
    s32 i;
#ifdef VERSION_G8ME01
    GXTexObj texObj;
    f32 a;
    f32 b;
    f32 s;
    s32 j;
#endif

    effect = effEntry();
    effect->type = "Levelup";
    effect->count = 4;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffLevelupWork) * effect->count);
    effect->userdata = work;
    effect->callback = effLevelupMain;
    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->timer = limit;
    work->state = 0;
    w = work + 1;
    for (i = 1; i < effect->count; i++, w++) {
#ifdef VERSION_G8ME01
        a = 0.0f;
        b = a;
        s = (u32)gp->language == 3 ? 0.9f : 1.0f;
        for (j = 0; j < i - 1; j++) {
            effGetTexObj(texid_tbl[gp->language][j], &texObj);
            a += s * (f32)GXGetTexObjWidth(&texObj);
        }
        effGetTexObj(texid_tbl[gp->language][j], &texObj);
        a += s * ((f32)GXGetTexObjWidth(&texObj) / 2.0f);
        for (j = 0; j < 3; j++) {
            effGetTexObj(texid_tbl[gp->language][j], &texObj);
            b += s * ((f32)GXGetTexObjWidth(&texObj) / 2.0f);
        }
        w->x = a - b;
#else
        w->x = getX(i);
#endif
        w->y = y_trans[0];
        w->z = 0.0f;
        w->xOffset = 0.0f;
        w->state = 0;
        w->scaleX = 1.0f;
        w->scaleY = i < 3 ? 1.0f : 0.0f;
    }
    return effect;
}

void effLevelupMain(EffectEntry* effect) {
    EffLevelupWork* work = effect->userdata;
    Vec position2;
    Vec position = { 0.0f, 0.0f, 0.0f };
    s32 limit;
    s32 i;
    EffLevelupWork* w;
    s32 done = 0;

    position.x = work->x;
    position.y = work->y;
    position.z = work->z;
    position2 = position;
    limit = work->timer;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        switch (work->state) {
            case 0:
                work->timer++;
                if ((u32)work->timer < 0x2e) {
                    work->y = y_trans[work->timer];
                    if (i == 1 && work->y == 0.0f) {
                        evtEntry(evt_shake, 0, 0);
                        psndSFXOn((const char*)0xa57);
                    }
                } else {
                    work->state++;
                    work->timer = 0;
                }
                break;
            case 1:
                work->timer++;
                if ((u32)work->timer < 0x13) {
                    if (i > 2) {
                        work->scaleY = y_scale[work->timer] / 100.0f;
                    }
                } else {
                    work->state++;
                    work->timer = 0;
                }
                break;
            case 2:
                if (work->timer++ > limit) {
                    work->timer = 0;
                    work->state++;
                }
                break;
            case 3:
                if (i < 2) {
                    work->xOffset = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(0.0f, 400.0f, 1, work->timer, 30);
                } else {
                    work->xOffset = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(0.0f, -400.0f, 1, work->timer, 30);
                }
                work->timer++;
                if (work->timer > 30) {
                    work->state++;
                    work->timer = 0;
                }
                break;
            case 4:
                done++;
                break;
        }
    }
    if (done >= effect->count - 1) {
        effDelete(effect);
    } else {
        dispEntry(CAMERA_2D, dispCalcZ(&position2), 2, (DispCallback)effLevelupDisp, effect);
    }
}

void effLevelupDisp(CameraId cameraId, EffectEntry* effect) {
    s32 ofs;
    s32 tofs;
    s32 xofs;
    EffLevelupWork* w;
    EffLevelupWork* work = effect->userdata;
    CameraEntry* cam;
    Mtx m0;
    Mtx rot;
    Mtx scale;
    Mtx viewMtx;
    GXTexObj texObj;
    GXColor color;
    f32 dx;
    f32 dy;
    f32 halfScale = 0.5f;
#ifdef VERSION_G8ME01
    GXTexObj texObj2;
    f32 width;
    f32 height;
#else
    f32 height = 48.0f;
#endif
    s32 i;
    s32 j;

    cam = camGetPtr(cameraId);
    PSMTXTrans(m0, work->x, work->y, work->z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(m0, rot, m0);
    PSMTXConcat(cam->view, m0, viewMtx);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    for (j = 1, xofs = 4; j >= 0; j--, xofs -= 4) {
#ifdef VERSION_G8ME01
        for (i = effect->count - 1, ofs = i * sizeof(EffLevelupWork), tofs = i * sizeof(s32); i >= 1; ofs -= sizeof(EffLevelupWork), tofs -= sizeof(s32), i--) {
#else
        for (i = effect->count - 1, ofs = i * sizeof(EffLevelupWork), tofs = i * sizeof(s32); i >= 1; i--, ofs -= sizeof(EffLevelupWork), tofs -= sizeof(s32)) {
#endif
            w = effect->userdata;
            (u8*)w += ofs;
            dx = xofs;
            dy = -j * 4;
            if (j != 0) {
#ifdef VERSION_G8ME01
                effGetTexObj(*(s32*)((u8*)texid_tbl[gp->language] + tofs + 2 * sizeof(s32)), &texObj);
#else
                effGetTexObj(*(s32*)((u8*)texid_tbl[gp->language != 0] + tofs + 2 * sizeof(s32)), &texObj);
#endif
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                color = (GXColor){ 0, 0, 0, 0x80 };
                GXSetChanMatColor(GX_COLOR0A0, color);
            } else {
#ifdef VERSION_G8ME01
                effGetTexObj(*(s32*)((u8*)texid_tbl[gp->language] + tofs - sizeof(s32)), &texObj);
#else
                effGetTexObj(*(s32*)((u8*)texid_tbl[gp->language != 0] + tofs - sizeof(s32)), &texObj);
#endif
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                color = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
                GXSetChanMatColor(GX_COLOR0A0, color);
            }
            PSMTXTrans(m0, dx + (w->x + w->xOffset), w->y + dy, w->z);
            PSMTXConcat(viewMtx, m0, m0);
            PSMTXScale(scale, w->scaleX, w->scaleY, 1.0f);
            PSMTXConcat(m0, scale, m0);
            GXLoadPosMtxImm(m0, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
#ifdef VERSION_G8ME01
            effGetTexObj(*(s32*)((u8*)texid_tbl[gp->language] + tofs - sizeof(s32)), &texObj2);
            width = GXGetTexObjWidth(&texObj2);
            height = GXGetTexObjHeight(&texObj2);
            if ((u32)gp->language == 3) {
                width *= 0.9f;
                height *= 0.9f;
            }
#endif
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            {
#ifdef VERSION_G8ME01
                f32 half = width * halfScale;
#else
                s32 arr[2][4] = { { 0, 128, 120, 24 }, { 0, 176, 68, 24 } };
                f32 width = *(s32*)((u8*)arr[gp->language != 0] + tofs);

                f32 half = width * halfScale;
#endif
                f32 negHalf = -width * halfScale;
                GXPosition3f32(negHalf, height, 0.0f);
                GXTexCoord2f32(0.0f, 0.0f);
                GXPosition3f32(half, height, 0.0f);
                GXTexCoord2f32(1.0f, 0.0f);
                GXPosition3f32(half, 0.0f, 0.0f);
                GXTexCoord2f32(1.0f, 1.0f);
                GXPosition3f32(negHalf, 0.0f, 0.0f);
                GXTexCoord2f32(0.0f, 1.0f);
            }
        }
    }
}

