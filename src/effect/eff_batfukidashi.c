#include "effect/eff_batfukidashi.h"

typedef struct Vec {
    f32 x;
    f32 y;
    f32 z;
} Vec;

typedef struct EffCoinFukidashiWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    s32 item;
    f32 unk18;
    s32 alpha;
    s32 timer;
    s32 counter;
} EffCoinFukidashiWork;

extern const Vec vec3_802fddb0;
extern u8 offset_tbl[];
extern const char str_CoinFukidashi_802fddc8[];
extern f32 float_0_804267d8;
extern f32 float_1_804267dc;
extern f32 float_100_804267f4;

void effCoinFukidashiDisp(void);
void* effEntry(s32 type, s32 item, f32 x, f32 y, f32 z, s32 timer);
void effDelete(void* effect);
void* __memAlloc(s32 heap, u32 size);
f32 dispCalcZ(Vec* pos);
void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 priority);

void effCoinFukidashiMain(void* effect) {
    Vec dispPos;
    Vec pos = vec3_802fddb0;
    EffCoinFukidashiWork* work = *(EffCoinFukidashiWork**)((s32)effect + 0xC);
    u32 flags;
    s32 timer;
    s32 alpha;

    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    flags = *(u32*)effect;
    if ((flags & 4) != 0) {
        *(u32*)effect = flags & ~4;
        work->timer = 0x10;
    }

    timer = work->timer;
    if (timer < 1000) {
        work->timer = timer - 1;
    }

    if (work->timer < 0x10) {
        work->alpha = work->timer << 4;
    } else {
        alpha = work->alpha + 0x10;
        work->alpha = alpha;
        if (work->alpha > 0xFF) {
            work->alpha = 0xFF;
        }
    }

    if (work->timer < 0) {
        effDelete(effect);
        return;
    }

    work->scale = (f32)offset_tbl[work->counter] / float_100_804267f4;
    work->counter++;
    if ((u32)work->counter >= 0x3D) {
        work->counter = 0;
    }
    dispEntry(4, 2, effCoinFukidashiDisp, effect, dispCalcZ(&dispPos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effCoinFukidashiEntry(s32 type, s32 item, f32 x, f32 y, f32 z, s32 timer) {
    void* effect = effEntry(type, item, x, y, z, timer);
    EffCoinFukidashiWork* work;
    f32 zero;
    f32 one;

    *(const char**)((s32)effect + 0x14) = str_CoinFukidashi_802fddc8;
    *(s32*)((s32)effect + 0x8) = 1;
    work = __memAlloc(3, *(s32*)((s32)effect + 0x8) * 0x28);
    *(void**)((s32)effect + 0xC) = work;
    zero = float_0_804267d8;
    *(void**)((s32)effect + 0x10) = effCoinFukidashiMain;
    one = float_1_804267dc;
    *(u32*)effect |= 2;

    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = zero;
    work->unk18 = one;
    work->item = item;
    if (timer <= 0) {
        work->timer = 1000;
    } else {
        work->timer = timer;
    }
    work->counter = 0;
    work->alpha = 0;
    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
