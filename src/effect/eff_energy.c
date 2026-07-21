#include "effect/eff_energy.h"
#include <dolphin/mtx.h>

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effEnergyMain(void* entry);
void effDelete(void* entry);
f32 dispCalcZ(Vec* pos);
void dispEntry(s32 cameraId, s32 order, void* callback, void* entry);
void effEnergyDisp(void* camera, void* entry);

extern char str_Energy_80302aec[];
extern const Vec vec3_80302ae0;
extern f32 float_0_80428a18;
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effEnergyEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 field10, f32 field20, f32 field18) {
    void* entry = effEntry();
    void* work;
    f32 zero;
    s32 alpha;
    s32 zeroInt;
    s32 delta;

    *(char**)((s32)entry + 0x14) = str_Energy_80302aec;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, (u32)(*(s32*)((s32)entry + 8) * 0x34));
    *(void**)((s32)entry + 0xC) = work;
    zero = float_0_80428a18;
    *(void**)((s32)entry + 0x10) = effEnergyMain;
    alpha = 0xFF;
    zeroInt = 0;
    delta = 0x20;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x1C) = zero;
    *(f32*)((s32)work + 0x20) = field20;
    *(f32*)((s32)work + 0x10) = field10;
    *(f32*)((s32)work + 0x14) = zero;
    *(f32*)((s32)work + 0x18) = field18;
    *(s32*)((s32)work + 0x24) = alpha;
    *(s32*)((s32)work + 0x2C) = zeroInt;
    *(s32*)((s32)work + 0x30) = delta;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x28) = 1000;
    } else {
        *(s32*)((s32)work + 0x28) = timer;
    }

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effEnergyMain(void* entry) {
    Vec zpos;
    Vec pos = vec3_80302ae0;
    void* work = *(void**)((s32)entry + 0xC);
    u32 flags;
    s32 value;

    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);
    zpos = pos;

    flags = *(u32*)entry;
    if (flags & 4) {
        *(u32*)entry = flags & ~4;
        *(s32*)((s32)work + 0x28) = 0x10;
    }

    value = *(s32*)((s32)work + 0x28);
    if (value < 1000) {
        *(s32*)((s32)work + 0x28) = value - 1;
    }

    if (*(s32*)((s32)work + 0x28) < 0x10) {
        *(s32*)((s32)work + 0x24) = *(s32*)((s32)work + 0x28) << 4;
    }

    if (*(s32*)((s32)work + 0x28) < 0) {
        effDelete(entry);
    } else {
        *(f32*)((s32)work + 0x1C) = *(f32*)((s32)work + 0x1C) + *(f32*)((s32)work + 0x20);
        *(f32*)((s32)work + 0x14) = *(f32*)((s32)work + 0x14) + *(f32*)((s32)work + 0x18);
        *(s32*)((s32)work + 0x2C) = *(s32*)((s32)work + 0x2C) + *(s32*)((s32)work + 0x30);

        if (*(s32*)((s32)work + 0x2C) > 0xFF) {
            *(s32*)((s32)work + 0x2C) = 0xFF;
            *(s32*)((s32)work + 0x30) = -0x20;
        }
        if (*(s32*)((s32)work + 0x2C) < 0) {
            *(s32*)((s32)work + 0x2C) = 0;
            *(s32*)((s32)work + 0x30) = 0x20;
        }

        dispCalcZ(&zpos);
        dispEntry(4, 2, effEnergyDisp, entry);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_energy 20260624_184929 */

/* stub-fill: effEnergyDisp | prototype_only | source_prototype */
void effEnergyDisp(void* cameraArg, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXLoadPosMtxImm(f32[3][4], s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32, s32, s32);
    extern f32 float_deg2rad_80428a08;
    extern u32 color_tbl[];

    u8 texObj[0x20];
    Mtx trans;
    Mtx scaleMtx;
    Mtx rot;
    Mtx model;
    u8* work = *(u8**)((u8*)effect + 0xC);
    s32 cameraId = (s32)cameraArg;
    void* camera = camGetPtr(cameraId);
    volatile f32* fifo = (volatile f32*)0xCC008000;
    s32 type = *(s32*)work;
    u32 color;
    u32 tev0;
    u32 tev1;
    f32 scale;
    f32 halfWidth = -8.0f * 0.5f;

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    scale = *(f32*)(work + 0x14) * *(f32*)(work + 0x10);
    PSMTXScale(scaleMtx, scale, scale, scale);
    PSMTXRotRad(rot, 'y', float_deg2rad_80428a08 * -*(f32*)((u8*)camera + 0x114));
    PSMTXConcat(trans, rot, trans);
    PSMTXRotRad(rot, 'z', float_deg2rad_80428a08 * *(f32*)(work + 0x1C));
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(trans, scaleMtx, model);
    PSMTXConcat((f32(*)[4])((u8*)camera + 0x11C), model, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    ((u8*)&color)[0] = (u8)*(s32*)(work + 0x2C);
    ((u8*)&color)[1] = (u8)*(s32*)(work + 0x2C);
    ((u8*)&color)[2] = (u8)*(s32*)(work + 0x2C);
    ((u8*)&color)[3] = (u8)*(s32*)(work + 0x24);
    GXSetChanMatColor(4, &color);
    tev0 = color_tbl[type * 2];
    tev1 = color_tbl[type * 2 + 1];
    GXSetTevColor(1, &tev0);
    GXSetTevColor(2, &tev1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 2, 4, 10, 15);
    GXSetTevAlphaIn(0, 7, 4, 5, 7);
    effGetTexObj(0x71, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXBegin(0x80, 0, 4);
    *fifo = halfWidth; *fifo = 4.0f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
    *fifo = 4.0f; *fifo = 4.0f; *fifo = 0.0f; *fifo = 2.0f; *fifo = 0.0f;
    *fifo = 4.0f; *fifo = halfWidth; *fifo = 0.0f; *fifo = 2.0f; *fifo = 1.0f;
    *fifo = halfWidth; *fifo = halfWidth; *fifo = 0.0f; *fifo = 0.0f; *fifo = 1.0f;
}

