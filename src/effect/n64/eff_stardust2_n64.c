#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "libc_decls.h"

double sin(double);
double cos(double);

typedef struct EffStardust2N64Work {
    s16 flag;     // 0x0
    s16 type;     // 0x2
    Vec position; // 0x4
    Vec velocity; // 0x10
    f32 scale;    // 0x1c
    s32 frame;    // 0x20
    s32 life;     // 0x24
    s32 color;    // 0x28
    s32 camera;   // 0x2c
} EffStardust2N64Work;

void effStardust2Main(EffectEntry* effect);
void effStardust2Disp(CameraId cameraId, EffectEntry* effect);

static effN64Vtx size8x8_tex22x22_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 0xFF},
    {8, -8, 0, 704, 0, 0, 0, 0, 0xFF},
    {8, 8, 0, 704, 704, 0, 0, 0, 0xFF},
    {-8, 8, 0, 0, 704, 0, 0, 0, 0xFF},
    {-8, -8, 0, 704, 0, 0, 0, 0, 0xFF},
    {8, -8, 0, 1408, 0, 0, 0, 0, 0xFF},
    {8, 8, 0, 1408, 704, 0, 0, 0, 0xFF},
    {-8, 8, 0, 704, 704, 0, 0, 0, 0xFF},
    {-8, -8, 0, 1408, 0, 0, 0, 0, 0xFF},
    {8, -8, 0, 2112, 0, 0, 0, 0, 0xFF},
    {8, 8, 0, 2112, 704, 0, 0, 0, 0xFF},
    {-8, 8, 0, 1408, 704, 0, 0, 0, 0xFF},
    {-8, -8, 0, 2112, 0, 0, 0, 0, 0xFF},
    {8, -8, 0, 2816, 0, 0, 0, 0, 0xFF},
    {8, 8, 0, 2816, 704, 0, 0, 0, 0xFF},
    {-8, 8, 0, 2112, 704, 0, 0, 0, 0xFF},
    {-8, -8, 0, 2816, 0, 0, 0, 0, 0xFF},
    {8, -8, 0, 3520, 0, 0, 0, 0, 0xFF},
    {8, 8, 0, 3520, 704, 0, 0, 0, 0xFF},
    {-8, 8, 0, 2816, 704, 0, 0, 0, 0xFF},
    {-8, -8, 0, 3520, 0, 0, 0, 0, 0xFF},
    {8, -8, 0, 4224, 0, 0, 0, 0, 0xFF},
    {8, 8, 0, 4224, 704, 0, 0, 0, 0xFF},
    {-8, 8, 0, 3520, 704, 0, 0, 0, 0xFF},
    {-8, -8, 0, 4224, 0, 0, 0, 0, 0xFF},
    {8, -8, 0, 4928, 0, 0, 0, 0, 0xFF},
    {8, 8, 0, 4928, 704, 0, 0, 0, 0xFF},
    {-8, 8, 0, 4224, 704, 0, 0, 0, 0xFF},
    {-8, -8, 0, 4928, 0, 0, 0, 0, 0xFF},
    {8, -8, 0, 5632, 0, 0, 0, 0, 0xFF},
    {8, 8, 0, 5632, 704, 0, 0, 0, 0xFF},
    {-8, 8, 0, 4928, 704, 0, 0, 0, 0xFF},

};

static u8 color_rotation_data[] = {
    0xFE, 0xAC, 0xAC, 0xFE, 0xAC, 0xD5, 0xFE, 0xB4, 0x9A, 0xD5, 0xB4, 0xFE,
    0xB4, 0xB4, 0xFE, 0xB4, 0xDD, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5,
    0xB4, 0xFE, 0xB4, 0xD5, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5, 0xAC,
};

static u8 speed_data[] = {0x1E, 0x3C, 0x5A, 0x64, 0x68, 0x6A, 0x6C, 0x6E, 0x70, 0x71, 0x71, 0x6E, 0x6B, 0x67, 0, 0};

static u8 scale_data[] = {0x1E, 0x3C, 0x5A, 0x64, 0x68, 0x6A, 0x6C, 0x6E, 0x70, 0x71, 0x71, 0x6E, 0x6B, 0x67, 0, 0};

