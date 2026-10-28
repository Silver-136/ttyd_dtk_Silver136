#include "eff/n64/eff_opuku_jetw_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_n64.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

typedef struct JetwEnt {
    s8 x, y;
    s16 sx, sy, rot;
    u8 alpha, pad;
} JetwEnt;

static JetwEnt adata[] = {
    {0, 0, 30, 140, 0, 255, 0},
    {0, 6, 30, 120, 0, 250, 0},
    {1, 12, 30, 110, -5, 240, 0},
    {1, 17, 50, 100, -20, 240, 0},
    {2, 22, 60, 90, -30, 240, 0},
    {2, 26, 70, 90, -40, 240, 0},
    {3, 28, 80, 85, -50, 240, 0},
    {4, 32, 90, 90, -60, 240, 0},
    {5, 35, 100, 100, -70, 240, 0},
    {7, 38, 110, 110, -80, 240, 0},
    {8, 40, 120, 120, -90, 240, 0},
    {11, 42, 130, 130, -100, 240, 0},
    {14, 43, 150, 150, -110, 240, 0},
    {18, 44, 190, 190, -120, 240, 0},
    {21, 45, 230, 230, -130, 240, 0},
    {24, 46, 270, 270, -140, 240, 0},
    {27, 47, 310, 310, -150, 240, 0},
    {30, 48, 350, 350, -160, 200, 0},
    {33, 49, 380, 380, -170, 140, 0},
    {36, 50, 400, 400, -180, 40, 0},
    {0, 0, 30, 140, 0, 255, 0},
    {0, 6, 30, 120, 0, 240, 0},
    {-1, 12, 30, 110, 5, 230, 0},
    {-1, 17, 50, 100, 20, 220, 0},
    {-2, 22, 60, 90, 30, 210, 0},
    {-2, 26, 70, 90, 40, 200, 0},
    {-3, 28, 80, 85, 50, 190, 0},
    {-4, 32, 90, 80, 60, 180, 0},
    {-5, 35, 100, 77, 70, 170, 0},
    {-7, 38, 110, 65, 80, 160, 0},
    {-8, 40, 120, 54, 90, 150, 0},
    {-11, 42, 130, 60, 100, 140, 0},
    {-14, 43, 140, 70, 110, 130, 0},
    {-18, 44, 150, 80, 120, 120, 0},
    {-21, 45, 160, 90, 130, 110, 0},
    {-24, 46, 170, 100, 140, 90, 0},
    {-27, 47, 180, 110, 150, 80, 0},
    {-30, 48, 190, 120, 160, 70, 0},
    {-33, 49, 200, 130, 170, 30, 0},
    {-36, 50, 210, 140, 180, 10, 0},
    {0, 0, 30, 140, 0, 255, 0},
    {0, 6, 30, 120, 0, 240, 0},
    {0, 12, 30, 110, 5, 230, 0},
    {0, 17, 50, 100, 20, 220, 0},
    {0, 22, 60, 90, 30, 210, 0},
    {0, 26, 70, 90, 40, 200, 0},
    {0, 28, 80, 85, 50, 190, 0},
    {0, 32, 90, 80, 60, 180, 0},
    {0, 35, 100, 77, 70, 170, 0},
    {0, 38, 110, 65, 80, 160, 0},
    {0, 40, 120, 54, 90, 150, 0},
    {0, 42, 130, 60, 100, 140, 0},
    {0, 43, 140, 70, 110, 130, 0},
    {0, 44, 150, 80, 120, 120, 0},
    {0, 45, 160, 90, 130, 110, 0},
    {0, 46, 170, 100, 140, 90, 0},
    {0, 47, 180, 110, 150, 80, 0},
    {0, 48, 190, 120, 160, 70, 0},
    {0, 49, 200, 130, 170, 30, 0},
    {0, 50, 210, 140, 180, 10, 0},
};

static struct {
    JetwEnt e[33];
    s16 pad;
} a2data = {{
    {0, 0, 30, 140, 0, 255, 0},
    {0, 6, 30, 120, 0, 240, 0},
    {1, 12, 30, 110, -5, 230, 0},
    {1, 17, 50, 100, -20, 220, 0},
    {2, 22, 60, 90, -30, 210, 0},
    {2, 26, 70, 90, -40, 200, 0},
    {3, 28, 80, 85, -50, 190, 0},
    {4, 32, 90, 80, -60, 180, 0},
    {5, 35, 100, 77, -70, 120, 0},
    {7, 38, 110, 65, -80, 80, 0},
    {8, 40, 120, 54, -90, 40, 0},
    {0, 0, 30, 140, 0, 255, 0},
    {0, 6, 30, 120, 0, 240, 0},
    {-1, 12, 30, 110, 5, 230, 0},
    {-1, 17, 50, 100, 20, 220, 0},
    {-2, 22, 60, 90, 30, 210, 0},
    {-2, 26, 70, 90, 40, 200, 0},
    {-3, 28, 80, 85, 50, 190, 0},
    {-4, 32, 90, 80, 60, 180, 0},
    {-5, 35, 100, 77, 70, 120, 0},
    {-7, 38, 110, 65, 80, 80, 0},
    {-8, 40, 120, 54, 90, 40, 0},
    {0, 0, 30, 140, 0, 255, 0},
    {0, 6, 30, 120, 0, 240, 0},
    {0, 12, 30, 110, 5, 230, 0},
    {0, 17, 50, 100, 20, 220, 0},
    {0, 22, 60, 90, 30, 210, 0},
    {0, 26, 70, 90, 40, 200, 0},
    {0, 28, 80, 85, 50, 190, 0},
    {0, 32, 90, 80, 60, 180, 0},
    {0, 36, 100, 77, 70, 120, 0},
    {0, 39, 110, 65, 80, 80, 0},
    {0, 42, 120, 54, 90, 40, 0},
}, 0};

