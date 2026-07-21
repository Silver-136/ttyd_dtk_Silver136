#include "effect/n64/eff_butterfly_n64.h"

typedef struct EffButterflyWork {
    s32 type;
    s32 timer;
    s32 unk_08;
    f32 x;
    f32 y;
    f32 z;
    f32 x2;
    f32 y2;
    f32 z2;
    s32 alpha;
    s32 unk_28;
    s32 unk_2c;
    s32 unk_30;
    f32 unk_34;
    f32 unk_38;
    u8 pad_3c[0xC];
} EffButterflyWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effButterflyMain(void* effect);

extern char str_ButterflyN64_802facf0[];
extern f32 float_50_80424f08;
extern f32 float_30_80424f0c;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effButterflyN64Entry(s32 type, f32 x, f32 y, f32 z) {
    void* entry = effEntry();
    EffButterflyWork* work;
    u32 flags;
    f32 f30;
    f32 f50;

    *(char**)((s32)entry + 0x14) = str_ButterflyN64_802facf0;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x48);
    *(EffButterflyWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effButterflyMain;
    f50 = float_50_80424f08;
    flags = *(u32*)entry;
    f30 = float_30_80424f0c;
    *(u32*)entry = flags | 2;
    work->type = type;
    work->timer = 1000;
    work->alpha = 0xFF;
    work->x = x;
    work->x2 = x;
    work->y = y;
    work->y2 = y;
    work->z = z;
    work->z2 = z;
    work->unk_08 = 0;
    work->unk_2c = 0;
    work->unk_30 = 0;
    work->unk_34 = f50;
    work->unk_38 = f30;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void effButterflyMain(void* effect) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effDelete(void* effect);
    extern s32 rand(void);
    extern double sin(f64 x);
    extern double cos(f64 x);
    extern double sqrt(f64 x);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effButterflyDisp(s32 cameraId, void* effect);
    extern f32 float_6p2832_80424efc;
    extern f32 float_360_80424f00;
    extern f32 float_0_80424ef8;
    extern f32 float_1_80424f04;
    EffButterflyWork* work;
    LocalVec3 pos;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 len;
    f32 invLen;
    s32 r;
    s32 mod;
    f32 angle;

    work = *(EffButterflyWork**)((s32)effect + 0xC);
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect = *(u32*)effect & ~4;
        work->timer = 900;
    }

    if (work->timer < 1000) {
        work->timer--;
    }
    work->unk_08++;

    if (work->timer < 0) {
        effDelete(effect);
        return;
    }

    if (work->timer < 0x10) {
        work->alpha = work->timer << 4;
    }

    work->unk_30--;
    if (work->unk_30 < 1) {
        r = rand();
        mod = r % 0x168;
        angle = (float_6p2832_80424efc * (f32)mod) / float_360_80424f00;
        dx = work->unk_34 * (f32)sin(angle) + work->x2 - work->x;

        r = rand();
        dy = work->y2 + (f32)(r % (s32)work->unk_38) - work->y;

        dz = work->unk_34 * (f32)cos(angle) + work->z2 - work->z;

        r = rand();
        work->unk_30 = (r % 100) + 10;

        if ((dx == float_0_80424ef8) && (dy == float_0_80424ef8) && (dz == float_0_80424ef8)) {
            dx = work->x2 - work->x;
            dy = work->y2 - work->y;
            dz = work->z2 - work->z;
            if ((dx == float_0_80424ef8) && (dy == float_0_80424ef8) && (dz == float_0_80424ef8)) {
                dx = float_1_80424f04;
                dy = float_0_80424ef8;
                dz = float_0_80424ef8;
            }
        }

        len = dx * dx + dy * dy + dz * dz;
        if (len != float_0_80424ef8) {
            invLen = float_1_80424f04 / (f32)sqrt(len);
        } else {
            invLen = float_0_80424ef8;
        }
        *(f32*)((s32)work + 0x3C) = dx * invLen;
        *(f32*)((s32)work + 0x40) = dy * invLen;
        *(f32*)((s32)work + 0x44) = dz * invLen;
        work->unk_28 = angleABf(float_0_80424ef8, float_0_80424ef8,
                                -*(f32*)((s32)work + 0x3C), *(f32*)((s32)work + 0x44));
    }

    work->unk_2c += 3;
    if (work->unk_2c > 0x1D) {
        work->unk_2c -= 0x1E;
    }

    work->x += *(f32*)((s32)work + 0x3C);
    work->y += *(f32*)((s32)work + 0x40);
    work->z += *(f32*)((s32)work + 0x44);
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispEntry(4, 1, effButterflyDisp, effect, dispCalcZ(&pos));
}

