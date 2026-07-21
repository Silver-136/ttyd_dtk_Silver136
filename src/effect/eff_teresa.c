#include "effect/eff_teresa.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effTeresaEntry(s32 type, f32 x, f32 y, f32 z) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern const char str_Teresa_802ff24c[];
    extern void effTeresaMain(void);
    extern f32 float_30_80427ad4;
    extern f32 float_0_80427ab4;
    extern const Vec vec3_802ff228;
    extern u32 psndSFXOn_3D(s32 lookup, Vec* position);

    void* entry;
    void* work;
    Vec pos;
    f32 modeFloat;
    s32 zero;
    s32 alpha;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_Teresa_802ff24c;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x40);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effTeresaMain;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    if (type == 0) {
        modeFloat = float_30_80427ad4;
    } else {
        modeFloat = float_0_80427ab4;
    }
    alpha = (type != 0) ? 0xFF : 0;
    zero = 0;
    *(f32*)((s32)work + 0x34) = modeFloat;
    *(s32*)((s32)work + 0x30) = zero;
    *(s32*)((s32)work + 0x2C) = zero;
    *(f32*)((s32)work + 0x28) = (f32)alpha;
    *(s32*)((s32)work + 0x3C) = zero;
    if (type == 0) {
        pos = vec3_802ff228;
        pos.x = *(f32*)((s32)work + 4);
        pos.y = *(f32*)((s32)work + 8);
        pos.z = *(f32*)((s32)work + 0xC);
        psndSFXOn_3D(0x5D5, &pos);
    }
    return entry;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
/* CHATGPT STUB FILL: main/effect/eff_teresa 20260624_184823 */

