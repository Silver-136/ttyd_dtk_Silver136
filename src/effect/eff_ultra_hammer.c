#include "effect/eff_ultra_hammer.h"
#include <dolphin/mtx.h>

__declspec(section ".rodata") const u32 dat_80303010[3] = { 0, 30, 15 };
__declspec(section ".rodata") const Vec vec3_8030301c = { 0.0f, 0.0f, 0.0f };
__declspec(section ".rodata") char str_UltraHammer_80303028[] = "UltraHammer";
__declspec(section ".rodata") const u32 gap_04_80303034_rodata = 0;

__declspec(section ".data") f32 negone_one_374[2] = { -1.0f, 1.0f };
__declspec(section ".data") const u32 gap_05_803BAC90_data[2] = { 0, 0 };
__declspec(section ".data") f32 one_negone_373[2] = { 1.0f, -1.0f };
__declspec(section ".data") const u32 gap_05_803BACA0_data[2] = { 0, 0 };
__declspec(section ".data") f32 negone_one_360[2] = { -1.0f, 1.0f };
__declspec(section ".data") const u32 gap_05_803BACB0_data[2] = { 0, 0 };
__declspec(section ".data") f32 one_negone_359[2] = { 1.0f, -1.0f };

__declspec(section ".data") u32 wakka_vertex_tbl[63] = {
    0x5F1B0000, 0xDD8750E6, 0x0000C1A7, 0x3AC70000, 0xAB871EE6, 0x00009D53,
    0x00000000, 0x986EE11A, 0x00009D53, 0xC5390000, 0xAB87AF1A, 0x0000C1A7,
    0xA0E50000, 0xDD879C00, 0x0000FC6E, 0xA0E50000, 0x1B54AF1A, 0x00003735,
    0xC5390000, 0x4D54E11A, 0x00005B88, 0x00000000, 0x606D1EE6, 0x00005B88,
    0x3AC70000, 0x4D5450E6, 0x00003735, 0x5F1B0000, 0x1B546400, 0x0000FC6E,
    0x2F8D0000, 0xECFB2873, 0x0000DF0B, 0x1D630000, 0xD3FB0F73, 0x0000CCE1,
    0x00000000, 0xCA6EF08D, 0x0000CCE1, 0xE29D0000, 0xD3FBD78D, 0x0000DF0B,
    0xD0730000, 0xECFBCE00, 0x0000FC6E, 0xD0730000, 0x0BE1D78D, 0x000019D1,
    0xE29D0000, 0x24E1F08D, 0x00002BFB, 0x00000000, 0x2E6D0F73, 0x00002BFB,
    0x1D630000, 0x24E12873, 0x000019D1, 0x2F8D0000, 0x0BE13200, 0x0000FC6E,
    0x00000000, 0xCA6E0000, 0x0000986E,
};
__declspec(section ".data") const u32 gap_05_803BADBC_data = 0;
__declspec(section ".data") u32 wakka_texcoord0_tbl[42] = {
    0x60000000, 0x64000000, 0x64002000, 0x60002000, 0x68000000, 0x68002000,
    0x6C000000, 0x6C002000, 0x20000000, 0x24000000, 0x24002000, 0x20002000,
    0x28000000, 0x28002000, 0x2C000000, 0x2C002000, 0x30000000, 0x30002000,
    0x34000000, 0x34002000, 0x38000000, 0x38002000, 0x3C000000, 0x3C002000,
    0x40000000, 0x40002000, 0x44000000, 0x44002000, 0x48000000, 0x48002000,
    0x4C000000, 0x4C002000, 0x50000000, 0x50002000, 0x54000000, 0x54002000,
    0x58000000, 0x58002000, 0x5C000000, 0x5C002000, 0x70000000, 0x70002000,
};

