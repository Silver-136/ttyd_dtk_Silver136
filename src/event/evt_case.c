#include "event/evt_case.h"

extern s32 evtGetValue();
extern void caseDelete(s32);
extern s32* caseIdToPtr(s32);


s32 evt_run_case_evt_bero(void* pEvt) {
    return 0;
}


s32 evt_run_case_evt(void* pEvt) {
    return 0;
}


s32 evt_del_case_evt(void* pEvt) {
    return 0;
}


u8 evtRunCaseEntry(u16 param_1, int param_2, s32 param_3, s32 param_4, s32 param_5, void* param_6) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_set_case_wrk(void* pEvt) {
    void* args = *(void**)((s32)pEvt + 0x18);
    s32 id = evtGetValue(pEvt, *(s32*)args);
    s32 index = evtGetValue(pEvt, *(s32*)((s32)args + 4));
    s32 value = evtGetValue(pEvt, *(s32*)((s32)args + 8));
    s32* entry = caseIdToPtr(id);

    entry[index + 0x15] = value;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_exit_case_evt(void* pEvt) {
    caseDelete(*(s32*)((s32)pEvt + 0x16C));
    return 2;
}
