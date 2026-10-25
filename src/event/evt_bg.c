#include "evt/evt_bg.h"
#include "driver/bgdrv.h"
#include "manager/evtmgr_cmd.h"

// BOOL on
USER_FUNC(evt_bg_disp_onoff) {
    if (evtGetValue(event, *event->args)) {
        bgDispOn();
    } else {
        bgDispOff();
    }
    return EVT_RETURN_DONE;
}

// u8 r, u8 g, u8 b, u8 a
USER_FUNC(evt_bg_set_color) {
    EvtWord* args = event->args;
    s32 r, g, b, a;

    r = evtGetValue(event, args[0]);
    g = evtGetValue(event, args[1]);
    b = evtGetValue(event, args[2]);
    a = evtGetValue(event, args[3]);
    bgSetColor((GXColor){ r, g, b, a });
    return EVT_RETURN_DONE;
}

// BOOL off
USER_FUNC(evt_bg_auto_scroll_onoff) {
    if (evtGetValue(event, *event->args)) {
        bgAutoScrollOff();
    } else {
        bgAutoScrollOn();
    }
    return EVT_RETURN_DONE;
}

// f32 x, f32 y
USER_FUNC(evt_bg_set_scrl_offset) {
    EvtWord* args = event->args;
    f32 x, y;

    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    bgSetScrlOffset(x, y);
    return EVT_RETURN_DONE;
}
