#include "effect/n64/eff_freeze_n64.h"

typedef struct EffFreezeWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    s32 frame;
    u8 colorR;
    u8 colorG;
    u8 colorB;
    u8 alpha;
    u8 color2R;
    u8 color2G;
    u8 color2B;
    u8 color2A;
    f32 scale;
    u8 unk_24;
    u8 unk_25;
    u8 pad_26[2];
    void* snowDust;
} EffFreezeWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void* effSnowDustN64Entry(s32, s32, s32, f32, f32, f32, f32, f32, f32);
void effFreezeMain(void* effect);

extern char str_FreezeN64_802fafe8[];
extern f32 float_10_80425388;
extern f32 float_32_804253a8;
extern f32 float_30_804253ac;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effFreezeN64Entry(s32 type, s32 param2, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffFreezeWork* work;
    s32 flags;
    f32 ten;
    f32 dustY;

    *(char**)((s32)entry + 0x14) = str_FreezeN64_802fafe8;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x2C);
    *(EffFreezeWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFreezeMain;
    flags = *(s32*)entry;
    *(s32*)entry = flags | 2;
    work->type = type;
    work->frame = 0;
    if (param2 <= 0) {
        work->timer = 1000;
    } else {
        work->timer = param2;
    }
    ten = float_10_80425388;
    work->alpha = 0xFF;
    work->x = x;
    work->y = y;
    dustY = ten + y;
    work->z = z;
    work->scale = scale;
    work->colorR = 0xFF;
    work->colorG = 0xFF;
    work->colorB = 0xFF;
    work->color2R = 0xF0;
    work->color2G = 0xFF;
    work->color2B = 0xFF;
    work->color2A = 0xFF;
    work->unk_25 = 0;
    work->unk_24 = 0;
    work->snowDust = effSnowDustN64Entry(1, 4, 0, x, dustY, z,
                                         float_32_804253a8, float_30_804253ac, scale);

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void effFreezeMain(void* effect) {
    extern s32 rand(void);
    extern void* effGlassN64Entry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 timer);
    extern void effDelete(void* effect);
    extern void effVaporN64Entry(f32 x, f32 y, f32 z, s32 type, s32 timer);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effFreezeDisp(s32 cameraId, void* effect);
    extern f32 float_0_80425384;
    extern f32 float_10_80425388;
    extern f32 float_3_8042538c;
    extern f32 float_0p3_80425390;
    extern f32 float_0p1_80425394;
    extern f32 float_0p7_80425398;
    extern f32 float_0p01_8042539c;
    extern f32 float_neg0p1_804253a0;
    extern f32 float_40_804253a4;

    u8* work;
    f32 pos[3];
    s32 timer;
    s32 frame;
    s32 i;
    s32 r;
    void* glass;
    u8* glassWork;
    void* snowDust;
    u8* snowWork;
    f32 scale;

    work = *(u8**)((s32)effect + 0xC);
    pos[0] = *(f32*)(work + 4);
    pos[1] = *(f32*)(work + 8);
    pos[2] = *(f32*)(work + 0xC);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 1000) {
        (*(s32*)(work + 0x10))--;
    }
    (*(s32*)(work + 0x14))++;

    timer = *(s32*)(work + 0x10);
    if (timer < 0) {
        for (i = 0; i < 10; i++) {
            r = rand();
            scale = *(f32*)(work + 0x20);
            glass = effGlassN64Entry(
                (*(f32*)(work + 4) + (f32)(rand() % 0x14)) - float_10_80425388,
                (*(f32*)(work + 0x20) * (f32)(rand() % 0x28)) + *(f32*)(work + 8),
                float_3_8042538c + *(f32*)(work + 0xC),
                scale * ((float_0p1_80425394 * float_0p7_80425398 * (f32)(r % 10)) + float_0p3_80425390),
                (i & 1) + 2,
                i + 0x1E);
            glassWork = *(u8**)((s32)glass + 0xC);
            *(f32*)(glassWork + 0x44) = float_0p1_80425394 * (f32)(rand() % 10);
            *(f32*)(glassWork + 0x48) = (float_0p01_8042539c * (f32)(rand() % 0x1E)) + float_0p1_80425394;
            *(f32*)(glassWork + 0x3C) = (f32)(rand() % 0x168);
            *(f32*)(glassWork + 0x40) = (f32)(rand() % 0x14);
            *(f32*)(glassWork + 0x4C) = (f32)((rand() % 10) - 5);
            *(f32*)(glassWork + 0x50) = (f32)((rand() % 10) - 5);
            *(f32*)(glassWork + 0x54) = float_0_80425384;
            *(f32*)(glassWork + 0x58) = float_neg0p1_804253a0;
        }
        effDelete(*(void**)(work + 0x28));
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x14);
    (*(s8*)(work + 0x25))--;
    if (*(s8*)(work + 0x25) < 0) {
        *(s8*)(work + 0x25) = (s8)((rand() % 0x3C) + 8);
    }
    if (*(s8*)(work + 0x25) < 8) {
        *(u8*)(work + 0x24) = *(s8*)(work + 0x25) / 2;
    } else {
        *(u8*)(work + 0x24) = 0;
    }

    if ((timer > 0x10) && ((frame % 0x10) == 0)) {
        effVaporN64Entry(*(f32*)(work + 4),
                         (float_40_804253a4 * *(f32*)(work + 0x20)) + *(f32*)(work + 8),
                         *(f32*)(work + 0xC), 2, 0x1E);
    }

    snowDust = *(void**)(work + 0x28);
    snowWork = *(u8**)((s32)snowDust + 0xC);
    *(f32*)(snowWork + 4) = *(f32*)(work + 4);
    *(f32*)(snowWork + 8) = float_10_80425388 + *(f32*)(work + 8);
    *(f32*)(snowWork + 0xC) = *(f32*)(work + 0xC);

    dispEntry(4, 2, effFreezeDisp, effect, dispCalcZ(pos));
}

