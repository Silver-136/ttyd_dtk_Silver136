#include "eff/n64/eff_kameki_tornade_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os/OSFastCast.h>
#include <stdlib.h>
#include <string.h>
#include "eff/eff_n64.h"
#include "dolphin/os.h"

double sin(double);
double cos(double);

#define Vec2Vtx(v, o) do { OSf32tos16(&(v)->x, &(o)[0]); OSf32tos16(&(v)->y, &(o)[1]); OSf32tos16(&(v)->z, &(o)[2]); } while (0)



void effKamekiTornadeMain(EffectEntry* effect);
void effKamekiTornadeDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effKamekiTornadeN64Entry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffKamekiTornadeN64Work* work;
    s32 i;
    s32 k;

    effect = effEntry();
    effect->type = "KamekiTornadeN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffKamekiTornadeN64Work));
    effect->userdata = work;
    effect->callback = effKamekiTornadeMain;
    effect->flags |= 2;
    work->type = type;
    work->frame = 0;
    if (life <= 0) {
        work->life = 1000;
    } else {
        work->life = life;
    }
    work->alpha = 0;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->c0r = 255;
    work->c0g = 255;
    work->c0b = 255;
    work->c1r = 255;
    work->c1g = 255;
    work->c1b = 235;
    for (i = 0; i < 8; i++) {
        work->px[i] = x;
        work->py[i] = y;
        work->pz[i] = z;
        work->sc[i] = 0.2f * (f32)i + 1.0f;
        work->ang[i] = (f32)(rand() % 360);
        work->angVel[i] = 0.1f * (f32)(rand() % 100) + 2.0f;
        work->radius[i] = 0.1f * (f32)(rand() % 30);
        work->rot[i] = 0.0f;
    }
    work->twist = 0.0f;
    work->rotZ = 0.0f;
    work->scrollS = 0.0f;
    work->scrollT = 0.0f;
    work->alphaScale = 1.0f;
    return effect;
}

void effKamekiTornadeMain(EffectEntry* effect) {
    EffKamekiTornadeN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 k;
    s32 life;
    s32 frame;
    f32 scale;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->life = 16;
    }
    if (work->life < 1000) {
        work->life--;
    }
    work->frame++;
    life = work->life;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    scale = work->scale;
    if (life < 16) {
        work->alpha = life * 16;
    }
    if (frame < 16) {
        work->alpha = frame * 16 + 15;
    }
    work->px[0] = work->position.x;
    work->py[0] = work->position.y;
    work->pz[0] = work->position.z;
    for (i = 7; i > 0; i--) {
        work->rot[i] = 4.0f * (work->px[i - 1] - work->px[i]);
        work->px[i] = work->px[i - 1];
        work->py[i] = work->py[i - 1] + 5.0f * scale;
        work->pz[i] = work->pz[i - 1];
        if (work->rot[i] > 90.0f) {
            work->rot[i] = 90.0f;
        } else if (work->rot[i] < -90.0f) {
            work->rot[i] = -90.0f;
        }
    }
    work->twist += 10.0f;
    work->scrollS += 12.959999f;
    work->scrollT += 17.28f;
    if (work->scrollS > 64.0f) {
        work->scrollS -= 64.0f;
    }
    if (work->scrollT > 64.0f) {
        work->scrollT -= 64.0f;
    }
    for (i = 0; i < 8; i++) {
        work->ang[i] += work->angVel[i];
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effKamekiTornadeDisp, effect);
}

