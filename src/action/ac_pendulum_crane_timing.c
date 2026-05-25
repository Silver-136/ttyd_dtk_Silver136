#include "action/ac_pendulum_crane_timing.h"

u16 _hp_tbl[14];
extern f32 float_0_80426eb8;
extern f32 float_45_80426ebc;
extern f32 float_100_80426ec0;
extern f32 float_90_80426ec4;

f32 compAngle(f32 a, f32 b);

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
    s32 value = (s32)((float_100_80426ec0 * (compAngle(float_0_80426eb8, angle) + float_45_80426ebc)) / float_90_80426ec4);

    if (value < 0) {
        value = 0;
    }
    if (value > 100) {
        value = 100;
    }
    return value;
}
