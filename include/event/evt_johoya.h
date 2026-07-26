#pragma once

#include <dolphin/types.h>

void johoya_init(void);
s32 johoya_set(void* flags, s32 value, s32 set);
s32 johoya_get(void* flags, s16 value);
int search_evt_no(char* label);
