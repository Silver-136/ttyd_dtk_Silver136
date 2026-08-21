#pragma once

#include <dolphin/types.h>

void itemInit(void);
void itemReInit(void);
void itemCoinDrop(float* position);
s32 N_itemPickUpFromFieldCheck(void);
u8 itemMain(void);
void itemModeChange(void* item, u16 mode);
s32 itemEntry(s32 name, s32 itemId, s32 mode, s32 collectExpr, s32 script, f32 x, f32 y, f32 z);
void itemDelete(void* name);
void* itemNameToPtr(s32 name);
void* itemHitCheck(f64 x, f64 y, f64 z, f64 radius);
void* itemNearDistCheck(float x, float y, float z, float maxDist);
void itemForceGet(void* item);
void itemNokoForceGet(void* item);
void itemFlagOn(void* item, u32 flags);
void itemFlagOff(void* item, u32 flags);
void itemStatusOn(void* item, u32 flags);
u32 itemStatus(void* item, u32 flags);
u8 itemseq_Bound(void* item);
s32 itemHitCheckSide(f32 moveX, f32 moveZ, void* item, f32* outX, f32* outZ, s32* outAngle);
