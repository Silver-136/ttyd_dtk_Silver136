#include "event/evt_fade.h"

extern s32 evtGetValue();
extern f32 evtGetFloat();
extern void fadeReset();
extern void fadeSoftFocusOn();
extern void fadeSoftFocusOff();
extern s32 fadeIsFinish();
extern void fadeSetAnimOfsPos(f32, f32);

s32 evt_fade_set_spot_pos(int param_1) {
    return 0;
}


s32 evt_fade_set_mapchange_type(int param_1) {
    return 0;
}


s32 evt_fade_entry(void* pEvt) {
    return 0;
}


s32 evt_fade_tec_onoff(int param_1) {
    return 0;
}


s32 evt_fade_set_anim_virtual_pos(int param_1) {
    return 0;
}


s32 evt_fade_out(void* pEvt, int param_2) {
    return 0;
}


s32 evt_fade_in(void* pEvt, int param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_fade_set_anim_ofs_pos(int param_1) {
    void* args = *(void**)((s32)param_1 + 0x18);
    f32 x = evtGetFloat(param_1, *(s32*)args);
    f32 y = evtGetFloat(param_1, *(s32*)((s32)args + 4));

    fadeSetAnimOfsPos(x, y);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_fade_softfocus_onoff(int param_1) {
    if (evtGetValue(param_1, **(s32**)((s32)param_1 + 0x18)) != 0) {
        fadeSoftFocusOn();
    } else {
        fadeSoftFocusOff();
    }
    return 2;
}


u32 evt_fade_end_wait(void) {
    s32 done = fadeIsFinish();
    return 2 & ~((done - 1 | 1 - done) >> 31);
}


s32 evt_fade_reset(void* param_1) {
    fadeReset(evtGetValue(param_1, **(s32**)((s32)param_1 + 0x18)));
    return 2;
}
