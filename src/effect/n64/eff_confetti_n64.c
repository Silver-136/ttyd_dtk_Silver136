#include "eff/eff_n64.h"
#include "eff/n64/eff_confetti_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

typedef struct EffConfettiN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    Vec velocity; // 0x10
    f32 accelY;   // 0x1c
    s32 life;     // 0x20
    s32 frame;    // 0x24
    s32 alpha;    // 0x28
    s32 count;    // 0x2c
    s32 delay;    // 0x30
    s32 camId;    // 0x34
} EffConfettiN64Work;

void effConfettiMain(EffectEntry* effect);
void effConfettiDisp(CameraId cameraId, EffectEntry* effect);

effN64Vtx size6x6_tex16x16_vtx[] = {
    {-3, -3, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {3, -3, 0, 0x200, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {3, 3, 0, 0x200, 0x200, 0xFF, 0xFF, 0xFF, 0xFF},
    {-3, 3, 0, 0, 0x200, 0xFF, 0xFF, 0xFF, 0xFF},
};

effN64Vtx size6x6_tex16x16_2_vtx[] = {
    {-3, -3, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {3, -3, 0, 0x200, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {3, 3, 0, 0x200, 0x200, 0xFF, 0xFF, 0xFF, 0xFF},
    {-3, 3, 0, 0, 0x200, 0xFF, 0xFF, 0xFF, 0xFF},
    {-34, 26, 30, 0, 0x800, 0xFF, 0xFF, 0xFF, 0xFF},
    {-28, 26, 30, 0x200, 0x800, 0xFF, 0xFF, 0xFF, 0xFF},
    {-28, 32, 30, 0x200, 0xA00, 0xFF, 0xFF, 0xFF, 0xFF},
    {-34, 32, 30, 0, 0xA00, 0xFF, 0xFF, 0xFF, 0xFF},
    {16, 46, 40, 0, 0x1000, 0xFF, 0xFF, 0xFF, 0xFF},
    {22, 46, 40, 0x200, 0x1000, 0xFF, 0xFF, 0xFF, 0xFF},
    {22, 52, 40, 0x200, 0x1200, 0xFF, 0xFF, 0xFF, 0xFF},
    {16, 52, 40, 0, 0x1200, 0xFF, 0xFF, 0xFF, 0xFF},
    {-14, 6, -50, 0, 0x1800, 0xFF, 0xFF, 0xFF, 0xFF},
    {-8, 6, -50, 0x200, 0x1800, 0xFF, 0xFF, 0xFF, 0xFF},
    {-8, 12, -50, 0x200, 0x1A00, 0xFF, 0xFF, 0xFF, 0xFF},
    {-14, 12, -50, 0, 0x1A00, 0xFF, 0xFF, 0xFF, 0xFF},
};

u8 color_table[] = {
    0xE8, 0xA0, 0xA8, 0xA8, 0x50, 0x58, 0xA0, 0xA8, 0xE8, 0x48, 0x48, 0xE8,
    0xA0, 0xE8, 0xA0, 0x60, 0xB0, 0x78, 0xE0, 0xE0, 0x58, 0xB0, 0xA0, 0x38,
    0xE8, 0xA0, 0xE8, 0xB0, 0x40, 0xA0, 0xA0, 0xD8, 0xD8, 0x58, 0xA8, 0xA8,
};

u8 anm_seq[] = {0, 1, 2, 3, 2, 1};

void effConfettiN64SetCamId(EffectEntry* effect, s32 camId) {
    EffConfettiN64Work* w = effect->userdata;
    w->camId = camId;
}

EffectEntry* effConfettiN64Entry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 scale) {
    s32 mod;
    EffectEntry* effect;
    EffConfettiN64Work* w;
    s32 k;
    s32 n;
    s32 base;
    s32 cnt;
    f32 s1, c1, ang;
    f32 s2, c2;
    f32 half;
    f32 speed;
    f32 r;

    effect = effEntry();
    mod = type % 10;
    switch (mod) {
    case 0:
        n = 12;
        break;
    case 1:
        n = 18;
        break;
    case 2:
        n = 24;
        break;
    case 4:
        n = 12;
        break;
    case 5:
        n = 18;
        break;
    case 6:
        n = 24;
        break;
    default:
        n = 48;
        break;
    }
    switch (mod) {
    case 0:
    case 4:
        speed = 3.0f;
        break;
    case 1:
    case 5:
        speed = 4.0f;
        break;
    case 6:
    default:
        speed = 5.0f;
        break;
    }
    effect->type = "ConfettiN64";
    cnt = n + 1;
    effect->count = cnt;
    w = __memAlloc(HEAP_EFFECT, cnt * sizeof(EffConfettiN64Work));
    effect->userdata = w;
    effect->callback = effConfettiMain;
    w[0].position.x = x;
    w[0].position.y = y;
    w[0].position.z = z;
    w[0].frame = 0;
    w[0].life = life;
    w[0].type = type;
    w[0].count = n;
    w[0].alpha = 0xFF;
    w[0].camId = 4;
    w++;
    base = rand() % 360;
    half = 0.5f * speed;
    half = half + half;
    for (k = 0; k < n; k++, w++) {
        r = speed * (0.01f * (f32)(rand() % 101));
        ang = (6.2831855f * ((f32)base + 360.0f * (f32)k / (f32)n)) / 360.0f;
        w->type = type;
        s1 = sin(ang);
        c1 = cos(ang);
        s2 = sin(ang);
        c2 = cos(ang);
        if (mod == 3) {
            w->position.x = 0.1f * (f32)(rand() % 701) - 35.0f;
            w->position.y = 0.0f;
            w->position.z = 0.1f * (f32)(rand() % 301) - 15.0f;
            w->velocity.x = 0.03f * (w->position.x + 0.1f * (f32)(rand() % 101) - 5.0f);
            w->velocity.y = -1.7f - 0.01f * (0.1f * (f32)(rand() % 801));
            w->velocity.z = 0.05f * (0.1f * (f32)(rand() % 201) - 10.0f);
            w->life = 120;
            w->delay = k * 3;
        } else {
            w->position.x = c2 * (r * s1);
            w->position.y = c2 * (r * c1);
            w->position.z = r * s2;
            w->velocity.x = c2 * (half * s1);
            w->velocity.y = c2 * (half * c1) + 1.0f;
            w->velocity.z = half * s2;
            if (mod >= 4) {
                w->accelY = -0.02f - 0.02f * (f32)((k + 1) % 3);
            } else {
                w->accelY = 0.0f;
            }
            w->delay = k & 3;
        }
    }
    return effect;
}

void effConfettiMain(EffectEntry* effect) {
    s32 mod;
    EffConfettiN64Work* w;
    s32 i;
    s32 type;
    s32 frame;
    s32 count;
    s32 camId;
    Vec position;
    s32 anm;

    w = effect->userdata;
    {
        Vec p = {0.0f, 0.0f, 0.0f};
        p.x = w->position.x;
        p.y = w->position.y;
        p.z = w->position.z;
        position = p;
    }
    camId = w->camId;
    w->life = w->life - 1;
    if (w->life < 0) {
        effDelete(effect);
        return;
    }
    w->frame = w->frame + 1;
    if (w->frame > 0x4F1A0) {
        w->frame = 10;
    }
    frame = w->frame;
    type = w->type;
    count = w->count;
    if (w->life < 10) {
        w->alpha = w->life * 25;
    }
    mod = type % 10;
    w++;
    anm = (frame >> 1) % 6;
    for (i = 0; i < count; i++, w++) {
        switch (mod) {
        case 3:
            if (w->delay > 0) {
                w->delay--;
                if (w->delay > 0) {
                    continue;
                }
            }
            w->life--;
            if (w->life < 0) {
                w->position.x = 0.1f * (f32)(rand() % 701) - 35.0f;
                w->position.y = 0.0f;
                w->position.z = 0.1f * (f32)(rand() % 301) - 15.0f;
                w->velocity.x = 0.03f * (w->position.x + 0.1f * (f32)(rand() % 101) - 5.0f);
                w->velocity.y = -1.7f - 0.01f * (0.1f * (f32)(rand() % 801));
                w->velocity.z = 0.05f * (0.1f * (f32)(rand() % 201) - 10.0f);
                w->life = 120;
                w->life--;
            }
            break;
        default:
            if (w->delay > 0) {
                w->delay--;
                if (w->delay > 0) {
                    continue;
                }
            }
            if (mod >= 4) {
                w->velocity.x *= 0.98f;
                w->velocity.y *= 0.98f;
                w->velocity.z *= 0.98f;
                if (frame > 5 && w->velocity.y > -0.5f) {
                    w->velocity.y += w->accelY;
                }
            }
            break;
        }
        w->position.x += w->velocity.x;
        w->position.y += w->velocity.y;
        w->position.z += w->velocity.z;
    }
    if (type < 10) {
        dispEntry(camId, dispCalcZ(&position), 2, (DispCallback)effConfettiDisp, effect);
    } else if (type < 20) {
        dispEntry(CAMERA_3D_EFFECTB, dispCalcZ(&position), 2, (DispCallback)effConfettiDisp, effect);
    } else {
        dispEntry(CAMERA_2D, dispCalcZ(&position), 2, (DispCallback)effConfettiDisp, effect);
    }
}

void effConfettiDisp(CameraId cameraId, EffectEntry* effect) {
    s32 anm;
    s32 type;
    EffConfettiN64Work* w;
    s32 i;
    s32 alpha;
    s32 count;
    CameraEntry* camera;
    s32 off;
    Mtx tmp;
    Mtx scale;
    Mtx rot;
    Mtx mtx;
    GXTexObj texObj;
    u8* col;

    camera = camGetPtr(cameraId);
    w = effect->userdata;
    anm = w->frame;
    type = w->type;
    count = w->count;
    alpha = w->alpha;
    PSMTXTrans(tmp, w->position.x, w->position.y, w->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(tmp, rot, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    w++;
    effGetTexObjN64(0x4B, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_C1);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    for (i = 0; i < count; i++, w++) {
        col = &color_table[(i % 12) * 3];
        if (w->delay > 0) {
            continue;
        }
        PSMTXTrans(tmp, w->position.x, w->position.y, w->position.z);
        PSMTXConcat(mtx, tmp, tmp);
        GXLoadPosMtxImm(tmp, 0);
        GXSetCurrentMtx(0);
        GXSetTevColor(GX_TEVREG0, (GXColor){col[0], col[1], col[2], alpha});
        GXSetTevColor(GX_TEVREG1, (GXColor){0x3C, 0x3C, 0x3C, 0});
        off = ((i + anm_seq[(anm >> 1) % 6]) & 0xF) << 6;
        PSMTXScale(scale, 0.0625f, 0.00390625f, 0.0f);
        PSMTXTrans(tmp, 0.0f, (f32)off, 0.0f);
        PSMTXConcat(scale, tmp, scale);
        GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
        switch (type % 10) {
        case 3:
            effSetVtxDescN64(size6x6_tex16x16_2_vtx);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 24);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            tri2(4, 5, 6, 4, 4, 6, 7, 4);
            tri2(8, 9, 10, 8, 8, 10, 11, 8);
            tri2(12, 13, 14, 12, 12, 14, 15, 12);
            break;
        default:
            effSetVtxDescN64(size6x6_tex16x16_vtx);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            break;
        }
    }
}
