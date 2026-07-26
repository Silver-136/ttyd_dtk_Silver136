#pragma once

#include <dolphin/types.h>

void hitInit(void);
void hitReInit(void);
void* hitEntry(void* mapObj, void* arg, s32 idx);
void hitEntryMOBJ(void* mapObj, void* arg);
void hitDelete(char* name);
void hitMain(void);
void hitObjFlagOn(char* name, u16 flag);
void hitObjFlagOff(char* name, u16 flag);
void hitGrpFlagOn(char* name, u16 value);
void hitGrpFlagOff(char* name, u16 value);
void hitObjAttrOn(char* name, u32 attr);
void hitObjAttrOff(char* name, u32 attr);
void hitGrpAttrOn(char* name, u32 value);
void hitGrpAttrOff(char* name, u32 value);
void hitReCalcMatrix(void* hit, void* arg);
s32 hitCheckVecFilter(s32* work, void* filter);
u8 hitCheckVecHitObjXZ(s32* ray, void* hitObj);
void* hitCheckSphereFilter(void* filter, f32 x, f32 y, f32 z, f32 radius);
void* hitNameToPtr(char* hitName);
void hitObjGetPos(char* name, f32* out);
void hitObjGetNormal(char* name, void* out);
s32 hitGetName(void* hit);
u32 hitGetAttr(void* hit);
void hitGrpDamageReturnSet(char* name, s32 value);
void* hitGetDamageReturnPos(void* hit);
void hitBindMapObj(char* hitName, char* mapObjName);
void hitBindUpdate(char* name);
