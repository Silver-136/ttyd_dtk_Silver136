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

void make_pri_table(void) {
    void* set;
    void* entry;
    s32* slotPtr;
    s32* idPtr;
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
    count = 0;
    for (n = 0; n < *(s32*)set; n++, entry = (void*)((s32)entry + 0x1B0)) {
        if ((*(u8*)((s32)entry + 8) & 1) != 0) {
            *slotPtr++ = n;
            *idPtr++ = *(s32*)((s32)entry + 0x15C);
            count++;
        }
    }
    priTblNum = count;

    slotPtr = priTbl;
    idPtr = priIDTbl;
    for (i = 0; i < count - 1; i++, slotPtr++, idPtr++) {
        slotJPtr = slotPtr + 1;
        idJPtr = idPtr + 1;
        for (j = i + 1; j < count; j++, slotJPtr++, idJPtr++) {
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
    s8* labelIds;
    u32** labelAddrs;

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

    labelIds = (s8*)evt + 0x1C;
    labelAddrs = (u32**)((s32)evt + 0x2C);
    for (i = 0; i < 2; i++) {
        labelIds[0] = -1; labelAddrs[0] = 0;
        labelIds[1] = -1; labelAddrs[1] = 0;
        labelIds[2] = -1; labelAddrs[2] = 0;
        labelIds[3] = -1; labelAddrs[3] = 0;
        labelIds[4] = -1; labelAddrs[4] = 0;
        labelIds[5] = -1; labelAddrs[5] = 0;
        labelIds[6] = -1; labelAddrs[6] = 0;
        labelIds[7] = -1; labelAddrs[7] = 0;
        labelIds += 8;
        labelAddrs += 8;
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
        if (opcode == 1) {
            break;
        }
        if (opcode == 3) {
            *(s8*)((s32)evt + 0x1C + n) = (s8)label;
            *(u32**)((s32)evt + 0x2C + n * 4) = cmd;
            n++;
        }
        if (n >= 16) {
            "unused";
        }
    }

    if (runMainF != 0 && (*(u8*)((s32)evt + 8) & 0x20) != 0) {
        priTbl[priTblNum] = index;
        priIDTbl[priTblNum] = *(s32*)((s32)evt + 0x15C);
        priTblNum++;
    }
    evtEntryRunCheck();
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
    s8* labelIds;
    u32** labelAddrs;

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

    labelIds = (s8*)evt + 0x1C;
    labelAddrs = (u32**)((s32)evt + 0x2C);
    for (i = 0; i < 2; i++) {
        labelIds[0] = -1; labelAddrs[0] = 0;
        labelIds[1] = -1; labelAddrs[1] = 0;
        labelIds[2] = -1; labelAddrs[2] = 0;
        labelIds[3] = -1; labelAddrs[3] = 0;
        labelIds[4] = -1; labelAddrs[4] = 0;
        labelIds[5] = -1; labelAddrs[5] = 0;
        labelIds[6] = -1; labelAddrs[6] = 0;
        labelIds[7] = -1; labelAddrs[7] = 0;
        labelIds += 8;
        labelAddrs += 8;
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
        if (opcode == 1) {
            break;
        }
        if (opcode == 3) {
            *(s8*)((s32)evt + 0x1C + n) = (s8)label;
            *(u32**)((s32)evt + 0x2C + n * 4) = cmd;
            n++;
        }
        if (n >= 16) {
            "unused";
        }
    }

    if (runMainF != 0 && (*(u8*)((s32)evt + 8) & 0x20) != 0) {
        priTbl[priTblNum] = index;
        priIDTbl[priTblNum] = *(s32*)((s32)evt + 0x15C);
        priTblNum++;
    }
    evtEntryRunCheck();
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
    s8* labelIds;
    u32** labelAddrs;

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

    labelIds = (s8*)evt + 0x1C;
    labelAddrs = (u32**)((s32)evt + 0x2C);
    for (i = 0; i < 2; i++) {
        labelIds[0] = -1; labelAddrs[0] = 0;
        labelIds[1] = -1; labelAddrs[1] = 0;
        labelIds[2] = -1; labelAddrs[2] = 0;
        labelIds[3] = -1; labelAddrs[3] = 0;
        labelIds[4] = -1; labelAddrs[4] = 0;
        labelIds[5] = -1; labelAddrs[5] = 0;
        labelIds[6] = -1; labelAddrs[6] = 0;
        labelIds[7] = -1; labelAddrs[7] = 0;
        labelIds += 8;
        labelAddrs += 8;
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
        if (opcode == 1) {
            break;
        }
        if (opcode == 3) {
            *(s8*)((s32)evt + 0x1C + n) = (s8)label;
            *(u32**)((s32)evt + 0x2C + n * 4) = cmd;
            n++;
        }
        if (n >= 16) {
            "unused";
        }
    }

    if (runMainF != 0) {
        priTbl[priTblNum] = index;
        priIDTbl[priTblNum] = *(s32*)((s32)evt + 0x15C);
        priTblNum++;
    }
    evtEntryRunCheck();
    if (evtID == 0) {
        evtID = 1;
    }
    return evt;
}

void* evtBrotherEntry(void* parentEvt, void* evtCode, u32 flags) {
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
    s8* labelIds;
    u32** labelAddrs;

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

    labelIds = (s8*)evt + 0x1C;
    labelAddrs = (u32**)((s32)evt + 0x2C);
    for (i = 0; i < 2; i++) {
        labelIds[0] = -1; labelAddrs[0] = 0;
        labelIds[1] = -1; labelAddrs[1] = 0;
        labelIds[2] = -1; labelAddrs[2] = 0;
        labelIds[3] = -1; labelAddrs[3] = 0;
        labelIds[4] = -1; labelAddrs[4] = 0;
        labelIds[5] = -1; labelAddrs[5] = 0;
        labelIds[6] = -1; labelAddrs[6] = 0;
        labelIds[7] = -1; labelAddrs[7] = 0;
        labelIds += 8;
        labelAddrs += 8;
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
        if (opcode == 1) {
            break;
        }
        if (opcode == 3) {
            *(s8*)((s32)evt + 0x1C + n) = (s8)label;
            *(u32**)((s32)evt + 0x2C + n * 4) = cmd;
            n++;
        }
        if (n >= 16) {
            "unused";
        }
    }

    if (runMainF != 0) {
        priTbl[priTblNum] = index;
        priIDTbl[priTblNum] = *(s32*)((s32)evt + 0x15C);
        priTblNum++;
    }
    if (evtID == 0) {
        evtID = 1;
    }
    evtEntryRunCheck();
    return evt;
}

void* evtRestart(void* entry) {
    s32 params;
    s32 opcode;
    s32 label;
    s32 i;
    s32 n;
    s32* cmd;
    s8* labelIds;
    s32** labelAddrs;

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

    labelIds = (s8*)entry + 0x1C;
    labelAddrs = (s32**)((u8*)entry + 0x2C);
    for (i = 0; i < 2; i++) {
        labelIds[0] = -1;
        labelAddrs[0] = 0;
        labelIds[1] = -1;
        labelAddrs[1] = 0;
        labelIds[2] = -1;
        labelAddrs[2] = 0;
        labelIds[3] = -1;
        labelAddrs[3] = 0;
        labelIds[4] = -1;
        labelAddrs[4] = 0;
        labelIds[5] = -1;
        labelAddrs[5] = 0;
        labelIds[6] = -1;
        labelAddrs[6] = 0;
        labelIds[7] = -1;
        labelAddrs[7] = 0;
        labelIds += 8;
        labelAddrs += 8;
    }

    n = 0;
    cmd = *(s32**)((u8*)entry + 0x14);
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
                *(s32**)((u8*)entry + 0x2C + n * 4) = cmd;
                n++;
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
void evtDeleteID(s32 id) {
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
void evtStart(void* pEvt, u32 flags) {
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
            u8* entrySet = work;
            u8* nested = *(u8**)(entry + 0x70);
            u8* child;
            s32 childCount;
            s32 j;

            if (*(s32*)((u8*)gp + 0x14) != 0) entrySet += 0xA0;
            if (nested != 0) {
                u8* nestedSet = work;
                if (*(s32*)((u8*)gp + 0x14) != 0) nestedSet += 0xA0;
                if (*(u8**)(nested + 0x70) != 0) evtStart(*(void**)(nested + 0x70), flags);
                child = *(u8**)(nestedSet + 0x90);
                childCount = *(s32*)nestedSet;
                for (j = 0; j < childCount; j++, child += 0x1B0) {
                    if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == nested) evtStart(child, flags);
                }
                if ((nested[0xC] & flags) != 0) nested[8] &= ~2;
            }

            child = *(u8**)(entrySet + 0x90);
            childCount = *(s32*)entrySet;
            for (j = 0; j < childCount; j++, child += 0x1B0) {
                if ((child[8] & 1) != 0 && *(u8**)(child + 0x74) == entry) {
                    u8* childSet = work;
                    u8* grandchild;
                    s32 grandchildCount;
                    if (*(s32*)((u8*)gp + 0x14) != 0) childSet += 0xA0;
                    if (*(u8**)(child + 0x70) != 0) evtStart(*(void**)(child + 0x70), flags);
                    grandchild = *(u8**)(childSet + 0x90);
                    grandchildCount = *(s32*)childSet;
                    while (grandchildCount-- > 0) {
                        if ((grandchild[8] & 1) != 0 && *(u8**)(grandchild + 0x74) == child) {
                            evtStart(grandchild, flags);
                        }
                        grandchild += 0x1B0;
                    }
                    if ((child[0xC] & flags) != 0) child[8] &= ~2;
                }
            }
            if ((entry[0xC] & flags) != 0) entry[8] &= ~2;
        }
    }
    if ((((u8*)pEvt)[0xC] & flags) != 0) ((u8*)pEvt)[8] &= ~2;
}

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