static effN64Vtx size16x16_tex32x32_vtx[] = {
    {-8, -8, 0, 0, 0, 255, 255, 255, 255},
    {8, -8, 0, 1024, 0, 255, 255, 255, 255},
    {8, 8, 0, 1024, 1024, 255, 255, 255, 255},
    {-8, 8, 0, 0, 1024, 255, 255, 255, 255},
};

effN64Vtx size6x22_tex32x32_vtx[] = {
    {-3, 0, 0, 0, 0, 255, 255, 255, 255},
    {3, 0, 0, 1024, 0, 255, 255, 255, 255},
    {3, 22, 0, 1024, 1024, 255, 255, 255, 255},
    {-3, 22, 0, 0, 1024, 255, 255, 255, 255},
};

static s32 jetw_ry_tbl[] = {0, 40, 55, 190, 20, 130, 270, 300};

void effOpukuJetwMain(EffectEntry* effect);
void effOpukuJetwDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effOpukuJetwN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffOpukuJetwN64Work* work;

    effect = effEntry();
    effect->type = "OpukuJetwN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffOpukuJetwN64Work));
    effect->userdata = work;
    effect->callback = effOpukuJetwMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
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
    work->colorR = 70;
    work->colorG = 180;
    work->colorB = 255;
    work->color2R = 255;
    work->color2G = 255;
    work->color2B = 255;
    work->rotZ = 0.0f;
    work->scaleX = 1.0f;
    work->scaleY = 1.0f;
    return effect;
}

void effOpukuJetwMain(EffectEntry* effect) {
    EffOpukuJetwN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->lifetime = 16;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    if (work->frame < 8) {
        work->alpha = (work->frame << 5) + 31;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effOpukuJetwDisp, effect);
}

void effOpukuJetwDisp(CameraId cameraId, EffectEntry* effect) {
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx mtx2;
    GXTexObj texObj;
    CameraEntry* camera;
    EffOpukuJetwN64Work* work;
    s32 lifetime, j, i;
    s32 frame, alpha, r, g, b;
    s32 n, m;
    JetwEnt* tbl;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    frame = work->frame;
    lifetime = work->lifetime;
    alpha = work->alpha;
    r = work->colorR;
    g = work->colorG;
    b = work->colorB;
    switch (work->type) {
    case 0:
        tbl = adata;
        n = 20;
        m = 3;
        break;
    default:
        tbl = a2data.e;
        n = 11;
        m = 3;
        break;
    }
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx2);
    PSMTXConcat(camera->view, mtx2, mtx2);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    effGetTexObjN64(0x34, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    GXSetCullMode(GX_CULL_NONE);
    PSMTXRotRad(rot, 'z', 0.017453292f * work->rotZ);
    PSMTXScale(scale, work->scaleX, work->scaleY, 1.0f);
    PSMTXConcat(rot, scale, scale);
    PSMTXConcat(mtx2, scale, mtx2);
    GXSetTevColor(GX_TEVREG1, (GXColor){work->color2R, work->color2G, work->color2B, alpha});
    if (n - 1 <= lifetime) {
        GXSetTevColor(GX_TEVREG0, (GXColor){r, g, b, alpha});
        GXLoadPosMtxImm(mtx2, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        effSetVtxDescN64(size16x16_tex32x32_vtx);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
    for (i = 0; i < n / 2; i++) {
        for (j = 0; j < m; j++) {
            s32 idx = (frame + i * 2) % n;
            JetwEnt* ptr = &tbl[j * n];

            if (idx <= frame * 5) {
                if (n - 1 - idx <= lifetime) {
                    PSMTXTrans(mtx, (f32)ptr[idx].x, 10.0f + (f32)ptr[idx].y, 0.0f);
                    // Scale and alpha come from tbl[idx] (the first group) for every group; only position and rotation use the group's own rows.
                    PSMTXScale(scale, 0.01f * (f32)tbl[idx].sx, 0.01f * (f32)tbl[idx].sy, 1.0f);
                    PSMTXConcat(mtx, scale, mtx);
                    PSMTXRotRad(rot, 'z', 0.017453292f * ((f32)ptr[idx].rot + (f32)jetw_ry_tbl[(j + i) & 7]));
                    PSMTXConcat(mtx, rot, mtx);
                    PSMTXConcat(mtx2, mtx, mtx);
                    GXLoadPosMtxImm(mtx, GX_PNMTX0);
                    GXSetCurrentMtx(GX_PNMTX0);
                    GXSetTevColor(GX_TEVREG0, (GXColor){r, g, b, (alpha * tbl[idx].alpha) >> 8});
                    effSetVtxDescN64(size16x16_tex32x32_vtx);
                    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                    tri2(0, 1, 2, 0, 0, 2, 3, 0);
                }
            }
        }
    }
}

