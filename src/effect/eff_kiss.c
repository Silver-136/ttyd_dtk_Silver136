#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "eff/n64/eff_stamp_n64.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "eff/n64/eff_stardust_n64.h"
#include "eff/eff_kiss.h"

double fabs(double);


typedef struct KissWork {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 vx;       // 0x10
    f32 vy;       // 0x14
    f32 angle;    // 0x18 (only used in first element)
    f32 rotation; // 0x1C
    f32 scaleX;   // 0x20
    f32 scaleY;   // 0x24
    s32 life;     // 0x28
    s32 frame;    // 0x2C
    s32 alpha;    // 0x30
    s32 pad;      // 0x34
} KissWork;

void effKissMain(EffectEntry* effect);
void effKissDisp(CameraId cameraId, EffectEntry* effect);
void effKissDisp2(CameraId cameraId, EffectEntry* effect);

u8 kuri_kiss[] = {
    0x43, 0x20, 0x12, 0x4F, 0x20, 0x14, 0x5E, 0x20, 0x16, 0x6C, 0x20, 0x19, 0x7A, 0x20, 0x1B, 0x86, 0x20, 0x1D,
    0x8F, 0x20, 0x1F, 0x95, 0x20, 0x20, 0x97, 0x20, 0x20, 0x99, 0x20, 0x20, 0x9A, 0x20, 0x21, 0x9B, 0x20, 0x21,
    0x9B, 0x20, 0x21, 0x9B, 0x20, 0x21, 0x9A, 0x20, 0x21, 0x99, 0x20, 0x21, 0x97, 0x20, 0x21, 0x96, 0x20, 0x20,
    0x94, 0x20, 0x20, 0x92, 0x20, 0x20, 0x91, 0x20, 0x20, 0x8F, 0x20, 0x20, 0x8E, 0x20, 0x20, 0x8D, 0x20, 0x20,
};
u8 kura_kiss[] = {
    0x43, 0x20, 0x20, 0x42, 0x22, 0x1F, 0x42, 0x24, 0x1F, 0x41, 0x26, 0x1F, 0x40, 0x28, 0x1F, 0x40, 0x2A, 0x1E,
    0x3F, 0x2C, 0x1E, 0x3D, 0x2E, 0x1D, 0x3C, 0x31, 0x1D, 0x3C, 0x33, 0x1C, 0x3B, 0x35, 0x1C, 0x3A, 0x37, 0x1C,
    0x3A, 0x39, 0x1C, 0x3A, 0x3B, 0x1C, 0x3A, 0x3E, 0x1C, 0x3B, 0x40, 0x1C, 0x3C, 0x42, 0x1D, 0x3E, 0x44, 0x1E,
    0x40, 0x47, 0x1F, 0x43, 0x49, 0x21, 0x4C, 0x4C, 0x26, 0x5D, 0x50, 0x2F, 0x71, 0x54, 0x3A, 0x83, 0x57, 0x44,
    0x8D, 0x59, 0x49, 0x92, 0x5A, 0x4B, 0x95, 0x5A, 0x4C, 0x98, 0x5B, 0x4D, 0x9A, 0x5B, 0x4D, 0x9C, 0x5B, 0x4E,
    0x9D, 0x5B, 0x4D, 0x9E, 0x5B, 0x4D, 0x9E, 0x5A, 0x4D, 0x9F, 0x5A, 0x4C, 0x9F, 0x5A, 0x4B, 0x9F, 0x5A, 0x4B,
    0xA0, 0x59, 0x4A, 0xA0, 0x59, 0x4A, 0xA1, 0x59, 0x49,
};
s16 kuri_kiss_r[] = {0,   -1,  -6,  -11, -14, -15, -11, 0,   15,  32,  50,  70,
                     90,  111, 133, 155, 178, 201, 224, 247, 270, 293, 316, 338};
GXColor kiss_col[] = {{0xEB, 0x26, 0x34, 0}, {0xF5, 0x6B, 0x75, 0}, {0xDA, 0, 0, 0}};
static u8 color_rotation_data[] = {
    0xFE, 0xAC, 0xAC, 0xFE, 0xAC, 0xD5, 0xFE, 0xB4, 0x9A, 0xD5, 0xB4, 0xFE,
    0xB4, 0xB4, 0xFE, 0xB4, 0xDD, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5,
    0xB4, 0xFE, 0xB4, 0xD5, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5, 0xAC,
};

