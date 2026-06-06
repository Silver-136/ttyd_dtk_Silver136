#include "effect/n64/eff_snow_n64.h"

typedef struct EffSnowWork {
    f32 x;
    f32 y;
    f32 z;
    f32 field_0C;
    f32 field_10;
    f32 field_14;
    f32 field_18;
    f32 field_1C;
    f32 field_20;
    f32 field_24;
    s32 type;
} EffSnowWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_SnowN64_802fbf84[];
extern f32 float_0_80426060;
extern f32 float_neg1p5_8042607c;
extern f32 float_1p5_80426080;
extern f32 float_neg5_80426084;
extern f32 float_5_80426088;

u8 effSnowDisp(int param_1, int param_2) {
    return 0;
}


u8 effSnowMain(int* param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effSnowN64Entry(s32 type, f32 x, f32 y, f32 z) {
    static s32 kaiten;
    void* entry;
    EffSnowWork* work;
    f32 onePointFive;
    f32 negOnePointFive;
    f32 zero;
    f32 sideSpeed;
    s32 next;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_SnowN64_802fbf84;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x2C);
    *(EffSnowWork**)((s32)entry + 0xC) = work;

    zero = float_0_80426060;
    *(void**)((s32)entry + 0x10) = effSnowMain;
    negOnePointFive = float_neg1p5_8042607c;
    work->x = x;
    onePointFive = float_1p5_80426080;
    work->y = y;
    work->z = z;
    work->field_0C = zero;
    work->field_10 = negOnePointFive;
    work->field_14 = zero;
    work->field_18 = zero;
    work->field_1C = zero;
    work->field_20 = onePointFive;
    if (kaiten != 0) {
        sideSpeed = float_neg5_80426084;
    } else {
        sideSpeed = float_5_80426088;
    }
    work->field_24 = sideSpeed;
    work->type = type;
    next = kaiten + 1;
    kaiten = next;
    if (next > 1) {
        kaiten = 0;
    }
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
