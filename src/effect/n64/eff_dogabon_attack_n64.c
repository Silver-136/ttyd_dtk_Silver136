#include "eff/eff_n64.h"
#include "eff/n64/eff_stone_smoke_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

double sin(double);
double cos(double);
int rand(void);

#define PIECES 25

typedef struct EffDogabonAttackN64Work {
    s32 type;           // 0x0
    Vec position;       // 0x4
    s32 lifetime;       // 0x10
    s32 frame;          // 0x14
    s32 r;              // 0x18
    s32 g;              // 0x1C
    s32 b;              // 0x20
    s32 alpha;          // 0x24
    f32 scale;          // 0x28
    f32 x[PIECES];      // 0x2C
    f32 y[PIECES];      // 0x90
    f32 z[PIECES];      // 0xF4
    f32 tx[PIECES];     // 0x158
    f32 ty[PIECES];     // 0x1BC
    f32 tz[PIECES];     // 0x220
    f32 sx0[PIECES];    // 0x284
    f32 sy0[PIECES];    // 0x2E8
    f32 sz0[PIECES];    // 0x34C
    f32 scaleX[PIECES]; // 0x3B0
    f32 scaleY[PIECES]; // 0x414
    f32 rotZ[PIECES];   // 0x478
    f32 rotY[PIECES];   // 0x4DC
    f32 vx[PIECES];     // 0x540
    f32 vy[PIECES];     // 0x5A4
    f32 vz[PIECES];     // 0x608
    s32 delay[PIECES];  // 0x66C
    s32 state[PIECES];  // 0x6D0
    s32 timer[PIECES];  // 0x734
} EffDogabonAttackN64Work;

void effDogabonAttackMain(EffectEntry* effect);
void effDogabonAttackDisp(CameraId cameraId, EffectEntry* effect);

u8 move_data_2[70] = {
    100, 100, 100, 100, 100, 100, 100, 100, 101, 102, 102, 103, 103, 104, 104, 104, 107, 107,
    110, 110, 118, 118, 126, 126, 134, 142, 145, 145, 145, 142, 134, 126, 126, 118, 118, 110,
    110, 105, 105, 105, 102, 103, 103, 104, 104, 104, 107, 107, 110, 110, 118, 118, 126, 126,
    134, 142, 145, 145, 145, 142, 134, 126, 126, 118, 118, 110, 110, 105, 105, 105};

effN64Vtx size12x12_tex32x32_vtx[] = {
    {-6, -6, 0, 0, 0, 0, 0, 0, 255},
    {6, -6, 0, 1024, 0, 0, 0, 0, 255},
    {6, 6, 0, 1024, 1024, 0, 0, 0, 255},
    {-6, 6, 0, 0, 1024, 0, 0, 0, 255},
};

static u8 scale_data[7] = {94, 92, 100, 105, 107, 105, 100};
u8 scale_data_2[8] = {20, 40, 58, 73, 85, 92, 97, 100};
u8 move_data_1[15] = {0, 2, 5, 9, 14, 20, 27, 35, 44, 54, 64, 74, 84, 94, 100};
u8 move_data_hazure[20] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 109, 117, 124, 130, 135, 139, 142, 144, 145};
static s8 y_data[] = {-5, -5, -5, -4, -4, -4, -3, -3, -2, -2, -1, -1, 0, 0, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5};

EffectEntry* effDogabonAttackN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffDogabonAttackN64Work* work;
    s32 i;
    f32 sn;
    f32 cs;
    f32 ang;

    effect = effEntry();
    effect->type = "DogabonAttackN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffDogabonAttackN64Work));
    effect->userdata = work;
    effect->callback = effDogabonAttackMain;
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
    work->r = 70;
    work->g = 180;
    work->b = 120;
    for (i = 0; i < PIECES; i++) {
        ang = 6.2831855f * (f32)(i * 1080 / 25) / 360.0f;
        sn = sin(ang);
        cs = cos(ang);
        work->rotZ[i] = rand() % 360 - 180;
        work->sx0[i] = 15.0f * sn;
        work->sy0[i] = y_data[i] + 10;
        work->sz0[i] = 15.0f * cs;
        work->tx[i] = 120.0f * sn;
        work->ty[i] = y_data[i] * 8 + 40;
        work->tz[i] = 120.0f * cs;
        work->vy[i] = 0.0f;
        work->delay[i] = i * 2 + 1;
        work->state[i] = 0;
        work->timer[i] = 0;
        work->rotY[i] = 0.0f;
    }
    return effect;
}

