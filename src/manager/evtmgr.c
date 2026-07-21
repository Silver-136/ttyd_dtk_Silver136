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

void* evtGetWork(void) {
    if (*(s32*)((s32)gp + 0x14) != 0) {
        return work + 0xA0;
    }
    return work;
}

void make_pri_table(void) {
    extern unsigned char work[];
    extern void* gp;
    extern s32 priTblNum;
    extern s32 priTbl[];
    extern s32 priTblIndex[];
    void* set;
    void* entry;
    s32 count;
    s32 i;
    s32 j;

    set = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = work + 0xA0;
    }

    entry = *(void**)((s32)set + 0x90);
    count = 0;
    for (i = 0; i < *(s32*)set; i++) {
        if ((*(u8*)((s32)entry + 8) & 1) != 0) {
            priTblIndex[count] = i;
            priTbl[count] = *(s32*)((s32)entry + 0x15C);
            count++;
        }
        entry = (void*)((s32)entry + 0x1B0);
    }
    priTblNum = count;

    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            s32 idxA = priTblIndex[i];
            s32 idxB = priTblIndex[j];
            void* base = *(void**)((s32)set + 0x90);
            if (*(u8*)((s32)base + idxA * 0x1B0 + 0xB) < *(u8*)((s32)base + idxB * 0x1B0 + 0xB)) {
                s32 idA = priTbl[i];
                s32 idB = priTbl[j];
                priTblIndex[i] = idxB;
                priTbl[j] = idA;
                priTblIndex[j] = idxA;
                priTbl[i] = idB;
            }
        }
    }
}

void evtmgrInit(void) {
    extern unsigned char work[];
    extern void* gp;
    extern s32 evtMax;
    extern s32 priTblNum;
    extern s32 runMainF;
    extern void* __memAlloc(s32, u32);
    extern void* memset(void*, int, u32);
    extern void evt_msg_init(void);
    void* ptr;
    void* gpPtr;

    *(s32*)work = 0x100;
    ptr = __memAlloc(0, 0x100 * 0x1B0);
    *(void**)(work + 0x90) = ptr;
    gpPtr = gp;
    *(s32*)(work + 0x9C) = *(s32*)((s32)gpPtr + 0x3C);
    *(s32*)(work + 0x98) = *(s32*)((s32)gpPtr + 0x38);
    memset(ptr, 0, *(s32*)work * 0x1B0);
    memset(work + 4, 0, 0x80);
    memset(work + 0x84, 0, 0xC);

    *(s32*)(work + 0xA0) = 0x100;
    ptr = __memAlloc(0, 0x100 * 0x1B0);
    *(void**)(work + 0x130) = ptr;
    gpPtr = gp;
    *(s32*)(work + 0x13C) = *(s32*)((s32)gpPtr + 0x3C);
    *(s32*)(work + 0x138) = *(s32*)((s32)gpPtr + 0x38);
    memset(ptr, 0, *(s32*)(work + 0xA0) * 0x1B0);
    memset(work + 0xA4, 0, 0x80);
    memset(work + 0x124, 0, 0xC);

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
    extern s32 evtID;
    extern s32 priTblNum;
    extern s32 priTbl[];
    extern s32 priTblIndex[];
    void* set;
    void* evt;
    void* labelBase;
    u32* cmd;
    u32* next;
    u32 op;
    s32 count;
    s32 index;
    s32 i;

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
    *(s32*)((s32)evt + 0x15C) = evtID;
    evtID++;
    *(void**)((s32)evt + 0x160) = (void*)-1;
    *(u8*)((s32)evt + 0xE) = 0xFF;
    *(u8*)((s32)evt + 0xF) = 0xFF;
    *(u8*)((s32)evt + 0xC) = 0xEF;
    *(void**)((s32)evt + 0x1A4) = 0;
    *(f32*)((s32)evt + 0x164) = evtSpd;
    *(f32*)((s32)evt + 0x168) = 0.0f;
    *(s32*)((s32)evt + 0x16C) = -1;
    *(void**)((s32)evt + 0x170) = 0;
    *(s32*)evt = 0;
    *(s32*)((s32)evt + 4) = 0;

    for (i = 0; i < 0x10; i++) {
        *(s8*)((s32)evt + 0x1C + i) = -1;
        *(void**)((s32)evt + 0x2C + i * 4) = 0;
        *(s32*)((s32)evt + 0x9C + i * 4) = 0;
    }
    *(s32*)((s32)evt + 0xDC) = 0;
    *(s32*)((s32)evt + 0xE0) = 0;
    *(s32*)((s32)evt + 0xE4) = 0;

    cmd = (u32*)script;
    labelBase = evt;
    i = 0;
    do {
        do {
            next = cmd + 1;
            op = *cmd & 0xFFFF;
            cmd = next + ((s32)*cmd >> 0x10);
        } while (op == 2);
        if (op >= 2 && op < 4) {
            *(s8*)((s32)evt + 0x1C + i) = *(s8*)next;
            *(u32**)((s32)labelBase + 0x2C) = cmd;
            labelBase = (void*)((s32)labelBase + 4);
            i++;
        }
    } while (op == 0 || op >= 2);

    if (runMainF != 0 && (*(u8*)((s32)evt + 8) & 0x20) != 0) {
        priTblIndex[priTblNum] = index;
        priTbl[priTblNum] = *(s32*)((s32)evt + 0x15C);
        priTblNum++;
    }
    evtEntryRunCheck();
    if (evtID == 0) {
        evtID = 1;
    }
    return evt;
}

