#include "effect/eff_sleep.h"
#include <dolphin/mtx.h>

void effDelete(void* entry);
f32 dispCalcZ(Vec* pos);
void dispEntry(s32 cameraId, s32 order, void* callback, void* entry);
void effSleepDisp(void* camera, void* entry);

extern const Vec vec3_80302ac0;

/* stub-fill: effSleepEntry | missing_definition | ghidra_signature */
void* effSleepEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 angle) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effSleepMain(void* entry);
    extern char str_Sleep_804289f0;
    extern f64 cos(f64 x);
    extern f64 sin(f64 x);
    extern f32 float_6p2832_804289d4;
    extern f32 float_16_804289fc;
    extern f32 float_360_804289f8;
    extern f32 float_0_804289e4;
    extern f32 float_0p2_80428a00;
    extern f32 float_1_804289e0;

    void* entry;
    void* work;
    void* child;
    f32 rad;
    f32 radius;
    f32 angleCopy;
    s32 i;
    s32 delay;

    angleCopy = angle;
    entry = effEntry();
    *(char**)((s32)entry + 0x14) = &str_Sleep_804289f0;
    *(s32*)((s32)entry + 8) = 4;
    work = __memAlloc(3, *(s32*)((s32)entry + 8) * 0x24);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effSleepMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = angleCopy;
    *(s32*)((s32)work + 0x14) = 0;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x18) = 1000;
    } else {
        *(s32*)((s32)work + 0x18) = timer;
    }
    *(s32*)((s32)work + 0x1C) = 0;

    rad = (float_6p2832_804289d4 * angleCopy) / float_360_804289f8;
    child = (void*)((s32)work + 0x24);
    delay = 0x10;
    for (i = 1; i < *(s32*)((s32)entry + 8); i++) {
        radius = float_16_804289fc * (f32)i;
        *(f32*)((s32)child + 4) = radius * (f32)cos(rad);
        *(f32*)((s32)child + 8) = radius * (f32)sin(rad);
        *(f32*)((s32)child + 0xC) = float_0_804289e4;
        *(f32*)((s32)child + 0x10) = float_1_804289e0 + float_0p2_80428a00 * (f32)i;
        *(s32*)((s32)child + 0x14) = 0;
        *(s32*)((s32)child + 0x20) = delay;
        delay += 0x10;
        child = (void*)((s32)child + 0x24);
    }
    return entry;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effSleepMain(void* entry) {
    void* work = *(void**)((s32)entry + 0xC);
    Vec zpos;
    Vec pos = vec3_80302ac0;
    u32 flags;
    s32 value;
    s32 i;
    void* child;

    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);
    zpos = pos;

    flags = *(u32*)entry;
    if (flags & 4) {
        *(u32*)entry = flags & ~4;
        *(s32*)((s32)work + 0x18) = 0x10;
    }

    value = *(s32*)((s32)work + 0x18);
    if (value < 1000) {
        *(s32*)((s32)work + 0x18) = value - 1;
    }

    if (*(s32*)((s32)work + 0x18) < 0x20) {
        *(s32*)((s32)work + 0x14) = *(s32*)((s32)work + 0x18) << 3;
    }

    if (*(s32*)((s32)work + 0x18) < 0) {
        effDelete(entry);
    } else {
        *(s32*)((s32)work + 0x1C) = *(s32*)((s32)work + 0x1C) + 1;
        if (*(s32*)((s32)work + 0x1C) < 0x20) {
            *(s32*)((s32)work + 0x14) = (*(s32*)((s32)work + 0x1C) << 3) + 7;
        }

        i = 1;
        child = (void*)((s32)work + 0x24);
        while (i < *(s32*)((s32)entry + 8)) {
            value = *(s32*)((s32)child + 0x20);
            if (value != 0) {
                *(s32*)((s32)child + 0x20) = value - 1;
            } else {
                *(s32*)((s32)child + 0x14) = *(s32*)((s32)child + 0x14) + 8;
                if (*(s32*)((s32)child + 0x14) > 0xFF) {
                    *(s32*)((s32)child + 0x14) = 0xFF;
                }
            }
            i++;
            child = (void*)((s32)child + 0x24);
        }

        dispCalcZ(&zpos);
        dispEntry(4, 2, effSleepDisp, entry);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_sleep 20260624_184929 */

/* stub-fill: effSleepDisp | prototype_only | source_prototype */
void effSleepDisp(void* cameraArg, void* effect) {
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
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXLoadPosMtxImm(f32[3][4], s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32, s32, s32);
    extern f64 sin(f64);
    extern void* gp;
    extern f32 float_deg2rad_804289cc;
    extern u32 dat_804289c8;

    u8 texObj[0x20];
    Mtx baseTrans;
    Mtx baseScale;
    Mtx rot;
    Mtx baseMtx;
    u8* work = *(u8**)((u8*)effect + 0xC);
    u8* child = work + 0x24;
    s32 cameraId = (s32)cameraArg;
    void* camera = camGetPtr(cameraId);
    s32 baseAlpha = *(s32*)(work + 0x14);
    s32 phase = 50;
    s32 i;

    PSMTXTrans(baseTrans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXScale(baseScale, *(f32*)(work + 0x10), *(f32*)(work + 0x10), *(f32*)(work + 0x10));
    PSMTXRotRad(rot, 'y', float_deg2rad_804289cc * -*(f32*)((u8*)camera + 0x114));
    PSMTXConcat(baseTrans, rot, baseTrans);
    PSMTXConcat(baseTrans, baseScale, baseMtx);
    PSMTXConcat((f32(*)[4])((u8*)camera + 0x11C), baseMtx, baseMtx);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    effGetTexObj(0x70, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);

    for (i = 1; i < *(s32*)((u8*)effect + 8); i++, child += 0x24, phase += 50) {
        if (*(s32*)(child + 0x20) == 0) {
            Mtx trans;
            Mtx scaleMtx;
            u32 color = dat_804289c8;
            volatile f32* fifo = (volatile f32*)0xCC008000;
            s32 alpha = (*(s32*)(child + 0x14) * baseAlpha) / 0xFF;
            s32 j;

            ((u8*)&color)[3] = (u8)alpha;
            GXSetChanMatColor(4, &color);
            PSMTXTrans(trans, *(f32*)(child + 4), *(f32*)(child + 8), *(f32*)(child + 0xC));
            PSMTXScale(scaleMtx, *(f32*)(child + 0x10), *(f32*)(child + 0x10), *(f32*)(child + 0x10));
            PSMTXConcat(trans, scaleMtx, trans);
            PSMTXConcat(baseMtx, trans, trans);
            GXLoadPosMtxImm(trans, 0);
            GXSetCurrentMtx(0);
            for (j = 0; j < 8; j++) {
                f32 wave0 = 0.5f * (f32)sin(6.2832f * (f32)(*(s32*)((u8*)gp + 0x10) + phase + j * 10) * 0.015625f);
                f32 wave1 = 0.5f * (f32)sin(6.2832f * (f32)(*(s32*)((u8*)gp + 0x10) + phase + (j + 1) * 10) * 0.015625f);
                f32 left = -8.0f * 0.5f;
                f32 u0 = (f32)j;
                f32 u1 = (f32)(j + 1);
                f32 v0 = (f32)(8 - j) * 0.125f;
                f32 v1 = (f32)(7 - j) * 0.125f;
                GXBegin(0x80, 0, 4);
                *fifo = left + wave1; *fifo = u1; *fifo = 0.0f; *fifo = 0.0f; *fifo = v1;
                *fifo = 4.0f + wave1; *fifo = u1; *fifo = 0.0f; *fifo = 1.0f; *fifo = v1;
                *fifo = 4.0f + wave0; *fifo = u0; *fifo = 0.0f; *fifo = 1.0f; *fifo = v0;
                *fifo = left + wave0; *fifo = u0; *fifo = 0.0f; *fifo = 0.0f; *fifo = v0;
            }
        }
    }
}

