#pragma once

#include <dolphin/types.h>

typedef struct BattleWorkAudience {
    u8 unk0[0x13914 - 0x0]; //0x0
} BattleWorkAudience;
STATIC_ASSERT_SIZEOF(BattleWorkAudience, 0x13914);

void BattleAudience_Init(void);
void BattleAudience_Main(void);
void BattleAudience_End(void);

void BattleAudience_Case_TurnEnd(void);
void BattleAudience_Case_HaikeiSet(void);
s32 BattleAudience_GetPresentItemType(void);

void BattleAudience_Case_AcrobatNoTry(void);
void BattleAudienceJoyEnding(void);
void BattleAudienceAddTargetNum(f32 added, f32 carry);
void BattleAudience_Delete(s32 id);
void BattleAudience_SetAnim(s32 memberIdx, s32 animIdx, s32 restart);
void BattleAudience_SetPosition(s32 id, f32 x, f32 y, f32 z);
void BattleAudience_SetRotate(s32 id, f32 x, f32 y, f32 z);
void BattleAudience_SetRotateOffset(s32 id, f32 x, f32 y, f32 z);
void BattleAudience_ChangeStatus(s32 memberIdx, s32 status);
void* BattleAudienceGetPtr(s32 id);
void* BattleAudienceBaseGetPtr(void);

void BattleAudience_Case_MarioDanger(void);
void BattleAudience_Case_MarioPinch(void);
void BattleAudience_Case_Escape(void);
void BattleAudience_Case_Escape_Good(void);
void BattleAudience_Case_Escape_Bad(void);
s32 BattleAudience_GetAudienceNum(void);
void BattleAudienceSoundClap(s32 vol, s32 frames);
void BattleAudienceSoundCheerKind(s32 kind, s32 vol, s32 frames);
void BattleAudienceSoundWhistleKind(s32 kind);
void BattleAudience_WinSetActive(s32 active);
s32 BattleAudience_GetPresentTargetUnitId(void);
s32 BattleAudience_GetPresentItemNo(void);
void BattleAudience_SetPresentItemNo(s32 itemNo);
void BattleAudienceSoundBombIgnite(s32 kind, s32 start, s32 frames);
s32 BattleAudience_GetAnimEnd(s32 id);
void BattleAudienceSoundItemThrow(s32 kind, s32 memberIdx);
void BattleAudienceNumToTarget(void);
void BattleAudience_Case_JumpNewRecord(void);
s32 BattleAudienceSoundCheck(s32 id);
void BattleAudienceSoundStop(s32 id);
void BattleAudienceSoundCheer(s32 vol, s32 frames);
void BattleAudienceSoundWhistle(void);
void BattleAudienceSoundSetVol(s32 id, s32 vol, s32 frames);
void* BattleAudienceSoundGetPtr(s32 id);
void BattleAudience_ApRecoveryBuild(void* apInfo);

void BattleAudience_ActInit(void);
void BattleAudience_GetPosition(s32 id, f32* x, f32* y, f32* z);
void BattleAudience_SetTarget(s32 memberIdx);
void BattleAudience_Attack(s32 id);
s32 BattleAudience_GetWaiting(s32 id);
s32 BattleAudience_GetAudienceNoFromOffset(s32 id, s32 offset, s32 arg);
s32 BattleAudience_HaitiRandForFallObject(void);
void BattleAudience_Case_ActionCommandBad(void* unit);
void BattleAudience_Case_GuardGood(void);
void BattleAudience_Case_GuardBad(void);
void BattleAudience_Case_AcrobatGood(void);
void BattleAudience_Case_AcrobatBad(void);
void BattleAudience_Case_GreatVictory(void);
void BattleAudience_Case_FastVictory(f32 value);
