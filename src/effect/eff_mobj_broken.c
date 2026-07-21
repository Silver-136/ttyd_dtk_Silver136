#include "effect/eff_mobj_broken.h"

void effMObjBrokenMain(void* effect);

void* animGetPtr(void);
void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
void GXSetZCompLoc(s32 beforeTex);
void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
void GXSetZMode(s32 enable, s32 func, s32 updateEnable);

void* effMObjBrokenEntry(double x, double y, double z, u32 typeId) {
    typedef struct { f32 x; f32 y; f32 z; } Vec;
    typedef struct {
        u32 typeId;
        Vec pos;
        f32 scale;
        s32 alpha;
        f32 rot;
        f32 angle;
        Vec vel;
        s32 poseId;
    } BrokenWork;

    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern s32 irand(s32 range);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void effKemuTestEntry(s32 type, double x, double y, double z, double scale);

    void* effect = effEntry();
    s32 count = 1;
    BrokenWork* work;
    BrokenWork* child;
    s32 i;
    f32 zero = 0.0f;
    f32 half = 0.5f;
    f32 randScale = 0.0625f;
    f32 divFive = 5.0f;

    if ((s32)typeId >= 0 && (s32)typeId < 7) {
        count = 17;
    } else if ((s32)typeId < 8) {
        count = 9;
    }

    *(char**)((s32)effect + 0x14) = "MObjBroken";
    *(s32*)((s32)effect + 8) = count;
    work = (BrokenWork*)__memAlloc(3, count * 0x30);
    *(BrokenWork**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effMObjBrokenMain;
    *(u32*)effect |= 2;

    work->typeId = typeId;
    work->pos.x = (f32)x;
    work->pos.y = (f32)y;
    work->pos.z = (f32)z;
    work->scale = 1.0f;
    work->poseId = -1;

    child = work + 1;
    for (i = 1; i < count; i++, child++) {
        s32 index = i;
        s32 n;
        s32 a = 0;
        s32 b = 0;
        s32 c = 0;
        f32 size = 12.5f;
        f32 halfSize;

        if (index > 9) {
            index -= 8;
        }
        n = index - 1;

        if ((s32)typeId >= 0 && (s32)typeId < 3) {
            size = 6.25f;
            a = ((n % 4) / 2);
            b = n / 4;
            c = (n % 4) & 1;
        } else if (((s32)typeId >= 3 && (s32)typeId < 6)) {
            size = 12.5f;
            a = ((n % 4) / 2);
            b = n / 4;
            c = (n % 4) & 1;
        } else if (typeId == 6 || ((s32)typeId >= 6 && (s32)typeId < 8)) {
            size = 12.5f;
            a = n & 1;
            b = n / 2;
            c = 0;
        }

        halfSize = size * half;
        child->pos.x = ((f32)a * size) - halfSize;
        child->pos.y = ((f32)b * size) + halfSize;
        child->pos.z = ((f32)c * size) - halfSize;
        child->scale = size;
        child->alpha = 0xFF;
        child->rot = zero;
        child->poseId = -1;
        child->vel.x = (f32)(irand(10) + 10) * randScale;
        child->angle = angleABf(zero, zero, child->pos.x, child->pos.z);
        child->vel.y = (f32)(irand(3) + 3);
        child->vel.z = (f32)(irand(5) + 2) / divFive;
    }

    if (typeId == 7) {
        effKemuTestEntry(0x10, x, (f32)(y + child[1].pos.y), z, 1.0f);
    } else {
        f32 scale = 1.0f;
        if ((s32)typeId < 3) {
            scale = 0.5f;
        }
        effKemuTestEntry(2, x, (f32)(y + child->pos.y), z, scale);
    }

    return effect;
}

void effMObjBrokenMain(void* effect) {
    typedef unsigned int u32;
    typedef int s32;
    typedef float f32;
    typedef struct { f32 x; f32 y; f32 z; } Vec;
    typedef struct {
        u32 typeId;
        Vec pos;
        f32 scale;
        s32 alpha;
        f32 rot;
        f32 angle;
        Vec vel;
        s32 poseId;
    } BrokenWork;

    extern void* gp;
    extern Vec vec3_802fef30;
    extern void animPoseRelease(s32 poseId);
    extern void effDelete(void* effect);
    extern s32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 animPoseEntry(const char* name, s32 inBattle);
    extern void animPoseSetAnim(s32 poseId, const char* animName, s32 force);
    extern f32 dispCalcZ(Vec* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* dispFunc, void* data, f32 z);
    extern void effMObjBrokenDisp(s32 cameraId, void* effect);
    extern void effMObjBrokenDisp2(s32 cameraId, void* effect);

    BrokenWork* work = *(BrokenWork**)((s32)effect + 0xC);
    Vec pos;
    Vec basePos;
    s32 deadCount = 0;
    s32 type;
    s32 inBattle;
    s32 i;
    BrokenWork* child;
    s32 flag;

    basePos = vec3_802fef30;
    basePos.x = work->pos.x;
    basePos.y = work->pos.y;
    basePos.z = work->pos.z;
    pos = basePos;

    flag = *(s32*)((s32)gp + 0x14);
    type = (s32)work->typeId;
    inBattle = ((u32)(-flag) | (u32)flag) >> 31;

    if ((*(u32*)effect & 4) != 0) {
        child = work;
        for (i = 0; i < *(s32*)((s32)effect + 8); i++, child++) {
            if (child->poseId != -1) {
                animPoseRelease(child->poseId);
            }
        }
        effDelete(effect);
        return;
    }

    if (type == 7) {
        if (animGroupBaseAsync("EFF_sango", inBattle, 0) == 0) {
            return;
        }
    }

    {
        f32 twoPi;
        f32 deg360;
        f32 sinCoef;
        f32 one;
        f32 pi;
        f32 cosCoef;
        f32 killY;

        twoPi = 6.2832f;
        i = 1;
        deg360 = 360.0f;
        child = work + 1;
        sinCoef = 0.16605f;
        one = 1.0f;
        pi = 3.1416f;
        cosCoef = 0.4967f;
        killY = -100.0f;

        for (; i < *(s32*)((s32)effect + 8); i++, child++) {
            f32 angle;
            f32 t;
            f32 t2;
            f32 sVal;
            f32 cVal;
            f32 small;

            if (type == 7 && child->poseId == -1) {
                child->poseId = animPoseEntry("EFF_sango", inBattle);
                animPoseSetAnim(child->poseId, (i & 1) ? "Z_2" : "Z_1", 1);
            }

            small = 0.00761f;
            angle = (twoPi * child->angle) / deg360;
            if (angle <= pi) {
                if (angle <= 1.5708f) {
                    t = angle;
                    t2 = t * t;
                    sVal = ((small * t2 - sinCoef) * t2 + one) * t;
                } else {
                    t = 1.5708f - (angle - 1.5708f);
                    t2 = t * t;
                    sVal = ((small * t2 - sinCoef) * t2 + one) * t;
                }
            } else if (angle < 4.7124f) {
                t = angle - pi;
                t2 = t * t;
                sVal = -(((small * t2 - sinCoef) * t2 + one) * t);
            } else {
                t = 1.5708f - (angle - 4.7124f);
                t2 = t * t;
                sVal = -(((small * t2 - sinCoef) * t2 + one) * t);
            }
            child->pos.x += child->vel.x * sVal;

            small = 0.03705f;
            angle = (twoPi * child->angle) / deg360;
            if (angle <= pi) {
                if (angle <= 1.5708f) {
                    t = angle;
                    t2 = t * t;
                    cVal = (small * t2 - cosCoef) * t2 + one;
                } else {
                    t = 1.5708f - (angle - 1.5708f);
                    t2 = t * t;
                    cVal = -((small * t2 - cosCoef) * t2 + one);
                }
            } else if (angle < 4.7124f) {
                t = angle - pi;
                t2 = t * t;
                cVal = -((small * t2 - cosCoef) * t2 + one);
            } else {
                t = 1.5708f - (angle - 4.7124f);
                t2 = t * t;
                cVal = (small * t2 - cosCoef) * t2 + one;
            }
            child->pos.z += child->vel.x * -cVal;
            child->pos.y += child->vel.y;
            child->vel.y -= child->vel.z;

            if (type < 6) {
                child->rot += (20.0f - child->rot) * 0.125f;
            } else {
                child->rot += (720.0f - child->rot) * 0.03125f;
            }

            if (child->pos.y < killY) {
                deadCount++;
            }
        }
    }

    if (deadCount < *(s32*)((s32)effect + 8) - 1) {
        if (type == 7) {
            dispEntry(4, 8, effMObjBrokenDisp2, effect, dispCalcZ(&pos));
        } else {
            dispEntry(4, 8, effMObjBrokenDisp, effect, dispCalcZ(&pos));
        }
    } else {
        child = work;
        for (i = 0; i < *(s32*)((s32)effect + 8); i++, child++) {
            if (child->poseId != -1) {
                animPoseRelease(child->poseId);
            }
        }
        effDelete(effect);
    }
}

void effMObjBrokenDisp(s32 cameraId, void* effect) {
    typedef struct { f32 x; f32 y; f32 z; } Vec;
    typedef struct {
        u32 typeId;
        Vec pos;
        f32 scale;
        s32 alpha;
        f32 rot;
        f32 angle;
        Vec vel;
        s32 poseId;
    } BrokenWork;
    typedef f32 Mtx[3][4];

    extern void* camGetPtr(s32 cameraId);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void PSMTXTrans(Mtx m, double x, double y, double z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void GXSetNumChans(s32 num);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 num);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 num);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void effGetTexObj(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern double cos(double x);
    extern double sin(double x);
    extern void PSMTXRotAxisRad(Mtx m, Vec* axis, double rad);
    extern void GXLoadPosMtxImm(Mtx m, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXDrawCube(void);

    BrokenWork* work = *(BrokenWork**)((s32)effect + 0xC);
    void* cam = camGetPtr(cameraId);
    u8 texObj[0x20];
    Mtx model;
    Mtx scaleMtx;
    Mtx rotMtx;
    Mtx viewModel;
    s32 i;
    u32 color;
    Vec axis;
    BrokenWork* child;
    f32 angle;
    f32 c;
    f32 s;

    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 3, 0);

    PSMTXTrans(model, work->pos.x, work->pos.y, work->pos.z);
    PSMTXScale(scaleMtx, work->scale, work->scale, work->scale);
    PSMTXConcat(model, scaleMtx, model);
    PSMTXConcat((void*)((s32)cam + 0x11C), model, viewModel);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOp(0, 0);
    GXSetCullMode(2);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    if ((s32)work->typeId < 6) {
        effGetTexObj((work->typeId % 3) + 0x38, texObj);
    } else {
        effGetTexObj(0x3B, texObj);
    }
    GXLoadTexObj(texObj, 0);

    color = 0x00000080;
    GXSetChanMatColor(4, &color);
    child = work + 1;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, child++) {
        angle = (6.283185f * child->angle) / 360.0f;
        c = (f32)cos(angle);
        s = (f32)sin(angle);
        axis.x = c;
        axis.y = 0.0f;
        axis.z = s;
        PSMTXTrans(model, child->pos.x + 5.0f, child->pos.y - 5.0f, child->pos.z);
        PSMTXScale(scaleMtx, child->scale, child->scale, child->scale);
        PSMTXConcat(model, scaleMtx, model);
        PSMTXRotAxisRad(rotMtx, &axis, 0.017453292f * -child->rot);
        PSMTXConcat(model, rotMtx, model);
        PSMTXScale(scaleMtx, 1.0f, 1.0f, 0.0f);
        PSMTXConcat(model, scaleMtx, model);
        PSMTXConcat(viewModel, model, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        GXDrawCube();
    }

    color = 0xFFFFFFFF;
    GXSetChanMatColor(4, &color);
    child = work + 1;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, child++) {
        angle = (6.283185f * child->angle) / 360.0f;
        c = (f32)cos(angle);
        s = (f32)sin(angle);
        axis.x = c;
        axis.y = 0.0f;
        axis.z = s;
        PSMTXTrans(model, child->pos.x, child->pos.y, child->pos.z);
        PSMTXScale(scaleMtx, child->scale, child->scale, child->scale);
        PSMTXConcat(model, scaleMtx, model);
        PSMTXRotAxisRad(rotMtx, &axis, 0.017453292f * -child->rot);
        PSMTXConcat(model, rotMtx, model);
        PSMTXScale(scaleMtx, 1.0f, 1.0f, 0.0f);
        PSMTXConcat(model, scaleMtx, model);
        PSMTXConcat(viewModel, model, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        GXDrawCube();
    }
}

void rendermodeFunc(s32 mode) {
    void* anim = animGetPtr();

    *(s32*)((s32)anim + 0xE0) = mode;
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 3, 0);
}


