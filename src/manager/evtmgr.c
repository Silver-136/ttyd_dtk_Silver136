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
    void* entry;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = work + 0xA0;
    }
    entry = (void*)((s32)*(void**)((s32)set + 0x90) + index * 0x1B0);
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
    switch (_mariostSystemLevel) {
        case 0:
            break;
        case 1:
            evtStopAll(1);
            break;
        case 2:
            evtStopAll(2);
            break;
        case 3:
            evtStopAll(0x10);
            break;
        case 4:
            evtStopAll(0xEF);
            break;
    }
}

void evtmgrReInit(void) {
    void* set = work;
    void* gpPtr = gp;
    s32 field38;
    s32 field3C;
    s32 count;

    if (*(s32*)((s32)gpPtr + 0x14) != 0) {
        set = work + 0xA0;
    }
    field38 = *(s32*)((s32)gpPtr + 0x38);
    field3C = *(s32*)((s32)gpPtr + 0x3C);
    *(s32*)((s32)set + 0x9C) = field3C;
    *(s32*)((s32)set + 0x98) = field38;
    count = *(s32*)set;
    memset(*(void**)((s32)set + 0x90), 0, count * 0x1B0);
    evtMax = 0;
    runMainF = 0;
    evt_msg_init();
}


u8 evtStart(void* pEvt, u32 param_2) {
    return 0;
}


u8 evtStop(int param_1, u32 param_2) {
    return 0;
}


void* evtChildEntry(void* event, void* script, s32 flags) {
    return 0;
}


void* evtBrotherEntry(void* parentEvt, void* evtCode, u32 flags) {
    return 0;
}


void evtEntryType(void* script, s32 a, s32 b, s32 type) {
}


void* evtEntry(void* script, s32 priority, s32 flags) {
    return 0;
}


u8 evtmgrMain(void) {
    return 0;
}


void evtDelete(void* evt) {
}


void* evtRestart(void* pEvt) {
    return 0;
}


u8 make_pri_table(void) {
    return 0;
}


u8 evtmgrInit(void) {
    return 0;
}


void evtStartID(s32 id) {
    void* set;
    void* entry;
    s32 i;

    set = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 0xA0);
    }
    entry = *(void**)((s32)set + 0x90);
    i = 0;
    while (i < *(s32*)set) {
        if ((*(u8*)((s32)entry + 8) & 1) != 0 && *(s32*)((s32)entry + 0x15C) == id) {
            evtStart(entry, 0xEF);
        }
        i++;
        entry = (void*)((s32)entry + 0x1B0);
    }
}


void evtStopID(s32 id) {
    void* set;
    void* entry;
    s32 i;

    set = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 0xA0);
    }
    entry = *(void**)((s32)set + 0x90);
    i = 0;
    while (i < *(s32*)set) {
        if ((*(u8*)((s32)entry + 8) & 1) != 0 && *(s32*)((s32)entry + 0x15C) == id) {
            evtStop((s32)entry, 0xEF);
        }
        i++;
        entry = (void*)((s32)entry + 0x1B0);
    }
}


void evtDeleteID(s32 id) {
    void* set;
    void* entry;
    s32 i;

    set = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 0xA0);
    }
    entry = *(void**)((s32)set + 0x90);
    i = 0;
    while (i < *(s32*)set) {
        if ((*(u8*)((s32)entry + 8) & 1) != 0 && *(s32*)((s32)entry + 0x15C) == id) {
            evtDelete(entry);
        }
        i++;
        entry = (void*)((s32)entry + 0x1B0);
    }
}


void evtStartAll(u32 flags) {
    void* set;
    void* entry;
    s32 i;

    set = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 0xA0);
    }
    entry = *(void**)((s32)set + 0x90);
    i = 0;
    while (i < *(s32*)set) {
        if ((*(u8*)((s32)entry + 8) & 1) != 0) {
            evtStart(entry, flags);
        }
        i++;
        entry = (void*)((s32)entry + 0x1B0);
    }
}


void evtStopAll(s32 type) {
    void* set;
    void* entry;
    s32 i;

    set = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 0xA0);
    }
    entry = *(void**)((s32)set + 0x90);
    i = 0;
    while (i < *(s32*)set) {
        if ((*(u8*)((s32)entry + 8) & 1) != 0) {
            evtStop((s32)entry, type);
        }
        i++;
        entry = (void*)((s32)entry + 0x1B0);
    }
}


void evtStartOther(void* except, u32 flags) {
    void* set;
    void* entry;
    s32 i;

    set = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 0xA0);
    }
    entry = *(void**)((s32)set + 0x90);
    i = 0;
    while (i < *(s32*)set) {
        if ((*(u8*)((s32)entry + 8) & 1) != 0 && entry != except) {
            evtStart(entry, flags);
        }
        i++;
        entry = (void*)((s32)entry + 0x1B0);
    }
}


void evtStopOther(void* except, u32 flags) {
    void* set;
    void* entry;
    s32 i;

    set = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 0xA0);
    }
    entry = *(void**)((s32)set + 0x90);
    i = 0;
    while (i < *(s32*)set) {
        if ((*(u8*)((s32)entry + 8) & 1) != 0 && entry != except) {
            evtStop((s32)entry, flags);
        }
        i++;
        entry = (void*)((s32)entry + 0x1B0);
    }
}
