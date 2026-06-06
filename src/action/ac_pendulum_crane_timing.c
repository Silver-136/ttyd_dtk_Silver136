#include "action/ac_pendulum_crane_timing.h"

u16 _hp_tbl[14];
extern void* _battleWorkPointer;
extern f32 float_0_80426eb8;
extern f32 float_45_80426ebc;
extern f32 float_100_80426ec0;
extern f32 float_90_80426ec4;
extern f32 float_neg300_80426ee4;
extern f32 float_30_80426ee8;

f32 compAngle(f32 a, f32 b);
f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
void actionCommandDisp(f32 x, f32 y);

s32 battleAcResult_PendulumCraneTiming(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_PendulumCraneTiming(void* wp) {
    if ((*(u32*)((s32)wp + 0x1CC0) & 1) != 0) {
        return;
    }
    *(s32*)((s32)wp + 0x1C9C) = 1002;
}

s32 _get_angle_hp(s32 hp) {
    u16* entry = _hp_tbl;

    while (hp > entry[0]) {
        entry += 2;
    }
    return entry[1];
}

s32 _get_angle_rate(f32 angle) {
    f32 base;
    f32 diff;
    f32 sum;
    f32 scale;
    s32 value;

    base = float_0_80426eb8;
    diff = compAngle(base, angle);
    sum = diff + float_45_80426ebc;
    scale = float_100_80426ec0;
    value = (s32)((scale * sum) / float_90_80426ec4);

    if (value < 0) {
        value = 0;
    }
    if (value > 100) {
        value = 100;
    }
    return value;
}

void battleAcDisp_PendulumCraneTiming(void) {
    s32 work = (s32)_battleWorkPointer + 0x1F20;
    s32 state = *(s32*)(work - 0x284);
    s32 timer;

    if (state == 0x3E8) {
        goto intro_display;
    }
    if (state >= 0x3E8) {
        if (state >= 0x3EC) {
            return;
        }
        if (state >= 0x3EA) {
            goto outro;
        }
        return;
    }
    if (state == 0x64) {
        goto intro_display;
    }
    if (state >= 0x64) {
        return;
    }
    if (state < 0x63) {
        return;
    }

    *(f32*)(work + 0x14) = intplGetValue(4, 0x14 - *(s32*)(work + 0x20), 0x14, float_neg300_80426ee4, float_30_80426ee8);

intro_display:
    actionCommandDisp(*(f32*)(work + 0x14), *(f32*)(work + 0x18));
    timer = *(s32*)(work + 0x20);
    if (timer > 0) {
        *(s32*)(work + 0x20) = timer - 1;
    }
    return;

outro:
    timer = *(s32*)(work + 0x20);
    if (timer >= 0x28) {
        *(f32*)(work + 0x14) = intplGetValue(4, timer - 0x28, 0x14, float_30_80426ee8, float_neg300_80426ee4);
    } else {
        *(f32*)(work + 0x14) = float_30_80426ee8;
    }
    actionCommandDisp(*(f32*)(work + 0x14), *(f32*)(work + 0x18));
    timer = *(s32*)(work + 0x20);
    if (timer < 0x3C) {
        *(s32*)(work + 0x20) = timer + 1;
    }
}

