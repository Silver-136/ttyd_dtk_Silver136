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

