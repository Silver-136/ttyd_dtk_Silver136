#include "event/evt_map.h"
#include "event/evt_cmd.h"

s32 evtGetValue(EventEntry* event, s32 value);
f32 evtGetFloat(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void N_mapDispOn(void);
void N_mapDispOff(void);
void mapBlendOff(void);
void mapBlendOff2(void);
void mapFogOn(void);
void mapFogOff(void);
void mapSetPlayRate(s32 id, f32 rate);
void mapSetTevCallback(s32 id, s32 callback);
void mapObjClearOffScreen(s32 id);
void mapGrpClearOffScreen(s32 id);
void mapCheckAnimation(s32 id, s32* outDone, f32* outFrame);

USER_FUNC(N_evt_mapdisp_onoff) {
    if (evtGetValue(event, event->args[0]) != 0) {
        N_mapDispOn();
    } else {
        N_mapDispOff();
    }
    return 2;
}

USER_FUNC(evt_map_blend_off) {
    if (evtGetValue(event, event->args[0]) == 0) {
        mapBlendOff();
    } else {
        mapBlendOff2();
    }
    return 2;
}

USER_FUNC(evt_map_fog_onoff) {
    if (evtGetValue(event, event->args[0]) != 0) {
        mapFogOn();
    } else {
        mapFogOff();
    }
    return 2;
}

USER_FUNC(evt_map_set_playrate) {
    s32* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    f32 rate = evtGetFloat(event, args[1]);
    mapSetPlayRate(id, rate);
    return 2;
}

USER_FUNC(evt_map_set_tevcallback) {
    s32* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 callback = evtGetValue(event, args[1]);
    mapSetTevCallback(id, callback);
    return 2;
}

USER_FUNC(evt_mapobj_clear_offscreen) {
    s32* args = event->args;
    s32 group = evtGetValue(event, args[0]);
    s32 id = evtGetValue(event, args[1]);
    if (group == 0) {
        mapObjClearOffScreen(id);
    } else {
        mapGrpClearOffScreen(id);
    }
    return 2;
}

USER_FUNC(evt_map_checkanim) {
    s32* args = event->args;
    s32 done;
    f32 frame;
    s32 id = evtGetValue(event, args[0]);
    mapCheckAnimation(id, &done, &frame);
    evtSetValue(event, args[1], done);
    evtSetValue(event, args[2], (s32)frame);
    return 2;
}
