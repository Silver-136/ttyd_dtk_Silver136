#pragma once

#include <dolphin/types.h>

u8 caseInit(void);
void caseReInit(void);
s32 caseEntry(void* param);
void caseDelete(s32 id);
void caseMain(void);
void* caseCheckHitObj(void* hit);
void* caseCheckHitObj2(u32 hit);
void* caseIdToPtr(s32 id);
