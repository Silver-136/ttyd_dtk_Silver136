#include "event/evt_twoddisp.h"

void* twodPtr;

s32 twoddisp_main(void) {
    extern void twoddisp_disp(void);
    extern f32 float_1000_80422f88;
    extern void dispEntry(s32, s32, void*, s32, f32);
    s32 i = 0;

    for (; i < 96; i += 8) {
    }
    for (; i < 100; i++) {
    }
    dispEntry(8, 0, twoddisp_disp, 0, float_1000_80422f88);
    return 2;
}


s32 evt_twoddisp_init(int param_1) {
    return 0;
}


void twoddisp_disp(void) {
    ;
}


s32 evt_twoddisp_flag_onoff(int param_1) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 strcmp(const char* s1, const char* s2);
    void* event = (void*)param_1;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 onoff = evtGetValue(event, args[0]);
    char* name = (char*)evtGetValue(event, args[1]);
    s32 i = 0;
    s32 offset = 0;
    void* entry;

    while (i < 100) {
        entry = (void*)((s32)twodPtr + offset);
        if (strcmp((char*)entry, name) == 0) {
            break;
        }
        i++;
        offset += 0x34;
    }
    if (i >= 100) {
        entry = 0;
    }

    if (onoff != 0) {
        *(u16*)((s32)entry + 0x10) |= evtGetValue(event, args[2]);
    } else {
        *(u16*)((s32)entry + 0x10) &= ~evtGetValue(event, args[2]);
    }
    return 2;
}


s32 evt_twoddisp_entry(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 strcmp(const char* s1, const char* s2);
    extern char* strcpy(char* dst, const char* src);
    extern f32 float_0_80422f80;
    extern f32 float_1_80422f84;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name = (char*)evtGetValue(pEvt, args[0]);
    s32 value = evtGetValue(pEvt, args[1]);
    s32 i = 0;
    s32 offset = 0;
    void* entry;

    while (i < 100) {
        entry = (void*)((s32)twodPtr + offset);
        strcmp((char*)entry, name);
        if (*(s8*)entry == 0) {
            strcpy((char*)entry, name);
            *(u16*)((s32)entry + 0x10) = 1;
            *(u16*)((s32)entry + 0x12) = 0;
            *(f32*)((s32)entry + 0x14) = float_0_80422f80;
            *(f32*)((s32)entry + 0x1C) = float_0_80422f80;
            *(f32*)((s32)entry + 0x18) = float_0_80422f80;
            *(f32*)((s32)entry + 0x20) = float_0_80422f80;
            *(s32*)((s32)entry + 0x24) = value;
            *(f32*)((s32)entry + 0x28) = float_1_80422f84;
            break;
        }
        i++;
        offset += 0x34;
    }
    return 2;
}


s32 evt_twoddisp_set_base_position(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern f32 evtGetFloat(void* event, s32 value);
    extern s32 strcmp(const char* s1, const char* s2);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name = (char*)evtGetValue(pEvt, args[0]);
    s32 i = 0;
    s32 offset = 0;
    void* entry;

    while (i < 100) {
        entry = (void*)((s32)twodPtr + offset);
        if (strcmp((char*)entry, name) == 0) {
            break;
        }
        i++;
        offset += 0x34;
    }
    if (i >= 100) {
        entry = 0;
    }
    *(f32*)((s32)entry + 0x1C) = evtGetFloat(pEvt, args[1]);
    *(f32*)((s32)entry + 0x20) = evtGetFloat(pEvt, args[2]);
    return 2;
}


s32 evt_twoddisp_set_position(int param_1) {
    extern s32 evtGetValue(void* event, s32 value);
    extern f32 evtGetFloat(void* event, s32 value);
    extern s32 strcmp(const char* s1, const char* s2);
    void* event = (void*)param_1;
    s32* args = *(s32**)((s32)event + 0x18);
    char* name = (char*)evtGetValue(event, args[0]);
    s32 i = 0;
    s32 offset = 0;
    void* entry;

    while (i < 100) {
        entry = (void*)((s32)twodPtr + offset);
        if (strcmp((char*)entry, name) == 0) {
            break;
        }
        i++;
        offset += 0x34;
    }
    if (i >= 100) {
        entry = 0;
    }
    *(f32*)((s32)entry + 0x14) = evtGetFloat(event, args[1]);
    *(f32*)((s32)entry + 0x18) = evtGetFloat(event, args[2]);
    return 2;
}
