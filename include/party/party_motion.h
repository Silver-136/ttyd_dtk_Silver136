#pragma once

#include <dolphin/types.h>

void unk_80151d20(void);
void N_partyFollowCloseOn(void* party);
void N_partyFollowCloseOff(void* party);
u8 party_motion_stay(s32 pParty);
u8 party_motion_homing_walk(s32 pParty);
u8 party_motion_homing_fly(s32 pParty);
void party_set_behind_dir(void* party, f32 dir);
u8 party_motion_behind_mario(s32 pParty);
void party_motion_behind_mario_continue(void* pParty);
void party_motion_behind_mario_fly(void* pParty);
void party_motion_beside_mario(void* pParty);
u8 party_motion_jump(s32 pParty);
void party_motion_jump2(void* pParty);
s32 party_motion_fall(void* pParty);
void party_motion_upstairs(void* party);
void party_motion_damage(void* pParty);
void allPartySlitOn(void);
void allPartySlitOff(void);
void allPartyForceSlitOff(void);
void U_allPartySlitOffReq(void);
void L_partyForceSlitOff(void* party);
void unk_8014c330(void* party);
