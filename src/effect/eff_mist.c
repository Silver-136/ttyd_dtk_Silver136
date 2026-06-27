#include "effect/eff_mist.h"

void effMistDisp(s32 cameraId, void* effect) {
    typedef struct Vec { f32 x; f32 y; f32 z; } Vec;
    typedef struct EffEntryLocal {
        u32 flags;
        u32 unk4;
        s32 count;
        void* work;
        void* mainFunc;
        void* typeName;
        char name[16];
    } EffEntryLocal;
    typedef struct MistWork {
        s32 type;
        Vec pos;
        Vec scale;
        Vec vel;
        f32 yVel;
        f32 yAccel;
        f32 yScale;
        f32 alpha;
        s32 counter;
        s32 timer;
        f32 angle;
        u8 r;
        u8 g;
        u8 b;
        u8 pad47;
    } MistWork;

    extern void* camGetPtr(s32 cameraId);
    extern void* mapGetWork(void);
    extern void effGetTexObj(s32 texId, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 texCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 stages);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 cnt, s32 type, s32 frac);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    extern f32 float_deg2rad_80427a38;
    extern f32 float_32_80427a3c;
    extern f32 float_255_80427a40;
    extern f32 float_0p5_80427a44;
    extern f32 float_16_80427a48;
    extern f32 float_0_80427a4c;
    extern f32 float_1_80427a50;
    extern u32 unk_804298c8;

    EffEntryLocal* eff;
    MistWork* work;
    MistWork* child;
    void* cam;
    void* cam2;
    void* mapWork;
    u8 texObj[0x20];
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 parentMtx[3][4];
    f32 scaleMtx[3][4];
    s32 i;
    s32 baseAlpha;
    f32 half;
    f32 size;
    f32 one;
    f32 zero;
    u32 color;
    volatile f32* fifo;

    eff = (EffEntryLocal*)effect;
    work = (MistWork*)eff->work;
    cam = camGetPtr(cameraId);
    baseAlpha = (s32)work->alpha;

    if (work->type == 4) {
        mapWork = mapGetWork();
        if ((*(u16*)((s32)mapWork + 4) & 2) != 0) {
            return;
        }
    }

    effGetTexObj(0x44, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);

    PSMTXTrans(transMtx, work->pos.x, work->pos.y, work->pos.z);
    cam2 = camGetPtr(cameraId);
    PSMTXRotRad(rotMtx, 0x79, float_deg2rad_80427a38 * -*(f32*)((s32)cam2 + 0x114));
    PSMTXConcat(transMtx, rotMtx, parentMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), parentMtx, parentMtx);

    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    half = -float_32_80427a3c * float_0p5_80427a44;
    size = float_16_80427a48;
    zero = float_0_80427a4c;
    one = float_1_80427a50;
    fifo = (volatile f32*)0xCC008000;
    child = work + 1;
    for (i = 1; i < eff->count; i++, child++) {
        PSMTXTrans(transMtx, child->pos.x, child->pos.y, child->pos.z);
        PSMTXScale(scaleMtx, child->scale.x, child->scale.y, child->scale.z);
        PSMTXConcat(transMtx, scaleMtx, transMtx);
        PSMTXConcat(parentMtx, transMtx, transMtx);
        GXLoadPosMtxImm(transMtx, 0);
        GXSetCurrentMtx(0);

        color = unk_804298c8;
        ((u8*)&color)[0] = child->r;
        ((u8*)&color)[1] = child->g;
        ((u8*)&color)[2] = child->b;
        ((u8*)&color)[3] = (u8)((child->alpha * (f32)baseAlpha) / float_255_80427a40);
        GXSetChanMatColor(4, &color);

        GXBegin(0x80, 0, 4);
        *fifo = half; *fifo = size; *fifo = zero; *fifo = zero; *fifo = zero;
        *fifo = size; *fifo = size; *fifo = zero; *fifo = one; *fifo = zero;
        *fifo = size; *fifo = half; *fifo = zero; *fifo = one; *fifo = one;
        *fifo = half; *fifo = half; *fifo = zero; *fifo = zero; *fifo = one;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMistMain(void* effect) {
    typedef struct Vec { f32 x; f32 y; f32 z; } Vec;
    typedef struct EffEntryLocal {
        u32 flags;
        u32 unk4;
        s32 count;
        void* work;
        void* mainFunc;
        void* typeName;
        char name[16];
    } EffEntryLocal;
    typedef struct MistWork {
        s32 type;
        Vec pos;
        Vec scale;
        Vec vel;
        f32 yVel;
        f32 yAccel;
        f32 yScale;
        f32 alpha;
        s32 counter;
        s32 timer;
        f32 angle;
        u8 r;
        u8 g;
        u8 b;
        u8 pad47;
    } MistWork;
    extern void effDelete(void* effect);
    extern s32 irand(s32 range);
    extern f32 dispCalcZ(Vec* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 z);
    extern void effMistDisp(s32 cameraId, void* effect);
    extern Vec vec3_802ff210;
    extern f32 float_0p9_80427a54;
    extern f32 float_0p88_80427a58;
    extern f32 float_1p02_80427a5c;
    extern f32 float_2_80427a60;
    extern f32 float_0_80427a4c;
    extern f32 float_255_80427a40;

    EffEntryLocal* eff;
    MistWork* work;
    MistWork* child;
    Vec pos;
    Vec pos2;
    s32 type;
    s32 timer;
    s32 i;
    s32 rnd;
    f32 zero;
    f32 maxAlpha;

    eff = (EffEntryLocal*)effect;
    work = (MistWork*)eff->work;
    pos = vec3_802ff210;
    pos.x = work->pos.x;
    pos.y = work->pos.y;
    pos.z = work->pos.z;
    pos2 = pos;
    type = work->type;

    if ((eff->flags & 4) != 0) {
        eff->flags &= ~4;
        work->timer = 0x20;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    work->counter++;
    timer = work->timer;
    if (timer < 0) {
        effDelete(eff);
        return;
    }
    if (timer < 0x20) {
        work->alpha = (f32)(timer << 3);
    }

    child = work + 1;
    for (i = 1; i < eff->count; i++, child++) {
        if (type == 4) {
            zero = float_0_80427a4c;
            child->pos.x += child->vel.x;
            child->pos.y += child->vel.y;
            child->pos.z += child->vel.z;
            child->vel.x *= float_0p9_80427a54;
            child->vel.y *= float_0p88_80427a58;
            child->vel.z *= float_0p9_80427a54;
            child->pos.y += child->yVel;
            child->yVel += child->yAccel;
            child->yAccel *= child->yScale;
            child->scale.x *= float_1p02_80427a5c;
            child->scale.y *= float_1p02_80427a5c;
            child->scale.z *= float_1p02_80427a5c;
            child->alpha -= float_2_80427a60;
            if (child->alpha < zero) {
                child->alpha = zero;
            }
        } else {
            if (child->counter < 0xFF) {
                rnd = irand(5);
                child->counter += rnd;
                child->alpha = (f32)child->counter;
                maxAlpha = float_255_80427a40;
                if (child->alpha > maxAlpha) {
                    child->alpha = maxAlpha;
                }
            } else {
                rnd = irand(5);
                child->alpha -= (f32)rnd;
                zero = float_0_80427a4c;
                if (child->alpha < zero) {
                    child->counter = 0;
                    child->alpha = zero;
                }
            }
            child->pos.x += child->vel.x;
            child->pos.y += child->vel.y;
            child->pos.z += child->vel.z;
        }
    }
    dispEntry(4, 2, effMistDisp, eff, dispCalcZ(&pos2));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void* effMistEntry(f32 x, f32 y, f32 z, f32 angle, s32 type, s32 timer) {
    typedef struct Vec { f32 x; f32 y; f32 z; } Vec;
    typedef struct EffEntryLocal {
        u32 flags;
        u32 unk4;
        s32 count;
        void* work;
        void* mainFunc;
        void* typeName;
        char name[16];
    } EffEntryLocal;
    typedef struct MistWork {
        s32 type;
        Vec pos;
        Vec scale;
        Vec vel;
        f32 yVel;
        f32 yAccel;
        f32 yScale;
        f32 alpha;
        s32 counter;
        s32 timer;
        f32 angle;
        u8 r;
        u8 g;
        u8 b;
        u8 pad47;
    } MistWork;
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* camGetPtr(s32 cameraId);
    extern s32 irand(s32 range);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern void effMistMain(void* effect);
    extern char str_Mist_80427a64[5];
    extern f32 float_255_80427a40;
    extern f32 float_0_80427a4c;
    extern f32 float_10_80427a70;
    extern f32 float_0p8_80427a6c;
    extern f32 float_20_80427a74;
    extern f32 float_1_80427a50;
    extern f32 float_6p2832_80427a78;
    extern f32 float_360_80427a7c;
    extern f32 float_1p1_80427a80;
    extern f32 float_neg2_80427a84;
    extern f32 float_0p95_80427a88;
    extern f32 float_neg0p04_80427a8c;
    extern f32 float_0p94_80427a90;
    extern f32 float_0p3_80427a94;
    extern f32 float_200_80427a98;

    EffEntryLocal* eff;
    MistWork* work;
    MistWork* child;
    void* cam;
    s32 count;
    s32 i;
    s32 v;
    f32 zero;
    f32 baseScale;
    f32 div10;
    f32 div20;
    f32 phase;
    f32 s;
    f32 c;

    eff = (EffEntryLocal*)effEntry();
    count = 0xB;
    if (type == 4) {
        count = 2;
    }
    eff->typeName = str_Mist_80427a64;
    eff->count = count;
    work = (MistWork*)__memAlloc(3, count * 0x48);
    eff->work = work;
    eff->mainFunc = effMistMain;
    eff->flags |= 2;

    work->type = type;
    work->pos.x = x;
    work->pos.y = y;
    work->pos.z = z;
    if (timer < 1) {
        work->timer = 1000;
    } else {
        work->timer = timer;
    }
    work->counter = 0;
    work->alpha = float_255_80427a40;
    cam = camGetPtr(4);
    work->angle = angle - *(f32*)((s32)cam + 0x114);

    zero = float_0_80427a4c;
    div10 = float_10_80427a70;
    baseScale = float_0p8_80427a6c;
    div20 = float_20_80427a74;
    child = work + 1;
    for (i = 1; i < eff->count; i++, child++) {
        child->pos.x = (f32)(irand(0x32) - 0x19);
        child->pos.y = (f32)(irand(0x32) - 0x19);
        child->pos.z = zero;
        child->scale.x = child->scale.y = child->scale.z = baseScale + ((f32)irand(0xC) / div10);
        child->vel.x = (f32)(irand(10) - 5) / div20;
        child->vel.y = (f32)(irand(10) - 5) / div20;
        child->vel.z = zero;
        child->alpha = zero;
        child->timer = irand(0x1E) + 0x1E;
        child->counter = 0;
        cam = camGetPtr(4);
        child->angle = angle - *(f32*)((s32)cam + 0x114);

        switch (type) {
            case 0:
                child->r = 0xFF;
                child->g = 0xFF;
                child->b = 0xFF;
                break;
            case 1:
                child->r = 0x0B;
                child->g = 0x09;
                child->b = 0x60;
                break;
            case 2:
                child->r = 0xE1;
                child->g = 0x99;
                child->b = 0xAF;
                break;
            case 3:
                child->r = 0x49;
                child->g = 0x65;
                child->b = 0;
                break;
            case 4:
                child->pos.x = float_0_80427a4c;
                child->pos.y = zero;
                child->pos.z = float_1_80427a50;
                phase = (float_6p2832_80427a78 * child->angle) / float_360_80427a7c;
                s = (f32)sin(phase);
                child->vel.x = float_1p1_80427a80 * s;
                child->vel.y = float_neg2_80427a84;
                phase = (float_6p2832_80427a78 * child->angle) / float_360_80427a7c;
                c = (f32)cos(phase);
                child->vel.z = float_1p1_80427a80 * -c;
                child->yVel = float_0p95_80427a88;
                child->yAccel = float_neg0p04_80427a8c;
                child->yScale = float_0p94_80427a90;
                child->r = 0xFF;
                child->g = 0xFF;
                child->b = 0xFF;
                child->scale.x = float_0p3_80427a94;
                child->scale.y = float_0p3_80427a94;
                child->scale.z = float_0p3_80427a94;
                child->alpha = float_200_80427a98;
                break;
            case 5:
                child->r = 0x46;
                child->g = 0;
                child->b = 0x32;
                break;
            case 6:
                child->r = 0;
                child->g = 0;
                child->b = 0;
                break;
        }
    }
    return eff;
}
