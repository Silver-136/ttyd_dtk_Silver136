#include "effect/eff_bomb.h"
void* effBombEntry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(s32 type);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* effConfettiEntry(s32 type, s32 count, f32 x, f32 y, f32 z);
    extern u8 effBombMain(int* effect);
    extern double sin(double x);
    extern double cos(double x);
    extern char str_Bomb_804247e8[];
    extern f32 float_0_804247d8;
    extern f32 float_deg2rad_804247c8;
    extern f32 float_10_804247f0;
    extern f32 float_90_804247f4;
    extern f32 float_45_804247f8;
    void* effect;
    u8* work;
    u8* entry;
    f32 radius;
    f32 step;
    f32 deg2rad;
    f32 zero;
    f32 angle;
    s32 i;

    effect = effEntry(type);
    *(char**)((s32)effect + 0x14) = str_Bomb_804247e8;
    *(s32*)((s32)effect + 8) = 9;
    work = __memAlloc(3, *(s32*)((s32)effect + 8) * 0x28);
    *(u8**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effBombMain;
    radius = float_10_804247f0 * scale;
    *(s32*)(work + 0) = type;
    entry = work + 0x28;
    step = float_90_804247f4;
    i = 1;
    *(f32*)(work + 4) = x;
    deg2rad = float_deg2rad_804247c8;
    *(f32*)(work + 8) = y;
    zero = float_0_804247d8;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x18) = scale;
    *(s32*)(work + 0x20) = 0;
    *(s32*)(work + 0x24) = 0;

    for (; i < *(s32*)((s32)effect + 8); i++) {
        angle = step * (f32)(i - 1);
        if (i > 4) {
            angle += float_45_804247f8;
        } else {
            angle += zero;
        }
        *(f32*)(entry + 0x1C) = angle;
        *(f32*)(entry + 4) = radius * (f32)sin(deg2rad * *(f32*)(entry + 0x1C));
        *(f32*)(entry + 8) = radius * (f32)cos(deg2rad * *(f32*)(entry + 0x1C));
        *(f32*)(entry + 0xC) = zero;
        *(f32*)(entry + 0x10) = zero;
        *(f32*)(entry + 0x14) = zero;
        *(s32*)(entry + 0x24) = 0;
        *(s32*)(entry + 0x20) = 0;
        entry += 0x28;
    }

    effConfettiEntry(4, 0x28, x, y, z);
    return effect;
}

