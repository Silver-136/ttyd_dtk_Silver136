#include "event/evt_bg.h"

extern s32 evtGetValue();
extern f32 evtGetFloat();
extern void bgAutoScrollOff();
extern void bgAutoScrollOn();
extern void bgDispOn();
extern void bgDispOff();
extern void bgSetScrlOffset(f32, f32);


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_bg_set_color(int param_1) {
    extern void bgSetColor(void* color);
    extern s32 unk_80429590;
    s32* args = *(s32**)(param_1 + 0x18);
    s32 r = evtGetValue(param_1, args[0]);
    s32 g = evtGetValue(param_1, args[1]);
    s32 b = evtGetValue(param_1, args[2]);
    s32 a = evtGetValue(param_1, args[3]);
    u32 sendColor;
    u32 color = unk_80429590;

    ((u8*)&color)[0] = r;
    ((u8*)&color)[1] = g;
    ((u8*)&color)[2] = b;
    ((u8*)&color)[3] = a;
    sendColor = color;
    bgSetColor(&sendColor);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

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