EffectEntry* effKissEntry(s32 type, f32 x, f32 y, f32 z, f32 angle) {
    EffectEntry* effect;
    KissWork* work;
    KissWork* p;
    s32 i;
    s32 n;
    f32 rad;
    f32 zero;
    f32 deg;

    effect = effEntry();
    n = 2;
    if (type == 5) {
        effStampN64Entry(2, x, y, z);
        n = 7;
    }
    effect->type = "kiss";
    effect->count = n;
    work = __memAlloc(HEAP_EFFECT, effect->count * sizeof(KissWork));
    effect->userdata = work;
    effect->callback = effKissMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scaleY = 1.0f;
    work->scaleX = 1.0f;
    work->life = (type != 4) ? 0x28 : 0x3C;
    work->frame = 0;
    deg = 6.2831855f * angle;
    zero = 0.0f;
    work->alpha = 0xFF;
    work->angle = angle;
    work->rotation = zero;
    p = work + 1;
    for (i = 1; i < effect->count; i++, p++) {
        p->position.x = zero;
        p->position.y = zero;
        p->position.z = zero;
        p->rotation = zero;
        p->life = 0;
        p->frame = 0;
        switch (type) {
        case 0:
        case 3:
            p->scaleY = 1.0f;
            p->scaleX = 1.0f;
            rad = deg / 360.0f;
            p->vx = 3.0f * (f32)sin(rad);
            p->vy = 3.0f * (f32)cos(rad);
            break;
        case 1:
        case 2:
            p->scaleY = zero;
            p->scaleX = zero;
            rad = deg / 360.0f;
            p->vx = 3.0f * (f32)sin(rad);
            p->vy = 3.0f * (f32)cos(rad);
            break;
        case 4:
            p->scaleY = 1.0f;
            p->scaleX = 1.0f;
            break;
        case 5:
            p->scaleY = 1.5f;
            p->scaleX = 1.5f;
            p->rotation = (360.0f * (f32)i) / (f32)(effect->count - 1);
            p->vx = 3.0f * (f32)sin(6.2831855f * p->rotation / 360.0f);
            p->vy = 3.0f * (f32)cos(6.2831855f * p->rotation / 360.0f);
            p->rotation *= -1.0f;
            break;
        }
    }
    return effect;
}

void effKissMain(EffectEntry* effect) {
    KissWork* work = effect->userdata;
    Vec pos;
    s32 type;
    s32 i;
    f32 ang;
    f32 rad;
    f32 a;

    pos = (Vec){work->position.x, work->position.y, work->position.z};
    type = work->type;
    ang = work->angle;
    work->frame++;
    work->life--;
    if (type == 3) {
        work->frame = work->frame % 12;
    }
    if (work->life < 0) {
        if (type == 4) {
            effStardustN64SetScale(effStardustN64Entry(1, work->position.x + work[1].position.x,
                                                       5.0f + (work->position.y + work[1].position.y),
                                                       work->position.z + work[1].position.z, 10.0f),
                                   0.5f);
        }
        effDelete(effect);
        return;
    }
    if (type != 4 && work->life < 16) {
        work->alpha = work->life * 15;
    }
    work++;
    switch (type) {
    case 0:
    case 3:
        for (i = 1; i < effect->count; i++, work++) {
            work->position.x += work->vx;
            work->position.y += work->vy;
            work->vx *= 0.9f;
            work->vy *= 0.9f;
        }
        break;
    case 1:
        ang = 6.2831855f * ang;
        for (i = 1; i < effect->count; i++, work++) {
            if ((u32)work->frame < 24) {
                f32 aa;
                f32 r;
                aa = kuri_kiss[work->frame * 3] / 32.0f;
                aa *= 1.2f;
                r = ang / 360.0f;
                work->position.x = aa * (f32)sin(r);
                work->position.y = aa * (f32)cos(r);
                work->scaleX = kuri_kiss[work->frame * 3 + 1] / 32.0f;
                work->scaleY = kuri_kiss[work->frame * 3 + 2] / 32.0f;
                work->rotation = kuri_kiss_r[work->frame];
                work->frame++;
            }
        }
        break;
    case 2:
        ang = 6.2831855f * ang;
        for (i = 1; i < effect->count; i++, work++) {
            if ((u32)work->frame < 39) {
                rad = ang / 360.0f;
                a = kura_kiss[work->frame * 3] / 32.0f;
                a *= 1.2f;
                work->position.x = a * (f32)sin(rad);
                work->position.y = a * (f32)cos(rad);
                work->scaleX = kura_kiss[work->frame * 3 + 1] / 32.0f;
                work->scaleY = kura_kiss[work->frame * 3 + 2] / 32.0f;
                work->frame++;
            }
        }
        break;
    case 4:
        work->position.x += 0.2f;
        { f32 s = sin(9.424778f * (f32)work->frame / 60.0f); work->position.y = 2.0f * __fabsf(s); }
        work->rotation = 12.0f * (f32)sin(9.424778f * (f32)work->frame / 60.0f);
        work->frame++;
        break;
    case 5:
        for (i = 1; i < effect->count; i++, work++) {
            work->position.x += work->vx;
            work->position.y += work->vy;
            work->vx *= 0.9f;
            work->vy *= 0.9f;
        }
        break;
    }
    if (type == 4 || type == 5) {
        dispEntry(CAMERA_3D, dispCalcZ(&pos), 2, (DispCallback)effKissDisp2, effect);
    } else {
        dispEntry(CAMERA_3D, dispCalcZ(&pos), 2, (DispCallback)effKissDisp, effect);
    }
}

