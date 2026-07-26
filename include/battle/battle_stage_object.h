#pragma once

#include <dolphin/types.h>

void BattleStageObjectMain(void);
void* BattleGetObjectPtr(s32 id);
void* BattleSearchObjectPtr(const char* name);
void BattleStageObjectInit(void);
void BattleStage_DestroyA1(void);
void BattleStage_DestroyA2(void);
void BattleStage_DestroyB(void);
void BattleStage_NozzleDirChangeCheck(void);
void BattleStage_WallCloseCheck(void);
void BattleStage_IronFrameFallCheck(void);
void BattleStage_NozzleWorkCheck(s32 force);
s32 BattleFogEndCheck(void);
s32 BattleFogForceStop(void);
s32 BattleStage_ObjectFallCheck(void);
