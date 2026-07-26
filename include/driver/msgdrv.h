#pragma once

#include <dolphin/types.h>

u8 msgInit(void);
void msgLoad(char* name, s32 index);
void* msgSearch(void* msgId);
s32 _ismbblead(s32 c);
char* msgGetCommand(char* input, char* command, char* argument);
s32 msgIconStr2ID(char* name);
s32 msgWindow_Entry(char* text, s32 type, s16 kind);
void msgWindow_Add(char* text, s32 id);
void msgWindow_Continue(s32 id);
void msgWindow_Repeat(s32 id);
void msgWindow_ForceClose(s32 id);
