#pragma once

#include <dolphin/types.h>

typedef struct NPCWork {
    u8 unk0[0x14 - 0x0]; //0x0
} NPCWork;

void npcInit(void);
void npcReset(s32 value);
void npcMain(void);
s32 npcGetReactionOfLivingBody(s32 battle);
void npcDelete(void* npc);
void npcDeleteGroup(void* npc);
void npcSetMarioAutoTalkPose(const char* stay, const char* talk);
void npcTuningRy(void* npc, f32 angle);
f32 npcTransRytoFaceDir(void* npc);
void* npcNearDistCheck(f32 x, f32 y, f32 z, f32 radius);
void npcGroupDead(void* npc, s32 scoreType);
void npcSetBattleInfo(void* npc, s32 info);
void npcStopForEvent(void);
void npcStopForOneEvent(void* npc);
void npcStartForEvent(void);
void npcStartForOneEvent(void* npc);
void npcSetColor(char* name, void* color);
void npcSetSlave(void* master, void* slave, s32 index);
void npcSetLink(void* a, void* b);
void npcExecAllInitEvt(void);
s32 npcWaitAllInitEvtEnd(void);
void npcExecAllReglEvt(void);
void npcGetBackItemEntry(void* npc);
void npcClearDeadInfo(void);
void npcKoopaModeEncountCheck(void);
s32 npcKoopaModeMobjBoundDeadCheck(void* hit);
s32 npcCalcScore(void* npc);
void fbatHitCheckAll(void);
void* fbatHitCheck(u32 flags, void* hitInfo);
s32 _npcHitCheckSphere(f64 x, f64 y, f64 z, f64 radius, s32 npc, f32* outDist);
s32 _npcHitCheckHammerAllMotion(f64 radius, f64 angle, s32 npc, f32* outDist);
