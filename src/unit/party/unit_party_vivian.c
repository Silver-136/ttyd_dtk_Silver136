#include "unit/party/unit_party_vivian.h"

s32 _disp_heart_entry_stop_check(void* evt) {
    extern void evtSetValue(void* evt, s32 arg, s32 value);

    evtSetValue(evt, **(s32**)((s32)evt + 0x18), *(s32*)((s32)evt + 0x8C));
    return 2;
}

s32 _disp_heart_entry_stop(void* evt) {
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void* evtGetPtrID(s32 id);

    void* ptr = evtGetPtrID(evtGetValue(evt, **(s32**)((s32)evt + 0x18)));

    *(s32*)((s32)ptr + 0x8C) = 1;
    return 2;
}
