#include "effect/n64/eff_radiation_n64.h"

void* effRadiationN64Entry(f32 x, f32 y, f32 z, f32 scale, s32 type) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effRadiationMain(void*);
    extern char str_RadiationN64_802fbdb8[];
    extern s32 effTblRandN64(s32, s32);
    extern s32 r_data[];
    extern f32 float_0_80425e1c, float_neg0p6_80425e40, float_10_80425e44, float_0p6_80425e48,
        float_0p1_80425e4c, float_2p5_80425e50, float_25_80425e54;
    void* entry;
    u8* work;
    u8* particle;
    s32 count, i;
    s32 wasOver9;
    entry = effEntry();
    wasOver9 = type > 9;
    if (wasOver9)
        type %= 10;
    count = type < 2 ? 9 : 0x11;
    *(char**)((s32)entry + 0x14) = str_RadiationN64_802fbdb8;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, count * 0x9C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effRadiationMain;
    *(s32*)entry |= 2;
    *(s32*)work = type;
    *(f32*)(work + 0x10) = x;
    *(f32*)(work + 0x14) = y;
    *(f32*)(work + 0x18) = z;
    *(f32*)(work + 0x1C) = scale;
    *(f32*)(work + 0x20) = scale;
    *(s32*)(work + 0x28) = 100;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x94) = wasOver9;
    *(s32*)(work + 0x98) = 4;

    particle = work;
    if (type < 2) {
        s32 seed0 = 3;
        s32 seed1 = 4;
        s32 seed2 = 5;
        for (i = 1; i < count; i++, particle += 0x9C) {
            s32 step = i - 1;
            *(f32*)(particle + 0x104) = (f32)((step * 30) / (count - 1) - 10);
            *(s32*)(particle + 0x118) = 50 - (step * 20) / (count - 1);
            *(f32*)(particle + 0x108) = float_2p5_80425e50 +
                float_25_80425e54 / (f32)*(s32*)(particle + 0x118);
            *(f32*)(particle + 0x10C) = (f32)effTblRandN64(0xB4, seed0);
            *(f32*)(particle + 0x110) = (f32)effTblRandN64(0xB4, seed1);
            *(f32*)(particle + 0x114) = (f32)effTblRandN64(0xB4, seed2);
            seed0 += 3;
            seed1 += 3;
            seed2 += 3;
            *(f32*)(particle + 0xF4) = *(f32*)(particle + 0x104);
            *(s32*)(particle + 0xC4) = *(s32*)(particle + 0x118);
            *(f32*)(particle + 0x128) = *(f32*)(particle + 0x108);
            *(f32*)(particle + 0xF8) = *(f32*)(particle + 0x10C);
            *(f32*)(particle + 0xFC) = *(f32*)(particle + 0x110);
            *(f32*)(particle + 0x100) = *(f32*)(particle + 0x114);
            *(s32*)(particle + 0xC0) = 0;
            *(s32*)(particle + 0xC8) = 0;
        }
    } else {
        s32 timer = 0x20;
        for (i = 1; i < count; i++, particle += 0x9C) {
            s32 dataIndex = (i - 1) * 5;
            *(f32*)(particle + 0xF4) = float_0_80425e1c;
            *(s32*)(particle + 0xC4) = timer;
            timer += 2;
            *(f32*)(particle + 0xF8) = (f32)r_data[dataIndex];
            *(f32*)(particle + 0xFC) = (f32)r_data[dataIndex + 1];
            *(f32*)(particle + 0x100) = (f32)r_data[dataIndex + 2];
            *(s32*)(particle + 0xC0) = 0;
            *(s32*)(particle + 0xC8) = 0;
            *(f32*)(particle + 0xD4) = float_0_80425e1c;
            *(f32*)(particle + 0xD0) = float_0_80425e1c;
            *(f32*)(particle + 0xE4) = float_0_80425e1c;
            *(f32*)(particle + 0xE8) = float_neg0p6_80425e40;
            *(f32*)(particle + 0xD8) = float_10_80425e44;
            *(f32*)(particle + 0xDC) = float_0_80425e1c;
            *(f32*)(particle + 0xEC) = float_0_80425e1c;
            *(f32*)(particle + 0xF0) = float_0p6_80425e48;
            *(f32*)(particle + 0x11C) = float_0_80425e1c;
            *(f32*)(particle + 0x120) = float_0_80425e1c;
            *(f32*)(particle + 0x124) = float_0p1_80425e4c * (f32)r_data[dataIndex + 4];
            *(f32*)(particle + 0x12C) = float_0_80425e1c;
        }
    }
    return entry;
}

