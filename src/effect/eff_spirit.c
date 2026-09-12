#include "effect/eff_spirit.h"

extern const f32 float_0_80428824[1];
extern const f32 float_2_80428828[1];
extern const f32 float_0p95_8042882c[1];
extern const f32 float_0p1_80428830[1];
extern const f32 float_3p1416_80428834[1];
extern const f32 float_50_80428838[1];
extern const f32 float_1_8042883c[1];
extern const f32 float_255_80428840[1];
extern const f32 float_37_80428844[1];
extern const f32 float_242_80428848[1];
extern const f32 float_254_8042884c[1];
extern const f32 float_246_80428850[1];
extern const f32 float_226_80428854[1];
extern const f32 float_65_80428858[1];

/* CHATGPT STUB FILL: main/effect/eff_spirit 20260624_184929 */

/* stub-fill: effSpiritEntry | missing_definition | ghidra_signature */
void* effSpiritEntry(f32 x, f32 y, f32 z, f32 targetX, f32 targetY, f32 targetZ, f32 scale, s32 kind, s32 flags) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern char str_Spirit_80302988[];
    extern int effSpiritMain(void);
    extern const f32 float_0_80428824[1];
    extern const f32 float_2_80428828[1];
    extern const f32 float_1_8042883c[1];

    void* entry;
    void* work;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Spirit_80302988;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x68);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effSpiritMain;
    *(u32*)entry |= 2;

    *(s32*)((s32)work + 0) = kind;
    *(f32*)((s32)work + 0x10) = x;
    *(f32*)((s32)work + 0x14) = y;
    *(f32*)((s32)work + 0x18) = z;
    *(f32*)((s32)work + 0x60) = scale;
    *(f32*)((s32)work + 0x64) = float_1_8042883c[0];
    *(s32*)((s32)work + 0xC) = 0;
    *(s32*)((s32)work + 8) = 1000;
    *(s32*)((s32)work + 4) = 0;
    *(f32*)((s32)work + 0x34) = float_0_80428824[0];
    *(f32*)((s32)work + 0x38) = float_2_80428828[0];
    *(f32*)((s32)work + 0x3C) = float_0_80428824[0];
    *(f32*)((s32)work + 0x1C) = *(f32*)((s32)work + 0x10);
    *(f32*)((s32)work + 0x20) = *(f32*)((s32)work + 0x14);
    *(f32*)((s32)work + 0x24) = *(f32*)((s32)work + 0x18);
    *(f32*)((s32)work + 0x28) = targetX;
    *(f32*)((s32)work + 0x2C) = targetY;
    *(f32*)((s32)work + 0x30) = targetZ;
    *(s32*)((s32)work + 0x48) = 0xFF;
    *(s32*)((s32)work + 0x4C) = 0xF2;
    *(s32*)((s32)work + 0x50) = 0;
    *(s32*)((s32)work + 0x54) = 0xCC;
    *(s32*)((s32)work + 0x58) = 0;
    *(s32*)((s32)work + 0x5C) = 0;
    *(s32*)((s32)work + 0x40) = 0;
    *(s32*)((s32)work + 0x44) = flags;
    return entry;
}

const f32 float_0_80428824[1] = { 0.0f };
const f32 float_2_80428828[1] = { 2.0f };
const f32 float_0p95_8042882c[1] = { 0.95f };
const f32 float_0p1_80428830[1] = { 0.1f };
const f32 float_3p1416_80428834[1] = { 3.14159274f };
const f32 float_50_80428838[1] = { 50.0f };
const f32 float_1_8042883c[1] = { 1.0f };
const f32 float_255_80428840[1] = { 255.0f };
const f32 float_37_80428844[1] = { 37.0f };
const f32 float_242_80428848[1] = { 242.0f };
const f32 float_254_8042884c[1] = { 254.0f };
const f32 float_246_80428850[1] = { 246.0f };
const f32 float_226_80428854[1] = { 226.0f };
const f32 float_65_80428858[1] = { 65.0f };

/* CHATGPT FALLBACK MISSING STUBS: main/effect/eff_spirit 20260624_191429 */

