#pragma once

#include <dolphin/types.h>
#include "evt/evt_word.h"
#include <dolphin/gx/GXStruct.h>
#include <dolphin/mtx.h>

struct BattleWork;
struct EffectEntry;
struct IconEntry;

typedef struct RankUpData {
    s16 startLevel; //0x0, the level you need to reach to be a certain rank
    s16 rankId; //0x2, the index of this rank for lookups
    const char* rankUpName; //0x4, name of the rank for pause menu
    const char* rankUpMessage; //0x8, text box when you rank up
} RankUpData;

// One of the three level-up choice objects (HP, FP, BP)
typedef struct SeqEndObj {
    Vec pos;                    // 0x0
    Vec scale;                  // 0xC
    Vec rot;                    // 0x18
    u16 value;                  // 0x24
    u16 nextValue;              // 0x26
    u8 paramDisp;               // 0x28
    u8 blinkTimer;              // 0x29, level-up object blink countdown (8 frames)
    u8 pad_2A[0x2C - 0x2A];     // 0x2A
    s32 blinkOn;                // 0x2C, drawn while nonzero
    GXColor color;              // 0x30
} SeqEndObj;

// BattleWork.seqEndWork: the battle result screen (star points, level up), BattleAlloc'd by btlseqEnd
typedef struct BattleSeqEndWork {
    s32 state;                     // 0x0
    s32 timer;                     // 0x4
    s16 exp;                       // 0x8, star points won (doubled by the badge)
    s16 expDisp;                   // 0xA, counts down while exp is added to the pouch
    s32 levelUp;                   // 0xC, set when the pouch exp reaches 100
    s32 selected;                  // 0x10
    s32 unk14;                     // 0x14
    s32 objCount;                  // 0x18
    u8 field_0x1C[0x24 - 0x1C];    // 0x1C
    s32 cursor;                    // 0x24
    s32 unk28;                     // 0x28
    s32 cursorCount;               // 0x2C, number of level-up choices
    u32 flags;                     // 0x30
    struct IconEntry* icons[19];   // 0x34
    s32 alpha[19];                 // 0x80
    s32 counter[19];               // 0xCC
    Vec iconPos[19];               // 0x118
    s32 evtId;                     // 0x1FC, running _jump_event / level-up event
    SeqEndObj objs[3];             // 0x200
    struct EffectEntry* expEffect; // 0x29C, star point counter effect
    s32 whistleTimer;              // 0x2A0
    s32 confettiEvtId;             // 0x2A4
    s32 rankUpEvtId;               // 0x2A8
} BattleSeqEndWork;

extern EvtWord _event_MajinaiEnd[];

extern EvtWord _confetti_control_event[];
extern EvtWord _curtain_close_event[];
extern EvtWord _jump_event[];
extern EvtWord _koura_delete_event[];
extern EvtWord _move_to_badge_point_event[];
extern EvtWord _move_to_flower_event[];
extern EvtWord _move_to_heart_event[];
extern EvtWord _rank_up_event[];
const char* BattleGetRankNameLabel(s32 level);
void btlseqEnd(struct BattleWork* wp);
