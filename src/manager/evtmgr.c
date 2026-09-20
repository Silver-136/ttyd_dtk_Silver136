#include "manager/evtmgr.h"

extern void* gp;
extern s32 _mariostSystemLevel;
void evtStopAll(s32 type);
void evt_msg_init(void);

u8 work[0x140];
s32 priTbl[0x100];
s32 priIDTbl[0x100];

s32 evtID = 1;
f32 evtSpd = 1.0f;

s32 evtMax;
s32 priTblNum;
s32 runMainF;

void* evtGetWork(void) {
    if (*(s32*)((s32)gp + 0x14) != 0) {
        return work + 0xA0;
    }
    return work;
}

#pragma opt_common_subs off
void make_pri_table(void) {
    void* set;
    void* entry;
    s32* slotPtr;
    s32* idPtr;
    s32* slotWrite;
    s32* idWrite;
    s32* slotJPtr;
    s32* idJPtr;
    s32 idI;
    s32 idJ;
    s32 slotI;
    s32 slotJ;
    s32 count;
    s32 n;
    s32 i;
    s32 j;

    set = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = work + 0xA0;
    }
    entry = *(void**)((s32)set + 0x90);
    slotPtr = priTbl;
    idPtr = priIDTbl;
    slotWrite = slotPtr;
    idWrite = idPtr;
    count = 0;
    for (n = 0; n < *(s32*)set; n++, entry = (void*)((s32)entry + 0x1B0)) {
        if ((*(u8*)((s32)entry + 8) & 1) != 0) {
            *slotWrite++ = n;
            *idWrite++ = *(s32*)((s32)entry + 0x15C);
            count++;
        }
    }
    priTblNum = count;

    for (i = 0; i < count - 1; i++, slotPtr++, idPtr++) {
        j = i + 1;
        slotJPtr = &priTbl[j];
        idJPtr = &priIDTbl[j];
        for (; j < count; j++, slotJPtr++, idJPtr++) {
            slotI = *slotPtr;
            slotJ = *slotJPtr;
            if (*(u8*)((s32)*(void**)((s32)set + 0x90) + slotI * 0x1B0 + 0xB) <
                *(u8*)((s32)*(void**)((s32)set + 0x90) + slotJ * 0x1B0 + 0xB)) {
                idI = *idPtr;
                idJ = *idJPtr;
                *slotPtr = slotJ;
                *idPtr = idJ;
                *slotJPtr = slotI;
                *idJPtr = idI;
            }
        }
    }
}
#pragma opt_common_subs reset

void evtmgrInit(void) {
    extern void* __memAlloc(s32, u32);
    extern void* memset(void*, int, u32);
    u8* wp;

    wp = work;
    *(s32*)wp = 0x100;
    *(void**)(wp + 0x90) = __memAlloc(0, *(s32*)wp * 0x1B0);
    *(s32*)(wp + 0x9C) = *(s32*)((u8*)gp + 0x3C);
    *(s32*)(wp + 0x98) = *(s32*)((u8*)gp + 0x38);
    memset(*(void**)(wp + 0x90), 0, *(s32*)wp * 0x1B0);
    memset(wp + 4, 0, 0x80);
    memset(wp + 0x84, 0, 0xC);

    wp = work + 0xA0;
    *(s32*)wp = 0x100;
    *(void**)(wp + 0x90) = __memAlloc(0, *(s32*)wp * 0x1B0);
    *(s32*)(wp + 0x9C) = *(s32*)((u8*)gp + 0x3C);
    *(s32*)(wp + 0x98) = *(s32*)((u8*)gp + 0x38);
    memset(*(void**)(wp + 0x90), 0, *(s32*)wp * 0x1B0);
    memset(wp + 4, 0, 0x80);
    memset(wp + 0x84, 0, 0xC);

    evtMax = 0;
    priTblNum = 0;
    runMainF = 0;
    evt_msg_init();
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

void* evtEntry(void* script, s32 pri, s32 flags) {
    void* set;
    void* evt;
    u32* cmd;
    u32 word;
    s32 params;
    s32 opcode;
    s32 label;
    s32 count;
    s32 index;
    s32 i;
    s32 n;
    s32 zero;
    u8* tableEntry;
    s8* labelIds;

    set = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = work + 0xA0;
    }
    count = *(s32*)set;
    index = 0;
    evt = *(void**)((s32)set + 0x90);
    while (count > 0) {
        if ((*(u8*)((s32)evt + 8) & 1) == 0) {
            break;
        }
        index++;
        evt = (void*)((s32)evt + 0x1B0);
        count--;
    }

    evtMax++;
    memset(evt, 0, 0x1B0);
    *(u8*)((s32)evt + 8) = flags | 1;
    *(void**)((s32)evt + 0x14) = script;
    *(void**)((s32)evt + 0x1A0) = script;
    *(void**)((s32)evt + 0x1A8) = script;
    *(u8*)((s32)evt + 0xA) = 0;
    *(void**)((s32)evt + 0x6C) = 0;
    *(void**)((s32)evt + 0x70) = 0;
    *(void**)((s32)evt + 0x74) = 0;
    *(u8*)((s32)evt + 0xB) = pri;
    *(s32*)((s32)evt + 0x15C) = evtID++;
    *(s32*)((s32)evt + 0x160) = -1;
    *(u8*)((s32)evt + 0xE) = -1;
    *(u8*)((s32)evt + 0xF) = -1;
    *(u8*)((s32)evt + 0xC) = 0xEF;
    *(void**)((s32)evt + 0x1A4) = 0;
    *(f32*)((s32)evt + 0x164) = evtSpd;
    *(f32*)((s32)evt + 0x168) = 0.0f;
    *(s32*)((s32)evt + 0x16C) = -1;
    *(void**)((s32)evt + 0x170) = 0;
    *(u64*)evt = 0;

    *(s32*)((s32)evt + 0x9C) = 0;
    *(s32*)((s32)evt + 0xA0) = 0;
    *(s32*)((s32)evt + 0xA4) = 0;
    *(s32*)((s32)evt + 0xA8) = 0;
    *(s32*)((s32)evt + 0xAC) = 0;
    *(s32*)((s32)evt + 0xB0) = 0;
    *(s32*)((s32)evt + 0xB4) = 0;
    *(s32*)((s32)evt + 0xB8) = 0;
    *(s32*)((s32)evt + 0xBC) = 0;
    *(s32*)((s32)evt + 0xC0) = 0;
    *(s32*)((s32)evt + 0xC4) = 0;
    *(s32*)((s32)evt + 0xC8) = 0;
    *(s32*)((s32)evt + 0xCC) = 0;
    *(s32*)((s32)evt + 0xD0) = 0;
    *(s32*)((s32)evt + 0xD4) = 0;
    *(s32*)((s32)evt + 0xD8) = 0;
    *(s32*)((s32)evt + 0xDC) = 0;
    *(s32*)((s32)evt + 0xE0) = 0;
    *(s32*)((s32)evt + 0xE4) = 0;

    i = 0;
    zero = i;
    tableEntry = (u8*)evt;
    for (n = 0; n < 2; n++) {
        labelIds = (s8*)evt + i + 0x1C;
        labelIds[0] = -1; *(u32**)(tableEntry + 0x2C) = (u32*)zero;
        labelIds[1] = -1; *(u32**)(tableEntry + 0x30) = (u32*)zero;
        labelIds[2] = -1; *(u32**)(tableEntry + 0x34) = (u32*)zero;
        labelIds[3] = -1; *(u32**)(tableEntry + 0x38) = (u32*)zero;
        labelIds[4] = -1; *(u32**)(tableEntry + 0x3C) = (u32*)zero;
        labelIds[5] = -1; *(u32**)(tableEntry + 0x40) = (u32*)zero;
        labelIds[6] = -1; *(u32**)(tableEntry + 0x44) = (u32*)zero;
        labelIds[7] = -1; *(u32**)(tableEntry + 0x48) = (u32*)zero;
        i += 8;
        tableEntry += 0x20;
    }

    n = 0;
    cmd = script;
    while (1) {
        word = *cmd;
        opcode = word & 0xFFFF;
        params = (s32)word >> 16;
        cmd++;
        label = *cmd;
        cmd += params;
        switch (opcode) {
            case 1:
                goto finished;
            case 3:
                *(s8*)((s32)evt + 0x1C + n) = (s8)label;
                *(u32**)((s32)evt + 0x2C + n * 4) = cmd;
                n++;
        }
        if (n >= 16) {
            "unused";
        }
    }

finished:
    if (runMainF != 0 && (*(u8*)((s32)evt + 8) & 0x20) != 0) {
        priTbl[priTblNum] = index;
        priIDTbl[priTblNum] = *(s32*)((s32)evt + 0x15C);
        priTblNum++;
    }
    ((void (*)(void*))evtEntryRunCheck)(evt);
    if (evtID == 0) {
        evtID = 1;
    }
    return evt;
}

