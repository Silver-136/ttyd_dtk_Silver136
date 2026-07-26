#pragma once

#include <dolphin/types.h>

void* getKoopaFireEfp(void);
s32 kpaNoHitEnemyCheck(void);
s32 kpaMutekiCheck(void);
void kpaMutekiOn(void);
void kpaMutekiOff(void);
void kpaMSetMutekiTimer(void);
void kpaChgPose(void* pose3d, void* pose2d);
s32 kpaGetLevel(void);
void kpaSetLevel(s32 level);
f32 kpaGetWidth(s32 level);
s32 kpaGetBodyStatus(void);
s32 kpaGetStageType(void);
s32 kpaGetStageViewType(void);
s32 kpaCamFollow(void);
void kpaPreInit(void);
void kpaMain(void);
f32 kpaSetJumpPara(void);
void kpaSetFallPara(void);
void kpa_jumpNpc(void);
s32 kpaFireAttackCheck(void);
u8 kpaAttackStart(void);
void kpaAttackStart2(void);
u8 kpaCancelAttackMode(void);
s32 kpaGetAttackMode(void);
f32 kpaGetBaseScale(s32 level);
s32 kpaHipAttackCheck(void);
s32 kpaJumpChk(void);
