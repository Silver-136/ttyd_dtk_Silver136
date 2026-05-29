#include "event/evt_hit.h"

s32 evt_hit_bind_update(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void hitBindUpdate(s32 name);
    s32* args = *(s32**)((s32)event + 0x18);

    hitBindUpdate(evtGetValue(event, args[0]));
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_hitobj_name_callback(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void name_callback_sub(s32 name, s32 callback, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 callback = evtGetValue(event, args[1]);

    name_callback_sub(name, callback, 0);
    return 2;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_hit_get_position(void* event, s32 isFirstCall) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern s32 evtGetValue(void* event, s32 value);
    extern void evtSetFloat(void* event, s32 target, f32 value);
    extern void hitObjGetPos(s32 name, Vec* pos);
    Vec pos;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);

    hitObjGetPos(name, &pos);
    evtSetFloat(event, args[1], pos.x);
    evtSetFloat(event, args[2], pos.y);
    evtSetFloat(event, args[3], pos.z);
    return 2;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset