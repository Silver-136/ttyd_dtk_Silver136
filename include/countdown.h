#pragma once

#include <dolphin/types.h>

void countDownEnd(void);
void countDownFlagOff(u16 flag);
void countDownFlagOn(u16 flag);
s32 countDownGetStatus(void);
void countDownInit(void);
void countDownMain(void);
void countDownSaveReStart(void);
void countDownSetRestartTime(s32 time);
void countDownStart(s32 time, s32 restartTime);