void* evtEntryType(void* script, s32 pri, s32 flags, s32 type) {
    extern s32 evtID;
    extern s32 priTblNum;
    extern s32 priTbl[];
    extern s32 priTblIndex[];
    void* set;
    void* evt;
    void* labelBase;
    u32* cmd;
    u32* next;
    u32 op;
    s32 count;
    s32 index;
    s32 i;

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
    *(s32*)((s32)evt + 0x15C) = evtID;
    evtID++;
    *(void**)((s32)evt + 0x160) = (void*)-1;
    *(u8*)((s32)evt + 0xE) = 0xFF;
    *(u8*)((s32)evt + 0xF) = 0xFF;
    *(u8*)((s32)evt + 0xC) = type;
    *(void**)((s32)evt + 0x1A4) = 0;
    *(f32*)((s32)evt + 0x164) = evtSpd;
    *(f32*)((s32)evt + 0x168) = 0.0f;
    *(s32*)((s32)evt + 0x16C) = -1;
    *(void**)((s32)evt + 0x170) = 0;
    *(s32*)evt = 0;
    *(s32*)((s32)evt + 4) = 0;

    for (i = 0; i < 0x10; i++) {
        *(s8*)((s32)evt + 0x1C + i) = -1;
        *(void**)((s32)evt + 0x2C + i * 4) = 0;
        *(s32*)((s32)evt + 0x9C + i * 4) = 0;
    }
    *(s32*)((s32)evt + 0xDC) = 0;
    *(s32*)((s32)evt + 0xE0) = 0;
    *(s32*)((s32)evt + 0xE4) = 0;

    cmd = (u32*)script;
    labelBase = evt;
    i = 0;
    do {
        do {
            next = cmd + 1;
            op = *cmd & 0xFFFF;
            cmd = next + ((s32)*cmd >> 0x10);
        } while (op == 2);
        if (op >= 2 && op < 4) {
            *(s8*)((s32)evt + 0x1C + i) = *(s8*)next;
            *(u32**)((s32)labelBase + 0x2C) = cmd;
            labelBase = (void*)((s32)labelBase + 4);
            i++;
        }
    } while (op == 0 || op >= 2);

    if (runMainF != 0 && (*(u8*)((s32)evt + 8) & 0x20) != 0) {
        priTblIndex[priTblNum] = index;
        priTbl[priTblNum] = *(s32*)((s32)evt + 0x15C);
        priTblNum++;
    }
    evtEntryRunCheck();
    if (evtID == 0) {
        evtID = 1;
    }
    return evt;
}
void* evtChildEntry(void* parentEvt, void* evtCode, s32 flags) {
    extern void* gp;
    extern f32 evtSpd;
    extern s32 evtMax;
    extern s32 evtID;
    extern s32 runMainF;
    extern s32 priTblNum;
    extern s32 priTbl[];
    extern s32 priTblIndex[];
    extern u8 work[];
    extern void evtEntryRunCheck(void);
    extern void* memset(void*, int, size_t);

    void* set;
    void* evt;
    void* labelBase;
    u32* cmd;
    u32* next;
    u32 op;
    s32 count;
    s32 index;
    s32 i;

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
    *(void**)((s32)parentEvt + 0x70) = evt;
    *(u8*)((s32)parentEvt + 8) = *(u8*)((s32)parentEvt + 8) | 0x10;
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
    *(s32*)((s32)evt + 0x15C) = evtID;
    evtID++;
    *(void**)((s32)evt + 0x160) = *(void**)((s32)parentEvt + 0x160);
    *(u8*)((s32)evt + 0xE) = 0xFF;
    *(u8*)((s32)evt + 0xF) = 0xFF;
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

    for (i = 0; i < 0x10; i++) {
        *(s8*)((s32)evt + 0x1C + i) = -1;
        *(void**)((s32)evt + 0x2C + i * 4) = 0;
        *(s32*)((s32)evt + 0x9C + i * 4) = *(s32*)((s32)parentEvt + 0x9C + i * 4);
    }
    *(s32*)((s32)evt + 0xDC) = *(s32*)((s32)parentEvt + 0xDC);
    *(s32*)((s32)evt + 0xE0) = *(s32*)((s32)parentEvt + 0xE0);
    *(s32*)((s32)evt + 0xE4) = *(s32*)((s32)parentEvt + 0xE4);

    cmd = (u32*)evtCode;
    labelBase = evt;
    i = 0;
    do {
        do {
            next = cmd + 1;
            op = *cmd & 0xFFFF;
            cmd = next + ((s32)*cmd >> 0x10);
        } while (op == 2);
        if (op >= 2 && op < 4) {
            *(s8*)((s32)evt + 0x1C + i) = *(s8*)next;
            *(u32**)((s32)labelBase + 0x2C) = cmd;
            labelBase = (void*)((s32)labelBase + 4);
            i++;
        }
    } while (op != 0);

    if (runMainF != 0) {
        priTblIndex[priTblNum] = index;
        priTbl[priTblNum] = *(s32*)((s32)evt + 0x15C);
        priTblNum++;
    }
    evtEntryRunCheck();
    if (evtID == 0) {
        evtID = 1;
    }
    return evt;
}

