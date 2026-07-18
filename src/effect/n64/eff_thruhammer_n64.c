#include "effect/n64/eff_thruhammer_n64.h"


void main_dl(void* effect, void* view) {
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    u8* work = *(u8**)((s32)effect + 0xC);
    f32 trans[3][4], rot[3][4], scale[3][4];
    s32 i;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        u8* part = work + i * 0x58;
        if (*(s32*)(part + 0x4C) >= 0) {
            PSMTXTrans(trans, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0xC));
            PSMTXRotRad(rot, 0x7A, 0.017453292f * *(f32*)(part + 0x34));
            PSMTXConcat(trans, rot, trans);
            PSMTXScale(scale, 0.3f * *(f32*)(part + 0x1C), 0.3f * *(f32*)(part + 0x20), 0.3f * *(f32*)(part + 0x24));
            PSMTXConcat(trans, scale, trans); PSMTXConcat(view, trans, trans);
            GXLoadPosMtxImm(trans, 0); GXSetCurrentMtx(0);
            PSMTXScale(scale, 0.015873f, 0.032258f, 0.0f); GXLoadTexMtxImm(scale, 0x1E, 1);
            GXBegin(0x90, 0, 0x36); tri2(0,1,2,0,2,3,4,0); tri2(2,4,5,0,1,3,2,0);
        }
    }
}

void effThruHammerDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void effSetVtxDescN64(void*);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetCullMode(s32);
    extern void main_dl(void*, void*);
    extern f32 float_deg2rad_80426360;
    extern u8 unk_80429844;
    extern u8 unk_8042984c;

    f32 trans[3][4];
    f32 rot[3][4];
    f32 view[3][4];
    u8 texObj[0x20];
    u8 color0[4];
    u8 color1[4];
    u8* work = *(u8**)((s32)effect + 0xC);
    void* camera = camGetPtr(cameraId);

    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0, 0, 0, 0);
    GXSetTevAlphaIn(0, 0, 1, 4, 7);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 2, 1, 8, 0);
    GXSetTevAlphaIn(1, 0, 0, 0, 0);
    effGetTexObjN64(0x23, texObj);
    GXLoadTexObj(texObj, 0);
    effGetTexObjN64(0x24, texObj);
    GXLoadTexObj(texObj, 1);
    effSetVtxDescN64((void*)0x803A9568);

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, 0x79,
                float_deg2rad_80426360 * -*(f32*)((s32)camGetPtr(4) + 0x114));
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat((void*)((s32)camera + 0x11C), trans, view);

    color0[0] = *(u8*)(work + 0x50);
    color0[1] = *(u8*)(work + 0x51);
    color0[2] = *(u8*)(work + 0x52);
    color0[3] = *(s32*)(work + 0x38);
    GXSetTevColor(1, color0);
    color1[0] = *(u8*)(work + 0x53);
    color1[1] = *(u8*)(work + 0x54);
    color1[2] = *(u8*)(work + 0x55);
    color1[3] = unk_80429844;
    GXSetTevColor(2, color1);
    GXSetCullMode(2);
    main_dl(effect, view);

    color0[0] = *(u8*)(work + 0x50);
    color0[1] = *(u8*)(work + 0x51);
    color0[2] = *(u8*)(work + 0x52);
    color0[3] = *(s32*)(work + 0x38);
    GXSetTevColor(1, color0);
    color1[0] = *(u8*)(work + 0x53);
    color1[1] = *(u8*)(work + 0x54);
    color1[2] = *(u8*)(work + 0x55);
    color1[3] = unk_8042984c;
    GXSetTevColor(2, color1);
    GXSetCullMode(1);
    main_dl(effect, view);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effThruHammerN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effThruHammerMain(void*);
    extern char str_ThruHammerN64_802fc180[];
    void* entry;
    u8* work;
    u8* part;
    s32 count;
    s32 i;

    entry = effEntry();
    if (type == 1) {
        goto count_one;
    }
    if (type >= 1) {
        goto count_one;
    }
    if (type >= 0) {
        goto count_four;
    }
    goto count_one;
count_four:
    count = 4;
    goto count_done;