void effBombMain(void* effect) {
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(void* pos);
    extern void* dispEntry(s32 cameraId, s32 order, void* callback, void* param, f32 priority);
    extern u8 effBombDisp(void);
    extern f32 float_100_804247e4;
    extern u8 scale_dt[];
    extern u8 scale_dt2[];
    extern f32 vec3_802f9aa0[3];
    u8* work;
    u8* entry;
    u8* table1;
    u8* table2;
    u8* table1_1;
    u8* table1_2;
    u8* table2_1;
    u8* table2_2;
    f32 pos[3];
    f32 dispPos[3];
    s32 oldFrame;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    pos[0] = vec3_802f9aa0[0];
    pos[1] = vec3_802f9aa0[1];
    pos[2] = vec3_802f9aa0[2];
    pos[0] = *(f32*)(work + 4);
    pos[1] = *(f32*)(work + 8);
    pos[2] = *(f32*)(work + 0xC);
    dispPos[0] = pos[0];
    dispPos[1] = pos[1];
    dispPos[2] = pos[2];

    oldFrame = *(s32*)(work + 0x20);
    *(s32*)(work + 0x20) = oldFrame + 1;
    if (*(s32*)(work + 0x20) > 0x24) {
        effDelete(effect);
        return;
    }

    table1 = scale_dt + oldFrame * 3;
    table2 = scale_dt2 + oldFrame * 3;
    table1_1 = table1 + 1;
    table1_2 = table1 + 2;
    table2_1 = table2 + 1;
    table2_2 = table2 + 2;
    entry = work + 0x28;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        if (i < 5) {
            if ((u32)oldFrame < 0x36U) {
                *(u32*)(entry + 0x24) = table1[0];
                *(f32*)(entry + 0x10) = (f32)table1_1[0] / float_100_804247e4;
                *(f32*)(entry + 0x14) = (f32)table1_2[0] / float_100_804247e4;
            }
        } else {
            if ((u32)oldFrame < 0x36U) {
                *(u32*)(entry + 0x24) = table2[0];
                *(f32*)(entry + 0x10) = (f32)table2_1[0] / float_100_804247e4;
                *(f32*)(entry + 0x14) = (f32)table2_2[0] / float_100_804247e4;
            }
        }
        entry += 0x28;
    }

    dispEntry(4, 2, effBombDisp, effect, dispCalcZ(dispPos));
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effBombDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObjLocal { u32 dummy[8]; } GXTexObjLocal;

    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(u32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXSetNumTexGens(u32);
    extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
    extern void GXSetNumTevStages(u32);
    extern void GXSetTevOrder(u32, u32, u32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevKAlphaSel(s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(u32, s32, u32, u32, u32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXSetCurrentMtx(u32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXBegin(s32, s32, s16);
    extern u32 dat_804247c0;
    extern u32 dat_804247c4;
    extern f32 float_deg2rad_804247c8;
    extern f32 float_30_804247cc;
    extern f32 float_1_804247d0;
    extern f32 float_0p0625_804247d4;
    extern f32 float_0_804247d8;
    extern f32 float_neg0p05_804247dc;
    extern f32 float_0p5_804247e0;

    u8* work;
    u8* part;
    void* cam;
    void* cam2;
    GXTexObjLocal texObj;
    Mtx base;
    Mtx rot;
    Mtx trans;
    Mtx scale;
    Mtx mtx;
    u32 color;
    volatile f32* fifo;
    f32 baseScale;
    f32 halfNeg;
    s32 i;

    cam = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);
    baseScale = *(f32*)(work + 0x18);
    effGetTexObj(0x24, &texObj);
    GXLoadTexObj(&texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    color = dat_804247c0;
    GXSetChanMatColor(4, &color);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 1);
    GXSetTevAlphaOp(0, 0xE, 0, 0, 1, 1);
    GXSetTevColorIn(0, 0xF, 0xB, 8, 0xF);
    GXSetTevAlphaIn(0, 4, 6, 5, 7);
    GXSetTevKAlphaSel(0, 4);
    GXSetTevOrder(1, 1, 0, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0xE, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0xF, 0xF, 0xF, 0xF);
    GXSetTevAlphaIn(1, 4, 6, 2, 7);
    GXSetTevKAlphaSel(1, 4);
    color = dat_804247c4;
    GXSetTevColor(2, &color);
    GXSetTevOrder(2, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 0, 2, 1, 0xF);
    GXSetTevAlphaIn(2, 0, 7, 1, 1);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    cam2 = camGetPtr(cameraId);
    PSMTXRotRad(rot, float_deg2rad_804247c8 * -*(f32*)((s32)cam2 + 0x114), 'y');
    PSMTXConcat(trans, rot, base);
    PSMTXConcat((void*)((s32)cam + 0x11C), base, base);
    GXSetCurrentMtx(0);
    baseScale *= float_30_804247cc;
    halfNeg = -float_1_804247d0 * float_0p5_804247e0;
    fifo = (volatile f32*)0xCC008000;
    part = work + 0x28;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x28) {
        PSMTXTrans(trans, float_0p0625_804247d4 * (f32)*(s32*)(part + 0x24), float_0_804247d8, float_0_804247d8);
        PSMTXScale(scale, float_0p0625_804247d4, float_1_804247d0, float_1_804247d0);
        PSMTXConcat(trans, scale, mtx);
        GXLoadTexMtxImm(mtx, 0x1E, 1);
        PSMTXTrans(trans, float_neg0p05_804247dc, float_neg0p05_804247dc, float_0_804247d8);
        PSMTXConcat(mtx, trans, mtx);
        GXLoadTexMtxImm(mtx, 0x21, 1);
        PSMTXTrans(trans, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0xC));
        PSMTXRotRad(rot, float_deg2rad_804247c8 * -*(f32*)(part + 0x1C), 'z');
        PSMTXScale(scale, baseScale * *(f32*)(part + 0x10), baseScale * *(f32*)(part + 0x14), baseScale);
        PSMTXConcat(trans, rot, mtx);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(base, mtx, mtx);
        GXLoadPosMtxImm(mtx, 0);
        GXBegin(0x80, 0, 4);
        *fifo = halfNeg; *fifo = float_1_804247d0; *fifo = float_0_804247d8; *fifo = float_0_804247d8; *fifo = float_0_804247d8;
        *fifo = float_0p5_804247e0; *fifo = float_1_804247d0; *fifo = float_0_804247d8; *fifo = float_1_804247d0; *fifo = float_0_804247d8;
        *fifo = float_0p5_804247e0; *fifo = float_0_804247d8; *fifo = float_0_804247d8; *fifo = float_1_804247d0; *fifo = float_1_804247d0;
        *fifo = halfNeg; *fifo = float_0_804247d8; *fifo = float_0_804247d8; *fifo = float_0_804247d8; *fifo = float_1_804247d0;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
