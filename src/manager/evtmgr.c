#include "manager/evtmgr.h"

extern void* gp;
extern f32 evtSpd;
extern s32 evtMax;
extern s32 runMainF;
extern s32 _mariostSystemLevel;
extern u8 work[];
void* memset(void* dest, int ch, size_t count);
void evtStopAll(s32 type);
void evt_msg_init(void);

void evtSetType(void* entry, s32 type) {
    *(u8*)((s32)entry + 0xC) = type;
}

void evtSetSpeed(void* entry, f32 speed) {
    *(f32*)((s32)entry + 0x164) = speed * evtSpd;
}

void evtSetPri(void* entry, s32 pri) {
    *(u8*)((s32)entry + 0xB) = pri;
}

void* evtGetWork(void) {
    if (*(s32*)((s32)gp + 0x14) != 0) {
        return work + 0xA0;
    }
    return work;
}

void* evtGetPtr(s32 index) {
    void* set = work;
    void* entry = (void*)((s32)*(void**)((s32)set + 0x90) + index * 0x1B0);
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = work + 0xA0;
        entry = (void*)((s32)*(void**)((s32)set + 0x90) + index * 0x1B0);
    }
    if ((*(u8*)((s32)entry + 8) & 1) != 0) {
        return entry;
    }
    return 0;
}

BOOL evtCheckID(s32 eventId) {
    void* set = work;
    s32 i;
    EventEntry* entry;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = work + 0xA0;
    }
    i = *(s32*)set;
    entry = *(EventEntry**)((s32)set + 0x90);
    for (; i > 0; i--) {
        if ((*(u8*)((s32)entry + 8) & 1) != 0 && *(s32*)((s32)entry + 0x15C) == eventId) {
            return TRUE;
        }
        entry = (EventEntry*)((s32)entry + 0x1B0);
    }
    return FALSE;
}

void* evtGetPtrID(s32 eventId) {
    void* set = work;
    s32 i;
    EventEntry* entry;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = work + 0xA0;
    }
    i = *(s32*)set;
    entry = *(EventEntry**)((s32)set + 0x90);
    for (; i > 0; i--) {
        if ((*(u8*)((s32)entry + 8) & 1) != 0 && *(s32*)((s32)entry + 0x15C) == eventId) {
            return entry;
        }
        entry = (EventEntry*)((s32)entry + 0x1B0);
    }
    return 0;
}

void evtEntryRunCheck(void) {
    if (_mariostSystemLevel == 2) {
        evtStopAll(2);
    } else if (_mariostSystemLevel < 2) {
        if (_mariostSystemLevel == 1) {
            evtStopAll(1);
        }
    } else if (_mariostSystemLevel < 4) {
        evtStopAll(0x10);
    } else if (_mariostSystemLevel == 4) {
        evtStopAll(0xEF);
    }
}

void evtmgrReInit(void) {
    void* set = work;
    s32 count;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = work + 0xA0;
    }
    *(s32*)((s32)set + 0x98) = *(s32*)((s32)gp + 0x38);
    *(s32*)((s32)set + 0x9C) = *(s32*)((s32)gp + 0x3C);
    count = *(s32*)set;
    memset(*(void**)((s32)set + 0x90), 0, count * 0x1B0);
    evtMax = 0;
    runMainF = 0;
    evt_msg_init();
}
