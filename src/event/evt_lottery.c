#include "event/evt_lottery.h"
#include "event/evt_cmd.h"

extern void* gp;
s32 evtGetValue(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);

void* lotteryGetPtr(void) {
    return (void*)((s32)gp + 0xA8);
}

USER_FUNC(evt_lottery_todaynum) {
    evtSetValue(event, event->args[0], *(s16*)((s32)gp + 0xB8));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_lottery_flag) {
    evtSetValue(event, event->args[0], *(u16*)((s32)gp + 0xA8));
    return EVT_RETURN_DONE;
}

