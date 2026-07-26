#pragma once

#include <dolphin/types.h>

void mobjInit(void);
void mobjReset(s32 value);
u8 mobjHitEntry(void* mobj, int arg);
int mobjEntry(char* name, char* animPoseName);
void mobjDelete(char* name);
void mobjSetPosition(f32 x, f32 y, f32 z, char* name);
void mobjMain(void);
void* mobjNameToPtr(char* name);
void* mobjNameToPtrNoAssert(char* name);
void* mobjHitObjPtrToPtr(void* hitObj);
void* mobjRunEvent(void* mobj, void* evtCode);
void mobjCalcMtx(void* mobj);
void mobjCalcMtx2(void* mobj);
s32 mobjGetHint(void* mobj);
s32 mobjCheckExec(void);
s32 mobjCheckItemboxOpen(void* mobj);
u32* mobjNearDistCheck2(double x1, double z1, double x2, double z2, char** name);
