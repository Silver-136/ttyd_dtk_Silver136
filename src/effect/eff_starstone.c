#include "effect/eff_starstone.h"

void effStarStoneDraw(void* camera, void* effect) {
    extern void effStarStoneDisp_1(void* camera, void* effect);
    extern void effStarStoneDisp_2(void* camera, void* effect);

    void* work = *(void**)((s32)effect + 0xC);

    switch (*(s32*)work) {
        case 0:
        case 6:
            effStarStoneDisp_1(camera, effect);
            break;
        default:
            effStarStoneDisp_2(camera, effect);
            break;
    }
}


/* CHATGPT STUB FILL: main/effect/eff_starstone 20260624_184128 */

/* stub-fill: effStarStoneDisp_2 | prototype_only | source_prototype */
void effStarStoneDisp_2(void* camera, void* effect) {
    return;
}

/* stub-fill: effStarStoneDisp_1 | prototype_only | source_prototype */
void effStarStoneDisp_1(void* camera, void* effect) {
    return;
}

/* stub-fill: effStarStoneMain | missing_definition | ghidra_signature */
u8 effStarStoneMain(s32 effEntry) {
    return 0;
}

/* stub-fill: effStarStoneEntry | missing_definition | ghidra_signature */
u8 effStarStoneEntry(s64 xPos, s64 yPos, s64 zPos, s64 param_4, s32 param_5) {
    typedef struct GXColorLocal {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } GXColorLocal;
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern u8 effStarStoneMain(s32 effEntry);
    extern void* effSnowDustN64Entry(f64 x, f64 y, f64 z, f64 scaleX, f64 scaleY, f64 scaleZ, s32 unk1, s32 unk2, s32 unk3);
    extern void* gpGlobals;
    extern const char str_StarStone_802fe6e0[];
    extern f32 float_0_80427094;
    extern f32 float_1_8042709c;
    extern f32 float_10_804270a4;
    extern f32 float_20_804270b4;
    extern f32 float_255_804270b8;
    extern f32 float_90_804270c0;
    extern f64 float_1p5_804270d4;
    extern f32 edge_color[8][3];
    extern u8 color_tbl[];
    extern u8 dat_80427088;

    void* entry;
    void* work;
    void* dust;
    s32 kind;
    s32 div;
    f32 zero;
    f32 scale;
    GXColorLocal color;
    u8* tbl;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_StarStone_802fe6e0;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, *(s32*)((s32)entry + 8) * 0x64);
    *(void**)((s32)entry + 0xC) = work;
    *(void (**)(void))((s32)entry + 0x10) = (void (*)(void))effStarStoneMain;
    *(u32*)entry |= 2;

    div = param_5 / 7 + (param_5 >> 0x1F);
    kind = param_5 + (div - (div >> 0x1F)) * -7;
    *(s32*)work = kind;
    *(u16*)((s32)work + 4) = 0;
    *(f32*)((s32)work + 8) = (f32)xPos;
    *(f32*)((s32)work + 0xC) = (f32)yPos;
    *(f32*)((s32)work + 0x10) = (f32)zPos;
    zero = float_0_80427094;
    scale = (f32)(float_1p5_804270d4 * (f64)param_4);
    *(f32*)((s32)work + 0x14) = scale;
    *(f32*)((s32)work + 0x1C) = zero;
    *(f32*)((s32)work + 0x24) = zero;
    *(f32*)((s32)work + 0x28) = zero;
    *(s32*)((s32)work + 0x38) = 4;
    *(s32*)((s32)work + 0x3C) = 0;
    *(f32*)((s32)work + 0x4C) = zero;
    *(f32*)((s32)work + 0x48) = zero;
    *(f32*)((s32)work + 0x54) = zero;
    *(f32*)((s32)work + 0x50) = zero;
    *(f32*)((s32)work + 0x58) = float_90_804270c0;
    *(u8*)((s32)work + 0x5C) = 0xFF;
    *(u8*)((s32)work + 0x5D) = 0xFF;
    *(u8*)((s32)work + 0x5E) = 0xFF;
    *(u8*)((s32)work + 0x5F) = 0xFF;
    *(s32*)((s32)work + 0x60) = 0;

    color.r = (u8)(s32)(float_255_804270b8 * edge_color[kind][0]);
    color.g = (u8)(s32)(float_255_804270b8 * edge_color[kind][1]);
    color.a = dat_80427088;
    color.b = (u8)(s32)(float_255_804270b8 * edge_color[kind][2]);
    *(GXColorLocal*)((s32)work + 0x30) = color;

    if (param_5 > 6) {
        *(u16*)((s32)work + 4) |= 2;
    }
    if ((*(u16*)((s32)work + 4) & 2) == 0) {
        tbl = &color_tbl[kind * 3];
        dust = effSnowDustN64Entry((f64)xPos,
                                   -((f64)(float_10_804270a4 * scale) - (f64)yPos),
                                   (f64)zPos,
                                   (f64)(float_20_804270b4 * scale),
                                   (f64)(float_20_804270b4 * scale),
                                   (f64)float_1_8042709c,
                                   2, 4, 0);
        *(void**)((s32)work + 0x34) = dust;
        *(u32*)((s32)*(void**)((s32)dust + 0xC) + 0x4C) = tbl[0];
        *(u32*)((s32)*(void**)((s32)dust + 0xC) + 0x50) = tbl[1];
        *(u32*)((s32)*(void**)((s32)dust + 0xC) + 0x54) = tbl[2];
    }
    *(u16*)((s32)*(void**)((s32)entry + 0xC) + 4) |= 4;
    if (param_5 > 6) {
        *(u16*)((s32)*(void**)((s32)entry + 0xC) + 4) &= 0xFFFB;
    }
    if (*(s32*)((s32)gpGlobals + 0x14) != 0) {
        *(u16*)((s32)*(void**)((s32)entry + 0xC) + 4) &= 0xFFFB;
    }
    return (u8)entry;
}

