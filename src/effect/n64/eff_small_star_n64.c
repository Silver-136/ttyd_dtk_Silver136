#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include <math.h>

#define PI 3.1415927f
#define HALF_PI 1.5707964f

typedef struct EffSmallStarN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    Vec velocity; // 0x10
    f32 rotation; // 0x1c
    f32 rotVel;   // 0x20
    s32 alpha;    // 0x24
    s32 life;     // 0x28
    s32 frame;    // 0x2c
    u8 camera;    // 0x30
} EffSmallStarN64Work;

static f32 kaiten;

void effSmallStarMain(EffectEntry* effect);
void effSmallStarDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effSmallStarN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz) {
    EffectEntry* effect;
    EffSmallStarN64Work* w;
    f32 px, py, pz;
    f32 inv;
    f32 zr;
    f32 len;
    f32 ang;
    s32 i;
    Mtx mtx;
    Vec axis;

    effect = effEntry();
    if (count == 0) {
        return NULL;
    }
    len = dx * dx + dy * dy + dz * dz;
    zr = 0.0f;
    if (len == zr) {
        return NULL;
    }
    inv = -1.0f / sqrtf(len);
    dx *= inv;
    dy *= inv;
    dz *= inv;
    if (dx != zr) {
        px = -dy / dx;
        py = 1.0f;
        pz = 0.0f;
    } else if (dy != zr) {
        px = 1.0f;
        py = -dx / dy;
        pz = 0.0f;
    } else {
        px = 1.0f;
        py = 0.0f;
        pz = -dx / dz;
    }
    len = px * px + py * py + pz * pz;
    if (len == zr) {
        return NULL;
    }
    inv = 1.0f / sqrtf(len);
    effect->type = "SmallStarN64";
    effect->count = count;
    px *= inv;
    py *= inv;
    pz *= inv;
    dx *= 8.0f;
    dy *= 8.0f;
    dz *= 8.0f;
    px *= 8.0f;
    py *= 8.0f;
    pz *= 8.0f;
    w = __memAlloc(HEAP_EFFECT, sizeof(EffSmallStarN64Work) * count);
    effect->userdata = w;
    effect->callback = effSmallStarMain;
    for (i = 0; i < count; i++, w++) {
        ang = 6.2831855f * camGetPtr(CAMERA_3D)->viewYaw / 360.0f;
        w->type = type;
        w->position.x = x;
        w->position.y = y;
        w->position.z = z;
        w->camera = 4;
        switch (type) {
        case 0:
        case 1:
        case 2: {
            Vec dir = {0.0f, 0.0f, 0.0f};
            dir.x = dx;
            dir.y = dy;
            dir.z = dz;
            axis = dir;
            PSMTXRotAxisRad(mtx, &axis, 0.017453292f * (360.0f * (f32)i / (f32)(count - 1)));
            w->velocity.x = dx + mtx[0][0] * px + mtx[0][1] * py + mtx[0][2] * pz;
            w->velocity.y = dy + mtx[1][0] * px + mtx[1][1] * py + mtx[1][2] * pz;
            w->velocity.z = dz + mtx[2][0] * px + mtx[2][1] * py + mtx[2][2] * pz;
            break;
        }
        case 3: {
            Vec v = {0.0f, 0.0f, 0.0f};
            f32 t;
            v.x = (f32)sin(ang);
            v.z = -(f32)cos(ang);
            axis = v;
            if (count == 1) {
                t = 0.0f;
            } else {
                t = 100.0f * (f32)i / (f32)(count - 1) - 50.0f;
            }
            PSMTXRotAxisRad(mtx, &axis, 0.017453292f * t);
            w->velocity.x = mtx[0][0] * dx + mtx[0][1] * dy + mtx[0][2] * dz;
            w->velocity.y = mtx[1][0] * dx + mtx[1][1] * dy + mtx[1][2] * dz;
            w->velocity.z = mtx[2][0] * dx + mtx[2][1] * dy + mtx[2][2] * dz;
            break;
        }
        case 4: {
            Vec v = {0.0f, 0.0f, 0.0f};
            v.x = (f32)sin(ang);
            v.z = -(f32)cos(ang);
            axis = v;
            PSMTXRotAxisRad(mtx, &axis, 0.017453292f * (360.0f * (f32)i / (f32)(count - 1)));
            w->velocity.x = mtx[0][0] * dx + mtx[0][1] * dy + mtx[0][2] * dz;
            w->velocity.y = mtx[1][0] * dx + mtx[1][1] * dy + mtx[1][2] * dz;
            w->velocity.z = mtx[2][0] * dx + mtx[2][1] * dy + mtx[2][2] * dz;
            break;
        }
        }
        switch (type) {
        case 0:
            w->rotVel = 0.0f;
            w->life = 24;
            break;
        case 1:
            w->rotVel = 20.0f;
            w->life = 24;
            break;
        case 2:
            w->rotVel = 20.0f;
            w->velocity.x *= 0.5f;
            w->velocity.y *= 0.5f;
            w->velocity.z *= 0.5f;
            w->life = 40;
            break;
        case 3:
        case 4:
            w->rotVel = 20.0f;
            w->life = 24;
            break;
        }
        w->rotation = 0.0f;
        w->frame = 0;
        w->alpha = 255;
    }
    kaiten += 15.0f;
    if (kaiten > 360.0f) {
        kaiten = 0.0f;
    }
    return effect;
}

