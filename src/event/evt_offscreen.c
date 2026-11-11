// Script commands for offscreen render targets (offscreendrv).

#include "evt/evt_offscreen.h"
#include "driver/offscreendrv.h"
#include "manager/evtmgr_cmd.h"

/** const char* name */
USER_FUNC(evt_offscreen_entry) {
    offscreenEntry((const char*)evtGetValue(event, *event->args));
    return EVT_RETURN_DONE;
}

/** const char* name */
USER_FUNC(evt_offscreen_delete) {
    offscreenDelete((const char*)evtGetValue(event, *event->args));
    return EVT_RETURN_DONE;
}

/** const char* name, s32 x (out), s32 y (out), s32 width (out), s32 height (out) */
USER_FUNC(evt_offscreen_get_boundingbox) {
    EvtWord* args = event->args;
    const char* name;
    s32 x, y, width, height;
    u16 left, top, right, bottom;

    name = (const char*)evtGetValue(event, args[0]);
    x = args[1];
    y = args[2];
    width = args[3];
    height = args[4];
    if (!offscreenGetBoundingBox_BeforeFrame(offscreenNameToId(name), &left, &top, &right, &bottom)) {
        return EVT_RETURN_BLOCK;
    }
    evtSetValue(event, x, left);
    evtSetValue(event, y, top);
    evtSetValue(event, width, right - left);
    evtSetValue(event, height, bottom - top);
    return EVT_RETURN_DONE;
}

/** same as above, but gives up waiting for a valid box after 5 frames */
USER_FUNC(evt_offscreen_get_boundingbox2) {
    EvtWord* args = event->args;
    const char* name;
    s32 x, y, width, height, id;
    BOOL valid;
    u16 left, top, right, bottom;

    name = (const char*)evtGetValue(event, args[0]);
    x = args[1];
    y = args[2];
    width = args[3];
    height = args[4];
    id = offscreenNameToId(name);
    bottom = right = top = left = 0;
    if (isFirstCall) {
        event->userdata[0] = 5;
    }
    valid = offscreenGetBoundingBox_BeforeFrame(id, &left, &top, &right, &bottom);
    event->userdata[0]--;
    if (!valid && event->userdata[0] != 0) {
        return EVT_RETURN_BLOCK;
    }
    evtSetValue(event, x, left);
    evtSetValue(event, y, top);
    evtSetValue(event, width, right - left);
    evtSetValue(event, height, bottom - top);
    return EVT_RETURN_DONE;
}

/** const char* name, s32 format, s32 param */
USER_FUNC(evt_offscreen_set_texture_fmt) {
    EvtWord* args = event->args;
    const char* name;
    s32 format, param;
    OffscreenEntry* entry;

    name = (const char*)evtGetValue(event, args[0]);
    format = evtGetValue(event, args[1]);
    param = evtGetValue(event, args[2]);
    entry = offscreenNameToPtr(name);
    entry->copyFmt = format;
    entry->texFmt = param;
    return EVT_RETURN_DONE;
}
