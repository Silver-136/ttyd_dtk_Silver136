#include "effect/eff_spark.h"


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effSparkMain(void* effect) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    extern void effDelete(void*);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effSparkDisp(s32, void*);
    extern double sin(double);
    extern double cos(double);
    extern s32 max_seq_num[];
    extern u8 a_data[];
    extern u8 a_data2[];
    extern f32 scale_data[];
    extern f32 scale_data2[];
    extern VecLocal vec3_802f96d0;
    extern f32 float_0p1_804245fc;
    extern f32 float_0p2_80424600;
    extern f32 float_360_80424604;
    extern f32 float_6p2832_80424608;
    extern f32 float_16_804245e8;
    extern f32 float_0_804245ec;
    extern f32 float_1_804245f8;

    u8* work;
    u8* part;
    VecLocal pos;
    s32 type;
    s32 alpha;
    s32 frame;
    s32 i;
    s32 mode;
    s32 phase;
    s32 seq;
    f32 accel;
    f32 damp;
    f32 angle;
    f32 rad;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802f96d0;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    type = *(s32*)work;
    alpha = *(s32*)(work + 0x30) & 0xFF;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x28) = 0x10;
    }
    if (*(s32*)(work + 0x28) < 1000) {
        *(s32*)(work + 0x28) -= 1;
    }
    frame = *(s32*)(work + 0x28);
    if (frame < 0) {
        effDelete(effect);
        return;
    }
    if (frame < 0x10) {
        *(s32*)(work + 0x30) = frame << 4;
    }
    *(s32*)(work + 0x2C) += 1;
    phase = *(s32*)(work + 0x2C);

    if (type == 0) {
        accel = float_0p1_804245fc;
        damp = float_0p2_80424600;
    } else {
        accel = float_0p2_80424600;
        damp = accel;
    }

    part = work + 0x38;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x38) {
        *(s32*)(part + 0x20) += 1;
        if (*(s32*)(part + 0x20) >= max_seq_num[type]) {
            *(s32*)(part + 0x20) = 0;
        }
        seq = *(s32*)(part + 0x20);
        if (seq >= 0) {
            if (seq == 0) {
                angle = (float_360_80424604 * (f32)i) / (f32)(*(s32*)((s32)effect + 8) - 1);
                rad = (float_6p2832_80424608 * angle) / float_360_80424604;
                *(f32*)(part + 4) = float_16_804245e8 * (f32)sin(rad);
                *(f32*)(part + 8) = float_16_804245e8 * (f32)cos(rad);
                *(f32*)(part + 0x10) = *(f32*)(part + 4) * accel;
                *(f32*)(part + 0x14) = *(f32*)(part + 8) * accel;
                *(f32*)(part + 0x18) = float_0_804245ec;
                *(f32*)(part + 0x1C) = float_0_804245ec;
                *(f32*)(part + 0x24) = -angle;
                *(f32*)(part + 0x34) = float_1_804245f8;
            }
            if (type == 0) {
                *(s32*)(part + 0x30) = (u32)a_data[seq];
                *(f32*)(part + 0x34) = scale_data[seq];
            } else {
                *(s32*)(part + 0x30) = (u32)a_data2[seq];
                *(f32*)(part + 0x34) = scale_data2[seq];
            }
            if ((phase & 1) == (i & 1)) {
                *(f32*)(part + 4) += *(f32*)(part + 0x10);
                *(f32*)(part + 8) += *(f32*)(part + 0x14);
            } else {
                *(f32*)(part + 4) += float_0p2_80424600 * *(f32*)(part + 0x10);
                *(f32*)(part + 8) += float_0p2_80424600 * *(f32*)(part + 0x14);
            }
            *(f32*)(part + 0x10) += damp * (*(f32*)(part + 0x18) - *(f32*)(part + 0x10));
            *(f32*)(part + 0x14) += damp * (*(f32*)(part + 0x1C) - *(f32*)(part + 0x14));
        }
    }

    mode = 1;
    if (alpha != 0xFF) {
        mode = 2;
    }
    dispEntry(4, mode, effSparkDisp, effect, dispCalcZ(&pos));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effSparkDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObjLocal { u32 dummy[8]; } GXTexObjLocal;

    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(u32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(u32);
    extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
    extern void GXSetNumTevStages(u32);
    extern void GXSetTevOrder(u32, u32, u32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(u32, s32, u32, u32, u32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(u32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXBegin(s32, s32, s16);
    extern f32 float_deg2rad_804245e4;
    extern f32 float_16_804245e8;
    extern f32 float_0_804245ec;
    extern f32 float_0p5_804245f0;
    extern f32 float_8_804245f4;
    extern f32 float_1_804245f8;
    extern u32 dat_804245e0;

    u8* work;
    u8* part;
    void* cam;
    void* cam2;
    GXTexObjLocal texObj;
    Mtx trans;
    Mtx scale;
    Mtx rot;
    Mtx base;
    Mtx local;
    f32 size;
    f32 halfNeg;
    u32 color;
    volatile f32* fifo;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    cam = camGetPtr(cameraId);
    effGetTexObj(0x1C, &texObj);
    GXLoadTexObj(&texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    size = *(f32*)(work + 0x34);
    PSMTXScale(scale, size, size, size);
    cam2 = camGetPtr(cameraId);
    PSMTXRotRad(rot, float_deg2rad_804245e4 * -*(f32*)((s32)cam2 + 0x114), 'y');
    PSMTXConcat(trans, rot, base);
    PSMTXConcat(base, scale, base);

    halfNeg = -float_16_804245e8;
    part = work + 0x38;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x38) {
        if (*(s32*)(part + 0x20) >= 0) {
            PSMTXRotRad(rot, float_deg2rad_804245e4 * *(f32*)(part + 0x24), 'z');
            size = *(f32*)(part + 0x34);
            PSMTXScale(scale, size, size, size);
            PSMTXTrans(trans, *(f32*)(part + 4), *(f32*)(part + 8), float_0_804245ec);
            PSMTXConcat(trans, rot, local);
            PSMTXConcat(local, scale, local);
            PSMTXConcat(base, local, local);
            PSMTXConcat((void*)((s32)cam + 0x11C), local, local);
            GXLoadPosMtxImm(local, 0);
            GXSetCurrentMtx(0);
            color = dat_804245e0;
            ((u8*)&color)[3] = (u8)((*(s32*)(work + 0x30) * *(s32*)(part + 0x30)) >> 8);
            GXSetChanMatColor(4, &color);
            GXBegin(0x80, 0, 4);
            size = halfNeg * float_0p5_804245f0;
            fifo = (volatile f32*)0xCC008000;
            *fifo = size;
            *fifo = float_8_804245f4;
            *fifo = float_0_804245ec;
            *fifo = float_0_804245ec;
            *fifo = float_0_804245ec;
            *fifo = float_8_804245f4;
            *fifo = float_8_804245f4;
            *fifo = float_0_804245ec;
            *fifo = float_1_804245f8;
            *fifo = float_0_804245ec;
            *fifo = float_8_804245f4;
            *fifo = size;
            *fifo = float_0_804245ec;
            *fifo = float_1_804245f8;
            *fifo = float_1_804245f8;
            *fifo = size;
            *fifo = size;
            *fifo = float_0_804245ec;
            *fifo = float_0_804245ec;
            *fifo = float_1_804245f8;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effSparkEntry(s32 type, f32 x, f32 y, f32 z) {
    typedef struct EffectEntry {
        s32 flags;
        s32 unk4;
        s32 live;
        void* work;
        void* main;
        char* name;
    } EffectEntry;
    extern EffectEntry* effEntry(void);
    extern void* __memAlloc(s32, u32);
    extern void effSparkMain(void);
    extern char str_Spark_8042460c[];
    extern s32 max_seq_num[];

    EffectEntry* effect;
    u8* work;
    f32 half;
    s32 i;

    effect = effEntry();
    effect->name = str_Spark_8042460c;
    effect->live = 6;
    work = __memAlloc(3, 0x150);
    effect->work = work;
    effect->main = effSparkMain;
    effect->flags |= 2;

    *(s32*)(work + 0x00) = type;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x28) = 1000;
    *(s32*)(work + 0x30) = 255;
    *(f32*)(work + 0x04) = x;
    *(f32*)(work + 0x08) = y;
    *(f32*)(work + 0x0C) = z;
    *(f32*)(work + 0x34) = 0.9f;

    half = (f32)max_seq_num[type] * 0.5f;
    for (i = 1; i <= 5; i++) {
        *(s32*)(work + 0x20 + (i * 0x38)) = (s32)(-1.0f - (half * (f32)(i & 1)));
    }

    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