void* evtEntryType(void* script, s32 pri, s32 flags, s32 type) {
    void* set;
    void* evt;
    u32* cmd;
    u32 word;
    s32 params;
    s32 opcode;
    s32 label;
    s32 count;
    s32 index;
    s32 i;
    s32 n;
    s32 zero;
    u8* tableEntry;
    s8* labelIds;

    set = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = work + 0xA0;
    }
    count = *(s32*)set;
    index = 0;
    evt = *(void**)((s32)set + 0x90);
    while (count > 0) {
        if ((*(u8*)((s32)evt + 8) & 1) == 0) {
            break;
        }
        index++;
        evt = (void*)((s32)evt + 0x1B0);
        count--;
    }

    evtMax++;
    memset(evt, 0, 0x1B0);
    *(u8*)((s32)evt + 8) = flags | 1;
    *(void**)((s32)evt + 0x14) = script;
    *(void**)((s32)evt + 0x1A0) = script;
    *(void**)((s32)evt + 0x1A8) = script;
    *(u8*)((s32)evt + 0xA) = 0;
    *(void**)((s32)evt + 0x6C) = 0;
    *(void**)((s32)evt + 0x70) = 0;
    *(void**)((s32)evt + 0x74) = 0;
    *(u8*)((s32)evt + 0xB) = pri;
    *(s32*)((s32)evt + 0x15C) = evtID++;
    *(s32*)((s32)evt + 0x160) = -1;
    *(u8*)((s32)evt + 0xE) = -1;
    *(u8*)((s32)evt + 0xF) = -1;
    *(u8*)((s32)evt + 0xC) = type;
    *(void**)((s32)evt + 0x1A4) = 0;
    *(f32*)((s32)evt + 0x164) = evtSpd;
    *(f32*)((s32)evt + 0x168) = 0.0f;
    *(s32*)((s32)evt + 0x16C) = -1;
    *(void**)((s32)evt + 0x170) = 0;
    *(s32*)evt = 0;
    *(s32*)((s32)evt + 4) = 0;

    *(s32*)((s32)evt + 0x9C) = 0;
    *(s32*)((s32)evt + 0xA0) = 0;
    *(s32*)((s32)evt + 0xA4) = 0;
    *(s32*)((s32)evt + 0xA8) = 0;
    *(s32*)((s32)evt + 0xAC) = 0;
    *(s32*)((s32)evt + 0xB0) = 0;
    *(s32*)((s32)evt + 0xB4) = 0;
    *(s32*)((s32)evt + 0xB8) = 0;
    *(s32*)((s32)evt + 0xBC) = 0;
    *(s32*)((s32)evt + 0xC0) = 0;
    *(s32*)((s32)evt + 0xC4) = 0;
    *(s32*)((s32)evt + 0xC8) = 0;
    *(s32*)((s32)evt + 0xCC) = 0;
    *(s32*)((s32)evt + 0xD0) = 0;
    *(s32*)((s32)evt + 0xD4) = 0;
    *(s32*)((s32)evt + 0xD8) = 0;
    *(s32*)((s32)evt + 0xDC) = 0;
    *(s32*)((s32)evt + 0xE0) = 0;
    *(s32*)((s32)evt + 0xE4) = 0;

    i = 0;
    zero = i;
    tableEntry = (u8*)evt;
    for (n = 0; n < 2; n++) {
        labelIds = (s8*)evt + i + 0x1C;
        labelIds[0] = -1; *(u32**)(tableEntry + 0x2C) = (u32*)zero;
        labelIds[1] = -1; *(u32**)(tableEntry + 0x30) = (u32*)zero;
        labelIds[2] = -1; *(u32**)(tableEntry + 0x34) = (u32*)zero;
        labelIds[3] = -1; *(u32**)(tableEntry + 0x38) = (u32*)zero;
        labelIds[4] = -1; *(u32**)(tableEntry + 0x3C) = (u32*)zero;
        labelIds[5] = -1; *(u32**)(tableEntry + 0x40) = (u32*)zero;
        labelIds[6] = -1; *(u32**)(tableEntry + 0x44) = (u32*)zero;
        labelIds[7] = -1; *(u32**)(tableEntry + 0x48) = (u32*)zero;
        i += 8;
        tableEntry += 0x20;
    }

    n = 0;
    cmd = script;
    while (1) {
        word = *cmd;
        opcode = word & 0xFFFF;
        params = (s32)word >> 16;
        cmd++;
        label = *cmd;
        cmd += params;
        switch (opcode) {
            case 1:
                goto finished;
            case 3:
                *(s8*)((s32)evt + 0x1C + n) = (s8)label;
                *(u32**)((s32)evt + 0x2C + n * 4) = cmd;
                n++;
        }
        if (n >= 16) {
            "unused";
        }
    }

