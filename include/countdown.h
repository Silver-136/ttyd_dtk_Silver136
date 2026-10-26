#pragma once

#include <dolphin/types.h>

// CountdownWork.flags (countDownFlagOn/Off, evt_sub_countdown_flag_onoff)
typedef enum CountdownFlags {
    COUNTDOWN_FLAG_RUNNING = (1 << 0),  // 0x1
    COUNTDOWN_FLAG_TIME_UP = (1 << 1),  // 0x2
    COUNTDOWN_FLAG_HIDDEN = (1 << 2),   // 0x4, not drawn
    COUNTDOWN_FLAG_STOPPED = (1 << 3),  // 0x8, time does not advance (countDownStop)
} CountdownFlags;

typedef struct CountdownWork {
    u16 flags; //0x0, CountdownFlags
    u8 pad_2[0x8 - 0x2]; //0x2
    s64 elapsed; //0x8
    s64 prevTime; //0x10
    u32 limit; //0x18
    u32 restart; //0x1c
    f32 x; //0x20
    f32 y; //0x24
    f32 offX; //0x28
    f32 offY; //0x2c
    s32 state; //0x30
    s32 counter; //0x34
    s32 sfxId; //0x38
    u8 pad_3C[0x40 - 0x3c]; //0x3c
} CountdownWork;

void countDownInit(void);
void countDownSaveReStart(void);



void countDownMain(void);
void countDownStart(u32 limit, u32 restart);
void countDownSetRestartTime(u32 restart);
void countDownStop(void);
void countDownEnd(void);
void countDownFlagOn(u16 flag);
void countDownFlagOff(u16 flag);
s32 countDownGetStatus(void);
