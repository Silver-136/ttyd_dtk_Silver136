#include "eff/n64/eff_kemuri1_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"


typedef struct EffKemuri11N64Work {
    u16 type;         // 0x0
    Vec position;     // 0x4
    Vec velocity;     // 0x10
    Vec scale;        // 0x1C
    s32 timer;        // 0x28
    s32 counter;      // 0x2C
    f32 rotY;         // 0x30
    f32 rotZ;         // 0x34
    s32 alpha;        // 0x38
    f32 texS;         // 0x3C
    f32 texSVel;      // 0x40
    f32 texT;         // 0x44
    f32 texTVel;      // 0x48
    s32 delay;        // 0x4C
    u8 r1, g1, b1;    // 0x50
    u8 r2, g2, b2;    // 0x53
    u8 pad[2];        // 0x56
} EffKemuri11N64Work;


static effN64Vtx stg_thruhammer_v[] = {
    {-34, -2, 20, 4267, 1024, 255, 255, 255, 255},
    {-52, 50, 0, 3840, 2048, 255, 255, 255, 0},
    {-34, -2, -20, 3413, 1024, 255, 255, 255, 255},
    {-52, 50, -30, 3413, 2048, 255, 255, 255, 0},
    {-26, 50, -45, 2987, 2048, 255, 255, 255, 0},
    {0, -2, -40, 2560, 1024, 255, 255, 255, 255},
    {0, 50, -60, 2560, 2048, 255, 255, 255, 0},
    {26, 50, -45, 2133, 2048, 255, 255, 255, 0},
    {34, -2, -20, 1707, 1024, 255, 255, 255, 255},
    {-52, 50, 30, 4267, 2048, 255, 255, 255, 0},
    {-26, 50, 45, 4693, 2048, 255, 255, 255, 0},
    {0, -2, 40, 5120, 1024, 255, 255, 255, 255},
    {0, 50, 60, 5120, 2048, 255, 255, 255, 0},
    {26, 50, 45, 5547, 2048, 255, 255, 255, 0},
    {52, 50, -30, 1707, 2048, 255, 255, 255, 0},
    {52, 50, 0, 1280, 2048, 255, 255, 255, 0},
    {34, -2, 20, 853, 1024, 255, 255, 255, 255},
    {52, 50, 30, 853, 2048, 255, 255, 255, 0},
    {26, 50, 45, 427, 2048, 255, 255, 255, 0},
    {0, -2, 40, 0, 1024, 255, 255, 255, 255},
};

void effKemuri11Main(EffectEntry* effect);
void effKemuri11Disp(CameraId cameraId, EffectEntry* effect);
static void main_dl(EffectEntry* effect, Mtx mtx);

EffectEntry* effKemuri11N64Entry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffKemuri11N64Work* work;
    s32 n;
    s32 ang;
    s32 i;

    effect = effEntry();
    switch (type) {
    case 0:
        type = 3;
        break;
    case 1:
        type = 2;
        break;
    case 2:
        type = 4;
        break;
    case 4:
        type = 6;
        break;
    case 5:
        type = 7;
        break;
    default:
        type = 5;
        break;
    }
    switch (type) {
    case 0:
        n = 3;
        break;
    case 1:
        n = 1;
        break;
    case 2:
    case 5:
        n = 3;
        break;
    default:
        n = 2;
        break;
    }
    effect->type = "Kemuri11N64";
    effect->count = n + 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffKemuri11N64Work) * (n + 1));
    effect->userdata = work;
    effect->callback = effKemuri11Main;
    work->timer = type >= 2 ? 60 : 30;
    work->counter = 0;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->alpha = 255;
    switch (type) {
    case 0:
    case 1:
        work->r1 = 0;
        work->g1 = 255;
        work->b1 = 122;
        work->r2 = 240;
        work->g2 = 255;
        work->b2 = 250;
        break;
    case 2:
        work->r1 = 125;
        work->g1 = 120;
        work->b1 = 100;
        work->r2 = 255;
        work->g2 = 255;
        work->b2 = 240;
        break;
    case 3:
        work->r1 = 220;
        work->g1 = 210;
        work->b1 = 200;
        work->r2 = 255;
        work->g2 = 255;
        work->b2 = 250;
        break;
    case 5:
        work->r1 = 225;
        work->g1 = 204;
        work->b1 = 93;
        work->r2 = 232;
        work->g2 = 231;
        work->b2 = 171;
        break;
    case 6:
        work->timer = 50;
        work->r1 = 212;
        work->g1 = 183;
        work->b1 = 5;
        work->r2 = 193;
        work->g2 = 163;
        work->b2 = 9;
        break;
    case 7:
        work->timer = 50;
        work->r1 = 33;
        work->g1 = 140;
        work->b1 = 141;
        work->r2 = 24;
        work->g2 = 165;
        work->b2 = 163;
        break;
    default:
        work->timer = 50;
        work->r1 = 208;
        work->g1 = 136;
        work->b1 = 40;
        work->r2 = 216;
        work->g2 = 169;
        work->b2 = 65;
        break;
    }
    work++;
    for (i = 1, ang = 30; i < n + 1; i++, work++) {
        switch (type) {
        case 0:
        case 1:
            work->delay = -1 - (i - 1) * 2;
            break;
        default:
            work->delay = -1 - (i - 1) * 3;
            break;
        }
        work->rotZ = 0.0f;
        work->rotY = (f32)ang;
        ang += 30;
    }
    if (type == 6) {
        effKemuri1N64Entry(5, x, y, z, 0.0f);
    } else if (type == 7) {
        effKemuri1N64Entry(6, x, y, z, 0.0f);
    }
    return effect;
}

