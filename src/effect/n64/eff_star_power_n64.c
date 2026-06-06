#include "effect/n64/eff_star_power_n64.h"

typedef struct EffStarPowerWork {
    s16 field_00;
    s16 type;
    f32 x;
    f32 y;
    f32 z;
    s32 lifetime;
    s32 frame;
    s32 r0;
    s32 g0;
    s32 b0;
    s32 a0;
    s32 r1;
    s32 g1;
    s32 b1;
    s32 a1;
    f32 scale;
    f32 field_3C;
    f32 field_40;
    f32 field_44;
    s32 field_48;
    s32 field_4C;
    s32 field_50;
    s32 field_54;
} EffStarPowerWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_StarPowerN64_802fc058[];
extern f32 float_0_804261ac;

u8 effStarPowerDisp(int param_1, int param_2) {
    return 0;
}


u8 effStarPowerMain(u32* param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effStarPowerN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    void* entry;
    EffStarPowerWork* work;
    f32 zero;
    s32 one;
    s32 zeroInt;
    s32 full;
    s32 half;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_StarPowerN64_802fc058;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x58);
    *(EffStarPowerWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effStarPowerMain;
    *(u32*)entry |= 2;

    one = 1;
    zeroInt = 0;
    work->field_00 = one;
    work->type = (s16)type;
    work->frame = zeroInt;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->x = x;
    full = 0xFF;
    half = 0x78;
    zeroInt = 0;
    work->y = y;
    zero = float_0_804261ac;
    work->z = z;
    work->scale = scale;
    work->r0 = full;
    work->g0 = full;
    work->b0 = half;
    work->r1 = full;
    work->g1 = half;
    work->b1 = zeroInt;
    work->a1 = full;
    work->a0 = full;
    work->field_3C = zero;
    work->field_40 = zero;
    work->field_44 = zero;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