finished:
    if (runMainF != 0 && (*(u8*)((s32)evt + 8) & 0x20) != 0) {
        priTbl[priTblNum] = index;
        priIDTbl[priTblNum] = *(s32*)((s32)evt + 0x15C);
        priTblNum++;
    }
    ((void (*)(void*))evtEntryRunCheck)(evt);
    if (evtID == 0) {
        evtID = 1;
    }
    return evt;
}

void* evtChildEntry(void* parentEvt, void* evtCode, s32 flags) {
    void* set;
    void* evt;
    u32* cmd;
    u32 word;
    s32 params;
    s32 opcode;
    s32 label;
    s32 count;
    s32 index;
    s32 i;
    s32 n;
    s32 zero;
    u8* tableEntry;
    s8* labelIds;

    set = work;
    if (*(s32*)((s32)gp + 0x14) != 0) set = work + 0xA0;
    count = *(s32*)set;
    index = 0;
    evt = *(void**)((s32)set + 0x90);
    while (count > 0) {
        if ((*(u8*)((s32)evt + 8) & 1) == 0) break;
        index++;
        evt = (void*)((s32)evt + 0x1B0);
        count--;
    }

    evtMax++;
    *(void**)((s32)parentEvt + 0x70) = evt;
    *(u8*)((s32)parentEvt + 8) |= 0x10;
    memset(evt, 0, 0x1B0);
    *(u8*)((s32)evt + 8) = flags | 1;
    *(void**)((s32)evt + 0x14) = evtCode;
    *(void**)((s32)evt + 0x1A0) = evtCode;
    *(void**)((s32)evt + 0x1A8) = evtCode;
    *(u8*)((s32)evt + 0xA) = 0;
    *(void**)((s32)evt + 0x6C) = parentEvt;
    *(void**)((s32)evt + 0x70) = 0;
    *(void**)((s32)evt + 0x74) = 0;
    *(u8*)((s32)evt + 0xB) = *(u8*)((s32)parentEvt + 0xB) + 1;
    *(s32*)((s32)evt + 0x15C) = evtID++;
    *(void**)((s32)evt + 0x160) = *(void**)((s32)parentEvt + 0x160);
    *(u8*)((s32)evt + 0xE) = -1;
    *(u8*)((s32)evt + 0xF) = -1;
    *(u8*)((s32)evt + 0xC) = *(u8*)((s32)parentEvt + 0xC);
    *(void**)((s32)evt + 0x1A4) = 0;
    *(void**)((s32)evt + 0x154) = *(void**)((s32)parentEvt + 0x154);
    *(void**)((s32)evt + 0x158) = *(void**)((s32)parentEvt + 0x158);
    *(f32*)((s32)evt + 0x164) = evtSpd;
    *(f32*)((s32)evt + 0x168) = 0.0f;
    *(s32*)((s32)evt + 0x16C) = -1;
    *(void**)((s32)evt + 0x170) = *(void**)((s32)parentEvt + 0x170);
    *(s32*)evt = 0;
    *(s32*)((s32)evt + 4) = 0;
    *(s32*)((s32)evt + 0x178) = *(s32*)((s32)parentEvt + 0x178);
    *(s32*)((s32)evt + 0x17C) = *(s32*)((s32)parentEvt + 0x17C);
    *(s32*)((s32)evt + 0x180) = *(s32*)((s32)parentEvt + 0x180);
    *(s32*)((s32)evt + 0x184) = *(s32*)((s32)parentEvt + 0x184);
    *(s32*)((s32)evt + 0x188) = *(s32*)((s32)parentEvt + 0x188);
    *(s32*)((s32)evt + 0x18C) = *(s32*)((s32)parentEvt + 0x18C);
    *(s32*)((s32)evt + 0x190) = *(s32*)((s32)parentEvt + 0x190);

    *(s32*)((s32)evt + 0x9C) = *(s32*)((s32)parentEvt + 0x9C);
    *(s32*)((s32)evt + 0xA0) = *(s32*)((s32)parentEvt + 0xA0);
    *(s32*)((s32)evt + 0xA4) = *(s32*)((s32)parentEvt + 0xA4);
    *(s32*)((s32)evt + 0xA8) = *(s32*)((s32)parentEvt + 0xA8);
    *(s32*)((s32)evt + 0xAC) = *(s32*)((s32)parentEvt + 0xAC);
    *(s32*)((s32)evt + 0xB0) = *(s32*)((s32)parentEvt + 0xB0);
    *(s32*)((s32)evt + 0xB4) = *(s32*)((s32)parentEvt + 0xB4);
    *(s32*)((s32)evt + 0xB8) = *(s32*)((s32)parentEvt + 0xB8);
    *(s32*)((s32)evt + 0xBC) = *(s32*)((s32)parentEvt + 0xBC);
    *(s32*)((s32)evt + 0xC0) = *(s32*)((s32)parentEvt + 0xC0);
    *(s32*)((s32)evt + 0xC4) = *(s32*)((s32)parentEvt + 0xC4);
    *(s32*)((s32)evt + 0xC8) = *(s32*)((s32)parentEvt + 0xC8);
    *(s32*)((s32)evt + 0xCC) = *(s32*)((s32)parentEvt + 0xCC);
    *(s32*)((s32)evt + 0xD0) = *(s32*)((s32)parentEvt + 0xD0);
    *(s32*)((s32)evt + 0xD4) = *(s32*)((s32)parentEvt + 0xD4);
    *(s32*)((s32)evt + 0xD8) = *(s32*)((s32)parentEvt + 0xD8);
    *(s32*)((s32)evt + 0xDC) = *(s32*)((s32)parentEvt + 0xDC);
    *(s32*)((s32)evt + 0xE0) = *(s32*)((s32)parentEvt + 0xE0);
    *(s32*)((s32)evt + 0xE4) = *(s32*)((s32)parentEvt + 0xE4);

    i = 0;
    zero = i;
    tableEntry = (u8*)evt;
    for (n = 0; n < 2; n++) {
        labelIds = (s8*)evt + i + 0x1C;
        labelIds[0] = -1; *(u32**)(tableEntry + 0x2C) = (u32*)zero;
        labelIds[1] = -1; *(u32**)(tableEntry + 0x30) = (u32*)zero;
        labelIds[2] = -1; *(u32**)(tableEntry + 0x34) = (u32*)zero;
        labelIds[3] = -1; *(u32**)(tableEntry + 0x38) = (u32*)zero;
        labelIds[4] = -1; *(u32**)(tableEntry + 0x3C) = (u32*)zero;
        labelIds[5] = -1; *(u32**)(tableEntry + 0x40) = (u32*)zero;
        labelIds[6] = -1; *(u32**)(tableEntry + 0x44) = (u32*)zero;
        labelIds[7] = -1; *(u32**)(tableEntry + 0x48) = (u32*)zero;
        i += 8;
        tableEntry += 0x20;
    }

    n = 0;
    cmd = evtCode;
    while (1) {
        word = *cmd;
        opcode = word & 0xFFFF;
        params = (s32)word >> 16;
        cmd++;
        label = *cmd;
        cmd += params;
        switch (opcode) {
            case 1:
                goto finished;
            case 3:
                *(s8*)((s32)evt + 0x1C + n) = (s8)label;
                *(u32**)((s32)evt + 0x2C + n * 4) = cmd;
                n++;
        }
        if (n >= 16) "unused";
    }

finished:
    if (runMainF != 0) {
        priTbl[priTblNum] = index;
        priIDTbl[priTblNum] = *(s32*)((s32)evt + 0x15C);
        priTblNum++;
    }
    ((void (*)(void*))evtEntryRunCheck)(evt);
    if (evtID == 0) evtID = 1;
    return evt;
}

