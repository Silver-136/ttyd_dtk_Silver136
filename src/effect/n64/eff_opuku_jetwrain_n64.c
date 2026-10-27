#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_n64.h"
#include "eff/n64/eff_opuku_jetwrain_n64.h"

double sin(double);




typedef struct EffOpukuJetwrainWork {
    s32 type;      // 0x0
    Vec position;  // 0x4
    s32 lifetime;  // 0x10
    s32 frame;     // 0x14
    f32 scale;     // 0x18
    u8 col[3];     // 0x1C
    u8 alpha;      // 0x1F
    u8 col2[3];    // 0x20
    s8 h[19][13];  // 0x23
    s16 v[19][13]; // 0x11A
} EffOpukuJetwrainWork;


static effN64Vtx opuku_jetwrain_vtx[16] = {
    {-160, -120, -320, 0, 0, 0, 0, 0, 255},
    {-144, -120, -320, 0x200, 0, 0, 0, 0, 255},
    {-160, -104, -320, 0, 0x200, 0, 0, 0, 255},
    {-144, -104, -320, 0x200, 0x200, 0, 0, 0, 255},
    {-160, -120, -320, 0, 0, 0, 0, 0, 255},
    {-144, -120, -320, 0x280, 0, 0, 0, 0, 255},
    {-160, -104, -320, 0, 0x200, 0, 0, 0, 255},
    {-144, -104, -320, 0x280, 0x200, 0, 0, 0, 255},
    {-160, -120, -320, 0, 0, 0, 0, 0, 255},
    {-144, -120, -320, 0x200, 0, 0, 0, 0, 255},
    {-160, -104, -320, 0, 0x280, 0, 0, 0, 255},
    {-144, -104, -320, 0x200, 0x280, 0, 0, 0, 255},
    {-160, -120, -320, 0, 0, 0, 0, 0, 255},
    {-144, -120, -320, 0x280, 0, 0, 0, 0, 255},
    {-160, -104, -320, 0, 0x280, 0, 0, 0, 255},
    {-144, -104, -320, 0x280, 0x280, 0, 0, 0, 255},
};

void effOpukuJetwrainDisp(CameraId cameraId, EffectEntry* effect);
void effOpukuJetwrainMain(EffectEntry* effect);

inline f32 sind(f32 deg) { return sin(6.2831855f * deg / 360.0f); }

inline int inline_fn2(s32 a) { return a < 12; }
inline int inline_fn(s32 n) { return n < 13; }

EffectEntry* effOpukuJetwrainN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale, s32 lifetime) {
    EffectEntry* effect;
    EffOpukuJetwrainWork* work;
    s32 i;
    s32 j;

    effect = effEntry();
    effect->type = "OpukuJetwrainN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffOpukuJetwrainWork));
    effect->userdata = work;
    effect->callback = effOpukuJetwrainMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->alpha = 0;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->col[0] = 10;
    work->col[1] = 110;
    work->col[2] = 255;
    work->col2[0] = 190;
    work->col2[1] = 220;
    work->col2[2] = 255;
    for (i = 0; i < 19; i++) {
        for (j = 0; inline_fn(j); j++) {
            work->h[i][j] = 0;
            work->v[i][j] = 0;
        }
    }
    return effect;
}

