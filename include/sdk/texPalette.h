#pragma once

#include <dolphin/types.h>

void* UnpackTexPalette(void* data);
void* TEXGet(void* tpl, s32 id);
void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 id);
