#pragma once

#include "battle/battle_audience.h"
#include "battle/battle_break_slot.h"
#include "battle/battle_camera.h"
#include "battle/battle_information.h"
#include "battle/battle_seq_command.h"
#include "battle/battle_unit.h"
#include "mario/mario_pouch.h"

#include <dolphin/mtx.h>

typedef enum BattleSequence {
    SEQ_UNKNOWN = 0,
    SEQ_INIT = 1,
    SEQ_FIRST_ACT = 2,
    SEQ_TURN = 3,
    SEQ_PHASE = 4,
    SEQ_MOVE = 5,
    SEQ_ACT = 6,
    SEQ_END = 7
} BattleSequence;

typedef struct BattleWorkPartyInfo {
    f32 sizeMultiplier; //0x0
    BattleWorkStatus statusEffects; //0x4
    u8 unk22[0x24 - 0x22]; //0x22
    s32 statusFlags; //0x24
    u16 unk28; //0x28
    u16 unk2A; //0x2A
} BattleWorkPartyInfo;
STATIC_ASSERT_SIZEOF(BattleWorkPartyInfo, 0x2C);

typedef struct EndSequenceWork {
    u8 unk0[0x2AC - 0x0]; //0x0
} EndSequenceWork;
STATIC_ASSERT_SIZEOF(EndSequenceWork, 0x2AC);

typedef struct BattleUnitSetup {
    BattleUnitData* data;  //0x0
    s8 alliance;           //0x4
    u8 unk5[0x8 - 0x5];    //0x5
    s32 attackPhase;       //0x8
    Vec position;          //0xC
    u8 unk18[0x30 - 0x18]; //0x18
} BattleUnitSetup;
STATIC_ASSERT_SIZEOF(BattleUnitSetup, 0x30);

typedef struct BattleWork {
    s16 turnCount;                      //0x0
    u8 unk2[0x8 - 0x2];                 //0x2
    BattleWorkAlliance allianceInfo[3]; //0x8
    BattleWorkUnit* units[64];          //0x20
    s32 movePriorityQueue[64];          //0x120
    u8 unk220[0x424 - 0x220];           //0x220
    s32 lectureUnitId;                  //0x424
    u8 unk428[0xEF4 - 0x428];           //0x428
    u32 flags;                          //0xEF4
    u8 unkEF8[0xF28 - 0xEF8];           //0xEF8
    EndSequenceWork* endSequenceWork;   //0xF28
    u8 unkF2C[0x171C - 0xF2C];          //0xF2C
    BattleWorkCommand commandMenu;      //0x171C
    u8 unk1C90[0x2738 - 0x1C90];        //0x1C90
    FieldBattleInfo* info;              //0x2738, TODO: rename if other info?
    u8 unk273C[0x2754 - 0x273C];        //0x273C
    BattleWorkCamera camera;            //0x2754
    BattleWorkAudience audience;        //0x2858
    BattleWorkBreakSlot bingoSlots;     //0x1616C, Match 3 BINGO! data
    BattleWorkPartyInfo partyInfo[7];   //0x162A0
    u8 unk163D4[0x19060 - 0x163D4];     //0x163D4
    ItemType reserveItems[4];           //0x19060
    s32 curtainSFXId;                   //0x19070, used in btlseqFirstAct for curtain sfx
    u8 unk19074[0x19098 - 0x19074];     //0x19074
} BattleWork;
STATIC_ASSERT_SIZEOF(BattleWork, 0x19098);

extern BattleWork* _battleWorkPointer;

void battleMain(void);
void BattleInit(FieldBattleInfo* info);
BOOL BattleMain(void);
void BattleEnd(void);
BOOL Btl_UnitSetup(BattleWork* wp);

void BattleSetSeq(BattleWork* wp, BattleSequence seq, s32 value);
s32 BattleGetSeq(BattleWork* wp, BattleSequence seq);

void* BattleAlloc(u32 size);
void BattleFree(void* ptr);
void* BattleGetPartyPtr(BattleWork* wp);
BattleWorkUnit* BattleGetUnitPtr(BattleWork* wp, s32 index);
void BattleSetUnitPtr(BattleWork* wp, s32 index, BattleWorkUnit* unit);

void BattleIncSeq(void* battleWork, s32 seqType);
void BattleStoreCoin(BattleWork* wp, s32 coin);
void BattleStoreExp(BattleWork* wp, s32 exp);
void BattleAfterReactionEntry(s32 unitId, s32 arg);
s32 BattleAfterReactionMain(void);
void BattleMajinaiDone(void);
void BattleMajinaiEndCheck(void);
void BattleStatusWindowAPRecoveryOn(void);
void BattleStatusWindowAPRecoveryOff(void);
void BattleStatusWindowEventOn(void);
void BattleStatusWindowSystemOn(void);
void BattleStatusWindowSystemOff(void);
void BattleConsumeReserveItem(void);
s32 BattleTransPartyIdToUnitKind(s32 partyId);