EffectEntry* effStardust2N64Entry(s32 type, s32 count, s32 camera, f32 x, f32 y, f32 z, f32 vx, f32 vy) {
    s32 n;
    EffectEntry* effect;
    EffStardust2N64Work* w;
    EffStardust2N64Work* p;
    s32 i;
    f32 step, stepY;
    f32 a, sn, cs, b, fy;
    s32 r;
    f32 tx, ty;
    f32 bx, ax, by, ay;
    f32 nm;

    effect = effEntry();
    effect->type = "Stardust2N64";
    effect->count = count + 1;
    w = __memAlloc(HEAP_EFFECT, sizeof(EffStardust2N64Work) * (count + 1));
    effect->userdata = w;
    effect->callback = effStardust2Main;
    if (type == 6) {
        w->flag = 1;
        type = 3;
    } else {
        w->flag = 0;
    }
    n = count + 1;
    w->position.x = x;
    w->position.y = y;
    w->position.z = z;
    w->frame = 0;
    w->type = type;
    w->life = camera;
    w->camera = 4;
    nm = (f32)n - 1.0f;
    bx = 0.1f * vx;
    by = 0.1f * vy;
    ax = 0.4f * vx;
    ay = 0.4f * vy;
    p = w + 1;
    for (i = 1; i < n; i++, p++) {
        f32 spread = 360.0f / nm * (f32)i;
        if (type > 1) {
            r = rand() % 360;
        } else {
            r = 0;
        }
        a = 6.2831855f * (spread + (f32)r) / 360.0f;
        sn = (f32)sin(a);
        cs = (f32)cos(a);
        b = (f32)sin(6.2831855f * (10.0f * spread - 90.0f) / 360.0f);
        fy = ((f32)i - 1.0f) * (vy / nm);
        tx = ax - bx * b;
        ty = ay - by * b;
        p->life = i + 30;
        p->frame = 0;
        p->scale = 1.0f;
        switch (type) {
        case 0:
        case 1:
            p->velocity.z = 0.0f;
            p->velocity.x = sn * tx;
            p->velocity.y = cs * ty;
            p->position.z = 0.0f;
            break;
        case 2:
        case 5:
            p->velocity.x = sn * tx;
            p->position.y = fy;
            p->position.z = cs * tx;
            p->velocity.y = 0.03f * (f32)(-(rand() % 10));
            p->velocity.z = 0.04f * (f32)(rand() % 10) + 0.01f;
            break;
        case 3:
            p->position.x = 0.005f * (sn * vx * (f32)(rand() % 100));
            p->position.y = 0.005f * (cs * vy * (f32)(rand() % 100));
            p->position.z = 0.0f;
            p->velocity.y = 0.0f;
            p->velocity.x = 0.0f;
            p->velocity.z = 0.0f;
            p->life += i + i;
            break;
        default:
            p->velocity.x = sn * tx;
            p->position.y = fy;
            p->position.z = cs * tx;
            p->velocity.y = 0.03f * (f32)(-(rand() % 10));
            p->velocity.z = 0.04f * (f32)(rand() % 10) + 0.01f;
            break;
        }
    }
    return effect;
}

#pragma opt_propagation off
void effStardust2Main(EffectEntry* effect) {
    EffStardust2N64Work* w = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;
    s32 done = 0;
    s32 frame;
    s32 camera;
    s32 idx;
    f32 ang;
    f32 s;

    tmp.x = w->position.x;
    tmp.y = w->position.y;
    tmp.z = w->position.z;
    position = tmp;
    camera = w->camera;
    w->life--;
    w->frame++;
    type = w->type;
    frame = w->frame;
    if (((u32)type <= 1 || type == 3 || type >= 10) && w->life < 0) {
        effDelete(effect);
        return;
    }
    w++;
    for (i = 0; i < effect->count - 1; i++, w++) {
        switch (type) {
        case 0:
        case 1:
            w->color = (frame + i) & 7;
            w->velocity.z += -0.02f;
            idx = frame - 1;
            if (idx < 14) {
                s = 0.01f * (f32)speed_data[idx];
                w->position.x = w->velocity.x * s;
                w->position.y = w->velocity.y * s + w->velocity.z;
                w->scale = 0.01f * (f32)scale_data[idx];
            } else {
                f32 one = 1.0f;
                f32 base = 360.0f / (f32)(effect->count - 1) * (f32)i + 60.0f;
                if (i & 1) {
                    base -= 120.0f;
                }
                w->type = 10;
                ang = 6.2831855f * base / 360.0f;
                w->velocity.x = one * (f32)sin(ang);
                w->velocity.y = one * (f32)cos(ang);
                w->velocity.z = -0.05f;
            }
            break;
        case 10:
        case 30:
            w->color = (frame + i) & 7;
            w->velocity.x *= 0.96f;
            w->velocity.y *= 0.96f;
            w->velocity.y += w->velocity.z;
            w->position.x += w->velocity.x;
            w->position.y += w->velocity.y;
            w->scale += 0.1f * (0.1f - w->scale);
            break;
        case 3:
            w->color = (frame + i + i) & 7;
            idx = frame - 1;
            if (idx < 14) {
                w->scale = 0.01f * (f32)scale_data[idx];
            } else {
                w->type = 30;
            }
            break;
        case 2:
        case 4:
        case 5:
        default:
            w->color = (frame + i + i) & 7;
            if (w->life < 0) {
                done++;
            }
            w->life--;
            if (w->life > 30 || w->life < 0) {
                w->color = -1;
                break;
            }
            w->frame++;
            frame = w->frame;
            idx = frame - 1;
            if (idx < 14) {
                w->scale = 0.01f * (f32)scale_data[idx];
                if (type == 5) {
                    w->velocity.y += w->velocity.z;
                    w->position.y += w->velocity.y;
                }
            } else {
                w->scale += 0.1f * (0.1f - w->scale);
                w->velocity.y += w->velocity.z;
                w->position.y += w->velocity.y;
            }
            if (type == 4) {
                ang = 6.2831855f * (f32)(frame * 12) / 360.0f;
                w->position.x = w->velocity.x * (f32)sin(ang);
                w->position.z = w->velocity.x * (f32)cos(ang);
            } else {
                w->position.x = w->velocity.x;
            }
            w->velocity.y *= 0.96f;
            break;
        }
    }
    if ((u32)type > 1 && type != 3 && type < 10 && done >= effect->count - 1) {
        effDelete(effect);
        return;
    }
    dispEntry(camera, dispCalcZ(&position), 2, (DispCallback)effStardust2Disp, effect);
}

