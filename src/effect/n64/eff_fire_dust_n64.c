#include "effect/n64/eff_fire_dust_n64.h"
void* effFireDustN64Entry(f32 x, f32 y, f32 z, f32 scaleA, f32 scaleB, s32 type, s32 count, u32 timer) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effFireDustMain(void* effect);
    extern char str_FireDustN64_802faea0[];
    extern f32 float_1_804251cc;
    extern f32 float_20_804251d0;
    void* entry;
    u8* work;
    u8* part;
    f32 step;
    u32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_FireDustN64_802faea0;
    *(s32*)((s32)entry + 8) = count + 2;
    work = __memAlloc(3, (count + 2) * 0x6C);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFireDustMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x3C) = 0;
    if ((s32)timer < 1) {
        *(s32*)(work + 0x38) = 1000;
    } else {
        *(s32*)(work + 0x38) = timer;
    }
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x10) = scaleA;
    *(f32*)(work + 0x14) = scaleB;
    *(f32*)(work + 0x60) = float_1_804251cc;
    *(s32*)(work + 0x4C) = 0xFF;
    *(s32*)(work + 0x40) = 0xFF;
    *(s32*)(work + 0x44) = 0xFF;
    *(s32*)(work + 0x48) = 0xFF;
    *(s32*)(work + 0x50) = 0xFF;
    *(s32*)(work + 0x54) = 0xFF;
    *(s32*)(work + 0x58) = 0x7F;
    *(s32*)(work + 0x5C) = 0xFF;

    if ((s32)timer < 0x14) {
        step = float_20_804251d0 / (f32)(count + 1);
    } else {
        step = (f32)timer / (f32)(count + 1);
    }

    part = work + 0x6C;
    for (i = 1; i < (u32)(count + 2); i++, part += 0x6C) {
        *(s32*)(part + 0x30) = (s32)(-(f32)i * step) - 1;
    }

    return entry;
}


void effFireDustMain(void* effect) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effDelete(void* effect);
    extern s32 rand(void);
    extern double sin(f64 x);
    extern double cos(f64 x);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effFireDustDisp(s32 cameraId, void* effect);
    extern f32 scale_data[];
    extern f32 float_0_80425198;
    extern f32 float_0p5_804251a4;
    extern f32 float_6p2832_804251a8;
    extern f32 float_360_804251ac;
    extern f32 float_15_804251b0;
    extern f32 float_0p4_804251b4;
    extern f32 float_2_804251b8;
    extern f32 float_4_804251bc;
    extern f32 float_0p2_804251c0;
    extern f32 float_0p6_804251c4;
    extern f32 float_0p04_804251c8;
    u8* work;
    u8* part;
    LocalVec3 pos;
    f32 angle;
    f32 halfHeight;
    s32 i;
    s32 r;

    work = *(u8**)((s32)effect + 0xC);
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x38) = 0x10;
    }
    if (*(s32*)(work + 0x38) < 1000) {
        (*(s32*)(work + 0x38))--;
    }
    (*(s32*)(work + 0x3C))++;
    if (*(s32*)(work + 0x38) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x38) < 8) {
        *(s32*)(work + 0x4C) = *(s32*)(work + 0x38) << 5;
    }

    halfHeight = float_0p5_804251a4 * *(f32*)(work + 0x14);
    part = work;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        part += 0x6C;
        (*(s32*)(part + 0x30))++;
        if (*(s32*)(part + 0x30) > 0x13) {
            *(s32*)(part + 0x30) = 0;
        }
        if (*(s32*)(part + 0x30) > -1) {
            if (*(s32*)(part + 0x30) == 0) {
                r = rand();
                angle = (float_6p2832_804251a8 * (f32)(r % 0x168)) / float_360_804251ac;
                rand();
                *(f32*)(part + 4) = *(f32*)(work + 0x10) * (f32)sin(angle);
                r = rand();
                *(f32*)(part + 8) = (f32)(r % (s32)*(f32*)(work + 0x14)) - halfHeight;
                *(f32*)(part + 0xC) = *(f32*)(work + 0x10) * (f32)cos(angle);
                *(f32*)(part + 0x18) = float_15_804251b0 * (f32)sin(angle);
                *(f32*)(part + 0x1C) = float_0p4_804251b4 * *(f32*)(part + 8);
                *(f32*)(part + 0x20) = float_15_804251b0 * (f32)cos(angle);
                *(f32*)(part + 0x24) = float_0p5_804251a4 * *(f32*)(part + 0x18);
                *(f32*)(part + 0x28) = float_0p5_804251a4 * *(f32*)(part + 0x1C);
                *(f32*)(part + 0x2C) = float_0p5_804251a4 * *(f32*)(part + 0x20);
                r = rand();
                *(f32*)(part + 0x64) = (f32)(r % 0xF);
                r = rand();
                *(f32*)(part + 0x68) = (f32)(r % 0xF);
                *(f32*)(part + 0x34) = float_0_80425198;
                *(s32*)(part + 0x4C) = 0xFF;
            }

            *(f32*)(part + 0x60) = float_2_804251b8 * scale_data[*(s32*)(part + 0x30)];
            *(f32*)(part + 0x34) += float_4_804251bc;
            *(f32*)(part + 4) += *(f32*)(part + 0x18);
            *(f32*)(part + 8) += *(f32*)(part + 0x1C);
            *(f32*)(part + 0xC) += *(f32*)(part + 0x20);
            *(f32*)(part + 0x64) += float_0p2_804251c0;
            *(f32*)(part + 0x68) += float_0p6_804251c4;
            *(f32*)(part + 0x18) += float_0p04_804251c8 * (*(f32*)(part + 0x24) - *(f32*)(part + 0x18));
            *(f32*)(part + 0x1C) += float_0p04_804251c8 * (*(f32*)(part + 0x28) - *(f32*)(part + 0x1C));
            *(f32*)(part + 0x20) += float_0p04_804251c8 * (*(f32*)(part + 0x2C) - *(f32*)(part + 0x20));
        }
    }

    dispEntry(4, 2, effFireDustDisp, effect, dispCalcZ(&pos));
}

