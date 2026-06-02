#include "event/evt_cloud.h"

s32 evt_cloud_get_mode(void* event, s32 isFirstCall) {
    extern void evtSetValue(void* event, s32 target, s32 value);
    extern void* cloud_p;
    s32* args = *(s32**)((s32)event + 0x18);
    void* cloud = cloud_p;

    if (cloud == 0) {
        evtSetValue(event, args[0], -1);
        return 2;
    }
    evtSetValue(event, args[0], *(s32*)((s32)cloud + 4));
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_cloud_set_cap_size(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* cloud_p;
    s32* args = *(s32**)((s32)event + 0x18);
    u16 width = evtGetValue(event, args[0]);
    s32 height = evtGetValue(event, args[1]);

    *(u16*)((s32)cloud_p + 0xC0) = width;
    *(u16*)((s32)cloud_p + 0xC2) = height;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_cloud_main(void* pEvt, int param_2) {
    return 0;
}


s32 evt_cloud_ent(void* pEvt) {
    return 0;
}


s32 evt_cloud_init(void) {
    return 0;
}
