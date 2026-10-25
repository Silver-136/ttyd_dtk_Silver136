#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_kemu_test.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "poly_trig.h"
#include "eff/eff_mobj_broken.h"


#define PI 3.1415927f
#define HALF_PI 1.5707964f

typedef struct EffMObjBrokenWork {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 scale;    // 0x10
    s32 alpha;    // 0x14
    f32 rotation; // 0x18
    f32 angle;    // 0x1C
    f32 speed;    // 0x20
    f32 velY;     // 0x24
    f32 gravity;  // 0x28
    s32 poseId;   // 0x2C
} EffMObjBrokenWork;

static void effMObjBrokenMain(EffectEntry* effect);
static void effMObjBrokenDisp(CameraId cameraId, EffectEntry* effect);
static void effMObjBrokenDisp2(CameraId cameraId, EffectEntry* effect);
static void rendermodeFunc(s32 arg);



EffectEntry* effMObjBrokenEntry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffMObjBrokenWork* work;
    s32 count;
    s32 i;
    s32 n;
    s32 col;
    s32 row;
    s32 depth;
    f32 s;

    effect = effEntry();
    count = 1;
    switch (type) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        count = 17;
        break;
    case 6:
        count = 17;
        break;
    case 7:
        count = 9;
        break;
    }
    effect->type = "MObjBroken";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, effect->count * sizeof(EffMObjBrokenWork));
    effect->userdata = work;
    effect->callback = effMObjBrokenMain;
    effect->flags |= 2;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 1.0f;
    work->poseId = -1;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        n = i;
        if (n > 9) {
            n -= 8;
        }
        switch (type) {
        case 0:
        case 1:
        case 2:
            s = 6.25f;
            col = (n - 1) % 4 / 2;
            row = (n - 1) / 4;
            depth = (n - 1) % 4 % 2;
            break;
        case 3:
        case 4:
        case 5:
            s = 12.5f;
            col = (n - 1) % 4 / 2;
            row = (n - 1) / 4;
            depth = (n - 1) % 4 % 2;
            break;
        case 6:
            s = 12.5f;
            col = (n - 1) % 2;
            row = (n - 1) / 2;
            depth = 0;
            break;
        case 7:
            s = 12.5f;
            col = (n - 1) % 2;
            row = (n - 1) / 2;
            depth = 0;
            break;
        }
        work->position.x = col * s - s / 2.0f;
        work->position.y = row * s + s / 2.0f;
        work->position.z = depth * s - s / 2.0f;
        work->scale = s;
        work->alpha = 255;
        work->rotation = 0.0f;
        work->poseId = -1;
        work->speed = (f32)(irand(10) + 10) / 16.0f;
        work->angle = angleABf(0.0f, 0.0f, work->position.x, work->position.z);
        work->velY = irand(3) + 3;
        work->gravity = (irand(5) + 2) / 5.0f;
    }
    if (type != 7) {
        effKemuTestEntry(2, x, y + (work - 1)->position.y, z, type < 3 ? 0.5f : 1.0f);
    } else {
        effKemuTestEntry(16, x, y + work->position.y, z, 1.0f);
    }
    return effect;
}

#pragma opt_propagation off
#pragma opt_lifetimes off
static void effMObjBrokenMain(EffectEntry* effect) {
    EffMObjBrokenWork* work = effect->userdata;
    s32 type;
    EffMObjBrokenWork* w2;
    s32 done = 0;
    s32 i;
    Vec pos2;
    Vec pos = {0.0f, 0.0f, 0.0f};
    EffMObjBrokenWork* w;
    s32 inBattle;

    pos.x = work->position.x;
    pos.y = work->position.y;
    pos.z = work->position.z;
    pos2 = pos;
    inBattle = gp->inBattle != 0;
    type = work->type;
    if (effect->flags & 4) {
        w = work;
        for (done = 0; done < effect->count; done++, w++) {
            if (w->poseId != -1) {
                animPoseRelease(w->poseId);
            }
        }
        effDelete(effect);
        return;
    }
    switch (type) {
    case 7:
        if (!animGroupBaseAsync("EFF_sango", inBattle, NULL)) {
            return;
        }
        break;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        switch (type) {
        case 7:
            if (work->poseId == -1) {
                work->poseId = animPoseEntry("EFF_sango", inBattle);
                animPoseSetAnim(work->poseId, (i & 1) ? "Z_2" : "Z_1", TRUE);
            }
            break;
        }
        work->position.x += work->speed * polySinf(6.2831855f * work->angle / 360.0f);
        work->position.z += work->speed * -polyCosf(6.2831855f * work->angle / 360.0f);
        work->position.y += work->velY;
        work->velY -= work->gravity;
        if (type < 6) {
            work->rotation += (20.0f - work->rotation) / 8.0f;
        } else {
            work->rotation += (720.0f - work->rotation) / 32.0f;
        }
        if (work->position.y < -100.0f) {
            done++;
        }
    }
    if (done >= effect->count - 1) {
        w2 = effect->userdata;
        for (type = 0; type < effect->count; type++, w2++) {
            if (w2->poseId != -1) {
                animPoseRelease(w2->poseId);
            }
        }
        effDelete(effect);
        return;
    }
    if (type != 7) {
        dispEntry(CAMERA_3D, dispCalcZ(&pos2), 8, (DispCallback)effMObjBrokenDisp, effect);
    } else {
        dispEntry(CAMERA_3D, dispCalcZ(&pos2), 8, (DispCallback)effMObjBrokenDisp2, effect);
    }
}

