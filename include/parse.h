#pragma once

#include <dolphin/types.h>

u8 parseInit(s32 param_1, s32 param_2);
u32 parsePush(char* tagName);
void parsePop(void);
void parsePopNext(void);
int parseTagGet1(char* param_1, int param_2, char* param_3, int param_4);
