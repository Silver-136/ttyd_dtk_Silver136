#include "battle/battle_break_slot.h"

extern void* _battleWorkPointer;
extern f32 float_490_804268ac;

void BattleBreakSlotDispReel(void);
void dispEntry(s32 cameraId, s32 renderMode, void* callback, s32 param, f32 priority);
void BattleFree(void* ptr);

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

void item_appear_force(void* reel, s32 item) {
    s32 offset;
    s32 i;

    i = 0;
    offset = 0;
    while (i < *(s32*)((s32)reel + 0x18)) {
        if (item == *(s32*)(*(s32*)((s32)reel + 0x14) + offset)) {
            *(s32*)((s32)reel + 0x1C) = i - 1;
        }
        if (*(s32*)((s32)reel + 0x1C) == -1) {
            *(s32*)((s32)reel + 0x1C) = *(s32*)((s32)reel + 0x18) - 1;
        }
        i++;
        offset += 4;
    }
}

void BattleBreakSlot_HideReel(void) {
    void* work;
    void* reel;
    s32 i;

    work = BattleBreakSlotGetPtr();
    reel = BattleBreakSlotReelGetPtr(0);
    for (i = 0; i < 3; i++) {
        *(u32*)((s32)reel + 4) |= 2;
        reel = (void*)((s32)reel + 0x5C);
    }
    *(s32*)((s32)work + 0xC) = 0;
}

void BattleBreakSlot_Start(void) {
    void* work = BattleBreakSlotGetPtr();
    void* reel;
    s32 i;

    if (*(s32*)((s32)work + 0xC) >= 3) {
        *(s32*)((s32)work + 0xC) = 0;
        *(s32*)((s32)work + 4) = 2;
        reel = BattleBreakSlotReelGetPtr(0);
        for (i = 0; i < 3; i++) {
            *(s32*)((s32)reel + 8) = 4;
            *(s32*)((s32)reel + 0xC) = 0;
            reel = (void*)((s32)reel + 0x5C);
        }
    }
}

void BattleBreakSlot_End(void) {
    BattleFree(*(void**)((s32)BattleBreakSlotReelGetPtr(0) + 0x14));
    BattleFree(*(void**)((s32)BattleBreakSlotReelGetPtr(1) + 0x14));
    BattleFree(*(void**)((s32)BattleBreakSlotReelGetPtr(2) + 0x14));
}
