#include "effect/n64/eff_broken_barrier_n64.h"

const char str_BrokenBarrierN64_802fac88[] = "BrokenBarrierN64";
f32 scale_data[] = {
    0.2f, 0.4f, 0.8f, 1.3f, 1.5f, 1.3f, 0.9f, 0.6f, 0.4f, 0.3f,
    0.2f, 0.4f, 0.8f, 1.3f, 1.5f, 1.3f, 0.9f, 0.6f, 0.4f, 0.3f,
};
void* effBrokenBarrierN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effBrokenBarrierMain(void);
    extern f32 float_30_80424e80;
    extern f32 float_0p33333_80424e84;
    extern f32 float_60_80424e88;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    s32 j;
    f32 step;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_BrokenBarrierN64_802fac88;
    *(s32*)((s32)entry + 8) = 0x3D;
    work = __memAlloc(3, 0x19BC);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBrokenBarrierMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x3C) = 0;
    if (timer <= 0) {
        *(s32*)(work + 0x38) = 1000;
    } else {
        *(s32*)(work + 0x38) = timer;
    }

    *(s32*)(work + 0x4C) = 0xFF;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x60) = scale;
    *(f32*)(work + 0x10) = float_30_80424e80;
    *(f32*)(work + 0x14) = float_30_80424e80;
    *(s32*)(work + 0x40) = 0xFF;
    *(s32*)(work + 0x44) = 0xFF;
    *(s32*)(work + 0x48) = 0xFF;
    *(s32*)(work + 0x50) = 0xD7;
    *(s32*)(work + 0x54) = 0xFF;
    *(s32*)(work + 0x58) = 0xEB;
    *(s32*)(work + 0x5C) = 0xFF;

    if (timer < 20) {
        step = float_0p33333_80424e84;
    } else {
        step = (f32)timer / float_60_80424e88;
    }

    part = work + 0x6C;
    j = 1;
    for (i = 0; i < 12; i++) {
        *(s32*)(part + 0x30) = (s32)(-(f32)j * step) - 1;
        j++;
        *(s32*)(part + 0x9C) = (s32)(-(f32)j * step) - 1;
        j++;
        *(s32*)(part + 0x108) = (s32)(-(f32)j * step) - 1;
        j++;
        *(s32*)(part + 0x174) = (s32)(-(f32)j * step) - 1;
        j++;
        *(s32*)(part + 0x1E0) = (s32)(-(f32)j * step) - 1;
        j++;
        part += 0x21C;
    }

    return entry;
}


