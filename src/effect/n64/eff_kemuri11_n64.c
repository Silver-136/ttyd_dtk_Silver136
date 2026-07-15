#include "effect/n64/eff_kemuri11_n64.h"


u8 effKemuri11Disp(int param_1, int param_2) {
    return 0;
}


void effKemuri11Main(void* effect) {
    typedef struct Vec3 { f32 x; f32 y; f32 z; } Vec3;
    extern void effDelete(void*);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effKemuri11Disp(s32, s32);
    extern Vec3 vec3_802fb328[];
    extern f32 float_0p8_80425880;
    extern f32 float_0p94_80425884;
    extern f32 float_6p2832_80425888;
    extern f32 float_360_8042588c;
    extern f32 float_0p5_80425890;
    extern f32 float_32_80425894;
    extern f32 float_0p9_804258a0;
    u8* work;
    u8* part;
    Vec3 pos;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb328[0];
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    *(s32*)(work + 0x28) += 1;
    *(s32*)(work + 0x24) -= 1;
    if (*(s32*)(work + 0x24) < 0) { effDelete(effect); return; }
    if (*(s32*)(work + 0x28) > 7) {
        if (*(u16*)work < 2) *(s32*)(work + 0x38) = (s32)((f32)*(s32*)(work + 0x38) * float_0p8_80425880);
        else *(s32*)(work + 0x38) = (s32)((f32)*(s32*)(work + 0x38) * float_0p94_80425884);
    }
    part = work + 0x58;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        *(s32*)(part + 0xA4) += 1;
        if (*(s32*)(part + 0xA4) == 0) {
            *(f32*)(part + 0x5C) = 0.0f;
            *(f32*)(part + 0x60) = 0.0f;
            *(f32*)(part + 0x64) = 0.0f;
            *(f32*)(part + 0x68) = -float_0p5_80425890 * (f32)sin((f64)(float_6p2832_80425888 * *(f32*)(part + 0x8C) / float_360_8042588c));
            *(f32*)(part + 0x6C) = float_0p5_80425890 * (f32)cos((f64)(float_6p2832_80425888 * *(f32*)(part + 0x8C) / float_360_8042588c));
            *(f32*)(part + 0x94) = float_32_80425894;
            *(f32*)(part + 0x98) = float_32_80425894;
            *(f32*)(part + 0xA0) = float_32_80425894;
        }
        *(f32*)(part + 0x94) += *(f32*)(part + 0x98);
        *(f32*)(part + 0x9C) += *(f32*)(part + 0xA0);
        *(f32*)(part + 0x98) *= float_0p9_804258a0;
        part += 0x58;
    }
    dispEntry(4, 2, effKemuri11Disp, effect, dispCalcZ(&pos));
}

void* effKemuri11N64Entry(f32 x, f32 y, f32 z, s32 type) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effKemuri11Main(void*);
    extern void* effKemuri1N64Entry(f32, f32, f32, f32, s32);
    extern char str_Kemuri11N64_802fb340[];
    void* entry;
    u8* work;
    u8* part;
    s32 kind;
    s32 count;
    s32 i;

    entry = effEntry();
    if (type == 3) kind = 5;
    else if (type == 1) kind = 2;
    else if (type == 0) kind = 3;
    else if (type == 2) kind = 4;
    else if (type == 5) kind = 7;
    else if (type == 4) kind = 6;
    else kind = 5;
    count = (kind == 2 || kind == 5) ? 3 : 2;
    *(char**)((s32)entry + 0x14) = str_Kemuri11N64_802fb340;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * 0x58);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri11Main;
    *(s32*)(work + 0x28) = kind > 1 ? 0x3C : 0x1E;
    *(s16*)(work + 0x2C) = 0;
    *(s16*)(work + 0x2E) = 0;
    *(s16*)work = kind;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s16*)(work + 0x38) = 0;
    *(s16*)(work + 0x3A) = 0xFF;
    part = work + 0x58;
    for (i = 1; i < count + 1; i++) {
        *(s32*)(part + 0xA4) = kind < 2 ? -((i - 1) * 2) - 1 : -((i - 1) * 3) - 1;
        *(f32*)(part + 0x8C) = (f32)(i * 0x1E);
        *(f32*)(part + 0x88) = 0.0f;
        part += 0x58;
    }
    if (kind == 6) effKemuri1N64Entry(x, y, z, 0.0f, 5);
    else if (kind == 7) effKemuri1N64Entry(x, y, z, 0.0f, 6);
    return entry;
}

void main_dl(void* effect, void* view) {
    typedef f32 Mtx[3][4];
    extern void PSMTXTrans(Mtx, f64, f64, f64);
    extern void PSMTXRotRad(Mtx, f64, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(u32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s16, s16, s16, s32, s16, s16, s16);
    extern f32 float_deg2rad_80425860;
    extern f32 float_0p3_80425864;
    extern f32 float_0p015625_80425868;
    extern f32 float_0p03125_8042586c;
    extern f32 float_1_80425870;
    extern f32 float_0_80425874;
    extern f32 float_0p25_80425878;
    extern f32 float_0p125_8042587c;
    u8* part;
    Mtx trans, rot, scale;
    s32 i;

    part = *(u8**)((s32)effect + 0xC) + 0x58;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        if (*(s32*)(part + 0x4C) >= 0) {
            PSMTXTrans(trans, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0x64));
            PSMTXRotRad(rot, float_deg2rad_80425860 * *(f32*)(part + 0x8C), 'z');
            PSMTXConcat(trans, rot, trans);
            PSMTXScale(scale, float_0p3_80425864 * *(f32*)(part + 0x74), float_0p3_80425864 * *(f32*)(part + 0x78), float_0p3_80425864 * *(f32*)(part + 0x7C));
            PSMTXConcat(trans, scale, trans);
            PSMTXConcat(view, trans, trans);
            GXLoadPosMtxImm(trans, 0);
            GXSetCurrentMtx(0);
            PSMTXScale(scale, float_0p015625_80425868, float_0p03125_8042586c, float_1_80425870);
            PSMTXTrans(trans, float_0_80425874, -*(f32*)(part + 0x94) * float_0p25_80425878, float_0_80425874);
            PSMTXConcat(scale, trans, scale);
            GXLoadTexMtxImm(scale, 0x1E, 1);
            PSMTXScale(scale, float_0p125_8042587c, float_0p125_8042587c, float_1_80425870);
            PSMTXTrans(trans, float_0_80425874, -*(f32*)(part + 0x9C), float_0_80425874);
            PSMTXConcat(scale, trans, scale);
            GXLoadTexMtxImm(scale, 0x21, 1);
            GXBegin(0x90, 0, 0x36);
            tri2(0, 1, 2, 0, 2, 3, 4);
            tri2(2, 4, 5, 0, 1, 3, 2);
            tri2(4, 6, 5, 0, 5, 6, 7);
        }
        part += 0x58;
    }
}

