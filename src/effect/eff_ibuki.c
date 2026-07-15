#include "effect/eff_ibuki.h"


/* CHATGPT STUB FILL: main/effect/eff_ibuki 20260624_184929 */

/* stub-fill: effIbukiDisp | missing_definition | ghidra_signature */
void effIbukiDisp(s32 camera, void* effect) {
    typedef f32 Mtx[3][4];
    typedef u32 GXTexObj[8];
    extern void effGetTexObj(s32 id, GXTexObj obj);
    extern void GXLoadTexObj(GXTexObj obj, s32 map);
    extern void GXSetNumChans(s32 n);
    extern void GXSetNumTexGens(s32 n);
    extern void GXSetNumTevStages(s32 n);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetBlendMode(s32 type, s32 src, s32 dst, s32 op);
    extern void GXSetCullMode(s32 mode);
    extern void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void GXLoadPosMtxImm(Mtx m, s32 id);
    extern void GXCallDisplayList(void* list, u32 size);
    extern u8 ibuki_dl_0_tbl[];
    extern u32 ibuki_dl_0_size_tbl[];
    u8* base = *(u8**)((u8*)effect + 0xC);
    s32 count = *(s32*)((u8*)effect + 8);
    GXTexObj tex;
    Mtx trans, scale, model;
    s32 i;
    u8 alpha = (u8)(*(s32*)(base + 0x38) * *(s32*)(base + 0x3C) / 255);

    GXSetNumChans(1);
    GXSetNumTexGens(2);
    GXSetNumTevStages(4);
    GXSetCullMode(0);
    GXSetBlendMode(1, 4, 5, 0);
    effGetTexObj(0x3C, tex);
    GXLoadTexObj(tex, 0);
    effGetTexObj(0x62, tex);
    GXLoadTexObj(tex, 1);
    PSMTXScale(scale, *(f32*)(base + 0x2C), *(f32*)(base + 0x2C), *(f32*)(base + 0x2C));
    if (*(s32*)base != 0) {
        scale[0][0] = -scale[0][0];
    }
    PSMTXTrans(trans, *(f32*)(base + 4), *(f32*)(base + 8), *(f32*)(base + 0xC));
    PSMTXConcat(trans, scale, model);
    GXLoadPosMtxImm(model, 0);
    for (i = 0; i < 4; i++) {
        GXSetTevOp(0, 0);
        GXSetTevOrder(0, 0, 0, 0);
        GXCallDisplayList(ibuki_dl_0_tbl + i * 0x20, ibuki_dl_0_size_tbl[i]);
    }
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    effGetTexObj(99, tex);
    GXLoadTexObj(tex, 0);
    for (i = 1; i < count; i++) {
        u8* child = base + i * 0x54;
        if (*(s32*)(child + 0x50) == 0) {
            u8 childAlpha = (u8)(*(s32*)(child + 0x4C) * alpha / 255);
            PSMTXScale(scale, *(f32*)(child + 0x40), *(f32*)(child + 0x40), 1.0f);
            PSMTXTrans(trans, *(f32*)(child + 4), *(f32*)(child + 8), *(f32*)(child + 0xC));
            PSMTXConcat(trans, scale, model);
            GXLoadPosMtxImm(model, 0);
            if (childAlpha != 0) GXCallDisplayList(ibuki_dl_0_tbl, ibuki_dl_0_size_tbl[0]);
        }
    }
}

