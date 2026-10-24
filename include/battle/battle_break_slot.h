#pragma once

#include "dolphin/types.h"
#include <dolphin/mtx.h>
#include "evt/evt_word.h"

typedef struct BreakSlotReel {
	s32 kind;        // 0x00
	u32 flags;       // 0x04
	s32 state;       // 0x08
	s32 counter;     // 0x0C
	s32 item;        // 0x10
	s32* cards;      // 0x14
	s32 numCards;    // 0x18
	s32 index;       // 0x1C
	Vec pos;         // 0x20
	Vec rot;         // 0x2C
	Vec scale;       // 0x38
	Vec pivot;       // 0x44
	f32 spinSpeed;    // 0x50, reel scroll per frame (eases to 22.5, decelerates by spinAccel to 20)
	f32 spinAccel;    // 0x54, added to spinSpeed after the stop press (negative)
	u8 alpha;        // 0x58
	u8 pad_59[3];    // 0x59
} BreakSlotReel;

typedef struct BreakSlotWork {
	u32 flags;                // 0x000
	s32 phase;                // 0x004
	s32 phaseTimer;           // 0x008, frame counter of the current phase
	s32 reelNo;               // 0x00C
	s32 breakTurn;            // 0x010
	f32 spMultiplier;         // 0x014, star power gain multiplier from the slot result (2.0/3.0; BattleAudience_ApRecoveryBuild)
	BreakSlotReel reels[3];   // 0x018
	s32 startSfxId;           // 0x12C, SFX_BTL_SLOT_START1 handle, stopped on the first reel
	struct EventEntry* evt;           // 0x130
} BreakSlotWork;


void BattleBreakSlot_Init(void);
void BattleBreakSlot_Main(void);
void BattleBreakSlot_End(void);




void BattleBreakSlot_DecBreakTurn(void);
BreakSlotWork* BattleBreakSlotGetPtr(void);

extern EvtWord doku_kinoko_effect[];
extern EvtWord flower_effect[];
extern EvtWord kinoko_effect[];
extern EvtWord msg_fault[];
extern EvtWord msg_fp_max[];
extern EvtWord msg_hp_max[];
extern EvtWord msg_star_max[];
extern EvtWord msg_super_max[];
extern EvtWord shine_effect[];
extern EvtWord star_effect[];

void BattleBreakSlot_HideReel(void);
