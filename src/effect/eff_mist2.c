#include "effect/eff_mist2.h"

/* stub-fill: effMist2Entry | missing_definition | ghidra_signature */
void* effMist2Entry(f32 x, f32 y, f32 z, f32 radius, f32 height, s32 type, s32 lifetime) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, u32);
    extern s32 irand(s32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern void effMist2Main(void*);
    extern char str_Mist2_80428270[];

    s32 count = type == 0 ? 30 : 10;
    void* effect = effEntry();
    s32* work = __memAlloc(3, (count + 1) * 0x48);
    s32* particle = work;
    s32 i;

    *(char**)((s32)effect + 0x14) = str_Mist2_80428270;
    *(s32*)((s32)effect + 8) = count + 1;
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effMist2Main;
    *(u32*)effect |= 2;
    work[0] = type;
    *(f32*)&work[1] = x;
    *(f32*)&work[2] = y;
    *(f32*)&work[3] = z;
    work[0xD] = lifetime < 1 ? 1000 : lifetime;
    work[0xC] = 0;
    *(f32*)&work[0xB] = 255.0f;
    *(f32*)&work[6] = 1.0f;
    *(u8*)((s32)work + 0x38) = 0;
    *(u8*)((s32)work + 0x39) = 0;
    *(u8*)((s32)work + 0x3A) = 0;

    for (i = 1; i < count + 1; i++) {
        f32 distance;
        f32 radians;
        particle[0x19] = (s32)radius;
        particle[0x1A] = (s32)height;
        distance = (f32)irand((s32)*(f32*)&particle[0x19]);
        radians = 6.2832f * (f32)irand(360) / 360.0f;
        *(f32*)&particle[0x13] = distance * (f32)sin(radians);
        *(f32*)&particle[0x14] = 0.0f;
        *(f32*)&particle[0x15] = distance * (f32)cos(radians);
        *(f32*)&particle[0x18] = 0.0f;
        *(f32*)&particle[0x17] = 0.5f + (f32)irand(100) / 100.0f;
        *(f32*)&particle[0x1D] = 255.0f;
        particle[0x1F] = irand(60) + 60;
        particle[0x1E] = 0;
        *(f32*)&particle[0x1C] = (f32)(5 - irand(10));
        *(f32*)&particle[0x1B] = 0.0f;
        particle[0x21] = irand(30);
        *(f32*)&particle[0x22] = 0.2f;
        *(f32*)&particle[0x23] = 0.0f;
        if (type == 1) {
            radians = 6.2832f * (f32)irand(360) / 360.0f;
            *(f32*)&particle[0x13] = *(f32*)&particle[0x19] * (f32)sin(radians);
            particle[0x14] = particle[0x1A];
            *(f32*)&particle[0x15] = *(f32*)&particle[0x19] * (f32)cos(radians);
        }
        particle += 0x12;
    }
    return effect;
}

/* stub-fill: effMist2Main | missing_definition | ghidra_signature */
void effMist2Main(void* effect) {
    extern void effDelete(void*);
    extern s32 irand(s32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f32 dispCalcZ(f32*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effMist2Disp(s32, void*);

    s32* work = *(s32**)((s32)effect + 0xC);
    s32* particle = work;
    f32 pos[3];
    s32 type = work[0];
    s32 i;

    pos[0] = *(f32*)&work[1];
    pos[1] = *(f32*)&work[2];
    pos[2] = *(f32*)&work[3];
    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        work[0xD] = 0x20;
    }
    if (work[0xD] < 1000) {
        work[0xD]--;
    }
    if (work[0xD] < 0) {
        effDelete(effect);
        return;
    }
    if (work[0xD] < 0x20) {
        *(f32*)&work[0xB] = (f32)(work[0xD] << 3);
    }
    work[0xC]++;

    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        if (particle[0x21] == 0) {
            particle[0x1F]--;
            if (particle[0x1F] < 0x20) {
                *(f32*)&particle[0x1D] = (f32)(particle[0x1F] << 3);
            }
            if (*(f32*)&particle[0x1D] > 0.0f) {
                *(f32*)&particle[0x1B] += *(f32*)&particle[0x1C];
                *(f32*)&particle[0x18] +=
                    (*(f32*)&particle[0x17] - *(f32*)&particle[0x18]) / 10.0f;
                if (type == 1) {
                    *(f32*)&particle[0x14] += *(f32*)&particle[0x23];
                    *(f32*)&particle[0x23] -= *(f32*)&particle[0x22];
                    if (*(f32*)&particle[0x14] < 0.0f) {
                        *(f32*)&particle[0x14] = 0.0f;
                    }
                }
            } else {
                f32 radius = (f32)irand((s32)*(f32*)&particle[0x19]);
                f32 radians = 6.2832f * (f32)irand(360) / 360.0f;
                *(f32*)&particle[0x13] = radius * (f32)sin(radians);
                *(f32*)&particle[0x14] = 0.0f;
                *(f32*)&particle[0x15] = radius * (f32)cos(radians);
                *(f32*)&particle[0x18] = 0.0f;
                *(f32*)&particle[0x17] = 0.5f + (f32)irand(100) / 100.0f;
                *(f32*)&particle[0x1D] = 255.0f;
                particle[0x1F] = irand(60) + 60;
                particle[0x1E] = 0;
                *(f32*)&particle[0x1C] = (f32)(5 - irand(10));
                *(f32*)&particle[0x1B] = 0.0f;
                particle[0x21] = irand(30);
                *(f32*)&particle[0x22] = 0.2f;
                *(f32*)&particle[0x23] = 0.0f;
            }
        } else {
            particle[0x21]--;
        }
        particle += 0x12;
    }
    dispEntry(4, 2, effMist2Disp, effect, dispCalcZ(pos));
}


