#include "evt/evt_env.h"
#include "manager/evtmgr_cmd.h"
#include "driver/envdrv.h"
#include <dolphin/os.h>

// s32 type, s32 msec (0 = forever)
USER_FUNC(evt_env_blur_on) {
    EvtWord* args = event->args;
    s32 type, msec;
    s64 now;

    type = evtGetValue(event, args[0]);
    msec = evtGetValue(event, args[1]);
    now = event->lifetime;
    if (isFirstCall) {
        envBlurOn(type, msec);
        event->userdata[0] = ((u64)now >> 32) & 0xFFFFFFFF;
        event->userdata[1] = now & 0xFFFFFFFF;
    }
    if (msec && OSTicksToMilliseconds(now - *(u64*)event->userdata) >= msec) {
        envBlurOff();
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}
