#pragma once

#include "evt/evt_cmd.h"
#include "party.h"

USER_FUNC(evt_snd_bgm_scope);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmoff_f);
USER_FUNC(evt_snd_bgmon_f);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfx_pit);
USER_FUNC(evt_snd_sfx_pos);
USER_FUNC(evt_snd_sfx_vol);
USER_FUNC(evt_snd_sfxchk);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon);
void party_motion_behind_mario(PartyEntry* party);
void party_motion_behind_mario_continue(PartyEntry* party);
void party_motion_behind_mario_fly(PartyEntry* party);
void party_motion_beside_mario(PartyEntry* party);
void party_motion_damage(PartyEntry* party);
void party_motion_homing_fly(PartyEntry* party);
void party_motion_homing_walk(PartyEntry* party);
void party_motion_jump(PartyEntry* party);
void party_motion_jump2(PartyEntry* party);
void party_motion_stay(PartyEntry* party);
void party_motion_upstairs(PartyEntry* party);
void party_motion_wait(PartyEntry* party);
void partyForceSlitOff(PartyEntry* party);
void partyFollowCloseOff(PartyEntry* party);
void partyFollowCloseOn(PartyEntry* party);
void allPartySlitOffReq(void);
void allPartyForceSlitOff(void);
void allPartySlitOff(void);
void allPartySlitOn(void);
void partyResetTouchWallCt(void);
void partySlitReset(PartyEntry* party);
s32 party_motion_fall(PartyEntry* party);
