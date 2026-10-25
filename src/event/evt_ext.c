#include "evt/evt_ext.h"
#include "manager/evtmgr_cmd.h"
#include "driver/extdrv.h"

// pose count, texture list, init function, main function, second main function
USER_FUNC(evt_ext_entry) {
    EvtWord* args = event->args;
    s32 count;
    ExtTexInfo* texInfo;
    void (*initFunc)(void);
    void (*mainFunc)(void);
    void (*mainFunc2)(void);

    count = evtGetValue(event, args[0]);
    texInfo = (ExtTexInfo*)evtGetValue(event, args[1]);
    initFunc = (void (*)(void))evtGetValue(event, args[2]);
    mainFunc = (void (*)(void))evtGetValue(event, args[3]);
    mainFunc2 = (void (*)(void))evtGetValue(event, args[4]);
    extEntry(count, texInfo, initFunc, mainFunc, mainFunc2);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_ext_reset) {
    extReset();
    return EVT_RETURN_DONE;
}