void effOpukuJetwrainMain(EffectEntry* effect) {
    s32 life;
    EffOpukuJetwrainWork* work;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 j;
    s32 frame;
    s32 alpha;
    f32 a;

    work = effect->userdata;
    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->lifetime = 32;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    life = work->lifetime;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    alpha = 255;
    if (life < 32) {
        alpha = life * 8;
    }
    if (frame < 16) {
        alpha = frame * 16 + 15;
    }
    work->alpha = alpha;
    a = (f32)alpha / 255.0f;
    for (i = 1; i < 18; i++) {
        work->h[i][6] = a * (-64.0f * sind((f32)((frame - i) * -20)));
    }
    for (i = 1; i < 18; i++) {
        for (j = 1; j < 12; j++) {
            f32 c4 = 4.0f * (f32)work->h[i][j];
            f32 corners = (f32)work->h[i - 1][j - 1] + ((f32)work->h[i + 1][j - 1] + ((f32)work->h[i + 1][j + 1] + (f32)work->h[i - 1][j + 1]));
            f32 edges = (f32)work->h[i][j - 1] + ((f32)work->h[i][j + 1] + ((f32)work->h[i + 1][j] + (f32)work->h[i - 1][j]));
            f32 half = 0.5f * corners;
            work->v[i][j] += (half + (edges - c4)) - 0.7f * c4;
            work->v[i][j] *= 0.98f;
            if (life < 32) {
                work->h[i][j] *= a;
            }
        }
    }
    for (i = 1; i < 18; i++) {
        for (j = 1; inline_fn2(j); j++) {
            work->h[i][j] += 0.02f * work->v[i][j];
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effOpukuJetwrainDisp, effect);
}

void effOpukuJetwrainDisp(CameraId cameraId, EffectEntry* effect) {
    s32 px;
    EffOpukuJetwrainWork* work;
    s32 alpha;
    Mtx trans;
    Mtx texMtx;
    Mtx44 ortho;
    Mtx view;
    GXTexObj texObj;
    f32 proj[7];
    void* buf;
    s32 j, i;
    s32 edgeY;
    s32 x1;
    s32 y0;
    s32 y1;
    s32 by, bx;
    s32 x0;
    s32 k;
    s32 edgeX;

    camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->alpha;
    GXGetProjectionv(proj);
    C_MTXOrtho(ortho, 0.0f, 480.0f, 0.0f, 608.0f, 1.0f, 32768.0f);
    GXSetProjection(ortho, GX_ORTHOGRAPHIC);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    buf = smartAlloc(GXGetTexBufferSize(304, 240, GX_TF_RGB565, GX_FALSE, 0), 3)->address;
    GXSetTexCopySrc(0, 0, 608, 480);
    GXSetTexCopyDst(304, 240, GX_TF_RGB565, GX_TRUE);
    GXCopyTex(buf, GX_FALSE);
    GXPixModeSync();
    GXInitTexObj(&texObj, buf, 304, 240, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_A0, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->col[0], work->col[1], work->col[2], alpha >> 1});
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(opuku_jetwrain_vtx);
    {
        Vec up = {0.0f, 1.0f, 0.0f};
        Vec target = {304.0f, 240.0f, 0.0f};
        Vec cam = {304.0f, 240.0f, 100.0f};
        C_MTXLookAt(view, &cam, &up, &target);
    }
    GXLoadPosMtxImm(view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    for (i = 0; i < 12; i++) {
        by = i * 16 + 24;
        edgeY = 0;
        if (i == 0) {
            y0 = -4;
            edgeY = 1;
        } else {
            y0 = 0;
        }
        if (i == 11) {
            y1 = 4;
            edgeY = 1;
        } else {
            y1 = 0;
        }
        for (j = 0; j < 18; j++) {
            bx = j * 16 + 16;
            edgeX = 0;
            if (j == 0) {
                x0 = -4;
                edgeX = 1;
            } else {
                x0 = 0;
            }
            if (j == 17) {
                x1 = 4;
                edgeX = 1;
            } else {
                x1 = 0;
            }
            PSMTXScale(texMtx, 0.00390625f, 0.00390625f, 1.0f);
            px = bx + x0;
            PSMTXTrans(trans, px, by + y0, 0.0f);
            PSMTXConcat(texMtx, trans, texMtx);
            GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
            if (edgeX != 0 && edgeY != 0) {
                k = 12;
            } else if (edgeX != 0) {
                k = 4;
            } else if (edgeY) {
                k = 8;
            } else {
                k = 0;
            }
            opuku_jetwrain_vtx[k].x = px * 4;
            opuku_jetwrain_vtx[k].y = work->h[j][i] + (by + y0) * 4;
            opuku_jetwrain_vtx[k + 1].x = (bx + x1 + 16) * 4;
            opuku_jetwrain_vtx[k + 1].y = work->h[j + 1][i] + (by + y0) * 4;
            opuku_jetwrain_vtx[k + 2].x = px * 4;
            opuku_jetwrain_vtx[k + 2].y = work->h[j][i + 1] + (by + y1 + 16) * 4;
            opuku_jetwrain_vtx[k + 3].x = (bx + x1 + 16) * 4;
            opuku_jetwrain_vtx[k + 3].y = work->h[j + 1][i + 1] + (by + y1 + 16) * 4;
            DCFlushRange(opuku_jetwrain_vtx, sizeof(opuku_jetwrain_vtx));
            GXInvalidateVtxCache();
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(k, k + 3, k + 1, 0, k, k + 2, k + 3, 0);
            sysWaitDrawSync();
        }
    }
    GXSetProjectionv(proj);
}


