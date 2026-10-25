#include "eff/n64/eff_snow_dust_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "system.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

double sin(double);
double cos(double);
s32 rand(void);
s32 irand(s32);

void effSnowDustMain(EffectEntry* effect);
void effSnowDustDisp(CameraId cameraId, EffectEntry* effect);


effN64Vtx size8x8_tex64x64_vtx[] = {
    {-4, -4, 0, 0, 0, 0, 0, 0, 255},
    {4, -4, 0, 2048, 0, 0, 0, 0, 255},
    {4, 4, 0, 2048, 2048, 0, 0, 0, 255},
    {-4, 4, 0, 0, 2048, 0, 0, 0, 255},
};

effN64Vtx size4x4_tex64x64_vtx[] = {
    {-2, -2, 0, 0, 0, 0, 0, 0, 255},
    {2, -2, 0, 2048, 0, 0, 0, 0, 255},
    {2, 2, 0, 2048, 2048, 0, 0, 0, 255},
    {-2, 2, 0, 0, 2048, 0, 0, 0, 255},
};

static effN64Vtx size12x12_tex64x64_vtx[] = {
    {-6, -6, 0, 0, 0, 0, 0, 0, 255},
    {6, -6, 0, 2048, 0, 0, 0, 0, 255},
    {6, 6, 0, 2048, 2048, 0, 0, 0, 255},
    {-6, 6, 0, 0, 2048, 0, 0, 0, 255},
};

static effN64Vtx size16x16_tex64x64_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 2048, 0, 0, 0, 0, 255},
    {8, 8, 0, 2048, 2048, 0, 0, 0, 255},
    {-8, 8, 0, 0, 2048, 0, 0, 0, 255},
};

static effN64Vtx size32x32_tex32x32_vtx[5] = {
    {-32, 0, 0, 0, 0, 255, 255, 255, 0},
    {0, -32, 0, 1024, 0, 255, 255, 255, 0},
    {32, 0, 0, 1024, 1024, 255, 255, 255, 0},
    {0, 32, 0, 0, 1024, 255, 255, 255, 0},
    {0, 0, 0, 0, 0, 255, 255, 255, 255},
};

static u8 scale_data[] = {
    2, 4, 8, 13, 15, 13, 9, 6, 4, 3, 2, 4, 8, 13, 15, 13, 9, 6, 4, 3, 2, 4, 8, 13, 15, 13, 9, 6, 4, 3,
};

static u8 scale_data2[] = {
    40, 90, 81, 72, 65, 59, 53, 47, 43, 38, 34, 31, 28, 25, 22, 20, 18, 16, 15, 13, 12, 10, 9, 8, 7, 7, 6, 5, 5, 4,
};

static u8 a_data[] = {
    200, 250, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 250, 200
};

EffectEntry* effSnowDustN64Entry(s32 type, s32 count, s32 lifetime, f32 x, f32 y, f32 z, f32 w, f32 h, f32 scale) {
    EffectEntry* effect;
    EffSnowDustN64Work* work;
    EffSnowDustN64Work* wp;
    f32 step;
    s32 i;

    effect = effEntry();
    effect->type = "SnowDustN64";
    effect->count = count + 1;
    work = __memAlloc(HEAP_EFFECT, (count + 1) * sizeof(EffSnowDustN64Work));
    effect->userdata = work;
    effect->callback = effSnowDustMain;
    effect->flags |= 2;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->alpha = 255;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->width = w;
    work->height = h;
    work->camId = 4;
    work->r = 255;
    work->g = 255;
    work->b = 255;
    switch (type) {
        case 0:
            work->kr = 255;
            work->kg = 255;
            work->kb = 255;
            work->ka = 255;
            break;
        case 1:
            work->kr = 0;
            work->kg = 127;
            work->kb = 255;
            work->ka = 255;
            break;
        case 2:
        case 3:
            work->kr = 255;
            work->kg = 255;
            work->kb = 128;
            work->ka = 255;
            break;
        case 6:
            work->r = 203;
            work->g = 148;
            work->b = 205;
            work->kr = 203;
            work->kg = 148;
            work->kb = 205;
            break;
        default:
            work->r = rand() % 127 + 128;
            work->g = rand() % (127 - (work->r - 128)) + 128;
            work->b = rand() % (127 - (work->r - 128) - (work->g - 128)) + 128;
            work->kr = 127;
            work->kg = 127;
            work->kb = 127;
            work->ka = 127;
            break;
    }
    if (work->lifetime < 30) {
        step = (f32)work->lifetime / (f32)count;
    } else {
        step = 30.0f / (f32)count;
    }
    wp = work + 1;
    for (i = 1; i < count + 1; i++, wp++) {
        wp->seq = (s32)(-(f32)i * step) - 1;
    }
    return effect;
}

