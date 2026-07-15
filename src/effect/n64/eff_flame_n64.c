#include "effect/n64/eff_flame_n64.h"


u8 effFlameDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXBegin(s32, s32, s16);
    u8* work = *(u8**)(effectAddress + 0xC);
    char* camera = camGetPtr(cameraId);
    Mtx trans, rot, scale, model;
    s32 pass;
    s32 vertex;

    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    GXSetNumTevStages(*(s32*)work == 0 ? 5 : 4);
    for (pass = 0; pass < 4; pass++) {
        GXSetTevOrder(pass, pass & 1, pass & 1, -1);
        GXSetTevColorIn(pass, 15, 8, 10, 15);
        GXSetTevAlphaIn(pass, 7, 4, 5, 7);
    }
    GXSetCullMode(0);
    for (pass = 0; pass < 3; pass++) {
        PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
        PSMTXRotRad(rot, -0.017453292f * *(f32*)(camera + 0x114), 'y');
        PSMTXScale(scale, *(f32*)(work + 0x1C), *(f32*)(work + 0x20), 1.0f);
        PSMTXConcat(trans, rot, model);
        PSMTXConcat(model, scale, model);
        PSMTXConcat(camera + 0x118, model, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x90, 0, 6);
        for (vertex = 0; vertex < 6; vertex++) {
            *(volatile f32*)0xCC008000 = (vertex & 1) ? 32.0f : -32.0f;
            *(volatile f32*)0xCC008000 = (vertex & 2) ? 64.0f : 0.0f;
            *(volatile f32*)0xCC008000 = (f32)pass;
            *(volatile f32*)0xCC008000 = (f32)(vertex & 1);
            *(volatile f32*)0xCC008000 = (f32)((vertex >> 1) & 1);
        }
    }
    return 0;
}

void effFlameMain(void* effect) {
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effFlameDisp(void);
    extern f32 float_0_804252ec;
    extern f32 float_0p5_804252fc;
    extern f32 float_0p02_80425300;
    extern f32 float_0p04_80425304;
    extern f32 float_64_80425308;
    extern f32 float_128_8042530c;
    extern f32 float_0p1_80425310;

    u8* work;
    f32 pos[3];
    s32 timer;
    s32 frame;
    f32 value;

    work = *(u8**)((s32)effect + 0xC);
    pos[0] = *(f32*)(work + 4);
    pos[1] = *(f32*)(work + 8);
    pos[2] = *(f32*)(work + 0xC);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x2C) = 0x10;
    }

    (*(s32*)(work + 0x30))++;
    if (*(s32*)(work + 0x30) > 0x4F1A0) {
        *(s32*)(work + 0x30) = 0x100;
    }
    if (*(s32*)(work + 0x2C) < 0x11) {
        (*(s32*)(work + 0x2C))--;
    }

    timer = *(s32*)(work + 0x2C);
    if (timer < 0) {
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x30);
    if (*(s32*)work < 2) {
        if (frame < 0xB) {
            *(s32*)(work + 0x28) = (s32)(float_0p5_804252fc * (f32)(0x80 - *(s32*)(work + 0x28)) +
                                        (f32)*(s32*)(work + 0x28));
        }
    } else if (frame < 0xB) {
        *(s32*)(work + 0x28) = ((frame * 0xFF) / 10) + (((frame * 0xFF) / 10) >> 0x1F);
        *(s32*)(work + 0x28) -= (*(s32*)(work + 0x28) >> 0x1F);
    }

    if (timer < 0x10) {
        *(s32*)(work + 0x28) = timer << 4;
    }

    if (*(s32*)work == 0) {
        *(f32*)(work + 0x38) += float_0p02_80425300 * (*(f32*)(work + 0x3C) - *(f32*)(work + 0x38));
        *(f32*)(work + 0x44) += float_0p02_80425300 * (*(f32*)(work + 0x48) - *(f32*)(work + 0x44));
        *(f32*)(work + 0x1C) += float_0p04_80425304 * (*(f32*)(work + 0x24) - *(f32*)(work + 0x1C));
        *(f32*)(work + 0x20) += float_0p04_80425304 * (*(f32*)(work + 0x24) - *(f32*)(work + 0x20));
    } else {
        *(f32*)(work + 0x38) += float_0p02_80425300 * (*(f32*)(work + 0x3C) - *(f32*)(work + 0x38));
        *(f32*)(work + 0x44) += float_0p02_80425300 * (*(f32*)(work + 0x48) - *(f32*)(work + 0x44));
        *(f32*)(work + 0x50) += float_0p02_80425300 * (*(f32*)(work + 0x54) - *(f32*)(work + 0x50));
        *(f32*)(work + 0x5C) += float_0p02_80425300 * (*(f32*)(work + 0x60) - *(f32*)(work + 0x5C));
    }

    *(f32*)(work + 0x34) += *(f32*)(work + 0x38);
    value = *(f32*)(work + 0x34);
    if (float_0_804252ec <= value) {
        if (float_64_80425308 < value) {
            *(f32*)(work + 0x34) = value - float_64_80425308;
        }
    } else {
        *(f32*)(work + 0x34) = value + float_64_80425308;
    }

    *(f32*)(work + 0x40) += *(f32*)(work + 0x44);
    value = *(f32*)(work + 0x40);
    if (float_0_804252ec <= value) {
        if (float_128_8042530c < value) {
            *(f32*)(work + 0x40) = value - float_128_8042530c;
        }
    } else {
        *(f32*)(work + 0x40) = value + float_128_8042530c;
    }

    *(f32*)(work + 0x4C) += *(f32*)(work + 0x50);
    value = *(f32*)(work + 0x4C);
    if (float_0_804252ec <= value) {
        if (float_64_80425308 < value) {
            *(f32*)(work + 0x4C) = value - float_64_80425308;
        }
    } else {
        *(f32*)(work + 0x4C) = value + float_64_80425308;
    }

    *(f32*)(work + 0x58) += *(f32*)(work + 0x5C);
    value = *(f32*)(work + 0x58);
    if (float_0_804252ec <= value) {
        if (float_128_8042530c < value) {
            *(f32*)(work + 0x58) = value - float_128_8042530c;
        }
    } else {
        *(f32*)(work + 0x58) = value + float_128_8042530c;
    }

    *(f32*)(work + 0x10) += float_0p1_80425310 * (*(f32*)(work + 4) - *(f32*)(work + 0x10));
    *(f32*)(work + 0x14) += float_0p1_80425310 * (*(f32*)(work + 8) - *(f32*)(work + 0x14));
    *(f32*)(work + 0x18) += float_0p1_80425310 * (*(f32*)(work + 0xC) - *(f32*)(work + 0x18));

    dispEntry(4, 2, effFlameDisp, effect, dispCalcZ(pos));
}