/* fallback stub-fill: map=effSpiritMain addr=0x80255824 size=0x000004c4 */
void effSpiritMain(void* effect) {
    extern void effDelete(void*);
    extern f64 sin(f64);
    extern f64 intplGetValue(f64, f64, s32, s32, s32);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effSpiritDisp(s32, void*);

    u8* entry = effect;
    u8* work = *(u8**)(entry + 0xC);
    f32 pos[3];
    s32 state;
    s32 step;

    pos[0] = *(f32*)(work + 0x10);
    pos[1] = *(f32*)(work + 0x14);
    pos[2] = *(f32*)(work + 0x18);
    if ((*(u32*)entry & 4) != 0) {
        *(u32*)entry &= ~4;
        *(s32*)(work + 8) = 0x10;
    }
    if (*(s32*)(work + 8) < 1000) {
        (*(s32*)(work + 8))--;
    }
    (*(s32*)(work + 0xC))++;
    if (*(s32*)(work + 8) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 8) < 0x10) {
        *(s32*)(work + 0x54) = *(s32*)(work + 8) << 4;
    }

    state = *(s32*)(work + 4);
    if (state == 0) {
        *(f32*)(work + 0x10) += (f32)sin((f64)((f32)*(s32*)(work + 0xC) * 0.5f));
        *(f32*)(work + 0x14) += *(f32*)(work + 0x38);
        *(f32*)(work + 0x38) *= float_0p95_8042882c[0];
        if (*(f32*)(work + 0x38) < float_0p1_80428830[0]) {
            *(f32*)(work + 0x1C) = *(f32*)(work + 0x10);
            *(f32*)(work + 0x20) = *(f32*)(work + 0x14);
            *(f32*)(work + 0x24) = *(f32*)(work + 0x18);
            (*(s32*)(work + 4))++;
        }
    } else if (state == 1) {
        step = *(s32*)(work + 0x40);
        if (step >= *(s32*)(work + 0x44)) {
            effDelete(effect);
            return;
        }
        step++;
        *(s32*)(work + 0x40) = step;
        *(f32*)(work + 0x10) = (f32)intplGetValue(*(f32*)(work + 0x1C), *(f32*)(work + 0x28), 0xB, step, *(s32*)(work + 0x44));
        *(f32*)(work + 0x14) = (f32)intplGetValue(*(f32*)(work + 0x20), *(f32*)(work + 0x2C), 0xB, step, *(s32*)(work + 0x44));
        *(f32*)(work + 0x18) = (f32)intplGetValue(*(f32*)(work + 0x24), *(f32*)(work + 0x30), 0xB, step, *(s32*)(work + 0x44));
        *(f32*)(work + 0x14) += float_50_80428838[0] * (f32)sin(float_3p1416_80428834[0] * (f32)step / (f32)*(s32*)(work + 0x44));
        *(f32*)(work + 0x64) = (f32)intplGetValue(float_1_8042883c[0], float_0_80428824[0], 0xB, step, *(s32*)(work + 0x44));
    }

    if (*(s32*)(work + 0x58) == 1) {
        *(s32*)(work + 0x48) = (s32)intplGetValue(float_37_80428844[0], float_226_80428854[0], 0xB, *(s32*)(work + 0x5C), 0x20);
        *(s32*)(work + 0x4C) = (s32)intplGetValue(float_254_8042884c[0], float_65_80428858[0], 0xB, *(s32*)(work + 0x5C), 0x20);
        *(s32*)(work + 0x50) = (s32)intplGetValue(float_246_80428850[0], float_255_80428840[0], 0xB, *(s32*)(work + 0x5C), 0x20);
        (*(s32*)(work + 0x5C))++;
        if (*(s32*)(work + 0x5C) > 0x1F) {
            *(s32*)(work + 0x58) = 2;
            *(s32*)(work + 0x5C) = 0;
        }
    } else if (*(s32*)(work + 0x58) < 1) {
        if (*(s32*)(work + 0x58) > -1) {
            *(s32*)(work + 0x48) = (s32)intplGetValue(float_255_80428840[0], float_37_80428844[0], 0xB, *(s32*)(work + 0x5C), 0x20);
            *(s32*)(work + 0x4C) = (s32)intplGetValue(float_242_80428848[0], float_254_8042884c[0], 0xB, *(s32*)(work + 0x5C), 0x20);
            *(s32*)(work + 0x50) = (s32)intplGetValue(float_0_80428824[0], float_246_80428850[0], 0xB, *(s32*)(work + 0x5C), 0x20);
            (*(s32*)(work + 0x5C))++;
            if (*(s32*)(work + 0x5C) > 0x1F) {
                *(s32*)(work + 0x58) = 1;
                *(s32*)(work + 0x5C) = 0;
            }
        }
    } else if (*(s32*)(work + 0x58) < 3) {
        *(s32*)(work + 0x48) = (s32)intplGetValue(float_226_80428854[0], float_255_80428840[0], 0xB, *(s32*)(work + 0x5C), 0x20);
        *(s32*)(work + 0x4C) = (s32)intplGetValue(float_65_80428858[0], float_242_80428848[0], 0xB, *(s32*)(work + 0x5C), 0x20);
        *(s32*)(work + 0x50) = (s32)intplGetValue(float_255_80428840[0], float_0_80428824[0], 0xB, *(s32*)(work + 0x5C), 0x20);
        (*(s32*)(work + 0x5C))++;
        if (*(s32*)(work + 0x5C) > 0x1F) {
            *(s32*)(work + 0x58) = 0;
            *(s32*)(work + 0x5C) = 0;
        }
    }
    dispEntry(4, 2, effSpiritDisp, effect, dispCalcZ(pos));
}


