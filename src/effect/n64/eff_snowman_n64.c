#include "effect/n64/eff_snowman_n64.h"

typedef struct EffSnowmanWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 lifetime;
    s32 frame;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    f32 field_28;
    f32 field_2C;
    f32 field_30;
    f32 field_34;
    f32 field_38;
    f32 field_3C;
    f32 field_40;
    f32 field_44;
    s32 field_48;
    s32 field_4C;
    f32 field_50;
    s32 field_54;
    s32 field_58;
    s32 field_5C;
} EffSnowmanWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_SnowmanN64_802fbfd0[];
extern f32 float_0_804260c0;
extern f32 float_neg1_804260cc;
extern f32 float_200_804260f0;
extern f32 float_neg8_804260f4;

void effSnowmanDisp(void) {
    ;
}


u8 effSnowmanMain(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effSnowmanN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    void* entry;
    EffSnowmanWork* work;
    f32 negOne;
    f32 negEight;
    f32 twoHundred;
    f32 zero;
    s32 zeroInt;
    s32 zeroInt2;
    s32 two;
    s32 i200;
    s32 i220;
    s32 i240;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_SnowmanN64_802fbfd0;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x60);
    *(EffSnowmanWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effSnowmanMain;
    *(u32*)entry |= 2;

    zeroInt = 0;
    work->type = type;
    work->frame = zeroInt;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }

    zeroInt2 = 0;
    zero = float_0_804260c0;
    work->field_24 = zeroInt2;
    two = 2;
    twoHundred = float_200_804260f0;
    i200 = 200;
    work->x = x;
    i220 = 220;
    i240 = 240;
    negEight = float_neg8_804260f4;
    work->y = y;
    negOne = float_neg1_804260cc;
    work->z = z;
    work->field_30 = zero;
    work->field_34 = twoHundred;
    work->field_38 = zero;
    work->field_3C = zero;
    work->field_40 = zero;
    work->field_44 = zero;
    work->field_54 = zeroInt2;
    work->field_58 = two;
    work->field_28 = scale;
    work->field_5C = zeroInt2;
    work->field_18 = i200;
    work->field_1C = i220;
    work->field_20 = i240;
    work->field_40 = negEight;
    work->field_2C = zero;
    work->field_50 = negOne;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
