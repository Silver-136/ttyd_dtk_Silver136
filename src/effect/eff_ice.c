#include "effect/eff_ice.h"

extern void* effEntry(void);
extern void* __memAlloc(s32 heap, u32 size);
extern void* effMayaAnimAlloc(void* data);
extern void effPlayMayaAnim(void* anim);
extern volatile f32 float_1_80424764;
extern void* ice_effAnimDt;
u8 effIceMain(u32* effect);

const char str_Ice_80424788[] = "Ice";
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effIceEntry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 duration) {
    void* entry;
    void* work;
    s32 main;
    f32 one;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_Ice_80424788;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x30);
    *(void**)((s32)entry + 0xC) = work;
    main = (s32)effIceMain;
    one = float_1_80424764;
    *(s32*)((s32)entry + 0x10) = main;
    *(u32*)entry |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = scale;
    *(f32*)((s32)work + 0x14) = one;
    *(f32*)((s32)work + 0x18) = one;
    *(f32*)((s32)work + 0x1C) = one;
    if (duration > 0) {
        *(s32*)((s32)work + 0x20) = duration;
    } else {
        *(s32*)((s32)work + 0x20) = 1000;
    }
    *(s32*)((s32)work + 0x24) = 0;
    *(s32*)((s32)work + 0x28) = 0;
    *(void**)((s32)work + 0x2C) = effMayaAnimAlloc(&ice_effAnimDt);
    effPlayMayaAnim(*(void**)((s32)work + 0x2C));
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effIceMain(u32* effect) {
    typedef struct VecLocal { f32 x; f32 y; f32 z; } VecLocal;

    extern void effCalcMayaAnim(void*);
    extern s32 irand(s32);
    extern void effKemuTestEntry(f32, f32, f32, s32);
    extern void effDeleteMayaAnim(void*);
    extern void effDelete(void*);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effIceDisp(s32, void*);
    extern VecLocal vec3_802f9a70;
    extern void* gp;
    extern f32 float_30_80424768;
    extern volatile f32 float_1_80424764;
    extern f32 float_1000_80424778;
    extern f32 float_40_8042477c;
    extern f32 float_0p99_80424780;
    extern f32 float_1p005_80424784;

    u8* work;
    VecLocal pos;
    s32 mode;
    s32 frame;
    s32 r;
    f32 mayaFrame;

    work = (u8*)effect[3];
    pos = vec3_802f9a70;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);

    if (*effect & 4) {
        *effect &= ~4;
        *(s32*)(work + 0x24) = 0x5A;
        *(s32*)(work + 0x28) = 1;
        /* First-pass baseline: leave exact global-time maya offset for a later shape pass. */
        *(s32*)(*(s32*)(work + 0x2C) + 8) = 0;
        *(s32*)(*(s32*)(work + 0x2C) + 0xC) = 0;
    }

    mode = *(s32*)(work + 0x28);
    if (mode == 1) {
        *(s32*)(work + 0x24) += 1;
        effCalcMayaAnim(*(void**)(work + 0x2C));
    } else if (mode < 1) {
        if (mode >= 0) {
            if (*(s32*)(work + 0x20) < 1000) {
                *(s32*)(work + 0x20) -= 1;
            }
            if (*(s32*)(work + 0x20) == 0) {
                *(s32*)(work + 0x28) = 1;
            }
            if (*(s32*)(work + 0x24) < 0x5A) {
                *(s32*)(work + 0x24) += 1;
                effCalcMayaAnim(*(void**)(work + 0x2C));
            }
        }
    } else if (mode < 3) {
        *(s32*)(work + 0x24) += 1;
        frame = *(s32*)(work + 0x24);
        if ((frame % 30) == 0) {
            r = irand((s32)(float_40_8042477c * *(f32*)(work + 0x10)));
            effKemuTestEntry(*(f32*)(work + 4) + (f32)(0x14 - r),
                             *(f32*)(work + 8) - (float_30_80424768 * (float_1_80424764 - *(f32*)(work + 0x18))),
                             *(f32*)(work + 0xC), 1);
        }
        *(f32*)(work + 0x18) *= float_0p99_80424780;
        *(f32*)(work + 0x14) *= float_1p005_80424784;
        *(f32*)(work + 0x1C) *= float_0p99_80424780;
    }

    mode = *(s32*)(work + 0x28);
    if (mode == 2) {
        if (*(s32*)(work + 0x24) > 200) {
            effDeleteMayaAnim(*(void**)(work + 0x2C));
            effDelete(effect);
            return 0;
        }
    } else if ((mode < 2) && (mode >= 0)) {
        mayaFrame = *(f32*)(*(s32*)(*(s32*)(work + 0x2C) + 0x10) + 4);
        if (mayaFrame < (f32)*(s32*)(work + 0x24)) {
            effDeleteMayaAnim(*(void**)(work + 0x2C));
            effDelete(effect);
            return 0;
        }
    }
    dispEntry(4, 2, effIceDisp, effect, dispCalcZ(&pos));
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effIceDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObjLocal { u32 dummy[8]; } GXTexObjLocal;

    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void* camGetPtr(s32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void GXSetNumChans(u32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(u32);
    extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
    extern void GXSetNumTevStages(u32);
    extern void GXSetTevOrder(u32, u32, u32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCurrentMtx(u32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void effDrawMayaPoly(s32);
    extern f32 float_deg2rad_80424758;
    extern f32 float_0_8042475c;
    extern f32 float_neg30_80424760;
    extern volatile f32 float_1_80424764;
    extern f32 float_30_80424768;
    extern f32 float_8_8042476c;
    extern f32 float_191p25_80424770;
    extern f32 float_127p5_80424774;
    extern u32 dat_80424750;
    extern u32 dat_80424754;

    u8* work;
    void* cam;
    void* anim;
    GXTexObjLocal texObj;
    Mtx trans;
    Mtx rot;
    Mtx base;
    Mtx scale;
    Mtx local;
    s32 count;
    s32 i;
    s32 texId;
    s32 alpha;
    s32 polyOffset;
    s32 mtxOffset;
    u32 color;
    f32 s;

    work = *(u8**)((s32)effect + 0xC);
    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    cam = camGetPtr(cameraId);
    PSMTXRotRad(rot, float_deg2rad_80424758 * -*(f32*)((s32)cam + 0x114), 'y');
    PSMTXConcat(trans, rot, base);
    PSMTXTrans(trans, float_0_8042475c, float_neg30_80424760, float_0_8042475c);
    PSMTXConcat(base, trans, base);
    PSMTXScale(scale, *(f32*)(work + 0x14), *(f32*)(work + 0x18), float_1_80424764);
    PSMTXConcat(base, scale, base);
    PSMTXTrans(trans, float_0_8042475c, float_30_80424768, float_0_8042475c);
    PSMTXConcat(base, trans, base);
    s = float_8_8042476c * *(f32*)(work + 0x10);
    PSMTXScale(scale, s, s, s);
    PSMTXConcat(base, scale, base);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetCurrentMtx(0);

    anim = *(void**)(work + 0x2C);
    count = **(s32**)(*(s32*)(*(s32*)((s32)anim + 0x10) + 8));
    mtxOffset = 0x34;
    polyOffset = 0x10;
    for (i = 1; i < count; i++) {
        texId = 0x20;
        if ((u32)i < (u32)(count - 1)) {
            texId = 0x1F;
        }
        effGetTexObj(texId, &texObj);
        GXLoadTexObj(&texObj, 0);
        if ((u32)i < (u32)(**(s32**)(*(s32*)(*(s32*)((s32)anim + 0x10) + 8)) - 1)) {
            effGetTexObj(0x1F, &texObj);
            GXLoadTexObj(&texObj, 0);
            alpha = (s32)(float_191p25_80424770 * *(f32*)(work + 0x1C));
            color = (dat_80424750 & 0xFFFFFF00) | (alpha & 0xFF);
            GXSetChanMatColor(4, &color);
        } else {
            effGetTexObj(0x20, &texObj);
            GXLoadTexObj(&texObj, 0);
            alpha = (s32)(float_127p5_80424774 * *(f32*)(work + 0x1C));
            color = (dat_80424754 & 0xFFFFFF00) | (alpha & 0xFF);
            GXSetChanMatColor(4, &color);
        }
        PSMTXConcat(base, (void*)((s32)anim + 0x14), local);
        PSMTXConcat(local, (void*)((s32)anim + mtxOffset + 0x14), local);
        cam = camGetPtr(cameraId);
        PSMTXConcat((void*)((s32)cam + 0x11C), local, local);
        GXLoadPosMtxImm(local, 0);
        effDrawMayaPoly(*(s32*)(*(s32*)(*(s32*)(*(s32*)((s32)anim + 0x10) + 8) + 4) + polyOffset));
        mtxOffset += 0x34;
        polyOffset += 0x10;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