#define WAKKA_DL(name, a, b, c, d, e) \
    __declspec(section ".sdata") u32 name[8] = { 0x980004##a, 0x0000##b, 0x0000##c, 0x0000##d, 0x0000##e, 0, 0, 0 }

__declspec(section ".sdata") u8 wakka_normal_tbl[3] = { 0x00, 0x40, 0x00 };
__declspec(section ".sdata") u8 gap_07_8041E0E3_sdata[29] = { 0 };
__declspec(section ".sdata") u8 wakka_color0_tbl[8] = {
    0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0x00,
};
__declspec(section ".sdata") u8 gap_07_8041E108_sdata[24] = { 0 };
WAKKA_DL(wakka_dl_0_0, 01, 0200, 0315, 0114, 0000);
WAKKA_DL(wakka_dl_0_1, 02, 0501, 0216, 0415, 0100);
WAKKA_DL(wakka_dl_0_2, 03, 0702, 0517, 0616, 0400);
WAKKA_DL(wakka_dl_0_3, 27, 2614, 0013, 2700, 0300);
WAKKA_DL(wakka_dl_0_4, 26, 2427, 2612, 2513, 2700);
WAKKA_DL(wakka_dl_0_5, 25, 2226, 2411, 2312, 2500);
WAKKA_DL(wakka_dl_0_6, 24, 2025, 2210, 2111, 2300);
WAKKA_DL(wakka_dl_0_7, 23, 1E24, 200F, 1F10, 2100);
WAKKA_DL(wakka_dl_0_8, 22, 1C23, 1E0E, 1D0F, 1F00);
WAKKA_DL(wakka_dl_0_9, 21, 1A22, 1C0D, 1B0E, 1D00);
WAKKA_DL(wakka_dl_0_10, 20, 1821, 1A0C, 190D, 1B00);
WAKKA_DL(wakka_dl_0_11, 1F, 1620, 180B, 170C, 1900);
WAKKA_DL(wakka_dl_0_12, 1E, 141F, 160A, 150B, 1700);
WAKKA_DL(wakka_dl_0_13, 1D, 121E, 1409, 130A, 1500);
WAKKA_DL(wakka_dl_0_14, 1C, 101D, 1208, 1109, 1300);
WAKKA_DL(wakka_dl_0_15, 1B, 0E1C, 1007, 0F08, 1100);
WAKKA_DL(wakka_dl_0_16, 1A, 0C1B, 0E06, 0D07, 0F00);
WAKKA_DL(wakka_dl_0_17, 19, 091A, 0C05, 0A06, 0D00);
WAKKA_DL(wakka_dl_0_18, 18, 0819, 0904, 0B05, 0A00);
WAKKA_DL(wakka_dl_0_19, 29, 012903, 0728, 012817, 0600);
#undef WAKKA_DL

__declspec(section ".data") void* wakka_dl_0_tbl[20] = {
    wakka_dl_0_0, wakka_dl_0_1, wakka_dl_0_2, wakka_dl_0_3, wakka_dl_0_4,
    wakka_dl_0_5, wakka_dl_0_6, wakka_dl_0_7, wakka_dl_0_8, wakka_dl_0_9,
    wakka_dl_0_10, wakka_dl_0_11, wakka_dl_0_12, wakka_dl_0_13, wakka_dl_0_14,
    wakka_dl_0_15, wakka_dl_0_16, wakka_dl_0_17, wakka_dl_0_18, wakka_dl_0_19,
};
__declspec(section ".data") u32 scroll_tbl[25] = {
    0x00000000, 0xBE155556, 0xBE955556, 0xBEE00001, 0xBF155556,
    0xBF3AAAAB, 0xBF600001, 0xBF82AAAB, 0xBF955556, 0xBFA80000,
    0xBFBAAAAB, 0xBFCD5555, 0xBFE00000, 0xBFF2AAAB, 0xC002AAAB,
    0xC00C0000, 0xC0155555, 0xC01EAAAB, 0xC0280000, 0xC0315555,
    0xC03AAAAB, 0xC0440000, 0xC04D5555, 0xC056AAAB, 0xC0600000,
};
__declspec(section ".data") const u32 gap_05_803BAF1C_data = 0;
__declspec(section ".sdata") u8 wakka_dl_0_size_tbl[20] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
__declspec(section ".sdata") const u32 gap_07_8041E3B4_sdata = 0;

__declspec(section ".sdata2") u32 dat_80428b88 = 0xFFFFFF00;
__declspec(section ".sdata2") f32 float_deg2rad_80428b8c = 0.017453292f;
__declspec(section ".sdata2") f32 float_0p6_80428b90 = 0.6f;
__declspec(section ".sdata2") f32 float_neg1_80428b94 = -1.0f;
__declspec(section ".sdata2") f32 float_1_80428b98 = 1.0f;
__declspec(section ".sdata2") f32 float_0_80428b9c = 0.0f;
__declspec(section ".sdata2") f32 float_3p5_80428ba0 = 3.5f;
__declspec(section ".sdata2") f32 float_neg4_80428ba4 = -4.0f;

void effDelete(void* entry);
f32 dispCalcZ(Vec* pos);
void dispEntry(s32 cameraId, s32 order, void* callback, void* entry);
void effUltraHammerDisp(void* camera, void* entry);

/* stub-fill: effUltraHammerEntry | missing_definition | ghidra_signature */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effUltraHammerEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effUltraHammerMain(void* entry);
    void* entry;
    void* work;
    void* child;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_UltraHammer_80303028;
    *(s32*)((s32)entry + 8) = 4;
    work = __memAlloc(3, 0xB0);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effUltraHammerMain;

    *(s32*)work = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = scale;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x20) = 1000;
    } else {
        *(s32*)((s32)work + 0x20) = timer;
    }
    *(s32*)((s32)work + 0x1C) = 0xFF;
    *(s32*)((s32)work + 0x24) = 0;

    child = (void*)((s32)work + 0x2C);
    for (i = 1; i < *(s32*)((s32)entry + 8); i++) {
        *(f32*)((s32)child + 4) = float_0_80428b9c;
        *(f32*)((s32)child + 8) = float_neg4_80428ba4 * (f32)(i - 1);
        *(f32*)((s32)child + 0xC) = float_0_80428b9c;
        *(f32*)((s32)child + 0x10) = float_1_80428b98;
        *(u32*)((s32)child + 0x28) = dat_80303010[i - 1];
        *(s32*)((s32)child + 0x24) = 0;
        *(s32*)((s32)child + 0x20) = 0;
        child = (void*)((s32)child + 0x2C);
    }
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effUltraHammerMain(void* entry) {
    void* work = *(void**)((s32)entry + 0xC);
    Vec zpos;
    Vec pos = vec3_8030301c;
    s32 timer;
    s32 i;
    f32 scroll;
    f32 hammerScale;

    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);
    zpos = pos;

    timer = *(s32*)((s32)work + 0x20);
    if (timer < 1000) {
        *(s32*)((s32)work + 0x20) = timer - 1;
    }
    if (*(s32*)((s32)work + 0x20) < 0x10) {
        *(s32*)((s32)work + 0x1C) = *(s32*)((s32)work + 0x20) << 4;
    }
    if (*(s32*)((s32)work + 0x20) < 0) {
        effDelete(entry);
    } else {
        i = 1;
        work = (void*)((s32)work + 0x2C);
        while (i < *(s32*)((s32)entry + 8)) {
            timer = *(s32*)((s32)work + 0x28);
            if (timer != 0) {
                *(s32*)((s32)work + 0x28) = timer - 1;
            } else if (*(s32*)((s32)work + 0x24) < 0x19) {
                hammerScale = float_3p5_80428ba0;
                scroll = ((f32*)scroll_tbl)[*(s32*)((s32)work + 0x24)];
                *(f32*)((s32)work + 0x14) = scroll;
                *(f32*)((s32)work + 0x10) = hammerScale;
                *(s32*)((s32)work + 0x24) = *(s32*)((s32)work + 0x24) + 1;
            } else {
                *(s32*)((s32)work + 0x28) = 0xF;
                *(s32*)((s32)work + 0x24) = 0;
            }
            i++;
            work = (void*)((s32)work + 0x2C);
        }
        dispCalcZ(&zpos);
        dispEntry(4, 2, effUltraHammerDisp, entry);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_ultra_hammer 20260624_185035 */

/* stub-fill: effUltraHammerDisp | prototype_only | source_prototype */
void effUltraHammerDisp(void* cameraArg, void* effect) {
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(f32 matrix[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 matrix[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(f32 matrix[3][4], s32 axis, f32 angle);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 out[3][4]);
    extern void effGetTexObj(s32 textureId, void* textureObject);
    extern void GXLoadTexObj(void* textureObject, s32 mapId);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetTevColor(s32 stage, void* color);
    extern void GXLoadPosMtxImm(f32 matrix[3][4], s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXLoadTexMtxImm(f32 matrix[3][4], s32 id, s32 type);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetArray(s32 attr, void* base, s32 stride);
    extern void GXCallDisplayList(void* list, u32 size);
    u8 textureObject[0x20];
    f32 base[3][4];
    f32 transform[3][4];
    f32 scaleMtx[3][4];
    f32 rotation[3][4];
    u8* work = *(u8**)((s32)effect + 0xC);
    u8* child = work + 0x2C;
    s32 cameraId = (s32)cameraArg;
    void* camera = camGetPtr(cameraId);
    s32 type = *(s32*)work;
    s32 i;

    PSMTXTrans(base, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXScale(scaleMtx, *(f32*)(work + 0x10), *(f32*)(work + 0x10),
                *(f32*)(work + 0x10));
    PSMTXRotRad(rotation, 'y', float_deg2rad_80428b8c *
                -*(f32*)((u8*)camGetPtr(cameraId) + 0x114));
    PSMTXConcat(base, rotation, base);
    PSMTXConcat(base, scaleMtx, transform);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 10);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 15, 15, 15, 0);
    GXSetTevAlphaIn(1, 7, 4, 6, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    effGetTexObj(0x75, textureObject);
    GXLoadTexObj(textureObject, 0);
    GXSetCullMode(0);
    {
        u32 color = dat_80428b88;
        ((u8*)&color)[3] = (u8)*(s32*)(work + 0x1C);
        GXSetTevColor(1, &color);
    }

    for (i = 1; i < *(s32*)((s32)effect + 8); i++, child += 0x2C) {
        s32 j;
        f32 scale;
        if (*(s32*)(child + 0x28) != 0) {
            continue;
        }
        PSMTXTrans(base, *(f32*)(child + 4), *(f32*)(child + 8), *(f32*)(child + 0xC));
        scale = float_0p6_80428b90 * *(f32*)(child + 0x10);
        PSMTXScale(scaleMtx, scale, scale, scale);
        PSMTXConcat(base, scaleMtx, base);
        if (type != 0) {
            PSMTXScale(scaleMtx, float_neg1_80428b94, float_1_80428b98,
                        float_1_80428b98);
            PSMTXConcat(base, scaleMtx, base);
        }
        PSMTXConcat(transform, base, base);
        PSMTXConcat((f32(*)[4])((u8*)camera + 0x11C), base, base);
        GXLoadPosMtxImm(base, 0);
        GXSetCurrentMtx(0);
        PSMTXTrans(base, *(f32*)(child + 0x14), *(f32*)(child + 0x18),
                    float_0_80428b9c);
        GXLoadTexMtxImm(base, 0x1E, 1);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 2);
        GXSetVtxAttrFmt(0, 9, 1, 3, 10);
        GXSetArray(9, wakka_vertex_tbl, 6);
        GXSetVtxDesc(10, 2);
        GXSetVtxAttrFmt(0, 10, 0, 1, 6);
        GXSetArray(10, (void*)0x8041E0E0, 3);
        GXSetVtxDesc(11, 2);
        GXSetVtxAttrFmt(0, 11, 1, 5, 0);
        GXSetArray(11, (void*)0x8041E100, 4);
        GXSetVtxDesc(13, 2);
        GXSetVtxAttrFmt(0, 13, 1, 3, 13);
        GXSetArray(13, wakka_texcoord0_tbl, 4);
        for (j = 0; j < 20; j++) {
            GXCallDisplayList(wakka_dl_0_tbl[j], (u32)wakka_dl_0_size_tbl[j] << 5);
        }
    }
}
