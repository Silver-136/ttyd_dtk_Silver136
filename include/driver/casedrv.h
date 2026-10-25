#pragma once

#include <dolphin/types.h>

typedef struct CaseSetup {
    u16 conditionId; //0x0, trigger condition (CaseEntry.conditionId list); | 0x8000 = flag (sets CaseEntry flags bit 2)
    u8 pad_2[2]; //0x2
    const char* hitObjName; //0x4
    u32 swFlag; //0x8
    void* activeFunc; //0xC
    s32 lwData[16]; //0x10
    u8 pad_50[0x54 - 0x50]; //0x50, never accessed
    void* evtCode; //0x54, TODO re-type
    s32 priority; //0x58
} CaseSetup;

typedef struct CaseEntry {
    u16 flags; //0x0, bit0 = in use, bit1 = triggered this frame (starts the event), bit2 = flag from setup 0x8000
    u8 pad_2[2]; //0x2
    s32 conditionId; //0x4, trigger: 0 standOn name, 1 not standOn, 2 under, 3 not under, 4 jump from+A, 5 standOn+A, 6 stick up, 7 dir trigger, 8 front, 9 front+A, 10 push, 11 not push, 12 push+A, 13 hitobj 0x1FC, 14 hitobj 0x1FC in motion 0x13, 15 partner(3) hit, 16 head, 17 activeFunc(), 18 swGet, 19 _swGet
    char hitObjName[0x40]; //0x8
    s32 caseId; //0x48
    u32 swFlag; //0x4C
    void* activeFunc; //0x50
    s32 lwData[16]; //0x54
    s32 evtId; //0x94
    void* evtCode; //0x98, TODO re-type
    s32 priority; //0x9C
} CaseEntry;

void caseInit(void);
void caseReInit(void);
s32 caseEntry(CaseSetup* setup);
void caseDelete(s32 caseId);
CaseEntry* caseIdToPtr(s32 caseId);
void caseMain(void);
CaseEntry* caseCheckHitObj(void* obj); // Takes HitEntry
