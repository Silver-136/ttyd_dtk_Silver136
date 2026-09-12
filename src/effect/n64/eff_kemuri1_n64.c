#include "effect/n64/eff_kemuri1_n64.h"

void* effKemuri1N64Entry(f32 x, f32 y, f32 z, f32 angle, s32 type) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effKemuri1Main(void*);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f32 reviseAngle(f32);
    extern char str_Kemuri1N64_802fb1c8[];
    void* entry = effEntry();
    u8* work;
    u8* part;
    s32 count;
    s32 i;

    if (type < 3) {
        if (type < 2) count = 3;
        else count = 1;
    } else if (type == 3) {
        count = 4;
    } else if (type < 8) {
        count = 9;
    } else {
        count = 3;
    }
    *(char**)((u8*)entry + 0x14) = str_Kemuri1N64_802fb1c8;
    *(s32*)((u8*)entry + 8) = count;
    work = __memAlloc(3, count * 0x50);
    *(u8**)((u8*)entry + 0xC) = work;
    *(void**)((u8*)entry + 0x10) = effKemuri1Main;
    if (type < 3) {
        if (type < 2) {
            if (type > -1) {
                *(s32*)(work + 0x38) = 0;
                *(s32*)(work + 0x4C) = 0xF;
            }
        } else {
            *(s32*)(work + 0x38) = 3;
            *(s32*)(work + 0x4C) = 0x41;
        }
    } else if (type == 3) {
        *(s32*)(work + 0x38) = 2;
        *(s32*)(work + 0x4C) = 0x81;
    } else if (type < 8) {
        *(s32*)(work + 0x38) = 3;
        *(s32*)(work + 0x4C) = 0x41;
    }
    *(s32*)(work + 0x3C) = 0;
    *(s16*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    part = work + 0x50;
    if (type < 3) {
        if (type < 2 && type > -1) {
            for (i = 1; i < count; i++, part += 0x50) {
                f32 direction;
                f32 rad;
                *(f32*)(part + 0x10) = -3.0f;
                *(f32*)(part + 0x14) = 0.3f;
                *(f32*)(part + 0x18) = 0.0f;
                *(f32*)(part + 0x1C) = 0.0f;
                *(f32*)(part + 0x20) = 1.75f;
                *(f32*)(part + 0x24) = -0.16f;
                *(f32*)(part + 0x28) = 0.0f;
                *(f32*)(part + 0x2C) = 0.0f;
                *(f32*)(part + 4) = 0.0f;
                *(f32*)(part + 8) = 0.0f;
                *(f32*)(part + 0xC) = 4.0f;
                direction = reviseAngle(180.0f * (f32)(i - 1) + 90.0f);
                rad = 6.2832f * direction / 360.0f;
                *(f32*)(part + 0x30) = (f32)sin(rad);
                *(f32*)(part + 0x34) = (f32)cos(rad);
                *(u8*)(part + 0x44) = 0xFF;
                *(u8*)(part + 0x45) = 0xFF;
                *(u8*)(part + 0x46) = 0xFF;
            }
        }

    } else if (type == 3) {
        for (i = 1; i < count; i++, part += 0x50) {
            f32 rad;
            if (i - 1 == 1) {
                rad = -2.3562f;
                *(f32*)(part + 8) = 4.0f;
                *(f32*)(part + 0x20) = 0.85f;
            } else if (i - 1 < 1) {
                rad = 2.3562f;
                *(f32*)(part + 8) = 0.0f;
                *(f32*)(part + 0x20) = 0.95f;
            } else {
                rad = 0.34907f;
                *(f32*)(part + 8) = 10.0f;
                *(f32*)(part + 0x20) = 0.75f;
            }
            *(f32*)(part + 4) = 25.0f * -(f32)sin(rad);
            *(f32*)(part + 0xC) = 25.0f * -(f32)cos(rad);
            *(f32*)(part + 0x24) = -0.04f;
            *(f32*)(part + 0x28) = 0.94f;
            *(f32*)(part + 0x30) = 0.0f;
            *(f32*)(part + 0x34) = 0.0f;
            *(u8*)(part + 0x44) = 0xFF;
            *(u8*)(part + 0x45) = 0xFF;
            *(u8*)(part + 0x46) = 0xFF;
        }

    } else if (type < 8) {
        for (i = 1; i < count; i++, part += 0x50) {
            f32 rad;
            *(f32*)(part + 0x10) = -3.0f;
            *(f32*)(part + 0x14) = 0.93f;
            *(f32*)(part + 0x20) = 0.0f;
            *(f32*)(part + 0x24) = 0.1f;
            *(f32*)(part + 0x28) = 0.94f;
            *(f32*)(part + 0x2C) = 0.94f;
            rad = 6.2832f * (angle + (f32)(((i - 1) * 360) / (count - 1))) / 360.0f;
            *(f32*)(part + 4) = 10.0f * -(f32)sin(rad);
            *(f32*)(part + 8) = 0.0f;
            *(f32*)(part + 0xC) = 10.0f * -(f32)cos(rad);
            *(f32*)(part + 0x30) = (f32)sin(rad);
            *(f32*)(part + 0x34) = (f32)cos(rad);
            if (type == 6) {
                *(u8*)(part + 0x44) = 0x21;
                *(u8*)(part + 0x45) = 0x8C;
                *(u8*)(part + 0x46) = 0x8D;
            } else if (type == 5) {
                *(u8*)(part + 0x44) = 0xDD;
                *(u8*)(part + 0x45) = 0xB7;
                *(u8*)(part + 0x46) = 5;
            } else {
                *(u8*)(part + 0x44) = 0xFF;
                *(u8*)(part + 0x45) = 0xFF;
                *(u8*)(part + 0x46) = 0xFF;
            }
        }

    }
    return entry;
}

