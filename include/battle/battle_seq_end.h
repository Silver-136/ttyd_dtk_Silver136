#pragma once

#include <dolphin/types.h>
#include "evt/evt_word.h"

typedef struct RankUpData {
    s16 startLevel; //0x0, the level you need to reach to be a certain rank
    s16 rankId; //0x2, the index of this rank for lookups
    const char* rankUpName; //0x4, name of the rank for pause menu
    const char* rankUpMessage; //0x8, text box when you rank up
} RankUpData;

typedef struct BattleSeqEndWork {
    u8 work[0x2AC - 0x0]; //0x0, blob; real layout is SeqEndWork in battle_seq_end.c
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