void effKamekiTornadeDisp(CameraId cameraId, EffectEntry* effect) {
    f32 scale;
    s32 i;
    EffKamekiTornadeN64Work* work;
    s32 life;
    effN64Vtx* tmpl;
    effN64Vtx* vtx;
    effN64Vtx* p;
    effN64Vtx* q;
    s32 k;
    s32 j;
    CameraEntry* camera;
    s32 a;
    f32 s;
    f32* scales;
    f32 r;
    f32 ang;
    Mtx mtx;
    Mtx tex;
    Mtx rot;
    Mtx base;
    GXTexObj texObj;
    Vec axis;
    Vec v1;
    Vec v2;
    f32 cosVal;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    a = work->alpha >> 1;
    life = work->life;
    scale = work->scale;
    GXSetNumChans(0);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_TEXC, GX_CC_CPREV, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_C0, GX_CC_C1, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    effGetTexObjN64(0x6C, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->c0r, work->c0g, work->c0b, (s32)((f32)a * work->alphaScale)});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->c1r, work->c1g, work->c1b, 32});
    GXSetCullMode(GX_CULL_NONE);
    tmpl = (effN64Vtx*)smartAlloc(sizeof(effN64Vtx) * 32, 3)->address;
    p = tmpl;
    for (i = 0, k = 0; i < 16; i++, k += 128) {
        p = &tmpl[i];
        r = (6.2831855f * (f32)(i * 360 / 15)) / 360.0f;
        p->x = 100.0f * (f32)cos(r);
        p->y = 0.0f;
        p->z = 100.0f * (f32)sin(r);
        p->s = k;
        p->t = 0;
        q = &tmpl[i + 16];
        q->x = 100.0f * (f32)cos(r);
        q->y = 0.0f;
        q->z = 100.0f * (f32)sin(r);
        // FAKE: (u32)(i * 128) * 4 == i * 512 for i in 0..15; the unsigned multiply leaves this IV to the back-end loop pass, so its init copies the i*360 IV (target mr r20,r27)
        q->s = (u32)(i * 128) * 4;
        q->t = 1024;
    }
    PSMTXRotRad(rot, 'z', 0.017453292f * work->rotZ);
    PSMTXScale(base, 0.1f, 0.1f, 0.1f);
    PSMTXConcat(rot, base, base);
    for (i = 0; i < 7; i++) {
        vtx = (effN64Vtx*)smartAlloc(sizeof(effN64Vtx) * 32, 3)->address;
        effSetVtxDescN64(vtx);
        PSMTXTrans(mtx, 10.0f * work->px[i], 10.0f * work->py[i], 10.0f * work->pz[i]);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rot[i]);
        PSMTXConcat(mtx, rot, mtx);
        axis = (Vec){-0.03f, 1.0f, 0.1f};
        PSMTXRotAxisRad(rot, &axis, 0.017453292f * (work->twist + (f32)(i * i)));
        PSMTXConcat(mtx, rot, mtx);
        s = work->sc[i] * scale;
        if (life < 16) {
            s += 0.02f * (f32)(127 - a);
        }
        PSMTXScale(tex, s, scale, s);
        PSMTXConcat(mtx, tex, mtx);
        ang = 10.0f * work->radius[i];
        s = 6.2831855f * work->ang[i] / 360.0f;
        r = (f32)sin(s);
        // FAKE: cos(s) through a temp (same call order: sin, then cos, then PSMTXTrans)
        cosVal = (f32)cos(s);
        PSMTXTrans(tex, ang * r, 0, ang * cosVal);
        PSMTXConcat(mtx, tex, mtx);
        PSMTXConcat(base, mtx, mtx);
        for (j = 0; j < 16; j++) {
            memcpy(&vtx[j], &tmpl[j], sizeof(effN64Vtx));
            OSInitFastCast();
            OSs16tof32(&vtx[j].x, &v1.x);
            OSs16tof32(&vtx[j].y, &v1.y);
            OSs16tof32(&vtx[j].z, &v1.z);
            PSMTXMultVec(mtx, &v1, &v1);
            OSInitFastCast();
            Vec2Vtx(&v1, &vtx[j].x);
        }
        PSMTXTrans(mtx, 10.0f * work->px[i + 1], 10.0f * work->py[i + 1], 10.0f * work->pz[i + 1]);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rot[i + 1]);
        // FAKE: alias of the work->sc array (address only, no load) used for the second scale argument
        scales = work->sc;
        PSMTXConcat(mtx, rot, mtx);
        axis = (Vec){0.03f, 1.0f, 0.0f};
        PSMTXRotAxisRad(rot, &axis, 0.017453292f * (work->twist + (f32)(i * i)));
        PSMTXConcat(mtx, rot, mtx);
        PSMTXScale(tex, work->sc[i + 1] * scale, scale, scales[i + 1] * scale);
        PSMTXConcat(mtx, tex, mtx);
        r = 10.0f * work->radius[i];
        s = (f32)sin(6.2831855f * work->ang[i] / 360.0f);
        PSMTXTrans(tex, r * s, 0.0f, r * (f32)cos(6.2831855f * work->ang[i] / 360.0f));
        PSMTXConcat(mtx, tex, mtx);
        PSMTXConcat(base, mtx, mtx);
        for (j = 16; j < 32; j++) {
            memcpy(&vtx[j], &tmpl[j], sizeof(effN64Vtx));
            OSInitFastCast();
            OSs16tof32(&vtx[j].x, &v2.x);
            OSs16tof32(&vtx[j].y, &v2.y);
            OSs16tof32(&vtx[j].z, &v2.z);
            PSMTXMultVec(mtx, &v2, &v2);
            OSInitFastCast();
            Vec2Vtx(&v2, &vtx[j].x);
        }
        GXInvalidateVtxCache();
        DCFlushRange(vtx, sizeof(effN64Vtx) * 32);
        PSMTXScale(tex, 0.015625f, 0.03125f, 0.0f);
        PSMTXTrans(mtx, work->scrollS + (f32)(i * 4), 0.0f, 0.0f);
        PSMTXConcat(tex, mtx, tex);
        GXLoadTexMtxImm(tex, GX_TEXMTX0, GX_MTX2x4);
        PSMTXScale(tex, 0.015625f, 0.03125f, 0.0f);
        PSMTXTrans(mtx, work->scrollT + (f32)(i * 2), 0.0f, 0.0f);
        PSMTXConcat(tex, mtx, tex);
        GXLoadTexMtxImm(tex, GX_TEXMTX1, GX_MTX2x4);
        GXLoadPosMtxImm(camera->view, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        for (j = 0; j < 15; j++) {
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(j + 16, j + 1, j, 0, j + 1, j + 16, j + 17, 1);
        }
    }
}


