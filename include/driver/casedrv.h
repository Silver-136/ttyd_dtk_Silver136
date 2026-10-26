#pragma once

#include <dolphin/types.h>

// CaseEntry.conditionId: what triggers the case event (caseMain); hit object names compare with Mario's hit
// objects. CaseSetup.conditionId may add 0x8000.
typedef enum CaseCondition {
    CASE_STAND_ON = 0, // standing on the hit object
    CASE_NOT_STAND_ON = 1, // not standing on it
    CASE_UNDER = 2, // below it (hitobjUnder)
    CASE_NOT_UNDER = 3, // not below it
    CASE_JUMP_FROM_A = 4, // A on the first air frame after jumping off it
    CASE_STAND_ON_A = 5, // A while standing on it
    CASE_STAND_ON_STICK = 6, // standing on it with the stick held (wStickDir2 > 30)
    CASE_STAND_ON_STICK_UP = 7, // standing on it, stick pushed up
    CASE_FRONT = 8, // facing it (hitobjFront)
    CASE_FRONT_A = 9, // A while facing it
    CASE_PUSH = 10, // pushing it
    CASE_NOT_PUSH = 11, // not pushing it
    CASE_PUSH_A = 12, // A while pushing it
    CASE_HAMMER = 13, // hit by the hammer
    CASE_HAMMER_2 = 14, // hit by the hammer in MARIO_MOTION_HAMMER_2
    CASE_BOBBERY = 15, // hit by Bobbery's action
    CASE_HEAD = 16, // hit by Mario's head from below
    CASE_FUNC = 17, // the case's activeFunc returns nonzero
    CASE_GSWF = 18, // swGet(swFlag)
    CASE_LSWF = 19, // _swGet(swFlag)
} CaseCondition;

typedef struct CaseSetup {
    u16 conditionId; //0x0, trigger condition (CaseEntry.conditionId list); | 0x8000 = flag (sets CaseEntry flags bit 2)
    u8 pad_2[2]; //0x2
    const char* hitObjName; //0x4
    u32 swFlag; //0x8
    void* activeFunc; //0xC
    s32 lwData[16]; //0x10
    u8 pad_50[0x54 - 0x50]; //0x50, never accessed
    void* evtCode; //0x54
    s32 priority; //0x58
} CaseSetup;

typedef struct CaseEntry {
    u16 flags; //0x0, bit0 = in use, bit1 = triggered this frame (starts the event), bit2 = flag from setup 0x8000
    u8 pad_2[2]; //0x2
    s32 conditionId; //0x4, CaseCondition
    char hitObjName[0x40]; //0x8
    s32 caseId; //0x48
    u32 swFlag; //0x4C
    void* activeFunc; //0x50
    s32 lwData[16]; //0x54
    s32 evtId; //0x94
    void* evtCode; //0x98
    s32 priority; //0x9C
} CaseEntry;

void caseInit(void);
void caseReInit(void);
s32 caseEntry(CaseSetup* setup);
void caseDelete(s32 caseId);
CaseEntry* caseIdToPtr(s32 caseId);
void caseMain(void);
CaseEntry* caseCheckHitObj(void* obj); // Takes HitEntry