static inline void effKissGXSetup(void) {
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
}

void effKissDisp(CameraId cameraId, EffectEntry* effect) {
    KissWork* work = effect->userdata;
    CameraEntry* cam;
    KissWork* q;
    Mtx mT, mS, mR, mBase;
    GXTexObj texObj;
    s32 i;
    s32 j;
    s32 pass;
    f32 ang;
    f32 angN;
    f32 deg2rad;
    f32 h, w, tu, tv, off;
    s32 alpha;
    s32 frame3;
    s32 type;

    cam = camGetPtr(cameraId);
    alpha = work->alpha;
    frame3 = work->frame * 3;
    ang = work->angle;
    type = work->type;
    PSMTXTrans(mT, work->position.x, work->position.y, work->position.z);
    PSMTXScale(mS, work->scaleX, work->scaleY, work->scaleY);
    PSMTXRotRad(mR, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mT, mR, mT);
    PSMTXConcat(mT, mS, mBase);
    effKissGXSetup();

    deg2rad = 0.017453292f;
    angN = deg2rad * -ang;
    for (pass = 0; pass < 2; pass++) {
        q = effect->userdata;
        q++;
        for (j = 1; j < effect->count; j++, q++) {
            off = 0.0f;
            switch (type) {
            case 0:
            case 1:
            case 2:
                effGetTexObj(20, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                if (pass == 0) {
                    off = 1.5f;
                    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0, 0, 0, alpha / 2});
                } else {
                    kiss_col[type].a = alpha;
                    GXSetChanMatColor(GX_COLOR0A0, kiss_col[type]);
                }
                w = 8.0f;
                tu = 2.0f;
                h = w;
                tv = 1.0f;
                break;
            case 3:
                if (pass == 0) {
                    effGetTexObj(89, &texObj);
                    GXLoadTexObj(&texObj, GX_TEXMAP0);
                    GXSetChanMatColor(GX_COLOR0A0, (GXColor){color_rotation_data[frame3], color_rotation_data[frame3 + 1], color_rotation_data[frame3 + 2], alpha});
                    h = 32.0f;
                    w = 40.0f;
                } else {
                    effGetTexObj(88, &texObj);
                    GXLoadTexObj(&texObj, GX_TEXMAP0);
                    GXSetChanMatColor(GX_COLOR0A0, (GXColor){60, 60, 60, alpha});
                    h = 20.0f;
                    w = 32.0f;
                }
                h *= 0.5f;
                tv = 1.0f;
                tu = tv;
                w *= 0.5f;
                break;
            }
            PSMTXTrans(mT, q->position.x + off, q->position.y - off, q->position.z);
            PSMTXRotRad(mR, 'z', angN);
            PSMTXConcat(mT, mR, mT);
            PSMTXRotRad(mR, 'y', deg2rad * q->rotation);
            PSMTXConcat(mT, mR, mT);
            if (type == 3) {
                PSMTXRotRad(mR, 'z', deg2rad * ang);
                PSMTXConcat(mT, mR, mT);
            }
            PSMTXScale(mS, q->scaleX, q->scaleY, q->scaleY);
            PSMTXConcat(mT, mS, mT);
            if (type == 3) {
                PSMTXTrans(mS, 0.0f, 5.0f, 0.0f);
                PSMTXConcat(mT, mS, mT);
            }
            PSMTXConcat(mBase, mT, mT);
            PSMTXConcat(cam->view, mT, mT);
            GXLoadPosMtxImm(mT, 0);
            GXSetCurrentMtx(0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            if (type == 3) {
                GXPosition3f32(-h / 2.0f, w / 2.0f, 0.0f);
                GXTexCoord2f32(0.0f, 0.0f);
                GXPosition3f32(h / 2.0f, w / 2.0f, 0.0f);
                GXTexCoord2f32(tu, 0.0f);
                GXPosition3f32(h / 2.0f, -w / 2.0f, 0.0f);
                GXTexCoord2f32(tu, tv);
                GXPosition3f32(-h / 2.0f, -w / 2.0f, 0.0f);
                GXTexCoord2f32(0.0f, tv);
            } else {
                GXPosition3f32(-h / 2.0f, w, 0.0f);
                GXTexCoord2f32(0.0f, 0.0f);
                GXPosition3f32(h / 2.0f, w, 0.0f);
                GXTexCoord2f32(tu, 0.0f);
                GXPosition3f32(h / 2.0f, 0.0f, 0.0f);
                GXTexCoord2f32(tu, tv);
                GXPosition3f32(-h / 2.0f, 0.0f, 0.0f);
                GXTexCoord2f32(0.0f, tv);
            }
        }
    }
    switch (type) {
    case 0:
    case 1:
    case 3:
        break;
    default:
        return;
    }
    {
        f32 scale2;
        f32 len;
        f32 w2;
        f32 off2;
        scale2 = 1.15f;
        for (i = 0; i < 2; i++) {
            off2 = 0.0f;
            q = effect->userdata;
            switch (type) {
            case 0:
            case 1:
            case 2:
                effGetTexObj(66, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                if (i == 0) {
                    off2 = 1.5f;
                    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0, 0, 0, alpha / 2});
                } else {
                    kiss_col[type].a = alpha;
                    GXSetChanMatColor(GX_COLOR0A0, kiss_col[type]);
                }
                break;
            case 3:
                if (i == 0) {
                    effGetTexObj(90, &texObj);
                    GXLoadTexObj(&texObj, GX_TEXMAP0);
                    GXSetChanMatColor(GX_COLOR0A0, (GXColor){color_rotation_data[frame3 + 0], color_rotation_data[frame3 + 1], color_rotation_data[frame3 + 2], alpha});
                } else {
                    effGetTexObj(66, &texObj);
                    GXLoadTexObj(&texObj, GX_TEXMAP0);
                    GXSetChanMatColor(GX_COLOR0A0, (GXColor){60, 60, 60, alpha});
                }
                break;
            }
            PSMTXTrans(mT, off2, -off2, 0.0f);
            PSMTXRotRad(mR, 'z', angN);
            PSMTXConcat(mR, mT, mT);
            PSMTXConcat(mBase, mT, mT);
            PSMTXConcat(cam->view, mT, mT);
            GXLoadPosMtxImm(mT, 0);
            GXSetCurrentMtx(0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            len = scale2 * q[1].position.y;
            w2 = 1.0f;
            if (type == 3 && i == 0) {
                w2 = 2.0f;
            }
            GXPosition3f32(-w2 / 2.0f, len, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(w2 / 2.0f, len, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(w2 / 2.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(-w2 / 2.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
        }
    }
}

void effKissDisp2(CameraId cameraId, EffectEntry* effect) {
    KissWork* work = effect->userdata;
    CameraEntry* cam;
    Mtx mT, mS, mR, mBase;
    GXTexObj texObj;
    s32 i;
    s32 alpha;
    f32 w, nw, hw, deg2rad;

    cam = camGetPtr(cameraId);
    alpha = work->alpha;
    PSMTXTrans(mT, work->position.x, work->position.y, work->position.z);
    PSMTXScale(mS, work->scaleX, work->scaleY, work->scaleY);
    PSMTXRotRad(mR, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mT, mR, mT);
    PSMTXConcat(mT, mS, mBase);
    effKissGXSetup();
    effGetTexObj(20, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xB5, 0xDA, alpha});
    deg2rad = 0.017453292f;
    w = 8.0f;
    hw = 4.0f;

    for (i = 1, work++; i < effect->count; i++, work++) {
        PSMTXTrans(mT, work->position.x, work->position.y, work->position.z);
        PSMTXScale(mS, work->scaleX, work->scaleY, 1.0f);
        nw = -w / 2.0f;
        PSMTXRotRad(mR, 'z', deg2rad * work->rotation);
        PSMTXConcat(mT, mS, mT);
        PSMTXConcat(mT, mR, mT);
        PSMTXConcat(mBase, mT, mT);
        PSMTXConcat(cam->view, mT, mT);
        GXLoadPosMtxImm(mT, 0);
        GXSetCurrentMtx(0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(nw, w, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(hw, w, 0.0f);
        GXTexCoord2f32(2.0f, 0.0f);
        GXPosition3f32(hw, 0.0f, 0.0f);
        GXTexCoord2f32(2.0f, 1.0f);
        GXPosition3f32(nw, 0.0f, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
    }
}
