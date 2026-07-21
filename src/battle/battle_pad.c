#include "battle/battle_pad.h"

void BtlPad_WorkUpdate(void* work, s32 flags);

extern void* _battleWorkPointer;

extern s32 BattlePadMultiCheckNow(s32 controller, s32 mask);
extern s32 BattlePadMultiCheckRepeat(s32 controller, s32 mask);
extern s32 BattlePadMultiCheckTrigger(s32 controller, s32 mask);
extern s32 BattlePadMultiCheckUp(s32 controller, s32 mask);
extern s32 BattlePadMultiCheckRecordTrigger(s32 controller, s32 recordIndex, s32 mask);
extern void BtlPad_WorkInit(void* work);

void BattlePadInit(void) {
    s32 offset;
    s32 i;
    s32 addr;

    i = 0;
    offset = 0;

    for (; i < 4; i++) {
        addr = offset + 0xF2C;
        addr = (s32)_battleWorkPointer + addr;
        BtlPad_WorkInit((void*)addr);
        offset += 0x1FC;
    }
}


void BattlePadManager(void) {
    s32 offset;
    s32 i;

    i = 0;
    offset = 0;
    for (; i < 4; i++) {
        BtlPad_WorkUpdate((void*)((s32)_battleWorkPointer + offset + 0xF2C), i);
        offset += 0x1FC;
    }
}

void BtlPad_WorkInit(void* work) {
    s32 i;
    s32 offset;
    void* entry;

    offset = 0;
    for (i = 0; i < 2; i++) {
        entry = (void*)((s32)work + offset);
        *(u16*)((s32)entry + 0) = 0;
        *(u8*)((s32)entry + 2) = 0;
        *(u8*)((s32)entry + 3) = 0;
        *(u8*)((s32)entry + 4) = 0;
        *(u8*)((s32)entry + 5) = 0;
        *(u8*)((s32)entry + 6) = 0;
        *(u8*)((s32)entry + 7) = 0;
        *(u8*)((s32)entry + 8) = 0;
        *(u8*)((s32)entry + 9) = 0;
        *(u8*)((s32)entry + 0xA) = 0;
        offset += 0xC;
    }

    offset = 0;
    for (i = 0; i < 30; i++) {
        entry = (void*)((s32)work + offset);
        *(s32*)((s32)entry + 0x18) = 0;
        *(s32*)((s32)entry + 0x90) = 0;
        *(s32*)((s32)entry + 0x108) = 0;
        offset += 4;
    }

    *(u8*)((s32)work + 0x1F8) = 0;
    *(u8*)((s32)work + 0x1F9) = 0;
}

