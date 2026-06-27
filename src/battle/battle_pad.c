#include "battle/battle_pad.h"
extern void* _battleWorkPointer;

extern s32 BattlePadMultiCheckNow(s32 controller, s32 mask);
extern s32 BattlePadMultiCheckRepeat(s32 controller, s32 mask);
extern s32 BattlePadMultiCheckTrigger(s32 controller, s32 mask);
extern s32 BattlePadMultiCheckUp(s32 controller, s32 mask);
extern s32 BattlePadMultiCheckRecordTrigger(s32 controller, s32 recordIndex, s32 mask);
extern void BtlPad_WorkInit(void* work);

s32 BattlePadGetNow(void) {
    return *(s32*)((s32)_battleWorkPointer + 0x1034);
}

s32 BattlePadGetTrigger(void) {
    return *(s32*)((s32)_battleWorkPointer + 0xF44);
}

s32 BattlePadMultiCheckNow(s32 controller, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0x1034);
    return value & mask;
}

s32 BattlePadMultiCheckRepeat(s32 controller, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0xFBC);
    return value & mask;
}

s32 BattlePadMultiCheckTrigger(s32 controller, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0xF44);
    return value & mask;
}

s32 BattlePadMultiCheckUp(s32 controller, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0x10AC);
    return value & mask;
}

s32 BattlePadMultiCheckRecordTrigger(s32 controller, s32 recordIndex, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0xF44 + recordIndex * 4);
    return value & mask;
}

s32 BattlePadCheckNow(s32 mask) {
    return BattlePadMultiCheckNow(0, mask);
}

s32 BattlePadCheckRepeat(s32 mask) {
    return BattlePadMultiCheckRepeat(0, mask);
}

s32 BattlePadCheckTrigger(s32 mask) {
    return BattlePadMultiCheckTrigger(0, mask);
}

s32 BattlePadCheckUp(s32 mask) {
    return BattlePadMultiCheckUp(0, mask);
}

s32 BattlePadCheckRecordTrigger(s32 recordIndex, s32 mask) {
    return BattlePadMultiCheckRecordTrigger(0, recordIndex, mask);
}

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

void BtlPad_WorkUpdate(void* work, s32 flags) {
    ;
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


void BattlePadManager(void) {
    ;
}
