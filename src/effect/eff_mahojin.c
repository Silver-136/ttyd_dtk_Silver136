#include "effect/eff_mahojin.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effMahojinEntry(s32 kind, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effMahojinMain(void);
    extern const char str_Mahojin_802fefb4[];

    void* entry = effEntry();
    s32 countKind = kind;
    s32 count;
    void* work;
    s32 i;
    void* ptr;

    countKind = 7;
    if (kind < 8) {
        countKind = kind;
    }
    count = countKind + 2;
    *(const char**)((s32)entry + 0x14) = str_Mahojin_802fefb4;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, count * 0x30);
    *(void**)((s32)entry + 0xC) = work;
    *(s32*)((s32)entry + 0x10) = (s32)effMahojinMain;
    *(u32*)entry |= 2;

    *(s32*)work = kind;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x10) = 1000;
    *(s32*)((s32)work + 0x14) = 0;
    *(s32*)((s32)work + 0x18) = -1;
    *(s32*)((s32)work + 0x1C) = -1;
    *(s32*)((s32)work + 0x20) = 0xFF;
    *(s32*)((s32)work + 0x24) = 0;
    *(s32*)((s32)work + 0x28) = 0;
    *(s32*)((s32)work + 0x2C) = 0;

    *(s32*)((s32)work + 0x30) = kind;
    *(f32*)((s32)work + 0x34) = x;
    *(f32*)((s32)work + 0x38) = y;
    *(f32*)((s32)work + 0x3C) = z;
    *(s32*)((s32)work + 0x40) = 1000;
    *(s32*)((s32)work + 0x44) = 0;
    *(s32*)((s32)work + 0x48) = -1;
    *(s32*)((s32)work + 0x4C) = -1;
    *(s32*)((s32)work + 0x50) = 0xFF;
    *(s32*)((s32)work + 0x54) = 0;
    *(s32*)((s32)work + 0x58) = 0;
    *(s32*)((s32)work + 0x5C) = 0;

    ptr = (void*)((s32)work + 0x60);
    for (i = 2; i < *(s32*)((s32)entry + 8); i++, ptr = (void*)((s32)ptr + 0x30)) {
        *(f32*)((s32)ptr + 4) = x;
        *(f32*)((s32)ptr + 8) = y;
        *(f32*)((s32)ptr + 0xC) = z;
        *(s32*)((s32)ptr + 0x18) = -1;
        *(s32*)((s32)ptr + 0x1C) = -1;
        *(s32*)((s32)ptr + 0x20) = 0;
        *(s32*)((s32)ptr + 0x24) = 0;
        *(s32*)((s32)ptr + 0x28) = 0;
        *(s32*)((s32)ptr + 0x2C) = 0;
    }
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void callback(void* data) {
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetTevOrder(s32 tev, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 tev, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 tev, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 tev, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 tev, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevSwapMode(s32 tev, s32 rasSel, s32 texSel);
    extern u32 dat_80427750;
    extern s32 mahojin_rate;

    u32 colorCopy;
    u32 color = dat_80427750;
    s32 tev = *(s32*)((s32)data + 0xC);
    s32 v10 = *(s32*)((s32)data + 0x10);
    s32 v14 = *(s32*)((s32)data + 0x14);
    s32 v18 = *(s32*)((s32)data + 0x18);

    ((u8*)&color)[3] = mahojin_rate;
    colorCopy = color;
    GXSetTevColor(1, &colorCopy);
    GXSetTevOrder(tev, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(tev, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(tev, 0, 0, 0, 1, 0);
    GXSetTevColorIn(tev, 0, 2, 3, 0xF);
    GXSetTevAlphaIn(tev, 7, 7, 7, 0);
    GXSetTevSwapMode(tev, 0, 0);
    *(s32*)((s32)data + 0xC) = tev + 1;
    *(s32*)((s32)data + 0x14) = v14;
    *(s32*)((s32)data + 0x18) = v18;
    *(s32*)((s32)data + 0x10) = v10;
}
