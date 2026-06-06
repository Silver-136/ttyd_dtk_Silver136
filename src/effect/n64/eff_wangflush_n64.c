#include "effect/n64/eff_wangflush_n64.h"

typedef struct EffWangFlushWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    f32 field_10;
    f32 field_14;
    f32 field_18;
    f32 field_1C;
    f32 field_20;
    s32 field_24;
    s32 field_28;
    s32 param;
    s32 field_30;
} EffWangFlushWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_WangflushN64_802fc258[];
extern f32 float_0_8042649c;
extern f32 float_30_804264c4;
extern f32 float_0p8_804264c8;

u8 effWangFlushDisp(int param_1, int param_2) {
    return 0;
}


u8 effWangFlushMain(int* param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effWangFlushN64Entry(s32 type, s32 param, f32 x, f32 y, f32 z) {
    void* entry;
    EffWangFlushWork* work;
    f32 zero;
    f32 defaultAngle;
    f32 scale;
    s32 zeroInt;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_WangflushN64_802fc258;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x34);
    *(EffWangFlushWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effWangFlushMain;

    zeroInt = 0;
    zero = float_0_8042649c;
    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->param = param;
    work->field_30 = zeroInt;
    work->field_24 = zeroInt;
    work->field_10 = zero;
    if (type == 0) {
        defaultAngle = float_30_804264c4;
        work->field_14 = defaultAngle;
    } else {
        work->field_14 = zero;
    }
    zero = float_0_8042649c;
    zeroInt = 0;
    scale = float_0p8_804264c8;
    work->field_18 = zero;
    work->field_1C = scale;
    work->field_28 = zeroInt;
    work->field_20 = zero;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
