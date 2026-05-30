#include "unit/party/unit_party_vivian.h"

s32 _disp_heart_entry_stop_check(void* evt) {
    extern void evtSetValue(void* evt, s32 arg, s32 value);

    evtSetValue(evt, **(s32**)((s32)evt + 0x18), *(s32*)((s32)evt + 0x8C));
    return 2;
}

s32 _disp_heart_entry_stop(void* evt) {
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void* evtGetPtrID(s32 id);

    void* ptr = evtGetPtrID(evtGetValue(evt, **(s32**)((s32)evt + 0x18)));

    *(s32*)((s32)ptr + 0x8C) = 1;
    return 2;
}


u8 battle_evt_majo_disp_on(void) {
    return 0;
}


void __makeTechMenuFunc(void* commandWork, s32 param_2) {
}


s32 battle_evt_majo_disp_off(int param_1, int param_2) {
    return 0;
}


u8 _disp_heart(void) {
    return 0;
}


u32 _disp_heart_entry(void* evt, int isFirstCall) {
    return 0;
}


u8 _get_move_frame(void) {
    return 0;
}


s32 _make_kagenuke_weapon(int param_1) {
    return 0;
}


s32 _vivian_make_extra_work_area(int param_1) {
    return 0;
}
