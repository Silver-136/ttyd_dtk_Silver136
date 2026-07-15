#include "effect/eff_machinegun.h"
#include <dolphin/mtx.h>

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effMachinegunMain(void* entry);
void effMachinegunDisp(void* camera, void* entry);
void effDelete(void* entry);
f32 dispCalcZ(Vec* pos);
void dispEntry(s32 cameraId, s32 order, void* callback, void* entry);
f64 cos(f64 value);
f64 sin(f64 value);

extern char str_Machinegun_80302a8c[];
extern Vec vec3_80302a80;
extern f32 float_6p2832_804289a0;
extern f32 float_360_804289a4;
extern f32 float_0p9_804289a8;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMachinegunMain(void* entry) {
    Vec pos = vec3_80302a80;
    Vec zpos;
    void* work = *(void**)((s32)entry + 0xC);

    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);
    zpos = pos;

    *(f32*)((s32)work + 4) += *(f32*)((s32)work + 0x18) * (f32)cos((float_6p2832_804289a0 * *(f32*)((s32)work + 0x14)) / float_360_804289a4);
    *(f32*)((s32)work + 8) += *(f32*)((s32)work + 0x18) * (f32)sin((float_6p2832_804289a0 * *(f32*)((s32)work + 0x14)) / float_360_804289a4);
    *(f32*)((s32)work + 0x18) *= float_0p9_804289a8;
    *(s32*)((s32)work + 0x1C) -= 10;

    if (*(s32*)((s32)work + 0x1C) < 0) {
        effDelete(entry);
    } else {
        dispCalcZ(&zpos);
        dispEntry(4, 2, effMachinegunDisp, entry);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma use_lmw_stmw off
void* effMachinegunEntry(s32 type, f32 x, f32 y, f32 z, f32 angle, f32 speed, f32 field10) {
    void* entry = effEntry();
    void* work;

    *(char**)((s32)entry + 0x14) = str_Machinegun_80302a8c;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x28);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMachinegunMain;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x14) = angle;
    *(f32*)((s32)work + 0x10) = field10;
    *(s32*)((s32)work + 0x1C) = 0xFF;
    *(f32*)((s32)work + 0x18) = speed;

    return entry;
}
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_machinegun 20260624_184929 */

/* stub-fill: effMachinegunDisp | prototype_only | source_prototype */
void effMachinegunDisp(void* cameraArg, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
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
    extern f32 float_deg2rad_80428984;
    extern u32 str_tl_80428980;

    u8 texObj[0x20];
    Mtx trans;
    Mtx rot;
    Mtx scaleMtx;
    u8* work = *(u8**)((u8*)effect + 0xC);
    s32 cameraId = (s32)cameraArg;
    void* camera = camGetPtr(cameraId);
    volatile f32* fifo = (volatile f32*)0xCC008000;
    u32 color = str_tl_80428980;
    f32 scale;
    f32 halfWidth = -32.0f * 0.5f;

    effGetTexObj(0x6D, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    ((u8*)&color)[3] = (u8)*(s32*)(work + 0x1C);
    GXSetChanMatColor(4, &color);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 10);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    scale = *(f32*)(work + 0x10);
    PSMTXScale(scaleMtx, scale, scale, scale);
    PSMTXRotRad(rot, 'y', float_deg2rad_80428984 * -*(f32*)((u8*)camera + 0x114));
    PSMTXConcat(trans, rot, trans);
    PSMTXRotRad(rot, 'z', float_deg2rad_80428984 * (180.0f + *(f32*)(work + 0x14)));
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(trans, scaleMtx, trans);
    PSMTXConcat((f32(*)[4])((u8*)camera + 0x11C), trans, trans);
    GXLoadPosMtxImm(trans, 0);
    GXSetCurrentMtx(0);
    GXBegin(0x80, 0, 4);
    *fifo = halfWidth; *fifo = 16.0f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
    *fifo = 16.0f; *fifo = 16.0f; *fifo = 0.0f; *fifo = 2.0f; *fifo = 0.0f;
    *fifo = 16.0f; *fifo = halfWidth; *fifo = 0.0f; *fifo = 2.0f; *fifo = 2.0f;
    *fifo = halfWidth; *fifo = halfWidth; *fifo = 0.0f; *fifo = 0.0f; *fifo = 2.0f;
}

