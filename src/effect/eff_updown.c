#include "effect/eff_updown.h"

extern void* effEntry(void);
extern void* __memAlloc(s32 heap, u32 size);
extern f32 float_1_80424624;

const char str_Updown_802f9708[] = "Updown";


void effUpdownDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObjLocal { u32 dummy[8]; } GXTexObjLocal;

    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(u32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(u32);
    extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXIdentity(Mtx);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void GXSetNumTevStages(u32);
    extern void GXSetTevOrder(u32, u32, u32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(u32, s32, u32, u32, u32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(u32);
    extern void polygon(s32, s32, f32, f32, f32, f32, f32, f32);
    extern void iconGetTexObj(void*, u16);
    extern u16 icon_id[];
    extern f32 float_3p1416_80424620;
    extern f32 float_1_80424624;
    extern f32 float_deg2rad_80424628;
    extern f32 float_neg12_8042462c;
    extern f32 float_24_80424630;
    extern f32 float_5_80424634;
    extern f32 float_0_80424638;
    extern f32 float_7_8042463c;
    extern f32 float_neg8_80424640;
    extern f32 float_16_80424644;
    extern f32 float_neg5_80424648;

    s32* work;
    void* cam;
    void* cam2;
    GXTexObjLocal texObj;
    Mtx trans;
    Mtx scale;
    Mtx rot;
    Mtx mtxA;
    Mtx mtxB;
    Mtx mtxC;
    s32 iconValue;
    s32 type;
    s32 alpha;
    s32 absIcon;
    s32 tens;

    work = *(s32**)((s32)effect + 0xC);
    cam = camGetPtr(cameraId);
    iconValue = work[6];
    type = work[0];
    alpha = *(u8*)((s32)work + 0x24);

    effGetTexObj(0x1D, &texObj);
    GXLoadTexObj(&texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    if (iconValue < 0) {
        PSMTXRotRad(rot, float_3p1416_80424620, 'z');
    } else {
        PSMTXIdentity(rot);
    }
    GXLoadTexMtxImm(rot, 0x1E, 1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    PSMTXTrans(trans, *(f32*)(work + 1), *(f32*)(work + 2), *(f32*)(work + 3));
    PSMTXScale(scale, *(f32*)(work + 4), *(f32*)(work + 5), float_1_80424624);
    cam2 = camGetPtr(cameraId);
    PSMTXRotRad(rot, float_deg2rad_80424628 * -*(f32*)((s32)cam2 + 0x114), 'y');
    PSMTXConcat(trans, scale, mtxA);
    PSMTXConcat(mtxA, rot, mtxB);
    PSMTXConcat(trans, rot, mtxC);
    PSMTXConcat((void*)((s32)cam + 0x11C), mtxB, mtxA);
    GXLoadPosMtxImm(mtxA, 0);
    GXSetCurrentMtx(0);
    if ((type == 0) || (type == 2)) {
        polygon(1, alpha, float_neg12_8042462c, float_24_80424630, float_24_80424630, float_24_80424630, float_1_80424624, float_1_80424624);
    } else {
        polygon(2, alpha, float_neg12_8042462c, float_24_80424630, float_24_80424630, float_24_80424630, float_1_80424624, float_1_80424624);
    }

    absIcon = iconValue;
    if (absIcon < 0) {
        absIcon = -absIcon;
    }
    tens = absIcon / 10;
    iconGetTexObj(&texObj, icon_id[absIcon - (tens * 10)]);
    GXLoadTexObj(&texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    if (iconValue < 0) {
        PSMTXTrans(trans, float_5_80424634, float_7_8042463c, float_1_80424624);
    } else {
        PSMTXTrans(trans, float_5_80424634, float_0_80424638, float_1_80424624);
    }
    if ((u32)work[8] < 0xF) {
        PSMTXConcat(mtxB, trans, mtxA);
    } else {
        PSMTXConcat(mtxC, trans, mtxA);
    }
    PSMTXConcat((void*)((s32)cam + 0x11C), mtxA, mtxA);
    GXLoadPosMtxImm(mtxA, 0);
    polygon(0, alpha, float_neg8_80424640, float_16_80424644, float_16_80424644, float_16_80424644, float_1_80424624, float_1_80424624);

    if (iconValue < 0) {
        iconGetTexObj(&texObj, 0x1F6);
        GXLoadTexObj(&texObj, 0);
        PSMTXTrans(trans, float_neg5_80424648, float_7_8042463c, float_1_80424624);
    } else {
        iconGetTexObj(&texObj, 0x1F5);
        GXLoadTexObj(&texObj, 0);
        PSMTXTrans(trans, float_neg5_80424648, float_0_80424638, float_1_80424624);
    }
    if ((u32)work[8] < 0xF) {
        PSMTXConcat(mtxB, trans, mtxA);
    } else {
        PSMTXConcat(mtxC, trans, mtxA);
    }
    PSMTXConcat((void*)((s32)cam + 0x11C), mtxA, mtxA);
    GXLoadPosMtxImm(mtxA, 0);
    polygon(0, alpha, float_neg8_80424640, float_16_80424644, float_16_80424644, float_16_80424644, float_1_80424624, float_1_80424624);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
void polygon(s32 type, s32 alpha, f32 x, f32 y, f32 w, f32 h, f32 s, f32 t) {
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 nverts);

    volatile f32* gxFifoF = (volatile f32*)0xCC008000;
    volatile u8* gxFifoB = (volatile u8*)0xCC008000;
    u8 r1;
    u8 g1;
    u8 b1;
    u8 a;
    u8 r2;
    u8 g2;
    u8 b2;
    f32 x2;
    f32 y2;

    switch (type) {
        case 0:
            b1 = 255;
            g1 = 255;
            r1 = 255;
            a = alpha;
            b2 = 255;
            g2 = 255;
            r2 = 255;
            break;
        case 1:
            r1 = 233;
            g1 = 74;
            b1 = 63;
            a = alpha;
            r2 = 254;
            g2 = 230;
            b2 = 62;
            break;
        case 2:
            r1 = 63;
            g1 = 63;
            b1 = 233;
            a = alpha;
            r2 = 116;
            g2 = 201;
            b2 = 255;
            break;
    }

    GXBegin(0x80, 0, 4);
    x2 = x + w;
    y2 = y - h;
    *gxFifoF = x;
    *gxFifoF = y;
    *gxFifoF = 0.0f;
    *gxFifoB = r1;
    *gxFifoB = g1;
    *gxFifoB = b1;
    *gxFifoB = a;
    *gxFifoF = 0.0f;
    *gxFifoF = 0.0f;
    *gxFifoF = x2;
    *gxFifoF = y;
    *gxFifoF = 0.0f;
    *gxFifoB = r1;
    *gxFifoB = g1;
    *gxFifoB = b1;
    *gxFifoB = a;
    *gxFifoF = s;
    *gxFifoF = 0.0f;
    *gxFifoF = x2;
    *gxFifoF = y2;
    *gxFifoF = 0.0f;
    *gxFifoB = r2;
    *gxFifoB = g2;
    *gxFifoB = b2;
    *gxFifoB = a;
    *gxFifoF = s;
    *gxFifoF = t;
    *gxFifoF = x;
    *gxFifoF = y2;
    *gxFifoF = 0.0f;
    *gxFifoB = r2;
    *gxFifoB = g2;
    *gxFifoB = b2;
    *gxFifoB = a;
    *gxFifoF = 0.0f;
    *gxFifoF = t;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effUpdownMain(void* effect) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef struct UpdownWork {
        s32 type;
        Vec3 pos;
        f32 scaleX;
        f32 scaleY;
        s32 life;
        s32 kind;
        s32 frame;
        u8 alpha;
    } UpdownWork;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effUpdownDisp(void);
    extern u8 scale_data[];
    extern f64 sin(f64);

    UpdownWork* work;
    s32 drawMode;
    s32 flags;
    s32 type;

    flags = *(s32*)effect;
    work = *(UpdownWork**)((s32)effect + 0xC);
    type = work->type;
    if (flags & 4) {
        *(s32*)effect = flags & ~4;
        work->life = 16;
    }
    if (work->life < 1000) {
        work->life--;
    }
    if (work->life < 0) {
        effDelete(effect);
        return 0;
    }
    if (work->life < 16) {
        work->alpha = work->life << 4;
    }
    if (type == 0 || type == 1) {
        if (work->frame < 15) {
            work->scaleX = (f32)scale_data[work->frame * 2] / 100.0f;
            work->scaleY = (f32)scale_data[work->frame * 2 + 1] / 100.0f;
        }
        else {
            work->scaleX = 1.0f;
            work->scaleY = 1.0f - (0.1f * (f32)sin((f32)(work->frame - 15) * 0.25f));
        }
    }
    work->frame++;
    drawMode = 1;
    if (work->alpha != 0xFF) {
        drawMode = 2;
    }
    dispEntry(4, drawMode, effUpdownDisp, effect, dispCalcZ(&work->pos));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effUpdownEntry(f32 x, s32 type, f32 y, s32 arg1, f32 z, s32 arg2) {
    void* entry;
    void* work;
    s32 main;
    f32 one;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_Updown_802f9708;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x28);
    *(void**)((s32)entry + 0xC) = work;
    main = (s32)effUpdownMain;
    one = float_1_80424624;
    *(s32*)((s32)entry + 0x10) = main;
    *(u32*)entry |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x14) = one;
    *(f32*)((s32)work + 0x10) = one;
    *(s32*)((s32)work + 0x18) = arg1;
    *(s32*)((s32)work + 0x1C) = arg2;
    *(s32*)((s32)work + 0x20) = 0;
    *(u8*)((s32)work + 0x24) = 0xFF;
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