void* evtBrotherEntry(void* parentEvt, void* evtCode, u32 flags) {
    extern s32 evtID;
    extern s32 priTblNum;
    extern s32 priTbl[];
    extern s32 priTblIndex[];
    void* set;
    void* evt;
    void* labelBase;
    u32* cmd;
    u32* next;
    u32 op;
    s32 count;
    s32 index;
    s32 i;

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
    *(void**)((s32)evt + 0x14) = evtCode;
    *(void**)((s32)evt + 0x1A0) = evtCode;
    *(void**)((s32)evt + 0x1A8) = evtCode;
    *(u8*)((s32)evt + 0xA) = 0;
    *(void**)((s32)evt + 0x6C) = 0;
    *(void**)((s32)evt + 0x74) = parentEvt;
    *(void**)((s32)evt + 0x70) = 0;
    *(u8*)((s32)evt + 0xB) = *(u8*)((s32)parentEvt + 0xB);
    *(s32*)((s32)evt + 0x15C) = evtID;
    evtID++;
    *(void**)((s32)evt + 0x160) = *(void**)((s32)parentEvt + 0x160);
    *(u8*)((s32)evt + 0xE) = 0xFF;
    *(u8*)((s32)evt + 0xF) = 0xFF;
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

    for (i = 0; i < 0x10; i++) {
        *(s8*)((s32)evt + 0x1C + i) = -1;
        *(void**)((s32)evt + 0x2C + i * 4) = 0;
        *(s32*)((s32)evt + 0x9C + i * 4) = *(s32*)((s32)parentEvt + 0x9C + i * 4);
    }
    *(s32*)((s32)evt + 0xDC) = *(s32*)((s32)parentEvt + 0xDC);
    *(s32*)((s32)evt + 0xE0) = *(s32*)((s32)parentEvt + 0xE0);
    *(s32*)((s32)evt + 0xE4) = *(s32*)((s32)parentEvt + 0xE4);

    cmd = (u32*)evtCode;
    labelBase = evt;
    i = 0;
    do {
        do {
            next = cmd + 1;
            op = *cmd & 0xFFFF;
            cmd = next + ((s32)*cmd >> 0x10);
        } while (op == 2);
        if (op >= 2 && op < 4) {
            *(s8*)((s32)evt + 0x1C + i) = *(s8*)next;
            *(u32**)((s32)labelBase + 0x2C) = cmd;
            labelBase = (void*)((s32)labelBase + 4);
            i++;
        }
    } while (op == 0 || op >= 2);

    if (runMainF != 0) {
        priTblIndex[priTblNum] = index;
        priTbl[priTblNum] = *(s32*)((s32)evt + 0x15C);
        priTblNum++;
    }
    if (evtID == 0) {
        evtID = 1;
    }
    evtEntryRunCheck();
    return evt;
}

