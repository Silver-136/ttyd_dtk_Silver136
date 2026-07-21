#include "effect/eff_toge_flush.h"
void effTogeFlushMain(void* effect);
void effTogeFlushDisp(s32 cameraId, void* effect);

extern void* effEntry(void);
extern void* __memAlloc(s32 heap, u32 size);
extern f32 float_0_804246c0;
extern f32 float_30_804246f4;
extern f32 float_0p8_804246f8;

const char str_TogeFlush_802f9748[] = "TogeFlush";
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effTogeFlushEntry(f32 x, s32 type, f32 y, s32 arg, f32 z) {
    void* entry;
    void* work;
    f32 zero;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_TogeFlush_802f9748;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x34);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effTogeFlushMain;
    zero = float_0_804246c0;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x2C) = arg;
    *(s32*)((s32)work + 0x30) = 0;
    *(s32*)((s32)work + 0x24) = 0;
    *(f32*)((s32)work + 0x10) = zero;
    if (type == 0 || type == 3) {
        *(f32*)((s32)work + 0x14) = float_30_804246f4;
    } else {
        *(f32*)((s32)work + 0x14) = zero;
    }
    *(f32*)((s32)work + 0x18) = zero;
    *(f32*)((s32)work + 0x1C) = float_0p8_804246f8;
    *(s32*)((s32)work + 0x28) = 0;
    *(f32*)((s32)work + 0x20) = zero;
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effTogeFlushMain(void* effect) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef struct TogeFlushWork {
        s32 type;
        Vec3 pos;
        f32 rot;
        f32 rotStep;
        f32 spin;
        f32 scale;
        f32 alphaScale;
        s32 alpha;
        s32 unk28;
        s32 life;
        s32 frame;
    } TogeFlushWork;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effTogeFlushDisp(void);
    extern f64 sin(f64);

    TogeFlushWork* work;
    s32 life;

    work = *(TogeFlushWork**)((s32)effect + 0xC);
    work->life--;
    work->frame++;
    life = work->life;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    if (work->frame <= 6) {
        work->alpha = (s32)((f32)(218 - work->alpha) * 0.5f + (f32)work->alpha);
    }
    if (work->frame > 6) {
        work->alpha = (s32)((f32)work->alpha * 0.78f);
    }

    work->spin += work->scale;
    work->scale += 0.29f;
    work->rot += work->rotStep;
    work->rotStep *= 0.93f;
    work->alphaScale = (0.5f * (f32)sin((6.2832f * (f32)(life * 50)) / 360.0f)) + 0.9f;
    work->unk28 = (s32)((64.0f * (f32)sin((6.2832f * (f32)(life * 40)) / 360.0f)) + 144.0f);
    dispEntry(4, 2, effTogeFlushDisp, effect, dispCalcZ(&work->pos));
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effTogeFlushDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObjLocal { u32 dummy[8]; } GXTexObjLocal;

    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
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
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(u32, s32, u32, u32, u32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void* camGetPtr(s32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(u32);
    extern void GXSetTevColor(s32, void*);
    extern void GXBegin(s32, s32, s16);
    extern u32 dat_80424698;
    extern u32 dat_8042469c;
    extern u32 dat_804246a0;
    extern u32 dat_804246a4;
    extern u32 dat_804246a8;
    extern u32 dat_804246ac;
    extern u32 dat_804246b0;
    extern u8 color_rotation_data[];
    extern f32 float_deg2rad_804246b4;
    extern f32 float_neg8_804246b8;
    extern f32 float_8_804246bc;
    extern f32 float_0_804246c0;
    extern f32 float_2_804246c4;
    extern f32 float_neg4_804246c8;
    extern f32 float_4_804246cc;

    s32* work;
    void* cam;
    GXTexObjLocal texObj;
    Mtx trans;
    Mtx rot;
    Mtx base;
    Mtx scale;
    Mtx mtx;
    s32 type;
    s32 frame;
    s32 alpha;
    s32 colorIndex;
    u32 color;
    volatile f32* fifo;

    work = *(s32**)((s32)effect + 0xC);
    type = work[0];
    frame = work[0xB];
    if (frame > 10) {
        effGetTexObj(0x19, &texObj);
    } else if (frame > 5) {
        effGetTexObj(0x1A, &texObj);
    } else {
        effGetTexObj(0x1B, &texObj);
    }
    GXLoadTexObj(&texObj, 0);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 2);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    PSMTXTrans(trans, *(f32*)(work + 1), *(f32*)(work + 2), *(f32*)(work + 3));
    cam = camGetPtr(cameraId);
    PSMTXRotRad(rot, float_deg2rad_804246b4 * -*(f32*)((s32)cam + 0x114), 'y');
    PSMTXConcat(trans, rot, base);
    PSMTXScale(scale, *(f32*)(work + 8), *(f32*)(work + 8), *(f32*)(work + 8));
    PSMTXConcat(base, scale, mtx);
    cam = camGetPtr(cameraId);
    PSMTXConcat((void*)((s32)cam + 0x11C), mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);

    alpha = work[10] & 0xFF;
    if (type == 2) {
        color = (dat_8042469c & 0xFFFFFF00) | alpha;
    } else if (type < 2) {
        if (type == 0) {
            colorIndex = (work[0xC] * 3) % 24;
            color = ((u32)color_rotation_data[colorIndex] << 24) |
                    ((u32)color_rotation_data[colorIndex + 1] << 16) |
                    ((u32)color_rotation_data[colorIndex + 2] << 8) | alpha;
        } else {
            color = (dat_80424698 & 0xFFFFFF00) | alpha;
        }
    } else if (type == 4) {
        color = (dat_804246a4 & 0xFFFFFF00) | alpha;
    } else {
        color = (dat_804246a0 & 0xFFFFFF00) | alpha;
    }
    GXSetTevColor(1, &color);
    GXBegin(0x80, 0, 4);
    fifo = (volatile f32*)0xCC008000;
    *fifo = float_neg8_804246b8; *fifo = float_8_804246bc; *fifo = float_0_804246c0; *fifo = float_0_804246c0; *fifo = float_0_804246c0;
    *fifo = float_8_804246bc; *fifo = float_8_804246bc; *fifo = float_0_804246c0; *fifo = float_2_804246c4; *fifo = float_0_804246c0;
    *fifo = float_8_804246bc; *fifo = float_neg8_804246b8; *fifo = float_0_804246c0; *fifo = float_2_804246c4; *fifo = float_2_804246c4;
    *fifo = float_neg8_804246b8; *fifo = float_neg8_804246b8; *fifo = float_0_804246c0; *fifo = float_0_804246c0; *fifo = float_2_804246c4;

    alpha = work[9];
    if (alpha > 0xFF) {
        alpha = 0xFF;
    }
    if (alpha > 0) {
        PSMTXScale(scale, *(f32*)(work + 6), *(f32*)(work + 6), *(f32*)(work + 6));
        PSMTXConcat(base, scale, mtx);
        PSMTXRotRad(rot, float_deg2rad_804246b4 * *(f32*)(work + 4), 'z');
        PSMTXConcat(mtx, rot, mtx);
        cam = camGetPtr(cameraId);
        PSMTXConcat((void*)((s32)cam + 0x11C), mtx, mtx);
        GXLoadPosMtxImm(mtx, 0);
        GXSetCurrentMtx(0);
        if (type == 4) {
            color = (dat_804246ac & 0xFFFFFF00) | (alpha & 0xFF);
        } else if ((type < 4) && (type > 2)) {
            color = (dat_804246a8 & 0xFFFFFF00) | (alpha & 0xFF);
        } else {
            color = (dat_804246b0 & 0xFFFFFF00) | (alpha & 0xFF);
        }
        GXSetTevColor(1, &color);
        GXBegin(0x80, 0, 4);
        *fifo = float_neg4_804246c8; *fifo = float_4_804246cc; *fifo = float_0_804246c0; *fifo = float_0_804246c0; *fifo = float_0_804246c0;
        *fifo = float_4_804246cc; *fifo = float_4_804246cc; *fifo = float_0_804246c0; *fifo = float_2_804246c4; *fifo = float_0_804246c0;
        *fifo = float_4_804246cc; *fifo = float_neg4_804246c8; *fifo = float_0_804246c0; *fifo = float_2_804246c4; *fifo = float_2_804246c4;
        *fifo = float_neg4_804246c8; *fifo = float_neg4_804246c8; *fifo = float_0_804246c0; *fifo = float_0_804246c0; *fifo = float_2_804246c4;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
