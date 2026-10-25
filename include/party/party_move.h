#pragma once

#include "party.h"
#include "dolphin/types.h"

void partyChgMot(PartyEntry* party, s32 mot);
void partyClearFootmark(void);
void partyClearJumpPara(PartyEntry* party);
void partyGetMoveDirSpd(PartyEntry* party, f32* dir, f32* spd);
void partyMove(PartyEntry* party);
void partyMoveNoHosei(PartyEntry* party);
void partyMoveWalk(PartyEntry* party);
void partyRecordFootmark(PartyEntry* party);
void partyWalkMain(PartyEntry* party);
void party_move_mode_behind_mario(PartyEntry* party);
void party_move_mode_beside_mario(PartyEntry* party);
void party_move_mode_stay(PartyEntry* party);
void party_move_mode_walk(PartyEntry* party);

void walkMain(PartyEntry* party);
void N_partyPlaySfxPartyLanding1(PartyEntry* party);
void partyChgMoveMode(PartyEntry* party, s32 mode);
void partyChgStayPose(PartyEntry* party);
void partyChgWalkPose(PartyEntry* party, f32 spd);
void partyChkWall(PartyEntry* party);
void partyClearFootmark2(void);
void partyFlyMain(PartyEntry* party);
void partyForceWalk(PartyEntry* party);
s32 partyGetFootmarkReadId(s32 id);
s32 partyGetFootmarkWriteId(s32 id);
void partyGetReadId(s32 id, f32* x, f32* y, f32* z);
f32 partyGetTargetDir(s32 id, f32 x, f32 z);
f32 partyGetTargetDist(s32 id, f32 x, f32 z);
f32 partyGetTargetDistY(s32 id, f32 y);
f32 partyGetTargetMovespd(s32 id);
f32 partyGetTargetMovespd2(PartyEntry* party);
void partyMoveCheckWall(PartyEntry* party);
void partyMoveFlyInit(PartyEntry* party, s32 arg);
void partyNextReadId(s32 id);
void partySetDir(PartyEntry* party, f32 x, f32 z);
void partySetDispDirToMario(PartyEntry* party);
void party_move_mode_wait(PartyEntry* party);
f32 partyForceSetSpd(PartyEntry* party, f32 spd);
void partyMoveBehindMario(PartyEntry* party, s32 arg);
void partyMoveBesideMario(PartyEntry* party, s32 arg);
f32 partySetTargetSpd(PartyEntry* party, f32 spd);