count_one:
    count = 1;
count_done:
    *(char**)((s32)entry + 0x14) = str_ThruHammerN64_802fc180;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * 0x58);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effThruHammerMain;
    *(s32*)(work + 0x28) = 0x3C;
    *(s32*)(work + 0x2C) = 0;
    *(s16*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x38) = 0xFF;
    *(u8*)(work + 0x50) = 0;
    *(u8*)(work + 0x51) = 0xFF;
    *(u8*)(work + 0x52) = 0x7A;
    *(u8*)(work + 0x53) = 0xF0;
    *(u8*)(work + 0x54) = 0xFF;
    *(u8*)(work + 0x55) = 0xFA;
    part = work + 0x58;
    for (i = 1; i < count + 1; i++, part += 0x58) {
        *(s32*)(part + 0x4C) = -1 - ((i - 1) << 2);
        *(f32*)(part + 0x34) = scale;
        *(f32*)(part + 0x30) = (f32)(i * 30);
    }
    return entry;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

void effThruHammerMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effThruHammerDisp(void);
    extern Vec3 vec3_802fc168[];
    extern f32 float_0_80426370;
    extern f32 float_0p8_8042637c;
    extern f32 float_6p2832_80426380;
    extern f32 float_360_80426384;
    extern f32 float_0p5_80426388;
    extern f32 float_32_8042638c;
    extern f32 float_10_80426390;
    extern f32 float_0p9_80426394;
    extern f32 float_0p2_80426398;
    extern f32 float_0p1_8042639c;
    extern f32 float_128_804263a0;
    extern double sin(double);
    extern double cos(double);
    u8* work;
    u8* part;
    Vec3 dispPos;
    Vec3 pos;
    Vec3* base;
    s32 i;
    f32 angle;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fc168;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    *(s32*)(work + 0x2C) += 1;
    *(s32*)(work + 0x28) -= 1;
    if (*(s32*)(work + 0x28) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x2C) >= 8) {
        *(s32*)(work + 0x38) = (s32)((f32)*(s32*)(work + 0x38) * float_0p8_8042637c);
    }
    part = work + 0x58;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x58) {
        *(s32*)(part + 0x4C) += 1;
        if (*(s32*)(part + 0x4C) < 0) {
            continue;
        }
        if (*(s32*)(part + 0x4C) == 0) {
            *(f32*)(part + 4) = float_0_80426370;
            *(f32*)(part + 8) = float_0_80426370;
            *(f32*)(part + 0xC) = float_0_80426370;
            angle = (*(f32*)(part + 0x34) * float_6p2832_80426380) / float_360_80426384;
            *(f32*)(part + 0x10) = float_0p5_80426388 * -(f32)sin(angle);
            *(f32*)(part + 0x14) = float_0p5_80426388 * (f32)cos(angle);
            *(f32*)(part + 0x18) = float_0_80426370;
            *(f32*)(part + 0x3C) = float_32_8042638c;
            *(f32*)(part + 0x40) = float_32_8042638c;
            *(f32*)(part + 0x48) = float_32_8042638c;
            *(f32*)(part + 0x20) = float_10_80426390;
            *(f32*)(part + 0x1C) = float_0p5_80426388;
            *(f32*)(part + 0x24) = float_0p5_80426388;
            *(f32*)(part + 0x44) = float_32_8042638c;
        }
        *(f32*)(part + 0x3C) += *(f32*)(part + 0x40);
        *(f32*)(part + 0x44) += *(f32*)(part + 0x48);
        *(f32*)(part + 0x40) *= float_0p9_80426394;
        *(f32*)(part + 0x1C) += float_0p2_80426398;
        *(f32*)(part + 0x24) += float_0p2_80426398;
        *(f32*)(part + 0x20) += float_0p2_80426398 * (float_0p1_8042639c - *(f32*)(part + 0x20));
        if (*(f32*)(part + 0x3C) > float_128_804263a0) {
            *(f32*)(part + 0x3C) = float_128_804263a0;
        }
    }
    dispEntry(4, 2, effThruHammerDisp, effect, dispCalcZ(&dispPos));
}

