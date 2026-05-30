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
    s32 i;
    s32 offset;
    void* battleWork;

    battleWork = _battleWorkPointer;
    offset = 0;

    for (i = 0; i < 4; i++) {
        BtlPad_WorkInit((void*)((s32)battleWork + 0xF2C + offset));
        offset += 0x1FC;
    }
}


void BtlPad_WorkUpdate(void* work, s32 flags) {
}


void BtlPad_WorkInit(void* work) {
}


void BattlePadManager(void) {
}