void* effFlameN64Entry(f32 x, f32 y, f32 z, f32 scale, s32 type) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effFlameMain(void* effect);
    extern char str_FlameN64_802fafa0[];
    extern f32 float_0_804252ec;
    extern f32 float_2_80425314;
    extern f32 float_0p6_80425318;
    extern f32 float_neg0p9_8042531c;
    extern f32 float_0p9_80425320;
    extern f32 float_0p04_80425304;
    extern f32 float_0p2_80425324;
    extern f32 float_1p3_80425328;
    extern f32 float_0p16_8042532c;
    extern f32 float_1p04_80425330;
    extern f32 float_0p1_80425310;
    extern f32 float_1p7_80425334;
    extern f32 float_0p08_80425338;
    extern f32 float_1p36_8042533c;

    void* entry;
    u8* work;
    f32 zero;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_FlameN64_802fafa0;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0xE0);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFlameMain;
    *(u32*)entry |= 2;

    zero = float_0_804252ec;
    *(s32*)work = type;
    *(s32*)(work + 0x2C) = 100;
    *(s32*)(work + 0x30) = 0;
    *(s32*)(work + 0x28) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x64) = zero;
    *(f32*)(work + 0x40) = zero;
    *(f32*)(work + 0x34) = zero;
    *(f32*)(work + 0x58) = zero;
    *(f32*)(work + 0x4C) = zero;

    if (type == 1) {
        *(u8*)(work + 0x68) = 0x4B;
        *(u8*)(work + 0x69) = 0x4B;
        *(u8*)(work + 0x6A) = 0x4B;
        *(u8*)(work + 0x6B) = 0xFF;
        *(u8*)(work + 0x6C) = 0;
        *(u8*)(work + 0x6D) = 0;
        *(u8*)(work + 0x6E) = 0;
        *(f32*)(work + 0x1C) = scale;
        *(f32*)(work + 0x20) = scale;
        *(f32*)(work + 0x24) = scale;
        *(f32*)(work + 0x38) = zero;
        *(f32*)(work + 0x44) = float_0p04_80425304;
        *(f32*)(work + 0x3C) = float_0p04_80425304;
        *(f32*)(work + 0x48) = float_0p9_80425320;
        *(f32*)(work + 0x50) = zero;
        *(f32*)(work + 0x5C) = float_0p04_80425304;
        *(f32*)(work + 0x54) = float_0p04_80425304;
        *(f32*)(work + 0x60) = float_0p9_80425320;
    } else if (type < 1) {
        if (type > -1) {
            *(u8*)(work + 0x68) = 0x4B;
            *(u8*)(work + 0x69) = 0x4B;
            *(u8*)(work + 0x6A) = 0x4B;
            *(u8*)(work + 0x6B) = 0xFF;
            *(u8*)(work + 0x6C) = 0xF7;
            *(u8*)(work + 0x6D) = 0x9B;
            *(u8*)(work + 0x6E) = 0;
            *(f32*)(work + 0x1C) = float_2_80425314 * scale;
            *(f32*)(work + 0x20) = float_0p6_80425318 * scale;
            *(f32*)(work + 0x24) = scale;
            *(f32*)(work + 0x38) = float_neg0p9_8042531c;
            *(f32*)(work + 0x44) = float_0p04_80425304;
            *(f32*)(work + 0x3C) = float_0p04_80425304;
            *(f32*)(work + 0x48) = float_0p9_80425320;
            *(f32*)(work + 0x50) = float_neg0p9_8042531c;
            *(f32*)(work + 0x5C) = float_0p04_80425304;
            *(f32*)(work + 0x54) = float_0p04_80425304;
            *(f32*)(work + 0x60) = float_0p9_80425320;
        }
    } else if (type < 3) {
        *(u8*)(work + 0x68) = 0x19;
        *(u8*)(work + 0x69) = 0x79;
        *(u8*)(work + 0x6A) = 0x8E;
        *(u8*)(work + 0x6B) = 0x22;
        *(u8*)(work + 0x6C) = 0;
        *(u8*)(work + 0x6D) = 0xFF;
        *(u8*)(work + 0x6E) = 0x51;
        *(f32*)(work + 0x1C) = scale;
        *(f32*)(work + 0x20) = scale;
        *(f32*)(work + 0x24) = scale;
        *(f32*)(work + 0x38) = zero;
        *(f32*)(work + 0x44) = float_0p04_80425304;
        *(f32*)(work + 0x3C) = float_0p2_80425324;
        *(f32*)(work + 0x48) = float_1p3_80425328;
        *(f32*)(work + 0x50) = zero;
        *(f32*)(work + 0x5C) = float_0p04_80425304;
        *(f32*)(work + 0x54) = float_0p16_8042532c;
        *(f32*)(work + 0x60) = float_1p04_80425330;
    } else {
        *(u8*)(work + 0x68) = 0xB3;
        *(u8*)(work + 0x69) = 0xA9;
        *(u8*)(work + 0x6A) = 0x55;
        *(u8*)(work + 0x6B) = 0x22;
        *(u8*)(work + 0x6C) = 0;
        *(u8*)(work + 0x6D) = 0xFF;
        *(u8*)(work + 0x6E) = 0x3F;
        *(f32*)(work + 0x1C) = scale;
        *(f32*)(work + 0x20) = scale;
        *(f32*)(work + 0x24) = scale;
        *(f32*)(work + 0x38) = zero;
        *(f32*)(work + 0x44) = float_0p04_80425304;
        *(f32*)(work + 0x3C) = float_0p1_80425310;
        *(f32*)(work + 0x48) = float_1p7_80425334;
        *(f32*)(work + 0x50) = zero;
        *(f32*)(work + 0x5C) = float_0p04_80425304;
        *(f32*)(work + 0x54) = float_0p08_80425338;
        *(f32*)(work + 0x60) = float_1p36_8042533c;
    }

    *(f32*)(work + 0x10) = *(f32*)(work + 4);
    *(f32*)(work + 0x14) = *(f32*)(work + 8);
    *(f32*)(work + 0x18) = *(f32*)(work + 0xC);
    return entry;
}

