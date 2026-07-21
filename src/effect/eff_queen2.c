#include "effect/eff_queen2.h"
#include <dolphin/mtx.h>

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effQueen2Main(void* entry);
void* camGetPtr(s32 cameraId);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
void PSMTXConcat(void* a, void* b, void* out);
f32 PSVECDistance(Vec* a, Vec* b);
void GXSetNumChans(s32 num);
void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
void GXSetChanMatColor(s32 chan, void* color);
void GXSetNumTexGens(s32 num);
void GXSetNumTevStages(s32 num);
void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 color);
void GXSetTevOp(s32 stage, s32 mode);
void GXSetCullMode(s32 mode);
void GXClearVtxDesc(void);
void GXSetVtxDesc(s32 attr, s32 type);
void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
void GXLoadPosMtxImm(void* m, s32 id);
void GXSetCurrentMtx(s32 id);
void GXDrawSphere(s32 rows, s32 cols);

extern u32 unk_80429908;
extern f32 float_10_80428a78;
extern const Vec vec3_80302b58;
extern char str_Queen2_80302b70[];
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effQueen2Entry(s32 type, f32 x, f32 y, f32 z) {
    void* entry = effEntry();
    void* work;
    Vec pos;
    const Vec* base;
    void* camera;
    f32 dist;
    f32 scale;
    s32 zero;

    *(char**)((s32)entry + 0x14) = str_Queen2_80302b70;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, (u32)(*(s32*)((s32)entry + 8) * 0x2C));
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effQueen2Main;
    base = &vec3_80302b58;
    *(u32*)entry |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    pos = *base;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    camera = camGetPtr(4);
    dist = PSVECDistance((Vec*)((s32)camera + 0xC), &pos);
    scale = dist / float_10_80428a78;
    zero = 0;
    *(f32*)((s32)work + 0x14) = scale;
    *(f32*)((s32)work + 0x10) = *(f32*)((s32)work + 0x14);
    *(s32*)((s32)work + 0x18) = zero;
    *(s32*)((s32)work + 0x24) = zero;
    *(s32*)((s32)work + 0x20) = zero;
    *(s32*)((s32)work + 0x28) = zero;

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_queen2 20260624_185035 */

/* stub-fill: effQueen2Main | prototype_only | source_prototype */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effQueen2Main(void* entry) {
    extern f32 intplGetValue(s32 type, s32 current, f32 start, f32 end, s32 max);
    extern void* effAkariChargeN64Entry(s32 type, s32 unused, f32 x, f32 y, f32 z, f32 scale);
    extern void* effTeresaEntry(s32 type, f32 x, f32 y, f32 z);
    extern void* effIndirectEntry(s32 type, s32 timer, f32 x, f32 y, f32 z);
    extern void effSoftDelete(void* entry);
    extern void effDelete(void* entry);
    extern f32 dispCalcZ(Vec* pos);
    extern void dispEntry(s32 cameraId, s32 order, void* callback, f32 priority, void* param);
    extern void effQueen2Disp(s32 cameraId, void* entry);
    extern f32 float_0_80428a7c;
    extern f32 float_128_80428a80;
    extern f32 float_3_80428a84;
    extern f32 float_1_80428a88;
    extern const Vec vec3_80302b64;

    void* work;
    void* child;
    const Vec* base;
    Vec pos;

    work = *(void**)((s32)entry + 0xC);
    base = &vec3_80302b64;
    pos = *base;
    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);

    if (*(u32*)entry & 4) {
        *(u32*)entry &= ~4;
        if (*(void**)((s32)work + 0x28) != 0) {
            effDelete(*(void**)((s32)work + 0x28));
        }
        effDelete(entry);
        return;
    }

    switch (*(s32*)((s32)work + 0x24)) {
        case 0:
            *(s32*)((s32)work + 0x20) += 1;
            *(s32*)((s32)work + 0x18) =
                (s32)intplGetValue(0xB, *(s32*)((s32)work + 0x20), float_0_80428a7c, float_128_80428a80, 0x3C);
            if (*(s32*)((s32)work + 0x20) > 0x3C) {
                *(s32*)((s32)work + 0x20) = 0;
                *(s32*)((s32)work + 0x24) += 1;
            }
            break;
        case 1:
            *(s32*)((s32)work + 0x20) += 1;
            *(f32*)((s32)work + 0x10) =
                intplGetValue(0xB, *(s32*)((s32)work + 0x20), *(f32*)((s32)work + 0x14), float_3_80428a84, 0x78);
            if (*(s32*)((s32)work + 0x20) > 0x78) {
                *(s32*)((s32)work + 0x20) = 0;
                *(s32*)((s32)work + 0x24) += 1;
                child = effAkariChargeN64Entry(0, 0, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8),
                                               *(f32*)((s32)work + 0xC), float_1_80428a88);
                *(void**)((s32)work + 0x28) = child;
                child = *(void**)((s32)child + 0xC);
                *(s32*)((s32)child + 0x18) = 0;
                *(s32*)((s32)child + 0x1C) = 0x9A;
                *(s32*)((s32)child + 0x20) = 0xFF;
                *(s32*)((s32)child + 0x28) = 0xFF;
                *(s32*)((s32)child + 0x2C) = 0xFF;
                *(s32*)((s32)child + 0x30) = 0xFF;
            }
            break;
        case 2:
            *(s32*)((s32)work + 0x20) += 1;
            if (*(s32*)((s32)work + 0x20) > 0x78) {
                *(s32*)((s32)work + 0x20) = 0;
                *(s32*)((s32)work + 0x24) += 1;
                effTeresaEntry(1, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
                effIndirectEntry(1, 0x3C, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
            }
            break;
        case 3:
            *(s32*)((s32)work + 0x20) += 1;
            if (*(s32*)((s32)work + 0x20) > 0x1E) {
                *(s32*)((s32)work + 0x20) = 0;
                *(s32*)((s32)work + 0x24) += 1;
                effTeresaEntry(1, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
                effIndirectEntry(1, 0x3C, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
                effSoftDelete(*(void**)((s32)work + 0x28));
            }
            break;
        case 4:
            effDelete(entry);
            return;
    }
    if (*(s32*)((s32)work + 0x24) >= 3) {
        *(s32*)((s32)work + 0x18) -= 2;
        if (*(s32*)((s32)work + 0x18) < 0) {
            *(s32*)((s32)work + 0x18) = 0;
        }
    }
    dispCalcZ(&pos);
    dispEntry(4, 2, effQueen2Disp, 0.0f, entry);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effQueen2Disp(s32 cameraId, void* entry) {
    u32 color;
    u32 tempColor;
    Mtx trans;
    Mtx scaleMtx;
    void* work = *(void**)((s32)entry + 0xC);
    void* camera = camGetPtr(cameraId);
    f32 scale;

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    tempColor = unk_80429908;
    ((u8*)&tempColor)[3] = (u8)*(s32*)((s32)work + 0x18);
    color = tempColor;
    GXSetChanMatColor(4, &color);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevOp(0, 4);
    GXSetCullMode(2);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
    scale = float_10_80428a78 * *(f32*)((s32)work + 0x10);
    PSMTXScale(scaleMtx, scale, scale, scale);
    PSMTXConcat(trans, scaleMtx, scaleMtx);
    PSMTXConcat((void*)((s32)camera + 0x11C), scaleMtx, scaleMtx);
    GXLoadPosMtxImm(scaleMtx, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(2);
    GXDrawSphere(0x10, 0x10);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

