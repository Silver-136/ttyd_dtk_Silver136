#include "effect/n64/eff_hokori_n64.h"


void effHokoriDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void PSMTXTrans(Mtx, f64, f64, f64);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXSetCullMode(s32);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_0p03125_80425480;
    extern f32 float_0p010417_80425484;
    extern f32 float_0_80425488;
    extern f32 float_deg2rad_8042548c;
    extern u8 size16x16_tex32x32_vtx[];
    extern u8 size16x32_tex32x64_vtx[];
    extern u8 size8x8_tex32x32_vtx[];
    u8 texObj[0x20];
    Mtx base;
    Mtx rotate;
    Mtx texMtx;
    Mtx draw;
    u8* entry = (u8*)effect;
    u8* work = *(u8**)(entry + 0xC);
    u8* part;
    u8* camera = (u8*)camGetPtr(cameraId);
    u8* camera3d;
    u32 color0;
    u32 color1;
    s32 type = *(s32*)work;
    s32 i;

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_0p03125_80425480, float_0p010417_80425484, float_0_80425488);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    color0 = ((u32)*(u8*)(work + 0x30) << 24) | ((u32)*(u8*)(work + 0x34) << 16) |
             ((u32)*(u8*)(work + 0x38) << 8) | *(u8*)(work + 0x18);
    color1 = ((u32)*(u8*)(work + 0x3C) << 24) | ((u32)*(u8*)(work + 0x40) << 16) |
             ((u32)*(u8*)(work + 0x44) << 8) | 0xFF;
    GXSetTevColor(1, &color0);
    GXSetTevColor(2, &color1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    if (type == 0) {
        GXSetTevColorIn(0, 15, 15, 15, 2);
        GXSetTevAlphaIn(0, 7, 5, 4, 7);
        effGetTexObjN64(0x15, texObj);
    } else {
        GXSetTevColorIn(0, 2, 12, 8, 15);
        GXSetTevAlphaIn(0, 7, 5, 4, 7);
        effGetTexObjN64(0x16, texObj);
    }
    GXLoadTexObj(texObj, 0);
    PSMTXTrans(base, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    camera3d = (u8*)camGetPtr(4);
    PSMTXRotRad(rotate, float_deg2rad_8042548c * -*(f32*)(camera3d + 0x114), 'y');
    PSMTXConcat(base, rotate, draw);
    PSMTXConcat((f32 (*)[4])(camera + 0x11C), draw, draw);
    GXSetCullMode(0);
    part = work + 0x48;
    for (i = 1; i < *(s32*)(entry + 8); i++, part += 0x48) {
        void* vtx;
        PSMTXTrans(base, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0xC));
        PSMTXScale(texMtx, *(f32*)(part + 0x28), *(f32*)(part + 0x24), *(f32*)(part + 0x28));
        PSMTXConcat(base, texMtx, base);
        PSMTXConcat(draw, base, base);
        GXLoadPosMtxImm(base, 0);
        GXSetCurrentMtx(0);
        vtx = type < 2 ? ((i & 1) ? size16x32_tex32x64_vtx : size16x16_tex32x32_vtx) : size8x8_tex32x32_vtx;
        effSetVtxDescN64(vtx);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    }
}

#pragma optimize_for_size off

void* effHokoriN64Entry(f32 x, f32 y, f32 z, s32 type, s32 duration) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effHokoriMain(void* effect);
    extern s32 rand(void);
    extern char str_HokoriN64_802fb0e8[];
    extern f32 float_0_80425488;
    extern f32 float_neg1_80425498;
    extern f32 float_1_8042549c;
    extern f32 float_200_804254a0;
    extern f32 float_0p1_804254a4;
    extern f32 float_0p2_804254a8;
    extern f32 float_0p4_804254ac;
    void* entry;
    u8* work;
    u8* part;
    s32 count;
    s32 i;

    entry = effEntry();
    count = type < 2 ? 5 : 4;
    *(char**)((s32)entry + 0x14) = str_HokoriN64_802fb0e8;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, count * 0x48);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effHokoriMain;

    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x2C) = duration;
    *(s32*)(work + 0x18) = 0xFF;
    *(s32*)(work + 0x30) = 0xA0;
    *(s32*)(work + 0x34) = 0xA0;
    *(s32*)(work + 0x38) = 0x8C;
    *(s32*)(work + 0x3C) = 0xA0;
    *(s32*)(work + 0x40) = 0xA0;
    *(s32*)(work + 0x44) = 0x8C;

    part = work + 0x48;
    for (i = 1; i < count; i++) {
        *(f32*)(part + 4) = (f32)(rand() % 200 - 100);
        *(f32*)(part + 8) = (f32)(rand() % 50);
        *(f32*)(part + 0xC) = float_0_80425488;
        *(f32*)(part + 0x10) = float_neg1_80425498;
        *(f32*)(part + 0x14) = float_0_80425488;
        *(f32*)(part + 0x1C) = float_1_8042549c;
        *(f32*)(part + 0x20) = float_1_8042549c;
        *(f32*)(part + 0x24) = (f32)(rand() % 10) / float_200_804254a0;

        if (type == 1) {
            *(f32*)(part + 0x28) = float_0p2_804254a8 * *(f32*)(part + 0x24);
        } else if (type == 0) {
            *(f32*)(part + 0x28) = float_0p1_804254a4 * *(f32*)(part + 0x24);
        } else {
            *(f32*)(part + 0x24) = float_0p4_804254ac;
            *(f32*)(part + 0x28) = float_0p4_804254ac;
            *(f32*)(part + 0x10) = float_0_80425488;
            *(f32*)(part + 4) = float_0p1_804254a4 * (f32)(rand() % 40 - 20);
            *(f32*)(part + 8) = float_0p1_804254a4 * (f32)(rand() % 40 - 20);
        }
        part += 0x48;
    }
    return entry;
}

#pragma optimize_for_size on

void effHokoriMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effHokoriDisp(s32, s32);
    extern Vec3 vec3_802fb0d0[];
    extern f32 float_0p9_80425490;
    extern f32 float_0p005_80425494;
    u8* work;
    u8* part;
    Vec3* base;
    Vec3 dispPos;
    Vec3 pos;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fb0d0;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    *(s32*)(work + 0x2C) -= 1;
    if (*(s32*)(work + 0x2C) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x2C) < 0x18) {
        *(s32*)(work + 0x18) = (s32)((f32)*(s32*)(work + 0x18) * float_0p9_80425490);
    }

    part = work + 0x48;
    if (*(s32*)work <= 1) {
        for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
            *(f32*)(part + 0x10) += *(f32*)(part + 0x14);
            *(f32*)(part + 8) += *(f32*)(part + 0x10);
            *(f32*)(part + 0x24) += float_0p005_80425494;
            *(f32*)(part + 0x1C) += *(f32*)(part + 0x24);
            *(f32*)(part + 0x20) += *(f32*)(part + 0x28);
            part += 0x48;
        }
    } else {
        for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
            *(f32*)(part + 0x10) += *(f32*)(part + 0x14);
            *(f32*)(part + 8) += *(f32*)(part + 0x10);
            *(f32*)(part + 0x1C) += *(f32*)(part + 0x24);
            *(f32*)(part + 0x20) += *(f32*)(part + 0x28);
            part += 0x48;
        }
    }

    dispEntry(4, 2, effHokoriDisp, effect, dispCalcZ(&dispPos));
}

