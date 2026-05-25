#include "event/evt_door.h"
#include "event/evt_cmd.h"

s32 DoorFLAG;
s32 door_mapflag;

s32 evtGetValue(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void* mapGetMapObj(void);
void mapErrorEntry(s32 type, s32 value);
void door_dark_flag_sub(s32 unk0, s32 unk1, s32 unk2, s32 unk3);

s32 evt_door_end_wait(void) {
    s32 flag = DoorFLAG;
    return 2 & ~((-flag | flag) >> 31);
}

USER_FUNC(evt_door_load_mapflag) {
    s32 value = evtGetValue(event, event->args[0]);
    s32* mapObj = mapGetMapObj();
    if (mapObj == 0) {
        mapErrorEntry(0, value);
        return 2;
    }
    *mapObj = door_mapflag;
    return 2;
}

USER_FUNC(evt_door_save_mapflag) {
    s32 value = evtGetValue(event, event->args[0]);
    s32* mapObj = mapGetMapObj();
    if (mapObj == 0) {
        mapErrorEntry(0, value);
        return 2;
    }
    door_mapflag = *mapObj;
    return 2;
}

USER_FUNC(door_dark_flag) {
    s32* args = event->args;
    void* info = (void*)evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    door_dark_flag_sub(value, *(s32*)((s32)info + 0x34), *(s32*)((s32)info + 0x38), *(s32*)((s32)info + 0x3C));
    return 2;
}

USER_FUNC(init_inside_group) {
    s32* args = event->args;
    void* info = (void*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], *(s32*)(*(s32*)info + 0x34));
    evtSetValue(event, args[2], *(s32*)(*(s32*)info + 0x38));
    return 2;
}

USER_FUNC(evt_door_set_param) {
    s32* args = event->args;
    void* info = (void*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    s32 value = evtGetValue(event, args[2]);
    if (flag == 0) {
        *(s32*)((s32)info + 0x34) = value;
    }
    return 2;
}
