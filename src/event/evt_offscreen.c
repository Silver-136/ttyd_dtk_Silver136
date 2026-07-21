#include "event/evt_offscreen.h"

extern s32 evtGetValue();
extern void offscreenDelete(s32);
extern void offscreenEntry(s32);
extern s32* offscreenNameToPtr(s32);


s32 evt_offscreen_entry(int param_1) {
    offscreenEntry(evtGetValue(param_1, **(s32**)(param_1 + 0x18)));
    return 2;
}

s32 evt_offscreen_delete(int param_1) {
    offscreenDelete(evtGetValue(param_1, **(s32**)(param_1 + 0x18)));
    return 2;
}

s32 evt_offscreen_get_boundingbox(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 offscreenNameToId(s32 name);
    extern s32 offscreenGetBoundingBox_BeforeFrame(s32 id, void* x0, void* y0, void* x1, void* y1);
    extern void evtSetValue(void* event, s32 var, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 outX = args[1];
    s32 outY = args[2];
    s32 outW = args[3];
    s32 outH = args[4];
    s32 id = offscreenNameToId(name);
    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;

    if (offscreenGetBoundingBox_BeforeFrame(id, &x0, &y0, &x1, &y1) == 0) {
        return 0;
    }
    evtSetValue(event, outX, x0);
    evtSetValue(event, outY, y0);
    evtSetValue(event, outW, x1 - x0);
    evtSetValue(event, outH, y1 - y0);
    return 2;
}


s32 evt_offscreen_get_boundingbox2(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 offscreenNameToId(s32 name);
    extern s32 offscreenGetBoundingBox_BeforeFrame(s32 id, void* x0, void* y0, void* x1, void* y1);
    extern void evtSetValue(void* event, s32 var, s32 value);
    void* evt = event;
    s32 first = isFirstCall;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    s32 outX = args[1];
    s32 outY = args[2];
    s32 outW = args[3];
    s32 outH = args[4];
    s32 id = offscreenNameToId(name);
    u16 x0 = 0;
    u16 y0 = 0;
    u16 x1 = 0;
    u16 y1 = 0;
    s32 result;
    s32 count;

    if (first != 0) {
        *(s32*)((s32)evt + 0x78) = 5;
    }
    result = offscreenGetBoundingBox_BeforeFrame(id, &x0, &y0, &x1, &y1);
    count = *(s32*)((s32)evt + 0x78);
    count--;
    *(s32*)((s32)evt + 0x78) = count;
    if (result == 0 && *(s32*)((s32)evt + 0x78) != 0) {
        return 0;
    }
    evtSetValue(evt, outX, x0);
    evtSetValue(evt, outY, y0);
    evtSetValue(evt, outW, x1 - x0);
    evtSetValue(evt, outH, y1 - y0);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_offscreen_set_texture_fmt(int param_1) {
    void* args = *(void**)(param_1 + 0x18);
    s32 name = evtGetValue(param_1, *(s32*)args);
    s32 format = evtGetValue(param_1, *(s32*)((s32)args + 4));
    s32 type = evtGetValue(param_1, *(s32*)((s32)args + 8));
    s32* offscreen = offscreenNameToPtr(name);

    offscreen[0xF] = format;
    offscreen[0x10] = type;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