void effBrokenBarrierMain(void* effect) {
    typedef union FloatBitsLocal {
        f32 value;
        u32 bits;
    } FloatBitsLocal;
    extern void effDelete(void*);
    extern int rand(void);
    extern double sin(double);
    extern double cos(double);
    extern double __frsqrte(double);
    extern float dispCalcZ(void*);
    extern void dispEntry(int, int, void*, void*, float);
    extern void effBrokenBarrierDisp(int, void*);
    extern f32 __float_nan;
    extern f32 vec3_802fac50[];
    extern f32 float_0p05_80424e6c;
    extern f32 float_1_80424e70;
    extern f64 double_0p5_802fac70;
    extern f64 double_3_802fac78;
    extern f64 double_0_802fac80;
    unsigned char* entry = (unsigned char*)effect;
    unsigned char* work = *(unsigned char**)(entry + 0xC);
    unsigned char* part;
    float pos[3];
    float width;
    float height;
    float halfWidth;
    float halfHeight;
    float lengthSq;
    float length;
    float angleX;
    float angleY;
    double value;
    double estimate;
    int widthInt;
    int heightInt;
    int type = *(int*)work;
    int timer;
    int category;
    int i;
    FloatBitsLocal classify;

    pos[0] = vec3_802fac50[0];
    pos[1] = vec3_802fac50[1];
    pos[2] = vec3_802fac50[2];
    pos[0] = *(float*)(work + 4);
    pos[1] = *(float*)(work + 8);
    pos[2] = *(float*)(work + 0xC);
    if ((*(unsigned int*)entry & 4) != 0) {
        *(unsigned int*)entry &= ~4;
        *(int*)(work + 0x38) = 16;
    }
    if (*(int*)(work + 0x38) < 1000) {
        (*(int*)(work + 0x38))--;
    }
    (*(int*)(work + 0x3C))++;
    timer = *(int*)(work + 0x38);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    if (timer < 8) {
        *(int*)(work + 0x4C) = timer << 5;
    }

    width = *(float*)(work + 0x10);
    height = *(float*)(work + 0x14);
    if (type < 4) {
        if (type >= 2) {
            width *= 4.0f;
            height *= 4.0f;
        }
    }
    widthInt = (int)width;
    heightInt = (int)height;
    halfWidth = 0.5f * width;
    halfHeight = 0.5f * height;

    part = work + 0x6C;
    for (i = 1; i < *(int*)(entry + 8); i++, part += 0x6C) {
        (*(int*)(part + 0x30))++;
        if (*(int*)(part + 0x30) > 19) {
            *(int*)(part + 0x30) = 0;
        }
        if (*(int*)(part + 0x30) >= 0) {
            if (*(int*)(part + 0x30) == 0) {
                angleY = (float)(rand() % 360);
                angleX = (float)(rand() % 360);
                if (type == 1) {
                    goto random_position;
                }
                if (type < 1) {
                    if (type < 0) {
                        goto random_position;
                    }
                } else if (type >= 3) {
                    goto random_position;
                }

                angleY = (6.283185f * angleY) / 360.0f;
                angleX = (6.283185f * angleX) / 360.0f;
                *(float*)(part + 4) = width * (float)sin(angleX) * (float)cos(angleY);
                *(float*)(part + 8) = height * (float)cos(angleX) * (float)cos(angleY);
                *(float*)(part + 0xC) = width * (float)sin(angleY);
                *(float*)(part + 0x18) = 0.4f * *(float*)(part + 4);
                *(float*)(part + 0x1C) = 0.4f * *(float*)(part + 8);
                *(float*)(part + 0x20) = 0.4f * *(float*)(part + 0xC);
                *(float*)(part + 0x24) = 0.1f * *(float*)(part + 0x18);
                *(float*)(part + 0x28) = 0.1f * *(float*)(part + 0x1C);
                *(float*)(part + 0x2C) = 0.1f * *(float*)(part + 0x20);
                goto initialized;

random_position:
                *(float*)(part + 4) = (float)(rand() % widthInt) - halfWidth;
                *(float*)(part + 8) = (float)(rand() % heightInt) - halfHeight;
                *(float*)(part + 0xC) = (float)(rand() % widthInt) - halfWidth;
                *(float*)(part + 0x18) = 0.3f * *(float*)(part + 4);
                *(float*)(part + 0x1C) = 0.3f * *(float*)(part + 8);
                *(float*)(part + 0x20) = 0.3f * *(float*)(part + 0xC);
                *(float*)(part + 0x24) = 0.02f * *(float*)(part + 0x18);
                *(float*)(part + 0x28) = 0.02f * *(float*)(part + 0x1C);
                *(float*)(part + 0x2C) = 0.02f * *(float*)(part + 0x20);

initialized:
                *(float*)(part + 0x64) = (float)(rand() % 15);
                *(float*)(part + 0x68) = (float)(rand() % 15);
                *(float*)(part + 0x34) = 0.0f;
                *(int*)(part + 0x4C) = 0xFF;
            }

            *(float*)(part + 0x60) = scale_data[*(int*)(part + 0x30)];
            *(float*)(part + 0x34) += 4.0f;
            if (type < 4) {
                if (type >= 2) {
                    *(float*)(part + 0x18) = float_0p05_80424e6c * -*(float*)(part + 4);
                    *(float*)(part + 0x1C) = float_0p05_80424e6c * -*(float*)(part + 8);
                    *(float*)(part + 0x20) = float_0p05_80424e6c * -*(float*)(part + 0xC);
                    lengthSq = *(float*)(part + 0x18) * *(float*)(part + 0x18) +
                               *(float*)(part + 0x1C) * *(float*)(part + 0x1C) +
                               *(float*)(part + 0x20) * *(float*)(part + 0x20);
                    value = (double)lengthSq;
                    if (value > double_0_802fac80) {
                        estimate = __frsqrte(value);
                        estimate = double_0p5_802fac70 * estimate *
                                   (double_3_802fac78 - value * estimate * estimate);
                        estimate = double_0p5_802fac70 * estimate *
                                   (double_3_802fac78 - value * estimate * estimate);
                        estimate = double_0p5_802fac70 * estimate *
                                   (double_3_802fac78 - value * estimate * estimate);
                        length = (float)(value * estimate);
                    } else {
                        length = lengthSq;
                        if (value < double_0_802fac80) {
                            length = __float_nan;
                        } else {
                            classify.value = lengthSq;
                            if ((classify.bits & 0x7F800000) == 0x7F800000) {
                                category = (classify.bits & 0x007FFFFF) ? 1 : 2;
                            } else if ((classify.bits & 0x7F800000) == 0) {
                                category = (classify.bits & 0x007FFFFF) ? 5 : 3;
                            } else {
                                category = 4;
                            }
                            if (category == 1) {
                                length = __float_nan;
                            }
                        }
                    }
                    if (length < float_1_80424e70) {
                        angleY = (6.283185f * (float)(rand() % 360)) / 360.0f;
                        angleX = (6.283185f * (float)(rand() % 360)) / 360.0f;
                        *(float*)(part + 4) = width * (float)sin(angleX) * (float)cos(angleY);
                        *(float*)(part + 8) = height * (float)cos(angleX) * (float)cos(angleY);
                        *(float*)(part + 0xC) = width * (float)sin(angleY);
                    }
                }
            }
            *(float*)(part + 4) += *(float*)(part + 0x18);
            *(float*)(part + 8) += *(float*)(part + 0x1C);
            *(float*)(part + 0xC) += *(float*)(part + 0x20);
            *(float*)(part + 0x64) += 0.2f;
            *(float*)(part + 0x68) += 0.6f;
            *(float*)(part + 0x18) += 0.04f * (*(float*)(part + 0x24) - *(float*)(part + 0x18));
            *(float*)(part + 0x1C) += 0.04f * (*(float*)(part + 0x28) - *(float*)(part + 0x1C));
            *(float*)(part + 0x20) += 0.04f * (*(float*)(part + 0x2C) - *(float*)(part + 0x20));
        }
    }
    dispEntry(4, 2, effBrokenBarrierDisp, effect, dispCalcZ(pos));
}

