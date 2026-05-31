#include "event/evt_lecture.h"

extern void* lect_work;
extern void evtSetValue();
extern s32 evtGetValue();
extern void marioStSystemLevel();
extern void marioStGetSystemLevel();

s32 lect_juyoitem_rel_get(void* event) {
    evtSetValue(event, **(s32**)((s32)event + 0x18), lect_work);
    return 2;
}

s32 lect_juyoitem_rel_set(void* event) {
    lect_work = (void*)evtGetValue(event, **(s32**)((s32)event + 0x18));
    return 2;
}

s32 lect_set_systemlevel(void* event) {
    marioStSystemLevel(evtGetValue(event, **(s32**)((s32)event + 0x18)));
    return 2;
}

s32 lect_test_systemlevel(void) {
    marioStGetSystemLevel();
    return 2;
}