void* evtBrotherEntry(void* parentEvt, void* evtCode, u32 flags) {
    typedef struct LocalEvtEntry {
        u32 lifetime[2];
        u8 flags;
        u8 params;
        u8 opcode;
        u8 priority;
        u8 type;
        u8 blocked;
        s8 loopDepth;
        s8 switchDepth;
        u8 pad_10[4];
        void* nextCommand;
        s32* args;
        s8 labelIdTable[16];
        u32* labelAddressTable[16];
        struct LocalEvtEntry* waitingEvent;
        struct LocalEvtEntry* waitingOnEvent;
        struct LocalEvtEntry* brotherEvent;
        u8 pad_78[0xDC];
        s32* uwBase;
        s32* ufBase;
        s32 eventId;
        s32 unitId;
        f32 timescale;
        f32 commandsLeft;
        s32 caseId;
        void* ownerNPC;
        u8 pad_174[0x2C];
        void* restartFrom;
        const char* name;
        void* currCommand;
        u32 field_1AC;
    } LocalEvtEntry;
    void* set;
    LocalEvtEntry* evt;
    LocalEvtEntry* brother = (LocalEvtEntry*)parentEvt;
    u32* cmd;
    u32 word;
    s32 params;
    s32 opcode;
    s32 label;
    s32 count;
    s32 index;
    s32 i;
    s32 n;
    s32 zero;
    s32 minusOne;
    u8* tableEntry;
    s8* labelIds;

    set = work;
    if (*(s32*)((s32)gp + 0x14) != 0) set = work + 0xA0;
    count = *(s32*)set;
    index = 0;
    evt = *(void**)((s32)set + 0x90);
    while (count > 0) {
        if ((*(u8*)((s32)evt + 8) & 1) == 0) break;
        index++;
        evt = (void*)((s32)evt + 0x1B0);
        count--;
    }

    evtMax++;
    memset(evt, 0, 0x1B0);
    evt->flags = flags | 1;
    minusOne = -1;
    evt->nextCommand = evtCode;
    evt->restartFrom = evtCode;
    evt->currCommand = evtCode;
    evt->opcode = 0;
    evt->waitingEvent = 0;
    evt->brotherEvent = brother;
    evt->waitingOnEvent = 0;
    evt->priority = brother->priority;
    evt->eventId = evtID++;
    evt->unitId = brother->unitId;
    evt->loopDepth = minusOne;
    evt->switchDepth = minusOne;
    evt->type = brother->type;
    evt->name = 0;
    evt->uwBase = brother->uwBase;
    evt->ufBase = brother->ufBase;
    evt->timescale = evtSpd;
    evt->commandsLeft = 0.0f;
    evt->caseId = minusOne;
    evt->ownerNPC = brother->ownerNPC;
    evt->lifetime[0] = 0;
    evt->lifetime[1] = 0;

    *(s32*)((s32)evt + 0x9C) = *(s32*)((s32)parentEvt + 0x9C);
    *(s32*)((s32)evt + 0xA0) = *(s32*)((s32)parentEvt + 0xA0);
    *(s32*)((s32)evt + 0xA4) = *(s32*)((s32)parentEvt + 0xA4);
    *(s32*)((s32)evt + 0xA8) = *(s32*)((s32)parentEvt + 0xA8);
    *(s32*)((s32)evt + 0xAC) = *(s32*)((s32)parentEvt + 0xAC);
    *(s32*)((s32)evt + 0xB0) = *(s32*)((s32)parentEvt + 0xB0);
    *(s32*)((s32)evt + 0xB4) = *(s32*)((s32)parentEvt + 0xB4);
    *(s32*)((s32)evt + 0xB8) = *(s32*)((s32)parentEvt + 0xB8);
    *(s32*)((s32)evt + 0xBC) = *(s32*)((s32)parentEvt + 0xBC);
    *(s32*)((s32)evt + 0xC0) = *(s32*)((s32)parentEvt + 0xC0);
    *(s32*)((s32)evt + 0xC4) = *(s32*)((s32)parentEvt + 0xC4);
    *(s32*)((s32)evt + 0xC8) = *(s32*)((s32)parentEvt + 0xC8);
    *(s32*)((s32)evt + 0xCC) = *(s32*)((s32)parentEvt + 0xCC);
    *(s32*)((s32)evt + 0xD0) = *(s32*)((s32)parentEvt + 0xD0);
    *(s32*)((s32)evt + 0xD4) = *(s32*)((s32)parentEvt + 0xD4);
    *(s32*)((s32)evt + 0xD8) = *(s32*)((s32)parentEvt + 0xD8);
    *(s32*)((s32)evt + 0xDC) = *(s32*)((s32)parentEvt + 0xDC);
    *(s32*)((s32)evt + 0xE0) = *(s32*)((s32)parentEvt + 0xE0);
    *(s32*)((s32)evt + 0xE4) = *(s32*)((s32)parentEvt + 0xE4);

    i = 0;
    zero = i;
    tableEntry = (u8*)evt;
    for (n = 0; n < 2; n++) {
        labelIds = (s8*)evt + i + 0x1C;
        labelIds[0] = minusOne; *(u32**)(tableEntry + 0x2C) = (u32*)zero;
        labelIds[1] = minusOne; *(u32**)(tableEntry + 0x30) = (u32*)zero;
        labelIds[2] = minusOne; *(u32**)(tableEntry + 0x34) = (u32*)zero;
        labelIds[3] = minusOne; *(u32**)(tableEntry + 0x38) = (u32*)zero;
        labelIds[4] = minusOne; *(u32**)(tableEntry + 0x3C) = (u32*)zero;
        labelIds[5] = minusOne; *(u32**)(tableEntry + 0x40) = (u32*)zero;
        labelIds[6] = minusOne; *(u32**)(tableEntry + 0x44) = (u32*)zero;
        labelIds[7] = minusOne; *(u32**)(tableEntry + 0x48) = (u32*)zero;
        i += 8;
        tableEntry += 0x20;
    }

    n = 0;
    cmd = evtCode;
    while (1) {
        word = *cmd;
        opcode = word & 0xFFFF;
        params = (s32)word >> 16;
        cmd++;
        label = *cmd;
        cmd += params;
        switch (opcode) {
            case 1:
                goto finished;
            case 3:
                *(s8*)((s32)evt + 0x1C + n) = (s8)label;
                *(u32**)((s32)evt + 0x2C + n * 4) = cmd;
                n++;
        }
        if (n >= 16) "unused";
    }

finished:
    if (runMainF != 0) {
        priTbl[priTblNum] = index;
        priIDTbl[priTblNum] = *(s32*)((s32)evt + 0x15C);
        priTblNum++;
    }
    if (evtID == 0) evtID = 1;
    ((void (*)(void*))evtEntryRunCheck)(evt);
    return evt;
}

