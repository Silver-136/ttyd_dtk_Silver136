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

    if (*(s32*)((s32)wp + 0x1CCC) == 1) {
        return *(s32*)((s32)wp + 0x1CE4) + ac_signaltiming_ok_frame_range_2[idx];
    }
    return *(s32*)((s32)wp + 0x1CE4) + ac_signaltiming_ok_frame_range[idx];
}