#pragma opt_propagation off
#pragma opt_lifetimes off
static inline f32 _cosf(f32 x) {
    f32 a = 0.03705f;
    f32 b = 0.4967f;
    f32 c = 1.0f;
    f32 x2;

    if (x <= PI) {
        if (x <= HALF_PI) {
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
        } else {
            x = HALF_PI - (x - HALF_PI);
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
            a = -a;
        }
    } else if (x < 4.712389f) {
        x = x - PI;
        x2 = x * x;
        a = a * x2 - b;
        a = a * x2 + c;
        a = -a;
    } else {
        x = HALF_PI - (x - 4.712389f);
        x2 = x * x;
        a = a * x2 - b;
        a = a * x2 + c;
    }
    return a;
}

inline f32 alphaf(f32 m, s32 x) { return x * m; }
void effSmallStarMain(EffectEntry* effect) {
    EffSmallStarN64Work* w = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    f32 ang;
    f32 c;
    s32 life;
    s32 i;

    tmp.x = w->position.x;
    tmp.y = w->position.y;
    tmp.z = w->position.z;
    position = tmp;
    w->life--;
    w->frame++;
    life = w->life;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    ang = 6.2831855f * (f32)(w->frame * 6);
    for (i = 0; i < effect->count; i++, w++) {
        if (w->type == 2) {
            c = _cosf(ang / 360.0f);
            w->velocity.y = 4.0f * c;
            w->velocity.x *= 0.94f;
            w->velocity.z *= 0.94f;
            if (w->velocity.y < 0.0f) {
                w->rotVel *= 0.8f;
                w->alpha = (s32)alphaf(0.6f, w->alpha);
            }
        } else {
            if (life > 5) {
                w->velocity.x *= 0.8f;
                w->velocity.y *= 0.8f;
                w->velocity.z *= 0.8f;
            }
            if (life < 20 && life > 5) {
                if (w->type == 0) {
                    w->rotVel += 1.0f;
                }
            } else if (life < 5) {
                w->rotVel *= 0.8f;
                w->alpha = (s32)alphaf(0.6f, w->alpha);
                w->velocity.x *= 0.7f;
                w->velocity.y *= 0.7f;
                w->velocity.z *= 0.7f;
            }
        }
        w->position.x += w->velocity.x;
        w->position.y += w->velocity.y;
        w->position.z += w->velocity.z;
        w->rotation += w->rotVel;
    }
    dispEntry(((EffSmallStarN64Work*)effect->userdata)->camera, dispCalcZ(&position), 2, (DispCallback)effSmallStarDisp, effect);
}

#pragma opt_propagation reset
#pragma opt_lifetimes reset

static effN64Vtx size16x16_tex32x32_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 0xFF},
    {8, -8, 0, 64, 0, 0, 0, 0, 0xFF},
    {8, 8, 0, 64, 32, 0, 0, 0, 0xFF},
    {-8, 8, 0, 0, 32, 0, 0, 0, 0xFF},
};

static u8 color_rotation_data[] = {
    0xFE, 0xAC, 0xAC, 0xFE, 0xAC, 0xD5, 0xFE, 0xB4, 0x9A, 0xD5, 0xB4, 0xFE,
    0xB4, 0xB4, 0xFE, 0xB4, 0xDD, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5,
    0xB4, 0xFE, 0xB4, 0xD5, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5, 0xAC,
};

static inline u8 color_rot_get(s32 idx) {
    return color_rotation_data[idx % 36];
}

void effSmallStarDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    s32 a, b, c;
    EffSmallStarN64Work* w;
    s32 i;
    GXTexObj texObj;
    Mtx mtx;
    Mtx rotY;
    Mtx rotZ;
    GXColor color;
    s32 t;

    camera = camGetPtr(cameraId);
    w = effect->userdata;
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    effGetTexObjN64(0x1D, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x16_tex32x32_vtx);
    t = (w->frame - 1) * 3;
    t = t % 36;
    a = t;
    b = t + 1;
    c = t + 2;
    for (i = 0; i < effect->count; a += 3, b += 3, c += 3, i++, w++) {
        color = (GXColor){0, 0, 0, 0};
        color.r = color_rot_get(a);
        color.g = color_rotation_data[b % 36];
        color.b = color_rotation_data[c % 36];
        color.a = w->alpha;
        GXSetTevColor(GX_TEVREG0, color);
        PSMTXTrans(mtx, w->position.x, w->position.y, w->position.z);
        PSMTXRotRad(rotY, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
        PSMTXRotRad(rotZ, 'z', 0.017453292f * w->rotation);
        PSMTXConcat(mtx, rotY, mtx);
        PSMTXConcat(mtx, rotZ, mtx);
        PSMTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}

void effSmallStarN64CamChg(EffectEntry* effect, u8 camera) {
    EffSmallStarN64Work* w = effect->userdata;
    s32 i;

    for (i = 0; i < effect->count; i++, w++) {
        w->camera = camera;
    }
}