void effBrokenBarrierDisp(int cameraId, int effect) {
    typedef float Mtx[3][4];
    extern void* camGetPtr(int);
    extern void PSMTXTrans(Mtx, double, double, double);
    extern void PSMTXScale(Mtx, float, float, float);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void PSMTXRotAxisRad(Mtx, float*, float);
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
    extern unsigned char main_dl(void*, Mtx);
    extern float float_0p0056818_80424e38;
    extern float float_0p045455_80424e3c;
    extern float float_0_80424e40;
    extern float float_2p0944_80424e44;
    extern float float_neg2p0944_80424e48;
    extern float vec3_802fac5c[];
    unsigned char texObj[0x20];
    Mtx view;
    Mtx rotated;
    Mtx rotation;
    Mtx scaled;
    Mtx base;
    unsigned char* entry = (unsigned char*)effect;
    unsigned char* work = *(unsigned char**)(entry + 0xC);
    unsigned char* camera = (unsigned char*)camGetPtr(cameraId);
    unsigned int color;

    PSMTXTrans(base, *(float*)(work + 4), *(float*)(work + 8), *(float*)(work + 0xC));
    PSMTXScale(scaled, *(float*)(work + 0x60), *(float*)(work + 0x60), *(float*)(work + 0x60));
    PSMTXConcat(base, scaled, rotated);

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
    PSMTXScale(scaled, float_0p0056818_80424e38, float_0p045455_80424e3c, float_0_80424e40);
    GXLoadTexMtxImm(scaled, 0x1E, 1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 2, 12, 8, 15);
    GXSetTevAlphaIn(0, 7, 4, 4, 7);
    GXSetCullMode(0);

    PSMTXConcat((float (*)[4])(camera + 0x11C), rotated, view);
    main_dl(entry, view);
    PSMTXRotAxisRad(rotation, vec3_802fac5c, float_2p0944_80424e44);
    PSMTXConcat(rotated, rotation, view);
    PSMTXConcat((float (*)[4])(camera + 0x11C), view, view);
    main_dl(entry, view);
    PSMTXRotAxisRad(rotation, vec3_802fac5c, float_neg2p0944_80424e48);
    PSMTXConcat(rotated, rotation, view);
    PSMTXConcat((float (*)[4])(camera + 0x11C), view, view);
    main_dl(entry, view);
}

