#pragma once

#include <dolphin/types.h>

char* romFontGetMessage(s32 messageId);
void romFontInit(void);
void romFontPrintGX(f32 x, f32 y, f32 scale, u32* color, const char* format, ...);
int romFontGetWidth(s32 message, s32 entry);