void* evtRestart(void* entry) {
    s32 params;
    s32 opcode;
    s32 label;
    s32 i;
    s32 n;
    s32* cmd;
    s32 zero;
    u8* tableEntry;
    s8* labelIds;

    zero = 0;
    cmd = *(s32**)((u8*)entry + 0x1A0);
    *(s32**)((u8*)entry + 0x14) = cmd;
    *(s32**)((u8*)entry + 0x1A8) = cmd;
    *(u8*)((u8*)entry + 0xA) = 0;
    *(f32*)((u8*)entry + 0x164) = 1.0f;
    *(f32*)((u8*)entry + 0x168) = 0.0f;
    *(s8*)((u8*)entry + 0xE) = -1;
    *(s8*)((u8*)entry + 0xF) = -1;
    *(f32*)((u8*)entry + 0x164) = evtSpd;
    *(f32*)((u8*)entry + 0x168) = 0.0f;
    *(s32*)((u8*)entry + 4) = 0;
    *(s32*)entry = 0;

    i = 0;
    tableEntry = (u8*)entry;
    for (n = 0; n < 2; n++) {
        labelIds = (s8*)entry + i + 0x1C;
        labelIds[0] = -1;
        *(s32**)(tableEntry + 0x2C) = (s32*)zero;
        labelIds[1] = -1;
        *(s32**)(tableEntry + 0x30) = (s32*)zero;
        labelIds[2] = -1;
        *(s32**)(tableEntry + 0x34) = (s32*)zero;
        labelIds[3] = -1;
        *(s32**)(tableEntry + 0x38) = (s32*)zero;
        labelIds[4] = -1;
        *(s32**)(tableEntry + 0x3C) = (s32*)zero;
        labelIds[5] = -1;
        *(s32**)(tableEntry + 0x40) = (s32*)zero;
        labelIds[6] = -1;
        *(s32**)(tableEntry + 0x44) = (s32*)zero;
        labelIds[7] = -1;
        *(s32**)(tableEntry + 0x48) = (s32*)zero;
        i += 8;
        tableEntry += 0x20;
    }

    n = 0;
    cmd = *(s32**)((u8*)entry + 0x14);
    tableEntry = (u8*)entry;
    while (1) {
        opcode = *cmd & 0xFFFF;
        params = *cmd++ >> 16;
        label = *cmd;
        cmd += params;

        switch (opcode) {
            case 1:
                goto finished;
            case 3:
                *(s8*)((u8*)entry + 0x1C + n) = (s8)label;
                *(s32**)(tableEntry + 0x2C) = cmd;
                n++;
                tableEntry += 4;
        }
        if (n >= 16) {
            "unused";
        }
    }

finished:
    evtEntryRunCheck();
    return entry;
}

