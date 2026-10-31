// Minigame result and ranking window of the Pianta Parlor, carried as evt_yuuranking.c by yuu and gor.
#pragma once

#include "evt/evt_cmd.h"

#include "manager/evtmgr.h"

typedef struct YuuRankingWork {
    EventEntry* event; // 0x00
    u16 flags;         // 0x04
    u32 dispFlags;     // 0x08
    f32 x;             // 0x0C
    f32 y;             // 0x10
    s32 type;          // 0x14, minigame
    s32 mode;          // 0x18
    s32 score;         // 0x1C
    s32 bonus;         // 0x20
    s32 mul;           // 0x24, bonus multiplier
    s32 rank;          // 0x28
    s32 max;           // 0x2C
    s32 phase;         // 0x30
    s32 monte;         // 0x34, Piantas (tokens) won
    s32 dispScore;     // 0x38, score shown while counting up
    s32 timer;         // 0x3C
} YuuRankingWork;

USER_FUNC(evt_yuuranking_alloc);
USER_FUNC(evt_yuuranking_free);
USER_FUNC(yuuranking_wait_button);