void effRadiationMain(void* effect) {
    extern void effDelete(void*);
    extern s32 effTblRandN64(s32, s32);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effRadiationDisp(void);
    extern s32 r_data[];
    extern f32 float_256_80425e3c, float_0_80425e1c, float_neg0p6_80425e40, float_10_80425e44;
    extern f32 float_0p6_80425e48, float_0p1_80425e4c;
    u8* work = *(u8**)((s32)effect + 0xC);
    f32 pos[3];
    u8* part;
    s32 type;
    s32 timer;
    s32 seed;
    s32 life;
    s32 i;

    pos[0] = *(f32*)(work + 4);
    pos[1] = *(f32*)(work + 8);
    pos[2] = *(f32*)(work + 0xC);
    type = *(s32*)work;
    if (*(s32*)(work + 0x28) < 0xB) {
        *(s32*)(work + 0x28) -= 1;
    }
    *(s32*)(work + 0x2C) += 1;
    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x28) = 10;
    }
    timer = *(s32*)(work + 0x28);
    if (timer < 0) {
        effDelete(effect);
        return;
    }

    part = work;
    if (type >= 0 && type < 2) {
        seed = (*(s32*)(work + 0x2C) * 10 + 1) * 3;
        for (i = 1; i < *(s32*)((u8*)effect + 8); i++, part += 0x9C) {
            *(s32*)(part + 0xC4) -= 1;
            *(s32*)(part + 0xC8) += 1;
            if (*(s32*)(part + 0xC8) < 10) {
                *(s32*)(part + 0xC0) = *(s32*)(part + 0xC8) * 12;
            }
            if (*(s32*)(part + 0xC4) < 5) {
                *(s32*)(part + 0xC0) = *(s32*)(part + 0xC4) * 25;
            }
            *(f32*)(part + 0xF4) += *(f32*)(part + 0x128);
            if (*(s32*)(part + 0xC4) < 1) {
                *(f32*)(part + 0x10C) = (f32)effTblRandN64(0xB4, seed);
                *(f32*)(part + 0x110) = (f32)effTblRandN64(0xB4, seed + 1);
                *(f32*)(part + 0x114) = (f32)effTblRandN64(0xB4, seed + 2);
                *(f32*)(part + 0xF4) = *(f32*)(part + 0x104);
                *(s32*)(part + 0xC4) = *(s32*)(part + 0x118);
                *(f32*)(part + 0x128) = *(f32*)(part + 0x108);
                *(f32*)(part + 0xF8) = *(f32*)(part + 0x10C);
                *(f32*)(part + 0xFC) = *(f32*)(part + 0x110);
                *(f32*)(part + 0x100) = *(f32*)(part + 0x114);
                *(s32*)(part + 0xC0) = 0;
                *(s32*)(part + 0xC8) = 0;
            }
        }
    } else {
        life = 0x20;
        for (i = 1; i < *(s32*)((u8*)effect + 8); i++, part += 0x9C, life += 2) {
            *(f32*)(part + 0xD0) += *(f32*)(part + 0xE4);
            if (*(f32*)(part + 0xD0) > float_256_80425e3c) *(f32*)(part + 0xD0) -= float_256_80425e3c;
            if (*(f32*)(part + 0xD0) < float_0_80425e1c) *(f32*)(part + 0xD0) += float_256_80425e3c;
            *(f32*)(part + 0xD8) += *(f32*)(part + 0xEC);
            if (*(f32*)(part + 0xD8) > float_256_80425e3c) *(f32*)(part + 0xD8) -= float_256_80425e3c;
            if (*(f32*)(part + 0xD8) < float_0_80425e1c) *(f32*)(part + 0xD8) += float_256_80425e3c;
            *(f32*)(part + 0xD4) += *(f32*)(part + 0xE8);
            if (*(f32*)(part + 0xD4) > float_256_80425e3c) *(f32*)(part + 0xD4) -= float_256_80425e3c;
            if (*(f32*)(part + 0xD4) < float_0_80425e1c) *(f32*)(part + 0xD4) += float_256_80425e3c;
            *(f32*)(part + 0xDC) += *(f32*)(part + 0xF0);
            if (*(f32*)(part + 0xDC) > float_256_80425e3c) *(f32*)(part + 0xDC) -= float_256_80425e3c;
            if (*(f32*)(part + 0xDC) < float_0_80425e1c) *(f32*)(part + 0xDC) += float_256_80425e3c;
            *(s32*)(part + 0xC4) -= 1;
            *(s32*)(part + 0xC8) += 1;
            if (*(s32*)(part + 0xC8) < 5) *(s32*)(part + 0xC0) = *(s32*)(part + 0xC8) * 50;
            if (*(s32*)(part + 0xC4) < 5) *(s32*)(part + 0xC0) = *(s32*)(part + 0xC4) * 50;
            if (timer < 10) *(s32*)(part + 0xC0) = (*(s32*)(part + 0xC0) * timer) / 10;
            if (*(s32*)(part + 0xC4) < 1) {
                s32 n = (i - 1) * 5;
                *(f32*)(part + 0xF4) = float_0_80425e1c;
                *(s32*)(part + 0xC4) = life;
                *(f32*)(part + 0xF8) = (f32)r_data[n];
                *(f32*)(part + 0xFC) = (f32)r_data[n + 1];
                *(f32*)(part + 0x100) = (f32)r_data[n + 2];
                *(s32*)(part + 0xC0) = 0;
                *(s32*)(part + 0xC8) = 0;
                *(f32*)(part + 0xD4) = float_0_80425e1c;
                *(f32*)(part + 0xD0) = float_0_80425e1c;
                *(f32*)(part + 0xE4) = float_0_80425e1c;
                *(f32*)(part + 0xE8) = float_neg0p6_80425e40;
                *(f32*)(part + 0xD8) = float_10_80425e44;
                *(f32*)(part + 0xDC) = float_0_80425e1c;
                *(f32*)(part + 0xEC) = float_0_80425e1c;
                *(f32*)(part + 0xF0) = float_0p6_80425e48;
                *(f32*)(part + 0x11C) = float_0_80425e1c;
                *(f32*)(part + 0x120) = float_0_80425e1c;
                *(f32*)(part + 0x124) = float_0p1_80425e4c * (f32)r_data[n + 4];
                *(s32*)(part + 0x12C) = 0;
            }
            if (*(s32*)(part + 0x12C) < 1 || --*(s32*)(part + 0x12C) < 1) {
                *(f32*)(part + 0xF8) += *(f32*)(part + 0x11C);
                *(f32*)(part + 0xFC) += *(f32*)(part + 0x120);
                *(f32*)(part + 0x100) += *(f32*)(part + 0x124);
            }
        }
    }
    if (*(s32*)(work + 0x94) == 0) {
        dispEntry(*(s32*)(work + 0x98), 2, effRadiationDisp, effect, dispCalcZ(pos));
    } else {
        dispEntry(*(s32*)(work + 0x98), 8, effRadiationDisp, effect, dispCalcZ(pos));
    }
}

void effRadiationDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXSetCullMode(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetTevColor(s32, void*);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_0p03125_80425e18, float_0_80425e1c, float_deg2rad_80425e24;
    u8 texObj[0x20];
    f32 trans[3][4], rot[3][4], mtx[3][4], scale[3][4];
    u8* work = *(u8**)((s32)effect + 0xC);
    s32 type = *(s32*)work, i;
    u32 color;
    if (*(s32*)(work + 0x94) != 0) {
        GXSetBlendMode(1, 4, 5, 0);
        GXSetZCompLoc(1);
        GXSetAlphaCompare(7, 0, 0, 7, 0);
        GXSetZMode(0, 3, 0);
    }
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 2, 1, 8, 0);
    GXSetTevAlphaIn(0, 0, 1, 7, 7);
    effGetTexObjN64(0x2C, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scale, float_0p03125_80425e18, float_0p03125_80425e18, float_0_80425e1c);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    GXSetCullMode(0);
    PSMTXTrans(trans, *(f32*)(work + 0x10), *(f32*)(work + 0x14), *(f32*)(work + 0x18));
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, work += 0x9C) {
        PSMTXRotRad(rot, 0x78, float_deg2rad_80425e24 * *(f32*)(work + 0xF8));
        PSMTXConcat(trans, rot, mtx);
        PSMTXRotRad(rot, 0x79, float_deg2rad_80425e24 * *(f32*)(work + 0xFC));
        PSMTXConcat(mtx, rot, mtx);
        PSMTXRotRad(rot, 0x7A, float_deg2rad_80425e24 * *(f32*)(work + 0x100));
        PSMTXConcat(mtx, rot, mtx);
        PSMTXTrans(scale, *(f32*)(work + 0xF4), float_0_80425e1c, float_0_80425e1c);
        PSMTXConcat(mtx, scale, mtx);
        GXLoadPosMtxImm(mtx, 0);
        GXSetCurrentMtx(0);
        color = *(u32*)(work + 0xC0);
        GXSetTevColor(1, &color);
        effSetVtxDescN64((void*)0x803A78A0);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
