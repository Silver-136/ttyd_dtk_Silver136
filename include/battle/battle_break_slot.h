#pragma once

#include <dolphin/types.h>

typedef struct BattleWorkBreakSlot {
    u8 unk0[0x134 - 0x0]; //0x0
} BattleWorkBreakSlot;
STATIC_ASSERT_SIZEOF(BattleWorkBreakSlot, 0x134);

void BattleBreakSlot_Init(void);
void BattleBreakSlot_Main(void);
void* BattleBreakSlotGetPtr(void);
void* BattleBreakSlotReelGetPtr(s32 index);
void BattleBreakSlot_Disp(void);
void BattleBreakSlot_Start(void);
void BattleBreakSlot_DecBreakTurn(void);
void BattleBreakSlot_PointInc(void);
s32 BattleBreakSlot_CheckReaction(void);
s32 BattleBreakSlot_GetBreakTurn(void);
void BattleBreakSlot_HideReel(void);
void BattleBreakSlot_End(void);