void* evtRestart(void* entry) {
    extern f32 evtSpd;
    extern void* evtEntryRunCheck(void);
    unsigned int* ip;
    unsigned int cmd;
    s32 opcode;
    s32 count;
    s32 i;
    void* slotBase;
    float zero;

    zero = 0.0f;
    ip = *(unsigned int**)((s32)entry + 0x1A0);
    *(unsigned int**)((s32)entry + 0x14) = ip;
    *(unsigned int**)((s32)entry + 0x1A8) = ip;
    *(u8*)((s32)entry + 0xA) = 0;
    *(f32*)((s32)entry + 0x164) = 1.0f;
    *(f32*)((s32)entry + 0x168) = zero;
    *(s8*)((s32)entry + 0xE) = -1;
    *(s8*)((s32)entry + 0xF) = -1;
    *(f32*)((s32)entry + 0x164) = evtSpd;
    *(f32*)((s32)entry + 0x168) = zero;
    *(s32*)((s32)entry + 4) = 0;
    *(s32*)entry = 0;

    slotBase = entry;
    for (i = 0; i < 0x10; i++) {
        *(s8*)((s32)entry + 0x1C + i) = -1;
        *(void**)((s32)slotBase + 0x2C) = 0;
        slotBase = (void*)((s32)slotBase + 4);
    }

    ip = *(unsigned int**)((s32)entry + 0x14);
    slotBase = entry;
    count = 0;
    while (1) {
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (s32)cmd >> 0x10;
        if (opcode == 1) {
            break;
        }
        if (opcode >= 2 && opcode < 4) {
            *(s8*)((s32)entry + 0x1C + count) = *(s8*)ip;
            *(unsigned int**)((s32)slotBase + 0x2C) = ip;
            slotBase = (void*)((s32)slotBase + 4);
            count++;
        }
        if (count >= 0x10) {
            continue;
        }
    }
    evtEntryRunCheck();
    return entry;
}

void evtmgrMain(void) {
    extern s32 priTblNum;
    extern s32 evtmgrCmd(void* entry);
    extern void make_pri_table(void);
    extern s32 priTbl[];
    extern s32 priTblId[];
    void* set;
    void* gpPtr;
    void* entry;
    u64 now;
    u64 last;
    u64 delta;
    u32 tick;
    u32 frameDelta;
    s32 i;
    s32 cmdResult;
    s32 stop;

    gpPtr = gp;
    set = work;
    if (*(s32*)((s32)gpPtr + 0x14) != 0) {
        set = work + 0xA0;
    }

    runMainF = 1;
    now = *(u64*)((s32)gpPtr + 0x38);
    last = *(u64*)((s32)set + 0x98);
    if (now < last) {
        delta = 0;
    } else {
        delta = now - last;
    }
    tick = (*(u32*)0x800000F8) / 4000;
    frameDelta = delta / tick;
    if (frameDelta > 500) {
        frameDelta = (1000 / *(s32*)((s32)gpPtr + 4)) * tick;
    }
    *(u64*)((s32)set + 0x98) = now;

    make_pri_table();
    i = 0;
    while (i < priTblNum) {
        entry = (void*)((s32)*(void**)((s32)set + 0x90) + priTblId[i] * 0x1B0);
        if ((*(u8*)((s32)entry + 8) & 1) != 0 &&
            *(s32*)((s32)entry + 0x15C) == priTbl[i] &&
            (*(u8*)((s32)entry + 8) & 0x92) == 0) {
            *(u64*)entry += frameDelta;
            *(f32*)((s32)entry + 0x168) += *(f32*)((s32)entry + 0x164);
            stop = 0;
            while (*(f32*)((s32)entry + 0x168) >= 1.0f) {
                *(f32*)((s32)entry + 0x168) -= 1.0f;
                cmdResult = evtmgrCmd(entry);
                if (cmdResult == 1) {
                    stop = 1;
                    break;
                }
                if (cmdResult == -1) {
                    break;
                }
            }
            if (stop != 0) {
                break;
            }
        }
        i++;
    }
    runMainF = 0;
}

