#include "battle/battle_break_slot.h"

extern void* _battleWorkPointer;
extern f32 float_490_804268ac;

void BattleBreakSlotDispReel(void);
void dispEntry(s32 cameraId, s32 renderMode, void* callback, s32 param, f32 priority);

void* BattleBreakSlotGetPtr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1616C);
}

s32 BattleBreakSlot_GetBreakTurn(void) {
    return *(s32*)((s32)BattleBreakSlotGetPtr() + 0x10);
}

void BattleBreakSlot_DecBreakTurn(void) {
    void* work = BattleBreakSlotGetPtr();

    *(s32*)((s32)work + 0x10) = *(s32*)((s32)work + 0x10) - 1;
    if (*(s32*)((s32)work + 0x10) < 0) {
        *(s32*)((s32)work + 0x10) = 0;
    }
}

void BattleBreakSlot_Disp(void) {
    f32 priority = float_490_804268ac;

    dispEntry(8, 0, BattleBreakSlotDispReel, 0, priority);
}

void* BattleBreakSlotReelGetPtr(s32 index) {
    s32 offset = index * 0x5C;
    void* work = BattleBreakSlotGetPtr();

    return (void*)((s32)work + offset + 0x18);
}

