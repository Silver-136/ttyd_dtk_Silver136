#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"


typedef struct EffFireworksN64Work {
    s32 type;        // 0x0
    Vec position;    // 0x4
    Vec vel;         // 0x10
    s32 lifetime;    // 0x1c
    s32 frame;       // 0x20
    s32 r;           // 0x24
    s32 g;           // 0x28
    s32 b;           // 0x2c
    s32 alpha;       // 0x30
    s32 unk34;       // 0x34
    s32 unk38;       // 0x38
    s32 unk3C;       // 0x3c
    s32 unk40;       // 0x40
    f32 scale;       // 0x44
    f32 scaleTarget; // 0x48
    s32 state;       // 0x4c
    f32 px[4];       // 0x50
    f32 py[4];       // 0x60
    f32 pz[4];       // 0x70
    f32 vx[4];       // 0x80
    f32 vy[4];       // 0x90
    f32 vz[4];       // 0xa0
} EffFireworksN64Work;

static s8 vtx[285] = {
    36, 0, 93, 54, -40, 74, 67, 0, 74, 82, -36, 45, 89, 0, 45, 60, -66, 45, 21, -64, 74, 28, -85, 45,
    10, -98, 15, 49, -86, 15, -90, -40, 15, -87, -19, 45, -99, 0, 15, -72, -53, 45, -87, 19, 45, -90, 40, 15,
    -66, 73, 15, -72, 53, 45, -45, 77, 45, -31, 94, 15, -9, 89, 45, -72, 53, 45, 11, -34, 93, 0, 0, 100,
    -54, -40, 74, -72, -53, 45, -45, -77, 45, -90, -40, 15, -66, -73, 15, 11, -34, 93, -21, -64, 74, 21, -64, 74,
    -9, -89, 45, 28, -85, 45, 10, 98, 15, 80, -58, 15, 97, -21, 15, 49, -86, 15, 10, -98, 15, -31, -94, 15,
    -66, -73, 15, 49, 86, 15, 80, 58, 15, 97, 21, 15, -29, -21, 93, 0, 0, 100, 11, 34, 93, -29, 21, 93,
    21, 64, 74, -21, 64, 74, -54, 40, 74, 28, 85, 45, 67, 0, 74, 89, 0, 45, 36, 0, 93, 0, 0, 100,
    11, 34, 93, 54, 40, 74, 21, 64, 74, -54, 40, 74, -72, 53, 45, 0, 0, 100, -29, -21, 93, -67, 0, 74,
    -54, -40, 74, -29, 21, 93, 0, 0, 100, 60, -66, 45, 82, -36, 45, 89, 0, 45, 28, -85, 45, 28, -85, 45,
    -9, -89, 45, -45, -77, 45, -72, -53, 45, 60, 66, 45, 28, 85, 45, 82, 36, 45, 28, 85, 45, 89, 0, 45,
    -9, 89, 45, -45, 77, 45, -72, 53, 45, 49, 86, 15, 10, 98, 15, 82, 36, 45, 97, -21, 15, 97, 21, 15,
    60, 66, 45, 28, 85, 45, -87, 19, 45, -66, 73, 15, -90, 40, 15, -87, -19, 45, -72, -53, 45};

static s8 vtx2[198] = {
    -48, -15, 0, -36, -26, 22, -45, 0, 22, -48, 15, 0, -36, 26, 22, -29, 40, 0, -36, 26, 22, -14, 43, 22,
    0, 50, 0, 14, 43, 22, 8, -25, 43, 14, -43, 22, 36, -26, 22, 26, 0, 43, 0, 0, 50, 0, -50, 0,
    29, -40, 0, 45, 0, 22, 29, -40, 0, 36, -26, 22, 14, -43, 22, 48, -15, 0, 45, 0, 22, 0, -50, 0,
    14, -43, 22, -14, -43, 22, -29, -40, 0, -36, -26, 22, 29, 40, 0, 36, 26, 22, 48, 15, 0, 14, 43, 22,
    45, 0, 22, 0, 0, 50, 8, 25, 43, -21, 15, 43, 14, 43, 22, 26, 0, 43, 45, 0, 22, 0, 0, 50,
    8, 25, 43, -21, -15, 43, 0, 0, 50, -36, -26, 22, 8, -25, 43, 0, 0, 50, -21, 15, 43, -21, -15, 43,
    -36, 26, 22, -14, 43, 22, -36, 26, 22, 29, 40, 0, 0, 50, 0, 36, 26, 22, 48, -15, 0, 48, 15, 0,
    14, 43, 22, -14, -43, 22, -48, -15, 0, -29, -40, 0, 14, -43, 22, -45, 0, 22, -36, -26, 22, -29, 40, 0,
    -48, 15, 0, 0, 0, 0};

