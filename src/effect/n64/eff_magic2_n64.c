#include "effect/n64/eff_magic2_n64.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effMagic2N64Entry(s32 type, s32 arg, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effMagic2Main(void*);
    extern s32 rand(void);
    extern char str_Magic2N64_802fb460[];
    extern f32 float_10_80425a68;
    extern f32 float_4_80425a6c;
    extern f32 float_0_80425a34;
    extern f32 float_0p2_80425a70;
    void* entry;
    u8* work;
    s32 r;
    s32 angle;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Magic2N64_802fb460;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x60);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMagic2Main;

    *(s32*)work = type;
    *(s32*)(work + 0x28) = arg;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x24) = 0;
    *(f32*)(work + 0x10) = x;
    *(f32*)(work + 0x14) = y;
    *(f32*)(work + 0x18) = z;
    *(s32*)(work + 0x44) = 0;
    *(s32*)(work + 0x40) = 0;
    *(s32*)(work + 0x4C) = 0;
    *(s32*)(work + 0x48) = 0;
    *(s32*)(work + 0x34) = 0;
    *(s32*)(work + 0x30) = 0;
    *(s32*)(work + 0x3C) = 0;
    *(s32*)(work + 0x38) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x1C) = float_10_80425a68;

    r = rand();
    angle = r - ((r / 0x168) * 0x168);
    *(f32*)(work + 0x58) = (f32)angle;
    *(f32*)(work + 0x5C) = float_4_80425a6c;
    *(f32*)(work + 0x50) = float_0_80425a34;
    *(f32*)(work + 0x54) = float_0_80425a34;

    if (type == 1) {
        *(s32*)(work + 0x3C) = 0x60;
        *(f32*)(work + 0x20) = scale;
    } else {
        *(s32*)(work + 0x44) = -2;
        *(s32*)(work + 0x4C) = -4;
        *(f32*)(work + 0x20) = float_0p2_80425a70 * scale;
    }
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void effMagic2Main(void* effect) {
    typedef struct Vec3 { f32 x; f32 y; f32 z; } Vec3;
    extern void effDelete(void*);
    extern f64 sin(f64);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effMagic2Disp(s32, s32);
    extern f32 float_0p7_80425a48;
    extern f32 float_6p2832_80425a4c;
    extern f32 float_360_80425a50;
    extern f32 float_0p4_80425a54;
    extern f32 float_0p1_80425a58;
    extern f32 float_0p3_80425a5c;
    u8* work;
    Vec3 pos;
    s32 timer;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    pos.x = *(f32*)(work + 0x10); pos.y = *(f32*)(work + 0x14); pos.z = *(f32*)(work + 0x18);
    *(s32*)(work + 0x28) -= 1;
    *(s32*)(work + 0x2C) += 1;
    timer = *(s32*)(work + 0x28);
    if (timer < 0) { effDelete(effect); return; }
    if (*(s32*)(work + 0x2C) < 11) *(s32*)(work + 0x24) = (*(s32*)(work + 0x2C) * 0xFF) / 10;
    if (timer < 6) *(s32*)(work + 0x24) = (timer * 0xFF) / 6;
    if (timer < 10 && *(s32*)work == 0) *(s32*)(work + 0x1C) = (s32)((f32)*(s32*)(work + 0x1C) * float_0p7_80425a48);
    for (i = 0; i < *(s32*)((s32)effect + 8); i++) {
        f32 wave = (f32)sin((f64)(float_6p2832_80425a4c * (f32)(timer * 10) / float_360_80425a50));
        if (*(s32*)work == 1)
            *(s32*)(work + 0x1C) = (s32)(float_0p4_80425a54 * (((f32)*(s32*)(work + 0x20) * float_0p1_80425a58 * wave + (f32)*(s32*)(work + 0x20)) - (f32)*(s32*)(work + 0x1C)) + (f32)*(s32*)(work + 0x1C));
        else
            *(s32*)(work + 0x1C) = (s32)(float_0p3_80425a5c * (((f32)*(s32*)(work + 0x20) * float_0p1_80425a58 * wave + (f32)*(s32*)(work + 0x20)) - (f32)*(s32*)(work + 0x1C)) + (f32)*(s32*)(work + 0x1C));
        *(s32*)(work + 0x58) += *(s32*)(work + 0x5C);
        *(s32*)(work + 0x50) += *(s32*)(work + 0x54);
        *(s32*)(work + 0x30) += *(s32*)(work + 0x34);
        *(s32*)(work + 0x38) += *(s32*)(work + 0x3C);
        if (*(s32*)(work + 0x30) < 0) *(s32*)(work + 0x30) += 0x100;
        if (*(s32*)(work + 0x38) < 0) *(s32*)(work + 0x38) += 0x100;
        work += 0x60;
    }
    dispEntry(4, 2, effMagic2Disp, effect, dispCalcZ(&pos));
}


