#include "effect/n64/eff_kemuri4_n64.h"

#pragma optimize_for_size off

void* effKemuri4N64Entry(s32 type, f32 x, f32 y, f32 z, f32 angle, f32 side) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKemuri4Main(void*);
    extern s32 rand(void);
    extern f32 reviseAngle(f32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_Kemuri4N64_802fb22c[];
    extern f32 float_0_804256a8;
    extern f32 float_1_804256b0;
    extern f32 float_5p4_804256b4;
    extern f32 float_0p5_804256b8;
    extern f32 float_neg0p07_804256bc;
    extern f32 float_0p003_804256c0;
    extern f32 float_6p2832_804256c4;
    extern f32 float_360_804256c8;
    void* entry;
    u8* work;
    f32 rad;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri4N64_802fb22c;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x88);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri4Main;

    *(s32*)work = 1;
    *(s16*)(work + 4) = (s16)type;
    *(f32*)(work + 0x6C) = float_0_804256a8;
    *(f32*)(work + 0xC) = x;
    *(f32*)(work + 0x10) = y;
    *(f32*)(work + 0x14) = z;
    *(f32*)(work + 0x18) = float_1_804256b0;
    *(f32*)(work + 0x1C) = float_1_804256b0;
    *(f32*)(work + 0x20) = float_1_804256b0;
    *(u8*)(work + 8) = 0xFF;
    *(f32*)(work + 0x68) = float_1_804256b0;
    *(f32*)(work + 0x64) = float_5p4_804256b4;
    *(f32*)(work + 0x60) = side != float_0_804256a8 ? -10.0f : 10.0f;
    *(f32*)(work + 0x70) = float_0p5_804256b8;
    *(f32*)(work + 0x74) = float_neg0p07_804256bc;
    *(f32*)(work + 0x78) = float_0p003_804256c0;
    *(f32*)(work + 0x7C) = float_0_804256a8;
    *(s16*)(work + 6) = 0xF;
    *(f32*)(work + 0x28) = angle;
    *(f32*)(work + 0x24) = float_0_804256a8;
    *(f32*)(work + 0x2C) = float_0_804256a8;
    *(u8*)(work + 9) = (u8)(((rand() % 100) >> 4) & 1);
    rad = float_6p2832_804256c4 * reviseAngle(angle + (side != float_0_804256a8 ? -90.0f : 90.0f)) / float_360_804256c8;
    *(f32*)(work + 0x80) = (f32)sin(rad);
    *(f32*)(work + 0x84) = (f32)cos(rad);

    return entry;
}

#pragma optimize_for_size on



