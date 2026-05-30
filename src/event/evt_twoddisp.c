#include "event/evt_twoddisp.h"

s32 twoddisp_main(void) {
    extern void twoddisp_disp(void);
    extern f32 float_1000_80422f88;
    extern void dispEntry(s32, s32, void*, s32, f32);
    s32 i = 0;

    for (; i < 96; i += 8) {
    }
    for (; i < 100; i++) {
    }
    dispEntry(8, 0, twoddisp_disp, 0, float_1000_80422f88);
    return 2;
}


s32 evt_twoddisp_init(int param_1) {
    return 0;
}


void twoddisp_disp(void) {
}


s32 evt_twoddisp_flag_onoff(int param_1) {
    return 0;
}


s32 evt_twoddisp_entry(void* pEvt) {
    return 0;
}


s32 evt_twoddisp_set_base_position(void* pEvt) {
    return 0;
}


s32 evt_twoddisp_set_position(int param_1) {
    return 0;
}
