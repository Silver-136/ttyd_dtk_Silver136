#include "effect/n64/eff_fire_ring_n64.h"

typedef struct EffFireRingWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    u8 pad_10[0xC];
    f32 unk_1c;
    f32 scaleStep;
    s32 alpha;
    s32 duration;
    s32 frame;
    f32 unk_30;
    f32 unk_34;
    f32 unk_38;
    f32 unk_3c;
    f32 unk_40;
    f32 unk_44;
    f32 unk_48;
    f32 unk_4c;
    u8 colorR;
    u8 colorG;
    u8 colorB;
    u8 pad_53;
} EffFireRingWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);

extern char str_FireRingN64_802faec8[];
extern f32 float_0_804251e4;
extern f32 float_neg1p1_804251fc;
extern f32 float_0p4_80425200;
extern f32 float_1_80425204;
extern f32 float_0p1_80425208;


u8 effFireRingDisp(int param_1, int param_2) {
    return 0;
}


u8 effFireRingMain(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effFireRingN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffFireRingWork* work;
    f32 zero;
    f32 scaleStep;

    *(char**)((s32)entry + 0x14) = str_FireRingN64_802faec8;
    *(s32*)((s32)entry + 0x8) = 0x20;
    work = __memAlloc(3, 0xA80);
    *(EffFireRingWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFireRingMain;
    scaleStep = float_0p1_80425208 * scale;
    zero = float_0_804251e4;

    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->alpha = 0xFF;
    work->duration = duration;
    work->frame = 0;
    work->unk_30 = zero;
    work->unk_34 = zero;
    work->unk_38 = zero;
    work->unk_3c = zero;
    work->unk_40 = float_neg1p1_804251fc;
    work->unk_44 = zero;
    work->unk_48 = float_0p4_80425200;
    work->unk_4c = float_1_80425204;
    work->unk_1c = zero;
    work->scaleStep = scaleStep;
    work->colorR = 0xFF;
    work->colorG = 0xFF;
    work->colorB = 0x40;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