#pragma opt_propagation reset
void effStardust2Disp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* cam;
    EffStardust2N64Work* w;
    s32 type;
    s32 i;
    s32 camera;
    s32 idx;
    effN64Vtx* vtx;
    Mtx mtx2;
    Mtx texMtx;
    Mtx rotY;
    Mtx mtx;
    GXTexObj texObj;
    f32 fr, fg, fb;
    s32 r, g, b;

    cam = camGetPtr(cameraId);
    w = effect->userdata;
    type = w->type;
    idx = (w->frame - 1) * 3;
    PSMTXTrans(mtx2, w->position.x, w->position.y, w->position.z);
    PSMTXRotRad(rotY, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx2, rotY, mtx);
    PSMTXConcat(cam->view, mtx, mtx);
    w++;
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(texMtx, 0.0056818184f, 0.045454547f, 0.0f);
    GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x20, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    switch (type) {
    case 0:
    case 1:
    case 2:
    case 10:
        fr = 1.0f;
        fg = 0.5f;
        fb = 0.1f;
        break;
    case 3:
    case 30:
        fr = 1.0f;
        fg = 0.8f;
        fb = 0.7f;
        break;
    case 5:
        fr = 1.4f;
        fb = 0.7f;
        fg = fr;
        break;
    default:
        fg = 0.7f;
        fr = 1.4f;
        fb = fg;
        break;
    }
    for (i = 0; i < effect->count - 1; i++, w++) {
        camera = w->color;
        if (camera < 0) {
            continue;
        }
        PSMTXTrans(mtx2, w->position.x, w->position.y, w->position.z);
        mtx2[2][2] = w->scale;
        mtx2[1][1] = w->scale;
        mtx2[0][0] = w->scale;
        PSMTXConcat(mtx, mtx2, mtx2);
        GXLoadPosMtxImm(mtx2, GX_PNMTX0);
        GXSetCurrentMtx(0);
        if (idx >= 36) {
            idx = 0;
        }
        r = (s32)((f32)color_rotation_data[idx++] * fr);
        g = (s32)((f32)color_rotation_data[idx++] * fg);
        b = (s32)((f32)color_rotation_data[idx++] * fb);
        if (r > 255) {
            r = 255;
        }
        if (g > 255) {
            g = 255;
        }
        if (b > 255) {
            b = 255;
        }
        GXSetTevColor(GX_TEVREG0, (GXColor){r, g, b, 255});
        switch (camera & 7) {
        case 0:
            vtx = size8x8_tex22x22_vtx + 12;
            break;
        case 1:
            vtx = size8x8_tex22x22_vtx + 16;
            break;
        case 2:
            vtx = size8x8_tex22x22_vtx + 20;
            break;
        case 3:
            vtx = size8x8_tex22x22_vtx + 24;
            break;
        case 4:
            vtx = size8x8_tex22x22_vtx + 28;
            break;
        case 5:
            vtx = size8x8_tex22x22_vtx;
            break;
        case 6:
            vtx = size8x8_tex22x22_vtx + 4;
            break;
        case 7:
            vtx = size8x8_tex22x22_vtx + 8;
            break;
        }
        effSetVtxDescN64(vtx);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}

void effStardust2N64SetDrawCam(EffectEntry* effect, s32 camera) {
    EffStardust2N64Work* w = effect->userdata;
    w->camera = camera;
}
