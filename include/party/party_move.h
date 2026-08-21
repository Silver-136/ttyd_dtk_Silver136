#pragma once

#include <dolphin/types.h>

u8 partyGetAppearPos(void* party, f32* outPos);
u8 partyGetAppearPos2(void* party, f32* outPos);
u8 partyGetAppearPos3(void* party, f32* outPos);
u8 partyGetAppearPos4(void* party, f32* outPos);
u8 partyGetAppearPos5(void* pParty, void* hitObj, f32* outPos);

void* partyGetFootmarkPtr(s32 id);
s32 partyGetFootmarkId(void* party);
f32 partyGetTargetDistY(s32 useParty);
f32 partyGetTargetMovespd2(void* pParty);
void partyNextReadId(s32 id);
void partyClearFootmark(void);
void partyClearFootmark2(void);
void partyRecordFootmark(void* pParty);
void partyClearJumpPara(void* party);
void unk_800cbfbc(void* party, f32 speed);
void unk_800cbf84(void* party, f32 value);
f32 unk_800cbeb0(void* party);
void N_partyPlaySfxPartyLanding1(void* party);
void unk_800cbc30(void* party);
void unk_800cbb10(void* party, f32 value);
void unk_800cba60(void* party);
s32 unk_800cb9bc(void* hit);
void partyMove(void* party);
void partyMoveNoHosei(void* party);
void partyChgMoveMode(void* party, s32 mode);
void partyChgMot(void* party, s32 mot);
void partySetDir(void* party, f32 x, f32 z);
void partyMoveBehindMario(void* pParty, u8 param_2);
u8 partyMoveBehindMario3(void* pParty, u8 param_2);
void partyMoveBesideMario(void* pParty, u8 param_2);
u8 partyMoveCheckWall(void* pParty);
void partyGetMoveDirSpd(void* pParty, f32* outDir, f32* outSpd);
u8 partyChkWall(void* pParty);
void partyWalkInit(void* pParty);
u8 partyWalkMain(void* pParty);
void party_move_mode_stay(void* party);
void party_move_mode_walk(void* party);
void party_move_mode_behind_mario(void* pParty);
void party_move_mode_beside_mario(void* pParty);
void partyForceWalk(void* pParty);
u8 partyMoveFlyInit(void* pParty, s32 param_2);
void partyFlyMain(void* pParty);