void effFireDustDisp(int cameraId, int effect) {
    typedef float Mtx[3][4];
    extern void* camGetPtr(int);
    extern void PSMTXTrans(Mtx, double, double, double);
    extern void PSMTXScale(Mtx, float, float, float);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void PSMTXRotRad(Mtx, double, char);
    extern void GXSetTevColor(int, void*);
    extern void effGetTexObjN64(int, void*);
    extern void GXLoadTexObj(void*, int);
    extern void GXSetNumChans(int);
    extern void GXSetNumTexGens(int);
    extern void GXSetTexCoordGen2(int, int, int, int, int, int);
    extern void GXLoadTexMtxImm(Mtx, int, int);
    extern void GXSetNumTevStages(int);
    extern void GXSetTevOrder(int, int, int, int);
    extern void GXSetTevColorOp(int, int, int, int, int, int);
    extern void GXSetTevAlphaOp(int, int, int, int, int, int);
    extern void GXSetTevColorIn(int, int, int, int, int);
    extern void GXSetTevAlphaIn(int, int, int, int, int);
    extern void GXSetCullMode(int);
    extern void main_dl(int, Mtx);
    extern float float_0p0056818_80425190;
    extern float float_0p045455_80425194;
    extern float float_0_80425198;
    extern float float_2p0944_8042519c;
    extern float float_neg2p0944_804251a0;
    unsigned char texObj[0x20];
    Mtx view;
    Mtx rotation;
    Mtx scaled;
    Mtx model;
    unsigned char* entry = (unsigned char*)effect;
    unsigned char* work = *(unsigned char**)(entry + 0xC);
    unsigned char* camera = (unsigned char*)camGetPtr(cameraId);
    unsigned int color;
    float size = *(float*)(work + 0x60);

    PSMTXTrans(model, *(float*)(work + 4), *(float*)(work + 8), *(float*)(work + 0xC));
    PSMTXScale(scaled, size, size, size);
    PSMTXConcat(model, scaled, model);
    color = ((unsigned int)*(unsigned char*)(work + 0x40) << 24) |
            ((unsigned int)*(unsigned char*)(work + 0x44) << 16) |
            ((unsigned int)*(unsigned char*)(work + 0x48) << 8) |
            *(unsigned char*)(work + 0x5C);
    GXSetTevColor(2, &color);
    effGetTexObjN64(0x20, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scaled, float_0p0056818_80425190, float_0p045455_80425194, float_0_80425198);
    GXLoadTexMtxImm(scaled, 0x1E, 1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 2, 12, 8, 15);
    GXSetTevAlphaIn(0, 7, 4, 4, 7);
    GXSetCullMode(0);
    PSMTXConcat((float (*)[4])(camera + 0x11C), model, view);
    main_dl(effect, view);
    PSMTXRotRad(rotation, float_2p0944_8042519c, 'y');
    PSMTXConcat(model, rotation, view);
    PSMTXConcat((float (*)[4])(camera + 0x11C), view, view);
    main_dl(effect, view);
    PSMTXRotRad(rotation, float_neg2p0944_804251a0, 'y');
    PSMTXConcat(model, rotation, view);
    PSMTXConcat((float (*)[4])(camera + 0x11C), view, view);
    main_dl(effect, view);
}

