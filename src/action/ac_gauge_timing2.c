#include "action/ac_gauge_timing2.h"

s32 battleAcResult_GaugeTiming2(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_GaugeTiming2(void* wp) {
    if ((*(u32*)((s32)wp + 0x1CC0) & 1) != 0) {
        return;
    }
    *(s32*)((s32)wp + 0x1C9C) = 1003;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_GaugeTiming2(void* camera, void* wp) {
    typedef struct GaugeTimingDispWork {
        u8 pad_00[0x14];
        f32 x;
        f32 y;
        u8 pad_1C[4];
        s32 timer;
    } GaugeTimingDispWork;
    GaugeTimingDispWork* disp;
    s32 state;
    s32 timer;

    extern f32 float_neg300_804280d0;
    extern f32 float_30_804280d4;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void actionCommandDisp(f32 x, f32 y);

    disp = (GaugeTimingDispWork*)((s32)wp + 0x1F20);
    state = *(s32*)((s32)wp + 0x1C9C);
    switch (state) {
        case 99:
            disp->x = intplGetValue(4, 0x14 - disp->timer, 0x14, float_neg300_804280d0, float_30_804280d4);
        case 100:
        case 1000:
        case 1002:
            actionCommandDisp(disp->x, disp->y);
            timer = disp->timer;
            if (timer > 0) {
                disp->timer = timer - 1;
            }
            break;
        case 1003:
        case 1004:
            timer = disp->timer;
            if (timer >= 0x28) {
                disp->x = intplGetValue(4, timer - 0x28, 0x14, float_30_804280d4, float_neg300_804280d0);
            } else {
                disp->x = float_30_804280d4;
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
