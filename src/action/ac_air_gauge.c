#include "action/ac_air_gauge.h"

void battleAcDelete_AirGauge(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1002;
}

s32 battleAcResult_AirGauge(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_AirGauge(void* camera, void* wp) {
    s32 disp;
    s32 state;
    s32 timer;

    extern f32 float_neg300_80426fc8;
    extern f32 float_30_80426fcc;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void actionCommandDisp(f32 x, f32 y);

    disp = (s32)wp + 0x1F20;
    state = *(s32*)((s32)wp + 0x1C9C);

    switch (state) {
        case 99:
            *(f32*)(disp + 0x14) = intplGetValue(
                4,
                0x14 - *(s32*)(disp + 0x20),
                0x14,
                float_neg300_80426fc8,
                float_30_80426fcc);
            /* fallthrough */
        case 100:
        case 1000:
            actionCommandDisp(*(f32*)(disp + 0x14), *(f32*)(disp + 0x18));
            timer = *(s32*)(disp + 0x20);
            if (timer > 0) {
                *(s32*)(disp + 0x20) = timer - 1;
            }
            break;
        case 1002:
        case 1003:
            timer = *(s32*)(disp + 0x20);
            if (timer >= 0x28) {
                *(f32*)(disp + 0x14) = intplGetValue(
                    4,
                    timer - 0x28,
                    0x14,
                    float_30_80426fcc,
                    float_neg300_80426fc8);
            } else {
                *(f32*)(disp + 0x14) = float_30_80426fcc;
            }
            actionCommandDisp(*(f32*)(disp + 0x14), *(f32*)(disp + 0x18));
            timer = *(s32*)(disp + 0x20);
            if (timer < 0x3C) {
                *(s32*)(disp + 0x20) = timer + 1;
            }
            break;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

f32 _GetGaugeDecValue(void) {
    extern void* _battleWorkPointer;
    extern s32 lbl_803AEDB8[];
    extern f32 float_0p3_80426fd0;
    extern f32 float_0p33_80426fd4;
    extern f32 float_0p36_80426fd8;
    extern f32 float_0p4_80426fdc;
    extern f32 float_0p44_80426fe0;
    extern f32 float_0p47_80426fe4;
    extern f32 float_0p5_80426fe8;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);

    s32 battleWork;
    s32 tableBase;
    s32 gaugeWork;
    s32 entry;
    f32 value;
    s32 count;

    battleWork = (s32)_battleWorkPointer;
    tableBase = (s32)lbl_803AEDB8;
    gaugeWork = battleWork + 0x1F4C;

    switch (*(u32*)(battleWork + 0x1CE4)) {
        case 0:
            value = float_0p3_80426fd0;
            entry = tableBase + 0x40;
            break;
        case 1:
            value = float_0p33_80426fd4;
            entry = tableBase + 0x7C;
            break;
        case 2:
            value = float_0p36_80426fd8;
            entry = tableBase + 0xB8;
            break;
        case 3:
            value = float_0p4_80426fdc;
            entry = tableBase + 0xF4;
            break;
        case 4:
            value = float_0p44_80426fe0;
            entry = tableBase + 0x130;
            break;
        case 5:
            value = float_0p47_80426fe4;
            entry = tableBase + 0x16C;
            break;
        case 6:
        default:
            entry = tableBase + 0x1A8;
            value = float_0p5_80426fe8;
            break;
    }

    count = *(s32*)(gaugeWork + 0xC);
    while (count > *(u32*)entry) {
        entry += 0xC;
        if (*(s32*)entry < 0) {
            break;
        }
    }

    if (*(s32*)entry <= 0) {
        *(s32*)(gaugeWork + 0x10) = 1;
        return value;
    }

    *(s32*)(gaugeWork + 0x10) = *(s32*)(entry + 8);
    value -= intplGetValue(
        0,
        count - *(s32*)(entry - 0xC),
        *(s32*)entry - *(s32*)(entry - 0xC),
        *(f32*)(entry - 8),
        *(f32*)(entry + 4));
    return value;
}