void BtlPad_WorkUpdate(void* work, s32 flags) {
    extern void* gp;
    void* pad;
    s32 i;
    s32 offset;
    s32 now;
    s32 previous;
    s32 repeat;
    s32 trigger;

    *(s32*)((s32)work + 0xC) = *(s32*)work;
    *(s32*)((s32)work + 0x10) = *(s32*)((s32)work + 4);
    *(s32*)((s32)work + 0x14) = *(s32*)((s32)work + 8);

    offset = 0x70;
    for (i = 0; i < 29; i++) {
        pad = (void*)((s32)work + offset);
        offset -= 4;
        *(s32*)((s32)pad + 0x1C) = *(s32*)((s32)pad + 0x18);
        *(s32*)((s32)pad + 0x94) = *(s32*)((s32)pad + 0x90);
        *(s32*)((s32)pad + 0x10C) = *(s32*)((s32)pad + 0x108);
    }

    *(u16*)work = *(s32*)((s32)gp + flags * 4 + 0x1328);
    *(u8*)((s32)work + 2) = *(u8*)((s32)gp + flags + 0x13B8);
    *(u8*)((s32)work + 3) = *(u8*)((s32)gp + flags + 0x13BC);
    *(u8*)((s32)work + 4) = *(u8*)((s32)gp + flags + 0x13C0);
    *(u8*)((s32)work + 5) = *(u8*)((s32)gp + flags + 0x13C4);
    *(u8*)((s32)work + 6) = *(u8*)((s32)gp + flags + 0x13C8);
    *(u8*)((s32)work + 7) = *(u8*)((s32)gp + flags + 0x13CC);
    *(u8*)((s32)work + 8) = 0;
    *(u8*)((s32)work + 9) = 0;

    *(s32*)((s32)work + 0x108) = *(u16*)work;
    *(s32*)((s32)work + 0x90) = *(s32*)((s32)gp + flags * 4 + 0x1348);
    *(s32*)((s32)work + 0x18) = *(s32*)((s32)gp + flags * 4 + 0x1338);
    *(s32*)((s32)work + 0x180) = *(s32*)((s32)gp + flags * 4 + 0x1368);

    if (*(s8*)((s32)work + 3) >= 30) {
        *(s32*)((s32)work + 0x108) |= 0x10000;
    }
    if (*(s8*)((s32)work + 3) <= -30) {
        *(s32*)((s32)work + 0x108) |= 0x20000;
    }
    if (*(s8*)((s32)work + 2) <= -30) {
        *(s32*)((s32)work + 0x108) |= 0x40000;
    }
    if (*(s8*)((s32)work + 2) >= 30) {
        *(s32*)((s32)work + 0x108) |= 0x80000;
    }
    if (*(s8*)((s32)work + 5) >= 30) {
        *(s32*)((s32)work + 0x108) |= 0x100000;
    }
    if (*(s8*)((s32)work + 5) <= -30) {
        *(s32*)((s32)work + 0x108) |= 0x200000;
    }
    if (*(s8*)((s32)work + 4) <= -30) {
        *(s32*)((s32)work + 0x108) |= 0x400000;
    }
    if (*(s8*)((s32)work + 4) >= 30) {
        *(s32*)((s32)work + 0x108) |= 0x800000;
    }

    now = *(s32*)((s32)work + 0x108);
    previous = *(s32*)((s32)work + 0x10C);
    *(s32*)((s32)work + 0x18) |= (now & ~previous) & 0xFFFF0000;

    if (now == 0) {
        *(s32*)((s32)work + 0x90) = 0;
        *(u8*)((s32)work + 0x1F8) = 0;
        *(u8*)((s32)work + 0x1F9) = 0;
    } else if (now != previous) {
        repeat = *(s32*)((s32)work + 0x90);
        *(s32*)((s32)work + 0x90) = repeat | (now & 0xFFFF0000);
        trigger = *(s32*)((s32)work + 0x18) & 0xFFFF0000;
        *(s32*)((s32)work + 0x90) &= trigger | 0xFFFF;
        *(u8*)((s32)work + 0x1F8) = 1;
        *(u8*)((s32)work + 0x1F9) = 0;
    } else {
        *(u8*)((s32)work + 0x1F9) += 1;
        if (*(s8*)((s32)work + 0x1F9) >= (*(s8*)((s32)work + 0x1F8) == 1 ? 16 : 8)) {
            *(s32*)((s32)work + 0x90) |= now & 0xFFFF0000;
            *(u8*)((s32)work + 0x1F8) = 2;
            *(u8*)((s32)work + 0x1F9) = 0;
        }
    }

    *(s32*)((s32)work + 0x180) |= (previous & (previous ^ now)) & 0xFFFF0000;
}

s32 BattlePadGetTrigger(void) {
    return *(s32*)((s32)_battleWorkPointer + 0xF44);
}

s32 BattlePadGetNow(void) {
    return *(s32*)((s32)_battleWorkPointer + 0x1034);
}

s32 BattlePadCheckTrigger(s32 mask) {
    return BattlePadMultiCheckTrigger(0, mask);
}

s32 BattlePadMultiCheckTrigger(s32 controller, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0xF44);
    return value & mask;
}

s32 BattlePadCheckRecordTrigger(s32 recordIndex, s32 mask) {
    return BattlePadMultiCheckRecordTrigger(0, recordIndex, mask);
}

s32 BattlePadMultiCheckRecordTrigger(s32 controller, s32 recordIndex, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0xF44 + recordIndex * 4);
    return value & mask;
}

s32 BattlePadCheckNow(s32 mask) {
    return BattlePadMultiCheckNow(0, mask);
}

s32 BattlePadMultiCheckNow(s32 controller, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0x1034);
    return value & mask;
}

s32 BattlePadCheckUp(s32 mask) {
    return BattlePadMultiCheckUp(0, mask);
}

s32 BattlePadMultiCheckUp(s32 controller, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0x10AC);
    return value & mask;
}

s32 BattlePadCheckRepeat(s32 mask) {
    return BattlePadMultiCheckRepeat(0, mask);
}

s32 BattlePadMultiCheckRepeat(s32 controller, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0xFBC);
    return value & mask;
}
