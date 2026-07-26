#pragma once

#include <dolphin/types.h>

void BattlePadInit(void);
void BattlePadManager(void);
void BtlPad_WorkInit(void* work);
void BtlPad_WorkUpdate(void* work, s32 flags);

s32 BattlePadGetTrigger(void);
s32 BattlePadGetNow(void);
s32 BattlePadCheckTrigger(s32 mask);
s32 BattlePadMultiCheckTrigger(s32 controller, s32 mask);
s32 BattlePadCheckRecordTrigger(s32 recordIndex, s32 mask);
s32 BattlePadMultiCheckRecordTrigger(s32 controller, s32 recordIndex, s32 mask);
s32 BattlePadCheckNow(s32 mask);
s32 BattlePadMultiCheckNow(s32 controller, s32 mask);
s32 BattlePadCheckUp(s32 mask);
s32 BattlePadMultiCheckUp(s32 controller, s32 mask);
s32 BattlePadCheckRepeat(s32 mask);
s32 BattlePadMultiCheckRepeat(s32 controller, s32 mask);