void effDogabonAttackMain(EffectEntry* effect) {
    EffDogabonAttackN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;
    s32 fm;
    f32 cn;
    f32 s;
    f32 t;
    f32 sn;
    f32 ang;
    f32 c1;
    f32 f28;
    Mtx trans;
    Mtx rot;
    EffectEntry* smoke;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    type = work->type;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->lifetime = 64;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    fm = work->frame % 7;
    for (i = 0; i < PIECES; i++) {
        f32 tx, ty, tz;
        s = 0.01f * scale_data[fm];
        work->scaleY[i] = s;
        work->scaleX[i] = 2.0f - s;
        tx = work->tx[i];
        ty = work->ty[i];
        tz = work->tz[i];
        switch (work->state[i]) {
        case 0:
            work->x[i] = tx;
            work->y[i] = ty;
            work->z[i] = tz;
            if (work->delay[i] != 0) {
                work->delay[i]--;
            } else {
                work->state[i] = 1;
                s = 0.01f * scale_data_2[work->timer[i]];
                work->scaleY[i] = s;
                work->scaleX[i] = s;
                work->timer[i]++;
            }
            break;
        case 1:
            s = 0.01f * scale_data_2[work->timer[i]];
            work->scaleY[i] = s;
            work->scaleX[i] = s;
            work->timer[i]++;
            if (work->timer[i] >= 8) {
                if (type == 0) {
                    work->state[i] = 2;
                } else {
                    work->state[i] = 20;
                }
                work->timer[i] = 0;
            }
            break;
        case 20:
            t = 0.01f * move_data_hazure[work->timer[i]];
            work->x[i] = t * (work->sx0[i] - tx) + tx;
            work->y[i] = t * (work->sy0[i] - ty) + ty;
            work->z[i] = t * (work->sz0[i] - tz) + tz;
            work->scaleY[i] = 1.0f;
            work->scaleX[i] = 1.0f;
            work->timer[i]++;
            if (work->timer[i] >= 20) {
                work->state[i] = 5;
                work->timer[i] = 0;
                work->vx[i] = 0.5f * work->sx0[i];
                work->vy[i] = 0.5f * work->sy0[i];
                work->vz[i] = 0.5f * work->sz0[i];
            }
            break;
        case 2:
            t = 0.01f * move_data_1[work->timer[i]];
            work->x[i] = t * (work->sx0[i] - tx) + tx;
            work->y[i] = t * (work->sy0[i] - ty) + ty;
            work->z[i] = t * (work->sz0[i] - tz) + tz;
            work->scaleY[i] = 1.0f;
            work->scaleX[i] = 1.0f;
            work->timer[i]++;
            if (work->timer[i] >= 15) {
                work->state[i] = 3;
                work->timer[i] = 0;
            }
            break;
        case 3:
            if (i == 24) {
                work->state[0] = 4;
                work->state[1] = 4;
                work->state[2] = 4;
                work->state[3] = 4;
                work->state[4] = 4;
                work->state[5] = 4;
                work->state[6] = 4;
                work->state[7] = 4;
                work->state[8] = 4;
                work->state[9] = 4;
                work->state[10] = 4;
                work->state[11] = 4;
                work->state[12] = 4;
                work->state[13] = 4;
                work->state[14] = 4;
                work->state[15] = 4;
                work->state[16] = 4;
                work->state[17] = 4;
                work->state[18] = 4;
                work->state[19] = 4;
                work->state[20] = 4;
                work->state[21] = 4;
                work->state[22] = 4;
                work->state[23] = 4;
                work->state[24] = 4;
            }
            break;
        case 4:
            f28 = 0.01f * move_data_2[work->timer[i]];
            c1 = cos(6.2831855f * (f32)(i * 180 / 25) / 360.0f);
            ang = 6.2831855f * ((f32)(i * 1080 / 25) + 0.5f * (1080.0f * (1.0f - f28)) * c1) / 360.0f;
            sn = sin(ang);
            cn = cos(ang);
            t = 15.0f / f28 / f28;
            work->x[i] = sn * t;
            work->y[i] = y_data[i] + 10;
            work->z[i] = cn * t;
            work->timer[i]++;
            if (work->timer[i] >= 70) {
                work->state[i] = 5;
                work->timer[i] = 0;
                work->vx[i] = 0.5f * work->sx0[i];
                work->vy[i] = 0.5f * work->sy0[i];
                work->vz[i] = 0.5f * work->sz0[i];
            }
            break;
        case 5:
            work->x[i] += work->vx[i];
            work->y[i] += work->vy[i];
            work->z[i] += work->vz[i];
            work->vx[i] *= 0.99f;
            work->vy[i] *= 0.99f;
            work->vz[i] *= 0.99f;
            work->vy[i] += -0.1f;
            work->timer[i]++;
            if (work->timer[i] > 16) {
                work->timer[i] = 0;
                work->state[i] = 6;
            }
            work->rotZ[i] *= 0.9f;
            break;
        case 6:
            PSMTXRotRad(rot, 'y', 0.017453292f * work->rotY[i]);
            PSMTXTrans(trans, work->x[i], work->y[i], work->z[i]);
            PSMTXConcat(rot, trans, trans);
            smoke = effStoneSmokeN64Entry(0, 16, work->position.x + trans[0][3], work->position.y + trans[1][3],
                                          work->position.z + trans[2][3], 1.0f);
            ((EffStoneSmokeN64Work*)smoke->userdata)->r2 = 100;
            ((EffStoneSmokeN64Work*)smoke->userdata)->g2 = 0;
            ((EffStoneSmokeN64Work*)smoke->userdata)->b2 = 0;
            work->state[i] = 7;
            break;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 1, (DispCallback)effDogabonAttackDisp, effect);
}

void effDogabonAttackDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffDogabonAttackN64Work* work;
    s32 i;
    s32 alpha;
    GXTexObj texObj;
    Mtx trans;
    Mtx scale;
    Mtx rot;
    Mtx mtx;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->alpha;
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(trans, scale, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){0, 0, 0, 0});
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_C0, GX_CC_C1, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(140, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size12x12_tex32x32_vtx);
    for (i = 0; i < PIECES; i++) {
        if (work->state[i] != 0 && work->state[i] != 7) {
            PSMTXRotRad(rot, 'y', 0.017453292f * work->rotY[i]);
            PSMTXTrans(trans, work->x[i], work->y[i], work->z[i]);
            PSMTXConcat(rot, trans, trans);
            PSMTXRotRad(rot, 'y', 0.017453292f * -work->rotY[i]);
            PSMTXConcat(trans, trans, trans);
            PSMTXRotRad(rot, 'z', 0.017453292f * work->rotZ[i]);
            PSMTXConcat(trans, rot, trans);
            PSMTXScale(scale, work->scaleX[i], work->scaleY[i], 1.0f);
            PSMTXConcat(trans, scale, trans);
            PSMTXConcat(mtx, trans, trans);
            GXLoadPosMtxImm(trans, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
        }
    }
}
