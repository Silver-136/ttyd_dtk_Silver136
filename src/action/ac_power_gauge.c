#include "action/ac_power_gauge.h"

s32 battleAcResult_PowerGauge(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_PowerGauge(void* wp) {
    if ((*(u32*)((s32)wp + 0x1CC0) & 1) != 0) {
        return;
    }
    *(s32*)((s32)wp + 0x1C9C) = 1004;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_PowerGauge(void* camera, void* wp) {
    typedef struct PowerGaugeDispWork {
        u8 pad_00[0x14];
        f32 x;
        f32 y;
        u8 pad_1C[4];
        s32 timer;
    } PowerGaugeDispWork;
    PowerGaugeDispWork* disp;
    s32 state;
    s32 timer;

    extern f32 float_neg300_80427b14;
    extern f32 float_30_80427b18;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void actionCommandDisp(f32 x, f32 y);

    disp = (PowerGaugeDispWork*)((s32)wp + 0x1F20);
    state = *(s32*)((s32)wp + 0x1C9C);

    switch (state) {
        case 99:
            disp->x = intplGetValue(
                4,
                0x14 - disp->timer,
                0x14,
                float_neg300_80427b14,
                float_30_80427b18);
            /* fallthrough */
        case 100:
        case 1000:
        case 1001:
        case 1003:
            actionCommandDisp(disp->x, disp->y);
            timer = disp->timer;
            if (timer > 0) {
                disp->timer = timer - 1;
            }
            break;
        case 1004:
        case 1005:
            timer = disp->timer;
            if (timer >= 0x28) {
                disp->x = intplGetValue(
                    4,
                    timer - 0x28,
                    0x14,
                    float_30_80427b18,
                    float_neg300_80427b14);
            } else {
                disp->x = float_30_80427b18;
            }
            actionCommandDisp(disp->x, disp->y);
            timer = disp->timer;
            if (timer < 0x3C) {
                disp->timer = timer + 1;
            }
            break;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
