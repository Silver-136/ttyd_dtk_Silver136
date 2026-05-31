#include "event/evt_bg.h"

extern s32 evtGetValue();
extern f32 evtGetFloat();
extern void bgAutoScrollOff();
extern void bgAutoScrollOn();
extern void bgDispOn();
extern void bgDispOff();
extern void bgSetScrlOffset(f32, f32);


s32 evt_bg_set_color(int param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_bg_set_scrl_offset(int param_1) {
    void* args = *(void**)((s32)param_1 + 0x18);
    f32 x = evtGetFloat(param_1, *(s32*)args);
    f32 y = evtGetFloat(param_1, *(s32*)((s32)args + 4));

    bgSetScrlOffset(x, y);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_bg_auto_scroll_onoff(int param_1) {
    if (evtGetValue(param_1, **(s32**)((s32)param_1 + 0x18)) != 0) {
        bgAutoScrollOff();
    } else {
        bgAutoScrollOn();
    }
    return 2;
}


s32 evt_bg_disp_onoff(int param_1) {
    if (evtGetValue(param_1, **(s32**)((s32)param_1 + 0x18)) != 0) {
        bgDispOn();
    } else {
        bgDispOff();
    }
    return 2;
}