void effKemuri1Main(void* effect) {
    typedef struct Vec3 { f32 x; f32 y; f32 z; } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effKemuri1Disp(s32, s32);
    extern Vec3 vec3_802fb1b0[];
    extern f32 float_16_804255c8;
    u8* work;
    Vec3 pos;
    s32 oldFrame;
    s32 nextFrame;
    s32 texture1;
    s32 texture0;
    s32 i;
    u16 type;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb1b0[0];
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    oldFrame = *(s32*)(work + 0x3C);
    nextFrame = oldFrame + 1;
    type = *(u16*)work;
    texture1 = (s32)(float_16_804255c8 * (f32)nextFrame / (f32)*(s32*)(work + 0x4C));
    texture0 = (s32)(float_16_804255c8 * (f32)oldFrame / (f32)*(s32*)(work + 0x4C));
    *(s32*)(work + 0x3C) = nextFrame;
    if (texture1 > 15) {
        texture1 = 15;
    }
    if (texture0 >= 16) {
        effDelete(effect);
        return;
    }
    *(s32*)(work + 0x40) = texture0;
    *(s32*)(work + 0x48) = texture1;
    work += 0x50;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, work += 0x50) {
        if (type == 3) {
            *(f32*)(work + 8) += *(f32*)(work + 0x20);
            *(f32*)(work + 0x20) += *(f32*)(work + 0x24);
            *(f32*)(work + 0x24) *= *(f32*)(work + 0x28);
        } else if (type < 3) {
            if (type < 2) {
                *(f32*)(work + 0x18) += *(f32*)(work + 0x1C);
                *(f32*)(work + 0x14) += *(f32*)(work + 0x18);
                *(f32*)(work + 0x10) += *(f32*)(work + 0x14);
                *(f32*)(work + 4) += *(f32*)(work + 0x10) * *(f32*)(work + 0x30);
                *(f32*)(work + 0xC) += *(f32*)(work + 0x10) * *(f32*)(work + 0x34);
                *(f32*)(work + 0x28) += *(f32*)(work + 0x2C);
                *(f32*)(work + 0x24) += *(f32*)(work + 0x28);
                *(f32*)(work + 0x20) += *(f32*)(work + 0x24);
                *(f32*)(work + 8) += *(f32*)(work + 0x20);
            }
        } else if (type < 8) {
            *(f32*)(work + 4) += *(f32*)(work + 0x10) * *(f32*)(work + 0x30);
            *(f32*)(work + 0xC) += *(f32*)(work + 0x10) * *(f32*)(work + 0x34);
            *(f32*)(work + 0x10) *= *(f32*)(work + 0x14);
            *(f32*)(work + 8) += *(f32*)(work + 0x20);
            *(f32*)(work + 0x20) += *(f32*)(work + 0x24);
            *(f32*)(work + 0x24) *= *(f32*)(work + 0x28);
            *(f32*)(work + 0x20) *= *(f32*)(work + 0x2C);
        }
    }
    if (*(s32*)((s32)effect + 8) - 1 <= 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effKemuri1Disp, effect, dispCalcZ(&pos));
    }
}