u8 main_dl(void* effect, f32 view[3][4]) {
    typedef f32 Mtx[3][4];
    extern void PSMTXTrans(Mtx,f32,f32,f32);
    extern void PSMTXRotRad(Mtx,f32,char);
    extern void PSMTXScale(Mtx,f32,f32,f32);
    extern void PSMTXConcat(Mtx,Mtx,Mtx);
    extern void GXLoadPosMtxImm(Mtx,s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetTevColor(s32,void*);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32,s32,s32);
    extern void tri2(s32,s32,s32,s32,s32,s32,s32);
    extern u8 size8x8_tex22x22_vtx[];
    u8* entry;
    u8* part;
    Mtx trans;
    Mtx rotate;
    Mtx scale;
    f32 alphaScale;
    f32 size;
    u32 color;
    void* vtx;
    s32 i;

    entry = effect;
    part = *(u8**)(entry + 0xC) + 0x6C;
    alphaScale = (f32)*(s32*)(*(u8**)(entry + 0xC) + 0x4C) / 255.0f;
    for (i = 1; i < *(s32*)(entry + 8); i++, part += 0x6C) {
        if (*(s32*)(part + 0x30) < 0) {
            continue;
        }
        PSMTXTrans(trans, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0xC));
        PSMTXRotRad(rotate, *(f32*)(part + 0x34) * 0.017453292f, 'z');
        size = *(f32*)(part + 0x60) * alphaScale;
        PSMTXScale(scale, size, size, size);
        PSMTXConcat(trans, rotate, trans);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(view, trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        color = ((u32)*(u8*)(part + 0x50) << 24) |
                ((u32)*(u8*)(part + 0x54) << 16) |
                ((u32)*(u8*)(part + 0x58) << 8) |
                (u8)((f32)*(s32*)(part + 0x4C) * alphaScale);
        GXSetTevColor(1, &color);
        switch (i & 7) {
            case 0:
                vtx = size8x8_tex22x22_vtx + 0xA8;
                break;
            case 1:
                vtx = size8x8_tex22x22_vtx + 0xE0;
                break;
            case 2:
                vtx = size8x8_tex22x22_vtx + 0x118;
                break;
            case 3:
                vtx = size8x8_tex22x22_vtx + 0x150;
                break;
            case 4:
                vtx = size8x8_tex22x22_vtx + 0x188;
                break;
            case 5:
                vtx = size8x8_tex22x22_vtx;
                break;
            case 6:
                vtx = size8x8_tex22x22_vtx + 0x38;
                break;
            case 7:
                vtx = size8x8_tex22x22_vtx + 0x70;
                break;
        }
        effSetVtxDescN64(vtx);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    }
    return 0;
}
