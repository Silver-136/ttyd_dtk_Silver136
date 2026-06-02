#include "event/evt_win.h"


void unk_8017b330(void) {
    extern void winMgrHelpMain();

    winMgrHelpMain();
}


void evt_unitwin_disp_func(s32 param_1, void* param_2) {
}


s32 evt_unitwin_selltable_setup(int param_1) {
    return 0;
}


s32 evt_unitwin_free(s32 param_1, int param_2) {
    return 0;
}


s32 evt_unitwin_alloc(void* pEvt) {
    return 0;
}


s32 unitwin_setmariolevel(void* pEvt) {
    return 0;
}


s32 set_new_goods_list(void* pEvt) {
    return 0;
}


s32 unitwin_setpartylank(int param_1) {
    return 0;
}


s32 evt_unitwin_main_func(void* pEvt) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 unitwincoin_disp_onoff(void* pEvt) {
    extern s32 evtGetValue(void* entry, s32 index);
    extern void* pouchGetPtr(void);
    extern s32 pouchGetCoin(void);
    extern void* _wp;
    s32 onoff;
    void* work;

    onoff = evtGetValue(pEvt, **(s32**)((s32)pEvt + 0x18));
    pouchGetPtr();
    work = _wp;
    *(s32*)((s32)work + 0x2C) = pouchGetCoin();
    *(s32*)((s32)work + 0x5C) = 0;
    if (onoff != 0) {
        *(u16*)work |= 0x1000;
    } else {
        *(u16*)work &= ~0x1000;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 unitwin_getpartylank(void* pEvt) {
    extern void* pouchGetPtr(void);
    extern s32 evtGetValue(void* entry, s32 index);
    extern s32 evtSetValue(void* entry, s32 index, s32 value);
    s32* args;
    void* pouch;
    void* partyData;
    s32 party;

    args = *(s32**)((s32)pEvt + 0x18);
    pouch = pouchGetPtr();
    party = evtGetValue(pEvt, args[0]);
    partyData = (void*)((s32)pouch + party * 0xE);
    evtSetValue(pEvt, args[1], *(s16*)((s32)partyData + 0xC));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u32 unitwincoin_wait(void) {
    return 0;
}


s32 unitwin_get_work_ptr(void* pEvt) {
    extern s32 evtSetValue(void* entry, s32 index, s32 value);
    extern void* _wp;

    evtSetValue(pEvt, **(s32**)((s32)pEvt + 0x18), (s32)_wp);
    return 2;
}
