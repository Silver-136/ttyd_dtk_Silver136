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
