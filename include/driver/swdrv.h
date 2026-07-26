#pragma once

#include <dolphin/types.h>

void swInit(void);
void swReInit(void);
void swSet(s32 index);
s32 swGet(s32 index);
void swClear(s32 index);
void swByteSet(s32 index, s32 value);
s32 swByteGet(s32 index);

void _swSet(s32 index);
s32 _swGet(s32 index);
void _swClear(s32 index);
void _swByteSet(s32 index, u8 value);
u8 _swByteGet(s32 index);
