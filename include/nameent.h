#pragma once

#include <dolphin/types.h>

void nameEntInit(void);
void nameEntReInit(void);
s32 nameEntPrepare(void);
void nameEntOn(int param_1);
void nameEntOff(void);
s32 nameEntWait(void);
s32 nameEntIsCancel(void);
char* nameEntGetName(void);
void nameEntMain(void);
