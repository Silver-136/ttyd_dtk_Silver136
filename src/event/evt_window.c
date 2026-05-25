#include "event/evt_window.h"
#include "event/evt_cmd.h"

extern void* mapalloc_base_ptr;

void nameEntOff(void);
s32 nameEntWait(void);
s32 nameEntPrepare(void);
void nameEntOn(s32 value);
void winOpenDisable(void);
void winOpenEnable(void);
void _mapFree(void* heap, void* ptr);

USER_FUNC(mainwindow_opendisable) {
    winOpenDisable();
    return 2;
}

USER_FUNC(mainwindow_openenable) {
    winOpenEnable();
    return 2;
}

USER_FUNC(evt_win_nameent_off) {
    nameEntOff();
    return 2;
}

USER_FUNC(evt_win_nameent_wait) {
    if (nameEntWait() != 0) {
        return 0;
    }
    return 2;
}

USER_FUNC(evt_win_nameent_on) {
    s32 value = evtGetValue(event, event->args[0]);

    if (nameEntPrepare() == 0) {
        return 0;
    }
    nameEntOn(value);
    return 2;
}

USER_FUNC(evt_win_item_desttable) {
    void* ptr = (void*)evtGetValue(event, event->args[0]);

    _mapFree(mapalloc_base_ptr, ptr);
    return 2;
}