void effSnowDustMain(EffectEntry* effect) {
    s32 wi, hi;
    s32 lifetime;
    EffSnowDustN64Work* work = effect->userdata;
    s32 i;
    s32 type;
    s32 r, g, b;
    s32 camId;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    f32 cs, sn, half;
    f32 px, py, pz;
    f32 w, h;
    f32 d;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    camId = work->camId;
    type = work->type;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->lifetime = 16;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    lifetime = work->lifetime;
    if (lifetime < 0) {
        effDelete(effect);
        return;
    }
    if (lifetime < 8) {
        work->alpha = lifetime << 5;
    }
    w = work->width;
    h = work->height;
    px = work->position.x;
    py = work->position.y;
    pz = work->position.z;
    cs = (f32)cos(6.2831855f * camGetPtr(CAMERA_3D)->viewYaw / 360.0f);
    sn = (f32)sin(6.2831855f * camGetPtr(CAMERA_3D)->viewYaw / 360.0f);
    wi = (s32)w;
    half = 0.5f * w;
    hi = (s32)h;
    r = work->r;
    g = work->g;
    b = work->b;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        work->seq++;
        if (work->seq >= 30) {
            if (lifetime < 30) {
                work->seq = 20;
            } else {
                work->seq = 0;
            }
        }
        if (work->seq < 0) {
            continue;
        }
        if (work->seq == 0) {
            h = (f32)(rand() % wi) - half;
            work->position.x = h * cs + px - sn;
            work->position.y = py + (f32)(rand() % hi);
            work->position.z = h * sn + pz + cs;
            work->r = r;
            work->g = g;
            work->b = b;
        }
        switch (type) {
            case 0:
            case 1:
                if (work->seq == 0) {
                    work->vx = 0.0f;
                    work->vy = 0.0f;
                    work->tx = 0.0f;
                    work->ty = 0.05f * (f32)(-(rand() % 10) - 2);
                    work->u = 0.0f;
                    work->v = 0.0f;
                    work->scale = 0.05f * (f32)(rand() % 10) + 0.5f;
                }
                work->alpha = a_data[work->seq];
                work->position.x += work->vx;
                work->position.y += work->vy;
                work->u += 0.2f;
                work->v += 0.6f;
                work->vx += 0.04f * (work->tx - work->vx);
                work->vy += 0.04f * (work->ty - work->vy);
                break;
            case 2:
            case 3:
                if (work->seq == 0) {
                    if (type == 2) {
                        work->vx = 0.05f * (f32)(rand() % 20 - 10);
                        work->vy = 0.05f * (f32)(rand() % 20 - 10);
                    } else {
                        work->vx = 0.0f;
                        work->vy = 0.0f;
                    }
                    work->tx = 0.0f;
                    work->ty = 0.0f;
                    work->u = (f32)(rand() % 15);
                    work->v = (f32)(rand() % 15);
                    work->alpha = 255;
                }
                work->scale = 0.1f * (f32)scale_data[work->seq];
                work->position.x += work->vx;
                work->position.y += work->vy;
                work->u += 0.2f;
                work->v += 0.6f;
                work->vx += 0.04f * (work->tx - work->vx);
                work->vy += 0.04f * (work->ty - work->vy);
                break;
            case 6:
                if (work->seq == 0) {
                    work->vx = 0.0f;
                    work->vy = 0.0f;
                    work->tx = 0.0f;
                    work->ty = 0.05f * (f32)(-(rand() % 10) - 2);
                    work->u = 0.0f;
                    work->v = 0.0f;
                    work->scale = 0.05f * (f32)(rand() % 10) + 0.5f;
                    work->frame = irand(360);
                }
                work->alpha = a_data[work->seq];
                work->alpha = 255;
                work->position.y += work->vy;
                work->u += 0.2f;
                work->v += 0.6f;
                work->vy += 0.04f * (work->ty - work->vy);
                d = distABf(position.x, position.z, work->position.x, work->position.z);
                d *= 0.97f;
                work->position.x = position.x + d * (f32)sin(6.2831855f * (f32)work->frame / 360.0f);
                work->position.z = position.z + d * (f32)cos(6.2831855f * (f32)work->frame / 360.0f);
                work->frame += 4;
                work->frame %= 360;
                break;
            default:
                if (work->seq == 0) {
                    work->u = 0.0f;
                    work->v = 0.0f;
                    work->alpha = 255;
                    work->scale = 1.0f;
                    work->r = rand() % 127 + 128;
                    work->g = rand() % (127 - (work->r - 128)) + 128;
                    work->b = rand() % (127 - (work->r - 128) - (work->g - 128)) + 128;
                }
                if (type == 4) {
                    work->scale = 0.01f * (f32)scale_data2[work->seq];
                } else {
                    work->scale = 0.01f * (f32)scale_data[work->seq];
                }
                break;
        }
    }
    dispEntry(camId, dispCalcZ(&position), 2, (DispCallback)effSnowDustDisp, effect);
}

void effSnowDustDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* cam;
    EffSnowDustN64Work* work;
    s32 i;
    s32 type;
    f32 scale;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx rotMtx;
    Mtx texMtx;
    Mtx texMtx1;

    cam = camGetPtr(cameraId);
    work = effect->userdata;
    scale = (f32)work->alpha / 255.0f * work->scale;
    type = work->type;
    GXSetTevColor(GX_TEVREG1, (GXColor){work->kr, work->kg, work->kb, work->ka});
    switch (type) {
        case 0:
        case 6:
            GXSetNumChans(0);
            GXSetNumTevStages(2);
            GXSetTevOrder(0, 0, 0, 0xff);
            GXSetTevColorOp(0, 0, 0, 0, 1, 0);
            GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
            GXSetTevColorIn(0, 0xf, 0xf, 0xf, 0xf);
            GXSetTevAlphaIn(0, 7, 4, 1, 7);
            GXSetTevOrder(1, 1, 1, 0xff);
            GXSetTevColorOp(1, 0, 0, 0, 1, 0);
            GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
            GXSetTevColorIn(1, 4, 2, 8, 0xf);
            GXSetTevAlphaIn(1, 7, 7, 7, 0);
            effGetTexObjN64(8, &texObj);
            GXLoadTexObj(&texObj, 0);
            effGetTexObjN64(0x65, &texObj);
            GXLoadTexObj(&texObj, 1);
            GXSetNumTexGens(2);
            GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
            GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7d);
            PSMTXScale(texMtx, 0.015625f, 0.015625f, 0.0f);
            PSMTXScale(texMtx1, 0.015625f, 0.015625f, 0.0f);
            break;
        case 1:
            GXSetNumChans(0);
            GXSetNumTevStages(2);
            GXSetTevOrder(0, 0, 0, 0xff);
            GXSetTevColorOp(0, 0, 0, 0, 1, 0);
            GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
            GXSetTevColorIn(0, 0xf, 0xf, 0xf, 0xf);
            GXSetTevAlphaIn(0, 7, 4, 1, 7);
            GXSetTevOrder(1, 1, 1, 0xff);
            GXSetTevColorOp(1, 0, 0, 0, 1, 0);
            GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
            GXSetTevColorIn(1, 4, 2, 8, 0xf);
            GXSetTevAlphaIn(1, 7, 7, 7, 0);
            GXSetTevOrder(2, 1, 1, 0xff);
            GXSetTevColorOp(2, 0, 0, 0, 1, 0);
            GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
            GXSetTevColorIn(2, 0, 0xc, 8, 0xf);
            GXSetTevAlphaIn(2, 7, 7, 7, 0);
            effGetTexObjN64(0x66, &texObj);
            GXLoadTexObj(&texObj, 0);
            effGetTexObjN64(0x65, &texObj);
            GXLoadTexObj(&texObj, 1);
            GXSetNumTexGens(2);
            GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
            GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7d);
            PSMTXScale(texMtx, 0.03125f, 0.03125f, 0.0f);
            PSMTXScale(texMtx1, 0.03125f, 0.03125f, 0.0f);
            break;
        case 2:
        case 3:
            GXSetNumChans(0);
            GXSetNumTevStages(1);
            GXSetTevOrder(0, 0, 0, 0xff);
            GXSetTevColorOp(0, 0, 0, 0, 1, 0);
            GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
            GXSetTevColorIn(0, 4, 2, 8, 0xf);
            GXSetTevAlphaIn(0, 7, 4, 1, 7);
            effGetTexObjN64(0x67, &texObj);
            GXLoadTexObj(&texObj, 0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
            PSMTXScale(texMtx, 0.00390625f, 0.015625f, 0.0f);
            break;
        case 4:
        case 5:
            GXSetNumChans(0);
            GXSetNumTevStages(2);
            GXSetTevOrder(0, 0, 0, 0xff);
            GXSetTevColorOp(0, 0, 0, 0, 1, 0);
            GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
            GXSetTevColorIn(0, 0xf, 8, 5, 2);
            GXSetTevAlphaIn(0, 7, 4, 1, 7);
            GXSetTevOrder(1, 0xff, 0xff, 0xff);
            GXSetTevColorOp(1, 1, 0, 0, 1, 0);
            GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
            GXSetTevColorIn(1, 0xf, 4, 5, 0);
            GXSetTevAlphaIn(1, 7, 7, 7, 0);
            effGetTexObjN64(0x68, &texObj);
            GXLoadTexObj(&texObj, 0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
            PSMTXScale(texMtx, 0.015625f, 0.015625f, 0.0f);
            break;
    }
    GXSetCullMode(0);
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->seq < 0) {
            continue;
        }
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rotMtx, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
        PSMTXConcat(mtx, rotMtx, mtx);
        PSMTXScale(scaleMtx, work->scale * scale, work->scale * scale, work->scale * scale);
        PSMTXConcat(mtx, scaleMtx, mtx);
        PSMTXConcat(cam->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, 0);
        GXSetCurrentMtx(0);
        GXSetTevColor(1, (GXColor){work->r, work->g, work->b, (s32)(scale * (f32)work->alpha)});
        GXSetTevKColor(0, (GXColor){80 - work->seq, 80 - work->seq, 80 - work->seq, 80 - work->seq});
        GXLoadTexMtxImm(texMtx, 0x1e, 1);
        PSMTXTrans(mtx, work->u, work->v, 0.0f);
        PSMTXConcat(texMtx1, mtx, mtx);
        GXLoadTexMtxImm(mtx, 0x21, 1);
        if (type == 2) {
            PSMTXTrans(mtx, (f32)((i & 0x1f) << 6), 64.0f, 0.0f);
            PSMTXConcat(texMtx, mtx, mtx);
            GXLoadTexMtxImm(mtx, 0x1e, 1);
        }
        switch (type) {
            case 0:
            case 6:
                effSetVtxDescN64(size4x4_tex64x64_vtx);
                GXBegin(0x90, 0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
                break;
            case 1:
                effSetVtxDescN64(size8x8_tex64x64_vtx);
                GXBegin(0x90, 0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
                break;
            case 2:
            case 3:
                effSetVtxDescN64(size12x12_tex64x64_vtx);
                GXBegin(0x90, 0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
                break;
            case 4:
            case 5:
                GXSetNumChans(0);
                GXSetNumTevStages(3);
                GXSetTevOrder(0, 0, 0, 0xff);
                GXSetTevColorOp(0, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
                GXSetTevColorIn(0, 0xf, 8, 5, 2);
                GXSetTevAlphaIn(0, 7, 4, 1, 7);
                GXSetTevOrder(1, 0xff, 0xff, 0xff);
                GXSetTevColorOp(1, 1, 0, 0, 1, 0);
                GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
                GXSetTevColorIn(1, 0xf, 4, 5, 0);
                GXSetTevAlphaIn(1, 7, 7, 7, 0);
                GXSetTevOrder(2, 0xff, 0xff, 0xff);
                GXSetTevColorOp(2, 1, 0, 0, 1, 0);
                GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
                GXSetTevColorIn(2, 0xe, 0xf, 0xf, 0);
                GXSetTevAlphaIn(2, 7, 7, 7, 0);
                GXSetTevKColorSel(2, 0xc);
                effSetVtxDescN64(size16x16_tex64x64_vtx);
                GXBegin(0x90, 0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
                GXSetNumChans(1);
                GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
                GXSetNumTevStages(1);
                GXSetTevOrder(0, 0xff, 0xff, 4);
                GXSetTevColorOp(0, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
                GXSetTevColorIn(0, 0xf, 0xf, 0xf, 0xc);
                GXSetTevAlphaIn(0, 7, 5, 6, 7);
                GXSetTevKAlphaSel(0, 0x1c);
                effSetVtxDescN64(size32x32_tex32x32_vtx);
                GXBegin(0x90, 0, 12);
                tri2(0, 1, 4, 0, 1, 2, 4, 0);
                tri2(0, 4, 3, 0, 4, 2, 3, 0);
                break;
        }
    }
}
