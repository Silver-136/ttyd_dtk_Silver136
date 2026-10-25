#pragma once

#include <dolphin/types.h>

struct WindowWork;

const char* winZenkakuStr(s32 n);
char* winZenkakuStrPad(s32 n, s32 digits);
void winMarioMain2(void);
void winMarioDisp(s32 camId, void* param, s32 idx);
void winMarioExit(struct WindowWork* w);
void winMarioInit(struct WindowWork* w);
void winMarioInit2(struct WindowWork* w);
s32 winMarioMain(struct WindowWork* w);