/* CHATGPT STUB FILL: main/effect/eff_mobj_broken 20260624_184823 */

/* stub-fill: effMObjBrokenDisp2 | prototype_only | source_prototype */
void effMObjBrokenDisp2(s32 cameraId, void* effect) {
    typedef f32 MtxLocal[3][4];

    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void animPoseSetMaterialFlagOn(s32 poseId, s32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern void animPoseSetGXFunc(s32 poseId, void* func, s32 user);
    extern void animPoseMain(s32 poseId);
    extern void animPoseDrawMtx(s32 poseId, void* mtx, f32 arg2, s32 layer, f32 arg4);
    extern void rendermodeFunc(s32 mode);
    extern u32 dat_80427638;
    extern u32 dat_8042763c;
    extern f32 float_deg2rad_80427640;
    extern f32 float_0_80427644;
    extern f32 float_0p8_80427648;

    void* workBase = *(void**)((s32)effect + 0xC);
    void* cam;
    void* work;
    MtxLocal transMtx;
    MtxLocal rotMtx;
    MtxLocal scaleMtx;
    MtxLocal baseMtx;
    MtxLocal modelMtx;
    s32 pass;
    s32 i;
    s32 offset;
    u32 color;

    cam = camGetPtr(cameraId);
    PSMTXTrans(transMtx, *(f32*)((s32)workBase + 4), *(f32*)((s32)workBase + 8), *(f32*)((s32)workBase + 0xC));
    PSMTXRotRad(rotMtx, 0x79, float_deg2rad_80427640 * -*(f32*)((s32)camGetPtr(cameraId) + 0x114));
    PSMTXScale(scaleMtx, *(f32*)((s32)workBase + 0x10), *(f32*)((s32)workBase + 0x10), *(f32*)((s32)workBase + 0x10));
    PSMTXConcat(transMtx, rotMtx, transMtx);
    PSMTXConcat(transMtx, scaleMtx, baseMtx);

    for (pass = 0; pass < 2; pass++) {
        work = (void*)((s32)*(void**)((s32)effect + 0xC) + 0x30);
        offset = (1 - pass) * 4;
        for (i = 1; i < *(s32*)((s32)effect + 8); i++, work = (void*)((s32)work + 0x30)) {
            s32 poseId = *(s32*)((s32)work + 0x2C);
            if (poseId != -1) {
                animPoseSetMaterialFlagOn(poseId, 0x40);
                color = (pass == 0) ? dat_80427638 : dat_8042763c;
                animPoseSetMaterialEvtColor(poseId, &color);
                PSMTXTrans(transMtx,
                           *(f32*)((s32)work + 4) + (f32)offset,
                           *(f32*)((s32)work + 8) - (f32)offset,
                           *(f32*)((s32)work + 0xC));
                PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10));
                PSMTXConcat(baseMtx, transMtx, transMtx);
                PSMTXConcat(transMtx, scaleMtx, modelMtx);
                animPoseSetGXFunc(poseId, rendermodeFunc, 0);
                animPoseMain(poseId);
                animPoseDrawMtx(poseId, modelMtx, float_0_80427644, 1, float_0p8_80427648);
                animPoseDrawMtx(poseId, modelMtx, float_0_80427644, 2, float_0p8_80427648);
                animPoseDrawMtx(poseId, modelMtx, float_0_80427644, 3, float_0p8_80427648);
            }
        }
    }
}