/* CHATGPT STUB FILL: main/effect/eff_mist2 20260624_184929 */

/* stub-fill: effMist2Disp | missing_definition | ghidra_signature */
void effMist2Disp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
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
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXLoadPosMtxImm(f32[3][4], s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetChanMatColor(s32, u32*);
    extern void GXBegin(s32, s32, s32);
    extern f32 float_deg2rad_80428240;

    u8* work = *(u8**)((u8*)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    f32 base[3][4];
    f32 trans[3][4];
    f32 rot[3][4];
    f32 scaleMtx[3][4];
    u8 texObj[0x20];
    volatile f32* fifo = (volatile f32*)0xCC008000;
    f32 half = -8.0f;
    f32 baseAlpha = *(f32*)(work + 0x2C);
    s32 i;

    effGetTexObj(0x5E, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevAlphaIn(0, 7, 4, 4, 7);
    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, 0x79, float_deg2rad_80428240 * -*(f32*)((s32)camGetPtr(cameraId) + 0x114));
    PSMTXScale(scaleMtx, *(f32*)(work + 0x18), *(f32*)(work + 0x18), *(f32*)(work + 0x18));
    PSMTXConcat(trans, rot, base);
    PSMTXConcat(base, scaleMtx, base);
    PSMTXConcat((f32(*)[4])((s32)camera + 0x34), base, base);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    for (i = 1; i < *(s32*)((u8*)effect + 8); i++) {
        u8* part = work + i * 0x48;
        f32 size = *(f32*)(part + 0x18);
        u32 color;
        ((u8*)&color)[0] = *(u8*)(work + 0x38);
        ((u8*)&color)[1] = *(u8*)(work + 0x39);
        ((u8*)&color)[2] = *(u8*)(work + 0x3A);
        ((u8*)&color)[3] = (u8)(*(f32*)(part + 0x2C) * baseAlpha / 255.0f);
        PSMTXTrans(trans, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0xC));
        PSMTXScale(scaleMtx, size, size, size);
        PSMTXRotRad(rot, 0x7A, float_deg2rad_80428240 * *(f32*)(part + 0x24));
        PSMTXConcat(trans, scaleMtx, trans);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(base, trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXSetChanMatColor(4, &color);
        GXBegin(0x80, 0, 4);
        fifo[0] = half; fifo[0] = 8.0f; fifo[0] = 0.0f; fifo[0] = 0.0f; fifo[0] = 0.0f;
        fifo[0] = 8.0f; fifo[0] = 8.0f; fifo[0] = 0.0f; fifo[0] = 1.0f; fifo[0] = 0.0f;
        fifo[0] = 8.0f; fifo[0] = half; fifo[0] = 0.0f; fifo[0] = 1.0f; fifo[0] = 1.0f;
        fifo[0] = half; fifo[0] = half; fifo[0] = 0.0f; fifo[0] = 0.0f; fifo[0] = 1.0f;
    }
}