/* stub-fill: effTeresaMain | prototype_only | source_prototype */
void effTeresaMain(void* effect) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    extern void* effKemuTestEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
    extern void effDelete(void* effect);
    extern u32 psndSFXOn_3D(s32 lookup, VecLocal* position);
    extern f32 dispCalcZ(VecLocal* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* dispFunc, void* data, f32 z);
    extern void effTeresaDisp(s32 cameraId, void* effect);
    extern VecLocal vec3_802ff234;
    extern VecLocal vec3_802ff240;
    extern f32 float_0p1_80427abc;
    extern f32 float_0p8_80427ac0;
    extern f32 float_1_80427ac4;
    extern f32 float_10_80427ac8;
    extern f32 float_178p5_80427acc;
    extern f32 float_0_80427ab4;
    extern f32 float_20_80427ad0;
    extern f32 float_30_80427ad4;
    extern f32 float_3_80427ad8;
    extern f32 float_0p98_80427adc;

    void* work = *(void**)((s32)effect + 0xC);
    VecLocal pos = vec3_802ff234;

    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);

    if (*(s32*)work == 0) {
        if (*(s32*)((s32)work + 0x3C) == 0) {
            if (*(f32*)((s32)work + 0x34) > float_0p1_80427abc) {
                *(f32*)((s32)work + 0x34) *= float_0p8_80427ac0;
            } else {
                VecLocal sndPos = vec3_802ff240;
                *(s32*)((s32)work + 0x3C) = 1;
                effKemuTestEntry(8, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8),
                                 *(f32*)((s32)work + 0xC), float_1_80427ac4);
                sndPos.x = *(f32*)((s32)work + 4);
                sndPos.y = *(f32*)((s32)work + 8);
                sndPos.z = *(f32*)((s32)work + 0xC);
                psndSFXOn_3D(0x5D6, &sndPos);
            }
            *(f32*)((s32)work + 0x38) += float_10_80427ac8;
            *(f32*)((s32)work + 0x28) = float_178p5_80427acc;
        } else {
            *(f32*)((s32)work + 0x38) = float_0_80427ab4;
            *(f32*)((s32)work + 0x34) += (float_20_80427ad0 - *(f32*)((s32)work + 0x34)) / float_30_80427ad4;
            *(f32*)((s32)work + 0x28) *= float_0p8_80427ac0;
            if (*(f32*)((s32)work + 0x28) < float_3_80427ad8) {
                effDelete(effect);
                return;
            }
        }
    } else {
        *(f32*)((s32)work + 0x34) += (float_20_80427ad0 - *(f32*)((s32)work + 0x34)) / float_30_80427ad4;
        *(f32*)((s32)work + 0x28) *= float_0p98_80427adc;
        if (*(f32*)((s32)work + 0x28) < float_3_80427ad8) {
            effDelete(effect);
            return;
        }
    }

    dispEntry(4, 2, effTeresaDisp, effect, dispCalcZ(&pos));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effTeresaDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct GXColor {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } GXColor;
    typedef struct EffectLike {
        char pad0[0xC];
        void* work;
    } EffectLike;
    typedef struct TeresaWork {
        s32 type;
        f32 x;
        f32 y;
        f32 z;
        char pad10[0x18];
        f32 alpha;
        s32 unk2C;
        s32 unk30;
        f32 scale;
        f32 rotZ;
        s32 unk3C;
    } TeresaWork;

    extern void* camGetPtr(s32 cameraId);
    extern void effGetTexObj(s32 texId, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 texCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void PSMTXTrans(Mtx m, double x, double y, double z);
    extern void PSMTXRotRad(Mtx m, double angle, char axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 comptype, s32 compsize, s32 frac);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 nverts);
    extern u32 dat_80427aa0;
    extern u32 unk_804298d0;
    extern f32 float_deg2rad_80427aa4;
    extern f32 float_32_80427aa8;
    extern f32 float_0p5_80427aac;
    extern f32 float_16_80427ab0;
    extern f32 float_0_80427ab4;
    extern f32 float_2_80427ab8;

    u8 texObj[0x20];
    Mtx mtx;
    Mtx rotMtx;
    Mtx scaleMtx;
    TeresaWork* work;
    void* cam;
    void* cam2;
    s32 type;
    f32 camAngle;
    f32 deg;
    f32 width;
    f32 height;
    f32 halfWidth;
    volatile f32* fifo;

    work = (TeresaWork*)((EffectLike*)effect)->work;
    cam = camGetPtr(cameraId);
    type = work->type;

    effGetTexObj(0x45, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);

    if (type == 0) {
        GXColor color;
        u32 raw;
        raw = dat_80427aa0;
        ((GXColor*)&raw)->a = (u8)(s32)work->alpha;
        color = *(GXColor*)&raw;
        GXSetChanMatColor(4, &color);
    } else {
        GXColor color;
        u32 raw;
        raw = unk_804298d0;
        ((GXColor*)&raw)->a = (u8)(s32)work->alpha;
        color = *(GXColor*)&raw;
        GXSetChanMatColor(4, &color);
    }

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);

    PSMTXTrans(mtx, (double)work->x, (double)work->y, (double)work->z);
    cam2 = camGetPtr(cameraId);
    camAngle = *(f32*)((s32)cam2 + 0x114);
    deg = float_deg2rad_80427aa4;
    camAngle = -camAngle;
    PSMTXRotRad(rotMtx, (double)(deg * camAngle), 'y');
    PSMTXConcat(mtx, rotMtx, mtx);
    PSMTXScale(scaleMtx, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, scaleMtx, mtx);
    PSMTXRotRad(rotMtx, (double)(float_deg2rad_80427aa4 * work->rotZ), 'z');
    PSMTXConcat(mtx, rotMtx, mtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXBegin(0x80, 0, 4);

    width = float_32_80427aa8;
    fifo = (volatile f32*)0xCC008000;
    halfWidth = -width;
    height = float_16_80427ab0;
    halfWidth = halfWidth * float_0p5_80427aac;

    *fifo = halfWidth;
    *fifo = height;
    *fifo = float_0_80427ab4;
    *fifo = float_0_80427ab4;
    *fifo = float_0_80427ab4;

    *fifo = height;
    *fifo = height;
    *fifo = float_0_80427ab4;
    *fifo = float_2_80427ab8;
    *fifo = float_0_80427ab4;

    *fifo = height;
    *fifo = halfWidth;
    *fifo = float_0_80427ab4;
    *fifo = float_2_80427ab8;
    *fifo = float_2_80427ab8;

    *fifo = halfWidth;
    *fifo = halfWidth;
    *fifo = float_0_80427ab4;
    *fifo = float_0_80427ab4;
    *fifo = float_2_80427ab8;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

