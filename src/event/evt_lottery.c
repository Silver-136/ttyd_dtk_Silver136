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



/* CHATGPT STUB FILL: main/event/evt_lottery 20260624_184929 */

/* stub-fill: evt_lottery_error_check | missing_definition | ghidra_signature */
s32 evt_lottery_error_check(int param_1) {
    return 0;
}

/* stub-fill: evt_lottery_buy | missing_definition | ghidra_signature */
s32 evt_lottery_buy(void* pEvt) {
    return 0;
}

/* stub-fill: evt_lottery | missing_definition | ghidra_signature */
s32 evt_lottery(void* pEvt) {
    return 0;
}