void main_dl(int effect, float view[3][4]) {
    typedef float Mtx[3][4];
    extern void PSMTXTrans(Mtx, double, double, double);
    extern void PSMTXRotRad(Mtx, double, char);
    extern void PSMTXScale(Mtx, float, float, float);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXLoadPosMtxImm(Mtx, int);
    extern void GXSetCurrentMtx(int);
    extern void GXSetTevColor(int, void*);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(int, int, int);
    extern void tri2(int, int, int, int, int, int, int);
    extern float float_255_80425188;
    extern float float_deg2rad_8042518c;
    extern unsigned char DAT_8039f480[], DAT_8039f4b8[], DAT_8039f4f0[], DAT_8039f528[];
    extern unsigned char DAT_8039f560[], size8x8_tex22x22_vtx[], DAT_8039f410[], DAT_8039f448[];
    unsigned char* entry = (unsigned char*)effect;
    unsigned char* work = *(unsigned char**)(entry + 0xC);
    unsigned char* part = work + 0x6C;
    void* vertexData = 0;
    Mtx rotation;
    Mtx scale;
    Mtx model;
    float alphaScale = (float)*(int*)(work + 0x4C) / float_255_80425188;
    int i;

    for (i = 1; i < *(int*)(entry + 8); i++, part += 0x6C) {
        unsigned int color;
        float size;
        if (*(int*)(part + 0x30) < 0) {
            continue;
        }
        PSMTXTrans(model, *(float*)(part + 4), *(float*)(part + 8), *(float*)(part + 0xC));
        PSMTXRotRad(rotation, float_deg2rad_8042518c * *(float*)(part + 0x34), 'z');
        size = *(float*)(part + 0x60) * alphaScale;
        PSMTXScale(scale, size, size, size);
        PSMTXConcat(model, rotation, model);
        PSMTXConcat(model, scale, model);
        PSMTXConcat(view, model, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        color = ((unsigned int)*(unsigned char*)(work + 0x50) << 24) |
                ((unsigned int)*(unsigned char*)(work + 0x54) << 16) |
                ((unsigned int)*(unsigned char*)(work + 0x58) << 8) |
                (unsigned char)((float)*(int*)(part + 0x4C) * alphaScale);
        GXSetTevColor(1, &color);
        switch (i & 7) {
            case 0: vertexData = DAT_8039f480; break;
            case 1: vertexData = DAT_8039f4b8; break;
            case 2: vertexData = DAT_8039f4f0; break;
            case 3: vertexData = DAT_8039f528; break;
            case 4: vertexData = DAT_8039f560; break;
            case 5: vertexData = size8x8_tex22x22_vtx; break;
            case 6: vertexData = DAT_8039f410; break;
            case 7: vertexData = DAT_8039f448; break;
        }
        effSetVtxDescN64(vertexData);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    }
}
