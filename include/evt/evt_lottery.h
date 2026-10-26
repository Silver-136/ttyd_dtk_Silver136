#pragma once

#include <dolphin/types.h>
#include <dolphin/os.h>
#include "evt/evt_cmd.h"

// lottery_info.flags (evt_lottery, evt_lottery_buy; scripts read them with evt_lottery_flag)
typedef enum LotteryFlags {
    LOTTERY_FLAG_INIT = (1 << 0),         // 0x1, today's number was drawn once
    LOTTERY_FLAG_TICKET = (1 << 1),       // 0x2, a ticket was bought
    LOTTERY_FLAG_CLOCK_ERROR = (1 << 2),  // 0x4, the clock went back past a saved time
    LOTTERY_FLAG_REBOUGHT = (1 << 3),     // 0x8, bought again after an error; cleared on the next day
    LOTTERY_FLAG_NEW_DAY = (1 << 4),      // 0x10, a new day since the last check
    LOTTERY_FLAG_MATCH4 = (1 << 12),      // 0x1000, today's number matches all four digits
    LOTTERY_FLAG_MATCH3 = (1 << 13),      // 0x2000, the last three digits
    LOTTERY_FLAG_MATCH2 = (1 << 14),      // 0x4000, the last two digits
    LOTTERY_FLAG_MATCH1 = (1 << 15),      // 0x8000, the last digit
} LotteryFlags;

typedef struct lottery_info {
    u16 flags; //0x0, LotteryFlags
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
