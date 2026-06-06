#include "effect/n64/eff_opuku_wprotect_n64.h"

typedef struct EffOpukuWprotectWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    s32 frame;
    s32 field_18;
    s32 field_1c;
    s32 field_20;
    s32 field_24;
    u8 pad_28[4];
    f32 scale;
    f32 field_30;
    f32 field_34;
    u8 pad_38[0x50];
    s32 field_88;
    s32 field_8c;
    s32 field_90;
    s32 field_94;
} EffOpukuWprotectWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_OpukuWprotectN64_802fbc28[];
extern f32 float_0_80425c10;


u8 effOpukuWprotectDisp(void) {
    return 0;
}


u8 effOpukuWprotectMain(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effOpukuWprotectN64Entry(s32 type, f32 x, f32 y, f32 z, s32 timer, f32 scale) {
    void* entry;
    EffOpukuWprotectWork* work;
    s32 zero;
    f32 zeroFloat;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_OpukuWprotectN64_802fbc28;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x98);
    *(EffOpukuWprotectWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effOpukuWprotectMain;

    zero = 0;
    *(u32*)entry |= 2;
    work->type = type;
    work->frame = zero;
    if (timer <= 0) {
        work->timer = 1000;
    } else {
        work->timer = timer;
    }

    work->field_24 = 0;
    zeroFloat = float_0_80425c10;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->field_18 = 0x2F;
    work->field_1c = 0x7F;
    work->field_20 = 0xFF;
    work->field_30 = zeroFloat;
    work->field_34 = zeroFloat;
    work->field_88 = -1;
    work->field_8c = -5;
    work->field_90 = -9;
    work->field_94 = -13;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
