#include "effect/n64/eff_booboo_n64.h"

typedef struct EffBoobooWork {
    s32 timer;
    s32 unk_04;
    s32 unk_08[11];
    f32 unk_34[11];
    f32 unk_60[11];
    f32 unk_8c[11];
    f32 unk_b8[11];
    u8 pad_e4[0x16];
    u8 unk_fa[11];
    u8 unk_105[11];
    f32 unk_110[11];
    u8 unk_13c[11];
    u8 pad_147;
} EffBoobooWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);

extern char str_BoobooN64_802fac08[];
extern f32 float_0_80424dec;


s32 texture_dl(int param_1, int param_2, s32 param_3) {
    return 0;
}


u8 effBoobooMain(int param_1) {
    return 0;
}


u8 boo_polygon_2(void) {
    return 0;
}


u8 boo_polygon_1(void) {
    return 0;
}


u8 boo_polygon_0(void) {
    return 0;
}


u8 effBoobooDisp(int param_1, int param_2) {
    return 0;
}


void* effBoobooN64Entry(void) {
    void* entry = effEntry();
    EffBoobooWork* work;
    register void* iter;
    register s32 i;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_BoobooN64_802fac08;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x148);
    *(EffBoobooWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBoobooMain;
    work->unk_04 = 0;
    iter = work;
    i = 0;
    zero = float_0_80424dec;
    work->timer = 1000;
    while (i < 11) {
        *(f32*)((s32)iter + 0x34) = zero;
        *(f32*)((s32)iter + 0x60) = zero;
        *(f32*)((s32)iter + 0x8C) = zero;
        *(f32*)((s32)iter + 0xB8) = zero;
        *(s32*)((s32)iter + 0x8) = 0;
        *((u8*)work + i + 0xFA) = 1;
        *((u8*)work + i + 0x105) = 1;
        *(f32*)((s32)iter + 0x110) = zero;
        iter = (void*)((s32)iter + 4);
        *((u8*)work + i + 0x13C) = 0;
        i++;
    }

    return entry;
}
