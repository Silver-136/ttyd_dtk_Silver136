#include "effect/n64/eff_kemuri5_n64.h"


#pragma optimize_for_size off

void* effKemuri5N64Entry(f32 x, f32 y, f32 z, f32 angle) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effKemuri5Main(void*);
    extern s32 rand(void);
    extern f64 reviseAngle(f64);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_Kemuri5N64_802fb24c[];
    extern f32 float_0_804256d8;
    extern f32 float_6p2832_804256e4;
    extern f32 float_360_804256e8;
    extern f32 float_1_80425704;
    extern f32 float_0p03_80425708;
    extern f32 float_1p7_8042570c;
    extern f32 float_0p5_80425710;
    extern f32 float_neg0p02_80425714;
    extern f32 float_5Eneg05_80425718;
    extern f32 float_neg3p9_8042571c;
    void* entry;
    u8* part;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri5N64_802fb24c;
    *(s32*)((s32)entry + 8) = 8;
    part = __memAlloc(3, 0x260);
    *(u8**)((s32)entry + 0xC) = part;
    *(void**)((s32)entry + 0x10) = effKemuri5Main;
    for (i = 0; i < 8; i++) {
        *(s32*)part = 1;
        *(u8*)(part + 8) = 0xFF;
        *(s16*)(part + 6) = 0x1E;
        *(f32*)(part + 0xC) = x;
        *(f32*)(part + 0x10) = y;
        *(f32*)(part + 0x14) = z;
        *(f32*)(part + 0x18) = float_1_80425704;
        *(f32*)(part + 0x1C) = float_1_80425704;
        *(f32*)(part + 0x20) = float_1_80425704;
        *(f32*)(part + 0x24) = float_0p03_80425708 * (f32)(rand() % 11) + float_1_80425704;
        *(f32*)(part + 0x28) = float_0p03_80425708 * (f32)(rand() % 11) + float_1p7_8042570c;
        *(f32*)(part + 0x2C) = (f32)(rand() % 61);
        *(f32*)(part + 0x30) = float_neg3p9_8042571c;
        *(f32*)(part + 0x34) = float_0p5_80425710;
        *(f32*)(part + 0x38) = float_neg0p02_80425714;
        *(f32*)(part + 0x3C) = float_5Eneg05_80425718;
        *(f32*)(part + 0x40) = float_0_804256d8;
        *(f32*)(part + 0x44) = (f32)sin(reviseAngle((f64)(angle + i * 45)) * float_6p2832_804256e4 / float_360_804256e8);
        *(f32*)(part + 0x48) = (f32)cos(reviseAngle((f64)(angle + i * 45)) * float_6p2832_804256e4 / float_360_804256e8);
        part += 0x4C;
    }
    return entry;
}

void effKemuri5Main(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern f32 reviseAngle(f32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKemuri5Disp(s32, s32);
    extern Vec3 vec3_802fb240;
    extern f32 float_12_804256e0;
    extern f32 float_6p2832_804256e4;
    extern f32 float_360_804256e8;
    extern f32 float_0p1_804256ec;
    extern f32 float_0p83_804256f0;
    extern f32 float_0p006_804256f4;
    extern f32 float_7p5_804256f8;
    extern f32 float_0p98_804256fc;
    extern f32 float_0p9_80425700;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 alive;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb240;
    pos.x = *(f32*)(work + 0xC);
    pos.y = *(f32*)(work + 0x10);
    pos.z = *(f32*)(work + 0x14);
    dispPos = pos;
    alive = 0;

    for (i = 0; i < *(s32*)((s32)effect + 8); i++) {
        if (*(s32*)work != 0) {
            *(s16*)(work + 6) -= 1;
            if (*(s16*)(work + 6) <= 0) {
                *(s32*)work = 0;
            } else {
                alive = 1;
                *(f32*)(work + 0x2C) = reviseAngle(float_12_804256e0 + *(f32*)(work + 0x2C));
                *(f32*)(work + 0x18) = *(f32*)(work + 0x24) +
                    float_0p1_804256ec *
                    (f32)sin(float_6p2832_804256e4 * *(f32*)(work + 0x2C) / float_360_804256e8);
                *(f32*)(work + 0x1C) = *(f32*)(work + 0x28) +
                    float_0p1_804256ec *
                    (f32)cos(float_6p2832_804256e4 * *(f32*)(work + 0x2C) / float_360_804256e8);
                *(f32*)(work + 0x30) *= float_0p83_804256f0;
                *(f32*)(work + 0xC) += *(f32*)(work + 0x30) * *(f32*)(work + 0x44);
                *(f32*)(work + 0x14) += *(f32*)(work + 0x30) * *(f32*)(work + 0x48);
                *(f32*)(work + 0x38) += *(f32*)(work + 0x3C);
                *(f32*)(work + 0x34) += *(f32*)(work + 0x38);
                *(f32*)(work + 0x10) += *(f32*)(work + 0x34);
                *(f32*)(work + 0x24) += float_0p006_804256f4 * (float_7p5_804256f8 - *(f32*)(work + 0x24));
                *(f32*)(work + 0x28) *= float_0p98_804256fc;
                if (*(s16*)(work + 6) < 10) {
                    *(f32*)(work + 0x28) *= float_0p9_80425700;
                }
                if (*(s16*)(work + 6) < 15) {
                    *(u8*)(work + 8) -= 0x10;
                }
            }
        }
        work += 0x4C;
    }

    if (alive == 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effKemuri5Disp, effect, dispCalcZ(&dispPos));
    }
}

#pragma optimize_for_size on

void effKemuri5Disp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXSetTevColor(s32, void*);
    extern void PSMTXTrans(Mtx, f64, f64, f64);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_0p0625_804256d4;
    extern f32 float_0_804256d8;
    extern f32 float_deg2rad_804256dc;
    u8 tex[0x20];
    Mtx texMtx;
    Mtx trans;
    Mtx rotate;
    u8* entry = (u8*)effect;
    u8* work = *(u8**)(entry + 0xC);
    u8* camera = (u8*)camGetPtr(cameraId);
    s32 i;

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_0p0625_804256d4, float_0p0625_804256d4, float_0_804256d8);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 8, 15, 15, 2);
    GXSetTevAlphaIn(0, 7, 4, 5, 7);
    effGetTexObjN64(0x4E, tex);
    GXLoadTexObj(tex, 0);
    GXSetCullMode(0);
    effSetVtxDescN64((void*)0x803A4D20);
    for (i = 0; i < *(s32*)(entry + 8); i++, work += 0x4C) {
        if (*(s32*)work != 0) {
            u8* camera3d;
            u32 color = 0xFFFFFF00 | *(u8*)(work + 8);
            GXSetTevColor(1, &color);
            PSMTXTrans(trans, *(f32*)(work + 0xC), *(f32*)(work + 0x10), *(f32*)(work + 0x14));
            camera3d = (u8*)camGetPtr(4);
            PSMTXRotRad(rotate, float_deg2rad_804256dc * -*(f32*)(camera3d + 0x114), 'y');
            PSMTXConcat(trans, rotate, trans);
            PSMTXScale(texMtx, *(f32*)(work + 0x18), *(f32*)(work + 0x1C), *(f32*)(work + 0x20));
            PSMTXConcat(trans, texMtx, trans);
            PSMTXConcat((f32 (*)[4])(camera + 0x11C), trans, trans);
            GXLoadPosMtxImm(trans, 0);
            GXBegin(0x90, 0, 6);
            tri2(0, 3, 1, 0, 0, 2, 3);
        }
    }
}