void effKemuri1Disp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXBegin(s32, s32, s32);
    extern u32 dat_804255a8;
    extern u32 dat_804255ac;
    extern f32 float_1_804255b0;
    extern f32 float_0p5_804255b4;
    extern f32 float_deg2rad_804255b8;
    extern f32 float_0p0625_804255bc;
    extern f32 float_0_804255c0;
    extern f32 float_2_804255c4;
    extern f32 float_16_804255c8;
    extern f32 float_24_804255cc;
    extern f32 float_32_804255d0;
    u8* w = *(u8**)((u8*)effect + 0xC);
    u8* p = w + 0x50;
    void* cam = camGetPtr(cameraId);
    Mtx base, m, r, s;
    u8 tex[0x20];
    u32 color;
    s32 i, j;
    s32 type = *(u16*)w;
    s32 texture0 = *(s32*)(w + 0x40);
    s32 texture1 = *(s32*)(w + 0x48);
    f32 size;
    f32 half = 0.5f;
    f32 scale = (type >= 5 && type < 8) ? 0.5f : 1.0f;
    f32 u0 = 0.0625f * (f32)texture0;
    f32 u1 = 0.0625f * (f32)texture1;
    volatile f32* fifo = (volatile f32*)0xCC008000;

    PSMTXTrans(m, *(f32*)(w + 4), *(f32*)(w + 8), *(f32*)(w + 0xC));
    PSMTXRotRad(r, 0x79, -*(f32*)((u8*)camGetPtr(cameraId) + 0x114) * 0.0174533f);
    PSMTXScale(s, scale, scale, scale);
    PSMTXConcat(m, r, m);
    PSMTXConcat(m, s, m);
    PSMTXConcat((u8*)cam + 0x11C, m, base);

    effGetTexObj(0x28, tex);
    GXLoadTexObj(tex, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevOrder(1, 1, 0, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0, 8, 2, 15);
    GXSetTevAlphaIn(1, 0, 4, 1, 7);
    color = 0x80808080;
    GXSetTevColor(1, &color);
    GXSetTevOrder(2, 0xFF, 0xFF, 4);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 15, 0, 10, 15);
    GXSetTevAlphaIn(2, 7, 0, 5, 7);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    for (i = 1; i < *(s32*)((u8*)effect + 8); i++, p += 0x50) {
        if (*(s32*)(p + 0x40) >= 16) {
            continue;
        }
        PSMTXTrans(m, u0, 0.0f, 0.0f);
        PSMTXScale(s, 0.0625f, 1.0f, 1.0f);
        PSMTXConcat(m, s, m);
        GXLoadTexMtxImm(m, 0x1E, 1);
        PSMTXTrans(m, u1, 0.0f, 0.0f);
        PSMTXScale(s, 0.0625f, 1.0f, 1.0f);
        PSMTXConcat(m, s, m);
        GXLoadTexMtxImm(m, 0x21, 1);
        for (j = 0; j < 2; j++) {
            if (j == 0) {
                color = 0xFFFFFF60;
                GXSetChanMatColor(4, &color);
                PSMTXTrans(m, *(f32*)(p + 4) + 2.0f, *(f32*)(p + 8) - 2.0f, *(f32*)(p + 0xC));
            } else {
                color = ((u8)*(p + 0x44) << 24) | ((u8)*(p + 0x45) << 16) |
                        ((u8)*(p + 0x46) << 8) | 0xFF;
                GXSetChanMatColor(4, &color);
                PSMTXTrans(m, *(f32*)(p + 4), *(f32*)(p + 8), *(f32*)(p + 0xC));
            }
            PSMTXConcat(base, m, m);
            GXLoadPosMtxImm(m, 0);
            GXBegin(0x80, 0, 4);
            if (type == 1) {
                size = float_24_804255cc;
            } else if (type == 0) {
                size = float_16_804255c8;
            } else {
                size = float_32_804255d0;
            }
            *fifo = -size * half; *fifo = size * half; *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
            *fifo = size * half;  *fifo = size * half; *fifo = 0.0f; *fifo = 1.0f; *fifo = 0.0f;
            *fifo = size * half;  *fifo = -size * half; *fifo = 0.0f; *fifo = 1.0f; *fifo = 1.0f;
            *fifo = -size * half; *fifo = -size * half; *fifo = 0.0f; *fifo = 0.0f; *fifo = 1.0f;
        }
    }
}
