#include "effect/n64/eff_coin_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffCoinSubWork {
    s32 unk_00;
    s32 frame;
    u8 pad_08[0x18];
    s32 delay;
} EffCoinSubWork;

typedef struct EffCoinWork {
    u8 pad_00[8];
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    s32 frame;
    u8 pad_1c[8];
    EffCoinSubWork sub[1];
} EffCoinWork;

void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fad40;


void effCoinDisp(int cameraId, int effect) {
    typedef float Mtx[3][4];
    extern void* camGetPtr(int);
    extern void PSMTXTrans(Mtx, double, double, double);
    extern void PSMTXRotRad(Mtx, double, char);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void PSMTXScale(Mtx, float, float, float);
    extern void GXSetNumChans(int);
    extern void GXSetNumTevStages(int);
    extern void GXSetTevOrder(int, int, int, int);
    extern void GXSetTevColorOp(int, int, int, int, int, int);
    extern void GXSetTevAlphaOp(int, int, int, int, int, int);
    extern void GXSetTevColorIn(int, int, int, int, int);
    extern void GXSetTevAlphaIn(int, int, int, int, int);
    extern void GXSetNumTexGens(int);
    extern void GXSetTexCoordGen2(int, int, int, int, int, int);
    extern void effGetTexObjN64(int, void*);
    extern void GXLoadTexObj(void*, int);
    extern void GXSetCullMode(int);
    extern void effSetVtxDescN64(void*);
    extern void GXSetTevColor(int, void*);
    extern void GXLoadPosMtxImm(Mtx, int);
    extern void GXSetCurrentMtx(int);
    extern void GXLoadTexMtxImm(Mtx, int, int);
    extern void GXBegin(int, int, int);
    extern void tri2(int, int, int, int, int, int, int);
    extern float float_deg2rad_80424f4c;
    extern float float_1_80424f50;
    extern float float_0p03125_80424f54;
    extern float float_0p00625_80424f58;
    extern float float_0_80424f5c;
    extern float float_32_80424f60;
    extern unsigned int dat_80424f48;
    unsigned char texObj[0x20];
    Mtx base;
    Mtx scale;
    Mtx rotation;
    Mtx model;
    unsigned char* camera = (unsigned char*)camGetPtr(cameraId);
    unsigned char* camera3d;
    unsigned char* entry = (unsigned char*)effect;
    unsigned char* work = *(unsigned char**)(entry + 0xC);
    unsigned char* sub;
    unsigned int color = dat_80424f48;
    int i;

    PSMTXTrans(base, *(float*)(work + 8), *(float*)(work + 0xC), *(float*)(work + 0x10));
    camera3d = (unsigned char*)camGetPtr(4);
    PSMTXRotRad(rotation, float_deg2rad_80424f4c * -*(float*)(camera3d + 0x114), 'y');
    PSMTXConcat(base, rotation, model);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 8, 15, 2, 2);
    GXSetTevAlphaIn(0, 7, 4, 4, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    effGetTexObjN64(0x84, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);
    effSetVtxDescN64((void*)0x8039EEE0);
    GXSetTevColor(1, &color);

    sub = work + 0x24;
    for (i = 1; i < *(int*)(entry + 8); i++, sub += 0x24) {
        float size;
        if (*(int*)(sub + 0x20) != 0) {
            continue;
        }
        size = *(float*)(sub + 0x1C);
        PSMTXTrans(base, *(float*)(sub + 8), *(float*)(sub + 0xC), *(float*)(sub + 0x10));
        if (size != float_1_80424f50) {
            PSMTXScale(scale, size, size, float_1_80424f50);
            PSMTXConcat(base, scale, base);
        }
        PSMTXConcat(model, base, base);
        PSMTXConcat((float (*)[4])(camera + 0x11C), base, base);
        GXLoadPosMtxImm(base, 0);
        GXSetCurrentMtx(0);
        PSMTXScale(scale, float_0p03125_80424f54, float_0p00625_80424f58, float_0_80424f5c);
        PSMTXTrans(base, float_0_80424f5c,
            float_32_80424f60 * (float)(4 - (*(int*)(sub + 4) >> 1)),
            float_0_80424f5c);
        PSMTXConcat(scale, base, scale);
        GXLoadTexMtxImm(scale, 0x1E, 1);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    }
}

void* effCoinN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effCoinMain(void*);
    extern char str_CoinN64_802fad58[];
    extern s32 _data[];
    extern f32 float_0p01_80424f64;
    void* entry;
    u8* work;
    u8* sub;
    s32 i;
    s32* data;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_CoinN64_802fad58;
    *(s32*)((s32)entry + 8) = 5;
    work = __memAlloc(3, 0xB4);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effCoinMain;

    *(s32*)work = type;
    *(s32*)(work + 0x18) = 0;
    *(s32*)(work + 0x14) = 0xE;
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;

    sub = work + 0x24;
    data = _data;
    for (i = 1; i < 5; i++) {
        *(f32*)(sub + 8) = (f32)data[1];
        *(f32*)(sub + 0xC) = (f32)data[2];
        *(f32*)(sub + 0x10) = (f32)data[3];
        *(s32*)(sub + 4) = -1;
        *(s32*)(sub + 0x20) = data[0];
        *(f32*)(sub + 0x1C) = scale * (f32)data[4] * float_0p01_80424f64;
        data += 5;
        sub += 0x24;
    }

    return entry;
}

void effCoinMain(void* effect) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffCoinWork* work;
    register s32 i;
    register EffCoinSubWork* sub;

    work = *(EffCoinWork**)((s32)effect + 0xC);
    zero = &vec3_802fad40;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    work->timer--;
    work->frame++;
    if (work->timer < 0) {
        effDelete(effect);
    } else {
        i = 1;
        sub = work->sub;
        while (i < *(s32*)((s32)effect + 0x8)) {
            if (sub->delay > 0) {
                sub->delay--;
                if (sub->delay > 0) {
                    goto next;
                }
            }
            sub->frame++;
            if (sub->frame >= 10) {
                sub->delay = -1;
            }
        next:
            i++;
            sub++;
        }
        dispEntry(4, 2, effCoinDisp, effect, dispCalcZ(&dispPos));
    }
}

