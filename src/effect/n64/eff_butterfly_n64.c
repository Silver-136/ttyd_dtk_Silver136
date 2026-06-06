#include "effect/n64/eff_butterfly_n64.h"

typedef struct EffButterflyWork {
    s32 type;
    s32 timer;
    s32 unk_08;
    f32 x;
    f32 y;
    f32 z;
    f32 x2;
    f32 y2;
    f32 z2;
    s32 alpha;
    s32 unk_28;
    s32 unk_2c;
    s32 unk_30;
    f32 unk_34;
    f32 unk_38;
    u8 pad_3c[0xC];
} EffButterflyWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);

extern char str_ButterflyN64_802facf0[];
extern f32 float_50_80424f08;
extern f32 float_30_80424f0c;


u8 effButterflyMain(void) {
    return 0;
}


u8 effButterflyDisp(int param_1, int param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effButterflyN64Entry(s32 type, f32 x, f32 y, f32 z) {
    void* entry = effEntry();
    EffButterflyWork* work;
    u32 flags;
    f32 f30;
    f32 f50;

    *(char**)((s32)entry + 0x14) = str_ButterflyN64_802facf0;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x48);
    *(EffButterflyWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effButterflyMain;
    f50 = float_50_80424f08;
    flags = *(u32*)entry;
    f30 = float_30_80424f0c;
    *(u32*)entry = flags | 2;
    work->type = type;
    work->timer = 1000;
    work->alpha = 0xFF;
    work->x = x;
    work->x2 = x;
    work->y = y;
    work->y2 = y;
    work->z = z;
    work->z2 = z;
    work->unk_08 = 0;
    work->unk_2c = 0;
    work->unk_30 = 0;
    work->unk_34 = f50;
    work->unk_38 = f30;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