/* stub-fill: effIbukiMain | missing_definition | ghidra_signature */
void effIbukiMain(void* effect) {
    extern void effDelete(void* effect);
    extern s32 irand(s32 max);
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXRotRad(f32 matrix[3][4], s32 axis, f32 radians);
    extern void PSMTXMultVec(f32 matrix[3][4], const f32* src, f32* dst);
    extern void effVaporN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);
    extern f64 cos(f64 angle);
    extern f64 sin(f64 angle);
    extern f64 dispCalcZ(const f32* position);
    extern void dispEntry(s32 camera, s32 order, void* callback, void* param, f32 z);
    extern void effIbukiDisp(s32 camera, void* effect);

    u8* work = *(u8**)((s32)effect + 0xC);
    f32 displayPosition[3];
    s32 i;
    s32 delay;

    displayPosition[0] = *(f32*)(work + 4);
    displayPosition[1] = *(f32*)(work + 8);
    displayPosition[2] = *(f32*)(work + 0xC);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x30) = 0x10;
    }
    if (*(s32*)(work + 0x30) < 1000) {
        *(s32*)(work + 0x30) -= 1;
    }
    if (*(s32*)(work + 0x30) < 0) {
        effDelete(effect);
        return;
    }

    if (*(s32*)(work + 0x34) < 0x10) {
        *(s32*)(work + 0x38) = *(s32*)(work + 0x34) * 0x10 + 0xF;
    }
    if (*(s32*)(work + 0x30) < 0x10) {
        *(s32*)(work + 0x38) = *(s32*)(work + 0x30) << 4;
    }
    if (*(s32*)(work + 0x34) > 0x3B && *(s32*)(work + 0x34) < 0x5C) {
        *(s32*)(work + 0x3C) = (*(s32*)(work + 0x34) - 0x3C) * 8 + 7;
    }

    *(s32*)(work + 0x34) += 1;
    *(f32*)(work + 0x40) -= 0.0125f;
    *(f32*)(work + 0x44) += 0.0125f;
    *(f32*)(work + 0x48) -= 0.0125f;
    *(f32*)(work + 0x4C) = 0.0f;

    if (*(s32*)(work + 0x34) > 0x78 && (*(s32*)(work + 0x34) % 0x10) == 0) {
        f32 position[3];
        f32 rotation[3][4];
        void* camera;

        position[0] = *(f32*)(work + 4) + (f32)irand(300);
        position[1] = 30.0f + *(f32*)(work + 8) + (f32)irand(100);
        position[2] = *(f32*)(work + 0xC);
        camera = camGetPtr(4);
        PSMTXRotRad(rotation, 'y', 0.017453292f * -*(f32*)((s32)camera + 0x114));
        PSMTXMultVec(rotation, position, position);
        effVaporN64Entry(2, 0x1E, position[0], position[1], position[2],
                         1.0f + (f32)irand(10) / 10.0f);
    }

    delay = 10;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        u8* particle = work + (i - 1) * 0x54;

        if (*(s32*)(particle + 0xA4) == 0) {
            *(f32*)(particle + 0x58) += *(f32*)(particle + 0x64);
            *(f32*)(particle + 0x5C) += *(f32*)(particle + 0x68);
            *(f32*)(particle + 0x60) += *(f32*)(particle + 0x6C);
            *(f32*)(particle + 0x70) += *(f32*)(particle + 0x78);
            *(f32*)(particle + 0x74) += *(f32*)(particle + 0x7C);
            *(f32*)(particle + 0x64) *= 1.01f;
            *(f32*)(particle + 0x68) *= 1.01f;
            *(f32*)(particle + 0x6C) *= 1.01f;
            *(f32*)(particle + 0x78) *= 1.01f;
            *(f32*)(particle + 0x7C) *= 1.01f;

            *(s32*)(particle + 0x8C) += 5;
            if (*(s32*)(particle + 0x8C) > 0xFF) {
                *(s32*)(particle + 0x8C) = 0xFF;
            }

            if (*(f32*)(particle + 0x58) > 300.0f) {
                s32 angle = 0x1E - irand(0x28);
                f32 radians = (6.2832f * (f32)angle) / 360.0f;

                *(f32*)(particle + 0x58) = 0.0f;
                *(f32*)(particle + 0x5C) = (f32)irand(0x1E);
                *(f32*)(particle + 0x60) = 0.0f;
                *(f32*)(particle + 0x64) = (f32)cos((f64)radians);
                *(f32*)(particle + 0x68) = (f32)sin((f64)radians);
                *(f32*)(particle + 0x6C) = 0.0f;
                *(f32*)(particle + 0x80) = 1.0f;
                *(s32*)(particle + 0x8C) = 0;
                *(s32*)(particle + 0xA4) = delay;
                *(f32*)(particle + 0x70) = 0.0f;
                *(f32*)(particle + 0x74) = 0.0f;

                if (irand(1000) < 500) {
                    *(f32*)(particle + 0x78) = (f32)(irand(10) + 10);
                    *(f32*)(particle + 0x7C) = 0.0f;
                } else {
                    *(f32*)(particle + 0x78) = 0.0f;
                    *(f32*)(particle + 0x7C) = (f32)(irand(10) + 10);
                }
            }
        } else {
            *(s32*)(particle + 0xA4) -= 1;
        }
        delay += 10;
    }

    dispEntry(4, 2, effIbukiDisp, effect, (f32)dispCalcZ(displayPosition));
}

