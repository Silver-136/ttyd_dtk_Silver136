#include "effect/n64/eff_stone_smoke_n64.h"

typedef struct EffStoneSmokeWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 lifetime;
    s32 frame;
    s32 r0;
    s32 g0;
    s32 b0;
    s32 a0;
    s32 field_28;
    s32 field_2C;
    s32 field_30;
    f32 scale;
    s32 field_38;
} EffStoneSmokeWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_StoneSmokeN64_802fc0e0[];

u8 effStoneSmokeDisp(void) {
    return 0;
}


u8 effStoneSmokeMain(u32* param_1) {
    return 0;
}


#pragma use_lmw_stmw off
void* effStoneSmokeN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    void* entry;
    EffStoneSmokeWork* work;
    s32 zero;
    s32 full;
    s32 ten;
    s32 half;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_StoneSmokeN64_802fc0e0;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x3C);
    *(EffStoneSmokeWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effStoneSmokeMain;
    *(u32*)entry |= 2;

    zero = 0;
    work->type = type;
    work->frame = zero;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }

    full = 0xFF;
    ten = 10;
    work->a0 = full;
    half = 0x78;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->r0 = full;
    work->g0 = full;
    work->b0 = full;
    work->field_28 = ten;
    work->field_2C = ten;
    work->field_30 = half;
    return entry;
}
#pragma use_lmw_stmw on