void effButterflyDisp(int cameraId, int effect) {
    typedef float Mtx[3][4];
    extern void* camGetPtr(int);
    extern void PSMTXTrans(Mtx, double, double, double);
    extern void PSMTXRotRad(Mtx, double, char);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void PSMTXScale(Mtx, float, float, float);
    extern void GXLoadPosMtxImm(Mtx, int);
    extern void GXSetCurrentMtx(int);
    extern void GXSetTevColor(int, void*);
    extern void GXSetNumChans(int);
    extern void GXSetNumTevStages(int);
    extern void GXSetTevOrder(int, int, int, int);
    extern void GXSetTevColorOp(int, int, int, int, int, int);
    extern void GXSetTevAlphaOp(int, int, int, int, int, int);
    extern void GXSetTevColorIn(int, int, int, int, int);
    extern void GXSetTevAlphaIn(int, int, int, int, int);
    extern void GXSetNumTexGens(int);
    extern void GXSetTexCoordGen2(int, int, int, int, int, int);
    extern void GXLoadTexMtxImm(Mtx, int, int);
    extern void effGetTexObjN64(int, void*);
    extern void GXLoadTexObj(void*, int);
    extern void GXSetCullMode(int);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(int, int, int);
    extern void tri2(int, int, int, int, int, int, int);
    extern signed char y_data[];
    extern unsigned char but_02_v[];
    extern unsigned char dat_80424ee0;
    extern float float_0p3_80424ee4;
    extern float float_deg2rad_80424ee8;
    extern float float_0p02_80424eec;
    extern float float_0p03125_80424ef0;
    extern float float_0p015625_80424ef4;
    extern float float_0_80424ef8;
    unsigned char texObj[0x20];
    Mtx scale;
    Mtx rotation;
    Mtx model;
    unsigned char* camera = (unsigned char*)camGetPtr(cameraId);
    int* work = *(int**)(effect + 0xC);
    int frame = work[0xB];
    unsigned char alpha = (unsigned char)work[9];
    unsigned int color;

    PSMTXTrans(model, (float)work[3],
        (float)work[4] + float_0p3_80424ee4 * (float)y_data[frame],
        (float)work[5]);
    PSMTXRotRad(rotation, float_deg2rad_80424ee8 * (float)work[10], 'y');
    PSMTXConcat(model, rotation, model);
    PSMTXScale(scale, float_0p02_80424eec, float_0p02_80424eec, float_0p02_80424eec);
    PSMTXConcat(model, scale, model);
    PSMTXConcat((float (*)[4])(camera + 0x11C), model, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);

    color = ((unsigned int)alpha << 24) | ((unsigned int)alpha << 16) |
            ((unsigned int)alpha << 8) | dat_80424ee0;
    GXSetTevColor(1, &color);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 2, 8, 15);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scale, float_0p03125_80424ef0, float_0p015625_80424ef4, float_0_80424ef8);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    effGetTexObjN64(work[0] + 0x78, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);
    effSetVtxDescN64(but_02_v + frame * 0x54);
    GXBegin(0x90, 0, 0xC);
    tri2(0, 1, 2, 0, 0, 2, 3);
    tri2(3, 4, 0, 0, 3, 5, 4);
}