void evtDelete(void* evt) {
    void* set;
    void* entry;
    void* waiting;
    s32 i;

    set = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = work + 0xA0;
    }
    if ((*(u8*)((s32)evt + 8) & 1) != 0) {
        if (*(void**)((s32)evt + 0x70) != 0) {
            evtDelete(*(void**)((s32)evt + 0x70));
        }
        entry = *(void**)((s32)set + 0x90);
        i = 0;
        while (i < *(s32*)set) {
            if ((*(u8*)((s32)entry + 8) & 1) != 0 && *(void**)((s32)entry + 0x74) == evt) {
                evtDelete(entry);
            }
            i++;
            entry = (void*)((s32)entry + 0x1B0);
        }
        waiting = *(void**)((s32)evt + 0x6C);
        if (waiting != 0) {
            *(u8*)((s32)waiting + 8) = *(u8*)((s32)waiting + 8) & 0xEF;
            *(void**)((s32)waiting + 0x70) = 0;
            *(s32*)((s32)waiting + 0x9C) = *(s32*)((s32)evt + 0x9C);
            *(s32*)((s32)waiting + 0xA0) = *(s32*)((s32)evt + 0xA0);
            *(s32*)((s32)waiting + 0xA4) = *(s32*)((s32)evt + 0xA4);
            *(s32*)((s32)waiting + 0xA8) = *(s32*)((s32)evt + 0xA8);
            *(s32*)((s32)waiting + 0xAC) = *(s32*)((s32)evt + 0xAC);
            *(s32*)((s32)waiting + 0xB0) = *(s32*)((s32)evt + 0xB0);
            *(s32*)((s32)waiting + 0xB4) = *(s32*)((s32)evt + 0xB4);
            *(s32*)((s32)waiting + 0xB8) = *(s32*)((s32)evt + 0xB8);
            *(s32*)((s32)waiting + 0xBC) = *(s32*)((s32)evt + 0xBC);
            *(s32*)((s32)waiting + 0xC0) = *(s32*)((s32)evt + 0xC0);
            *(s32*)((s32)waiting + 0xC4) = *(s32*)((s32)evt + 0xC4);
            *(s32*)((s32)waiting + 0xC8) = *(s32*)((s32)evt + 0xC8);
            *(s32*)((s32)waiting + 0xCC) = *(s32*)((s32)evt + 0xCC);
            *(s32*)((s32)waiting + 0xD0) = *(s32*)((s32)evt + 0xD0);
            *(s32*)((s32)waiting + 0xD4) = *(s32*)((s32)evt + 0xD4);
            *(s32*)((s32)waiting + 0xD8) = *(s32*)((s32)evt + 0xD8);
            *(s32*)((s32)waiting + 0xDC) = *(s32*)((s32)evt + 0xDC);
            *(s32*)((s32)waiting + 0xE0) = *(s32*)((s32)evt + 0xE0);
            *(s32*)((s32)waiting + 0xE4) = *(s32*)((s32)evt + 0xE4);
            *(s32*)((s32)waiting + 0x17C) = *(s32*)((s32)evt + 0x17C);
            *(s32*)((s32)waiting + 0x178) = *(s32*)((s32)evt + 0x178);
            *(s32*)((s32)waiting + 0x180) = *(s32*)((s32)evt + 0x180);
            *(s32*)((s32)waiting + 0x184) = *(s32*)((s32)evt + 0x184);
            *(s32*)((s32)waiting + 0x188) = *(s32*)((s32)evt + 0x188);
            *(s32*)((s32)waiting + 0x18C) = *(s32*)((s32)evt + 0x18C);
            *(s32*)((s32)waiting + 0x190) = *(s32*)((s32)evt + 0x190);
        }
        *(u8*)((s32)evt + 8) = *(u8*)((s32)evt + 8) & 0xFE;
        memset(evt, 0, 0x1B0);
        evtMax--;
    }
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void evtDeleteID(int id) {
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
        if ((*(unsigned char*)((int)entry + 8) & 1) != 0 && *(int*)((int)entry + 0x15C) == id) {
            evtDelete(entry);
        }
        i++;
        entry = (void*)((int)entry + 0x1B0);
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
#pragma use_lmw_stmw off
void evtStop(int pEvt, u32 flags) {
    extern void* gp;
    extern u8 work[];
    u8* set = work;
    u8* waiting;
    u8* entry;
    s32 count;
    s32 i;

    if (*(s32*)((u8*)gp + 0x14) != 0) set += 0xA0;
    waiting = *(u8**)(pEvt + 0x70);
    if (waiting != 0) {
        u8* nestedSet = work;
        u8* nested = *(u8**)(waiting + 0x70);
        if (*(s32*)((u8*)gp + 0x14) != 0) nestedSet += 0xA0;
        if (nested != 0) {
            u8* child;
            s32 childCount;
            if (*(u8**)(nested + 0x70) != 0) evtStop((s32)*(void**)(nested + 0x70), flags);
            child = *(u8**)(nestedSet + 0x90);
            childCount = *(s32*)nestedSet;
            for (i = 0; i < childCount; i++, child += 0x1B0) {
                if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == nested) evtStop((s32)child, flags);
            }
            if ((nested[0xC] & flags) != 0) nested[8] |= 2;
        }
        entry = *(u8**)(nestedSet + 0x90);
        count = *(s32*)nestedSet;
        for (i = 0; i < count; i++, entry += 0x1B0) {
            if ((entry[8] & 1) != 0 && *(u8**)(entry + 0x74) == waiting) {
                u8* childSet = work;
                u8* child;
                s32 childCount;
                if (*(s32*)((u8*)gp + 0x14) != 0) childSet += 0xA0;
                if (*(u8**)(entry + 0x70) != 0) evtStop((s32)*(void**)(entry + 0x70), flags);
                child = *(u8**)(childSet + 0x90);
                childCount = *(s32*)childSet;
                while (childCount-- > 0) {
                    if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == entry) evtStop((s32)child, flags);
                    child += 0x1B0;
                }
                if ((entry[0xC] & flags) != 0) entry[8] |= 2;
            }
        }
        if ((waiting[0xC] & flags) != 0) waiting[8] |= 2;
    }

    entry = *(u8**)(set + 0x90);
    count = *(s32*)set;
    for (i = 0; i < count; i++, entry += 0x1B0) {
        if ((entry[8] & 1) != 0 && *(u8**)(entry + 0x74) == (u8*)pEvt) {
            u8* childSet = work;
            u8* child;
            s32 childCount;
            if (*(s32*)((u8*)gp + 0x14) != 0) childSet += 0xA0;
            if (*(u8**)(entry + 0x70) != 0) evtStop((s32)*(void**)(entry + 0x70), flags);
            child = *(u8**)(childSet + 0x90);
            childCount = *(s32*)childSet;
            while (childCount-- > 0) {
                if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == entry) evtStop((s32)child, flags);
                child += 0x1B0;
            }
            if ((entry[0xC] & flags) != 0) entry[8] |= 2;
        }
    }
    if ((*(u8*)(pEvt + 0xC) & flags) != 0) *(u8*)(pEvt + 8) |= 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void evtStart(void* pEvt, u32 flags) {
    extern void* gp;
    extern u8 work[];
    u8* set = work;
    u8* waiting;
    u8* entry;
    s32 count;
    s32 i;

    if (*(s32*)((u8*)gp + 0x14) != 0) set += 0xA0;
    waiting = *(u8**)((u8*)pEvt + 0x70);
    if (waiting != 0) {
        u8* nestedSet = work;
        u8* nested = *(u8**)(waiting + 0x70);
        if (*(s32*)((u8*)gp + 0x14) != 0) nestedSet += 0xA0;
        if (nested != 0) {
            u8* child;
            s32 childCount;
            if (*(u8**)(nested + 0x70) != 0) evtStart(*(void**)(nested + 0x70), flags);
            child = *(u8**)(nestedSet + 0x90);
            childCount = *(s32*)nestedSet;
            for (i = 0; i < childCount; i++, child += 0x1B0) {
                if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == nested) evtStart(child, flags);
            }
            if ((nested[0xC] & flags) != 0) nested[8] &= ~2;
        }

        entry = *(u8**)(nestedSet + 0x90);
        count = *(s32*)nestedSet;
        for (i = 0; i < count; i++, entry += 0x1B0) {
            if ((entry[8] & 1) != 0 && *(u8**)(entry + 0x74) == waiting) {
                u8* childSet = work;
                u8* child;
                s32 childCount;
                if (*(s32*)((u8*)gp + 0x14) != 0) childSet += 0xA0;
                if (*(u8**)(entry + 0x70) != 0) evtStart(*(void**)(entry + 0x70), flags);
                child = *(u8**)(childSet + 0x90);
                childCount = *(s32*)childSet;
                while (childCount-- > 0) {
                    if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == entry) evtStart(child, flags);
                    child += 0x1B0;
                }
                if ((entry[0xC] & flags) != 0) entry[8] &= ~2;
            }
        }
        if ((waiting[0xC] & flags) != 0) waiting[8] &= ~2;
    }

    entry = *(u8**)(set + 0x90);
    count = *(s32*)set;
    for (i = 0; i < count; i++, entry += 0x1B0) {
        if ((entry[8] & 1) != 0 && *(void**)(entry + 0x74) == pEvt) {
            u8* childSet = work;
            u8* child;
            s32 childCount;
            if (*(s32*)((u8*)gp + 0x14) != 0) childSet += 0xA0;
            if (*(u8**)(entry + 0x70) != 0) evtStart(*(void**)(entry + 0x70), flags);
            child = *(u8**)(childSet + 0x90);
            childCount = *(s32*)childSet;
            while (childCount-- > 0) {
                if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == entry) evtStart(child, flags);
                child += 0x1B0;
            }
            if ((entry[0xC] & flags) != 0) entry[8] &= ~2;
        }
    }
    if ((((u8*)pEvt)[0xC] & flags) != 0) ((u8*)pEvt)[8] &= ~2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void evtStopID(int id) {
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
        if ((*(unsigned char*)((int)entry + 8) & 1) != 0 && *(int*)((int)entry + 0x15C) == id) {
            evtStop((int)entry, 0xEF);
        }
        i++;
        entry = (void*)((int)entry + 0x1B0);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void evtStartID(int id) {
    register void* set;
    void* entry;
    int i;

    set = work;
    if (*(int*)((int)gp + 0x14) != 0) {
        set = (void*)((int)set + 0xA0);
    }
    entry = *(void**)((int)set + 0x90);
    i = 0;
    while (i < *(int*)set) {
        if ((*(unsigned char*)((int)entry + 8) & 1) != 0 && *(int*)((int)entry + 0x15C) == id) {
            evtStart(entry, 0xEF);
        }
        i++;
        entry = (void*)((int)entry + 0x1B0);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset

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
            evtStart(entry, flags);
        }
        i++;
        entry = (void*)((int)entry + 0x1B0);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
void evtStopOther(void* except, unsigned long flags) {
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
        if ((*(unsigned char*)((int)entry + 8) & 1) != 0 && entry != except) {
            evtStop((int)entry, flags);
        }
        i++;
        entry = (void*)((int)entry + 0x1B0);
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
void evtStartOther(void* except, unsigned long flags) {
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
        if ((*(unsigned char*)((int)entry + 8) & 1) != 0 && entry != except) {
            evtStart(entry, flags);
        }
        i++;
        entry = (void*)((int)entry + 0x1B0);
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