static s8 vtx3[168] = {
    -9, 105, 0, -17, 87, 0, 17, 87, 0, 9, 105, 0, 0, 122, 0, -26, 69, 0, 26, 69, 0, -35, 52, 0,
    35, 52, 0, -54, 49, 0, -70, -1, 0, -56, -15, 0, 0, -56, 0, 56, -15, 0, 70, -1, 0, 54, 49, 0,
    -60, -34, 0, -17, -65, 0, 17, -65, 0, 60, -34, 0, 85, 13, 0, 74, 46, 0, 99, 27, 0, 93, 43, 0,
    12, 33, 0, 12, 22, 0, 24, 22, 0, 24, 33, 0, 12, 10, 0, 24, 10, 0, 24, 45, 0, -24, 33, 0,
    -24, 22, 0, -24, 45, 0, 113, 40, 0, -63, -53, 0, -66, -73, 0, -74, 46, 0, -93, 43, 0, 35, -74, 0,
    52, -83, 0, 12, 45, 0, -12, 22, 0, -12, 33, 0, -24, 10, 0, -12, 10, 0, -12, 45, 0, -52, -83, 0,
    -35, -74, 0, -70, -92, 0, -85, 13, 0, -99, 27, 0, -113, 40, 0, 63, -53, 0, 66, -73, 0, 70, -92, 0};

struct {
    s8* v;
    s32 n;
} vtx_data[3] = {{vtx, 95}, {vtx2, 66}, {vtx3, 56}};


void effFireworksMain(EffectEntry* effect);
void effFireworksDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effFireworksN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, f32 w) {
    EffectEntry* effect;
    EffFireworksN64Work* work;

    effect = effEntry();
    effect->type = "FireworksN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffFireworksN64Work));
    effect->userdata = work;
    effect->callback = effFireworksMain;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime + 32;
    }
    work->state = 0;
    work->alpha = 255;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 0.0f;
    work->vel.x = vx;
    work->vel.y = vy;
    work->vel.z = vz;
    work->scaleTarget = w;
    work->r = 255;
    work->g = 255;
    work->b = 100;
    work->unk34 = 255;
    work->unk38 = 100;
    work->unk3C = 150;
    work->unk40 = 255;
    work->px[0] = work->position.x;
    work->py[0] = work->position.y - 1000.0f;
    work->pz[0] = work->position.z;
    work->vx[0] = 0.0f;
    work->vy[0] = 0.0f;
    work->vz[0] = 0.0f;
    work->px[1] = work->position.x;
    work->py[1] = work->position.y - 1000.0f;
    work->pz[1] = work->position.z;
    work->vx[1] = 0.0f;
    work->vy[1] = 0.0f;
    work->vz[1] = 0.0f;
    work->px[2] = work->position.x;
    work->py[2] = work->position.y - 1000.0f;
    work->pz[2] = work->position.z;
    work->vx[2] = 0.0f;
    work->vy[2] = 0.0f;
    work->vz[2] = 0.0f;
    work->px[3] = work->position.x;
    work->py[3] = work->position.y - 1000.0f;
    work->pz[3] = work->position.z;
    work->vx[3] = 0.0f;
    work->vy[3] = 0.0f;
    work->vz[3] = 0.0f;
    return effect;
}

