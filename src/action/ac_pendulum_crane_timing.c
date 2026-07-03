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



/* CHATGPT STUB FILL: main/action/ac_pendulum_crane_timing 20260624_184128 */

/* stub-fill: actionCommandDisp | prototype_only | source_prototype */
void actionCommandDisp(f32 x, f32 y) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void* _battleWorkPointer;
    extern u32 vec3_802fe4e8[];
    extern f32 float_neg200_80426ec8;
    extern f32 float_25_80426ecc;
    extern f32 float_1_80426ed0;
    extern f32 float_88_80426ed4;
    extern f32 float_50_80426ed8;
    extern f32 float_100_80426ec0;
    extern f32 float_neg288_80426ee0;
    extern f32 float_45_80426ebc;
    extern void* camGetPtr(s32 cameraId);
    extern void iconDispGx(f32 scale, Vec* pos, s32 flags, s32 iconId);
    extern void btlDispGXInit2DRasta(void);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void btlDispGXQuads2DRasta(double x1, double y1, double x2, double y2, s32 r, s32 g, s32 b, s32 a);
    Vec iconPos;
    Vec markerPos;
    void* cam;
    f32 baseX;
    f32 barY;
    f32 x1;
    f32 x2;
    s32 i;
    s32 intensity;

    camGetPtr(8);
    iconPos = *(Vec*)vec3_802fe4e8;
    baseX = float_neg200_80426ec8 + x;
    iconPos.x = baseX;
    iconPos.y = float_25_80426ecc + y;
    iconDispGx(float_1_80426ed0, &iconPos, 0x10, 0x94);

    cam = camGetPtr(8);
    btlDispGXInit2DRasta();
    GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);

    x1 = baseX - float_88_80426ed4;
    barY = float_50_80426ed8 + y;
    for (i = 0; i < 7; i++) {
        if (i != 0) {
            x1 = baseX - float_88_80426ed4 + (f32)(_hp_tbl[(i - 1) * 2] * 0xAF) / float_100_80426ec0;
        }
        x2 = baseX - float_88_80426ed4 + (f32)(_hp_tbl[i * 2] * 0xAF) / float_100_80426ec0;
        intensity = 3 - ((3 - i) < 0 ? -(3 - i) : (3 - i));
        btlDispGXQuads2DRasta((double)x1, (double)barY, (double)x2, (double)barY, intensity * 0x3C, 0, intensity * -0x3C - 0x4C, 0xFF);
    }

    markerPos = *(Vec*)(vec3_802fe4e8 + 3);
    markerPos.x = float_neg288_80426ee0 + x + (f32)(*(s32*)((s32)_battleWorkPointer + 0x1CD0) * 0xB0) / float_100_80426ec0;
    markerPos.y = float_45_80426ebc + y;
    iconDispGx(float_1_80426ed0, &markerPos, 0x10, 0x9E);
}

/* stub-fill: battleAcMain_PendulumCraneTiming | missing_definition | ghidra_signature */
u8 battleAcMain_PendulumCraneTiming(void) {
    return 0;
}