void evtmgrMain(void) {
    extern s32 evtmgrCmd(void* entry);
    extern void make_pri_table(void);
    void* set;
    void* gpPtr;
    u8* entry;
    s64 now;
    s64 delta;
    u32 ticksPerMsec;
    s32 i;
    s32 cmdResult;
    s32 finished;
    s32* entryNum;
    s32* eventId;

    gpPtr = gp;
    set = work;
    if (*(s32*)((u8*)gpPtr + 0x14) != 0) {
        set = work + 0xA0;
    }

    runMainF = 1;
    now = *(s64*)((u8*)gpPtr + 0x38);
    delta = now - *(s64*)((u8*)set + 0x98);
    if (delta < 0) {
        delta = 0;
    }

    ticksPerMsec = (*(u32*)0x800000F8 >> 2) / 1000;
    if (delta / ticksPerMsec > 500) {
        delta = (1000 / *(s32*)((u8*)gpPtr + 4)) * ticksPerMsec;
    }
    *(s64*)((u8*)set + 0x98) = now;

    make_pri_table();
    entryNum = priTbl;
    eventId = priIDTbl;
    for (i = 0; i < priTblNum; i++, entryNum++, eventId++) {
        entry = *(u8**)((u8*)set + 0x90) + *entryNum * 0x1B0;
        if ((entry[8] & 1) != 0 &&
            *(s32*)(entry + 0x15C) == *eventId &&
            (entry[8] & 0x92) == 0) {
            *(s64*)entry += delta;
            *(f32*)(entry + 0x168) += *(f32*)(entry + 0x164);
            finished = 0;
            while (1) {
                if (*(f32*)(entry + 0x168) < 1.0f) {
                    break;
                }
                *(f32*)(entry + 0x168) -= 1.0f;
                cmdResult = evtmgrCmd(entry);
                if (cmdResult == 1) {
                    finished = 1;
                    break;
                }
                if (cmdResult == -1) {
                    break;
                }
            }
            if (finished != 0) {
                break;
            }
        }
    }
    runMainF = 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void evtDelete(void* evt) {
    typedef struct LocalEvtEntry {
        u8 pad_00[8];
        u8 flags;
        u8 pad_09[0x63];
        struct LocalEvtEntry* waitingEvent;
        struct LocalEvtEntry* waitingOnEvent;
        struct LocalEvtEntry* brotherEvent;
        u8 pad_78[0x24];
        s32 lwData[16];
        s32 lfData[3];
        u8 pad_E8[0x90];
        s32 printWindowId;
        s32 selectWindowId;
        u32 printWindowFlags;
        struct {
            f32 x;
            f32 y;
        } msg;
        u32 unk18C;
        u32 msgPriority;
        u8 pad_194[0x1C];
    } LocalEvtEntry;
    typedef struct LocalEvtWork {
        s32 count;
        u8 pad_04[0x8C];
        LocalEvtEntry* entries;
        u8 pad_94[0xC];
    } LocalEvtWork;
    LocalEvtEntry* current = evt;
    LocalEvtWork* set = (LocalEvtWork*)work;
    LocalEvtEntry* entry;
    register LocalEvtEntry* waiting;
    s32 i;

    if (*(s32*)((s32)gp + 0x14) != 0) set++;
    if (current->flags & 1) {
        if (current->waitingOnEvent != 0) evtDelete(current->waitingOnEvent);
        entry = set->entries;
        for (i = 0; i < set->count; i++, entry++) {
            if ((entry->flags & 1) && entry->brotherEvent == current) evtDelete(entry);
        }
        waiting = current->waitingEvent;
        if (waiting != 0) {
            waiting->flags &= 0xEF;
            waiting->waitingOnEvent = 0;
            waiting->lwData[0] = current->lwData[0];
            waiting->lwData[1] = current->lwData[1];
            waiting->lwData[2] = current->lwData[2];
            waiting->lwData[3] = current->lwData[3];
            waiting->lwData[4] = current->lwData[4];
            waiting->lwData[5] = current->lwData[5];
            waiting->lwData[6] = current->lwData[6];
            waiting->lwData[7] = current->lwData[7];
            waiting->lwData[8] = current->lwData[8];
            waiting->lwData[9] = current->lwData[9];
            waiting->lwData[10] = current->lwData[10];
            waiting->lwData[11] = current->lwData[11];
            waiting->lwData[12] = current->lwData[12];
            waiting->lwData[13] = current->lwData[13];
            waiting->lwData[14] = current->lwData[14];
            waiting->lwData[15] = current->lwData[15];
            waiting->lfData[0] = current->lfData[0];
            waiting->lfData[1] = current->lfData[1];
            waiting->lfData[2] = current->lfData[2];
            waiting->selectWindowId = current->selectWindowId;
            waiting->printWindowId = current->printWindowId;
            waiting->printWindowFlags = current->printWindowFlags;
            waiting->msg = current->msg;
            waiting->unk18C = current->unk18C;
            waiting->msgPriority = current->msgPriority;
        }
        current->flags &= 0xFE;
        memset(current, 0, 0x1B0);
        evtMax--;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void evtDeleteID(s32 id) {
    typedef struct LocalEvtEntry {
        u8 pad_00[8];
        u8 flags;
        u8 pad_09[0x153];
        s32 eventId;
        u8 pad_160[0x50];
    } LocalEvtEntry;
    typedef struct LocalEvtWork {
        s32 count;
        u8 pad_04[0x8C];
        LocalEvtEntry* entries;
        u8 pad_94[0xC];
    } LocalEvtWork;
    LocalEvtWork* set = (LocalEvtWork*)work;
    LocalEvtEntry* entry;
    int i;

    if (*(int*)((int)gp + 0x14) != 0) {
        set++;
    }
    entry = set->entries;
    for (i = 0; i < set->count; i++, entry++) {
        if ((entry->flags & 1) && entry->eventId == id) {
            evtDelete(entry);
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset


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

void evtSetPri(void* entry, s32 pri) {
    *(u8*)((s32)entry + 0xB) = pri;
}

void evtSetSpeed(void* entry, f32 speed) {
    *(f32*)((s32)entry + 0x164) = speed * evtSpd;
}

void evtSetType(void* entry, s32 type) {
    *(u8*)((s32)entry + 0xC) = type;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw on

void evtStop(int pEvt, u32 flags) {
    u8* set = work;
    u8* waiting;
    u8* entry;
    s32 i;

    if (*(s32*)((u8*)gp + 0x14) != 0) set += 0xA0;
    waiting = *(u8**)(pEvt + 0x70);
    if (waiting != 0) {
        u8* nestedSet = work;
        u8* nested = *(u8**)(waiting + 0x70);
        if (*(s32*)((u8*)gp + 0x14) != 0) nestedSet += 0xA0;
        if (nested != 0) {
            u8* child;
            if (*(u8**)(nested + 0x70) != 0) evtStop((s32)*(void**)(nested + 0x70), flags);
            child = *(u8**)(nestedSet + 0x90);
            for (i = 0; i < *(s32*)nestedSet; i++, child += 0x1B0) {
                if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == nested) evtStop((s32)child, flags);
            }
            if ((nested[0xC] & flags) != 0) nested[8] |= 2;
        }

        entry = *(u8**)(nestedSet + 0x90);
        for (i = 0; i < *(s32*)nestedSet; i++, entry += 0x1B0) {
            if ((entry[8] & 1) != 0 && *(u8**)(entry + 0x74) == waiting) {
                u8* childSet;
                u8* child;
                s32 j;
                if (*(s32*)((u8*)gp + 0x14) != 0) {
                    childSet = work + 0xA0;
                } else {
                    childSet = work;
                }
                if (*(u8**)(entry + 0x70) != 0) evtStop((s32)*(void**)(entry + 0x70), flags);
                child = *(u8**)(childSet + 0x90);
                for (j = 0; j < *(s32*)childSet; j++) {
                    if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == entry) evtStop((s32)child, flags);
                    child += 0x1B0;
                }
                if ((entry[0xC] & flags) != 0) entry[8] |= 2;
            }
        }
        if ((waiting[0xC] & flags) != 0) waiting[8] |= 2;
    }

    entry = *(u8**)(set + 0x90);
    for (i = 0; i < *(s32*)set; i++, entry += 0x1B0) {
        if ((entry[8] & 1) != 0 && *(void**)(entry + 0x74) == (void*)pEvt) {
            u8* entrySet;
            u8* nested = *(u8**)(entry + 0x70);
            u8* child;
            s32 j;

            if (*(s32*)((u8*)gp + 0x14) != 0) {
                entrySet = work + 0xA0;
            } else {
                entrySet = work;
            }
            if (nested != 0) {
                u8* nestedSet;
                if (*(s32*)((u8*)gp + 0x14) != 0) {
                    nestedSet = work + 0xA0;
                } else {
                    nestedSet = work;
                }
                if (*(u8**)(nested + 0x70) != 0) evtStop((s32)*(void**)(nested + 0x70), flags);
                child = *(u8**)(nestedSet + 0x90);
                for (j = 0; j < *(s32*)nestedSet; j++, child += 0x1B0) {
                    if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == nested) evtStop((s32)child, flags);
                }
                if ((nested[0xC] & flags) != 0) nested[8] |= 2;
            }

            child = *(u8**)(entrySet + 0x90);
            for (j = 0; j < *(s32*)entrySet; j++, child += 0x1B0) {
                if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == entry) {
                    u8* childSet;
                    u8* grandchild;
                    s32 k;
                    if (*(s32*)((u8*)gp + 0x14) != 0) {
                        childSet = work + 0xA0;
                    } else {
                        childSet = work;
                    }
                    if (*(u8**)(child + 0x70) != 0) evtStop((s32)*(void**)(child + 0x70), flags);
                    grandchild = *(u8**)(childSet + 0x90);
                    for (k = 0; k < *(s32*)childSet; k++) {
                        if ((grandchild[8] & 1) != 0 && *(u8**)(grandchild + 0x74) == child) {
                            evtStop((s32)grandchild, flags);
                        }
                        grandchild += 0x1B0;
                    }
                    if ((child[0xC] & flags) != 0) child[8] |= 2;
                }
            }
            if ((entry[0xC] & flags) != 0) entry[8] |= 2;
        }
    }
    if ((*(u8*)(pEvt + 0xC) & flags) != 0) *(u8*)(pEvt + 8) |= 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
#pragma opt_common_subs off
void evtStart(void* pEvt, u32 flags) {
    u8* set = work;
    u8* waiting;
    u8* entry;
    s32 i;
    s32 inBattle = *(s32*)((u8*)gp + 0x14);

    if (inBattle != 0) set += 0xA0;
    waiting = *(u8**)((u8*)pEvt + 0x70);
    if (waiting != 0) {
        u8* waitingSet = work;
        u8* nested = *(u8**)(waiting + 0x70);
        u8* battleSet;
        if (inBattle != 0) waitingSet += 0xA0;
        if (nested != 0) {
            u8* nestedChildSet = work;
            u8* child;
            if (inBattle != 0) nestedChildSet += 0xA0;
            if (*(u8**)(nested + 0x70) != 0) evtStart(*(void**)(nested + 0x70), flags);
            child = *(u8**)(nestedChildSet + 0x90);
            for (i = 0; i < *(s32*)nestedChildSet; i++, child += 0x1B0) {
                if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == nested) evtStart(child, flags);
            }
            if ((nested[0xC] & flags) != 0) nested[8] &= ~2;
        }
        battleSet = work + 0xA0;
        entry = *(u8**)(waitingSet + 0x90);
        for (i = 0; i < *(s32*)waitingSet; i++, entry += 0x1B0) {
            if ((entry[8] & 1) != 0 && *(u8**)(entry + 0x74) == waiting) {
                u8* childSet;
                u8* child;
                s32 j;
                if (*(s32*)((u8*)gp + 0x14) != 0) childSet = battleSet;
                else childSet = work;
                if (*(u8**)(entry + 0x70) != 0) evtStart(*(void**)(entry + 0x70), flags);
                child = *(u8**)(childSet + 0x90);
                for (j = 0; j < *(s32*)childSet; j++, child += 0x1B0) {
                    if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == entry) evtStart(child, flags);
                }
                if ((entry[0xC] & flags) != 0) entry[8] &= ~2;
            }
        }
        if ((waiting[0xC] & flags) != 0) waiting[8] &= ~2;
    }

    {
        u8* battleSet = work + 0xA0;
        entry = *(u8**)(set + 0x90);
        for (i = 0; i < *(s32*)set; i++, entry += 0x1B0) {
            if ((entry[8] & 1) != 0 && *(void**)(entry + 0x74) == pEvt) {
                u8* entrySet;
                u8* nested = *(u8**)(entry + 0x70);
                u8* child;
                s32 j;
                s32 entryBattle = *(s32*)((u8*)gp + 0x14);

                if (entryBattle != 0) entrySet = battleSet;
                else entrySet = work;
                if (nested != 0) {
                    u8* nestedSet;
                    if (entryBattle != 0) nestedSet = battleSet;
                    else nestedSet = work;
                    if (*(u8**)(nested + 0x70) != 0) evtStart(*(void**)(nested + 0x70), flags);
                    child = *(u8**)(nestedSet + 0x90);
                    for (j = 0; j < *(s32*)nestedSet; j++, child += 0x1B0) {
                        if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == nested) evtStart(child, flags);
                    }
                    if ((nested[0xC] & flags) != 0) nested[8] &= ~2;
                }
                child = *(u8**)(entrySet + 0x90);
                for (j = 0; j < *(s32*)entrySet; j++, child += 0x1B0) {
                    if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == entry) {
                        u8* childSet;
                        u8* grandchild;
                        s32 k;
                        if (*(s32*)((u8*)gp + 0x14) != 0) childSet = battleSet;
                        else childSet = work;
                        if (*(u8**)(child + 0x70) != 0) evtStart(*(void**)(child + 0x70), flags);
                        grandchild = *(u8**)(childSet + 0x90);
                        for (k = 0; k < *(s32*)childSet; k++, grandchild += 0x1B0) {
                            if ((grandchild[8] & 1) != 0 && *(u8**)(grandchild + 0x74) == child) evtStart(grandchild, flags);
                        }
                        if ((child[0xC] & flags) != 0) child[8] &= ~2;
                    }
                }
                if ((entry[0xC] & flags) != 0) entry[8] &= ~2;
            }
        }
    }
    if ((((u8*)pEvt)[0xC] & flags) != 0) ((u8*)pEvt)[8] &= ~2;
}
#pragma opt_common_subs reset