void effFireworksMain(EffectEntry* effect) {
    EffFireworksN64Work* work = effect->userdata;
    Vec position2;
    Vec position = {0.0f, 0.0f, 0.0f};
    s32 frame;
    s32 i;
    s32 idx;
    f32 s;



    position.x = work->position.x;
    position.y = work->position.y;
    position.z = work->position.z;
    position2 = position;
    if (effect->flags & 4) {
        effect->flags &= ~4;
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
    frame = work->frame;
    if (work->lifetime < 32) {
        work->alpha = work->lifetime * 8;
    }
    if (work->state == 1) {
        work->position.x += work->vel.x;
        work->position.y += work->vel.y;
        work->position.z += work->vel.z;
        work->vel.x *= 0.95f;
        work->vel.y *= 0.95f;
        work->vel.z *= 0.95f;
        work->vel.y -= 0.15f;
        work->scale += 0.11f * (work->scaleTarget - work->scale);
    } else {
        idx = frame & 3;
        work->px[idx] = work->position.x - work->vel.x * (f32)(32 - frame);
        s = (f32)sin(6.2831855f * (f32)(frame * 90 >> 5) / 360.0f);
        work->py[idx] = work->position.y - work->vel.y * (f32)(32 - frame) - (80.0f - 80.0f * s);
        work->pz[idx] = work->position.z - work->vel.z * (f32)(32 - frame);
        work->vx[idx] = 0.1f * (f32)(rand() % 11 - 5);
        work->vy[idx] = 0.1f * (f32)(rand() % 11 - 5);
        work->vz[idx] = 0.1f * (f32)(rand() % 11 - 5);
        for (i = 0; i < 4; i++) {
            work->px[i] += work->vx[i];
            work->py[i] += work->vy[i];
            work->pz[i] += work->vz[i];
            work->vy[i] -= 0.15f;
            if (frame >= 27) {
                work->py[i] = -1000.0f;
            }
        }
        if (frame >= 32) {
            work->state = 1;
            work->frame = 1;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position2), 2, (DispCallback)effFireworksDisp, effect);
}

void effFireworksDisp(CameraId cameraId, EffectEntry* effect) {
    static s32 salpha;
    static s32 sflag;
    CameraEntry* camera;
    EffFireworksN64Work* work;
    GXTexObj texObj;
    f32 s;
    f32 c;
    s32 i;
    s32 type;
    s8* v;
    s32 count;
    s32 alpha;
    s32 state;
    Mtx44 ortho;
    Mtx view;
    Vec t;
    Vec target;
    Vec pos;
    Vec up;
    f32 proj[7];
    f32 ang;
    f32 sx;
    f32 sy;
    f32 x;
    f32 y;
    f32 z;
    f32 ox;
    f32 oy;
    f32 oz;
    f32 sc;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->alpha;
    type = work->type;
    ang = 6.2831855f * -camGetPtr(CAMERA_3D)->viewYaw / 360.0f;
    s = (f32)sin(ang);
    c = (f32)cos(ang);
    state = work->state;
    if (sflag != gp->retraceCount) {
        sflag = gp->retraceCount;
        salpha = 0;
    }
    if (salpha < alpha) {
        salpha = alpha;
    }
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    effGetTexObjN64(8, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 0);
    ox = work->position.x;
    oy = work->position.y;
    oz = work->position.z;
    sc = work->scale;
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->r, work->g, work->b, 0});
    if (state == 1) {
        v = vtx_data[type].v;
        count = vtx_data[type].n;
    } else {
        v = 0;
        count = 4;
    }
    for (i = 0; i < count; i++, v += 3) {
        if (state == 1) {
            if (rand() % 17 < 6) {
                continue;
            }
            x = ox + sc * (v[0] * c + v[2] * s);
            y = oy + v[1] * sc;
            z = oz + sc * (-v[0] * s + v[2] * c);
        } else {
            x = work->px[i];
            y = work->py[i];
            z = work->pz[i];
        }
        {
            Vec pt = {0.0f, 0.0f, 0.0f};
            pt.x = x;
            pt.y = y;
            pt.z = z;
            t = pt;
            PSMTXMultVec(camera->view, &t, &t);
            PSMTX44MultVec(camera->projection, &t, &t);
            sx = t.x * gp->fbWidth / 2.0f;
            sy = t.y * gp->efbHeight / 2.0f;
            {
            target = (Vec){0.0f, 0.0f, 0.0f};
            pos = (Vec){0.0f, 0.0f, 1000.0f};
            up = (Vec){0.0f, 1.0f, 0.0f};
            GXGetProjectionv(proj);
            MTXOrtho(ortho, 480.0f, 0.0f, 0.0f, 608.0f, 1.0f, 32768.0f);
            GXSetProjection(ortho, GX_ORTHOGRAPHIC);
            MTXLookAt(view, &pos, &up, &target);
            GXLoadPosMtxImm(view, GX_PNMTX0);
            GXSetCurrentMtx(0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(sx, sy, 0.0f);
            GXTexCoord2s16(0, 1);
            GXPosition3f32(sx + 8.0f, sy, 0.0f);
            GXTexCoord2s16(1, 1);
            GXPosition3f32(sx + 8.0f, sy - 8.0f, 0.0f);
            GXTexCoord2s16(1, 0);
            GXPosition3f32(sx, sy - 8.0f, 0.0f);
            GXTexCoord2s16(0, 0);
            GXSetProjectionv(proj);
            }
        }
    }
}








