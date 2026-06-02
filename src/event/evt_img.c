#include "event/evt_img.h"

s32 evt_img_entry(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void imgEntry(s32 name, s32 flag);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 flag = *(s32*)((s32)gp + 0x14);

    imgEntry(name, ((u32)(-flag) | (u32)flag) >> 31);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_img_set_shadow(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void imgSetShadow(void* img, s32 shadow);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 shadow = evtGetValue(event, args[1]);
    s32 flag = *(s32*)((s32)gp + 0x14);

    imgSetShadow(imgNameToPtr(name, ((u32)(-flag) | (u32)flag) >> 31), shadow);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 N_evt_img_set_z(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern f32 evtGetFloat(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    f32 z = evtGetFloat(event, args[1]);
    s32 flag = *(s32*)((s32)gp + 0x14);
    void* img = imgNameToPtr(name, ((u32)(-flag) | (u32)flag) >> 31);

    *(f32*)((s32)img + 0xFC) = z;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_img_alloc_capture(void* pEvt) {
    return 0;
}


s32 evt_img_set_virtual_point(int param_1) {
    return 0;
}


s32 evt_img_set_color(int param_1) {
    return 0;
}


s32 evt_img_set_paper(int param_1) {
    return 0;
}


s32 evt_img_set_position(int param_1) {
    return 0;
}


s32 evt_img_onoff(int param_1) {
    return 0;
}


s32 evt_img_set_paper_anim(int param_1) {
    return 0;
}


s32 evt_img_set_paper_timerate(int param_1) {
    return 0;
}


s32 evt_img_wait_animend(int param_1, int param_2) {
    return 0;
}


s32 evt_img_free_capture(int param_1) {
    return 0;
}


s32 evt_img_release(int param_1) {
    return 0;
}


s32 evt_img_clear_virtual_point(int param_1) {
    return 0;
}