#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void evtStopID(int id) {
    typedef struct LocalEvtEntry {
        u8 pad_00[8];
        u8 flags;
        u8 pad_09[0x153];
        s32 eventId;
        u8 pad_160[0x50];
    } LocalEvtEntry;
    typedef struct LocalEvtWork {
        s32 count;
        u8 pad_04[0x8C];
        LocalEvtEntry* entries;
        u8 pad_94[0xC];
    } LocalEvtWork;
    LocalEvtWork* set;
    LocalEvtEntry* entry;
    int i;

    set = (LocalEvtWork*)work;
    if (*(int*)((int)gp + 0x14) != 0) {
        set++;
    }
    entry = set->entries;
    for (i = 0; i < set->count; i++, entry++) {
        if ((entry->flags & 1) && entry->eventId == id) {
            evtStop((int)entry, 0xEF);
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void evtStartID(int id) {
    typedef struct LocalEvtEntry {
        u8 pad_00[8];
        u8 flags;
        u8 pad_09[0x153];
        s32 eventId;
        u8 pad_160[0x50];
    } LocalEvtEntry;
    typedef struct LocalEvtWork {
        s32 count;
        u8 pad_04[0x8C];
        LocalEvtEntry* entries;
        u8 pad_94[0xC];
    } LocalEvtWork;
    LocalEvtWork* set;
    LocalEvtEntry* entry;
    int i;

    set = (LocalEvtWork*)work;
    if (*(int*)((int)gp + 0x14) != 0) {
        set++;
    }
    entry = set->entries;
    for (i = 0; i < set->count; i++, entry++) {
        if ((entry->flags & 1) && entry->eventId == id) {
            evtStart(entry, 0xEF);
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void evtStopAll(s32 type) {
    void* set;
    register void* entry;
    int i;

    set = work;
    if (*(int*)((int)gp + 0x14) != 0) {
        set = (void*)((int)set + 0xA0);
    }
    entry = *(void**)((int)set + 0x90);
    i = 0;
    while (i < *(int*)set) {
        if ((*(unsigned char*)((int)entry + 8) & 1) != 0) {
            evtStop((int)entry, type);
        }
        i++;
        entry = (void*)((int)entry + 0x1B0);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void evtStartAll(unsigned long flags) {
    typedef struct LocalEvtEntry {
        u8 pad_00[8];
        u8 flags;
        u8 pad_09[0x1A7];
    } LocalEvtEntry;
    typedef struct LocalEvtWork {
        s32 count;
        u8 pad_04[0x8C];
        LocalEvtEntry* entries;
        u8 pad_94[0xC];
    } LocalEvtWork;
    LocalEvtWork* set;
    LocalEvtEntry* entry;
    int i;

    set = (LocalEvtWork*)work;
    if (*(int*)((int)gp + 0x14) != 0) {
        set++;
    }
    entry = set->entries;
    for (i = 0; i < set->count; i++, entry++) {
        if ((entry->flags & 1) != 0) {
            evtStart(entry, flags);
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
void evtStopOther(void* except, unsigned long flags) {
    typedef struct LocalEvtEntry {
        u8 pad_00[8];
        u8 flags;
        u8 pad_09[0x1A7];
    } LocalEvtEntry;
    typedef struct LocalEvtWork {
        s32 count;
        u8 pad_04[0x8C];
        LocalEvtEntry* entries;
        u8 pad_94[0xC];
    } LocalEvtWork;
    LocalEvtWork* set;
    LocalEvtEntry* entry;
    int i;

    set = (LocalEvtWork*)work;
    if (*(int*)((int)gp + 0x14) != 0) {
        set++;
    }
    entry = set->entries;
    for (i = 0; i < set->count; i++, entry++) {
        if ((entry->flags & 1) != 0 && entry != except) {
            evtStop((int)entry, flags);
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
void evtStartOther(void* except, unsigned long flags) {
    typedef struct LocalEvtEntry {
        u8 pad_00[8];
        u8 flags;
        u8 pad_09[0x1A7];
    } LocalEvtEntry;
    typedef struct LocalEvtWork {
        s32 count;
        u8 pad_04[0x8C];
        LocalEvtEntry* entries;
        u8 pad_94[0xC];
    } LocalEvtWork;
    LocalEvtWork* set;
    LocalEvtEntry* entry;
    int i;

    set = (LocalEvtWork*)work;
    if (*(int*)((int)gp + 0x14) != 0) {
        set++;
    }
    entry = set->entries;
    for (i = 0; i < set->count; i++, entry++) {
        if ((entry->flags & 1) != 0 && entry != except) {
            evtStart(entry, flags);
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


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