void effKemuri4Main(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void* marioGetPtr(void);
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKemuri4Disp(void);
    extern void PSMTXRotRad(f32 m[3][4], char axis, f32 rad);
    extern void PSMTXTrans(f32 m[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 m[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 ab[3][4]);
    extern Vec3 vec3_802fb220[];
    extern f32 float_0_804256a8;
    extern f32 float_0p004_804256c0;
    extern f32 float_1_804256b0;
    extern f32 float_neg100_804256cc;
    extern f32 float_deg2rad_804256ac;
    u8* work;
    Vec3* base;
    Vec3 pos;
    Vec3 dispPos;
    f32 mScale[3][4];
    f32 mTrans[3][4];
    f32 mRotX[3][4];
    f32 mRotY[3][4];
    f32 mRotZ[3][4];
    s32 alive;
    s32 i;
    s16 timer;

    i = 0;
    alive = 0;
    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fb220;
    pos = *base;
    pos.x = *(f32*)(work + 0xC);
    pos.y = *(f32*)(work + 0x10);
    pos.z = *(f32*)(work + 0x14);
    dispPos = pos;

    while (i < *(s32*)((s32)effect + 8)) {
        if (*(s32*)work != 0) {
            timer = *(s16*)(work + 6) - 1;
            *(s16*)(work + 6) = timer;
            if (timer <= 0) {
                *(s32*)work = 0;
            } else {
                alive = 1;
                if (*(u16*)(work + 4) == 0 &&
                    *(f32*)(work + 0x68) != float_0_804256a8 &&
                    *(u16*)((s32)marioGetPtr() + 0x2E) == 0) {
                    *(s16*)(work + 6) += 1;
                } else {
                    *(f32*)(work + 0x78) += *(f32*)(work + 0x7C);
                    *(f32*)(work + 0x74) += *(f32*)(work + 0x78);
                    *(f32*)(work + 0x70) += *(f32*)(work + 0x74);
                    *(f32*)(work + 0x2C) += *(f32*)(work + 0x70) * *(f32*)(work + 0x60);
                    *(f32*)(work + 0xC) -= *(f32*)(work + 0x80) * (*(f32*)(work + 0x70) * *(f32*)(work + 0x64));
                    *(f32*)(work + 0x14) -= *(f32*)(work + 0x84) * (*(f32*)(work + 0x70) * *(f32*)(work + 0x64));
                    *(u8*)(work + 8) -= 0xC;
                    if (*(f32*)(work + 0x70) < float_0_804256a8) {
                        *(f32*)(work + 0x68) = float_0_804256a8;
                    }
                }

                PSMTXRotRad(mRotX, 'x', float_deg2rad_804256ac * *(f32*)(work + 0x24));
                PSMTXRotRad(mRotY, 'y', float_deg2rad_804256ac * *(f32*)(work + 0x28));
                PSMTXRotRad(mRotZ, 'z', float_deg2rad_804256ac * *(f32*)(work + 0x2C));
                PSMTXTrans(mTrans, *(f32*)(work + 0xC), *(f32*)(work + 0x10), *(f32*)(work + 0x14));
                PSMTXScale(mScale, *(f32*)(work + 0x18), *(f32*)(work + 0x1C), *(f32*)(work + 0x20));
                PSMTXConcat(mRotX, mRotZ, mRotZ);
                PSMTXConcat(mRotY, mRotZ, mRotZ);
                PSMTXConcat(mRotZ, mScale, mScale);
                PSMTXConcat(mTrans, mScale, (f32 (*)[4])(work + 0x30));
            }
        }
        i++;
        work += 0x88;
    }

    if (alive != 0) {
        dispEntry(4, 1, effKemuri4Disp, effect, dispCalcZ(&dispPos));
    } else {
        effDelete(effect);
    }
}

void effKemuri4Disp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void GXSetCullMode(s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetTevColor(s32, void*);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern f32 float_0p0625_804256a4;
    extern f32 float_0_804256a8;
    u8 tex[0x20];
    Mtx texMtx;
    Mtx draw;
    u8* entry = (u8*)effect;
    u8* work = *(u8**)(entry + 0xC);
    u8* camera = (u8*)camGetPtr(cameraId);
    s32 i;

    GXSetCullMode(0);
    for (i = 0; i < *(s32*)(entry + 8); i++, work += 0x88) {
        if (*(s32*)work != 0) {
            u32 color = 0xFFFFFFFF;
            effGetTexObjN64(*(u8*)(work + 9) == 0 ? 0x4D : 0x4C, tex);
            GXLoadTexObj(tex, 0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
            PSMTXScale(texMtx, float_0p0625_804256a4, float_0p0625_804256a4, float_0_804256a8);
            GXLoadTexMtxImm(texMtx, 0x1E, 1);
            GXSetNumChans(0);
            GXSetNumTevStages(1);
            GXSetTevOrder(0, 0, 0, 0xFF);
            GXSetTevColorOp(0, 0, 0, 0, 1, 0);
            GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
            GXSetTevColorIn(0, 15, 8, 2, 15);
            GXSetTevAlphaIn(0, 7, 4, 5, 7);
            PSMTXConcat((f32 (*)[4])(camera + 0x11C), (f32 (*)[4])(work + 0x30), draw);
            GXLoadPosMtxImm(draw, 0);
            GXSetTevColor(1, &color);
            effSetVtxDescN64((void*)0x803A4C3E);
            GXBegin(0x90, 0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3);
            GXSetNumChans(1);
            GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
            GXSetNumTexGens(0);
            GXSetNumTevStages(1);
            GXSetTevOrder(0, 0xFF, 0xFF, 4);
            GXSetTevOp(0, 4);
            effSetVtxDescN64((void*)0x803A4CCA);
            GXBegin(0x90, 0, 6);
            tri2(0, 1, 2, 0, 3, 4, 5);
        }
    }
}
