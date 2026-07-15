#include "effect/n64/eff_fireworks_n64.h"


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effFireworksDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern double sin(f64 x);
    extern double cos(f64 x);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 tex, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 type, s32 compCnt, s32 frac);
    extern void GXSetTevColor(s32 id, void* color);
    extern s32 rand(void);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSMTX44MultVec(void* mtx, void* src, void* dst);
    extern void GXGetProjectionv(void* dst);
    extern void C_MTXOrtho(f32 top, f32 bottom, f32 left, f32 right, f32 nearZ, f32 farZ, void* mtx);
    extern void GXSetProjection(void* mtx, s32 type);
    extern void C_MTXLookAt(void* mtx, void* camPos, void* camUp, void* target);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void GXSetProjectionv(void* src);
    extern void* gp;
    extern f32 float_6p2832_804252a0;
    extern f32 float_360_804252a4;
    extern f32 float_0p5_804252a8;
    extern f32 float_480_804252ac;
    extern f32 float_0_804252b0;
    extern f32 float_608_804252b4;
    extern f32 float_1_804252b8;
    extern f32 float_32768_804252bc;
    extern f32 float_8_804252c0;

    volatile f32* fifoF;
    volatile u16* fifoU16;
    void* cam;
    void* cam3d;
    u8 texObj[0x20];
    f32 proj[7];
    f32 ortho[4][4];
    f32 look[3][4];
    f32 pos[3];
    f32 out[3];
    f32 eye[3];
    f32 up[3];
    f32 target[3];
    u8* work;
    f32 angle;
    f32 s;
    f32 c;
    f32 sx;
    f32 sy;
    f32 size;
    s32 i;
    s32 mode;
    s32 color;
    s32 count;

    fifoF = (volatile f32*)0xCC008000;
    fifoU16 = (volatile u16*)0xCC008000;
    cam = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);
    color = *(s32*)(work + 0x30);
    mode = *(s32*)(work + 0x4C);

    cam3d = camGetPtr(4);
    angle = (float_6p2832_804252a0 * -*(f32*)((s32)cam3d + 0x114)) / float_360_804252a4;
    s = (f32)sin(angle);
    c = (f32)cos(angle);

    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 2, 1, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 4, 6, 7);
    effGetTexObjN64(8, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 3, 0);

    color = (*(u8*)(work + 0x24) << 24) | (*(u8*)(work + 0x28) << 16) |
            (*(u8*)(work + 0x2C) << 8) | *(u8*)(work + 0x30);
    GXSetTevColor(1, &color);
    GXSetTevColor(2, &color);

    count = (mode == 1) ? 8 : 4;
    for (i = 0; i < count; i++) {
        if (mode == 1) {
            if ((rand() % 0x11) <= 5) {
                continue;
            }
            pos[0] = *(f32*)(work + 4) + (*(f32*)(work + 0x48) * ((f32)((i & 3) - 1) * c + (f32)((i >> 1) - 1) * s));
            pos[1] = *(f32*)(work + 8) + (*(f32*)(work + 0x48) * (f32)((i & 3) - 1));
            pos[2] = *(f32*)(work + 0xC) + (*(f32*)(work + 0x48) * (-(f32)((i & 3) - 1) * s + (f32)((i >> 1) - 1) * c));
        } else {
            pos[0] = *(f32*)(work + 0x50 + i * 4);
            pos[1] = *(f32*)(work + 0x60 + i * 4);
            pos[2] = *(f32*)(work + 0x70 + i * 4);
        }

        PSMTXMultVec((void*)((s32)cam + 0x11C), pos, out);
        PSMTX44MultVec((void*)((s32)cam + 0x15C), out, out);
        sx = out[0] * (f32)*(u16*)((s32)gp + 0x1C) * float_0p5_804252a8;
        sy = out[1] * (f32)*(u16*)((s32)gp + 0x1E) * float_0p5_804252a8;

        eye[0] = float_0_804252b0;
        eye[1] = float_0_804252b0;
        eye[2] = float_0_804252b0;
        up[0] = float_0_804252b0;
        up[1] = float_1_804252b8;
        up[2] = float_0_804252b0;
        target[0] = float_0_804252b0;
        target[1] = float_0_804252b0;
        target[2] = float_1_804252b8;

        GXGetProjectionv(proj);
        C_MTXOrtho(float_480_804252ac, float_0_804252b0, float_0_804252b0,
                   float_608_804252b4, float_1_804252b8, float_32768_804252bc, ortho);
        GXSetProjection(ortho, 1);
        C_MTXLookAt(look, up, target, eye);
        GXLoadPosMtxImm(look, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x80, 0, 4);
        size = float_8_804252c0;
        *fifoF = sx;        *fifoF = sy;        *fifoF = float_0_804252b0; *fifoU16 = 0; *fifoU16 = 1;
        *fifoF = sx + size; *fifoF = sy;        *fifoF = float_0_804252b0; *fifoU16 = 1; *fifoU16 = 1;
        *fifoF = sx + size; *fifoF = sy - size; *fifoF = float_0_804252b0; *fifoU16 = 1; *fifoU16 = 0;
        *fifoF = sx;        *fifoF = sy - size; *fifoF = float_0_804252b0; *fifoU16 = 0; *fifoU16 = 0;
        GXSetProjectionv(proj);
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void effFireworksMain(void* effect) {
    extern void effDelete(void* effect);
    extern s32 rand(void);
    extern double sin(f64 x);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effFireworksDisp(s32 cameraId, void* effect);
    extern f32 float_6p2832_804252a0;
    extern f32 float_360_804252a4;
    extern f32 float_0p95_804252c4;
    extern f32 float_0p15_804252c8;
    extern f32 float_0p11_804252cc;
    extern f32 float_80_804252d0;
    extern f32 float_0p1_804252d4;
    extern f32 float_neg1000_804252d8;

    u8* work;
    f32 pos[3];
    f32 angle;
    f32 damp;
    f32 gravity;
    f32 lerp;
    f32 s;
    s32 frame;
    s32 slot;
    s32 r;
    s32 i;
    u8* p;

    work = *(u8**)((s32)effect + 0xC);
    pos[0] = *(f32*)(work + 4);
    pos[1] = *(f32*)(work + 8);
    pos[2] = *(f32*)(work + 0xC);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x1C) = 0x10;
    }
    if (*(s32*)(work + 0x1C) < 1000) {
        (*(s32*)(work + 0x1C))--;
    }
    (*(s32*)(work + 0x20))++;

    if (*(s32*)(work + 0x1C) < 0) {
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x20);
    if (*(s32*)(work + 0x1C) < 0x20) {
        *(s32*)(work + 0x30) = *(s32*)(work + 0x1C) << 3;
    }

    damp = float_0p95_804252c4;
    gravity = float_0p15_804252c8;
    lerp = float_0p11_804252cc;

    if (*(s32*)(work + 0x4C) == 1) {
        *(f32*)(work + 4) += *(f32*)(work + 0x10);
        *(f32*)(work + 8) += *(f32*)(work + 0x14);
        *(f32*)(work + 0xC) += *(f32*)(work + 0x18);
        *(f32*)(work + 0x10) *= damp;
        *(f32*)(work + 0x14) *= damp;
        *(f32*)(work + 0x18) *= damp;
        *(f32*)(work + 0x14) -= gravity;
        *(f32*)(work + 0x44) += lerp * (*(f32*)(work + 0x48) - *(f32*)(work + 0x44));
    } else {
        slot = (frame & 3) * 4;
        angle = (float_6p2832_804252a0 * (f32)((frame * 0x5A) >> 5)) / float_360_804252a4;

        *(f32*)(work + 0x50 + slot) = *(f32*)(work + 4) - (*(f32*)(work + 0x10) * (f32)(0x20 - frame));
        s = (f32)sin(angle);
        *(f32*)(work + 0x60 + slot) =
            (*(f32*)(work + 8) - (*(f32*)(work + 0x14) * (f32)(0x20 - frame))) +
            (float_80_804252d0 * s - float_80_804252d0);
        *(f32*)(work + 0x70 + slot) = *(f32*)(work + 0xC) - (*(f32*)(work + 0x18) * (f32)(0x20 - frame));

        r = rand();
        *(f32*)(work + 0x80 + slot) = float_0p1_804252d4 * (f32)((r % 0xB) - 5);
        r = rand();
        *(f32*)(work + 0x90 + slot) = float_0p1_804252d4 * (f32)((r % 0xB) - 5);
        r = rand();
        *(f32*)(work + 0xA0 + slot) = float_0p1_804252d4 * (f32)((r % 0xB) - 5);

        if (frame < 0x1B) {
            for (i = 0; i < 4; i++) {
                p = work + (i * 4);
                *(f32*)(p + 0x50) += *(f32*)(p + 0x80);
                *(f32*)(p + 0x60) += *(f32*)(p + 0x90);
                *(f32*)(p + 0x70) += *(f32*)(p + 0xA0);
                *(f32*)(p + 0x90) -= gravity;
            }
        } else {
            for (i = 0; i < 4; i++) {
                p = work + (i * 4);
                *(f32*)(p + 0x50) += *(f32*)(p + 0x80);
                *(f32*)(p + 0x60) += *(f32*)(p + 0x90);
                *(f32*)(p + 0x70) += *(f32*)(p + 0xA0);
                *(f32*)(p + 0x90) -= gravity;
                *(f32*)(p + 0x60) = float_neg1000_804252d8;
            }
        }

        if (frame > 0x1F) {
            *(s32*)(work + 0x4C) = 1;
            *(s32*)(work + 0x20) = 1;
        }
    }

    dispEntry(4, 2, effFireworksDisp, effect, dispCalcZ(pos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effFireworksN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effFireworksMain(void*);
    extern char str_FireworksN64_802faf78[];
    extern f32 float_0_804252b0;
    extern f32 float_1000_804252dc;
    void* entry;
    u8* work;
    f32 zero;
    f32 yOff;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_FireworksN64_802faf78;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0xB0);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFireworksMain;

    *(s32*)work = type;
    *(s32*)(work + 0x20) = 0;
    if (timer <= 0) {
        *(s32*)(work + 0x1C) = 1000;
    } else {
        *(s32*)(work + 0x1C) = timer + 0x20;
    }
    *(s32*)(work + 0x4C) = 0;
    *(s32*)(work + 0x30) = 0xFF;
    zero = float_0_804252b0;
    yOff = y - float_1000_804252dc;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x44) = zero;
    *(f32*)(work + 0x10) = vx;
    *(f32*)(work + 0x14) = vy;
    *(f32*)(work + 0x18) = vz;
    *(f32*)(work + 0x48) = scale;
    *(s32*)(work + 0x24) = 0xFF;
    *(s32*)(work + 0x28) = 0xFF;
    *(s32*)(work + 0x2C) = 100;
    *(s32*)(work + 0x34) = 0xFF;
    *(s32*)(work + 0x38) = 100;
    *(s32*)(work + 0x3C) = 0x96;
    *(s32*)(work + 0x40) = 0xFF;

    *(f32*)(work + 0x50) = x;
    *(f32*)(work + 0x60) = yOff;
    *(f32*)(work + 0x70) = z;
    *(f32*)(work + 0x80) = zero;
    *(f32*)(work + 0x90) = zero;
    *(f32*)(work + 0xA0) = zero;
    *(f32*)(work + 0x54) = x;
    *(f32*)(work + 0x64) = yOff;
    *(f32*)(work + 0x74) = z;
    *(f32*)(work + 0x84) = zero;
    *(f32*)(work + 0x94) = zero;
    *(f32*)(work + 0xA4) = zero;
    *(f32*)(work + 0x58) = x;
    *(f32*)(work + 0x68) = yOff;
    *(f32*)(work + 0x78) = z;
    *(f32*)(work + 0x88) = zero;
    *(f32*)(work + 0x98) = zero;
    *(f32*)(work + 0xA8) = zero;
    *(f32*)(work + 0x5C) = x;
    *(f32*)(work + 0x6C) = yOff;
    *(f32*)(work + 0x7C) = z;
    *(f32*)(work + 0x8C) = zero;
    *(f32*)(work + 0x9C) = zero;
    *(f32*)(work + 0xAC) = zero;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

