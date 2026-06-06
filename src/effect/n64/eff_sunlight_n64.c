#include "effect/n64/eff_sunlight_n64.h"

typedef struct EffSunlightWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 lifetime;
    s32 frame;
    u8 color0[7];
    u8 pad_1F;
    f32 field_20;
    f32 field_24;
    f32 field_28;
    f32 field_2C;
    f32 field_30;
    f32 scale;
    s32 field_38;
    s32 field_3C;
} EffSunlightWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_SunlightN64_802fc128[];
extern f32 float_0_80426300;


u8 effSunlightDisp(void) {
    return 0;
}


u8 effSunlightMain(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effSunlightN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    void* entry;
    EffSunlightWork* work;
    f32 zero;
    s32 zeroInt;
    s32 full;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_SunlightN64_802fc128;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x40);
    *(EffSunlightWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effSunlightMain;
    *(u32*)entry |= 2;

    zeroInt = 0;
    work->type = type;
    work->frame = zeroInt;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }

    work->x = x;
    full = 0xFF;
    zero = float_0_80426300;
    zeroInt = 0;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->color0[0] = full;
    work->color0[1] = full;
    work->color0[2] = full;
    work->color0[3] = full;
    work->color0[4] = full;
    work->color0[5] = full;
    work->color0[6] = full;
    work->field_20 = zero;
    work->field_24 = zero;
    work->field_28 = zero;
    work->field_2C = zero;
    work->field_30 = zero;
    work->field_3C = full;
    work->field_38 = zeroInt;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
