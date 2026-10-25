#pragma once

#include <dolphin/types.h>
#include <dolphin/os.h>
#include "evt/evt_cmd.h"

typedef struct lottery_info {
    u16 flags; //0x0
    u8 pad_0x2[0x6]; //0x2
    OSTime lastTime; //0x8
    s16 todayNum; //0x10
    u8 pad_0x12[0x6]; //0x12
    OSTime buyTime; //0x18
    s16 number; //0x20
    s16 days[4]; //0x22
    u8 pad_0x2A[0x6]; //0x2A
    OSTime times[2]; //0x30
} lottery_info;

lottery_info* lotteryGetPtr(void);

extern BOOL dbg_lotteryinfo; // evt_lottery
USER_FUNC(evt_lottery);
USER_FUNC(evt_lottery_buy);
USER_FUNC(evt_lottery_error_check);
USER_FUNC(evt_lottery_flag);
USER_FUNC(evt_lottery_todaynum);
