#include "effect/n64/eff_freeze_n64.h"

typedef struct EffFreezeWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    s32 frame;
    u8 colorR;
    u8 colorG;
    u8 colorB;
    u8 alpha;
    u8 color2R;
    u8 color2G;
    u8 color2B;
    u8 color2A;
    f32 scale;
    u8 unk_24;
    u8 unk_25;
    u8 pad_26[2];
    void* snowDust;
} EffFreezeWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void* effSnowDustN64Entry(s32, s32, s32, f32, f32, f32, f32, f32, f32);

extern char str_FreezeN64_802fafe8[];
extern f32 float_10_80425388;
extern f32 float_32_804253a8;
extern f32 float_30_804253ac;


u8 effFreezeMain(void) {
    return 0;
}


u8 effFreezeDisp(int param_1, int param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effFreezeN64Entry(s32 type, s32 param2, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffFreezeWork* work;
    s32 flags;
    f32 ten;
    f32 dustY;

    *(char**)((s32)entry + 0x14) = str_FreezeN64_802fafe8;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x2C);
    *(EffFreezeWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFreezeMain;
    flags = *(s32*)entry;
    *(s32*)entry = flags | 2;
    work->type = type;
    work->frame = 0;
    if (param2 <= 0) {
        work->timer = 1000;
    } else {
        work->timer = param2;
    }
    ten = float_10_80425388;
    work->alpha = 0xFF;
    work->x = x;
    work->y = y;
    dustY = ten + y;
    work->z = z;
    work->scale = scale;
    work->colorR = 0xFF;
    work->colorG = 0xFF;
    work->colorB = 0xFF;
    work->color2R = 0xF0;
    work->color2G = 0xFF;
    work->color2B = 0xFF;
    work->color2A = 0xFF;
    work->unk_25 = 0;
    work->unk_24 = 0;
    work->snowDust = effSnowDustN64Entry(1, 4, 0, x, dustY, z,
                                         float_32_804253a8, float_30_804253ac, scale);

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
