#pragma once

#include <dolphin/types.h>

typedef struct CountdownWork {
    u16 flags; //0x0
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
