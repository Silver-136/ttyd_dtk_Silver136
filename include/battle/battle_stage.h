#pragma once

#include <dolphin/types.h>

void* BattleStageGetPtr(void);
void BattleStageInit(void);
void BattleStageMain(void);
void BattleStageEnd(void);
void BattleStageDisp(void);

void BattleStageOnLightInCommand(void);
void BattleStageOffLightInCommand(void);
void BattleStageOnLightFriendInCommand(void);
void BattleStageOffLightFriendInCommand(void);
void BattleStageOnLightEnemyInCommand(void);
void BattleStageOffLightEnemyInCommand(void);

s32 BattleStageGetLightNumberByName(const char* name);
s32 BattleStageSysLightEntry(char* name);
void BattleStageLightRelease(s32 lightId, s32 fadeFrames, s32 unk);
void BattleStageLightSetLightColor(s32 lightId, u8 r, u8 g, u8 b, s32 fadeFrames, s32 unk);
void BattleStageLightSetWidth(s32 index, s32 width, s32 frames, s32 type);
void BattleStageLightSetPosition(s32 lightId, f32 x, f32 y, f32 z, s32 unk1, s32 unk2);
void BattleStageLightSetTarget(s32 lightId, f32 x, f32 y, f32 z, s32 unk1, s32 unk2);
void BattleStageLightSetTargetByUnit(
    s32 lightId, f32 x, f32 y, f32 z, s32 unitId, s32 fadeFrames, s32 unk);
void BattleStageLightSetUpVec(s32 lightId, f32 x, f32 y, f32 z, s32 unk1, s32 unk2);

void BattleStageSetStgDark(s32 value, s32 frames, s32 type);
void BattleStageSetStgDarkBase(s32 value, s32 frames, s32 type);
void BattleStageReturnStgDarkBase(s32 frames, s32 type);
void BattleStageSetAudDark(s32 value, s32 frames, s32 type);
void BattleStageSetAudDarkBase(s32 value, s32 frames, s32 type);
void BattleStageReturnAudDarkBase(s32 frames, s32 type);

void* BatSpotGetPtr(void);
