#include "event/evt_msg.h"

typedef struct EvtEntry {
    u8 pad_0[0x18];
    s32* args;
    u8 pad_1[0x15C];
    s32 msgWindowId;
    u8 pad_2[0x14];
    s32 value;
} EvtEntry;

s32 evtGetValue(EvtEntry* evt, s32 value);
void msgWindow_ForceClose(s32 id);
void msgWindow_Continue(s32 id);
s32 windowCheckID(s32 id);

void evt_msg_init(void) {
}

s32 evt_msg_pri(EvtEntry* evt) {
    evt->value = evtGetValue(evt, evt->args[0]);
    return 2;
}

s32 evt_msg_close(EvtEntry* evt) {
    msgWindow_ForceClose(evtGetValue(evt, evt->args[0]));
    return 2;
}

s32 evt_msg_continue(EvtEntry* evt, s32 first) {
    if (first != 0) {
        msgWindow_Continue(evt->msgWindowId);
    }
    if (windowCheckID(evt->msgWindowId) != 0) {
        return 0;
    }
    return 2;
}


s32 _evt_msg_print(void* param_1, int param_2, u32 param_3, char* param_4, s32 param_5, char* param_6) {
    return 0;
}


u8 evt_msg_print_insert(int param_1, int param_2) {
    return 0;
}


s32 evt_msg_print_add_insert(int param_1, int param_2) {
    return 0;
}


s32 evt_msg_select(void* pEvt, int param_2) {
    return 0;
}


s32 evt_msg_print_party_add(int param_1, int param_2) {
    return 0;
}


s32 evt_msg_print_battle_party(int param_1, int param_2) {
    return 0;
}


u8 evt_msg_print_party(void* pEvt, int param_2) {
    return 0;
}


u32 evt_msg_print_add(void* pEvt, int param_2) {
    return 0;
}


s32 evt_msg_togelr(int param_1) {
    return 0;
}


s32 evt_msg_toge(void* pEvt) {
    return 0;
}


s32 evt_msg_print(void* evtEntry, int param_2) {
    return 0;
}


s32 evt_msg_npc(int param_1) {
    return 0;
}


s32 evt_msg_repeat(void* pEvt, int param_2) {
    return 0;
}