void effKemuri11Main(EffectEntry* effect) {
    EffKemuri11N64Work* work = effect->userdata;
    s32 i;
    s32 type;
    s32 cnt;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    f32 sv;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->counter++;
    work->timer--;
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    type = work->type;
    cnt = work->counter;
    switch (type) {
    case 0:
    case 1:
        if (cnt >= 8) {
            work->alpha *= 0.8f;
        }
        break;
    default:
        if (cnt >= 8) {
            work->alpha *= 0.94f;
        }
        break;
    }
    for (i = 1, work++; i < effect->count; i++, work++) {
        work->delay++;
        if (work->delay < 0) {
            continue;
        }
        if (work->delay == 0) {
            work->position.x = 0.0f;
            work->position.y = 0.0f;
            work->position.z = 0.0f;
            sv = sin(6.2831855f * work->rotZ / 360.0f);
            work->velocity.x = 0.5f * -sv;
            sv = cos(6.2831855f * work->rotZ / 360.0f);
            work->velocity.y = 0.5f * sv;
            work->velocity.z = 0.0f;
            work->texS = 32.0f;
            work->texSVel = 32.0f;
            work->texTVel = 32.0f;
            work->scale.y = 8.0f - (f32)i;
            switch (type) {
            case 0:
            case 1:
                sv = 0.5f + (f32)i;
                work->scale.x = sv;
                work->scale.z = 0.5f * sv;
                break;
            case 2:
            case 5:
                work->scale.x = (f32)(i + 2);
                work->scale.z = 0.5f * (f32)(i + 2);
                break;
            case 3:
                work->scale.x = (f32)(i + 3);
                work->scale.z = 0.5f * (f32)(i + 3);
                work->texS = 16.0f;
                work->texSVel = 16.0f;
                work->texTVel = 16.0f;
                break;
            default:
                sv = 0.5f * (f32)(i + 3);
                work->scale.x = sv;
                work->scale.z = 0.5f * sv;
                work->scale.y *= 0.8f;
                work->texS = 16.0f;
                if (type == 6 || type == 7) {
                    work->texSVel = 8.0f;
                    work->texTVel = 8.0f;
                } else {
                    work->texSVel = 16.0f;
                    work->texTVel = 16.0f;
                }
                break;
            }
            work->texT = 32.0f;
        }
        work->texS += work->texSVel;
        work->texT += work->texTVel;
        switch (type) {
        case 0:
        case 1:
            work->texSVel *= 0.9f;
            break;
        case 2:
        case 5:
            work->texSVel *= 0.75f;
            work->texTVel *= 0.99f;
            break;
        default:
            if (type == 6 || type == 7) {
                work->texSVel *= 0.88f;
                work->texTVel *= 0.98f;
            } else {
                work->texSVel *= 0.75f;
                work->texTVel *= 0.95f;
            }
            break;
        }
        work->scale.x *= 1.002f;
        work->scale.z *= 1.002f;
        if (work->texS > 128.0f) {
            work->texS = 128.0f;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effKemuri11Disp, effect);
}

void effKemuri11Disp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffKemuri11N64Work* work;
    u8 r1, g1, b1, r2, g2, b2;
    s32 cr1, cg1, cb1, cr2, cg2, cb2;
    s32 type;
    s32 alpha;
    Mtx trans;
    Mtx rot;
    Mtx mtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    type = work->type;
    r1 = work->r1;
    g1 = work->g1;
    b1 = work->b1;
    r2 = work->r2;
    g2 = work->g2;
    b2 = work->b2;
    cr1 = r1;
    cg1 = g1;
    cb1 = b1;
    cr2 = r2;
    cg2 = g2;
    cb2 = b2;
    if (type > 1) {
        cr1 >>= 2;
        cg1 >>= 2;
        cb1 >>= 2;
        cr2 >>= 2;
        cg2 >>= 2;
        cb2 >>= 2;
    }
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    switch (type) {
    case 0:
    case 1:
        GXSetNumTevStages(2);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        effGetTexObjN64(0x23, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        effGetTexObjN64(0x24, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP1);
        break;
    default:
        GXSetNumTevStages(3);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_APREV, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_C0, GX_CC_C1, GX_CC_CPREV, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        effGetTexObjN64(0x25, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        effGetTexObjN64(0x24, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP1);
        break;
    }
    effSetVtxDescN64(stg_thruhammer_v);
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    alpha = work->alpha;
    GXSetTevColor(GX_TEVREG0, (GXColor){cr1, cg1, cb1, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){cr2, cg2, cb2});
    GXSetCullMode(GX_CULL_BACK);
    main_dl(effect, mtx);
    GXSetTevColor(GX_TEVREG0, (GXColor){r1, g1, b1, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){r2, g2, b2});
    GXSetCullMode(GX_CULL_FRONT);
    main_dl(effect, mtx);
}

static void main_dl(EffectEntry* effect, Mtx camMtx) {
    EffKemuri11N64Work* work = effect->userdata;
    s32 i;
    Mtx m;
    Mtx rot;
    Mtx scale;
    f32 t;

    for (i = 1, work++; i < effect->count; i++, work++) {
        if (work->delay < 0) {
            continue;
        }
        PSMTXTrans(m, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rotZ);
        PSMTXConcat(m, rot, m);
        PSMTXScale(scale, 0.3f * work->scale.x, 0.3f * work->scale.y, 0.3f * work->scale.z);
        PSMTXConcat(m, scale, m);
        PSMTXRotRad(rot, 'y', 0.017453292f * work->rotY);
        PSMTXConcat(m, rot, m);
        PSMTXConcat(camMtx, m, m);
        GXLoadPosMtxImm(m, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        PSMTXScale(scale, 0.015625f, 0.03125f, 1.0f);
        t = work->texS;
        PSMTXTrans(m, 0.0f, -t / 4.0f, 0.0f);
        PSMTXConcat(scale, m, scale);
        GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
        PSMTXScale(scale, 0.125f, 0.125f, 1.0f);
        PSMTXTrans(m, 0.0f, -work->texT, 0.0f);
        PSMTXConcat(scale, m, scale);
        GXLoadTexMtxImm(scale, GX_TEXMTX1, GX_MTX2x4);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 54);
        tri2(0, 1, 2, 0, 2, 3, 4, 0);
        tri2(2, 4, 5, 0, 1, 3, 2, 0);
        tri2(4, 6, 5, 0, 5, 6, 7, 0);
        tri2(5, 7, 8, 0, 0, 9, 1, 0);
        tri2(10, 9, 0, 0, 11, 10, 0, 0);
        tri2(11, 12, 10, 0, 13, 12, 11, 0);
        tri2(8, 14, 15, 0, 7, 14, 8, 0);
        tri2(8, 15, 16, 0, 15, 17, 16, 0);
        tri2(16, 17, 18, 0, 16, 18, 19, 0);
    }
}