/* stub-fill: effIbukiEntry | missing_definition | ghidra_signature */
void* effIbukiEntry(s32 type, s32 duration, f32 x, f32 y, f32 z) {
    extern void* effEntry(s32 type);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effIbukiMain(void);
    extern s32 irand(s32 max);
    extern f64 cos(f64 x);
    extern f64 sin(f64 x);
    extern char str_Ibuki_804284e0[];
    extern f32 float_1_804284c8;
    extern f32 float_0_804284a0;
    extern f32 float_6p2832_804284d8;
    extern f32 float_360_804284dc;

    void* effect;
    void* work;
    void* part;
    s32 i;
    s32 angle;
    f32 radians;

    effect = effEntry(type);
    *(char**)((s32)effect + 0x14) = str_Ibuki_804284e0;
    *(s32*)((s32)effect + 8) = 0x21;
    work = __memAlloc(3, 0xAD4);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effIbukiMain;
    *(u32*)effect |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x30) = (duration > 0) ? duration : 1000;
    *(s32*)((s32)work + 0x34) = 0;
    *(f32*)((s32)work + 0x2C) = float_1_804284c8;
    *(s32*)((s32)work + 0x38) = 0;
    *(s32*)((s32)work + 0x3C) = 0;
    *(f32*)((s32)work + 0x40) = float_0_804284a0;
    *(f32*)((s32)work + 0x44) = float_0_804284a0;
    *(f32*)((s32)work + 0x48) = float_0_804284a0;
    *(f32*)((s32)work + 0x4C) = float_0_804284a0;

    part = (void*)((s32)work + 0x54);
    for (i = 1, angle = 10; i < *(s32*)((s32)effect + 8); i++, angle += 10, part = (void*)((s32)part + 0x54)) {
        angle = 0x1E - irand(0x28);
        *(f32*)((s32)part + 4) = float_0_804284a0;
        *(f32*)((s32)part + 8) = (f32)irand(0x1E);
        *(f32*)((s32)part + 0xC) = float_0_804284a0;
        radians = (float_6p2832_804284d8 * (f32)angle) / float_360_804284dc;
        *(f32*)((s32)part + 0x10) = (f32)cos(radians);
        *(f32*)((s32)part + 0x14) = (f32)sin(radians);
        *(f32*)((s32)part + 0x18) = float_0_804284a0;
        *(f32*)((s32)part + 0x2C) = float_1_804284c8;
        *(s32*)((s32)part + 0x38) = 0;
        *(s32*)((s32)part + 0x50) = i * 10;
        *(f32*)((s32)part + 0x1C) = float_0_804284a0;
        *(f32*)((s32)part + 0x20) = float_0_804284a0;
        if (irand(1000) < 500) {
            *(f32*)((s32)part + 0x24) = (f32)(irand(10) + 10);
            *(f32*)((s32)part + 0x28) = float_0_804284a0;
        } else {
            *(f32*)((s32)part + 0x24) = float_0_804284a0;
            *(f32*)((s32)part + 0x28) = (f32)(irand(10) + 10);
        }
    }

    return effect;
}
