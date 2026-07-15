#include "effect/eff_charge.h"


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effChargeDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObjLocal { u32 dummy[8]; } GXTexObjLocal;

    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXTransApply(Mtx, Mtx, f32, f32, f32);
    extern void* camGetPtr(s32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumChans(u32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(u32);
    extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
    extern void GXSetNumTevStages(u32);
    extern void GXSetTevOrder(u32, u32, u32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(u32, s32, u32, u32, u32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(u32);
    extern void GXBegin(s32, s32, s16);
    extern u32 unk_80429610;
    extern f32 float_1_80424658;
    extern f32 float_deg2rad_8042465c;
    extern f32 float_0_80424660;
    extern f32 float_neg8_80424664;
    extern f32 float_8_80424668;
    extern f32 float_neg24_8042466c;
    extern f32 float_neg12_80424670;
    extern f32 float_12_80424674;
    extern f32 float_neg36_80424678;

    u8* work;
    u8* part;
    void* cam;
    GXTexObjLocal texObj;
    Mtx base;
    Mtx rot;
    Mtx trans;
    Mtx scale;
    Mtx local;
    u32 color;
    u32 color2;
    volatile f32* fifo;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    PSMTXScale(base, *(f32*)(work + 0x28), *(f32*)(work + 0x28), float_1_80424658);
    PSMTXTransApply(base, base, *(f32*)(work + 8), *(f32*)(work + 0xC), *(f32*)(work + 0x10));
    cam = camGetPtr(cameraId);
    PSMTXRotRad(rot, float_deg2rad_8042465c * -*(f32*)((s32)cam + 0x114), 'y');
    PSMTXConcat(base, rot, base);
    effGetTexObj(0x1E, &texObj);
    GXLoadTexObj(&texObj, 0);

    color = ((*(u32*)(work + 0x3C) & 0xFF) << 24) | ((*(u32*)(work + 0x40) & 0xFF) << 16) |
            ((*(u32*)(work + 0x44) & 0xFF) << 8) | (unk_80429610 & 0xFF);
    GXSetTevColor(2, &color);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 8, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 2, 4, 0, 0xF);
    GXSetTevAlphaIn(1, 7, 0, 0, 7);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    part = work + 0x48;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x48) {
        if (*(f32*)(part + 0x28) != float_0_80424660) {
            color2 = ((*(u32*)(work + 0x30) & 0xFF) << 24) | ((*(u32*)(work + 0x34) & 0xFF) << 16) |
                     ((*(u32*)(work + 0x38) & 0xFF) << 8) | (*(u32*)(part + 0x14) & 0xFF);
            GXSetTevColor(1, &color2);
            PSMTXTrans(trans, *(f32*)(part + 8), *(f32*)(part + 0xC), *(f32*)(part + 0x10));
            PSMTXScale(scale, *(f32*)(part + 0x28), *(f32*)(part + 0x28), *(f32*)(part + 0x28));
            PSMTXConcat(trans, scale, trans);
            PSMTXConcat(base, trans, trans);
            cam = camGetPtr(cameraId);
            PSMTXConcat((void*)((s32)cam + 0x11C), trans, local);
            GXLoadPosMtxImm(local, 0);
            GXSetCurrentMtx(0);
            GXBegin(0x80, 0, 4);
            fifo = (volatile f32*)0xCC008000;
            if ((i & 1) == 0) {
                *fifo = float_neg8_80424664; *fifo = float_neg8_80424664; *fifo = float_0_80424660; *fifo = float_0_80424660; *fifo = float_0_80424660;
                *fifo = float_8_80424668; *fifo = float_neg8_80424664; *fifo = float_0_80424660; *fifo = float_1_80424658; *fifo = float_0_80424660;
                *fifo = float_8_80424668; *fifo = float_neg24_8042466c; *fifo = float_0_80424660; *fifo = float_1_80424658; *fifo = float_1_80424658;
                *fifo = float_neg8_80424664; *fifo = float_neg24_8042466c; *fifo = float_0_80424660; *fifo = float_0_80424660; *fifo = float_1_80424658;
            } else {
                *fifo = float_neg12_80424670; *fifo = float_neg12_80424670; *fifo = float_0_80424660; *fifo = float_0_80424660; *fifo = float_0_80424660;
                *fifo = float_12_80424674; *fifo = float_neg12_80424670; *fifo = float_0_80424660; *fifo = float_1_80424658; *fifo = float_0_80424660;
                *fifo = float_12_80424674; *fifo = float_neg36_80424678; *fifo = float_0_80424660; *fifo = float_1_80424658; *fifo = float_1_80424658;
                *fifo = float_neg12_80424670; *fifo = float_neg36_80424678; *fifo = float_0_80424660; *fifo = float_0_80424660; *fifo = float_1_80424658;
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effChargeMain(void* effect) {
    typedef struct VecLocal { f32 x; f32 y; f32 z; } VecLocal;

    extern void effDelete(void*);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effChargeDisp(s32, void*);
    extern double sin(double);
    extern double cos(double);
    extern VecLocal vec3_802f9710;
    extern f32 float_0_80424660;
    extern f32 float_6p2832_8042467c;
    extern f32 float_360_80424680;
    extern f32 float_100_80424684;
    extern f32 float_0p2_80424688;
    extern f32 float_80_8042468c;

    u8* work;
    u8* part;
    VecLocal pos;
    s32 frame;
    s32 alpha;
    s32 i;
    s32 temp;
    f32 angle0;
    f32 angle1;
    f32 sin0;
    f32 cos0;
    f32 sin1;
    f32 cos1;
    f32 radius;
    s32 parity;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802f9710;
    pos.x = *(f32*)(work + 8);
    pos.y = *(f32*)(work + 0xC);
    pos.z = *(f32*)(work + 0x10);
    if (*(s32*)(work + 0x18) < 1000) {
        *(s32*)(work + 0x18) -= 1;
    }
    *(s32*)(work + 0x1C) += 1;
    if (*(s32*)(work + 0x18) < 0) {
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x1C);
    if (frame < 0x15) {
        alpha = frame * 0xC;
    } else {
        alpha = 0xFF;
    }
    if (frame & 1) {
        alpha /= 2;
    }
    *(s32*)(work + 0x14) = alpha;
    parity = frame & 1;

    part = work + 0x48;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x48) {
        radius = *(f32*)(part + 0x2C);
        angle1 = (float_6p2832_8042467c * *(f32*)(part + 0x20)) / float_360_80424680;
        sin1 = (f32)sin(angle1);
        cos1 = (f32)cos(angle1);
        angle0 = (float_6p2832_8042467c * *(f32*)(part + 0x24)) / float_360_80424680;
        sin0 = (f32)sin(angle0);
        cos0 = (f32)cos(angle0);
        *(f32*)(part + 8) = cos0 * radius * sin1;
        *(f32*)(part + 0xC) = cos0 * radius * cos1;
        *(f32*)(part + 0x10) = radius * sin0;
        temp = (s32)((float_100_80424684 - radius) * 0x1FE);
        temp = temp / 100;
        if (temp < 0) {
            temp = -temp;
        }
        *(s32*)(part + 0x14) = temp;
        if (*(s32*)(part + 0x14) > 0xFF) {
            *(s32*)(part + 0x14) = 0xFF;
        }
        *(f32*)(part + 0x28) = float_0p2_80424688 + (radius / float_80_8042468c);
        *(f32*)(part + 0x2C) = radius - (f32)(((i & 3) * 2) + 2);
        if (*(f32*)(part + 0x2C) < float_0_80424660) {
            *(f32*)(part + 0x2C) = float_0_80424660;
            *(f32*)(part + 0x28) = float_0_80424660;
        }
        if (parity) {
            *(f32*)(part + 0x20) += 5.0f;
            *(f32*)(part + 0x24) += 2.0f;
        } else {
            *(f32*)(part + 0x20) += 2.0f;
            *(f32*)(part + 0x24) += 5.0f;
        }
    }
    dispEntry(4, 2, effChargeDisp, effect, dispCalcZ(&pos));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effChargeEntry(s32 type, s32 arg, f32 x, f32 y, f32 z, f32 scale) {
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
    extern void effChargeMain(void);
    extern char str_Charge_802f9728[];

    EffectEntry* effect;
    u8* work;
    u8* particle;
    s32 i;
    s32 prev;
    s32 cur;
    f32 angle;

    effect = effEntry();
    effect->name = str_Charge_802f9728;
    effect->live = 31;
    work = __memAlloc(3, 0x8B8);
    effect->work = work;
    effect->main = effChargeMain;

    *(s32*)(work + 4) = type;
    *(s32*)(work + 0x1C) = 0;
    *(s32*)(work + 0x18) = arg;
    *(s32*)(work + 0x14) = 0;
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;
    *(f32*)(work + 0x28) = scale;

    switch (type) {
        case 0:
            *(s32*)(work + 0x30) = 255;
            *(s32*)(work + 0x34) = 0;
            *(s32*)(work + 0x38) = 255;
            *(s32*)(work + 0x3C) = 255;
            *(s32*)(work + 0x40) = 255;
            *(s32*)(work + 0x44) = 255;
            break;
        case 1:
            *(s32*)(work + 0x30) = 255;
            *(s32*)(work + 0x34) = 20;
            *(s32*)(work + 0x38) = 33;
            *(s32*)(work + 0x3C) = 50;
            *(s32*)(work + 0x40) = 20;
            *(s32*)(work + 0x44) = 10;
            break;
        default:
            *(s32*)(work + 0x30) = 0;
            *(s32*)(work + 0x34) = 0;
            *(s32*)(work + 0x38) = 0;
            *(s32*)(work + 0x3C) = 255;
            *(s32*)(work + 0x40) = 255;
            *(s32*)(work + 0x44) = 255;
            break;
    }

    particle = work + 0x48;
    for (i = 1; i < 31; i += 2) {
        prev = i - 1;
        cur = i;
        angle = 360.0f * (f32)prev;
        *(f32*)(particle + 0x20) = (angle * 9.0f) / 30.0f;
        *(f32*)(particle + 0x24) = 0.0f;
        *(f32*)(particle + 0x2C) = (f32)((prev * 100) / 30);
        angle = 360.0f * (f32)cur;
        *(s32*)particle = 0;
        *(f32*)(particle + 0x68) = (angle * 9.0f) / 30.0f;
        *(f32*)(particle + 0x6C) = 0.0f;
        *(f32*)(particle + 0x74) = (f32)((cur * 100) / 30);
        *(s32*)(particle + 0x48) = 0;
        particle += 0x90;
    }

    return effect;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

