#include "event/evt_npc.h"

void _npc_check_coin(void* npc, s32* coin) {
    *coin += *(u8*)((s32)npc + 0x315);
}

USER_FUNC(evt_npc_check_escape_battle) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void marioSetMutekiTime(s32 time);
    if (evtGetValue(event, event->args[0]) == 4) {
        marioSetMutekiTime(3000);
    }
    return 2;
}

USER_FUNC(evt_npc_start_for_event) {
    extern void npcStartForEvent(void);
    npcStartForEvent();
    return 2;
}

USER_FUNC(evt_npc_stop_for_event) {
    extern void npcStopForEvent(void);
    npcStopForEvent();
    return 2;
}

USER_FUNC(evt_npc_change_fbat_mode) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void fbatChangeMode(s16 mode);
    fbatChangeMode((s16)evtGetValue(event, event->args[0]));
    return 2;
}