void effMagic2Disp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef f32 Mtx44[4][4];
    extern void* camGetPtr(s32);
    extern void GXGetProjectionv(f32*);
    extern void GXGetViewportv(f32*);
    extern void PSMTX44Trans(f32,f32,f32,Mtx44);
    extern void PSMTX44Scale(f32,f32,f32,Mtx44);
    extern void PSMTX44Concat(Mtx44,Mtx44,Mtx44);
    extern void C_MTXPerspective(Mtx44,f32,f32,f32,f32);
    extern void GXSetProjection(Mtx44,s32);
    extern void GXSetProjectionv(f32*);
    extern void PSMTXTrans(Mtx,f32,f32,f32);
    extern void PSMTXScale(Mtx,f32,f32,f32);
    extern void PSMTXConcat(Mtx,Mtx,Mtx);
    extern void GXLoadPosMtxImm(Mtx,s32);
    extern void GXLoadTexMtxImm(Mtx,s32,s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetTevColor(s32,void*);
    extern void GXSetScissorBoxOffset(s32,s32);
    extern void magic2_4_dl(void);
    extern void magic2_2_line_dl(void);
    u8* entry = effect;
    u8* work = *(u8**)(entry + 0xC);
    f32 projection[7];
    f32 viewport[6];
    Mtx44 trans44, scale44, perspective;
    Mtx mtx, scale;
    u32 color1, color2;
    s32 type = *(s32*)work;
    s32 angle = *(s32*)(work + 0x2C) * 3;
    s32 i;

    camGetPtr(cameraId);
    GXGetProjectionv(projection);
    GXGetViewportv(viewport);
    PSMTX44Trans(*(f32*)(work + 0x10), *(f32*)(work + 0x14), *(f32*)(work + 0x18), trans44);
    PSMTX44Scale(*(f32*)(work + 0x1C), *(f32*)(work + 0x1C), 1.0f, scale44);
    PSMTX44Concat(trans44, scale44, trans44);
    C_MTXPerspective(perspective, type == 1 ? 130.0f : 30.0f, 1.2667f, 1.0f, 32768.0f);
    PSMTX44Concat(perspective, trans44, perspective);
    GXSetProjection(perspective, 0);
    if (type == 1) {
        PSMTXTrans(mtx, 0.0f, 0.0f, -70.0f);
    } else {
        PSMTXTrans(mtx, 0.0f, 0.0f, -80.0f);
    }
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    for (i = 0; i < *(s32*)(entry + 8); i++, work += 0x60) {
        angle = (angle + 30) % 24;
        color1 = 0xFFFFFF00 | (u8)*(s32*)(work + 0x24);
        color2 = 0xFFFFFF00 | (u8)(*(s32*)(work + 0x24) / 2);
        GXSetTevColor(1, &color1);
        GXSetTevColor(2, &color2);
        if (type == 0) {
            PSMTXScale(scale, 0.015625f, 0.015625f, 1.0f);
            PSMTXTrans(mtx, (f32)(*(s32*)(work + 0x30) / 4), (f32)(*(s32*)(work + 0x40) / 4), 0.0f);
            PSMTXConcat(scale, mtx, mtx); GXLoadTexMtxImm(mtx, 0x1E, 1);
            PSMTXScale(scale, 0.015625f, 0.015625f, 1.0f);
            PSMTXTrans(mtx, (f32)(*(s32*)(work + 0x38) / 4), (f32)(*(s32*)(work + 0x48) / 4), 0.0f);
            PSMTXConcat(scale, mtx, mtx); GXLoadTexMtxImm(mtx, 0x21, 1);
            magic2_2_line_dl();
        } else {
            PSMTXScale(scale, 0.015625f, 0.0625f, 0.0f);
            PSMTXTrans(mtx, (f32)(*(s32*)(work + 0x30) / 4), (f32)(*(s32*)(work + 0x40) / 4), 0.0f);
            PSMTXConcat(scale, mtx, mtx); GXLoadTexMtxImm(mtx, 0x1E, 1);
            PSMTXScale(scale, 0.015625f, 0.0625f, 1.0f);
            PSMTXTrans(mtx, (f32)(*(s32*)(work + 0x38) / 4), (f32)(*(s32*)(work + 0x48) / 4), 0.0f);
            PSMTXConcat(scale, mtx, mtx); GXLoadTexMtxImm(mtx, 0x21, 1);
            magic2_4_dl();
        }
    }
    GXSetProjectionv(projection);
    GXSetScissorBoxOffset(0, 0);
}

