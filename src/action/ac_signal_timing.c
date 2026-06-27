#include "action/ac_signal_timing.h"

extern void* _battleWorkPointer;
s32 ac_signaltiming_ok_frame_range[4];
s32 ac_signaltiming_ok_frame_range_2[4];

s32 battleAcResult_SignalTiming(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_SignalTiming(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1004;
}

s32 _get_ok_frame_range(s32 idx) {
    void* wp = _battleWorkPointer;

    switch (*(s32*)((s32)wp + 0x1CCC)) {
        case 1:
            return *(s32*)((s32)wp + 0x1CE4) + ac_signaltiming_ok_frame_range_2[idx];
        default:
            return *(s32*)((s32)wp + 0x1CE4) + ac_signaltiming_ok_frame_range[idx];
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_SignalTiming(void* camera, void* wp) {
    typedef struct SignalTimingDispWork {
        u8 pad_00[0x14];
        f32 x;
        f32 y;
        u8 pad_1C[4];
        s32 timer;
    } SignalTimingDispWork;
    SignalTimingDispWork* disp;
    s32 state;
    s32 timer;

    extern f32 float_neg300_80427c70;
    extern f32 float_30_80427c74;
    extern f32 float_0_80427c54;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void actionCommandDisp(f32 x, f32 y);

    disp = (SignalTimingDispWork*)((s32)wp + 0x1F20);
    state = *(s32*)((s32)wp + 0x1C9C);

    switch (state) {
        case 99:
        case 100:
            disp->x = intplGetValue(
                4,
                0x14 - disp->timer,
                0x14,
                float_neg300_80427c70,
                float_30_80427c74);
            actionCommandDisp(disp->x, disp->y);
            timer = disp->timer;
            if (timer > 0) {
                disp->timer = timer - 1;
            }
            break;
        case 1000:
        case 1001:
        case 1002:
        case 1003:
            actionCommandDisp(float_30_80427c74, float_0_80427c54);
            break;
        case 1004:
        case 1005:
            timer = disp->timer;
            if (timer >= 0x28) {
                disp->x = intplGetValue(
                    4,
                    timer - 0x28,
                    0x14,
                    float_30_80427c74,
                    float_neg300_80427c70);
            } else {
                disp->x = float_30_80427c74;
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


/* CHATGPT STUB FILL: main/action/ac_signal_timing 20260624_184823 */

/* stub-fill: actionCommandDisp | prototype_only | source_prototype */
void actionCommandDisp(f32 x, f32 y) {
    return;
}

/* stub-fill: battleAcMain_SignalTiming | missing_definition | ghidra_signature */
s32 battleAcMain_SignalTiming(int param_1) {
    return 0;
}
