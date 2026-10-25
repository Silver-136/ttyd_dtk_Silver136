#pragma once

#include <dolphin/types.h>

typedef struct ParseWork {
    char* ptr; //0x0
    s32 pos[10]; //0x4
    s32 end[10]; //0x2C
    s32 depth; //0x54
} ParseWork;

void parseInit(void* ptr, u32 size);
BOOL parsePush(const char* tag);
void parsePop(void);
void parsePopNext(void);
BOOL parseGet1Next(s32 type, void* data);
BOOL parseTagGet1(const char* tag, s32 type, void* data);