void magic2_2_line_dl(void) {
    extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32); extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void GXSetCullMode(s32); extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32); u8 tex[0x20];
    GXSetNumChans(1); GXSetChanCtrl(4,0,0,1,0,0,2); GXSetNumTexGens(2);
    GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);
    effGetTexObjN64(0x11,tex); GXLoadTexObj(tex,0); GXLoadTexObj(tex,1); GXSetNumTevStages(3);
    GXSetTevOrder(0,0,0,-1); GXSetTevColorOp(0,1,0,0,1,3); GXSetTevAlphaOp(0,0,0,0,1,0); GXSetTevColorIn(0,15,3,8,14); GXSetTevAlphaIn(0,7,6,4,7);
    GXSetTevOrder(1,1,1,4); GXSetTevColorOp(1,0,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0); GXSetTevColorIn(1,15,2,8,2); GXSetTevAlphaIn(1,7,0,5,7);
    GXSetTevOrder(2,1,1,4); GXSetTevColorOp(2,1,0,0,1,0); GXSetTevAlphaOp(2,0,0,0,1,0); GXSetTevColorIn(2,15,8,4,0); GXSetTevAlphaIn(2,7,4,5,0);
    GXSetCullMode(0); effSetVtxDescN64((void*)0x803A5AB8); GXBegin(0x90,0,0x48);
    tri2(0,1,2,0,0,2,3,0); tri2(4,5,1,0,4,1,0,0); tri2(3,2,6,0,3,6,7,0); tri2(8,9,5,0,8,5,4,0);
    tri2(10,11,9,0,10,9,8,0); tri2(12,13,11,0,12,11,10,0); tri2(7,6,14,0,7,14,15,0);
    tri2(16,17,18,0,16,18,19,0); tri2(19,18,20,0,19,20,21,0); tri2(22,23,17,0,22,17,16,0);
    tri2(24,25,23,0,24,23,22,0); tri2(15,14,25,0,15,25,24,0);
}

void magic2_4_dl(void) {
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void effGetTexObjN64(s32,void*);
    extern void GXLoadTexObj(void*,s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32,s32,s32);
    extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    u8 tex[0x20];
    GXSetNumChans(1); GXSetChanCtrl(4,0,0,1,0,0,2);
    GXSetNumTexGens(2); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);
    effGetTexObjN64(0x12,tex); GXLoadTexObj(tex,0); effGetTexObjN64(0x13,tex); GXLoadTexObj(tex,1);
    GXSetNumTevStages(3);
    GXSetTevOrder(0,0,0,-1); GXSetTevColorOp(0,1,0,0,1,3); GXSetTevAlphaOp(0,0,0,0,1,0); GXSetTevColorIn(0,15,3,8,14); GXSetTevAlphaIn(0,7,6,4,7);
    GXSetTevOrder(1,1,1,-1); GXSetTevColorOp(1,0,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0); GXSetTevColorIn(1,15,2,8,2); GXSetTevAlphaIn(1,7,0,5,7);
    GXSetTevOrder(2,1,1,-1); GXSetTevColorOp(2,1,0,0,1,0); GXSetTevAlphaOp(2,0,0,0,1,0); GXSetTevColorIn(2,15,8,4,0); GXSetTevAlphaIn(2,7,4,5,0);
    GXSetCullMode(0); effSetVtxDescN64((void*)0x803A5C24); GXBegin(0x90,0,0x48);
    tri2(0,1,2,0,0,2,3,0); tri2(4,5,1,0,4,1,0,0); tri2(3,2,6,0,3,6,7,0); tri2(8,9,5,0,8,5,4,0);
    tri2(10,11,9,0,10,9,8,0); tri2(12,13,11,0,12,11,10,0); tri2(7,6,14,0,7,14,15,0);
    tri2(16,17,18,0,16,18,19,0); tri2(19,18,20,0,19,20,21,0); tri2(22,23,17,0,22,17,16,0);
    tri2(24,25,23,0,24,23,22,0); tri2(15,14,25,0,15,25,24,0);
}