#pragma opt_propagation reset
#pragma opt_lifetimes reset

static void effMObjBrokenDisp(CameraId cameraId, EffectEntry* effect) {
    EffMObjBrokenWork* work = effect->userdata;
    CameraEntry* camera;
    Mtx mT, mR, mS, mN;
    GXTexObj texObj;
    GXColor color;
    s32 type;
    s32 i;
    EffMObjBrokenWork* w;
    Vec axis2a, axis2b, axisa, axisb;

    camera = camGetPtr(cameraId);
    type = work->type;
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    PSMTXTrans(mT, work->position.x, work->position.y, work->position.z);
    PSMTXScale(mS, work->scale, work->scale, work->scale);
    PSMTXConcat(mT, mS, mT);
    PSMTXConcat(camera->view, mT, mN);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    if (type < 6) {
        effGetTexObj(type % 3 + 56, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
    } else {
        effGetTexObj(59, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
    }
    color = (GXColor){0, 0, 0, 128};
    GXSetChanMatColor(GX_COLOR0A0, color);
    w = effect->userdata;
    w++;
    for (i = 1; i < effect->count; i++, w++) {
        axisa = (Vec){0.0f, 0.0f, 0.0f};
        axisa.x = (f32)cos(6.2831855f * w->angle / 360.0f);
        axisa.z = (f32)sin(6.2831855f * w->angle / 360.0f);
        axis2a = axisa;
        PSMTXTrans(mT, 5.0f + w->position.x, w->position.y - 5.0f, w->position.z);
        PSMTXScale(mS, w->scale, w->scale, w->scale);
        PSMTXConcat(mT, mS, mT);
        PSMTXRotAxisRad(mR, &axis2a, 0.017453292f * -w->rotation);
        PSMTXConcat(mT, mR, mT);
        PSMTXScale(mS, 1.0f, 1.0f, 0.0f);
        PSMTXConcat(mT, mS, mT);
        PSMTXConcat(mN, mT, mT);
        GXLoadPosMtxImm(mT, 0);
        GXSetCurrentMtx(0);
        GXDrawCube();
    }
    color = (GXColor){255, 255, 255, 255};
    GXSetChanMatColor(GX_COLOR0A0, color);
    w = effect->userdata;
    w++;
    for (i = 1; i < effect->count; i++, w++) {
        axisb = (Vec){0.0f, 0.0f, 0.0f};
        axisb.x = (f32)cos(6.2831855f * w->angle / 360.0f);
        axisb.z = (f32)sin(6.2831855f * w->angle / 360.0f);
        axis2b = axisb;
        PSMTXTrans(mT, w->position.x, w->position.y, w->position.z);
        PSMTXScale(mS, w->scale, w->scale, w->scale);
        PSMTXConcat(mT, mS, mT);
        PSMTXRotAxisRad(mR, &axis2b, 0.017453292f * -w->rotation);
        PSMTXConcat(mT, mR, mT);
        PSMTXScale(mS, 1.0f, 1.0f, 0.0f);
        PSMTXConcat(mT, mS, mT);
        PSMTXConcat(mN, mT, mT);
        GXLoadPosMtxImm(mT, 0);
        GXSetCurrentMtx(0);
        GXDrawCube();
    }
}

static void rendermodeFunc(s32 arg) {
    animGetPtr()->currDispMode = arg;
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
}

static void effMObjBrokenDisp2(CameraId cameraId, EffectEntry* effect) {
    s32 off;
    EffMObjBrokenWork* work;
    EffMObjBrokenWork* top;
    s32 i;
    s32 pass;
    Mtx mT, mS, mR, mBase;
    GXColor color;

    top = effect->userdata;
    camGetPtr(cameraId);
    PSMTXTrans(mT, top->position.x, top->position.y, top->position.z);
    PSMTXRotRad(mR, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXScale(mS, top->scale, top->scale, top->scale);
    PSMTXConcat(mT, mR, mT);
    PSMTXConcat(mT, mS, mBase);
    for (pass = 0; pass < 2; pass++) {
        work = effect->userdata;
        work++;
        off = (1 - pass) * 4;
        for (i = 1; i < effect->count; i++, work++) {
            if (work->poseId == -1) {
                continue;
            }
            if (pass == 0) {
                animPoseSetMaterialFlagOn(work->poseId, 0x40);
                color = (GXColor){0, 0, 0, 128};
                animPoseSetMaterialEvtColor(work->poseId, color);
            } else {
                animPoseSetMaterialFlagOn(work->poseId, 0x40);
                color = (GXColor){255, 255, 255, 254};
                animPoseSetMaterialEvtColor(work->poseId, color);
            }
            PSMTXTrans(mT, work->position.x + off, work->position.y - off, work->position.z);
            PSMTXScale(mS, work->scale, work->scale, work->scale);
            PSMTXConcat(mBase, mT, mT);
            PSMTXConcat(mT, mS, mT);
            animPoseSetGXFunc(work->poseId, (void (*)(s32))rendermodeFunc, FALSE);
            animPoseMain(work->poseId);
            animPoseDrawMtx(work->poseId, mT, 1, 0.0f, 0.8f);
            animPoseDrawMtx(work->poseId, mT, 2, 0.0f, 0.8f);
            animPoseDrawMtx(work->poseId, mT, 3, 0.0f, 0.8f);
        }
    }
}
