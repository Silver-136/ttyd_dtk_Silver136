#include "event/evt_offscreen.h"

extern s32 evtGetValue();
extern void offscreenDelete(s32);
extern void offscreenEntry(s32);
extern s32* offscreenNameToPtr(s32);


s32 evt_offscreen_get_boundingbox2(int param_1, int param_2) {
    return 0;
}


s32 evt_offscreen_get_boundingbox(int param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_offscreen_set_texture_fmt(int param_1) {
    void* args = *(void**)(param_1 + 0x18);
    s32 name = evtGetValue(param_1, *(s32*)args);
    s32 format = evtGetValue(param_1, *(s32*)((s32)args + 4));
    s32 type = evtGetValue(param_1, *(s32*)((s32)args + 8));
    s32* offscreen = offscreenNameToPtr(name);

    offscreen[0xF] = format;
    offscreen[0x10] = type;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_offscreen_delete(int param_1) {
    offscreenDelete(evtGetValue(param_1, **(s32**)(param_1 + 0x18)));
    return 2;
}


s32 evt_offscreen_entry(int param_1) {
    offscreenEntry(evtGetValue(param_1, **(s32**)(param_1 + 0x18)));
    return 2;
}
