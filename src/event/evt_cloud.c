#include "event/evt_cloud.h"

extern u8 cloud[];
extern void* cloud_p;
extern u8 cloud_once_flag;
extern char str_i_name_PCT1d_802f73d8[];
extern char str_o_name_PCT1d_802f73e4[];
s32 sprintf(char* str, const char* fmt, ...);

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
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 evtEntry(void* script, s32 priority, s32 flags);
    extern void* evt_cloud_main_evt;
    extern char str_P_oof_cloud_1_802f73c8[];
    extern char* strncpy(char* dest, const char* src, unsigned long n);
    s32* args;
    s32 indirectName;
    s32 name;
    s32 outName;
    s32 mode;
    s32 i;
    u8* entry;

    args = *(s32**)((s32)pEvt + 0x18);
    indirectName = evtGetValue(pEvt, args[0]);
    name = evtGetValue(pEvt, args[1]);
    outName = evtGetValue(pEvt, args[2]);
    mode = evtGetValue(pEvt, args[3]);

    entry = cloud;
    for (i = 0; i < 0x10; i++, entry += 0xC8) {
        if ((*(u16*)entry & 1) == 0) {
            break;
        }
    }
    if (i >= 0x10) {
        return 2;
    }

    *(u16*)entry |= 1;
    strncpy((char*)entry + 8, str_P_oof_cloud_1_802f73c8, 0x20);
    if (indirectName == 0) {
        strncpy((char*)entry + 0x68, (char*)name, 0x20);
    } else {
        strncpy((char*)entry + 0x68, *(char**)name, 0x20);
    }
    strncpy((char*)entry + 0x88, (char*)outName, 0x20);
    *(s32*)(entry + 0xB4) = mode;
    *(s32*)(entry + 0xA8) = 0xB4;
    *(s32*)(entry + 0xAC) = 0;
    *(s32*)(entry + 4) = 0;
    *(s32*)(entry + 0xB8) = 0;
    *(u16*)(entry + 0xC0) = 0;
    *(u16*)(entry + 0xC2) = 0;
    *(s32*)(entry + 0xB0) = evtEntry(evt_cloud_main_evt, 0, 0);
    *(s32*)(*(s32*)(entry + 0xB0) + 0x9C) = (s32)entry;
    cloud_p = entry;
    if (indirectName != 0) {
        *(s32*)(entry + 0xBC) = name;
        *(u16*)entry |= 2;
    }
    return 2;
}

s32 evt_cloud_init(void) {
    char* iName;
    s32 zero;
    char* oName;
    u8* entry;
    s32 i;

    entry = cloud;
    iName = str_i_name_PCT1d_802f73d8;
    oName = str_o_name_PCT1d_802f73e4;
    i = 0;
    zero = 0;
    while (i < 0x10) {
        *(u16*)entry = zero;
        sprintf((char*)entry + 0x28, iName, i);
        sprintf((char*)entry + 0x48, oName, i);
        i++;
        entry += 0xC8;
    }
    cloud_p = NULL;
    cloud_once_flag = 0;
    return 2;
}