void effFreezeDisp(int cameraId, int effect) {
    typedef float Mtx[3][4];
    extern void* camGetPtr(int);
    extern void PSMTXTrans(Mtx, double, double, double);
    extern void PSMTXRotRad(Mtx, double, char);
    extern void PSMTXScale(Mtx, float, float, float);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
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
    extern float float_deg2rad_80425378;
    extern float float_0p03125_8042537c;
    extern float float_0p0039062_80425380;
    extern float float_0_80425384;
    extern unsigned char size32x64_tex32x64_vtx[];
    unsigned char texObj[0x20];
    Mtx rotation;
    Mtx scale;
    Mtx model;
    unsigned char* work = *(unsigned char**)(effect + 0xC);
    unsigned char* camera = (unsigned char*)camGetPtr(cameraId);
    unsigned char* camera3d;
    unsigned int color1;
    unsigned int color2;
    float size;

    PSMTXTrans(model, *(float*)(work + 4), *(float*)(work + 8), *(float*)(work + 0xC));
    camera3d = (unsigned char*)camGetPtr(4);
    PSMTXRotRad(rotation, float_deg2rad_80425378 * -*(float*)(camera3d + 0x114), 'y');
    size = *(float*)(work + 0x20);
    PSMTXScale(scale, size, size, size);
    PSMTXConcat(model, rotation, model);
    PSMTXConcat(model, scale, model);
    PSMTXConcat((float (*)[4])(camera + 0x11C), model, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    color1 = ((unsigned int)*(unsigned char*)(work + 0x18) << 24) |
             ((unsigned int)*(unsigned char*)(work + 0x19) << 16) |
             ((unsigned int)*(unsigned char*)(work + 0x1A) << 8) |
             *(unsigned char*)(work + 0x1B);
    color2 = *(unsigned int*)(work + 0x1C);
    GXSetTevColor(1, &color1);
    GXSetTevColor(2, &color2);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 2, 10, 8, 15);
    GXSetTevAlphaIn(0, 7, 4, 4, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scale, float_0p03125_8042537c, float_0p0039062_80425380, float_0_80425384);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    effGetTexObjN64(0x8A, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);
    effSetVtxDescN64(size32x64_tex32x64_vtx + *(signed char*)(work + 0x24) * 0x38);
    GXBegin(0x90, 0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3);
}