/* CHATGPT STUB FILL: main/effect/eff_spirit 20260624_184929 */

/* stub-fill: effSpiritDisp | missing_definition | ghidra_signature */
void effSpiritDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
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
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern f32 float_deg2rad_80428810;

    u8 texObj[0x20];
    f32 trans[3][4];
    f32 rot[3][4];
    f32 scaleMtx[3][4];
    u8* work = *(u8**)((u8*)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    volatile f32* fifo = (volatile f32*)0xCC008000;
    u32 color;
    f32 scale;
    f32 halfWidth = -8.0f * 0.5f;

    PSMTXTrans(trans, *(f32*)(work + 0x10), *(f32*)(work + 0x14), *(f32*)(work + 0x18));
    PSMTXRotRad(rot, 'y', float_deg2rad_80428810 * -*(f32*)((u8*)camera + 0x114));
    scale = *(f32*)(work + 0x60) * *(f32*)(work + 0x64);
    PSMTXScale(scaleMtx, scale, scale, scale);
    PSMTXConcat(trans, rot, rot);
    PSMTXConcat(rot, scaleMtx, scaleMtx);
    PSMTXConcat((f32(*)[4])((u8*)camera + 0x11C), scaleMtx, scaleMtx);
    GXLoadPosMtxImm(scaleMtx, 0);
    GXSetCurrentMtx(0);

    effGetTexObj(0x67, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    ((u8*)&color)[0] = (u8)*(s32*)(work + 0x48);
    ((u8*)&color)[1] = (u8)*(s32*)(work + 0x4C);
    ((u8*)&color)[2] = (u8)*(s32*)(work + 0x50);
    ((u8*)&color)[3] = (u8)(0.8f * (f32)*(s32*)(work + 0x54));
    GXSetChanMatColor(4, &color);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 10);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXBegin(0x80, 0, 4);
    *fifo = halfWidth; *fifo = 4.0f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
    *fifo = 4.0f; *fifo = 4.0f; *fifo = 0.0f; *fifo = 2.0f; *fifo = 0.0f;
    *fifo = 4.0f; *fifo = halfWidth; *fifo = 0.0f; *fifo = 2.0f; *fifo = 2.0f;
    *fifo = halfWidth; *fifo = halfWidth; *fifo = 0.0f; *fifo = 0.0f; *fifo = 2.0f;
}

